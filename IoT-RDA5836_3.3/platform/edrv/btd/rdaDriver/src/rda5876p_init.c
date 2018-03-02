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
//#include "kal_release.h"      	/* Basic data type */
//#include "stack_common.h"          /* message and module IDs */
//#include "stack_msgs.h"            /* enum for message IDs */
//#include "app_ltlcom.h"       	/* Task message communiction */
//#include "syscomp_config.h"         /* type of system module components */
//#include "task_config.h"      	/* Task creation */
//#include "stacklib.h"        	   /* Basic type for dll, evshed, stacktimer */
//#include "event_shed.h"       	/* Event scheduler */
//#include "stack_timer.h"      	/* Stack timer */
//#include "stdio.h"                  /* Basic C I/O functions */
//#include "eint.h"                   /* External interrupt */

//#include "uart_sw.h"               /* header file for UART */
#include "rdabt_drv.h"
//#include "mmi_trace.h"

PUBLIC BOOL g_FMOpen_Flag = FALSE;
PUBLIC BOOL g_BTOpen_Flag = FALSE;

#ifdef __RDA_CHIP_R16_5876P__

extern unsigned short rdabt_chipid,rdabt_revid;

const unsigned short rdabt_phone_init_16[][2] = 
{                                                                                        

};                                                   

const unsigned short rdabt_rf_init_16[][2] = 
{
{0x003f,0x0000},   //
{0x0001,0x1FFF},   //;//Padrv_gain_tb_en=1
{0x0006,0x161C},   //;//Padrv_op_gain=11;Padrv_ibit_psk<3:0>=000;Padrv_input_range_psk<1:0>=11 
{0x0007,0x040D},   //;
{0x0008,0x8326},   //;//padrv_lo_tune_psk[5:0]=10000;Lna_notch_en=1
{0x0009,0x04B5},   //;//rmx_imgrej=1 
{0x000B,0x238F},   //;//Filter_cal_mode =1
{0x000C,0x85E8},   //;//filter_bpmode<2:0>=101
{0x000E,0x0920},   //;//Tx_cal_polarity=1
{0x000F,0x8DB3},   //;//adc_iq_swap=1
{0x0010,0x1402},   //;//tx_sys_cal_sel<1:0>=10
{0x0012,0x5604},   //;//Padrv_opa_bandwidth[1:0]=10;16'h1604;//;padrv_input_range<1:0>=00     
{0x0014,0x4ECC},   //;
{0x0015,0x5124},   //;pll_gain_rx<2:0> 100
{0x0018,0x0812},   //;//pll_refmulti2_en=1;
{0x0019,0x10C8},   //;//pll_adcclk_en=1
{0x001E,0x3024},   //;//Pll_lpf_gain_tx<1:0>=00;Pll_pfd_res_tx<5:0>=100100
{0x0023,0x9991},   //;PSK
{0x0024,0x2369},   //;
{0x0027,0x8881},   //;GFSK
{0x0028,0x2358},   //;
{0x0032,0x0E00},   //;//tx_dsp_reset_delay=2
{0x003f,0x0001},   //
{0x000A,0x0018},   //;//thermo_pll_vcoibit_8<3:0>=1000
{0x0000,0x020f},   //;//80H,16'hd19f;
{0x0001,0xf9cf},   //;
{0x0002,0xfc2f},   //;
{0x0003,0xf92f},   //;
{0x0004,0xfa2f},   //;
{0x0005,0xfc2f},   //;
{0x0006,0xfb3f},   //;
{0x0007,0x7fff},   //;
{0x0018,0xFFFF},   //;
{0x0019,0xFFFF},   //;
{0x001A,0xFFFF},   //;
{0x001B,0xFFFF},   //;
{0x001C,0xFFFF},   //;
{0x001D,0xFFFF},   //;
{0x001E,0xFFFF},   //;
{0x001F,0xFFFF},   //;padrv_gain;9FH,16'hFFEC;padrv_gain20101103;improve ACPR;
/*{0x0022,0x0E93},   //;//default
{0x0025,0x03E1},   //;//default
{0x0026,0x47A5},   //;//default;set voltage=1.2v
{0x0027,0x0108},   //; 
{0x0028,0x6800},   //;//default
{0x002D,0x006A},   //;//default OFF_FM;
{0x002F,0x1100},   //;//default
{0x0032,0x88F9},   //;//TM=011;DN=1111*/
{0x003f,0x0000},   //     
};

const unsigned short rdabt_rf_init_16_c[][2] = 
{
{0x003f,0x0000},   //
{0x0001,0x1FFF},   //;//Padrv_gain_tb_en=1
{0x0006,0x161C},   //;//Padrv_op_gain=11;Padrv_ibit_psk<3:0>=000;Padrv_input_range_psk<1:0>=11 
{0x0007,0x040D},   //;
{0x0008,0x8326},   //;//padrv_lo_tune_psk[5:0]=10000;Lna_notch_en=1
{0x0009,0x04B5},   //;//rmx_imgrej=1 
{0x000B,0x238F},   //;//Filter_cal_mode =1
{0x000C,0x85E8},   //;//filter_bpmode<2:0>=101
{0x000E,0x0920},   //;//Tx_cal_polarity=1
{0x000F,0x8DB3},   //;//adc_iq_swap=1
{0x0010,0x1400},   //;//tx_sys_cal_sel<1:0>=10
{0x0012,0x5604},   //;//Padrv_opa_bandwidth[1:0]=10;16'h1604;//;padrv_input_range<1:0>=00     
{0x0014,0x4ECC},   //;
{0x0015,0x5124},   //;pll_gain_rx<2:0> 100
{0x0018,0x0812},   //;//pll_refmulti2_en=1;
{0x0019,0x10C8},   //;//pll_adcclk_en=1
{0x001E,0x3024},   //;//Pll_lpf_gain_tx<1:0>=00;Pll_pfd_res_tx<5:0>=100100
{0x0023,0x1111},   //;PSK
{0x0024,0x2468},   //;
{0x0027,0x1111},   //;GFSK
{0x0028,0x2358},   //;
{0x0032,0x0E00},   //;//tx_dsp_reset_delay=2
{0x003f,0x0001},   //
{0x000A,0x0018},   //;//thermo_pll_vcoibit_8<3:0>=1000
{0x0000,0x020f},   //;//80H,16'hd19f;
{0x0001,0xf9cf},   //;
{0x0002,0xfc2f},   //;
{0x0003,0xf92f},   //;
{0x0004,0xfa2f},   //;
{0x0005,0xfc2f},   //;
{0x0006,0xfb3f},   //;
{0x0007,0x7fff},   //;
{0x0017,0xE7F4},   //;
{0x0018,0xF8F8},   //;
{0x0019,0xFDFD},   //;
{0x001A,0xF2F3},   //;
{0x001B,0xFEFF},   //;
{0x001C,0xFEFF},   //;
{0x001D,0xFFFF},   //;
{0x001E,0xFFFF},   //;
{0x001F,0xFFFF},   //;padrv_gain;9FH,16'hFFEC;padrv_gain20101103;improve ACPR;
/*{0x0022,0x0E93},   //;//default
{0x0025,0x03E1},   //;//default
{0x0026,0x47A5},   //;//default;set voltage=1.2v
{0x0027,0x0108},   //; 
{0x0028,0x6800},   //;//default
{0x002D,0x006A},   //;//default OFF_FM;
{0x002F,0x1100},   //;//default
{0x0032,0x88F9},   //;//TM=011;DN=1111*/
{0x003f,0x0000},   //     
};

const UINT32 rdabt_pskey_rf_16_c[][2] =
{
{0x40240000,0x2004f39c},   //; SPI2_CLK_EN PCLK_SPI2_EN
{0x800000C0,0x0000000F},   //; CHIP_PS PSKEY: Total number -----------------
{0x800000C4,0x003F0000},   //;         PSKEY: Page 0
{0x800000C8,0x0041000B},   //;//TX_DC_OFFSET+/_
{0x800000CC,0x004225BD},   //;//42H=32D0,1CH+9K,2CH -11K,afc_smtif
{0x800000D0,0x0043BFC6},   //;//43H=BFC6
{0x800000D4,0x0044040F},   //;//44H=0580;44H=040F;tx_gain_dr=0
{0x800000D8,0x004908E4},   //;//49H=08E4;
{0x800000DC,0x00694075},   //;//TX_DC_OFFSET+/_
{0x800000E0,0x006B10C0},   //;
{0x800000E4,0x003F0001},   //;         PSKEY: Page 1  
{0x800000E8,0x00402000},   //;
{0x800000EC,0x00453000},   //;
{0x800000F0,0x0047D132},   //;//20120512
{0x800000F4,0x00490008},   //;
{0x800000F8,0x003F0000},   //;         PSKEY: Page 0
{0x80000040,0x10000000},   //;         PSKEY: Flage
{0x40240000,0x2000f29c},   //; SPI2_CLK_EN PCLK_SPI2_EN
};

const UINT32 rdabt_pskey_rf_16[][2] =
{
{0x40240000,0x2004f39c},   //; SPI2_CLK_EN PCLK_SPI2_EN
{0x800000C0,0x0000000E},   //; CHIP_PS PSKEY: Total number -----------------
{0x800000C4,0x003F0000},   //;         PSKEY: Page 0
{0x800000C8,0x0041000B},   //;//TX_DC_OFFSET+/_
{0x800000CC,0x004232D0},   //;//42H=32D0,1CH+9K,2CH -11K,afc_smtif
{0x800000D0,0x0043BFC6},   //;//43H=BFC6
{0x800000D4,0x0044040F},   //;//44H=0580;44H=040F;tx_gain_dr=0
{0x800000D8,0x004908E4},   //;//49H=08E4;
{0x800000DC,0x00694075},   //;//TX_DC_OFFSET+/_
{0x800000E0,0x006B10C0},   //;
{0x800000E4,0x003F0001},   //;         PSKEY: Page 1  
{0x800000E8,0x00453000},   //;
{0x800000EC,0x0047D132},   //;//20120512
{0x800000F0,0x00490008},   //;
{0x800000F4,0x003F0000},   //;         PSKEY: Page 0
{0x80000040,0x10000000},   //;         PSKEY: Flage
{0x40240000,0x2000f29c},   //; SPI2_CLK_EN PCLK_SPI2_EN
};
UINT32 rda_pskey_16[][2] =
{
{0x800000a8,0x0Bbaba30},//min power level
#ifdef __BT_PCM_OVER_UART__
{0x80000070,0x00002080},
#else
{0x80000070,0x00002000},//zhou siyou esco->soc 0x00002000->0x00002008
#endif
{0x80000074,0xa5025010},
{0x80000078,0x0a004000},
{0x8000007c,0xb530b530},
//{0x80000084,0x9098c007},
#if defined( _RDA_PATCH_UART3_) || defined(_RDA_PATCH_UART4_) || defined(_RDA_PATCH_UART5_) || defined(_RDA_PATCH_UART6_) || defined(_RDA_PATCH_UART7_) || defined(_RDA_PATCH_UART8_) || defined(_RDA_PATCH_UART9_)
{0x80000064,0x0030d400},//new baudrate 3200000
#else
{0x80000064,BT_UART_BAUDRATE},//new baudrate
#endif

#ifdef USE_VOC_CVSD
{0x80000084,0x9098c008},//csvd
{0x80000040,0x0402f200},
#else
{0x80000040,0x0400f200},//PSKEY: modify flag
#endif

#ifdef __RDABT_DISABLE_EDR__
{0x800004f0,0xf88dffff}, ///disable edr
{0x800004f4,0x83711998}, ///disable 3m esco ev4 ev5

#else  //__RDABT_DISABLE_EDR__
 
{0x800004f4,0x83711998}, ///disable 3m esco ev4 ev5

#endif

#ifdef BT_UART_BREAK_INT_WAKEUP
{0x800000a4,0x0f20280a}, //use uart tx to wake host	
{0x80000040,0x02000000},
{0x40200010,0x00007fff}, // set hostwake  gpio to	input
#else
//host wakeup
{0x40200010,0x00007f7f},
#endif

//CoolSand Only CTS-->GPIO2
{0x40200044,0x0000003c}, //;
{0x700000b0,0x0000000a},


{0x80000550,0x003c03fd}, // set buffer size
{0x80000554,0x000f0006},
{0x80000558,0x003c03fd},
{0x8000055c,0x000f0004},
{0x80000560,0x0001003c},

};



   


const UINT32 rda_trap_16[][2] = 
{
    {0x40180004,0x0001b578},
    {0x40180024,0x0001dc68},
    {0x40180008,0x00003034},
    {0x40180028,0x00000014},
    {0x4018000c,0x0001514c},
	{0x4018002c,0x00000014},
	
	{0x40180010,0x000193b8}, //Authentication_Requested
	{0x40180030,0x00023718},
	{0x40180014,0x000193cc },//seconed security zhou siyou 20121016
	{0x40180034,0x00000014},
  {0x40180018,0x000142dc}, // for a2dp audio, fix 3M packet
  {0x40180038,0x00017170},
  {0x4018001c,0x00013050}, //edr
  {0x4018003c,0x00000014},
  {0x40180020,0x00015460},//w2
  {0x40180040,0x0002de58},

/*{0x80000010,0xea000062},//ltaddr
{0x800001a0,0x05d41027},  
{0x800001a4,0x00611181}, 
{0x800001a8,0x00811004}, 
{0x800001ac,0x05d1102a}, 
{0x800001b0,0x01510000}, 
{0x800001b4,0xe3a00bae}, 
{0x800001b8,0x1280fff6}, 
{0x800001bc,0x0280ffd6}, 
{0x40180100,0x0002d684},
{0x40180120,0x000312f0},

{0x80000350,0xe51ff004}, //pause enc 
{0x80000354,0x00002864},
{0x80000358,0xe92d4010},
{0x8000035c,0xebfffffb},
{0x80000360,0xe3a01000},
{0x80000364,0xe5c01054},
{0x80000368,0xe8bd8010},
{0x80000014,0xea0000d4},
{0x8000036c,0x03a00c79},
{0x80000370,0x0280f010},
{0x80000374,0xe3550012},
{0x80000378,0x059f000c},
{0x8000037c,0x05860004},
{0x80000380,0xe3a00c79},
{0x80000384,0x0280f07c},
{0x80000388,0xe280f030},
{0x8000038c,0x80000358},           
{0x40180104,0x0000790c},
{0x40180124,0x000312f4},*/

{0x40180108,0x00015090},///2ev3-ev3
{0x40180128,0x00019118},


{0x8000001c,0xea0000ed},////release sco
{0x800003c0,0xe51ff004},
{0x800003c4,0x00020418},
{0x800003c8,0xe51ff004},
{0x800003cc,0x00020544},
{0x800003d0,0xe51ff004},
{0x800003d4,0x00001594},
{0x800003d8,0xe59f0024},
{0x800003dc,0xebfffff7},
{0x800003e0,0xe3a00024},
{0x800003e4,0xe1a01004},
{0x800003e8,0xebfffff8},
{0x800003ec,0xe1a01004},
{0x800003f0,0xe3a00027},
{0x800003f4,0xebfffff5},
{0x800003f8,0xe59f0008},
{0x800003fc,0xebfffff1},
{0x80000400,0xe8bd8010},
{0x80000404,0x80000408},
{0x80000408,0x00000000} ,
{0x4018010c,0x00004a14},
{0x4018012c,0x000312fc},

#if 0


{0x8000000c,0xea00008b},//afh
{0x80000240,0xe59f0014},
{0x80000244,0xe5d00000},
{0x80000248,0xE3500030},
{0x8000024c,0xC8bd8ff8},
{0x80000250,0xe59f0008},
{0x80000254,0xe3a0ea1c},  //mov pc 0x1c0e4
{0x80000258,0xe28ef0e4},
{0x8000025C,0x800016b8},
{0x80000260,0x800016bb},
{0x40180018,0x0001c0e0},
{0x40180038,0x000312ec},


{0x80000000,0xea0023C9},//afh
{0x40180010,0x0001c73c},
{0x40180030,0x000312e0},
{0x80000004,0xea00239A},
{0x40180014,0x0001c620},
{0x40180034,0x000312e4},




{0x80008e00,0xe51ff004},
{0x80008e04,0x0002c72c},
{0x80008e08,0xe51ff004},
{0x80008e0c,0x0001bd20},
{0x80008e10,0xe51ff004},
{0x80008e14,0x0001d884},
{0x80008e18,0xe51ff004},
{0x80008e1c,0x0001db28},
{0x80008e20,0xe51ff004},
{0x80008e24,0x0000860c},
{0x80008e28,0xe51ff004},
{0x80008e2c,0x0001dd7c},
{0x80008e30,0xe51ff004},
{0x80008e34,0x00024ac8},
{0x80008e38,0xe51ff004},
{0x80008e3c,0x0001e3cc},
{0x80008e40,0xe92d4008},
{0x80008e44,0xe3a008c5},
{0x80008e48,0xebffffec},
{0x80008e4c,0xe59f101c},
{0x80008e50,0xe3c00e40},
{0x80008e54,0xe5d11007},
{0x80008e58,0xe2800004},
{0x80008e5c,0xe08101a0},
{0x80008e60,0xe2400064},
{0x80008e64,0xe1a00c00},
{0x80008e68,0xe1a00c40},
{0x80008e6c,0xe8bd8008},
{0x80008e70,0x80002e34},
{0x80008e74,0xe92d4070},
{0x80008e78,0xe59f609c},
{0x80008e7c,0xe1a05000},
{0x80008e80,0xe5d64008},
{0x80008e84,0xe1a00004},
{0x80008e88,0xebffffde},
{0x80008e8c,0xe3500000},
{0x80008e90,0x08bd8070},
{0x80008e94,0xe2041007},
{0x80008e98,0xe59f0080},
{0x80008e9c,0xe3a02001},
{0x80008ea0,0xe7d001a4},
{0x80008ea4,0xe1100112},
{0x80008ea8,0x08bd8070},
{0x80008eac,0xe1d600da},
{0x80008eb0,0xe59f206c},
{0x80008eb4,0xe1d610db},
{0x80008eb8,0xe1d23dd3},
{0x80008ebc,0xe59f2064},
{0x80008ec0,0xe1530000},
{0x80008ec4,0xba000004},
{0x80008ec8,0xe281300a},
{0x80008ecc,0xe1500003},
{0x80008ed0,0xda000004},
{0x80008ed4,0xe3710046},
{0x80008ed8,0xaa000002},
{0x80008edc,0xe7d23004},
{0x80008ee0,0xe2833050},
{0x80008ee4,0xe7c23004},
{0x80008ee8,0xe7d22004},
{0x80008eec,0xe3520070},
{0x80008ef0,0x81a01004},
{0x80008ef4,0x81a00005},
{0x80008ef8,0x88bd4070},
{0x80008efc,0x8affffc3},
{0x80008f00,0xe2811004},
{0x80008f04,0xe1500001},
{0x80008f08,0xb1a01004},
{0x80008f0c,0xb1a00005},
{0x80008f10,0xb8bd4070},
{0x80008f14,0xbaffffbf},
{0x80008f18,0xe8bd8070},
{0x80008f1c,0x80001880},
{0x80008f20,0x8000378c},
{0x80008f24,0x800004c8},
{0x80008f28,0x8000182f},
{0x80008f2c,0xe92d4038},
{0x80008f30,0xe1a04000},
{0x80008f34,0xebffffb9},
{0x80008f38,0xe3500000},
{0x80008f3c,0x0a000021},
{0x80008f40,0xe1a00004},
{0x80008f44,0xebffffb7},
{0x80008f48,0xe3500000},
{0x80008f4c,0x0a00001d},
{0x80008f50,0xe59f5074},
{0x80008f54,0xe5d50000},
{0x80008f58,0xe3500000},
{0x80008f5c,0x1a000019},
{0x80008f60,0xe5d40000},
{0x80008f64,0xe3500000},
{0x80008f68,0x0a000016},
{0x80008f6c,0xebffffaf},
{0x80008f70,0xe3500000},
{0x80008f74,0x1a000013},
{0x80008f78,0xe3a00470},
{0x80008f7c,0xe5900030},
{0x80008f80,0xe1a00900},
{0x80008f84,0xe1a01e20},
{0x80008f88,0xe5d40002},
{0x80008f8c,0xe3500001},
{0x80008f90,0x1a000001},
{0x80008f94,0xe3510010},
{0x80008f98,0x3a000001},
{0x80008f9c,0xe3500000},
{0x80008fa0,0x1a000008},
{0x80008fa4,0xebffffa5},
{0x80008fa8,0xe5c5000a},
{0x80008fac,0xeb000007},
{0x80008fb0,0xe3a00002},
{0x80008fb4,0xebffff9f},
{0x80008fb8,0xe5c50008},
{0x80008fbc,0xe5854004},
{0x80008fc0,0xe3a00001},
{0x80008fc4,0xe5c50000},
{0x80008fc8,0xe8bd8038},
{0x80008fcc,0x80001880},
{0x80008fd0,0xe59f3014},
{0x80008fd4,0xe1d310db},
{0x80008fd8,0xe0811081},
{0x80008fdc,0xe0810000},
{0x80008fe0,0xe1a00140},
{0x80008fe4,0xe5c3000b},
{0x80008fe8,0xe1a0f00e},
{0x80008fec,0x80001880},

#endif

{0x40180000,0x0000ffff},	
};

const unsigned short rdabt_bton_fmoff_init_16_c[][2] = 
{
    {0x003f,0x0001},//page 1
    {0x0025,0x03e1},//
    {0x0026,0x43a5},//set voltage=1.2v
    {0x0028,0x6800},//
    {0x002d,0x00aa},//FM_OFF
    {0x002f,0x1100},//
    {0x0032,0x88f9},//TM=001;DN=1111
    {0x0022,0x0e93},//
    {0x003f,0x0000} //page 0
};

const unsigned short rdabt_bton_fmoff_init_16[][2] = 
{
    {0x003f,0x0001},//page 1
    {0x0025,0x03e1},//
    {0x0026,0x47a5},//set voltage=1.2v
    {0x0028,0x6800},//
    {0x002d,0x00aa},//FM_OFF
    {0x002f,0x1100},//
    {0x0032,0x88f9},//TM=001;DN=1111
    {0x0022,0x0e93},//
    {0x003f,0x0000} //page 0
};

const unsigned short rdabt_bton_fmon_init_16[][2] = 
{
    {0x003f,0x0001},//page 1
    {0x0025,0x07ff},//
    {0x0026,0x23b3},//set voltage=1.2v
    {0x0028,0x6800},//
    {0x002d,0x006a},//FM_OFF
    {0x002f,0x1100},//
    {0x0032,0x88f9},//TM=001;DN=1111
    {0x0022,0x0e93},//
    {0x003f,0x0000} //page 0
};

const unsigned short rdabt_btoff_fmon_init_16[][2] = 
{
    {0x003f,0x0001},//page 1
    {0x0025,0x67e5},//
    {0x0026,0x23a3},//set voltage=1.2v
    {0x0028,0x2800},//
    {0x002d,0x006a},//FM_OFF
    {0x002f,0x2100},//
    {0x0032,0x88f9},//TM=001;DN=1111
    {0x0022,0x0ea3},//
    {0x003f,0x0000} //page 0
};

const unsigned short rdabt_btoff_fmoff_init_16[][2] = 
{
 //   {0x003f,0x0001},//page 1
 //   {0x003f,0x0000} //page 0
};


UINT16 rdabt_dccal_16[][2]=
{
{0x0030,0x0129},
{0x0030,0x012b}
};

void RDABT_5876p_bton_fmoff_patch(void)
{
	unsigned char i=0;
	I2C_Open();
    if(rdabt_revid == 0x01) //5876p verB
    {
    	for(i =0;i<sizeof(rdabt_bton_fmoff_init_16)/sizeof(rdabt_bton_fmoff_init_16[0]); i++)
    	{
    		rdabt_iic_rf_write_data(rdabt_bton_fmoff_init_16[i][0],&rdabt_bton_fmoff_init_16[i][1],1);
    	}
    }
    else//5876p verC
    {
    	for(i =0;i<sizeof(rdabt_bton_fmoff_init_16_c)/sizeof(rdabt_bton_fmoff_init_16_c[0]); i++)
    	{
    		rdabt_iic_rf_write_data(rdabt_bton_fmoff_init_16_c[i][0],&rdabt_bton_fmoff_init_16_c[i][1],1);
    	}
    }
	RDABT_DELAY(10); 
	I2C_Close();
}

void RDABT_5876p_bton_fmon_patch(void)
{
	unsigned char i=0;	
	I2C_Open();
	for(i =0;i<sizeof(rdabt_bton_fmon_init_16)/sizeof(rdabt_bton_fmon_init_16[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_bton_fmon_init_16[i][0],&rdabt_bton_fmon_init_16[i][1],1);
	}
	RDABT_DELAY(10); 
	I2C_Close();	
}

void RDABT_5876p_btoff_fmon_patch(void)
{
	unsigned char i=0;	
	I2C_Open();
	for(i =0;i<sizeof(rdabt_btoff_fmon_init_16)/sizeof(rdabt_btoff_fmon_init_16[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_btoff_fmon_init_16[i][0],&rdabt_btoff_fmon_init_16[i][1],1);
	}
	RDABT_DELAY(10); 
	I2C_Close();	
}

void RDABT_5876p_btoff_fmoff_patch(void)
{
	unsigned char i=0;	
	I2C_Open();
	for(i =0;i<sizeof(rdabt_btoff_fmoff_init_16)/sizeof(rdabt_btoff_fmoff_init_16[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_btoff_fmoff_init_16[i][0],&rdabt_btoff_fmoff_init_16[i][1],1);
	}
	RDABT_DELAY(10); 
	I2C_Close();	
}


void RDABT_phone_Intialization_r16(void)
{
	unsigned char i=0;
	for(i =0;i<sizeof(rdabt_phone_init_16)/sizeof(rdabt_phone_init_16[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_phone_init_16[i][0],&rdabt_phone_init_16[i][1],1);		
	}
}

void Rdabt_Pskey_Write_rf_r16(void)
{
   UINT32 i;
   if(rdabt_revid == 0x01) //5876p verB
   {
       for(i=0;i<sizeof(rdabt_pskey_rf_16)/sizeof(rdabt_pskey_rf_16[0]);i++)
       {
          rdabt_wirte_memory(rdabt_pskey_rf_16[i][0],&rdabt_pskey_rf_16[i][1],1,0);
          RDABT_DELAY(1);
       }
   }
   else //5876p verC
   {
       for(i=0;i<sizeof(rdabt_pskey_rf_16_c)/sizeof(rdabt_pskey_rf_16_c[0]);i++)
       {
          rdabt_wirte_memory(rdabt_pskey_rf_16_c[i][0],&rdabt_pskey_rf_16_c[i][1],1,0);
          RDABT_DELAY(1);
       }
   }
   RDABT_DELAY(10);	
}


void Rdabt_Pskey_Write_r16(void)
{
   UINT32 i;
   for(i=0;i<sizeof(rda_pskey_16)/sizeof(rda_pskey_16[0]);i++)
   {
      rdabt_wirte_memory(rda_pskey_16[i][0],&rda_pskey_16[i][1],1,0);
      RDABT_DELAY(1);
   }
	
}

   
/*void rdabt_set_filter_r16(void)
{
   UINT16 num_send;	
   rdabt_adp_uart_tx(rda_filter_16,sizeof(rda_filter_16),&num_send);
   RDABT_DELAY(1);
}*/


void Rdabt_trap_write_r16(void)
{
   UINT32 i;
   for(i=0;i<sizeof(rda_trap_16)/sizeof(rda_trap_16[0]);i++)
   {
      rdabt_wirte_memory(rda_trap_16[i][0],&rda_trap_16[i][1],1,0);
      RDABT_DELAY(1);
   }	
}




void  RDABT_rf_Intialization_r16(void)
{
	unsigned char i=0;
    if(rdabt_revid == 0x01) //5876p verB
    {
    	for(i =0;i<sizeof(rdabt_rf_init_16)/sizeof(rdabt_rf_init_16[0]); i++){
    		rdabt_iic_rf_write_data(rdabt_rf_init_16[i][0],&rdabt_rf_init_16[i][1],1);		
    	}	
    }
    else //5876p verC
    {
    	for(i =0;i<sizeof(rdabt_rf_init_16_c)/sizeof(rdabt_rf_init_16_c[0]); i++){
    		rdabt_iic_rf_write_data(rdabt_rf_init_16_c[i][0],&rdabt_rf_init_16_c[i][1],1);		
    	}
    }
	RDABT_DELAY(10);  
}


void rdabt_DC_write_r16(void)
{
   UINT32 i;
   for(i=0;i<sizeof(rdabt_dccal_16)/sizeof(rdabt_dccal_16[0]);i++)
   {
      rdabt_iic_rf_write_data(rdabt_dccal_16[i][0],&rdabt_dccal_16[i][1],1);
      RDABT_DELAY(2);
   }	
}


void RDABT_core_Intialization_r16(void)
{
#ifdef RDA_BT_debug
    kal_prompt_trace(0,"RDABT_core_Intialization_r16");
#endif    
    rda_bt_pin_to_low();
    rda_bt_ldoon_toggle_high();    
    rda_bt_reset_to_high();
    RDABT_DELAY(10); //wait for digi running
    RDABT_rf_Intialization_r16();
    Rdabt_Pskey_Write_rf_r16();
    rdabt_DC_write_r16();
    Rdabt_Pskey_Write_r16();
    //rdabt_set_filter_r16();
    Rdabt_trap_write_r16();
}

#endif// __RDA_CHIP_R16_5876p__

#endif

