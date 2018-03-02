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


#ifndef _PNG_H
#define _PNG_H

#include "mmi_png_dec.h"
#ifndef SHEEN_VC_DEBUG
//#include "lcd_sw_inc.h"
#endif

//typedef long INT32;
//typedef unsigned short UINT16;


// 错误码定义
#define PNG_NO_ERROR 0
// 关键块
#define IHDRERR 1
#define IHDRCRCERR 2
#define PLTEERR 3
#define PLTECRCERR 4
#define IDATERR 5
#define IDATCRCERR 6
#define IENDERR 7
#define IENDCRCERR 8

// 辅助块
#define gAMAERR 9
#define gAMACRCERR 10
#define tRNSERR 11
#define tRNSCRCERR 12
#define sBITERR 13
#define sBITCRCERR 14
#define bKGDERR 15
#define bKGDCRCERR 16
#define cHRMERR 17
#define cHRMCRCERR 18
#define hISTERR 19
#define hISTCRCERR 20
#define iCCPERR 21
#define iCCPCRCERR 22
#define iTXtERR 23
#define iTXtCRCERR 24
#define pHYsERR 25
#define pHYsCRCERR 26
#define sPLTERR 27
#define sPLTCRCERR 28
#define sRGBERR 29
#define sRGBCRCERR 30
#define tEXtERR 31
#define tEXtCRCERR 32
#define tIMEERR 33
#define tIMECRCERR 34
#define zTXtERR 35
#define zTXtCRCERR 36
#define UnknownERR 37
#define UnknownCRCERR 38
#define ChunkExtra 39

#define INFLATEERR 40
#define RESIZEERR 41
#define INPUTERR 42
#define MEMERR 43
#define SUPPORTERR 44
#define CLIPOVER 45 //clip area decode over,not an err.sheen

// 色彩类型
#define GreyScale 0
#define TrueColor 2
#define IndexColor 3
#define GreyScaleAlpha 4
#define TrueColorAlpha 6

// sRGB
#define sRGBPerceptual 0
#define sRGBRelative 1
#define sRGBSaturation 2
#define sRGBAbsolute 3
#define sRGBRenderingLast 4

#define sRGBgAMA 45455
#define sRGBWhiteX 31270
#define sRGBWhiteY 32900
#define sRGBRedX 64000
#define sRGBRedY 33000
#define sRGBGreenX 30000
#define sRGBGreenY 60000
#define sRGBBlueX 15000
#define sRGBBlueY 6000

// 图像数据变换
#define EXPAND 0x01
#define DEPTH16to8 0x02
#define tRNSEXPAND 0x04
#define GREY2RGB 0x08
#define bKGDEXPAND 0x10
#define gAMAEXPAND 0x20
#define INTERLACE 0x40

#define DEFAULTbKGD 0x80
          
  //in coolsand plarform, we support back ground image 
  //that would replace the default back ground colour  added by elisa 2009-10-9
#define BKGDImage   0x100 

// Filter Types
#define FilterTypeNone 0
#define FilterTypeSub 1
#define FilterTypeUp 2
#define FilterTypeAverage 3
#define FilterTypePaeth 4

// just set sizes of png are smaller than screen
#define MaxWidth 1280//LCD_WIDTH//
#define MaxHeight 1024//LCD_HEIGHT//
#define MaxPixelBytes 2

#define ROWBYTES(pixel_bits, width) \
    ((pixel_bits) >= 8 ? \
    ((width) * (((UINT32)(pixel_bits)) >> 3)) : \
    (( ((width) * ((UINT32)(pixel_bits))) + 7) >> 3) )

#define composite(composite, fg, alpha, bg)                            \
	(composite) = (BYTE)(((UINT16)(fg) * (UINT16)(alpha) +    \
(UINT16)(bg) * (UINT16)(255 - (UINT16)(alpha))+(UINT16)128)>>8)

#define composite16(composite, fg, alpha, bg)                         \
	(composite) = (UINT32)(((UINT32)(fg) * (UINT32)(alpha) + \
(UINT32)(bg)*(UINT32)(65535L - (UINT32)(alpha))+(UINT32)32768)>>16)


#pragma pack(1)

typedef struct  
{
	UINT32 Width;
	UINT32 Height;
	UINT8 BitDepth;
	UINT8 ColorType;
	UINT8 Compression;
	UINT8 Fileter;
	UINT8 Interlace;
} IHDRCHUNK;

typedef struct  
{
	UINT8 Red;
	UINT8 Green;
	UINT8 Blue;
} PLTECHUNK;

typedef struct
{
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE grey;
	BYTE alpha;
} sBITInfo;

typedef struct  
{
	BYTE First;
	BYTE Second;
	BYTE Third;
	BYTE Forth;
} CHUNKTYPE;

typedef struct  
{
	UINT16 tRNSGrey;
	UINT16 tRNSRed;
	UINT16 tRNSGreen;
	UINT16 tRNSBlue;	
} TRANSINFO;

typedef struct  
{
	UINT32 xPixPerUint;
	UINT32 yPixPerUnit;
	BYTE UnitSpec;
} pHYsINFO;

typedef struct  
{
	UINT8 IHDRNum;
	UINT8 PLTENum;
	UINT8 IDATNum;
	UINT8 IENDNum;
	UINT8 cHRMNum;
	UINT8 gAMANum;
	UINT8 iCCPNum;
	UINT8 sBITNum;
	UINT8 sRGBNum;
	UINT8 bKGDNuM;
	UINT8 hISTNum;
	UINT8 tRNSNum;
	UINT8 pHYsNum;
	UINT8 sPLTNum;
	UINT8 tIMENum;
	UINT8 iTXtNum;
	UINT8 tEXtNum;
	UINT8 zTXtNum;
} FILECHUNKS;

typedef struct  
{
	UINT32 WhiteX;
	UINT32 WhiteY;
	UINT32 RedX;
	UINT32 RedY;
	UINT32 GreenX;
	UINT32 GreenY;
	UINT32 BlueX;
	UINT32 BlueY; 
} cHRMINFO;

typedef struct  
{
	UINT16 bKGDGrey;
	UINT16 bKGDRed;
	UINT16 bKGDGreen;
	UINT16 bKGDBlue;
	BYTE bKGDPlteIndex;
} bKGDINFO;

typedef struct  
{
	char sPLTPaltName[80];
	BYTE sPLTSampDepth;
	UINT16 sPLTEntryNum;
	BYTE sPLTEntrySize;
} sPLTINFO;

typedef struct  
{
	UINT16 sPLTRed;
	UINT16 sPLTGreen;
	UINT16 sPLTBlue;
	UINT16 sPLTAlpha;
	UINT16 sPLTFreq;
} sPLTPALETTE;

typedef struct
{
	UINT32 Width; /* width of row */
	UINT32 RowBytes; /* number of bytes in row */
	BYTE ColorType; /* color type of row */
	BYTE BitDepth; /* bit depth of row */
	BYTE Channels; /* number of channels (1, 2, 3, or 4) */
	BYTE PixelDepth; /* bits per pixel (depth * channels) */
} ROWINFO;

#pragma pack()

int IHDRHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int cHRMHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int gAMAHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int iCCPHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int sBITHandle(BYTE **PngFileContent, UINT32 ChunkLength, sBITInfo *SigBit);
int sRGBHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int PLTEHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int bKGDHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int hISTHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int tRNSHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int pHYsHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int sPLTHandle(BYTE **PngFileContent, UINT32 ChunkLength);
// int IDATHandle(Byte **PngFileContent, UINT32 ChunkLength, PLTECHUNK **ppBkgColor);
int IDATHandle(BYTE **PngFileContent, UINT32 ChunkLength, BYTE *ImagaData, UINT32 OutBufferSize);
int tIMEHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int iTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int tEXtHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int zTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int IENDHandle(BYTE **PngFileContent, UINT32 ChunkLength);
int UnknownHandle(BYTE **PngFileContent, UINT32 ChunkLength);

void UpdateInfo();
int ProcessOneRow(BYTE **PngFileContent, BYTE* ImageRowData, BYTE SkipRow);
void ResizeOneRow(BYTE * Src_Buf,UINT32 Src_Width,BYTE * Dec_Buf,UINT32 Dec_Width);
void GenOneCol(BYTE * Pre_Buf,BYTE * Cur_Buf,UINT32 Dec_Width,BYTE * Dec_Buf,INT32 Wn);


#endif

