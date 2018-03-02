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


#include "cswtype.h"
#include <stdio.h>
#include <stdlib.h>
#include "fs.h"
//#include "oslmemory.h"
#include "mmc_jpeg_enc.h"
#include "fs.h"


#define MMI_TS_ID_BASE        100

#define MMI_TS_ID             (MMI_TS_ID_BASE)
//#include "mmi_trace.h"
#ifndef MMI_ON_WIN32
typedef int FILE;
#endif /*MMI_ON_WIN32*/
/* Standard quantization tables for luminance and chrominance. Scaled version
 * of these are used by the encoder for a given quality.
 * These tables come from the IJG code, which takes them from the JPeg specs,
 * and are generic quantization tables that give good results on most images.
 */
//data array/////////////

void _jpeg_putw(int w);
void _jpeg_putc(int c);
void _jpeg_new_chunk(int type);
void _jpeg_chunk_putw(int w);
void _jpeg_chunk_putc(int c);
void _jpeg_write_chunk(void);
void write_header(short quality_select ,short width,short height);
void apply_fdct(short *data);
void format_number(int num, int *category, int *bits);
void _jpeg_flush_bits(void);
void put_bits(int value, int num_bits);
int encode_pass(char *Buff,short ImgWidth,short ImgHeight);
int encode_block(short *block, int type, int *old_dc);
void  Init_GlobalVar(/*FILE* fp_out*/);
int MMF_ISP_VGAto1280_1024New( unsigned short hei, unsigned short wid,  unsigned short* rgb_buf, unsigned short* output_buf );


#define FIX_0_298631336		2446
#define FIX_0_390180644		3196
#define FIX_0_541196100		4433
#define FIX_0_765366865		6270
#define FIX_0_899976223		7373
#define FIX_1_175875602		9633
#define FIX_1_501321110		12299
#define FIX_1_847759065		15137
#define FIX_1_961570560		16069
#define FIX_2_053119869		16819
#define FIX_2_562915447		20995
#define FIX_3_072711026		25172


#define CHUNK_SOI		0xffd8
#define CHUNK_EOI		0xffd9

#define CHUNK_JPG0		0xf0
#define CHUNK_JPG13		0xfd
#define CHUNK_DHP		0xde
#define CHUNK_EXP		0xdf
#define CHUNK_DNL		0xdc
#define CHUNK_DAC		0xcc
#define CHUNK_SOF3		0xc3
#define CHUNK_SOF5		0xc5
#define CHUNK_SOF6		0xc6
#define CHUNK_SOF7		0xc7
#define CHUNK_SOF9		0xc9
#define CHUNK_SOF10		0xca
#define CHUNK_SOF11		0xcb
#define CHUNK_SOF13		0xcd
#define CHUNK_SOF14		0xce
#define CHUNK_SOF15		0xcf
#define CHUNK_APP0		0xe0
#define CHUNK_APP1		0xe1
#define CHUNK_APP2		0xe2
#define CHUNK_APP3		0xe3
#define CHUNK_APP4		0xe4
#define CHUNK_APP5		0xe5
#define CHUNK_APP6		0xe6
#define CHUNK_APP7		0xe7
#define CHUNK_APP8		0xe8
#define CHUNK_APP9		0xe9
#define CHUNK_APP10		0xea
#define CHUNK_APP11		0xeb
#define CHUNK_APP12		0xec
#define CHUNK_APP13		0xed
#define CHUNK_APP14		0xee
#define CHUNK_APP15		0xef

#define CHUNK_TEM		0x01
#define CHUNK_RST0		0xffd0
#define CHUNK_RST1		0xffd1
#define CHUNK_RST2		0xffd2
#define CHUNK_RST3		0xffd3
#define CHUNK_RST4		0xffd4
#define CHUNK_RST5		0xffd5
#define CHUNK_RST6		0xffd6
#define CHUNK_RST7		0xffd7

#define CHUNK_JPG		0xc8

#define CHUNK_SOF0		0xc0
#define CHUNK_SOF1		0xc1
#define CHUNK_SOF2		0xc2
#define CHUNK_DHT		0xc4
#define CHUNK_SOS		0xda
#define CHUNK_DQT		0xdb
#define CHUNK_DRI		0xdd
#define CHUNK_COM		0xfe

#define SOF0_DEFINED		0x01
#define DHT_DEFINED		0x02
#define SOS_DEFINED		0x04
#define DQT_DEFINED		0x08
#define APP0_DEFINED		0x10
#define APP1_DEFINED		0x20
#define DRI_DEFINED		0x40
#define IS_PROGRESSIVE		0x80

#define JFIF_OK			0x1F
#define EXIF_OK			0x2F

#define LUMINANCE		0
#define CHROMINANCE		1

#define PASS_WRITE		0
#define PASS_COMPUTE_HUFFMAN    1


//error code
#define FUNC_OK 0
#define FILE_OPEN_ERROR -1
#define VLC_ERROR -2
#define SAMPLING_PARAMETER_ERROR -3


//#define  USEFILE             1
#define  TEMPOUTBUFSIZE      512*20
//mode selection///////////////////

#define ISP_DATA_ERROR  -1
#define ISP_STATE_ERROR -2
#define ISP_FUN_OK       1
#define int_abs(a)    	(a)>=0?(a):(-a)
#define fun_max(a,b)	(a)>(b)?(a):(b)


const short huffman_dc_0[24]={//[leng][code]

	0x2 ,0x0 ,
	0x3 ,0x2 ,
	0x3 ,0x3 ,
	0x3 ,0x4 ,
	0x3 ,0x5 ,
	0x3 ,0x6 ,
	0x4 ,0xe ,
	0x5 ,0x1e ,
	0x6 ,0x3e ,
	0x7 ,0x7e ,
	0x8 ,0xfe ,
	0x9 ,0x1fe

};

const short huffman_dc_1[24]={//[leng][code]
    0x2 ,0x0 ,
	0x2 ,0x1 ,
	0x2 ,0x2 ,
	0x3 ,0x6 ,
	0x4 ,0xe ,
	0x5 ,0x1e,
	0x6 ,0x3e,
	0x7 ,0x7e,
	0x8 ,0xfe,
	0x9 ,0x1fe,
	0xa ,0x3fe,
	0xb ,0x7fe 
};
const  unsigned short huffman_ac_0[324]={//[leng][code]
0x4 ,0xa ,  //00
0xb ,0x7f9 ,//F0
  
0x2 ,0x0 ,    
0x2 ,0x1 ,    
0x3 ,0x4 ,    
0x4 ,0xb ,    
0x5 ,0x1a ,   
0x7 ,0x78 ,   
0x8 ,0xf8 ,   
0xa ,0x3f6 ,  
0x10 ,0xff82 ,
0x10 ,0xff83 ,
              
0x4 ,0xc ,    
0x5 ,0x1b ,   
0x7 ,0x79 ,   
0x9 ,0x1f6 ,  
0xb ,0x7f6 ,  
0x10 ,0xff84 ,
0x10 ,0xff85 ,
0x10 ,0xff86 ,
0x10 ,0xff87 ,
0x10 ,0xff88 ,
              
0x5 ,0x1c ,   
0x8 ,0xf9 ,   
0xa ,0x3f7 ,  
0xc ,0xff4 ,  
0x10 ,0xff89 ,
0x10 ,0xff8a ,
0x10 ,0xff8b ,
0x10 ,0xff8c ,
0x10 ,0xff8d ,
0x10 ,0xff8e ,
              

0x6 ,0x3a ,   
0x9 ,0x1f7 ,  
0xc ,0xff5 ,  
0x10 ,0xff8f ,
0x10 ,0xff90 ,
0x10 ,0xff91 ,
0x10 ,0xff92 ,
0x10 ,0xff93 ,
0x10 ,0xff94 ,
0x10 ,0xff95 ,
              
0x6 ,0x3b ,   
0xa ,0x3f8 ,  
0x10 ,0xff96 ,
0x10 ,0xff97 ,
0x10 ,0xff98 ,
0x10 ,0xff99 ,
0x10 ,0xff9a ,
0x10 ,0xff9b ,
0x10 ,0xff9c ,
0x10 ,0xff9d ,
              
0x7 ,0x7a ,   
0xb ,0x7f7 ,  
0x10 ,0xff9e ,
0x10 ,0xff9f ,
0x10 ,0xffa0 ,
0x10 ,0xffa1 ,
0x10 ,0xffa2 ,
0x10 ,0xffa3 ,
0x10 ,0xffa4 ,
0x10 ,0xffa5 ,
              
0x7 ,0x7b ,   
0xc ,0xff6 ,  
0x10 ,0xffa6 ,
0x10 ,0xffa7 ,
0x10 ,0xffa8 ,
0x10 ,0xffa9 ,
0x10 ,0xffaa ,
0x10 ,0xffab ,
0x10 ,0xffac ,
0x10 ,0xffad ,
              
0x8 ,0xfa ,   
0xc ,0xff7 ,  
0x10 ,0xffae ,
0x10 ,0xffaf ,
0x10 ,0xffb0 ,
0x10 ,0xffb1 ,
0x10 ,0xffb2 ,
0x10 ,0xffb3 ,
0x10 ,0xffb4 ,
0x10 ,0xffb5 ,
              
0x9 ,0x1f8 ,  
0xf ,0x7fc0 , 
0x10 ,0xffb6 ,
0x10 ,0xffb7 ,
0x10 ,0xffb8 ,
0x10 ,0xffb9 ,
0x10 ,0xffba ,
0x10 ,0xffbb ,
0x10 ,0xffbc ,
0x10 ,0xffbd ,
              
0x9 ,0x1f9 ,  
0x10 ,0xffbe ,
0x10 ,0xffbf ,
0x10 ,0xffc0 ,
0x10 ,0xffc1 ,
0x10 ,0xffc2 ,
0x10 ,0xffc3 ,
0x10 ,0xffc4 ,
0x10 ,0xffc5 ,
0x10 ,0xffc6 ,
              
0x9 ,0x1fa ,  
0x10 ,0xffc7 ,
0x10 ,0xffc8 ,
0x10 ,0xffc9 ,
0x10 ,0xffca ,
0x10 ,0xffcb ,
0x10 ,0xffcc ,
0x10 ,0xffcd ,
0x10 ,0xffce ,
0x10 ,0xffcf ,
              
0xa ,0x3f9 ,  
0x10 ,0xffd0 ,
0x10 ,0xffd1 ,
0x10 ,0xffd2 ,
0x10 ,0xffd3 ,
0x10 ,0xffd4 ,
0x10 ,0xffd5 ,
0x10 ,0xffd6 ,
0x10 ,0xffd7 ,
0x10 ,0xffd8 ,
              
0xa ,0x3fa ,  
0x10 ,0xffd9 ,
0x10 ,0xffda ,
0x10 ,0xffdb ,
0x10 ,0xffdc ,
0x10 ,0xffdd ,
0x10 ,0xffde ,
0x10 ,0xffdf ,
0x10 ,0xffe0 ,
0x10 ,0xffe1 ,
              
0xb ,0x7f8 ,  
0x10 ,0xffe2 ,
0x10 ,0xffe3 ,
0x10 ,0xffe4 ,
0x10 ,0xffe5 ,
0x10 ,0xffe6 ,
0x10 ,0xffe7 ,
0x10 ,0xffe8 ,
0x10 ,0xffe9 ,
0x10 ,0xffea ,
              
0x10 ,0xffeb ,
0x10 ,0xffec ,
0x10 ,0xffed ,
0x10 ,0xffee ,
0x10 ,0xffef ,
0x10 ,0xfff0 ,
0x10 ,0xfff1 ,
0x10 ,0xfff2 ,
0x10 ,0xfff3 ,
0x10 ,0xfff4 ,
    
              
              
  
0x10 ,0xfff5 ,
0x10 ,0xfff6 ,
0x10 ,0xfff7 ,
0x10 ,0xfff8 ,
0x10 ,0xfff9 ,
0x10 ,0xfffa ,
0x10 ,0xfffb ,
0x10 ,0xfffc ,
0x10 ,0xfffd ,
0x10 ,0xfffe 
};

const  unsigned short  huffman_ac_1[324]={
0x2 ,0x0 ,  //00
0xa ,0x3fa ,//F0
   
0x2 ,0x1 ,    
0x3 ,0x4 ,    
0x4 ,0xa ,    
0x5 ,0x18 ,   
0x5 ,0x19 ,   
0x6 ,0x38 ,   
0x7 ,0x78 ,   
0x9 ,0x1f4 ,  
0xa ,0x3f6 ,  
0xc ,0xff4 ,  
              

0x4 ,0xb ,    
0x6 ,0x39 ,   
0x8 ,0xf6 ,   
0x9 ,0x1f5 ,  
0xb ,0x7f6 ,  
0xc ,0xff5 ,  
0x10 ,0xff88 ,
0x10 ,0xff89 ,
0x10 ,0xff8a ,
0x10 ,0xff8b ,
              

0x5 ,0x1a ,   
0x8 ,0xf7 ,   
0xa ,0x3f7 ,  
0xc ,0xff6 ,  
0xf ,0x7fc2 , 
0x10 ,0xff8c ,
0x10 ,0xff8d ,
0x10 ,0xff8e ,
0x10 ,0xff8f ,
0x10 ,0xff90 ,
              

0x5 ,0x1b ,   
0x8 ,0xf8 ,   
0xa ,0x3f8 ,  
0xc ,0xff7 ,  
0x10 ,0xff91 ,
0x10 ,0xff92 ,
0x10 ,0xff93 ,
0x10 ,0xff94 ,
0x10 ,0xff95 ,
0x10 ,0xff96 ,
              

0x6 ,0x3a ,   
0x9 ,0x1f6 ,  
0x10 ,0xff97 ,
0x10 ,0xff98 ,
0x10 ,0xff99 ,
0x10 ,0xff9a ,
0x10 ,0xff9b ,
0x10 ,0xff9c ,
0x10 ,0xff9d ,
0x10 ,0xff9e ,
              

0x6 ,0x3b ,   
0xa ,0x3f9 ,  
0x10 ,0xff9f ,
0x10 ,0xffa0 ,
0x10 ,0xffa1 ,
0x10 ,0xffa2 ,
0x10 ,0xffa3 ,
0x10 ,0xffa4 ,
0x10 ,0xffa5 ,
0x10 ,0xffa6 ,
              

0x7 ,0x79 ,   
0xb ,0x7f7 ,  
0x10 ,0xffa7 ,
0x10 ,0xffa8 ,
0x10 ,0xffa9 ,
0x10 ,0xffaa ,
0x10 ,0xffab ,
0x10 ,0xffac ,
0x10 ,0xffad ,
0x10 ,0xffae ,
              

0x7 ,0x7a ,   
0xb ,0x7f8 ,  
0x10 ,0xffaf ,
0x10 ,0xffb0 ,
0x10 ,0xffb1 ,
0x10 ,0xffb2 ,
0x10 ,0xffb3 ,
0x10 ,0xffb4 ,
0x10 ,0xffb5 ,
0x10 ,0xffb6 ,
              

0x8 ,0xf9 ,   
0x10 ,0xffb7 ,
0x10 ,0xffb8 ,
0x10 ,0xffb9 ,
0x10 ,0xffba ,
0x10 ,0xffbb ,
0x10 ,0xffbc ,
0x10 ,0xffbd ,
0x10 ,0xffbe ,
0x10 ,0xffbf ,
              

0x9 ,0x1f7 ,  
0x10 ,0xffc0 ,
0x10 ,0xffc1 ,
0x10 ,0xffc2 ,
0x10 ,0xffc3 ,
0x10 ,0xffc4 ,
0x10 ,0xffc5 ,
0x10 ,0xffc6 ,
0x10 ,0xffc7 ,
0x10 ,0xffc8 ,
              

0x9 ,0x1f8 ,  
0x10 ,0xffc9 ,
0x10 ,0xffca ,
0x10 ,0xffcb ,
0x10 ,0xffcc ,
0x10 ,0xffcd ,
0x10 ,0xffce ,
0x10 ,0xffcf ,
0x10 ,0xffd0 ,
0x10 ,0xffd1 ,
              

0x9 ,0x1f9 ,  
0x10 ,0xffd2 ,
0x10 ,0xffd3 ,
0x10 ,0xffd4 ,
0x10 ,0xffd5 ,
0x10 ,0xffd6 ,
0x10 ,0xffd7 ,
0x10 ,0xffd8 ,
0x10 ,0xffd9 ,
0x10 ,0xffda ,
              

0x9 ,0x1fa ,  
0x10 ,0xffdb ,
0x10 ,0xffdc ,
0x10 ,0xffdd ,
0x10 ,0xffde ,
0x10 ,0xffdf ,
0x10 ,0xffe0 ,
0x10 ,0xffe1 ,
0x10 ,0xffe2 ,
0x10 ,0xffe3 ,
              

0xb ,0x7f9 ,  
0x10 ,0xffe4 ,
0x10 ,0xffe5 ,
0x10 ,0xffe6 ,
0x10 ,0xffe7 ,
0x10 ,0xffe8 ,
0x10 ,0xffe9 ,
0x10 ,0xffea ,
0x10 ,0xffeb ,
0x10 ,0xffec ,
              

0xe ,0x3fe0 , 
0x10 ,0xffed ,
0x10 ,0xffee ,
0x10 ,0xffef ,
0x10 ,0xfff0 ,
0x10 ,0xfff1 ,
0x10 ,0xfff2 ,
0x10 ,0xfff3 ,
0x10 ,0xfff4 ,
0x10 ,0xfff5 ,
              
              
              
              
              
  
0xf ,0x7fc3 , 
0x10 ,0xfff6 ,
0x10 ,0xfff7 ,
0x10 ,0xfff8 ,
0x10 ,0xfff9 ,
0x10 ,0xfffa ,
0x10 ,0xfffb ,
0x10 ,0xfffc ,
0x10 ,0xfffd ,
0x10 ,0xfffe 

};

const  int l_c_quant_table0[128]={
0x1000 ,0x1745 ,0x1555 ,0x1249 ,0x1555 ,0x1999 ,0x1000 ,0x1249 ,
0x13b1 ,0x1249 ,0xe38 ,0xf0f ,0x1000 ,0xd79 ,0xaaa ,0x666 ,
0x9d8 ,0xaaa ,0xba2 ,0xba2 ,0xaaa ,0x539 ,0x750 ,0x6eb ,
0x8d3 ,0x666 ,0x469 ,0x505 ,0x432 ,0x444 ,0x47d ,0x505 ,
0x492 ,0x4a7 ,0x400 ,0x38e ,0x2c8 ,0x348 ,0x400 ,0x3c3 ,
0x2f1 ,0x3b5 ,0x4a7 ,0x492 ,0x333 ,0x259 ,0x329 ,0x2f1 ,
0x2b1 ,0x29c ,0x27c ,0x276 ,0x27c ,0x421 ,0x353 ,0x243 ,
0x21d ,0x249 ,0x28f ,0x222 ,0x2c8 ,0x288 ,0x27c ,0x295 ,
0xf0f ,0xe38 ,0xe38 ,0xaaa ,0xc30 ,0xaaa ,0x572 ,0x9d8 ,
0x9d8 ,0x572 ,0x295 ,0x3e0 ,0x492 ,0x3e0 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,
0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295 ,0x295

};
const int l_c_quant_table1[128]={
0x8000 ,0x10000 ,0x10000 ,0x10000 ,0x10000 ,0x10000 ,0x8000 ,0x10000 ,
0x10000 ,0x10000 ,0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x4000 ,
0x5555 ,0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x3333 ,0x4000 ,0x4000 ,
0x5555 ,0x4000 ,0x2aaa ,0x3333 ,0x2aaa ,0x2aaa ,0x2aaa ,0x3333 ,
0x2aaa ,0x2aaa ,0x2aaa ,0x2492 ,0x1c71 ,0x2000 ,0x2aaa ,0x2492 ,
0x1c71 ,0x2492 ,0x2aaa ,0x2aaa ,0x2000 ,0x1745 ,0x2000 ,0x1c71 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x2aaa ,0x2000 ,0x1745 ,
0x1555 ,0x1745 ,0x1999 ,0x1555 ,0x1c71 ,0x1999 ,0x1999 ,0x1999 ,
0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x8000 ,0x3333 ,0x5555 ,
0x5555 ,0x3333 ,0x1999 ,0x2492 ,0x2aaa ,0x2492 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,
0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 ,0x1999 

};

static const  unsigned char inverse_Zig_Zag[64]={
							0,1,8,16,9,2,3,10,
							17,24,32,25,18,11,4,5,
							12,19,26,33,40,48,41,34,
							27,20,13,6,7,14,21,28,
							35,42,49,56,57,50,43,36,
							29,22,15,23,30,37,44,51,
							58,59,52,45,38,31,39,46,
							53,60,61,54,47,55,62,63
						};
//global variable//////////
static unsigned char* chunk;
static char * chunkptr;
static char * chunk_end;
static int chunk_len;
static int current_byte = 0;
static int current_bit = 8;
#ifndef MMI_ON_WIN32
FILE *pout;
#endif /*MMI_ON_WIN32*/
int dc_y, dc_cb, dc_cr;
const int*  quant_table=NULL;
unsigned long outputlen=0;
static int fileHandle=0;
UINT8 gCameraCreateFileFlag=0;
//input stream/////////////

//unsigned char  ouput_ref[]={
//#include "VGAREG.tab"
//};
//////////////////////////////////function api
//optimized by zouying   2007-9-7 
//remove some big tabs
/////////////////////////////////

//summury of the program////////
//const table: 1240 shorts -> 2480bytes
//temp  ram:   64*4 = 384 shorts  -> 768bytes
//input  buf:   1280*16*2 = 40960bytes
//output buf:   1024bytes  
//jpeg size : 218632bytes  
//bmp  size : 2621440bytes   


void  Init_GlobalVar(/*FILE* fp_out*/)
{
    ///init global variable/////////////////
	//pout = fp_out;
	dc_y = dc_cb = dc_cr = 0;
        current_byte = 0;
        current_bit = 8;
	chunkptr = chunk;
	chunk_end = chunk + TEMPOUTBUFSIZE;
	outputlen=0;
}

JPEGENC_ERROR  MMC_JpegEncode(char* filename,short quality,short ImgWidth,short ImgHeight,short* InBuff,unsigned long* outlen)
{
	short * Buff;
        int i;
        int interval=16;
	mmi_trace(1,"MMC_JpegEncode\n");
//	time_measure_start();
        if(ImgWidth%8!=0 ||ImgHeight%8!=0 )
          {
          	mmi_trace(1,"MMC_JpegEncode,%8 return,ImgWidth is %d,ImgHeight is %d\n",ImgWidth,ImgHeight);
             return ENC_WRONGSIZE;
          }

#if 0
	fileHandle = FS_Open(filename, FM_WRITE, 0 );
	if (!fileHandle )
	{
		fileHandle = FS_Open(filename, FM_CREAT, 0);
	}
#else
	//FS_DeleteFile(filename);
	gCameraCreateFileFlag = 1;
	fileHandle = FS_Create(filename, 0);
	gCameraCreateFileFlag = 0;
     
#endif

#if 0
	if(fileHandle == NULL)
	{	  
		return  ENC_FAILTOCREAT;
	}
	else
#endif		
	if(fileHandle>=0)
	{
		mmi_trace(1,"success to creat  file, name: %s, handle: %d\n", filename, fileHandle);
	}
	else
	{
		mmi_trace(1,"error,fail to create file");
		return ENC_FAILTOCREAT;
	}

			
	
	chunk = (unsigned char*)OslMalloc(TEMPOUTBUFSIZE);

       Init_GlobalVar(/*fp_out*/);
	   mmi_trace(1,"MMC_JpegEncode,1111\n");

	write_header(quality,ImgWidth,ImgHeight);
		mmi_trace(1,"MMC_JpegEncode,2222\n");

	Buff = (short *)OslMalloc(ImgWidth*2*16);
	
	    

//	ass(NULL == Buff||NULL == chunk);
//           mmi_trace(1,"MMC_JpegEncode,Buff is %x\n",Buff);
        if(ImgWidth%16!=0 || ImgHeight%16!=0) interval=8;
        for (i=0;i<(ImgHeight/interval);i++)//for (lie=(ImgHeight-1);lie>=0;lie-=16)//
	{
	        if ( ImgWidth == 1280 && ImgHeight==1024) 
		{
		   //use function to finish 640*480 -> 1280*1024
	           MMF_ISP_VGAto1280_1024New(480,640,(unsigned short*)InBuff,(unsigned short*)Buff);
	        }
		else
		{
		   //read buf
		   memcpy(Buff,InBuff+i*ImgWidth*interval,ImgWidth*2*interval);
		}
	
		encode_pass((char*)Buff,ImgWidth,ImgHeight);
	}
	mmi_trace(1,"MMC_JpegEncode,444\n");
		   
		   
	_jpeg_flush_bits();
	_jpeg_putw(CHUNK_EOI);
	if((unsigned long)chunkptr != (unsigned long)(chunk + TEMPOUTBUFSIZE))
	{
		//fwrite(chunk,(unsigned long)chunkptr - (unsigned long)chunk,1,pout);
		MMI_FS_Write(fileHandle, chunk, (unsigned int)chunkptr - (unsigned int)chunk,&outputlen);
		outputlen+=607;
	}
	else
	{
		outputlen+=607;
	}
	
	*outlen = outputlen;
	mmi_trace(1,"MMC_JpegEncode,555\n");

	mmi_trace(1,"MMC_JpegEncode,666\n");

	if (FS_Close(fileHandle) < 0)
	{
		mmi_trace(1,"fail to close file!handle: %d \n", fileHandle);
		return ENC_FAILTOCLOSE;
	}
	else
		mmi_trace(1,"encode ok and close file\n");
//	mmi_trace(1,"MMC_JpegEncode,time is %d\n",time_measure_end()/104000);

	return NO_ENC_ERRO;
		
}
int encode_pass(char *buff,short width,short height)
{

	short y1[64], y2[64], y3[64], y4[64], cb[64], cr[64];
	short *y1_ptr, *y2_ptr, *y3_ptr, *y4_ptr, *cb_ptr, *cr_ptr;
	int func_ret;
	int block_x, x, y;
	int addr;
	int temp_width,temp_width2_sub16;
	int temp_width_add16,temp_width_mpy8,temp_width_mpy9,temp_width_mpy8_add16,temp_width_mpy9_add16;

	
	temp_width = width*2;

	temp_width2_sub16 = (temp_width << 1) - 16;
	temp_width_add16 = temp_width + 16;
	temp_width_mpy8 = temp_width << 3;
	temp_width_mpy9 = temp_width_mpy8 + temp_width;
	temp_width_mpy8_add16 = temp_width_mpy8 + 16;
	temp_width_mpy9_add16 = temp_width_mpy9 + 16;

	if (width%16==0 && height%16==0)//411
	{
	
				//for (block_y = 0; block_y < raw_height; block_y += 16) {
					//temp_addr = buff ;//+ block_y*temp_width;
					for (block_x = 0; block_x < temp_width; block_x += 32) {
						addr = (int)buff+block_x;
						y1_ptr = y1;
						y2_ptr = y2;
						y3_ptr = y3;
						y4_ptr = y4;
						cb_ptr = cb;
						cr_ptr = cr;
						for (y = 0; y < 8; y += 2) {
							for (x = 0; x < 8; x += 2) {
								int r, g, b;
								register unsigned short *ptr = (unsigned short *)addr;
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y1_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*cb_ptr = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*cr_ptr = (((r << 7) + (g * -107) + (b * -21)) >> 8);

								*(y1_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr = (unsigned short *)(addr + temp_width);
								*(y1_ptr + 8) = (((((ptr[0]&0xf800)>>8) * 76) + (((ptr[0]&0x07e0)>>3) * 151) + (((ptr[0]&0x001f)<<3) * 29)) >> 8) - 128;
								*(y1_ptr + 9) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr = (unsigned short *)addr + 16;
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y2_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*(cb_ptr + 4) = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*(cr_ptr + 4) = (((r << 7) + (g * -107) + (b * -21)) >> 8);
								*(y2_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr =  (unsigned short *)addr + temp_width_add16;
								*(y2_ptr + 8) = (((((ptr[0]&0xf800)>>8) * 76) + (((ptr[0]&0x07e0)>>3) * 151) + (((ptr[0]&0x001f)<<3) * 29)) >> 8) - 128;
								*(y2_ptr + 9) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr =  (unsigned short *)(addr + temp_width_mpy8);
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y3_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*(cb_ptr + 32) = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*(cr_ptr + 32) = (((r << 7) + (g * -107) + (b * -21)) >> 8);
								*(y3_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr = (unsigned short *)(addr + temp_width_mpy9);
								*(y3_ptr + 8) = (((((ptr[0]&0xf800)>>8) * 76) + (((ptr[0]&0x07e0)>>3) * 151) + (((ptr[0]&0x001f)<<3) * 29)) >> 8) - 128;
								*(y3_ptr + 9) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr =  (unsigned short *)(addr + temp_width_mpy8_add16);
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y4_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*(cb_ptr + 36) = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*(cr_ptr + 36) = (((r << 7) + (g * -107) + (b * -21)) >> 8);
								*(y4_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr =  (unsigned short *)(addr + temp_width_mpy9_add16);
								*(y4_ptr + 8) = (((((ptr[0]&0xf800)>>8) * 76) + (((ptr[0]&0x07e0)>>3) * 151) + (((ptr[0]&0x001f)<<3) * 29)) >> 8) - 128;
								*(y4_ptr + 9) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								addr += 4;
								y1_ptr += 2;
								y2_ptr += 2;
								y3_ptr += 2;
								y4_ptr += 2;
								cb_ptr++;
								cr_ptr++;
							}
							addr += temp_width2_sub16;
							y1_ptr += 8;
							y2_ptr += 8;
							y3_ptr += 8;
							y4_ptr += 8;
							cb_ptr += 4;
							cr_ptr += 4;
						}
						if((func_ret = encode_block(y1, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(y2, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(y3, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(y4, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cb, CHROMINANCE, &dc_cb)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cr, CHROMINANCE, &dc_cr)) != FUNC_OK)
						{
							return func_ret;  
						}
					}
		}
		else if (width%16==0 && height%16!=0) //422
		{
               temp_width2_sub16 = (temp_width - 16);
			   {
						for (block_x = 0; block_x < temp_width; block_x += 32) {
						addr = (int)buff + block_x;
						y1_ptr = y1;
						y2_ptr = y2;
						cb_ptr = cb;
						cr_ptr = cr;
						for (y = 0; y < 8; y++) {
							for (x = 0; x < 8; x += 2) {
								int r, g, b;
								register unsigned short *ptr = (unsigned short *)addr;
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y1_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*cb_ptr = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*cr_ptr = (((r << 7) + (g * -107) + (b * -21)) >> 8);
								*(y1_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;

								ptr = (unsigned short *)(addr + 16);
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);
								*y2_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*(cb_ptr + 4) = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*(cr_ptr + 4) = (((r << 7) + (g * -107) + (b * -21)) >> 8);
								*(y2_ptr + 1) = (((((ptr[1]&0xf800)>>8) * 76) + (((ptr[1]&0x07e0)>>3) * 151) + (((ptr[1]&0x001f)<<3) * 29)) >> 8) - 128;
								addr += 4;
								y1_ptr += 2;
								y2_ptr += 2;
								cb_ptr ++;
								cr_ptr ++;
							}
							addr += temp_width2_sub16;
							cb_ptr += 4;
							cr_ptr += 4;
						}
						if((func_ret = encode_block(y1, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(y2, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cb, CHROMINANCE, &dc_cb)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cr, CHROMINANCE, &dc_cr)) != FUNC_OK)
						{
							return func_ret;  
						}
					}
				}
		}
		else//444
		{
			temp_width2_sub16 = (temp_width - 16);
			{

					for (block_x = 0; block_x < temp_width; block_x += 16) {
						addr = (int)buff + block_x;
						y1_ptr = y1;
						cb_ptr = cb;
						cr_ptr = cr;
						for (y = 0; y < 8; y++) {
							for (x = 0; x < 8; x += 2) {
								int r, g, b;
								register unsigned short *ptr = (unsigned short *)addr;
								
								r = (int)((ptr[0]&0xf800)>>8);
								g = (int)((ptr[0]&0x07e0)>>3);
								b = (int)((ptr[0]&0x001f)<<3);

								*y1_ptr = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*cb_ptr = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*cr_ptr = (((r << 7) + (g * -107) + (b * -21)) >> 8);

								r = (int)((ptr[1]&0xf800)>>8);
								g = (int)((ptr[1]&0x07e0)>>3);
								b = (int)((ptr[1]&0x001f)<<3);

								*(y1_ptr + 1) = (((r * 76) + (g * 151) + (b * 29)) >> 8) - 128;
								*(cb_ptr + 1) = (((r * -43) + (g * -85) + (b << 7)) >> 8);
								*(cr_ptr + 1) = (((r << 7) + (g * -107) + (b * -21)) >> 8);

								addr += 4;
								y1_ptr += 2;
								cb_ptr += 2;
								cr_ptr += 2;
							}
							addr += temp_width2_sub16;
						}
						if((func_ret = encode_block(y1, LUMINANCE, &dc_y)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cb, CHROMINANCE, &dc_cb)) != FUNC_OK)
						{
							return func_ret;  
						}
						if((func_ret = encode_block(cr, CHROMINANCE, &dc_cr)) != FUNC_OK)
						{
							return func_ret;  
						}
					}
				}
		}

	return FUNC_OK;
}


const unsigned char head1[20]={
	0xff,0xd8,0xff,0xe0,0x0,0x10,0x4a,0x46,0x49,0x46,0x0,0x1,0x1,
	0x0,0x0,0x1,0x0,0x1,0x0,0x0
};

const unsigned char head_q_l[136]={
   0xff,0xdb,0x0,0x84,0x0,0x10,0xb,0xc,0xe,0xc,0xa,0x10,0xe,0xd,0xe,
   0x12,0x11,0x10,0x13,0x18,0x28,0x1a,0x18,0x16,0x16,0x18,0x31,0x23,
   0x25,0x1d,0x28,0x3a,0x33,0x3d,0x3c,0x39,0x33,0x38,0x37,0x40,0x48,
   0x5c,0x4e,0x40,0x44,0x57,0x45,0x37,0x38,0x50,0x6d,0x51,0x57,0x5f,
   0x62,0x67,0x68,0x67,0x3e,0x4d,0x71,0x79,0x70,0x64,0x78,0x5c,0x65,
   0x67,0x63,0x1,0x11,0x12,0x12,0x18,0x15,0x18,0x2f,0x1a,0x1a,0x2f,
   0x63,0x42,0x38,0x42,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,
   0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,
   0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,
   0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,
   0x63,0x63,0xff,0xc0
};

const unsigned char head_q_h[136]={
	0xff,0xdb,0x0,0x84,0x0,0x2,0x1,0x1,0x1,0x1,0x1,0x2,0x1,0x1,0x1,
	0x2,0x2,0x2,0x2,0x2,0x4,0x3,0x2,0x2,0x2,0x2,0x5,0x4,0x4,0x3,
	0x4,0x6,0x5,0x6,0x6,0x6,0x5,0x6,0x6,0x6,0x7,0x9,0x8,0x6,0x7,
	0x9,0x7,0x6,0x6,0x8,0xb,0x8,0x9,0xa,0xa,0xa,0xa,0xa,0x6,0x8,
	0xb,0xc,0xb,0xa,0xc,0x9,0xa,0xa,0xa,0x1,0x2,0x2,0x2,0x2,0x2,
	0x2,0x5,0x3,0x3,0x5,0xa,0x7,0x6,0x7,0xa,0xa,0xa,0xa,0xa,0xa,
	0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,
	0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,
	0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xff,0xc0
};

unsigned char head_size_128_411[10]={
	0x0,0x11,0x8,0x0,0x60,0x0,0x80,0x3,0x1,0x22,
};


const unsigned char head_4[441]={
	    0x0,0x2,0x11,0x1,0x3,0x11,0x1,
		0xff,0xc4,0x1,0xa2,0x0,0x0,0x1,0x5,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0x10,0x0,0x2,0x1,0x3,0x3,0x2,0x4,
		0x3,0x5,0x5,0x4,0x4,0x0,0x0,0x1,0x7d,0x1,0x2,0x3,0x0,0x4,0x11,0x5,0x12,0x21,0x31,0x41,
		0x6,0x13,0x51,0x61,0x7,0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x8,0x23,0x42,0xb1,0xc1,0x15,
		0x52,0xd1,0xf0,0x24,0x33,0x62,0x72,0x82,0x9,0xa,0x16,0x17,0x18,0x19,0x1a,0x25,0x26,0x27,
		0x28,0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
		0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,
		0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x92,
		0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,
		0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,
		0xca,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xe8,0xe9,0xea,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0x1,0x0,0x3,0x1,0x1,
		0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,
		0x9,0xa,0xb,0x11,0x0,0x2,0x1,0x2,0x4,0x4,0x3,0x4,0x7,0x5,0x4,0x4,0x0,0x1,0x2,0x77,0x0,
		0x1,0x2,0x3,0x11,0x4,0x5,0x21,0x31,0x6,0x12,0x41,0x51,0x7,0x61,0x71,0x13,0x22,0x32,0x81,
		0x8,0x14,0x42,0x91,0xa1,0xb1,0xc1,0x9,0x23,0x33,0x52,0xf0,0x15,0x62,0x72,0xd1,0xa,0x16,
		0x24,0x34,0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,0x27,0x28,0x29,0x2a,0x35,0x36,0x37,
		0x38,0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
		0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
		0x7a,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,
		0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,0xd5,0xd6,
		0xd7,0xd8,0xd9,0xda,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xf2,0xf3,0xf4,0xf5,
		0xf6,0xf7,0xf8,0xf9,0xfa,0xff,0xda,0x0,0xc,0x3,0x1,0x0,0x2,0x11,0x3,0x11,0x0,0x3f,0x0
};
void write_header(short quality_select ,short width,short height)
{

///////////////////////////////////////////////////////////////////////////

	FS_Write(fileHandle,(unsigned char  *)head1 , 20);//fwrite(head1,20,1,pout);// memcpy(&RAM_EXTERN[RAME_HEADER1_ADDR],head1,20);

	if (quality_select==0)
	FS_Write(fileHandle,(unsigned char  *)head_q_l ,136 );//fwrite(head_q_l,136,1,pout);//memcpy(&RAM_EXTERN[RAME_HEAD_Q_ADDR],head_q_l,136);
	else
	FS_Write(fileHandle,(unsigned char  *)head_q_h ,136 );   //fwrite(head_q_h,136,1,pout);//memcpy(&RAM_EXTERN[RAME_HEAD_Q_ADDR],head_q_h,136);


	head_size_128_411[3]=height>>8;
	head_size_128_411[4]=height&0xff;
	head_size_128_411[5]=width>>8;
	head_size_128_411[6]=width&0xff;
	if (width%16!=0) 
	{
	           head_size_128_411[9] = 0x11;
	}
	else if (height%16!=0)//422
	{
		head_size_128_411[9] = 0x21;
	}
	else
	{
		head_size_128_411[9] = 0x22;
	}
	
	FS_Write(fileHandle,head_size_128_411,10);//fwrite(head_size_128_411,10,1,pout);

	FS_Write(fileHandle,(unsigned char  *)head_4 ,441 );//fwrite(head_4,441,1,pout);//memcpy(&RAM_EXTERN[RAME_HEAD4_ADDR],head_4,443);

	             // memcpy(RAM_EXTERN,Buff,TOTAL422_SIZE);
	// fflush(pout);// fwrite(RAM_EXTERN,607,1,pout);
	             // return 0;

	 
	 if (quality_select==1)
	 {
		 quant_table = l_c_quant_table1;	
	 }
	 else
	 {
		 quant_table = l_c_quant_table0;	
	 }
}


void _jpeg_putw(int w)
{
	_jpeg_putc((w >> 8)& 0xff);	
	_jpeg_putc(w & 0xff);
}

void _jpeg_putc(int c)
{
	*chunkptr++ = c;
	if(chunkptr == chunk_end)
	{
		FS_Write(fileHandle,chunk,TEMPOUTBUFSIZE);//fwrite(&chunk,TEMPOUTBUFSIZE,1,pout);
		chunkptr = chunk;
		outputlen+=TEMPOUTBUFSIZE;
	}
}

void _jpeg_new_chunk(int type)
{
	*chunkptr++ = 0xff;
	*chunkptr++ = type;
	chunk_len = 2;
}

void _jpeg_chunk_putw(int w)
{
	_jpeg_chunk_putc((w >> 8) & 0xff);
	_jpeg_chunk_putc(w & 0xff);
}

void _jpeg_chunk_putc(int c)
{
	char *p = (char *)chunkptr + chunk_len;
	
	*p = c;
	chunk_len++;
}

void _jpeg_write_chunk(void)
{
	chunkptr[0] = (chunk_len >> 8) & 0xff;
	chunkptr[1] = chunk_len & 0xff;
	chunkptr += chunk_len;

	current_bit = 8; //7;
	current_byte = 0;
}


/* encode_block:
 *  Encodes an 8x8 basic block of coefficients of given type (luminance or
 *  chrominance) and writes it to the output stream.
 */
int encode_block(short *block, int type, int *old_dc)
{
	//HUFFMAN_TABLE *dc_table, *ac_table;
	const unsigned short *ac_table1,*dc_table1;
	const int *pTempQuanTab;//int *quant_table;
	int index;
	int num_zeroes;
	int category, bits;
	int dc_tmp,value0;
	
	//dc_table = &huffman_dc_table[type];
	//ac_table = &huffman_ac_table[type];
	if (type==0)
	{
        dc_table1 = (unsigned short *)huffman_dc_0;
		ac_table1 = huffman_ac_0+4;
	}
	else
	{
        dc_table1 = huffman_dc_1;
		ac_table1 = huffman_ac_1+4;
	}
	pTempQuanTab = quant_table + 64*type;//quant_table = l_c_quant_table[type];

	
	
	apply_fdct(block);
	dc_tmp = (block[0] * pTempQuanTab[0]) >> 19;	
	value0 = dc_tmp - *old_dc;
	*old_dc = dc_tmp;
	format_number(value0, &category, &bits);
	{
//		HUFFMAN_ENTRY *entry = dc_table->code[category];
//		if(!entry)
//		return VLC_ERROR;
//		put_bits(entry->encoded_value,entry->bits_length);
		put_bits((int)dc_table1[category*2+1],(int)dc_table1[category*2]);

	}
	put_bits(bits, category);

	num_zeroes = 0;
	for (index = 1; index < 64; index++) {
		register int value;
#if 1
		value = block[inverse_Zig_Zag[index]];
		value = ((value * pTempQuanTab[index]) >> 19) + ((value >> 31) & 0x1);
#else
		value = block[inverse_Zig_Zag[index]];
		if (value < 0) {
			value = ((value * quant_table[index]) - (quant_table[index] >> 1) >> 19) + 1;
		}
		else
			value = ((value * quant_table[index]) + (quant_table[index] >> 1)) >> 19;
#endif
		if (value == 0)
			num_zeroes++;
		else {
				while (num_zeroes > 15){
					
				{
					//HUFFMAN_ENTRY *entry = ac_table->code[0xf0];
					//if(!entry)
					//	return VLC_ERROR;					
					//put_bits(entry->encoded_value,entry->bits_length);
					put_bits(ac_table1[-1],ac_table1[-2]);

				}
				num_zeroes -= 16;
			}
			format_number(value, &category, &bits);
			value = (num_zeroes << 4) | category;
			{
//				HUFFMAN_ENTRY *entry = ac_table->code[value];
//				if(!entry)
//					return VLC_ERROR;
//				put_bits(entry->encoded_value,entry->bits_length);
                put_bits((int)ac_table1[num_zeroes*20 + (category-1)*2 + 1],
					(int)ac_table1[num_zeroes*20 + (category-1)*2]);

			}
			put_bits(bits, category);
			num_zeroes = 0;
		}
	}
	if (num_zeroes > 0) {
		{
			//HUFFMAN_ENTRY *entry = ac_table->code[0x0];	
			//if(!entry)
			//	return VLC_ERROR;
			//put_bits(entry->encoded_value,entry->bits_length);
	        put_bits(ac_table1[-3],ac_table1[-4]);
		}
	}
	return FUNC_OK;
}
/* apply_fdct:
 *  Applies the forward discrete cosine transform to the given input block,
 *  in the form of a vector of 64 coefficients.
 *  This uses integer fixed point math and is based on code by the IJG.
 */
void apply_fdct(short *data)
{
	int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	int tmp10, tmp11, tmp12, tmp13;
	int z1, z2, z3, z4, z5;
	register short *dataptr = data;
	int i;
	
	for (i = 8; i; i--) {
		tmp0 = dataptr[0] + dataptr[7];
		tmp7 = dataptr[0] - dataptr[7];
		tmp1 = dataptr[1] + dataptr[6];
		tmp6 = dataptr[1] - dataptr[6];
		tmp2 = dataptr[2] + dataptr[5];
		tmp5 = dataptr[2] - dataptr[5];
		tmp3 = dataptr[3] + dataptr[4];
		tmp4 = dataptr[3] - dataptr[4];
		
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		
		dataptr[0] = (tmp10 + tmp11) << 2;
		dataptr[4] = (tmp10 - tmp11) << 2;
		
		z1 = (tmp12 + tmp13) * FIX_0_541196100;
		dataptr[2] = (z1 + (tmp13 * FIX_0_765366865)) >> 11;
		dataptr[6] = (z1 + (tmp12 * -FIX_1_847759065)) >> 11;
		
		z1 = tmp4 + tmp7;
		z2 = tmp5 + tmp6;
		z3 = tmp4 + tmp6;
		z4 = tmp5 + tmp7;
		z5 = (z3 + z4) * FIX_1_175875602;
		
		tmp4 *= FIX_0_298631336;
		tmp5 *= FIX_2_053119869;
		tmp6 *= FIX_3_072711026;
		tmp7 *= FIX_1_501321110;
		z1 *= -FIX_0_899976223;
		z2 *= -FIX_2_562915447;
		z3 *= -FIX_1_961570560;
		z4 *= -FIX_0_390180644;
		
		z3 += z5;
		z4 += z5;
		
		dataptr[7] = (tmp4 + z1 + z3) >> 11;
		dataptr[5] = (tmp5 + z2 + z4) >> 11;
		dataptr[3] = (tmp6 + z2 + z3) >> 11;
		dataptr[1] = (tmp7 + z1 + z4) >> 11;
		
		dataptr += 8;
	}
	
	dataptr = data;
	for (i = 8; i; i--) {
		tmp0 = dataptr[0] + dataptr[56];
		tmp7 = dataptr[0] - dataptr[56];
		tmp1 = dataptr[8] + dataptr[48];
		tmp6 = dataptr[8] - dataptr[48];
		tmp2 = dataptr[16] + dataptr[40];
		tmp5 = dataptr[16] - dataptr[40];
		tmp3 = dataptr[24] + dataptr[32];
		tmp4 = dataptr[24] - dataptr[32];
		
		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;
		
		dataptr[0] = (tmp10 + tmp11) >> 2;
		dataptr[32] = (tmp10 - tmp11) >> 2;
		
		z1 = (tmp12 + tmp13) * FIX_0_541196100;
		dataptr[16] = (z1 + (tmp13 * FIX_0_765366865)) >> 15;
		dataptr[48] = (z1 + (tmp12 * -FIX_1_847759065)) >> 15;
		
		z1 = tmp4 + tmp7;
		z2 = tmp5 + tmp6;
		z3 = tmp4 + tmp6;
		z4 = tmp5 + tmp7;
		z5 = (z3 + z4) * FIX_1_175875602;
		
		tmp4 *= FIX_0_298631336;
		tmp5 *= FIX_2_053119869;
		tmp6 *= FIX_3_072711026;
		tmp7 *= FIX_1_501321110;
		z1 *= -FIX_0_899976223;
		z2 *= -FIX_2_562915447;
		z3 *= -FIX_1_961570560;
		z4 *= -FIX_0_390180644;
		
		z3 += z5;
		z4 += z5;
		dataptr[56] = (tmp4 + z1 + z3) >> 15;
		dataptr[40] = (tmp5 + z2 + z4) >> 15;
		dataptr[24] = (tmp6 + z2 + z3) >> 15;
		dataptr[8] = (tmp7 + z1 + z4) >> 15;
		
		dataptr++;
	}
}

/* format_number:
 *  Computes the category and bits of a given number.
 */
void format_number(int num, int *category, int *bits)
{
	int abs_num, mask, num_sign;
	register int cat;
	mask = num >> 31;
	abs_num = (num ^ mask) - mask;	
	for (cat = 0; abs_num; cat++)
		abs_num >>= 1;

	*category = cat;

	num_sign = mask & 0x1;//((num>>cat)&0x1);
	*bits = num + (num_sign << cat) - num_sign;
}

/* put_bits:
 *   Writes some bits to the output stream.
 */
void put_bits(int value, int num_bits)
{
	register int crt_bit = current_bit;
	register int crt_byte = current_byte;
	while(num_bits > crt_bit)
	{
		num_bits -= crt_bit;
		crt_byte |= (value >> num_bits);
		*chunkptr++ = crt_byte;
		if(chunkptr == chunk_end)
		{
			FS_Write(fileHandle,chunk,TEMPOUTBUFSIZE);//fwrite(&chunk,TEMPOUTBUFSIZE,1,pout);
			chunkptr = chunk;
			outputlen+=TEMPOUTBUFSIZE;
		}
		if ((crt_byte&0xff) == 0xff)
		{
			*chunkptr++ = 0;
			if(chunkptr == chunk_end)
			{
				FS_Write(fileHandle,chunk,TEMPOUTBUFSIZE);//fwrite(&chunk,TEMPOUTBUFSIZE,1,pout);
				chunkptr = chunk;
				outputlen+=TEMPOUTBUFSIZE;
			}
		}
		crt_bit = 8;
		crt_byte = 0;
	}
	crt_bit -= num_bits;
	crt_byte |= (value << crt_bit);
	current_bit = crt_bit;
	current_byte = crt_byte;
}
/* _jpeg_put_bit:
 *  Writes a bit to the output stream.
*/
int _jpeg_put_bit(int bit)
{
	current_byte |= (bit << current_bit);
	current_bit--;
	if (current_bit < 0) {
		_jpeg_putc(current_byte);
		if (current_byte == 0xff)
			_jpeg_putc(0);
		current_bit = 8;
		current_byte = 0;
	}
	return FUNC_OK;
}
/* _jpeg_flush_bits:
 *  Flushes the current byte by filling unset bits with 1.
 */
void _jpeg_flush_bits(void)
{
	current_bit--;
	while (current_bit <7)
		_jpeg_put_bit(1);
}


int MMF_ISP_VGAto1280_1024New( unsigned short hei, unsigned short wid,  unsigned short* rgb_buf, unsigned short* output_buf )
{
	
	
	int	x,y;
	int	m_min,n_min,m_max,n_max;
	int	R_max_max, G_max_max, B_max_max;
 	int	R_max_min, G_max_min, B_max_min;
	int	R_min_max, G_min_max, B_min_max;
	int	R_min_min, G_min_min, B_min_min;
	int	piex_min_min;
	int	piex_min_max;
	int	piex_max_min;
	int	piex_max_max;
	int 	R, G, B;
	int 	row_min, row_max;
	int 	y_temp;
	static int  line_temp = 0;
	static int  a = 0;
    static int  u = 0;

	//printf("line_temp=%d      a=%d      u=%d      ", line_temp, a, u);

	if( rgb_buf==NULL)  return ISP_DATA_ERROR;
	if( (hei!=480)  || (wid!=640) )
	{
		return ISP_DATA_ERROR;
	}
	

	for( y=line_temp; y<line_temp+16; y++ )
	{		
		if( y==a )
		{
			y_temp = y-u;
			u++;
			a = a+16;
		}
		else
		{
			y_temp = y-u;
		}
			
		if( (y_temp & 0x0001)==0 )
		{
			m_min  = (y_temp>>1);
			m_max  = m_min;

			row_min  = m_min*wid;
			row_max  = row_min;
		}
		else
		{
			m_min  =  ((y_temp-1)>>1);
			m_max =  m_min+1;

			row_min  = m_min*wid;
			row_max = row_min+wid;
		}
			
		for ( x=0; x<1280; x++ ) 
		{
			if( (x & 0x0001)==0 )
			{
				n_min  =  (x>>1);

				piex_min_min	=	(int)(*(rgb_buf +row_min+n_min));
				piex_max_min	=	(int)(*(rgb_buf +row_max+n_min));

				R_min_min	=	(int)(piex_min_min>>11);
				R_min_max	=	R_min_min;
				R_max_min	=	(int)(piex_max_min>>11);
				R_max_max	=	R_max_min;

				G_min_min	= 	(int)((piex_min_min & 0x07ff)>>5);
				G_min_max	=	G_min_min;
				G_max_min	=	(int)((piex_max_min & 0x07ff)>>5);
				G_max_max	=	G_max_min;

				B_min_min	=	(int)(piex_min_min & 0x001f);
				B_min_max	=	B_min_min;
				B_max_min	=	(int)(piex_max_min & 0x001f);
				B_max_max	=	B_max_min;
			
			}
			else
			{
				n_min  =  ((x-1)>>1);
				n_max =  n_min+1;

				piex_min_min	=	(int)(*(rgb_buf +row_min+n_min));
				piex_min_max	=	(int)(*(rgb_buf +row_min+n_max));
				piex_max_min	=	(int)(*(rgb_buf +row_max+n_min));
				piex_max_max	=	(int)(*(rgb_buf +row_max+n_max));

				R_min_min	=	(int)(piex_min_min>>11);
				R_min_max	=	(int)(piex_min_max>>11);
				R_max_min	=	(int)(piex_max_min>>11);
				R_max_max	=	(int)(piex_max_max>>11);

				G_min_min	= 	(int)((piex_min_min & 0x07ff)>>5);
				G_min_max	=	(int)((piex_min_max & 0x07ff)>>5);
				G_max_min	=	(int)((piex_max_min & 0x07ff)>>5);
				G_max_max	=	(int)((piex_max_max & 0x07ff)>>5);

				B_min_min	=	(int)(piex_min_min & 0x001f);
				B_min_max	=	(int)(piex_min_max & 0x001f);
				B_max_min	=	(int)(piex_max_min & 0x001f);
				B_max_max	=	(int)(piex_max_max & 0x001f);


			}

			if( (y_temp & 0x0001)==0 )
			{
				R	=	( R_min_min + R_min_max ) >>1;
				G	=	( G_min_min + G_min_max) >>1;
				B	=	( B_min_min + B_min_max ) >>1;	
			}
			else if( (x & 0x0001)==0 )
			{
				R	=	( R_min_min + R_max_min ) >>1;
				G	=	( G_min_min + G_max_min ) >>1;
				B	=	( B_min_min + B_max_min ) >>1;
			}
			else
			{
				R	=	( R_min_min + R_min_max + R_max_min  + R_max_max) >>2;
				G	=	( G_min_min + G_min_max + G_max_min + G_max_max ) >>2;
				B	=	( B_min_min + B_min_max  + B_max_min + B_max_max) >>2;
			}
			output_buf[(y-line_temp)*1280+x]	=	((unsigned short)R<<11) |(((unsigned short)G<<5) & 0x07ff) | ((unsigned short)B & 0x001f);
		}

	}
	line_temp = line_temp+16;

	if(line_temp == 1024)
	{
		line_temp=0;
	}
	
	if( a==1024)
	{
		a=0;
	}

	if( u==64)
	{
		u=0;
	}
	
	return ISP_FUN_OK;
}
