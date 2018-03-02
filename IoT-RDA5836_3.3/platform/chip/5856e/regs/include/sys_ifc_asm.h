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
// File name      : sys_ifc_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:15                                                  
// Last modified  : 2016-09-29 11:13:15                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:15 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SYS_IFC_ASM_H__
#define __SYS_IFC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_get_ch                     REG_SYS_IFC_BASE+0x00
#define REG_dma_status                 REG_SYS_IFC_BASE+0x04
#define REG_debug_status               REG_SYS_IFC_BASE+0x08

#define REG_std_ch_0__std_ch_ctrl      REG_SYS_IFC_BASE+0x10
#define REG_std_ch_0__std_ch_status    REG_SYS_IFC_BASE+0x14
#define REG_std_ch_0__std_ch_start_addr REG_SYS_IFC_BASE+0x18
#define REG_std_ch_0__std_ch_tc        REG_SYS_IFC_BASE+0x1c
#define REG_std_ch_1__std_ch_ctrl      REG_SYS_IFC_BASE+0x20
#define REG_std_ch_1__std_ch_status    REG_SYS_IFC_BASE+0x24
#define REG_std_ch_1__std_ch_start_addr REG_SYS_IFC_BASE+0x28
#define REG_std_ch_1__std_ch_tc        REG_SYS_IFC_BASE+0x2c

//get_ch
#define SYS_IFC_CH_TO_USE(n)           (((n)&0xF)<<0)      

//dma_status
#define SYS_IFC_CH_ENABLE(n)           (((n)&0xFF)<<0)     
#define SYS_IFC_CH_BUSY(n)             (((n)&0x7F)<<16)    

//debug_status
#define SYS_IFC_DBG_STATUS             (1<<0)              

//std_ch_0__std_ch_ctrl
#define SYS_IFC_ENABLE                 (1<<0)              
#define SYS_IFC_DISABLE                (1<<1)              
#define SYS_IFC_CH_RD_HW_EXCH          (1<<2)              
#define SYS_IFC_CH_WR_HW_EXCH          (1<<3)              
#define SYS_IFC_AUTODISABLE            (1<<4)              
#define SYS_IFC_SIZE                   (1<<5)              
#define SYS_IFC_REQ_SRC(n)             (((n)&0x7)<<8)      
#define SYS_IFC_FLUSH                  (1<<16)             

//std_ch_0__std_ch_status
#define SYS_IFC_ENABLE                 (1<<0)              
#define SYS_IFC_FIFO_EMPTY             (1<<4)              

//std_ch_0__std_ch_start_addr
#define SYS_IFC_START_ADDR(n)          (((n)&0xFFFFFFFF)<<0)

//std_ch_0__std_ch_tc
#define SYS_IFC_TC(n)                  (((n)&0x7FFFFF)<<0) 

//std_ch_1__std_ch_ctrl
#define SYS_IFC_ENABLE                 (1<<0)              
#define SYS_IFC_DISABLE                (1<<1)              
#define SYS_IFC_CH_RD_HW_EXCH          (1<<2)              
#define SYS_IFC_CH_WR_HW_EXCH          (1<<3)              
#define SYS_IFC_AUTODISABLE            (1<<4)              
#define SYS_IFC_SIZE                   (1<<5)              
#define SYS_IFC_REQ_SRC(n)             (((n)&0x7)<<8)      
#define SYS_IFC_FLUSH                  (1<<16)             

//std_ch_1__std_ch_status
#define SYS_IFC_ENABLE                 (1<<0)              
#define SYS_IFC_FIFO_EMPTY             (1<<4)              

//std_ch_1__std_ch_start_addr
#define SYS_IFC_START_ADDR(n)          (((n)&0xFFFFFFFF)<<0)

//std_ch_1__std_ch_tc
#define SYS_IFC_TC(n)                  (((n)&0x7FFFFF)<<0) 


#endif


