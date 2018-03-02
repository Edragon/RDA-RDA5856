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
#include "register.h"

extern const GPIO_CFG_CONFIG_T *g_gpio_config; 
#ifdef KEY_LED_MUX_FUNCTION

UINT32 key_pin_mux_val = 0;
UINT16 key_pmu_val = 0;
UINT32 key_enterInterrupt = 0;
#endif
UINT8 gpio_led_on = 0;

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
	//UINT16 reg_data;
	UINT32 scStatus = COS_EnterCriticalSection(); 

    hwp_pmuc->pwr_sys_ctrl |= PMUC_KEYSENSE_ENB;

	//iomux gpio
	hal_GpioSetPinMux(HAL_GPIO_0_0, HAL_GPIO_ALTFUNC_CFG_0);
	hal_GpioSetOut(HAL_GPIO_0_0);
	
	COS_ExitCriticalSection(scStatus); 
}
#ifdef KEY_LED_MUX_FUNCTION

 PUBLIC VOID gpio_KeySenseMode(void)
 {
 	UINT32 scStatus = COS_EnterCriticalSection(); 


	//iomux keysense
	hal_GpioSetPinMux(HAL_GPIO_0_0, key_pin_mux_val); 
	hal_GpioSetIn(HAL_GPIO_0_0);

    hwp_pmu_intf->keysense_ctrl1 &=~PMU_INTF_KEY_ENABLE;
    hwp_pmuc->pwr_sys_ctrl &= ~PMUC_KEYSENSE_ENB;
    hwp_pmu_intf->keysense_ctrl1 |=PMU_INTF_KEY_ENABLE;

	COS_ExitCriticalSection(scStatus); 
 }
 PUBLIC VOID gpio_KeyLedModeTransfer(void)
 {
 	 HAL_APO_ID_T apo;
	 UINT32 key_down_flag = 0;
 	int32 timeBegin = hal_TimGetUpTime();
	apo.gpioId=HAL_GPIO_0_0; 
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
 #endif
 PUBLIC VOID gpio_KeyLedInit(void)
 {
#ifdef KEY_LED_MUX_FUNCTION
 	HAL_APO_ID_T apo;
        apo.gpioId=HAL_GPIO_0_0; 
 	key_pin_mux_val = hal_GpioGetPinMux(HAL_GPIO_0_0);
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
    if(index>=g_gpio_config->led_count)
        return;
    if(on^g_gpio_config->ledHighLight)
    {
    	hal_GpioClr(g_gpio_config->leds[index]);
       if(HAL_GPIO_NONE != g_gpio_config->leds[index].gpioId)
            gpio_led_on &=~(1<<index);
    }
    else
    {
        hal_GpioSet(g_gpio_config->leds[index]);
        if(HAL_GPIO_NONE != g_gpio_config->leds[index].gpioId)
            gpio_led_on |= 1<<index;
    }  
}


