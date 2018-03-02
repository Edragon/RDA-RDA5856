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

#ifndef _GPIO_EDRV_H_
#define _GPIO_EDRV_H_

enum {
    GPIO_INVALID            = -1,
    GPIO_NOT_DETECTED       = 0,
    GPIO_DETECTED           = 1,
};


PUBLIC UINT32 gpio_initialise(VOID);

PUBLIC UINT32 gpio_detect_linein(VOID);
PUBLIC UINT32 gpio_detect_earpiece(VOID);
PUBLIC UINT32 gpio_detect_tcard(VOID);
PUBLIC VOID gpio_detect_usbdisk(VOID);

PUBLIC VOID gpio_SetLED(int led_mask);

#endif // #ifndef _GPIO_EDRV_H_


