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


#ifndef _AP_RECORD_H_
#define _AP_RECORD_H_

#include "resource_id.h"
#include "ap_common.h" 
#include "ap_gui.h"
#include "ap_message.h"


#define RECORD_DIR              L"RECORD"
#define VOICE_DIR               L"VOICE" 
#define MUSIC_DIR               L"MUSIC" 

//磁盘的剩余量不足时，功能失败，以及电压低，提示的次数；
#define	SPACE_LOW		      (20L*512)	//剩余bytes
#define LOW_POWER_COUNT		4	//停止录音时低电压发生的次数

typedef enum
{
	RECORD_STOP,
	RECORD_RECORD,
	RECORD_PALY
}RECORD_STATUS;


#define  MICREC_NOSTART      RESULT_RECORD_NOSTART                //非FM录音,此时录音源由g_mrecord_vars.rec_source决定
#define  MICREC_START   RESULT_RECORD_START         //非FM进程下,按REC键直接进入MIC录音.
#define  FMREC_START    RESULT_FMREC_START    //FM录音.FM进程收听电台时按热键REC进行的FM录音
#define  FMREC_NOSTART  RESULT_FMREC_NOSTART  //FM录音.FM进程收听电台时选择"电台录音"菜单进行的FM录音

//录音源常量定义(全局变量:g_rec_from会用到!)
#define  R_SPDIF          0x80
#define  R_FM             0x40
#define  R_LINEIN         0x20
#define  R_MIC            0x10

#if (APP_SUPPORT_RGBLCD==1)
#define RECORD_CUR_TIME_X   36
#define RECORD_TIME_Y   36

#define RECORD_FILENAME_Y   70

#define RECORD_FILETIME_X   18
#define RECORD_FILETIME_Y   100

#define RECORD_FILEDATE_X   106
#define RECORD_FILEDATE_Y   100
#elif (APP_SUPPORT_LCD==1)
#define RECORD_CUR_TIME_X   20
#define RECORD_TIME_Y   16

#define RECORD_FILENAME_Y   32

#define RECORD_FILETIME_X   0
#define RECORD_FILETIME_Y   48

#define RECORD_FILEDATE_X   88
#define RECORD_FILEDATE_Y   48

#endif


typedef struct
{
	UINT16 rec_sample_rate;	//采样率	
	UINT16 wav_num;       //当前录音文件的编号
    UINT8 volume;
	file_location_t location;
	UINT16 maigc;		//用来判断vm里的数据是否有效
}record_vars_t;

typedef struct
{
    UINT16 maigc; //用来判断vm里的数据是否有效
    UINT8 track_mode;    //0：自动分曲，1：关闭分曲
    UINT8 rec_bitrate;   //0：320kbps，1：256kbps，…，9：8kbps.(通过BitRate_Tab[]转换成module层的接口参数)
    UINT8 rec_source;    //当前设置的录音源。0：Line IN，1：SPDIF，2：MIC
    UINT8 rec_num;       //当前录音文件的编号
    file_location_t location;
}mrecord_vars_t;        //mrecord:指MP3 录音.



/*关闭录音通道的接口函数*/
void RECORD_CloseRecRoute(void);/*共同函数，关闭硬件通道*/
BOOL RECORD_ResetDir(file_location_t *location,WCHAR *DirName);
void RECORD_TotalNum(void);/*计算录音的文件总数目*/

/*不同录音模式的入口函数*/
INT32 RECORD_Entry(INT32 param);//外部接口函数

INT32 record_op(INT32 status); 
INT32 record_read(VOID *buf, INT32 len); 

INT32 RECORD_Entry(INT32 param);

#endif  /* _AP_RECORD_H_*/



