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
 * MODULE NAME:    GPIO_detect.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda gpio detect drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpio_linein.c 1857 2010-09-01 11:25:00Z huazeng $
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
#include "hal_host.h"
#include "cos.h"
#include "event.h"
#include "gpio_edrv.h"
#include "tgt_board_cfg.h"
#include "sys_irq.h"
#include "tgt_app_cfg.h"

extern const GPIO_CFG_CONFIG_T *g_gpio_config;
UINT8 volatile EarDetectFlag=0xaa;
UINT8 linein_time_handle;

#ifdef MCD_TFCARD_SUPPORT
#define  GPIO_TCARD_DETECT_INTERVAL  500
UINT8 g_tflash_detect_flag;
static UINT8 g_timer_id_gpio_card = 0;
#endif
PUBLIC BOOL mcd_CardDetectUpdateStatus(VOID);

PUBLIC UINT32 gpio_detect_linein(void)
{
    if(g_gpio_config->gpio_lineIn == HAL_GPIO_NONE)
        return GPIO_INVALID;
    if(hal_GpioGet(g_gpio_config->gpio_lineIn) == g_gpio_config->lineinPlugHigh)
        return GPIO_DETECTED;
    return GPIO_NOT_DETECTED;
}

PROTECTED VOID gpio_IrqLineinHandler()
{
    COS_EVENT ev;
    ev.nEventId  = EV_LINE_IN;
    if(hal_GpioGet(g_gpio_config->gpio_lineIn) == g_gpio_config->lineinPlugHigh)
        ev.nParam1  = 1;
    else
        ev.nParam1  = 0;

    COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

VOID gpio_LineinDetectDelay(VOID* param)
{
    HAL_GPIO_CFG_T tfCfg;
    HAL_GPIO_GPIO_ID_T gpio = g_gpio_config->gpio_lineIn;
    COS_EVENT ev;
	ev.nEventId  = EV_LINE_IN;

    if (hal_GpioGet(gpio) == g_gpio_config->lineinPlugHigh)
    {
            ev.nParam1  = 1;
    }
    else
    {
            ev.nParam1  = 0;
    }
    
	COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	
    tfCfg.irqMask.rising = TRUE;
    tfCfg.irqMask.falling = TRUE;
    tfCfg.irqMask.debounce = TRUE;
    tfCfg.irqMask.level = FALSE;
    hal_GpioIrqSetMask(gpio, &tfCfg.irqMask);
    hal_GpioResetIrq(gpio);
    linein_time_handle = 0;
}

VOID gpio_LineinDetectHandler(VOID)
{
    HAL_GPIO_GPIO_ID_T gpio = g_gpio_config->gpio_lineIn;
    HAL_GPIO_CFG_T tfCfg;
    tfCfg.irqMask.rising = FALSE;
    tfCfg.irqMask.falling = FALSE;
    tfCfg.irqMask.debounce = TRUE;
    tfCfg.irqMask.level = FALSE;
    hal_GpioIrqSetMask(gpio, &tfCfg.irqMask);
    hal_GpioResetIrq(gpio);
    //sxr_StopFunctionTimer(gpio_EarDetectDelay);
    COS_KillTimer(linein_time_handle);
    linein_time_handle = COS_SetTimer(1500, gpio_LineinDetectDelay, NULL, COS_TIMER_MODE_SINGLE);
}


VOID gpio_LineinDetectIrqInit(void)
{
    HAL_GPIO_IRQ_MASK_T tfCfg;
    tfCfg.rising=TRUE;
    tfCfg.falling = TRUE;
    tfCfg.debounce = TRUE;
    tfCfg.level=FALSE;
    hal_GpioSetPinMux(g_gpio_config->gpio_lineIn, HAL_GPIO_ALTFUNC_CFG_7);
    hal_GpioUpOrDn(g_gpio_config->gpio_lineIn,HAL_GPIO_UP);
    hal_GpioIrqSetMask(g_gpio_config->gpio_lineIn,&tfCfg);
    hal_GpioIrqSetHandler(g_gpio_config->gpio_lineIn,gpio_LineinDetectHandler);
}


VOID gpio_MutePinInit(void)
{
    hal_GpioSetPinMux(g_gpio_config->gpioMute.gpioId, HAL_GPIO_ALTFUNC_CFG_0);
    hal_GpioSetOut(g_gpio_config->gpioMute.gpioId);
}


// =============================================================================
//  Earphone detection stuff
// =============================================================================
UINT8 ear_time_handle;

VOID gpio_EarDetectDelay(VOID *parm)
{
    HAL_GPIO_CFG_T eraCfg;
    HAL_GPIO_GPIO_ID_T gpio = g_gpio_config->earpieceDetectGpio;

   // if (ear_handler != NULL)
    {
    	COS_EVENT ev;
        if (hal_GpioGet(gpio) == g_gpio_config->earpiecePlugHigh)
        {
            //(*ear_handler)(FALSE);
            ev.nParam1 = 1;
        }
        else
        {
            //(*ear_handler)(TRUE);
            ev.nParam1 = 0;
        }
		ev.nEventId = EV_DM_EAR_PEACE_DETECT_IND;
		COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        
    }
    eraCfg.irqMask.rising = TRUE;
    eraCfg.irqMask.falling = TRUE;
    eraCfg.irqMask.debounce = TRUE;
    eraCfg.irqMask.level = FALSE;
    hal_GpioIrqSetMask(gpio, &eraCfg.irqMask);
    hal_GpioResetIrq(gpio);
    ear_time_handle = 0;
}

VOID gpio_EarDetectHandler(VOID)
{
    HAL_GPIO_GPIO_ID_T gpio = g_gpio_config->earpieceDetectGpio;
    HAL_GPIO_CFG_T eraCfg;
    eraCfg.irqMask.rising = FALSE;
    eraCfg.irqMask.falling = FALSE;
    eraCfg.irqMask.debounce = TRUE;
    eraCfg.irqMask.level = FALSE;
    hal_GpioIrqSetMask(gpio, &eraCfg.irqMask);
    hal_GpioResetIrq(gpio);
    //sxr_StopFunctionTimer(gpio_EarDetectDelay);
    COS_KillTimer(ear_time_handle);
    ear_time_handle = COS_SetTimer(100, gpio_EarDetectDelay, NULL, COS_TIMER_MODE_SINGLE);
}

PUBLIC UINT32 gpio_detect_earpiece(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) // 5856 only support ep;
    return 1;
#endif
    if (g_gpio_config->earpieceDetectGpio == HAL_GPIO_NONE)
    {
        return g_gpio_config->earpiecePlugin;
    }
    else
    {
        return hal_GpioGet(g_gpio_config->earpieceDetectGpio ) == g_gpio_config->earpiecePlugHigh;
    }
}

#ifdef MCD_TFCARD_SUPPORT
void gpio_CardDetectTimerHandler(VOID *param)
{
    BOOL card_status = mcd_CardDetectUpdateStatus();
    
    if(card_status == g_tflash_detect_flag)
    {
        COS_EVENT ev;
        ev.nEventId = EV_FS_TFLASH_DETECT;
        ev.nParam1  = card_status;
        hal_HstSendEvent(SYS_EVENT, 0x14061556);
        hal_HstSendEvent(SYS_EVENT, ev.nParam1);
        COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        g_timer_id_gpio_card = 0;
    }
    else //status changed, re-detect card
    {
        g_tflash_detect_flag = card_status;
        g_timer_id_gpio_card = COS_SetTimer(GPIO_TCARD_DETECT_INTERVAL, gpio_CardDetectTimerHandler, NULL, COS_TIMER_MODE_SINGLE);
    }
}
#endif

UINT8 gpio_CardDetectPowerOn(VOID)
{
#ifdef MCD_TFCARD_SUPPORT
    #if (APP_SUPPORT_SD_TRACE == 1)
	return 0;
	#endif
    if(g_gpio_config->cardDetectGpio != HAL_GPIO_4_4)
    {
        return 0;
    }
    return (gpio_detect_tcard());
#else
    return 0;
#endif
}

PUBLIC VOID gpio_detect_tcard_conv_to_sd(VOID)
{
#ifdef MCD_TFCARD_SUPPORT
    COS_EVENT ev;
    if(g_gpio_config->cardDetectGpio != HAL_GPIO_4_4)
    {
        return;
    }

    if(g_timer_id_gpio_card)
    {
        COS_KillTimer(g_timer_id_gpio_card);
        g_timer_id_gpio_card = 0;
    }
    ev.nEventId = EV_FS_TFLASH_DETECT;
    ev.nParam1  = 0;
    COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#endif
}

VOID gpio_CardDetectHandler(VOID)
{
#ifdef MCD_TFCARD_SUPPORT
    hal_HstSendEvent(SYS_EVENT, 0x14061555);

    g_tflash_detect_flag = mcd_CardDetectUpdateStatus();

    if(g_timer_id_gpio_card == 0)
    {
        g_timer_id_gpio_card = COS_SetTimer(GPIO_TCARD_DETECT_INTERVAL, gpio_CardDetectTimerHandler, NULL, COS_TIMER_MODE_SINGLE);
    }
    else
    {
        COS_ChangeTimer(g_timer_id_gpio_card, GPIO_TCARD_DETECT_INTERVAL);
    }
#endif
}

/*VOID pm_USBDiskDetect(VOID)
{
    COS_EVENT ev;

    ev.nEventId = EV_FS_USBDISK_DETECT;
    ev.nParam1  = cardPlugged;
    ev.nParam2  = 0;
    ev.nParam3  = 0;
    COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}*/

PUBLIC UINT32 gpio_detect_tcard(VOID)
{
#ifdef MCD_TFCARD_SUPPORT
    if(g_gpio_config->cardDetectGpio == HAL_GPIO_NONE)
        return GPIO_INVALID;
    if(hal_GpioGet(g_gpio_config->cardDetectGpio) == g_gpio_config->gpioCardDetectHigh)
        return GPIO_DETECTED;
#endif
    return GPIO_NOT_DETECTED;
}

PUBLIC VOID gpio_detect_usbdisk(VOID)
{

}

#ifdef UART2_SUPPORT
VOID gpio_Uart2DetectHandler(VOID)
{
    uart2_wakeup();
}

#endif

BOOL gpio_detect_usb(void)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    if(g_gpio_config->gpioUSBDetectEnable)
        return (hal_GpioGet(g_gpio_config->gpioUSBDetect)==g_gpio_config->gpioUSBPlugHigh);
    else
        return (pmd_GetChargerHwStatus() == 2 /*CHR_STATUS_AC_ON*/);
#endif
 return FALSE;
}



