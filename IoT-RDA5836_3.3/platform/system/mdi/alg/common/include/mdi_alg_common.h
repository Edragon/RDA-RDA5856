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




#ifndef _MDI_ALG_COMMON_H_
#define _MDI_ALG_COMMON_H_
#ifndef SHEEN_VC_DEBUG
#include "cs_types.h"
#else
#include "setup.h"
#define volatile
#endif

typedef enum {
A_TYPE_MPEG2_AAC=0,
A_TYPE_MPEG4_AAC,
A_TYPE_AAC,
A_TYPE_AMR,
A_TYPE_MP3,
A_TYPE_PCM,
A_TYPE_ADPCM,
A_TYPE_COOK,
A_TYPE_RA,
A_TYPE_DRA,

V_TYPE_H263,
V_TYPE_MPEG4,
V_TYPE_MJPEG,
V_TYPE_AVI_XVID,
V_TYPE_AVI_DX50,
V_TYPE_AVI_DIV3,
V_TYPE_H264,
V_TYPE_RV,

AV_TYPE_INVALID
}AV_TYPE_ENUM;

//check voc work state in RM/H264 decoder
typedef struct
{
	volatile UINT8 *pVOC_WORK;//VOC_WORK_NON= 0, VOC_WORK_AUD =1, VOC_WORK_VID =2
	volatile INT16 *pVOC_VID;
	volatile INT16 *pVOC_AUD;
	void (*vid_voc_isr)();
	INT32 (*aud_nframe_dec)();//for c audio decoder
	void (*vid_display)();//display frame after start voc
}VocWorkStruct;

typedef struct
{
	unsigned int syncword;//sync word ==0xAAAAAAAA for frame data start.
	unsigned int timestamp;//display time stamp, ms
	unsigned int samplesize;//this video or audio frame sample size, if lost some data fill the real get. size not include avframeinfohdstr itself.
}avframeinfohdstr;//header info before each frame data

typedef struct
{
	short avid;//0=audio hdl,1=video hdl
	short frminfoflag;//flag info need frame header avframeinfohdstr or not. 0=not need, 1=need fill.
	unsigned int bytesneed;//data size need fill
}avfillstr;//==avagentstr public

typedef struct
{
	//public
	short avid;//0=audio hdl,1=video hdl
	short frminfoflag;//flag info need frame header avframeinfohdstr or not. 0=not need, 1=need fill.
	unsigned int bytesneed;//data size need fill
	//private
	int bufupdate;//other task maybe call avstream_fillBuf. no=0,updating=1,over=2
	unsigned char *ringbufptr;//ringbuf address
	unsigned char *rdptr;//read data pos. rdptr-wtptr>=4bytes(rdptr right, wtptr left) or rdptr-wtptr<0(rdptr left, wtptr right)
	unsigned char *wtptr;//fill data pos.
	int ringbuflen;//buf bytes size
	int mirrorlen;//each mirror area bytes size at ringbuf head and tail.
}avagentstr;//for fill

typedef struct
{
	unsigned char *ringbufptr;//ringbuf address
	unsigned char *rdptr;//read data pos. rdptr-wtptr>=4bytes(rdptr right, wtptr left) or rdptr-wtptr<0(rdptr left, wtptr right)
	unsigned char *wtptr;//fill data pos.
	int ringbuflen;//buf bytes size
	int mirrorlen;//each mirror area bytes size at ringbuf head and tail.
	short frminfoflag;//flag info need frame header avframeinfohdstr or not. 0=not need, 1=need fill.
	avagentstr *avstreamhdl_agent;//avstreamhdl's agent, copy current audio hdl info for fill task avstream_srv_fillBuf.
}avstreamstr;

typedef struct
{
	avstreamstr *avstreamhdl_a;//audio stream handle,by avstream_create or null
	avstreamstr *avstreamhdl_v;//video stream handle, by avstream_create or null
	void(*avstream_srv_callback)(short avid);
		
	unsigned int totaltime;//ms
	unsigned int avtimelineflag;// 1=same(cmmb) or 0=different(rtsp) timeline for audio/video time stamp reference.
	//audio info
	unsigned int fmt_a;//audio format
	unsigned int samplerate_a;
	unsigned short bits_a;
	unsigned short channels_a;
	unsigned int totalfrm_a;//total frame count or time/data length
	unsigned int baseclock_a;//scale of 1 second for timestamp. get from protocol. (exp.rtsp same with samplerate_a,cmmb 22500)
	//video info
	unsigned int fmt_v;//video format
	unsigned short width_v;
	unsigned short height_v;
	//unsigned short fps_v;
	unsigned int totalfrm_v;//total frame count or time/data length
	unsigned int baseclock_v;//scale of 1 second for timestamp. get from protocol. (exp.rtsp 90000,cmmb 22500)
	unsigned int frmtimescale_v;//fps=baseclock_v/frmtimescale_v
}avinfostr;//as filehandle to player open

//for http down load
INT32 http_fs_parsehead(INT32 handle);
void http_fs_init(INT32 handle);
INT32 http_fs_read(INT32 handle, UINT8 *pBuf, UINT32 iLen);
INT32 http_fs_seek(INT32 handle, INT32 iOffset, UINT8 iOrigin);
INT32 http_fs_getFileSize(INT32 handle);
INT32 http_fs_isEndOfFile(INT32 handle);

//server
avstreamstr *avstream_srv_create(INT32 avmode);
avfillstr* avstream_srv_getfillhdl(avinfostr* avinfohdl);
int avstream_srv_fillBuf(avfillstr* avfillhdl,unsigned char*srcbuf,int srclen);
void avstream_srv_fillover(avfillstr* avfillhdl);
void avstream_srv_reset(avinfostr* avinfohdl);
void avstream_srv_destroyall(avinfostr* avinfohdl);
//client
void avstream_clt_checkagent(avstreamstr* avstreamhdl);
void avstream_clt_sendagent(avstreamstr* avstreamhdl);
int avstream_clt_getDataLen(avstreamstr* avstreamhdl);
unsigned char* avstream_clt_getDataPos(avstreamstr* avstreamhdl);
void avstream_clt_moveDataPos(avstreamstr* avstreamhdl,int consumeLen);
int avstream_clt_getData(avstreamstr* avstreamhdl,unsigned char* output, unsigned int blkSize, unsigned int blkNum);
int avstream_clt_getheader(avstreamstr* avstreamhdl,avframeinfohdstr* header);



#endif


