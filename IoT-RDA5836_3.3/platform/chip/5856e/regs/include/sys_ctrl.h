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
// File name      : sys_ctrl                                            
// Author         : liqiu                                                 
// Date of created: 2016-08-29 14:59:58                                                  
// Last modified  : 2016-08-29 14:59:58                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-08-29 14:59:58 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __SYS_CTRL_H__
#define __SYS_CTRL_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"
#include "register.h"
#define hwp_sysCtrl    hwp_sys_ctrl
#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//per_soft_reset1
typedef union
{
    U32 v;
    struct
    {
        U32 xcpu_rstb                      :1 ; /*0 :0 , XCPU function reset. Active low.1: unreset0: reset*/
        U32 voc_rstb                       :1 ; /*1 :1 , VOC  function reset. Active low.1: unreset0: reset*/
        U32 voc_ahb_rstb                   :1 ; /*2 :2 , VOC AHB reset. Active low.1: unreset0: reset*/
        U32 spiflash_rstb                  :1 ; /*3 :3 , SPIFLASH function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset1_reserved_2     :1 ; /*4 :4 , No description.               */        U32 audifc_rstb                    :1 ; /*5 :5 , AUDIFC function reset. Active low.1: unreset0: reset*/
        U32 sysifc_rstb                    :1 ; /*6 :6 , SYSIFC function reset. Active low.1: unreset0: reset*/
        U32 usbc_rstb                      :1 ; /*7 :7 , USBC function reset reset. Active low.1: unreset0: reset*/
        U32 dma_rstb                       :1 ; /*8 :8 , DMA function reset. Active low.1: unreset0: reset*/
        U32 bt_dbm_rstb                    :1 ; /*9 :9 , BT_DBM function reset. Active low.1: unreset0: reset*/
        U32 wdt_timer_rstb                 :1 ; /*10:10, WDT timer  function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset1_reserved_1     :1 ; /*11:11, No description.               */        U32 wdt_rstb                       :1 ; /*12:12, WDT function reset. Active low.1: unreset0: reset*/
        U32 usbc_phy_rstb                  :1 ; /*13:13, USBC PHY function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset1_reserved_0     :17; /*30:14, No description.               */
        U32 global_rstb                    :1 ; /*31:31,  Global reset. Active low. Reset entire DBB1: unreset0: reset*/
    } b;
} T_SYS_CTRL_per_soft_reset1;

//per_soft_reset2
typedef union
{
    U32 v;
    struct
    {
        U32 aif_rstb                       :1 ; /*0 :0 , AIF function reset. Active low.1: unreset0: reset*/
        U32 timer_rstb                     :1 ; /*1 :1 , Timer function reset. Active low.1: unreset1: reset*/
        U32 pwm_rstb                       :1 ; /*2 :2 , PWM function reset. Active low.1: unreset0: reset*/
        U32 i2c_rstb                       :1 ; /*3 :3 , I2C function reset. Active low.1: unreset0: reset*/
        U32 sdmmc_rstb                     :1 ; /*4 :4 , SDMMC function reset. Active low.1: unreset0: reset*/
        U32 spi_rstb                       :1 ; /*5 :5 , SPI function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset2_reserved_0     :26; /*31:6 , No description.               */
    } b;
} T_SYS_CTRL_per_soft_reset2;

//per_soft_reset3
typedef union
{
    U32 v;
    struct
    {
        U32 uart_rstb                      :1 ; /*0 :0 , UART function reset. Active low.1: unreset0: reset*/
        U32 dbg_hst_rstb                   :1 ; /*1 :1 , DEBUG_HOST function reset. Active low.1: unreset0: reset*/
        U32 usb_det_rstb                   :1 ; /*2 :2 , USB_DET function reset. Active low.1: unreset0: reset*/
        U32 ir_rstb                        :1 ; /*3 :3 , IR function reset. Active low.1: unreset0: reset*/
        U32 gpio_rstb                      :1 ; /*4 :4 , GPIO function reset. Active low.1: unreset0: reset*/
        U32 bt_rstb                        :1 ; /*5 :5 , BT function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset3_reserved_1     :1 ; /*6 :6 , No description.               */
        U32 codec_rstb                     :1 ; /*7 :7 , Audio Codec function reset. Active low.1: unreset0: reset*/
        U32 per_soft_reset3_reserved_0     :24; /*31:8 , No description.               */
    } b;
} T_SYS_CTRL_per_soft_reset3;

//per_clk_force_on
typedef union
{
    U32 v;
    struct
    {
        U32 sdmmc_clk_force_on             :1 ; /*0 :0 , Force SDMMC function clock on.1: enable0: disable*/
        U32 spi_clk_force_on               :1 ; /*1 :1 , Force SPI function clock on.1: enable0: disable*/
        U32 uart_clk_force_on              :1 ; /*2 :2 , Force UART function clock on.1: enable0: disable*/
        U32 dbg_hst_clk_force_on           :1 ; /*3 :3 , Force DEBUG_HOST debug clock on.1: enable0: disable*/
        U32 i2c_clk_force_on               :1 ; /*4 :4 , Force I2C function clock on.1: enable0: disable*/
        U32 aif_clk_force_on               :1 ; /*5 :5 , Force AIF function clock on.1: enable0: disable*/
        U32 dma_clk_force_on               :1 ; /*6 :6 , Force DMA function clock on.1: enable0: disable*/
        U32 pwm_clk_force_on               :1 ; /*7 :7 , Force PWM function clock on.1: enable0: disable*/
        U32 sysifc_clk_force_on            :1 ; /*8 :8 , Force AUDIFC function clock on.1: enable0: disable*/
        U32 audifc_clk_force_on            :1 ; /*9 :9 , Force AUDIFC function clock on.1: enable0: disable*/
        U32 xcpu_int_clk_force_on          :1 ; /*10:10, Force XCP internal function clock on.1: enable0: disable*/
        U32 xcpu_clk_force_on              :1 ; /*11:11, Force XCP function clock on.1: enable0: disable*/
        U32 voc_clk_force_on               :1 ; /*12:12, Force VOC function clock on.1: enable0: disable*/
        U32 per_clk_force_on_reserved_0    :16; /*28:13, No description.               */
        U32 ahb_mem_clk_force_on           :1 ; /*29:29, Force AHB memory access clock on.1: enable0: disable*/
        U32 ahb_reg_clk_force_on           :1 ; /*30:30, Force AHB register access clock on.1: enable0: disable*/
        U32 apb_reg_clk_force_on           :1 ; /*31:31, Force APB register access clock on.1: enable0: disable*/
    } b;
} T_SYS_CTRL_per_clk_force_on;

//per_module_en
typedef union
{
    U32 v;
    struct
    {
        U32 voc_en                         :1 ; /*0 :0 , Enable VOC1: enable0: disable*/
        U32 per_module_en_reserved_4       :1 ; /*1 :1 , No description.               */
        U32 usbc_en                        :1 ; /*2 :2 , Enable USBC1: enable0: disable*/
        U32 spiflash_en                    :1 ; /*3 :3 , Enable SPIFLASH Controller1: enable0: disable*/
        U32 per_module_en_reserved_3       :1 ; /*4 :4 , No description.               */        U32 bt_en                          :1 ; /*5 :5 , Enable BT1: enable0: disable*/
        U32 bt_dbm_en                      :1 ; /*6 :6 , Enable Debug Host1: enable0: disable*/
        U32 dbg_hst_en                     :1 ; /*7 :7 , Enable Debug Host1: enable0: disable*/
        U32 uart_en                        :1 ; /*8 :8 , Enable UART1: enable0: disable*/
        U32 per_module_en_reserved_2       :1 ; /*9 :9 , No description.               */
        U32 ir_en                          :1 ; /*10:10, Enable IR1: enable0: disable*/
        U32 gpadc_en                       :1 ; /*11:11, Enable IR1: enable0: disable*/
        U32 gpio_en                        :1 ; /*12:12, Enable GPIO1: enable0: disable*/
        U32 timer_en                       :1 ; /*13:13, Enable TIMER1: enable0: disable*/
        U32 usb_det_en                     :1 ; /*14:14, Enable USB_DET1: enable0: disable*/
        U32 per_module_en_reserved_1       :1 ; /*15:15, No description.               */        U32 wdt_en                         :1 ; /*16:16, Enable WDT1: enable0: disable*/
        U32 per_module_en_reserved_0       :15; /*31:17, No description.               */
    } b;
} T_SYS_CTRL_per_module_en;

//spiflash_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 spiflash_clk_sel_fast          :1 ; /*0 :0 , System clock select fast clock.1: fast0: slow*/
        U32 spiflash_fast_clk_sel          :2 ; /*2 :1 , System clock selection.3: spiflash_pll_div_clk2: xtal_52m1: xtal_39m0: xtal_26m*/
        U32 spiflash_pll_clk_div           :4 ; /*6 :3 , spiflash_pll_div_clk divider select. Output frequency is sys_pll_clk *sys_pll_clk_div +1) /16*/        U32 spiflash_pll_clk_ld_cfg        :1 ; /*7 :7 , SPIFLASH PLL clock load configration*/
        U32 spiflash_clk_cfg_reserved_0    :24; /*31:8 , No description.               */
    } b;
} T_SYS_CTRL_spiflash_clk_cfg;

//audio_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 aif_tx_stb_div                 :11; /*10:0 , AIF tx_stb divider configuration.*/
        U32 aif_tx_stb_div_en              :1 ; /*11:11, Enable tx_stb divider 1: enable0: disable*/
        U32 aif_tx_stb_div_ld_cfg          :1 ; /*12:12, Tx_stb divider load configuration */
        U32 audio_clk_cfg_reserved_0       :1 ; /*13:13, No description.               */
        U32 codec_clk_sel_pll              :1 ; /*14:14, codec clock source selection.1: Audio_PLL0: XTAL_26M*/
        U32 aif_bclk_div                   :11; /*25:15, AIF bclk divider configuration*/
        U32 aif_bclk_div_ld_cfg            :1 ; /*26:26, AIF bclk divider load configuration*/
        U32 aif_bclk_div_en                :1 ; /*27:27, Enable tx_stb divider 1: enable0: disable*/
        U32 i2s_mclk_div                   :2 ; /*29:28, I2S MCLK divider.3: div82: div41: div20: div1*/
        U32 i2s_mclk_div_ld_cfg            :1 ; /*30:30, I2S MCLK divider load configuration*/
        U32 i2s_mclk_div_en                :1 ; /*31:31, Enable I2S mclk output divider enable1: enable0: disable*/
    } b;
} T_SYS_CTRL_audio_clk_cfg;

//pwm_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 pwm_clk_div                    :10; /*9 :0 , PWM clock divider.            */
        U32 pwm_clk_ld_cfg                 :1 ; /*10:10, PWM clock load configration   */
        U32 pwm_clk_cfg_reserved_0         :21; /*31:11, No description.               */
    } b;
} T_SYS_CTRL_pwm_clk_cfg;

//dbg_hst_uart_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 dbg_hst_uart_clk_sel_52m       :1 ; /*0 :0 , Debug host uart clock source select1: XTAL_52M0: XTAL*/
        U32 dbg_hst_uart_clk_denom         :10; /*10:1 , Debug host uart clock denominator*/
        U32 dbg_hst_uart_clk_num           :10; /*20:11, Debug host uart clock numerator*/
        U32 dbg_hst_uart_clk_ld_cfg        :1 ; /*21:21, Debug host uart clock load configuration*/
        U32 dbg_hst_uart_clk_cfg_reserved_0 :10; /*31:22, No description.               */
    } b;
} T_SYS_CTRL_dbg_hst_uart_clk_cfg;

//uart_clk_cfg
typedef union
{
    U32 v;
    struct
    {
        U32 uart_clk_sel_52m               :1 ; /*0 :0 , UART clock source select1: XTAL_52M0: XTAL*/
        U32 uart_clk_denom                 :10; /*10:1 , UART clock denominator        */
        U32 uart_clk_num                   :10; /*20:11, UART clock numerator          */
        U32 uart_clk_ld_cfg                :1 ; /*21:21, UART clock load configuration */
        U32 uart_clk_cfg_reserved_0        :10; /*31:22, No description.               */
    } b;
} T_SYS_CTRL_uart_clk_cfg;

//wakeup_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 force_wakeup                   :1 ; /*0 :0 , Force wakeup                  */
        U32 wakeup_ctrl_reserved_1         :7 ; /*7 :1 , No description.               */
        U32 uart_rxd_break_wakeup_bt_en    :1 ; /*8 :8 , Enable UART rxd break to eakeup BT*/
        U32 wakeup_ctrl_reserved_0         :23; /*31:9 , No description.               */
    } b;
} T_SYS_CTRL_wakeup_ctrl;

//RF_IF_CTRL
typedef union
{
    U32 v;
    struct
    {
        U32 rf_reg_src_sel                 :1 ; /*0 :0 , RF interface register access method selection.
0: APB
1: SPI*/
        U32 RF_IF_CTRL_reserved_0          :31; /*31:1 , No description.               */
    } b;
} T_SYS_CTRL_RF_IF_CTRL;

//pad_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 sflash_drv                     :3 ; /*2 :0 , SFLASH PAD driver control     */
        U32 pad_ctrl_reserved_0            :29; /*31:3 , No description.               */
    } b;
} T_SYS_CTRL_pad_ctrl;

//i2s_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 i2s_sel                        :1 ; /*0 :0 , I2S Selection.0: AIF1: FM   */        U32 i2s_ctrl_reserved_0            :31; /*31:1 , No description.               */
    } b;
} T_SYS_CTRL_i2s_ctrl;

//xcpu_dbg_stall
typedef union
{
    U32 v;
    struct
    {
        U32 xcpu_dbg_stall_reserved_1      :8 ; /*7 :0 , RESERVED                      */
        U32 stalled                        :1 ; /*8 :8 , When 1 the cpu is stalled and stays stalled until this bit is set to 0, when 0 the stall condition depends on the breakpoint trigger. If the breakpoint is triggered, the cpu stalls. It can be released by writing a 0 to this bit.*/
        U32 xcpu_dbg_stall_reserved_0      :23; /*31:9 , RESERVED                      */
    } b;
} T_SYS_CTRL_xcpu_dbg_stall;

//xcpu_cache_ram_disable
typedef union
{
    U32 v;
    struct
    {
        U32 xcpu_use_mode                  :1 ; /*0 :0 , When 1 the XCPU deselects its cache rams automatically when not in use, when 0 this feature is disabled.*/
        U32 xcpu_clk_off_mode              :1 ; /*1 :1 , When 1 the cache rams are automatically deselected  when the XCPU clock is shutdown, when 0 this feature is disabled.*/
        U32 tcm_dcache_disable             :1 ; /*2 :2 , When 1, disable access ram/rom throygh dcache*/
        U32 xcpu_cache_ram_disable_reserved_1 :5 ; /*7 :3 , RESERVED                      */
        U32 dtag_ram_clk_force_on          :1 ; /*8 :8 , Force dtag_ram clock on       */
        U32 ddata_ram_clk_force_on         :1 ; /*9 :9 , Force ddata_ram clock on      */
        U32 itag_ram_clk_force_on          :1 ; /*10:10, Force itag_ram clock on       */
        U32 idata_ram_clk_force_on         :1 ; /*11:11, Force idata_ram clock on      */
        U32 xcpu_cache_ram_disable_reserved_0 :20; /*31:12, RESERVED                      */
    } b;
} T_SYS_CTRL_xcpu_cache_ram_disable;

//mem_ctrl0
typedef union
{
    U32 v;
    struct
    {
        U32 rom_dvse                       :1 ; /*0 :0 , No description.               */
        U32 rom_dvs                        :4 ; /*4 :1 , No description.               */
        U32 dram_dvse                      :1 ; /*5 :5 , No description.               */
        U32 dram_dvs                       :3 ; /*8 :6 , No description.               */
        U32 bram_dvse                      :1 ; /*9 :9 , No description.               */
        U32 bram_dvs                       :3 ; /*12:10, No description.               */
        U32 cache_ram_dvse                 :1 ; /*13:13, No description.               */
        U32 cache_ram_dvs                  :4 ; /*17:14, No description.               */
        U32 tag_ram_dvse                   :1 ; /*18:18, No description.               */
        U32 tag_ram_dvs                    :4 ; /*22:19, No description.               */
        U32 mem_ctrl0_reserved_0           :9 ; /*31:23, RESERVED                      */
    } b;
} T_SYS_CTRL_mem_ctrl0;

//mem_ctrl1
typedef union
{
    U32 v;
    struct
    {
        U32 bt_link_ram_dvse               :1 ; /*0 :0 , No description.               */
        U32 bt_link_ram_dvs                :4 ; /*4 :1 , No description.               */
        U32 bt_modem_ram_dvse              :1 ; /*5 :5 , No description.               */
        U32 bt_modem_ram_dvs               :4 ; /*9 :6 , No description.               */
        U32 usbc_ram_dvse                  :1 ; /*10:10, No description.               */
        U32 usbc_ram_dvs                   :4 ; /*14:11, No description.               */
        U32 voc_ram_dvse                   :1 ; /*15:15, No description.               */
        U32 voc_ram_dvs                    :3 ; /*18:16, No description.               */
        U32 mem_ctrl1_reserved_0           :13; /*31:19, RESERVED                      */
    } b;
} T_SYS_CTRL_mem_ctrl1;

//dbg_mux_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 dbg_out_sel                    :8 ; /*7 :0 , Debug out selection           */
        U32 dbg_out_idx                    :4 ; /*11:8 , Debug out index               */
        U32 dbg_mux_ctrl_reserved_0        :2 ; /*13:12, No description.               */
        U32 dbg_out_nibble_swap            :1 ; /*14:14, Half Byte swap of dbg_out     */
        U32 dbg_out_byte_swap              :1 ; /*15:15, Byte swap of dbg_out          */
        U32 dbg_out_nibble_shift           :1 ; /*16:16, nibble shift enable
0: nibble_shift_disable   Output is dbg_out[15:0]
1: nibble_shift_en           Output is prcoess according dbg_out_nibble_mode*/
        U32 dbg_out_nibble_shift_mode      :1 ; /*17:17, nibble shift mode
0: nibble_shift_mode0   Ouptut is {dbg_out[11:0], dbg_out[15:12]}
1: nibble_shift_mode1   Ouptut is {dbg_out[15:12], dbg_out[7:4], dbg_out[11:8], dbg_out[3:0]}*/
        U32 dbg_clk_sel                    :6 ; /*23:18, Debug clock selection         */
        U32 dbg_trig_sel                   :8 ; /*31:24, Debug trigger selection       */
    } b;
} T_SYS_CTRL_dbg_mux_ctrl;

//mcu_warm_rst_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 mcu_warm_rst_ctrl_reserved_1   :2 ; /*1 :0 , No description.               */
        U32 iomux_warm_rst_en              :1 ; /*2 :2 , 0: iomux cannot be reset by warm reset1: iomux can be reset by warm reset*/
        U32 mcu_warm_rst_ctrl_reserved_0   :29; /*31:3 , No description.               */
    } b;
} T_SYS_CTRL_mcu_warm_rst_ctrl;

//mcu_warm_reset_cause
typedef union
{
    U32 v;
    struct
    {
        U32 xcpu_rst                       :1 ; /*0 :0 , 1: Indicate the warm reset is caused by xcpu reset*/
        U32 wdt_rst                        :1 ; /*1 :1 , 1: Indicate the warm reset is caused by watch dog*/
        U32 global_soft_rst                :1 ; /*2 :2 , 1: Indicate the warm reset is caused by global soft reset*/
        U32 dbg_hst_rst                    :1 ; /*3 :3 , 1: Indicate the warm reset is caused by debug  host reset*/
        U32 alarm_cause                    :1 ; /*4 :4 , 1: Indicate RTC alarm happened*/
        U32 mcu_warm_reset_cause_reserved_2 :3 ; /*7 :5 , No description.               */
        U32 rst_cause_clr                  :1 ; /*8 :8 , Clear reset cause             */
        U32 mcu_warm_reset_cause_reserved_1 :7 ; /*15:9 , No description.               */
        U32 boot_mode                      :6 ; /*21:16, Boot Mode                     */
        U32 mcu_warm_reset_cause_reserved_0 :9 ; /*30:22, No description.               */
        U32 function_test_mode             :1 ; /*31:31, Function test mode. MCU keep in while 1 loop.*/
    } b;
} T_SYS_CTRL_mcu_warm_reset_cause;

typedef struct
{
    volatile      T_SYS_CTRL_per_soft_reset1                per_soft_reset1; /*0x0  , RW   , 0x800037EF, Peripheral Soft  Reset Register 1*/
    volatile      T_SYS_CTRL_per_soft_reset2                per_soft_reset2; /*0x4  , RW   , 0x0000003F, Peripheral Soft Reset Register 2*/
    volatile      T_SYS_CTRL_per_soft_reset3                per_soft_reset3; /*0x8  , RW   , 0x000000BF, Peripheral Soft Reset Register 3*/
    volatile     T_SYS_CTRL_per_clk_force_on               per_clk_force_on; /*0xc  , RW   , 0x00000008, Force Peripheral Clock On Register */
    volatile        T_SYS_CTRL_per_module_en                  per_module_en; /*0x10 , RW   , 0x00010088, Peripheral Module Enable Register*/
    volatile                             U32                  reserved_1[2];
    volatile     T_SYS_CTRL_spiflash_clk_cfg               spiflash_clk_cfg; /*0x1c , RW   , 0x00000079, SPI Flash Clock Configuration Register */
    volatile        T_SYS_CTRL_audio_clk_cfg                  audio_clk_cfg; /*0x20 , RW   , 0x03FF87FF, Audio  Clock Configuration Register*/
    volatile          T_SYS_CTRL_pwm_clk_cfg                    pwm_clk_cfg; /*0x24 , RW   , 0x000003FF, PWM Clock Configuration Register */
    volatile                             U32                     reserved_2;
    volatile T_SYS_CTRL_dbg_hst_uart_clk_cfg           dbg_hst_uart_clk_cfg; /*0x2c , RW   , 0x0000080E,  Debug Host UART Clock Configuration Register */
    volatile         T_SYS_CTRL_uart_clk_cfg                   uart_clk_cfg; /*0x30 , RW   , 0x0000080E, UART Clock Configuration Register */
    volatile          T_SYS_CTRL_wakeup_ctrl                    wakeup_ctrl; /*0x34 , RW   , 0x00000000, Wakeup Control Register       */
    volatile           T_SYS_CTRL_RF_IF_CTRL                     RF_IF_CTRL; /*0x38 , RW   , 0x00000000, RF Interface Control Register */
    volatile                             U32                  reserved_3[2];
    volatile             T_SYS_CTRL_pad_ctrl                       pad_ctrl; /*0x44 , RW   , 0x00000004, PAD Control Register          */
    volatile                             U32                  reserved_4[2];
    volatile             T_SYS_CTRL_i2s_ctrl                       i2s_ctrl; /*0x50 , RW   , 0x00000000, I2S Control Register          */
    volatile                             U32                 reserved_5[12];
    volatile       T_SYS_CTRL_xcpu_dbg_stall                 xcpu_dbg_stall; /*0x84 , RW   , 0x00000000, XCPU Debug Stall Register     */
    volatile                             U32                     reserved_6;
    volatile T_SYS_CTRL_xcpu_cache_ram_disable         xcpu_cache_ram_disable; /*0x8c , RW   , 0x00000005, XCPU CACHE RAM  Disable Config Register*/
    volatile            T_SYS_CTRL_mem_ctrl0                      mem_ctrl0; /*0x90 , RW   , 0x00000000, Memory Control Register 0     */
    volatile            T_SYS_CTRL_mem_ctrl1                      mem_ctrl1; /*0x94 , RW   , 0x00000000, Memory Control Register 1     */
    volatile                             U32                     reserved_7;
    volatile         T_SYS_CTRL_dbg_mux_ctrl                   dbg_mux_ctrl; /*0x9c , RW   , 0x00000000, Debug Mux Control Register    */
    volatile                             U32                 reserved_8[88];
    volatile    T_SYS_CTRL_mcu_warm_rst_ctrl              mcu_warm_rst_ctrl; /*0x200, RW   , 0x00000000, MCU Warm Reset Control Register*/
    volatile T_SYS_CTRL_mcu_warm_reset_cause           mcu_warm_reset_cause; /*0x204, RW   , 0x00000000, MCU Warm Reset Cause Register */
} T_HWP_SYS_CTRL_T;
#else
typedef struct
{
    volatile                             U32                per_soft_reset1; /*0x0  , RW   , 0x800037EF, Peripheral Soft  Reset Register 1*/
    volatile                             U32                per_soft_reset2; /*0x4  , RW   , 0x0000003F, Peripheral Soft Reset Register 2*/
    volatile                             U32                per_soft_reset3; /*0x8  , RW   , 0x000000BF, Peripheral Soft Reset Register 3*/
    volatile                             U32               per_clk_force_on; /*0xc  , RW   , 0x00000008, Force Peripheral Clock On Register */
    volatile                             U32                  per_module_en; /*0x10 , RW   , 0x00010088, Peripheral Module Enable Register*/
    volatile                             U32                  reserved_9[2];
    volatile                             U32               spiflash_clk_cfg; /*0x1c , RW   , 0x00000079, SPI Flash Clock Configuration Register */
    volatile                             U32                  audio_clk_cfg; /*0x20 , RW   , 0x03FF87FF, Audio  Clock Configuration Register*/
    volatile                             U32                    pwm_clk_cfg; /*0x24 , RW   , 0x000003FF, PWM Clock Configuration Register */
    volatile                             U32                    reserved_10;
    volatile                             U32           dbg_hst_uart_clk_cfg; /*0x2c , RW   , 0x0000080E,  Debug Host UART Clock Configuration Register */
    volatile                             U32                   uart_clk_cfg; /*0x30 , RW   , 0x0000080E, UART Clock Configuration Register */
    volatile                             U32                    wakeup_ctrl; /*0x34 , RW   , 0x00000000, Wakeup Control Register       */
    volatile                             U32                     RF_IF_CTRL; /*0x38 , RW   , 0x00000000, RF Interface Control Register */
    volatile                             U32                 reserved_11[2];
    volatile                             U32                       pad_ctrl; /*0x44 , RW   , 0x00000004, PAD Control Register          */
    volatile                             U32                 reserved_12[2];
    volatile                             U32                       i2s_ctrl; /*0x50 , RW   , 0x00000000, I2S Control Register          */
    volatile                             U32                reserved_13[12];
    volatile                             U32                 xcpu_dbg_stall; /*0x84 , RW   , 0x00000000, XCPU Debug Stall Register     */
    volatile                             U32                    reserved_14;
    volatile                             U32         xcpu_cache_ram_disable; /*0x8c , RW   , 0x00000005, XCPU CACHE RAM  Disable Config Register*/
    volatile                             U32                      mem_ctrl0; /*0x90 , RW   , 0x00000000, Memory Control Register 0     */
    volatile                             U32                      mem_ctrl1; /*0x94 , RW   , 0x00000000, Memory Control Register 1     */
    volatile                             U32                    reserved_15;
    volatile                             U32                   dbg_mux_ctrl; /*0x9c , RW   , 0x00000000, Debug Mux Control Register    */
    volatile                             U32                reserved_16[88];
    volatile                             U32              mcu_warm_rst_ctrl; /*0x200, RW   , 0x00000000, MCU Warm Reset Control Register*/
    volatile                             U32           mcu_warm_reset_cause; /*0x204, RW   , 0x00000000, MCU Warm Reset Cause Register */
} T_HWP_SYS_CTRL_T;

//per_soft_reset1
#define SYS_CTRL_XCPU_RSTB             (1<<0)              
#define SYS_CTRL_XCPU_RSTB_UNRESET     (1<<0)              
#define SYS_CTRL_XCPU_RSTB_RESET       (0<<0)              

#define SYS_CTRL_VOC_RSTB              (1<<1)              
#define SYS_CTRL_VOC_RSTB_UNRESET      (1<<1)              
#define SYS_CTRL_VOC_RSTB_RESET        (0<<1)              

#define SYS_CTRL_VOC_AHB_RSTB          (1<<2)              
#define SYS_CTRL_VOC_AHB_RSTB_UNRESET  (1<<2)              
#define SYS_CTRL_VOC_AHB_RSTB_RESET    (0<<2)              

#define SYS_CTRL_SPIFLASH_RSTB         (1<<3)              
#define SYS_CTRL_SPIFLASH_RSTB_UNRESET (1<<3)              
#define SYS_CTRL_SPIFLASH_RSTB_RESET   (0<<3)              


#define SYS_CTRL_AUDIFC_RSTB           (1<<5)              
#define SYS_CTRL_AUDIFC_RSTB_UNRESET   (1<<5)              
#define SYS_CTRL_AUDIFC_RSTB_RESET     (0<<5)              

#define SYS_CTRL_SYSIFC_RSTB           (1<<6)              
#define SYS_CTRL_SYSIFC_RSTB_UNRESET   (1<<6)              
#define SYS_CTRL_SYSIFC_RSTB_RESET     (0<<6)              

#define SYS_CTRL_USBC_RSTB             (1<<7)              
#define SYS_CTRL_USBC_RSTB_UNRESET     (1<<7)              
#define SYS_CTRL_USBC_RSTB_RESET       (0<<7)              

#define SYS_CTRL_DMA_RSTB              (1<<8)              
#define SYS_CTRL_DMA_RSTB_UNRESET      (1<<8)              
#define SYS_CTRL_DMA_RSTB_RESET        (0<<8)              

#define SYS_CTRL_BT_DBM_RSTB           (1<<9)              
#define SYS_CTRL_BT_DBM_RSTB_UNRESET   (1<<9)              
#define SYS_CTRL_BT_DBM_RSTB_RESET     (0<<9)              

#define SYS_CTRL_WDT_TIMER_RSTB        (1<<10)             
#define SYS_CTRL_WDT_TIMER_RSTB_UNRESET (1<<10)             
#define SYS_CTRL_WDT_TIMER_RSTB_RESET  (0<<10)             


#define SYS_CTRL_WDT_RSTB              (1<<12)             
#define SYS_CTRL_WDT_RSTB_UNRESET      (1<<12)             
#define SYS_CTRL_WDT_RSTB_RESET        (0<<12)             

#define SYS_CTRL_USBC_PHY_RSTB         (1<<13)             
#define SYS_CTRL_USBC_PHY_RSTB_UNRESET (1<<13)             
#define SYS_CTRL_USBC_PHY_RSTB_RESET   (0<<13)             

#define SYS_CTRL_GLOBAL_RSTB           (1<<31)             
#define SYS_CTRL_GLOBAL_RSTB_UNRESET   (1<<31)             
#define SYS_CTRL_GLOBAL_RSTB_RESET     (0<<31)             


//per_soft_reset2
#define SYS_CTRL_AIF_RSTB              (1<<0)              
#define SYS_CTRL_AIF_RSTB_UNRESET      (1<<0)              
#define SYS_CTRL_AIF_RSTB_RESET        (0<<0)              

#define SYS_CTRL_TIMER_RSTB            (1<<1)              
#define SYS_CTRL_TIMER_RSTB_UNRESET    (1<<1)              
#define SYS_CTRL_TIMER_RSTB_RESET      (0<<1)              

#define SYS_CTRL_PWM_RSTB              (1<<2)              
#define SYS_CTRL_PWM_RSTB_UNRESET      (1<<2)              
#define SYS_CTRL_PWM_RSTB_RESET        (0<<2)              

#define SYS_CTRL_I2C_RSTB              (1<<3)              
#define SYS_CTRL_I2C_RSTB_UNRESET      (1<<3)              
#define SYS_CTRL_I2C_RSTB_RESET        (0<<3)              

#define SYS_CTRL_SDMMC_RSTB            (1<<4)              
#define SYS_CTRL_SDMMC_RSTB_UNRESET    (1<<4)              
#define SYS_CTRL_SDMMC_RSTB_RESET      (0<<4)              

#define SYS_CTRL_SPI_RSTB              (1<<5)              
#define SYS_CTRL_SPI_RSTB_UNRESET      (1<<5)              
#define SYS_CTRL_SPI_RSTB_RESET        (0<<5)              


//per_soft_reset3
#define SYS_CTRL_UART_RSTB             (1<<0)              
#define SYS_CTRL_UART_RSTB_UNRESET     (1<<0)              
#define SYS_CTRL_UART_RSTB_RESET       (0<<0)              

#define SYS_CTRL_DBG_HST_RSTB          (1<<1)              
#define SYS_CTRL_DBG_HST_RSTB_UNRESET  (1<<1)              
#define SYS_CTRL_DBG_HST_RSTB_RESET    (0<<1)              

#define SYS_CTRL_USB_DET_RSTB          (1<<2)              
#define SYS_CTRL_USB_DET_RSTB_UNRESET  (1<<2)              
#define SYS_CTRL_USB_DET_RSTB_RESET    (0<<2)              

#define SYS_CTRL_IR_RSTB               (1<<3)              
#define SYS_CTRL_IR_RSTB_UNRESET       (1<<3)              
#define SYS_CTRL_IR_RSTB_RESET         (0<<3)              

#define SYS_CTRL_GPIO_RSTB             (1<<4)              
#define SYS_CTRL_GPIO_RSTB_UNRESET     (1<<4)              
#define SYS_CTRL_GPIO_RSTB_RESET       (0<<4)              

#define SYS_CTRL_BT_RSTB               (1<<5)              
#define SYS_CTRL_BT_RSTB_UNRESET       (1<<5)              
#define SYS_CTRL_BT_RSTB_RESET         (0<<5)              

#define SYS_CTRL_CODEC_RSTB            (1<<7)              
#define SYS_CTRL_CODEC_RSTB_UNRESET    (1<<7)              
#define SYS_CTRL_CODEC_RSTB_RESET      (0<<7)              


//per_clk_force_on
#define SYS_CTRL_SDMMC_CLK_FORCE_ON    (1<<0)              
#define SYS_CTRL_SDMMC_CLK_FORCE_ON_ENABLE (1<<0)              
#define SYS_CTRL_SDMMC_CLK_FORCE_ON_DISABLE (0<<0)              

#define SYS_CTRL_SPI_CLK_FORCE_ON      (1<<1)              
#define SYS_CTRL_SPI_CLK_FORCE_ON_ENABLE (1<<1)              
#define SYS_CTRL_SPI_CLK_FORCE_ON_DISABLE (0<<1)              

#define SYS_CTRL_UART_CLK_FORCE_ON     (1<<2)              
#define SYS_CTRL_UART_CLK_FORCE_ON_ENABLE (1<<2)              
#define SYS_CTRL_UART_CLK_FORCE_ON_DISABLE (0<<2)              

#define SYS_CTRL_DBG_HST_CLK_FORCE_ON  (1<<3)              
#define SYS_CTRL_DBG_HST_CLK_FORCE_ON_ENABLE (1<<3)              
#define SYS_CTRL_DBG_HST_CLK_FORCE_ON_DISABLE (0<<3)              

#define SYS_CTRL_I2C_CLK_FORCE_ON      (1<<4)              
#define SYS_CTRL_I2C_CLK_FORCE_ON_ENABLE (1<<4)              
#define SYS_CTRL_I2C_CLK_FORCE_ON_DISABLE (0<<4)              

#define SYS_CTRL_AIF_CLK_FORCE_ON      (1<<5)              
#define SYS_CTRL_AIF_CLK_FORCE_ON_ENABLE (1<<5)              
#define SYS_CTRL_AIF_CLK_FORCE_ON_DISABLE (0<<5)              

#define SYS_CTRL_DMA_CLK_FORCE_ON      (1<<6)              
#define SYS_CTRL_DMA_CLK_FORCE_ON_ENABLE (1<<6)              
#define SYS_CTRL_DMA_CLK_FORCE_ON_DISABLE (0<<6)              

#define SYS_CTRL_PWM_CLK_FORCE_ON      (1<<7)              
#define SYS_CTRL_PWM_CLK_FORCE_ON_ENABLE (1<<7)              
#define SYS_CTRL_PWM_CLK_FORCE_ON_DISABLE (0<<7)              

#define SYS_CTRL_SYSIFC_CLK_FORCE_ON   (1<<8)              
#define SYS_CTRL_SYSIFC_CLK_FORCE_ON_ENABLE (1<<8)              
#define SYS_CTRL_SYSIFC_CLK_FORCE_ON_DISABLE (0<<8)              

#define SYS_CTRL_AUDIFC_CLK_FORCE_ON   (1<<9)              
#define SYS_CTRL_AUDIFC_CLK_FORCE_ON_ENABLE (1<<9)              
#define SYS_CTRL_AUDIFC_CLK_FORCE_ON_DISABLE (0<<9)              

#define SYS_CTRL_XCPU_INT_CLK_FORCE_ON (1<<10)             
#define SYS_CTRL_XCPU_INT_CLK_FORCE_ON_ENABLE (1<<10)             
#define SYS_CTRL_XCPU_INT_CLK_FORCE_ON_DISABLE (0<<10)             

#define SYS_CTRL_XCPU_CLK_FORCE_ON     (1<<11)             
#define SYS_CTRL_XCPU_CLK_FORCE_ON_ENABLE (1<<11)             
#define SYS_CTRL_XCPU_CLK_FORCE_ON_DISABLE (0<<11)             

#define SYS_CTRL_VOC_CLK_FORCE_ON      (1<<12)             
#define SYS_CTRL_VOC_CLK_FORCE_ON_ENABLE (1<<12)             
#define SYS_CTRL_VOC_CLK_FORCE_ON_DISABLE (0<<12)             

#define SYS_CTRL_AHB_MEM_CLK_FORCE_ON  (1<<29)             
#define SYS_CTRL_AHB_MEM_CLK_FORCE_ON_ENABLE (1<<29)             
#define SYS_CTRL_AHB_MEM_CLK_FORCE_ON_DISABLE (0<<29)             

#define SYS_CTRL_AHB_REG_CLK_FORCE_ON  (1<<30)             
#define SYS_CTRL_AHB_REG_CLK_FORCE_ON_ENABLE (1<<30)             
#define SYS_CTRL_AHB_REG_CLK_FORCE_ON_DISABLE (0<<30)             

#define SYS_CTRL_APB_REG_CLK_FORCE_ON  (1<<31)             
#define SYS_CTRL_APB_REG_CLK_FORCE_ON_ENABLE (1<<31)             
#define SYS_CTRL_APB_REG_CLK_FORCE_ON_DISABLE (0<<31)             


//per_module_en
#define SYS_CTRL_VOC_EN                (1<<0)              
#define SYS_CTRL_VOC_EN_ENABLE         (1<<0)              
#define SYS_CTRL_VOC_EN_DISABLE        (0<<0)              

#define SYS_CTRL_USBC_EN               (1<<2)              
#define SYS_CTRL_USBC_EN_ENABLE        (1<<2)              
#define SYS_CTRL_USBC_EN_DISABLE       (0<<2)              

#define SYS_CTRL_SPIFLASH_EN           (1<<3)              
#define SYS_CTRL_SPIFLASH_EN_ENABLE    (1<<3)              
#define SYS_CTRL_SPIFLASH_EN_DISABLE   (0<<3)              


#define SYS_CTRL_BT_EN                 (1<<5)              
#define SYS_CTRL_BT_EN_ENABLE          (1<<5)              
#define SYS_CTRL_BT_EN_DISABLE         (0<<5)              

#define SYS_CTRL_BT_DBM_EN             (1<<6)              
#define SYS_CTRL_BT_DBM_EN_ENABLE      (1<<6)              
#define SYS_CTRL_BT_DBM_EN_DISABLE     (0<<6)              

#define SYS_CTRL_DBG_HST_EN            (1<<7)              
#define SYS_CTRL_DBG_HST_EN_ENABLE     (1<<7)              
#define SYS_CTRL_DBG_HST_EN_DISABLE    (0<<7)              

#define SYS_CTRL_UART_EN               (1<<8)              
#define SYS_CTRL_UART_EN_ENABLE        (1<<8)              
#define SYS_CTRL_UART_EN_DISABLE       (0<<8)              

#define SYS_CTRL_IR_EN                 (1<<10)             
#define SYS_CTRL_IR_EN_ENABLE          (1<<10)             
#define SYS_CTRL_IR_EN_DISABLE         (0<<10)             

#define SYS_CTRL_GPADC_EN              (1<<11)             
#define SYS_CTRL_GPADC_EN_ENABLE       (1<<11)             
#define SYS_CTRL_GPADC_EN_DISABLE      (0<<11)             

#define SYS_CTRL_GPIO_EN               (1<<12)             
#define SYS_CTRL_GPIO_EN_ENABLE        (1<<12)             
#define SYS_CTRL_GPIO_EN_DISABLE       (0<<12)             

#define SYS_CTRL_TIMER_EN              (1<<13)             
#define SYS_CTRL_TIMER_EN_ENABLE       (1<<13)             
#define SYS_CTRL_TIMER_EN_DISABLE      (0<<13)             

#define SYS_CTRL_USB_DET_EN            (1<<14)             
#define SYS_CTRL_USB_DET_EN_ENABLE     (1<<14)             
#define SYS_CTRL_USB_DET_EN_DISABLE    (0<<14)             

#define SYS_CTRL_WDT_EN                (1<<16)             
#define SYS_CTRL_WDT_EN_ENABLE         (1<<16)             
#define SYS_CTRL_WDT_EN_DISABLE        (0<<16)             

//spiflash_clk_cfg
#define SYS_CTRL_SPIFLASH_CLK_SEL_FAST (1<<0)              
#define SYS_CTRL_SPIFLASH_CLK_SEL_FAST_FAST (1<<0)              
#define SYS_CTRL_SPIFLASH_CLK_SEL_FAST_SLOW (0<<0)              

#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL(n) (((n)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_SPIFLASH_PLL_DIV_CLK (((3)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_52M (((2)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_39M (((1)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_XTAL_26M (((0)&0x3)<<1)      
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_MASK (0x3<<1)            
#define SYS_CTRL_SPIFLASH_FAST_CLK_SEL_SHIFT (1)                 

#define SYS_CTRL_SPIFLASH_PLL_CLK_DIV(n) (((n)&0xF)<<3)      
#define SYS_CTRL_SPIFLASH_PLL_CLK_DIV_MASK (0xF<<3)            
#define SYS_CTRL_SPIFLASH_PLL_CLK_DIV_SHIFT (3)                 

#define SYS_CTRL_SPIFLASH_PLL_CLK_LD_CFG (1<<7)              


//audio_clk_cfg
#define SYS_CTRL_AIF_TX_STB_DIV(n)     (((n)&0x7FF)<<0)    
#define SYS_CTRL_AIF_TX_STB_DIV_MASK   (0x7FF<<0)          
#define SYS_CTRL_AIF_TX_STB_DIV_SHIFT  (0)                 

#define SYS_CTRL_AIF_TX_STB_DIV_EN     (1<<11)             
#define SYS_CTRL_AIF_TX_STB_DIV_EN_ENABLE (1<<11)             
#define SYS_CTRL_AIF_TX_STB_DIV_EN_DISABLE (0<<11)             

#define SYS_CTRL_AIF_TX_STB_DIV_LD_CFG (1<<12)             

#define SYS_CTRL_CODEC_CLK_SEL_PLL     (1<<14)             
#define SYS_CTRL_CODEC_CLK_SEL_PLL_AUDIO_PLL (1<<14)             
#define SYS_CTRL_CODEC_CLK_SEL_PLL_XTAL_26M (0<<14)             

#define SYS_CTRL_AIF_BCLK_DIV(n)       (((n)&0x7FF)<<15)   
#define SYS_CTRL_AIF_BCLK_DIV_MASK     (0x7FF<<15)         
#define SYS_CTRL_AIF_BCLK_DIV_SHIFT    (15)                

#define SYS_CTRL_AIF_BCLK_DIV_LD_CFG   (1<<26)             
#define SYS_CTRL_AIF_BCLK_DIV_EN       (1<<27)             
#define SYS_CTRL_AIF_BCLK_DIV_EN_ENABLE (1<<27)             
#define SYS_CTRL_AIF_BCLK_DIV_EN_DISABLE (0<<27)             

#define SYS_CTRL_I2S_MCLK_DIV(n)       (((n)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_DIV8     (((3)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_DIV4     (((2)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_DIV2     (((1)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_DIV1     (((0)&0x3)<<28)     
#define SYS_CTRL_I2S_MCLK_DIV_MASK     (0x3<<28)           
#define SYS_CTRL_I2S_MCLK_DIV_SHIFT    (28)                

#define SYS_CTRL_I2S_MCLK_DIV_LD_CFG   (1<<30)             
#define SYS_CTRL_I2S_MCLK_DIV_EN       (1<<31)             
#define SYS_CTRL_I2S_MCLK_DIV_EN_ENABLE (1<<31)             
#define SYS_CTRL_I2S_MCLK_DIV_EN_DISABLE (0<<31)             


//pwm_clk_cfg
#define SYS_CTRL_PWM_CLK_DIV(n)        (((n)&0x3FF)<<0)    
#define SYS_CTRL_PWM_CLK_DIV_MASK      (0x3FF<<0)          
#define SYS_CTRL_PWM_CLK_DIV_SHIFT     (0)                 

#define SYS_CTRL_PWM_CLK_LD_CFG        (1<<10)             

//dbg_hst_uart_clk_cfg
#define SYS_CTRL_DBG_HST_UART_CLK_SEL_52M (1<<0)              
#define SYS_CTRL_DBG_HST_UART_CLK_SEL_52M_XTAL_52M (1<<0)              
#define SYS_CTRL_DBG_HST_UART_CLK_SEL_52M_XTAL (0<<0)              

#define SYS_CTRL_DBG_HST_UART_CLK_DENOM(n) (((n)&0x3FF)<<1)    
#define SYS_CTRL_DBG_HST_UART_CLK_DENOM_MASK (0x3FF<<1)          
#define SYS_CTRL_DBG_HST_UART_CLK_DENOM_SHIFT (1)                 

#define SYS_CTRL_DBG_HST_UART_CLK_NUM(n) (((n)&0x3FF)<<11)   
#define SYS_CTRL_DBG_HST_UART_CLK_NUM_MASK (0x3FF<<11)         
#define SYS_CTRL_DBG_HST_UART_CLK_NUM_SHIFT (11)                

#define SYS_CTRL_DBG_HST_UART_CLK_LD_CFG (1<<21)             

//uart_clk_cfg
#define SYS_CTRL_UART_CLK_SEL_52M      (1<<0)              
#define SYS_CTRL_UART_CLK_SEL_52M_XTAL_52M (1<<0)              
#define SYS_CTRL_UART_CLK_SEL_52M_XTAL (0<<0)              

#define SYS_CTRL_UART_CLK_DENOM(n)     (((n)&0x3FF)<<1)    
#define SYS_CTRL_UART_CLK_DENOM_MASK   (0x3FF<<1)          
#define SYS_CTRL_UART_CLK_DENOM_SHIFT  (1)                 

#define SYS_CTRL_UART_CLK_NUM(n)       (((n)&0x3FF)<<11)   
#define SYS_CTRL_UART_CLK_NUM_MASK     (0x3FF<<11)         
#define SYS_CTRL_UART_CLK_NUM_SHIFT    (11)                

#define SYS_CTRL_UART_CLK_LD_CFG       (1<<21)             


//wakeup_ctrl
#define SYS_CTRL_FORCE_WAKEUP          (1<<0)              

#define SYS_CTRL_UART_RXD_BREAK_WAKEUP_BT_EN (1<<8)              


//RF_IF_CTRL
#define SYS_CTRL_RF_REG_SRC_SEL        (1<<0)              

#define SYS_CTRL_RF_REG_SRC_SEL_APB    (0<<0)              
#define SYS_CTRL_RF_REG_SRC_SEL_SPI    (1<<0)              


//pad_ctrl
#define SYS_CTRL_SFLASH_DRV(n)         (((n)&0x7)<<0)      
#define SYS_CTRL_SFLASH_DRV_MASK       (0x7<<0)            
#define SYS_CTRL_SFLASH_DRV_SHIFT      (0)                 

//i2s_ctrl
#define SYS_CTRL_I2S_SEL               (1<<0)              
#define SYS_CTRL_I2S_SEL_AIF           (0<<0)              
#define SYS_CTRL_I2S_SEL_FM            (1<<0)              

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
#define SYS_CTRL_ROM_DVS_MASK          (0xF<<1)            
#define SYS_CTRL_ROM_DVS_SHIFT         (1)                 

#define SYS_CTRL_DRAM_DVSE             (1<<5)              

#define SYS_CTRL_DRAM_DVS(n)           (((n)&0x7)<<6)      
#define SYS_CTRL_DRAM_DVS_MASK         (0x7<<6)            
#define SYS_CTRL_DRAM_DVS_SHIFT        (6)                 

#define SYS_CTRL_BRAM_DVSE             (1<<9)              

#define SYS_CTRL_BRAM_DVS(n)           (((n)&0x7)<<10)     
#define SYS_CTRL_BRAM_DVS_MASK         (0x7<<10)           
#define SYS_CTRL_BRAM_DVS_SHIFT        (10)                

#define SYS_CTRL_CACHE_RAM_DVSE        (1<<13)             

#define SYS_CTRL_CACHE_RAM_DVS(n)      (((n)&0xF)<<14)     
#define SYS_CTRL_CACHE_RAM_DVS_MASK    (0xF<<14)           
#define SYS_CTRL_CACHE_RAM_DVS_SHIFT   (14)                

#define SYS_CTRL_TAG_RAM_DVSE          (1<<18)             

#define SYS_CTRL_TAG_RAM_DVS(n)        (((n)&0xF)<<19)     
#define SYS_CTRL_TAG_RAM_DVS_MASK      (0xF<<19)           
#define SYS_CTRL_TAG_RAM_DVS_SHIFT     (19)                

//mem_ctrl1
#define SYS_CTRL_BT_LINK_RAM_DVSE      (1<<0)              

#define SYS_CTRL_BT_LINK_RAM_DVS(n)    (((n)&0xF)<<1)      
#define SYS_CTRL_BT_LINK_RAM_DVS_MASK  (0xF<<1)            
#define SYS_CTRL_BT_LINK_RAM_DVS_SHIFT (1)                 

#define SYS_CTRL_BT_MODEM_RAM_DVSE     (1<<5)              

#define SYS_CTRL_BT_MODEM_RAM_DVS(n)   (((n)&0xF)<<6)      
#define SYS_CTRL_BT_MODEM_RAM_DVS_MASK (0xF<<6)            
#define SYS_CTRL_BT_MODEM_RAM_DVS_SHIFT (6)                 

#define SYS_CTRL_USBC_RAM_DVSE         (1<<10)             

#define SYS_CTRL_USBC_RAM_DVS(n)       (((n)&0xF)<<11)     
#define SYS_CTRL_USBC_RAM_DVS_MASK     (0xF<<11)           
#define SYS_CTRL_USBC_RAM_DVS_SHIFT    (11)                

#define SYS_CTRL_VOC_RAM_DVSE          (1<<15)             

#define SYS_CTRL_VOC_RAM_DVS(n)        (((n)&0x7)<<16)     
#define SYS_CTRL_VOC_RAM_DVS_MASK      (0x7<<16)           
#define SYS_CTRL_VOC_RAM_DVS_SHIFT     (16)                

//dbg_mux_ctrl
#define SYS_CTRL_DBG_OUT_SEL(n)        (((n)&0xFF)<<0)     
#define SYS_CTRL_DBG_OUT_SEL_MASK      (0xFF<<0)           
#define SYS_CTRL_DBG_OUT_SEL_SHIFT     (0)                 

#define SYS_CTRL_DBG_OUT_IDX(n)        (((n)&0xF)<<8)      
#define SYS_CTRL_DBG_OUT_IDX_MASK      (0xF<<8)            
#define SYS_CTRL_DBG_OUT_IDX_SHIFT     (8)                 
#define SYS_CTRL_DBG_OUT_NIBBLE_SWAP   (1<<14)             

#define SYS_CTRL_DBG_OUT_BYTE_SWAP     (1<<15)             

#define SYS_CTRL_DBG_OUT_NIBBLE_SHIFT  (1<<16)             

#define SYS_CTRL_DBG_OUT_NIBBLE_SHIFT_MODE (1<<17)             

#define SYS_CTRL_DBG_CLK_SEL(n)        (((n)&0x3F)<<18)    
#define SYS_CTRL_DBG_CLK_SEL_MASK      (0x3F<<18)          
#define SYS_CTRL_DBG_CLK_SEL_SHIFT     (18)                

#define SYS_CTRL_DBG_TRIG_SEL(n)       (((n)&0xFF)<<24)    
#define SYS_CTRL_DBG_TRIG_SEL_MASK     (0xFF<<24)          
#define SYS_CTRL_DBG_TRIG_SEL_SHIFT    (24)                


//mcu_warm_rst_ctrl
#define SYS_CTRL_IOMUX_WARM_RST_EN     (1<<2)              
#define SYS_CTRL_IOMUX_WARM_RST_EN_IOMUX_ENABLE (1<<2)              
#define SYS_CTRL_IOMUX_WARM_RST_EN_IOMUX_DISABLE (0<<2)              


//mcu_warm_reset_cause
#define SYS_CTRL_XCPU_RST              (1<<0)              
#define SYS_CTRL_WDT_RST               (1<<1)              

#define SYS_CTRL_GLOBAL_SOFT_RST       (1<<2)              

#define SYS_CTRL_DBG_HST_RST           (1<<3)              

#define SYS_CTRL_ALARM_CAUSE           (1<<4)              

#define SYS_CTRL_RST_CAUSE_CLR         (1<<8)              

#define SYS_CTRL_BOOT_MODE(n)          (((n)&0x3F)<<16)    
#define SYS_CTRL_BOOT_MODE_MASK        (0x3F<<16)          
#define SYS_CTRL_BOOT_MODE_SHIFT       (16)                

#define SYS_CTRL_FUNCTION_TEST_MODE    (1<<31)             


#endif

#endif


