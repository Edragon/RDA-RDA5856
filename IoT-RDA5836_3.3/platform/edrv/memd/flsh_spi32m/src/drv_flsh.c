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
/// @file drv_flsh.c                                                          //
/// That file provides the driver implementation for the SPI      //
/// flash.                                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "hal_ebc.h"
#include "hal_sys.h"
//#include "sxs_io.h"
#include "string.h"
#include "memd_m.h"
#include "memdp_debug.h"
#include "memdp.h"
#include "hal_host.h"
#include "hal_timers.h"
#include "hal_spi_flash.h"

// =============================================================================
//  MACROS
// =============================================================================

#define OPCODE_WRITE_ENABLE 0x06
#define OPCODE_READ_STATUS 0x05
#define OPCODE_WRITE_STATUS 0x01
#define OPCODE_READ_STATUS_1 0x00
//0x32: cmd and addr need use one line to be send, data need use 4 lines to be send
//0x38: cmd need use one line to be send, addr and data need use 4 lines to be send
#define OPCODE_QUAD_PAGE_PROGRAM 0x32
#define OPCODE_BLOCK_ERASE_64K 0xd8
#define OPCODE_SECTOR_ERASE 0x20
#define OPCODE_CHIP_ERASE 0x60
#define OPCODE_READ_DATA 0x03
#define OPCODE_QUAD_FAST_READ 0xeb


#define OPCODE_SR_WRITE_ENABLE 0x50
#define OPCODE_RDSR 0x05
#define OPCODE_WRR 0x01
//#define OPCODE_RCR 0x35
#define OPCODE_QPP 0x32
#define OPCODE_SE 0xd8
#define OPCODE_P4E 0x20
#define OPCODE_BE 0x60
#define OPCODE_READ 0x03
#define OPCODE_QIQR 0xeb
//for test OPCODE_HIGH_P_MODE
//#define OPCODE_HIGH_P_MODE 0xff
#define OPCODE_RDI 0xab
#define OPCODE_DP 0xb9
#if (CHIP_ASIC_ID > CHIP_ASIC_ID_5855)
#define OPCODE_HIGH_P_MODE 0x00 //NOP
#else
#define OPCODE_HIGH_P_MODE 0xa3
#endif
#define OPCODE_ReadReset 0xff
#define OPCODE_PROGRAM_ERASE_SUSPEND 0xb0
#define OPCODE_PER 0x30

//#define OPCODE_BLOCK_ERASE_64K      0x20    // sector erase

#define OPCODE_REMS    0x90    // read manufacture/device ID 
#define OPCODE_RDID    0x9f    // read identification
#define OPCODE_BE_1    0x52    // 32KB block erase
#define OPCODE_BE_2    0xd8    // 64kB block erase
#define SECTOR_SIZE 0x10000

#define OPCODE_ENQPI  0x35
#define OPCODE_DISABLEQPI   0xf5
#define OPCODE_ENRST  0x66
#define OPCODE_RST      0x99

/*****************************************************************/
//  FLASH_TYPE_ID = 0 : normal flash
//  FLASH_TYPE_ID = 1 : x25u3235e
//  FLASH_TYPE_ID = 2 : en25q32
/*****************************************************************/
#define FLASH_TYPE_ID   0
#define TOTAL_FLASH_ID  3

//#define PAGE_SIZE ((OPCODE_QUAD_PAGE_PROGRAM == 0x38)?128:256)

// Get the page number from the _physical_ flash address
#define GET_PAGE_PHYS(PhysAddr)  (UINT8)(((UINT32)PhysAddr >> 22) & 0x7)

#define IS_QUARD_CMD_EN (HAL_SPI_FLASH_CMD_QUARD_MODE & p_flsh_config.cmdMode)

#define memd_FlashWaitWriteInProgress(flash_addr) while(memd_FlashWriteInProgress(flash_addr))

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
/// Base address
UINT32 g_memdFlashBaseAddress = 0;


MEMD_SPIFLSH_RDID_T g_spiflash_ID;

UINT32 g_spiflash_pagesize = 256;


PRIVATE HAL_SPI_FLASH_CFG_T p_flsh_config;




UINT8 g_dif_opcode_high_p_mode = OPCODE_HIGH_P_MODE;
UINT8 g_dif_opcode_suspend = OPCODE_PROGRAM_ERASE_SUSPEND;
UINT8 g_dif_opcode_resume = OPCODE_PER;
UINT8 g_dif_opcode_enable_QPI = OPCODE_ENQPI;
UINT8 g_dif_opcode_disable_QPI = OPCODE_DISABLEQPI;
UINT8 g_dif_opcode_enable_RST = OPCODE_ENRST;
UINT8 g_dif_opcode_execute_RST = OPCODE_RST;
UINT8 g_dif_opcode_write = OPCODE_QUAD_PAGE_PROGRAM;

UINT32        RUN_Status4=0;
UINT32        RUN_Status=0;
UINT32        RUN_Status2=0;
UINT32        RUN_Status3=0;
// =============================================================================
//  FUNCTIONS
// =============================================================================

PUBLIC BOOL memd_FlashWriteInProgress(UINT32 flash_addr)
{
    UINT8 opmode;
    UINT32 flash_status = 0;

    if(IS_QUARD_CMD_EN)
        opmode = 2;
    else if(FLASH_TYPE_ID)
        opmode = 1;
    else
        opmode = 0;

    hal_SpiFlashClearFifoData(TRUE, FALSE);
    hal_SpiFlashSendCmd(OPCODE_READ_STATUS, flash_addr, opmode, 1);
    hal_SpiFlashGetRxData((UINT8*)&flash_status, 1);

    return (flash_status & 0x1);
}

PUBLIC UINT32 get_flash_status_h(UINT32 flash_addr)
{
    UINT8 opmode;
    UINT32 flash_status;

    if(IS_QUARD_CMD_EN)
        opmode = 2;
    else if(FLASH_TYPE_ID)
        opmode = 1;
    else
        opmode = 0;

    hal_SpiFlashClearFifoData(TRUE, FALSE);
    hal_SpiFlashSendCmd(OPCODE_READ_STATUS_1, flash_addr, opmode, 1);
    hal_SpiFlashGetRxData((UINT8*)&flash_status, 1);
    return flash_status;
}



PUBLIC VOID get_flash_ID(UINT32 flash_addr)
{
    UINT8 opmode;
    BOOL quard_flag;
    UINT8 data_array[3];

    if(FLASH_TYPE_ID==0)
    {
        opmode = 0;
    }
    else if(FLASH_TYPE_ID>=TOTAL_FLASH_ID)
    {
        opmode = 3;
    }
    else
    {
        opmode = 3;
    }

    hal_SpiFlashClearFifoData(TRUE, TRUE);
    hal_SpiFlashSendCmd(OPCODE_RDID, flash_addr, opmode, 3);
    hal_SpiFlashGetRxData(data_array, 3);

    g_spiflash_ID.manufacturerID = data_array[0];
    g_spiflash_ID.device_memory_type_ID = data_array[1];
    g_spiflash_ID.device_capacity_type_ID = data_array[2];

}

PUBLIC void write_flash_status(UINT32 flash_addr, UINT8 status_val)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    BOOL quard_flag;
    UINT8 data_array[2];
    UINT8 data_size;

    if(FLASH_TYPE_ID==0)
    {
        if((g_spiflash_ID.manufacturerID == MEMD_FLASH_MXIC)||(g_spiflash_ID.manufacturerID ==MEMD_FLASH_ESMT))
        {
            opmode = 0;
            data_array[0] = status_val;
            data_array[1] = 0x0;
            data_size = 1;
        }
        else
        {
            opmode = 0;
            data_array[0] = 0x0;
            data_array[1] = status_val;
            data_size = 2;
        }
    }
    else if(FLASH_TYPE_ID>=TOTAL_FLASH_ID)
    {
        opmode = 3;
        data_array[1] = status_val;
        data_array[0] = 0x0;
        data_size = 2;
    }
    else
    {
        opmode = 3;
        data_array[1] = 0x0;
        data_array[0] = status_val;
        data_size = 1;
    }

    if(IS_QUARD_CMD_EN)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;


    hal_SpiFlashClearFifoData(TRUE, TRUE);

    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    hal_SpiFlashPushTxFifoData(data_array,data_size,quard_flag);
    hal_SpiFlashSendCmd(OPCODE_WRITE_STATUS, flash_addr, opmode, 0);

    memd_FlashWaitWriteInProgress(flash_addr);

    return ;
}

PUBLIC BOOL spi_flash_quard_ctrl(BOOL quard_en_flag, UINT8 dual_mode)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 qmode_cmd;

    UINT8 opmode;
    UINT8 status_val;
    UINT32 flash_addr;


    if(FLASH_TYPE_ID)
        opmode = 3;
    else
        opmode = 0;

    if(quard_en_flag&(FLASH_TYPE_ID ==1))
        status_val = 0x40;
    else if(FLASH_TYPE_ID ==2)
        status_val = 0x0;
    else if(quard_en_flag)
    {
        if((g_spiflash_ID.manufacturerID == MEMD_FLASH_MXIC)||(g_spiflash_ID.manufacturerID ==MEMD_FLASH_ESMT))
            status_val = 0x40;
        else
            status_val = 0x02;
    }
    else
        status_val = 0x0;

    if(dual_mode == 1)
        flash_addr = 0x400000;
    else if(dual_mode == 3)
        flash_addr = 0x800000;
    else
        flash_addr = 0x0;
    if(g_spiflash_ID.manufacturerID == MEMD_FLASH_EON)
    {

    }
    else
    {
        write_flash_status(0,status_val);
        if(dual_mode)
            write_flash_status(flash_addr,status_val);
    }

    if(FLASH_TYPE_ID == 0x2)
    {
        if(quard_en_flag)
            qmode_cmd = 0x38;
        else
            qmode_cmd = 0xff;

        hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
        hal_SpiFlashSendCmd(qmode_cmd,flash_addr,opmode,0);

        if(dual_mode)
        {
            hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
            hal_SpiFlashSendCmd(qmode_cmd,flash_addr,opmode,0);
        }
    }
    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);


    if(quard_en_flag)
        p_flsh_config.readMode = HAL_SPI_FLASH_READ_QUARD_MODE;

    if((FLASH_TYPE_ID == 2)&quard_en_flag)
        p_flsh_config.cmdMode = HAL_SPI_FLASH_CMD_QUARD_MODE;

    hal_SpiFlashInit(&p_flsh_config);

    return TRUE;
}



#ifdef SPI_FLASH_CONTINUOUS_READ
PUBLIC VOID spi_flash_open_continuous_read(VOID)
{
    hal_SpiFlashContinuousMode(TRUE);
}

PUBLIC VOID spi_flash_close_continuous_read(VOID)
{
    hal_SpiFlashContinuousMode(FALSE);
}

#endif

UINT32 NOT_SUSPEND;

PUBLIC BOOL spi_flash_block_erase(UINT32 flash_addr)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;

    RUN_Status=2;
    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    opmode = 0;


    NOT_SUSPEND=0;


    addr_tmp = flash_addr&0xffffff;
    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);
    hal_SpiFlashSendCmd(OPCODE_BLOCK_ERASE_64K, addr_tmp, opmode,0);
    memd_FlashWaitWriteInProgress(addr_tmp);


    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
    hal_SpiFlashEnableRead();

    return TRUE;
}


PUBLIC MEMD_ERR_T spi_flash_sector_erase_time(UINT32 flash_addr,UINT32 time)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;


    UINT32 suspend_time = 0,max_time = 0;
    max_time = time MILLI_SECONDS;
    UINT32 now = hal_TimGetUpTime();
    RUN_Status=2;
    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    opmode = 0;


    NOT_SUSPEND=0;


    addr_tmp = flash_addr&0xffffff;
    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);
    hal_SpiFlashSendCmd(OPCODE_SECTOR_ERASE,addr_tmp,opmode,0);
    do
    {
        suspend_time = hal_TimGetUpTime();
        if (suspend_time - now > max_time)
        {
            hal_SpiFlashSendCmd(g_dif_opcode_suspend,addr_tmp,3,0);
            memd_FlashWaitWriteInProgress(addr_tmp);
            hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
            hal_SpiFlashEnableRead();
            return MEMD_ERR_SUSPEND;
        }
    }
    while(memd_FlashWriteInProgress(addr_tmp));


    RUN_Status4 = suspend_time - now;
    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
    hal_SpiFlashEnableRead();

    return MEMD_ERR_NO;
}

PUBLIC MEMD_ERR_T spi_flash_sector_erase_nosuspend(UINT32 flash_addr)
{
    UINT32 addr_tmp;
    UINT8 opmode;

    RUN_Status=2;
    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    opmode = 0;

    NOT_SUSPEND=0;

    addr_tmp = flash_addr&0xffffff;
    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, opmode, 0);
    hal_SpiFlashSendCmd(OPCODE_SECTOR_ERASE,addr_tmp,opmode,0);

    memd_FlashWaitWriteInProgress(addr_tmp);

    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
    hal_SpiFlashEnableRead();

    return MEMD_ERR_NO;
}


PUBLIC BOOL spi_flash_sector_erase(UINT32 flash_addr)
{
    spi_flash_sector_erase_nosuspend(flash_addr);
    return TRUE;
}

PUBLIC MEMD_ERR_T spi_flash_sector_erase_resume(UINT32 flash_addr,UINT32 time)
{
    UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;


    UINT32 suspend_time = 0,max_time = 0;
    max_time = time MILLI_SECONDS;
    UINT32 now = hal_TimGetUpTime();

    opmode = 0;

    hal_SpiFlashDisableRead();
    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    NOT_SUSPEND=0;


    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);
    addr_tmp = flash_addr&0xffffff;
    hal_SpiFlashSendCmd(g_dif_opcode_resume,addr_tmp,3,0);

    do
    {
        suspend_time = hal_TimGetUpTime();
        if (suspend_time - now > max_time)
        {
            hal_SpiFlashSendCmd(g_dif_opcode_suspend,addr_tmp,3,0);
            memd_FlashWaitWriteInProgress(flash_addr);
            hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
            hal_SpiFlashEnableRead();
            return MEMD_ERR_SUSPEND;
        }
    }
    while(memd_FlashWriteInProgress(flash_addr));

    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, 0, 0);
    hal_SpiFlashEnableRead();
    return MEMD_ERR_NO;
}


PUBLIC BOOL spi_flash_chip_erase(UINT32 flash_addr)
{
    while(1);

    return TRUE;
}

PUBLIC BOOL spi_flash_write(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 addr_array[3];
    UINT8 opmode;
    BOOL quard_flag;
    BOOL addr_quard_flag;
    UINT8 write_cmd;

    RUN_Status=1;

    if(!data_size)
        return TRUE;

    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    if(IS_QUARD_CMD_EN)
        quard_flag = TRUE;
    else
        quard_flag = FALSE;

    write_cmd = g_dif_opcode_write;

    if( quard_flag == FALSE)
        write_cmd = 0x02;

    if(write_cmd == 0x38)
    {
        opmode = 3;
        addr_quard_flag = TRUE;
    }
    else
    {
        opmode = 0;
        addr_quard_flag = FALSE;
    }

    addr_tmp = flash_addr&0xffffff;

	hal_SpiFlashClearFifoData(FALSE, TRUE);
    if(write_cmd == 0x38)
    {
        addr_array[2] = (UINT8)(addr_tmp&0xff);
        addr_array[1] = (UINT8)((addr_tmp>>8)&0xff);
        addr_array[0] = (UINT8)((addr_tmp>>16)&0xff);
        hal_SpiFlashPushTxFifoData(addr_array, 3, addr_quard_flag);
    }

    hal_SpiFlashPushTxFifoData(data_array,data_size,quard_flag);
    addr_tmp = flash_addr&0xffffff;
    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, 0, 0);
    hal_SpiFlashSendCmd(write_cmd, addr_tmp,opmode,0);
    memd_FlashWaitWriteInProgress(addr_tmp);

    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, opmode, 0);
    hal_SpiFlashEnableRead();

    return TRUE;
}

#if 1
PUBLIC void write_flash_security_REG_lock(UINT32 flash_addr)
{
    UINT8 data_tmp;

    data_tmp = get_flash_status_h(0) & 0xff;

    if(((flash_addr >> 12) & 0x3) == 1)
        data_tmp |= 0x8;
    else if(((flash_addr >> 12) & 0x3) == 2)
        data_tmp |= 0x10;
    else if(((flash_addr >> 12) & 0x3) == 3)
        data_tmp |= 0x20;

    write_flash_status(flash_addr, data_tmp);

    return ;
}

PUBLIC void flash_security_REG_read(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size)
{
    UINT8 addr_array[4];

    addr_array[2] = (UINT8)(flash_addr & 0xff);
    addr_array[1] = (UINT8)((flash_addr >> 8) & 0xff);
    addr_array[0] = (UINT8)((flash_addr >> 16) & 0xff);
    addr_array[3] = 0x0;

    //one line extend read
    hal_SpiFlashClearFifoData(TRUE,TRUE);
    hal_SpiFlashPushTxFifoData(addr_array, 4, FALSE);
    hal_SpiFlashSendCmd(0x48, flash_addr, 1, data_size);
    hal_SpiFlashGetRxData(data_array, data_size);

    return;
}

PUBLIC MEMD_ERR_T spi_flash_security_REG_erase(UINT32 flash_addr)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 opmode;
    UINT8 addr_array[3];

    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);

    opmode = 0;

    addr_tmp = flash_addr&0xffffff;
    addr_array[2] = (UINT8)(addr_tmp & 0xff);
    addr_array[1] = (UINT8)((addr_tmp >> 8) & 0xff);
    addr_array[0] = (UINT8)((addr_tmp >> 16) & 0xff);

    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, 0, 0);

    //one line extend read
	hal_SpiFlashClearFifoData(FALSE, TRUE);
    hal_SpiFlashPushTxFifoData(addr_array, 3, FALSE);

    hal_SpiFlashSendCmd(0x44, addr_tmp, 3, 0);

    memd_FlashWaitWriteInProgress(addr_tmp);

    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, addr_tmp, opmode, 0);
    hal_SpiFlashEnableRead();

    return MEMD_ERR_NO;
}

PUBLIC BOOL spi_flash_security_REG_write(UINT32 flash_addr, UINT8 data_array[], UINT32 data_size)
{
    volatile UINT32 data_tmp_32;
    UINT32 addr_tmp;
    UINT8 addr_array[3];

    if(!data_size)
        return TRUE;

    hal_SpiFlashDisableRead();

    hal_SpiFlashSendCmd(OPCODE_ReadReset, flash_addr, 0, 0);
	hal_SpiFlashClearFifoData(FALSE, TRUE);
    addr_tmp = flash_addr & 0xffffff;
    addr_array[2] = (UINT8)(addr_tmp & 0xff);
    addr_array[1] = (UINT8)((addr_tmp >> 8) & 0xff);
    addr_array[0] = (UINT8)((addr_tmp >> 16) & 0xff);
    hal_SpiFlashPushTxFifoData(addr_array, 3, FALSE);
    hal_SpiFlashPushTxFifoData(data_array, data_size, FALSE);

    addr_tmp = flash_addr & 0xffffff;
    hal_SpiFlashSendCmd(OPCODE_WRITE_ENABLE, addr_tmp, 0, 0);
    hal_SpiFlashSendCmd(0X42, addr_tmp, 3, 0);
    memd_FlashWaitWriteInProgress(addr_tmp);

    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, flash_addr, 0, 0);
    hal_SpiFlashEnableRead();

    return TRUE;
}

PUBLIC MEMD_ERR_T memd_Flash_security_REG_Erase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    UINT8 addr_tmp = ((UINT32)startFlashAddress >> 12) & 3;
    UINT8 addr_tmp_end = ((UINT32)endFlashAddress >> 12) & 3;
    MEMD_ERR_T status = MEMD_ERR_NO;
    UINT32 temp_erase_addr = (UINT32)startFlashAddress;

    if(endFlashAddress == NULL) addr_tmp_end = addr_tmp;

    if(addr_tmp <= 0 || addr_tmp > 3) return MEMD_ERR_ERASE;
    if(addr_tmp_end <= 0 || addr_tmp_end > 3) return MEMD_ERR_ERASE;
    if(addr_tmp > addr_tmp_end)return MEMD_ERR_ERASE;

    while(addr_tmp <= addr_tmp_end)
    {
        status |= spi_flash_security_REG_erase(temp_erase_addr);
        addr_tmp += 1;
        temp_erase_addr += 0x1000;
    }
    return status;
}

PUBLIC MEMD_ERR_T memd_Flash_security_REG_Read(UINT8 *flashAddress, UINT32 byteSize, UINT8*  buffer)
{
    flash_security_REG_read((UINT32)flashAddress, buffer, byteSize);
    return MEMD_ERR_NO;
}

PUBLIC MEMD_ERR_T memd_Flash_security_REG_Write(UINT8 *flashAddress,
        UINT32 byteSize,
        UINT32 * pWrittenByteSize,
        CONST UINT8* buffer)
{
    UINT32 write_addr_tmp = (UINT32)flashAddress;
    UINT8 *buffer_ptr;

    if(byteSize > 256) return MEMD_ERR_WRITE;

    buffer_ptr = (UINT8*)buffer;

    if(byteSize > 128)
    {
        spi_flash_security_REG_write(write_addr_tmp, buffer_ptr, 128);
        byteSize -= 128;
        write_addr_tmp +=128;
        buffer_ptr +=128;
    }

    spi_flash_security_REG_write(write_addr_tmp, buffer_ptr, byteSize);

    return MEMD_ERR_NO;
}

PUBLIC VOID memd_Flash_security_REG_Lock(UINT8 *flashAddress)
{
    write_flash_security_REG_lock((UINT32)flashAddress);
}
#endif


// =============================================================================

// =============================================================================
PUBLIC MEMD_FLASH_LAYOUT_T* memd_FlashOpen(CONST MEMD_FLASH_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening flash with a NULL configuration");
    memd_FlashSetLayout();
    memset(&p_flsh_config, 0 ,sizeof(p_flsh_config));

    if(FLASH_TYPE_ID == 0x1)
        p_flsh_config.clkDiv = 0x8;
    else
        p_flsh_config.clkDiv = 0x2;

    p_flsh_config.sampleDelay = 0x1;

    hal_SysSetSpiFlashClock(MEMD_SPIFLASH_FREQ);
    hal_SpiFlashInit(&p_flsh_config);

    get_flash_ID(0);

    spi_flash_quard_ctrl(TRUE,0);

#ifdef SPI_FLASH_CONTINUOUS_READ
    spi_flash_open_continuous_read();
#endif



    g_memdFlashBaseAddress = (UINT32) hal_EbcFlashOpen(HAL_SYS_FREQ_39M, &cfg->csConfig, NULL);
    /*
    > w 0xa1a25000,6
    > w 0xa1a25008,0x12
    > w 0xa1a25008,0x12
    > w 0xa1a25008,0x34
    > w 0xa1a25008,0x56
    > w 0xa1a25008,0x78
    > w 0xa1a25000,0x10002
    */
    hal_HstSendEvent(BOOT_EVENT,0x55555666);
    hal_HstSendEvent(BOOT_EVENT,g_spiflash_ID.manufacturerID);
    hal_HstSendEvent(BOOT_EVENT,g_spiflash_ID.device_memory_type_ID);
    hal_HstSendEvent(BOOT_EVENT,g_spiflash_ID.device_capacity_type_ID);

    if(g_spiflash_ID.manufacturerID == MEMD_FLASH_MXIC)
    {
        g_spiflash_pagesize = 128;
        g_dif_opcode_high_p_mode = 0x00; //NOP
        g_dif_opcode_suspend = 0xb0;
        g_dif_opcode_resume = 0x30;


        g_dif_opcode_enable_QPI = 0x35;
        g_dif_opcode_disable_QPI = 0xf5;
        g_dif_opcode_enable_RST = 0x66;
        g_dif_opcode_execute_RST = 0x99;
    }
    else
    {
        g_spiflash_pagesize = 256;
        g_dif_opcode_high_p_mode = OPCODE_HIGH_P_MODE;
        g_dif_opcode_suspend = 0x75;
        g_dif_opcode_resume = 0x7a;


        g_dif_opcode_enable_QPI = 0x38;
        g_dif_opcode_disable_QPI = 0xff;
        g_dif_opcode_enable_RST = 0x99;
        g_dif_opcode_execute_RST = 0x66;
    }

    if(g_spiflash_ID.manufacturerID == MEMD_FLASH_MXIC)
        g_dif_opcode_write = 0x38;
    else if(g_spiflash_ID.manufacturerID == MEMD_FLASH_FIDELIX && g_spiflash_ID.device_memory_type_ID == MEMD_FLASH_FIDELIX_18V)
        g_dif_opcode_write = 0x02;
    else if(g_spiflash_ID.manufacturerID == MEMD_FLASH_EON || g_spiflash_ID.manufacturerID == MEMD_FLASH_SPANSION || g_spiflash_ID.manufacturerID ==MEMD_FLASH_BERGMICRO)
        g_dif_opcode_write = 0x02;

    hal_HstSendEvent(SYS_EVENT,g_spiflash_pagesize);
    return ((MEMD_FLASH_LAYOUT_T*) &g_memdFlashLayout);
}

PUBLIC VOID memd_FlashClose(VOID)
{
#ifdef SPI_FLASH_CONTINUOUS_READ
    spi_flash_close_continuous_read();
#endif
    return;
}

#ifdef MEMD_RAM_IS_BURST
#ifdef RAM_CLK_IS_52M
#define PSRAM_BCR 0x08590f
#else
#ifdef RAM_CLK_IS_78M
#define PSRAM_BCR 0x08710f
#else
#ifdef RAM_CLK_IS_104M
#define PSRAM_BCR 0x08710f
#else
#ifdef RAM_CLK_IS_125M
#define PSRAM_BCR 0x08410f
#else
#ifdef RAM_CLK_IS_139M
#define PSRAM_BCR 0x08410f
#error sram clk is out of 133MHz.
#else
#error sram clk is out of 133MHz.
#endif //RAM_CLK_IS_139M
#endif //RAM_CLK_IS_125M
#endif //RAM_CLK_IS_104M
#endif //RAM_CLK_IS_78M
#endif//RAM_CLK_IS_52M

PRIVATE VOID HAL_FUNC_INTERNAL
memd_RamConfigureBurstMode(REG16* csBase, REG32* pCRE)
{
    UINT16 read;

    // Using the CR controlled method
    // Configure BCR
    // PSRAM_BCR[15] : 0 = synch / 1 = asynch
    // PSRAM_BCR[14] : 0 = variable Latency / 1 = Fixed latency
    // PSRAM_BCR[13:11] : Latency count 011(3)-111(7) clock cycle could be used
    // PSRAM_BCR[10] : Wait Polarity 0 = Active Low / 1 = Active High
    // PSRAM_BCR[9] : MUST be set to 0
    // PSRAM_BCR[8] : Wait Config 0 = Asserted during wait / 1 = Asserted 1 cycle before wait
    // PSRAM_BCR[7:6] : MUST be set to 0
    // PSRAM_BCR[5:4] : driver strength bits 0 = Full Drive / 1 = 1/2 Drive / 2 = 1/4 Drive
    // PSRAM_BCR[3] : 0 = Wrap / 1 = No wrap
    // PSRAM_BCR[2:0] : Burst len 001 = 4 words / 010 = 8 words / 011 = 16 words / 100 = 32 words / 111 Continuous
    read=*(csBase+PSRAM_BCR);


    *pCRE=1; // MEM_FIFO_CRE
    *(csBase+PSRAM_BCR)=read; // read burst with latency = 3 fixed (4 clocks)

    {
        UINT32 flush __attribute__((unused)) = *pCRE;
    }

    *pCRE=0;

}
#endif //!MEMD_RAM_IS_BURST


// =============================================================================
// memd_RamOpen
// -----------------------------------------------------------------------------
/// That function configures the RAM.
/// It must be called to configure the best settings for the RAM, thus enabling
/// the fatest access.
/// @param cfg Ram configuration used to open the RAM.
///
// =============================================================================
PUBLIC VOID MEMD_BOOT_FUNC memd_RamOpen(CONST MEMD_RAM_CONFIG_T* cfg)
{
    MEMD_ASSERT(cfg != NULL, "Opening RAM with a NULL configuration");
    // Open External SRAM
#ifdef MEMD_RAM_IS_BURST
    hal_EbcSramOpen(&cfg->csConfig, memd_RamConfigureBurstMode);
#else //!MEMD_RAM_IS_BURST
    hal_EbcCsOpen(HAL_EBC_SRAM, &cfg->csConfig);
#endif //!MEMD_RAM_IS_BURST
	
/*
[16 14...13]size: 0000: 8Mbit	  0001: 16Mbit	   0010: 32Mbit      0011: 64Mbit  
			    1000:128Mbit     1001:256Mbit
[12]chip num: 0: 1 chip 	       1: 2 chip
*/
	*(volatile unsigned int*)ADDR_SAVING_FLASHID_4_USB = FLASH_1_32MBIT;
}

// =============================================================================

// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase2(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    BOOL ret_flag;

    while(endFlashAddress >startFlashAddress)
    {
        ret_flag = spi_flash_sector_erase((UINT32)startFlashAddress);
        if(!ret_flag) return MEMD_ERR_ERASE;
        startFlashAddress+=0x1000;
    }
//    if(ret_flag) return MEMD_ERR_NO;
    return MEMD_ERR_NO;
}

PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end);

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors.
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC  MEMD_ERR_T memd_FlashErase_First(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT32 status;
    VOLATILE UINT8 * ptr;
//    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;

#ifdef FLASH_DEBUG
    hal_HstSendEvent(SYS_EVENT,0x98765432);
#endif
    phys_start_add = (UINT32)startFlashAddress;

    MEMD_ASSERT(phys_start_add < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0,
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    }
    else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0,
                    "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);

        // Sector Erase command
        status =spi_flash_sector_erase_time((UINT32)ptr,time);
        if(status!=MEMD_ERR_NO)
        {
            return status;
        }

        // note the pooling could be done on data == 0xff also

        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        }
        else
        {
            Start = End;
        }
    }


    // erase done
    return MEMD_ERR_NO;
}

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors.
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC  MEMD_ERR_T memd_FlashErase_Continue(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time)
{
    UINT32 status;
    VOLATILE UINT8 * ptr;

    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;

#ifdef FLASH_DEBUG
    hal_HstSendEvent(SYS_EVENT,0x98765432);
#endif
    phys_start_add = (UINT32)startFlashAddress;

    MEMD_ASSERT(phys_start_add < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0,
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    }
    else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0,
                    "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        // Start should already be aligned to sector boundary, so shouldn't need any more masking
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);

        // Sector Erase command
        if(NOT_SUSPEND)
        {
            status =spi_flash_sector_erase_time((UINT32)ptr,time);
        }
        else
        {
            status =spi_flash_sector_erase_resume((UINT32)ptr,time);
        }
        if(status != MEMD_ERR_ERASE)
        {
            return status;
        }

        // note the pooling could be done on data == 0xff also

        if (End != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(End, &Start, &End);
        }
        else
        {
            Start = End;
        }
    }

//    hal_HstSendEvent(SYS_EVENT,0x89000000|((UINT32)endFlashAddress&0xffffff));

    if(status) return MEMD_ERR_ERASE;

    // erase done
    return MEMD_ERR_NO;
}

// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors.
/// Addresses <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase(UINT8 *startFlashAddress, UINT8 *endFlashAddress)
{
    VOLATILE UINT8 * ptr;
//    VOLATILE UINT16 *BankAddr;
    UINT32 Start, End;
    UINT32 phys_end_add, phys_start_add;
    BOOL ret_flag=TRUE;
    UINT32 u_TempSectorSize;

#ifdef FLASH_DEBUG
    hal_HstSendEvent(SYS_EVENT,0x98765432);
#endif
    phys_start_add = (UINT32)startFlashAddress;

    MEMD_ASSERT(phys_start_add < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

    MEMD_ASSERT((phys_start_add & 0xe0000000) == 0,
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    // Check that start & end addresses are aligned
    if (endFlashAddress == NULL)
    {
        memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
        phys_end_add = End;
    }
    else
    {
        phys_end_add = (UINT32)endFlashAddress;
        MEMD_ASSERT((phys_end_add & 0xe0000000) == 0,
                    "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");
        if (phys_end_add != FLASH_SIZE)
        {
            memd_FlashGetSectorLimits(phys_end_add , &Start, &End);
            if (phys_end_add != Start)
            {
                MEMD_ASSERT(0, "flash erase error 1,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
                return MEMD_ERR_ALIGN;
            }
        }
    }

    u_TempSectorSize = memd_FlashGetSectorLimits(phys_start_add, &Start, &End);
    if (phys_start_add != Start)
    {
        MEMD_ASSERT(0, "flash erase error 2,phys_end_add = 0x%x,start = 0x%x.",phys_end_add,Start);
        return MEMD_ERR_ALIGN;
    }


    while (Start != phys_end_add)
    {
        {
//            BankAddr = (VOLATILE UINT16 *)(g_memdFlashBaseAddress + (Start&FLASHBANK_MASK));
            // Start should already be aligned to sector boundary, so shouldn't need any more masking
            ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + Start);
        }

//        status = hal_SysEnterCriticalSection();
        if(u_TempSectorSize == 0x1000) //sector size is 4K
        {
            // Sector Erase command
            ret_flag |=spi_flash_sector_erase((UINT32)ptr);
        }
        else //sector size is 64K
        {
            //64k block erase
            ret_flag |=spi_flash_block_erase((UINT32)ptr);
        }
//        hal_SysExitCriticalSection(status);


        // note the pooling could be done on data == 0xff also

        if (End != FLASH_SIZE)
        {
            u_TempSectorSize = memd_FlashGetSectorLimits(End, &Start, &End);
        }
        else
        {
            Start = End;
        }
    }


    if(!ret_flag) return MEMD_ERR_ERASE;

    // erase done
    return MEMD_ERR_NO;
}

#ifdef FLASH_DEBUG
#define MCP_DLEN 512
UINT8 buffer_psram[MCP_DLEN];
UINT8 buffer_flash[MCP_DLEN];
#endif
// =============================================================================
// memd_FlashWrite
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashWrite(UINT8 *flashAddress,
                                  UINT32 byteSize,
                                  UINT32 * pWrittenByteSize,
                                  CONST UINT8* buffer)
{
    BOOL ret_flag=TRUE;
    UINT32 i;
    UINT32 times;
    UINT32 remain_bytes;
    UINT32 one_page_bytes;
    UINT32 addr_tmp;
    UINT8* buf_ptr;

    MEMD_ASSERT((UINT32)flashAddress < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

#ifdef FLASH_DEBUG
    UINT8* buf_pds;
    UINT32 byteSize_bak=byteSize;

    hal_HstSendEvent(SYS_EVENT,0x68000000 | ((int)flashAddress&0xffffff));
    flashAddress = g_memdFlashBaseAddress + flashAddress;
    memcpy(buffer_psram, (UINT8 *)buffer, ((byteSize<MCP_DLEN)?byteSize:MCP_DLEN));
    memcpy(buffer_flash, (UINT8 *)flashAddress, ((byteSize<MCP_DLEN)?byteSize:MCP_DLEN));
#else
    flashAddress = g_memdFlashBaseAddress + flashAddress;
#endif

    one_page_bytes = g_spiflash_pagesize;

    addr_tmp = (UINT32)flashAddress;
    buf_ptr = (UINT8*)buffer;
    *pWrittenByteSize = 0;
//    status = hal_SysEnterCriticalSection();

    if((((UINT32)flashAddress & ~(one_page_bytes - 1)) != 0) && (((UINT32)flashAddress + byteSize) >= (((UINT32)flashAddress & ~(one_page_bytes - 1)) + one_page_bytes)))
    {
        remain_bytes = (((UINT32)flashAddress & ~(one_page_bytes - 1)) + one_page_bytes) - ((UINT32)flashAddress);
        byteSize -= remain_bytes;
        if(remain_bytes <= one_page_bytes)
        {
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        }
        else
        {
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, one_page_bytes);
            addr_tmp = addr_tmp + one_page_bytes;
            buf_ptr = buf_ptr + one_page_bytes;
            *pWrittenByteSize += one_page_bytes;

            remain_bytes=remain_bytes-one_page_bytes;
            ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        }
        addr_tmp = addr_tmp + remain_bytes;
        buf_ptr = buf_ptr + remain_bytes;
        *pWrittenByteSize += remain_bytes;

    }
    if(!ret_flag)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_WRITE;
    }
    if(!byteSize)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_NO;
    }

    times = byteSize/one_page_bytes;
    remain_bytes = byteSize%one_page_bytes;


    for(i=0; (i<times)&&ret_flag; i++)
    {
        ret_flag =  spi_flash_write(addr_tmp, buf_ptr, one_page_bytes);
        addr_tmp = addr_tmp + one_page_bytes;
        buf_ptr = buf_ptr + one_page_bytes;
        *pWrittenByteSize += one_page_bytes;

    }
    if(!ret_flag)
    {
//        hal_SysExitCriticalSection(status);
        return MEMD_ERR_WRITE;
    }

    if(remain_bytes!=0)
    {
        ret_flag =  spi_flash_write(addr_tmp, buf_ptr, remain_bytes);
        *pWrittenByteSize += remain_bytes;
    }
    if(!ret_flag)
    {
        return MEMD_ERR_WRITE;
    }
    else
    {
#ifdef FLASH_DEBUG
        {
            buf_pds = (UINT8*)flashAddress;
            buf_ptr = (UINT8*)buffer;

            for(i=0; (i<byteSize_bak); i++)
            {
                if(buf_pds[i] != buf_ptr[i])
                {
                    hal_HstSendEvent(SYS_EVENT, 0x12345678);
                    hal_HstSendEvent(SYS_EVENT, buf_pds[i]);
                    hal_HstSendEvent(SYS_EVENT, buf_ptr[i]|(byteSize_bak<<16));
                    hal_HstSendEvent(SYS_EVENT, (UINT32)(buf_pds)+i);
                    hal_HstSendEvent(SYS_EVENT, (UINT32)buf_ptr+i);
                    ret_flag =0;
                }
                if(i<MCP_DLEN)
                {
                    if(buffer_psram[i] != buf_ptr[i])
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x87654321);
                        hal_HstSendEvent(SYS_EVENT, buffer_psram[i]);
                        hal_HstSendEvent(SYS_EVENT, buf_ptr[i]);
                        hal_HstSendEvent(SYS_EVENT, (UINT32)(buffer_psram)+i);
                        hal_HstSendEvent(SYS_EVENT, (UINT32)buf_ptr+i);
                        ret_flag =0;
                    }
                    if(!ret_flag)
                    {
                        if(buf_pds[i] != buffer_flash[i])
                        {
                            hal_HstSendEvent(SYS_EVENT, 0x12345678);
                            hal_HstSendEvent(SYS_EVENT, buf_pds[i]);
                            hal_HstSendEvent(SYS_EVENT, buffer_flash[i]);
                            hal_HstSendEvent(SYS_EVENT, (UINT32)(buf_pds)+i);
                            hal_HstSendEvent(SYS_EVENT, (UINT32)buffer_flash+i);
                        }
                    }
                }
            }
            if(!ret_flag)
            {
                asm("break 7");
            }
        }
#endif
        return MEMD_ERR_NO;
    }
}



// =============================================================================
// memd_FlashRead
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashRead(UINT8 *flashAddress,UINT32  byteSize,UINT32* pReadByteSize,UINT8*  buffer)
{
    VOLATILE UINT8 * ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;

    MEMD_ASSERT(phys_add < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    if (GET_PAGE_PHYS(phys_add) <= 2)
    {
        ptr = (VOLATILE UINT8 *)(g_memdFlashBaseAddress + phys_add);
    }
    else
    {
        ptr = (VOLATILE UINT8 *)((g_memdFlashBaseAddress + phys_add) - ((GET_PAGE_PHYS(phys_add)-3)*0x400000));
        // Configure Page Register
        hal_EbcConfigRemap((HAL_EBC_FLSH_PHYSADD_T)phys_add);
    }


    /* could do aligned read from flash to improve bus accesses as it is uncached */
    memcpy(buffer, (UINT8 *)ptr, byteSize);
    *pReadByteSize=byteSize;
    return MEMD_ERR_NO;
}

PUBLIC VOLATILE CONST UINT8* memd_FlashGetGlobalAddress(UINT8 *flashAddress)
{
    VOLATILE CONST UINT8* ptr;
    UINT32 phys_add;

    phys_add = (UINT32)flashAddress;

    MEMD_ASSERT(phys_add < FLASH_SIZE,
                "flash_address is out of the flash chip, not an valid adresse");

    MEMD_ASSERT((phys_add & 0xe0000000) == 0,
                "flash_address is expected to be a byte offset within the flash chip, not an absolute adresse");

    if (GET_PAGE_PHYS(phys_add) <= 2)
    {
        ptr = (VOLATILE CONST UINT8 *)(g_memdFlashBaseAddress + phys_add);
    }
    else  // Page > 2, so remap
    {
        ptr = (VOLATILE CONST UINT8 *)((g_memdFlashBaseAddress + phys_add) - ((GET_PAGE_PHYS(phys_add)-3)*0x400000));
        // Configure Page Register
        hal_EbcConfigRemap((HAL_EBC_FLSH_PHYSADD_T)phys_add);
    }

    return ptr;
}

PRIVATE UINT32 memd_FlashGetSectorLimits(UINT32 sectorId, UINT32 *start, UINT32 *end)
{
    UINT32 aStart, aEnd;
    int bankNum = 0;
    int majSecIdx, minSecIdx;
    UINT32 sectSize;

    if (sectorId == g_memdFlashLayout.fullSize)
    {
        *start = g_memdFlashLayout.fullSize;
        *end = g_memdFlashLayout.fullSize;
        return 0;
    }
    else if (sectorId > g_memdFlashLayout.fullSize)
    {
        *start = 0;
        *end = 0;
        MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
        return 0;
    }
    else
    {
        while ( (sectorId >= g_memdFlashLayout.bankLayout[bankNum+1].bankStartAddr) &&
                (bankNum < (g_memdFlashLayout.numberOfBank-1)) )
        {
            ++bankNum;
        }

        aStart = g_memdFlashLayout.bankLayout[bankNum].bankStartAddr;
        majSecIdx = 0;
        aEnd = aStart +
               (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);

        while (sectorId >= aEnd)
        {
            if (majSecIdx == 2)
            {
                *start = 0;
                *end = 0;
                MEMD_ASSERT(FALSE, "Out of accessible flash space !!!!");
                return 0;
            }
            else
            {
                ++majSecIdx;
                aStart = aEnd;
                aEnd = aStart +
                       (g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][0] *
                        g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1]);
            }
        }

        minSecIdx = 0;
        sectSize = g_memdFlashLayout.bankLayout[bankNum].sectorLayoutTable[majSecIdx][1];
        aEnd = aStart + sectSize;

        while (sectorId >= aEnd)
        {
            aStart = aEnd;
            aEnd = aStart + sectSize;
            ++minSecIdx;
        }

        if((sectorId % 0x10000) != 0)
        {
            sectSize = 0x1000;
            *start = sectorId & (~0xfff);
            *end = (sectorId & (~0xfff)) + sectSize;
        }
        else
        {
            *start = aStart;
            *end = aEnd;
        }

        return sectSize;
    }
}

PRIVATE MEMD_ERR_T memd_SetLock(UINT8 *startFlashAddress, UINT8 *endFlashAddress, BOOL Enable)
{
    return MEMD_ERR_NO;
}

PRIVATE UINT16 memd_LockStatus(UINT8 * SectorAddr, UINT16 *Lock)
{
    *Lock = 0;
    return MEMD_ERR_NO;
}


// =============================================================================
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
                "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
                "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    return memd_SetLock(startFlashAddress, endFlashAddress, TRUE);
}


// =============================================================================

// ============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress)
{
    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
                "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");
    MEMD_ASSERT((((UINT32)endFlashAddress) & 0xe0000000) == 0,
                "endFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    return memd_SetLock(startFlashAddress, endFlashAddress, FALSE);
}

// =============================================================================

// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus)
{
    UINT16 lock;

    MEMD_ASSERT((((UINT32)startFlashAddress) & 0xe0000000) == 0,
                "startFlashAddress is expected to be a byte offset within the flash chip, not an absolute adresse");

    MEMD_ERR_T status = memd_LockStatus(startFlashAddress, &lock);

    if (lock == 1)
    {
        *lockStatus = TRUE;
    }
    else
    {
        *lockStatus = FALSE;
    }

    return status;
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize, UINT32 *blockSize)
{
    UINT32 Start, End;

    *userFieldBase = g_memdFlashLayout.userDataBase;
    *userFieldSize = g_memdFlashLayout.userDataSize;
    *sectorSize = memd_FlashGetSectorLimits(*userFieldBase, &Start, &End);
    *blockSize = g_memdFlashLayout.userDataSectorLogicalBlockSize;
}

UINT32 FLASH_IN_DP=0;

// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashSleep(VOID)
{
#ifdef dual_spi_flash
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_RDI, 0, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN
    hal_SpiFlashSendCmd(OPCODE_DP, 0, 0, 0);
#endif
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0x400000, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_RDI, 0x400000, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN
    hal_SpiFlashSendCmd(OPCODE_DP, 0x400000, 0, 0);
#endif
#else
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_RDI, 0, 0, 0);
#ifdef SPI_FLASH_DEEP_POWER_DOWN
    hal_SpiFlashSendCmd(OPCODE_DP, 0, 0, 0);
#endif
#endif

    FLASH_IN_DP=1;
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashWakeup(VOID)
{
#ifdef dual_spi_flash
    hal_SpiFlashSendCmd(OPCODE_RDI, 0, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0, 0, 0);
    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, 0, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_RDI, 0x400000, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0x400000, 0, 0);
    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, 0x400000, 0, 0);
#else
    hal_SpiFlashSendCmd(OPCODE_RDI, 0, 0, 0);
    hal_SpiFlashSendCmd(OPCODE_ReadReset, 0, 0, 0);
    hal_SpiFlashSendCmd(g_dif_opcode_high_p_mode, 0, 0, 0);
#endif
    FLASH_IN_DP=0;
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}

PUBLIC UINT32 memd_FlashErase2SuspendTime(VOID)
{
    //MILLI_SECONDS
    if(g_spiflash_ID.manufacturerID == MEMD_FLASH_MICRON)
        return FLASH_MICRON_ERASE2SUSPEND_TIME;
    else if(g_spiflash_ID.manufacturerID == MEMD_FLASH_GIGADEVICE)
        return FLASH_GIGADEVICE_ERASE2SUSPEND_TIME;
    else
        return FLASH_COMMON_ERASE2SUSPEND_TIME;
}

PUBLIC VOID memd_FlashRst(VOID)
{
}

