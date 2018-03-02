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
// File name      : spi_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:13                                                  
// Last modified  : 2016-09-29 11:13:13                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:13 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SPI_ASM_H__
#define __SPI_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ctrl                       REG_SPI_BASE+0x00   
#define REG_status                     REG_SPI_BASE+0x04   
#define REG_rxtx_buffer                REG_SPI_BASE+0x08   
#define REG_cfg                        REG_SPI_BASE+0x0C   
#define REG_pattern                    REG_SPI_BASE+0x10   
#define REG_stream                     REG_SPI_BASE+0x14   
#define REG_pin_control                REG_SPI_BASE+0x18   
#define REG_irq                        REG_SPI_BASE+0x1C   

//ctrl
#define SPI_ENABLE                     (1<<0)              
#define SPI_CS_SEL(n)                  (((n)&0x3)<<1)      
#define SPI_INPUT_MODE                 (1<<4)              
#define SPI_CLOCK_POLARITY             (1<<5)              
#define SPI_CLOCK_DELAY(n)             (((n)&0x3)<<6)      
#define SPI_DO_DELAY(n)                (((n)&0x3)<<8)      
#define SPI_DI_DELAY(n)                (((n)&0x3)<<10)     
#define SPI_CS_DELAY(n)                (((n)&0x3)<<12)     
#define SPI_CS_PULSE(n)                (((n)&0x3)<<14)     
#define SPI_FRAME_SIZE(n)              (((n)&0x3F)<<16)    
#define SPI_LCD_3_LINE                 (1<<22)             
#define SPI_DCX_OEN                    (1<<23)             
#define SPI_OE_DELAY(n)                (((n)&0x1F)<<24)    
#define SPI_CTRL_DATA_MUX_SEL          (1<<29)             
#define SPI_INPUT_SEL(n)               (((n)&0x3)<<30)     

//status
#define SPI_ACTIVE_STATUS              (1<<0)              
#define SPI_CAUSE_RX_OVF_IRQ           (1<<3)              
#define SPI_CAUSE_TX_TH_IRQ            (1<<4)              
#define SPI_CAUSE_TX_DMA_IRQ           (1<<5)              
#define SPI_CAUSE_RX_TH_IRQ            (1<<6)              
#define SPI_CAUSE_RX_DMA_IRQ           (1<<7)              
#define SPI_TX_OVF                     (1<<9)              
#define SPI_RX_UDF                     (1<<10)             
#define SPI_RX_OVF                     (1<<11)             
#define SPI_TX_TH                      (1<<12)             
#define SPI_TX_DMA_DONE                (1<<13)             
#define SPI_RX_TH                      (1<<14)             
#define SPI_RX_DMA_DONE                (1<<15)             
#define SPI_TX_SPACE(n)                (((n)&0x1F)<<16)    
#define SPI_RX_LEVEL(n)                (((n)&0x1F)<<24)    
#define SPI_FIFO_FLUSH                 (1<<30)             

//rxtx_buffer
#define SPI_DATA(n)                    (((n)&0x1FFFFFFF)<<0)
#define SPI_CS(n)                      (((n)&0x3)<<29)     
#define SPI_INPUT_DATA                 (1<<31)             

//cfg
#define SPI_CS_POLARITY_0              (1<<0)              
#define SPI_CS_POLARITY_1              (1<<1)              
#define SPI_CLOCK_DIVIDER(n)           (((n)&0x3FF)<<16)   
#define SPI_CLOCK_LIMITER              (1<<28)             

//pattern
#define SPI_PATTERN(n)                 (((n)&0xFF)<<0)     
#define SPI_PATTERN_MODE               (1<<8)              
#define SPI_PATTERN_SELECTOR           (1<<9)              

//stream
#define SPI_TX_STREAM_BIT              (1<<0)              
#define SPI_TX_STREAM_MODE             (1<<8)              
#define SPI_TX_STREAM_STOP_WITH_RX_DMA_DONE (1<<16)             

//pin_control
#define SPI_CLK_CTRL(n)                (((n)&0x3)<<0)      
#define SPI_DO_CTRL(n)                 (((n)&0x3)<<2)      
#define SPI_CS0_CTRL(n)                (((n)&0x3)<<4)      
#define SPI_CS1_CTRL(n)                (((n)&0x3)<<6)      

//irq
#define SPI_MASK_RX_OVF_IRQ            (1<<0)              
#define SPI_MASK_TX_TH_IRQ             (1<<1)              
#define SPI_MASK_TX_DMA_IRQ            (1<<2)              
#define SPI_MASK_RX_TH_IRQ             (1<<3)              
#define SPI_MASK_RX_DMA_IRQ            (1<<4)              
#define SPI_TX_THRESHOLD(n)            (((n)&0x3)<<5)      
#define SPI_RX_THRESHOLD(n)            (((n)&0x3)<<7)      


#endif


