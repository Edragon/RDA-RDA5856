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


#include "tgt_app_cfg.h"
#include "rdabt_drv.h"

#ifdef __RDA_CHIP_R22_5856E__

#define RDA_READ_UINT32( _register_ ) \
        (*((volatile unsigned int *)(_register_)))

#define RDA_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))

#define rdabt_write_pskey(id,data,length) SYSHal_Config_Apply(id,length,data)
extern void SYSHal_Config_Apply(UINT8 id, UINT8 length, const UINT8 *data);
extern void HWdelay_Wait_For_us(UINT32 units_of_u_secs);
extern void HWdelay_Wait_For_ms(UINT32 units_of_m_secs, UINT32 use_native_clk);

const UINT32 btcore_rf_init_step1[][2] = 
{
{ 0xA1A00038,0x0},       //SYS_CTRL: APB config for RF registers
{ 0xA1A01014,0xD8E1771}, //XTAL offset, need to recalibration for a new xtal/PCB
{ 0xA1A1908C,0x20002400}, //RF_IF: bt_gain_table_0
{ 0xA1A19090,0x10003000}, //RF_IF: bt_gain_table_1
{ 0xA1A19094,0x30103410}, //RF_IF: bt_gain_table_2
{ 0xA1A19098,0x38103C10}, //RF_IF: bt_gain_table_3
{ 0xA1A1909C,0x3D103F10}, //RF_IF: bt_gain_table_4
{ 0xA1A190A0,0x3F303FF0}, //RF_IF: bt_gain_table_5
{ 0xA1A190A4,0x3FF43FF8}, //RF_IF: bt_gain_table_6
{ 0xA1A190A8,0x3FF83FF8}, //RF_IF: bt_gain_table_7
{ 0xA1A19014,0x0},        //RF_IF: rf_ivref_avdd_ibit
{ 0xA1A19018,0x8204060E}, //RF_IF: bt_lna_reg_bit; bt_lan_vcas_bit; bt_rmx_reg_bit
{ 0xA1A19034,0xA79019C},  //RF_IF: rfpll_presc_reg_bit=0xE 
{ 0xA1A1906C,0xC300},     //RF_IF: vco_ibit_reg=0xF 
{ 0xA1A19030,0x1C8},      //RF_IF: vco_buf_ibit
{ 0xA1A19020,0xAE00304},  //RF_IF: adc_delay_ibit=4'h8; rxfil_cap_bit=4'hE
{ 0xA1A19060,0x3FA3F},    //RF_IF: dc_offset_init_delay=3; dc_cal_cnt=0xF; rxfil_offset_range_bit=0x7
{ 0xA1A19070,0x1200043},  //RF_IF: rfpll_init_delay
{ 0xA1A190E0,0x1BE4},     //RF_IF
{ 0xA1A16010,0xA1CF},     //MODEM: tx_gain_gfsk in BR=0xCF
{ 0xA1A16018,0x1B8},      //MODEM: tx_gain_gfsk_edr=0xB8
{ 0xA1A161A4,0xC25A},     //MODEM: notch_a=0xC; edr3_adapt_en=1;
{ 0xA1A161A8,0x14},       //MODEM: edr3_adapt_th=0x14
{ 0xA1A0F018,0x3A083000}, //BT_CORE: modem_txon_delay; modem_txoff_delay
{ 0xA1A19080,0x40},       //RF_IF: pu_lo_rx_dr
{ 0xA1A19084,0x100040},   //RF_IF: pu_lo_rx_reg=1; pu_rf_iobuf_reg=1
{ 0xA1A19004,0x11},       //RF_IF: bt_hopping_en=0;
{ 0xA1A19004,0x13},       //RF_IF: chip_self_cal_enable=1
}; 

const UINT32 btcore_rf_init_step2[][2] = 
{
{ 0xA1A19080,0x0},        //RF_IF: pu_lo_rx_dr=0
{ 0xA1A19004,0x33},       //RF_IF: bt_hopping_en=1;
{ 0xA1A190C0,0x22232425}, //RF_IF: APC for tx
{ 0xA1A190C4,0x2D26273F}, //RF_IF: bt_padrv_tmx_gain_table_1
{0xA1A00038,0x1},         //SYS_CTRL: APB config for RF registers
};

PRIVATE void bt_delay(UINT32 delay, UINT8 nouse)
{
	//void (*pDelay)(UINT32 , UINT8 );
	//pDelay = //0xa1e16601;//0x81e16621;
	//(*pDelay)(delay, nouse);
	HWdelay_Wait_For_ms(delay, nouse);
}

void BT_Wakeup_Configuration_step1(void)
{
      
       *(uint32*)(0xA1A00038) = 0x0;
       *(uint32*)(0xA1A01014) = 0xD8E1771;
       *(uint32*)(0xA1A1908C) = 0x20002400;
       *(uint32*)(0xA1A19090) = 0x10003000;
       *(uint32*)(0xA1A19094) = 0x30103410;
       *(uint32*)(0xA1A19098) = 0x38103C10;
       *(uint32*)(0xA1A1909C) = 0x3D103F10;
       *(uint32*)(0xA1A190A0) = 0x3F303FF0;
       *(uint32*)(0xA1A190A4) = 0x3FF43FF8;
       *(uint32*)(0xA1A190A8) = 0x3FF83FF8;
       *(uint32*)(0xA1A19014) = 0x0;
       *(uint32*)(0xA1A19018) = 0x8204060E;  //0x8A04060E;
       *(uint32*)(0xA1A19034) = 0xA79019C;
       *(uint32*)(0xA1A1906C) = 0xC300;   //0xD300;
       *(uint32*)(0xA1A19030) = 0x1C8; //0x1A9;
       *(uint32*)(0xA1A19020) = 0xAE00304;
	   *(uint32*)(0xA1A19060) = 0x3FA3F;
       *(uint32*)(0xA1A19070) = 0x1200043;
       *(uint32*)(0xA1A190E0) = 0x1BE4;     //RF_IF
       *(uint32*)(0xA1A16010) = 0xA1CF;     //MODEM: tx_gain_gfsk in BR=0xCF
       *(uint32*)(0xA1A16018) = 0x1B8;      //MODEM: tx_gain_gfsk_edr=0xB8
       *(uint32*)(0xA1A161A4) = 0xC25A;     //MODEM: notch_a=0xC; edr3_adapt_en=1;
       *(uint32*)(0xA1A161A8) = 0x14;       //MODEM: edr3_adapt_th=0x14
       *(uint32*)(0xA1A0F018) = 0x3A083000; //BT_CORE: modem_txon_delay; modem_txoff_delay    
 
       //VCO peak detection calibration
       *(uint32*)(0xA1A19080) = 0x14f178;
       *(uint32*)(0xA1A19084) = 0x14f178;
       *(uint32*)(0xA1A19078) = 0x20;
        HWdelay_Wait_For_us(10);
       *(uint32*)(0xA1A19078) = 0x30;
       while(!((*(uint32*)(0xA1A1907c) & 0x10) == 0x10)) HWdelay_Wait_For_us(20);

       //VCO calibration
       *(uint32*)(0xA1A19074) = 0x9600;
       *(uint32*)(0xA1A19070) = 0x1200843;
       *(uint32*)(0xA1A19078) = 0x8;
       HWdelay_Wait_For_us(10);
       *(uint32*)(0xA1A19078) = 0xc;
       while(!((*(uint32*)(0xA1A1907c) & 0x8) == 0x8)) HWdelay_Wait_For_us(20);

       //DC calibration
       *(uint32*)(0xA1A19004) = 0x31;
       *(uint32*)(0xA1A19078) = 0x280;
       HWdelay_Wait_For_us(10);
       *(uint32*)(0xA1A19078) = 0x2c0;
}
void BT_Wakeup_Configuration_step2(void)
{
       //release
       *(uint32*)(0xA1A19080) = 0x0;
       *(uint32*)(0xA1A19070) = 0x1200043;
       *(uint32*)(0xA1A19078) = 0x0;
       *(uint32*)(0xA1A19078) = 0x3;
       
       *(uint32*)(0xA1A190C0) = 0x22232425;
       *(uint32*)(0xA1A190C4) = 0x2D26273F;
       *(uint32*)(0xA1A00038) = 0x1;


}

// this function is called by bt_core when initialise
void RDABT_core_rf_Initialization(void)
{
	int i;

	//bt_delay(10, FALSE);
	for(i=0;i<sizeof(btcore_rf_init_step1)/sizeof(btcore_rf_init_step1[0]); i++)
	{
		RDA_WRITE_UINT32(btcore_rf_init_step1[i][0],btcore_rf_init_step1[i][1]);
		//bt_delay(20, FALSE);
	}  

	bt_delay(200, FALSE);
	for(i=0;i<sizeof(btcore_rf_init_step2)/sizeof(btcore_rf_init_step2[0]); i++)
	{
		RDA_WRITE_UINT32(btcore_rf_init_step2[i][0],btcore_rf_init_step2[i][1]);
		//bt_delay(20, FALSE);
	}      
}


const uint8 rdabt_pskey_sleep[] = {0xa0, 0x28, 0x50, 0x02, 0x0a, 0x0a};

//const uint8 rdabt_pskey_rf_setting[] = {0x00,0x0a,0x00,0x0c,0x40,0x30,0xb5,0x30,0xb5,0x30,0xba,0xba};
#if APP_SUPPORT_SOUNDBAR == 1
const uint8 rdabt_pskey_rf_setting[] = {0x00,0x11,0x00,0x0c,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0,0xa0};//old
#else
const uint8 rdabt_pskey_rf_setting[] = {0x00,0x11,0x00,0x0c,0x50,0x50,0xb5,0x50,0xb5,0x50,0xa0,0xba};
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
    
#if (0) // CHIP_AS_RF_BOARD
    {
        /*Using 5856 60-pin chips as RF Board, by xzc*/

        const UINT32 btcore_rf_init_rf_board = 
        {
            {0xa16082fc,0x00000001},//3f,page1   for weipeng rf ext mode by skj
            {0xa16083D8,0x3},
            {0xa16082fc,0x00000000}
        };

        for(i=0;i<sizeof(btcore_rf_init_rf_board)/sizeof(btcore_rf_init_rf_board[0]); i++)
        {
        	RDA_WRITE_UINT32(btcore_rf_init_rf_board[i][0],btcore_rf_init_rf_board[i][1]);
        	delay(20, FALSE);
        } 
        
        *(volatile unsigned int*)(0xa1a05024) = 0x7;//for weipeng 86 rf to ip by skj
        *(volatile unsigned int*)(0xa1a05034) = 0x7;
        *(volatile unsigned int*)(0xa1a05038) = 0x7;
        *(volatile unsigned int*)(0xa1a0503c) = 0x7;
        *(volatile unsigned int*)(0xa1a05078) = 0x7;  
    }
#endif
}

#endif // __RDA_CHIP_r21_5856__

#endif // __BT_RDABT__
 

