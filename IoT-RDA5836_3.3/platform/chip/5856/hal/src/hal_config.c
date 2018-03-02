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
//                                                                            //
/// @file hal_config.c
/// Implementation of HAL initialization related with the particular instance
/// of the IP.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"
#include "global_macros.h"
#include "cfg_regs.h"
#include "gpio.h"
#include "tcu.h"
#include "uart.h"

#include "halp_sys.h"
#include "halp_debug.h"
#include "halp_gpio.h"
#include "halp_config.h"
#include "halp_pwm.h"

#include "hal_config.h"
#include "hal_gouda.h"
#include "hal_gpio.h"
#include "hal_pwm.h"

#include "tgt_hal_cfg.h"
#include "boot_map.h"

#include "pmd_m.h"
#include "iomux.h"
#include "gpio_config.h"
#include "tgt_board_cfg.h"
#include "hal_host.h" // this is to send debug events

#define LPG_PWL_PWT_PWM_USED(bitoffset) \
                ((halCfg->lpg_pwl_pwt_pwm_mask & (0x3 << (bitoffset*2)))\
                         >> (bitoffset*2))
                         
#define PWMUSED LPG_PWL_PWT_PWM_USED(0)
#define PWTUSED LPG_PWL_PWT_PWM_USED(1)
#define PWLUSED LPG_PWL_PWT_PWM_USED(2)
#define LPGUSED LPG_PWL_PWT_PWM_USED(3)

// PWM
#define PWM_USE_GPIO_0_7 (PWMUSED == 1)
#define PWM_USE_GPIO_3_4 (PWMUSED == 2)

// PWT
#define PWT_USE_GPIO_0_6 (PWTUSED == 1)
#define PWT_USE_GPIO_3_5 (PWTUSED == 2)

// PWL
#define PWL_USE_GPIO_3_6 (PWLUSED == 2)

// LPG
#define LPG_USE_GPIO_0_5 (LPGUSED == 1)
#define LPG_USE_GPIO_3_7 (LPGUSED == 2)

#define ADD_AVAILABLE_GPIO(index, offset)   \
        availableGpio[(index)/4] |= HAL_GPIO_BIT(index, offset);

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

CONST HAL_CFG_CONFIG_T* g_halCfg;
extern PROTECTED BoardSetup_GPIO_CFG BoardsetupGpioCfg[TGT_HAL_PINNUM];
// =============================================================================
//  FUNCTIONS
// =============================================================================
extern PUBLIC VOID pmd_SoftStart(VOID);

// =============================================================================
// hal_BoardConfigClk32k
// -----------------------------------------------------------------------------
/// Configure CLK_32K output.
/// @param enable TRUE to configure, and FALSE to disable.
// ============================================================================
PROTECTED VOID hal_BoardConfigClk32k(BOOL enable)
{
    HAL_ASSERT(g_halCfg->useClk32k == TRUE, "32K clock is not configured");

    if (enable)
    {
        // Setup the pin as 32K clock output
        hwp_configRegs->Alt_mux_select =
            (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
            CFG_REGS_PWL1_CLK_32K;
    }
    else
    {
        // Setup the pin as GPO (and low ouput has been
        // configured in hal_BoardSetup())
        hwp_configRegs->Alt_mux_select =
            (hwp_configRegs->Alt_mux_select & ~CFG_REGS_PWL1_MASK) |
            CFG_REGS_PWL1_GPO_6;
    }
}


// =============================================================================
// hal_BoardSetupFp
// -----------------------------------------------------------------------------
/// Apply board dependent configuration to HAL for flash programmer
/// @param halCfg Pointer to HAL configuration structure (from the target
/// module).
// ============================================================================
#if defined(_FLASH_PROGRAMMER) && defined(FORCE_GPIO_INPUT_GPO_LOW)
PROTECTED VOID hal_BoardSetupFp(CONST HAL_CFG_CONFIG_T* halCfg)
{
    // Store the config pointer for later use in hal
    g_halCfg = halCfg;

    // Setup all GPIO pins as input mode
    hwp_configRegs->GPIO_Mode = 0xFFFFFFFF;
//    hwp_gpio->gpio_oen_set_in = 0xFFFFFFFF;
    // Set all GPO pins to low level
//    hwp_gpio->gpo_clr = 0xFFFFFFFF;

    // Set the LPSCOs polarity from PMD
    // this is assuming xvc needs V_RF, anyway pal will get the real info
    // from xcv and configure LPS properly.
    //hwp_tcu->LPS_PU_Ctrl |= pmd_ConfigureRfLowPower(TRUE, FALSE);  Not use tcu in 5856

    // Initialize Clock frequency global variables :
    // Out of boot, the system frequency is on the RF clock
    // that is (normally) 26 MHz, but this info is confirmed/given
    // in the configuration structure, so help ourselves !
    g_halSysSystemFreq = halCfg->rfClkFreq;
}
#endif

// =============================================================================
// hal_BoardSetupGeneral
// -----------------------------------------------------------------------------
/// Apply board dependent configuration to HAL for general purpose
/// @param halCfg Pointer to HAL configuration structure (from the target
/// module).
// =============================================================================
// =============================================================================
// pin number
// =============================================================================

PROTECTED VOID hal_BoardSetupGeneral(void)
{
    UINT8 i;
    HAL_APO_ID_T apo;
    UINT8 offset_index = 0;
    HAL_GPIO_CFG_T cfg;
    UINT32 hst_count = 0;
    UINT32 *hst_config = (UINT32*)0xa1c00000;

    hal_HstSendEvent(SYS_EVENT,0x55555555);
    hal_HstSendEvent(SYS_EVENT,sizeof(BoardsetupGpioCfg));
    hal_HstSendEvent(SYS_EVENT,sizeof(BoardsetupGpioCfg[0]));
    for(i=0;i<TGT_HAL_PINNUM;i++)
    {
        apo.gpioId = BoardsetupGpioCfg[i].id;
        if(BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_4) // save host uart config for download
        {
            hst_count++;
            hst_config[hst_count*2] = (UINT32) &(hwp_iomux->IOMUX_PXX_CTRL[apo.id/8][apo.id%8]);
            hst_config[hst_count*2+1] = 4;
        }

		if((BoardsetupGpioCfg[i].id == HAL_GPIO_4_0)||(BoardsetupGpioCfg[i].id == HAL_GPIO_3_7))
	    {
	        continue;
		}
		
        if(BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_0)
    	{
    	    if((BoardsetupGpioCfg[i].id == HAL_GPIO_0_6)||(BoardsetupGpioCfg[i].id == HAL_GPIO_0_7))
    	    {
    	        hal_GpioAltFuncConfig(BoardsetupGpioCfg[i].id, BoardsetupGpioCfg[i].altcfg, BoardsetupGpioCfg[i].drv, BoardsetupGpioCfg[i].updown);
    		    continue;
    	    }
    	    cfg.direction = BoardsetupGpioCfg[i].direction;
    	    cfg.drv    = BoardsetupGpioCfg[i].drv;
    	    cfg.updown  =BoardsetupGpioCfg[i].updown;
    	    hal_GpioOpen(BoardsetupGpioCfg[i].id, &cfg);
    	}
    	else
    	{
    	    if((BoardsetupGpioCfg[i].id == HAL_GPIO_4_3)||(BoardsetupGpioCfg[i].id == HAL_GPIO_4_4)||(BoardsetupGpioCfg[i].id == HAL_GPIO_4_5))
    	    {
    		   if(BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_3)
    		   {
    		       offset_index = apo.id%8;
    		       hwp_iomux->IOMUX_PXX_CTRL[4][offset_index] |= IOMUX_PXX_CFG(HAL_GPIO_ALTFUNC_CFG_3);
    		       cfg.direction = BoardsetupGpioCfg[i].direction;
    	           cfg.drv    = BoardsetupGpioCfg[i].drv;
    	           cfg.updown  =BoardsetupGpioCfg[i].updown;
    		       hal_GpioOpen(BoardsetupGpioCfg[i].id, &cfg);
    			   continue;
    		   }
    	    }
    	    if((BoardsetupGpioCfg[i].enint == TRUE) && (BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_7))
            {
                hal_GpioIrqConfig(BoardsetupGpioCfg[i].id, BoardsetupGpioCfg[i].irqMask,i);
                continue;
            }
    	    hal_GpioAltFuncConfig(BoardsetupGpioCfg[i].id, BoardsetupGpioCfg[i].altcfg, BoardsetupGpioCfg[i].drv, BoardsetupGpioCfg[i].updown);
    	}
    }

    if(hst_count > 0)
    {
        hst_config[0] = hst_count*2;
        hst_config[1] = BOOT_SECTOR_IOMUX_VALID_TAG;
    }
    else
    {
        hst_config[0] = 0;
        hst_config[1] = 0;
    }
}

PROTECTED VOID hal_BoardConfigP37P40(void)
{
    UINT8 i;
    //HAL_APO_ID_T apo;
    //UINT8 offset_index = 0;
    HAL_GPIO_CFG_T cfg;
    
  
    for(i=0;i<TGT_HAL_PINNUM;i++)
    {
	   if((BoardsetupGpioCfg[i].id != HAL_GPIO_4_0)&&(BoardsetupGpioCfg[i].id != HAL_GPIO_3_7))
	   {
	        continue;
       }
		
       if(BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_0)
       {
    	    cfg.direction = BoardsetupGpioCfg[i].direction;
    	    cfg.drv    = BoardsetupGpioCfg[i].drv;
    	    cfg.updown  =BoardsetupGpioCfg[i].updown;
    	    hal_GpioOpen(BoardsetupGpioCfg[i].id, &cfg);
    	}
    	else
    	{
    	    if((BoardsetupGpioCfg[i].enint == TRUE) && (BoardsetupGpioCfg[i].altcfg == HAL_GPIO_ALTFUNC_CFG_7))
            {
                hal_GpioIrqConfig(BoardsetupGpioCfg[i].id, BoardsetupGpioCfg[i].irqMask,i);
                continue;
            }
    	    hal_GpioAltFuncConfig(BoardsetupGpioCfg[i].id, BoardsetupGpioCfg[i].altcfg, BoardsetupGpioCfg[i].drv, BoardsetupGpioCfg[i].updown);
    	}
    }
}

// =============================================================================
// hal_BoardSetup
// -----------------------------------------------------------------------------
/// Apply board dependent configuration to HAL
/// @param halCfg Pointer to HAL configuration structure (from the target
/// module).
// ============================================================================
PROTECTED VOID hal_BoardSetup(CONST HAL_CFG_CONFIG_T* halCfg)
{
    // Store the config pointer for later use in hal
    g_halCfg = halCfg;

#if defined(_FLASH_PROGRAMMER) && defined(FORCE_GPIO_INPUT_GPO_LOW)

    hal_BoardSetupFp(halCfg);

#else // ! (_FLASH_PROGRAMMER && FORCE_GPIO_INPUT_GPO_LOW) 
	pmd_SoftStart();
    hal_TimDelay(1000 MS_WAITING);
    hal_BoardSetupGeneral();
   
#endif //!(_FLASH_PROGRAMMER && FORCE_GPIO_INPUT_GPO_LOW)

    // Set the LPSCOs polarity from PMD
    // this is assuming xvc needs V_RF, anyway pal will get the real info
    // from xcv and configure LPS properly.
    //hwp_tcu->LPS_PU_Ctrl |= pmd_ConfigureRfLowPower(TRUE, FALSE);

    // Initialize Clock frequency global variables :
    // Out of boot, the system frequency is on the RF clock
    // that is (normally) 26 MHz, but this info is confirmed/given
    // in the configuration structure, so help ourselves !
    g_halSysSystemFreq = halCfg->rfClkFreq;
    
}


// TODO: To be removed after 8809p has its own PMD driver
PUBLIC VOID hal_ConfigFmIo(BOOL enable) {}
PUBLIC VOID hal_ConfigBtIo(BOOL enable) {}
