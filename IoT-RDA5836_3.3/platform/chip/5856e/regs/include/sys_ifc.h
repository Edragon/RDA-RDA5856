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
// File name      : sys_ifc                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:14                                                  
// Last modified  : 2016-09-29 11:13:14                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:14 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SYS_IFC_H__
#define __SYS_IFC_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#include "globals.h"
#include "8809p_generic_config.h"

#define hwp_sysIfc hwp_sys_ifc

// =============================================================================
//  MACROS
// =============================================================================
#define SYS_IFC_ADDR_ALIGN                       (0)
#define SYS_IFC_TC_LEN                           (23)
#define SYS_IFC_STD_CHAN_NB                      (SYS_IFC_NB_STD_CHANNEL)
#define SYS_IFC_RFSPI_CHAN                       (1)

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//get_ch
typedef union
{
    U32 v;
    struct
    {
        U32 ch_to_use                      :4 ; /*3 :0 , This field indicates which standard channel to use. Before using a channel, the CPU read this register to know which channel must be used. After reading this registers, the channel is to be regarded as busy. After reading this register, if the CPU doesn't want to use the specified channel, the CPU must write a disable_ in the control register of the channel to release the channel. 0000 = use Channel0 0001 = use Channel1 0010 = use Channel2 ... 0111 = use Channel7 1111 = all channels are busy*/
        U32 get_ch_reserved_0              :28; /*31:4 , No description.               */
    } b;
} T_SYS_IFC_get_ch;

//dma_status
typedef union
{
    U32 v;
    struct
    {
        U32 ch_enable                      :8 ; /*7 :0 , This register indicates which channel is eNonebled. It is a copy of the enable bit of the control register of each channel. One bit per channel, for example: 0000_0000 = All channels disable_d 0000_0001 = Ch0 enabled 0000_0010 = Ch1 enabled 0000_0100 = Ch2 enabled 0000_0101 = Ch0 and Ch2 enabled 0000_0111 = Ch0, Ch1 and Ch2 enabled 1111_1111 = all channels eNonebled*/
        U32 dma_status_reserved_1          :8 ; /*15:8 , No description.               */
        U32 ch_busy                        :7 ; /*22:16, This register indicates which standard channel is busy (this field doesn't include the RF_SPI channel). A standard channel is mark as busy, when a channel is eNonebled or a previous reading of the GET_CH register, the field CH_TO_USE indicates this channel. One bit per channel*/
        U32 dma_status_reserved_0          :9 ; /*31:23, No description.               */
    } b;
} T_SYS_IFC_dma_status;

//debug_status
typedef union
{
    U32 v;
    struct
    {
        U32 dbg_status                     :1 ; /*0 :0 , Debug Channel Status .0= The debug channel is running (not idle) 1= The debug channel is in idle mode*/
        U32 debug_status_reserved_0        :31; /*31:1 , No description.               */
    } b;
} T_SYS_IFC_debug_status;

//std_ch_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Channel Enable, write one in this bit eNoneble the channel. When the channel is eNonebled, for a peripheral to memory transfer the DMA wait request from peripheral to start transfer.*/
        U32 disable                        :1 ; /*1 :1 , Channel Disable, write one in this bit disable_ the channel. When writing one in this bit, the current AHB transfer and current APB transfer (if one in progress) is completed and the channel is then disable_d.*/
        U32 ch_rd_hw_exch                  :1 ; /*2 :2 , Read FIFO data exchange high 8-bit and low 8-bit. 0: Exchange; 1: No exchange.*/
        U32 ch_wr_hw_exch                  :1 ; /*3 :3 , Set Auto-disable_ mode0 = when TC reach zero the channel is not automatically released.1 = At the end of the transfer when TC reach zero the channel is automatically disable_d. the current channel is released.*/
        U32 autodisable                    :1 ; /*4 :4 , Set Auto-disable_ mode0 = when TC reach zero the channel is not automatically released.1 = At the end of the transfer when TC reach zero the channel is automatically disable_d. the current channel is released.*/
        U32 size                           :1 ; /*5 :5 , Peripheral Size 0= 8-bit peripheral 1= 32-bit peripheral*/
        U32 std_ch_ctrl_reserved_2         :2 ; /*7 :6 , No description.               */
        U32 req_src                        :3 ; /*10:8 , Select DMA Request source0: SYS_ID_TX_UART1: SYS_ID_RX_UART2: SYS_ID_TX_SDMMC3: SYS_ID_RX_SDMMC4: SYS_ID_TX_SPI15: SYS_ID_RX_SPI16: SYS_ID_TX_DEBUG_UART7: SYS_ID_RX_DEBUG_UART*/
        U32 std_ch_ctrl_reserved_1         :5 ; /*15:11, No description.               */
        U32 flush                          :1 ; /*16:16, When one, flush the interNonel FIFO channel. This bit must be used only in case of Rx transfer. Until this bit is 1, the APB request is masked. The flush doesn't release the channel. Before writting back this bit to zero the interNonel fifo must empty.*/
        U32 std_ch_ctrl_reserved_0         :15; /*31:17, No description.               */
    } b;
} T_SYS_IFC_std_ch_ctrl;

//std_ch_status
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Enable bit, when '1' the channel is running*/
        U32 std_ch_status_reserved_1       :3 ; /*3 :1 , No description.               */
        U32 fifo_empty                     :1 ; /*4 :4 , The internal channel fifo is empty*/
        U32 std_ch_status_reserved_0       :27; /*31:5 , No description.               */
    } b;
} T_SYS_IFC_std_ch_status;

//std_ch_start_addr
typedef union
{
    U32 v;
    struct
    {
        U32 start_addr                     :32; /*31:0 , AHB Address. This field represent the start address of the transfer. For a 32-bit peripheral, this address must be aligned 32-bit.*/
    } b;
} T_SYS_IFC_std_ch_start_addr;

//std_ch_tc
typedef union
{
    U32 v;
    struct
    {
        U32 tc                             :23; /*22:0 , Transfer Count, this field indicated the transfer size_ in bytes to perform. During a transfer a write in this register add the new value to the current TC. A read of this register return the current current transfer count.*/
        U32 std_ch_tc_reserved_0           :9 ; /*31:23, No description.               */
    } b;
} T_SYS_IFC_std_ch_tc;

typedef struct
{
    volatile           T_SYS_IFC_std_ch_ctrl                    std_ch_ctrl;
    volatile         T_SYS_IFC_std_ch_status                  std_ch_status;
    volatile     T_SYS_IFC_std_ch_start_addr              std_ch_start_addr;
    volatile             T_SYS_IFC_std_ch_tc                      std_ch_tc;

} T_sysifc_ch;

typedef struct
{
    volatile                T_SYS_IFC_get_ch                         get_ch; /*0x0  , RO   , 0x00000000, No description.               */
    volatile            T_SYS_IFC_dma_status                     dma_status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile          T_SYS_IFC_debug_status                   debug_status; /*0x8  , RO   , 0x00000001, No description.               */
    volatile                             U32                     reserved_1;
    volatile                     T_sysifc_ch                      std_ch[2]; /*     , sysifc_ch, NA        , struct block                  */
} T_HWP_SYS_IFC_T;
#else
typedef struct
{
    volatile            U32                    std_ch_ctrl;
    volatile            U32                  std_ch_status;
    volatile            U32              std_ch_start_addr;
    volatile            U32                      std_ch_tc;

} T_sysifc_ch;

typedef struct
{
    volatile                             U32                         get_ch; /*0x0  , RO   , 0x00000000, No description.               */
    volatile                             U32                     dma_status; /*0x4  , RO   , 0x00000000, No description.               */
    volatile                             U32                   debug_status; /*0x8  , RO   , 0x00000001, No description.               */
    volatile                             U32                     reserved_2;
    volatile                     T_sysifc_ch                      std_ch[2]; /*     , sysifc_ch, NA        , struct block                  */
} T_HWP_SYS_IFC_T;

//get_ch
#define SYS_IFC_CH_TO_USE(n)           (((n)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH0      (((0)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH1      (((1)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH2      (((2)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH3      (((3)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH4      (((4)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH5      (((5)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH6      (((6)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_USE_CH7      (((7)&0xF)<<0)      
#define SYS_IFC_CH_TO_USE_MASK         (0xF<<0)            
#define SYS_IFC_CH_TO_USE_SHIFT        (0)                 


//dma_status
#define SYS_IFC_CH_ENABLE(n)           (((n)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_ALL_CH_DISABLED (((0)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_CH0_ENABLED  (((1)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_CH1_ENABLED  (((2)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_CH2_ENABLED  (((3)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_CH_0_2_ENABLED (((4)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_CH_0_1_2_ENABLED (((5)&0xFF)<<0)     
#define SYS_IFC_CH_ENABLE_MASK         (0xFF<<0)           
#define SYS_IFC_CH_ENABLE_SHIFT        (0)                 

#define SYS_IFC_CH_BUSY(n)             (((n)&0x7F)<<16)    
#define SYS_IFC_CH_BUSY_MASK           (0x7F<<16)          
#define SYS_IFC_CH_BUSY_SHIFT          (16)                


//debug_status
#define SYS_IFC_DBG_STATUS             (1<<0)              
#define SYS_IFC_DBG_STATUS_DBG_CH_RUN  (0<<0)              
#define SYS_IFC_DBG_STATUS_DBG_CH_IDLE (1<<0)              

//std_ch_0__std_ch_ctrl
#define SYS_IFC_ENABLE                 (1<<0)              

#define SYS_IFC_DISABLE                (1<<1)              

#define SYS_IFC_CH_RD_HW_EXCH          (1<<2)              
#define SYS_IFC_CH_RD_HW_EXCH_EXCHANGE (0<<2)              
#define SYS_IFC_CH_RD_HW_EXCH_NO_EXCHANGE (1<<2)              

#define SYS_IFC_CH_WR_HW_EXCH          (1<<3)              
#define SYS_IFC_CH_WR_HW_EXCH_AUTO_DIS_MODE0 (0<<3)              
#define SYS_IFC_CH_WR_HW_EXCH_AUTO_DIS_MODE1 (1<<3)              

#define SYS_IFC_AUTODISABLE            (1<<4)              
#define SYS_IFC_AUTODISABLE_AUTO_DISABLE_CLOSE (0<<4)              
#define SYS_IFC_AUTODISABLE_AUTO_DISABLE_OPEN (1<<4)              

#define SYS_IFC_SIZE                   (1<<5)              
#define SYS_IFC_SIZE_PER_SIZE_8        (0<<5)              
#define SYS_IFC_SIZE_PER_SIZE_32       (1<<5)              

#define SYS_IFC_REQ_SRC(n)             (((n)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_TX_UART (((0)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_RX_UART (((1)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_TX_SDMMC (((2)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_RX_SDMMC (((3)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_TX_SPI1 (((4)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_RX_SPI1 (((5)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_TX_DEBUG_UART (((6)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_SYS_ID_RX_DEBUG_UART (((7)&0x7)<<8)      
#define SYS_IFC_REQ_SRC_MASK           (0x7<<8)            
#define SYS_IFC_REQ_SRC_SHIFT          (8)                 

#define SYS_IFC_FLUSH                  (1<<16)             


//std_ch_0__std_ch_status
#define SYS_IFC_ENABLE                 (1<<0)              

#define SYS_IFC_FIFO_EMPTY             (1<<4)              


//std_ch_0__std_ch_start_addr
#define SYS_IFC_START_ADDR(n)          (((n)&0xFFFFFFFF)<<0)
#define SYS_IFC_START_ADDR_MASK        (0xFFFFFFFF<<0)     
#define SYS_IFC_START_ADDR_SHIFT       (0)                 


//std_ch_0__std_ch_tc
#define SYS_IFC_TC(n)                  (((n)&0x7FFFFF)<<0) 
#define SYS_IFC_TC_MASK                (0x7FFFFF<<0)       
#define SYS_IFC_TC_SHIFT               (0)                 

#endif

#endif


