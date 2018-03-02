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


#ifndef AP_EXIF_H
#define AP_EXIF_H

#include "cs_types.h"
#define STRIPCOUNTMAX           1  //only support thumbnail nocompression
#define	EXIFHEADLENGTH			10

/*DEFINE TAG VALUE*/
#define	IMAGEWIDTH					0x0100
#define	IMAGELENGTH					0x0101
#define MAKE						0x010F
#define MODEL						0x0110
#define	ORIENTATION					0x0112
#define DATETIME        			0x0132
#define	EXIFIFDPOINTER				0x8769
#define EXIFDATETIME            	0x9003
#define EXIFVERSION            		0x9000
#define APERTURE            		0x9202
#define EXPOSUREBIAS            	0x9204

typedef struct
{
	uint8	Make[26];
	uint8	Model[30];
	uint8	DateTime[20];
	uint32  ImageOffset;
	uint16	ImageWidth;
	uint16	ImageHeight;
	uint16	Orientation;
	uint16	ExifVersion;
	uint32	Aperture_1;
	uint32	Aperture_2;
	uint32	ExposureBiasValue1;
	uint32	ExposureBiasValue2;
	uint32	FileSize;
} EXIFINFO;

/* FUNCTION  */
int32  Ap_GetImageInfo(uint8* FileBuf);//(uint32 FileIndex)
int Ap_ParseJPEGFile(uint8 *buf);
int Ap_ParseExifMarker(uint32 Offset);
int Ap_ParseSofMarker(uint32 Offset);
int Ap_ParseImageExif(uint32 APPOffset);
int Ap_ParseExifIFD(uint32 IFDOffset, uint32 *NextOffset);
int Ap_ParseTagInfo(uint32 TagOffset);
int Ap_CheckSOFInfo(uint8* StartOfFrame);
uint16 GetWord( uint8 *buf, uint8 ucBytesOrder);
uint32 GetDword( uint8 *buf, uint8 ucBytesOrder);
void Ap_ParseFileInit(void);
void Ap_ParseFileEnd(void);

#endif//AP_EXIF_H
