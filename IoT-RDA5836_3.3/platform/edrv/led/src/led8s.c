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
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_host.h"
#include "string.h"
#include "cos.h"
#include "event.h"
#include "edrvp_debug.h"
//#include "cfg_regs.h"
//#include "global_macros.h"
#include "sxr_tim.h"
#include "gui_led8s_display.h"

PUBLIC VOID LightOn_Data();

#define HAL_LED_GPIO_1 HAL_GPIO_13
#define HAL_LED_GPIO_2 HAL_GPIO_8
#define HAL_LED_GPIO_3 HAL_GPIO_26
#define HAL_LED_GPIO_4 HAL_GPIO_18
#define HAL_LED_GPIO_5 HAL_GPIO_27
#define HAL_LED_GPIO_6 HAL_GPIO_19
#define HAL_LED_GPIO_7 HAL_GPIO_21

HAL_APO_ID_T Led_Gpio1 = {
							.gpioId= HAL_GPIO_13
						};
HAL_APO_ID_T Led_Gpio2 = {
							.gpioId= HAL_GPIO_8	
						};
HAL_APO_ID_T Led_Gpio3 = {
							.gpioId= HAL_GPIO_26							
						};
HAL_APO_ID_T Led_Gpio4 = {
							.gpioId= HAL_GPIO_18							
						};
HAL_APO_ID_T Led_Gpio5 = {
							.gpioId= HAL_GPIO_27						
						};
HAL_APO_ID_T Led_Gpio6 = {
							.gpioId= HAL_GPIO_19						
						};
HAL_APO_ID_T Led_Gpio7 = {
							.gpioId= HAL_GPIO_21			
						};

UINT8 timer_id = 0;
# if 0
// ============================================================================
// led8s_update_screen
// ----------------------------------------------------------------------------
/// Update buff to led8s screen
/// @param Data Pointer to the screen buffer
/// @return #LCDD_ERR_NO or #LCDD_ERR_DEVICE_NOT_FOUND.
// ============================================================================
PUBLIC VOID led8s_update_screen(UINT16 flag, UINT8* data)
{
	GPIO_RegInit();
	LightOn_Init();
	
	hal_HstSendEvent(SYS_EVENT, 0x20151218);

	code[0] = data[0];
	code[1] = data[1];
	code[2] = data[2];
	code[3] = data[3];

	if(timer_id == 0)
	{
		timer_id = COS_SetTimer(15, LightOn_Data, NULL, COS_TIMER_MODE_PERIODIC);
	}

	return 0;
}

/*
***init GPIO register before light leds;
*/
PUBLIC VOID GPIO_RegInit()
{
	hwp_configRegs->GPIO_Mode = 0xc2c2100|(hwp_configRegs->GPIO_Mode);//set GPIO(8,13,18,19,21,26,27) gpio mode
	hwp_configRegs->IO_Drive1_Select &= ~CFG_REGS_CAM_DRIVE_MASK; //most strong V
	hwp_configRegs->IO_Drive1_Select |= CFG_REGS_CAM_DRIVE_FAST_FAST;
	hwp_configRegs->IO_Drive1_Select &= ~CFG_REGS_GPO_DRIVE_MASK; //most strong V
	hwp_configRegs->IO_Drive1_Select |= CFG_REGS_GPO_DRIVE_FAST_FAST;
	hwp_configRegs->Alt_mux_select |= CFG_REGS_GPIO_CAM3_GPIO ; //Select GPIO for pin: CAM_PCLK CAM_DATA_4 CAM_DATA_5 CAM_DATA_6 CAM_DATA_7.
}

/*
***Init GPIO before light leds;
*/
PUBLIC VOID LightOn_Init()
{
	hal_GpioClose(HAL_LED_GPIO_1);
	hal_GpioClose(HAL_LED_GPIO_3);
	hal_GpioClose(HAL_LED_GPIO_2);
	hal_GpioClose(HAL_LED_GPIO_4);
	hal_GpioClose(HAL_LED_GPIO_5);
	hal_GpioClose(HAL_LED_GPIO_6);
	hal_GpioClose(HAL_LED_GPIO_7);

	hal_GpioClr(Led_Gpio1);
	hal_GpioClr(Led_Gpio2);
	hal_GpioClr(Led_Gpio3);
	hal_GpioClr(Led_Gpio4);
	hal_GpioClr(Led_Gpio5);
	hal_GpioClr(Led_Gpio6);
	hal_GpioClr(Led_Gpio7);
}

/*
***点亮4个7位数码管
*/
PUBLIC VOID LightOn_Data()
{
	LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_4,Led_Gpio3);//5e
	
	for(int i=0;i<=4 ;i++)
	{
		if(i == 0)
		{	
			LightOn_Data1(code[0]);
		}
		else if(i== 1)
		{
			LightOn_Data2(code[1]);
		}
		else if(i== 2)
		{
			LightOn_Data3(code[2]);
		}
		else if(i== 3)
		{
			LightOn_Data4(code[3]);
		}
	}
}

/*
***点亮具体某个灯管
*/
PUBLIC VOID LightOn_LedN(HAL_GPIO_GPIO_ID_T HAL_LED_GPIO_n1,HAL_GPIO_GPIO_ID_T HAL_LED_GPIO_n2,HAL_APO_ID_T Led_Gpion)
{
	hal_GpioSetOut(HAL_LED_GPIO_n1);//打开GPIO
	hal_GpioSetOut(HAL_LED_GPIO_n2);
	hal_GpioSet(Led_Gpion);
	//COS_Sleep(1);
	sxr_Sleep(1*16384/1500);
	hal_GpioClr(Led_Gpion);
	hal_GpioClose(HAL_LED_GPIO_n1);//关闭GPIO
	hal_GpioClose(HAL_LED_GPIO_n2);
}

/*
col: __a_         1
    f|   |b     0   3
     |_g_|        2
    e|   |c     4   5
     |___|        6
       d

***点亮第一个数字
*/
PUBLIC VOID LightOn_Data1(UINT8 led_data)
{
	switch(led_data)
	{
		case 0x7b: // 0 
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			break;
		case 0x28: // 1
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			break;
		case 0x5e: // 2
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			
			break;
		case 0x6e: // 3
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d		
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x2d: // 4
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x67: // 5
			//hal_HstSendEvent(SYS_EVENT, 0x20151220);
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x77: // 6
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x2a: // 7
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x7f: // 8
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;
		case 0x6f: // 9
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_2,Led_Gpio1);//1a
			LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_3,Led_Gpio1);//1b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_1,Led_Gpio4);//1c
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_1,Led_Gpio5);//1d
			//LightOn_LedN(HAL_LED_GPIO_1,HAL_LED_GPIO_4,Led_Gpio1);//1e
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_1,Led_Gpio2);//1f
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_1,Led_Gpio3);//1g
			break;	
		default:
			break;
	}
}

/*
***点亮第二个数字
*/
PUBLIC VOID LightOn_Data2(UINT8 led_data)
{
	switch(led_data)
	{
		case 0x7b: // 0
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x28: // 1
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x5e: // 2
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x6e: // 3
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x2d: // 4
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x67: // 5
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x77: // 6
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x2a: // 7
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x7f: // 8
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		case 0x6f: // 9
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_3,Led_Gpio2);//2a
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_4,Led_Gpio2);//2b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_2,Led_Gpio5);//2c
			LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_6,Led_Gpio2);//2d
			//LightOn_LedN(HAL_LED_GPIO_2,HAL_LED_GPIO_5,Led_Gpio2);//2e
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_2,Led_Gpio3);//2f
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_2,Led_Gpio4);//2g
			break;
		default:
			break;
	}
}

/*
***点亮第三个数字
*/
PUBLIC VOID LightOn_Data3(UINT8 led_data)
{

	switch(led_data)
	{
		case 0x7b: // 0
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x28: // 1
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x5e: // 2
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x6e: // 3
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x2d: // 4
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x67: // 5
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x77: // 6
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			//LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x2a: // 7
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x7f: // 8
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		case 0x6f: // 9
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_4,Led_Gpio5);//3a
			LightOn_LedN(HAL_LED_GPIO_3,HAL_LED_GPIO_5,Led_Gpio3);//3b
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_5,Led_Gpio4);//3c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_1,Led_Gpio6);//3d
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_3,Led_Gpio6);//3e
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_3,Led_Gpio4);//3f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_3,Led_Gpio5);//3g
			break;
		default:
			break;
	}
}


/*
***点亮第四个数字
*/
PUBLIC VOID LightOn_Data4(UINT8 led_data)
{
	switch(led_data)
	{
		case 0x7b: // 0
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x28: // 1
			//LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x5e: // 2
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x6e: // 3
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x2d: // 4
			//LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x67: // 5
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x77: // 6
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x2a: // 7
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			//LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			//LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x7f: // 8
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;
		case 0x6f: // 9
			LightOn_LedN(HAL_LED_GPIO_7,HAL_LED_GPIO_6,Led_Gpio7);//4a
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_7,Led_Gpio6);//4b
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_6,Led_Gpio5);//4c
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_4,Led_Gpio6);//4d
			//LightOn_LedN(HAL_LED_GPIO_4,HAL_LED_GPIO_6,Led_Gpio4);//4e
			LightOn_LedN(HAL_LED_GPIO_6,HAL_LED_GPIO_5,Led_Gpio6);//4f
			LightOn_LedN(HAL_LED_GPIO_5,HAL_LED_GPIO_7,Led_Gpio5);//4g
			break;	
		default:
			break;
	}
}

#else

// second method
HAL_GPIO_GPIO_ID_T HAL_LED_GPIO_n2[7]={HAL_LED_GPIO_1,HAL_LED_GPIO_2,HAL_LED_GPIO_3,HAL_LED_GPIO_4,
										HAL_LED_GPIO_5,HAL_LED_GPIO_6,HAL_LED_GPIO_7};

HAL_APO_ID_T Led_Gpion[7]={{
							.gpioId= HAL_GPIO_13
						},
						{
							.gpioId= HAL_GPIO_8
						},
						{
							.gpioId= HAL_GPIO_26
						},
						{
							.gpioId= HAL_GPIO_18
						},
						{
							.gpioId= HAL_GPIO_27
						},
						{
							.gpioId= HAL_GPIO_19
						},
						{
							.gpioId= HAL_GPIO_21
						}};

UINT8 next_part = 1;
UINT8 seg[7][7] = {{0}};
UINT8 init_flag = 0;

PUBLIC VOID LightOn_Part();
PUBLIC VOID LightOn_Init();
PUBLIC VOID GPIO_RegInit();
PUBLIC VOID Close_AllGpio();
PUBLIC VOID LightOn_PartN();
PUBLIC VOID LightOn_Table(UINT16 flag,UINT8 *code);
PUBLIC VOID LightOn_PartValidLed(HAL_GPIO_GPIO_ID_T GPIO_n2,
								HAL_APO_ID_T Led_Gpion1);
PUBLIC VOID LightOn_Table(UINT16 flag,UINT8 *code);

PUBLIC VOID led8s_update_screen(UINT16 flag, UINT8* data)
{	
	GPIO_RegInit();
	Close_AllGpio();
	hal_GpioSetOut(HAL_LED_GPIO_1);
	hal_GpioClr(Led_Gpio1);
	hal_GpioSetOut(HAL_LED_GPIO_2);
	hal_GpioSet(Led_Gpio2);
	
	//hal_GpioClose(HAL_LED_GPIO_3);
	//hal_GpioClose(HAL_LED_GPIO_4);
	return;
	
	if(init_flag == 0)
	{
		GPIO_RegInit();
		LightOn_Init();
		init_flag = 1;
	}
	
	hal_HstSendEvent(SYS_EVENT, 0x20151218);

	LightOn_Table(flag,data);
	
	if(timer_id == 0)
	{
		//timer_id = COS_SetTimer(1, LightOn_Part, NULL, COS_TIMER_MODE_PERIODIC);
		sxr_StartFunctionTimer(1,LightOn_Part, 0,0);
		timer_id = 1;
	}

	return;
}


PUBLIC VOID LightOn_Part()
{
	switch(next_part)
	{
		case 1:
			LightOn_PartN(); //part1
			next_part++;
		break;
		case 2:
			LightOn_PartN(); //part2
			next_part++;
		break;
		case 3:
			LightOn_PartN(); //part3
			next_part++;
		break;
		case 4:
			LightOn_PartN(); //part4
			next_part++;
		break;
		case 5:
			LightOn_PartN(); //part5
			next_part++;
		break;
		case 6:
			LightOn_PartN(); //part6
			next_part++;
		break;
		case 7:
			LightOn_PartN(); //part7
			next_part = 1;
		break;
		default:
			
			break;
	}
	sxr_StartFunctionTimer(1, LightOn_Part, 0,0);
}

PUBLIC VOID LightOn_PartN()
{
	//UINT32 csStatus = hal_SysEnterCriticalSection();
	
	Close_AllGpio();
	hal_GpioSetOut(HAL_LED_GPIO_n2[next_part-1]);
	hal_GpioClr(Led_Gpion[next_part-1]);
	
	for(int i=0;i<7;i++)
	{
		if(seg[next_part-1][i]==1)
		{
			LightOn_PartValidLed(HAL_LED_GPIO_n2[i],Led_Gpion[i]);
		}
	}
	//hal_SysExitCriticalSection(csStatus);
}

PUBLIC VOID LightOn_PartValidLed(HAL_GPIO_GPIO_ID_T GPIO_n2,
								HAL_APO_ID_T Led_Gpion1)
{
	//open GPIO
	hal_GpioSet(Led_Gpion1);
	hal_GpioSetOut(GPIO_n2);
}
PUBLIC VOID Close_AllGpio() 
{
	//close GPIO
	hal_GpioClose(HAL_LED_GPIO_1);
	hal_GpioClose(HAL_LED_GPIO_2);
	hal_GpioClose(HAL_LED_GPIO_3);
	hal_GpioClose(HAL_LED_GPIO_4);
	hal_GpioClose(HAL_LED_GPIO_5);
	hal_GpioClose(HAL_LED_GPIO_6);
	hal_GpioClose(HAL_LED_GPIO_7);
}
PUBLIC VOID Clear_AllGpio()
{
	//clear GPIO
	hal_GpioClr(Led_Gpio1);
	hal_GpioClr(Led_Gpio2);
	hal_GpioClr(Led_Gpio3);
	hal_GpioClr(Led_Gpio4);
	hal_GpioClr(Led_Gpio5);
	hal_GpioClr(Led_Gpio6);
	hal_GpioClr(Led_Gpio7);
}


/*
***config GPIO register before light leds;
*/
PUBLIC VOID GPIO_RegInit()
{
#if 0
	hwp_configRegs->GPIO_Mode = 0xc2c2100|(hwp_configRegs->GPIO_Mode);//set GPIO(8,13,18,19,21,26,27) gpio mode
	
	hwp_configRegs->IO_Drive1_Select &= ~CFG_REGS_CAM_DRIVE_MASK; //most strong V_CAM Pad drive select
	hwp_configRegs->IO_Drive1_Select |= CFG_REGS_CAM_DRIVE_FAST_FAST;
	
	hwp_configRegs->IO_Drive1_Select &= ~CFG_REGS_GPO_DRIVE_MASK; //most strong V_STD Pad drive select
	hwp_configRegs->IO_Drive1_Select |= CFG_REGS_GPO_DRIVE_FAST_FAST;
	
	hwp_configRegs->Alt_mux_select |= CFG_REGS_GPIO_CAM3_GPIO ; //Select GPIO for pin: CAM_PCLK CAM_DATA_4 CAM_DATA_5 CAM_DATA_6 CAM_DATA_7.

	hwp_configRegs->IO_Drive1_Select &= ~CFG_REGS_KEYPAD_DRIVE_MASK;//most strong V_STD Pad drive select
	hwp_configRegs->IO_Drive1_Select |= CFG_REGS_KEYPAD_DRIVE_FAST_FAST;
#endif
}

/*
***Init GPIO before light leds;
*/
PUBLIC VOID LightOn_Init()
{
	hal_GpioClose(HAL_LED_GPIO_1);
	hal_GpioClose(HAL_LED_GPIO_3);
	hal_GpioClose(HAL_LED_GPIO_2);
	hal_GpioClose(HAL_LED_GPIO_4);
	hal_GpioClose(HAL_LED_GPIO_5);
	hal_GpioClose(HAL_LED_GPIO_6);
	hal_GpioClose(HAL_LED_GPIO_7);
}

PUBLIC VOID LightOn_Table(UINT16 flag,UINT8 *code)
{
	memset(seg,0,sizeof(seg));
	
	if(flag&GUI_ICON_SD)
	{
		seg[5][2] = 1;
		seg[3][2] = 1;
	}
	if(flag&GUI_ICON_USB)
	{
		seg[5][0] = 1;
		seg[3][2] = 1;
	}
	if(flag&GUI_ICON_FM)
	{
		seg[1][5] = 1;
		seg[4][6] = 1;
	}
	if(flag&GUI_ICON_COLON)
	{
		seg[4][0] = 1;
	}
	else if(flag&GUI_ICON_DECPOINT)
	{
		seg[6][2] = 1;
	}
	
	switch(code[0])
	{
		case 0x7b: // 0 
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			break;
		case 0x28: // 1
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			break;
		case 0x5e: // 2
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			//seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			seg[3][0] = 1;//1e
			//seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			
			break;
		case 0x6e: // 3
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			//seg[3][0] = 1;//1e
			//seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;
		case 0x2d: // 4
			//seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			//seg[0][4] = 1;//1d
			//seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;
		case 0x67: // 5
			seg[1][0] = 1;//1a
			//seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			//seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;
		case 0x77: // 6
			seg[1][0] = 1;//1a
			//seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;
		case 0x2a: // 7
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			//seg[0][4] = 1;//1d
			//seg[3][0] = 1;//1e
			//seg[0][1] = 1;//1f
			//seg[0][2] = 1;//1g
			break;
		case 0x7f: // 8
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;
		case 0x6f: // 9
			seg[1][0] = 1;//1a
			seg[2][0] = 1;//1b
			seg[0][3] = 1;//1c
			seg[0][4] = 1;//1d
			//seg[3][0] = 1;//1e
			seg[0][1] = 1;//1f
			seg[0][2] = 1;//1g
			break;	
		default:
			break;
	}

	switch(code[1])
	{
		case 0x7b: // 0
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			//seg[1][3] = 1;//2g
			break;
		case 0x28: // 1
			//seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			//seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			//seg[1][2] = 1;//2f
			//seg[1][3] = 1;//2g
			break;
		case 0x5e: // 2
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			//seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			seg[4][1] = 1;//2e
			//seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x6e: // 3
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			//seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x2d: // 4
			//seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			//seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x67: // 5
			seg[2][1] = 1;//2a
			//seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x77: // 6
			seg[2][1] = 1;//2a
			//seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x2a: // 7
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			//seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			//seg[1][2] = 1;//2f
			//seg[1][3] = 1;//2g
			break;
		case 0x7f: // 8
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		case 0x6f: // 9
			seg[2][1] = 1;//2a
			seg[3][1] = 1;//2b
			seg[1][4] = 1;//2c
			seg[5][1] = 1;//2d
			//seg[4][1] = 1;//2e
			seg[1][2] = 1;//2f
			seg[1][3] = 1;//2g
			break;
		default:
			break;
	}

	switch(code[2])
	{
		case 0x7b: // 0
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			//seg[2][4] = 1;//3g
			break;
		case 0x28: // 1
			//seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			//seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			//seg[2][3] = 1;//3f
			//seg[2][4] = 1;//3g
			break;
		case 0x5e: // 2
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			//seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			seg[2][5] = 1;//3e
			//seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x6e: // 3
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			//seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x2d: // 4
			//seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			//seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x67: // 5
			seg[3][4] = 1;//3a
			//seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x77: // 6
			seg[3][4] = 1;//3a
			//seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x2a: // 7
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			//seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			//seg[2][3] = 1;//3f
			//seg[2][4] = 1;//3g
			break;
		case 0x7f: // 8
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		case 0x6f: // 9
			seg[3][4] = 1;//3a
			seg[4][2] = 1;//3b
			seg[4][3] = 1;//3c
			seg[0][5] = 1;//3d
			//seg[2][5] = 1;//3e
			seg[2][3] = 1;//3f
			seg[2][4] = 1;//3g
			break;
		default:
			break;
	}

	switch(code[3])
	{
		case 0x7b: // 0
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			//seg[6][4] = 1;//4g
			break;
		case 0x28: // 1
			//seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			//seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			//seg[4][5] = 1;//4f
			//seg[6][4] = 1;//4g
			break;
		case 0x5e: // 2
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			//seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			seg[5][3] = 1;//4e
			//seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x6e: // 3
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			//seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x2d: // 4
			//seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			//seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x67: // 5
			seg[5][6] = 1;//4a
			//seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x77: // 6
			seg[5][6] = 1;//4a
			//seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x2a: // 7
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			//seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			//seg[4][5] = 1;//4f
			//seg[6][4] = 1;//4g
			break;
		case 0x7f: // 8
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;
		case 0x6f: // 9
			seg[5][6] = 1;//4a
			seg[6][5] = 1;//4b
			seg[5][4] = 1;//4c
			seg[3][5] = 1;//4d
			//seg[5][3] = 1;//4e
			seg[4][5] = 1;//4f
			seg[6][4] = 1;//4g
			break;	
		default:
			break;
	}
}
#endif
