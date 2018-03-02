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

////////////////////////////////////////////////////////////////////////////////
#ifdef CAMERA_SUPPORT

#include "cs_types.h"
#include "global.h"
#include "event.h"
#include "mcip_debug.h"
#include "camera_m.h"
#include "mmc_camera.h"
#include "mmc.h"
#include "mmc_timer.h"
//#include "lcd_if.h"
#include "lcdd_m.h"
#include "vpp_audiojpegenc.h"
#include "sxr_tls.h"
#include "csw.h"
#include "hal_timers.h"
#include "hal_sys.h"
#include "mci_lcd.h"
#include "mci.h"
#include "fs.h"
#include "pmd_m.h"
//#include "mmc_previewvoc.h"
#include "mmc_jpegencvoc.h"
//#include "vpp_preview.h"


#include "mmc_videorecord.h"

#ifdef _CAM_SPECIAL_GC6113_
#include "base_prv.h"
#endif

#include "hal_gouda.h"
volatile int32 countdown =3;
volatile int32 sensorready=0;

UINT16* sensorbuf = NULL;
UINT16* blendBuffer = NULL;

// pointer to the memory which stores the image sent out by ISP
LCDD_FBW_T displayFbw;

//CAM_SIZE_T MY_NPIX = CAM_NPIX_VGA;

// recors the width and the height of the image sent out by ISP
volatile UINT16 srcwidth=0, srcheight=0; // the width and height of the image sent out by ISP
volatile UINT16 cutwidth=0, cutheight=0; // the width and height of the image to be zoomed

volatile CAM_INFO_STRUCT CameraInfo = {0, };

volatile UINT32 startPos = 0;   // the start pixel to be previewed
volatile UINT16 bset=0; // 0 - special effect was not initiated, 1 - special effect was initiated  


static INT32 multicapture=0;
static INT32 previewstatus=0;
static UINT32 cam_wbValue = 0;


#if defined(VIDEO_CHAT_SUPPORT) || defined(__PRJ_WITH_SPICAM__)
#define   Q15      32767
#define   Q14      16384
#define   S15      15
#define   MASK     (Q15-1)

#ifndef DRV_RGB2PIXEL565
#define DRV_RGB2PIXEL565(r,g,b)	\
	((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3))
#endif /* DRV_RGB2PIXEL565 */

const BYTE rgb_sat[768] = 
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
    0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
    0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
    0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
    0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
};
const short TableV2G[]  =
{
 0,   0,   1,   2,   2,   3,   4,   5,   5,   6,   7,   7,   8,   9,  10,  10, 
 11,  12,  12,  13,  14,  15,  15,  16,  17,  17,  18,  19,  20,  20,  21,  22, 
 22,  23,  24,  25,  25,  26,  27,  27,  28,  29,  30,  30,  31,  32,  32,  33, 
 34,  35,  35,  36,  37,  37,  38,  39,  40,  40,  41,  42,  42,  43,  44,  45, 
 45,  46,  47,  47,  48,  49,  50,  50,  51,  52,  52,  53,  54,  55,  55,  56, 
 57,  57,  58,  59,  60,  60,  61,  62,  62,  63,  64,  65,  65,  66,  67,  67, 
 68,  69,  70,  70,  71,  72,  72,  73,  74,  75,  75,  76,  77,  77,  78,  79, 
 80,  80,  81,  82,  82,  83,  84,  85,  85,  86,  87,  87,  88,  89,  90,  90, 
 91,  92,  92,  93,  94,  95,  95,  96,  97,  97,  98,  99,  100, 100, 101, 102, 
 102, 103, 104, 105, 105, 106, 107, 107, 108, 109, 110, 110, 111, 112, 112, 113, 
 114, 115, 115, 116, 117, 117, 118, 119, 120, 120, 121, 122, 122, 123, 124, 125, 
 125, 126, 127, 127, 128, 129, 130, 130, 131, 132, 132, 133, 134, 135, 135, 136, 
 137, 137, 138, 139, 140, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 147, 
 148, 149, 150, 150, 151, 152, 152, 153, 154, 155, 155, 156, 157, 157, 158, 159, 
 160, 160, 161, 162, 162, 163, 164, 165, 165, 166, 167, 167, 168, 169, 170, 170, 
 171, 172, 172, 173, 174, 175, 175, 176, 177, 177, 178, 179, 180, 180, 181, 182
};
const short TabV2R[] =
{
-179 ,-178 ,-177 ,-175 ,-174 ,-172 ,-171 ,-170 ,
-168 ,-167 ,-165 ,-164 ,-163 ,-161 ,-160 ,-158 ,
-157 ,-156 ,-154 ,-153 ,-151 ,-150 ,-149 ,-147 ,
-146 ,-144 ,-143 ,-142 ,-140 ,-139 ,-137 ,-136 ,
-135 ,-133 ,-132 ,-130 ,-129 ,-128 ,-126 ,-125 ,
-123 ,-122 ,-121 ,-119 ,-118 ,-116 ,-115 ,-114 ,
-112 ,-111 ,-109 ,-108 ,-107 ,-105 ,-104 ,-102 ,
-101 ,-100 , -98 , -97 , -95 , -94 , -93 , -91 ,
 -90 , -88 , -87 , -86 , -84 , -83 , -81 , -80 ,
 -79 , -77 , -76 , -74 , -73 , -72 , -70 , -69 ,
 -67 , -66 , -65 , -63 , -62 , -60 , -59 , -57 ,
 -56 , -55 , -53 , -52 , -50 , -49 , -48 , -46 ,
 -45 , -43 , -42 , -41 , -39 , -38 , -36 , -35 ,
 -34 , -32 , -31 , -29 , -28 , -27 , -25 , -24 ,
 -22 , -21 , -20 , -18 , -17 , -15 , -14 , -13 ,
 -11 , -10 ,  -8 ,  -7 ,  -6 ,  -4 ,  -3 ,  -1 ,
   0 ,   1 ,   3 ,   4 ,   6 ,   7 ,   8 ,  10 ,
  11 ,  13 ,  14 ,  15 ,  17 ,  18 ,  20 ,  21 ,
  22 ,  24 ,  25 ,  27 ,  28 ,  29 ,  31 ,  32 ,
  34 ,  35 ,  36 ,  38 ,  39 ,  41 ,  42 ,  43 ,
  45 ,  46 ,  48 ,  49 ,  50 ,  52 ,  53 ,  55 ,
  56 ,  57 ,  59 ,  60 ,  62 ,  63 ,  65 ,  66 ,
  67 ,  69 ,  70 ,  72 ,  73 ,  74 ,  76 ,  77 ,
  79 ,  80 ,  81 ,  83 ,  84 ,  86 ,  87 ,  88 ,
  90 ,  91 ,  93 ,  94 ,  95 ,  97 ,  98 , 100 ,
 101 , 102 , 104 , 105 , 107 , 108 , 109 , 111 ,
 112 , 114 , 115 , 116 , 118 , 119 , 121 , 122 ,
 123 , 125 , 126 , 128 , 129 , 130 , 132 , 133 ,
 135 , 136 , 137 , 139 , 140 , 142 , 143 , 144 ,
 146 , 147 , 149 , 150 , 151 , 153 , 154 , 156 ,
 157 , 158 , 160 , 161 , 163 , 164 , 165 , 167 ,
 168 , 170 , 171 , 172 , 174 , 175 , 177 , 178 ,
};
const short TabU2G[] =
{
 135 , 135 , 135 , 134 , 134 , 134 , 133 , 133 ,
 133 , 132 , 132 , 132 , 131 , 131 , 131 , 130 ,
 130 , 130 , 129 , 129 , 129 , 128 , 128 , 128 ,
 127 , 127 , 127 , 126 , 126 , 126 , 125 , 125 ,
 124 , 124 , 124 , 123 , 123 , 123 , 122 , 122 ,
 122 , 121 , 121 , 121 , 120 , 120 , 120 , 119 ,
 119 , 119 , 118 , 118 , 118 , 117 , 117 , 117 ,
 116 , 116 , 116 , 115 , 115 , 115 , 114 , 114 ,
 113 , 113 , 113 , 112 , 112 , 112 , 111 , 111 ,
 111 , 110 , 110 , 110 , 109 , 109 , 109 , 108 ,
 108 , 108 , 107 , 107 , 107 , 106 , 106 , 106 ,
 105 , 105 , 105 , 104 , 104 , 104 , 103 , 103 ,
 102 , 102 , 102 , 101 , 101 , 101 , 100 , 100 ,
 100 ,  99 ,  99 ,  99 ,  98 ,  98 ,  98 ,  97 ,
  97 ,  97 ,  96 ,  96 ,  96 ,  95 ,  95 ,  95 ,
  94 ,  94 ,  94 ,  93 ,  93 ,  93 ,  92 ,  92 ,
  91 ,  91 ,  91 ,  90 ,  90 ,  90 ,  89 ,  89 ,
  89 ,  88 ,  88 ,  88 ,  87 ,  87 ,  87 ,  86 ,
  86 ,  86 ,  85 ,  85 ,  85 ,  84 ,  84 ,  84 ,
  83 ,  83 ,  83 ,  82 ,  82 ,  82 ,  81 ,  81 ,
  80 ,  80 ,  80 ,  79 ,  79 ,  79 ,  78 ,  78 ,
  78 ,  77 ,  77 ,  77 ,  76 ,  76 ,  76 ,  75 ,
  75 ,  75 ,  74 ,  74 ,  74 ,  73 ,  73 ,  73 ,
  72 ,  72 ,  72 ,  71 ,  71 ,  71 ,  70 ,  70 ,
  69 ,  69 ,  69 ,  68 ,  68 ,  68 ,  67 ,  67 ,
  67 ,  66 ,  66 ,  66 ,  65 ,  65 ,  65 ,  64 ,
  64 ,  64 ,  63 ,  63 ,  63 ,  62 ,  62 ,  62 ,
  61 ,  61 ,  61 ,  60 ,  60 ,  60 ,  59 ,  59 ,
  58 ,  58 ,  58 ,  57 ,  57 ,  57 ,  56 ,  56 ,
  56 ,  55 ,  55 ,  55 ,  54 ,  54 ,  54 ,  53 ,
  53 ,  53 ,  52 ,  52 ,  52 ,  51 ,  51 ,  51 ,
  50 ,  50 ,  50 ,  49 ,  49 ,  49 ,  48 ,  48 ,
};
const short TabU2B[] =
{
-227 ,-226 ,-224 ,-222 ,-220 ,-219 ,-217 ,-215 ,
-213 ,-212 ,-210 ,-208 ,-206 ,-204 ,-203 ,-201 ,
-199 ,-197 ,-196 ,-194 ,-192 ,-190 ,-188 ,-187 ,
-185 ,-183 ,-181 ,-180 ,-178 ,-176 ,-174 ,-173 ,
-171 ,-169 ,-167 ,-165 ,-164 ,-162 ,-160 ,-158 ,
-157 ,-155 ,-153 ,-151 ,-149 ,-148 ,-146 ,-144 ,
-142 ,-141 ,-139 ,-137 ,-135 ,-134 ,-132 ,-130 ,
-128 ,-126 ,-125 ,-123 ,-121 ,-119 ,-118 ,-116 ,
-114 ,-112 ,-110 ,-109 ,-107 ,-105 ,-103 ,-102 ,
-100 , -98 , -96 , -94 , -93 , -91 , -89 , -87 ,
 -86 , -84 , -82 , -80 , -79 , -77 , -75 , -73 ,
 -71 , -70 , -68 , -66 , -64 , -63 , -61 , -59 ,
 -57 , -55 , -54 , -52 , -50 , -48 , -47 , -45 ,
 -43 , -41 , -40 , -38 , -36 , -34 , -32 , -31 ,
 -29 , -27 , -25 , -24 , -22 , -20 , -18 , -16 ,
 -15 , -13 , -11 ,  -9 ,  -8 ,  -6 ,  -4 ,  -2 ,
   0 ,   1 ,   3 ,   5 ,   7 ,   8 ,  10 ,  12 ,
  14 ,  15 ,  17 ,  19 ,  21 ,  23 ,  24 ,  26 ,
  28 ,  30 ,  31 ,  33 ,  35 ,  37 ,  39 ,  40 ,
  42 ,  44 ,  46 ,  47 ,  49 ,  51 ,  53 ,  54 ,
  56 ,  58 ,  60 ,  62 ,  63 ,  65 ,  67 ,  69 ,
  70 ,  72 ,  74 ,  76 ,  78 ,  79 ,  81 ,  83 ,
  85 ,  86 ,  88 ,  90 ,  92 ,  93 ,  95 ,  97 ,
  99 , 101 , 102 , 104 , 106 , 108 , 109 , 111 ,
 113 , 115 , 117 , 118 , 120 , 122 , 124 , 125 ,
 127 , 129 , 131 , 133 , 134 , 136 , 138 , 140 ,
 141 , 143 , 145 , 147 , 148 , 150 , 152 , 154 ,
 156 , 157 , 159 , 161 , 163 , 164 , 166 , 168 ,
 170 , 172 , 173 , 175 , 177 , 179 , 180 , 182 ,
 184 , 186 , 187 , 189 , 191 , 193 , 195 , 196 ,
 198 , 200 , 202 , 203 , 205 , 207 , 209 , 211 ,
 212 , 214 , 216 , 218 , 219 , 221 , 223 , 225 ,
};


void  zoom_line_yuv(uint16* srcbuf,uint16* dec, uint16 dec_w,int scalex)
{
    //input format : y1 u y2 v 
    short	wa, wb, i, w;
    int		wn;
    uint16	y, y1, u, u1, v, v1;
    short	*pb = dec;
    short	*pbu = dec + dec_w;
    uint16	*pa = srcbuf;

    wn = 0;
    for (i = 0; i < (dec_w >> 1); i++)
    {
        wb = ( wn) & (MASK);
        wa = Q15 - wb;

        w  = (wn >> S15);
        /////////u v//////////
        pa = srcbuf + (w << 1);
        u = (*(pa)) >> 8;
        v = (*(pa + 1)) >> 8;
        u1 = (*(pa + 2)) >> 8;
        v1 = (*(pa + 3)) >> 8;
        /////////y////////////
        pa = srcbuf + w ;
        y = ((*pa++)) & 0xff;
        y1 = ((*pa)) & 0xff;

        *pb++ =(y * wa + y1 * wb) >> S15;
        *pbu++ = (u * wa + u1 * wb) >> S15;
        *pbu++ = (v * wa + v1 * wb) >> S15;

        wn += scalex;
    }

    for (i = 0; i < (dec_w >> 1); i++)
    {
        wb = ( wn) & (MASK);
        wa = Q15 - wb;

        w  = (wn >> S15);

        pa = srcbuf + w ;
        y = ((*pa++)) & 0xff;
        y1 = ((*pa)) & 0xff;

        *pb++ =(y * wa + y1 * wb) >> S15;

        wn += scalex;
    }
}

int Coolsand_bilinerz_zoom_op_yuv2rgb(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa, wb, i, j;
    int    w,scalex,scaley;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    int y,u,v,y1,u1,v1,y2,r,g,b;
    short*  tmpbuf =(short*)mmc_MemMallocBack(dec_w*8) ;
    short*  ptmp2 = tmpbuf + dec_w*2;
    short  num1,num2,num3;
    BYTE *irgb_sat = (BYTE *)(rgb_sat+256);

    if(tmpbuf==NULL)
        return -1;

    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1; 
    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;
    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;
    w  = 0;
    num2 = -2;
    num3 = 1;
    //p3 = pa;
    //p4 = p3 + src_w;
    //zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
    //zoom_line_yuv(p4,ptmp2,dec_w,scalex);
    p1 = tmpbuf;
    p2 = ptmp2;
    for (j=0;j<dec_h;j++)
    {
        num1 = (w>>S15);
        p3 = pa + (num1*src_w);
        p4 = p3 + src_w;

        if (num1-num2==1)
        {
            if (num3==1)
            {
                zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                p2 = tmpbuf;
                p1 = ptmp2;
                num3=0;
            }
            else
            {
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }
        }
        else if (num1!=num2)
        {
            zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
            zoom_line_yuv(p4,ptmp2,dec_w,scalex);
            p1 = tmpbuf;
            p2 = ptmp2;
            num3 = 1;
        }
        wb = (w)&(MASK);
        wa = Q15 - wb;

        p3 = p1;
        p4 = p2;
        p5 = p1 + dec_w;
        p6 = p2 + dec_w;
        for (i=0;i<(dec_w>>1);i++)
        {
            y = *p3++;
            u = *p5++;
            v = *p5++;
            y1 = *p4++;
            u1 = *p6++;
            v1 = *p6++;
            y = ((y*wa + y1*wb)>>S15);
            u = ((u*wa + u1*wb)>>S15);
            v = ((v*wa + v1*wb)>>S15);
            ////////////////////////////
            y1 = *p3++;
            y2 = *p4++;
            y1 = ((y1*wa + y2*wb)>>S15);
            r = irgb_sat[y + TabV2R[v]];
            g = irgb_sat[y + TabU2G[u] - TableV2G[v]];
            b = irgb_sat[y + TabU2B[u]];
            *pb++ = DRV_RGB2PIXEL565(r,g,b);
            r = irgb_sat[y1 + TabV2R[v]];
            g = irgb_sat[y1 + TabU2G[u] - TableV2G[v]];
            b = irgb_sat[y1 + TabU2B[u]];
            *pb++ = DRV_RGB2PIXEL565(r,g,b);
        }
        w+=scaley;
        num2 = num1;
    }
    mmc_MemFreeLast((char*)tmpbuf);
    return 0;
}

int Coolsand_bilinerz_zoom_op_yuv(uint16* srcbuf,uint16 src_w,uint16 src_h,uint16 cut_w,uint16 cut_h,uint16* decbuf,uint16 dec_w,uint16 dec_h)
{
    short  offx,offy;
    short  wa,wb,i,j;
    int    w,scalex,scaley;
    uint16  *pb ,*pa,*p1,*p2,*p3,*p4,*p5,*p6;
    short y,u,v,y1,u1,v1;
    short*  tmpbuf =NULL;
    short*  ptmp2 = NULL;
    short  num1,num2,num3;

    if ((src_w == dec_w) && (src_h == dec_h))
    {
        if (srcbuf != decbuf)
            memcpy((UINT8*)decbuf, (UINT8*)srcbuf, ((src_w*src_h)<<1));
        return 0;
    }
    tmpbuf = (short*)mmc_MemMallocBack(dec_w*8) ;
    ptmp2 = tmpbuf + dec_w*2;
    if(tmpbuf==NULL)
        return -1;

    offx = (src_w - cut_w)>>1;
    offx = (offx>>1)<<1;
    offy = (src_h - cut_h)>>1;

    scalex=((cut_w-1)<<S15)/dec_w;
    scaley=((cut_h-1)<<S15)/dec_h;

    pa = srcbuf + offy*src_w + offx;
    pb = decbuf;

    w  = 0;
    num2 = -2;
    num3 = 1;

    p1 = tmpbuf;
    p2 = ptmp2;

    if (dec_h >= 480)
    {
        short*  PTmp=(short*)mmc_MemMallocBack(320*61*2) ;
        if(PTmp==NULL)
            return -1;

        memcpy(PTmp,decbuf+320*179,320*61*2);//keep one more line
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);

            if (num1>(178-offy))//for 640*480
            {
                pa = PTmp + offx;
                w = w & MASK;
                num1 = num1-179+offy;
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }
            else
            {
                p3 = pa + (num1*src_w);
                p4 = p3 + src_w;
            }

            if (num1-num2==1)
            {
                if (num3==1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }

            wb =( w)&(MASK);
            wa = Q15 - wb;

            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;
            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);

                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }

            w+=scaley;
            num2 = num1;
        }

        mmc_MemFreeLast((char*)PTmp);
    }
    else
    {
        for (j=0;j<dec_h;j++)
        {
            num1 = (w>>S15);
            p3 = pa + (num1*src_w);
            p4 = p3 + src_w;

            if (num1 - num2 == 1)
            {
                if (num3 == 1)
                {
                    zoom_line_yuv(p4,tmpbuf,dec_w,scalex);
                    p2 = tmpbuf;
                    p1 = ptmp2;
                    num3=0;
                }
                else
                {
                    zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                    p1 = tmpbuf;
                    p2 = ptmp2;
                    num3 = 1;
                }
            }
            else if (num1!=num2)
            {
                zoom_line_yuv(p3,tmpbuf,dec_w,scalex);
                zoom_line_yuv(p4,ptmp2,dec_w,scalex);
                p1 = tmpbuf;
                p2 = ptmp2;
                num3 = 1;
            }

            wb =( w)&(MASK);
            wa = Q15 - wb;

            p3 = p1;
            p4 = p2;
            p5 = p1 + dec_w;
            p6 = p2 + dec_w;

            for (i=0;i<(dec_w>>1);i++)
            {
                y = *p3++;
                u = *p5++;
                y1 = *p4++;
                u1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                u = ((u*wa + u1*wb)>>S15);

                *pb++ = (u<<8)|(y);
                /////////////////////////////
                y = *p3++;
                v = *p5++;
                y1 = *p4++;
                v1 = *p6++;

                y = ((y*wa + y1*wb)>>S15);
                v = ((v*wa + v1*wb)>>S15);
                *pb++ = (v<<8)|(y);
            }
            w+=scaley;
            num2 = num1;
        }
    }

    mmc_MemFreeLast((char*)tmpbuf);

    return 0;
}
#endif


VOID lcd_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{

}

void set_multicaputure(int i)
{
    multicapture = i;
}

void set_mmc_camera_preview()
{
    previewstatus = 1;
}



#ifdef __PRJ_WITH_SPICAM__

typedef enum {
    MMC_TASK_STOPPED,
    MMC_TASK_INITIALIZED,
    MMC_TASK_PREVIEWING,
    MMC_TASK_CAPTURING,
} MMC_TASK_STATE_E;

#define MSG_MMC_BAL_CAPTURE_REQ 0x1001
#define MMC_CAM_SRC_WIDTH 240
#define MMC_CAM_SRC_HEIGHT 320


static MMC_TASK_STATE_E s_mmcTaskState = MMC_TASK_STOPPED;



//UINT16 capture_buffer[240*320];
int Lily_Camera_PowerOn(INT32  videomode, void(*cb)(int32))
{
    UINT16* InvalideAddress = (UINT16*)convertaddr(NULL);
    UINT8 camId = 0xff;

    if (s_mmcTaskState != MMC_TASK_STOPPED)
        return MDI_RES_CAMERA_ERR_FAILED;

    mmc_SetCurrMode(MMC_MODE_CAMERA);
    camera_Reserve(NULL);
    // note here: just power on the sensor but don't start the preview
    camera_PowerOn(CAM_NPIX_QVGA, 128, 160, CAM_FORMAT_RGB565);
    camId = camera_GetId();
    if (camId == 0)
    {
        camera_PowerOff();
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }

    srcwidth = MMC_CAM_SRC_WIDTH;
    srcheight = MMC_CAM_SRC_HEIGHT;
    sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(srcwidth*srcheight*4));

    if (sensorbuf == InvalideAddress)
    {
        camera_PowerOff();
        return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }
    blendBuffer = sensorbuf + srcwidth*srcheight;

    bset = 0;
    s_mmcTaskState = MMC_TASK_INITIALIZED;
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PowerOff()
{
    if (s_mmcTaskState == MMC_TASK_CAPTURING)
        while (sensorready == 0)
            COS_SleepByTick(16);

    Lily_Camera_PreviewOff();
    bset = 2;
    mmc_SetCurrMode(MMC_MODE_IDLE);
    camera_PowerOff();
    mmc_MemFreeAll();

    s_mmcTaskState = MMC_TASK_STOPPED;
    return MDI_RES_CAMERA_SUCCEED;
}
extern VOID pas6175_preview(UINT16 Width, UINT16 Height);

int Lily_Camera_Preview(CAM_PREVIEW_STRUCT* data)
{
    if (s_mmcTaskState != MMC_TASK_INITIALIZED)
        return MDI_RES_CAMERA_ERR_FAILED;

    mci_LcdStartBypass(lcd_BlockWriteBypass);

    CameraInfo.start_x = data->start_x;
    CameraInfo.start_y = data->start_y;
    CameraInfo.end_x = data->end_x;
    CameraInfo.end_y = data->end_y;
    CameraInfo.preview_width = data->end_x - data->start_x + 1;
    CameraInfo.preview_height = data->end_y - data->start_y + 1;
    CameraInfo.image_height = data->image_height;
    CameraInfo.image_width = data->image_width;
    CameraInfo.preview_rotate=0;
    CameraInfo.nightmode = data->nightmode;

    cutwidth = srcwidth;
    cutheight = srcwidth * CameraInfo.preview_height/ CameraInfo.preview_width;
    if(cutheight > srcheight)
    {
        cutheight = srcheight;
        cutwidth = srcheight * CameraInfo.preview_width/CameraInfo.preview_height;
    }
    startPos = srcwidth*((srcheight-cutheight)>>1) + ((srcwidth-cutwidth)>>1);

    displayFbw.fb.buffer = NULL;
    displayFbw.fb.width = MMC_LcdWidth;
    displayFbw.fb.height = MMC_LcdHeight;
    displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
    displayFbw.roi.width = MMC_LcdWidth;
    displayFbw.roi.height = MMC_LcdHeight;
    displayFbw.roi.x = 0;
    displayFbw.roi.y = 0;

    pas6175_preview(128, 160);

    if(bset==0)
    {
        COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID, COS_TIMER_MODE_SINGLE, 200);        
        bset = 1;
    }

    s_mmcTaskState = MMC_TASK_PREVIEWING;
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PreviewOff()
{
    if (s_mmcTaskState != MMC_TASK_PREVIEWING)
        return MDI_RES_CAMERA_ERR_FAILED;

    mci_LcdStopBypass();

    s_mmcTaskState = MMC_TASK_INITIALIZED;
    return MDI_RES_CAMERA_SUCCEED;
}


int Lily_Camera_Capture(CAM_CAPTURE_STRUCT *data) 
{
    COS_EVENT ev = {0, };

    if (s_mmcTaskState == MMC_TASK_STOPPED)
        return MDI_RES_CAMERA_ERR_FAILED;

    Lily_Camera_PreviewOff();

    sensorready = 0;
    ev.nEventId = MSG_MMC_BAL_CAPTURE_REQ;
    ev.nParam1 = (UINT32)sensorbuf;
    COS_SendEvent(BAL_TH(COS_BK_TASK_PRI), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    //camera_SetFlashL(CAM_FLASH_FORCED);
    while (sensorready == 0)
    {
        //sxr_Sleep(500*16384/1000);
        COS_SleepByTick(8192);
    }
    
    return MDI_RES_CAMERA_SUCCEED;
}


int Lily_Capture_Preview()
{
    HAL_GOUDA_VID_LAYER_DEF_T gouda_vid_def = {0, };  
    LCDD_ERR_T err = LCDD_ERR_NO;
    UINT16 tmpw = CameraInfo.preview_width;
    UINT16 tmph = CameraInfo.preview_height;

    gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
    gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
    gouda_vid_def.width =  cutwidth;
    gouda_vid_def.height = cutheight;
    gouda_vid_def.stride = srcwidth*2;

    gouda_vid_def.alpha = 0xFF;
    gouda_vid_def.cKeyColor = 0;
    gouda_vid_def.cKeyEn = FALSE;
    gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
    gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;

    gouda_vid_def.pos.tlPX = CameraInfo.start_x;
    gouda_vid_def.pos.tlPY = CameraInfo.start_y;
    gouda_vid_def.pos.brPX  = CameraInfo.end_x;
    gouda_vid_def.pos.brPY  = CameraInfo.end_y;

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
    hal_GoudaVidLayerOpen(&gouda_vid_def);

    memset((UINT8*)blendBuffer, 0, srcwidth*srcheight*2);
    displayFbw.fb.buffer = blendBuffer; 
    err = lcdd_Blit16(&displayFbw,0 ,0);
    while (LCDD_ERR_NO != err)
    {
        COS_SleepByTick(500);
        err = lcdd_Blit16(&displayFbw,0 ,0);
    };
    hal_GoudaVidLayerClose();

    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_camera_savePhoto(int  file_handle)
{
    int len = 0;
    int zoomRes = 1;
    UINT16 dec_w = 0, dec_h = 0;

    if (file_handle < 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]FILE HANDLE IS INVALIBLE!");
        return MDI_RES_CAMERA_ERR_FAILED;
    }

    initVocJpegEncode();
    if ((CameraInfo.image_width < srcwidth) && (CameraInfo.image_height < srcheight))
    {
        dec_w = CameraInfo.image_width;
        dec_h = CameraInfo.image_height;
    }
    else
    {
        dec_w = 320;
        dec_h = 240;
    }

    zoomRes = Coolsand_bilinerz_zoom_op_yuv(sensorbuf, srcwidth, srcheight, cutwidth, cutheight, 
                                        blendBuffer, dec_w, dec_h);
    if (zoomRes)
    {
        quitVocJpegEncode();
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    MMC_jpgEn(CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, 
                                    (char*)convertaddr(blendBuffer), (char*)sensorbuf, 1);    


    len= FS_Write(file_handle, (uint8*)sensorbuf + 1, getJpegEncodeLen());
    quitVocJpegEncode();

    if (len <= 0)
    {
        return MDI_RES_CAMERA_ERR_DISK_FULL;
    }
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_SetEffect(CAM_SET_PARA effectCode,uint32 value)
{
    uint32 localEffectCode=0;
    uint32 localValue=0;    
    UINT16 tmpw = 0;
    UINT16 tmph = 0;

    switch (effectCode)
    {
        case CAM_PARAM_CONTRAST:
            localEffectCode = CameraSetContrast;
            localValue =5- value;
            if(localValue < 0)
            {
                localValue = 0;
            }
            camera_SetContrastL(localValue);
            break;
            
        case CAM_PARAM_BRIGHTNESS:
            localEffectCode = CameraSetBrightness;
            localValue = value+1;
            if(localValue > 5)
            {
                localValue = 5;
            }
            camera_SetBrightnessL(localValue);
            break;
            
        case CAM_PARAM_EFFECT:
            switch(value)
            {
                case MDI_CAMERA_EFFECT_NORMAL:
                    localEffectCode = CAM_SPE_NORMAL;
                    break;
                case MDI_CAMERA_EFFECT_ANTIQUE:
                    localEffectCode = CAM_SPE_ANTIQUE;
                    break;
                case MDI_CAMERA_EFFECT_REDRISH:
                    localEffectCode = CAM_SPE_REDISH;
                    break;
                case MDI_CAMERA_EFFECT_GREENRISH:
                    localEffectCode = CAM_SPE_GREENISH;
                    break;
                case MDI_CAMERA_EFFECT_BLUERISH:
                    localEffectCode = CAM_SPE_BLUEISH;
                    break;
                case MDI_CAMERA_EFFECT_BLACKWHITE:
                    localEffectCode = CAM_SPE_BLACKWHITE;
                    break;
                case MDI_CAMERA_EFFECT_NEGATIVENESS:
                    localEffectCode = CAM_SPE_NEGATIVE;
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]ERROR CAM_PARAM_EFFECT");
                    localEffectCode = MDI_CAMERA_EFFECT_NORMAL;
                    break;
            }
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]localEffectCode=%d",localEffectCode);
            camera_SetSpecialEffect(localEffectCode);
            break;
            
        case CAM_PARAM_WB:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_WB=%d",value);
            switch(value)
            {
                case 0:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
                case 1:
                    camera_SetWhiteBalanceL(CAM_WB_DAYLIGHT);
                    break;
                case 2:
                    camera_SetWhiteBalanceL(CAM_WB_TUNGSTEN);
                    break;
                case 3:
                    camera_SetWhiteBalanceL(CAM_WB_OFFICE);
                    break;
                case 4:
                    camera_SetWhiteBalanceL(CAM_WB_CLOUDY);
                    break;				
                default:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
            }
            break;
            
        case CAM_PARAM_IMAGESIZE:
            if(1) //(MY_NPIX != CAM_NPIX_QQVGA)//camera
            {
                switch(value)
                {
                    case 1:
                        hal_HstSendEvent(SYS_EVENT,0xbaba0001);
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 2:
                        hal_HstSendEvent(SYS_EVENT,0xbaba0002);
                        CameraInfo.image_width =  176;
                        CameraInfo.image_height = 220;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= MMC_LcdHeight;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = 0;
                        break;
                    case 3:
                        hal_HstSendEvent(SYS_EVENT,0xbaba0003);
                        CameraInfo.image_width =  320;
                        CameraInfo.image_height = 240;
                        CameraInfo.preview_width = MMC_LcdWidth;
                        CameraInfo.preview_height= 
                                    (MMC_LcdWidth*CameraInfo.image_height /CameraInfo.image_width)&0xfffffffe;
                        CameraInfo.start_x = 0;
                        CameraInfo.start_y = (MMC_LcdHeight - CameraInfo.preview_height) >> 1;
                        break;
                    default:
                        hal_HstSendEvent(SYS_EVENT,0xbaba0004);                        
                        hal_HstSendEvent(SYS_EVENT,value);                        
                        CameraInfo.image_width =  MMC_LcdWidth;
                        CameraInfo.image_height = MMC_LcdHeight;
                        break;
                }
                
                tmpw = CameraInfo.preview_width;
                tmph = CameraInfo.preview_height;
                cutwidth = srcwidth;
                cutheight = srcwidth * tmph / tmpw;
                if(cutheight > srcheight)
                {
                    cutheight = srcheight;
                    cutwidth = srcheight * tmpw/tmph;
                }
                startPos = srcwidth*((srcheight-cutheight)>>1) + ((srcwidth-cutwidth)>>1);
            }
            break;
            
        default:
            break;
    }
	//return 0;
}

void Camera_UserMsg(COS_EVENT *pnMsg)
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Camera_UserMsg pnMsg->nEventId:%d ",pnMsg->nEventId);

    switch (pnMsg->nEventId)
    {
        case EV_TIMER:
            switch(pnMsg->nParam1)
            {
                case MMC_CAMERA_TIMER_ID:
                    if (bset < 2) //bset==2 means the camera is power off
                        Lily_Camera_SetEffect(CAM_PARAM_WB, CameraInfo.nightmode);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

#else

#ifdef _ATV_SPLIT_
#undef _ATV_SPLIT_  // this functionality is disabled now
#endif

#if defined(__CAMERA_FULL_OSD__) || defined(__IMAGE_ROTATE_EN__)
ROTATE_TABLE_T rtable = {0,};
#endif

extern void mmc_atvIntraFrameAveAsm(UINT32* imgBuf, UINT32 srcwidth, UINT32 cutwidth, UINT32 cutheight);

volatile UINT16 cutwidth_orig = 0, cutheight_orig = 0;
INT32 previewstop = 0; // 0 - in preview; 1 - preview stopped



#if defined(_CAM_SMALL_MEM_) && defined(GALLITE_IS_8805)
#define _CAM_SCALE_IN_VOC_
#endif

#if defined(_CAM_SCALE_IN_VOC_)
static UINT16 turn = 0;
static UINT16* bufPointer[2] = {NULL, NULL};
static BOOL s_vocinited = FALSE;
#endif


BOOL isVideoRecMode = FALSE;
PRIVATE HAL_SYS_VOC_FREQ_T g_mmcCameraPrevVocFreq = 0;

// this variable is not used now for gallite, and kept here just for the compatibility with greenstone
MMC_CAMERA_CB mmc_camCB; 


#if defined(__IMAGE_ROTATE_EN__) || defined(__CAMERA_FULL_OSD__)

UINT8 rotate_table_init(UINT16 imgw, UINT16 imgh)
{
    UINT32 size = imgw * imgh;

    rtable.idxTable = (UINT8*)COS_MALLOC(size>>3);
    if(rtable.idxTable == NULL)
        return 0;

    memset(rtable.idxTable, 0, (size>>3));
    rtable.sidx = 0;
    rtable.midx = size>>3;

    return 1;
}

void rotate_table_deinit()
{
	COS_FREE(rtable.idxTable);
    rtable.idxTable = NULL;
    rtable.sidx = rtable.midx = 0;
}

void rotate_table_reset()
{
    memset(rtable.idxTable, 0, rtable.midx);
    rtable.sidx = 0;
}

UINT8 rotate_table_set_idx(UINT32 idx)
{
    UINT32 i = 0;
    UINT32 index = idx >> 3;
    UINT32 offset = idx & 7;

    if(((rtable.idxTable[index])&(1<<offset)) != 0)
        return 0;

    rtable.idxTable[index] |= (1<<offset);
    for(i = rtable.sidx; i < rtable.midx;)
        if(rtable.idxTable[i] == 0xff) i++;
        else break;

    rtable.sidx = i;
    return 1;
}

UINT32 rotate_table_get_idx()
{
    UINT32 i = 0;

    if(rtable.sidx != rtable.midx)
        for(i = 0; i < 8; i++)
            if((rtable.idxTable[rtable.sidx]>>i)&1);
            else break;

    return (rtable.sidx<<3)+i;
}

UINT32 image_get_dst_idx(UINT32 sidx, 
					   IMG_ROTATE_E rotate,
					   UINT16 imgw,
					   UINT16 imgh)
{
    UINT32 y = sidx / imgw;
    UINT32 x = sidx % imgw;

    if(rotate == ROTATE_90)
        return (x*imgh + imgh - y - 1);
    else if(rotate == ROTATE_270)
        return ((imgw-x-1)*imgh + y);
    else
        return 0;
}

UINT8 image_rotate(UINT16* buf, IMG_ROTATE_E rotate, UINT16 width, UINT16 height)
{
    UINT32 v = 0;
    UINT32 sidx = 0;
    UINT32 didx = 0;
    UINT32 imgSize = width*height;

    if (!buf || !width || !height || rotate >= ROTATE_MAX || (imgSize&7)) 
        return 0;

    if (rotate == ROTATE_0) 
        return 1;

    if (rotate == ROTATE_180)
    {
        UINT32 max = imgSize >> 1;
        v = 0;
        do {
            buf[v] ^= buf[imgSize-v-1];
            buf[imgSize-v-1] ^= buf[v];
            buf[v] ^= buf[imgSize-v-1];
            v++;

            buf[v] ^= buf[imgSize-v-1];
            buf[imgSize-v-1] ^= buf[v];
            buf[v] ^= buf[imgSize-v-1];
            v++;

            buf[v] ^= buf[imgSize-v-1];
            buf[imgSize-v-1] ^= buf[v];
            buf[v] ^= buf[imgSize-v-1];
            v++;

            buf[v] ^= buf[imgSize-v-1];
            buf[imgSize-v-1] ^= buf[v];
            buf[v] ^= buf[imgSize-v-1];
            v++;
        } while(v != max);

        return 1;
    }

    rotate_table_reset();

    sidx = rotate_table_get_idx();
    v = buf[sidx];
    while(sidx != imgSize) 
    {
        do {
            didx = image_get_dst_idx(sidx, rotate, width, height);
            if(rotate_table_set_idx(didx) == 0) break;

            buf[didx] ^= v;
            v ^= buf[didx];
            buf[didx] ^= v;

            sidx = didx;
        } while(sidx != imgSize);

        sidx = rotate_table_get_idx();
        v = buf[sidx];
    }

    return 1;
}
#endif



#define AVERAGE_FRAME_NUM 3

#ifdef __MMI_ANALOGTV__ 
#include "atvd_m.h"
extern void atvif_ISPSettingInit(void);
extern void atvif_PreviewSettingInit(void);
extern ATV_ISP_SETTING_T* atvif_GetISPSetting(void);
static ATV_ISP_SETTING_T* Atv_Isp_Settint=NULL;
#ifdef _ATV_SPLIT_
static INT16 s_atvBlendCtrl = -1;
#endif
#endif

#ifdef _CAM_SPECIAL_GC6113_
UINT8* gTransBufPtr[2] = {NULL, NULL};
UINT8 gTransBufIdx = 0;
UINT32 gTransBufSize = 0;
#endif

int Lily_Camera_PowerOn(INT32 videomode, void(*cb)(int32))
{
    UINT16* InvalideAddress = (UINT16*)convertaddr(NULL);
    UINT8 camId = 0xff;

MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_CAMERA]Lily_Camera_PowerOn");
    memset(&CameraInfo, 0, sizeof(CameraInfo));
#ifdef __MMI_ANALOGTV__
    if (atvif_GetCurModule() == 1)
    {
        mmc_SetCurrMode(MMC_MODE_CAMERA);
        atvif_ISPSettingInit();

        Atv_Isp_Settint = atvif_GetISPSetting();
        CameraInfo.sensor_width = CameraInfo.prev_width = Atv_Isp_Settint->SrcWidth;
        CameraInfo.sensor_height = CameraInfo.prev_height= Atv_Isp_Settint->Srcheight;
        CameraInfo.prev_size = CameraInfo.sensor_size = CameraInfo.prev_width*CameraInfo.prev_height;
#if defined(IP_CAMERA_SUPPORT)
        camera_Reserve(Cam_CallbackFun);
#endif 
    }
    else
#endif
    {
        // Camera mode
        if (videomode == 0)
        {
            mmc_SetCurrMode(MMC_MODE_CAMERA);
            
#if defined(_CAM_SPECIAL_GC6113_)
            camera_Reserve(Cam_CallbackFun_gc6113);
            camera_GetImageInfo(&(CameraInfo.sensor_size), &(CameraInfo.sensor_width), &(CameraInfo.sensor_height));
            if (CameraInfo.sensor_size == 0) 
                return MDI_RES_CAMERA_ERR_HW_NOT_READY;

            CameraInfo.prev_size = CAM_NPIX_QQVGA_2;
            CameraInfo.prev_width = 128;
            CameraInfo.prev_height = 160;
#else
#if defined(IP_CAMERA_SUPPORT)
            camera_Reserve(Cam_CallbackFun);
#endif 
            camera_GetImageInfo(&(CameraInfo.sensor_size), &(CameraInfo.sensor_width), &(CameraInfo.sensor_height));
            if (CameraInfo.sensor_size == 0) 
                return MDI_RES_CAMERA_ERR_HW_NOT_READY;

            if (CameraInfo.sensor_size > CAM_NPIX_VGA)
            {
                CameraInfo.prev_size = CAM_NPIX_VGA;
                CameraInfo.prev_width = 640;
                CameraInfo.prev_height = 480;
            }
            else
            {
                CameraInfo.prev_size = CameraInfo.sensor_size;
                CameraInfo.prev_width = CameraInfo.sensor_width;
                CameraInfo.prev_height = CameraInfo.sensor_height;
            }
#endif            
        }
        // Video Recoder mode
        else
        {
#if defined(VIDEO_RECORDER_SUPPORT)
            camera_Reserve(VidRec_CallbackFun);
            camera_GetImageInfo(&(CameraInfo.sensor_size), &(CameraInfo.sensor_width), &(CameraInfo.sensor_height));
            if (CameraInfo.sensor_size == 0) 
                return MDI_RES_CAMERA_ERR_HW_NOT_READY;
        
    #if defined (VDOREC_MODE_VGA)
            if (CameraInfo.sensor_size >= CAM_NPIX_VGA)
            {
                CameraInfo.prev_size = CAM_NPIX_VGA;
                CameraInfo.prev_width = 640;
                CameraInfo.prev_height = 480;
            }
            else if (CameraInfo.sensor_size == CAM_NPIX_QVGA)
            {
                CameraInfo.prev_size = CameraInfo.sensor_size;
                CameraInfo.prev_width = CameraInfo.sensor_width;
                CameraInfo.prev_height = CameraInfo.sensor_height;
            }
            else
            {
                return MDI_RES_CAMERA_ERR_PREVIEW_FAILED;
            }
    #elif defined (VDOREC_MODE_QVGA)
            if (CameraInfo.sensor_size >= CAM_NPIX_VGA)
            {
                CameraInfo.prev_size = CAM_NPIX_QVGA;
                CameraInfo.prev_width = 320;
                CameraInfo.prev_height = 240;
            }
            else if (CameraInfo.sensor_size == CAM_NPIX_QVGA)
            {
                CameraInfo.prev_size = CameraInfo.sensor_size;
                CameraInfo.prev_width = CameraInfo.sensor_width;
                CameraInfo.prev_height = CameraInfo.sensor_height;
            }    
            else
            {
                return MDI_RES_CAMERA_ERR_PREVIEW_FAILED;
            }      
    #elif defined(VDOREC_MODE_QCIF)
            if (CameraInfo.sensor_size >= CAM_NPIX_VGA)
            {
                CameraInfo.prev_size = CAM_NPIX_QVGA;
                CameraInfo.prev_width = 320;
                CameraInfo.prev_height = 240;
            }
            else
            {
                CameraInfo.prev_size = CameraInfo.sensor_size;
                CameraInfo.prev_width = CameraInfo.sensor_width;
                CameraInfo.prev_height = CameraInfo.sensor_height;
            }
    #else
            CameraInfo.prev_size = CAM_NPIX_QQVGA;
            CameraInfo.prev_width = 160;
            CameraInfo.prev_height = 120;
    #endif
            isVideoRecMode = TRUE;
#else
            return MDI_RES_CAMERA_ERR_HW_NOT_READY;
#endif
        }
    }
    camera_PowerOn(CameraInfo.prev_size, CameraInfo.prev_width, CameraInfo.prev_height, CAM_FORMAT_RGB565);
    camId = camera_GetId();
    if (camId == 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Invalide sensor id: %d", camId);
        camera_PowerOff();
        return MDI_RES_CAMERA_ERR_HW_NOT_READY;
    }
    sensorbuf = NULL;
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc(360*288*2));
        blendBuffer = sensorbuf;
    }
    else if(videomode == 0)
#else
    if (videomode == 0)
#endif
    {
        blendBuffer	= NULL;
#if defined(_CAM_SCALE_IN_VOC_)
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc((CameraInfo.prev_width*CameraInfo.prev_height)<<2));
        blendBuffer = sensorbuf + CameraInfo.prev_width*CameraInfo.prev_height;
        bufPointer[0] = sensorbuf;
        bufPointer[1] = blendBuffer;
#elif defined(_CAM_SPECIAL_GC6113_)
        gTransBufSize = 240*320;
        blendBuffer = (UINT16*)convertaddr(mmc_MemMalloc((CameraInfo.prev_width*CameraInfo.prev_height)<<3));
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc((CameraInfo.prev_width*CameraInfo.prev_height)<<3));
        gTransBufPtr[0] = (UINT8*)(blendBuffer);
        gTransBufPtr[1] = gTransBufPtr[0] + gTransBufSize;
        gTransBufIdx = 0;
#else
        blendBuffer = (UINT16*)convertaddr(mmc_MemMalloc(((CameraInfo.sensor_width*CameraInfo.sensor_height)<<1) + 20480));
        sensorbuf = blendBuffer + 10240;
#endif        
    }
#if defined(VIDEO_RECORDER_SUPPORT)
    else
    {
#if defined(_CAM_SPECIAL_GC6113_)
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc((CameraInfo.prev_width*CameraInfo.prev_height)<<3));
#else
        sensorbuf = (UINT16*)convertaddr(mmc_MemMalloc((CameraInfo.prev_width*CameraInfo.prev_height)<<1));
#endif
        blendBuffer = sensorbuf;
    }
#endif
    if ((sensorbuf == InvalideAddress) || (blendBuffer == InvalideAddress))
    {
        camera_PowerOff();
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview lack of memory,sensorbuf=%d",sensorbuf);
        return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }

#ifdef __CAMERA_FULL_OSD__
    if (rotate_table_init(CameraInfo.prev_width, CameraInfo.prev_height) == 0)
    {
        camera_PowerOff();
        mmc_MemFreeAll();
        rotate_table_deinit();
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview lack of memory,sensorbuf=%d",sensorbuf);
        return MDI_RES_CAMERA_ERR_MEMORY_NOT_ENOUGH;
    }
    rotate_table_deinit();
#endif

#if defined(_CAM_SPECIAL_GC6113_)
    if (isVideoRecMode == FALSE)
        memset((UINT8*)blendBuffer, 0, ((CameraInfo.prev_width * CameraInfo.prev_height)<<3));
    memset((UINT8*)sensorbuf, 0, ((CameraInfo.prev_width * CameraInfo.prev_height)<<3));
#else
    memset((UINT8*)sensorbuf, 0, ((CameraInfo.prev_width * CameraInfo.prev_height)<<1));
#endif

    multicapture = 0;
    bset = 0;

    mmc_camCB = cb;

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() != 1)
#endif
    {
#if (CHIP_HAS_ASYNC_TCU)
#if defined(VDOREC_MODE_VGA) || defined(VDOREC_MODE_QVGA)
        if (isVideoRecMode)
            g_mmcCameraPrevVocFreq = hal_SysSetVocClock(HAL_SYS_VOC_FREQ_312M);
        else
#endif            
            g_mmcCameraPrevVocFreq = hal_SysSetVocClock(HAL_SYS_VOC_FREQ_208M);
#else
        g_mmcCameraPrevVocFreq = hal_SysSetVocClock(HAL_SYS_VOC_FREQ_78M);
#endif //CHIP_HAS_ASYNC_TCU
#if defined(_CAM_SCALE_IN_VOC_)
        if (videomode == 0)
        {
            initVocPreviewYUV();
            s_vocinited = TRUE;
        }
#endif
    }
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PowerOff()
{
MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PowerOff");

    countdown = 0;
    previewstop = 1;
    bset = 2;
    isVideoRecMode = FALSE;
#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.nightmode == 1 && CameraInfo.flash_en == 1)
        {
            camera_SetFlashL(CAM_FLASH_NONE);
        }
    }

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (g_mmcCameraPrevVocFreq != 0)
        {
            hal_SysSetVocClock(g_mmcCameraPrevVocFreq);
            g_mmcCameraPrevVocFreq = 0;
        }
#if defined(_CAM_SCALE_IN_VOC_)
        if (s_vocinited == TRUE)
        {
            quitPreview();
            s_vocinited = FALSE;
        }
#endif
    }

    mmc_SetCurrMode(MMC_MODE_IDLE);
    Lily_Camera_PreviewOff();
    camera_PowerOff();
    mmc_MemFreeAll();

    sensorbuf = NULL;
    blendBuffer = NULL;
#if defined(_CAM_SCALE_IN_VOC_)
    bufPointer[0] = bufPointer[1] = NULL;
#endif

#ifdef _CAM_SPECIAL_GC6113_
    gTransBufPtr[0] = gTransBufPtr[1] = NULL;
    gTransBufIdx = 0;
    gTransBufSize = 0;
#endif
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_Preview(CAM_PREVIEW_STRUCT* data)
{
#if (0)
MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_Preview");
#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        atvif_PreviewSettingInit();
#ifdef _ATV_SPLIT_
        s_atvBlendCtrl = -1;
#endif		
        countdown = 0;
    }	
    else
#endif
        countdown = 2;

#if defined(__CAMERA_FULL_OSD__)
#ifdef __MMI_ANALOGTV__ 
    if (atvif_GetCurModule() != 1)
#endif
    {
        data->start_x = data->start_y = 0;
        data->end_x = MMC_LcdWidth - 1;
        data->end_y = MMC_LcdHeight - 1;
    }
#endif

    CameraInfo.prev_win.width = data->end_x - data->start_x + 1;
    CameraInfo.prev_win.height = data->end_y - data->start_y + 1;

    if (isVideoRecMode
#ifdef __MMI_ANALOGTV__ 
            || (atvif_GetCurModule() == 1)
#endif
    )
    {
        CameraInfo.prev_win.start_x = data->start_x;
        CameraInfo.prev_win.start_y = data->start_y;
        CameraInfo.prev_win.end_x = data->end_x;
        CameraInfo.prev_win.end_y = data->end_y;
    }
    else
    {
        CameraInfo.prev_win.start_x = (MMC_LcdWidth -CameraInfo.prev_win.width) >> 1;
        CameraInfo.prev_win.start_y = (MMC_LcdHeight - CameraInfo.prev_win.height) >> 1;            
        CameraInfo.prev_win.end_x = CameraInfo.prev_win.start_x + CameraInfo.prev_win.width - 1;
        CameraInfo.prev_win.end_y = CameraInfo.prev_win.start_y + CameraInfo.prev_win.height - 1;
    }

    CameraInfo.image_height = data->image_height;
    CameraInfo.image_width = data->image_width;
    CameraInfo.nightmode = data->nightmode;
    CameraInfo.preview_rotate = data->preview_rotate;
    CameraInfo.flash_en = data->flashenable;

#if defined(__CAMERA_FULL_OSD__)
#ifdef __MMI_ANALOGTV__
    if (atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.preview_rotate == 0)
            if ((CameraInfo.image_width > CameraInfo.image_height) != (MMC_LcdWidth > MMC_LcdHeight))
            {
                if (MMC_LcdWidth > MMC_LcdHeight) 
                    CameraInfo.img_rotate = ROTATE_90;
                else 
                    CameraInfo.img_rotate = ROTATE_270;
            }
            else
                CameraInfo.img_rotate = ROTATE_0;
    }
#endif

    // Initialize Frame Buffer Window
    // Buffer pointer will be set at the time of display
    // Other parameters are constant
    displayFbw.fb.buffer = NULL;
    displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
    displayFbw.roi.x = displayFbw.roi.y = 0;
#ifdef DUAL_LCD
    if(IsSubLcdPreview())
    {
#if (CHIP_HAS_GOUDA == 1)
        lcd_fb_close_overlay();
#endif
        displayFbw.fb.width = CameraInfo.image_width;
        displayFbw.fb.height = CameraInfo.image_height;
        displayFbw.roi.width = CameraInfo.image_width;
        displayFbw.roi.height = CameraInfo.image_height;
    }
    else
#endif        
    {
        displayFbw.fb.width = MMC_LcdWidth;
        displayFbw.fb.height = MMC_LcdHeight;
        displayFbw.roi.width = MMC_LcdWidth;
        displayFbw.roi.height = MMC_LcdHeight;
    }

    cutwidth = CameraInfo.prev_width;
    cutheight = cutwidth * CameraInfo.prev_win.height/ CameraInfo.prev_win.width;
    if(cutheight > CameraInfo.prev_height)
    {
        cutheight = CameraInfo.prev_height;
        cutwidth = cutheight * CameraInfo.prev_win.width/CameraInfo.prev_win.height;
    }
    cutwidth_orig = cutwidth;
    cutheight_orig = cutheight;
    cutheight = (cutheight_orig*10/(10+2*(CameraInfo.previewZoom)))&(~7);
    cutwidth	= (cutwidth_orig*10/(10+2*(CameraInfo.previewZoom)))&(~7);   
    
    startPos = ((CameraInfo.prev_height - cutheight)>>1)*CameraInfo.prev_width + ((CameraInfo.prev_width - cutwidth)>>1);

    previewstop = 0;
    previewstatus = 1;
    sensorready = 0;

    if (CameraInfo.preview_rotate == 1)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview_SetDirRotation ");
        displayFbw.fb.width =  MMC_LcdHeight;
        displayFbw.fb.height = MMC_LcdWidth;
        displayFbw.roi.width = MMC_LcdHeight;
        displayFbw.roi.height = MMC_LcdWidth;
        displayFbw.roi.x = 0;
        displayFbw.roi.y = 0;
        lcdd_SetDirRotation();
    }

    if(bset == 0)
    {
        Lily_Camera_SetEffect(CAM_PARAM_ADD_FRAME, data->addFrame);
        Lily_Camera_SetEffect(CAM_PARAM_QUALITY, data->imageQuality);
        Lily_Camera_SetEffect(CAM_PARAM_ZOOM_FACTOR, data->factor);

        cam_wbValue = data->whiteBlance;
        COS_SetTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID, COS_TIMER_MODE_SINGLE, 200);

        Lily_Camera_SetEffect(CAM_PARAM_EFFECT, data->specialEffect);
        Lily_Camera_SetEffect(CAM_PARAM_NIGHT_MODE, data->nightmode);
        Lily_Camera_SetEffect(CAM_PARAM_CONTRAST, data->contrast);
        Lily_Camera_SetEffect(CAM_PARAM_BRIGHTNESS, data->brightNess);
        bset = 1;
    }

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview and before camera_PrepareImageCaptureL");

    mci_LcdStartBypass(lcd_BlockWriteBypass);

#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
    {
        if (CameraInfo.nightmode == 1 && CameraInfo.flash_en == 1)
        {
            camera_SetFlashL(CAM_FLASH_FORCED);
        }
    }

#if defined(_CAM_SCALE_IN_VOC_)
    turn = 0;    
#endif


#ifdef _CAM_SPECIAL_GC6113_
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, ((CameraInfo.prev_width*CameraInfo.prev_height)<<2), sensorbuf);
    gTransBufIdx = 0;
#else        
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_width*CameraInfo.prev_height), sensorbuf);
#endif

#ifdef __MMI_ANALOGTV__ 
    if(atvif_GetCurModule() == 1)
    {
        camera_CaptureImage();
    }
#endif
#endif /* 0 */
    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Camera_PreviewOff(void)
{
#if (0)
    int res;
MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA]Lily_Camera_PreviewOff");

    if (CameraInfo.preview_rotate == 1)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Preview_SetDirDefault ");
        lcdd_SetDirDefault();
    }	

#ifdef __MMI_ANALOGTV__
	if(atvif_GetCurModule())
	{
		atvif_PreviewStop();
	}
	else
#endif
    {
        if (CameraInfo.nightmode == 1 && CameraInfo.flash_en== 1)
        {
            camera_SetFlashL(CAM_FLASH_NONE);
        }
    }

    previewstop = 1;
    previewstatus = 0;

    res = COS_KillTimerEX(GetMCITaskHandle(MBOX_ID_MMC), MMC_CAMERA_TIMER_ID);
    MCI_TRACE(MCI_VIDEO_TRC, 0, "[MMC_CAMERA]KILL TIMER res=%d", res);

    mci_LcdStopBypass();

    while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();
#endif /* 0 */
    return MDI_RES_CAMERA_SUCCEED;
}

#ifdef _CAM_SPECIAL_GC6113_
void Cam_CallbackFun_gc6113(UINT8 id, UINT8 bufIdx)
{
    switch (id)
    {
        case 0: // ok for previewing
            mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CALLBACK);
            if (gTransBufIdx == 0)
                camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_size<<2), blendBuffer);
            else
                camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_size<<2), sensorbuf);
            gTransBufIdx = (gTransBufIdx + 1) & 1;
            break;
        case 1: // incomplete frame for previewing
            hal_HstSendEvent(SYS_EVENT,0xe0);
            if (!previewstop)
            {
                if (gTransBufIdx == 0)
                    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_size<<2), blendBuffer);
                else
                    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_size<<2), sensorbuf);
                gTransBufIdx = (gTransBufIdx + 1) & 1;
            }
            break;
        case 2: // partful data of a frame for capturing
            //mmc_SendMsg(GetMCITaskHandle(MBOX_ID_MMC), 
            mmc_SendMsg(BAL_TH(COS_BK_TASK_PRI),
                            MSG_CAM_PART_DATA, 
                            (UINT32)bufIdx);
            break;
        case 3: // final indicator of a frame for capturing
            //mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), 
            mmc_SendMsg(BAL_TH(COS_BK_TASK_PRI),
                            MSG_CAM_FULL_DATA, 
                            0);
            break;
    }
}
#endif

void mmc_CamBlendDsp()
{
	CAM_CAPTURE_STRUCT data;
	
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]mmc_CamBlendDsp tick: %d",hal_TimGetUpTime());

	data.image_width = CameraInfo.image_width ;
	data.image_height = CameraInfo.image_height ;
	
	Lily_Camera_Capture(&data);
}

int Lily_Camera_Capture(CAM_CAPTURE_STRUCT *data) 
{
#define CAM_CAPTURE_TIMEOUT	(150)

    UINT32 timeout = CAM_CAPTURE_TIMEOUT;
MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Capture time:%d",hal_TimGetUpTime());
    MCI_TRACE(MCI_CAMERA_TRC, 0, 
            "[MMC_CAMERA]Lily_Camera_Capture data->image_width:%d, data->image_height:%d",
            data->image_width,data->image_height);

    CameraInfo.image_width = data->image_width;
    CameraInfo.image_height = data->image_height; 

    previewstop = 1;	

    if (CameraInfo.flash_en == 1 && CameraInfo.nightmode == 0)
        camera_SetFlashL(CAM_FLASH_FORCED);

    countdown = AVERAGE_FRAME_NUM;
    sensorready = 0;
#if 1//defined(_CAM_SCALE_IN_VOC_)
   // turn = 0;
    camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_width*CameraInfo.prev_height), sensorbuf);  
#endif
    camera_CaptureImage();

    /// wait not more than 15 secs
    while((sensorready == 0) && (--timeout != 0))
    {
        COS_SleepByTick(1600);
    }

    if (sensorready == 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]failed to capture a frame");
        return MDI_RES_CAMERA_ERR_FAILED;
    }

    if (CameraInfo.flash_en == 1 && CameraInfo.nightmode == 0)
        camera_SetFlashL(CAM_FLASH_NONE);

    if (multicapture)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]cont save photo");
        Lily_Capture_Preview();
    }

    return MDI_RES_CAMERA_SUCCEED;
}

int Lily_Capture_Preview()
{
    typedef LCDD_ERR_T (*DISP_FUNC)(CONST LCDD_FBW_T* frameBufferWin, UINT16 startX, UINT16 startY);    

    DISP_FUNC pFunc_Lcd_Disp = NULL;
    HAL_GOUDA_VID_LAYER_DEF_T gouda_vid_def = {0, };  
    LCDD_ERR_T err = LCDD_ERR_NO;

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview,multicaputure=%d srcwidth=%d", multicapture, CameraInfo.sensor_width);

#ifdef DUAL_LCD
    if(IsSubLcdPreview())
        pFunc_Lcd_Disp = lcdd_SubBlit16;            
    else
#endif
        pFunc_Lcd_Disp = lcdd_Blit16;

    previewstatus = 0;
    if (CameraInfo.sensor_size > CAM_NPIX_SXGA)
    {
        
        INT32 lcd_disp_w = -1;
        INT32 lcd_disp_h = -1;
        UINT16 gouda_disp_w = CameraInfo.prev_win.width >> 1;
        UINT16 gouda_disp_h = CameraInfo.prev_win.height >> 1;

#define CAM_PART_DISP   \
    {	                    \
        do {            \
            COS_SleepByTick(16);                      \
        } while(hal_GoudaIsActive());   \
        hal_GoudaVidLayerClose();       \
        hal_GoudaVidLayerOpen(&gouda_vid_def);      \
        err = pFunc_Lcd_Disp(&displayFbw, displayFbw.roi.x ,displayFbw.roi.y);     \
        while (LCDD_ERR_NO != err)              \
        {                                                   \
            COS_SleepByTick(10);                           \
            err = pFunc_Lcd_Disp(&displayFbw,displayFbw.roi.x ,displayFbw.roi.y);  \
        };                          \
    }                                   \


#ifdef DUAL_LCD
        if(IsSubLcdPreview())
        {
            lcd_disp_w = MMC_SubLcdWidth >> 1;
            lcd_disp_h = MMC_SubLcdHeight >> 1;
        }
        else
#endif
        {
            lcd_disp_w = MMC_LcdWidth >> 1;
            lcd_disp_h = MMC_LcdHeight >> 1;
        }

        cutwidth = CameraInfo.sensor_width;
        cutheight = cutwidth * CameraInfo.image_height/ CameraInfo.image_width;
        if(cutheight > CameraInfo.sensor_height)
        {
            cutheight = CameraInfo.sensor_height;
            cutwidth = cutheight * CameraInfo.image_width/CameraInfo.image_height;
        }
        
        cutheight = cutheight*10/(10+2*(CameraInfo.previewZoom))&(~1);
        cutwidth = cutwidth*10/(10+2*(CameraInfo.previewZoom))&(~1);   

        if (cutwidth > 5*CameraInfo.image_width)
        {
            cutwidth = 5*CameraInfo.image_width;
            cutheight = 5*CameraInfo.image_height;
        }

        startPos = ((CameraInfo.sensor_height-cutheight)>>1)*CameraInfo.sensor_width + ((CameraInfo.sensor_width-cutwidth)>>1); 

        gouda_vid_def.alpha = 0xFF;
        gouda_vid_def.cKeyColor = 0;
        gouda_vid_def.cKeyEn = FALSE;
        gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;
        gouda_vid_def.stride = CameraInfo.sensor_width<<1;
    #if defined(__CAMERA_FULL_OSD__)
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;
    #else
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
    #endif
        gouda_vid_def.height = cutheight>>1;
        gouda_vid_def.width = cutwidth>>1;

        displayFbw.fb.buffer = (uint16*)NULL;
        displayFbw.roi.width = lcd_disp_w;
        displayFbw.roi.height = lcd_disp_h;

        // part 1
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview part1, tick: %d", hal_TimGetUpTime());
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
        gouda_vid_def.pos.tlPX = CameraInfo.prev_win.start_x;
        gouda_vid_def.pos.brPX = CameraInfo.prev_win.start_x + gouda_disp_w - 1;
        gouda_vid_def.pos.tlPY = CameraInfo.prev_win.start_y;
        gouda_vid_def.pos.brPY =CameraInfo.prev_win.start_y + gouda_disp_h - 1;
        displayFbw.roi.x = 0;
        displayFbw.roi.y = 0;
    MCI_TRACE(MCI_CAMERA_TRC, 0, "start_x %d, start_y %d", gouda_vid_def.pos.tlPX, gouda_vid_def.pos.tlPY);    

        CAM_PART_DISP

        // part2
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview part2, tick: %d",hal_TimGetUpTime());        
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos+(cutwidth>>1));
        gouda_vid_def.pos.tlPX = CameraInfo.prev_win.start_x + gouda_disp_w;
        gouda_vid_def.pos.brPX = CameraInfo.prev_win.end_x;
        gouda_vid_def.pos.tlPY = CameraInfo.prev_win.start_y;
        gouda_vid_def.pos.brPY =CameraInfo.prev_win.start_y + gouda_disp_h - 1;
        displayFbw.roi.x = lcd_disp_w;
        displayFbw.roi.y = 0;
    MCI_TRACE(MCI_CAMERA_TRC, 0, "start_x %d, start_y %d", gouda_vid_def.pos.tlPX, gouda_vid_def.pos.tlPY);    
        
        CAM_PART_DISP


        // part3
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview part3, tick: %d",hal_TimGetUpTime());    
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos+(cutheight>>1)*CameraInfo.sensor_width);
        gouda_vid_def.pos.tlPX = CameraInfo.prev_win.start_x;
        gouda_vid_def.pos.brPX = CameraInfo.prev_win.start_x +gouda_disp_w - 1;
        gouda_vid_def.pos.tlPY = CameraInfo.prev_win.start_y + gouda_disp_h;
        gouda_vid_def.pos.brPY =CameraInfo.prev_win.end_y;
        displayFbw.roi.x = 0;
        displayFbw.roi.y = lcd_disp_h;
    MCI_TRACE(MCI_CAMERA_TRC, 0, "start_x %d, start_y %d", gouda_vid_def.pos.tlPX, gouda_vid_def.pos.tlPY);    

        CAM_PART_DISP

        // part4
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview part4, tick: %d",hal_TimGetUpTime());    
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos+(cutheight>>1)*CameraInfo.sensor_width + (cutwidth>>1));
        gouda_vid_def.pos.tlPX = CameraInfo.prev_win.start_x + gouda_disp_w;
        gouda_vid_def.pos.brPX = CameraInfo.prev_win.end_x;
        gouda_vid_def.pos.tlPY = CameraInfo.prev_win.start_y + gouda_disp_h;
        gouda_vid_def.pos.brPY =CameraInfo.prev_win.end_y;    
        displayFbw.roi.x = lcd_disp_w;
        displayFbw.roi.y = lcd_disp_h;
    MCI_TRACE(MCI_CAMERA_TRC, 0, "start_x %d, start_y %d", gouda_vid_def.pos.tlPX, gouda_vid_def.pos.tlPY);    

        CAM_PART_DISP

MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Capture_Preview end, tick: %d",hal_TimGetUpTime());    
    }
    else
    {
#if defined(_CAM_SCALE_IN_VOC_)

        CutZoomInStruct   cutzoom = {0, };      
        UINT16 tmpw = CameraInfo.prev_win.width;
        UINT16 tmph = CameraInfo.prev_win.height;

        cutzoom.inStreamBufAddr = (INT32*)sensorbuf;
        cutzoom.outStreamBufAddr = (INT32*)blendBuffer;
        cutzoom.blendBufAddr = NULL;
        cutzoom.src_w = CameraInfo.sensor_width;
        cutzoom.src_h = CameraInfo.sensor_height;
        cutzoom.cut_w = cutwidth;
        cutzoom.cut_h = cutheight;
        cutzoom.dec_w = tmpw;
        cutzoom.dec_h = tmph;
        cutzoom.reset = 1;

        MMC_Preview_Wait(&cutzoom);        

        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;
        gouda_vid_def.addrY = (UINT32*)blendBuffer;
        gouda_vid_def.width = tmpw;
        gouda_vid_def.height = tmph;
        gouda_vid_def.stride = tmpw<<1;

#elif defined(_CAM_SPECIAL_GC6113_)

        UINT16 srcw = 240;
        UINT16 srch = 320;
        UINT16 prew = CameraInfo.image_width;
        UINT16 preh = CameraInfo.image_height;

        cutwidth = srcw;
        cutheight = srcw * preh/ prew;
        if(cutheight > srch)
        {
            cutheight = srch;
            cutwidth = srch * prew/preh;
        }
        cutheight = cutheight*10/(10+2*(CameraInfo.previewZoom))&(~1);
        cutwidth = cutwidth*10/(10+2*(CameraInfo.previewZoom))&(~1); 

        startPos = ((srch - cutheight)/2)*srcw + (srcw - cutwidth)/2;
        gouda_vid_def.stride = srcw*2;
#else
        gouda_vid_def.stride = (CameraInfo.sensor_width<<1);
#endif
#ifdef __CAMERA_FULL_OSD__
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_RGB565;
#else
        gouda_vid_def.fmt = HAL_GOUDA_IMG_FORMAT_YUYV;
#endif
        gouda_vid_def.addrY = (UINT32*)(sensorbuf+startPos);
        gouda_vid_def.width =  cutwidth;    
        gouda_vid_def.height = cutheight;

        gouda_vid_def.alpha = 0xFF;
        gouda_vid_def.cKeyColor = 0;
        gouda_vid_def.cKeyEn = FALSE;
        gouda_vid_def.cKeyMask = HAL_GOUDA_CKEY_MASK_OFF;
        gouda_vid_def.depth = HAL_GOUDA_VID_LAYER_BETWEEN_2_1;

        gouda_vid_def.pos.tlPX = CameraInfo.prev_win.start_x;
        gouda_vid_def.pos.tlPY = CameraInfo.prev_win.start_y;
        gouda_vid_def.pos.brPX  = CameraInfo.prev_win.end_x;
        gouda_vid_def.pos.brPY  = CameraInfo.prev_win.end_y;

        while(hal_GoudaIsActive());
        hal_GoudaVidLayerClose();
#if defined(__PRJ_WITH_SPILCD__)&& !defined(__PRJ_WITH_SPICAM__)
        extern U16 simulator_Merge_buffer[];	
        displayFbw.fb.buffer = simulator_Merge_buffer;
#else
        displayFbw.fb.buffer = (uint16*)NULL;
#endif
        hal_GoudaVidLayerOpen(&gouda_vid_def);

        err = pFunc_Lcd_Disp(&displayFbw, 0 ,0);
        while (LCDD_ERR_NO != err)
        {
            COS_SleepByTick(10);
            err = pFunc_Lcd_Disp(&displayFbw,0 ,0);
        };        
    }

    do {
        COS_SleepByTick(16);
    } while(hal_GoudaIsActive());
    hal_GoudaVidLayerClose();

    return MDI_RES_CAMERA_SUCCEED;
}

void camera_cut_image( UINT16* inbuf, UINT16* outbuf, UINT16 srcw, UINT16 srch, UINT16 cutw, UINT16 cuth)
{
    UINT32 start_pos = ((((srch-cuth)>>1)*srcw + ((srcw-cutw)>>1))&(~0x1));
    UINT16 *psrc = NULL;
    UINT16 *pdst = outbuf;
    UINT16 i = 0, j = 0;
    const UINT8 step4 = cutw&3;
    const UINT8 step8 = cutw&7;

    MCI_TRACE(MCI_CAMERA_TRC, 0, 
                "image_cut, srcw %d srch %d cutw %d cuth %d startPos %d",
                srcw, srch, cutw, cuth, start_pos);

    if (!inbuf || !outbuf) return;
    if (start_pos == 0 && inbuf == outbuf) return;
    inbuf = (UINT16*)(((UINT32)inbuf)&(~0x20000000));
    hal_SysInvalidateCache(inbuf, (srcw*srch)<<1);

    for (j = 0; j < cuth; j++)
    {
        psrc = inbuf + start_pos + srcw*j;
        i = 0;
        do{
            *pdst++ = psrc[i++];
            *pdst++ = psrc[i++];
            if (!step4)
            {
                *pdst++ = psrc[i++];
                *pdst++ = psrc[i++];
            }
            if (!step8)
            {
                *pdst++ = psrc[i++];
                *pdst++ = psrc[i++];
                *pdst++ = psrc[i++];
                *pdst++ = psrc[i++];
            }
        } while(i < cutw);
    }
}

int Lily_camera_savePhoto(int  file_handle)
{
    INT32 len = 0;
    UINT16 width = cutwidth;
    UINT16 height = cutheight;
    UINT16* inbuf = sensorbuf;

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]Lily_camera_savePhoto, file hdl %d", file_handle);
    if (file_handle < 0)
    {
        MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]FILE HANDLE IS INVALIBLE!");
        return MDI_RES_CAMERA_ERR_FAILED;
    }

    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA] start to encode a JPEG image");
    camera_cut_image(sensorbuf, sensorbuf, CameraInfo.sensor_width, CameraInfo.sensor_height, cutwidth, cutheight);

#ifdef __CAMERA_FULL_OSD__
    if (CameraInfo.img_rotate != 0)
    {
        rotate_table_init(cutwidth, cutheight);
        image_rotate(sensorbuf, CameraInfo.img_rotate, cutwidth, cutheight);
        rotate_table_deinit();
        if (CameraInfo.img_rotate&1)
        {
            width = cutheight;
            height = cutwidth;
        }
    }
#endif

    if (((cutwidth*cutheight)<<1) < (CameraInfo.sensor_size))
    {
        inbuf = sensorbuf + (CameraInfo.sensor_size >>1);
        memcpy((inbuf), (sensorbuf), ((cutwidth*cutheight)<<1));
    }
    initVocJpegEncode();

    MMC_jpgEn(width, height, CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, (char*)inbuf, (char*)blendBuffer, 1);

    len = FS_Write(file_handle, (uint8*)blendBuffer + 1, getJpegEncodeLen());
    MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]SAVE PHOTO LEN=%d, actual len=%d", getJpegEncodeLen(),len);

    quitVocJpegEncode();
    if (len <= 0)
        return MDI_RES_CAMERA_ERR_DISK_FULL;

    return MDI_RES_CAMERA_SUCCEED;
}
typedef enum{
    CAM_STATE_IDLE,
    CAM_STATE_GET_SENSOR_DATA,
    CAM_STATE_SENSOR_DATA_READY,
    CAM_STATE_ENCODE_DATA,
    CAM_STATE_ENCODE_DONE,
};
static UINT32 cam_state = CAM_STATE_IDLE;
static UINT8 cam_media_mode = CAPTURE_MODE_CAMERA;

#if defined(IP_CAMERA_SUPPORT)


extern void IPCameraCostTimeStart(UINT8 idx);
extern void IPCameraCostTimeEnd(UINT8 idx);
 void IPCameraCostTimePrintf(void); 

typedef struct
{
    UINT32 count;
    UINT32 total;
    UINT32 avg;
    UINT32 last_tick;
} IpcameCostTimeStruct;

IpcameCostTimeStruct IpcameCostTime[10] = {0};

void IPCameraCostTimePrintf(void)
{
#if !defined(WIN32)
    UINT32 scStatus;
    UINT8 i;
    scStatus = COS_EnterCriticalSection();

    MCI_TRACE(MCI_CAMERA_TRC,0,"IPcam cost,%d,%d - %d,%d,%d - %d",
              IpcameCostTime[1].avg,IpcameCostTime[2].avg,
              IpcameCostTime[4].avg,IpcameCostTime[5].avg,IpcameCostTime[6].avg,
              IpcameCostTime[7].avg);

    for(i = 0; i < sizeof(IpcameCostTime) /sizeof(IpcameCostTime[0]); i++)
    {
        IpcameCostTime[i].count = 0;
        IpcameCostTime[i].total = 0;
        IpcameCostTime[i].avg = 0;
    }

    COS_ExitCriticalSection(scStatus);
#endif /* WIN32 */
}



void IPCameraCostTimeStart(UINT8 idx)
{
#if !defined(WIN32)
    UINT32 scStatus;
    scStatus = COS_EnterCriticalSection();

    IpcameCostTime[idx].last_tick = hal_TimGetUpTime();
    COS_ExitCriticalSection(scStatus);

#endif /* WIN32 */
}

void IPCameraCostTimeEnd(UINT8 idx)
{
#if !defined(WIN32)
    UINT32 scStatus;
    scStatus = COS_EnterCriticalSection();

    if(0 != IpcameCostTime[idx].last_tick)
    {
        IpcameCostTime[idx].total += hal_TimGetUpTime() - IpcameCostTime[idx].last_tick;
        IpcameCostTime[idx].last_tick = 0;
        IpcameCostTime[idx].count ++;
        IpcameCostTime[idx].avg = IpcameCostTime[idx].total / IpcameCostTime[idx].count;
    }
    COS_ExitCriticalSection(scStatus);
#endif /* WIN32 */
}

#if defined(__ZBAR_SUPPORT__)
#define QRCODE_MAX_LEN  500

extern int zbar_decode_qrcode(int width, int height, void *raw, char *out_buffer, unsigned int out_len);
void camera_to_grey(UINT16* inbuf, UINT8* outbuf, UINT16 w, UINT16 h)
{
    UINT16 *psrc = NULL;
    UINT8 *pdst = outbuf;
    UINT16 i = 0, j = 0;

    MCI_TRACE(MCI_CAMERA_TRC, 0, "camera_to_grey, w %d h %d ",
                w,h);

    if (!inbuf || !outbuf) return;
    inbuf = (UINT16*)(((UINT32)inbuf)&(~0x20000000));
    hal_SysInvalidateCache(inbuf, (w*h)<<1);

    for(i = 0; i < h ; i++)
    {
        psrc = inbuf + w * i;

        for(j = 0; j < w; j++)
        {
            // U0Y0V0Y1
            *pdst = (UINT8) (*psrc & 0xFF);
            
            psrc++;
            pdst++;
        }
    }
}


static UINT32 Lily_cameraDecodeQrcode(void)
{
    if(CAM_STATE_ENCODE_DATA != cam_state)
    {
        COS_EVENT ev;
        UINT32 cost_time;
        UINT8 *qrcode_outbuffer = (UINT8 *) blendBuffer;
        UINT16 qrcode_outbuffer_len = 0;

        cam_state = CAM_STATE_ENCODE_DATA;

        cost_time = hal_TimGetUpTime();        

        camera_cut_image(sensorbuf, sensorbuf, CameraInfo.sensor_width, CameraInfo.sensor_height, cutwidth, cutheight);
        camera_to_grey(sensorbuf,sensorbuf,cutwidth, cutheight);

        hal_TimWatchDogPause(1);
        qrcode_outbuffer_len = zbar_decode_qrcode(cutwidth,cutheight,sensorbuf, qrcode_outbuffer, QRCODE_MAX_LEN);
        hal_TimWatchDogPause(0);

        if(qrcode_outbuffer_len > 0)
        {
            UINT8 *buffer;

            buffer = (UINT8 *) COS_MALLOC(qrcode_outbuffer_len);
            if(buffer)
            {
                memcpy(buffer, qrcode_outbuffer, qrcode_outbuffer_len);
                
                ev.nEventId = EV_CAMERA_GET_QRCODE_DATA_IND;
                ev.nParam1 = (UINT32) buffer;
                ev.nParam2 = qrcode_outbuffer_len;
                COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }
        }

        MCI_TRACE(MCI_CAMERA_TRC, 0, "Lily_cameraDecodeQrcode time:%d, len=%d",
                    hal_TimGetUpTime() - cost_time,
                    qrcode_outbuffer_len);

        cam_state = CAM_STATE_IDLE;
        Lily_Camera_CaptureFrame();
    }
    return 0;
}

#endif /* __ZBAR_SUPPORT__ */

void Cam_CallbackFun(UINT8 id,  UINT8 ActiveBufNum)
{
	MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Cam_CallbackFun id:%d", id);
    
    camera_CancelCaptureImage();
    if(id != 0)
    	return;
    if((CameraInfo.flash_en == 1 && CameraInfo.nightmode == 0)
#if defined(__ZBAR_SUPPORT__)
        ||(CAPTURE_MODE_QRCODE == cam_media_mode)
#endif /* __ZBAR_SUPPORT__ */
    )
    {
        camera_SetFlashL(CAM_FLASH_NONE);
    }

    IPCameraCostTimeEnd(1);

	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CALLBACK);
}

int Lily_Camera_CaptureFrame(void) 
{
    if(CAM_STATE_GET_SENSOR_DATA != cam_state)
    {
        cam_state = CAM_STATE_GET_SENSOR_DATA;
        
        MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Lily_Camera_Capture time:%d",hal_TimGetUpTime());

        IPCameraCostTimeStart(1);
        if((CameraInfo.flash_en == 1 && CameraInfo.nightmode == 0)
#if defined(__ZBAR_SUPPORT__)
            ||(CAPTURE_MODE_QRCODE == cam_media_mode)
#endif /* __ZBAR_SUPPORT__ */
        )
        {
                camera_SetFlashL(CAM_FLASH_FORCED);
        }

        camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_width*CameraInfo.prev_height), sensorbuf);
        //camera_CaptureImage();
    }
    return MDI_RES_CAMERA_SUCCEED;
}

UINT32 Lily_cameraEncodeFrame(void)
{
    UINT32 len = 0;
    UINT16 width = cutwidth;
    UINT16 height = cutheight;
    UINT16* inbuf = sensorbuf;

    if(CAM_STATE_ENCODE_DATA != cam_state)
    {
        cam_state = CAM_STATE_ENCODE_DATA;

        IPCameraCostTimeStart(2);
        MCI_TRACE(MCI_CAMERA_TRC, 0, "Lily_cameraEncodeFrame time:%d",hal_TimGetUpTime());

        camera_cut_image(sensorbuf, sensorbuf, CameraInfo.sensor_width, CameraInfo.sensor_height, cutwidth, cutheight);

#ifdef __CAMERA_FULL_OSD__
        if (CameraInfo.img_rotate != 0)
        {
            rotate_table_init(cutwidth, cutheight);
            image_rotate(sensorbuf, CameraInfo.img_rotate, cutwidth, cutheight);
            rotate_table_deinit();
            if (CameraInfo.img_rotate&1)
            {
                width = cutheight;
                height = cutwidth;
            }
        }
#endif

        if (((cutwidth*cutheight)<<1) < (CameraInfo.sensor_size))
        {
            inbuf = sensorbuf + (CameraInfo.sensor_size >>1);
            memcpy((inbuf), (sensorbuf), ((cutwidth*cutheight)<<1));
        }

        MMC_jpgEn(width, height, CameraInfo.image_width, CameraInfo.image_height, CameraInfo.image_quality, (char*)inbuf, (char*)blendBuffer, 1);
    }
    return len;
}

UINT32 Lily_cameraGetEncodeData(UINT8 *filebuffer, UINT32 buffersize)
{
    UINT32 wait_time;
    UINT32 DataLen;
    
    if (!filebuffer)
    {
        MCI_TRACE(MCI_CAMERA_TRC, 0, "filebuffer IS INVALIBLE!");
        return 0;
    }

    hal_HstSendEvent(SYS_EVENT, 0xaaaaaaaa);
    //Wait encode data ready
    wait_time = hal_TimGetUpTime();
    while(CAM_STATE_ENCODE_DONE != cam_state) COS_Sleep(10);
    wait_time = hal_TimGetUpTime() - wait_time;
    MCI_TRACE(MCI_CAMERA_TRC, 0, "Lily_cameraGetEncodeData wait_time:%d",wait_time);


    //Copy data to filebuffer
    DataLen = getJpegEncodeLen();
    MCI_TRACE(MCI_CAMERA_TRC, 0, "Lily_cameraGetEncodeData,ptr=%x,size=%d,DataLen=%d",
                    filebuffer,buffersize,DataLen);
	if(DataLen <= buffersize)
	{
		memcpy(filebuffer, (uint8*)blendBuffer + 1, DataLen);		
    	cam_state = CAM_STATE_IDLE;
        hal_HstSendEvent(SYS_EVENT, 0xbbbbbbbb);
        hal_HstSendEvent(SYS_EVENT, wait_time);
		//mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_CAM_CAPTURE_FRAME);
		        	hal_HstSendEvent(SYS_EVENT, 0x2015A000);
            Lily_Camera_CaptureFrame();

        return DataLen;
	}
	else
	{
	    return 0;
	}
}


void Lily_cameraStartCapture(CAM_CAPTURE_STRUCT *data) 
{
    MCI_TRACE(MCI_CAMERA_TRC, 0, 
            "Lily_cameraPrepareCapture data->image_width:%d, data->image_height:%d",
            data->image_width,data->image_height);

    CameraInfo.image_width = data->image_width;
    CameraInfo.image_height = data->image_height; 

    cutwidth = data->image_width;
    cutheight = data->image_height; 
    cam_media_mode = data->media_mode;
    
#if defined(__ZBAR_SUPPORT__)
    if(CAPTURE_MODE_QRCODE == cam_media_mode)
    {
    }
    else
#endif /* __ZBAR_SUPPORT__ */
    if(CAPTURE_MODE_CAMERA == cam_media_mode)
    {
        initVocJpegEncode();
    }

    //camera_PrepareImageCaptureL(CAM_FORMAT_RGB565, (CameraInfo.prev_width*CameraInfo.prev_height), sensorbuf);
    Lily_Camera_CaptureFrame();
}

void Lily_cameraStopCapture(void)
{
    if(CAPTURE_MODE_CAMERA == cam_media_mode)
        quitVocJpegEncode();

    camera_SetFlashL(CAM_FLASH_NONE);
    cam_state = CAM_STATE_IDLE;
}
#endif /* IP_CAMERA_SUPPORT */

#ifdef _CAM_SPECIAL_GC6113_
void mmc_camResize(UINT8* in, UINT32 inSize, UINT8* out)
{
    UINT32  i;
    UINT32  data1, data2;
    UINT16 res;
    UINT32  count = inSize>>3;
    UINT32* ptr_in  = (UINT32*)in;
    UINT16* ptr_out = (UINT16*)out;

    for(i= count; i>0; i--)
    {
        data1 = *(ptr_in++);
        data2 = *(ptr_in++);
        res  = ((data1<<30)>>16) | (((data1>>8)<<30)>>18) | (((data1>>16)<<30)>>20) | (((data1>>24)<<30)>>22)
                | ((data2<<30)>>24) | (((data2>>8)<<30)>>26) | (((data2>>16)<<30)>>28) | (((data2>>24)<<30)>>30); 
        *(ptr_out++) = res;
    }
}
#endif


void Camera_UserMsg(COS_EVENT *pnMsg)
{   
	MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]Camera_UserMsg pnMsg->nEventId:%d ",pnMsg->nEventId);

    switch (pnMsg->nEventId)
    {
        case MSG_CAM_CAPTURE_FRAME:
        	hal_HstSendEvent(SYS_EVENT, 0x2015A000);
            #if defined(IP_CAMERA_SUPPORT)
            Lily_Camera_CaptureFrame();
            #endif 
            break;

        case MSG_CAM_CALLBACK:
            hal_HstSendEvent(SYS_EVENT, 0x2015B000);
            cam_state = CAM_STATE_SENSOR_DATA_READY;
#if defined(__ZBAR_SUPPORT__)
            if(CAPTURE_MODE_QRCODE == cam_media_mode)
            {
                Lily_cameraDecodeQrcode();
                break;
            }
            else
#endif /* __ZBAR_SUPPORT__ */
            {
                /* NOTICE: Fall through */
            }
        case MSG_JPEGENC_START:
            hal_HstSendEvent(SYS_EVENT, 0x2015C000);
            #if defined(IP_CAMERA_SUPPORT)
            Lily_cameraEncodeFrame();
            #endif
            break;

        case MSG_JPEGENC_CALLBACK:
        	hal_HstSendEvent(SYS_EVENT, 0x2015D000);
            #if defined(IP_CAMERA_SUPPORT)
   	        IPCameraCostTimeEnd(2);
            #endif 
            cam_state = CAM_STATE_ENCODE_DONE;
            break;
            
/*
        case EV_TIMER:
            switch(pnMsg->nParam1)
            {
                case MMC_CAMERA_TIMER_ID:
                    MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg  EV_TIMER");
                    if (bset < 2)//bset==2 means the camera is power off
                        Lily_Camera_SetEffect(CAM_PARAM_WB, cam_wbValue);
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Invalid timer ID in  camera");
                    break;
            }
            break;
*/
            
        default:
            if(!mmc_JpegEncUserMsg(pnMsg))
                MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Camera_UserMsg:%d",pnMsg->nEventId);
            break;
    }
}

int Lily_Camera_SetEffect(CAM_SET_PARA effectCode,uint32 value)
{
    uint32 localEffectCode=0;
    uint32 localValue=0;
    UINT16 tmpw = 0, tmph = 0;  

#ifdef _ATV_SPLIT_
#ifdef __MMI_ANALOGTV__
    if(atvif_GetCurModule() != 1)
#endif
#endif	
        countdown = 1;

    switch(effectCode)
    {
        case CAM_PARAM_CONTRAST:
            localEffectCode = CameraSetContrast;
            localValue =5- value;
            if(localValue == 0)
            {
                localValue = 0;
            }
            //cameffect.contrast = value;
            camera_SetContrastL(localValue);
            break;

        case CAM_PARAM_BRIGHTNESS:
            localEffectCode = CameraSetBrightness;
            localValue = value+1;
            if(localValue > 5)
            {
                localValue = 5;
            }
            //cameffect.bright = value;
            camera_SetBrightnessL(localValue);
            break;

        case CAM_PARAM_IMAGESIZE:
            if (isVideoRecMode == TRUE)
                break;
            
#ifdef DUAL_LCD
            if(IsSubLcdPreview())
            {
                break;
            }
#endif
            MCI_TRACE(MCI_CAMERA_TRC, 0,"[MMC_CAMERA]Image Size Change, %d", value);
            switch(value)
            {
                case 0x01:
                    CameraInfo.image_width =  MMC_LcdWidth;
                    CameraInfo.image_height = MMC_LcdHeight;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = MMC_LcdHeight;
            #endif
                    break;

                case 0x02: //176x220
                    CameraInfo.image_width =  176;
                    CameraInfo.image_height = 220;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*5)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/5) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x03: // 320x240
                    CameraInfo.image_width =  320;
                    CameraInfo.image_height = 240;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;
                
                case 0x04: //640x480
                    CameraInfo.image_width =  640;
                    CameraInfo.image_height = 480;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x05: //800x600
                    CameraInfo.image_width =  800;
                    CameraInfo.image_height = 600;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x06: //1280x960
                    CameraInfo.image_width =  1280;
                    CameraInfo.image_height = 960;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x07: //1280x1024
                    CameraInfo.image_width =  1280;
                    CameraInfo.image_height = 1024;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth<<2)/5)+1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight*5)>>2) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x08: //1600x1200
                    CameraInfo.image_width =  1600;
                    CameraInfo.image_height = 1200;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x09: //2048x1536
                    CameraInfo.image_width =  2048;
                    CameraInfo.image_height = 1536;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x0a: //3200x1800
                    CameraInfo.image_width =  3200;
                    CameraInfo.image_height = 1800;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*9)>>4)+1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<4)/9) + 1) & (~1);
                    }
            #endif
                    break;

                case 0x0b: //3200x2400
                    CameraInfo.image_width =  3200;
                    CameraInfo.image_height = 2400;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = (((MMC_LcdWidth*3)>>2) + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight<<2)/3) + 1) & (~1);
                    }                        
            #endif
                    break;

                case 0x23: //240x320
                    CameraInfo.image_width =  240;
                    CameraInfo.image_height = 320;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = ((MMC_LcdWidth<<2)/3 + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight*3)>>2) + 1) & (~1);
                    }
            #endif
                    break;
                
                case 0x24: // 480x640
                    CameraInfo.image_width =  480;
                    CameraInfo.image_height = 640;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = ((MMC_LcdWidth<<2)/3 + 1)&(~1);
                    if (CameraInfo.prev_win.height > MMC_LcdHeight)
                    {
                        CameraInfo.prev_win.height = MMC_LcdHeight;
                        CameraInfo.prev_win.width = (((MMC_LcdHeight*3)>>2) + 1) & (~1);
                    }
            #endif
                    break;
                
                default:
                    CameraInfo.image_width =  MMC_LcdWidth;
                    CameraInfo.image_height = MMC_LcdHeight;
            #ifndef __CAMERA_FULL_OSD__
                    CameraInfo.prev_win.width = MMC_LcdWidth;
                    CameraInfo.prev_win.height = MMC_LcdHeight;
            #endif
                    break;                
            }

    #ifdef __CAMERA_FULL_OSD__
            CameraInfo.prev_win.width = MMC_LcdWidth;
            CameraInfo.prev_win.height = MMC_LcdHeight;
            if ((CameraInfo.image_width > CameraInfo.image_height) != (MMC_LcdWidth > MMC_LcdHeight))
                if (MMC_LcdWidth > MMC_LcdHeight) CameraInfo.img_rotate = ROTATE_90;
                else CameraInfo.img_rotate = ROTATE_270;
            else
                CameraInfo.img_rotate = ROTATE_0;
    #endif
            tmpw = CameraInfo.prev_win.width;
            tmph = CameraInfo.prev_win.height;

            CameraInfo.prev_win.start_x = (MMC_LcdWidth -tmpw) >> 1;
            CameraInfo.prev_win.start_y = (MMC_LcdHeight - tmph) >> 1;            
            CameraInfo.prev_win.end_x = CameraInfo.prev_win.start_x + tmpw - 1;
            CameraInfo.prev_win.end_y = CameraInfo.prev_win.start_y + tmph - 1;

            cutwidth = CameraInfo.prev_width;
            cutheight = CameraInfo.prev_width * tmph / tmpw;
            if(cutheight > CameraInfo.prev_height)
            {
                cutheight = CameraInfo.prev_height;
                cutwidth = CameraInfo.prev_height * tmpw/tmph;
            }
            cutwidth_orig = cutwidth;
            cutheight_orig = cutheight;
            
            cutheight = (cutheight_orig*10/(10+2*(CameraInfo.previewZoom))) & (~7);
            cutwidth = (cutwidth_orig*10/(10+2*(CameraInfo.previewZoom))) & (~7);  
           
            startPos = ((CameraInfo.prev_height - cutheight)>>1)*CameraInfo.prev_width + ((CameraInfo.prev_width - cutwidth)>>1);

            break;	
            
        case CAM_PARAM_ADD_FRAME:
            CameraInfo.addFrame = value;
            break;	
        /*******the followings is ought to return each case****/		
        case CAM_PARAM_ZOOM_FACTOR:
            cutwidth = (cutwidth_orig*10/(10+(value<<1))) & (~7);
            cutheight = (cutheight_orig*10/(10+(value<<1))) & (~7);
            
            CameraInfo.previewZoom = value;
            startPos = ((CameraInfo.prev_height - cutheight)>>1)*CameraInfo.prev_width + ((CameraInfo.prev_width - cutwidth)>>1);
            break;
            
        case CAM_PARAM_QUALITY:
            if(value == 0)
            {
                CameraInfo.image_quality =0;
            }
            else
            {
                CameraInfo.image_quality = 1;
            }
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_QUALITY=%d",value);
            break;
            
        case CAM_PARAM_EFFECT:
            switch(value)
            {
                case MDI_CAMERA_EFFECT_NORMAL:
                    localEffectCode = CAM_SPE_NORMAL;
                    break;
                case MDI_CAMERA_EFFECT_ANTIQUE:
                    localEffectCode = CAM_SPE_ANTIQUE;
                    break;
                case MDI_CAMERA_EFFECT_REDRISH:
                    localEffectCode = CAM_SPE_REDISH;
                    break;
                case MDI_CAMERA_EFFECT_GREENRISH:
                    localEffectCode = CAM_SPE_GREENISH;
                    break;
                case MDI_CAMERA_EFFECT_BLUERISH:
                    localEffectCode = CAM_SPE_BLUEISH;
                    break;
                case MDI_CAMERA_EFFECT_BLACKWHITE:
                    localEffectCode = CAM_SPE_BLACKWHITE;
                    break;
                case MDI_CAMERA_EFFECT_NEGATIVENESS:
                    localEffectCode = CAM_SPE_NEGATIVE;
                    break;
                default:
                    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_CAMERA_ERROR]ERROR CAM_PARAM_EFFECT");
                    localEffectCode = MDI_CAMERA_EFFECT_NORMAL;
                    break;
            }
            //cameffect.effect = value;
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]localEffectCode=%d",localEffectCode);
            camera_SetSpecialEffect(localEffectCode);
            break;
            
        case CAM_PARAM_NIGHT_MODE:
            localEffectCode = CameraSetBrightness;
            localValue = value;
            //cameffect.nightmode = value;
            if(TRUE== localValue)
            {
                camera_SetNightMode(1);//CameraIocontrol(CameraNightModeEn,NULL,NULL);
                CameraInfo.nightmode = 1;
                if (CameraInfo.flash_en == 1)
                    camera_SetFlashL(CAM_FLASH_FORCED);
            }
            else
            {
                camera_SetNightMode(0);//CameraIocontrol(CameraNightModeDis,NULL,NULL);
                CameraInfo.nightmode = 0;
                if (CameraInfo.flash_en == 1)
                    camera_SetFlashL(CAM_FLASH_NONE);
            }	
            break;	
            
        case CAM_PARAM_WB:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_WB=%d",value);
            switch(value)
            {
                case 0:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
                case 1:
                    camera_SetWhiteBalanceL(CAM_WB_DAYLIGHT);
                    break;
                case 2:
                    camera_SetWhiteBalanceL(CAM_WB_TUNGSTEN);
                    break;
                case 3:
                    camera_SetWhiteBalanceL(CAM_WB_OFFICE);
                    break;
                case 4:
                    camera_SetWhiteBalanceL(CAM_WB_CLOUDY);
                    break;				
                default:
                    camera_SetWhiteBalanceL(CAM_WB_AUTO); /*Auto */
                    break;
            }
            break;
            
        case CAM_PARAM_FLASH:
            MCI_TRACE(MCI_CAMERA_TRC, 0, "[MMC_CAMERA]CAM_PARAM_FLASHlocalEffectCode=%d,value is %d",localEffectCode,value);
            CameraInfo.flash_en=value;
            if (CameraInfo.flash_en == 0 && CameraInfo.nightmode == 1)
                camera_SetFlashL(CAM_FLASH_NONE);
            break;
            
        default:
            break;			
    }	

    return 0;	
}	

#endif
#endif /* CAMERA_SUPPORT */
