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
 * MODULE NAME:    rda5875_init.c
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    rda5875 initiation.
 * MAINTAINER:     mql
 * CREATION DATE:  05/01/10
 *
 * SOURCE CONTROL: $Id: rda5875_init.c,v3.48 2012/01/04 15:50:38 mql Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2010-2011 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   
 *   version 3.48
 *				 
 *************************************************************************/

#ifdef __BT_RDABT__
#include "stdio.h"                  /* Basic C I/O functions */

#include "tgt_app_cfg.h"
#include "rdabt_drv.h"


#ifdef __RDA_CHIP_R21_5856__

#define RDA_READ_UINT32( _register_ ) \
        (*((volatile unsigned int *)(_register_)))

#define RDA_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))

#define rdabt_write_pskey(id,data,length) SYSHal_Config_Apply(id,length,data)
extern void SYSHal_Config_Apply(UINT8 id, UINT8 length, const UINT8 *data);
extern void HWdelay_Wait_For_us(UINT32 units_of_u_secs);
extern void HWdelay_Wait_For_ms(UINT32 units_of_m_secs, UINT32 use_native_clk);

const UINT32 btcore_rf_init_21_3[][2] = 
{
//itm:5856 U03 BT_initial_4dbm_48pin
{0Xa16082fc,0X00000000},//page 0
{0Xa1608214,0X000002f6},//05
{0Xa1608218,0X00005314},//06
#if defined(__CT_CHIP_PKG_RDA5856Q32__)
{0Xa160821c,0X0000070d},//07
#elif defined(__CT_CHIP_PKG_RDA5856L__)||defined(__CT_CHIP_PKG_RDA5856LE__)
{0Xa160821c,0X0000048d},//07   //24   0X0000048d  -> 0X000002ad
#else
{0Xa160821c,0X000002ad},//07   //24   0X0000048d  -> 0X000002ad
#endif
#ifdef Low_Power_Consumption
{0Xa1608224,0X0000068e},//09
{0Xa160822c,0X0000231f},//0B
{0Xa1608278,0X00003080},//1e
{0Xa1608248,0X00005618},//12  //
{0Xa1608250,0X00000e8e},//14  //
{0Xa1608258,0X0000b518},//16
{0Xa1608274,0X00004334},//1d
{0Xa1608290,0X00009999},//24  //0X0000bbbb -> 0X00009999
{0Xa16082a0,0X00006666},//28  //0X00007777 -> 0X00006666
{0Xa16082ac,0X000000b8},//2b
{0Xa16082c8,0X00001f00},//32
{0Xa16082d4,0X0000001e},//35
{0Xa16082fc,0X00000001},//page 1
{0Xa1608228,0X00000008},//8a
{0Xa160827c,0X0000f3f3},//9f  //

//item:5856 U03 BT_APC
{0Xa16082fc,0X00000000},//page 0
{0Xa16082fc,0X00000001},//page 1
{0Xa1608268,0X0000efef},//9a   
{0Xa160826c,0X0000efef},//9b
{0Xa1608270,0X0000efef},//9c
{0Xa1608274,0X0000efef},//9d
{0Xa1608278,0X0000f2f2},//9e
{0Xa160827c,0X0000f2f2},//9f
#else
{0Xa160822c,0X0000231f},//0B
{0Xa1608278,0X00003080},//1e
{0Xa1608248,0X00005610},//12  //
{0Xa1608250,0X00000e8e},//14  //
{0Xa1608258,0X0000b518},//16
{0Xa1608274,0X00004334},//1d
{0Xa1608290,0X00009999},//24  //0X0000bbbb -> 0X00009999
{0Xa16082a0,0X00006666},//28  //0X00007777 -> 0X00006666
{0Xa16082ac,0X000000b8},//2b
{0Xa16082c8,0X00001f00},//32
{0Xa16082d4,0X0000001e},//35
{0Xa16082fc,0X00000001},//page 1
{0Xa1608228,0X0000000f},//8a
{0Xa160827c,0X0000f3f3},//9f  //

//item:5856 U03 BT_APC
{0Xa16082fc,0X00000000},//page 0
{0Xa1608290,0X0000569c},//24    //0X0000569c->0x0000697b power control ok
{0Xa16082a0,0X00004579},//28  //0X00004579->0x00003547
{0Xa16082fc,0X00000001},//page 1
{0Xa1608268,0X0000efef},//9a   
{0Xa160826c,0X0000efef},//9b
{0Xa1608270,0X0000efef},//9c
{0Xa1608274,0X0000efef},//9d
{0Xa1608278,0X0000f3f3},//9e
{0Xa160827c,0X0000f3f3},//9f
#endif
#if defined(__CT_CHIP_PKG_RDA5856Q32__)
//item:5856 BT_AGC       //
{0Xa16082fc,0X00000001},//page 1
{0Xa1608200,0X0000020f},//80
{0Xa1608204,0X0000f9cf},//81
{0Xa1608208,0X0000fc2f},//82
{0Xa160820c,0X0000f92f},//83
{0Xa1608210,0X0000fa2f},//84
{0Xa1608214,0X0000fc2f},//85
{0Xa1608218,0X0000fb3f},//86
{0Xa160821c,0X00007fff},//87
#endif//	

{0xa16082fc,0x00000000},//3f,page0
{0xa16082c0,0x00000129},//30
{0xa16082c0,0x0000012b},//30

//modem

{0xa16082fc,0x00000001},//page 1
{0xa160831c,0x0000dc3a},////c7
{0xa16082fc,0x00000000},//page 0
{0xa1608348,0x00000401},//52
{0xa16083ac,0x00001100},//6b
{0xa160830c,0x0000FF49},//43
{0xa1608310,0x0000a088},//44
{0xa160833c,0x00004220},//4f
{0xa160831c,0x000055c7},//47
{0xa1608320,0x00003057},//48
//{0xa1608330,0x0000400a},//4C
{0xa1608304,0x0000400b},
{0xa1608308,0x00002D71}//42



};

const UINT32 btcore_rf_init_21[][2] = 
{

{0Xa16082fc,0X00000000},  
{0Xa1608208,0X0000fe00},//02
{0Xa1608218,0X00005314},//06
{0Xa160821c,0X0000052D},//07  0X0000060D 
{0Xa1608220,0X00009007},//08
{0Xa1608224,0X000006a6},//09
{0Xa160822c,0X0000231f},//0b
{0Xa1608238,0X00000920},//0e
{0Xa160823c,0X00009C03},//{0Xa160823c,0X00008cf1},//0f

{0Xa1608248,0X00007d07},//12
{0Xa1608250,0X00000888},//14
{0Xa1608254,0X000053b4},//15
{0Xa1608258,0X00000518},//{0Xa1608258,0X00003b18},//16
{0Xa160825c,0X00002200},//17
{0Xa1608260,0X00001e12},//{0Xa1608260,0X00009f12},//18
{0Xa1608264,0X000010c8},//19


{0Xa1608274,0X0000a65f},//1d 0X0000db1d 
{0Xa1608278,0X00003080},//1e 0X0000308f 
{0Xa16082ac,0X000000b8},//2b 
{0xa16082c0,0x0000012b},//30
{0xa16082c8,0x00000f00},//32
{0xa16082d4,0x0000000f},//35
{0Xa16082fc,0X00000001},//3f,page1
{0Xa1608200,0X000001af},//80
{0Xa1608204,0X000001bf},//81
{0Xa1608208,0X00000a3f},//82
{0Xa160820c,0X0000323f},//83
{0Xa1608210,0X00003a3f},//84
{0Xa1608214,0X00005bff},//85
{0Xa1608218,0X00003fff},//86
{0Xa160821c,0X00007fff},//87
{0Xa1608228,0X0000000f},//8a
{0xa16082fc,0x00000000},//3f,page0
#if 0
{0xa1608304,0x0000000b},//41
{0xa1608308,0x000025bd},//42
{0xa160830c,0x00008fc6},//43
{0xa1608310,0x0000056f},//44
{0xa1608324,0x000008e4},//49
{0xa16083a4,0x00004075},//69
{0xa16083ac,0x000010c0},//6b

{0xa16082fc,0x00000001},//3f,page1
{0xa1608268,0x0000f0f0},//9a
{0xa160826c,0x0000f0f0},//9b
{0xa1608270,0x0000f0f0},//9c
{0xa1608274,0x0000f8f8},//9d
{0xa1608278,0x0000fdfd},//9e
{0xa160827c,0x0000fdfd},//9f
{0xa1608300,0x00002000},//c0
//{0xa1608314,0x00003000},
{0xa1608314,0x00002fff},//c5
{0xa160831c,0x0000d132},//c7
{0xa1608324,0x00000008},//c9
{0xa16083D8,0x00000000},//f6
{0xa16082fc,0x00000000},//bf


{0xa16082c0,0x00000129},//30
{0xa16082c0,0x0000012b},//30

#endif
 

//bt_initial_0dbm
{0Xa1608214,0X000002f6},//05 add for test
{0Xa1608218,0X00005314},//06 
{0Xa160821c,0X000004c1},//07 4CD 
{0Xa160822c,0X0000231f},//0b
{0Xa1608278,0X00003080},//1e
{0Xa1608248,0X00005400},//12   5400 5608
{0Xa1608250,0X00000e8e},//14
{0Xa1608258,0X0000b518},//16
{0Xa1608274,0X00004334},//1d
{0Xa1608290,0X0000eeee},//24 add for power control test
{0Xa16082a0,0X0000248f},//28 // 9999  power control 359f  

{0Xa16082ac,0X000000b8},//2b
{0Xa16082c8,0X00001f00},//32
{0Xa16082d4,0X0000001e},//35
{0xa16082fc,0x00000001},//3f,page1
{0Xa1608228,0X0000000f},//8a
{0Xa160827c,0X0000ffff},//9f


{0xa1608268,0x0000ffff},//9a // power control
{0xa160826c,0x0000ffff},//9b
{0xa1608270,0x0000ffff},//9c
{0xa1608274,0x0000ffff},//9d
{0xa1608278,0x0000ffff},//9e
{0xa160827c,0x0000ffff},//9f

//bt dc

{0xa16082fc,0x00000000},//3f,page0
{0xa16082c0,0x00000129},//30
{0xa16082c0,0x0000012b},//30


//MODEM
{ 0xa16082fc,0x00000001},//3f page1
{ 0xa160831c,0x0000dc3a},//c7
{ 0xa16082fc,0x00000000},//3f page0
{ 0xa1608348,0x00000401},//52 for ble  405
{ 0xa16083ac,0x00001100},//6b
{ 0xa160830c,0x0000FF49},//43
{ 0xa1608310,0x0000a088},//0A0C8 ////44
{ 0xa160833c,0x00004220},//4f 
{ 0xa160831c,0x000055c7},//47
// { 0xa1608328,0x00001000},//4a  xpz
{ 0xa1608320,0x00003057},//48
{ 0xa1608324,0x00000a86},//49
{ 0xa1608308,0x00001f00},//42
{ 0xa1608304,0x0000400b},
{ 0xa1608360,0x0000083c} //58


};


const UINT32 rf_register_21 [] =
{
    0x000010,
    0x065314,
    0x07040d,
    0x0B231f,
    0x1e3080,
    0x127600,
    0x140e8e,
    0x16b518,
    0x28cccc,
    0x1d4334,
    0x2b00b8,
    0x321f00,
    0x35001e,
    0x3f0001,
    0x0a000f,
    0x1fffff,
    0x3f0000,
};

PRIVATE void bt_delay(UINT32 delay, UINT8 nouse)
{
	//void (*pDelay)(UINT32 , UINT8 );
	//pDelay = //0xa1e16601;//0x81e16621;
	//(*pDelay)(bt_delay, nouse);
	HWdelay_Wait_For_ms(delay, nouse);
}

// this function is called by bt_core when initialise
void RDABT_core_rf_Initialization(void)
{
	int i;

       //rdabt_write_pskey(0x26, (UINT8 *)rf_register_21, (UINT8)sizeof(rf_register_21));

	bt_delay(10, FALSE);
	if((RDA_READ_UINT32(0xA1A26000)&(0xFFF))<2)//hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID)<2
	{
		for(i=0;i<sizeof(btcore_rf_init_21)/sizeof(btcore_rf_init_21[0]); i++)
		{
			RDA_WRITE_UINT32(btcore_rf_init_21[i][0],btcore_rf_init_21[i][1]);
			bt_delay(20, FALSE);
		}  
	}
	else
	{
		for(i=0;i<sizeof(btcore_rf_init_21_3)/sizeof(btcore_rf_init_21_3[0]); i++)
		{
			RDA_WRITE_UINT32(btcore_rf_init_21_3[i][0],btcore_rf_init_21_3[i][1]);
			bt_delay(20, FALSE);
		} 

		
	}
	bt_delay(20, FALSE);
}


const uint8 rdabt_pskey_sleep[] = {0xa0, 0x60, 0x50, 0x02, 0x0a, 0x0a};

//const uint8 rdabt_pskey_rf_setting[] = {0x00,0x0a,0x00,0x0c,0x40,0x30,0xb5,0x30,0xb5,0x30,0xba,0xba};
#if APP_SUPPORT_SOUNDBAR == 1
const uint8 rdabt_pskey_rf_setting[] = {0x00,0x16,0x00,0x0c,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0};//old
#else
const uint8 rdabt_pskey_rf_setting[] = {0x00,0x16,0x00,0x0c,0x50,0x50,0xb5,0x50,0xb5,0x50,0xa0,0xba};
#endif
//const uint8 rdabt_pskey_rf_setting[] = {0x00,0x16,0x00,0x0c,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0};//old
//const uint8 rdabt_pskey_pcm_config[] = {0x08,0xc0,0x18,0x10};
#ifdef USE_VOC_CVSD
/*
rdabt_pskey_acl_buf_size_setting
	u_int16	aclDataPacketLength   
	u_int8	scoDataPacketLength
	u_int16	numAclDataPackets
	u_int16	numScoDataPackets
	u_int16	in.aclDataPacketLength
	u_int8	in.scoDataPacketLength
	u_int16	in.numAclDataPackets
	u_int16	in.numScoDataPackets
	u_int16	escotxbuffLength
*/
#if APP_SUPPORT_A2DP_SOURCE==1 || APP_SUPPORT_SOUNDBAR == 1
const uint8 rdabt_pskey_acl_buf_size_setting[] = {0xc0,0x03,0x3c,0x04,0x00,0x0f,0x00,0xfd,0x03,0x3c,0x02,0x00,0x0f,0x00,0x3c,0x00};
#else
const uint8 rdabt_pskey_acl_buf_size_setting[] = {0xc0,0x01,0x3c,0x06,0x00,0x0f,0x00,0xfd,0x03,0x3c,0x02,0x00,0x0f,0x00,0x3c,0x00};
#endif
#else
#if APP_SUPPORT_A2DP_SOURCE==1 || APP_SUPPORT_SOUNDBAR == 1
const uint8 rdabt_pskey_acl_buf_size_setting[] = {0xc0,0x03,0x78,0x04,0x00,0x08,0x00,0xfd,0x03,0x78,0x03,0x00,0x08,0x00,0x78,0x00};
#else
const uint8 rdabt_pskey_acl_buf_size_setting[] = {0xc0,0x01,0x78,0x06,0x00,0x08,0x00,0xfd,0x03,0x78,0x03,0x00,0x08,0x00,0x78,0x00};
#endif
#endif
#ifdef __BT_PCM_OVER_UART__
const uint8 rdabt_pskey_sys_config[] = {0x80,0x20,0x08,0x00};
#else
const uint8 rdabt_pskey_sys_config[] = {0x00,0x20,0x08,0x00};
#endif
#ifdef USE_VOC_CVSD
const uint8 rdabt_pskey_pcm_config[] = {0x08,0xc0,0x98,0x90};
#else
const uint8 rdabt_pskey_pcm_config[] = {0x07,0xc0,0x9d,0x90};	
#endif
const uint32 rdabt_pskey_sys_feature[2] = {
#ifdef __RDABT_DISABLE_EDR__
0xf88ffeff, ///disable edr
#else
#if APP_SUPPORT_SOUNDBAR == 1
0xfa8ffeff, ///enable edr  0xfa8ffeff, only enable 2M, 0xfe8ffeff enable 2M&3M
#else
0xfe8ffeff, ///enable edr  0xfa8ffeff, only enable 2M, 0xfe8ffeff enable 2M&3M
#endif
#endif
#ifdef __RDABT_ENABLE_SP__
0x837bfdd8, ///disable 3m esco ev4 ev5
#else
0x8373fdd8, ///disable 3m esco ev4 ev5
#endif
};

void Rdabt_Pskey_Write_r21(void)
{
   //int32 i;

 //   rdabt_write_pskey(0x35, rdabt_pskey_hostwake, sizeof(rdabt_pskey_hostwake));
    rdabt_write_pskey(0x21, rdabt_pskey_sleep, sizeof(rdabt_pskey_sleep));
//    rdabt_write_pskey(0x15, rdabt_pskey_sys_config, sizeof(rdabt_pskey_sleep));
    rdabt_write_pskey(0x24, rdabt_pskey_rf_setting, sizeof(rdabt_pskey_rf_setting));
    rdabt_write_pskey(0x17, rdabt_pskey_pcm_config, sizeof(rdabt_pskey_pcm_config));
    rdabt_write_pskey(0x19, (const UINT8*)rdabt_pskey_sys_feature, sizeof(rdabt_pskey_sys_feature));
    rdabt_write_pskey(0x20, rdabt_pskey_acl_buf_size_setting, sizeof(rdabt_pskey_acl_buf_size_setting));
//   rdabt_write_pskey(0x25, rdabt_pskey_rssi_config, sizeof(rdabt_pskey_rssi_config));	
   
   
	
}

void RDABT_core_Intialization(void)
{
   // EDRV_TRACE(EDRV_BTD_TRC, 0, "RDABT_core_Intialization");

    Rdabt_Pskey_Write_r21();
}

#endif // __RDA_CHIP_r21_5856__

#endif // __BT_RDABT__
 

