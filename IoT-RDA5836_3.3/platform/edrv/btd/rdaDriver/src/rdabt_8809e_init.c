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

//sync btdV3.45
#ifdef __RDA_CHIP_R18_8809E__

const unsigned short rdabt_phone_init_18[][2] = 
{
#if 0
	{0x3F, 0x0001}, // page 1
	{0x32, 0x0009}, // Init UART IO status
#endif
	{0x3F, 0x0000}, // page 0
};

const unsigned short rdabt_rf_init_18[][2] = 
{
	{0x3F, 0x0000}, // page 0
	{0x01, 0x1fff}, // Padrv_gain_tb_en=1
	{0x06, 0x161c}, // Padrv_op_gain=11;Padrv_ibit_psk<3:0>=000;Padrv_input_range_psk<1:0>=11 
	{0x07, 0x040d}, // 
	{0x08, 0x8326}, // padrv_lo_tune_psk[5:0]=10000;Lna_notch_en=1
	{0x09, 0x04b5}, // rmx_imgrej=1 
	{0x0B, 0x238f}, // Filter_cal_mode =1
	{0x0C, 0x85e8}, // filter_bpmode<2:0>=101
	{0x0E, 0x0920}, // Tx_cal_polarity=1
	{0x0F, 0x8db3}, // adc_iq_swap=1
	{0x10, 0x1400}, // tx_sys_cal_sel<1:0>=10
	{0x12, 0x560c}, // 12H,16'h1604;//Filter_cal_mode=0;padrv_ibit<3:0>=000;padrv_input_range<1:0>=00     
	//{0x12, 0x1604}, // Lower the BT Tx power
	{0x14, 0x4ecc}, // 
	{0x18, 0x0812}, // pll_refmulti2_en=1;
	{0x19, 0x10c8}, // pll_adcclk_en=1
	{0x1E, 0x3024}, // Pll_lpf_gain_tx<1:0>=00;Pll_pfd_res_tx<5:0>=100100
	// TX GAIN
	{0x23, 0x7777}, // PSK
	{0x24, 0x2368}, // 
	{0x27, 0x5555}, // GFSK
	{0x28, 0x1247}, // 
	{0x32, 0x0200}, // tx_dsp_reset_delay=2
	// AGC_by_xudonglin
	{0x3F, 0x0001}, // page 1
	{0x00, 0x020f}, // 80H,16'hd19f;
	{0x01, 0xf9cf}, // 
	{0x02, 0xfc2f}, // 
	{0x03, 0xf92f}, // 
	{0x04, 0xfa2f}, // 
	{0x05, 0xfc2f}, // 
	{0x06, 0xfb3f}, // 
	{0x07, 0x7fff}, // 
	{0x0A, 0x0018}, // thermo_pll_vcoibit_8<3:0>=1000
	// apc;padrv_gain
	{0x18, 0xffff}, // 
	{0x19, 0xffff}, // 
	{0x1A, 0xffff}, // 
	{0x1B, 0xffff}, // 
	{0x1C, 0xffff}, // 
	{0x1D, 0xffff}, // 
	{0x1E, 0xffff}, // 
	{0x1F, 0xffff}, // padrv_gain;9FH,16'hFFEC;padrv_gain20101103;improve ACPR;
	{0x22, 0x0e93}, // default
	{0x25, 0x03e1}, // default
	{0x26, 0x47a5}, // default;set voltage=1.2v
	{0x27,0x0108},  //osc_stable_timer
	{0x28, 0x6800}, // default
	{0x2D, 0x006a}, // default OFF_FM;
	{0x2F, 0x1100}, // default
	{0x32, 0x88f9}, // TM=001;DN=1111
	{0x3F, 0x0000}, // page 0
};


const uint32 rdabt_pskey_rf_18[] =
{
	0x0000000b, // CHIP_PS PSKEY: Total number -----------------
	0x003f0000, // PSKEY: Page 0
	0x0041000b, // TX_DC_OFFSET+/_
	0x00430fc6, // 43H=BFC6
	0x0044048f, // 44H=0580;
	0x00694075, // TX_DC_OFFSET+/_
	0x006b10c0, // 
	0x003f0001, // PSKEY: Page 1  
	0x00453000, // 
	0x0047f13b, // 20120512
	0x00490008, // 
	0x003f0000, // PSKEY: Page 0
	0x10000000, // PSKEY: Flage
};


const uint16 rdabt_dccal_18[][2]=
{
// item:rda8809bt_verB_DC_CAL
{0x30, 0x0129}, // 
{0x30, 0x012b}, // 
};


const uint32 rda_pskey_18[][2] =
{
#ifdef __RDABT_DISABLE_EDR__

{0x80000470,0xf88dffff}, ///disable edr
#ifdef __RDABT_ENABLE_SP__
{0x80000474,0x83791818}, ///disable 3m esco ev4 ev5
#else
{0x80000474,0x83711818}, ///disable 3m esco ev4 ev5
#endif

#else // !__RDABT_DISABLE_EDR__

{0x80000470,0xfe8dffff}, ///enable edr

#ifdef __RDABT_ENABLE_SP__
{0x80000474,0x83791998}, ///disable 3m esco ev4 ev5
#else
{0x80000474,0x83711998}, ///disable 3m esco ev4 ev5
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
#ifdef USE_VOC_CVSD
	{0x800004d0,0x003c03fd}, // set buffer size
	{0x800004d4,0x000f0006},
	{0x800004d8,0x003c03fd},
	{0x800004dc,0x000f0004},
	{0x800004e0,0x0001003c},
#else	
	{0x800004d0,0x007803fd}, // set buffer size
	{0x800004d4,0x00080006},
	{0x800004d8,0x007803fd},
	{0x800004dc,0x00080004},
	{0x800004e0,0x00010078},
#endif

	{0x800004cc,0x04040012},//set bt version to V2.1
};

const uint32 rda_trap_18[][2] = 
{
	{0x40180004,0x0000f5ac},//esco dequeues
	{0x40180024,0x00001cf0},	
	{0x40180008,0x0000f6d0},	
	{0x40180028,0x00001cf0},	
	
	//{0x4018000c,0x000132d0},//edr
	//{0x4018002c,0x00000014},
	{0x4018000c,0x000156dc},//w2
	{0x4018002c,0x0002e958},
	
	{0x40180010,0x00015318},//2ev3-ev3
	{0x40180030,0x0001a2cc},
	
{0x80000000,0xea000044},////release sco
{0x80000100,0xe51ff004},
{0x80000104,0x00021b98},
{0x80000108,0xe51ff004},
{0x8000010c,0x00021cc4},
{0x80000110,0xe51ff004},
{0x80000114,0x0000175c},
{0x80000118,0xe59f0024},
{0x8000011c,0xebfffff7},
{0x80000120,0xe3a00015},
{0x80000124,0xe1a01004},
{0x80000128,0xebfffff8},
{0x8000012c,0xe1a01004},
{0x80000130,0xe3a00018},
{0x80000134,0xebfffff5},
{0x80000138,0xe59f0008},
{0x8000013c,0xebfffff1},
{0x80000140,0xe8bd8010},
{0x80000144,0x80000148},
{0x80000148,0x00000000} ,
{0x40180014,0x00004754},
{0x40180034,0x00031e14},


#if 1 // afh patch
{0x80000004,0xea001cC8},
{0x40180018,0x0001e164}, 
{0x40180038,0x00031e18}, 
{0x80000008,0xea001c99}, 
{0x4018001c,0x0001e048}, 
{0x4018003c,0x00031e1c}, 
                         
{0x80007200,0xe51ff004},
{0x80007204,0x0002b808}, 
{0x80007208,0xe51ff004}, 
{0x8000720c,0x0001bfe8}, 
{0x80007210,0xe51ff004}, 
{0x80007214,0x0001db00}, 
{0x80007218,0xe51ff004}, 
{0x8000721c,0x0001dda4}, 
{0x80007220,0xe51ff004}, 
{0x80007224,0x000087e8}, 
{0x80007228,0xe51ff004}, 
{0x8000722c,0x0001dff8}, 
{0x80007230,0xe51ff004}, 
{0x80007234,0x00024128}, 
{0x80007238,0xe51ff004}, 
{0x8000723c,0x0001e648}, 
{0x80007240,0xe92d4008}, 
{0x80007244,0xe3a008c5}, 
{0x80007248,0xebffffec}, 
{0x8000724c,0xe59f101c}, 
{0x80007250,0xe3c00e40}, 
{0x80007254,0xe5d11002}, 
{0x80007258,0xe2800004}, 
{0x8000725c,0xe08101a0}, 
{0x80007260,0xe2400064}, 
{0x80007264,0xe1a00c00}, 
{0x80007268,0xe1a00c40}, 
{0x8000726c,0xe8bd8008}, 
{0x80007270,0x800027d4}, 
{0x80007274,0xe92d4070}, 
{0x80007278,0xe59f609c}, 
{0x8000727c,0xe1a05000}, 
{0x80007280,0xe5d64008}, 
{0x80007284,0xe1a00004}, 
{0x80007288,0xebffffde}, 
{0x8000728c,0xe3500000}, 
{0x80007290,0x08bd8070}, 
{0x80007294,0xe2041007}, 
{0x80007298,0xe59f0080}, 
{0x8000729c,0xe3a02001}, 
{0x800072a0,0xe7d001a4}, 
{0x800072a4,0xe1100112}, 
{0x800072a8,0x08bd8070}, 
{0x800072ac,0xe1d600da}, 
{0x800072b0,0xe59f206c}, 
{0x800072b4,0xe1d610db}, 
{0x800072b8,0xe1d23dd3}, 
{0x800072bc,0xe59f2064}, 
{0x800072c0,0xe1530000}, 
{0x800072c4,0xba000004}, 
{0x800072c8,0xe281300a}, 
{0x800072cc,0xe1500003}, 
{0x800072d0,0xda000004}, 
{0x800072d4,0xe3710046}, 
{0x800072d8,0xaa000002}, 
{0x800072dc,0xe7d23004}, 
{0x800072e0,0xe2833050}, 
{0x800072e4,0xe7c23004}, 
{0x800072e8,0xe7d22004}, 
{0x800072ec,0xe3520070}, 
{0x800072f0,0x81a01004}, 
{0x800072f4,0x81a00005}, 
{0x800072f8,0x88bd4070}, 
{0x800072fc,0x8affffc3}, 
{0x80007300,0xe2811004}, 
{0x80007304,0xe1500001}, 
{0x80007308,0xb1a01004}, 
{0x8000730c,0xb1a00005}, 
{0x80007310,0xb8bd4070}, 
{0x80007314,0xbaffffbf}, 
{0x80007318,0xe8bd8070}, 
{0x8000731c,0x800013c4}, 
{0x80007320,0x80003108}, 
{0x80007324,0x8000044c}, 
{0x80007328,0x80001373}, 
{0x8000732c,0xe92d4038}, 
{0x80007330,0xe1a04000}, 
{0x80007334,0xebffffb9}, 
{0x80007338,0xe3500000}, 
{0x8000733c,0x0a000021}, 
{0x80007340,0xe1a00004}, 
{0x80007344,0xebffffb7}, 
{0x80007348,0xe3500000}, 
{0x8000734c,0x0a00001d}, 
{0x80007350,0xe59f5074}, 
{0x80007354,0xe5d50000}, 
{0x80007358,0xe3500000}, 
{0x8000735c,0x1a000019}, 
{0x80007360,0xe5d40000}, 
{0x80007364,0xe3500000}, 
{0x80007368,0x0a000016}, 
{0x8000736c,0xebffffaf}, 
{0x80007370,0xe3500000}, 
{0x80007374,0x1a000013}, 
{0x80007378,0xe3a00470}, 
{0x8000737c,0xe5900030}, 
{0x80007380,0xe1a00900}, 
{0x80007384,0xe1a01e20}, 
{0x80007388,0xe5d40002}, 
{0x8000738c,0xe3500001}, 
{0x80007390,0x1a000001}, 
{0x80007394,0xe3510010}, 
{0x80007398,0x3a000001}, 
{0x8000739c,0xe3500000}, 
{0x800073a0,0x1a000008}, 
{0x800073a4,0xebffffa5}, 
{0x800073a8,0xe5c5000a}, 
{0x800073ac,0xeb000007}, 
{0x800073b0,0xe3a00002}, 
{0x800073b4,0xebffff9f}, 
{0x800073b8,0xe5c50008}, 
{0x800073bc,0xe5854004}, 
{0x800073c0,0xe3a00001}, 
{0x800073c4,0xe5c50000}, 
{0x800073c8,0xe8bd8038}, 
{0x800073cc,0x800013c4}, 
{0x800073d0,0xe59f3014}, 
{0x800073d4,0xe1d310db}, 
{0x800073d8,0xe0811081}, 
{0x800073dc,0xe0810000}, 
{0x800073e0,0xe1a00140}, 
{0x800073e4,0xe5c3000b}, 
{0x800073e8,0xe1a0f00e}, 
{0x800073ec,0x800013c4}, 


{0x8000000c,0xea001cf7},
{0x800073f0,0xe59f0014},
{0x800073f4,0xe5d00000},
{0x800073f8,0xE3500030},
{0x800073fc,0xC8bd8ff8},
{0x80007400,0xe59f0008},
{0x80007404,0xe3a0eb76},
{0x80007408,0xe28effc3},
{0x8000740C,0x80001370},
{0x80007410,0x80001373},
{0x40180020,0x0001db08},
{0x40180040,0x00031e20},


#endif

{0x80000010,0xea000050}, // no merge sco packet
{0x80000158,0xe3530015},
{0x8000015c,0x13a03c19},
{0x80000160,0x11dc11ba},
{0x80000164,0x1283f0b4},
{0x80000168,0xe3a01c1a},
{0x8000016c,0xe281f010},
{0x40180100,0x000019b0},
{0x40180120,0x00031e24},


{0x40180104,0x00009818}, // iphone4s connect
{0x40180124,0x00000014},

{0x80000014,0xea0000b9}, // update rssi
{0x80000300,0xe59f3014},
{0x80000304,0xe1a01000},
{0x80000308,0xe20000ff},
{0x8000030c,0xe2800c55},
{0x80000310,0xe5830000},
{0x80000314,0xe3a00a1d},
{0x80000318,0xe280ffaa},
{0x8000031c,0x8000043c},                    
{0x8000043c,0x00000000},                    

{0x40180108,0x0001d2a4},
{0x40180128,0x00031e28},
#ifdef FCC_TEST_OPEN
{0x80000200,0xe51ff004},//test
{0x80000204,0x000319bc},
{0x80000208,0xe51ff004},
{0x8000020c,0x00020A24},
{0x80000210,0xe51ff004},
{0x80000214,0x0001eb60},
{0x80000218,0xe51ff004},
{0x8000021c,0x0001ed90},
{0x80000220,0xe92d4010},
{0x80000224,0xe1a04000},
{0x80000228,0xe59f0044},
{0x8000022c,0xe3a01000},
{0x80000230,0xe1c010b0},
{0x80000234,0xe5d4000a},
{0x80000238,0xe59f2038},
{0x8000023c,0xe3500001},
{0x80000240,0x15821000},
{0x80000244,0x1a000002},
{0x80000248,0xe3a00001},
{0x8000024c,0xe5820000},
{0x80000250,0xeb00000a},
{0x80000254,0xe1a01004},
{0x80000258,0xe59f401c},
{0x8000025c,0xe3a02014},
{0x80000260,0xe1a00004},
{0x80000264,0xebffffe5},
{0x80000268,0xe1a00004},
{0x8000026c,0xe8bd4010},
{0x80000270,0xeaffffe4},
{0x80000274,0x800013dc},
{0x80000278,0x800002cc},
{0x8000027c,0x80001434},
{0x80000280,0xe59f1004},
{0x80000284,0xe59f0004},
{0x80000288,0xeaffffe0},
{0x8000028c,0x22115876},
{0x80000290,0x800002d0},
{0x80000294,0xe92d4008},
{0x80000298,0xe3a00470},
{0x8000029c,0xe590001c},
{0x800002a0,0xe2800003},
{0x800002a4,0xe3c01003},
{0x800002a8,0xe59f0014},
{0x800002ac,0xebffffd9},
{0x800002b0,0xe59f1010},
{0x800002b4,0xe5c1000b},
{0x800002b8,0xe8bd4008},
{0x800002bc,0xe1a00001},
{0x800002c0,0xeaffffd0},
{0x800002c4,0x800002d0},
{0x800002c8,0x80001434},
{0x800002cc,0x00000000},
{0x800002d0,0x00000000},
{0x800002d4,0x00000000},
{0x800002d8,0x00000000},
{0x800002dc,0x00000000},
                        
{0x800002e0,0xeb000001},
{0x800002e4,0xe3a01b88},
{0x800002e8,0xe281ff8f},
{0x800002ec,0xe51ff004},
{0x800002f0,0x80000220},
{0x80000018,0xea0000b0},
{0x4018010c,0x00022238},
{0x4018012c,0x00031e2c},
   
                        
{0x80000320,0xe59f0018},
{0x80000324,0xe5900000},
{0x80000328,0xe3500001},
{0x8000032c,0x0b000004},
{0x80000330,0xe59f0004},
{0x80000334,0xe3a01bc6},
{0x80000338,0xe281fffb},
{0x8000033c,0x800027d4},
{0x80000340,0x800002cc},
{0x80000344,0xe51ff004},
{0x80000348,0x80000294},
{0x8000001c,0xea0000bf},
{0x40180110,0x00031be8}, 
{0x40180130,0x00031e30}, 

#endif // FOR NCC TEST
{0x40180000,0x0000ffff}
};

const UINT32 rda_trap_calib_18[][2] = 
{

};

void RDABT_phone_Intialization_r18(void)
{
	unsigned char i=0;
	for(i =0;i<sizeof(rdabt_phone_init_18)/sizeof(rdabt_phone_init_18[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_phone_init_18[i][0],&rdabt_phone_init_18[i][1],1);		
	}
}

void  RDABT_rf_Intialization_r18(void)
{
	unsigned char i=0;
	for(i=0;i<sizeof(rdabt_rf_init_18)/sizeof(rdabt_rf_init_18[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_rf_init_18[i][0],&rdabt_rf_init_18[i][1],1);
	}
	RDABT_DELAY(10);
}

void Rdabt_Pskey_Write_rf_r18(void)
{
    uint32 data = 0x0004f39c;
    rdabt_wirte_memory(0x40240000, &data, 1, 0);// SPI2_CLK_EN PCLK_SPI2_EN
    RDABT_DELAY(2);
    rdabt_write_pskey(0x26, rdabt_pskey_rf_18, sizeof(rdabt_pskey_rf_18));
    RDABT_DELAY(2);
    data = 0x0000f29c;
    rdabt_wirte_memory(0x40240000, &data, 1, 0);// Disable SPI2_CLK_EN PCLK_SPI2_EN
   
}

void rdabt_DC_write_r18(void)
{
   int32 i;
   for(i=0;i<sizeof(rdabt_dccal_18)/sizeof(rdabt_dccal_18[0]);i++)
   {

      rdabt_iic_rf_write_data(rdabt_dccal_18[i][0],&rdabt_dccal_18[i][1],1);
      RDABT_DELAY(2);
   }
   RDABT_DELAY(10);
}

#ifdef FCC_TEST_OPEN
const uint8 rdabt_pskey_sleep[] = {0x00, 0x10, 0x50, 0x01, 0xa, 0x8};
#else
const uint8 rdabt_pskey_sleep[] = {0xa0, 0x10, 0x50, 0x01, 0xa, 0x8};
#endif
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


void Rdabt_Pskey_Write_r18(void)
{
   int32 i;
   uint32 buadrate;
   for(i=0;i<sizeof(rda_pskey_18)/sizeof(rda_pskey_18[0]);i++)
   {
      rdabt_wirte_memory(rda_pskey_18[i][0],&rda_pskey_18[i][1],1,0);
      RDABT_DELAY(1);
   }
 //   rdabt_write_pskey(0x35, rdabt_pskey_hostwake, sizeof(rdabt_pskey_hostwake));
 //   RDABT_DELAY(5);
 #ifdef FCC_TEST_OPEN
   rdabt_write_pskey(0x21, rdabt_pskey_sleep, sizeof(rdabt_pskey_sleep));
   RDABT_DELAY(5);
 #endif
    rdabt_write_pskey(0x15, rdabt_pskey_sys_config, sizeof(rdabt_pskey_sys_config));
    RDABT_DELAY(5);
    rdabt_write_pskey(0x24, rdabt_pskey_rf_setting, sizeof(rdabt_pskey_rf_setting));
    RDABT_DELAY(5);
    rdabt_write_pskey(0x17, rdabt_pskey_pcm_config, sizeof(rdabt_pskey_pcm_config));
    RDABT_DELAY(5);
    buadrate = BT_UART_BAUDRATE;
    rdabt_write_pskey(0x30, &buadrate, sizeof(buadrate));
    RDABT_DELAY(5);
//   rdabt_write_pskey(0x25, rdabt_pskey_rssi_config, sizeof(rdabt_pskey_rssi_config));	
   
   
	
}

void Rdabt_trap_write_r18(void)
{
//   extern BOOL  g_CalbClock;
   int32 i;
   
#if 0   
   if(g_CalbClock)
   {
       hal_HstSendEvent(1, 0x88888888);
       for(i=0;i<sizeof(rda_trap_calib_18)/sizeof(rda_trap_calib_18[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_calib_18[i][0],&rda_trap_calib_18[i][1],1,0x0);
          RDABT_DELAY(1);
       }	
   }
   else
#endif   
   {
       for(i=0;i<sizeof(rda_trap_18)/sizeof(rda_trap_18[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_18[i][0],&rda_trap_18[i][1],1,0);
          RDABT_DELAY(1);
       }	
#if 0
	   	rdabt_adp_uart_stop();
		rdabt_adp_uart_start();
		rdabt_adp_uart_configure(115200,FALSE);
		rdabt_uart_register_cb(); 
		rdabt_baudrate_ctrl();

		RDABT_DELAY(100);
	   for(i=0;i<sizeof(rda_trap_18_2)/sizeof(rda_trap_18_2[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_18_2[i][0],&rda_trap_18_2[i][1],1,0);
          RDABT_DELAY(1);
       }
#endif	   
  }
}

void RDABT_core_Intialization_r18(void)
{
    EDRV_TRACE(EDRV_BTD_TRC, 0, "RDABT_core_Intialization_r18");

    RDABT_rf_Intialization_r18();
    Rdabt_Pskey_Write_rf_r18();
    rdabt_DC_write_r18();
    Rdabt_Pskey_Write_r18();
    Rdabt_trap_write_r18();
}

#endif // __RDA_CHIP_r18_8809__

#endif // __BT_RDABT__
 

