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
#include "gpio_config.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"
#include "sxr_tim.h"
#include "sxr_tls.h"

const GPIO_CFG_CONFIG_T *g_gpio_config;


PRIVATE VOID gpio_Muteinit(void)
{
    int i;
    HAL_GPIO_CFG_T tfCfg;
    HAL_APO_ID_T gpioMute = g_gpio_config->gpioMute;

    if (FALSE == g_gpio_config->gpioMuteEnable)
        return ;
    tfCfg.direction = HAL_GPIO_DIRECTION_OUTPUT;
    tfCfg.irqHandler = NULL;
    tfCfg.irqMask.rising=FALSE;
    tfCfg.irqMask.falling = FALSE;
    tfCfg.irqMask.debounce = FALSE;
    tfCfg.irqMask.level=FALSE;

    if(gpioMute.type == HAL_GPIO_TYPE_IO)
        hal_GpioOpen(gpioMute.gpioId,&tfCfg);

    gpio_SetMute(TRUE);
}



PUBLIC VOID gpio_SetMute(BOOL mute_flag)
{
    HAL_APO_ID_T gpioMute = g_gpio_config->gpioMute;

   if (FALSE == g_gpio_config->gpioMuteEnable)
       return ;

    if(mute_flag)
    {

        if(g_gpio_config->gpioMuteHigh)
            hal_GpioSet(gpioMute);
        else
            hal_GpioClr(gpioMute);
        hal_HstSendEvent(SYS_EVENT,0x09031348);
    }
    else
    {
        if(g_gpio_config->gpioMuteHigh)
            hal_GpioClr(gpioMute);
        else
            hal_GpioSet(gpioMute);
        hal_HstSendEvent(SYS_EVENT,0x09031349);
    }
}

PUBLIC UINT32 gpio_initialise(VOID)
{
    g_gpio_config = tgt_GetGPIOCfg();
    HAL_APO_ID_T gpioMute = g_gpio_config->gpioMute;
    
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    if(g_gpio_config->gpio_irc != HAL_GPIO_NONE)
        gpio_IRCinit(g_gpio_config->gpio_irc);
#endif

    //if(g_gpio_config->led_count > 0)
    //    gpio_LEDinit();

    if(g_gpio_config->key_count > 0)
        gpio_Keyinit();

    if (gpioMute.gpoId != HAL_GPO_NONE)
        gpio_Muteinit(); //2012-9-27

    gpio_DetectInit();
}

