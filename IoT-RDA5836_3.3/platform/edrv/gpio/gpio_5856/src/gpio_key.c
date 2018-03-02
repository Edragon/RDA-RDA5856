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
 * MODULE NAME:    GPIO_ir.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda ir drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpioir.c 1857 2010-09-01 11:25:00Z huazeng $
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
#include "cos.h"
#include "event.h"
#include "string.h"

extern const GPIO_CFG_CONFIG_T *g_gpio_config;

PROTECTED UINT8 key_status[HAL_GPIO_MAX_GPIOKEYS];
EXPORT HAL_GPIO_IRQ_HANDLER_T g_halGpioRegistry[16];

PROTECTED VOID gpio_scan_keys()
{
    INT i;
    UINT8 value;
    COS_EVENT ev;

    ev.nEventId  = 0;
    ev.nParam1  = 0;

    for(i=0; i<g_gpio_config->key_count; i++)
    {
        if(hal_GpioGet(g_gpio_config->gpio_keys[i]) == g_gpio_config->high_keydown)
            value = 1;
        else
            value = 0;

        if(value != key_status[i])
        {
            if(value)
                ev.nEventId = EV_KEY_DOWN;
            else
                ev.nEventId = EV_KEY_UP;
            ev.nParam1 = i+2; // for double key added
            COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            key_status[i] = value;
        }
    }
}


PROTECTED VOID gpio_key_timeout(void *param)
{
    gpio_scan_keys();
}

//update by wangxin gpio
PUBLIC void gpio_Keyinit(void)
{
    int i;
    HAL_GPIO_CFG_T tfCfg;
    tfCfg.direction = HAL_GPIO_DIRECTION_INPUT;
    if(g_gpio_config->key_interrupt_mode)
    {
        tfCfg.irqHandler = gpio_scan_keys;
        tfCfg.irqMask.rising= TRUE;
        tfCfg.irqMask.falling = TRUE;
        tfCfg.irqMask.debounce = TRUE;
        tfCfg.irqMask.level=FALSE;
    }
    else
    {
        tfCfg.irqHandler = NULL;
        tfCfg.irqMask.rising= FALSE;
        tfCfg.irqMask.falling = FALSE;
        tfCfg.irqMask.debounce = FALSE;
        tfCfg.irqMask.level=FALSE;
    }

   for(i=0; i<g_gpio_config->key_count; i++)
    {
        hal_GpioOpen(g_gpio_config->gpio_keys[i],&tfCfg);
	    hal_GpioIrqSetMask(g_gpio_config->gpio_keys[i], &tfCfg.irqMask);
	    g_halGpioRegistry[g_gpio_config->gpio_keyirqnum[i]] = tfCfg.irqHandler;
    }
    
    if(!g_gpio_config->key_interrupt_mode)
    {
        COS_SetTimer(100, gpio_key_timeout, NULL, COS_TIMER_MODE_PERIODIC);
    }

    memset(key_status, 0, HAL_GPIO_MAX_GPIOKEYS);
}


