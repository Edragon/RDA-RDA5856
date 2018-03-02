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



/*
modify for area decode
output 1. blend RGB565 with alpha to background
		2. RGB565 and alpha
		3. AlphaRGB
ver. 20110716
sheen
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if 0 // support png later
#ifndef SHEEN_VC_DEBUG
#include "cs_types.h"
//#include "dsm_cf.h"
#include "ts.h"
#include "cos.h"
#endif
 

#include "zlib.h"
#include "png.h"
#include "mmi_png_dec.h"

#ifndef SHEEN_VC_DEBUG
//#include "mmi_trace.h"
#include "fs.h"
//#include "fat_fs.h"
#include "imagep_debug.h"
//#include "gdi_datatype.h"
//#include "gdi_include.h"
//#include "oslmemory.h"
#endif


#include "mci.h"
#include "hal_error.h"


typedef INT32 FS_HANDLE;
#define OslMfree free
#define OslMalloc malloc


const BYTE IHDR[5] = { 73,  72,  68,  82, '\0'};
const BYTE IDAT[5] = { 73,  68,  65,  84, '\0'};
const BYTE IEND[5] = { 73,  69,  78,  68, '\0'};
const BYTE PLTE[5] = { 80,  76,  84,  69, '\0'};
const BYTE bKGD[5] = { 98,  75,  71,  68, '\0'};
const BYTE cHRM[5] = { 99,  72,  82,  77, '\0'};
const BYTE gAMA[5] = {103,  65,  77,  65, '\0'};
const BYTE hIST[5] = {104,  73,  83,  84, '\0'};
const BYTE iCCP[5] = {105,  67,  67,  80, '\0'};
const BYTE iTXt[5] = {105,  84,  88, 116, '\0'};
const BYTE pHYs[5] = {112,  72,  89, 115, '\0'};
const BYTE sBIT[5] = {115,  66,  73,  84, '\0'};
const BYTE sPLT[5] = {115,  80,  76,  84, '\0'};
const BYTE sRGB[5] = {115,  82,  71,  66, '\0'};
const BYTE tEXt[5] = {116,  69,  88, 116, '\0'};
const BYTE tIME[5] = {116,  73,  77,  69, '\0'};
const BYTE tRNS[5] = {116,  82,  78,  83, '\0'};
const BYTE zTXt[5] = {122,  84,  88, 116, '\0'};

extern IHDRCHUNK IHDRInfo;
extern FILECHUNKS FileChunks;
extern z_stream PNGzstream;
extern UINT32 IDATSize; // IDAT块数据大小
extern UINT32 CurRowNumber;            // 当前行号
extern UINT32 CurPassRowNum;           // 当前Pass的行数
extern UINT32 CurPassRowBytes;         // 当前行的字节数
extern UINT32 CurInterlaceRowWidth;    // 当前交织行的宽度
extern BYTE CurPass;                   // 当前的Pass号
extern BYTE *tRNSIndex ; // 索引图像透明信息
extern bKGDINFO bKGDInfo;      // 背景块信息
extern UINT32 RowBytes;        // 原始图像一行的字节数
extern UINT32 transform ;   // 变换信息
extern UINT8 ImagePass;        // 图像Pass数
extern UINT8 PixelDepth;       // 像素比特深度
extern BYTE *RowBuffer;         // 行缓冲区
extern BYTE *PreRow ;
extern UINT16 TransNum;

extern UINT16 *hISTContent ;
extern sPLTPALETTE *sPLTPalette ;

FS_HANDLE	PngFileHandle;//-1 means no file handle
Png_StreamBuf PngStreamBuf;
U8	*rgb_output_ptr;      //RGA565 / alphaRGA output addr
U8	*alpha_output_ptr;      //alpha output addr
U8	*bkgrd_ptr;      //RGB565 background addr
U8	*rgb_temp_ptr;// temp RGB565alpha for resize or interlace.

INT16 PngCutOffsetX1;	//cut area x start offset in png
INT16 PngCutOffsetY1;	//cut area y start offset in png
INT16 PngCutOffsetX2;	//cut area x end offset in png
INT16 PngCutOffsetY2;	//cut area y end offset in png
U16 png_decode_mode;
INT16 png_resized_width;
INT16 png_resized_height;
INT16	png_bkgrd_width;
INT16	png_bkgrd_height;
U8 set_default_alpha;//all alpha set 255 if need

#ifndef SHEEN_VC_DEBUG
extern void gdi_get_alpha_blending_source_layer(gdi_handle *src_layer_handle);
extern void gdi_image_layer_copy(S32 x1, S32 y1, S32 x2, S32 y2, gdi_handle dst_layer, gdi_handle src_layer);
#endif
/*
U8 *FileContent,		// png stream input
UINT32 FileSize,		//file size in FileContent
U8 *PNGOutBuffer,         // rgb565 alphaRGB output addr
UINT32 OutBufferSize,     //PNGOutBuffer size
U8 *AlphaOutBuffer,		//alpha ouput buffer.
UINT32 AlphaBufferSize,
U8 *BkgrdBuffer,			//background buffer to blend
U16 BkgrdClipX1,	//display x start offset in background 
U16 BkgrdClipX2,	//display x end offset in background
U16 BkgrdClipY1,	//display y start offset in background
U16 BkgrdClipY2,	//display y end offset in background
S16 BkgrdOffsetX, //img x offset in background , may be minus
S16 BkgrdOffsetY, //img y offset in background , may be minus
U16 BkgrdWidth,	//background width
U16 BkgrdHeight,	//background height
U16 ZoomWidth,	//resize width with DecMode0, (ZoomWidth=0 means no resize)
U16 ZoomHeight,	//resize height with DecMode0, (ZoomHeight=0 means no resize)
U16 DecMode,	//0= rgb565 blend to BkgrdBuffer & resize enable & cut area enable
				//1= rgb565 to PNGOutBuffer & alpha to AlphaOutBuffer & no resize & cut area enable
				//2= alphaRGB to PNGOutBuffer & no resize & cut area enable.
UINT32 *ImgWidth,         // output area width
UINT32 *ImgHeigh         // output area height

NOTE: input BkgrdClip & BkgrdOffset to get cut area for all of DecMode.
sheen
20110718
*/
int MMF_PngDecodePicture(U8 *FileContent,
			  UINT32 FileSize,
			  U8 *PNGOutBuffer,         
			  UINT32 OutBufferSize,     
			  U8 *AlphaOutBuffer,	
			  UINT32 AlphaBufferSize,
			  U8 *BkgrdBuffer,		
			  U16 BkgrdClipX1,	
			  U16 BkgrdClipX2,	
			  U16 BkgrdClipY1,	
			  U16 BkgrdClipY2,	
			  S16 BkgrdOffsetX, 
			  S16 BkgrdOffsetY, 
			  U16 BkgrdWidth,
			  U16 BkgrdHeight,	
			  U16 ZoomWidth,	
			  U16 ZoomHeight,	
			  U16 DecMode,
			  UINT32 *ImgWidth,
			  UINT32 *ImgHeigh 
			  )
{
	INT32 res;
	BYTE *PngFileContent = FileContent;
	UINT32 ChunkLength = 0;
	CHUNKTYPE ChunkType;
	int errcode=0;             
	BYTE PNGSignature[] = {137, 80, 78, 71, 13, 10, 26, 10};//"PNG"

#ifdef __PNG_DECODE_WITH_VOC__


	MCI_PNGDEC_STATUS_T status_png;

	UINT32  *MCI_FileContent=FileContent;
	UINT32 MCI_FileSize=FileSize;

	UINT32 *MCI_PNGOutBuffer=PNGOutBuffer;         
	UINT32 MCI_OutBufferSize=OutBufferSize;
    
	UINT32 *MCI_AlphaOutBuffer=AlphaOutBuffer;	
	UINT32 MCI_AlphaBufferSize=AlphaBufferSize;

	INT32 *MCI_BkgrdBuffer=BkgrdBuffer;		
	INT16 MCI_BkgrdClipX1=BkgrdClipX1;	
	INT16 MCI_BkgrdClipX2=BkgrdClipX2;	
	INT16 MCI_BkgrdClipY1=BkgrdClipY1;	
	INT16 MCI_BkgrdClipY2=BkgrdClipY2;	

	INT16 MCI_BkgrdOffsetX=BkgrdOffsetX; 
	INT16 MCI_BkgrdOffsetY=BkgrdOffsetY; 

	INT16 MCI_BkgrdWidth=BkgrdWidth;
	INT16 MCI_BkgrdHeight=BkgrdHeight;
	
	INT16 MCI_ZoomWidth=ZoomWidth;	
	INT16 MCI_ZoomHeight=ZoomHeight;
	
	INT16 MCI_DecMode=DecMode;
	INT16 MCI_alpha_blending_to_transparentColor=gdi_get_alpha_blending_to_transparentColor();


	UINT32 *MCI_ImgWidth=ImgWidth;
	UINT32 *MCI_ImgHeigh=ImgHeigh;

	UINT32  *MCI_WindowBuf=NULL;

	UINT32 *MCI_rgb_temp_ptr=NULL;

	UINT32 MCI_PngFileHandle=PngFileHandle;

	
#endif

		

	mmi_trace(TRUE,"MMF_PngDecodePicture\n");
	mmi_trace(TRUE,"PNG FileContent %x FileSize %d\n",FileContent,FileSize);
	mmi_trace(TRUE,"PNG PNGOutBuffer %x OutBufferSize %d AlphaOutBuffer %x AlphaBufferSize %d\n",PNGOutBuffer,OutBufferSize,
		AlphaOutBuffer,AlphaBufferSize);
	mmi_trace(TRUE,"PNG BkgrdBuffer %x BkgrdClipX1 %d BkgrdClipX2 %d\n",BkgrdBuffer,BkgrdClipX1,BkgrdClipX2);
	mmi_trace(TRUE,"PNG BkgrdClipY1 %d BkgrdClipY2 %d BkgrdOffsetX %d BkgrdOffsetY %d\n",BkgrdClipY1,BkgrdClipY2,BkgrdOffsetX,BkgrdOffsetY);
	mmi_trace(TRUE,"PNG BkgrdWidth %d BkgrdHeight %d ZoomWidth %d ZoomHeight %d\n",BkgrdWidth,BkgrdHeight,ZoomWidth,ZoomHeight);
	mmi_trace(TRUE,"PNG DecMode %d &ImgWidth %x &ImgHeigh %x\n",DecMode,ImgWidth,ImgHeigh);

	if(!FileContent ||!ImgWidth ||!ImgHeigh)
		return INPUTERR;

	if(BkgrdClipX1>BkgrdClipX2 || BkgrdClipY1>BkgrdClipY2 || BkgrdClipX2 >=BkgrdWidth
		|| BkgrdClipY2>=BkgrdHeight)
		return INPUTERR;

	PngStreamBuf.PNGSTREAM= FileContent;
	PngStreamBuf.byteleft= FileSize;

	png_decode_mode= DecMode;
	png_bkgrd_width= BkgrdWidth;
	png_bkgrd_height= BkgrdHeight;
	set_default_alpha=0;
	
// init all global parameters
       PNG_Init();
	
	if (memcmp(PNGSignature, PngFileContent, 8) != 0)
	{
		//printf("This is not a PNG file!\n");
//		exit(0);
	}
	else
	{
		PngFileContent += 8;
		PngStreamBuf.byteleft -=8;
		//printf("It is a PNG file!\n");
	}

	ChunkLength = ((*PngFileContent) << 24) + ((*(PngFileContent + 1)) << 16)
		+ ((*(PngFileContent + 2)) << 8) + (*(PngFileContent + 3));
	PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	
	ChunkType.First = *PngFileContent;
	ChunkType.Second = *(PngFileContent + 1);
	ChunkType.Third = *(PngFileContent + 2);
	ChunkType.Forth = *(PngFileContent + 3);
	
	if (memcmp(&ChunkType, IHDR, 4) == 0)
	{
		errcode = IHDRHandle(&PngFileContent, ChunkLength);

		if (errcode != 0)
		{
			return errcode;
		}
	}
	else
	{
		return IHDRERR;
	}

	mmi_trace(TRUE,"PNG IHDRInfo Width %d Height %d\n",IHDRInfo.Width,IHDRInfo.Height);
	mmi_trace(TRUE,"PNG IHDRInfo Interlace %d\n",IHDRInfo.Interlace);

	if(png_decode_mode==0 && ZoomWidth>0 && ZoomHeight>0)
	{
		png_resized_width= ZoomWidth;
		png_resized_height= ZoomHeight;
	}
	else//no resize
	{
		png_resized_width= IHDRInfo.Width;
		png_resized_height= IHDRInfo.Height;
	}
	
	//set cut area in original or resize png
	if(BkgrdOffsetX +png_resized_width > BkgrdClipX1 && BkgrdOffsetX <=BkgrdClipX2)
	{
		res=(INT32)BkgrdClipX1- (INT32)BkgrdOffsetX;
		if(res>0)
			PngCutOffsetX1=res;
		else
		{
			PngCutOffsetX1=0;
			BkgrdClipX1= BkgrdOffsetX;//reset clipX1
		}
		res=(INT32)BkgrdClipX2- ((INT32)BkgrdOffsetX+ (INT32)png_resized_width -1);
		if(res >= 0)
			PngCutOffsetX2=png_resized_width-1;
		else
			PngCutOffsetX2=(INT32)png_resized_width+ res -1;
	}
	else
	{
		mmi_trace(TRUE,"PNG out of display clip area.\n");
		return PNG_NO_ERROR;
	}
	
	if(BkgrdOffsetY +png_resized_height > BkgrdClipY1 && BkgrdOffsetY <=BkgrdClipY2)
	{
		res=(INT32)BkgrdClipY1- (INT32)BkgrdOffsetY;
		if(res>0)
			PngCutOffsetY1=res;
		else
		{
			PngCutOffsetY1=0;
			BkgrdClipY1= BkgrdOffsetY;//reset clipY1
		}
		res=(INT32)BkgrdClipY2- ((INT32)BkgrdOffsetY+ (INT32)png_resized_height -1);
		if(res >= 0)
			PngCutOffsetY2=png_resized_height-1;
		else
			PngCutOffsetY2=(INT32)png_resized_height+ res -1;
	}
	else
	{
		mmi_trace(TRUE,"PNG out of display clip area.\n");
		return PNG_NO_ERROR;
	}



#ifndef SHEEN_VC_DEBUG
	if(png_decode_mode==0)
	{
		gdi_handle 	baselayer,activelayer;
		gdi_get_alpha_blending_source_layer(&baselayer);
		gdi_layer_get_active(&activelayer);  
		if((activelayer != baselayer)&&(activelayer != -1)&&(baselayer != -1))
		{

				
			gdi_image_layer_copy(BkgrdClipX1,BkgrdClipY1,
				BkgrdClipX1+(PngCutOffsetX2-PngCutOffsetX1),BkgrdClipY1+(PngCutOffsetY2-PngCutOffsetY1),
				activelayer,baselayer);
		}
	}
#endif


#ifdef __PNG_DECODE_WITH_VOC__

#if (defined(GALLITE_IS_8805) || defined(GALLITE_IS_8806))
    csw_SetVOCFreqActivity(CSW_VOC_FREQ_78M);
#else
    csw_SetVOCFreqActivity(CSW_VOC_FREQ_250M);
#endif

	if(IHDRInfo.Width<1000&&png_resized_width== IHDRInfo.Width && png_resized_height== IHDRInfo.Height && !IHDRInfo.Interlace&&PngFileHandle==-1)
	{

	    MCI_WindowBuf=(UINT32 *)OslMalloc(64*1024);

	 	if(!MCI_WindowBuf)
	 	{
	 		mmi_trace(TRUE,"PNG window malloc err\n");
	 		return MEMERR;
	 	}		

	    if ( MCI_PngdecStart
			(MCI_FileContent,
			 MCI_FileSize,
			 MCI_PNGOutBuffer,         
			 MCI_OutBufferSize,     
			 MCI_AlphaOutBuffer,	
			 MCI_AlphaBufferSize,
			 MCI_BkgrdBuffer,		
			 MCI_BkgrdClipX1,	
			 MCI_BkgrdClipX2,	
			 MCI_BkgrdClipY1,	
			 MCI_BkgrdClipY2,	
			 MCI_BkgrdOffsetX, 
			 MCI_BkgrdOffsetY, 
			 MCI_BkgrdWidth,
			 MCI_BkgrdHeight,	
			 MCI_ZoomWidth,	
			 MCI_ZoomHeight,	
			 MCI_DecMode,
			 MCI_alpha_blending_to_transparentColor,
			 MCI_ImgWidth,
			 MCI_ImgHeigh ,			 
			 MCI_WindowBuf,
			 MCI_rgb_temp_ptr,
			 MCI_PngFileHandle,
			 NULL) != HAL_ERR_RESOURCE_BUSY)
	    {

	
		    	do
		    	{
		    		
		    	
		    	} while(MCI_PngdecGetDecodingStatus(&status_png) == FALSE);    

			//hal_HstSendEvent(SYS_EVENT,0xabcd1235);

			if(MCI_WindowBuf)
			{
				mmi_trace(TRUE,"PNG window free\n");

				OslMfree(MCI_WindowBuf);
			}



			return status_png.errorStatus;
		}


		if(MCI_WindowBuf)
		{
			mmi_trace(TRUE,"PNG window free no voc\n");
			OslMfree(MCI_WindowBuf);
		}		

	}

	hal_HstSendEvent(SYS_EVENT,0xabcd2233);






#endif


	
	switch(png_decode_mode)
	{
	case 0:
		{
			if(!BkgrdBuffer)
				return INPUTERR;

			rgb_output_ptr= 0;
			alpha_output_ptr= 0;
			
			bkgrd_ptr= BkgrdBuffer+ ((UINT32)BkgrdWidth*(UINT32)BkgrdClipY1 + (UINT32)BkgrdClipX1)*2;//rgb565 bkgrd
		}
		
		break;
	case 1:
		{
			if(!PNGOutBuffer || !AlphaOutBuffer)
				return INPUTERR;
			res= (INT32)(PngCutOffsetX2- PngCutOffsetX1 +1)*(INT32)(PngCutOffsetY2- PngCutOffsetY1 +1)<<1;//rgb565
			if(res > OutBufferSize)
				return INPUTERR;
			res>>=1;//alpha
			if(res > AlphaBufferSize)
				return INPUTERR;
			rgb_output_ptr= PNGOutBuffer;
			alpha_output_ptr= AlphaOutBuffer;
			bkgrd_ptr= 0;
			
		}
		break;
	case 2:
		{
			if(!PNGOutBuffer)
				return INPUTERR;
			res= (INT32)(PngCutOffsetX2- PngCutOffsetX1 +1)*(INT32)(PngCutOffsetY2- PngCutOffsetY1 +1)<<2;//alphaRGB
			if(res > OutBufferSize)
				return INPUTERR;
			rgb_output_ptr= PNGOutBuffer;
			alpha_output_ptr= 0;
			bkgrd_ptr= 0;
			
		}
		break;
	default:
		return INPUTERR;
	}

	if(IHDRInfo.BitDepth>8)
	{
		mmi_trace(TRUE,"PNG BitDepth=%d unspport\n",IHDRInfo.BitDepth);
		return SUPPORTERR;
	}


	RowBuffer= OslMalloc(((IHDRInfo.Width<<2)+ 32)<<1);     
	if(!RowBuffer)
	{
		mmi_trace(TRUE,"PNG RowBuffer malloc err\n");
		return MEMERR;
	}
	PreRow= RowBuffer+ (IHDRInfo.Width<<2)+ 32; 
	//FinalRowData= PreRow+ (IHDRInfo.Width<<2)+ 32); 
	memset(RowBuffer,0,((IHDRInfo.Width<<2)+ 32)<<1);

	rgb_temp_ptr=0;
	if(	png_resized_width!= IHDRInfo.Width || png_resized_height!= IHDRInfo.Height || IHDRInfo.Interlace)
	{
		rgb_temp_ptr= OslMalloc(IHDRInfo.Width*IHDRInfo.Height*3); //RGB565alpha
		if(!rgb_temp_ptr)
		{
			mmi_trace(TRUE,"PNG rgb_temp_ptr malloc err\n");
			OslMfree(RowBuffer);
			return MEMERR;
		}
	}



	for (;;)
	{
		ChunkLength = ((*PngFileContent) << 24) + ((*(PngFileContent + 1)) << 16)
			+ ((*(PngFileContent + 2)) << 8) + (*(PngFileContent + 3));
		PngFileContent += 4;
		PngStreamBuf.byteleft -=4;
		/*
		if((INT32)ChunkLength+4 > MAXPNG_BUF){
			mmi_trace(TRUE,"too large png, not support!---zhangl\n");
			errcode= ChunkExtra;
			break;
		}*/
		if((INT32)ChunkLength > (PngStreamBuf.byteleft+8) && PngFileHandle!=-1){
			INT32 byte_load=0;
			memmove(PngStreamBuf.PNGSTREAM,PngFileContent,PngStreamBuf.byteleft);
			MMI_FS_Read(PngFileHandle, (PngStreamBuf.PNGSTREAM+PngStreamBuf.byteleft), (MAXPNG_BUF-PngStreamBuf.byteleft),&byte_load );			
			PngStreamBuf.byteleft+=byte_load;
			PngFileContent=PngStreamBuf.PNGSTREAM;
		}
		
		ChunkType.First = *PngFileContent;
		ChunkType.Second = *(PngFileContent + 1);
		ChunkType.Third = *(PngFileContent + 2);
		ChunkType.Forth = *(PngFileContent + 3);
		
		if (memcmp(&ChunkType, cHRM, 4) == 0)
		{
			errcode = cHRMHandle(&PngFileContent, ChunkLength);

			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, gAMA, 4) == 0)
		{
			errcode = gAMAHandle(&PngFileContent, ChunkLength);

			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, iCCP, 4) == 0)
		{
			errcode = iCCPHandle(&PngFileContent, ChunkLength);
			
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, sBIT, 4) == 0)
		{
			sBITInfo SigBit;
			errcode = sBITHandle(&PngFileContent, ChunkLength, &SigBit);
			
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, sRGB, 4) == 0)
		{
			errcode = sRGBHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, PLTE, 4) == 0)
		{
			errcode = PLTEHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, bKGD, 4) == 0)
		{
			errcode = bKGDHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, hIST, 4) == 0)
		{
			errcode = hISTHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, tRNS, 4)  == 0)
		{
//			TRANSINFO *TransInfo = (TRANSINFO *)malloc(sizeof(TRANSINFO));
			errcode = tRNSHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, pHYs, 4) == 0)
		{
			errcode = pHYsHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, sPLT, 4) == 0)
		{
			errcode = sPLTHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if ( memcmp(&ChunkType, IDAT, 4) == 0)
		{
			errcode = IDATHandle(&PngFileContent, ChunkLength, PNGOutBuffer, OutBufferSize);

			if ( errcode != 0)
			{
				//return errcode;
				if(errcode == CLIPOVER)
					errcode=0;
				break;
			}
		}
		else if (memcmp(&ChunkType, tIME, 4) == 0)
		{
			errcode = tIMEHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, iTXt, 4) == 0)
		{
			errcode = iTXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, tEXt, 4) == 0)
		{
			errcode = tEXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, zTXt, 4) == 0)
		{
			errcode = zTXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				//return errcode;
				break;
			}
		}
		else if (memcmp(&ChunkType, IEND, 4) == 0)
		{
			errcode = IENDHandle(&PngFileContent, ChunkLength);
			/*if ( errcode != 0)
			{
				return errcode;
			}*/
			
//IMGDP_EXIT(10020);

			//return 0;
			break;
		}
		else
		{
			errcode = UnknownHandle(&PngFileContent, ChunkLength);
			if (errcode != 0)
			{
				//return errcode;
				break;
			}
		}
	}

	//output area width/height
	*ImgWidth = (UINT32)(PngCutOffsetX2- PngCutOffsetX1+ 1);
	*ImgHeigh = (UINT32)(PngCutOffsetY2- PngCutOffsetY1+ 1);

	if (hISTContent != NULL)
		OslMfree(hISTContent);
	hISTContent=NULL;
	
	//	if (tRNSIndex != NULL)
	//		OslMfree(tRNSIndex);
	
	if (sPLTPalette != NULL)
		OslMfree(sPLTPalette);
	sPLTPalette=NULL;


	if(RowBuffer)
		OslMfree(RowBuffer);

	if(rgb_temp_ptr)
		OslMfree(rgb_temp_ptr);

	mmi_trace(TRUE,"MMF_PngDecodePicture over. errcode=%d \n",errcode);

			

	return errcode;
}


VOID PNG_Init(VOID){
	
	 //INIT  PNG文件中出现的各个chunk
	memset(&FileChunks,0,sizeof(FILECHUNKS)); 
	transform=0;
	TransNum=0;
 	IDATSize=0;
	PNGzstream.next_in=NULL;
	PNGzstream.avail_in=0;
	PNGzstream.avail_out=0;
	PNGzstream.total_in=0;
	PNGzstream.total_out=0;
	PNGzstream.adler=0;
	PNGzstream.state=Z_NULL;
	PNGzstream.msg=NULL;
	PNGzstream.next_out=NULL;
	PNGzstream.data_type=0;
	PNGzstream.zalloc=NULL;
	PNGzstream.zfree=NULL;
	PNGzstream.opaque=NULL;
	PNGzstream.adler=0;
	PNGzstream.reserved=0;
	
	CurRowNumber=0;            
	CurPassRowNum=0;           
	CurPassRowBytes=0;         
	CurInterlaceRowWidth=0;    
	CurPass=0;     

	memset(&bKGDInfo,0,sizeof(bKGDINFO)); 
	memset(&IHDRInfo,0,sizeof(IHDRCHUNK)); 

	tRNSIndex = NULL; 
	RowBytes=0;
	transform=0;
	ImagePass=0;
	PixelDepth=0; 
//	png_resized_width=0;
//	png_resized_height=0;
	//ResizePng_Need=0;
		
	RowBuffer = NULL;        
	PreRow = NULL;    
	
	//FinalRowData = NULL;     
    //memset(RowBytesTempArray,0,(MaxWidth << 2) + 32);
	//memset(RowBytesArray,0,(MaxWidth << 2) + 2);
    //memset(FinalRowArray,0,(MaxWidth << 2) + 32);

}

#if 0
#if (CSW_EXTENDED_API_LCD == 1)
#include "lcdd_m.h"
#include "mci_lcd.h"
    LCDD_FBW_T png_displayFbw;
   uint16  *png_backbuffer;
   uint16  *png_blendBuffer;
VOID lcd_png_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
   if(png_backbuffer)
   	{
      memcpy((void*)png_backbuffer,(void*)buffer,MMC_LcdWidth*MMC_LcdHeight*2);
   	}
}
#endif



void PNG_blendbg(uint16* bgbuf, uint16* pngbuf, UINT32 png_width, UINT32 png_height,uint16 preview_startx,uint16 preview_starty){

	UINT32 i=0;
	uint16* bg_row_start=bgbuf + MMC_LcdWidth * preview_starty;
	uint16 bg_line_offset=preview_startx+10;

	for(i=0;i<png_height;++i)
		memcpy(bg_row_start+(MMC_LcdWidth*i+bg_line_offset),pngbuf+(png_width*i),png_width<<1);

}


extern char * mmc_MemMalloc(int32 nsize);
extern void mmc_MemFreeAll();
extern uint32 get_lcd_frame_buffer_address(void);

int PNG_display(unsigned short preview_startx,unsigned short preview_starty){

	int err_decode=0;

       BYTE *pFileContent = NULL;
	BYTE *OutBuffer = NULL;
       UINT32 OutBufferSize;
	UINT32 Width;
	UINT32 Height;

	
       pFileContent = FileArray;


	pFileContent = FileArray;

	err_decode = PNGDecode(pFileContent, &OutBuffer, &OutBufferSize,  &Width, &Height);

	mmi_trace(1,"---png decoder is over:%d!---",err_decode);
	 png_backbuffer =(UINT16*)mmc_MemMalloc(MMC_LcdWidth*MMC_LcdHeight*2);
	 png_blendBuffer =(UINT16*)get_lcd_frame_buffer_address();

       memcpy(png_backbuffer,png_blendBuffer,MMC_LcdWidth*MMC_LcdHeight*2);
	if(err_decode==0){ 
		 mmi_trace(1,"---png width:%d and height:%d!---",Width,Height);
	        PNG_blendbg(png_backbuffer,(uint16*)OutBuffer,Width,Height,preview_startx,preview_starty);
		}
	
#if (CSW_EXTENDED_API_LCD == 1)
        // Initialize Frame Buffer Window
        // Buffer pointer will be set at the time of display
        // Other parameters are constant
        png_displayFbw.fb.buffer = png_backbuffer;
        png_displayFbw.fb.width = MMC_LcdWidth;
        png_displayFbw.fb.height = MMC_LcdHeight;
        png_displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
        png_displayFbw.roi.width = MMC_LcdWidth;
        png_displayFbw.roi.height = MMC_LcdHeight;
        png_displayFbw.roi.x = 0;
        png_displayFbw.roi.y = 0;

        // Start Bypass every time previewstatus is set to 1
        // Stop it when it is set to 0
        //  mci_LcdStartBypass(lcd_png_BlockWriteBypass);
#endif
        // Display the blended image
        lcdd_Blit16(&png_displayFbw,0 ,0);

	 mmc_MemFreeAll();
	
	return err_decode;
	

}

#endif

#ifdef SHEEN_VC_DEBUG
//#if 0

#pragma pack(1)
/*BMP文件头*/
typedef struct
{
	U16 bfType;          //文件类型，2Bytes，值为ox4D42(字符BM)
	UINT32 bfSize;         // 4Bytes,整个文件的大小，单位为字节
	U16 bfReserved1;     //保留，2Bytes，置位为0
	U16 bfReserved2;     //保留，2Bytes，置位为0
	UINT32 bfOffBits;      //从文件头开始到实际图像数据之间的字节偏移量，4Bytes
} BITMAPFILEHEADER;


/*BMP文件信息头*/
typedef struct  
{
	UINT32 biSize;             //BMP信息头大小(字节数)，4Bytes
	INT32 biWidth;             //图像的宽度，以像素为单位，4Bytes
	INT32 biHeight;            //图像的高度，以像素为单位，4Bytes
	U16 biPlanes;            //说明目标设备位面数，固定值为1，2Bytes
	U16 biBitCount;          //每像素比特数，2Bytes，其值为：1、4、8、16、24、32
	UINT32 biCompression;      //图像数据压缩方法，4Bytes
	UINT32 biSizeImage;        //图像的大小(字节数)，4Bytes，该数必须是4的整数倍，当使用BI_RGB格式时，可置为0
	INT32 biXPelsPerMeter;     //水平分辨率，4Bytes，单位：像素/米
	INT32 biYPelsPerMeter;     //垂直分辨率，4Bytes，单位：像素/米
	UINT32 biClrUsed;          //实际使用的色表中的颜色索引数,4Bytes
	UINT32 biClrImportant;     //对图像显示有重要影响的颜色索引的数目,4Bytes
} BITMAPINFOHEADER;
#pragma pack()


unsigned char RGB565Pal[16] = {0x00, 0xf8, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
			                  0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//rgb565 需要写此16字节

void main()
{
	//FILE *fIn=fopen("F:\\测试文件\\PNG\\apple-tim-cook,R-K-207056-1死机.png","rb");
	FILE *fIn=fopen("F:\\测试文件\\PNG\\1x80.png","rb");
	FILE *fOut=fopen("F:\\测试文件\\PNG\\test.bmp","wb");
	FILE *fAlpha=fopen("F:\\测试文件\\PNG\\alpha.yuv","wb");
	BITMAPFILEHEADER *mypbmfh;
	BITMAPINFOHEADER *mypbmih;

	U8 *FileContent;
	UINT32 FileSize;
	U8 *PNGOutBuffer;
	UINT32 OutBufferSize; 
	U8 *AlphaOutBuffer;
	UINT32 AlphaBufferSize;
	U8 *BkgrdBuffer;		
	U16 BkgrdClipX1= 0;	
	U16 BkgrdClipX2= 0;	
	U16 BkgrdClipY1= 10;	
	U16 BkgrdClipY2= 69;	
	S16 BkgrdOffsetX= 0;
	S16 BkgrdOffsetY= 0;
	U16 BkgrdWidth= BkgrdClipX2 +1;
	U16 BkgrdHeight= BkgrdClipY2 +1;	
	U16 ZoomWidth= 0;
	U16 ZoomHeight= 0;
	U16 DecMode= 0;
	UINT32 ImgWidth;
	UINT32 ImgHeigh;

	PngFileHandle= -1;

	fseek(fIn,0,SEEK_END);
	FileSize=ftell(fIn);
	FileContent= malloc(FileSize);
	fseek(fIn,0,SEEK_SET);
	fread(FileContent ,1,FileSize,fIn);
	
	OutBufferSize= (BkgrdClipX2- BkgrdClipX1 +1)*(BkgrdClipY2- BkgrdClipY1 +1)*4;
	PNGOutBuffer= malloc(OutBufferSize);

	AlphaBufferSize= (BkgrdClipX2- BkgrdClipX1 +1)*(BkgrdClipY2- BkgrdClipY1 +1);
	AlphaOutBuffer=malloc(AlphaBufferSize);

	BkgrdBuffer= malloc(BkgrdWidth*BkgrdHeight*2);//rgb565
	memset(BkgrdBuffer, 255, BkgrdWidth*BkgrdHeight*2);//back ground color

	MMF_PngDecodePicture(FileContent,
		FileSize,
		PNGOutBuffer,         
		OutBufferSize,     
		AlphaOutBuffer,	
		AlphaBufferSize,
		BkgrdBuffer,		
		BkgrdClipX1,	
		BkgrdClipX2,	
		BkgrdClipY1,	
		BkgrdClipY2,	
		BkgrdOffsetX, 
		BkgrdOffsetY, 
		BkgrdWidth,
		BkgrdHeight,	
		ZoomWidth,	
		ZoomHeight,	
		DecMode,
		&ImgWidth,
		&ImgHeigh );

	//bmp header
	mypbmfh = ( BITMAPFILEHEADER *)malloc(14);
	mypbmih = (BITMAPINFOHEADER *)malloc(40);

	mypbmfh->bfType = 0x4d42;
	mypbmfh->bfReserved1 = 0;
	mypbmfh->bfReserved2 = 0;
	if(DecMode==0)
		mypbmfh->bfSize = 70 + BkgrdWidth*BkgrdHeight*2;
	else if(DecMode==1)
		mypbmfh->bfSize = 70 + ImgWidth*ImgHeigh*2;
	else if(DecMode==2)
		mypbmfh->bfSize = 70 + ImgWidth*ImgHeigh*4;
	mypbmfh->bfOffBits = 70;
	
	
	mypbmih->biSize = 40;
	if(DecMode==0)
	{
		mypbmih->biWidth =BkgrdWidth;
		mypbmih->biHeight =-((long)BkgrdHeight);//加-号可以使bmp图像上下倒转显示方式（原本bmp存储时上下倒转）
		mypbmih->biBitCount = 16;
		mypbmih->biCompression = 3;//rgb565 写0颜色不正确。
		mypbmih->biSizeImage= BkgrdWidth*BkgrdHeight*2;
	}
	else if(DecMode==1)
	{
		mypbmih->biWidth =ImgWidth;
		mypbmih->biHeight =-((long)ImgHeigh);//加-号可以使bmp图像上下倒转显示方式（原本bmp存储时上下倒转）
		mypbmih->biBitCount = 16;
		mypbmih->biCompression = 3;//rgb565 写0颜色不正确。
		mypbmih->biSizeImage= ImgWidth*ImgHeigh*2;
	}
	else if(DecMode==2)
	{
		mypbmih->biWidth =ImgWidth;
		mypbmih->biHeight =-((long)ImgHeigh);//加-号可以使bmp图像上下倒转显示方式（原本bmp存储时上下倒转）
		mypbmih->biBitCount = 32;
		mypbmih->biCompression = 0;//rgb565 写0颜色不正确。
		mypbmih->biSizeImage= ImgWidth*ImgHeigh*4;
	}

	mypbmih->biPlanes = 1;
	mypbmih->biXPelsPerMeter= 0;
	mypbmih->biYPelsPerMeter= 0;
	mypbmih->biClrUsed= 0;
	mypbmih->biClrImportant= 0;
	
	fwrite(mypbmfh, 14, 1, fOut);
	fwrite(mypbmih, 40, 1, fOut);
	fwrite(&RGB565Pal, 16, 1, fOut);//rgb565 需要写此16字节
	//bmp header end

	if(DecMode==0)
		fwrite(BkgrdBuffer,mypbmih->biSizeImage,1,fOut);
	else if(DecMode==1)
	{
		fwrite(PNGOutBuffer,mypbmih->biSizeImage,1,fOut);
		fwrite(AlphaOutBuffer,ImgWidth*ImgHeigh,1,fAlpha);
	}
	else if(DecMode==2)
		fwrite(PNGOutBuffer,mypbmih->biSizeImage,1,fOut);


	fclose(fIn);
	fclose(fOut);
	fclose(fAlpha);
	free(FileContent);
	free(PNGOutBuffer);
	free(AlphaOutBuffer);
	free(BkgrdBuffer);
	free(mypbmfh);
	free(mypbmih);
}
#endif
#endif // if 0

