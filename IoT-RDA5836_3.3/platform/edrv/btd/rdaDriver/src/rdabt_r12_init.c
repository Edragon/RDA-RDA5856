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

#include "rdabt_drv.h"

//sync btdV3.45
#if defined( __RDA_CHIP_R12_5876__) || defined(__RDA_CHIP_R12_5870P__) || defined(__RDA_CHIP_R12_5876M__)



//sync btdV3.45
const UINT32 rda_pskey_12[][2] =
{
{0x800000a8,0x0Bbaba30},//min power level

#ifdef __BT_PCM_OVER_UART__
{0x80000070,0x00002080},
#else
{0x80000070,0x00002000},//zhou siyou esco->soc 0x00002000->0x00002008
#endif
{0x80000074,0xa5025010},
{0x80000078,0x0f054000},
{0x8000007c,0xb530b530},
{0x80000064,BT_UART_BAUDRATE},//new baudrate 460800
#ifdef USE_VOC_CVSD
{0x80000084,0x9098c008},//csvd
{0x80000040,0x0402f200},
#else
{0x80000040,0x0400f200},//PSKEY: modify flag
#endif



{0x40200044,0x0000003c}, //;  	


#ifdef __RDABT_DISABLE_EDR__
{0x800004ec,0xf88dffff}, ///disable edr
#ifdef __RDABT_ENABLE_SP__
{0x800004f0,0x83791998}, ///disable 3m esco ev4 ev5 20120814 zhou siyou  ->0x83711f98->0x83711b98  remove noise
#else
{0x800004f0,0x83711998}, ///disable 3m esco ev4 ev5  20120814 zhou siyou  ->0x83711f98->0x83711b98  remove noise
#endif

#else  //__RDABT_DISABLE_EDR__

{0x800004ec,0xfe8dffff}, ///enable edr
#ifdef __RDABT_ENABLE_SP__
{0x800004f0,0x83791998}, ///disable 3m esco ev4 ev5 0x83793998
#else
{0x800004f0,0x83711998}, ///disable 3m esco ev4 ev5 0x83713998
#endif

#endif

{0x8000054c,0x003c03fd}, // set buffer size
{0x80000550,0x000f0006},
{0x80000554,0x003c03fd},
{0x80000558,0x000f0004},
{0x8000055c,0x0001003c},
{0x80000560,0x00000003},

};



const UINT32 rda_trap_12[][2] = 
{

{0x40180100,0x000068b0},//inc power
{0x40180120,0x000068f4},
{0x40180104,0x000066b0},//dec power
{0x40180124,0x000068f4},
{0x40180108,0x0000f8c4},///all rxon
{0x40180128,0x00026948},
{0x4018010c,0x000142c4},///ensure high speed packet
{0x4018012c,0x00024644},
{0x40180110,0x00015440},//w2
{0x40180130,0x0002f524},
{0x40180114,0x00013038},
{0x40180134,0x00000014},
{0x40180118,0x0002d04c},//connection patch   
{0x40180138,0x00000014},                     
{0x4018011c,0x0001a338},//encrpyt            
{0x4018013c,0x0001ac1c},      

#if 0
{0x8000000c,0xea0000cb}, // update rssi
{0x80000340,0xe59f3014},
{0x80000344,0xe1a01000},
{0x80000348,0xe20000ff},
{0x8000034c,0xe2800c55},
{0x80000350,0xe5830000},
{0x80000354,0xe3a00a1d},
{0x80000358,0xe280f02c},
{0x8000035c,0x80000400},
{0x80000400,0x00000000},
                
{0x4018001c,0x0001d028},
{0x4018003c,0x00032d20},
#endif





{0x80000014,0xea000055},//name req
{0x80000170,0xe59f0024},
{0x80000174,0xe5801000},
{0x80000178,0xe3a00002},
{0x8000017c,0xe59f100c},
{0x80000180,0xe1a02004},
{0x80000184,0xe3a03001},
{0x80000188,0xe59fe004},
{0x8000018c,0xe59ff004},
{0x80000190,0x800001a0},
{0x80000194,0x0000b6ec},
{0x80000198,0x0001bab8},
{0x8000019c,0x800001b4},
{0x800001a0,0xe59f1004},
{0x800001a4,0xe5911000},
{0x800001a8,0xe59ff008},
{0x800001ac,0x800001b4},
{0x800001b0,0x00030648},
{0x800001b4,0x00000000},
{0x800001b8,0x00030648},
{0x40180004,0x0000b6e8},
{0x40180024,0x00032d28},

{0x80000018,0xea000078},//   sniff in sco           
{0x80000200,0x05d41027},
{0x80000204,0x00611181},
{0x80000208,0x00811004},
{0x8000020c,0x05d1102a},
{0x80000210,0x01510000},
{0x80000214,0xe3a00bbb},
{0x80000218,0x1280ff79},
{0x8000021c,0x0280ff59},
{0x40180008,0x0002ed60},
{0x40180028,0x00032d2c},


// for weixin and xiaomi dead
{0x8000001c,0xea000041},
{0x80000110,0xe51ff004},
{0x80000114,0x00021bbc}, 
{0x80000118,0xe51ff004},
{0x8000011c,0x00021ce8}, 
{0x80000120,0xe51ff004},
{0x80000124,0x00001614}, 
{0x80000128,0xe59f0024}, 
{0x8000012c,0xebfffff7}, 
{0x80000130,0xe3a00024}, 
{0x80000134,0xe1a01004}, 
{0x80000138,0xebfffff8}, 
{0x8000013c,0xe1a01004}, 
{0x80000140,0xe3a00027}, 
{0x80000144,0xebfffff5}, 
{0x80000148,0xe59f0008}, 
{0x8000014c,0xebfffff1}, 
{0x80000150,0xe8bd8010}, 
{0x80000154,0x80000408},
{0x80000158,0x00000000},
{0x4018000c,0x00004764},
{0x4018002c,0x00032d30},





{0x80000010,0xea000070},   ////packet length to real length in queues
{0x800001d8,0xe3530024},//cmp r3 0x24                      
{0x800001dc,0x13a03c18},  //movne r3,0x1800                
{0x800001e0,0x11dc11ba},   //LDRHne     r1,[r12,#0x1a]     
{0x800001e4,0x1283f06c},    //addne pc,r3,0x6c             
{0x800001e8,0xe3a01c18},   //mov r1,0x1800                 
{0x800001ec,0xe281f0c8},  //add pc,r1,0xc8                
{0x4018001c,0x00001868},                                  
{0x4018003c,0x00032d24},



{0x8000000c,0xea00008b},//afh
{0x80000240,0xe59f0014},
{0x80000244,0xe5d00000},
{0x80000248,0xE3500030},
{0x8000024c,0xC8bd8ff8},
{0x80000250,0xe59f0008},
{0x80000254,0xe3a0eb76},  //mov pc 0x1d890
{0x80000258,0xe28ef090},
{0x8000025C,0x8000182c},
{0x80000260,0x8000182f},
{0x40180018,0x0001d88c},
{0x40180038,0x00032d20},



{0x80000000,0xea0023C9},//afh
{0x40180010,0x0001dee8},
{0x40180030,0x00032d14},
{0x80000004,0xea00239A},
{0x40180014,0x0001ddcc},
{0x40180034,0x00032d18},




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


{0x40180020,0x0001936c},// pair after auth fail
{0x40180040,0x00001ca8},

//解决华为honor 3c,黑米手机esco无法建立的问题
{ 0x40180004, 0x00016ac0 }, // esco air mode
{ 0x40180024, 0x0001fbf4 },


{0x40180000,0x0000ffff},



	
};


const UINT32 rda_trap_calib_12[][2] = 
{
{0x80000000,0xea0023C6}, //   b  0x80008f20
{0x40180004,0x000223d8}, //
{0x40180024,0x00032d14}, //

{0x80000004,0xea00003e}, //
{0x80000100,0xe59f0024}, //   ldr r0=80009128
{0x80000104,0xe5900000}, //  ldr r0,[r0,0]
{0x80000108,0xe3500000}, //  cmp r0,0
{0x8000010c,0x05d48004}, //    ....    LDReqB     r8,[r4,#4]
{0x80000110,0x03a00a20}, // moveq r0,0x20000
{0x80000114,0x0280fe2e}, // addeq pc,r0,0x2e0
{0x80000118,0xe59f0010}, // ldr r0=0x70000030
{0x8000011c,0xe5901000}, // ldr r1,[r0,0]
{0x80000120,0xe3811a20}, // r1,r1,#0x20000
{0x80000124,0xe5801000}, //  str r1,[r0,0]
{0x80000128,0xe8bd83f8}, //    ....    LDMFD    r13!,{r3-r9,pc}
{0x8000012c,0x80009128}, //
{0x80000130,0x70000030}, //

{0x40180008,0x000202dc}, //
{0x40180028,0x00032d18}, //

{0x80008f00,0xe51ff004}, //    ....    LDR      pc,0x80008f04
{0x80008f04,0x00003458}, //    d4..    DCD    13412
{0x80008f08,0xe51ff004}, //    ....    LDR      pc,0x80008f0c
{0x80008f0c,0x00000c90}, //    ....    DCD    3228
{0x80008f10,0xe51ff004}, //    ....    LDR      pc,0x80008f14
{0x80008f14,0x0002c88c}, //    T...    DCD    182100
{0x80008f18,0xe51ff004}, //    ....    LDR      pc,0x80008f1c
{0x80008f1c,0x0001f3a8}, //    ....    DCD    127496
{0x80008f20,0xe92d4ff0}, //    .O-.    STMFD    r13!,{r4-r11,r14}
{0x80008f24,0xe5d05000}, //    .P..    LDRB     r5,[r0,#0]
{0x80008f28,0xe1a04000}, //    .@..    MOV      r4,r0
{0x80008f2c,0xe5d00001}, //    ....    LDRB     r0,[r0,#1]
{0x80008f30,0xe24dd01c}, //    ..M.    SUB      r13,r13,#0x1c
{0x80008f34,0xe58d0018}, //    ....    STR      r0,[r13,#0x18]
{0x80008f38,0xe2840002}, //    ....    ADD      r0,r4,#2
{0x80008f3c,0xebffffef}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80008f00
{0x80008f40,0xe1a06000}, //    .`..    MOV      r6,r0
{0x80008f44,0xe2840006}, //    ....    ADD      r0,r4,#6
{0x80008f48,0xebffffec}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80008f00
{0x80008f4c,0xe58d0014}, //    ....    STR      r0,[r13,#0x14]
{0x80008f50,0xe284000a}, //    ....    ADD      r0,r4,#0xa
{0x80008f54,0xebffffe9}, //    ....    BL       $Ven$AA$L$$HCIparam_Get_Uint32  ; 0x80008f00
{0x80008f58,0xe58d0010}, //    ....    STR      r0,[r13,#0x10]
{0x80008f5c,0xe5d4000e}, //    ....    LDRB     r0,[r4,#0xe]
{0x80008f60,0xe3550000}, //    ..U.    CMP      r5,#0
{0x80008f64,0xe58d000c}, //    ....    STR      r0,[r13,#0xc]
{0x80008f68,0xe5d4000f}, //    ....    LDRB     r0,[r4,#0xf]
{0x80008f6c,0xe58d0008}, //    ....    STR      r0,[r13,#8]
{0x80008f70,0xe5d42010}, //    . ..    LDRB     r2,[r4,#0x10]
{0x80008f74,0xe58d2004}, //    . ..    STR      r2,[r13,#4]
{0x80008f78,0xe5d48011}, //    ....    LDRB     r8,[r4,#0x11]
{0x80008f7c,0xe5d49012}, //    ....    LDRB     r9,[r4,#0x12]
{0x80008f80,0xe5d4a013}, //    ....    LDRB     r10,[r4,#0x13]
{0x80008f84,0xe5d47014}, //    .p..    LDRB     r7,[r4,#0x14]
{0x80008f88,0xe5d41015}, //    ....    LDRB     r1,[r4,#0x15]
{0x80008f8c,0xe5d40016}, //    ....    LDRB     r0,[r4,#0x16]
{0x80008f90,0xe0810400}, //    ....    ADD      r0,r1,r0,LSL #8
{0x80008f94,0xe3c04b40}, //    @K..    BIC      r4,r0,#0x10000
{0x80008f98,0xe59f0178}, //    x...    LDR      r0,0x80009118
{0x80008f9c,0x0a00004c}, //    L...    BEQ      0x800090d4
{0x80008fa0,0xe3a01001}, //    ....    MOV      r1,#1
{0x80008fa4,0xe3560000}, //    ..V.    CMP      r6,#0
{0x80008fa8,0xe5801000}, //    ....    STR      r1,[r0,#0]
{0x80008fac,0x128d1010}, //    ....    ADDNE    r1,r13,#0x10
{0x80008fb0,0x11a00006}, //    ....    MOVNE    r0,r6
{0x80008fb4,0x128d2014}, //    . ..    ADDNE    r2,r13,#0x14
{0x80008fb8,0xe3a05000}, //    .P..    MOV      r5,#0
{0x80008fbc,0xe3a0b000}, //    ....    MOV      r11,#0
{0x80008fc0,0x1bffffd0}, //    ....    BLNE     $Ven$AA$L$$BTaddr_Build_Sync_Word  ; 0x80008f08
{0x80008fc4,0xe59d1014}, //    ....    LDR      r1,[r13,#0x14]
{0x80008fc8,0xe3a00470}, //    p...    MOV      r0,#0x70000000
{0x80008fcc,0xe5801008}, //    ....    STR      r1,[r0,#8]
{0x80008fd0,0xe59d1010}, //    ....    LDR      r1,[r13,#0x10]
{0x80008fd4,0xe580100c}, //    ....    STR      r1,[r0,#0xc]
{0x80008fd8,0xe59d2004}, //    . ..    LDR      r2,[r13,#4]
{0x80008fdc,0xe20a1007}, //    ....    AND      r1,r10,#7
{0x80008fe0,0xe3a034ff}, //    .4..    MOV      r3,#0xff000000
{0x80008fe4,0xe0032c02}, //    .,..    AND      r2,r3,r2,LSL #24
{0x80008fe8,0xe1821001}, //    ....    ORR      r1,r2,r1
{0x80008fec,0xe3580000}, //    ..X.    CMP      r8,#0
{0x80008ff0,0x13a029fc}, //    .)..    MOVNE    r2,#0x3f0000
{0x80008ff4,0x10022809}, //    .(..    ANDNE    r2,r2,r9,LSL #16
{0x80008ff8,0x13825480}, //    .T..    ORRNE    r5,r2,#0x80000000
{0x80008ffc,0xe1a08000}, //    ....    MOV      r8,r0
{0x80009000,0xe5801020}, //     ...    STR      r1,[r0,#0x20]
{0x80009004,0xe3a0a480}, //    ....    MOV      r10,#0x80000000
{0x80009008,0xe3a06000}, //    .`..    MOV      r6,#0
{0x8000900c,0xe5885024}, //    $P..    STR      r5,[r8,#0x24]
{0x80009010,0xe3a00000}, //    ....    MOV      r0,#0
{0x80009014,0xe1500004}, //    ..P.    CMP      r0,r4
{0x80009018,0x359f10fc}, //    ...5    LDRCC    r1,0x8000911c
{0x8000901c,0x37c17000}, //    .p.7    STRCCB   r7,[r1,r0]
{0x80009020,0x32800001}, //    ...2    ADDCC    r0,r0,#1
{0x80009024,0x33c00b40}, //    @..3    BICCC    r0,r0,#0x10000
{0x80009028,0x3afffff9}, //    ...:    BCC      0x80009014
{0x8000902c,0xe59d0018}, //    ....    LDR      r0,[r13,#0x18]
{0x80009030,0xe2800cc0}, //    ....    ADD      r0,r0,#0xc000
{0x80009034,0xe2800dc6}, //    ....    ADD      r0,r0,#0x3180
{0x80009038,0xe3800b80}, //    ....    ORR      r0,r0,#0x20000
{0x8000903c,0xebffffb3}, //    ....    BL       $Ven$AA$L$$_HWradio_ProgNow  ; 0x80008f10
{0x80009040,0xe59d000c}, //    ....    LDR      r0,[r13,#0xc]
{0x80009044,0xe3500000}, //    ..P.    CMP      r0,#0
{0x80009048,0x05886034}, //    4`..    STREQ    r6,[r8,#0x34]
{0x8000904c,0x1588a034}, //    4...    STRNE    r10,[r8,#0x34]
{0x80009050,0xe1a0152b}, //    +...    MOV      r1,r11,LSR #10
{0x80009054,0xe1a01501}, //    ....    MOV      r1,r1,LSL #10
{0x80009058,0xe3c40cfc}, //    ....    BIC      r0,r4,#0xfc00
{0x8000905c,0xe1800001}, //    ....    ORR      r0,r0,r1
{0x80009060,0xe59d1008}, //    ....    LDR      r1,[r13,#8]
{0x80009064,0xe3a02df0}, //    .-..    MOV      r2,#0x3c00
{0x80009068,0xe0021501}, //    ....    AND      r1,r2,r1,LSL #10
{0x8000906c,0xe3c00df0}, //    ....    BIC      r0,r0,#0x3c00
{0x80009070,0xe1810000}, //    ....    ORR      r0,r1,r0
{0x80009074,0xe3c00a60}, //    `...    BIC      r0,r0,#0x60000
{0x80009078,0xe3800b80}, //    ....    ORR      r0,r0,#0x20000
{0x8000907c,0xe5880030}, //    0...    STR      r0,[r8,#0x30]
{0x80009080,0xe59f0098}, //    ....    LDR      r0,0x80009120
{0x80009084,0xe5d01001}, //    ....    LDRB     r1,[r0,#1]
{0x80009088,0xe5d02008}, //    . ..    LDRB     r2,[r0,#8]
{0x8000908c,0xe26110dd}, //    ..a.    RSB      r1,r1,#0xdd
{0x80009090,0xe2811e40}, //    @...    ADD      r1,r1,#0x400
{0x80009094,0xe2622f9b}, //    ./b.    RSB      r2,r2,#0x26c
{0x80009098,0xe1811802}, //    ....    ORR      r1,r1,r2,LSL #16
{0x8000909c,0xe5881140}, //    @...    STR      r1,[r8,#0x140]
{0x800090a0,0xe5d01002}, //    ....    LDRB     r1,[r0,#2]
{0x800090a4,0xe5d00009}, //    ....    LDRB     r0,[r0,#9]
{0x800090a8,0xe2611f90}, //    ..a.    RSB      r1,r1,#0x240
{0x800090ac,0xe26000be}, //    ..`.    RSB      r0,r0,#0xbe
{0x800090b0,0xe2800e40}, //    @...    ADD      r0,r0,#0x400
{0x800090b4,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x800090b8,0xe1810800}, //    ....    ORR      r0,r1,r0,LSL #16
{0x800090bc,0xe5880158}, //    X...    STR      r0,[r8,#0x158]
{0x800090c0,0xe5886170}, //    pa..    STR      r6,[r8,#0x170]
{0x800090c4,0xe59f0058}, //    X...    LDR      r0,0x80009124
{0x800090c8,0xe5880028}, //    (...    STR      r0,[r8,#0x28]
{0x800090cc,0xe28dd01c}, //    ....    ADD      r13,r13,#0x1c
{0x800090d0,0xe8bd8ff0}, //    ....    LDMFD    r13!,{r4-r11,pc}
{0x800090d4,0xe3a01000}, //    ....    MOV      r1,#0
{0x800090d8,0xe3a02470}, //    p$..    MOV      r2,#0x70000000
{0x800090dc,0xe5821030}, //    0...    STR      r1,[r2,#0x30]
{0x800090e0,0xe3a06000}, //    .`..    MOV      r6,#0
{0x800090e4,0xe5806000}, //    .`..    STR      r6,[r0,#0]
{0x800090e8,0xe59f0030}, //    0...    LDR      r0,0x80009120
{0x800090ec,0xe5d01002}, //    ....    LDRB     r1,[r0,#2]
{0x800090f0,0xe5d00009}, //    ....    LDRB     r0,[r0,#9]
{0x800090f4,0xe2611f90}, //    ..a.    RSB      r1,r1,#0x240
{0x800090f8,0xe26000be}, //    ..`.    RSB      r0,r0,#0xbe
{0x800090fc,0xe2800e40}, //    @...    ADD      r0,r0,#0x400
{0x80009100,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x80009104,0xe1810800}, //    ....    ORR      r0,r1,r0,LSL #16
{0x80009108,0xe3800c80}, //    ....    ORR      r0,r0,#0x8000
{0x8000910c,0xe5820158}, //    X...    STR      r0,[r2,#0x158]
{0x80009110,0xebffff80}, //    ....    BL       $Ven$AA$L$$LSLCirq_R2P_Turn_On_Intr  ; 0x80008f18
{0x80009114,0xeaffffec}, //    ....    B        0x800090cc
{0x80009118,0x80009128}, //    (...    DCD    2147520808
{0x8000911c,0x70000200}, //    ...p    DCD    1879048704
{0x80009120,0x80002e34}, //    ....    DCD    2147531416
{0x80009124,0x0000f3fb}, //    ....    DCD    62459
{0x80009128,0x00000000}, //        
{0x40180000,0x00000003}, //                
};

const UINT8 rdabt_unsniff_prerxon_12[] = {0x01,0x72,0xfc,0x05,0x05,0x64,0x00,0x00,0x00};

//////////////////////////////////////////////////////////




void Rdabt_Pskey_Write_r12(void)
{
   UINT32 i;
   for(i=0;i<sizeof(rda_pskey_12)/sizeof(rda_pskey_12[0]);i++)
   {
      rdabt_wirte_memory(rda_pskey_12[i][0],&rda_pskey_12[i][1],1,0x0);
      RDABT_DELAY(2);
   }
	RDABT_DELAY(2);  	
}


void Rdabt_trap_write_r12(void)
{
   UINT32 i;
   extern BOOL g_test_mode;
   extern BOOL  g_CalbClock;

   if(g_CalbClock)
   {
       hal_HstSendEvent(1, 0x88888888);
       for(i=0;i<sizeof(rda_trap_calib_12)/sizeof(rda_trap_calib_12[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_calib_12[i][0],&rda_trap_calib_12[i][1],1,0x0);
          RDABT_DELAY(1);
       }	
   }
   else
   {
       for(i=0;i<sizeof(rda_trap_12)/sizeof(rda_trap_12[0]);i++)
       {
          rdabt_wirte_memory(rda_trap_12[i][0],&rda_trap_12[i][1],1,0x0);
          RDABT_DELAY(1);
       }	
   }
}

void Rdabt_unsniff_prerxon_write_r12(void)
{
   UINT16 num_send;	
   rdabt_adp_uart_tx(rdabt_unsniff_prerxon_12,sizeof(rdabt_unsniff_prerxon_12),&num_send);
   RDABT_DELAY(1);
}


#ifdef __RDA_CHIP_R12_5990__

/* segment =0x20*/
const kal_uint8 rda_segment_5990_bt[] =    {0x01,0x72,0xfc,0x05,0x09,0x32,0xa0,0x00,0x20};

/* wifi_frame =0x20*/
const kal_uint8 rda_wifi_frame_5990_bt[] = {0x01,0x72,0xfc,0x05,0x0b,0x20,0x00,0x00,0x00};


void Rdabt_set_segement_5990_bt(void)
{
   kal_uint16 num_send;	
   rdabt_uart_tx(rda_segment_5990_bt,sizeof(rda_segment_5990_bt),&num_send);
   RDABT_DELAY(1);
}


void Rdabt_set_wifi_frame_5990_bt(void)
{
   kal_uint16 num_send;	
   rdabt_uart_tx(rda_wifi_frame_5990_bt,sizeof(rda_wifi_frame_5990_bt),&num_send);
   RDABT_DELAY(1);
}

#endif



void Rdabt_patch_write_r12(void)
{
}

#endif//#if defined__RDA_CHIP_R10__ || __RDA_CHIP_R10_5872H__

#endif


