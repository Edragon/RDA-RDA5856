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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file boot_loader.c                                                         //
/// That file contains the XCPU init function                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "bb_sram.h"

#include "boot.h"
#include "boot_host.h"
#include "bootp_debug.h"
#include "bootp_loader.h"

#include "tgt_hal_cfg.h"

#include "hal_host.h"
#include "hal_mem_map.h"
#include "hal_open.h"
#include "hal_timers.h"
#include "hal_sys.h"
#include "hal_map_engine.h"
#include "hal_overlay.h"

#include "hal_debug.h"
#include "hal_dma.h"
#include "hal_bt.h"

//#include "tgt_calib_m.h"
#include "boot_map.h"
#include "pmd_config.h"

// For the opening of the flash
#include "memd_m.h"
#include "tgt_memd_cfg.h"
#include "tgt_m.h"
#if defined(TGT_WITH_TS_MODEL_rda1207)
#include "rda1207.h"
#endif /* TGT_WITH_TS_MODEL_rda1207 */

#include "sys_irq.h"
#include "gpio.h"
#include "register.h"
// Boot loader entry point.
extern VOID boot_LoaderEnter(UINT32 param);
extern PROTECTED VOID mon_Event(UINT32 evt);
// ROM version number (defined in ROM)
extern CONST UINT32 g_bootRomVersionNumber;

// Standard main entry, user function.
extern int main(int, char*[]);

// Calibration entry point.
extern int calib_StubMain(int, char*[]);

// initialise cos memory
VOID COS_MemInit();

// =============================================================================
// g_halFixedPointer
// -----------------------------------------------------------------------------
/// Address of the fixed pointer. At this address, the host tools
/// will find a pointer to the configuare structure.
// =============================================================================

#define BOOT_LOADER_SECTION_START __attribute__((section(".text.boot_loader_start")))

// initialise user data area in flash
UINT32 pmd_GetEncryptUid();

// =============================================================================
//  boot_LoaderEnter
// -----------------------------------------------------------------------------
/// Real implementation of the boot loader, loading code after the stack has 
/// been reseted in #boot_LoaderEnter();
///
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID boot_Loader(UINT32 param);

#if 0
#include "boot_sector_chip.h"
extern PROTECTED VOID boot_save_context(uint32 lpmode);
void boot_Pm(uint8 pmmode)
{
    uint32 data;
	
    g_fastEntry = BOOT_FASTENTRY_VALID_TAG;
    hwp_pmuc->pwr_sys_ctrl &=  ~PMUC_PM_REG_MASK;
    data = hwp_pmuc->pwr_sys_ctrl | PMUC_PM_REG(pmmode);
    boot_save_context(data);
}
#endif
// =============================================================================
//  boot_LoaderEnter
// -----------------------------------------------------------------------------
/// Entry point for the code out of the boot sector.
///
/// To compensate for the stack usage of the boot romed code, this function
/// resets the stack pointer at the usable top of the internal SRAM, and call
/// the 'real' boot loader without any local variable on the stack that could
/// thus get corrupted.
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
PROTECTED VOID BOOT_LOADER_SECTION_START boot_LoaderEnter(UINT32 param)
{
    // Initializes IRQ stack top to the top of the available sram
    // and set here stack pointer to the desired location,
    // at the top of the int sram that is not preallocated for, eg,
    // the mailbox. This location is provided through the symbol
    // properly defined in the linker script _sys_sram_top.
    sxr_IrqStack_pointer = (UINT32) &_sys_sram_top;
    // $29 is the SP register.
#ifdef CHIP_CPU_IS_MIPS
    asm volatile("move $29, %0" : /* no output */ : "r"(sxr_IrqStack_pointer));
#endif
    #if 0
    mon_Event(0x61);
    boot_Pm(1);
    mon_Event(0x62);
    while(1);
   #endif
    boot_Loader(param);

    // Sep. 05, 2009 - Hua Zeng - Workaround for stack corruption issue after resetting SP.
    // If we are not using -mips16 (ISA is mips1 by default), with -O2 or more optimazation specified,
    // in case that the final statement is a function call, the compiler will pop the using stack first
    // and then branch to the final function call. Unfortunately, here we just reset the SP to
    // the top of the stack space, so the stack of the final function falls beyond the stack top.
    // The workaournd is to add a dummy statement as the final statement so as to avoid the optimization.
    asm volatile("nop");
}

// =============================================================================
//  boot_BreakBootMain
// -----------------------------------------------------------------------------
/// Called when launching the code in elfmaster debug mode. The purpose is to 
/// break when the code starts, to begin a debugging session.
///
/// Hi! This is Elf Master. You got here because you started me
/// with the "Run Program" option. Your code has been restarted,
/// and is currently stalled at the beginning. You can hit
/// "continue" to unstall it, or you can set some additional 
/// breakpoints where you need to.
// =============================================================================
PROTECTED VOID boot_BreakBootMain( VOID )
{
    // WELCOME TO ELFMASTER!
#ifdef CHIP_CPU_IS_MIPS
    BREAKPOINT;
#endif
    main(0, 0);
}

// =============================================================================
//  boot_Loader
// -----------------------------------------------------------------------------
/// Real implementation of the boot loader, loading code after the stack has 
/// been reseted in #boot_LoaderEnter();
///
/// @param param This parameter as a dummy value when we boot normally from ROM,
/// but it has a special value when the function is called by a jump command
/// of a Boot Monitor (Host, USB or UART). This is used to change the main of
/// the code (where we will jump at the end of the boot loading sequence).
// =============================================================================
extern VOID btcpu_test1(VOID);

extern UINT32 _bt_ram_start;
extern UINT32 _bt_ram_end;
extern UINT32 btrom_data_ram_start;
extern UINT32 btrom_data_inrom_end;
extern UINT32 btrom_data_ram_size;
extern UINT32 btrom_data_inrom_base;
extern UINT32 _tflash_sys_sram_start;
extern UINT32 _sys_sram_tflash_start_location;
extern UINT32 _sys_sram_tflash_end_location;
extern UINT32 _sys_sram_data_start;
extern UINT32 _tflash_sram_bss_start;
extern UINT32 _tflash_sram_bss_end;
extern UINT32 _res_data_flash_start_location;

BOOL g_test_mode = FALSE; 
BOOL boot_get_testmode(VOID); 
VOID boot_set_testmode(BOOL mode); 
BOOL boot_get_testmode(VOID)
{
  return g_test_mode; 
}
VOID boot_set_testmode(BOOL mode)
{
  g_test_mode = mode; 
}

PROTECTED VOID boot_Loader(UINT32 param)
{
    UINT32* src;
    UINT32* dst;
#ifndef _FLASH_PROGRAMMER
    UINT32 system_flag;
#endif

    // NO FUNCTION CALL ALLOWED AS LONG AS THE CODE HAS NOT BEEN
    // COPIED FROM FLASH TO RAM...
	
#if 0 // tianwq
    // If a WD occured, treat it before initializing the bb SRAM.
    if (hwp_sysCtrl->Reset_Cause & SYS_CTRL_WDT_RST)
    {
        if  ((g_halWatchdogCtx.validityMarker == 0xC001C001) ||
            (g_halWatchdogCtx.validityMarker == 0xDEADBEEF) )
        {
            // This is a real watchdog and the structure is valid.
            UINT32* saved_context_ptr = (UINT32*) KSEG1(REG_BB_SRAM_BASE);
            UINT32 i;

            // Copy from BB_SRAM into External sram structure.
            for (i = 0; i < sizeof(HAL_DBG_ERROR_CTX_T)/4; i++)
            {
                *(&(g_halWatchdogCtx.errorContext.zero) + i) =
                    *(saved_context_ptr + i);
            }
            // reset the marker to normal
            g_halWatchdogCtx.validityMarker = 0xC001C001;
            hal_TimeWdRecover();
        }
        else
        {
            // The structure is not valid behave as in case of soft reset.
            g_halWatchdogCtx.validityMarker = 0xDEADBEEF;
            //hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_WATCHDOG_STATUS;
        }
    }
    else
    {
        g_halWatchdogCtx.validityMarker = 0xC001C001;
    }
#endif

#ifndef _FLASH_PROGRAMMER
    system_flag = *(((U32 *)(&_flash_start))+1);
#endif
#ifdef MEMD_EXTA_OPEN_EARLY
    pmd_OpalSpiWrite(RDA_ADDR_LDO_POWER_ON_SETTING1, 0xBFD3); // RDA_ADDR_LDO_POWER_ON_SETTING1, set pu_vio1p8_pon
#endif

    // Set the global variables from ROM not set by the ROM code.
    xcpu_error_code   = 0;
    xcpu_error_status = 0;
    xcpu_sp_context   = 0;

#ifndef _FLASH_PROGRAMMER
    // Copy code & data from ROM space to Dualport SRAM.
    for (dst = (UINT32*) &_bcpu_sys_sram_bss_start;
         dst < (UINT32*) &_bcpu_sys_sram_bss_end;
         dst++)
    {
        *dst = 0;
    }

    // Clear BSS in Dualport SRAM.
    for (dst = (UINT32*) &_dualport_bss_start;
         dst < (UINT32*) &_dualport_bss_end;
         dst++)
    {
        *dst = 0;
    }

    // Clear BSS in Dualport SRAM.
    for (dst = (UINT32*) &_ram_bss_start;
         dst < (UINT32*) &_ram_bss_end;
         dst++)
    {
        *dst = 0;
    }
         
    // Clear BSS for TFlash upgrade module.
    for (dst = (UINT32*) &_tflash_sram_bss_start;
         dst < (UINT32*) &_tflash_sram_bss_end;
         dst++)
    {
        *dst = 0;
    }
    // copy data for TFlash upgrade module
    for (src = (UINT32*) &_sys_sram_tflash_start_location,
             dst = (UINT32*) &_sys_sram_data_start;
         src < (UINT32*) &_sys_sram_tflash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

#endif
#if 1
    // Copy irq code from ROM space to internal SRAM.
    for (src = (UINT32*) &_irq_sys_sram_flash_start_location,
             dst = (UINT32*) &_irq_sys_sram_start;
         src < (UINT32*) &_irq_sys_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }
#else	
    hal_DmaMemCopy((UINT32*)&_irq_sys_sram_start, (UINT32*)&_irq_sys_sram_flash_start_location, (UINT32)&_irq_sys_sram_size);
#endif
    // Copy code & data from ROM space to internal SRAM.
    for (src = (UINT32*) &_sys_sram_flash_start_location,
             dst = (UINT32*) &_sys_sram_start;
         src < (UINT32*) &_sys_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }
#if 1
    // Clear BSS in Internal SRAM.
    for (dst = (UINT32*) &_sys_sram_bss_start;
         dst < (UINT32*) &_sys_sram_bss_end;
         dst++)
    {
        *dst = 0;
    }
#else
    hal_DmaMemSet((UINT32*) &_sys_sram_bss_start, 0, (UINT32)&_sys_sram_bss_size);
#endif

#ifndef _FLASH_PROGRAMMER // flash don't use bt
#ifdef BTCORE_IN_RAM
    for (src = (UINT32*) &_bt_sram_flash_start_location,
             dst = (UINT32*) &_bt_sram_start;
         src < (UINT32*) &_bt_sram_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }
#endif
#endif


    // setup heap
    COS_MemInit();


#ifndef _FLASH_PROGRAMMER // flash don't use host config and memory manager
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
    //hal_BTOverlayLoad(HAL_BTOVERLAY_FILESYSTEM);
    hal_BTOverlayLoad(HAL_BTOVERLAY_BLUETOOTH);
#endif


    // Initialise Resource
    {
        extern  U8 *gpResourceBase;
        gpResourceBase = (U8*)&_res_data_flash_start_location;
    }
#endif

#ifdef REWRITE_ROM_AT_BOOT
#ifdef FPGA
    // Copy data from flash space to internal ROM.

    // Unlock.
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_ENABLE;

    for (src = (UINT32*) &_int_rom_flash_start_location,
             dst = (UINT32*) &_int_rom_start;
         src < (UINT32*) &_int_rom_flash_end_location;
         src++, dst++)
    {
        *dst = *src;
    }

    // Lock.
    hwp_memBridge->Rom_Patch[0] = MEM_BRIDGE_PATCH_DISABLE;
#endif // FPGA
#else // !REWRITE_ROM_AT_BOOT
    // Check that the ROM version in the chip is the same as the ROM version
    // of the ROM Library against which the code has been compiled.
    //if(ROM_LINKED_AGAINST_VERSION != *(UINT32*)ROM_VERSION_LOCATION)
    //if(ROM_LINKED_AGAINST_VERSION != *(UINT32*)&_int_rom_version_number) // for 8808 or later
    if(0)//ROM_LINKED_AGAINST_VERSION != g_bootRomVersionNumber)
    {
        // Mismatch!
        hal_HstSendEvent(BOOT_EVENT,BOOT_LOADER_EVENT_ROM_ERROR);
        while(1);
    }
#endif // !REWRITE_ROM_AT_BOOT

#ifndef _FLASH_PROGRAMMER
#ifndef NO_BCPU_IMAGE
#ifndef CHIP_HAS_BCPU_ROM
    // Same thing for the bcpu. These are needed by GDB.
    bcpu_error_code   = HAL_DBG_ERROR_NO;
    bcpu_error_status = 0xc001c001;
    bcpu_sp_context   = (UINT32*)0x0db90db9;
#endif
#endif

#endif // _FLASH_PROGRAMMER

#ifndef _FLASH_PROGRAMMER
//#if 0
    // FUNCTIONS CAN BE CALL NOW!
    // Enable exception (gdb breaks), Host, Debug and Page Spy external it,
    // but not regular external it.
    #ifdef CHIP_CPU_IS_MIPS
    asm volatile("li    $27,    (0x0001 | 0xff00)\n\t"
                 "mtc0  $0, $13\n\t"
                 "nop\n\t"
                 "mtc0  $27, $12\n\t"
                 // k1 is status for os irq scheme.
                 "li    $27, 1");
    #endif
       
#endif

    // get CdacDelta
#ifndef _FLASH_PROGRAMMER
    //hal_HstSendEvent(BOOT_EVENT,BOOT_LOADER_EVENT_START_CODE);
    {
        extern INT16 g_CdacDelta;
        if((system_flag&4)==0)
            g_CdacDelta = (INT16)*(((INT32 *)(&_flash_start))+2);
    }
#endif

 //   hal_TimDelay(1638*3);
    tgt_Open();
#ifdef _FLASH_PROGRAMMER
    // Initialize the map engine.
    hal_MapEngineOpen();
#else
    hal_Open(tgt_GetHalConfig());
    hal_HstSendEvent(BOOT_EVENT,BOOT_LOADER_EVENT_START_CODE);
    //mon_Event(BOOT_LOADER_EVENT_START_CODE);
#endif

#ifndef _FLASH_PROGRAMMER // flash don't need decrpyt and testmode
    if(system_flag&2) // decrypt flash
    {
        UINT8 *data_buf = (UINT8*)&_res_data_flash_start_location;
        UINT32 encrypt_key = 0;
        int count;
        count = ((data_buf[2])+(data_buf[3]<<8));
        while(count>=0)
        {
            if((data_buf[(2+count)*4])==0x03)
            {
                count = (2+count)*4+((data_buf[(2+count)*4+2])+((data_buf[(2+count)*4+3])<<8))+38;
                encrypt_key = 0x5851;//data_buf[count] + (data_buf[count+1]<<8) + (data_buf[count+2]<<16) + (data_buf[count+3]<<24);
                encrypt_key = encrypt_key ^ (pmd_GetEncryptUid()<<2);
                break;
            }
            count --;
        }

        count = 0;
        for (dst = (UINT32*) &_sys_sram_start;
             count < ((UINT32)&_sys_sram_txt_size)/4-4;
             dst++, count++)
        {
            *dst = (*dst) ^ encrypt_key;
       }

    }

    // check test mode
    {
        //extern BOOL g_test_mode;
        UINT32 s;
        if(system_flag&1)
        {
            //g_test_mode = TRUE;
            boot_set_testmode(TRUE); 
            system_flag = system_flag&~1;
            memd_FlashWrite((UINT8*)0x04,sizeof(UINT32), &s,(CHAR*)(&system_flag));
        }
    }
#if CHIP_HAS_BTCPU==1
    if(tgt_GetPmdConfig()->power[PMD_POWER_BT].powerOnState)
        hal_BtStart(NULL, NULL, NULL);
#endif
#endif

    BOOT_PROFILE_PULSE(XCPU_MAIN);

    // Selects where we will boot: normal main or calib stub main.
    switch(param)
    {
#if defined(SERVICE_CALIB_SUPPORT)&&!defined(_FLASH_PROGRAMMER)
        case CALIB_MAGIC_TAG:
            calib_StubMain(0, 0);
            break;
#endif
        case BOOT_MAIN_TAG_BREAKBOOT:
            // Gdb needs the flash to be open to be able to step in the romulator!!!
            boot_BreakBootMain();
            break;
        default:
            main(0, 0);
            break;
    }
}

