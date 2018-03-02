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

#include "cs_types.h"
#include "mci.h"
#include "image.h"

/***************************************************************************************
data:		    set input buffer address
outbuf:			set output buffer address
outbuf_len:		output buffer size, not less than display area.
width:			set scaled width of the full output image.
height:			set scaled height of the full output image.
start_x:		set x start position of the display area in scaled image.(0==first)
end_x:			set x end position of the display area in scaled image.
start_y:		set y start position of the display area in scaled image.(0==first)
end_y:			set y end position of the display area in scaled image.
kind:			set kind of the input&output format.(default=0 input jpeg in buf & output RGB565)
decUnit->width:	output display image width
decUnit->height: output display image height
decUnit->len:	output display image data size.

!!! NOTE:
	1. input width/height will be adjust inside with aspect ratio also limit within original width/height.
	2. start_x/end_x/start_y/end_y sign the real output area also be adjust with width/height.
	3. set output area within LCD width/height.

****************************************************************************************/

MCI_ERR_T MCI_DecodePicture(UINT8 *data, UINT16 length, char* outbuf,long outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit, short kind, UINT16 img_id)
{

    if((data[0]=='B')&&(data[1]=='M'))	/* bitmap file */
    {
        MMC_ReadBitmapPicture(data, outbuf, outbuf_len, width, height, start_x, start_y, end_x, end_y, decUnit, kind);
    }
    else if((data[0]==0xff)&&(data[1]==0xd8))	/* JPEG file */
    {
        MMF_JpegDecodePicture(data, outbuf, outbuf_len, width, height, start_x, start_y, end_x, end_y, decUnit, kind, NULL, img_id);
    }
    else
        return MCI_ERR_UNKNOWN_FORMAT;

}


