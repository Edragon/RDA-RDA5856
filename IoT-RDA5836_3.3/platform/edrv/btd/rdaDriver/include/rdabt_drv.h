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


#ifndef  __RDA_BT_H__
#define __RDA_BT_H__

#include "cs_types.h"
#include "cos.h"
#include "edrvp_debug.h"

#ifndef APP_SUPPORT_SOUNDBAR
#define APP_SUPPORT_SOUNDBAR 0
#endif

#ifndef BLUETOOTH_USE_MARLIN
#define BLUETOOTH_USE_MARLIN 0
#endif

#ifndef APP_SUPPORT_HFP_AG
#define APP_SUPPORT_HFP_AG 0
#endif

#define RDABT_DELAY(DURATION) COS_Sleep(DURATION*5)

#define BT_FUNC_INTERNAL    __attribute__((section(".btsramtext")))
#define BT_DATA_INTERNAL    __attribute__((section(".btsramdata")))

#define BT_UART_BAUDRATE        921600/*460800*//*230400*/

//extern kal_uint8 bt_sleephdl;

typedef enum{
	RDA_NONE_CHIP_ENUM = 0,
	RDA_BT_R5_5868_ENUM,
	RDA_BT_R7_5868PLUS_ENUM,
	RDA_BT_R8_5870_ENUM,
	RDA_BT_R10_5868E_ENUM,
	RDA_BT_R10_5872_ENUM,
	RDA_BT_R10_AL_5868H_ENUM,
	RDA_BT_R10_AL_5872H_ENUM,
	RDA_BT_R10_5870E_ENUM,
	RDA_BT_R11_5872P_ENUM,
	RDA_BT_R11_5875_ENUM,
	RDA_BT_R12_5876_ENUM,
	RDA_BT_R12_5870P_ENUM,
 	RDA_BT_R12_5876M_ENUM,
 	RDA_BT_R16_5876P_ENUM,	
	RDA_BT_R12_5990_ENUM,
	RDA_BT_R17_8809_ENUM,
	RDA_BT_R18_8809E_ENUM,
	RDA_BT_R19_8809P_ENUM,
	RDA_BT_R20_5855_ENUM
}RDABT_CHIP_ENUM;

//#define __RDABT_DISABLE_EDR__

#define __RDABT_ENABLE_SP__

#define __BT_PCM_OVER_UART__

#define  __RDABT_USE_IIC__


//#define __RDA_BT_5868_SERIES__
//#define __RDA_BT_587x_SERIES__
//#define __RDA_BT_5875_SERIES__

#ifdef __RDA_BT_5868_SERIES__
//#define __RDA_CHIP_R5_5868__
//#define __RDA_CHIP_R7_5868plus__
//#define __RDA_CHIP_R10_5868E__
//#define __RDA_CHIP_R10_5868H__
#endif

#ifdef __RDA_BT_587x_SERIES__
//#define __RDA_CHIP_R8_5870__
//#define __RDA_CHIP_R10_5872__
//#define __RDA_CHIP_R10_5872H__
//#define __RDA_CHIP_R10_5870E__
//#define __RDA_CHIP_R11_5872P__
#endif

#ifdef __RDA_BT_5875_SERIES__
//#define __RDA_CHIP_R11_5875__
#endif


#ifdef __RDA_BT_587x_SERIES__
//#define __587x_USE_DCDC__
#endif


#ifdef __RDA_BT_5875_SERIES__
//#define __5875_USE_DCDC__
#endif

//如果定义了r5，r7，r10-5868e 中的任意2种或者3种，打开此宏，否则请关闭
//make sure_RDA5868_USE_IIC__  is defined
#ifdef __RDA_BT_5868_SERIES__
#define __RDA_AUTO_CHIP_5868__
#endif

#if defined( __RDA_BT_587x_SERIES__)
#define __RDA_AUTO_CHIP_587x__
#endif

#if defined(__RDA_BT_5875_SERIES__)
#define __RDA_AUTO_CHIP_5875__
#endif


#if !defined(__RDA_BT_5868_SERIES__) && !defined(__RDA_BT_587x_SERIES__) && !defined(__RDA_BT_5875_SERIES__) && !defined(__RDA_BT_5876M_SERIES__)
//#error "please define at least one chip"
#endif

#if defined( __RDABT_USE_IIC__) && (defined(__RDA_CHIP_R5_5868__)||defined(__RDA_CHIP_R7_5868plus__))
#define __RDA_SOFT_RESET__
#endif


#if defined ( __RDA_CHIP_R7_5868plus__) 
#define __RDA_SLEEP_ROM__
#endif




void GPO_ModeSetup(uint16 pin, uint16 conf_dada);
void GPIO_ModeSetup(uint16 pin, uint16 conf_dada);
void GPIO_WriteIO(char data, char port);
void GPIO_InitIO(char direction, char port);
void BtRadio_SetupGPIO(char val, unsigned char pin);

void RDA_bt_Power_On_Reset(void);
void rda_bt_pin_to_low(void);
void rda_bt_ldoon_toggle_high(void);
void rda_bt_reset_to_high(void);

//void rdabt_uart_start(void);
//void rdabt_uart_configure(unsigned baud);
//void rdabt_uart_stop(void);
//unsigned char rdabt_uart_tx(kal_uint8 *buf, unsigned short num_to_send, unsigned short *num_send);
#define rdabt_uart_stop rda5868_uart_stop
#define rdabt_uart_start rda5868_uart_start
#define rdabt_uart_configure rda5868_uart_configure
#define rdabt_uart_rx rdabt_adp_uart_rx
#define rdabt_uart_tx rdabt_adp_uart_tx

void rdabt_wirte_memory(unsigned int addr,const unsigned int *data,unsigned char len,unsigned char memory_type);

void rdabt_iic_rf_write_data(unsigned char regaddr, const unsigned short *data, unsigned char datalen);
void rdabt_iic_rf_read_data(unsigned char regaddr, unsigned short *data, unsigned char datalen);
UINT8 rdabt_iic_core_read_data(unsigned int regaddr, unsigned int *data, unsigned char datalen);
UINT8 rdabt_iic_core_write_data(unsigned int regaddr, const unsigned int *data, unsigned char datalen);

void rdawifi_iic_rf_write_data(unsigned char regaddr, const unsigned short *data, unsigned char datalen);
void rdawifi_iic_rf_read_data(unsigned char regaddr, unsigned short *data, unsigned char datalen);

void rda_bt_power_on(void);
void rda_bt_rf_init(void);
void rda_bt_uart_init(void);



#ifdef __RDA_CHIP_R5_5868__
void RDABT_rf_Intialization_r5(void);
void RDABT_core_Intialization_r5(void);
void	Rdabt_Pskey_Write_rf_r5(void);
#endif

#ifdef __RDA_CHIP_R7_5868plus__
void RDABT_rf_Intialization_r7(void);
void RDABT_core_Intialization_r7(void);
void Rdabt_Pskey_Write_rf_r7(void);
#endif

#ifdef __RDA_CHIP_R8_5870__
void RDABT_rf_Intialization_r8(void);
void RDABT_core_Intialization_r8(void);
void Rdabt_Pskey_Write_rf_r8(void);
#endif


#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__) || defined(__RDA_CHIP_R10_5872H__) || defined(__RDA_CHIP_R10_5868H__) || defined(__RDA_CHIP_R10_5870E__)
void Rdabt_Pskey_Write_r10(void);
void Rdabt_trap_write_r10(void);
void Rdabt_patch_write_r10(void);
void Rdabt_acl_patch_write_r10(void);
void Rdabt_unsniff_prerxon_write_r10(void);
void Rdabt_setfilter_r10(void);

#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__)
void RDABT_rf_Intialization_r10(void);
void RDABT_core_Intialization_r10(void);
void  Rdabt_core_uart_Intialization_r10(void);
void Rdabt_Pskey_Write_rf_r10(void);
#endif

#if defined( __RDA_CHIP_R10_5872H__) || defined(__RDA_CHIP_R10_5868H__)
void RDABT_rf_Intialization_r10_al(void);
void RDABT_core_Intialization_r10_al(void);
void Rdabt_Pskey_Write_rf_r10_al(void);
void rdabt_DC_write_r10_al(void);
#endif

#if defined(__RDA_CHIP_R10_5870E__)
void RDABT_core_Intialization_r10_e(void);
void RDABT_rf_Intialization_r10_e(void);
void Rdabt_Pskey_Write_rf_r10_e(void);
void rdabt_DC_write_r10_e(void);

#endif


#endif


#if defined(__RDA_CHIP_R11_5872P__) || defined(__RDA_CHIP_R11_5875__)
void Rdabt_setfilter_r11(void);
void Rdabt_unsniff_prerxon_write_r11(void);
void Rdabt_trap_write_r11(void);
void Rdabt_Pskey_Write_r11(void);
void Rdabt_patch_write_r11(void);

#if defined(__RDA_CHIP_R11_5872P__)
void RDABT_core_Intialization_r11_p(void);
void RDABT_rf_Intialization_r11_p(void);
void Rdabt_Pskey_Write_rf_r11_p(void);
void rdabt_DC_write_r11_p(void);
#endif

#if defined(__RDA_CHIP_R11_5875__)
void RDABT_core_Intialization_r11(void);
void RDABT_rf_Intialization_r11(void);
void Rdabt_Pskey_Write_rf_r11(void);
void rdabt_DC_write_r11(void);
void RDABT_phone_Intialization_r11(void);
#endif

#endif


#if defined(__RDA_CHIP_R12_5876__) || defined(__RDA_CHIP_R12_5990__)
void Rdabt_setfilter_r12(void);
void Rdabt_unsniff_prerxon_write_r12(void);
void Rdabt_trap_write_r12(void);
void Rdabt_Pskey_Write_r12(void);
void Rdabt_patch_write_r12(void);

void RDABT_core_Intialization_r12(void);
void RDABT_rf_Intialization_r12(void);
void Rdabt_Pskey_Write_rf_r12(void);
void rdabt_DC_write_r12(void);
void RDABT_phone_Intialization_r12(void);
void RDA_5990_power_on(void);
void RDA_5990_power_off(void);
void RDA_5990_clock_en(void);
#endif

#ifdef __RDA_CHIP_R16_5876P__
void RDABT_5876p_bton_fmoff_patch(void);
void RDABT_5876p_bton_fmon_patch(void);
void RDABT_5876p_btoff_fmon_patch(void);
void RDABT_5876p_btoff_fmoff_patch(void);
void RDABT_phone_Intialization_r16(void);
void Rdabt_Pskey_Write_rf_r16(void);
void Rdabt_Pskey_Write_r16(void);
void rdabt_set_filter_r16(void);
void Rdabt_trap_write_r16(void);
void RDABT_rf_Intialization_r16(void);
void rdabt_DC_write_r16(void);
void RDABT_core_Intialization_r16(void);
#endif
#ifdef __RDA_CHIP_R17_8809__
void RDABT_phone_Intialization_r17(void);
void RDABT_rf_Intialization_r17(void);
void Rdabt_Pskey_Write_rf_r17(void);
void rdabt_DC_write_r17(void);
void Rdabt_Pskey_Write_r17(void);
void Rdabt_trap_write_r17(void);
void RDABT_core_Intialization_r17(void);
#endif

#if defined( __RDA_AUTO_CHIP_5868__) || defined(__RDA_AUTO_CHIP_587x__) || defined(__RDA_AUTO_CHIP_5875__)
uint8 rdabt_get_chipid(void);
#endif

#endif

