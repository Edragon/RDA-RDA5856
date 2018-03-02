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
// File name      : sys_ctrl_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:51                                                  
// Last modified  : 2016-12-14 15:57:51                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:51 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SYS_CTRL_ASM_H__
#define __SYS_CTRL_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_per_soft_reset1            REG_SYS_CTRL_BASE+0x00
#define REG_per_soft_reset2            REG_SYS_CTRL_BASE+0x04
#define REG_per_soft_reset3            REG_SYS_CTRL_BASE+0x08
#define REG_per_clk_force_on           REG_SYS_CTRL_BASE+0x0C
#define REG_per_module_en              REG_SYS_CTRL_BASE+0x10

#define REG_spiflash_clk_cfg           REG_SYS_CTRL_BASE+0x1C
#define REG_audio_clk_cfg              REG_SYS_CTRL_BASE+0x20
#define REG_pwm_clk_cfg                REG_SYS_CTRL_BASE+0x24

#define REG_dbg_hst_uart_clk_cfg       REG_SYS_CTRL_BASE+0x2C
#define REG_uart_clk_cfg               REG_SYS_CTRL_BASE+0x30
#define REG_wakeup_ctrl                REG_SYS_CTRL_BASE+0x34
#define REG_RF_IF_CTRL                 REG_SYS_CTRL_BASE+0x38

#define REG_pad_ctrl                   REG_SYS_CTRL_BASE+0x44

#define REG_i2s_ctrl                   REG_SYS_CTRL_BASE+0x50

#define REG_xcpu_dbg_stall             REG_SYS_CTRL_BASE+0x84

#define REG_xcpu_cache_ram_disable     REG_SYS_CTRL_BASE+0x8C
#define REG_mem_ctrl0                  REG_SYS_CTRL_BASE+0x90
#define REG_mem_ctrl1                  REG_SYS_CTRL_BASE+0x94

#define REG_dbg_mux_ctrl               REG_SYS_CTRL_BASE+0x9C

#define REG_mcu_warm_rst_ctrl          REG_SYS_CTRL_BASE+0x200
#define REG_mcu_warm_reset_cause       REG_SYS_CTRL_BASE+0x204

//per_soft_reset1
#define SYS_CTRL_XCPU_RSTB             (1<<0)              
#define SYS_CTRL_VOC_RSTB              (1<<1)              
#define SYS_CTRL_VOC_AHB_RSTB          (1<<2)              
#define SYS_CTRL_SPIFLASH_RSTB         (1<<3)              
#define SYS_CTRL_AUDIFC_RSTB           (1<<5)              
#define SYS_CTRL_SYSIFC_RSTB           (1<<6)              
#define SYS_CTRL_USBC_RSTB             (1<<7)              
#define SYS_CTRL_DMA_RSTB              (1<<8)              
#define SYS_CTRL_BT_DBM_RSTB           (1<<9)              
#define SYS_CTRL_WDT_TIMER_RSTB        (1<<10)             
#define SYS_CTRL_WDT_RSTB              (1<<12)             
#define SYS_CTRL_USBC_PHY_RSTB         (1<<13)             
#define SYS_CTRL_GLOBAL_RSTB           (1<<31)             

//per_soft_reset2
#define SYS_CTRL_AIF_RSTB              (1<<0)              
#define SYS_CTRL_TIMER_RSTB            (1<<1)              
#define SYS_CTRL_PWM_RSTB              (1<<2)              
#define SYS_CTRL_I2C_RSTB              (1<<3)              
#define SYS_CTRL_SDMMC_RSTB            (1<<4)              
#define SYS_CTRL_SPI_RSTB              (1<<5)              

//per_soft_reset3
#define SYS_CTRL_UART_RSTB             (1<<0)              
#define SYS_CTRL_DBG_HST_RSTB          (1<<1)              
#define SYS_CTRL_USB_DET_RSTB          (1<<2)              
#define SYS_CTRL_IR_RSTB               (1<<3)              
#define SYS_CTRL_GPIO_RSTB             (1<<4)              
#define SYS_CTRL_BT_RSTB               (1<<5)              
#define SYS_CTRL_CODEC_RSTB            (1<<7)              

//per_clk_force_on
#define SYS_CTRL_SDMMC_CLK_FORCE_ON    (1<<0)              
#define SYS_CTRL_SPI_CLK_FORCE_ON      (1<<1)              
#define SYS_CTRL_UART_CLK_FORCE_ON     (1<<2)              
#define SYS_CTRL_DBG_HST_CLK_FORCE_ON  (1<<3)              
#define SYS_CTRL_I2C_CLK_FORCE_ON      (1<<4)              
#define SYS_CTRL_AIF_CLK_FORCE_ON      (1<<5)              
#define SYS_CTRL_DMA_CLK_FORCE_ON      (1<<6)              
#define SYS_CTRL_PWM_CLK_FORCE_ON      (1<<7)              
#define SYS_CTRL_SYSIFC_CLK_FORCE_ON   (1<<8)              
#define SYS_CTRL_AUDIFC_CLK_FORCE_ON   (1<<9)              
#define SYS_CTRL_XCPU_INT_CLK_FORCE_ON (1<<10)             
#define SYS_CTRL_XCPU_CLK_FORCE_ON     (1<<11)             
#define SYS_CTRL_VOC_CLK_FORCE_ON      (1<<12)             
#define SYS_CTRL_AHB_MEM_CLK_FORCE_ON  (1<<29)             
#define SYS_CTRL_AHB_REG_CLK_FORCE_ON  (1<<30)             
#define SYS_CTRL_APB_REG_CLK_FORCE_ON  (1<<31)             

//per_module_en
#define SYS_CTRL_VOC_EN                (1<<0)              
#define SYS_CTRL_USBC_EN               (1<<2)              
#define SYS_CTRL_SPIFLASH_EN           (1<<3)              
#define SYS_CTRL_BT_EN                 (1<<5)              
#define SYS_CTRL_BT_DBM_EN             (1<<6)              
#define SYS_CTRL_DBG_HST_EN            (1<<7)              
#define SYS_CTRL_UART_EN               (1<<8)              
#define SYS_CTRL_IR_EN                 (1<<10)             
#define SYS_CTRL_GPADC_EN              (1<<11)             
#define SYS_CTRL_GPIO_EN               (1<<12)             
#define SYS_CTRL_TIMER_EN              (1<<13)             
#define SYS_CTRL_USB_DET_EN            (1<<14)             
#define SYS_CTRL_WDT_EN                (1<<16)             

//spiflash_clk_cfg
#define SYS_CTRL_SPIFLASH_CLK_SEL_FAST (1<<0)              
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL(n) (((n)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_PLL_CLK_DIV(n) (((n)&0xF)<<3)      
#define SYS_CTRL_SPIFLASH_PLL_CLK_LD_CFG (1<<7)

#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL(n) (((n)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_SPIFLASH_PLL_DIV_CLK (((3)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_52M (((2)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_39M (((1)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M (((0)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_MASK (0x3<<1)            
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_SHIFT (1)  

//audio_clk_cfg
#define SYS_CTRL_AIF_TX_STB_DIV(n)     (((n)&0x7FF)<<0)    
#define SYS_CTRL_AIF_TX_STB_DIV_EN     (1<<11)             
#define SYS_CTRL_AIF_TX_STB_DIV_LD_CFG (1<<12)             
#define SYS_CTRL_CODEC_CLK_SEL_PLL     (1<<14)             
#define SYS_CTRL_AIF_BCLK_DIV(n)       (((n)&0x7FF)<<15)   
#define SYS_CTRL_AIF_BCLK_DIV_LD_CFG   (1<<26)             
#define SYS_CTRL_AIF_BCLK_DIV_EN       (1<<27)             
#define SYS_CTRL_I2S_MCLK_DIV(n)       (((n)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_LD_CFG   (1<<30)             
#define SYS_CTRL_I2S_MCLK_DIV_EN       (1<<31)             

//pwm_clk_cfg
#define SYS_CTRL_PWM_CLK_DIV(n)        (((n)&0x3FF)<<0)    
#define SYS_CTRL_PWM_CLK_LD_CFG        (1<<10)             

//dbg_hst_uart_clk_cfg
#define SYS_CTRL_DBG_HST_UART_CLK_SEL_52M (1<<0)              
#define SYS_CTRL_DBG_HST_UART_CLK_DENOM(n) (((n)&0x3FF)<<1)    
#define SYS_CTRL_DBG_HST_UART_CLK_NUM(n) (((n)&0x3FF)<<11)   
#define SYS_CTRL_DBG_HST_UART_CLK_LD_CFG (1<<21)             

//uart_clk_cfg
#define SYS_CTRL_UART_CLK_SEL_52M      (1<<0)              
#define SYS_CTRL_UART_CLK_DENOM(n)     (((n)&0x3FF)<<1)    
#define SYS_CTRL_UART_CLK_NUM(n)       (((n)&0x3FF)<<11)   
#define SYS_CTRL_UART_CLK_LD_CFG       (1<<21)             

//wakeup_ctrl
#define SYS_CTRL_FORCE_WAKEUP          (1<<0)              
#define SYS_CTRL_UART_RXD_BREAK_WAKEUP_BT_EN (1<<8)              

//RF_IF_CTRL
#define SYS_CTRL_RF_REG_SRC_SEL        (1<<0)              

//pad_ctrl
#define SYS_CTRL_SFLASH_DRV(n)         (((n)&0x7)<<0)      

//i2s_ctrl
#define SYS_CTRL_I2S_SEL               (1<<0)              

//xcpu_dbg_stall
#define SYS_CTRL_STALLED               (1<<8)              

//xcpu_cache_ram_disable
#define SYS_CTRL_XCPU_USE_MODE         (1<<0)              
#define SYS_CTRL_XCPU_CLK_OFF_MODE     (1<<1)              
#define SYS_CTRL_TCM_DCACHE_DISABLE    (1<<2)              
#define SYS_CTRL_DTAG_RAM_CLK_FORCE_ON (1<<8)              
#define SYS_CTRL_DDATA_RAM_CLK_FORCE_ON (1<<9)              
#define SYS_CTRL_ITAG_RAM_CLK_FORCE_ON (1<<10)             
#define SYS_CTRL_IDATA_RAM_CLK_FORCE_ON (1<<11)             

//mem_ctrl0
#define SYS_CTRL_ROM_DVSE              (1<<0)              
#define SYS_CTRL_ROM_DVS(n)            (((n)&0xF)<<1)      
#define SYS_CTRL_DRAM_DVSE             (1<<5)              
#define SYS_CTRL_DRAM_DVS(n)           (((n)&0x7)<<6)      
#define SYS_CTRL_BRAM_DVSE             (1<<9)              
#define SYS_CTRL_BRAM_DVS(n)           (((n)&0x7)<<10)     
#define SYS_CTRL_CACHE_RAM_DVSE        (1<<13)             
#define SYS_CTRL_CACHE_RAM_DVS(n)      (((n)&0xF)<<14)     
#define SYS_CTRL_TAG_RAM_DVSE          (1<<18)             
#define SYS_CTRL_TAG_RAM_DVS(n)        (((n)&0xF)<<19)     

//mem_ctrl1
#define SYS_CTRL_BT_LINK_RAM_DVSE      (1<<0)              
#define SYS_CTRL_BT_LINK_RAM_DVS(n)    (((n)&0xF)<<1)      
#define SYS_CTRL_BT_MODEM_RAM_DVSE     (1<<5)              
#define SYS_CTRL_BT_MODEM_RAM_DVS(n)   (((n)&0xF)<<6)      
#define SYS_CTRL_USBC_RAM_DVSE         (1<<10)             
#define SYS_CTRL_USBC_RAM_DVS(n)       (((n)&0xF)<<11)     
#define SYS_CTRL_VOC_RAM_DVSE          (1<<15)             
#define SYS_CTRL_VOC_RAM_DVS(n)        (((n)&0x7)<<16)     

//dbg_mux_ctrl
#define SYS_CTRL_DBG_OUT_SEL(n)        (((n)&0xFF)<<0)     
#define SYS_CTRL_DBG_OUT_IDX(n)        (((n)&0xF)<<8)      
#define SYS_CTRL_DBG_OUT_NIBBLE_SWAP   (1<<14)             
#define SYS_CTRL_DBG_OUT_BYTE_SWAP     (1<<15)             
#define SYS_CTRL_DBG_OUT_NIBBLE_SHIFT  (1<<16)             
#define SYS_CTRL_DBG_OUT_NIBBLE_SHIFT_MODE (1<<17)             
#define SYS_CTRL_DBG_CLK_SEL(n)        (((n)&0x3F)<<18)    
#define SYS_CTRL_DBG_TRIG_SEL(n)       (((n)&0xFF)<<24)    

//mcu_warm_rst_ctrl
#define SYS_CTRL_IOMUX_WARM_RST_EN     (1<<2)              

//mcu_warm_reset_cause
#define SYS_CTRL_XCPU_RST              (1<<0)              
#define SYS_CTRL_WDT_RST               (1<<1)              
#define SYS_CTRL_GLOBAL_SOFT_RST       (1<<2)              
#define SYS_CTRL_DBG_HST_RST           (1<<3)              
#define SYS_CTRL_ALARM_CAUSE           (1<<4)              
#define SYS_CTRL_RST_CAUSE_CLR         (1<<8)              
#define SYS_CTRL_BOOT_MODE(n)          (((n)&0x3F)<<16)    
#define SYS_CTRL_FUNCTION_TEST_MODE    (1<<31)      

#define SYS_CTRL_BOOT_MODE(n)          (((n)&0x3F)<<16)    
#define SYS_CTRL_BOOT_MODE_MASK        (0x3F<<16)          
#define SYS_CTRL_BOOT_MODE_SHIFT       (16) 


#endif


