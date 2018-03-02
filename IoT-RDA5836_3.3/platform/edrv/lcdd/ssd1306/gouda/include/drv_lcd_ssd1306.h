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

/*
 * drv_lcd_ssd1306.h
 *
 *  Created on: 2016Äê9ÔÂ8ÈÕ
 *      Author: yongminhu
 */

#include "hal_spi.h"
#include "hal_gpio.h"
#include "lcdd_types.h"
#include "hal_timers.h"
//#include "ap_common.h"

#ifndef PLATFORM_EDRV_LCDD_SSD1306_INCLUDE_DRV_LCD_SSD1306_H_
#define PLATFORM_EDRV_LCDD_SSD1306_INCLUDE_DRV_LCD_SSD1306_H_


#define SPI_4_WIRE_MODE
//#define SPI_3_WIRE_MODE

typedef struct _LCD_GPIO_CONFIG_STRUCT_T
{
	//gpio  for SCLK
	HAL_APO_ID_T gpio_sclk;

	//gpio  for SDO
	HAL_APO_ID_T gpio_sdo;

	//gpio  for RES#
	HAL_APO_ID_T gpio_res;

	//gpio  for D/C#
	HAL_APO_ID_T gpio_dc;

	//gpio  for CS#
	HAL_APO_ID_T gpio_cs;
} LCD_GPIO_CONFIG_STRUCT_T;

#define LCD_SSD1306_GPIOPIN_SCLK		HAL_GPIO_1_0
#define LCD_SSD1306_GPIOPIN_SDO		HAL_GPIO_1_3
#define LCD_SSD1306_GPIOPIN_RES		HAL_GPIO_3_0
#define LCD_SSD1306_GPIOPIN_DC		HAL_GPIO_3_1
#define LCD_SSD1306_GPIOPIN_CS		HAL_GPIO_1_1

PUBLIC LCDD_ERR_T lcdd_Open(VOID);
PUBLIC LCDD_ERR_T lcdd_Init(VOID);
PUBLIC LCDD_ERR_T lcdd_update_screen(UINT8* Data, LCDD_ROI_T *rect);

#endif /* PLATFORM_EDRV_LCDD_SSD1306_INCLUDE_DRV_LCD_SSD1306_H_ */
