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



#include "string.h"

#if 0 // support png later

#ifndef SHEEN_VC_DEBUG
//#include <memory.h>
#include "cos.h"

#include "cs_types.h"
//#include "dsm_cf.h"
#include "ts.h"
#endif

#include "zlib.h"
#include "png.h"
#include "mmi_png_dec.h"

#ifndef SHEEN_VC_DEBUG
#include "fs_type.h"
#include "oslmemory.h"
#include "mmi_trace.h"
#include "imagep_debug.h"
#include "gdi_include.h"
#endif

IHDRCHUNK IHDRInfo;   // IHDR信息
UINT8 ImgChannels;    // 图像通道数
INT32 gammer;         // 图像gammer值
PLTECHUNK Palete[256];   // 图像色表
UINT32 PlteSize;         // 色表大小
UINT32 EntryNum;         // 色表项数
pHYsINFO pHYsInfo;       // pHYs信息
FILECHUNKS FileChunks;   // PNG文件中出现的各个chunk

UINT16 *hISTContent = NULL;
sPLTPALETTE *sPLTPalette = NULL;

UINT16 TransNum;        // 透明色数量
TRANSINFO tRNSInfo;     // 非索引图像tRNS信息
BYTE *tRNSIndex = NULL; // 索引图像透明信息
bKGDINFO bKGDInfo;      // 背景块信息
PLTECHUNK DefaultBkgd;  // 背景
cHRMINFO cHRMInfo;      // cHRM信息
sPLTINFO sPLTInfo;      // sPLT信息
BYTE sRGBRenderContent; // sRGB信息
UINT32 RowBytes;        // 原始图像一行的字节数
UINT32 transform = 0;   // 变换信息
UINT8 ImagePass;        // 图像Pass数
UINT32 IDATSize;        // IDAT块数据大小
UINT8 PixelDepth;       // 像素比特深度
z_stream PNGzstream;

//BYTE BufferArray[MaxWidth * MaxHeight * MaxPixelBytes];
//BYTE* RowArray[MaxHeight];

BYTE tRNSIndexArray[256];

extern Png_StreamBuf PngStreamBuf;
extern U8	*rgb_output_ptr;      //RGA565 / alphaRGA output addr
extern U8	*alpha_output_ptr;      //alpha output addr
extern U8	*bkgrd_ptr;      //RGB565 background addr
extern U8	*rgb_temp_ptr;// temp RGB565alpha for resize or interlace.

extern INT16 PngCutOffsetX1;	//cut area x start offset in png
extern INT16 PngCutOffsetY1;	//cut area y start offset in png
extern INT16 PngCutOffsetX2;	//cut area x end offset in png
extern INT16 PngCutOffsetY2;	//cut area y end offset in png
extern U16 png_decode_mode;
extern INT16 png_resized_width;
extern INT16 png_resized_height;
extern INT16	png_bkgrd_width;
extern INT16	png_bkgrd_height;
extern U8 set_default_alpha;//all alpha set 255 if need

int IHDRHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 IHDRcrc = 0;
	UINT32 FileIHDRcrc = 0;

	FileChunks.IHDRNum++;

	if (FileChunks.IHDRNum > 1 || ChunkLength != 13)
	{
		return IHDRERR;
	}

	IHDRcrc = crc32(IHDRcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	FileIHDRcrc = (*(*PngFileContent + 13) << 24) + ((*(*PngFileContent + 14)) << 16)
			+ ((*(*PngFileContent + 15)) << 8) + (*(*PngFileContent + 16));

	if ( IHDRcrc != FileIHDRcrc)
	{
		return IHDRCRCERR;
	}

	IHDRInfo.Width = (*(*PngFileContent) << 24) + ((*(*PngFileContent + 1)) << 16)
			+ ((*(*PngFileContent + 2)) << 8) + (*(*PngFileContent + 3));
	IHDRInfo.Height = (*(*PngFileContent + 4) << 24) + ((*(*PngFileContent + 5)) << 16)
			+ ((*(*PngFileContent + 6)) << 8) + (*(*PngFileContent + 7));
	IHDRInfo.BitDepth = (*PngFileContent)[8];
	IHDRInfo.ColorType = (*PngFileContent)[9];
	IHDRInfo.Compression = (*PngFileContent)[10];
	IHDRInfo.Fileter = (*PngFileContent)[11];
	IHDRInfo.Interlace = (*PngFileContent)[12];

	switch(IHDRInfo.ColorType)
	{
	case GreyScale:
	case IndexColor:
		ImgChannels = 1;
		break;

	case TrueColor:
		ImgChannels = 3;
		break;

	case GreyScaleAlpha:
		ImgChannels = 2;	
		break;

	case TrueColorAlpha:
		ImgChannels = 4;	
		break;
	}

	PixelDepth = IHDRInfo.BitDepth * ImgChannels;

	RowBytes = ROWBYTES(PixelDepth, IHDRInfo.Width);

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=(ChunkLength +4);
	return 0;
}

int cHRMHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 cHRMcrc = 0;
	UINT32 FilecHRMcrc = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before cHRM!");
		return cHRMERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid cHRM after PLTE!");
		return cHRMERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild cHRM after IDAT!");
		return cHRMERR;
	}
	else if (FileChunks.cHRMNum > 1)	
	{
		//printf("Duplicate cHRM chunk!");
		return cHRMERR;
	}
	else if (ChunkLength != 32)
	{
		//printf("Invalid cHRM chunk length!");
		return cHRMERR;
	}
	
	FileChunks.cHRMNum++;
	
	cHRMcrc = crc32(cHRMcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	
	FilecHRMcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (cHRMcrc != FilecHRMcrc)
	{
		return cHRMCRCERR;
	}

	if (FileChunks.sRGBNum == 0)
	{
		cHRMInfo.WhiteX = (*(*PngFileContent) << 24) + ((*(*PngFileContent + 1)) << 16)
			+ ((*(*PngFileContent + 2)) << 8) + (*(*PngFileContent + 3));
		cHRMInfo.WhiteY = ((*(*PngFileContent + 4)) << 24) + ((*(*PngFileContent + 5)) << 16)
			+ ((*(*PngFileContent + 6)) << 8) + (*(*PngFileContent + 7));
		cHRMInfo.RedX = ((*(*PngFileContent + 8)) << 24) + ((*(*PngFileContent + 9)) << 16)
			+ ((*(*PngFileContent + 10)) << 8) + (*(*PngFileContent + 11));
		cHRMInfo.RedY = ((*(*PngFileContent + 12)) << 24) + ((*(*PngFileContent + 13)) << 16)
			+ ((*(*PngFileContent + 14)) << 8) + (*(*PngFileContent + 15));
		cHRMInfo.GreenX = ((*(*PngFileContent + 16)) << 24) + ((*(*PngFileContent + 17)) << 16)
			+ ((*(*PngFileContent + 18)) << 8) + (*(*PngFileContent + 19));
		cHRMInfo.GreenY = ((*(*PngFileContent + 20)) << 24) + ((*(*PngFileContent + 21)) << 16)
			+ ((*(*PngFileContent + 22)) << 8) + (*(*PngFileContent + 23));
		cHRMInfo.BlueX = ((*(*PngFileContent + 24)) << 24) + ((*(*PngFileContent + 25)) << 16)
			+ ((*(*PngFileContent + 26)) << 8) + (*(*PngFileContent + 27));
		cHRMInfo.BlueY =((*(*PngFileContent + 28)) << 24) + ((*(*PngFileContent + 29)) << 16)
			+ ((*(*PngFileContent + 30)) << 8) + (*(*PngFileContent + 31));
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int gAMAHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 gAMAcrc = 0;
	UINT32 FilegAMAcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before gAMA!");
		return gAMAERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid gAMA after PLTE!");
		return gAMAERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild gAMA after IDAT!");
		return gAMAERR;
	}
	else if (FileChunks.gAMANum > 1)	
	{
		//printf("Duplicate gAMA chunk!");
		return gAMAERR;
	}
	else if (ChunkLength != 4)
	{
		//printf("Invalid gAMA chunk length!");
		return gAMAERR;
	}
		
	FileChunks.gAMANum++;

	gAMAcrc = crc32(gAMAcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilegAMAcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));

	if (gAMAcrc != FilegAMAcrc)
	{
		return gAMACRCERR;
	}

	if (FileChunks.sRGBNum == 0)
	{
		gammer = (*(*PngFileContent) << 24) + (*((*PngFileContent) + 1) << 16)
			+ (*((*PngFileContent) + 2) << 8) + (*((*PngFileContent) + 3));
		
		if ( gammer > 0x7fffffffL)
		{
			gammer = 0x7fffffffL;
		}
		else if (gammer < 0)
		{
			gammer = 0;
		}
	}

	*PngFileContent += 4 + ChunkLength;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int iCCPHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 iCCPcrc = 0;
	UINT32 FileiCCPEcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before iCCP!");
		return iCCPERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid iCCP after PLTE!");
		return iCCPERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild iCCP after IDAT!");
		return iCCPERR;
	}
	else if (FileChunks.iCCPNum > 1)	
	{
		//printf("Duplicate iCCP chunk!");
		return iCCPERR;
	}
	else if (FileChunks.sRGBNum != 0)
	{
		//printf("There is a sRGB chunk!");
		return iCCPERR;
	}

	FileChunks.iCCPNum++;

	iCCPcrc = crc32(iCCPcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileiCCPEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (iCCPcrc != FileiCCPEcrc)
	{
		return iCCPCRCERR;
	}

// ..................

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sBITHandle(BYTE **PngFileContent, UINT32 ChunkLength, sBITInfo *SigBit)
{
	UINT32 sBITcrc = 0;
	UINT32 FilesBITcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sBIT!");
		return sBITERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid sBIT after PLTE!");
		return sBITERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild sBIT after IDAT!");
		return sBITERR;
	}
	else if (FileChunks.sBITNum > 1)	
	{
		//printf("Duplicate sBIT chunk!");
		return sBITERR;
	}

	FileChunks.sBITNum++;

	sBITcrc = crc32(sBITcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesBITcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sBITcrc != FilesBITcrc)
	{
		return sBITCRCERR;
	}

	if (IHDRInfo.ColorType == GreyScale)
	{
		if (ChunkLength != 1)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}
		
		SigBit->grey = **PngFileContent;
	}
	else if (IHDRInfo.ColorType == TrueColor || IHDRInfo.ColorType == IndexColor)
	{
		if (ChunkLength != 3)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->red = **PngFileContent;
		SigBit->green = *(*PngFileContent + 1);
		SigBit->blue = *(*PngFileContent + 2);
	}
	else if (IHDRInfo.ColorType == GreyScaleAlpha)
	{
		if (ChunkLength != 2)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->grey = **PngFileContent;
		SigBit->alpha = *(*PngFileContent + 1);
	}
	else if (IHDRInfo.ColorType == TrueColorAlpha)
	{
		if (ChunkLength != 4)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->red = **PngFileContent;
		SigBit->green = *(*PngFileContent + 1);
		SigBit->blue = *(*PngFileContent + 2);
		SigBit->alpha = *(*PngFileContent + 3);
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sRGBHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 sRGBcrc = 0;
	UINT32 FilesRGBcrc = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sRGB!");
		return sRGBERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid sRGB after PLTE!");
		return sRGBERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild sRGB after IDAT!");
		return sRGBERR;
	}
	else if (FileChunks.sRGBNum > 1)	
	{
		//printf("Duplicate sRGB chunk!");
		return sRGBERR;
	}
	else if (FileChunks.iCCPNum != 0)
	{
		//printf("There is a iCCP chunk!");
		return sRGBERR;
	}
	else if (ChunkLength != 1)
	{
		//printf("Invalid chunk length!");
		return sRGBERR;
	}
		
	FileChunks.sRGBNum++;
	
	sRGBcrc = crc32(sRGBcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesRGBcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sRGBcrc != FilesRGBcrc)
	{
		return sRGBCRCERR;
	}

	sRGBRenderContent = **PngFileContent;

	if (sRGBRenderContent > sRGBRenderingLast)
	{
		//printf("Unknown rendering content!");
		return sRGBERR;
	}

	gammer = 45455;

	cHRMInfo.WhiteX = 31270;
	cHRMInfo.WhiteY = 32900;
	cHRMInfo.RedX = 64000;
	cHRMInfo.RedY = 33000;
	cHRMInfo.GreenX = 30000;
	cHRMInfo.GreenY = 60000;
	cHRMInfo.BlueX = 15000;
	cHRMInfo.BlueY = 6000;

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int PLTEHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 PLTEcrc = 0;
	UINT32 FilePLTEcrc = 0;
	UINT32 i;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before PLTE!");
		return PLTEERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid PLTE after IDAT!");
		return PLTEERR;
	}
	else if (FileChunks.PLTENum > 1)
	{
		//printf("Duplicate PLTE chunk!");
		return PLTEERR;
	}
	else if (ChunkLength > 768 || ChunkLength % 3)
	{
		//printf("Invalid PLTE chunk length!");
		return PLTEERR;
	}

	FileChunks.PLTENum++;
	
	PLTEcrc = crc32(PLTEcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilePLTEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (PLTEcrc != FilePLTEcrc)
	{
		return PLTECRCERR;
	}

	EntryNum = ChunkLength / 3;
	for (i = 0; i < EntryNum; i++)
	{
		Palete[i].Red = *(*PngFileContent)++;
		Palete[i].Green = *(*PngFileContent)++;
		Palete[i].Blue = *(*PngFileContent)++;
	}

	PlteSize = ChunkLength;
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	return 0;
}

int bKGDHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 bKGDcrc = 0;
	UINT32 FilebKGDcrc = 0;
	UINT32 TrueLength = 0;
	
/*	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before bKGD!");
		return bKGDERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before bKGD!");
		return bKGDERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid bKGD after IDAT!");
		return bKGDERR;
	}
	else if (FileChunks.bKGDNuM > 1)
	{
		//printf("Duplicate bKGD chunk!");
		return bKGDERR;
	}
*/	
	FileChunks.bKGDNuM++;
	
	bKGDcrc = crc32(bKGDcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilebKGDcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (bKGDcrc != FilebKGDcrc)
	{
		return bKGDCRCERR;
	}

//	bKGDInfo = (bKGDINFO *)OslMalloc(9);

	if (IHDRInfo.ColorType == GreyScale || IHDRInfo.ColorType == GreyScaleAlpha)
	{
		TrueLength = 2;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}
		bKGDInfo.bKGDRed =
		bKGDInfo.bKGDGreen =
		bKGDInfo.bKGDBlue =
		bKGDInfo.bKGDGrey = (*(*PngFileContent) << 8) + *((*PngFileContent) + 1);
	}
	else if (IHDRInfo.ColorType == TrueColor || IHDRInfo.ColorType == TrueColorAlpha)
	{
		TrueLength = 6;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}
		bKGDInfo.bKGDRed = (*(*PngFileContent) << 8) + *((*PngFileContent) + 1);
		bKGDInfo.bKGDGreen = (*((*PngFileContent) + 2) << 8) + *((*PngFileContent) + 3);
		bKGDInfo.bKGDBlue = (*((*PngFileContent) + 4) << 8) + *((*PngFileContent) + 5);
	}
	else if (IHDRInfo.ColorType == IndexColor)
	{
		TrueLength = 1;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}

		bKGDInfo.bKGDPlteIndex = **PngFileContent;
		if (PlteSize)
		{
			if (bKGDInfo.bKGDPlteIndex > PlteSize)
			{
				//printf("Incorrect bKGD chunk index value!");
				return bKGDERR;
			}

			bKGDInfo.bKGDRed = Palete[bKGDInfo.bKGDPlteIndex].Red;
			bKGDInfo.bKGDGreen = Palete[bKGDInfo.bKGDPlteIndex].Green;
			bKGDInfo.bKGDBlue = Palete[bKGDInfo.bKGDPlteIndex].Blue;
		}
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int hISTHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 hISTcrc = 0;
	UINT32 FilehISTcrc = 0;
	UINT16 hISTEntryNum;
	BYTE buf[2];
	UINT16 i;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before hIST!");
		return hISTERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before hIST!");
		return hISTERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid hIST after IDAT!");
		return hISTERR;
	}
	else if (FileChunks.hISTNum > 1)
	{
		//printf("Duplicate hIST chunk!");
		return hISTERR;
	}

	FileChunks.hISTNum++;

	hISTcrc = crc32(hISTcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilehISTcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (hISTcrc != FilehISTcrc)
	{
		return hISTCRCERR;
	}

	hISTEntryNum = ChunkLength / 2;
	if (hISTEntryNum != PlteSize || hISTEntryNum > 256)
	{
		//printf("Incorrect hIST chunk length!");
		return hISTERR;
	}

	hISTContent = (UINT16 *)OslMalloc(ChunkLength);

//	memcpy(hISTContent, PngFileContent, ChunkLength);
//	*PngFileContent += ChunkLength + 4;

	for (i = 0; i < hISTEntryNum; i++)
	{
		buf[0] = *(*PngFileContent)++;
		buf[1] = *(*PngFileContent)++;

		hISTContent[i] = (buf[0] << 8) + buf[1]; 
	}

	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	return 0;
}

int tRNSHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tRNScrc = 0;
	UINT32 FiletRNScrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before tRNS!");
		return tRNSERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before tRNS!");
		return tRNSERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid tRNS after IDAT!");
		return tRNSERR;
	}
	else if (FileChunks.tRNSNum > 1)
	{
		//printf("Duplicate tRNS chunk!");
		return tRNSERR;
	}

	FileChunks.tRNSNum++;

	tRNScrc = crc32(tRNScrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletRNScrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tRNScrc != FiletRNScrc)
	{	
		return PLTECRCERR;
	}

//	(*TransInfo).TransNum = (UINT16 *)OslMalloc(2);

	if (IHDRInfo.ColorType == GreyScale)
	{
		if (ChunkLength != 2)
		{
			//printf("Invalid tRNS chunk length!");
			return tRNSERR;
		}
		
		TransNum = 1;

		tRNSInfo.tRNSGrey = (**PngFileContent << 8) + *(*PngFileContent + 1);
	}
	else if (IHDRInfo.ColorType == TrueColor)
	{
		if (ChunkLength != 6)
		{
			//printf("Invalid tRNS chunk length!");	
			return tRNSERR;
		}

		TransNum = 1;

		tRNSInfo.tRNSRed = (**PngFileContent << 8) + *(*PngFileContent + 1);
		tRNSInfo.tRNSGreen = (*(*PngFileContent + 2) << 8) + *(*PngFileContent + 3);
		tRNSInfo.tRNSBlue = (*(*PngFileContent + 4) << 8) + *(*PngFileContent + 5);
	}
	else if (IHDRInfo.ColorType == IndexColor)
	{
		if (ChunkLength > PlteSize || ChunkLength == 0)
		{
			//printf("Invalid tRNS chunk length!");	
			return tRNSERR;
		}
		
		TransNum = ChunkLength;

//		tRNSIndex = (BYTE *)OslMalloc(ChunkLength);
		tRNSIndex = tRNSIndexArray;
		memcpy(tRNSIndex, *PngFileContent, ChunkLength);
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int pHYsHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 pHYscrc = 0;
	UINT32 FilepHYscrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before pHYs!");
		return pHYsERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid pHYs after IDAT!");
		return pHYsERR;
	}
	else if (FileChunks.pHYsNum > 1)
	{
		//printf("Duplicate pHYs chunk!");
		return pHYsERR;
	}
	else if (ChunkLength != 9)
	{
		//printf("Invalid pHYs chunk length!");
		return pHYsERR;
	}
	
	FileChunks.pHYsNum++;

	pHYscrc = crc32(pHYscrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilepHYscrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (pHYscrc != FilepHYscrc)
	{
		return pHYsCRCERR;
	}

	pHYsInfo.xPixPerUint = (*(*PngFileContent) << 24) + (*((*PngFileContent) + 1) << 16)
		+ (*((*PngFileContent) + 2) << 8) + (*((*PngFileContent) + 3));
	pHYsInfo.yPixPerUnit = (*((*PngFileContent) + 4) << 24) + (*((*PngFileContent) + 5) << 16)
		+ (*((*PngFileContent) + 6) << 8) + (*((*PngFileContent) + 7));
	pHYsInfo.UnitSpec = *((*PngFileContent) + 8);

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sPLTHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 sPLTcrc = 0;
	UINT32 FilesPLTcrc = 0;
//	int i;

//	BYTE *EntryStart = *PngFileContent;
//	UINT32 DataLength = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sPLT!");
		return sPLTERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid sPLT after IDAT!");
		return sPLTERR;
	}
	
	FileChunks.sPLTNum++;
	
	sPLTcrc = crc32(sPLTcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesPLTcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sPLTcrc != FilesPLTcrc)
	{
	return sPLTCRCERR;
	}
	
#if 0
	for (i = 0; *EntryStart; EntryStart++, i++)
	{
		if (i > 79)
		{
			//printf("sPLT name too long!");
			return sPLTERR;
		}

		sPLTInfo.sPLTPaltName[i] = *EntryStart;
	}

	EntryStart++;
	if (EntryStart > *PngFileContent + ChunkLength)
	{
		//printf("sPLT chunk format incorrect!");
		return sPLTERR;
	}

	sPLTInfo.sPLTSampDepth = *EntryStart++;
	sPLTInfo.sPLTEntrySize = (sPLTInfo.sPLTSampDepth == 8 ? 6 : 10);

	DataLength = ChunkLength - (EntryStart - *PngFileContent);

	if (DataLength % sPLTInfo.sPLTEntrySize)
	{
		//printf("sPLT chunk length is wrong!");
		return sPLTERR;
	}
	
	sPLTInfo.sPLTEntryNum = DataLength / sPLTInfo.sPLTEntrySize;

	if ((sPLTPalette = (sPLTPALETTE *)OslMalloc(DataLength)) == NULL)
	{
		//printf("Fail to alloc memory for sPLTPalette!");
		return sPLTERR;
	}

	for (i = 0; i < sPLTInfo.sPLTEntryNum; i++)
	{
		if (sPLTInfo.sPLTSampDepth == 8)
		{
			sPLTPalette->sPLTRed = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTGreen = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTBlue = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTAlpha = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTFreq = (*EntryStart << 8) + (*(EntryStart + 1));
			EntryStart += 2;
		}
		else
		{
			sPLTPalette->sPLTRed = (*EntryStart << 8) + (*(EntryStart + 1));
			sPLTPalette->sPLTGreen = ((*(EntryStart + 2)) << 8) + (*(EntryStart + 3));
			sPLTPalette->sPLTBlue = ((*(EntryStart + 4)) << 8) + (*(EntryStart + 5));
			sPLTPalette->sPLTAlpha = ((*(EntryStart + 6)) << 8) + (*(EntryStart + 7));
			sPLTPalette->sPLTFreq = ((*(EntryStart + 8)) << 8) + (*(EntryStart + 9));
			EntryStart += 10;
		}
	}
#endif

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

//int IDATHandle(BYTE **PngFileContent, UINT32 ChunkLength, PLTECHUNK **ppBkgColor)
extern INT MMI_FS_Read(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen);


int IDATHandle(BYTE **PngFileContent, UINT32 ChunkLength, BYTE *ImagaData, UINT32 OutBufferSize)
{
//IMGDP_ENTRY(10021);
	//BYTE **ImageRowData = NULL;         // 图像行指针
	UINT32 i;
	UINT32 j;
	int ErrCode;
//	int pngfile_offect=0;
	UINT32 IDATcrc = 0;
	UINT32 FileIDATcrc = 0;
	S32	start=0, end=0;
	S32	x_draw_count=0, y_draw_count=0;
	BYTE skipRow;
	BYTE *imgOutPtr=0;
	UINT32 imgOutStride=0;

	FileChunks.IDATNum++;
	
	IDATcrc = crc32(IDATcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	FileIDATcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (IDATcrc != FileIDATcrc)
	{
		return IDATCRCERR;
	}

	IDATSize = ChunkLength;

	if (IHDRInfo.BitDepth == 16)
	{
		transform |= DEPTH16to8;
	}

	if (IHDRInfo.BitDepth < 8)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (IHDRInfo.ColorType == IndexColor)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (FileChunks.tRNSNum != 0)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (IHDRInfo.ColorType == GreyScale || IHDRInfo.ColorType == GreyScaleAlpha)
	{
		transform |= GREY2RGB;
	}

	transform |= BKGDImage;       //set it as the default transform method    

	if (FileChunks.bKGDNuM != 0)
	{
		transform |= bKGDEXPAND;
	}
	else// if(!(transform & BKGDImage))     //else
	{
	/*if there's no defined background colour in bKGDHandle, these code will be executed---zhangl*/	
	//set the default back ground as our image viewer brackground color  elisa 2009-9-29
		DefaultBkgd.Red =   216;
		DefaultBkgd.Green = 240;
		DefaultBkgd.Blue = 248;
       
		transform |= DEFAULTbKGD;
	}
           
/*
	gAMA
*/

	UpdateInfo();
	
	ErrCode = inflateInit(&PNGzstream);
	
	if (IHDRInfo.Interlace)
	{
		transform |= INTERLACE;
		ImagePass = 7;

		imgOutPtr= rgb_temp_ptr;
		imgOutStride= IHDRInfo.Width*3;//RGB565alpha
		start=0;
		end=IHDRInfo.Height-1;
	}
	else
	{
		ImagePass = 1;

		if(png_decode_mode==0)
		{
			if(png_resized_width== IHDRInfo.Width && png_resized_height== IHDRInfo.Height)
			{
				imgOutPtr= bkgrd_ptr;
				imgOutStride= png_bkgrd_width<<1;//rgb565
				start= PngCutOffsetY1;
				end= PngCutOffsetY2;
			}
			else//resize
			{
				imgOutPtr= rgb_temp_ptr;
				imgOutStride= IHDRInfo.Width*3;//RGB565alpha
				start= 0;
				end= IHDRInfo.Height-1;
			}
		}
		else if(png_decode_mode==1)
		{
			imgOutPtr= rgb_output_ptr;
			imgOutStride= (PngCutOffsetX2-PngCutOffsetX1+1)<<1;//rgb565
			start= PngCutOffsetY1;
			end= PngCutOffsetY2;
		}
		else if(png_decode_mode==2)
		{
			imgOutPtr= rgb_output_ptr;
			imgOutStride= (PngCutOffsetX2-PngCutOffsetX1+1)<<2;//alphaRGB
			start= PngCutOffsetY1;
			end= PngCutOffsetY2;
		}
	}


	for (i = 0; i < ImagePass; i++)
	{
		if (IHDRInfo.Interlace)
		{
			imgOutPtr= rgb_temp_ptr;
		}

		for (j = 0; j < IHDRInfo.Height; j++)
		{
			if(j<start)
				skipRow=1;
			else
				skipRow=0;
			if(j>end)
			{	
				ErrCode=CLIPOVER;//not an err
				break;
			}
			ErrCode = ProcessOneRow(PngFileContent, imgOutPtr,skipRow);
			if(!skipRow)
				imgOutPtr+=imgOutStride;
		}
	}
	inflateEnd(&PNGzstream);

//add by guosz
	
	if(png_resized_width!=IHDRInfo.Width || png_resized_height!=IHDRInfo.Height)//resize
	{
		U16 r,g,b,a,r1,r2,g1,g2,b1,b2,a1,a2;
		UINT32 offsetx,offsety;
		UINT32 Scalex,Scaley,alphay,alphax;
		U8 *pOriPos;
		U8   *adp,*adp1;
		UINT32 oriStartX,oriStartY;
		U8 rgb565[2];
		
		x_draw_count = PngCutOffsetX2-PngCutOffsetX1+1;
		y_draw_count = PngCutOffsetY2-PngCutOffsetY1+1;
		
		Scaley=(IHDRInfo.Height<<16)/png_resized_height;
		Scalex=(IHDRInfo.Width<<16)/png_resized_width;

		oriStartX=(Scalex*(UINT32)PngCutOffsetX1)>>16;//x offset in original png
		oriStartY=(Scaley*(UINT32)PngCutOffsetY1)>>16;//y offset in original png

		pOriPos = rgb_temp_ptr+ (oriStartY*IHDRInfo.Width+ oriStartX)*3;//original pos in RGB565alpha png

		imgOutStride= (png_bkgrd_width-x_draw_count)<<1;//rgb565 stride

		for(i=0;i<(UINT32)y_draw_count;i++)
		{
			offsety = ((i*Scaley)>>16)+1;
			alphay = (i*Scaley)&0xffff;

			//if(i==y_draw_count-1)
			//	offsety-=1;
			if(oriStartY+ offsety >= IHDRInfo.Height)
				offsety= IHDRInfo.Height -1;

			adp=pOriPos+ offsety * IHDRInfo.Width*3;

			for(j=0;j<(UINT32)x_draw_count;j++)
			{
				//temp1 =0;
				offsetx = ((j*Scalex)>>16)+1;
				alphax = (j*Scalex)&0xffff;

				//if ( j== x_draw_count-1)
				//	offsetx -= 1;
				if(oriStartX+ offsetx >= IHDRInfo.Width)
					offsetx= IHDRInfo.Width -1;

				//adp1 = adp + offsety * IHDRInfo.Width + offsetx;
				adp1=adp+ offsetx*3;
				rgb565[0]=*adp1;
				rgb565[1]=*(adp1+1);
				r1 = ((*(U16*)rgb565) & 0xf800 )>>8;
				g1 = ((*(U16*)rgb565) & 0x7E0 )>>3;
				b1 = ((*(U16*)rgb565) & 0x1F) << 3;
				a1 = *(adp1+ 2);
				rgb565[0]=*(adp1-3);
				rgb565[1]=*(adp1-2);
				r2 = ((*(U16*)rgb565) & 0xf800 )>>8;
				g2 = ((*(U16*)rgb565) & 0x7E0 )>>3;
				b2 = ((*(U16*)rgb565) & 0x1F) << 3;
				a2 = *(adp1- 1);
				r = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;
				a = (a1 * alphax + a2 * (0x10000 - alphax)+32768) >>16;

				adp1 = adp1 - IHDRInfo.Width*3;
				rgb565[0]=*adp1;
				rgb565[1]=*(adp1+1);
				r1 = ((*(U16*)rgb565) & 0xf800 )>>8;
				g1 = ((*(U16*)rgb565) & 0x7E0 )>>3;
				b1 = ((*(U16*)rgb565) & 0x1F) << 3;
				a1 = *(adp1+ 2);
				rgb565[0]=*(adp1-3);
				rgb565[1]=*(adp1-2);
				r2 = ((*(U16*)rgb565) & 0xf800 )>>8;
				g2 = ((*(U16*)rgb565) & 0x7E0 )>>3;
				b2 = ((*(U16*)rgb565) & 0x1F) << 3;
				a2 = *(adp1- 1);
				r1 = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g1 = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b1 = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;
				a1 = (a1 * alphax + a2 * (0x10000 - alphax)+32768) >>16;
				r = (r * alphay + r1 * (0x10000 - alphay)+32768) >>16;
				g = (g * alphay + g1 * (0x10000 - alphay)+32768) >>16;
				b = (b * alphay + b1 * (0x10000 - alphay)+32768) >>16;
				a = (a * alphay + a1 * (0x10000 - alphay)+32768) >>16;

				if(a>250)//process as 255
					*((U16*)bkgrd_ptr)= ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
				else if(a>4)//back ground not change when a<=0 process as 0
				{
					r1 = ((*(U16*)bkgrd_ptr) & 0xf800 )>>8;
					g1 = ((*(U16*)bkgrd_ptr) & 0x7E0 )>>3;
					b1 = ((*(U16*)bkgrd_ptr) & 0x1F) << 3;

					composite(r, r, a, r1);
					composite(g, g, a, g1);
					composite(b, b, a, b1);
					*((U16*)bkgrd_ptr)= ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
				}
				bkgrd_ptr+=2;
			}
			bkgrd_ptr+=imgOutStride;
		}
	}
	else if(IHDRInfo.Interlace)
	{
		U8   *adp;
		UINT32 imgInStride;
		U8 r,g,b,r1,g1,b1,a;
		U8 rgb565[2];

		x_draw_count = PngCutOffsetX2-PngCutOffsetX1+1;
		y_draw_count = PngCutOffsetY2-PngCutOffsetY1+1;
		
		adp = rgb_temp_ptr+ (PngCutOffsetY1*IHDRInfo.Width+ PngCutOffsetX1)*3;//original pos in RGB565alpha png
		imgInStride= (IHDRInfo.Width- x_draw_count)*3;

		if(png_decode_mode==0)
		{
			for(i=0;i<(UINT32)y_draw_count;i++)
			{
				for(j=0;j<(UINT32)x_draw_count;j++)
				{
					a=*(adp+2);
					if(a==255)
					{
						*(bkgrd_ptr)=*adp;
						*(bkgrd_ptr+ 1)=*(adp+ 1);
					}
					else if(a!=0)
					{
						r = ((*(U16*)bkgrd_ptr) & 0xf800 )>>8;
						g = ((*(U16*)bkgrd_ptr) & 0x7E0 )>>3;
						b = ((*(U16*)bkgrd_ptr) & 0x1F) << 3;

						rgb565[0]=*adp;
						rgb565[1]=*(adp+1);
						r1 = (*(U16*)(rgb565)& 0xf800 )>>8;
						g1 = ((*(U16*)(rgb565)& 0x7E0 )>>3);
						b1 = (*(U16*)rgb565  & 0x1F) << 3;

						composite(r, r1, a, r);
						composite(g, g1, a, g);
						composite(b, b1, a, b);
						*((U16*)bkgrd_ptr)= ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
					}
										
					bkgrd_ptr+=2;
					adp+=3;
				}
				adp= adp +imgInStride;
				bkgrd_ptr += (png_bkgrd_width- x_draw_count)<<1;
			}
		}
		else if(png_decode_mode==1)//rgb565+alpha
		{
			for(i=0;i<(UINT32)y_draw_count;i++)
			{
				
				for(j=0;j<(UINT32)x_draw_count;j++)
				{
					*(rgb_output_ptr)=*adp;
					*(rgb_output_ptr+ 1)=*(adp+ 1);
					*(alpha_output_ptr++)=*(adp+2);
					adp+=3;
					rgb_output_ptr+=2;
				}
				adp= adp +imgInStride;
			}
		}
		else if(png_decode_mode==2)//alphaRGB
		{
			for(i=0;i<(UINT32)y_draw_count;i++)
			{
				for(j=0;j<(UINT32)x_draw_count;j++)
				{
					rgb565[0]=*adp;
					rgb565[1]=*(adp+1);
					*(rgb_output_ptr++)=*(adp+ 2);
					*(rgb_output_ptr++) = (U8)((*(U16*)(rgb565)& 0xf800 )>>8);//r
					*(rgb_output_ptr++) = (U8)((*(U16*)(rgb565)& 0x7E0 )>>3);//g
					*(rgb_output_ptr++) = (U8)((*(U16*)(rgb565) & 0x1F) << 3);//b
					adp+=3;
				}
				adp= adp +imgInStride;
			}
		}
	}

	if(png_decode_mode==1 && set_default_alpha)
		memset(alpha_output_ptr,0xff,y_draw_count*x_draw_count);//set output alpha 255 when interlace

	return ErrCode;
}

int tIMEHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tIMEcrc = 0;
	UINT32 FiletIMEcrc = 0;

	if (FileChunks.tIMENum > 1)
	{
		//printf("Duplicate tIME chunk!");
		return tIMEERR;
	}
	else if (ChunkLength != 7)
	{
		//printf("Invalid tIME chunk length!");
		return tIMEERR;
	}
		
	FileChunks.tIMENum++;

	tIMEcrc = crc32(tIMEcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletIMEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tIMEcrc != FiletIMEcrc)
	{
		return tIMECRCERR;
	}	

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int iTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 iTXtcrc = 0;
	UINT32 FileiTXtcrc = 0;

	FileChunks.iTXtNum++;

	iTXtcrc = crc32(iTXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileiTXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (iTXtcrc != FileiTXtcrc)
	{
		return iTXtCRCERR;
	}
	
	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int tEXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tEXtcrc = 0;
	UINT32 FiletEXtcrc = 0;

	FileChunks.tEXtNum++;

	tEXtcrc = crc32(tEXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletEXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tEXtcrc != FiletEXtcrc)
	{
		return tEXtCRCERR;
	}	

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int zTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 zTXtcrc = 0;
	UINT32 FilezTXtcrc = 0;

	FileChunks.zTXtNum++;

	zTXtcrc = crc32(zTXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilezTXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (zTXtcrc != FilezTXtcrc)
	{
		return zTXtCRCERR;
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int IENDHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 IENDcrc = 0;
	UINT32 FileIENDcrc = 0;

//	free(RowBuffer);
//	free(PreRow);

	if (hISTContent != NULL)
		OslMfree(hISTContent);
	hISTContent=NULL;

//	if (tRNSIndex != NULL)
//		OslMfree(tRNSIndex);

	if (sPLTPalette != NULL)
		OslMfree(sPLTPalette);
	sPLTPalette=NULL;

	if (FileChunks.IENDNum > 1)
	{
		//printf("Duplicate IEND chunk!");
		return IENDERR;
	}
	else if (ChunkLength != 0)
	{
		//printf("Invalid IEND chunk length!");
		return IENDERR;
	}

	FileChunks.IENDNum++;

	IENDcrc = crc32(IENDcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileIENDcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (IENDcrc != FileIENDcrc)
	{
		return IENDCRCERR;
	}

	return 0;
}

int UnknownHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 Unknowncrc = 0;
	UINT32 FileUnknowncrc = 0;
	
	Unknowncrc = crc32(Unknowncrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileUnknowncrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (Unknowncrc != FileUnknowncrc)
	{
		return UnknownCRCERR;
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

#endif // if 0

