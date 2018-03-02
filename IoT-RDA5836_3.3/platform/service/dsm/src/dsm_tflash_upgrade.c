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

#include "dsm_cf.h"
#include "dsm_dbg.h"
#include "chip_id.h"
#include "fs.h"
#include "memd_m.h"
#include "vds_api.h"
#include "hal_mem_map.h"
#include "hal_host.h"
#include "tgt_app_cfg.h"
#include "boot_sector.h"
#include "boot_sector_chip.h"
#include "boot_ispi.h"
#include "hal_timers.h"
#include "hal_bt.h"
#include "string.h"


#define T_UPDATE_CFG_NAME             "/update.cfg"
#define T_UPDATE_PROG_KEY             "T_PROGRAMMER"
#define T_UPDATE_KEY                  "T_UPDATE"
#define T_UPDATE_PROG_BIN_KEY         "BIN_T_PROGRAMMER"
#define T_UPDATE_BIN_KEY              "BIN_T_UPDATE"
#define T_UPDATE_CFP_KEY              "T_CFP"
#define T_CLEAR_USER_DATA_KEY         "T_CLEAR_USER_DATA"

#define T_UPDATE_MAX_LINE               128

#define T_UPDATE_BLOCK_SIZE 0x1000   //16k Bytes and 4k double words 
//#define T_UPDATE_BLOCK_SIZE 0x4000   //16k Bytes and 4k double words 

#define T_UPDATE_BUF_BYTE 0x4000
#define T_UPDATE_BUF_DWORD 0x1000


#define T_UPDATE_BUF_BYTE_STEP_1 0x4000
#define T_UPDATE_BUF_DWORD_STEP_1 0x1000


#ifdef SERVICE_BASE_FS_SUPPORT
//extern uint32 bigbuf[0x2000]; //

#if defined(_T_UPGRADE_PROGRAMMER)
extern uint32 bigbuf[0x1000]; // 
#else
extern uint32 bigbuf[0x1000]; // 

#endif
PRIVATE BOOT_TFLASH_UPGRADE_STRUCT_T 
    HAL_BOOT_TFLASH_UPGRADE_SECTION g_DsmTflashUpgradeStruct;

VOID  DSM_Program_Fist_Part_Lod();
VOID DSM_TFlash_Upgrade_Step2_Lod();
VOID  DSM_Program_Fist_Part_Bin();
VOID DSM_TFlash_Upgrade_Lod(INT32 disk, INT32 file);
extern INT32 FS_GetLodHeader(int32 inode_lod, FS_LodHeader *header);
EXPORT PUBLIC BOOL spi_flash_block_erase(UINT32 flash_addr);
 VOID DSM_TFlash_Upgrade_Step2()
{
#ifdef UPGRADE_BY_BIN
    DSM_TFlash_Upgrade_Step2_Bin();
#else
    DSM_TFlash_Upgrade_Step2_Lod();
#endif
}
 VOID DSM_Program_Fist_Part() 
{
#ifdef UPGRADE_BY_BIN
    DSM_Program_Fist_Part_Bin(); 
#else
    DSM_Program_Fist_Part_Lod();
#endif
}

 VOID DSM_TFlash_Upgrade(INT32 disk, INT32 file)
{
#ifdef UPGRADE_BY_BIN
    DSM_TFlash_Upgrade_Bin(disk,file);
#else
    DSM_TFlash_Upgrade_Lod(disk,file);
#endif
}


BOOL DSM_UpgradeStructGetValid(VOID)
{
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();
    // Is the address word-aligned?
    if (((UINT32)pUpgradeStruct & 0x3) != 0)
    {
        return FALSE;
    }

    // Is the address in INT-SRAM space?
    if ( ! (
             ( (UINT32)pUpgradeStruct >= KSEG0(REG_INT_SRAM_BASE) &&
                  (UINT32)pUpgradeStruct < KSEG0(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) ) ||
             ( (UINT32)pUpgradeStruct >= KSEG1(REG_INT_SRAM_BASE) &&
                  (UINT32)pUpgradeStruct < KSEG1(REG_INT_SRAM_BASE)+sizeof(HWP_INT_SRAM_T) )
           ) )
    {
        return FALSE;
    }

    if( (pUpgradeStruct->validTag != BOOT_TFLASH_UPGRADE_VALID_TAG) ||
        (pUpgradeStruct->checkSum != ~BOOT_TFLASH_UPGRADE_VALID_TAG) )
    {
        return FALSE;
    }
    return TRUE;
}

MEMD_ERR_T DSM_Program_Packet(FS_LodPacket *packet) 
{
    UINT32 ByteSize;
    MEMD_ERR_T result;

    hal_HstSendEvent(BOOT_EVENT, 0x92ade500);
    hal_HstSendEvent(BOOT_EVENT, packet->address);
    hal_HstSendEvent(BOOT_EVENT, packet->nbWords);
    
    if(packet->address==0)
        return MEMD_ERR_ALIGN;

    UINT32 cs = hal_SysEnterCriticalSection();
    result = memd_FlashErase2((UINT8*)(packet->address&0xffffff), (UINT8*) ((packet->address&0xffffff)+packet->nbWords*4));
    hal_HstSendEvent(BOOT_EVENT, result);
    if(result == MEMD_ERR_NO)
        result = memd_FlashWrite((UINT8*)(packet->address&0xffffff), packet->nbWords*4, &ByteSize, (UINT8*)packet->words);
    hal_HstSendEvent(BOOT_EVENT, result);
    hal_SysExitCriticalSection(cs);
    return result;
}

MEMD_ERR_T DSM_Program_Packet_Noerase(FS_LodPacket *packet) 
{
    UINT32 ByteSize;
    MEMD_ERR_T result;

    hal_HstSendEvent(BOOT_EVENT, 0x92ade500);
    hal_HstSendEvent(BOOT_EVENT, packet->address);
    hal_HstSendEvent(BOOT_EVENT, packet->nbWords);
    
    if(packet->address==0)
        return MEMD_ERR_ALIGN;

    UINT32 cs = hal_SysEnterCriticalSection();
    //result = memd_FlashErase2(packet->address&0xffffff, (packet->address&0xffffff)+packet->nbWords*4);
    //hal_HstSendEvent(BOOT_EVENT, result);
    //if(result == MEMD_ERR_NO)
        result = memd_FlashWrite((UINT8*)(packet->address&0xffffff), packet->nbWords*4, &ByteSize, (UINT8*)packet->words);
    hal_HstSendEvent(BOOT_EVENT, result);
    hal_SysExitCriticalSection(cs);
    return result;
}


VOID  DSM_Program_Fist_Part_Lod()
{
    FS_LodPacket packet;
    UINT32 flash_pattern;
    UINT32 endpos;
    COS_EVENT ev;
    UINT32 ByteSize;
    INT32 file;
    INT32 card_ok = -1;
    //MEMD_ERR_T result;

    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct;

    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();

    while(card_ok!=ERR_SUCCESS)
    {
        COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
        if(ev.nEventId == EV_FS_UDISK_READY)
        {
            COS_Sleep(100);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
        else
        {
            COS_Sleep(100);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade223);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
    }
    
    hal_TimRtcIrqSetMask(FALSE); // disable time clock
    
   
    hal_HstSendEvent(BOOT_EVENT, 0x92ade300);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->system_lod_file);
   
    if(pUpgradeStruct->clearUserData)
    {
        FS_LodGetUserLodPacket(pUpgradeStruct->system_lod_file, &packet);
        hal_HstSendEvent(BOOT_EVENT, 0x92ade400);
        hal_HstSendEvent(BOOT_EVENT, packet.address);
        hal_HstSendEvent(BOOT_EVENT, packet.nbWords);

        UINT32 cs = hal_SysEnterCriticalSection();
        memd_FlashErase2((UINT8*)(packet.address&0xffffff), (UINT8*)((packet.address&0xffffff)+packet.nbWords*4));
        hal_SysExitCriticalSection(cs);
    }

    file = FS_OpenDirect(pUpgradeStruct->system_lod_file, FS_O_RDONLY, 0);
    hal_HstSendEvent(BOOT_EVENT, file);


    FS_LodGetNextPacket(file, &packet);
    endpos = packet.words[7];
    do{
        if(packet.address==0)
            break;

        if(DSM_Program_Packet(&packet) != MEMD_ERR_NO)
            break;

        if(FS_LodGetNextPacket(file, &packet)!=ERR_SUCCESS)
            break;
        if((packet.address&0xffffff) >= (endpos&0xffffff))
        {
            break;
        }
    }while(1);
    
    FS_Close(file);
    if(pUpgradeStruct->system_cfp_file != 0)
    {
        file = FS_OpenDirect(pUpgradeStruct->system_cfp_file, FS_O_RDONLY, 0);
        hal_HstSendEvent(BOOT_EVENT, 0x92ade450);

        FS_GetLine(NULL, 0, file); // reset line buffer
        FS_LodGetNextPacket(file, &packet);
        DSM_Program_Packet(&packet);
        FS_Close(file);
    }

    pUpgradeStruct->step = 3;

    // end of program
    hal_HstSendEvent(BOOT_EVENT, 0x92ade600);
    flash_pattern = BOOT_FLASH_PROGRAMMED_PATTERN;
    memd_FlashWrite((UINT8*)0, 4, &ByteSize, (UINT8*)&flash_pattern);
    flash_pattern = 0;
    memd_FlashWrite((UINT8*)4, 4, &ByteSize, (UINT8*)&flash_pattern);

    //TODO:  add upgrade success notify here

    hal_SysRestart();
}

VOID DSM_TFlash_Upgrade_Step2_Lod()
{
    COS_EVENT ev;
    FS_LodPacket packet;
    INT32 file;
    UINT32 startpos;
    //MEMD_ERR_T result;
    INT32 card_ok = -1;
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct;

    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();
    hal_HstSendEvent(BOOT_EVENT, 0x92ade220);
    hal_HstSendEvent(BOOT_EVENT, (UINT32)pUpgradeStruct);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->deviceType);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->system_lod_file);

    if(pUpgradeStruct->deviceType == FS_DEV_TYPE_USBDISK)
    {
        while(card_ok!=ERR_SUCCESS)
        {
            hal_HstSendEvent(BOOT_EVENT, 0x92ade221);
            COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
            if(ev.nEventId == EV_FS_UDISK_READY)
            {
                COS_Sleep(100);
                card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
                hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
                hal_HstSendEvent(BOOT_EVENT, card_ok);
            }
        }
    }
    else
    {
        while(card_ok!=ERR_SUCCESS)
        {
            hal_HstSendEvent(BOOT_EVENT, 0x92ade221);
            //COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
    }
    
    hal_TimRtcIrqSetMask(FALSE); // disable time clock
    file = FS_OpenDirect(pUpgradeStruct->system_lod_file, FS_O_RDONLY, 0);
    hal_HstSendEvent(BOOT_EVENT, 0x92ade230);
    hal_HstSendEvent(BOOT_EVENT, file);

    FS_LodGetNextPacket(file, &packet);
    startpos = packet.words[7];
    hal_HstSendEvent(BOOT_EVENT, startpos);
    FS_GetLine(NULL, (startpos&0xffc000)*2, file);
    do{
        if(FS_LodGetNextPacket(file, &packet)!=ERR_SUCCESS)
            break;
        if((startpos&0xffffff)+packet.nbWords*4>= (startpos&0xffffff))
            break;
    }while(1);    
    do{
        if(packet.address == 0)
            break;
        if(DSM_Program_Packet(&packet) != MEMD_ERR_NO)
            break;
        if(FS_LodGetNextPacket(file, &packet)!=ERR_SUCCESS)
            break;
    }while(1);
    hal_HstSendEvent(BOOT_EVENT, 0x92ade260);
    
    pUpgradeStruct->step = 2;

    boot_IspiSendData(1,0x0b<<16|((12)<<2),FALSE); // enable abb pll
    // program finish, jump to xcpu flash
    {
        VOID(*flashEntry)(UINT32);
        flashEntry = (VOID(*)(UINT32)) startpos;
        flashEntry(BOOT_SECTOR_STANDARD_MAIN);
    }

}

VOID DSM_TFlash_Upgrade_Lod(INT32 disk, INT32 file)
{
    char buff[T_UPDATE_MAX_LINE];
    //char *ptr;
    INT32 program_file = -1;
    INT32 res;
    WCHAR *filename; 
    INT32 f;
    FS_LodPacket packet;
    //MEMD_ERR_T result;
    UINT32 flash_pattern;
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct;
   
    hal_HstSendEvent(BOOT_EVENT, 0x92ade010);
    hal_HstSendEvent(BOOT_EVENT, disk);
    hal_HstSendEvent(BOOT_EVENT, file);

    CSW_TRACE(BASE_FFS_TS_ID,  "Enter DSM_TFlash_Upgrade, disk=%d, file=%d",disk, file);
    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();

    if(DSM_UpgradeStructGetValid())
    {
        if(pUpgradeStruct->step == 2)
        {
            DSM_Program_Fist_Part();
        }
        FS_Close(file);
        return;
    }
    
    g_DsmTflashUpgradeStruct.system_lod_file = 0;
    g_DsmTflashUpgradeStruct.clearUserData = 0;
    g_DsmTflashUpgradeStruct.system_cfp_file = 0;

    do{
        if(FS_GetLine(buff, T_UPDATE_MAX_LINE, file)==NULL)
            break;
        CSW_TRACE(BASE_FFS_TS_ID,  "DSM_TFlash_Upgrade read cfg file, read line:%s",buff);
        if(strncmp(buff, T_UPDATE_KEY, strlen(T_UPDATE_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_KEY)+1, strlen(buff)-strlen(T_UPDATE_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                FS_GetFileINo(f, &g_DsmTflashUpgradeStruct.system_lod_file);
                FS_Close(f);
            }
            COS_Free(filename);
        }
        if(strncmp(buff, T_UPDATE_CFP_KEY, strlen(T_UPDATE_CFP_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_CFP_KEY)+1, strlen(buff)-strlen(T_UPDATE_CFP_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                FS_GetFileINo(f, &g_DsmTflashUpgradeStruct.system_cfp_file);
                FS_Close(f);
            }
            COS_Free(filename);
        }
        if(strncmp(buff, T_UPDATE_PROG_KEY, strlen(T_UPDATE_PROG_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_PROG_KEY)+1, strlen(buff)-strlen(T_UPDATE_PROG_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                program_file = f;
            }
            COS_Free(filename);
        }
        if(strncmp(buff, T_CLEAR_USER_DATA_KEY, strlen(T_CLEAR_USER_DATA_KEY))==0)
        {
            if(strncmp(buff+strlen(T_CLEAR_USER_DATA_KEY)+1, "YES", 3) == 0)
                g_DsmTflashUpgradeStruct.clearUserData = 1;
        }
    }while(1);

    hal_HstSendEvent(BOOT_EVENT, 0x92ade050);
    hal_HstSendEvent(BOOT_EVENT, program_file);
    hal_HstSendEvent(BOOT_EVENT, g_DsmTflashUpgradeStruct.system_lod_file);

    if(program_file >= 0)
    {
        if(g_DsmTflashUpgradeStruct.system_lod_file == 0)
        {
            FS_Close(program_file);
            return;
        }
    }
    else
        return;

    // start program
    hal_HstSendEvent(BOOT_EVENT, 0x92ade100);
    hal_HstSendEvent(BOOT_EVENT, g_DsmTflashUpgradeStruct.system_lod_file);
    hal_HstSendEvent(BOOT_EVENT, program_file);
    //TODO:  add upgrade start notify here

#if APP_SUPPORT_BLUETOOTH==1
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_8809)
    hal_BtStop();
#endif
#endif    
    hal_TimWatchDogClose();
    CSW_TRACE(BASE_FFS_TS_ID,  "DSM_TFlash_Upgrade start program, program_file=%d, system_lod_file=%x",program_file, g_DsmTflashUpgradeStruct.system_lod_file);
    do{
        if(FS_LodGetNextPacket(program_file, &packet)!=ERR_SUCCESS)
        {
            break;
        }
        if(DSM_Program_Packet(&packet) != MEMD_ERR_NO)
            break;

    }while(1);

    FS_Close(program_file);

    // jump to program start
    g_DsmTflashUpgradeStruct.deviceType = disk;
    g_DsmTflashUpgradeStruct.step = 1;
    g_DsmTflashUpgradeStruct.validTag = BOOT_TFLASH_UPGRADE_VALID_TAG;
    g_DsmTflashUpgradeStruct.checkSum = ~BOOT_TFLASH_UPGRADE_VALID_TAG;
    *boot_BootSectorGetTflashUpgradeStructPointer() = &g_DsmTflashUpgradeStruct;

    hal_HstSendEvent(BOOT_EVENT, 0x92ade160);
    flash_pattern = BOOT_FLASH_PROGRAMMED_PATTERN;
    memd_FlashWrite((UINT8*)0, 4, &res, (UINT8*)&flash_pattern);
    flash_pattern = 0;
    memd_FlashWrite((UINT8*)4, 4, &res, (UINT8*)&flash_pattern);
    //hal_TimDelay(50);
    //hal_TimDelay(50);
    //hal_TimDelay(50);
    //hal_TimDelay(1638);
    //{
     //   VOID(*flashEntry)(UINT32);
     //   flashEntry = (VOID(*)(UINT32))0xa8000010;
      //  flashEntry(BOOT_SECTOR_STANDARD_MAIN);
    //}
    hal_SysRestart();
}



VOID DSM_TFlash_Upgrade_Bin(INT32 disk, INT32 file)
{
    char buff[T_UPDATE_MAX_LINE];
    //char *ptr;
    //INT32 program_file = -1;
    INT32 program_bin_file = -1; 
    INT32 res;
    WCHAR *filename; 
    INT32 f;
    INT32 ret; 
    FS_LodPacket packet;
    //MEMD_ERR_T result;
    UINT32 flash_pattern;
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct;
    FS_LodHeader lod_header; 

    hal_HstSendEvent(BOOT_EVENT, 0x92ade010);
    hal_HstSendEvent(BOOT_EVENT, disk);
    hal_HstSendEvent(BOOT_EVENT, file);


    CSW_TRACE(BASE_FFS_TS_ID,  "Enter DSM_TFlash_Upgrade, disk=%d, file=%d",disk, file);
    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();

    if(DSM_UpgradeStructGetValid())
    {
        if(pUpgradeStruct->step == 2)
        {
            DSM_Program_Fist_Part_Bin();
        }
        FS_Close(file);
        return;
    }
    
    //g_DsmTflashUpgradeStruct.system_lod_file = 0;
    g_DsmTflashUpgradeStruct.system_bin_file = 0;
    g_DsmTflashUpgradeStruct.clearUserData = 0;
    g_DsmTflashUpgradeStruct.system_cfp_file = 0;

    do{
        if(FS_GetLine(buff, T_UPDATE_MAX_LINE, file)==NULL)
            break;
        CSW_TRACE(BASE_FFS_TS_ID,  "DSM_TFlash_Upgrade read cfg file, read line:%s",buff);
        
        if(strncmp(buff, T_UPDATE_KEY, strlen(T_UPDATE_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_KEY)+1, strlen(buff)-strlen(T_UPDATE_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                ret = FS_GetFileINo(f, &g_DsmTflashUpgradeStruct.system_lod_file);
                FS_Close(f);               
            }

            COS_Free(filename);
            continue; 
        }
        
        if(strncmp(buff, T_UPDATE_CFP_KEY, strlen(T_UPDATE_CFP_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_CFP_KEY)+1, strlen(buff)-strlen(T_UPDATE_CFP_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                FS_GetFileINo(f, &g_DsmTflashUpgradeStruct.system_cfp_file);
                FS_Close(f);
            }
            COS_Free(filename);
            continue; 
        }

        #if 0
        if(strncmp(buff, T_UPDATE_PROG_KEY, strlen(T_UPDATE_PROG_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_PROG_KEY)+1, strlen(buff)-strlen(T_UPDATE_PROG_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                program_file = f;
            }
            COS_Free(filename);
        }
        #endif
        if(strncmp(buff, T_CLEAR_USER_DATA_KEY, strlen(T_CLEAR_USER_DATA_KEY))==0)
        {
            if(strncmp(buff+strlen(T_CLEAR_USER_DATA_KEY)+1, "YES", 3) == 0)
                g_DsmTflashUpgradeStruct.clearUserData = 1;

            continue; 
        }

        // Bin support 
        if(strncmp(buff, T_UPDATE_BIN_KEY, strlen(T_UPDATE_BIN_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_BIN_KEY)+1, strlen(buff)-strlen(T_UPDATE_BIN_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                ret = FS_GetFileINo(f, &g_DsmTflashUpgradeStruct.system_bin_file);
                FS_Close(f);
            }
            COS_Free(filename);
            continue; 
        }

        if(strncmp(buff, T_UPDATE_PROG_BIN_KEY, strlen(T_UPDATE_PROG_BIN_KEY))==0)
        {
            ML_LocalLanguage2UnicodeBigEnding(buff+strlen(T_UPDATE_PROG_BIN_KEY)+1, strlen(buff)-strlen(T_UPDATE_PROG_BIN_KEY)-1, (UINT8**)&filename, &res, NULL);
            f = FS_Open((PCSTR)filename, FS_O_RDONLY, 0);
            if(f >= 0)
            {
                program_bin_file = f;
            }
            COS_Free(filename);
            continue; 
        }
                
    }while(1);

    hal_HstSendEvent(BOOT_EVENT, 0x92ade050);
    //hal_HstSendEvent(BOOT_EVENT, program_file);
    hal_HstSendEvent(BOOT_EVENT, program_bin_file);
    hal_HstSendEvent(BOOT_EVENT, g_DsmTflashUpgradeStruct.system_lod_file);
    hal_HstSendEvent(BOOT_EVENT, g_DsmTflashUpgradeStruct.system_bin_file);

    if(program_bin_file >= 0)
    {
        if(g_DsmTflashUpgradeStruct.system_bin_file == 0)
        {
            FS_Close(program_bin_file);
            return;
        }
    }
    else
        return;
    
    // start program
    hal_HstSendEvent(BOOT_EVENT, 0x92ade100);
    //TODO:  add upgrade start notify here

#if APP_SUPPORT_BLUETOOTH==1
    hal_BtStop();
#endif    
    hal_TimWatchDogClose();

    FS_GetLodHeader(g_DsmTflashUpgradeStruct.system_lod_file, &lod_header);
    
    //CSW_TRACE(BASE_FFS_TS_ID,  "DSM_TFlash_Upgrade start program, program_file=%d, system_lod_file=%x",program_file, g_DsmTflashUpgradeStruct.system_lod_file);
    int addr = lod_header.base, i = 0; 
    int blocks = (INT32)FS_GetFileSize(program_bin_file)/T_UPDATE_BUF_BYTE_STEP_1; 
    int left = (INT32)FS_GetFileSize(program_bin_file)%T_UPDATE_BUF_BYTE_STEP_1; 
    packet.words = (UINT32 *)bigbuf; 
    
    hal_HstSendEvent(BOOT_EVENT, blocks);
    hal_HstSendEvent(BOOT_EVENT, left);

    
 
    for ( i = 0; i < blocks; i++ )
    {
        packet.address = addr; //flash start address  
        packet.nbWords = T_UPDATE_BUF_DWORD_STEP_1; 
        packet.nbBytes = T_UPDATE_BUF_BYTE_STEP_1; 
        FS_Read(program_bin_file, (UINT8*)packet.words, packet.nbBytes); 
        DSM_Program_Packet(&packet); 
        addr += T_UPDATE_BUF_BYTE_STEP_1; 
    }

    if (left > 0)
    {
        packet.address = addr; //flash start address  
        packet.nbWords = left>>2; 
        packet.nbBytes = left; 
        FS_Read(program_bin_file, (UINT8*)packet.words, packet.nbBytes); 
        DSM_Program_Packet(&packet); 
    }  
    
    FS_Close(program_bin_file);

    // jump to program start
    g_DsmTflashUpgradeStruct.deviceType = disk;
    g_DsmTflashUpgradeStruct.step = 1;
    g_DsmTflashUpgradeStruct.validTag = BOOT_TFLASH_UPGRADE_VALID_TAG;
    g_DsmTflashUpgradeStruct.checkSum = ~BOOT_TFLASH_UPGRADE_VALID_TAG;
    
    *boot_BootSectorGetTflashUpgradeStructPointer() = &g_DsmTflashUpgradeStruct;

    hal_HstSendEvent(BOOT_EVENT, 0x92ade160);
    //hal_HstSendEvent(BOOT_EVENT, g_DsmTflashUpgradeStruct);
    hal_HstSendEvent(BOOT_EVENT, (UINT32)&g_DsmTflashUpgradeStruct);
    flash_pattern = BOOT_FLASH_PROGRAMMED_PATTERN;
    memd_FlashWrite((UINT8*)0, 4, &res, (UINT8*)&flash_pattern);
    flash_pattern = 0;
    memd_FlashWrite((UINT8*)4, 4, &res, (UINT8*)&flash_pattern);

    hal_SysRestart();
}



VOID DSM_TFlash_Upgrade_Step2_Bin()
{
    COS_EVENT ev;
    FS_LodPacket packet;
    //INT32 file;
    INT32 file_bin; 
    UINT32 startpos;
    //MEMD_ERR_T result;
    INT32 card_ok = -1;
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct;
    FS_LodHeader lod_header; 

    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();
   
    hal_HstSendEvent(BOOT_EVENT, 0x92ade220);
    
    hal_HstSendEvent(BOOT_EVENT, (UINT32)pUpgradeStruct);   
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->deviceType);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->system_bin_file);
    
    //hal_TimDelay(50);
    if(pUpgradeStruct->deviceType == FS_DEV_TYPE_USBDISK)
    {
        while(card_ok!=ERR_SUCCESS)
        {
            hal_HstSendEvent(BOOT_EVENT, 0x92ade221);
            COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
            if(ev.nEventId == EV_FS_UDISK_READY)
            {
                COS_Sleep(100);
                card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
                hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
                hal_HstSendEvent(BOOT_EVENT, card_ok);
            }
        }
    }
    else
    {
        while(card_ok!=ERR_SUCCESS)
        {
            hal_HstSendEvent(BOOT_EVENT, 0x92ade221);
            //COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
    }
    
    hal_TimRtcIrqSetMask(FALSE); // disable time clock

    file_bin = FS_OpenDirect(pUpgradeStruct->system_bin_file, FS_O_RDONLY, 0);
 
    hal_HstSendEvent(BOOT_EVENT, 0x92ade230);
    hal_HstSendEvent(BOOT_EVENT, file_bin);
    
    packet.words = (UINT32 *)bigbuf; 

    FS_Read(file_bin, (UINT8*)packet.words, 8*4); 
    
    startpos = packet.words[7]; 
   
    int program_size = (INT32)FS_GetFileSize(file_bin) - (startpos&0xffc000); 
    int blocks = program_size/T_UPDATE_BUF_BYTE; 
    int left_size = program_size%T_UPDATE_BUF_BYTE;
    
    hal_HstSendEvent(BOOT_EVENT, startpos);
    hal_HstSendEvent(BOOT_EVENT, blocks);
    hal_HstSendEvent(BOOT_EVENT, program_size);
    hal_HstSendEvent(BOOT_EVENT, left_size);

    FS_Seek(file_bin, (startpos&0xffc000), FS_SEEK_SET);

    FS_GetLodHeader(pUpgradeStruct->system_lod_file, &lod_header);


    int addr = lod_header.base; 

    addr += (startpos&0xffc000); 

    //UINT32 startaddr = addr; 
    UINT32 endaddr = addr + program_size; 
    UINT32 tmpaddr = addr; 
    UINT32 startFlash, endFlash;
    UINT32 cs; 

    /* The start pos may not 64kb aligned, so you have to erase such space in 
     * 16kb stride, once it's address divisible by 64kb, erase in 64kb block. 
     */
      
    while(tmpaddr < endaddr)
    {
       if (!(tmpaddr % 0x10000))
       {
            startFlash = tmpaddr&0xffffff; 
            endFlash = startFlash + 0x10000; 
            //memd_FlashErase(startFlash, NULL); 
            
            cs = hal_SysEnterCriticalSection();
            spi_flash_block_erase((UINT32)startFlash);
            hal_SysExitCriticalSection(cs);
            
            tmpaddr += 0x10000; 
       }
       else
       {
            startFlash = tmpaddr&0xffffff; 
            endFlash = startFlash + 0x1000; 
            
            cs = hal_SysEnterCriticalSection();
            memd_FlashErase((UINT8*)startFlash, (UINT8*)endFlash); 
            hal_SysExitCriticalSection(cs);
            
            tmpaddr += 0x1000; 
       }
    }    

    for (int i = 0; i < blocks; i++ )
    {
        packet.address = addr; //flash start address  
        packet.nbWords = T_UPDATE_BUF_DWORD; 
        packet.nbBytes = T_UPDATE_BUF_BYTE; 
        FS_Read(file_bin, (UINT8*)packet.words, packet.nbBytes); 
        DSM_Program_Packet_Noerase(&packet); 
        addr += T_UPDATE_BUF_BYTE; 
    }

    if (left_size > 0)
    {
        packet.address = addr; //flash start address  
        packet.nbWords = left_size>>2; 
        packet.nbBytes= left_size; 
        FS_Read(file_bin, (UINT8*)packet.words, packet.nbBytes); 
        DSM_Program_Packet_Noerase(&packet); 
    }
        
    hal_HstSendEvent(BOOT_EVENT, 0x92ade260);
    hal_HstSendEvent(BOOT_EVENT, startpos);
    
    pUpgradeStruct->step = 2;

    boot_IspiSendData(1,0x0b<<16|((12)<<2),FALSE); // enable abb pll

    // program finish, jump to xcpu flash
    {
        VOID(*flashEntry)(UINT32);
        flashEntry = (VOID(*)(UINT32)) startpos;
        flashEntry(BOOT_SECTOR_STANDARD_MAIN);
    }
}


VOID  DSM_Program_Fist_Part_Bin()
{
    FS_LodPacket packet;
    UINT32 flash_pattern;
    UINT32 endpos;
    COS_EVENT ev;
    UINT32 byte_size;
    INT32 file; 
    INT32 file_bin; 
    INT32 card_ok = -1;
    //MEMD_ERR_T result;
    int addr; 
    int ret = 0;
    FS_LodHeader lod_header; 
    
    
    BOOT_TFLASH_UPGRADE_STRUCT_T *pUpgradeStruct = NULL;

    pUpgradeStruct = *boot_BootSectorGetTflashUpgradeStructPointer();

    while(card_ok!=ERR_SUCCESS)
    {
        COS_WaitEvent(MOD_APP, &ev, COS_WAIT_FOREVER);
        if(ev.nEventId == EV_FS_UDISK_READY)
        {
            COS_Sleep(100);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade222);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
        else
        {
            COS_Sleep(100);
            card_ok = FS_MountDevice(pUpgradeStruct->deviceType);
            hal_HstSendEvent(BOOT_EVENT, 0x92ade223);
            hal_HstSendEvent(BOOT_EVENT, card_ok);
        }
    }
    
    hal_TimRtcIrqSetMask(FALSE); // disable time clock

    packet.words = (UINT32 *)bigbuf; 

    
    FS_GetLodHeader(pUpgradeStruct->system_lod_file, &lod_header);

    file_bin = FS_OpenDirect(pUpgradeStruct->system_bin_file, FS_O_RDONLY, 0);

    hal_HstSendEvent(BOOT_EVENT, 0x92ade300);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->system_lod_file);
    hal_HstSendEvent(BOOT_EVENT, pUpgradeStruct->system_bin_file);
    hal_HstSendEvent(BOOT_EVENT, file_bin);

    if(pUpgradeStruct->clearUserData)
    {
        FS_LodGetUserLodPacket(pUpgradeStruct->system_lod_file, &packet);
        hal_HstSendEvent(BOOT_EVENT, 0x92ade400);
        hal_HstSendEvent(BOOT_EVENT, packet.address);
        hal_HstSendEvent(BOOT_EVENT, packet.nbWords);

        UINT32 cs = hal_SysEnterCriticalSection();
        memd_FlashErase2((UINT8*)(packet.address&0xffffff), (UINT8*)((packet.address&0xffffff)+packet.nbWords*4));
        hal_SysExitCriticalSection(cs);
    }
 

    FS_Read(file_bin, (UINT8*)packet.words, 8*4); 
    endpos = packet.words[7];
   
    int blocks = (endpos&0xffffff)/T_UPDATE_BUF_BYTE_STEP_1; 
    int left = (endpos&0xffffff)%T_UPDATE_BUF_BYTE_STEP_1; 
    
    FS_Seek(file_bin, 0, FS_SEEK_SET);

    addr = lod_header.base; 

    //UINT32 startaddr = addr; 
    UINT32 endaddr = addr + (0xffc000&endpos); 
    UINT32 tmpaddr = addr; 
    UINT32 startFlash, endFlash; 
    
    UINT32 cs; 
    
    /*  First erase all space to program. As the last flash programming 
     *  starts from 0x08000000, so first erase in 64kb stride, and then 
     *  erase the left space which is less than 64kb (always 16kb aligned). 
     */
    while(tmpaddr + 0x10000 <= endaddr)
    {
       if (!(tmpaddr % 0x10000))
       {
           startFlash = tmpaddr&0xffffff; 

           cs = hal_SysEnterCriticalSection();
           // memd_FlashErase(tmpaddr&0xffffff, tmpaddr&0xffffff + 0x10000); 
           //spi_flash_block_erase2((UINT32)startFlash);
           spi_flash_block_erase((UINT32)startFlash);
           hal_SysExitCriticalSection(cs);

           tmpaddr += 0x10000;            
       }
    }
    
    while (tmpaddr + 0x1000 <= endaddr)
    {
       startFlash = tmpaddr&0xffffff; 
       endFlash = startFlash + 0x1000;       
       
       cs = hal_SysEnterCriticalSection();
       memd_FlashErase2((UINT8*)startFlash, (UINT8*)endFlash);
       hal_SysExitCriticalSection(cs);
       
       tmpaddr += 0x1000; 
    }
      
    for (int i = 0; i < blocks; i ++)
    {        
        packet.address = addr; //flash start address  
        packet.nbWords = T_UPDATE_BUF_DWORD_STEP_1; 
        packet.nbBytes= T_UPDATE_BUF_BYTE_STEP_1; 
        ret = FS_Read(file_bin, (UINT8*)packet.words, packet.nbBytes); 
        DSM_Program_Packet_Noerase(&packet); 
        addr += T_UPDATE_BUF_BYTE_STEP_1; 
    }

    if (left > 0)
    {        
        packet.address = addr; //flash start address  
        packet.nbWords = left>>2; 
        packet.nbBytes= left; 
        ret = FS_Read(file_bin, (UINT8*)packet.words, left); 
        DSM_Program_Packet_Noerase(&packet); 
    }
    
    FS_Close(file_bin);

    if(pUpgradeStruct->system_cfp_file != 0)
    {
        file = FS_OpenDirect(pUpgradeStruct->system_cfp_file, FS_O_RDONLY, 0);
        hal_HstSendEvent(BOOT_EVENT, 0x92ade450);

        FS_GetLine(NULL, 0, file); // reset line buffer
        FS_LodGetNextPacket(file, &packet);
        DSM_Program_Packet(&packet);
        FS_Close(file);
    }    

    pUpgradeStruct->step = 3;

    // end of program
    hal_HstSendEvent(BOOT_EVENT, 0x92ade600);
    flash_pattern = BOOT_FLASH_PROGRAMMED_PATTERN;
    memd_FlashWrite((UINT8*)0, 4, &byte_size, (UINT8*)&flash_pattern);
    flash_pattern = 0;
    memd_FlashWrite((UINT8*)4, 4, &byte_size, (UINT8*)&flash_pattern);

    //TODO:  add upgrade success notify here

    hal_SysRestart();
}

#endif // SERVICE_BASE_FS_SUPPORT

