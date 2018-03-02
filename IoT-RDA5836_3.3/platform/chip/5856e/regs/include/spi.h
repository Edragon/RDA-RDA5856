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
// File name      : spi                                            
// Author         : liqiu                                                 
// Date of created: 2016-09-29 11:13:13                                                  
// Last modified  : 2016-09-29 11:13:13                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-09-29 11:13:13 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SPI_H__
#define __SPI_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define HWP_SPI_T T_HWP_SPI_T

#define hwp_spi1		(T_HWP_SPI_T*)hwp_spi
#define hwp_spi2        (T_HWP_SPI_T*)hwp_spi
#define hwp_spi3        (T_HWP_SPI_T*)hwp_spi

// =============================================================================
//  MACROS
// =============================================================================
#define SPI_TX_FIFO_SIZE                         (4)
#define SPI_RX_FIFO_SIZE                         (4)

#define REG_SPI1_BASE               0x01A11000
#define REG_SPI2_BASE               0x01A12000
#define REG_SPI3_BASE               0x01A13000

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 enable                         :1 ; /*0 :0 , Enable the module and activate the chip select selected by CS_sel field.*/
        U32 cs_sel                         :2 ; /*2 :1 , Selects the active CS.0  :  CS01  :  CS12  :  CS23  :  CS3*/
        U32 ctrl_reserved_0                :1 ; /*3 :3 , No description.               */
        U32 input_mode                     :1 ; /*4 :4 , When set to 1 the inputs are activated, else only the output is driven and no data are stored in the receive FIFO. Notes: The Input_mode bit status is also readable onto the bit rxtx_buffer[31].*/
        U32 clock_polarity                 :1 ; /*5 :5 , The spi clock polarity when '0' the clock disabled level is low, and the first edge is a rising edge. When '1' the clock disabled level is high, and the first edge is a falling edge.*/
        U32 clock_delay                    :2 ; /*7 :6 , Transfer start to first edge delay value from 0 to 2 is the number of spi clock half period between the CS activation and the first clock edge.*/
        U32 do_delay                       :2 ; /*9 :8 , Transfer start to first data out delay value from 0 to 2 is the number of spi clock half period between the CS activation and the first data out*/
        U32 di_delay                       :2 ; /*11:10, Transfer start to first data in sample delay value from 0 to 3 is the number of spi clock half period between the CS activation and the first data in sampled. NOTE: DI_Delay must be less or equal to DO_Delay + CS_Delay + 2. In other words DI_Delay can be 3 only if DO_Delay and CS_Delay are not both equal to 0.*/
        U32 cs_delay                       :2 ; /*13:12, Transfer end to chip select deactivation delay value from 0 to 3 is the number of spi clock half period between the end of transfer and CS deactivation*/
        U32 cs_pulse                       :2 ; /*15:14, Chip select deactivation to reactivation minimum delay value from 0 to 3 is the number of spi clock half period between the CS deactivation and a new CS activation (CS will activate only if more data are available in the transmit FIFO)*/
        U32 frame_size                     :6 ; /*21:16, Frame Size The frame size is the binary value of this register + 1 valid value are 3 to 31 (frame size 4 to 32bits)*/
        U32 lcd_3_line                     :1 ; /*22:22, 0:spi lcd 4-lines mode1: spi lcd 3-lines mode*/
        U32 dcx_oen                        :1 ; /*23:23, 0:output valid                */
        U32 oe_delay                       :5 ; /*28:24, OE delay When 0: regular mode, SPI_DO pin as output only. Value from 1 to 31 is the number of data out to transfert before the SPI_DO pin switch to input.*/
        U32 ctrl_data_mux_sel              :1 ; /*29:29, Selects the active CS and Input_reg either from the ctrl or rxtx_buffer register. If SPI FIFO 8b or 32b, when set to "0": CS from CS_sel and INPUT from Input_mode in the register ctrl. Only if SPI FIFO 32b, when set to "1": CS and INPUT from SPI DATA.(Do not work for FIFO8b)0  :   Ctrl_reg_sel1  :   Data_reg_sel*/
        U32 input_sel                      :2 ; /*31:30, Selects the input line to be used as SPI data in.(Not used for SPI3) 2'b00 : SPI_DI_0  when "00" the SPI_DI_0 is used. 2'b01 : SPI_DI_1  When "01" the SPI_DI_1 is used. 2'b10 : SPI_DI_2  When "10" the SPI_DI_2 is used. When "11" reserved.*/
    } b;
} T_SPI_ctrl;

//status
typedef union
{
    U32 v;
    struct
    {
        U32 active_status                  :1 ; /*0 :0 , 0: NO_TRANS 1: TRANS when a transfer is in progress.*/
        U32 status_reserved_4              :2 ; /*2 :1 , No description.               */
        U32 cause_rx_ovf_irq               :1 ; /*3 :3 , The receive FIFO overflow irq cause. Writing a '1' clear the receive overflow status and cause.*/
        U32 cause_tx_th_irq                :1 ; /*4 :4 , The transmit FIFO overflow threshold irq cause.*/
        U32 cause_tx_dma_irq               :1 ; /*5 :5 , The transmit Dma Done irq cause. Writing a '1' clear the transmit Dma Done status and cause.*/
        U32 cause_rx_th_irq                :1 ; /*6 :6 , The receive FIFO threshold irq cause.*/
        U32 cause_rx_dma_irq               :1 ; /*7 :7 , The receive Dma Done irq cause. Writing a '1' clear the receive Dma Done status and cause.*/
        U32 status_reserved_3              :1 ; /*8 :8 , No description.               */
        U32 tx_ovf                         :1 ; /*9 :9 , The transmit FIFO overflow status. Writing a '1' clear the transmit overflow status and cause.*/
        U32 rx_udf                         :1 ; /*10:10, The receive FIFO underflow status. Writing a '1' clear the receive underflow status and cause.*/
        U32 rx_ovf                         :1 ; /*11:11, The receive FIFO overflow status. Writing a '1' clear the receive overflow status and cause.*/
        U32 tx_th                          :1 ; /*12:12, The transmit FIFO threshold status.*/
        U32 tx_dma_done                    :1 ; /*13:13, The transmit Dma Done status. Writing a '1' clear the transmit Dma Done status and cause.*/
        U32 rx_th                          :1 ; /*14:14, The receive FIFO threshold status.*/
        U32 rx_dma_done                    :1 ; /*15:15, The receive Dma Done status. Writing a '1' clear the receive Dma Done status and cause.*/
        U32 tx_space                       :5 ; /*20:16, Transmit FIFO Space Number of empty spot in the FIFO*/
        U32 status_reserved_2              :3 ; /*23:21, No description.               */
        U32 rx_level                       :5 ; /*28:24, Receive FIFO level Number of DATA in the FIFO*/
        U32 status_reserved_1              :1 ; /*29:29, No description.               */
        U32 fifo_flush                     :1 ; /*30:30, write 1 to flush fifo         */
        U32 status_reserved_0              :1 ; /*31:31, No description.               */
    } b;
} T_SPI_status;

//rxtx_buffer
typedef union
{
    U32 v;
    struct
    {
        U32 data                           :29; /*28:0 , Write to the transmit FIFO or read in the receive fifo.*/
        U32 cs                             :2 ; /*30:29, Chip Select on which write the data written in the Fifo.*/
        U32 input_data                     :1 ; /*31:31, Set this bit to one when the data received while sending this peculiar data are expected to be kept in the FIFO, otherwise no data is recorded in the FIFO.*/
    } b;
} T_SPI_rxtx_buffer;

//cfg
typedef union
{
    U32 v;
    struct
    {
        U32 cs_polarity_0                  :1 ; /*0 :0 , Chip select polarity0  :  active_high       chip select is active high1  :  active_low       chip select is active low*/
        U32 cs_polarity_1                  :1 ; /*1 :1 , Chip select polarity0  :  active_high       chip select is active high1  :  active_low       chip select is active low*/
        U32 cfg_reserved_2                 :14; /*15:2 , No description.               */
        U32 clock_divider                  :10; /*25:16, Clock Divider The state machine clock is generated by dividing the system clock by the value of this register + 1. So the output clock is divided by (register + 1)*2*/
        U32 cfg_reserved_1                 :2 ; /*27:26, No description.               */
        U32 clock_limiter                  :1 ; /*28:28, When enabled the clock input to the divider is not the system clock, but a limited version of it: It cannot be above 52MHz, so the output clock will never be above 26MHz. for system clock of 104Mhz the clock input to the divider is 52Mhz, for system clock of 78Mhz the clock input to the divider is 39Mhz, for lower system clock value, the input to the divider is the system clock.*/
        U32 cfg_reserved_0                 :3 ; /*31:29, No description.               */
    } b;
} T_SPI_cfg;

//pattern
typedef union
{
    U32 v;
    struct
    {
        U32 pattern                        :8 ; /*7 :0 , MMC Pattern value for RX pattern match mode.*/
        U32 pattern_mode                   :1 ; /*8 :8 , Enable the pattern mode.0  :  disabled      Spi Behaviour.1  :  enabled      Pattern matching.*/
        U32 pattern_selector               :1 ; /*9 :9 , Select the RX pattern matching mode when the pattern_mode is enabled( set 1). Used for SD/MMC SPI mode.0  :  UNTIL      No datas are written into the RX FIFO UNTIL the received data is equal to the pattern.1  :  WHILE      No datas are written into the RX FIFO WHILE the received data is equal to the pattern.*/
        U32 pattern_reserved_0             :22; /*31:10, No description.               */
    } b;
} T_SPI_pattern;

//stream
typedef union
{
    U32 v;
    struct
    {
        U32 tx_stream_bit                  :1 ; /*0 :0 , When TX stream mode is enabled, once the TX fifo is empty, all new bits send have the value of this bit.0  :  zero1  :  one*/
        U32 stream_reserved_2              :7 ; /*7 :1 , No description.               */
        U32 tx_stream_mode                 :1 ; /*8 :8 , Enable the TX stream mode. Used for SD/MMC SPI mode. When enabled, this mode provide infinite bit stream for sending, after fifo is empty the extra bits generated all have the same value. The value is in tx_stream_bit.0  :  disabled1  :  enabled*/
        U32 stream_reserved_1              :7 ; /*15:9 , No description.               */
        U32 tx_stream_stop_with_rx_dma_done :1 ; /*16:16, Allow to automatically clear the tx_stream_mode when Rx_Dma_Done is set.0  :   disabled1  :   enabled*/
        U32 stream_reserved_0              :15; /*31:17, No description.               */
    } b;
} T_SPI_stream;

//pin_control
typedef union
{
    U32 v;
    struct
    {
        U32 clk_ctrl                       :2 ; /*1 :0 , 0  :   Spi_Ctrl       The Spi_Clk pin is set OUTPUT(Basic SPI Behaviour).1  :   Input_Ctrl       The Spi_Clk pin is set INPUT (High Impedance).2  :   Force_0_Ctrl       The Spi_Clk pin is set OUTPUT and forced to 0.3  :   Force_1_Ctrl       The Spi_Clk pin is set OUTPUT and forced to 1.*/
        U32 do_ctrl                        :2 ; /*3 :2 , 0  :   Spi_Ctrl       The Spi_DO pin is set OUTPUT(Basic SPI Behaviour).1  :   Input_Ctrl       The Spi_DO pin is set INPUT (High Impedance).2  :   Force_0_Ctrl       The Spi_DO pin is set OUTPUT and forced to 0.3  :   Force_1_Ctrl       The Spi_DO pin is set OUTPUT and forced to 1.*/
        U32 cs0_ctrl                       :2 ; /*5 :4 ,  0  :   Spi_Ctrl       The Spi_CS0 pin is set OUTPUT(Basic SPI Behaviour).1  :   Input_Ctrl       The Spi_CS0 pin is set INPUT (High Impedance).2  :   Force_0_Ctrl       The Spi_CS0 pin is set OUTPUT and forced to 0.3  :   Force_1_Ctrl      The Spi_CS0 pin is set OUTPUT and forced to 1.*/
        U32 cs1_ctrl                       :2 ; /*7 :6 ,  0  :   Spi_Ctrl       The Spi_CS1 pin is set OUTPUT(Basic SPI Behaviour). 1  :   Input_Ctrl       The Spi_CS1 pin is set INPUT (High Impedance). 2  :   Force_0_Ctrl       The Spi_CS1 pin is set OUTPUT and forced to 0. 3  :   Force_1_Ctrl      The Spi_CS1 pin is set OUTPUT and forced to 1.*/
        U32 pin_control_reserved_0         :24; /*31:8 , No description.               */
    } b;
} T_SPI_pin_control;

//irq
typedef union
{
    U32 v;
    struct
    {
        U32 mask_rx_ovf_irq                :1 ; /*0 :0 , Mask the receive FIFO overflow irq*/
        U32 mask_tx_th_irq                 :1 ; /*1 :1 , Mask the transmit FIFO threshold irq*/
        U32 mask_tx_dma_irq                :1 ; /*2 :2 , Mask the transmit Dma Done irq*/
        U32 mask_rx_th_irq                 :1 ; /*3 :3 , Mask the receive FIFO threshold irq*/
        U32 mask_rx_dma_irq                :1 ; /*4 :4 , Mask the receive DMA Done irq */
        U32 tx_threshold                   :2 ; /*6 :5 , Transmit FIFO threshold this threshold is used to generate the irq.0  :   1_Empty_Slot1  :   2_Empty_Slots2  :   3_Empty_Slots3  :   4_Empty_Slots*/
        U32 rx_threshold                   :2 ; /*8 :7 , Receive FIFO threshold this threshold is used to generate the irq.0  :   1_Valid_Data1  :   2_Valid_Data2  :   3_Valid_Data3  :   4_Valid_Data*/
        U32 irq_reserved_0                 :23; /*31:9 , No description.               */
    } b;
} T_SPI_irq;

typedef struct
{
    volatile                      T_SPI_ctrl                           ctrl; /*0x0  , RW   , 0x1F7FFFF0, No description.               */
    volatile                    T_SPI_status                         status; /*0x4  , RW   , 0x00041000, No description.               */
    volatile               T_SPI_rxtx_buffer                    rxtx_buffer; /*0x8  , RW   , 0x80000000, No description.               */
    volatile                       T_SPI_cfg                            cfg; /*0xc  , RW   , 0x13FF0003, No description.               */
    volatile                   T_SPI_pattern                        pattern; /*0x10 , RW   , 0x00000000, No description.               */
    volatile                    T_SPI_stream                         stream; /*0x14 , RW   , 0x00000000, No description.               */
    volatile               T_SPI_pin_control                    pin_control; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                       T_SPI_irq                            irq; /*0x1c , RW   , 0x000001E0, No description.               */
} T_HWP_SPI_T;
#else
typedef struct
{
    volatile                             U32                           ctrl; /*0x0  , RW   , 0x1F7FFFF0, No description.               */
    volatile                             U32                         status; /*0x4  , RW   , 0x00041000, No description.               */
    volatile                             U32                    rxtx_buffer; /*0x8  , RW   , 0x80000000, No description.               */
    volatile                             U32                            cfg; /*0xc  , RW   , 0x13FF0003, No description.               */
    volatile                             U32                        pattern; /*0x10 , RW   , 0x00000000, No description.               */
    volatile                             U32                         stream; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                             U32                    pin_control; /*0x18 , RW   , 0x00000000, No description.               */
    volatile                             U32                            irq; /*0x1c , RW   , 0x000001E0, No description.               */
} T_HWP_SPI_T;

//ctrl
#define SPI_ENABLE                     (1<<0)              

#define SPI_CS_SEL(n)                  (((n)&0x3)<<1)      
#define SPI_CS_SEL_CS0                 (((0)&0x3)<<1)      
#define SPI_CS_SEL_CS1                 (((1)&0x3)<<1)      
#define SPI_CS_SEL_CS2                 (((2)&0x3)<<1)      
#define SPI_CS_SEL_CS3                 (((3)&0x3)<<1)      
#define SPI_CS_SEL_MASK                (0x3<<1)            
#define SPI_CS_SEL_SHIFT               (1)                 

#define SPI_INPUT_MODE                 (1<<4)              

#define SPI_CLOCK_POLARITY             (1<<5)              

#define SPI_CLOCK_DELAY(n)             (((n)&0x3)<<6)      
#define SPI_CLOCK_DELAY_MASK           (0x3<<6)            
#define SPI_CLOCK_DELAY_SHIFT          (6)                 

#define SPI_DO_DELAY(n)                (((n)&0x3)<<8)      
#define SPI_DO_DELAY_MASK              (0x3<<8)            
#define SPI_DO_DELAY_SHIFT             (8)                 

#define SPI_DI_DELAY(n)                (((n)&0x3)<<10)     
#define SPI_DI_DELAY_MASK              (0x3<<10)           
#define SPI_DI_DELAY_SHIFT             (10)                

#define SPI_CS_DELAY(n)                (((n)&0x3)<<12)     
#define SPI_CS_DELAY_MASK              (0x3<<12)           
#define SPI_CS_DELAY_SHIFT             (12)                

#define SPI_CS_PULSE(n)                (((n)&0x3)<<14)     
#define SPI_CS_PULSE_MASK              (0x3<<14)           
#define SPI_CS_PULSE_SHIFT             (14)                

#define SPI_FRAME_SIZE(n)              (((n)&0x3F)<<16)    
#define SPI_FRAME_SIZE_MASK            (0x3F<<16)          
#define SPI_FRAME_SIZE_SHIFT           (16)                

#define SPI_LCD_3_LINE                 (1<<22)             

#define SPI_DCX_OEN                    (1<<23)             

#define SPI_OE_DELAY(n)                (((n)&0x1F)<<24)    
#define SPI_OE_DELAY_MASK              (0x1F<<24)          
#define SPI_OE_DELAY_SHIFT             (24)                

#define SPI_CTRL_DATA_MUX_SEL          (1<<29)             
#define SPI_CTRL_DATA_MUX_SEL_CTRL_REG_SEL (0<<29)             
#define SPI_CTRL_DATA_MUX_SEL_DATA_REG_SEL (1<<29)             

#define SPI_INPUT_SEL(n)               (((n)&0x3)<<30)     
#define SPI_INPUT_SEL_SPI_DI_0         (((0)&0x3)<<30)     
#define SPI_INPUT_SEL_SPI_DI_1         (((1)&0x3)<<30)     
#define SPI_INPUT_SEL_SPI_DI_2         (((2)&0x3)<<30)     
#define SPI_INPUT_SEL_MASK             (0x3<<30)           
#define SPI_INPUT_SEL_SHIFT            (30)                


//status
#define SPI_ACTIVE_STATUS              (1<<0)              
#define SPI_ACTIVE_STATUS_NO_TRANS     (0<<0)              
#define SPI_ACTIVE_STATUS_TRANS        (1<<0)              

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
#define SPI_TX_SPACE_MASK              (0x1F<<16)          
#define SPI_TX_SPACE_SHIFT             (16)                

#define SPI_RX_LEVEL(n)                (((n)&0x1F)<<24)    
#define SPI_RX_LEVEL_MASK              (0x1F<<24)          
#define SPI_RX_LEVEL_SHIFT             (24)   

#define SPI_FIFO_FLUSH                 (1<<30)             



//rxtx_buffer
#define SPI_DATA(n)                    (((n)&0x1FFFFFFF)<<0)
#define SPI_DATA_MASK                  (0x1FFFFFFF<<0)     
#define SPI_DATA_SHIFT                 (0)                 

#define SPI_CS(n)                      (((n)&0x3)<<29)     
#define SPI_CS_MASK                    (0x3<<29)           
#define SPI_CS_SHIFT                   (29)                

#define SPI_INPUT_DATA                 (1<<31)             


//cfg
#define SPI_CS_POLARITY_0              (1<<0)              
#define SPI_CS_POLARITY_0_ACTIVE_HIGH  (0<<0)              
#define SPI_CS_POLARITY_0_ACTIVE_LOW   (1<<0)              

#define SPI_CS_POLARITY_1              (1<<1)              
#define SPI_CS_POLARITY_1_ACTIVE_HIGH  (0<<1)              
#define SPI_CS_POLARITY_1_ACTIVE_LOW   (1<<1)   

#define SPI_CLOCK_DIVIDER(n)           (((n)&0x3FF)<<16)   
#define SPI_CLOCK_DIVIDER_MASK         (0x3FF<<16)         
#define SPI_CLOCK_DIVIDER_SHIFT        (16)                

#define SPI_CLOCK_LIMITER              (1<<28)             


//pattern
#define SPI_PATTERN(n)                 (((n)&0xFF)<<0)     
#define SPI_PATTERN_MASK               (0xFF<<0)           
#define SPI_PATTERN_SHIFT              (0)                 

#define SPI_PATTERN_MODE               (1<<8)              
#define SPI_PATTERN_MODE_DISABLED      (0<<8)              
#define SPI_PATTERN_MODE_ENABLED       (1<<8)              

#define SPI_PATTERN_SELECTOR           (1<<9)              
#define SPI_PATTERN_SELECTOR_UNTIL     (0<<9)              
#define SPI_PATTERN_SELECTOR_WHILE     (1<<9)              


//stream
#define SPI_TX_STREAM_BIT              (1<<0)              
#define SPI_TX_STREAM_BIT_ZERO         (0<<0)              
#define SPI_TX_STREAM_BIT_ONE          (1<<0)              

#define SPI_TX_STREAM_MODE             (1<<8)              
#define SPI_TX_STREAM_MODE_DISABLED    (0<<8)              
#define SPI_TX_STREAM_MODE_ENABLED     (1<<8)              

#define SPI_TX_STREAM_STOP_WITH_RX_DMA_DONE (1<<16)             
#define SPI_TX_STREAM_STOP_WITH_RX_DMA_DONE_DISABLED (0<<16)             
#define SPI_TX_STREAM_STOP_WITH_RX_DMA_DONE_ENABLED (1<<16)             


//pin_control
#define SPI_CLK_CTRL(n)                (((n)&0x3)<<0)      
#define SPI_CLK_CTRL_SPI_CTRL          (((0)&0x3)<<0)      
#define SPI_CLK_CTRL_INPUT_CTRL        (((1)&0x3)<<0)      
#define SPI_CLK_CTRL_FORCE_0_CTRL      (((2)&0x3)<<0)      
#define SPI_CLK_CTRL_FORCE_1_CTRL      (((3)&0x3)<<0)      
#define SPI_CLK_CTRL_MASK              (0x3<<0)            
#define SPI_CLK_CTRL_SHIFT             (0)                 

#define SPI_DO_CTRL(n)                 (((n)&0x3)<<2)      
#define SPI_DO_CTRL_SPI_CTRL           (((0)&0x3)<<2)      
#define SPI_DO_CTRL_INPUT_CTRL         (((1)&0x3)<<2)      
#define SPI_DO_CTRL_FORCE_0_CTRL       (((2)&0x3)<<2)      
#define SPI_DO_CTRL_FORCE_1_CTRL       (((3)&0x3)<<2)      
#define SPI_DO_CTRL_MASK               (0x3<<2)            
#define SPI_DO_CTRL_SHIFT              (2)                 

#define SPI_CS0_CTRL(n)                (((n)&0x3)<<4)      
#define SPI_CS0_CTRL_SPI_CTRL          (((0)&0x3)<<4)      
#define SPI_CS0_CTRL_INPUT_CTRL        (((1)&0x3)<<4)      
#define SPI_CS0_CTRL_FORCE_0_CTRL      (((2)&0x3)<<4)      
#define SPI_CS0_CTRL_FORCE_1_CTRL      (((3)&0x3)<<4)      
#define SPI_CS0_CTRL_MASK              (0x3<<4)            
#define SPI_CS0_CTRL_SHIFT             (4)                 

#define SPI_CS1_CTRL(n)                (((n)&0x3)<<6)      
#define SPI_CS1_CTRL_SPI_CTRL          (((0)&0x3)<<6)      
#define SPI_CS1_CTRL_INPUT_CTRL        (((1)&0x3)<<6)      
#define SPI_CS1_CTRL_FORCE_0_CTRL      (((2)&0x3)<<6)      
#define SPI_CS1_CTRL_FORCE_1_CTRL      (((3)&0x3)<<6)      
#define SPI_CS1_CTRL_MASK              (0x3<<6)            
#define SPI_CS1_CTRL_SHIFT             (6)                 


//irq
#define SPI_MASK_RX_OVF_IRQ            (1<<0)              

#define SPI_MASK_TX_TH_IRQ             (1<<1)              

#define SPI_MASK_TX_DMA_IRQ            (1<<2)              

#define SPI_MASK_RX_TH_IRQ             (1<<3)              

#define SPI_MASK_RX_DMA_IRQ            (1<<4)              

#define SPI_TX_THRESHOLD(n)            (((n)&0x3)<<5)      
#define SPI_TX_THRESHOLD_1_EMPTY_SLOT  (((0)&0x3)<<5)      
#define SPI_TX_THRESHOLD_2_EMPTY_SLOTS (((1)&0x3)<<5)      
#define SPI_TX_THRESHOLD_3_EMPTY_SLOTS (((2)&0x3)<<5)      
#define SPI_TX_THRESHOLD_4_EMPTY_SLOTS (((3)&0x3)<<5)      
#define SPI_TX_THRESHOLD_MASK          (0x3<<5)            
#define SPI_TX_THRESHOLD_SHIFT         (5)                 

#define SPI_RX_THRESHOLD(n)            (((n)&0x3)<<7)      
#define SPI_RX_THRESHOLD_1_VALID_DATA  (((0)&0x3)<<7)      
#define SPI_RX_THRESHOLD_2_VALID_DATA  (((1)&0x3)<<7)      
#define SPI_RX_THRESHOLD_3_VALID_DATA  (((2)&0x3)<<7)      
#define SPI_RX_THRESHOLD_4_VALID_DATA  (((3)&0x3)<<7)      
#define SPI_RX_THRESHOLD_MASK          (0x3<<7)            
#define SPI_RX_THRESHOLD_SHIFT         (7)                 


#endif

#endif


