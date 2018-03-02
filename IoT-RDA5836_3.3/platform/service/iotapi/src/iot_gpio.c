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

#include "cs_types.h"
#include "hal_gpio.h"

// ============================================================================
// gpio_Open
// ----------------------------------------------------------------------------
///
///
/// @param port, GPIO port number
/// @param direction, Open GPIO as input or output or both
// ===========================================================================
VOID  gpio_Open(UINT8 port, UINT8 direction)
{
	HAL_GPIO_CFG_T cfg;

	cfg.direction = direction;
	cfg.irqHandler = NULL;
	cfg.irqMask.rising=FALSE;
	cfg.irqMask.falling = FALSE;
	cfg.irqMask.debounce = FALSE;
	cfg.irqMask.level=FALSE;

	hal_GpioOpen((HAL_GPIO_GPIO_ID_T)port, &cfg);
}

// ============================================================================
// gpio_Close
// ----------------------------------------------------------------------------
/// Close the GPIO port
///
/// @param port, GPIO port number
// ===========================================================================
VOID  gpio_Close(UINT8 port)
{
	hal_GpioClose((HAL_GPIO_GPIO_ID_T)port);
}

// ============================================================================
// gpio_Write
// ----------------------------------------------------------------------------
/// Write data to GPIO port
///
/// @param port, GPIO port number
/// @param data, data write to GPIO
// ===========================================================================
VOID gpio_Write(UINT8 port, UINT8 data)
{
	HAL_APO_ID_T gpio;

	gpio.gpioId = (HAL_GPIO_GPIO_ID_T)port;

	#define GPIO_SET_ONE 1
	#define GPIO_SET_ZERO 0

	if (data == GPIO_SET_ONE)
	    hal_GpioSet(gpio);
	else if(data == GPIO_SET_ZERO)
	    hal_GpioClr(gpio);
}

// ============================================================================
// gpio_Read
// ----------------------------------------------------------------------------
/// Read data from GPIO port
///
/// @param ptr Memory pointer 
// ===========================================================================
UINT32 gpio_Read(UINT8 port)
{
	return hal_GpioGet((HAL_GPIO_GPIO_ID_T)port);
}

