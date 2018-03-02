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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_rf_config.c                                                    //
/// Implementation of HAL initialization related with the particular instance
/// of the IP.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "cfg_regs.h"
#include "halp_debug.h"
#include "halp_sys.h"
#include "hal_sys.h"
#include "hal_rf.h"
#include "rf_if.h"
#include "sxr_tls.h"


void hal_Rf_SoftReset( void ) 
{
    hwp_rf_if->sys_control = 0x0;
    hwp_rf_if->sys_control= RF_IF_SOFT_RESETN;
}
  
void hal_Rf_Self_Calibration(void) 
{
    hwp_rf_if->sys_control &= ~RF_IF_CHIP_SELF_CAL_ENABLE;
    hwp_rf_if->sys_control |= RF_IF_CHIP_SELF_CAL_ENABLE;
    while (((hwp_rf_if->cal_status) & (RF_IF_BT_SELF_CAL_READY|RF_IF_FM_SELF_CAL_READY)) == 0) 
    {
    }
}

void hal_Rf_Set_Rf_Mode( uint8 rf_mode ) 
{ 
    if (rf_mode == 0)    //bt mode
    {
	   hwp_rf_if->sys_control &= ~RF_IF_RF_MODE;
	   hwp_rf_if->rxflt_adc_setting |= RF_IF_ADC_CLK_DIV2;
    } 
    else                         //fm mode
    {
	   hwp_rf_if->sys_control |= RF_IF_RF_MODE;
	   hwp_rf_if->rxflt_adc_setting &= ~RF_IF_ADC_CLK_DIV2;
    }
}

void hal_Rf_set_Fm_AdcClk_Mode(uint8 clkmode)
{
    hwp_rf_if->sys_control &= ~RF_IF_FM_ADCLK_MODE_MASK;
    if(clkmode == 0)
    {
        hwp_rf_if->sys_control |= RF_IF_FM_ADCLK_MODE_DIVIDER;
    }
    else if(clkmode == 1)
    {
         hwp_rf_if->sys_control |= RF_IF_FM_ADCLK_MODE_AUDIO;
    }
    else
    {
         hwp_rf_if->sys_control |= RF_IF_FM_ADCLK_MODE_ADPLL;
		 
         //turn on fm clk
         hwp_rf_if->adpll_setting1 |= RF_IF_ADPLL_FM_SEL;
         hwp_rf_if->adpll_setting1 |= RF_IF_ADPLL_CLK_FM_EN; 
    }
}

void hal_RF_Gain_Table_Init(uint32 lan_gain, uint32 rmx_gain0, uint32 rmx_gain1 ,uint32 rxflt_gain)
{
    hwp_rf_if->fm_lna_gain_table = lan_gain; 
    hwp_rf_if->fm_rmx_gain_table_0 = rmx_gain0; 
    hwp_rf_if->fm_rmx_gain_table_1 = rmx_gain1; 
    hwp_rf_if->fm_rxflt_gain_table = rxflt_gain; 
}

void hal_RF_Init(void)
{
    hal_Rf_SoftReset();
    hal_Rf_Self_Calibration();
    hal_Rf_set_Fm_AdcClk_Mode(0);
    hal_RF_Gain_Table_Init(0x1bffffff,0x12,0x34567777,0x1b);
}
