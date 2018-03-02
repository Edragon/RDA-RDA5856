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
#ifdef __RDA_CHIP_R17_8809__

const unsigned short rdabt_phone_init_17[][2] = 
{
#if 0
{0x3F, 0x0001}, // page 1
{0x32, 0x0009}, // Init UART IO status
#endif
{0x3F, 0x0000}, // page 0
};

const unsigned short rdabt_rf_init_17[][2] = 
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


const uint32 rdabt_pskey_rf_17[][2] =
{
#if 1
// item:RDA8809BT_B_digi_20120512
{0x40240000, 0x0004f39c}, // SPI2_CLK_EN PCLK_SPI2_EN
{0x800000C0, 0x0000000b}, // CHIP_PS PSKEY: Total number -----------------
{0x800000C4, 0x003f0000}, // PSKEY: Page 0
{0x800000C8, 0x0041000b}, // TX_DC_OFFSET+/_
{0x800000CC, 0x00430fc6}, // 43H=BFC6
{0x800000D0, 0x0044048f}, // 44H=0580;
{0x800000D4, 0x00694075}, // TX_DC_OFFSET+/_
{0x800000D8, 0x006b10c0}, // 
{0x800000DC, 0x003f0001}, // PSKEY: Page 1  
{0x800000E0, 0x00453000}, // 
{0x800000E4, 0x0047f13b}, // 20120512
{0x800000E8, 0x00490008}, // 
{0x800000EC, 0x003f0000}, // PSKEY: Page 0
{0x80000040, 0x10000000}, // PSKEY: Flage
{0x40240000, 0x0000f29c}, // Disable SPI2_CLK_EN PCLK_SPI2_EN
#else
// for EDR Guard Interval Test
// item:RDA8809BT_B_digi_20120512
{0x40240000, 0x0004f39c}, // SPI2_CLK_EN PCLK_SPI2_EN
{0x800000C0, 0x0000000C}, // CHIP_PS PSKEY: Total number -----------------
{0x800000C4, 0x003f0000}, // PSKEY: Page 0
{0x800000C8, 0x0041000b}, // TX_DC_OFFSET+/_
{0x800000CC, 0x00430fc6}, // 43H=BFC6
{0x800000D0, 0x0044048f}, // 44H=0580;
{0x800000D4, 0x004A0800},////Min_error_th
{0x800000D8, 0x00694075}, // TX_DC_OFFSET+/_                   
{0x800000DC, 0x006b10c0}, //                                   
{0x800000E0, 0x003f0001}, // PSKEY: Page 1                     
{0x800000E4, 0x00453000}, //                                   
//{0x800000E8, 0x0047f13b}, // 20120512        guard time 4.7us
{0x800000E8, 0x0047f1fb}, // 20130507   guard time 5.05us                                       
{0x800000EC, 0x00490008}, //                                   
{0x800000F0,0x003f0000}, // PSKEY: Page 0                     
{0x80000040, 0x10000000}, // PSKEY: Flage
{0x40240000, 0x0000f29c}, // Disable SPI2_CLK_EN PCLK_SPI2_EN

#endif
};


const uint16 rdabt_dccal_17[][2]=
{
// item:rda8809bt_verB_DC_CAL
{0x30, 0x0129}, // 
{0x30, 0x012b}, // 
};


const uint32 rda_pskey_17[][2] =
{
{0x800000a8,0x0cbaba30},//min power level 0x0cbfbf30->0x0cbaba30
#ifdef __BT_PCM_OVER_UART__
{0x80000070,0x00002080},
#else
{0x80000070,0x00002000},
#endif
{0x80000074,0xa5025010},
{0x80000078,0x0f054000},
{0x8000007c,0xb530b530},
//{0x80000084,0x9098c007},
#ifdef USE_VOC_CVSD
{0x80000084,0x9098c008},//csvd
#endif
//{0x80000064,0x001C2000},//new baudrate 3200000
{0x80000064,BT_UART_BAUDRATE},//new baudrate 3200000
#ifdef USE_VOC_CVSD
{0x80000040,0x0402f200},//PSKEY: CVSD
#else
{0x80000040,0x0400f200},//PSKEY: modify flag
#endif

#ifdef __RDABT_DISABLE_EDR__

{0x800004f0,0xf88dffff}, ///disable edr
{0x800004f4,0x83713818}, ///disable 3m esco ev4 ev5

#else // !__RDABT_DISABLE_EDR__

{0x800004f0,0xfe8dffff}, ///enable edr
{0x800004f4,0x83713998}, ///disable 3m esco ev4 ev5

#endif // !__RDABT_DISABLE_EDR__

#ifdef BT_UART_BREAK_INT_WAKEUP
{0x800000a4,0x0f20280a}, // use uart tx to wake host	
{0x80000040,0x02000000},
{0x40200010,0x00007fff}, // set hostwake gpio to input
#else
//host wakeup
{0x40200010,0x00007f7f},
#endif
#ifdef USE_VOC_CVSD
{0x80000550,0x003c03fd}, // set buffer size
{0x80000554,0x000f0006},
{0x80000558,0x003c03fd},
{0x8000055c,0x000f0004},
{0x80000560,0x0001003c},
#else	
{0x80000550,0x007803fd}, // set buffer size
{0x80000554,0x00080006},
{0x80000558,0x007803fd},
{0x8000055c,0x00080004},
{0x80000560,0x00010078},
#endif
//CoolSand Only CTS-->GPIO2
//{0x40200044,0x0000003c}, //;
};

const uint32 rda_trap_17[][2] = 
{
	{0x40180004,0x0001b0cc},												
	{0x40180024,0x0001d7bc},												
	{0x40180008,0x00002e2c},												
	{0x40180028,0x00000014},	
	{0x4018000c,0x00014cb0}, 
	{0x4018002c,0x00000014},
	{0x40180010,0x00018f1c}, 
	{0x40180030,0x0001a1ac},
	{0x40180014,0x0001669c}, // ESCO link connect by headset 20130131
	{0x40180034,0x00017928},
//	{0x40180014,0x00018f30 },//seconed security zhou siyou 20121016
//	{0x40180024,0x00000014},
  {0x40180018,0x00013e40}, // for a2dp audio, fix 3M packet
  {0x40180038,0x0000a2f0},
  {0x4018001c,0x00012bb4}, //edr
  {0x4018003c,0x00000014},
  {0x40180020,0x00014fc4},//w2
  {0x40180040,0x0002c328},

{0x80000010,0xea000062},//ltaddr
{0x800001a0,0x05d41027},  
{0x800001a4,0x00611181}, 
{0x800001a8,0x00811004}, 
{0x800001ac,0x05d1102a}, 
{0x800001b0,0x01510000}, 
{0x800001b4,0xe3a00bae}, 
{0x800001b8,0x1280fff6}, 
{0x800001bc,0x0280ffd6}, 
{0x40180100,0x0002bb54},
{0x40180120,0x0002ee20},

{0x80000350,0xe51ff004}, //pause enc 
{0x80000354,0x0000270c},
{0x80000358,0xe92d4010},
{0x8000035c,0xebfffffb},
{0x80000360,0xe3a01000},
{0x80000364,0xe5c01054},
{0x80000368,0xe8bd8010},
{0x80000014,0xea0000d4},
{0x8000036c,0x03a00c74},
{0x80000370,0x0280f074},
{0x80000374,0xe3550012},
{0x80000378,0x059f000c},
{0x8000037c,0x05860004},
{0x80000380,0xe3a00c74},
{0x80000384,0x0280f0e0},
{0x80000388,0xe280f094},
{0x8000038c,0x80000358},           
{0x40180104,0x00007470},
{0x40180124,0x0002ee24},

{0x40180108,0x00014bf4},///2ev3-ev3
{0x40180128,0x00018c7c},


{0x8000001c,0xea0000ed},////release sco
{0x800003c0,0xe51ff004},
{0x800003c4,0x0001fa54},
{0x800003c8,0xe51ff004},
{0x800003cc,0x0001fb80},
{0x800003d0,0xe51ff004},
{0x800003d4,0x00001590},
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
{0x4018010c,0x00004578},
{0x4018012c,0x0002ee2c},


{0x80000020,0xea00006c},	////packet length to real length in queues
{0x800001d8,0xe3530024},	//cmp r3 0x24
{0x800001dc,0x13a03c17},	//movne r3,0x1700
{0x800001e0,0x11dc11ba},	//LDRHne     r1,[r12,#0x1a]
{0x800001e4,0x1283f0e8},	//addne pc,r3,0xe8
{0x800001e8,0xe3a01c18},	//mov r1,0x1800
{0x800001ec,0xe281f044},	//add pc,r1,0x44
{0x40180110,0x000017e4},
{0x40180130,0x0002ee30},

{0x80000018,0xea0000b8}, // update rssi
{0x80000300,0xe59f3014},
{0x80000304,0xe1a01000},
{0x80000308,0xe20000ff},
{0x8000030c,0xe2800c55},
{0x80000310,0xe5830000},
{0x80000314,0xe3a00a1b},
{0x80000318,0xe280ffee},
{0x8000031c,0x8000043c},
{0x8000043c,0x00000000},
{0x40180118,0x0001b3b4},
{0x40180138,0x0002ee28},

{0x40180000,0x0000ffff},	
};



const UINT32 rda_trap_calib_17[][2] = 
{
{0x80000000,0xea001a86}, //   b  0x80006a20
{0x40180004,0x000200b4}, //
{0x40180024,0x0002ee10}, //

{0x80000004,0xea00003e}, //
{0x80000100,0xe59f0024}, //   ldr r0=80006c28
{0x80000104,0xe5900000}, //  ldr r0,[r0,0]
{0x80000108,0xe3500000}, //  cmp r0,0
{0x8000010c,0x05d48004}, //    ....    LDReqB     r8,[r4,#4]
{0x80000110,0x03a00b79}, // moveq r0,0x1e400
{0x80000114,0x0280ffb2}, // addeq pc,r0,0x2c8
{0x80000118,0xe59f0010}, // ldr r0=0x70000030
{0x8000011c,0xe5901000}, // ldr r1,[r0,0]
{0x80000120,0xe3811a20}, // r1,r1,#0x20000
{0x80000124,0xe5801000}, //  str r1,[r0,0]
{0x80000128,0xe8bd83f8}, //    ....    LDMFD    r13!,{r3-r9,pc}
{0x8000012c,0x80006c28}, //
{0x80000130,0x70000030}, //

{0x40180008,0x0001e6c4}, //
{0x40180028,0x0002ee14}, //

{0x80006a00,0xe51ff004}, //    ....    LDR      pc,0x80006a04
{0x80006a04,0x0000321c}, //    d4..    DCD    13412
{0x80006a08,0xe51ff004}, //    ....    LDR      pc,0x80006a0c
{0x80006a0c,0x00000c20}, //    ....    DCD    3228
{0x80006a10,0xe51ff004}, //    ....    LDR      pc,0x80006a14
{0x80006a14,0x00029368}, //    T...    DCD    182100
{0x80006a18,0xe51ff004}, //    ....    LDR      pc,0x80006a1c
{0x80006a1c,0x0001d74c}, //    ....    DCD    127496
{0x80006a20,0xe92d4ff0}, //    .O-.    STMFD    r13!,{r4-r11,r14}
{0x80006a24,0xe5d05000}, //    .P..    LDRB     r5,[r0,#0]
{0x80006a28,0xe1a04000}, //    .@..    MOV      r4,r0
{0x80006a2c,0xe5d00001}, //    ....    LDRB     r0,[r0,#1]
{0x80006a30,0xe24dd01c}, //    ..M.    SUB      r13,r13,#0x1c
{0x80006a34,0xe58d0018}, //    ....    STR      r0,[r13,#0x18]
{0x80006a38,0xe2840002}, //    ....    ADD      r0,r4,#2
{0x80006a3c,0xebffffef}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80006a00
{0x80006a40,0xe1a06000}, //    .`..    MOV      r6,r0
{0x80006a44,0xe2840006}, //    ....    ADD      r0,r4,#6
{0x80006a48,0xebffffec}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80006a00
{0x80006a4c,0xe58d0014}, //    ....    STR      r0,[r13,#0x14]
{0x80006a50,0xe284000a}, //    ....    ADD      r0,r4,#0xa
{0x80006a54,0xebffffe9}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80006a00
{0x80006a58,0xe58d0010}, //    ....    STR      r0,[r13,#0x10]
{0x80006a5c,0xe5d4000e}, //    ....    LDRB     r0,[r4,#0xe]
{0x80006a60,0xe3550000}, //    ..U.    CMP      r5,#0
{0x80006a64,0xe58d000c}, //    ....    STR      r0,[r13,#0xc]
{0x80006a68,0xe5d4000f}, //    ....    LDRB     r0,[r4,#0xf]
{0x80006a6c,0xe58d0008}, //    ....    STR      r0,[r13,#8]
{0x80006a70,0xe5d42010}, //    . ..    LDRB     r2,[r4,#0x10]
{0x80006a74,0xe58d2004}, //    . ..    STR      r2,[r13,#4]
{0x80006a78,0xe5d48011}, //    ....    LDRB     r8,[r4,#0x11]
{0x80006a7c,0xe5d49012}, //    ....    LDRB     r9,[r4,#0x12]
{0x80006a80,0xe5d4a013}, //    ....    LDRB     r10,[r4,#0x13]
{0x80006a84,0xe5d47014}, //    .p..    LDRB     r7,[r4,#0x14]
{0x80006a88,0xe5d41015}, //    ....    LDRB     r1,[r4,#0x15]
{0x80006a8c,0xe5d40016}, //    ....    LDRB     r0,[r4,#0x16]
{0x80006a90,0xe0810400}, //    ....    ADD      r0,r1,r0,LSL #8
{0x80006a94,0xe3c04b40}, //    @K..    BIC      r4,r0,#0x10000
{0x80006a98,0xe59f0178}, //    x...    LDR      r0,0x80006c18
{0x80006a9c,0x0a00004c}, //    L...    BEQ      0x80006bd4
{0x80006aa0,0xe3a01001}, //    ....    MOV      r1,#1
{0x80006aa4,0xe3560000}, //    ..V.    CMP      r6,#0
{0x80006aa8,0xe5801000}, //    ....    STR      r1,[r0,#0]
{0x80006aac,0x128d1010}, //    ....    ADDNE    r1,r13,#0x10
{0x80006ab0,0x11a00006}, //    ....    MOVNE    r0,r6
{0x80006ab4,0x128d2014}, //    . ..    ADDNE    r2,r13,#0x14
{0x80006ab8,0xe3a05000}, //    .P..    MOV      r5,#0
{0x80006abc,0xe3a0b000}, //    ....    MOV      r11,#0
{0x80006ac0,0x1bffffd0}, //    ....    BLNE     $Ven$AA$L$$BTaddr_Build_Sync_Word  ; 0x80006a08
{0x80006ac4,0xe59d1014}, //    ....    LDR      r1,[r13,#0x14]
{0x80006ac8,0xe3a00470}, //    p...    MOV      r0,#0x70000000
{0x80006acc,0xe5801008}, //    ....    STR      r1,[r0,#8]
{0x80006ad0,0xe59d1010}, //    ....    LDR      r1,[r13,#0x10]
{0x80006ad4,0xe580100c}, //    ....    STR      r1,[r0,#0xc]
{0x80006ad8,0xe59d2004}, //    . ..    LDR      r2,[r13,#4]
{0x80006adc,0xe20a1007}, //    ....    AND      r1,r10,#7
{0x80006ae0,0xe3a034ff}, //    .4..    MOV      r3,#0xff000000
{0x80006ae4,0xe0032c02}, //    .,..    AND      r2,r3,r2,LSL #24
{0x80006ae8,0xe1821001}, //    ....    ORR      r1,r2,r1
{0x80006aec,0xe3580000}, //    ..X.    CMP      r8,#0
{0x80006af0,0x13a029fc}, //    .)..    MOVNE    r2,#0x3f0000
{0x80006af4,0x10022809}, //    .(..    ANDNE    r2,r2,r9,LSL #16
{0x80006af8,0x13825480}, //    .T..    ORRNE    r5,r2,#0x80000000
{0x80006afc,0xe1a08000}, //    ....    MOV      r8,r0
{0x80006b00,0xe5801020}, //     ...    STR      r1,[r0,#0x20]
{0x80006b04,0xe3a0a480}, //    ....    MOV      r10,#0x80000000
{0x80006b08,0xe3a06000}, //    .`..    MOV      r6,#0
{0x80006b0c,0xe5885024}, //    $P..    STR      r5,[r8,#0x24]
{0x80006b10,0xe3a00000}, //    ....    MOV      r0,#0
{0x80006b14,0xe1500004}, //    ..P.    CMP      r0,r4
{0x80006b18,0x359f10fc}, //    ...5    LDRCC    r1,0x80006c1c
{0x80006b1c,0x37c17000}, //    .p.7    STRCCB   r7,[r1,r0]
{0x80006b20,0x32800001}, //    ...2    ADDCC    r0,r0,#1
{0x80006b24,0x33c00b40}, //    @..3    BICCC    r0,r0,#0x10000
{0x80006b28,0x3afffff9}, //    ...:    BCC      0x80006b14
{0x80006b2c,0xe59d0018}, //    ....    LDR      r0,[r13,#0x18]
{0x80006b30,0xe2800cc0}, //    ....    ADD      r0,r0,#0xc000
{0x80006b34,0xe2800dc6}, //    ....    ADD      r0,r0,#0x3180
{0x80006b38,0xe3800b80}, //    ....    ORR      r0,r0,#0x20000
{0x80006b3c,0xebffffb3}, //    ....    BL       $Ven$AA$L$$_HWradio_ProgNow  ; 0x80006a10
{0x80006b40,0xe59d000c}, //    ....    LDR      r0,[r13,#0xc]
{0x80006b44,0xe3500000}, //    ..P.    CMP      r0,#0
{0x80006b48,0x05886034}, //    4`..    STREQ    r6,[r8,#0x34]
{0x80006b4c,0x1588a034}, //    4...    STRNE    r10,[r8,#0x34]
{0x80006b50,0xe1a0152b}, //    +...    MOV      r1,r11,LSR #10
{0x80006b54,0xe1a01501}, //    ....    MOV      r1,r1,LSL #10
{0x80006b58,0xe3c40cfc}, //    ....    BIC      r0,r4,#0xfc00
{0x80006b5c,0xe1800001}, //    ....    ORR      r0,r0,r1
{0x80006b60,0xe59d1008}, //    ....    LDR      r1,[r13,#8]
{0x80006b64,0xe3a02df0}, //    .-..    MOV      r2,#0x3c00
{0x80006b68,0xe0021501}, //    ....    AND      r1,r2,r1,LSL #10
{0x80006b6c,0xe3c00df0}, //    ....    BIC      r0,r0,#0x3c00
{0x80006b70,0xe1810000}, //    ....    ORR      r0,r1,r0
{0x80006b74,0xe3c00a60}, //    `...    BIC      r0,r0,#0x60000
{0x80006b78,0xe3800b80}, //    ....    ORR      r0,r0,#0x20000
{0x80006b7c,0xe5880030}, //    0...    STR      r0,[r8,#0x30]
{0x80006b80,0xe59f0098}, //    ....    LDR      r0,0x80006c20
{0x80006b84,0xe5d01001}, //    ....    LDRB     r1,[r0,#1]
{0x80006b88,0xe5d02008}, //    . ..    LDRB     r2,[r0,#8]
{0x80006b8c,0xe26110dd}, //    ..a.    RSB      r1,r1,#0xdd
{0x80006b90,0xe2811e40}, //    @...    ADD      r1,r1,#0x400
{0x80006b94,0xe2622f9b}, //    ./b.    RSB      r2,r2,#0x26c
{0x80006b98,0xe1811802}, //    ....    ORR      r1,r1,r2,LSL #16
{0x80006b9c,0xe5881140}, //    @...    STR      r1,[r8,#0x140]
{0x80006ba0,0xe5d01002}, //    ....    LDRB     r1,[r0,#2]
{0x80006ba4,0xe5d00009}, //    ....    LDRB     r0,[r0,#9]
{0x80006ba8,0xe2611f90}, //    ..a.    RSB      r1,r1,#0x240
{0x80006bac,0xe26000be}, //    ..`.    RSB      r0,r0,#0xbe
{0x80006bb0,0xe2800e40}, //    @...    ADD      r0,r0,#0x400
{0x80006bb4,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x80006bb8,0xe1810800}, //    ....    ORR      r0,r1,r0,LSL #16
{0x80006bbc,0xe5880158}, //    X...    STR      r0,[r8,#0x158]
{0x80006bc0,0xe5886170}, //    pa..    STR      r6,[r8,#0x170]
{0x80006bc4,0xe59f0058}, //    X...    LDR      r0,0x80006c24
{0x80006bc8,0xe5880028}, //    (...    STR      r0,[r8,#0x28]
{0x80006bcc,0xe28dd01c}, //    ....    ADD      r13,r13,#0x1c
{0x80006bd0,0xe8bd8ff0}, //    ....    LDMFD    r13!,{r4-r11,pc}
{0x80006bd4,0xe3a01000}, //    ....    MOV      r1,#0
{0x80006bd8,0xe3a02470}, //    p$..    MOV      r2,#0x70000000
{0x80006bdc,0xe5821030}, //    0...    STR      r1,[r2,#0x30]
{0x80006be0,0xe3a06000}, //    .`..    MOV      r6,#0
{0x80006be4,0xe5806000}, //    .`..    STR      r6,[r0,#0]
{0x80006be8,0xe59f0030}, //    0...    LDR      r0,0x80006c20
{0x80006bec,0xe5d01002}, //    ....    LDRB     r1,[r0,#2]
{0x80006bf0,0xe5d00009}, //    ....    LDRB     r0,[r0,#9]
{0x80006bf4,0xe2611f90}, //    ..a.    RSB      r1,r1,#0x240
{0x80006bf8,0xe26000be}, //    ..`.    RSB      r0,r0,#0xbe
{0x80006bfc,0xe2800e40}, //    @...    ADD      r0,r0,#0x400
{0x80006c00,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x80006c04,0xe1810800}, //    ....    ORR      r0,r1,r0,LSL #16
{0x80006c08,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x80006c0c,0xe5820158}, //    X...    STR      r0,[r2,#0x158]
{0x80006c10,0xebffff80}, //    ....    BL       $Ven$AA$L$$LSLCirq_R2P_Turn_On_Intr  ; 0x80006a18
{0x80006c14,0xeaffffec}, //    ....    B        0x80006bcc
{0x80006c18,0x80006c28}, //    (l..    DCD    2147511336
{0x80006c1c,0x70000200}, //    ...p    DCD    1879048704
{0x80006c20,0x80002cf8}, //    ....    DCD    2147521944
{0x80006c24,0x0000f3fb}, //    ....    DCD    62459

{0x80006c28,0x00000000}, //

{0x40180000,0x00000003}, //  ;
};

void RDABT_phone_Intialization_r17(void)
{
	unsigned char i=0;
	for(i =0;i<sizeof(rdabt_phone_init_17)/sizeof(rdabt_phone_init_17[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_phone_init_17[i][0],&rdabt_phone_init_17[i][1],1);		
	}
}

void  RDABT_rf_Intialization_r17(void)
{
	unsigned char i=0;
	for(i=0;i<sizeof(rdabt_rf_init_17)/sizeof(rdabt_rf_init_17[0]); i++)
	{
		rdabt_iic_rf_write_data(rdabt_rf_init_17[i][0],&rdabt_rf_init_17[i][1],1);
	}
	RDABT_DELAY(10);
#if 0
    UINT16 data;
	for(i=0;i<sizeof(rdabt_rf_init_17)/sizeof(rdabt_rf_init_17[0]); i++)
	{
        if (rdabt_rf_init_17[i][0] == 0x3f)
        {
            rdabt_iic_rf_write_data(rdabt_rf_init_17[i][0],&rdabt_rf_init_17[i][1],1);
        }
		rdabt_iic_rf_read_data(rdabt_rf_init_17[i][0],&data,1);
        sxs_fprintf((15<<5)|(1<<21), "RFInit 0x%02x = 0x%04x", rdabt_rf_init_17[i][0], data);
	}
#endif
}

void Rdabt_Pskey_Write_rf_r17(void)
{
   int32 i;
   for(i=0;i<sizeof(rdabt_pskey_rf_17)/sizeof(rdabt_pskey_rf_17[0]);i++)
   {

      rdabt_wirte_memory(rdabt_pskey_rf_17[i][0],&rdabt_pskey_rf_17[i][1],1,0);
      RDABT_DELAY(2);
   }
#if 0
   unsigned int data;
   for(i=0;i<sizeof(rdabt_pskey_rf_17)/sizeof(rdabt_pskey_rf_17[0]);i++)
   {

      rdabt_iic_core_read_data(rdabt_pskey_rf_17[i][0],&data,1);
      sxs_fprintf((15<<5)|(1<<21), "PSKey 0x%08x = 0x%08x", rdabt_pskey_rf_17[i][0], data);
   }
#endif
}

void rdabt_DC_write_r17(void)
{
   int32 i;
   for(i=0;i<sizeof(rdabt_dccal_17)/sizeof(rdabt_dccal_17[0]);i++)
   {

      rdabt_iic_rf_write_data(rdabt_dccal_17[i][0],&rdabt_dccal_17[i][1],1);
      RDABT_DELAY(2);
   }
   RDABT_DELAY(10);
#if 0
   UINT16 data;
   for(i=0;i<sizeof(rdabt_dccal_17)/sizeof(rdabt_dccal_17[0]);i++)
   {

      rdabt_iic_rf_read_data(rdabt_dccal_17[i][0],&data,1);
      sxs_fprintf((15<<5)|(1<<21), "DC 0x%02x = 0x%04x", rdabt_dccal_17[i][0], data);
   }
#endif
}

void Rdabt_Pskey_Write_r17(void)
{
   int32 i;
   for(i=0;i<sizeof(rda_pskey_17)/sizeof(rda_pskey_17[0]);i++)
   {
      rdabt_wirte_memory(rda_pskey_17[i][0],&rda_pskey_17[i][1],1,0);
      RDABT_DELAY(1);
   }
	
}

void Rdabt_trap_write_r17(void)
{
   extern BOOL  g_CalbClock;
   int32 i;
   
   if(g_CalbClock)
   {
       hal_HstSendEvent(1, 0x88888888);
       for(i=0;i<sizeof(rda_trap_calib_17)/sizeof(rda_trap_calib_17[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_calib_17[i][0],&rda_trap_calib_17[i][1],1,0x0);
          RDABT_DELAY(1);
       }	
   }
   else
   {
       for(i=0;i<sizeof(rda_trap_17)/sizeof(rda_trap_17[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_17[i][0],&rda_trap_17[i][1],1,0);
          RDABT_DELAY(1);
       }	
  }
}

void RDABT_core_Intialization_r17(void)
{
#ifdef RDA_BT_debug
    kal_prompt_trace(0,"RDABT_core_Intialization_r17");
#endif

    RDABT_rf_Intialization_r17();
    Rdabt_Pskey_Write_rf_r17();
    rdabt_DC_write_r17();
    Rdabt_Pskey_Write_r17();

    Rdabt_trap_write_r17();
}

#endif // __RDA_CHIP_R17_8809__

#endif // __BT_RDABT__
 

