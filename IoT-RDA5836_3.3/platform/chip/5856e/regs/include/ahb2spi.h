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
// File name      : ahb2spi                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 13:50:57                                                  
// Last modified  : 2016-08-29 13:50:57                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 13:50:57 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB2SPI_H__
#define __AHB2SPI_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Enable the module and activate the chip select selected by CS_sel field.*/
        U32 clock_polarity                 :1 ; /*1 :1 , The spi clock polarity when '0' the clock disabled level is low, and the first edge is a rising edge. When '1' the clock disabled level is high, and the first edge is a falling edge.*/
        U32 clock_delay                    :2 ; /*3 :2 , Transfer start to first edge delay value from 0 to 2 is the number of spi clock half period between the CS activation and the first clock edge.*/
        U32 do_delay                       :2 ; /*5 :4 , Transfer start to first data out delay value from 0 to 2 is the number of spi clock half period between the CS activation and the first data out*/
        U32 di_delay                       :2 ; /*7 :6 , Transfer start to first data in sample delay value from 1 to 3 is the number of spi clock half period between the CS activation and the first data in sampled. */
        U32 cs_delay                       :2 ; /*9 :8 , Transfer end to chip select deactivation delay value from 0 to 3 is the number of spi clock half period between the end of transfer and CS deactivation. The real_cs_delay=cs_delay + do_delay + di_delay*/
        U32 cs_pulse                       :2 ; /*11:10, Chip select deactivation to reactivation minimum delay value from 1 to 3 is the number of spi clock half period between the CS deactivation and a new CS activation */
        U32 rw_polarity                    :1 ; /*12:12, rw_polarity=0:    spi_do output RW controller 1=write  0 = read; rw_polarity=1:    spi_do output RW controller 0=write  1 =read.*/
        U32 int_mask                       :1 ; /*13:13, interrupt mask, 0 valid       */
        U32 tx_finish_mask                 :1 ; /*14:14, trans finish mask, 0 valid.   */
        U32 fifo_empty_mask                :1 ; /*15:15, fifo empty mask, 0 valid.     */
        U32 fifo_full_mask                 :1 ; /*16:16, fifo full mask, 0 valid.      */
        U32 spi_clk_dividor                :10; /*26:17, spi clk dividor               */
        U32 cs_polarity                    :4 ; /*30:27, spi_cs polarity               */
        U32 fifo_flush                     :1 ; /*31:31, fifo flush                    */
    } b;
} T_AHB2SPI_ctrl;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 spi_int                        :1 ; /*0 :0 , when int_mask=1:    int=1, SPI interrupt, Writing an '1' clear this interrupt.When int_mask=0:    int=0.*/
        U32 tx_finish                      :1 ; /*1 :1 , when tx_finish_mask=1:    tx_finish=0: A transfer is in progress    tx_finish=1: A transfer is finished.When tx_finish_mask=0:    tx_finish=0.*/
        U32 fifo_empty                     :1 ; /*2 :2 , fifo empty status,  1 valid. when fifo_empty_mask=0, fifo_empty=0.*/
        U32 fifo_full                      :1 ; /*3 :3 , fifo full status, 1 valid. When fifo_full_mask=0, fifo_full=0.*/
        U32 tx_space                       :3 ; /*6 :4 , Transmit FIFO Space Number of empty spot in the FIFO*/
        U32 status_reserved_0              :25; /*31:7 , No description.               */
    } b;
} T_AHB2SPI_status;

//frame_size
typedef union
{
    U32 v;
    struct
    {
        U32 ch0_data_width                 :2 ; /*1 :0 , 2'h0:16bits           2'h1:22bits           2'h2:24bits           2'h3:reserved*/
        U32 ch0_addr_width                 :2 ; /*3 :2 , 0:7bits;1:8bits;2:9bits.    */
        U32 ch1_data_width                 :2 ; /*5 :4 , 2'h0:16bits           2'h1:22bits           2'h2:24bits           2'h3:reserved*/
        U32 ch1_addr_width                 :2 ; /*7 :6 , 0:7bits;1:8bits;2:9bits.    */
        U32 ch2_data_width                 :2 ; /*9 :8 , 2'h0:16bits           2'h1:22bits           2'h2:24bits           2'h3:reserved*/
        U32 ch2_addr_width                 :2 ; /*11:10, 0:7bits;1:8bits;2:9bits.    */
        U32 ch3_data_width                 :2 ; /*13:12, 2'h0:16bits2'h1:22bits2'h2:24bits           2'h3:reserved*/
        U32 ch3_addr_width                 :2 ; /*15:14, 0:7bits;1:8bits;2:9bits.    */
        U32 frame_size_reserved_0          :16; /*31:16, No description.               */
    } b;
} T_AHB2SPI_frame_size;

//raw_status
typedef union
{
    U32 v;
    struct
    {
        U32 int_raw                        :1 ; /*0 :0 , SPI interrupt                 */
        U32 tx_finish_raw                  :1 ; /*1 :1 , 0: A transfer is in progress,1: A transfer is finished*/
        U32 fifo_empty_raw                 :1 ; /*2 :2 , fifo empty status             */
        U32 fifo_full_raw                  :1 ; /*3 :3 , fifo full status              */
        U32 raw_status_reserved_0          :28; /*31:4 , No description.               */
    } b;
} T_AHB2SPI_raw_status;

//feature
typedef union
{
    U32 v;
    struct
    {
        U32 pos_simp_neg_send              :1 ; /*0 :0 , 1: the spi slaver use the master sclk posedge to simple data and negedge to send data0: the spi slaver use the master sclk posedge to simple data and posedge to send data*/
        U32 fifo_depth                     :3 ; /*3 :1 , use the software way to change the fifo depth*/
        U32 feature_reserved_0             :28; /*31:4 , No description.               */
    } b;
} T_AHB2SPI_feature;

typedef struct
{
    volatile                  T_AHB2SPI_ctrl                           ctrl; /*0x0  , RW   , 0x7FFE0D44, No description.               */
    volatile                T_AHB2SPI_status                         status; /*0x4  , RO   , 0x00000040, No description.               */
    volatile            T_AHB2SPI_frame_size                     frame_size; /*0x8  , RW   , 0x00000000, No description.               */
    volatile            T_AHB2SPI_raw_status                     raw_status; /*0xc  , RW   , 0x00000004, No description.               */
    volatile               T_AHB2SPI_feature                        feature; /*0x10 , RW   , 0x00000008, No description.               */
} T_HWP_AHB2SPI_T;
#else
typedef struct
{
    volatile                             U32                           ctrl; /*0x0  , RW   , 0x7FFE0D44, No description.               */
    volatile                             U32                         status; /*0x4  , RO   , 0x00000040, No description.               */
    volatile                             U32                     frame_size; /*0x8  , RW   , 0x00000000, No description.               */
    volatile                             U32                     raw_status; /*0xc  , RW   , 0x00000004, No description.               */
    volatile                             U32                        feature; /*0x10 , RW   , 0x00000008, No description.               */
} T_HWP_AHB2SPI_T;

//ctrl
#define AHB2SPI_ENABLE                 (1<<0)              

#define AHB2SPI_CLOCK_POLARITY         (1<<1)              

#define AHB2SPI_CLOCK_DELAY(n)         (((n)&0x3)<<2)      
#define AHB2SPI_CLOCK_DELAY_MASK       (0x3<<2)            
#define AHB2SPI_CLOCK_DELAY_SHIFT      (2)                 

#define AHB2SPI_DO_DELAY(n)            (((n)&0x3)<<4)      
#define AHB2SPI_DO_DELAY_MASK          (0x3<<4)            
#define AHB2SPI_DO_DELAY_SHIFT         (4)                 

#define AHB2SPI_DI_DELAY(n)            (((n)&0x3)<<6)      
#define AHB2SPI_DI_DELAY_MASK          (0x3<<6)            
#define AHB2SPI_DI_DELAY_SHIFT         (6)                 

#define AHB2SPI_CS_DELAY(n)            (((n)&0x3)<<8)      
#define AHB2SPI_CS_DELAY_MASK          (0x3<<8)            
#define AHB2SPI_CS_DELAY_SHIFT         (8)                 

#define AHB2SPI_CS_PULSE(n)            (((n)&0x3)<<10)     
#define AHB2SPI_CS_PULSE_MASK          (0x3<<10)           
#define AHB2SPI_CS_PULSE_SHIFT         (10)                

#define AHB2SPI_RW_POLARITY            (1<<12)             

#define AHB2SPI_INT_MASK               (1<<13)             

#define AHB2SPI_TX_FINISH_MASK         (1<<14)             

#define AHB2SPI_FIFO_EMPTY_MASK        (1<<15)             

#define AHB2SPI_FIFO_FULL_MASK         (1<<16)             

#define AHB2SPI_SPI_CLK_DIVIDOR(n)     (((n)&0x3FF)<<17)   
#define AHB2SPI_SPI_CLK_DIVIDOR_MASK   (0x3FF<<17)         
#define AHB2SPI_SPI_CLK_DIVIDOR_SHIFT  (17)                

#define AHB2SPI_CS_POLARITY(n)         (((n)&0xF)<<27)     
#define AHB2SPI_CS_POLARITY_MASK       (0xF<<27)           
#define AHB2SPI_CS_POLARITY_SHIFT      (27)                

#define AHB2SPI_FIFO_FLUSH             (1<<31)             


//status
#define AHB2SPI_SPI_INT                (1<<0)              

#define AHB2SPI_TX_FINISH              (1<<1)              

#define AHB2SPI_FIFO_EMPTY             (1<<2)              

#define AHB2SPI_FIFO_FULL              (1<<3)              

#define AHB2SPI_TX_SPACE(n)            (((n)&0x7)<<4)      
#define AHB2SPI_TX_SPACE_MASK          (0x7<<4)            
#define AHB2SPI_TX_SPACE_SHIFT         (4)                 


//frame_size
#define AHB2SPI_CH0_DATA_WIDTH(n)      (((n)&0x3)<<0)      
#define AHB2SPI_CH0_DATA_WIDTH_16BITS  (((0)&0x3)<<0)      
#define AHB2SPI_CH0_DATA_WIDTH_22BITS  (((1)&0x3)<<0)      
#define AHB2SPI_CH0_DATA_WIDTH_24BITS  (((2)&0x3)<<0)      
#define AHB2SPI_CH0_DATA_WIDTH_RESERVED (((3)&0x3)<<0)      
#define AHB2SPI_CH0_DATA_WIDTH_MASK    (0x3<<0)            
#define AHB2SPI_CH0_DATA_WIDTH_SHIFT   (0)                 

#define AHB2SPI_CH0_ADDR_WIDTH(n)      (((n)&0x3)<<2)      
#define AHB2SPI_CH0_ADDR_WIDTH_7BITS;  (((0)&0x3)<<2)      
#define AHB2SPI_CH0_ADDR_WIDTH_8BITS;  (((1)&0x3)<<2)      
#define AHB2SPI_CH0_ADDR_WIDTH_9BITS.  (((2)&0x3)<<2)      
#define AHB2SPI_CH0_ADDR_WIDTH_MASK    (0x3<<2)            
#define AHB2SPI_CH0_ADDR_WIDTH_SHIFT   (2)                 

#define AHB2SPI_CH1_DATA_WIDTH(n)      (((n)&0x3)<<4)      
#define AHB2SPI_CH1_DATA_WIDTH_16BITS  (((0)&0x3)<<4)      
#define AHB2SPI_CH1_DATA_WIDTH_22BITS  (((1)&0x3)<<4)      
#define AHB2SPI_CH1_DATA_WIDTH_24BITS  (((2)&0x3)<<4)      
#define AHB2SPI_CH1_DATA_WIDTH_RESERVED (((3)&0x3)<<4)      
#define AHB2SPI_CH1_DATA_WIDTH_MASK    (0x3<<4)            
#define AHB2SPI_CH1_DATA_WIDTH_SHIFT   (4)                 

#define AHB2SPI_CH1_ADDR_WIDTH(n)      (((n)&0x3)<<6)      
#define AHB2SPI_CH1_ADDR_WIDTH_7BITS;  (((0)&0x3)<<6)      
#define AHB2SPI_CH1_ADDR_WIDTH_8BITS;  (((1)&0x3)<<6)      
#define AHB2SPI_CH1_ADDR_WIDTH_9BITS.  (((2)&0x3)<<6)      
#define AHB2SPI_CH1_ADDR_WIDTH_MASK    (0x3<<6)            
#define AHB2SPI_CH1_ADDR_WIDTH_SHIFT   (6)                 

#define AHB2SPI_CH2_DATA_WIDTH(n)      (((n)&0x3)<<8)      
#define AHB2SPI_CH2_DATA_WIDTH_16BITS  (((0)&0x3)<<8)      
#define AHB2SPI_CH2_DATA_WIDTH_22BITS  (((1)&0x3)<<8)      
#define AHB2SPI_CH2_DATA_WIDTH_24BITS  (((2)&0x3)<<8)      
#define AHB2SPI_CH2_DATA_WIDTH_RESERVED (((3)&0x3)<<8)      
#define AHB2SPI_CH2_DATA_WIDTH_MASK    (0x3<<8)            
#define AHB2SPI_CH2_DATA_WIDTH_SHIFT   (8)                 

#define AHB2SPI_CH2_ADDR_WIDTH(n)      (((n)&0x3)<<10)     
#define AHB2SPI_CH2_ADDR_WIDTH_7BITS;  (((0)&0x3)<<10)     
#define AHB2SPI_CH2_ADDR_WIDTH_8BITS;  (((1)&0x3)<<10)     
#define AHB2SPI_CH2_ADDR_WIDTH_9BITS.  (((2)&0x3)<<10)     
#define AHB2SPI_CH2_ADDR_WIDTH_MASK    (0x3<<10)           
#define AHB2SPI_CH2_ADDR_WIDTH_SHIFT   (10)                

#define AHB2SPI_CH3_DATA_WIDTH(n)      (((n)&0x3)<<12)     
#define AHB2SPI_CH3_DATA_WIDTH_16BITS  (((0)&0x3)<<12)     
#define AHB2SPI_CH3_DATA_WIDTH_22BITS  (((1)&0x3)<<12)     
#define AHB2SPI_CH3_DATA_WIDTH_24BITS  (((2)&0x3)<<12)     
#define AHB2SPI_CH3_DATA_WIDTH_RESERVED (((3)&0x3)<<12)     
#define AHB2SPI_CH3_DATA_WIDTH_MASK    (0x3<<12)           
#define AHB2SPI_CH3_DATA_WIDTH_SHIFT   (12)                

#define AHB2SPI_CH3_ADDR_WIDTH(n)      (((n)&0x3)<<14)     
#define AHB2SPI_CH3_ADDR_WIDTH_7BITS;  (((0)&0x3)<<14)     
#define AHB2SPI_CH3_ADDR_WIDTH_8BITS;  (((1)&0x3)<<14)     
#define AHB2SPI_CH3_ADDR_WIDTH_9BITS.  (((2)&0x3)<<14)     
#define AHB2SPI_CH3_ADDR_WIDTH_MASK    (0x3<<14)           
#define AHB2SPI_CH3_ADDR_WIDTH_SHIFT   (14)                


//raw_status
#define AHB2SPI_INT_RAW                (1<<0)              

#define AHB2SPI_TX_FINISH_RAW          (1<<1)              
#define AHB2SPI_TX_FINISH_RAW_A        (1<<1)              
#define AHB2SPI_TX_FINISH_RAW_A        (1<<1)              

#define AHB2SPI_FIFO_EMPTY_RAW         (1<<2)              

#define AHB2SPI_FIFO_FULL_RAW          (1<<3)              


//feature
#define AHB2SPI_POS_SIMP_NEG_SEND      (1<<0)              
#define AHB2SPI_POS_SIMP_NEG_SEND_THE  (1<<0)              
#define AHB2SPI_POS_SIMP_NEG_SEND_THE  (1<<0)              

#define AHB2SPI_FIFO_DEPTH(n)          (((n)&0x7)<<1)      
#define AHB2SPI_FIFO_DEPTH_MASK        (0x7<<1)            
#define AHB2SPI_FIFO_DEPTH_SHIFT       (1)                 


#endif

#endif


