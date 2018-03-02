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

/*************************************************************************
 *
 * MODULE NAME:    GPIO_leds.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda leds drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpio_leds.c 1857 2010-09-01 11:25:00Z huazeng $
 *
 *
 * NOTES TO USERS:
 *   None.
 *
 *************************************************************************/

#include "cs_types.h"
#include "gpio_config.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "cos.h"
#include "event.h"
#include "edrvp_debug.h"
#include "opal.h"
#include "sxr_tim.h"
#include "hal_ispi.h"

extern const GPIO_CFG_CONFIG_T *g_gpio_config; 
UINT32 key_pin_mux_val = 0;
UINT16 key_pmu_val = 0;
UINT32 key_enterInterrupt = 0;

#if 0
PUBLIC VOID gpio_LEDinit()
{
    int i;
    HAL_GPIO_CFG_T tfCfg;
    tfCfg.direction = HAL_GPIO_DIRECTION_OUTPUT;
    tfCfg.irqHandler = NULL;
    tfCfg.irqMask.rising=FALSE;
    tfCfg.irqMask.falling = FALSE;
    tfCfg.irqMask.debounce = FALSE;
    tfCfg.irqMask.level=FALSE;
    for(i=0; i<g_gpio_config->led_count; i++)
    {
        if(g_gpio_config->leds[i].type == HAL_GPIO_TYPE_IO)
            hal_GpioOpen(g_gpio_config->leds[i].gpioId,&tfCfg);
    }
}
#endif
 PUBLIC VOID gpio_KeyLedMode(void)
{
	UINT16 reg_data;
	UINT32 scStatus = COS_EnterCriticalSection(); 

	//Check There is no other ispi operation pending
	if(hal_IspiCsActivate(HAL_ISPI_CS_PMU))
	{
		hal_IspiCsDeactivate(HAL_ISPI_CS_PMU);

		//Disable irq
		pmd_RegIrqSettingSet(RDA_PMU_EOMIRQ_CLEAR|RDA_PMU_KEYIRQ_CLEAR);

		//set bit15 1
		reg_data = pmd_RDARead(RDA_ADDR_SYS_CTRL);
		pmd_RDAWrite(RDA_ADDR_SYS_CTRL,RDA_PMU_SYS_CTRL_IO_KEYSENSE_ENB|reg_data);

		//iomux gpio
		hal_GpioSetPinMux(HAL_GPIO_0_4, HAL_GPIO_ALTFUNC_CFG_0);
		hal_GpioSetOut(HAL_GPIO_0_4);
	}
	
	COS_ExitCriticalSection(scStatus); 
}
 PUBLIC VOID gpio_KeySenseMode(void)
 {
 	UINT16 reg_data;
 	UINT32 scStatus = COS_EnterCriticalSection(); 

	//Check There is no other ispi operation pending
	if(hal_IspiCsActivate(HAL_ISPI_CS_PMU))
	{
		hal_IspiCsDeactivate(HAL_ISPI_CS_PMU);

		//iomux keysense
		hal_GpioSetPinMux(HAL_GPIO_0_4, key_pin_mux_val); 
		hal_GpioSetIn(HAL_GPIO_0_4);

		//set bit15 0
		reg_data = pmd_RDARead(RDA_ADDR_SYS_CTRL);
		reg_data &= ~(RDA_PMU_SYS_CTRL_IO_KEYSENSE_ENB);
		pmd_RDAWrite(RDA_ADDR_SYS_CTRL,reg_data);

		//Enable irq
		pmd_RegIrqSettingSet(RDA_PMU_KEYIRQ_MASK|RDA_PMU_EOMIRQ_CLEAR|RDA_PMU_KEYIRQ_CLEAR);
	}

	COS_ExitCriticalSection(scStatus); 
 }
 PUBLIC VOID gpio_KeyLedModeTransfer(void)
 {
 	 HAL_APO_ID_T apo;
	 //UINT32 key_down_flag = 0;
 	//int32 timeBegin = hal_TimGetUpTime();
	apo.gpioId=HAL_GPIO_0_4; 
	static int led_flopy = 0;
	if(led_flopy)
	{
		if(!key_enterInterrupt)
		{
			gpio_KeyLedMode();
			sxr_StartFunctionTimer(100 MILLI_SECONDS,gpio_KeyLedModeTransfer,(VOID*)NULL,0x03); 	
		}
	}
	else
	{
		if(!key_enterInterrupt)
		{
			gpio_KeySenseMode();
			sxr_StartFunctionTimer(5 MILLI_SECONDS,gpio_KeyLedModeTransfer,(VOID*)NULL,0x03); 
		}
	}
	led_flopy = !led_flopy;
 }
 PUBLIC VOID gpio_KeyLedInit(void)
 {
#ifdef KEY_LED_MUX_FUNCTION
 	HAL_APO_ID_T apo;
        apo.gpioId=HAL_GPIO_0_4; 
 	key_pin_mux_val = hal_GpioGetPinMux(HAL_GPIO_0_4);
	gpio_KeyLedMode();
	sxr_StartFunctionTimer(5 MILLI_SECONDS,gpio_KeyLedModeTransfer,(VOID*)NULL,0x03); 
#endif
 }
 PUBLIC VOID enable_LedKeyTimer(VOID)
 {
#ifdef KEY_LED_MUX_FUNCTION
 	sxr_StartFunctionTimer(100 MILLI_SECONDS,gpio_KeyLedModeTransfer,(VOID*)NULL,0x03); 
#endif
 }
PUBLIC VOID gpio_SetLED(int index, int on)
{
    if(on^g_gpio_config->ledHighLight)
    {
	hal_GpioClr(g_gpio_config->leds[index]);
    }
    else
    {
        hal_GpioSet(g_gpio_config->leds[index]);
    }  
}


