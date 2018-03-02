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
//    $Author: huangyx $                                                        //
//    $Date:20151110                                                       //
//    $Revision: $                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///     @file hal_spi_flash.c                                                       //
///     Implementation of the SPI Flash .                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "hal_spi_flash.h"
#include "hal_host.h"
#ifdef CHIP_5856E
#include "spiflash.h"
#else
#include "spi_flsh.h"
#endif
#include "halp_debug.h"
#include "sys_ctrl.h"
#include "sys_irq.h"

// =============================================================================
//
// -----------------------------------------------------------------------------
// =============================================================================


// =============================================================================
//  MACROS
// =============================================================================
#ifndef SPIFLASH_NO_TRACE
#define HAL_SPIFLASH_ASSERT(BOOL, format, ...)
#else
#define HAL_SPIFLASH_ASSERT   HAL_ASSERT
#endif


//ramrun busy status is ok,when in flash status is always busy,so do not check the busy status 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
	#ifndef _FLASH_PROGRAMMER
	    #define hal_SpiFlashWaitBusy()	///while (gc_halSpiFlashHwp->status&SPI_FLASH_BUSY_STAT)
	#else
        #define hal_SpiFlashWaitBusy()	while (gc_halSpiFlashHwp->status&SPI_FLASH_BUSY_STAT)
	#endif
#else
	#ifndef CHIP_5856E
	    #define hal_SpiFlashWaitBusy()	while (gc_halSpiFlashHwp->status&SPI_FLASH_BUSY_STAT)
    #else
        #define hal_SpiFlashWaitBusy()	while (gc_halSpiFlashHwp->status&SPIFLASH_BUSY_STAT)
    #endif
#endif

#ifndef CHIP_5856E
#define hal_SpiFlashWaitRead()	while ((gc_halSpiFlashHwp->status&SPI_FLASH_RX_FIFO_EMPTY)==0)
#else
#define hal_SpiFlashWaitRead()	while ((gc_halSpiFlashHwp->status&SPIFLASH_RX_FIFO_EMPTY)==0)
#endif

#if CHIP_HAS_FLSH_CACHE

#define hal_FlashCacheEnable()  do{\
    if(gc_halFlashCahceOn)	\
    {	\
        gc_halFlashCacheHwp->cache_conf |= FLSH_CACHE_ENABLE;\
    }\
}while(0)

#define hal_FlashCacheDisable() do{\
	gc_halFlashCacheHwp->cache_conf &= ~FLSH_CACHE_ENABLE;\
}while(0)

#endif /* CHIP_HAS_FLSH_CACHE */
// =============================================================================
//  TYPES
// =============================================================================
PRIVATE HWP_SPI_FLSH_T* CONST gc_halSpiFlashHwp = hwp_spi_flsh;


// =============================================================================
//  FUNCTIONS
// =============================================================================
#if CHIP_HAS_FLSH_CACHE
PRIVATE HWP_FLSH_CACHE_T* CONST gc_halFlashCacheHwp = hwp_flsh_cache;
//Disable cache always if FALSE
PRIVATE BOOL gc_halFlashCahceOn;
/******************************************************************************
 * hal_FlashCacheOn: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_FlashCacheOn(HAL_FLASH_CACHE_MAP_T map)
{
    UINT32 config = gc_halFlashCacheHwp->cache_conf;

    config &= (~(FLSH_CACHE_SIZE_MASK | FLSH_CACHE_FLASH_MAP_MASK));
    config |= FLSH_CACHE_SIZE(HAL_FLASH_CACHE_TO_CACHE_SIZE(map)) 
    			|FLSH_CACHE_FLASH_MAP(HAL_FLASH_CACHE_TO_FLASH_MAP(map)) 
    			| FLSH_CACHE_ENABLE;

    gc_halFlashCacheHwp->cache_conf = config;

    //Force flush cache for now(hardware problem)
    //gc_halFlashCacheHwp->flush = FLSH_CACHE_LINE_FLUSH;
    //while(gc_halFlashCacheHwp->flush & FLSH_CACHE_LINE_FLUSH);

    gc_halFlashCahceOn = TRUE;
}

/******************************************************************************
 * hal_FlashCacheOff: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_FlashCacheOff(VOID)
{
    hal_FlashCacheDisable();
    gc_halFlashCahceOn = FALSE;
}
PUBLIC VOID hal_FlashCacheFlush(VOID)
{
    // Enter critical section.
    UINT32 status = hwp_sysIrq->sc;

    //hal_FlashCacheDisable(); Disable cache is NOT necessary when both CPU run on ram
    gc_halFlashCacheHwp->cache_flush = FLSH_CACHE_LINE_FLUSH;
    while(gc_halFlashCacheHwp->cache_flush & FLSH_CACHE_LINE_FLUSH);
    //hal_FlashCacheEnable();
    // Exit critical section.
    hwp_sysIrq->sc = status;
}

/******************************************************************************
 * hal_FlashCacheDebugOn: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_FlashCacheDebugOn(VOID)
{
    gc_halFlashCacheHwp->cache_irq_mask = FLSH_CACHE_FLUSH_DONE_IRQ_MASK;
}

/******************************************************************************
 * hal_FlashCacheIrqHandler: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PROTECTED VOID hal_FlashCacheIrqHandler(UINT8 interruptId)
{
    UINT32 irq = gc_halFlashCacheHwp->cache_irq;
    UINT32 status = gc_halFlashCacheHwp->cache_status ;
    
    hal_HstSendEvent(BOOT_EVENT,0xcac0e000 + interruptId);
    hal_HstSendEvent(BOOT_EVENT,irq);
    hal_HstSendEvent(BOOT_EVENT,status);
    
    gc_halFlashCacheHwp->cache_status = FLSH_CACHE_STATUS_ADDR_RANGE_ERR
                                  |FLSH_CACHE_STATUS_WRITE_ERR
                                  |FLSH_CACHE_STATUS_CACHE_DISABLE_ERR
                                  |FLSH_CACHE_STATUS_FLUSH_DONE
                                  ;
}


#endif /* CHIP_HAS_FLSH_CACHE */


PUBLIC BOOL hal_SpiFlashIsBusy(VOID)
{
    UINT32 data_tmp_32;
    data_tmp_32 = gc_halSpiFlashHwp->status;
#ifndef CHIP_5856E
    return (((data_tmp_32&SPI_FLASH_BUSY_STAT)==1)||((data_tmp_32&SPI_FLASH_READ_STATE_BUSY)==0));
#else
    return (((data_tmp_32&SPIFLASH_BUSY_STAT)==1)||((data_tmp_32&SPIFLASH_READ_STATE_BUSY)==0));
#endif
}

PUBLIC BOOL hal_SpiFlashIsTxFifoEmpty(VOID)
{
#ifndef CHIP_5856E
    return (hwp_spi_flsh->status&SPI_FLASH_TX_FIFO_EMPTY);
#else
    return (hwp_spi_flsh->status&SPIFLASH_TX_FIFO_EMPTY);
#endif
}


/******************************************************************************
 * hal_SpiFlashDisableRead: 
 * DESCRIPTION: - Disable continous read and cache before erase/write spi flash
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
PUBLIC VOID hal_SpiFlashDisableRead(VOID)
{
	hal_SpiFlashWaitRead();
#ifdef SPI_FLASH_CONTINUOUS_READ
    hal_SpiFlashWaitBusy();
    gc_halSpiFlashHwp->dual_spi = SPI_FLASH_AHB_READ_DISABLE;
#endif
#if CHIP_HAS_FLSH_CACHE
    hal_FlashCacheDisable();
#endif /* CHIP_HAS_FLSH_CACHE */
    return ;
}


PUBLIC VOID hal_SpiFlashEnableRead(VOID)
{
#ifdef SPI_FLASH_CONTINUOUS_READ
    hal_SpiFlashWaitBusy();
    gc_halSpiFlashHwp->dual_spi = 0x0;
#endif
#if CHIP_HAS_FLSH_CACHE
    hal_FlashCacheEnable();
#endif /* CHIP_HAS_FLSH_CACHE */
    return ;
}

/****************************************************************************/
/*  data_array[]: bytes array to be push                                    */
/*  data_cnt:           push data count                                     */
/*  quard_flag:     true for quard operation, false for serial operation;   */
/*                               only valid for extended operation          */
/****************************************************************************/
PUBLIC BOOL hal_SpiFlashPushTxFifoData(UINT8 data_array[], UINT32 data_cnt, BOOL quard_flag)
{
    UINT32 i, data_tmp_32;

	hal_SpiFlashWaitBusy();

    for(i = 0; i < data_cnt; i++)
    {
        HAL_SPIFLASH_ASSERT((data_array != NULL) ,"NULL destination for PushTxFifoData");

        data_tmp_32 = (UINT32)data_array[i];
        if(quard_flag)
            data_tmp_32 = data_tmp_32 | 0x100;
        #ifndef CHIP_5856E
        if(gc_halSpiFlashHwp->status&SPI_FLASH_TX_FIFO_FULL)
        #else
        if(gc_halSpiFlashHwp->status&SPIFLASH_TX_FIFO_FULL)
        #endif
        {
        	return FALSE;
        }
        else
        {
        	gc_halSpiFlashHwp->tx_fifo_data = data_tmp_32;
        }
    }
    return TRUE;
}

PUBLIC UINT32 hal_SpiFlashGetRxData(UINT8* destAddress, UINT32 length)
{
    UINT32 i;
    UINT32 nbAvailable = length;

    // Check the address pointer.
    HAL_SPIFLASH_ASSERT((destAddress != NULL) ,"NULL destination for Rx");
    #ifndef CHIP_5856E
    while (gc_halSpiFlashHwp->status&SPI_FLASH_RX_FIFO_EMPTY);
    #else
    while (gc_halSpiFlashHwp->status&SPIFLASH_RX_FIFO_EMPTY);
    #endif
    hal_SpiFlashWaitBusy();
    
    // Get data byte by byte.
    for (i = 0; i < nbAvailable; i++)
    {
        *(destAddress + i) = (UINT8) gc_halSpiFlashHwp->rx_fifo_data;
    }

    return i;
}



PROTECTED BOOL hal_SpiFlashClearFifoData(BOOL clr_read_flag,BOOL clr_write_flag)
{
    UINT32 data_tmp_32 = 0;
    UINT32 wait_flag = 0;


    if(clr_read_flag)
    {
        #ifndef CHIP_5856E
        data_tmp_32 |= SPI_FLASH_RX_FIFO_CLR;
        wait_flag |= SPI_FLASH_RX_FIFO_EMPTY;
        #else
        data_tmp_32 |= SPIFLASH_RX_FIFO_CLR;
        wait_flag |= SPIFLASH_RX_FIFO_EMPTY;
        #endif
    }

    if(clr_write_flag)
    {
        #ifndef CHIP_5856E
        data_tmp_32 |= SPI_FLASH_TX_FIFO_CLR;
        wait_flag |= SPI_FLASH_TX_FIFO_EMPTY;
        #else
        data_tmp_32 |= SPIFLASH_TX_FIFO_CLR;
        wait_flag |= SPIFLASH_TX_FIFO_EMPTY;
        #endif
    }

    hal_SpiFlashWaitBusy();
    gc_halSpiFlashHwp->fifo_ctrl = data_tmp_32;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
    gc_halSpiFlashHwp->fifo_ctrl &= ~data_tmp_32;
#endif
    return TRUE;
}

/********************************************************************/
/*  opmode = 0: normal operation                                                                        */
/*  opmode = 1: extended serial read                                                                */
/*  opmode = 2: extended quard read                                                                 */
/*  opmode = 3: extended write          (OPCODE_WRITE_ENABLE, flash_addr, opmode, 0);                                                               */
/********************************************************************/
PROTECTED VOID hal_SpiFlashSendCmd(UINT8 cmd, UINT32 addr, UINT8 opmode, UINT16 blocksize)
{
    UINT32 data_tmp_32;

    if(blocksize!=0)
    {
        hal_SpiFlashWaitBusy();
        #ifndef CHIP_5856E  
        data_tmp_32 = gc_halSpiFlashHwp->trans_mode &(~SPI_FLASH_TX_BLOCK_SIZE_MASK);
        gc_halSpiFlashHwp->trans_mode = data_tmp_32|SPI_FLASH_TX_BLOCK_SIZE(blocksize);
        #else
        data_tmp_32 = gc_halSpiFlashHwp->trans_mode &(~SPIFLASH_TX_BLOCK_SIZE_MASK);
        gc_halSpiFlashHwp->trans_mode = data_tmp_32|SPIFLASH_TX_BLOCK_SIZE(blocksize);
        #endif 
    }
	
    if(opmode==0)
    {
        data_tmp_32 = ((addr<<8)&0xffffff00)|cmd;
    }
    else if(opmode==1)
    {
        data_tmp_32 = (((cmd<<8)|0x10000)&0x000fff00);
    }
    else if(opmode==2)
    {
        data_tmp_32 = (((cmd<<8)|0x30000)&0x000fff00);
    }
    else if(opmode==3)
    {
        data_tmp_32 = ((cmd<<8)&0x000fff00);
    }
    else
    {
        data_tmp_32 = ((cmd<<8)&0x000fff00);
    }

    hal_SpiFlashWaitBusy();
    gc_halSpiFlashHwp->tx_cmd_addr = data_tmp_32;

    return;
}

PUBLIC BOOL hal_SpiFlashInit(HAL_SPI_FLASH_CFG_T *cfg)
{
    UINT32 data_tmp_32;

    HAL_SPIFLASH_ASSERT((cfg != NULL) ,"NULL cfg");

	data_tmp_32 = 0;
    switch(cfg->readMode)
    {
    	case HAL_SPI_FLASH_READ_DUAL_MODE:
            #ifndef CHIP_5856E
    		data_tmp_32 |= SPI_FLASH_DUAL_MODE;
            #else
            data_tmp_32 |= SPIFLASH_DUAL_MODE;
            #endif
    		break;

    	case HAL_SPI_FLASH_READ_QUARD_MODE:
            #ifndef CHIP_5856E
    		data_tmp_32 |= SPI_FLASH_QUAD_MODE;
            #else
            data_tmp_32 |= SPIFLASH_QUAD_MODE;
            #endif
    		break;
    		
    	case HAL_SPI_FLASH_READ_SERIAL_MODE:
    	default:
    		break;
    }
    
    switch(cfg->cmdMode)
    {
    	case HAL_SPI_FLASH_CMD_DUAL_MODE:
    		HAL_SPIFLASH_ASSERT(0, "NOT support now!");
    		break;

    	case HAL_SPI_FLASH_CMD_QUARD_MODE:
            #ifndef CHIP_5856E
    		data_tmp_32 |= SPI_FLASH_CMD_QUAD_EN;
            #else
            data_tmp_32 |= SPIFLASH_CMD_QUAD;
            #endif
    		break;
    		
    	case HAL_SPI_FLASH_CMD_SERIAL_MODE:
    	default:
    		break;
    }
    #ifndef CHIP_5856E
    data_tmp_32 |= SPI_FLASH_CLK_DIVIDER(cfg->clkDiv) | SPI_FLASH_SAMPLE_DELAY(cfg->sampleDelay);
    #else
    data_tmp_32 |= SPIFLASH_CLK_DIVIDER(cfg->clkDiv) | SPIFLASH_SAMPLE_DELAY(cfg->sampleDelay);
    #endif

	if(cfg->dinCapEdge)
	{
        #ifndef CHIP_5856E
		data_tmp_32 |= SPI_FLASH_DIN_CAP_EDGE;
        #else
        data_tmp_32 |= SPIFLASH_DIN_CAP_EDGE;
        #endif
	}
	
	if(cfg->holdPin)
	{
        #ifndef CHIP_5856E
		data_tmp_32 |= SPI_FLASH_HOLD_PIN;
        #else
        data_tmp_32 |= SPIFLASH_HOLD_PIN;
        #endif
	}
	
	if(cfg->wpPin)
	{
        #ifndef CHIP_5856E
		data_tmp_32 |= SPI_FLASH_WPROTECT_PIN;
        #else
        data_tmp_32 |= SPIFLASH_WPROTECT_PIN;
        #endif
	}
	
	hal_HstSendEvent(SYS_EVENT, data_tmp_32);
    hal_SpiFlashWaitBusy();
    
    gc_halSpiFlashHwp->flash_config = data_tmp_32;
    return TRUE;
}

PUBLIC VOID hal_SpiFlashContinuousMode(BOOL enable)
{
    hal_SpiFlashWaitBusy();

    if(enable)
    {
        #ifndef CHIP_5856E
        gc_halSpiFlashHwp->trans_mode |= SPI_FLASH_TX_CONT_MODE;
        #else
        gc_halSpiFlashHwp->trans_mode |= SPIFLASH_TX_CONT_MODE;
        #endif
    }
    else
    {
        #ifndef CHIP_5856E
        gc_halSpiFlashHwp->trans_mode &= ~SPI_FLASH_TX_CONT_MODE;
        #else
        gc_halSpiFlashHwp->trans_mode &= ~SPIFLASH_TX_CONT_MODE;
        #endif
    }
}

#if (FLASH_LIFE_TEST_FOR_PUYA_8M_FLASH == 1)
inline BOOL hal_SpiFlashPushTxFifoDataTst(UINT8 data_array[], UINT32 data_cnt, BOOL quard_flag)
{
    UINT32 i, data_tmp_32;

    hal_SpiFlashWaitBusy();

    for(i = 0; i < data_cnt; i++)
    {
        data_tmp_32 = (UINT32)data_array[i];
        if(quard_flag)
            data_tmp_32 = data_tmp_32 | 0x100;
        #ifndef CHIP_5856E
        if(gc_halSpiFlashHwp->status&SPI_FLASH_TX_FIFO_FULL)
        #else
        if(gc_halSpiFlashHwp->status&SPIFLASH_TX_FIFO_FULL)
        #endif
        {
        	return FALSE;
        }
        else
        {
        	gc_halSpiFlashHwp->tx_fifo_data = data_tmp_32;
        }
    }
    return TRUE;
}
inline VOID hal_SpiFlashSendCmdTst(UINT8 cmd, UINT32 addr, UINT8 opmode, UINT16 blocksize)
{
    UINT32 data_tmp_32;

    if(opmode==0)
    {
        data_tmp_32 = ((addr<<8)&0xffffff00)|cmd;
    }
    hal_SpiFlashWaitBusy();
    gc_halSpiFlashHwp->tx_cmd_addr = data_tmp_32;
    return;
}

PUBLIC VOID setup_dual_page_mode(VOID)
{
	//send cmd write enable + WRCR(0x31)+0x80
	hal_SpiFlashSendCmdTst(0x6, 0, 0, 0);
	hal_SpiFlashPushTxFifoDataTst(0x80,1,0);
	hal_SpiFlashSendCmdTst(0,0x31,0,0);
}
PUBLIC VOID exit_dual_page_mode(VOID)
{
	//send cmd wren(0x06) +WRCR(0x31)+0x00h
	hal_SpiFlashSendCmdTst(0x6, 0, 0, 0);
	hal_SpiFlashPushTxFifoDataTst(0x00,1,0);
	hal_SpiFlashSendCmdTst(0,0x31,0,0);
}
PUBLIC VOID enter_test_mode_command(VOID)
{
	//send cmd 0xa8 0x8a 0x65
	hal_SpiFlashSendCmdTst(0xa8,0,0,0);
	hal_SpiFlashSendCmdTst(0x8a,0,0,0);
	hal_SpiFlashSendCmdTst(0x65,0,0,0);
}
PUBLIC VOID exit_test_mode_command(VOID)
{
	UINT8 data_patern[4] = {0x88,0x89,0x98,0x99}; 
	//cmd data is 0x66 0x99
	UINT8 cmd_data[2] = {0x66,0x99};
	//config quard mode 
	gc_halSpiFlashHwp->flash_config = 0x00010110;
	//send cmd data 0x66
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[0]&(0x3<<4))>>4)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[0]&(0x3<<2))>>2)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[0]&(0x3<<0))>>0)],1,1);
	hal_SpiFlashSendCmdTst(0,data_patern[((cmd_data[0]&(0x3<<6))>>6)],0,0);

	//send cmd data 0x99
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[1]&(0x3<<4))>>4)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[1]&(0x3<<2))>>2)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((cmd_data[1]&(0x3<<0))>>0)],1,1);
	hal_SpiFlashSendCmdTst(0,data_patern[((cmd_data[1]&(0x3<<6))>>6)],0,0);
}
PUBLIC UINT32 page_chip_program(UINT32 flash_addr,UINT8 *p_data,UINT32 size)
{
	UINT8 data_patern[4] = {0x88,0x89,0x98,0x99}; 
	gc_halSpiFlashHwp->flash_config = 0x00010210;
	//send cmd 0xd4 and 24 bit addr
	UINT8 i = 0,j = 0;
	UINT8 addr = 0;
	UINT8 *pData = p_data;
	UINT8 status = 0;
	//send 6bit cmd 
	hal_SpiFlashPushTxFifoDataTst(data_patern[((0xD4&(0x3<<4))>>4)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((0xD4&(0x3<<2))>>2)],1,1);
	hal_SpiFlashPushTxFifoDataTst(data_patern[((0xD4&(0x3<<0))>>0)],1,1);

	//send 24 bit addr
	for(i = 0;i <3;i++)
	{
		addr =  (flash_addr&(0xff<<((2-i)*8)))>>((2-i)*8);
		for(j = 0;j < 4;j++)
		{
			status = hal_SpiFlashPushTxFifoDataTst(data_patern[((addr&(0x3<<(3-j)*2))>>(3-j)*2)],1,1);
		}
	}

	
	//send data 8 bytes
	for(i = 0;i <29*2;i++,pData++)
	{
		for(j = 0;j<4;j++)
		{
			status = hal_SpiFlashPushTxFifoDataTst(data_patern[((*pData&(0x3<<(3-j)*2))>>(3-j)*2)],1,1);
		}
	}
	//send 2bit cmd 
	hal_SpiFlashSendCmdTst(0,data_patern[((0xD4&(0x3<<6))>>6)],0,0);

	//send 57 bytes data to fifo 
	for(i = 0;i < 227*2;i++,pData++)
	{
		for(j = 0;j<4;j++)
		{
			status = hal_SpiFlashPushTxFifoDataTst(data_patern[((*pData&(0x3<<(3-j)*2))>>(3-j)*2)],1,1);
		}
	}
	return status;
}
#endif


