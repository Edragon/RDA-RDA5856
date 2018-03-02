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
// File name      : ahb2spi_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 13:50:57                                                  
// Last modified  : 2016-08-29 13:50:57                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 13:50:57 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB2SPI_ASM_H__
#define __AHB2SPI_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_ctrl                       REG_AHB2SPI_BASE+0x00
#define REG_status                     REG_AHB2SPI_BASE+0x04
#define REG_frame_size                 REG_AHB2SPI_BASE+0x08
#define REG_raw_status                 REG_AHB2SPI_BASE+0x0c
#define REG_feature                    REG_AHB2SPI_BASE+0x10

//ctrl
#define AHB2SPI_ENABLE                 (1<<0)              
#define AHB2SPI_CLOCK_POLARITY         (1<<1)              
#define AHB2SPI_CLOCK_DELAY(n)         (((n)&0x3)<<2)      
#define AHB2SPI_DO_DELAY(n)            (((n)&0x3)<<4)      
#define AHB2SPI_DI_DELAY(n)            (((n)&0x3)<<6)      
#define AHB2SPI_CS_DELAY(n)            (((n)&0x3)<<8)      
#define AHB2SPI_CS_PULSE(n)            (((n)&0x3)<<10)     
#define AHB2SPI_RW_POLARITY            (1<<12)             
#define AHB2SPI_INT_MASK               (1<<13)             
#define AHB2SPI_TX_FINISH_MASK         (1<<14)             
#define AHB2SPI_FIFO_EMPTY_MASK        (1<<15)             
#define AHB2SPI_FIFO_FULL_MASK         (1<<16)             
#define AHB2SPI_SPI_CLK_DIVIDOR(n)     (((n)&0x3FF)<<17)   
#define AHB2SPI_CS_POLARITY(n)         (((n)&0xF)<<27)     
#define AHB2SPI_FIFO_FLUSH             (1<<31)             

//status
#define AHB2SPI_SPI_INT                (1<<0)              
#define AHB2SPI_TX_FINISH              (1<<1)              
#define AHB2SPI_FIFO_EMPTY             (1<<2)              
#define AHB2SPI_FIFO_FULL              (1<<3)              
#define AHB2SPI_TX_SPACE(n)            (((n)&0x7)<<4)      

//frame_size
#define AHB2SPI_CH0_DATA_WIDTH(n)      (((n)&0x3)<<0)      
#define AHB2SPI_CH0_ADDR_WIDTH(n)      (((n)&0x3)<<2)      
#define AHB2SPI_CH1_DATA_WIDTH(n)      (((n)&0x3)<<4)      
#define AHB2SPI_CH1_ADDR_WIDTH(n)      (((n)&0x3)<<6)      
#define AHB2SPI_CH2_DATA_WIDTH(n)      (((n)&0x3)<<8)      
#define AHB2SPI_CH2_ADDR_WIDTH(n)      (((n)&0x3)<<10)     
#define AHB2SPI_CH3_DATA_WIDTH(n)      (((n)&0x3)<<12)     
#define AHB2SPI_CH3_ADDR_WIDTH(n)      (((n)&0x3)<<14)     

//raw_status
#define AHB2SPI_INT_RAW                (1<<0)              
#define AHB2SPI_TX_FINISH_RAW          (1<<1)              
#define AHB2SPI_FIFO_EMPTY_RAW         (1<<2)              
#define AHB2SPI_FIFO_FULL_RAW          (1<<3)              

//feature
#define AHB2SPI_POS_SIMP_NEG_SEND      (1<<0)              
#define AHB2SPI_FIFO_DEPTH(n)          (((n)&0x7)<<1)      


#endif


