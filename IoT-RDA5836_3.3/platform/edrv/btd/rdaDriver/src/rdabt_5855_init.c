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



#ifdef __BT_RDABT__
#include "stdio.h"                  /* Basic C I/O functions */

#include "rdabt_drv.h"

#ifdef __RDA_CHIP_R20_5855__

#define RDA_READ_UINT32( _register_ ) \
        (*((volatile unsigned int *)(_register_)))

#define RDA_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))


const UINT32 btcore_rf_init_20[][2] = 
{
{0Xa16082fc,0X00000000},
{0Xa1608208,0X0000fe00},//02
{0Xa160821c,0X0000052D},//07  0X0000060D 
{0Xa1608220,0X00009007},//08
{0Xa1608224,0X000006a6},//09
{0Xa1608238,0X00000920},//0e
{0Xa160823c,0X00009C03},//{0Xa160823c,0X00008cf1},//0f

{0Xa1608248,0X00007f1f},//12
{0Xa1608250,0X000008c8},//14
{0Xa1608254,0X000053b4},//15
{0Xa1608258,0X00003518},//{0Xa1608258,0X00003b18},//16
{0Xa160825c,0X00002200},//17
{0Xa1608260,0X00001e12},//{0Xa1608260,0X00009f12},//18
{0Xa1608264,0X000010c8},//19
{0Xa1608248,0X00003516},//12


{0Xa1608274,0X0000a61f},//1d 0X0000db1d 
{0Xa1608278,0X000030a4},//1e 0X0000308f 
{0xa16082c0,0x0000012b},//30
{0Xa16082fc,0X00000001},//3f,page1
{0Xa1608200,0X000001af},//80
{0Xa1608204,0X000001bf},//81
{0Xa1608208,0X00000a3f},//82
{0Xa160820c,0X0000323f},//83
{0Xa1608210,0X00003a3f},//84
{0Xa1608214,0X00005bff},//85
{0Xa1608218,0X00003fff},//86
{0Xa160821c,0X00007fff},//87
{0xa16082fc,0x00000000},//3f,page0

{0xa1608304,0x0000000b},//41
{0xa1608308,0x000025bd},//42
{0xa160830c,0x00008fc6},//43
{0xa1608310,0x0000056f},//44
{0xa1608324,0x000008e4},//49
{0xa16083a4,0x00004075},//69
{0xa16083ac,0x000010c0},//6b
{0xa16082fc,0x00000001},//3f,page1
{0xa160827c,0x0000f7f7},//9f
{0xa1608300,0x00002000},//c0
//{0xa1608314,0x00003000},
{0xa1608314,0x00002fff},//c5
{0xa160831c,0x0000d132},//c7
{0xa1608324,0x00000008},//c9
{0xa16083D8,0x00000000},//f6
{0xa16082fc,0x00000000},//bf


{0xa16082c0,0x00000129},//30
{0xa16082c0,0x0000012b},//30

};




const uint32 rda_pskey_20[][2] =
{
#ifdef __RDABT_DISABLE_EDR__

{0x80000470,0xf88dffff}, ///disable edr
#ifdef __RDABT_ENABLE_SP__
{0x80000474,0x83793818}, ///disable 3m esco ev4 ev5
#else
{0x80000474,0x83713818}, ///disable 3m esco ev4 ev5
#endif

#else // !__RDABT_DISABLE_EDR__

{0x80000470,0xfe8dffff}, ///enable edr

#ifdef __RDABT_ENABLE_SP__
{0x80000474,0x83793998}, ///disable 3m esco ev4 ev5
#else
{0x80000474,0x83713998}, ///disable 3m esco ev4 ev5
#endif

#endif // !__RDABT_DISABLE_EDR__

#ifdef BT_UART_BREAK_INT_WAKEUP
//{0x800000a4,0x0f20280a}, // use uart tx to wake host	
//{0x80000040,0x02000000},
{0x40200010,0x00007fff}, // set hostwake gpio to input
#else
//host wakeup
{0x40200010,0x00007f7f},
#endif
	{0x800004d0,0x007803fd}, // set buffer size
	{0x800004d4,0x00080006},
	{0x800004d8,0x007803fd},
	{0x800004dc,0x00080004},
	{0x800004e0,0x00010078},

};

void BT_FUNC_INTERNAL delay(UINT32 delay, UINT8 nouse)
{
	void (*pDelay)(UINT32 , UINT8 );
	//pDelay = //0xa1e16601;//0x81e16621;
	//(*pDelay)(delay, nouse);
	HWdelay_Wait_For_ms(delay, nouse);
}

// this function is called by bt_core when initialise
void BT_FUNC_INTERNAL RDABT_core_rf_Initialization(void)
{
	int i;

	delay(10, FALSE);
	for(i=0;i<sizeof(btcore_rf_init_20)/sizeof(btcore_rf_init_20[0]); i++)
	{
		RDA_WRITE_UINT32(btcore_rf_init_20[i][0],btcore_rf_init_20[i][1]);
		delay(20, FALSE);
	}
	delay(20, FALSE);
}


const uint8 rdabt_pskey_sleep[] = {0xa, 0x10, 0x50, 0x01, 0xa, 0x8};
const uint8 rdabt_pskey_rf_setting[] = {0x00,0x0a,0x00,0x0c,0x40,0x30,0xb5,0x30,0xb5,0x30,0xba,0xba};
//const uint8 rdabt_pskey_pcm_config[] = {0x08,0xc0,0x18,0x10};

#ifdef __BT_PCM_OVER_UART__
const uint8 rdabt_pskey_sys_config[] = {0x80,0x20,0x08,0x00};
#else
const uint8 rdabt_pskey_sys_config[] = {0x00,0x20,0x08,0x00};
#endif
#ifdef USE_VOC_CVSD
const uint8 rdabt_pskey_pcm_config[] = {0x08,0xc0,0x98,0x90};
#else
const uint8 rdabt_pskey_pcm_config[] = {0x07,0xc0,0x98,0x90};	
#endif


void Rdabt_Pskey_Write_r20(void)
{
   int32 i;
   for(i=0;i<sizeof(rda_pskey_20)/sizeof(rda_pskey_20[0]);i++)
   {
      rdabt_wirte_memory(rda_pskey_20[i][0],&rda_pskey_20[i][1],1,0);
      RDABT_DELAY(1);
   }
 //   rdabt_write_pskey(0x35, rdabt_pskey_hostwake, sizeof(rdabt_pskey_hostwake));
 //   RDABT_DELAY(5);
//    rdabt_write_pskey(0x21, rdabt_pskey_sleep, sizeof(rdabt_pskey_sleep));
//    RDABT_DELAY(5);
    rdabt_write_pskey(0x15, rdabt_pskey_sys_config, sizeof(rdabt_pskey_sleep));
    RDABT_DELAY(5);
    rdabt_write_pskey(0x24, rdabt_pskey_rf_setting, sizeof(rdabt_pskey_rf_setting));
    RDABT_DELAY(5);
    rdabt_write_pskey(0x17, rdabt_pskey_pcm_config, sizeof(rdabt_pskey_pcm_config));
    RDABT_DELAY(5);
//   rdabt_write_pskey(0x25, rdabt_pskey_rssi_config, sizeof(rdabt_pskey_rssi_config));	
   
   
	
}

void RDABT_core_Intialization_r20(void)
{
    EDRV_TRACE(EDRV_BTD_TRC, 0, "RDABT_core_Intialization_r18");

    //RDABT_rf_Intialization_r19();
    Rdabt_Pskey_Write_r20();
}

#endif // __RDA_CHIP_r20_5855__

#endif // __BT_RDABT__
 

