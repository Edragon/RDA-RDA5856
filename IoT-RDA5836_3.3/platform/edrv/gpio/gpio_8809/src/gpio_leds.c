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


extern const GPIO_CFG_CONFIG_T *g_gpio_config;

/*
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
*/

PUBLIC VOID gpio_SetLED(int index, int on)
{
    if(on^g_gpio_config->ledHighLight)
    {
              pmd_SetLevel(g_gpio_config->leds[index],0);
    }
    else
    {
             pmd_SetLevel(g_gpio_config->leds[index],1);
    }
}


