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
//                                                                            //
/// @file  
/// That file describes the configuration of the structure used to set up
///  
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

#include "cs_types.h"

#include "hal_gpio.h"



// =============================================================================
//  
// -----------------------------------------------------------------------------
///
/// Make sure to include gpioic_config.h before  
// =============================================================================
#define HAL_GPIO_MAX_LED                8
#define HAL_GPIO_MAX_GPIOKEYS           8
#define NONEED                          0x55
// =============================================================================
//  
// -----------------------------------------------------------------------------
///  
// ============================================================================

///@todo
/// - add: ADMUX (check only, in target only ?)
/// - change pwlGlowPosition: new enum (add in hal_pwl): invalid, none, pwl0, pwl1
/// .
//update by wangxin gpio
 typedef struct
{
    HAL_GPIO_GPIO_ID_T          id;
    HAL_GPIO_DIRECTION_T        direction;
    HAL_GPIO_ALTFUNC_CFG_ID_T   altcfg;
    HAL_GPIO_UP_DOWN_ID_T       updown;
    UINT8                       drv;
    UINT8                       enint;       /* enable interrupt or not */
    UINT8                       gpio_intnum; /* gpio interrupt num */
    HAL_GPIO_IRQ_MASK_T         irqMask;
    HAL_GPIO_IRQ_HANDLER_T      irqHandler; 
}BoardSetup_GPIO_CFG;



typedef struct GPIO_CONFIG_STRUCT_T
{
    // gpio for irc
    HAL_GPIO_GPIO_ID_T gpio_irc;
    // gpio for NFC
    HAL_GPIO_GPIO_ID_T gpio_NFC;
    // leds config
    UINT8 led_count;
    /// when set to TRUE, the GPIO value 1 is for open the led light
    BOOL ledHighLight;
    #if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856)
    HAL_APO_ID_T leds[HAL_GPIO_MAX_LED]; // PMD_LEVEL_ID_T
    #else
    UINT8 leds[HAL_GPIO_MAX_LED];
    #endif

    // gpio key config
    UINT8 key_count;
    BOOL  key_interrupt_mode; // true if use interrupt mode, else use scan mode
    BOOL  high_keydown; // true if high for key down
    HAL_GPIO_GPIO_ID_T gpio_keys[HAL_GPIO_MAX_GPIOKEYS];
    UINT8 gpio_keyirqnum[HAL_GPIO_MAX_GPIOKEYS];
    HAL_GPIO_GPIO_ID_T gpio_lineIn;
    /// when set to TRUE, the GPIO value 1 is deal as linein plugin
    BOOL lineinPlugHigh;

    /// Gpio Connected to socket to detect card insertion/removal .
    /// (set to #HAL_GPIO_NONE) if not available.
    HAL_GPIO_GPIO_ID_T  cardDetectGpio;
    /// Define the polarity of the above GPIO: 
    /// \c TRUE GPIO is high when card is present,
    /// \c FALSE GPIO is low when card is present.
    BOOL                gpioCardDetectHigh;
    UINT8      cardDetectGpioIntNum;
    /// The GPIO pin to detect earpiece
    HAL_GPIO_GPIO_ID_T earpieceDetectGpio;
    /// when set to TRUE, the GPIO value 1 is deal as earpice plugin
    BOOL earpiecePlugHigh;
    /// If no GPIO check, earpiece plugin or not
    BOOL earpiecePlugin;

    // the GPIO pin to mute
    HAL_APO_ID_T gpioMute;
    // enable gpio mute function
    BOOL gpioMuteEnable;
    // when set to TRUE, set the GPIO value 1 to mute
    BOOL gpioMuteHigh;

    // the GPIO for wake up uart2
    HAL_GPIO_GPIO_ID_T uart2_wake;

    /// The GPIO pin to detect USB
    HAL_GPIO_GPIO_ID_T gpioUSBDetect;
    /// when set to TRUE, the GPIO value 1 is deal as usb plugin
    BOOL gpioUSBPlugHigh;
    /// If detect usb by GPIO or not
    BOOL gpioUSBDetectEnable;
}GPIO_CFG_CONFIG_T;


PUBLIC CONST GPIO_CFG_CONFIG_T* tgt_GetGPIOCfg(VOID);

#endif // 


