//------------------------------------------------------------------------

// Copyright (C) 2016 RDA microelectronic Inc.                                           

// All Rights Reserved                                                    

// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                

// The copyright notice above does not evidence any actual or intended    

// publication of such source code.                                       

// No part of this code may be reproduced, stored in a retrieval system,  

// or transmitted, in any form or by any means, electronic, mechanical,   

// photocopying, recording, or otherwise, without the prior written       

// permission of RDA microelectronic Inc.                                                 

//------------------------------------------------------------------------

// File name      : register                                            

// Author         : liqiu                                                 

// Date of created: 2016-08-29 14:59:45                                                  

// Last modified  : 2016-08-29 14:59:45                                                  

//------------------------------------------------------------------------

// Reversion History:                                                     

// 2016-08-29 14:59:45 --  Create by liqiu                                               

//------------------------------------------------------------------------

#ifndef __REGISTER_H__

#define __REGISTER_H__

#if 0
#include "iomux.h"
#include "gpio.h"
#include "spi_flsh.h"
#include "sdmmc.h"
#include "timer.h"
#include "sys_ctrl.h"
#include "debug_host.h"
#include "debug_uart.h"
#include "i2c_master.h"
#include "calendar.h"
#include "dma.h"
#include "fm.h"
#include "gpadc.h"
#include "ir.h"
#include "uart.h"
#include "sys_ifc.h"
#include "bb_ifc.h"
#include "sys_irq.h"
#include "spi.h"
#include "aif.h"


#define hwp_timer ((T_HWP_TIMER_T*)REG_TIMER_BASE)
#define hwp_sys_ctrl ((T_HWP_SYS_CTRL_T*)REG_SYS_CTRL_BASE)
#define hwp_debug_uart ((T_HWP_DEBUG_UART_T*)REG_DEBUG_UART_BASE)
#define hwp_iomux ((T_HWP_IOMUX_T*)REG_IOMUX_BASE)
#define hwp_gpio ((T_HWP_GPIO_T*)REG_GPIO_BASE)
#define hwp_debugHost ((T_HWP_DEBUG_HOST_T*)REG_DEBUGHOST_REG_BASE)
#define hwp_i2c_master ((T_HWP_I2C_MASTER_T*)REG_I2C_MASTER_BASE)
#define hwp_spiflash ((T_HWP_SPIFLASH_T*)REG_SPIFLASH_BASE)
#define hwp_flshcache ((T_HWP_AHB_ICACHE_T*)REG_FLASH_CACHE_BASE)
#define hwp_sdmmc ((T_HWP_SDMMC_T*)REG_SDMMC_BASE)
#define hwp_calendar ((T_HWP_CALENDAR_T*)REG_CALENDAR_BASE)
#define hwp_dma ((T_HWP_DMA_T*)REG_DMA_BASE)
#define hwp_fm  ((T_HWP_FM_DSP_T*)REG_FM_DSP_BASE)
#define hwp_gpadc ((T_HWP_GPADC_T*)REG_GPADC_BASE)
#define hwp_ir  ((T_HWP_IR_T*)REG_IR_BASE)
#define hwp_Uart ((T_HWP_UART_T*)REG_UART_BASE)
#define hwp_Uart2 ((T_HWP_UART_T*)REG_UART2_BASE)
#define hwp_sysIfc ((T_HWP_SYS_IFC_T*)REG_SYS_IFC_BASE)
#define hwp_bbIfc ((T_HWP_BB_IFC_T*)REG_BB_IFC_BASE)
#define hwp_sysIrq ((T_HWP_SYS_IRQ_T*)REG_SYS_IRQ_BASE)
#define hwp_spi1    ((T_HWP_SPI_T*)REG_SPI_BASE)
#define hwp_aif     ((T_HWP_AIF_T*)REG_AIF_BASE)
#endif


#endif


