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
// File name      : uart_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 15:00:02                                                  
// Last modified  : 2016-08-29 15:00:02                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 15:00:02 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __UART_ASM_H__
#define __UART_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define UART_RX_FIFO_SIZE                        (32)
#define UART_TX_FIFO_SIZE                        (16)
#define NB_RX_FIFO_BITS                          (5)
#define NB_TX_FIFO_BITS                          (4)


#define REG_ctrl                       REG_UART_BASE+0x00  
#define REG_status                     REG_UART_BASE+0x04  
#define REG_rxtx_buffer                REG_UART_BASE+0x08  
#define REG_irq_mask                   REG_UART_BASE+0x0C  
#define REG_irq_cause                  REG_UART_BASE+0x10  
#define REG_triggers                   REG_UART_BASE+0x14  
#define REG_cmd_set                    REG_UART_BASE+0x18  
#define REG_cmd_clr                    REG_UART_BASE+0x1C  
#define REG_auto_baud                  REG_UART_BASE+0x20  

//ctrl
#define UART_ENABLE                    (1<<0)              
#define UART_DATA_BITS                 (1<<1)              
#define UART_TX_STOP_BITS              (1<<2)              
#define UART_PARITY_ENABLE             (1<<3)              
#define UART_PARITY_SELECT(n)          (((n)&0x3)<<4)      
#define UART_DIVISOR_MODE              (1<<20)             
#define UART_IRDA_ENABLE               (1<<21)             
#define UART_DMA_MODE                  (1<<22)             
#define UART_AUTO_FLOW_CONTROL         (1<<23)             
#define UART_LOOP_BACK_MODE            (1<<24)             
#define UART_RX_LOCK_ERR               (1<<25)             
#define UART_RX_BREAK_LENGTH(n)        (((n)&0xF)<<28)     

//status
#define UART_RX_FIFO_LEVEL(n)          (((n)&0x7F)<<0)     
#define UART_TX_FIFO_SPACE(n)          (((n)&0x1F)<<8)     
#define UART_TX_ACTIVE                 (1<<14)             
#define UART_RX_ACTIVE                 (1<<15)             
#define UART_RX_OVERFLOW_ERR           (1<<16)             
#define UART_TX_OVERFLOW_ERR           (1<<17)             
#define UART_RX_PARITY_ERR             (1<<18)             
#define UART_RX_FRAMING_ERR            (1<<19)             
#define UART_RX_BREAK_INT              (1<<20)             
#define UART_DCTS                      (1<<24)             
#define UART_CTS                       (1<<25)             
#define UART_DTR                       (1<<28)             
#define UART_CLK_ENABLED               (1<<31)             

//rxtx_buffer
#define UART_DATA(n)                   (((n)&0xFF)<<0)     

//irq_mask
#define UART_TX_MODEM_STATUS           (1<<0)              
#define UART_RX_DATA_AVAILABLE         (1<<1)              
#define UART_TX_DATA_NEEDED            (1<<2)              
#define UART_RX_TIMEOUT                (1<<3)              
#define UART_RX_LINE_ERR               (1<<4)              
#define UART_TX_DMA_DONE               (1<<5)              
#define UART_RX_DMA_DONE               (1<<6)              
#define UART_RX_DMA_TIMEOUT            (1<<7)              
#define UART_DTR_RISE                  (1<<8)              
#define UART_DTR_FALL                  (1<<9)              

//irq_cause
#define UART_TX_MODEM_STATUS           (1<<0)              
#define UART_RX_DATA_AVAILABLE         (1<<1)              
#define UART_TX_DATA_NEEDED            (1<<2)              
#define UART_RX_TIMEOUT                (1<<3)              
#define UART_RX_LINE_ERR               (1<<4)              
#define UART_TX_DMA_DONE               (1<<5)              
#define UART_RX_DMA_DONE               (1<<6)              
#define UART_RX_DMA_TIMEOUT            (1<<7)              
#define UART_DTR_RISE                  (1<<8)              
#define UART_DTR_FALL                  (1<<9)              
#define UART_TX_MODEM_STATUS_U         (1<<16)             
#define UART_RX_DATA_AVAILABLE_U       (1<<17)             
#define UART_TX_DATA_NEEDED_U          (1<<18)             
#define UART_RX_TIMEOUT_U              (1<<19)             
#define UART_RX_LINE_ERR_U             (1<<20)             
#define UART_TX_DMA_DONE_U             (1<<21)             
#define UART_RX_DMA_DONE_U             (1<<22)             
#define UART_RX_DMA_TIMEOUT_U          (1<<23)             
#define UART_DTR_RISE_U                (1<<24)             
#define UART_DTR_FALL_U                (1<<25)             

//triggers
#define UART_RX_TRIGGER(n)             (((n)&0x3F)<<0)     
#define UART_TX_TRIGGER(n)             (((n)&0xF)<<8)      
#define UART_AFC_LEVEL(n)              (((n)&0x3F)<<16)    

//cmd_set
#define UART_RI                        (1<<0)              
#define UART_DCD                       (1<<1)              
#define UART_DSR                       (1<<2)              
#define UART_TX_BREAK_CONTROL          (1<<3)              
#define UART_TX_FINISH_N_WAIT          (1<<4)              
#define UART_RX_RTS                    (1<<5)              
#define UART_RX_FIFO_RESET             (1<<6)              
#define UART_TX_FIFO_RESET             (1<<7)              

//cmd_clr
#define UART_RI                        (1<<0)              
#define UART_DCR                       (1<<1)              
#define UART_DSR                       (1<<2)              
#define UART_TX_BREAK_CONTROL          (1<<3)              
#define UART_TX_FINISH_N_WAIT          (1<<4)              
#define UART_RX_CPU_RTS                (1<<5)              

//auto_baud
#define UART_AUTO_ENABLE               (1<<0)              
#define UART_AUTO_TRACKING             (1<<1)              
#define UART_VERIFY_2BYTE              (1<<2)              
#define UART_VERIFY_CHAR0(n)           (((n)&0xFF)<<8)     
#define UART_VERIFY_CHAR1(n)           (((n)&0xFF)<<16)    


#endif


