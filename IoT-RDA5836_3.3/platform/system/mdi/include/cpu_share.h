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


#ifndef _CPU_SHARE_H_
#define _CPU_SHARE_H_
#include "cs_types.h"
/* comm buf size set */
#define COMM_IRQCAUSE_INVALID   	0
#define COMM_IRQCAUSE_SENDDATA  	1
#define COMM_IRQCAUSE_SLEEP     	2
#define COMM_IRQCAUSE_FREQUENCY_52MHZ   3
#define COMM_IRQCAUSE_FREQUENCY_78MHZ  4

#define COMM_ADDRESS 	0x1c02001c  /*RTC REG*/

#define MCI_MSG_BUF_SIZE 1024
#define DI_MSG_BUF_SIZE 1024
#define FSM_MSG_BUF_SIZE 1024

#define MMC_MSG_TYPE_INVALID 	0
#define MMC_MSG_TYPE_AUDIO 		1
#define MMC_MSG_TYPE_VIDEO 		2
#define MMC_MSG_TYPE_RECORD 	3
#define MMC_MSG_TYPE_CAMERA 	4
#define MMC_MSG_TYPE_VIDREC 	5
#define MMC_MSG_TYPE_IMAGE 		6
#define MMC_MSG_TYPE_DI			  7
#define MMC_MSG_TYPE_FSM		  8
#define MMC_MSG_TYPE_OTHER		9


typedef enum _mci_func_list
{
//audio msg start---------------------------------------------------
	MCI_MEDIA_PLAY_REQ = MMC_MSG_TYPE_AUDIO * 256,			// 256
	MCI_MEDIA_STOP_REQ,			
	MCI_MEDIA_PAUSE_REQ,			
	MCI_MEDIA_RESUME_REQ,		
	MCI_MEDIA_PLAY_FINISH_IND,	
	MCI_MEDIA_GETID3_REQ,
	MCI_MEDIA_SET_VOLUME_REQ,//JIASHUO ADD
	MCI_RING_SET_VOLUME_REQ,
	MCI_MEDIA_SET_OUTPUTPATH_REQ,//JIASHUO ADD
	MCI_MEDIA_SET_EQ_REQ,//JIASHUO ADD
	MCI_MEDIA_SET_LILY_GPIO_LEVEL,//JIASHUO ADD
	//jiashuo
	//zouying for mp3 dec

//audio msg end----------------------------------------------------

//video msg start---------------------------------------------------
	MCI_VID_CLOSE_REQ = MMC_MSG_TYPE_VIDEO * 256,			// 512
	MCI_VID_OPEN_FILE_REQ,		
	MCI_VID_FILE_READY_IND,		
	MCI_VID_PLAY_REQ,				
	MCI_VID_PLAY_FINISH_IND,		
	MCI_VID_STOP_REQ,				
	MCI_VID_PAUSE_REQ,			
	MCI_VID_RESUME_REQ,			
	MCI_VID_SEEK_REQ,				
	MCI_VID_SEEK_DONE_IND,
	MCI_VID_INFO_REQ,
	MCI_VID_SET_MODE_REQ,

	//shenh for video dec

//video msg end---------------------------------------------------

//----Audio record start------------------------------------------
	MCI_RECORD_START_REQ = MMC_MSG_TYPE_RECORD * 256,		//768
	MCI_RECORD_STOP_REQ,				
	MCI_RECORD_PAUSE_REQ,
	MCI_RECORD_RESUME_REQ,
	//fmradio
	MCI_FMR_RDA5800_INIT,
	MCI_FMR_RDA5800_CLOSE,
	MCI_FMR_RDA5800_OPEN,
	MCI_FMR_RDA5800_TUNE,
	MCI_FMR_RDA5800_MUTE,
	MCI_FMR_RDA5800_SET_VOL,
	MCI_FMR_RDA5800_SEEK,
	MCI_FMR_RDA5800_I2S,
	MCI_FMR_RDA5800_STOPI2S,
	//FMRadio record
	MCI_FMR_RECORD_START_REQ,
	MCI_FMR_RECORD_STOP_REQ,				
	MCI_FMR_RECORD_PAUSE_REQ,
	MCI_FMR_RECORD_RESUME_REQ,
	MCI_FMR_RECORD_STOP_IND,
	
	REC_START_SAMPLE_REQ,
	REC_STOP_SAMPLE_REQ,
	REC_PCMDATA_IND,
  //JIASHUO FOR MP3ENC
  //MSG_MMC_AUDIOENC_VOC_INT,
	MSG_COMM_REFRESH_LCD_ADD,
	MSG_COMM_CANCEL_ADD,
//----Audio record end---------------------------------------------

//camera msg start-------------------------------------------------
	MCI_CAM_POWER_UP_REQ = MMC_MSG_TYPE_CAMERA * 256,		// 1024
	MCI_CAM_POWER_DOWN_REQ,	
	MCI_CAM_PREVIEW_REQ,			
	MCI_CAM_STOP_REQ,				
	MCI_CAM_CAPTURE_REQ,			
	MCI_CAM_SET_PARAM_REQ,	
	MCI_CAM_CAPTURE_IND, //chenhe add for Lily terminal only
	MCI_CAM_CAPTURE_MS_IND,	//chenhe add for Lily terminal only
	MCI_CAM_PREVIEW_CALLBACK,
	MCI_CAM_SAVE_PHOTO,
	MCI_CAM_CANCEL_LASTSAVE,
	MCI_CAM_PLAY_BACK,//chenhe for play back	


	CALL_SPEACH_ON,//for call to open voice device.
	JADE_USB_DETECT,
	JADE_USB_CONNECT,
	JADE_USB_DISCONNECT,	
	MCI_WEB_CAMERA_START,
//camera msg end---------------------------------------------------
  
//----Video record start-------------------------------------------
	MCI_REC_VIDEO_PREVIEW_START_REQ = MMC_MSG_TYPE_VIDREC * 256,	// 1280
	MCI_REC_VIDEO_PREVIEW_STOP_REQ,		
	MCI_REC_VIDEO_START_REQ,				
	MCI_REC_VIDEO_STOP_REQ,				
	MCI_REC_VIDEO_PAUSE_REQ,				
	MCI_REC_VIDEO_RESUME_REQ,			
	MCI_REC_VIDEO_SOUND_INPUT_REQ,		

	MCI_REC_NOMEMEY_FINISH_IND,    
	MCI_REC_VIDEO_ADJUST_SETTING_REQ,
	MCI_REC_VIDEO_FINISH_IND,           //bao add 20071230
//----Video record end----------------------------------------------

//image msg start---------------------------------------------------
	MCI_IMG_DECODE_REQ = MMC_MSG_TYPE_IMAGE * 256,			// 1536
	MCI_IMG_DECODE_EVENT_IND,	
	MCI_IMG_DECODE_FINISH_IND,	
	MCI_IMG_RESIZE_REQ,			
	MCI_IMG_STOP_REQ,	
	MCI_IMG_DISPLAY_IND,
	MCI_IMG_CHECK_DECODE_STATE,
//image msg end-----------------------------------------------------

//di msg start------------------------------------------------------
	DI_XXX_XXX_XXX = MMC_MSG_TYPE_DI * 256,			// 1792
//di msg end--------------------------------------------------------

//fsm msg start-----------------------------------------------------
//FSM_XXX_XXX_XXX = MMC_MSG_TYPE_FSM * 256,			// 2048


	MCI_FSM_OPEN_REQ= MMC_MSG_TYPE_FSM * 256,			// 2048,
	MCI_FSM_CLOSE_REQ,
	MCI_FSM_WRITE_REQ,
	MCI_FSM_READ_REQ,
	MCI_FSM_SEEK_REQ,
	MCI_FSM_GETFILESIZE_REQ,
	MCI_FSM_CREATEDIR_REQ,
	MCI_FSM_GETDISKINFO_REQ,
	MCI_FSM_DELETE_REQ,
	MCI_FSM_DELETEALL_REQ,
	MCI_FSM_FINDFIRST_REQ,
	MCI_FSM_FINDNEXT_REQ,
	MCI_FSM_FINDCLOSE_REQ,
	MCI_FSM_REMOVEDIR_REQ,
	MCI_FSM_MOVE_REQ,
	MCI_FSM_GET_FILEATTR,
	MCI_FSM_HAS_CARD,//chenhe add for check the card whether is in exist on lily


	MCI_FSM_FORMAT_REQ,
	MCI_FSM_FORMAT_IND,

	MCI_FSM_COPY_REQ,
	MCI_FSM_COPY_IND,
	
	MCI_FSM_RENAME_REQ,
	MCI_FSM_ISENDOFFILE_REQ,
	MCI_FSM_GETFILESIZEBYNAME_REQ,
	MCI_FSM_GETFILENAME_REQ,
	MCI_FSM_SORTBYNAME_REQ,

	MCI_FSM_OPEN_IND,			
	MCI_FSM_CLOSE_IND,
	MCI_FSM_WRITE_IND,
	MCI_FSM_READ_IND,
	MCI_FSM_SEEK_IND,
	MCI_FSM_GETFILESIZE_IND,
	MCI_FSM_CREATEDIR_IND,
	MCI_FSM_GETDISKINFO_IND,
	MCI_FSM_DELETE_IND,
	MCI_FSM_DELETEALL_IND,
	MCI_FSM_FINDFIRST_IND,
	MCI_FSM_FINDNEXT_IND,
	MCI_FSM_FINDCLOSE_IND,
	MCI_FSM_REMOVEDIR_IND,
	MCI_FSM_MOVE_IND,
	MCI_FSM_RENAME_IND,
	MCI_FSM_ISENDOFFILE_IND,
	MCI_FSM_GETFILESIZEBYNAME_IND,
	MCI_FSM_GETFILENAME_IND,
	MCI_FSM_SORTBYNAME_IND,
MCI_FSM_COPY_ABORT_REQ,
MCI_FSM_CHANGEFILESIZE_REQ,//gdm add for change file size 080522
//fsm msg end-------------------------------------------------------

//other msg start---------------------------------------------------
	OTHER_XXX_XXX_XXX = MMC_MSG_TYPE_OTHER* 256,			// 2304
	MCI_MULTIMEDIA_MAX_NUM,					
	MCI_REC_MIN_NUM,						
	MCI_REC_MAX_NUM,	
  X_RESULT_MSG,
  MCI_X_MSG,
  MCI_GET_LILY_STATUS,
  
//jade server msg start ----------------------
SERVER_GET_SYSTEMTIME,
SERVER_GET_OEM2UNICODE,
SERVER_GET_UNICODE2OEM,
SERVER_GET_TM_GETSYSTEMTIME,
SERVER_GET_TM_SYSTEMTIMETOFILETIME,
SERVER_GET_TM_FILETIMETOSYSTEMTIME,
SERVER_GET_SYSTEMTIME_PARA,

//other msg end-----------------------------------------------------

}MCI_FUNC_LIST;

typedef enum{
    CAPTURE_MODE_CAMERA = 0,
    CAPTURE_MODE_VIDEO,
#if defined(__ZBAR_SUPPORT__)
    CAPTURE_MODE_QRCODE,
#endif /* __ZBAR_SUPPORT__ */
}CAM_CAPTURE_MEDIA_MODE;

//#include "cs_types.h"
//chenhe for camera,071203 ++
typedef struct
{
	unsigned short start_x;
	unsigned short start_y;
	unsigned short end_x;
	unsigned short end_y;
	unsigned int image_width;
	unsigned int image_height;
	unsigned short preview_rotate;	//xiaoyifeng add  10/11/1
	//effects
	unsigned short nightmode;
	unsigned short imageQuality;
	unsigned short factor;
	unsigned short contrast;
	unsigned short specialEffect;
	unsigned short brightNess;
	unsigned short whiteBlance;
	unsigned short exposure;
	unsigned short addFrame;
	// flash
	unsigned short flashenable;
	
	
}CAM_PREVIEW_STRUCT;
typedef struct
{
	unsigned short image_width;
	unsigned short image_height;
	unsigned char media_mode;
	
}CAM_CAPTURE_STRUCT;

typedef struct
{
  // prview setting
	unsigned short previewStartX;
	unsigned short previewStartY;
  unsigned short previewWidth;
  unsigned short previewHeight;
  
  unsigned short imageWidth;
  unsigned short imageHeight;  

  unsigned short zoomFactor;
  unsigned short exposureVerify;
  unsigned short contrast;
  unsigned short brightnessLevel;
  
  unsigned short whiteBalance;
  unsigned short specialEffect;
  unsigned short nightMode;
  unsigned short lcdrotate;

  //record setting
  unsigned short IfRecordAudio;
  unsigned short fileSizePermit;
  unsigned short recordTimePermit;
  unsigned short encodeQuality;

  //reserved for 4-bytes allignment
  unsigned short * sensorBuf;
}MMC_VDOREC_SETTING_STRUCT;
//chenhe for camera,071203 --


#endif
/* comm buf size set */

