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
 * MODULE NAME:    GPIO_edrv.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda gpio drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpio_edrv.c 1857 2010-09-01 11:25:00Z huazeng $
 *
 *
 * NOTES TO USERS:
 *   None.
 *
 *************************************************************************/

#include "cs_types.h"
#include "tgt_app_cfg.h"
#include "gpio_config.h"
#include "gpio_edrv.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"
#include "sxr_tim.h"
#include "sxr_tls.h"
#include "register.h"
#include "sys_irq.h"

const GPIO_CFG_CONFIG_T *g_gpio_config;


PUBLIC VOID gpio_SetMute(BOOL mute_flag)
{
    HAL_APO_ID_T gpioMute = g_gpio_config->gpioMute;
    if (HAL_GPIO_NONE == g_gpio_config->gpioMute.gpioId)
       return ;

   if (FALSE == g_gpio_config->gpioMuteEnable)
       return ;

    if(mute_flag)
    {
        if(g_gpio_config->gpioMuteHigh)
            hal_GpioSet(gpioMute);
        else
            hal_GpioClr(gpioMute);
    }
    else
    {
        if(g_gpio_config->gpioMuteHigh)
            hal_GpioClr(gpioMute);
        else
            hal_GpioSet(gpioMute);
    }
}

PUBLIC VOID gpio_ConvSdFunction(void)
{
    UINT32 gpio_mask;
    //UINT32 status = hal_SysEnterCriticalSection();

    gpio_mask = hwp_sysIrq->mask_set & SYS_IRQ_SYS_IRQ_GPIO;
    hwp_sysIrq->mask_clear = SYS_IRQ_SYS_IRQ_GPIO;

    hal_GpioSetPinMux(g_gpio_config->cardDetectGpio, HAL_GPIO_ALTFUNC_CFG_4);

    //Clear all card detect it during switch iomux
    hal_TimDelay(2 MS_WAITING);
    while(hwp_gpio->int_status & (1<<g_gpio_config->cardDetectGpioIntNum))
    {
        hwp_gpio->int_clear = 1<<g_gpio_config->cardDetectGpioIntNum;
    }

    hwp_sysIrq->mask_set = gpio_mask;
    //hal_SysExitCriticalSection(status);
}

PUBLIC VOID gpio_ConvDetFunction(void)
{
    UINT32 gpio_mask;
    //UINT32 status = hal_SysEnterCriticalSection();
  //  hal_HstSendEvent(SYS_EVENT,0x20170614);

    gpio_mask = hwp_sysIrq->mask_set & SYS_IRQ_SYS_IRQ_GPIO;
    hwp_sysIrq->mask_clear = SYS_IRQ_SYS_IRQ_GPIO;
    hal_GpioSetPinMux(g_gpio_config->cardDetectGpio, HAL_GPIO_ALTFUNC_CFG_11);
    hal_GpioUpOrDn(g_gpio_config->cardDetectGpio,HAL_GPIO_UP);

    //Clear all card detect it during switch iomux
    hal_TimDelay(2 MS_WAITING);
    while(hwp_gpio->int_status & (1<<g_gpio_config->cardDetectGpioIntNum))
    {
        hwp_gpio->int_clear = 1<<g_gpio_config->cardDetectGpioIntNum;
    }

    hwp_sysIrq->mask_set = gpio_mask;
    //hal_SysExitCriticalSection(status);
}

PUBLIC void gpio_initialise(VOID)
{
    g_gpio_config = tgt_GetGPIOCfg();
    
#ifdef GPIO_IR_SUPPORT
    if(g_gpio_config->gpio_irc != HAL_GPIO_NONE)
        gpio_IRCinit(g_gpio_config->gpio_irc);
#endif
    if(g_gpio_config->key_count > 0)
    	{
    	 extern PUBLIC void gpio_Keyinit(void);
        gpio_Keyinit();
    }
	
#if (APP_SUPPORT_I2C_TRACE == 0)
    if (g_gpio_config->gpioMute.gpioId != HAL_GPIO_NONE)
	{
		gpio_MutePinInit();
	}
	if(g_gpio_config->gpio_lineIn != HAL_GPIO_NONE)
	{
              gpio_LineinDetectIrqInit();
              gpio_LineinDetectDelay(NULL);
	} 
#else
       if (g_gpio_config->gpioMute.gpioId != HAL_GPIO_NONE)
	{
	       extern VOID gpio_MutePinInit(void);
		gpio_MutePinInit();
	}
	if(g_gpio_config->gpio_lineIn != HAL_GPIO_NONE)
	{
	       extern VOID gpio_LineinDetectIrqInit(void);
		 extern VOID gpio_LineinDetectDelay(VOID* param);
              gpio_LineinDetectIrqInit();
              gpio_LineinDetectDelay(NULL);
	} 
#endif

}



