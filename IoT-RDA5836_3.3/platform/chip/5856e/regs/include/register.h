/* 
 * Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

                                            

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

//Auto-gen by reg_gen

//#include "bt_link.h"
#include "bb_ifc.h"
#include "aif.h"
#include "audio_codec.h"
#include "fm_dsp.h"
#include "voc_cfg.h"
#include "voc_ahb.h"
#include "sys_ctrl.h"
#include "pmuc.h"
#include "sys_ifc.h"
#include "iomux.h"
#include "uart.h"
#include "sdmmc.h"
#include "spi.h"
#include "debug_uart.h"
#include "dma.h"
#include "calendar.h"
#include "ir.h"
#include "gpio.h"
#include "timer.h"
#include "pwm.h"
#include "usb_role_det.h"
#include "bt_core.h"
#include "dbm.h"
#include "gpadc.h"
#include "i2c_master.h"
#include "wdt.h"
#include "trap.h"
#include "bt_modem.h"
#include "sys_irq.h"
#include "xcpu_reg.h"
#include "rf_if.h"
#include "abb.h"
#include "pmu_intf.h"
#include "cfg_regs.h"
#include "debug_host.h"
#include "usbc.h"
#include "spiflash.h"

#define hwp_abb ((T_HWP_ABB_T*)REG_ABB_BASE)
#define hwp_uart ((T_HWP_UART_T*)REG_UART_BASE)
//#define hwp_rom1 ((T_HWP_ROM1_T*)REG_ROM1_BASE)
#define hwp_sys_irq ((T_HWP_SYS_IRQ_T*)REG_SYS_IRQ_BASE)
#define hwp_pwm ((T_HWP_PWM_T*)REG_PWM_BASE)
#define hwp_timer ((T_HWP_TIMER_T*)REG_TIMER_BASE)
#define hwp_bt_modem ((T_HWP_BT_MODEM_T*)REG_BT_MODEM_BASE)
#define hwp_bt_core ((T_HWP_BT_CORE_T*)REG_BT_CORE_BASE)
#define hwp_trap ((T_HWP_TRAP_T*)REG_TRAP_BASE)
#define hwp_wdt ((T_HWP_WDT_T*)REG_WDT_BASE)
#define hwp_cfg_regs ((T_HWP_CFG_REGS_T*)REG_CFG_REGS_BASE)
#define hwp_sdmmc ((T_HWP_SDMMC_T*)REG_SDMMC_BASE)
//#define hwp_voc_iram ((T_HWP_VOC_IRAM_T*)REG_VOC_IRAM_BASE)
//#define hwp_voc_xram ((T_HWP_VOC_XRAM_T*)REG_VOC_XRAM_BASE)
#define hwp_debug_host ((T_HWP_DEBUG_HOST_T*)REG_DEBUG_HOST_BASE)
#define hwp_debug_uart ((T_HWP_DEBUG_UART_T*)REG_DEBUG_UART_BASE)
#define hwp_pmuc ((T_HWP_PMUC_T*)REG_PMUC_BASE)
#define hwp_fm_dsp ((T_HWP_FM_DSP_T*)REG_FM_DSP_BASE)
#define hwp_calendar ((T_HWP_CALENDAR_T*)REG_CALENDAR_BASE)
#define hwp_ir ((T_HWP_IR_T*)REG_IR_BASE)
#define hwp_voc_ahb ((T_HWP_VOC_AHB_T*)REG_VOC_AHB_BASE)
#define hwp_sys_ctrl ((T_HWP_SYS_CTRL_T*)REG_SYS_CTRL_BASE)
#define hwp_usb_role_det ((T_HWP_USB_ROLE_DET_T*)REG_USB_ROLE_DET_BASE)
#define hwp_aif ((T_HWP_AIF_T*)REG_AIF_BASE)
#define hwp_dbm ((T_HWP_DBM_T*)REG_DBM_BASE)
//#define hwp_bt_link ((T_HWP_BT_LINK_T*)REG_BT_LINK_BASE)
#define hwp_usb ((T_HWP_USB_T*)REG_USB_BASE)
#define hwp_sys_ifc ((T_HWP_SYS_IFC_T*)REG_SYS_IFC_BASE)
#define hwp_iomux ((T_HWP_IOMUX_T*)REG_IOMUX_BASE)
#define hwp_spi ((T_HWP_SPI_T*)REG_SPI_BASE)
#define hwp_gpadc ((T_HWP_GPADC_T*)REG_GPADC_BASE)
#define hwp_i2c_master ((T_HWP_I2C_MASTER_T*)REG_I2C_MASTER_BASE)
#define hwp_dma ((T_HWP_DMA_T*)REG_DMA_BASE)
#define hwp_bb_ifc ((T_HWP_BB_IFC_T*)REG_BB_IFC_BASE)
#define hwp_rf_if ((T_HWP_RF_IF_T*)REG_RF_IF_BASE)
#define hwp_spiflash ((T_HWP_SPIFLASH_T*)REG_SPIFLASH_BASE)
//#define hwp_spiflash_mem ((T_HWP_SPIFLASH_MEM_T*)REG_SPIFLASH_MEM_BASE)
#define hwp_voc_cfg ((T_HWP_VOC_CFG_T*)REG_VOC_CFG_BASE)
//#define hwp_rom2 ((T_HWP_ROM2_T*)REG_ROM2_BASE)
#define hwp_audio_codec ((T_HWP_AUDIO_CODEC_T*)REG_AUDIO_CODEC_BASE)
#define hwp_gpio ((T_HWP_GPIO_T*)REG_GPIO_BASE)
//#define hwp_buf_ram ((T_HWP_BUF_RAM_T*)REG_BUF_RAM_BASE)
//#define hwp_voc_yram ((T_HWP_VOC_YRAM_T*)REG_VOC_YRAM_BASE)
//#define hwp_data_ram2 ((T_HWP_DATA_RAM2_T*)REG_DATA_RAM2_BASE)
//#define hwp_data_ram1 ((T_HWP_DATA_RAM1_T*)REG_DATA_RAM1_BASE)
#define hwp_xcpu_reg ((T_HWP_XCPU_REG_T*)REG_XCPU_REG_BASE)
#define hwp_pmu_intf ((T_HWP_PMU_INTF_T*)REG_PMU_INTF_BASE)

#endif





