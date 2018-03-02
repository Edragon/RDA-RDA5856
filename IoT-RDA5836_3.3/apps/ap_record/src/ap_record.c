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


#include "ap_record.h"
#include "ap_media.h"
#include "ap_music.h"
#include "event.h"
#include "hal_host.h"
#include "hal_debug.h"



#if APP_SUPPORT_RECORD==1
#define RECORD_FFPLAY_STEP           7  // second

#define RECORD_SET_SOURCE               AP_MSG_USER_RECORDER+1
#define RECORD_SET_SAMPRATE             AP_MSG_USER_RECORDER+2
#define RECORD_SELECT_FOLDER            AP_MSG_USER_RECORDER+3

//Common variables
extern INT8            m_adagencDataBuffer[];
extern UINT8            fselDirLayer;  //dir  layer
extern UINT8            fselDirNo[];    //Number of current valid directory
extern BOOL            ui_auto_update;


UINT32                  g_file_size = 0;             //当前录音文件的page数
UINT32                  g_rec_time = 0;              //实时显示录音的时间
UINT32                  g_rec_bitrate = 0;
UINT32                  g_free_space = 0;            //剩余磁盘空间

//播放录音时,快进/快退标志位
static UINT8    ffflag = 0;

//本模块的全局变量，只有在进入本模块的时候调用
record_vars_t *g_record_vars = NULL;
//录音总文件数目
UINT8   g_file_num = 0;              //录音文件总个数
UINT8 g_max_file_num = 0; // max file number
INT8   g_play_direct = 0;   // search play file dir

UINT32 g_totalTime = 0;
UINT32 g_file_date = 0;
UINT32 g_file_time = 0;

//当前文件的全局变量
static UINT8    g_rec_from = 0;    //当前的录音源
//当前状态
enum
{
    stop,
    prepareing,
    paused,
    recording,
    playing,
    play_paused,
    wait_to_save,
    saving
}   g_rec_status = stop;

enum
{
    RECORD_REDRAW_A = 0x1,
    RECORD_REDRAW_B = 0x2,
};

static UINT8 g_record_screen_status = RECORD_STOP;

extern const TCHAR ExtVoice[];

static void RECORD_Display(UINT32 param);
static BOOL RECORD_CheckLowBatt(void);
static INT32 RECORD_EnterStatus(UINT8 status);

/*********************************************************************************
* Description : 设置录音缺省目录\Record\Music
*
* Arguments   :
*
* Returns     :
*
* Notes       : 把\Record\Voice做为当前目录
*
*********************************************************************************/
BOOL RECORD_ResetDir(file_location_t *location,WCHAR *DirName)
{
    // INT32  status;
    //UINT32 inode;

//    tcscpy(location->path, DirName);

//    location->subdir_entry[11]    = 0x00;
//    location->disk        = FS_DEV_TYPE_TFLASH;
    location->file_name[0] = 0;
     
    FS_ChangeDir((PCSTR)FS_CD_ROOT);
				
    if(ERR_SUCCESS!=FS_ChangeDir((PCSTR)RECORD_DIR))
    {
	    if(ERR_SUCCESS!=FS_MakeDir((PCSTR)RECORD_DIR, 0))
                return FALSE;          // 创建目录失败
            FS_ChangeDir((PCSTR)RECORD_DIR);
    } 
		
    if(ERR_SUCCESS!=FS_ChangeDir((PCSTR)DirName))
    {
        if(ERR_SUCCESS!=FS_MakeDir((PCSTR)DirName, 0))
            return FALSE;          // 创建目录失败
        FS_ChangeDir((PCSTR)DirName);
    }

    FS_GetSubDirEntry(&(location->subdir_entry));

//    location->dirLayer  = 1;
//    FS_ChangeDir(DirName);
//    FS_GetCurDir(sizeof(location->path),location->path);

    return TRUE;
}
/*********************************************************************************
* Description : draw A, B, CD, EF, BATT, G,  并且刷新界面
*
* Arguments   :
*
* Returns     :
*
* Notes       :
*
*********************************************************************************/
static void RECORD_Display(UINT32 param)
{
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    INT8 buf[8];
    static BOOL time_twinkling = FALSE;

    if(AP_Support_LCD())
    {
        ui_auto_update = FALSE;

        if(param & RECORD_REDRAW_A)
        {
            GUI_ClearScreen(NULL);

            GUI_DisplayIconA(GUI_IMG_MREC);
            GUI_ResShowImage(GUI_IMG_MUSICTYPE, 1, POSITION_B, 0);
            if(g_rec_status == playing || g_rec_status == play_paused)
            {
                GUI_DisplayIconC(GUI_IMG_PLAY);
            }
            else if(g_rec_from == R_FM)
            {
                GUI_DisplayIconC(GUI_IMG_RA_FM);
            }
            GUI_DisplayNumbers(POSITION_B+3, 0, 3, GUI_IMG_BITRATENUMS, g_rec_bitrate);

            GUI_ResShowImage(GUI_IMG_DEVICE, g_record_vars->location.disk, POSITION_F, 0);
            //BT_DisplayIcon(101,0);

            //sprintf(buf, "%03d", g_record_vars->wav_num);
            //GUI_DisplayText(POSITION_D, 0, buf);
            sprintf(buf, "%03d", g_file_num);
            GUI_DisplayText(POSITION_D, 0, buf);

            GUI_DisplayBattaryLever();

            GUI_DisplayText(LCD_WIDTH/2-4, RECORD_TIME_Y, "/");
            GUI_DisplayTimeText(LCD_WIDTH/2+4, RECORD_TIME_Y, GUI_TIME_DISPLAY_AUTO, g_totalTime);

            //file name & time
            memcpy(buf, g_record_vars->location.file_name, 6);
            buf[6] = 0;
            GUI_DisplayText(24, RECORD_FILENAME_Y, buf);
            GUI_DisplayText(72, RECORD_FILENAME_Y, ".WAV");
            GUI_DisplayDateText(RECORD_FILETIME_X, RECORD_FILETIME_Y, TRUE, g_file_date);
            GUI_DisplayTimeText(RECORD_FILEDATE_X, RECORD_FILEDATE_Y, GUI_TIME_DISPLAY_AUTO, g_file_time*1000);
        }

        // current time
        time_twinkling = !time_twinkling;
        if((param & RECORD_REDRAW_B) && (g_rec_status == paused) && time_twinkling)
        {
            GUI_DisplayText(RECORD_CUR_TIME_X, RECORD_TIME_Y, "     ");
        }
        else
        {
            GUI_DisplayTimeText(RECORD_CUR_TIME_X, RECORD_TIME_Y, GUI_TIME_DISPLAY_AUTO, g_rec_time);
        }



        GUI_UpdateScreen(NULL);
        ui_auto_update = TRUE;
    }
#elif APP_SUPPORT_LED8S==1
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_ICON_RECORD|GUI_ICON_COLON,0,0);
    if(g_record_vars->location.disk == FS_DEV_TYPE_USBDISK)
        GUI_ResShowPic(GUI_ICON_USB,0,0);
    else
        GUI_ResShowPic(GUI_ICON_SD,0,0);
    if(g_rec_status == recording || g_rec_status == playing)
        GUI_ResShowPic(GUI_ICON_PLAY,0,0);
    else
        GUI_ResShowPic(GUI_ICON_PAUSE,0,0);

    GUI_DisplayTimeLEDs(g_rec_time);

    GUI_UpdateScreen(NULL);
#endif

#if (APP_SUPPORT_LED == 1)
    if(g_rec_status == playing)
    {
        LED_SetPattern(GUI_LED_REC_PLAY, LED_LOOP_INFINITE);
    }
    else if(g_rec_status == recording)
    {
        if(g_rec_from == R_FM)
        {
            LED_SetPattern(GUI_LED_REC_FM, LED_LOOP_INFINITE);
        }
        else if(g_rec_from == R_MIC)
        {
            LED_SetPattern(GUI_LED_REC_MIC, LED_LOOP_INFINITE);
        }
    }
    else if(g_rec_status == stop)
    {
        LED_SetPattern(GUI_LED_REC_STOP, LED_LOOP_INFINITE);
    }
    else if(g_rec_status == paused)
    {
        LED_SetPattern(GUI_LED_REC_PAUSE, LED_LOOP_INFINITE);
    }
#endif /* APP_SUPPORT_LED */

}


/*********************************************************************************
* Description : 根据当前录音文件的文件大小, 查表判断电压是否低
*
* Arguments   : batt, 电池电压
        全局变量
        g_file_size, 当前录音文件的文件大小,单位page
*
* Returns     : TRUE, 电压低
        FALSE, 电压正常
*
* Notes       :
*
*********************************************************************************/
static BOOL RECORD_CheckLowBatt(void)
{
    UINT32 batt_level=12;
    UINT32 batt = GetBattery();

    if(IsChargePlugIn())
        return FALSE;

    // file < 32M
    if(g_file_size < 32L * 1024 * 2)
    {
        // if(g_comval->BatteryType == BATT_TYPE_ALKALINE ) batt_level = 4;
        // else if(g_comval->BatteryType == BATT_TYPE_NIH ) batt_level = 5;
        // else if(g_comval->BatteryType == BATT_TYPE_LITHIUM) batt_level = 11;
        // else APP_ASSERT(FALSE);
        batt_level --;
    }

    if(batt <= batt_level)
        return TRUE;
    else
        return FALSE;
}



/*********************************************************************************
* Description : 分析剩余空间,计算可录音时间
*
* Arguments   :
*
* Returns     : 全局变量
        g_free_space, 剩余空间,单位page
*
* Notes       :
*
*********************************************************************************/
static void RECORD_RestTime(void)
{
    //UINT32 total_sec=0L;
    //ap_time_t time;

    g_free_space = FS_GetSpace(g_record_vars->location.disk,FS_GET_SPACE_FREE);

    if(g_free_space > SPACE_LOW)
        g_free_space -= SPACE_LOW;
    else
    {
        g_free_space = 0;
        return;
    }
    hal_HstSendEvent(SYS_EVENT,g_free_space);

    if(g_rec_from == R_FM)
    {
        g_rec_bitrate = g_record_vars->rec_sample_rate * 8 * 2 / 1000;
    }
    else
    {
        g_rec_bitrate = 64;
    }

    g_totalTime = g_free_space / (g_rec_bitrate / 8 );
}


/*********************************************************************************
* Description : 计算当前目录下的总录音文件个数
*
* Arguments   :
*
* Returns     : 全局变量
        g_file_num, 总录音文件个数
*
* Notes       :
*
*********************************************************************************/
void RECORD_TotalNum(void)
{
    UINT64 inode;
    UINT32 file_number;
    UINT8 filename[30];

    g_file_num = 0;

    if(ERR_SUCCESS==FS_GetNextEntry(&inode, (PTCHAR)ExtVoice,FS_DIR_FILE))
    {

        do
        {
            file_number = 0;
            FS_GetShortName(inode, filename);
            sscanf(filename+3, "%d", &file_number);
            if(file_number > 0)
            {
                g_file_num++;
                if(file_number > g_max_file_num)
                    g_max_file_num = file_number;
            }
        }
        while( ERR_SUCCESS==FS_GetNextEntry(&inode, (PTCHAR)ExtVoice,FS_DIR_FILE_CONT));
    }

}


/*********************************************************************************
* Description : 格式化文件名
*
* Arguments   : 全局变量
        g_record_vars->rec_type
*
* Returns     : 全局变量
        g_record_vars->location.filename
*
* Notes       : RECxx.WAV or RECxx.ACT
*
*********************************************************************************/
static void RECORD_FileName(void)
{
    sprintf(g_record_vars->location.file_name, "REC%03d  WAV", g_record_vars->wav_num);
}

/******************************************************************************
 * RECORD_FileInfo:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static BOOL RECORD_FileInfo(void)
{
    Open_param_t param;
    INT32 result;


    RECORD_FileName();

    param.file_name = g_record_vars->location.file_name;
    param.mode = MEDIA_PLAYREC;
    param.type = Music_WAV;

    //open codec
    result = mediaSendCommand(MC_OPEN, (INT32)&param);

    if(!result) // open file fail, select next file
    {
        app_trace(APP_RECORD_TRC, "RECORD_FileInfo(%d), error",g_record_vars->wav_num);
        return FALSE;
    }

    if(AP_Support_LCD())
    {
        // get file infomation
        audInfoStruct *pAudioinfo = COS_Malloc_BTOverlay(sizeof(audInfoStruct));
        if(mediaSendCommand(MC_GETFILEINFO, (INT32)pAudioinfo))
        {
            g_rec_bitrate = pAudioinfo->bitRate/1000;
            g_totalTime = pAudioinfo->time;
            g_file_date = pAudioinfo->createDate;
            g_file_time = pAudioinfo->createTime;
        }
        COS_Free_BTOverlay(pAudioinfo);
    }
    else
    {
        mediaSendCommand(MC_GETTOTALTIME, (INT32)&g_totalTime);
    }

    mediaSendCommand(MC_CLOSE, 0);

    return TRUE;
}

/*********************************************************************************
* Description : 生成新的录音文件名
*
* Arguments   : 全局变量
        g_record_vars->rec_type
*
* Returns     : TRUE, 成功
        FALSE, 编号01~99 文件都存在
        全局变量
        g_record_vars->location
        g_record_vars->wav_num
        g_file_name_time[], 文件名,文件时间字符串
*
* Notes       :
*
*********************************************************************************/
static BOOL RECORD_NewFileName(void)
{
    date_t date;
    ap_time_t time;

    INT32 num;

    g_record_vars->wav_num = 1;
    RECORD_FileName();

    num = GenNewName(&g_record_vars->location);

    if(num == 0) return FALSE;

    if(num > g_max_file_num)
        g_max_file_num = num;

    g_record_vars->wav_num = num;

    RECORD_FileName();

    //build g_file_name_time for display
    TM_GetDateAndTime(&date, &time);

    g_file_date = ((date.year)<<9) + date.month*32 +date.day;
    g_file_time = time.hour *3600 + time.minute * 60 + time.second;
    return TRUE;
}



BOOL FMR_IsActive(void)
{
    return g_rec_status != stop && g_rec_from == R_FM;
}

UINT16 FMR_Get_SampleRate(void)
{
    return g_record_vars->rec_sample_rate;
}
/******************************************************************************
 * Record_PrepareForNewRecord:
 * DESCRIPTION: -
 * Check disk available storage and record files, and
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT16 Record_PrepareForNewRecord(void)
{
#if APP_SUPPORT_MENU==1
    if(g_file_num >= 200 && AP_Support_MENU())
    {
        return GUI_STR_MDIRFULL;
    }
#endif

    RECORD_RestTime();

    //check disk out
    if(g_free_space == 0)
    {
        return GUI_STR_MWRIERR;
    }
    if(!RECORD_NewFileName())
    {
        return GUI_STR_MFILEOUT;
    }

    return 0;
}

/******************************************************************************
 * Record_PlayStaus_OpenFile:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL Record_PlayStaus_OpenFile(void)
{
    Open_param_t param;
    INT32 result;

    mediaSendCommand(MC_STOP, 0);
    mediaSendCommand(MC_CLOSE, 0);

    if(RECORD_FileInfo())
    {
        RECORD_FileName();

        param.file_name = g_record_vars->location.file_name;
        param.mode = MEDIA_PLAYREC;
        param.type = Music_WAV;
        //open codec
        result = mediaSendCommand(MC_OPEN, (INT32)&param);

        SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_A);

        return TRUE;
    }

    return FALSE;
}

/******************************************************************************
 * Record_Init:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT16 Record_Init(INT32 param)
{
    INT16 error = 0;
    //INT32 result=0;

    BOOL status;
    WCHAR pathname[20];

    g_record_vars = (record_vars_t*)NVRAMGetData(VM_AP_RECORD,sizeof(record_vars_t));

    if(g_record_vars->maigc != MAGIC_RECORD)
    {
        g_record_vars->maigc = MAGIC_RECORD;
        g_record_vars->rec_sample_rate=16000; // 16K
        g_record_vars->wav_num = 1;
        g_record_vars->volume = 8;
        g_record_vars->location.disk = FS_DEV_TYPE_TFLASH;
    }
#if APP_SUPPORT_INDEPEND_VOLUMNE==0
    g_record_vars->volume = g_comval->Volume;
#endif

    #ifdef APP_RECORD_VOICE_INRAM
    /* Store voice data in ram, so no FS needed */
    return 0; 
    #endif

	app_trace(APP_RECORD_TRC, "Record_Init: Mount Disk, default=%d",g_record_vars->location.disk);

    //初始化文件系统
    if(MountDisk(g_record_vars->location.disk))
    {
    }
    else if(MountDisk(FS_DEV_TYPE_TFLASH))
    {
        g_record_vars->location.disk = FS_DEV_TYPE_TFLASH;
    }
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
#if APP_SUPPORT_USB
    else if(MountDisk(FS_DEV_TYPE_USBDISK))
    {
        g_record_vars->location.disk = FS_DEV_TYPE_USBDISK;
    }
#endif
#endif
    else
    {
        return GUI_STR_MREADERR;
    }

	app_trace(APP_RECORD_TRC, "Record_Init: Get saved directory");

    status = FS_GetEntryName(g_record_vars->location.subdir_entry, 20, (PTCHAR)pathname);
    if(status == ERR_SUCCESS && ERR_SUCCESS==FS_ChangeSubDir(g_record_vars->location.subdir_entry))
    {
        status =TRUE;
    }
    else
    {
		app_trace(APP_RECORD_TRC, "Record_Init: Reset directory");
        status = RECORD_ResetDir(&g_record_vars->location,VOICE_DIR);
    }


    if(status)
    {

        if((MICREC_START == param) || (MICREC_NOSTART == param))
        {
            g_rec_from = R_MIC;
        }
#if APP_SUPPORT_FM_RECORD
        else if((FMREC_START == param) || (FMREC_NOSTART == param))
        {
            g_rec_from = R_FM;
        }
#endif

    }
    else
    {
		app_trace(APP_RECORD_TRC, "Record_Init: Get free space");
        g_free_space = FS_GetSpace(g_record_vars->location.disk ,FS_GET_SPACE_FREE);
        error = GUI_STR_MREADERR;
    }

	app_trace(APP_RECORD_TRC, "Record_Init: Finish, err=%d, rec_from=%d", error, g_rec_from);

    return error;
}

/******************************************************************************
 * Record_PlayStatus_SelectFile:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void Record_PlayStatus_SelectFile(BOOL forward)
{
    if(forward)
    {
        if(++(g_record_vars->wav_num) > g_max_file_num)
            g_record_vars->wav_num = 1;
    }
    else
    {
        if(--(g_record_vars->wav_num) < 1)
            g_record_vars->wav_num = g_max_file_num;
    }
}

void RECORD_VolumeCb( INT32 Vol )
{
    mediaSendCommand(MC_SETVOLUME, Vol);
    g_record_vars->volume = Vol;
    g_comval->Volume = Vol;
}

INT32 ChangePlayVolume( UINT32 key )
{
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    extern slider_t         slider;

    if(AP_Support_LCD())
    {
        //show background
        GUI_ClearScreen(NULL);
        GUI_ResShowPic(GUI_IMG_VOLUMEBK,0,0);

        slider.min = 0;
        slider.max = MUSIC_MAX_VOLUME;
        slider.value = g_record_vars->volume;
        slider.step = 1;

        ui_auto_update = FALSE;
        GUI_Slider(& slider, RECORD_VolumeCb);
        ui_auto_update = TRUE;

    }
    else
#endif
    {
        if((key&0xffff0000) == AP_KEY_VOLADD)
        {
            g_record_vars->volume++;
            if(g_record_vars->volume > MUSIC_MAX_VOLUME)
                g_record_vars->volume = MUSIC_MAX_VOLUME;
        }
        if((key&0xffff0000) == AP_KEY_VOLSUB)
        {
            g_record_vars->volume--;
            if(g_record_vars->volume < 0)
                g_record_vars->volume = 0;
        }
        mediaSendCommand(MC_SETVOLUME, g_record_vars->volume);
        g_comval->Volume = g_record_vars->volume;
    }


    return 0;
}

INT32 rec_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
        return 0;

    switch(value)
    {
        case RECORD_SET_SOURCE:
            return g_rec_from != param;
            break;
        case RECORD_SET_SAMPRATE:
            return g_rec_from == R_FM && g_record_vars->rec_sample_rate!= param;
            break;
    }
    return 0;
}

/******************************************************************************
 * RECORD_DiskFullCallback:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void RECORD_DiskFullCallback(void)
{
    GUI_Display_Menu(GUI_MENU_RECORD, rec_menu_callback);
}


/******************************************************************************
 * Record_recording_callback_hdlr:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void Record_recording_callback_hdlr(UINT32 result )
{
    if(result == RECORDING_DISK_ERROR)    //致命错误
    {
        GUI_DisplayMessage(0,GUI_STR_MREADERR, NULL, GUI_MSG_FLAG_WAIT);
        //GUI_DisplayErr(GUI_STR_MREADERR);
        //APP_ASSERT(FALSE);
        //while(1) {;}    //dead loop
        //APP_DUMP("disk write error!", 0);
    }

}

/******************************************************************************
 * Record_playing_callback_hdlr:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void Record_playing_callback_hdlr(UINT32 result )
{
    Record_PlayStatus_SelectFile(TRUE);

    if(Record_PlayStaus_OpenFile())
    {
        mediaSendCommand(MC_PLAY, 0);
        g_rec_status = playing;
    }

}
#if APP_SUPPORT_LCD==1
void Record_SelectFolderCb(file_location_t *f, UINT16 result)
{
	if(LIST_CALLBACK_SELECT_ITEM == result)
	{
	    g_record_vars->location = *f;
	    RECORD_EnterStatus((UINT8)RECORD_STOP);
   	}
}
#endif

#if (APP_SUPPORT_USB)
static void RecordHandleUSBPlugOut(UINT32 param)
{
    if(g_record_vars->location.disk == FS_DEV_TYPE_USBDISK)
    	GUI_DisplayMessageWithCB(0,GUI_STR_CARDPULL, NULL, AppGotoMainMenu, GUI_MSG_FLAG_WAIT);
}
#endif /* APP_SUPPORT_USB */

static void RecordHandleTFalshCardPlugInOut(UINT32 param)
{
	if(1 == param)//Plug in
	{
	}
	else if(0 == param)//plug out
	{
        if(g_record_vars->location.disk == FS_DEV_TYPE_TFLASH)
	    	GUI_DisplayMessageWithCB(0,GUI_STR_CARDPULL, NULL, AppGotoMainMenu, GUI_MSG_FLAG_WAIT);
	}
}
/******************************************************************************
 * RECORD_EnterStatus:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT32 RECORD_EnterStatus(UINT8 status)
{
    UINT8 old_status = g_record_screen_status;
    UINT8 new_status = status;
    INT16 error = 0;
    media_status_t rec_stat;

    app_trace(APP_RECORD_TRC,"RECORD_EnterStatus:%d, old=%d",new_status, old_status);

    switch(old_status)
    {
        case RECORD_STOP:
            break;

        case RECORD_RECORD:
            GUI_DisplayMessage(0, GUI_STR_RSAVING, NULL, GUI_MSG_FLAG_DISPLAY);
			ClearProtocolEventHandler(EV_AUDIO_RECORD_FINISH_IND);
#if APP_SUPPORT_FM_RECORD
            if(R_FM == g_rec_from)
            {
                error = AppFMExternalDo(MC_CLOSE, 0);
            }
#endif
            mediaSendCommand(MC_STOP, 0);
            mediaSendCommand(MC_GETSTATUS, (int)&rec_stat);
            //check status for codec stop
            if(rec_stat.status == RECORDING_STOP)
            {
                app_trace(APP_RECORD_TRC,"RECORD save file OK");
                GUI_DisplayMessage(0,GUI_STR_RECORDOK, NULL, GUI_MSG_FLAG_DISPLAY);
            }
            else
            {
                app_trace(APP_RECORD_TRC,"RECORD save file Fail!");
            }

            mediaSendCommand(MC_CLOSE, 0);
            break;

        case RECORD_PALY:
			ClearProtocolEventHandler(EV_AUDIO_PLAY_FINISH_IND);
            mediaSendCommand(MC_STOP, 0);
            mediaSendCommand(MC_CLOSE, 0);
            break;

        default:
            break;
    }

    //Refresh file info and Recount record files
    RECORD_TotalNum();
    RECORD_FileName();
    RECORD_FileInfo();
    //init the counter
    g_rec_time = 0;

    switch(new_status)
    {
        case RECORD_STOP:
            g_rec_status = stop;
            break;

        case RECORD_RECORD:
        {
            Open_param_t param;

           #ifndef APP_RECORD_VOICE_INRAM
            error = Record_PrepareForNewRecord();

            //More than 200 rec files
            if(GUI_STR_MDIRFULL == error)
            {
                GUI_DisplayMessageWithCB(0,error, NULL, RECORD_DiskFullCallback, GUI_MSG_FLAG_WAIT);
                return error;
            }
            else if(error !=0)//Disk full Or get RECORD_NewFileName error
            {
                GUI_DisplayMessageWithCB(0,error, NULL, AppGobackScreen, GUI_MSG_FLAG_WAIT);
                return error;
            }

            g_file_size = 0L;
	        
			//update by wangxin record
            #ifndef CHIP_5856

            //检查电压
            if(RECORD_CheckLowBatt())
            {
                app_trace(APP_RECORD_TRC, "RECORD low battery!");
                GUI_DisplayMessage(0,GUI_STR_MPWROUT, NULL, GUI_MSG_FLAG_WAIT);
                return 0;
            }
            #endif
#if APP_SUPPORT_FM_RECORD
            if(R_FM == g_rec_from)
            {
                if(!AppFMExternalDo(MC_OPEN, 0))
                {
                    app_trace(APP_RECORD_TRC, "start fm error");
                    GUI_DisplayMessage(0,GUI_STR_RECORDERROR, NULL, GUI_MSG_FLAG_WAIT);
                    return 0;
                }
            }
            #endif
			#endif
            //防止在录音过程中切换bank
            g_rec_status = prepareing;

            param.file_name = g_record_vars->location.file_name;
            param.mode = MEDIA_RECORD;
            param.type = Music_WAV;
            //open codec
            error = mediaSendCommand(MC_OPEN, (INT32)&param);
            if(!error)
            {
                app_trace(APP_RECORD_TRC, "open record file error ");
                GUI_DisplayMessage(0,GUI_STR_RECORDERROR, NULL, GUI_MSG_FLAG_WAIT);
                return 0;
            }

            error = mediaSendCommand(MC_RECORD, 0);
            if(!error)
            {
                app_trace(APP_RECORD_TRC, "start record error");
                GUI_DisplayMessage(0,GUI_STR_RECORDERROR, NULL, GUI_MSG_FLAG_WAIT);
                return 0;
            }
            
			SetProtocolEventHandler(Record_recording_callback_hdlr, EV_AUDIO_RECORD_FINISH_IND);
            g_rec_status = recording;
            app_trace(APP_RECORD_TRC, "start record ... ");
        }
        break;

        case RECORD_PALY:
            Record_PlayStaus_OpenFile();

            RECORD_VolumeCb(g_record_vars->volume);
            mediaSendCommand(MC_PLAY, 0);
            g_rec_status = playing;

			SetProtocolEventHandler(Record_playing_callback_hdlr, EV_AUDIO_PLAY_FINISH_IND);
            app_trace(APP_RECORD_TRC, "play record file");
            break;

        default:
            break;

    }


    g_record_screen_status = new_status;


    SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_A);

    // LED_SetPattern(GUI_LED_MUSIC_STOP + MIN(PlayStatus, FollowRec), LED_LOOP_INFINITE);
    return 0;
}

/******************************************************************************
 * RECORD_CommonKeyHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL RECORD_CommonKeyHandler(UINT32 key)
{
#if APP_SUPPORT_MENU==1
    INT16 error = 0;
#endif
    switch(key)
    {
        case AP_MSG_WAIT_TIMEOUT:
            if(g_rec_status == stop || g_rec_status== paused || g_rec_status == play_paused)
                APP_Sleep();
            break;

        case AP_KEY_MODE | AP_KEY_PRESS:
#if APP_SUPPORT_MENU==1
            if(AP_Support_MENU())
            {
                GUI_Display_Menu(GUI_MENU_RECORD, rec_menu_callback);
            }
            else
#endif
                AppGotoMainMenu();
            break;

        case AP_KEY_MODE|AP_KEY_LONG: // switch record source
            if(!MESSAGE_IsHold())
                break;
        case RECORD_SET_SOURCE:
#if APP_SUPPORT_FM_RECORD
            if(R_MIC == g_rec_from)
            {
                SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_FMREC_NOSTART);
            }
            else
#endif
            {
                SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_RECORD_NOSTART);
            }
            break;

        case RECORD_SET_SAMPRATE:
#if APP_SUPPORT_MENU==1
            if(g_rec_from == R_FM)
            {
                UINT16 param;

                GUI_GetMenuResult(&param);
                g_record_vars->rec_sample_rate = param;
                g_rec_bitrate = g_record_vars->rec_sample_rate * 8 * 2 / 1000;
                error = Record_PrepareForNewRecord();

                if(error !=0)
                {
                    GUI_DisplayMessageWithCB(0,error, NULL, AppGobackScreen, GUI_MSG_FLAG_WAIT);
                    break;
                }

                SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_A);
            }
#endif
            break;

        case RECORD_SELECT_FOLDER:
#if APP_SUPPORT_LCD==1
            GUI_EntryFileBrowser(&g_record_vars->location, DIRECTORY_ALL, g_comval->langid, Record_SelectFolderCb);
#endif
            break;

        case AP_KEY_VOLSUB | AP_KEY_DOWN:
        case AP_KEY_VOLADD | AP_KEY_DOWN:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
	 #if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_VOLADD:
	 case AT_COMMON_VOLSUB:
	 #endif
            ChangePlayVolume(key);
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

BOOL Record_PlayStatusKeyHandler(UINT32 key)
{
    switch( key )
    {
        case AP_MSG_STANDBY:
            //do nothing
            break;

        case AP_KEY_PLAY | AP_KEY_PRESS:
	#if defined(__AT_MOD_RECORD__)
	 case AT_RECORD_PAUSE:
	 #endif
            RECORD_EnterStatus(RECORD_STOP);
            break;

        case AP_KEY_PREV| AP_KEY_PRESS:
        #if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_PREV:
	 #endif
			
            Record_PlayStatus_SelectFile(FALSE);
            Record_PlayStaus_OpenFile();
            mediaSendCommand(MC_PLAY, 0);
            g_rec_status = playing;
            break;

        case AP_KEY_NEXT | AP_KEY_PRESS:
	 #if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_NEXT:
	 #endif
            Record_PlayStatus_SelectFile(TRUE);
            Record_PlayStaus_OpenFile();
            mediaSendCommand(MC_PLAY, 0);
            g_rec_status = playing;
            break;


        case AP_KEY_PLAY | AP_KEY_HOLD:
        case AP_KEY_REC | AP_KEY_PRESS:
            RECORD_EnterStatus(RECORD_RECORD);
            break;

        case AP_KEY_NEXT | AP_KEY_LONG: //快进
         #if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_FORE:
	 #endif
            if(ffflag == 0)
            {
                mediaSendCommand(MC_STOP, 0);
                g_rec_status = play_paused;
            }
            ffflag = 1;

            g_rec_time += RECORD_FFPLAY_STEP*1000;
            if(g_rec_time > g_totalTime)
                g_rec_time = g_totalTime;

            SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_B);

            break;
        case AP_KEY_PREV | AP_KEY_LONG: //快退
         #if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_BACK:
	 #endif
            if(ffflag == 0)
            {
                mediaSendCommand(MC_STOP, 0);
                g_rec_status = play_paused;
            }
            ffflag = -1;

            if(g_rec_time  < RECORD_FFPLAY_STEP*1000)
                g_rec_time = 0;
            else
                g_rec_time -= RECORD_FFPLAY_STEP*1000;
            SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_B);

            break;

        case AP_KEY_NEXT | AP_KEY_UP: //快进
        case AP_KEY_PREV | AP_KEY_UP: //快退
        #if defined(__AT_MOD_COMMON__)	
	 case AT_COMMON_FORE_PLAY:
	 case AT_COMMON_BACK_PLAY:
	 #endif
            if(ffflag!=0)
            {
                INT32 progress = g_rec_time * 10000 / g_totalTime;
                if(progress > 9990)
                {
                    Record_PlayStatus_SelectFile(TRUE);
                    Record_PlayStaus_OpenFile();
                    progress = 0;
                }

                mediaSendCommand(MC_PLAY, progress);
                g_rec_status = playing;

                ffflag = 0;
            }
            break;

        case AP_MSG_RTC:
            if(g_rec_status == playing)
            {
                //media_status_t status_buf;
                mediaSendCommand(MC_GETTIME, (INT32)&g_rec_time);
                SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_B);
            }

            break;

        default:
            return RECORD_CommonKeyHandler( key);
    }//switch()

    return FALSE;
}
BOOL Record_RecStatusKeyHandler(UINT32 key)
{
    static UINT8 low_power_count=0;        //获得低电压的次数

    switch( key )
    {
        //overwrite auto standby
        case AP_MSG_STANDBY:
        case AP_MSG_LOCK:
            //do nothing
            break;

        case AP_KEY_PLAY | AP_KEY_PRESS:
        case AP_KEY_REC | AP_KEY_PRESS:
            if(g_rec_status == recording)
            {
                mediaSendCommand(MC_PAUSE, 0);
                g_rec_status = paused;
            }
            else if(g_rec_status == paused)
            {
                mediaSendCommand(MC_RECORD, 0);
                g_rec_status = recording;
            }
            SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_A);
            break;

        case AP_KEY_PLAY | AP_KEY_HOLD:
            //record end and start new file
            if(MESSAGE_IsHold())
            {
                RECORD_EnterStatus(RECORD_RECORD);
            }
            break;

        case AP_KEY_NEXT | AP_KEY_PRESS:
        case AP_KEY_PREV | AP_KEY_PRESS:
	#if defined(__AT_MOD_COMMON__)	
	case AT_COMMON_NEXT:
	 case AT_COMMON_PREV:
	 #endif
	 #if defined(__AT_MOD_RECORD__)
	 case AT_RECORD_PAUSE:
	 #endif
	     hal_HstSendEvent(APP_EVENT,0x11223344);
            RECORD_EnterStatus(RECORD_STOP);
            break;

        case AP_MSG_RTC:
            if(RECORD_CheckLowBatt())
                low_power_count++;
            else
                low_power_count = 0;

            //检测低电压的次数
            if(low_power_count > LOW_POWER_COUNT)
            {
                low_power_count = 0;
                GUI_DisplayMessage(0, GUI_STR_MPWROUT, NULL, GUI_MSG_FLAG_DISPLAY);
                RECORD_EnterStatus(RECORD_STOP);
            }
            else
            {
                if(g_rec_status == recording)
                {
                    mediaSendCommand(MC_GETTIME, (INT32)&g_rec_time);
                    //RECORD_TimeDisplay();
                }
                SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_B);
            }
            break;

        default:
            return RECORD_CommonKeyHandler( key);
    }

    return TRUE;
}
BOOL Record_StopStatusKeyHandler(UINT32 key)
{
    switch( key )
    {
        case AP_MSG_RTC:
        {
            date_t date;
            ap_time_t time;

            TM_GetDateAndTime(&date, &time);
            g_file_date = ((date.year)<<9) + date.month*32 +date.day;
            g_file_time = time.hour *3600 + time.minute * 60 + time.second;
            SendAppEvent(EV_UI_FW_REDRAW, RECORD_REDRAW_A);
        }
        break;
#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
       case AP_KEY_PLAY | AP_KEY_PRESS:
       case AP_KEY_REC |AP_KEY_PRESS:
	      #if defined(__AT_MOD_RECORD__)
	      case AT_RECORD_START:
	      #endif
            RECORD_EnterStatus(RECORD_RECORD);
            break;

#else
        case AP_KEY_PLAY | AP_KEY_HOLD:
        case AP_KEY_REC |AP_KEY_PRESS:
	#if defined(__AT_MOD_RECORD__)
	 case AT_RECORD_START:
	 #endif
            RECORD_EnterStatus(RECORD_RECORD);
            break;

        case AP_KEY_PLAY | AP_KEY_PRESS:
            if(g_file_num > 0)
            {
                RECORD_EnterStatus(RECORD_PALY);
            }
            break;
#endif

        case AP_KEY_NEXT | AP_KEY_PRESS:
	#if defined(__AT_MOD_COMMON__)	
	case AT_COMMON_NEXT:
	 #endif
            if(g_file_num > 0)
            {
                Record_PlayStatus_SelectFile(TRUE);
                RECORD_EnterStatus(RECORD_PALY);
            }
            break;

        case AP_KEY_PREV | AP_KEY_PRESS:
	#if defined(__AT_MOD_COMMON__)	
	case AT_COMMON_PREV:
	 #endif		
            if(g_file_num > 0)
            {
                Record_PlayStatus_SelectFile(FALSE);
                RECORD_EnterStatus(RECORD_PALY);
            }
            break;

        default:
            return RECORD_CommonKeyHandler( key);
    }//switch(key);

    return TRUE;
}



/******************************************************************************
 * RecordMainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL RecordMainMsgHandler(COS_EVENT *pEvent)
{
    UINT32 key;
    BOOL ret = TRUE;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
			SetProtocolEventHandler(RecordHandleTFalshCardPlugInOut, EV_FS_TFLASH_DETECT);
#if (APP_SUPPORT_USB)
			SetProtocolEventHandler(RecordHandleUSBPlugOut, AP_MSG_USB_PULLOUT);
#endif
            break;

        case EV_UI_FW_ON_RESUME:
            RECORD_Display(RECORD_REDRAW_A | RECORD_REDRAW_B);
            break;

        case EV_UI_FW_REDRAW:
            if(0 == pEvent->nParam1)
            {
                pEvent->nParam1 = RECORD_REDRAW_A | RECORD_REDRAW_B;
            }
            app_trace(APP_RECORD_TRC,"Record redraw=0x%4x",pEvent->nParam1);
            RECORD_Display(pEvent->nParam1);
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case EV_UI_FW_ON_EXIT:
			ClearProtocolEventHandler(EV_FS_TFLASH_DETECT);
#if (APP_SUPPORT_USB)
			ClearProtocolEventHandler(AP_MSG_USB_PULLOUT);
#endif
            RECORD_EnterStatus(RECORD_STOP);
            NVRAMWriteData();
#if APP_SUPPORT_FM_RECORD
            if(g_rec_from == R_FM)
                AppFMExternalDo(MC_CLOSE, 0)
#endif
            SetPAVolume(0);
            break;

        default:
            //key event
            key = MESSAGE_ConvertToKeyValue(pEvent);
            switch(g_record_screen_status)
            {
                case RECORD_PALY:
                    ret = Record_PlayStatusKeyHandler(key);
                    break;

                case RECORD_STOP:
                    ret = Record_StopStatusKeyHandler(key);
                    break;

                case RECORD_RECORD:
                    ret = Record_RecStatusKeyHandler(key);
                    break;

                default:
                    //Invaild status
                    APP_ASSERT(0);
                    break;
            }
            break;
    }

    return ret;

}


INT32 RECORD_Stop(INT32 param)
{
    hal_HstSendEvent(APP_EVENT,0x66666601);
    RECORD_EnterStatus(RECORD_STOP);
    hal_HstSendEvent(APP_EVENT,0x66666602);
    return 0;
}

/*********************************************************************************
* Description : 主函数
*
* Arguments   :
        param:
            0, 不自动开始录音
            1, 自动开始录音
*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
INT32 RECORD_Entry(INT32 param)
{

    INT16 error = 0;
    
	app_trace(APP_RECORD_TRC, "====RECORD_Entry====");

    MESSAGE_Initial(g_comval);

    GUI_DisplayMessage(0, GUI_STR_RINIT, NULL, GUI_MSG_FLAG_DISPLAY);
    error = Record_Init(param);
    if(0 != error)
    {
        app_trace(APP_RECORD_TRC, "Record_Init Error_id=%d",error);
        GUI_DisplayMessageWithCB(0,error, NULL, AppGotoMainMenu, GUI_MSG_FLAG_WAIT);
        return 1;
    }
    else
    {
        app_trace(APP_RECORD_TRC, "Record_Init OK");
        EntryNewScreen(SCR_ID_RECORD_MAIN, RecordMainMsgHandler,NULL, 0, 0);

        if((MICREC_START == param) || (FMREC_START == param))
            RECORD_EnterStatus(RECORD_RECORD);
        else
            RECORD_EnterStatus(RECORD_STOP);
		//update by wangxin record

        return 0;
    }

    return 0;
}
#ifdef APP_RECORD_VOICE_INRAM

extern INT32  g_audYocPos; 
extern INT32 g_audYocReadPos; 
extern INT8* g_audYoc; 
INT32 record_read(VOID *buf, INT32 len)
{
    if (NULL == buf) return -1; 
    if (0x32000 < g_audYocReadPos + len) return -1; 

    memcpy(buf, g_audYoc + g_audYocReadPos, len);
    g_audYocReadPos += len; 
    
    return 0;
}

/* 
 * status: RECORD_STOP and RECORD_RECORD
 */


INT32 record_op(INT32 status)
{
    UINT8 old_status ; //= g_record_screen_status;
    UINT8 new_status = status;
    INT16 error = 0;
    media_status_t rec_stat;

    old_status = (status == RECORD_STOP) ? RECORD_RECORD: RECORD_STOP; 

    app_trace(APP_RECORD_TRC,"RECORD_EnterStatus:%d, old=%d",new_status, old_status);

    switch(old_status)
    {
        case RECORD_STOP:
            break;

        case RECORD_RECORD:

            ClearProtocolEventHandler(EV_AUDIO_RECORD_FINISH_IND);
            mediaSendCommand(MC_STOP, 0);
            mediaSendCommand(MC_GETSTATUS, (int)&rec_stat);
            //check status for codec stop
            if(rec_stat.status == RECORDING_STOP)
            {
                app_trace(APP_RECORD_TRC,"RECORD save file OK");
                GUI_DisplayMessage(0,GUI_STR_RECORDOK, NULL, GUI_MSG_FLAG_DISPLAY);
            }
            else
            {
                app_trace(APP_RECORD_TRC,"RECORD save file Fail!");
            }

            mediaSendCommand(MC_CLOSE, 0);
            break;

        case RECORD_PALY:
            ClearProtocolEventHandler(EV_AUDIO_PLAY_FINISH_IND);
            mediaSendCommand(MC_STOP, 0);
            mediaSendCommand(MC_CLOSE, 0);
            break;

        default:
            break;
    }

    g_rec_time = 0;

    switch(new_status)
    {
        case RECORD_STOP:
            g_rec_status = stop;
            break;

        case RECORD_RECORD:
        {
            Open_param_t param;
            
            g_audYocReadPos = 0; 
            g_audYocPos = 0; 

            //防止在录音过程中切换bank
            g_rec_status = prepareing;
            strcpy(g_record_vars->location.file_name, "null");

            param.file_name = g_record_vars->location.file_name;
            param.mode = MEDIA_RECORD;
            param.type = Music_WAV;
            //open codec
            error = mediaSendCommand(MC_OPEN, &param);
            if(!error)
            {
                app_trace(APP_RECORD_TRC, "open record file error ");
                GUI_DisplayMessage(0,GUI_STR_RECORDERROR, NULL, GUI_MSG_FLAG_WAIT);
                return 0;
            }

            error = mediaSendCommand(MC_RECORD, 0);
            if(!error)
            {
                app_trace(APP_RECORD_TRC, "start record error");
                GUI_DisplayMessage(0,GUI_STR_RECORDERROR, NULL, GUI_MSG_FLAG_WAIT);
                return 0;
            }
            
            SetProtocolEventHandler(Record_recording_callback_hdlr, EV_AUDIO_RECORD_FINISH_IND);
            g_rec_status = recording;

            app_trace(APP_RECORD_TRC, "start record ... ");
        }
        break;

        default:
            break;

    }


    g_record_screen_status = new_status;

}

#endif

#endif
