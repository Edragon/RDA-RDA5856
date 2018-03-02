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
#ifndef _8809P_ASM_H_
#define _8809P_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
#error "You are trying to use in a normal C code the assembly H description of 'gallite'."
#endif

#include "globals_asm.h"
#include "global_macros_asm.h"
#include "8809p_generic_config_asm.h"
#include "bb_ahb_monitor_asm.h"
#include "bb_cp2_asm.h"
#include "bb_ifc_asm.h"
#include "bb_irq_asm.h"
#include "bb_sram_asm.h"
#include "comregs_asm.h"
#include "bcpu_cache_asm.h"
#include "bcpu_tag_asm.h"
#include "bist_asm.h"
#include "cipher_asm.h"
#include "debug_host_asm.h"
#include "debug_host_internals_asm.h"
#include "debug_uart_asm.h"
#include "dma_asm.h"
#include "gouda_asm.h"
#include "mem_bridge_asm.h"
#include "debug_port_asm.h"
#include "gpio_asm.h"
#include "i2c_master_asm.h"
#include "itlv_asm.h"
#include "page_spy_asm.h"
#include "rf_if_asm.h"
#include "rf_spi_asm.h"
#include "sci_asm.h"
#include "spi_asm.h"
#include "sys_ahb_monitor_asm.h"
#include "sys_ctrl_asm.h"
#include "sys_ifc_asm.h"
#include "sys_ifc2_asm.h"
#include "sys_irq_asm.h"
#include "tcu_asm.h"
#include "timer_asm.h"
#include "uart_asm.h"
#include "vitac_asm.h"
#include "xcor_asm.h"
#include "cp0_asm.h"
#include "regdef_asm.h"
#include "xcpu_cache_asm.h"
#include "xcpu_tag_asm.h"
#include "xcpu_asm.h"
#include "keypad_asm.h"
#include "pwm_asm.h"
#include "calendar_asm.h"
#include "aif_asm.h"
#include "usbc_asm.h"
#include "sdmmc_asm.h"
#include "camera_asm.h"
#include "cfg_regs_asm.h"
#include "voc_ram_asm.h"
#include "voc_ahb_asm.h"
#include "voc_cfg_asm.h"
#include "tester_chip_asm.h"
#include "test_uart_asm.h"
#include "memory_burst_adapter_asm.h"

/// System APB base
#define REG_SYS_APB_BASE                         (0X01A00000)
/// Baseband APB base
#define REG_BB_APB_BASE                          (0X01900000)
/// VOC AHB base
#define REG_VOC_BASE                             (0X01940000)


#endif
