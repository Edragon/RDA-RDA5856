/* 
 * Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef MCU_WIFI_PLAT
#include "cs_types.h"

#include "flash_prog_map.h"

#include "tgt_memd_cfg.h"
#include "memd_m.h"
#include "pmd_m.h"
#include "boot_host.h"
#include "boot_usb_monitor.h"

#include "hal_sys.h"
#include "hal_debug.h"
#include "hal_dma.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_map_engine.h"
#include "hal_gpio.h"

// define the folowing to see the error code returned by the flash driver as host events
#define DEBUG_EVENTS
#ifdef DEBUG_EVENTS
#include "hal_host.h"
#endif
//#include "event.h"

extern VOID mon_Event(UINT32 ch);
extern VOID boot_HostUsbEvent(UINT32 ch);
extern BOOL fp_CheckFlashScratch(VOID);
extern INT32 fp_FlashLifeTest(UINT32 *cycle_count, UINT32 **error_address);

extern BOOT_HST_EXECUTOR_T g_bootHstExecutorCtx;

#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
  #define SEND_EVENT(x) boot_HostUsbEvent((UINT32)x)
#else
  #define SEND_EVENT(x) mon_Event((UINT32)x)
#endif

//#define SEND_EVENT(x)  hal_HstSendEvent(0xff,(UINT32)x)   //mon_Event(x)
 
#ifndef FLASH_USB_SEND_EVENT_DELAY
#define FLASH_USB_SEND_EVENT_DELAY    8192 // 0.5s
#endif /* FLASH_USB_SEND_EVENT_DELAY */

#define FP_DATA_BUFFER_LOCATION __attribute__((section(".ramucbss")))

BOOT_HOST_USB_MONITOR_CTX_T g_flashProgUsbContext;

//-----------------------------
// Three Data Buffers are available for CoolWatcher
//-----------------------------
#if (FPC_BUFFER_SIZE/4*4 != FPC_BUFFER_SIZE)
#error "FPC_BUFFER_SIZE should be aligned with word boundary"
#endif

PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferA[FPC_BUFFER_SIZE/4];
PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferB[FPC_BUFFER_SIZE/4];
PROTECTED UINT32 FP_DATA_BUFFER_LOCATION g_dataBufferC[FPC_BUFFER_SIZE/4];

EXPORT PUBLIC UINT32 _boot_loader_magic_number_in_flash;
PUBLIC UINT32 flash_test_count = 0;
 
PROTECTED HAL_MAP_VERSION_T g_flashProgVersion = {0, 0x20080121, 0, "Flash Programmer Version 1"};
PUBLIC FLASH_PROG_MAP_ACCESS_T g_flashProgDescr =
    { {FPC_PROTOCOL_MAJOR,FPC_PROTOCOL_MINOR},{{FPC_NONE, (UINT32)0, 0, 0} ,{FPC_NONE, (UINT32)0, 0, 0}},
        (UINT8*)g_dataBufferA, (UINT8*)g_dataBufferB, (UINT8*)g_dataBufferC, FPC_BUFFER_SIZE};

#if !(CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
    #error error: You can t use FASTPF_USE_USB without CHIP_HAS_USB
#endif

// =============================================================================
// FLASH_PROGRAMMER_SPECIAL_SECTOR_T
// -----------------------------------------------------------------------------
/// Describe the special sector (use a magic number the first word)
// =============================================================================
typedef struct
{
    UINT8* address;
    UINT32 magicNumber;
} FLASH_PROGRAMMER_SPECIAL_SECTOR_T;

// Check whether flash programmer data buffer is accessible
PROTECTED BOOL fp_CheckDataBuffer(VOID)
{
    int i, j;
    static CONST UINT32 pattern[] = { 0xa5a5a5a5, 0x5a5a5a5a, 0x4f35b7da, 0x8e354c91, };
    static VOLATILE UINT32* CONST dataAry[] = {
        (VOLATILE UINT32 *)&g_dataBufferA[0],
        (VOLATILE UINT32 *)&g_dataBufferA[FPC_BUFFER_SIZE/4 - 1],
        (VOLATILE UINT32 *)&g_dataBufferB[0],
        (VOLATILE UINT32 *)&g_dataBufferB[FPC_BUFFER_SIZE/4 - 1],
        (VOLATILE UINT32*)&g_dataBufferC[0],
        (VOLATILE UINT32*)&g_dataBufferC[FPC_BUFFER_SIZE/4 - 1],
    };

    for (i=0; i<sizeof(dataAry)/sizeof(dataAry[0]); i++)
    {
        for (j=0; j<sizeof(pattern)/sizeof(pattern[0]); j++)
        {
            *(dataAry[i]) = pattern[j];
            if (*(dataAry[i]) != pattern[j])
            {
                return FALSE;
            }
        }
    }

#if (RAM_SIZE!=0)
#ifndef NO_PSRAM_HIGH_ADDR_CHECK
    typedef struct
    {
        UINT32 startAddr;
        UINT32 len;
    } PSRAM_RANGE_T;

    UINT32 psramBase = (UINT32)hal_EbcGetCsAddress(HAL_EBC_SRAM);
    PSRAM_RANGE_T range[] = {
        { psramBase+0x3c0000, 0x100, },
        { psramBase+0x3fff00, 0x100, },
    };

    UINT32 psramAddr;
    for (i=0; i<sizeof(range)/sizeof(range[0]); i++)
    {
        
        for (psramAddr=range[i].startAddr;
             psramAddr+4<=range[i].startAddr+range[i].len;
             psramAddr+=4)
        {
            for (j=0; j<sizeof(pattern)/sizeof(pattern[0]); j++)
            {
                *(VOLATILE UINT32*)psramAddr = pattern[j];
                if (*(VOLATILE UINT32*)psramAddr != pattern[j])
                {
                    SEND_EVENT(psramAddr);
                    return FALSE;
                }
            }
        }
    }
#endif
#endif
    return TRUE;
}

// This xcpu_main replaces the regular xcpu_main present in the platform library
#ifdef CHIP_CPU_IS_MIPS
PROTECTED int main(VOID)
#else
PROTECTED int user_main(VOID)
#endif
{
    VOLATILE FPC_COMMAND_DATA_T*         cmds =
        (FPC_COMMAND_DATA_T*) HAL_SYS_GET_UNCACHED_ADDR((UINT32)g_flashProgDescr.commandDescr);
    INT32                                i = 0;
    UINT32                               cpt;
    UINT32                               cpt2;
    UINT32                               wrong_flash_addr = 0;
    INT32                                errorDuringCommand;
    INT32                                status = MEMD_ERR_NO;
    FPC_COMMAND_TYPE_T                   c;
    UINT32                               s;
    UINT32                               sectorWrite = 0;
    HAL_DMA_FCS_CFG_T                    fcsCtx;
    UINT8                                fcs[3];
    UINT32*                              verifySectorSize;
    UINT32*                              verifySectorAddress;
    UINT32*                              verifyAwaitedFcs;  
    UINT32                               verifyFirstBadSector;
    UINT32                               m_CalibRfCrcAdd = 0;
    UINT32                               userBase, userSize, userSector, userBlock;
    int test = 0;
    BOOT_HST_EXECUTOR_T*    pBootCtx;
    FLASH_PROGRAMMER_SPECIAL_SECTOR_T    flashSpecialSector[] = {
        { .address = 0x00000000,
            .magicNumber = 0xD9EF0045 },
        { .address = 0x00000000,
            .magicNumber = 0x00000000 },
    };

      
    hal_SysRequestFreq(HAL_SYS_FREQ_APP_USER_0, HAL_SYS_FREQ_104M, NULL);

    cmds[0].cmd = FPC_NONE;
    cmds[1].cmd = FPC_NONE;

#ifdef CHIP_CPU_IS_MIPS
//  enable exception (gdb breaks) but not external it, keep handler in rom
    asm volatile("li    $27,    (0x00400000 | 0x0001 | 0xc000)\n\t"
                 "mtc0  $0, $13\n\t"
                 "nop\n\t"
                 "mtc0  $27, $12\n\t"
//  k1 is status for os irq scheme
                 "li    $27, 1");
#endif

#if CHIP_ASIC_ID==CHIP_ASIC_ID_5857
   *(UINT32 *)(0x40140000+0x18) |= 0x110;
#endif  

    MEMD_FLASH_LAYOUT_T *flash_layout = memd_FlashOpen(tgt_GetMemdFlashConfig());
    //memd_RamOpen(tgt_GetMemdRamConfig());
    memd_FlashGetUserFieldInfo(&userBase, &userSize, &userSector, &userBlock);

    if (!fp_CheckDataBuffer())
    {
        SEND_EVENT(EVENT_FLASH_PROG_MEM_RESET);
        hal_SysResetOut(TRUE);
        pmd_EnableMemoryPower(FALSE);
        hal_TimDelay(2 * HAL_TICK1S);
        pmd_EnableMemoryPower(TRUE);
        hal_TimDelay(10);
        hal_SysResetOut(FALSE);
        hal_TimDelay(10);

        memd_FlashOpen(tgt_GetMemdFlashConfig());
        memd_RamOpen(tgt_GetMemdRamConfig());

        if (!fp_CheckDataBuffer())
        {
            SEND_EVENT(EVENT_FLASH_PROG_MEM_ERROR);
            while(1);
        }
    }

#if 0
    fp_CheckFlashScratch();
    while(1);
#endif

#if 0
    status = fp_FlashLifeTest(&flash_test_count, &verifySectorAddress);
    while(1){
        SEND_EVENT(0xef000000 + flash_test_count);
        SEND_EVENT(status);
        SEND_EVENT(verifySectorAddress);
        //SEND_EVENT(cmds[i].size);
        hal_TimDelay(16384);
    };
#endif
    
    //pBootCtx = HAL_SYS_GET_UNCACHED_ADDR(&g_bootHstExecutorCtx);
    // Record the communication structure into HAL Mapping Engine
    hal_MapEngineRegisterModule(HAL_MAP_ID_FLASH_PROG, &g_flashProgVersion, &g_flashProgDescr);
    // reuse g_bootHstExecutorCtx to save the address of flash program info
    //pBootCtx->hstMonitorXCtx.cmdType = (UINT32)HAL_SYS_GET_UNCACHED_ADDR(&g_flashProgVersion);
    //pBootCtx->hstMonitorXCtx.pc = (UINT32)HAL_SYS_GET_UNCACHED_ADDR(&g_flashProgDescr);
#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
	boot_UsbResetTransfert();
    g_flashProgUsbContext.MonitorMode = BOOT_HOST_USB_MODE_BOOT;
    boot_HostUsbOpen(&g_flashProgUsbContext, 0,0);
	//mon_Event(0x1509250e);
#endif
    
    //boot_HostUsbEvent(EVENT_FLASH_PROG_READY);
    SEND_EVENT(EVENT_FLASH_PROG_READY);
    //SEND_EVENT(pmd_GetEncryptUid());
    while (1)
    {
#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB == 1)
        boot_HostUsbRecv();
        boot_HostUsbSend();
#endif
        test ++;
        c = cmds[i].cmd;

	    //SEND_EVENT(c);
        switch (c)
        {
        case FPC_NONE:
        case FPC_DONE:
        case FPC_ERROR:
        case FPC_FCS_ERROR:
        case FPC_FLASH_NOT_AT_FF:
            break;

        case FPC_PROGRAM:
            errorDuringCommand = 0;

#if 0
            if((test%0x10)==0xf || cmds[i].size < FPC_BUFFER_SIZE)
                cmds[i].cmd  = FPC_ERROR;
            else                
            cmds[i].cmd  = FPC_DONE;
            // switch to next command
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
            i           ^= 1;              
            break;
#endif            
            if(wrong_flash_addr != 0 && wrong_flash_addr != (UINT32)cmds[i].flashAddr)
            {
                cmds[i].cmd  = FPC_DONE;
                errorDuringCommand = 1;
            }
            // Do the RAM check if we are asked too. 
            // Do also a check of the FLASH (verify if everything is at FF).
            // These operations are port of the intensive verify.
            else if(cmds[i].fcs & 0x80000000)
            { 
                fcsCtx.srcAddr      = cmds[i].ramAddr;
                fcsCtx.transferSize = cmds[i].size;
                fcsCtx.userHandler  = NULL;
                
                hal_DmaFcsStart(&fcsCtx);
                hal_DmaReadFcs(fcs);    
                
                if( (cmds[i].fcs & 0x00FFFFFF) != (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]) )
                {
                    errorDuringCommand = 1;
                    cmds[i].cmd = FPC_FCS_ERROR; // FCS error
                    wrong_flash_addr = (UINT32)cmds[i].flashAddr;
                }
                else
                {
                    wrong_flash_addr = 0;
                    // Verify the content of the flash, if every byte is at FF
                    verifySectorAddress = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress((UINT8*)cmds[i].flashAddr));
                    for(cpt=0;cpt<cmds[i].size;cpt+=4,verifySectorAddress++)
                    {
                        if(*verifySectorAddress!=0xFFFFFFFF)
                        {
                            errorDuringCommand = 1;
                            SEND_EVENT((UINT32 )verifySectorAddress);
                            cmds[i].cmd = FPC_FLASH_NOT_AT_FF; // FCS error
                            break;
                        }
                    } 
                }
            }
            
            //Everything's ok til now
            if(errorDuringCommand==0)
            {
                //hal_HstSendEvent(SYS_EVENT,cmds[i].flashAddr);
                //SEND_EVENT(cmds[i].flashAddr);
		        //SEND_EVENT( (CHAR*) (cmds[i].ramAddr));	
                status       = memd_FlashWrite(cmds[i].flashAddr,
                                               cmds[i].size, &s,
                                               (CHAR*) (cmds[i].ramAddr));
                cmds[i].size = s;
                if (status != MEMD_ERR_NO)
                {
                    errorDuringCommand = 1;
                    cmds[i].cmd = FPC_ERROR; //Standard error
                }
                else
                { 
                    // Flash write was ok, write magic number.
                    for(cpt = 0; flashSpecialSector[cpt].magicNumber != 0x00000000; ++cpt)
                    {
                        if(cmds[i].flashAddr == flashSpecialSector[cpt].address)
                        {
                            sectorWrite |= 1<<cpt;
                        }
                    }
                }
            }
            // ERROR MANAGEMENT FOR FPC_PROGRAM
            if(errorDuringCommand)
            { // programm Error
                
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(SYS_EVENT,0xef00000-status);
#endif
            }
            else
            {
                // Set command is done, switch to next command
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;              
            }
            break;
            
        case FPC_ERASE_SECTOR:
            if(userBase == (UINT32)cmds[i].flashAddr)
            {
                status = memd_FlashErase(cmds[i].flashAddr,cmds[i].flashAddr+userSize);
            }
            else
                status = memd_FlashErase(cmds[i].flashAddr,NULL);
            if (status == MEMD_ERR_NO)
            { // OK
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
            }
            else
            { // Error
                cmds[i].cmd = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(SYS_EVENT,0xee00000-status);
#endif
            }
            break;
            
        case FPC_ERASE_CHIP:
            // Not implemented.
            cmds[i].cmd = FPC_ERROR;
            SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
            break;
            
        case FPC_GET_FINALIZE_INFO:
            // Count the number of sectors we have in the flashSpecialSector.
            cpt2 = 0;
            while(flashSpecialSector[cpt2].magicNumber != 0)
                cpt2++;
            
            // Write into ram all the magic numbers and their addresses
            verifySectorAddress     = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr);
            for(cpt = 0; cpt < cpt2 ; cpt++)
            {
                *verifySectorAddress = (UINT32)flashSpecialSector[cpt].address;
                verifySectorAddress++;
                *verifySectorAddress = flashSpecialSector[cpt].magicNumber;
                verifySectorAddress++;
            }
            
            // Inform PC that the command has been treated
            cmds[i].size = cpt2;
            cmds[i].cmd  = FPC_DONE;
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
            i           ^= 1;            
            
            break;
        case FPC_CHECK_FCS:
            s = 0;
#if CHIP_ASIC_ID!=CHIP_ASIC_ID_5857           
            verifySectorAddress     = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr);
            verifySectorSize        = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr+4);
            verifyAwaitedFcs        = (UINT32*)HAL_SYS_GET_UNCACHED_ADDR(cmds[i].ramAddr+8);
            verifyFirstBadSector    = 0xFFFFFFFF;
            
            for(cpt=0; cpt<cmds[i].size; cpt++)
            {
                fcsCtx.srcAddr      = (UINT8*)HAL_SYS_GET_UNCACHED_ADDR(memd_FlashGetGlobalAddress((UINT8*)*verifySectorAddress));
                fcsCtx.transferSize = *verifySectorSize;
                fcsCtx.userHandler  = NULL;

                if(*verifySectorAddress == userBase) // don't check user data
                    continue;
                hal_DmaFcsStart(&fcsCtx);
                hal_DmaReadFcs(fcs);    
                if( *verifyAwaitedFcs != (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]) )
                {
                    // Uh, oh. Memcmp failed, there's an error.
                    s++;
                    // Replace FCS in ram for this block by the bad fcs
                    *verifyAwaitedFcs = (fcs[0]<<16 | (fcs[1] << 8) | fcs[2]);
                    // Memorize first bad sector
                    if(verifyFirstBadSector == 0xFFFFFFFF)
                    {
                        verifyFirstBadSector = cpt;
                    }
                }
                verifySectorAddress += 3;
                verifySectorSize    += 3;
                verifyAwaitedFcs    += 3;
            }
#endif
            if(s!=0)
            {
                //Send error code.
                cmds[i].ramAddr     = (UINT8*)verifyFirstBadSector;   
                cmds[i].cmd         = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
            }
            else
            {
                // No error, write the done flag and pass to the other command
                cmds[i].cmd  = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
            }
            break;
            case FPC_CHECK_CALIBRFPART_CRC:

                fcsCtx.srcAddr      = (UINT8*)(((UINT32)(cmds[i].flashAddr))|0x80000000);    // calibration sector address
                hal_HstSendEvent(SYS_EVENT,fcsCtx.srcAddr[3] << 24|fcsCtx.srcAddr[2]<<16|fcsCtx.srcAddr[1] << 8|fcsCtx.srcAddr[0]);
                if(fcsCtx.srcAddr[3] == 0xCA && fcsCtx.srcAddr[2] == 0x1B)
                { 
                    if((fcsCtx.srcAddr[1] << 8|fcsCtx.srcAddr[0]) > 0x0203 )
                    { 
                        m_CalibRfCrcAdd   = 0x1cf8;   // calibration sector size 0x1cfc
                        fcsCtx.transferSize = 0x1360;    // The length of RF calibration data
                        fcsCtx.userHandler  = NULL;

                        hal_DmaFcsStart(&fcsCtx);
                        hal_DmaReadFcs(fcs); 

                        if((fcs[0] == fcsCtx.srcAddr[m_CalibRfCrcAdd])&&
                                (fcs[1] == fcsCtx.srcAddr[m_CalibRfCrcAdd+1])&&
                                (fcs[2] == fcsCtx.srcAddr[m_CalibRfCrcAdd+2]))
                        { 
                            cmds[i].cmd  = FPC_DONE;
                            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                            i           ^= 1;
                        }
                        else
                        { 
                            cmds[i].cmd  = FPC_DONE;
                            SEND_EVENT(EVENT_CALIB_RFCRC_ERROR + i);
                        }

                    }
                    else  // for low calibration version
                    { 
                        cmds[i].cmd  = FPC_DONE;
                        SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                        i           ^= 1;
                    }
                }
                else
                { 
                    cmds[i].cmd  = FPC_DONE;
                    SEND_EVENT(EVENT_CALIB_RFCRC_ERROR + i);
                }
                break;
            
        case FPC_END:
            status = MEMD_ERR_NO;
            for (cpt = 0; sectorWrite >> cpt; ++cpt)
            {
                if (sectorWrite & (1<<cpt))
                {
                    status = memd_FlashWrite(flashSpecialSector[cpt].address,
                                             sizeof(UINT32), &s,
                                             (CHAR*)(&flashSpecialSector[cpt].magicNumber));
                    if (status != MEMD_ERR_NO)
                    {
                        break;
                    }
#if CHIP_ASIC_ID==CHIP_ASIC_ID_5857
                    // Check if the write of the magic number succeeded
                    if(*((UINT32*)(memd_FlashGetGlobalAddress(flashSpecialSector[cpt].address)))
                         != flashSpecialSector[cpt].magicNumber)
#else
                    if(*((UINT32*)(((UINT32)(memd_FlashGetGlobalAddress(flashSpecialSector[cpt].address)))|0xa0000000))
                         != flashSpecialSector[cpt].magicNumber)
#endif
                    {
                        status = -127; // FIXME: A bit permissive.
                        break;   
                    }
                }
            }
            
            if(status == MEMD_ERR_NO) 
            {
                UINT32 TestmodeFlag = 0x00002021; // move a0,zero
                //if(cmds[i].size == 0) // enable testmode or not
                TestmodeFlag = cmds[i].size;
                // write testmode flag
                status = memd_FlashWrite((UINT8*)0x04,
                                         sizeof(UINT32), &s,
                                         (UINT8*)(&TestmodeFlag));
            }
            
            if (status == MEMD_ERR_NO)
            { // OK
                cmds[i].cmd = FPC_DONE;
                SEND_EVENT(EVENT_FLASH_PROG_READY + i);
                i           ^= 1;
#if (CHIP_HAS_USB == 1) && (FASTPF_USE_USB==1)
                //pmd_SetLevel(PMD_LEVEL_LCD, 7);
                //pmd_SetLevel(PMD_LEVEL_KEYPAD, 7);
                //pmd_EnablePower(PMD_POWER_CAMERA_FLASH, TRUE);
#endif
            }
            else
            { // programm Error
                cmds[i].cmd = FPC_ERROR;
                SEND_EVENT(EVENT_FLASH_PROG_ERROR + i);
#ifdef DEBUG_EVENTS
                hal_HstSendEvent(SYS_EVENT,0xef00000-status);
#endif
            }
            sectorWrite = 0;
			
            break;

        case FPC_RESTART:
            cmds[i].cmd = FPC_DONE;
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
            // Sleep a bit to wait for a few ms to be sure the event has been sent.
            hal_HstWaitLastEventSent();
            // Wait 3 32kHz ticks to let the event some distance ahead
            // before unleashing the dogs.
            //hal_TimDelay(3);
            
            // Restart the power cycle the USB Phy and restart the system.
            hal_SysRestart();
            break;
        default:
            cmds[i].cmd = FPC_ERROR;
            SEND_EVENT(EVENT_FLASH_PROG_READY + i);
#ifdef DEBUG_EVENTS
            hal_HstSendEvent(SYS_EVENT,0xeee0000);
#endif
            break;
        }
    }
}

#endif
