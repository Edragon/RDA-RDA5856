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


#include "ap_music.h"
#include "ap_media.h"
#include "ap_gui.h"
#include "event.h"
#include "hal_host.h"
#ifndef WIN32
#include "hal_sys.h"
#else
#include "fs.h"
#endif

extern INT8 g_play_disk;
extern INT8 g_next_play_disk;
extern UINT8 fselPLOrCommonDir;

#if APP_SUPPORT_MUSIC==1

//外部变量，在每次进入一个人模块的时候都将被重新初始化
extern slider_t              slider;
extern BOOL                  ui_auto_update;
extern INT8 g_timer_setting_flag;
#ifndef WIN32
extern UINT32 usb_poweron_det;
#endif

#define TO_PROGRESS(time)  ((((UINT64)time) * MAX_PROGRESS_CNT)/g_music_gui_ptr->TotalTime)
#define TO_TIME(progress)  ((progress) * g_music_gui_ptr->TotalTime/MAX_PROGRESS_CNT)

music_vars_t *g_music_vars = NULL;//本AP的公共变量
music_screen_gui_t *g_music_gui_ptr =NULL;
//static FOLLOW_STATUS_INFO_PTR s_follow_info_ptr = NULL;
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
UINT32 play_Stop_Key_Status=0;
#endif
#define UPDATE_TFLASH_CFG_NAME  L"update.cfg"

#if APP_SUPPORT_CHANGE_FOLDER == 1
static UINT8 changeFolderStatus = 0;    //long press PREV,NEXT to change to PREV or NEXT folder
#endif

extern VOID DSM_TFlash_Upgrade(INT32 disk, INT32 file);
extern U16 GetCurrentScreenId(void);
extern PUBLIC void hal_SysSetVocClockDefault(void);
extern INT32 DSM_DeactiveDevice(PCSTR pszDevName);
extern void hal_DbgAssert(const char* format, ...);

/***********************************************************************************************************************
    ==FLASH 与卡的 报错处理
设 A 表示有无格式化属性 (+A:已格；-A:未格)
   B 表示有无文件属性   (+B:有文件；-B:无文件)
   X 表示无意义(如在盘未格式化时谈有无文件就无意义)

   C 表示FLASH
   H 表示SD卡

下面用组合列出所有情况：
1) C[+A+B],H[+A+B] //两个盘都已格式化，都有文件                    //不报任何错，而直接恢复退出时的断点即可
2) C[+A+B],H[+A-B] //两个盘都已格式化，FLASH有文件，卡无文件       //直接跳到FLASH中即可
3) C[+A+B],H[-AX]  //FLASH已格式化，卡没格式化，FLASH有文件        //先报"插卡错误",再跳到FLASH中

4) C[+A-B],H[+A+B] //两个盘都已格式化，FLASH无文件，卡有文件       //直接跳到卡中
5) C[+A-B],H[+A-B] //两个盘都已格式化，两个盘都无文件              //先报"磁盘没文件",再报"插卡没文件",然后退出
6) C[+A-B],H[-AX]  //FLASH已格式化，卡没格式化，FLASH无文件        //先报"插卡错误",再报"磁盘没文件",然后退出

7) C[-AX],H[+A+B]  //FLASH没格式化，卡已格式化，卡中有文件         //先报"磁盘错误",然后跳到卡中
8) C[-AX],H[+A-B]  //FLASH没格式化，卡已格式化，卡中无文件         //先报"磁盘错误",再报"卡没文件",然后退出
9) C[-AX],H[-AX]   //两个盘都没格式化                              //先报"磁盘错误" ，再报“插卡错误”,然后退出
***********************************************************************************************************************/



/******************************************************************************
** 名字:  文件选择器初始化及报错显示
** 接口:  INT32 InitFileSelector(void)
** 描述:  把文件选择器初始化到有效的盘上，如找不到，则报错退出
** 输入参数: void
** 输出参数: 0: 已完成初始化文件选择器动作
             否则: 没有有效的盘或盘上无文件，初始化失败，要求退出AP
** 使用说明:
********************************************************************************/
static INT16 InitFileSelector(void)
{
    //INT16 ret = 0;
    FS_DEV_TYPE DevType;
    UINT8       FsType;

    FS_GetRootType(&DevType, &FsType);
    if(g_play_disk != DevType)
    {
        return GUI_STR_MREADERR;//非法，
    }

    if((DevType != FS_DEV_TYPE_INVALID) && ((FsType == FS_TYPE_FAT) || (FsType == FS_TYPE_EXFAT)))
    {
        //g_play_disk = DevType;
        //hal_HstSendEvent(SYS_EVENT, 0x16091202);
        //hal_HstSendEvent(SYS_EVENT, g_play_disk);
#ifdef MULTI_DIR_TRAVERSAL_SUPPORT
        if (fselInit(FSEL_TYPE_MUSIC, FSEL_ALL_SEQUENCE, FSEL_TYPE_NEW_PLAYLIST, g_play_disk))
#else
        if (fselInit(FSEL_TYPE_MUSIC, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, g_play_disk))
#endif
        {
            return 0;
        }

        if (fselGetErrID() == 0)
        {
            return GUI_STR_MREADERR;//非法，
        }

        /*显示卡文件状况*/
        if(g_music_gui_ptr->musicamount==0)
        {
            g_music_gui_ptr->DiskStateFlag |= SetDiskNoFile_def;//标志磁盘无文件
            //  卡无文件
            return GUI_STR_CFILEOUT;
        }
    }

    return GUI_STR_MREADERR;//要退出 ap
}

#ifdef MULTI_DIR_TRAVERSAL_SUPPORT
INT32 MUSIC_DeleteFile(UINT16 num)
{
    mediaSendCommand(MC_STOP, 0);
    if(mediaSendCommand(MC_CLOSE, 0))
    {
        app_trace(APP_MUSIC_TRC, "delete file start!");
        if(fselDelFileByNo(FSEL_DEL_FILE, num))
        {
            if (UpdateFileSelector() == 0)//根据location设置文件选择器参数
            {
                hal_HstSendEvent(SYS_EVENT, 0x20131011);
                //显示卡无文件
                FS_UnmountRoot();
                return GUI_STR_CFILEOUT;
            }

            //SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_MUSIC);
        }
        else
            app_trace(APP_MUSIC_TRC, "delete file failed!");
    }
    else
    {

    }

    return 0;
}
#endif

/******************************************************************************
 * MUSIC_Init:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT16 MUSIC_Init(void)
{
    INT16 error = 0;

    //MUSIC_GetPara();      //获取音乐播放参数
    //FS_PowerOn();
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    ui_auto_update = TRUE;
#endif

    hal_HstSendEvent(SYS_EVENT, 0x2013100a);
    if (!MountDisk(g_play_disk))
    {
        //显示磁盘错
        return GUI_STR_MREADERR;
    }
#if APP_SUPPORT_LCD==0  //added for T_card update without LCD. Search TF card, if there is a file with the name "update.cfg", then do the update.
    {
        INT32 file = -1;
        file=FS_Open((PCSTR)UPDATE_TFLASH_CFG_NAME, FS_O_RDONLY, 0);
        hal_HstSendEvent(APP_EVENT, file);
        if(file >=0)
        {
            hal_HstSendEvent(APP_EVENT, 0x92ade000);
            hal_HstSendEvent(APP_EVENT, file);
            hal_HstSendEvent(APP_EVENT, g_play_disk);
#ifndef WIN32
            DSM_TFlash_Upgrade(g_play_disk, file);
#endif
        }
    }
#endif

    MUSIC_GetPara();

    hal_HstSendEvent(SYS_EVENT, 0x2013100b);
    g_music_gui_ptr = (music_screen_gui_t *) COS_Malloc_BTOverlay(sizeof(music_screen_gui_t));
	memset(g_music_gui_ptr, 0x0, sizeof(music_screen_gui_t));
	g_music_gui_ptr->screen_status = RESULT_UI_EXIT;
    g_music_gui_ptr->ERROR_EXIT=0;

    g_music_gui_ptr->DiskStateFlag |= SetDiskOKFlag_def;//标志 flash 已格式化

    //初始化变量
    g_music_gui_ptr->DiskStateFlag    = 0;  //磁盘状态字，描述磁盘状态
    g_music_gui_ptr->currentMusicType = Music_NON;
#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
    g_music_gui_ptr->lyricflag        = FALSE; //有无歌词的标示
#endif
    g_music_gui_ptr->musicsequence    = 0x00;
    g_music_gui_ptr->musicamount      = 0x00;

////////////////////////////////////////从上次意外退出的地方播放///////////////////////////////////////
    g_music_gui_ptr->Openparam.file_entry  = g_music_vars->location[g_play_disk].file_entry;//文件名地址
    //hal_HstSendEvent(SYS_EVENT, 0x16091203);
    //hal_HstSendEvent(SYS_EVENT, g_music_vars->location[g_play_disk].file_entry);

    //g_music_gui_ptr->Openparam.BreakPTSave = &g_music_vars->BreakPTSave.breakPT;    //备份参数地址
///////////////////////////////////////////////////////////////////////////////////////////////////////

    g_music_gui_ptr->TotalTime = 0;
    //g_music_vars->CurrentTime = 0;

    g_music_gui_ptr->isFileInfoGot = FALSE;

    //if ( !((g_music_vars->fselmod == FSEL_TYPE_COMMONDIR)||(g_music_vars->fselmod == FSEL_TYPE_PLAYLIST)))
    {
        //  g_music_vars->fselmod = FSEL_TYPE_COMMONDIR;
    }
//////////////////////////////////////////初始化文件播放参数//////////////////////////////////////////
    //初始化文件选择器
    hal_HstSendEvent(SYS_EVENT, 0x2013100c);
    error = InitFileSelector();
    if (0 != error)
    {
        //初始化文件器失败，没有音频文件
        FS_UnmountRoot();
        return error;
    }

    app_trace(APP_MUSIC_TRC, "init file selector finish! fselPLOrCommonDir: %d", fselPLOrCommonDir);
    hal_HstSendEvent(SYS_EVENT, 0x2013100d);


    hal_HstSendEvent(SYS_EVENT, 0x20131010);
    do
    {
        if (UpdateFileSelector() == 0)//根据location设置文件选择器参数
        {
            hal_HstSendEvent(SYS_EVENT, 0x20131011);
            //显示卡无文件
            FS_UnmountRoot();
            return GUI_STR_CFILEOUT;
        }
    }
    while(0);
    hal_HstSendEvent(SYS_EVENT, 0x20131012);
    g_music_gui_ptr->musicamount   = fselGetTotal(); //当前歌曲目录总数
    g_music_gui_ptr->musicsequence = fselGetNo();    //获得当前歌曲编号

    fselSetMode(g_music_vars->repeat_mode); //循环模式

    return 0;
}

static void MUSIC_DeInit(void)
{
    MUSIC_EnterStatus(RESULT_UI_EXIT);

#if APP_SUPPORT_FM_OUT
    FM_OutPut(0xfe, NULL, NULL);  // close fm
#endif

    NVRAMWriteData();

    if(g_play_disk == FS_DEV_TYPE_TFLASH)
    {
        TCHAR *dev_name=TEXT("mmc0");
        INT32 iRet = DSM_DeactiveDevice(dev_name);
        if( ERR_SUCCESS != iRet )
        {
            hal_HstSendEvent(SYS_EVENT, 0xbbc01000);
        }
    }

#if APP_SUPPORT_FOLLOW_COMPARE==1
    //删除跟读时可能产生的临时文件,在正常退出时保证这个文件能被删除
    FS_Delete( FollowReadTempLongName );
#endif

    if(NULL != g_music_gui_ptr)
    {
        COS_Free_BTOverlay(g_music_gui_ptr);
        g_music_gui_ptr = NULL;
    }
}

/*********************************************************************************
* Description : 音量设置回调函数
*
* Arguments   :
*
* Notes       :
*
*********************************************************************************/
void MUSIC_VolumeCb( INT32 Vol )
{
    if(g_music_gui_ptr->PlayStatus != MusicStop)
        mediaSendCommand(MC_SETVOLUME, Vol);
    g_music_vars->volume = Vol;
    g_comval->Volume = Vol;
}

static void MUSIC_SwitchToNextMode(void)
{
    //g_play_disk = FS_DEV_TYPE_TFLASH;
    AppGotoMainMenu();
}

#if APP_SUPPORT_MENU==1
void MUSIC_SetRepeatPeroidCb( INT32 value )
{
    ui_auto_update = TRUE;
    g_music_vars->maxReplayTime = (UINT8) value;

}

void MUSIC_SetRepeatTimesCb( INT32 value )
{
    ui_auto_update = TRUE;
    g_music_vars->replaytimes = (UINT8) value;
}

void MUSIC_SelectFileCb(file_location_t *f, UINT16 result)
{
    if(LIST_CALLBACK_SELECT_ITEM == result)
    {
        //hal_HstSendEvent(SYS_EVENT, 0x16091205);
        //hal_HstSendEvent(SYS_EVENT, g_play_disk);

        g_music_vars->location[g_play_disk] = *f;

        UpdateFileSelector();
        MUSIC_SelectAudio(Music_Cur, Stop_key);

        MUSIC_EnterStatus(RESULT_UI_PLAY);
    }
}

INT32 Delete_file(void)
{
    INT32 ret = 0;
    char curr_file_name[128] = {0};

    mediaSendCommand(MC_STOP, 0);

    if(mediaSendCommand(MC_CLOSE, 0))
    {
        FS_GetFileName(g_music_vars->location[g_play_disk].file_entry, sizeof(curr_file_name), curr_file_name);

        ret = FS_Delete(curr_file_name);

        app_trace(APP_MUSIC_TRC, "Delete_file:%s, ret:%d",curr_file_name, ret);

        if(0 == ret)
        {
            SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_MUSIC);
        }
    }
    else
    {

    }
    return 0;
}

#endif

INT32 music_menu_callback(INT32 type, INT32 value, INT32 param, UINT8 **string)
{
    if(MENU_CALLBACK_QUERY_ACTIVE == type)
    {
        if(value == GUI_MENU_MUSIC_EQ)
            return g_music_vars->eq_type;
        else if(value == GUI_MENU_MUSIC_LOOP)
            return g_music_vars->repeat_mode;
        return 0;
    }

#if APP_SUPPORT_LCD
    if(g_timer_setting_flag==1)
    {
        if(value == MUSIC_SELECT_FILE)
            return g_music_gui_ptr->PlayStatus == MusicStop;
        else
            return FALSE;
    }
#endif

    switch(value)
    {
        case MUSIC_START_REPEAT:
            return g_music_gui_ptr->PlayStatus == NormalPlay;
        case MUSIC_SET_FOLLOW:
            return g_music_gui_ptr->PlayStatus == LoopPlay;
        case MUSIC_SELECT_FILE:
            return TRUE;//g_music_gui_ptr->PlayStatus == MusicStop;
#if APP_SUPPORT_FM_OUT
        case MUSIC_OPEN_FMOUT:
            return !g_music_vars->fm_out;
        case MUSIC_CLOSE_FMOUT:
            return g_music_vars->fm_out;
#endif
#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
        case MUSIC_OPEN_LYRIC:
            return !g_music_vars->show_lyric;
        case MUSIC_CLOSE_LYRIC:
            return g_music_vars->show_lyric;
#endif
        default:
            break;
    }
    return 0;
}

/*********************************************************************************
*             void MUSIC_ClearBreakTimePara(void)
*
* Description : 清除MP3播放断点保存信息
*
* Arguments   : void
*
* Returns     : void
*
* Notes       :
*
*********************************************************************************/
void MUSIC_ClearBreakTimePara(void)
{
    //g_music_vars->MusicOpenMod           = Nor_open;
    //g_music_vars->BreakPTSave.breakPT    = 0; //开始断点为0
    // g_music_vars->CurrentTime = 0;
//    g_music_vars->CurrentTime.hour    = 0;
//    g_music_vars->CurrentTime.minute  = 0;
//    g_music_vars->CurrentTime.second  = 0;
    // g_music_vars->BreakPTSave.ApointSave = 0; //开始断点为0
    // g_music_vars->BreakPTSave.BpointSave = 0; //开始断点为0
    g_music_vars->CurrentTime[g_play_disk] = 0;
    //g_music_vars->CurrentTime = 0;
    StartIntro(TRUE)//因为时间清0了，所以浏览要从头开始
}

/*********************************************************************************
*             void MUSIC_GetPara(void)
*
* Description : 读系统变量和ap局部变量
*
* Arguments   : void
*
* Returns     : void
*
* Notes       :
*
*********************************************************************************/
void MUSIC_GetPara( void )
{
    //kval_t kval;
    UINT8 i = 0;

    //读出是否刚上电，如是，则要重调时间。
    //NVRAMRead(&kval, VM_KERNEL, sizeof(kval_t));

    //g_PowerOffFlag = kval.rtcresetflag;

    //NVRAMRead(& g_music_vars, VM_AP_MUSIC, sizeof(g_music_vars));
    g_music_vars = (music_vars_t*)NVRAMGetData(VM_AP_MUSIC,sizeof(music_vars_t));

    //check if valid
    if (g_music_vars->magic != MAGIC_MUSIC)
    {
        g_music_vars->magic       = MAGIC_MUSIC;
        g_music_vars->volume      = MUSIC_MAX_VOLUME/2;
        g_music_vars->eq_type     = g_comval->music_cfg.eq_mode;
        g_music_vars->replaytimes = g_comval->music_cfg.repeat_count;
        g_music_vars->maxReplayTime = g_comval->music_cfg.repeat_time;
        //g_music_vars->replaygap   = 5;
        if(g_comval->music_cfg.loop_mode == 1)
            g_music_vars->repeat_mode = FSEL_ALL_REPEATONE;
        else
            g_music_vars->repeat_mode = FSEL_ALL_REPEATALL;
        //g_music_vars->play_rate   = 0;
        //g_play_disk = FS_DEV_TYPE_TFLASH;
        //g_music_vars->play_listmod  = mlistnor;
        //g_music_vars->fselmod       = FSEL_TYPE_COMMONDIR;
        /*g_music_vars->MusicOpenMod  = Nor_open;
        g_music_vars->Wow  =0;
        g_music_vars->Bass =0;
        g_music_vars->Srs  =0; */
        g_music_vars->Lyric_Size = 16;
        g_music_vars->show_lyric = 1;
#if APP_SUPPORT_FM_OUT
        g_music_vars->fm_out = 0;
        g_music_vars->freq  = g_comval->fm_value.fm_sendfreq*100;
#endif
        for (i=0; i<FS_DEV_TYPE_MAX; i++)
        {
            g_music_vars->location[i].subdir_name[0] = 0;
            g_music_vars->location[i].subdir_entry = 0;
            g_music_vars->location[i].file_name[0] = 0;
            g_music_vars->location[i].file_entry = 0;
        }
        //hal_HstSendEvent(SYS_EVENT, 0x16091206);

        MUSIC_ClearBreakTimePara();
    }

    g_music_vars->location[FS_DEV_TYPE_TFLASH].disk = FS_DEV_TYPE_TFLASH;
    g_music_vars->location[FS_DEV_TYPE_USBDISK].disk = FS_DEV_TYPE_USBDISK;

#if APP_SUPPORT_INDEPEND_VOLUMNE==0
    g_music_vars->volume = g_comval->Volume;
#endif

}


//============改变音量================
INT32 ChangeVolume( UINT32 key )
{
    BOOL play_tone = FALSE;

#if (APP_SUPPORT_RGBLCD==1) ||(APP_SUPPORT_LCD==1)
    if(AP_Support_LCD() && g_music_gui_ptr->PlayStatus <= NormalPlay) // dont use slider in follow mode
    {
        //show background
        GUI_ClearScreen(NULL);
        GUI_ResShowPic(GUI_IMG_VOLUMEBK,0,0);

        slider.min = 0;
        slider.max = MUSIC_MAX_VOLUME;
        slider.value = g_music_vars->volume;
        slider.step = 1;

        ui_auto_update = FALSE;
        GUI_Slider(&slider, MUSIC_VolumeCb);
        ui_auto_update = TRUE;

        StartIntro(FALSE);
    }
    else
#endif
    {
        if(((key&0xffff0000) == AP_KEY_VOLADD) ||(key == AT_COMMON_VOLADD))
        {
            g_music_vars->volume++;
            if(g_music_vars->volume > MUSIC_MAX_VOLUME)
            {
                g_music_vars->volume = MUSIC_MAX_VOLUME;
                play_tone = TRUE;
            }
        }
        if(((key&0xffff0000) == AP_KEY_VOLSUB) ||(key == AT_COMMON_VOLSUB))
        {
            g_music_vars->volume--;
            if(g_music_vars->volume < 0)
            {
                g_music_vars->volume = 0;
                play_tone = TRUE;
            }
        }

        if(g_music_gui_ptr->PlayStatus != MusicStop)
            mediaSendCommand(MC_SETVOLUME, g_music_vars->volume);
        g_comval->Volume = g_music_vars->volume;


        if(play_tone)
            GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_VOL_MAXORMIN, GUI_MSG_FLAG_WAIT);
    }
    return 0;
}

/*********************************************************************************
*             Music_type_t CheckMusicType(INT8 *filename)
*
* Description : 检测当前是哪种类型的MUSIC文件
*
* Arguments   : INT8 *filename  文件名
*
* Returns     : Music_type_t 文件类型
*
* Notes       :
*
*********************************************************************************/
Music_type_t MUSIC_CheckType( TCHAR * filename )
{
    INT32 i;

    const TCHAR *const ExtMusicstr [] =
    {
        "MP3", "WMA", "ASF", "WAV", "AAC","SBC"
#if 0
        ,"VM","AU","SND","PCM","AMR","AWB","AIF","AIFF","AIFC"
#endif
    };
    /*小写转换为大写*/
    for (i = 8; i < 11; i++)
    {
        if (filename[i] >= 97 /* 'a' */  && filename[i] <= 122 /* 'z' */ )
        {
            filename[i] -= 32 /* ('a'-'A') */ ;
        }
    }

    if (strcmp(filename+8, ExtMusicstr[0]) == 0)
    {
        return Music_MP3;
    }

    else if (strcmp(filename+8, ExtMusicstr[3]) == 0)
    {
        return Music_WAV;
    }
    else if (strcmp(filename+8, ExtMusicstr[5]) == 0)
    {
        return Music_SBC;
    }
#ifndef TARGET_AAC_SUPPORT_OFF
    else if (strcmp(filename+8, ExtMusicstr[4]) == 0)
    {
        return Music_AAC;
    }
#endif
#ifndef TARGET_WMA_SUPPORT_OFF
    else if (strcmp(filename+8, ExtMusicstr[1]) == 0)
    {
        return Music_WMA;
    }
#endif
#if 0

    else if (strcmp(filename+8, ExtMusicstr[4]) == 0)
    {
        return Music_OGG;
    }
    else if (strcmp(filename+8, ExtMusicstr[2]) == 0)
    {
        return Music_ASF;
    }
    else if (strcmp(filename+8, ExtMusicstr[5]) == 0)
    {
        return Music_VM;
    }
    else if ((strcmp(filename+8, ExtMusicstr[6]) == 0)||(strcmp(filename+8,    ExtMusicstr[7]) ==   0))
    {
        return Music_AU;
    }
    else if (strcmp(filename+8, ExtMusicstr[8]) == 0)
    {
        return Music_PCM;
    }
    else if( (strcmp(filename+8, ExtMusicstr[9]) == 0) ||(strcmp(filename+8, ExtMusicstr[10]) == 0))
    {
        return Music_AMR;
    }
    else if ((strcmp(filename+8, ExtMusicstr[11]) == 0)||(strcmp(filename+8,ExtMusicstr[12]) ==0)||(strcmp(filename+8, ExtMusicstr[13]) ==0))
    {
        return Music_AIFF;
    }
#endif
    else
    {
        return Music_NON;
    }
}



/*********************************************************************************
*             INT32 MUSIC_GetInformation(void)
*
* Description : 读取歌曲信息,歌词
*
* Arguments   : void
*
* Returns     : INT32
*
* Notes       :
*
*********************************************************************************/
BOOL MUSIC_GetInformation( void )
{
    INT32          result;
    audInfoStruct  *pAudioinfo;
    //ap_time_t      total_time;

    hal_HstSendEvent(SYS_EVENT,0x09220010);
    //hal_HstSendEvent(SYS_EVENT, 0x16091207);
    //hal_HstSendEvent(SYS_EVENT, g_play_disk);

    g_music_gui_ptr->currentMusicType = MUSIC_CheckType(g_music_vars->location[g_play_disk].file_name);

    g_music_gui_ptr->Openparam.type      = g_music_gui_ptr->currentMusicType;
    g_music_gui_ptr->Openparam.mode      = MEDIA_PLAY;
    g_music_gui_ptr->Openparam.file_entry = g_music_vars->location[g_play_disk].file_entry;

    result = mediaSendCommand(MC_OPEN,(INT32)&(g_music_gui_ptr->Openparam));
    if(!result)
        return FALSE;

    g_music_gui_ptr->FFPlay_CNT = FALSE;
    if (!g_music_gui_ptr->isFileInfoGot)
    {
#if (APP_SUPPORT_RGBLCD==1) ||(APP_SUPPORT_LCD==1)
        //memset(&g_curaudioinfo, 0, sizeof(g_curaudioinfo));/* get id3 tags info */
        if(AP_Support_LCD())
        {
            pAudioinfo = COS_Malloc_BTOverlay(sizeof(audInfoStruct));
            //g_music_gui_ptr->TagInfoBuf[0] = '\0';
            memset(g_music_gui_ptr->TagInfoBuf, 0, MUSIC_MAX_TAGINFO);
            //if(Audio_GetDetailsInfo(g_music_vars->location.file_entry,&g_curaudioinfo,g_music_gui_ptr->currentMusicType,g_comval->langid))
            if(mediaSendCommand(MC_GETFILEINFO, pAudioinfo))
            {
#if 1
                INT32 sec;
                if(pAudioinfo->title[0] != 0)
                {
                    strncpy(g_music_gui_ptr->TagInfoBuf, pAudioinfo->title, 30);
                    //hal_DbgAssert("g_music_gui_ptr->TagInfoBuf:%s",pAudioinfo->title);
                    strcat(g_music_gui_ptr->TagInfoBuf, " ");
                    strncat(g_music_gui_ptr->TagInfoBuf, pAudioinfo->artist, 30);
                    strcat(g_music_gui_ptr->TagInfoBuf, " ");
                    strncat(g_music_gui_ptr->TagInfoBuf, pAudioinfo->album, 30);
                    strcat(g_music_gui_ptr->TagInfoBuf, " ");
                    strncat(g_music_gui_ptr->TagInfoBuf, pAudioinfo->author, 30);
                    strcat(g_music_gui_ptr->TagInfoBuf, "  ");
#endif
                }
                //        memcpy(g_music_vars->location.artist,pAudioinfo->artist,MAX_FILE_LEN);

                g_music_gui_ptr->BitRate = pAudioinfo->bitRate/1000;
                g_music_gui_ptr->TotalTime =  pAudioinfo->time;
            }

            COS_Free_BTOverlay(pAudioinfo);
            //---处理没有TAG信息的情况
            if (g_music_gui_ptr->TagInfoBuf[0] == 0)
            {
                //hal_HstSendEvent(SYS_EVENT, 0x16091208);
                //hal_HstSendEvent(SYS_EVENT, g_play_disk);

                FS_GetEntryName(g_music_vars->location[g_play_disk].file_entry,MUSIC_MAX_TAGINFO,g_music_gui_ptr->TagInfoBuf);
                //gui_unicode2local(g_music_gui_ptr->TagInfoBuf, MUSIC_MAX_TAGINFO-2);
                app_trace(APP_MUSIC_TRC, "FileName:%s",g_music_gui_ptr->TagInfoBuf);
            }

            //---在TAG信息后加个空格避免滚屏时串在一起
            strcat(g_music_gui_ptr->TagInfoBuf, " ");
            hal_HstSendEvent(SYS_EVENT,0x09220025);

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
            if(g_music_vars->show_lyric)
            {
                //hal_HstSendEvent(SYS_EVENT, 0x16091209);
                //hal_HstSendEvent(SYS_EVENT, g_play_disk);

                g_music_gui_ptr->lyricflag =lyricLoad(g_music_vars->location[g_play_disk].file_entry);
                if(g_music_gui_ptr->lyricflag)
                {
                    g_music_gui_ptr->lyric_next_time = 0;
                    memset(g_music_gui_ptr->lyric_buff, 0, AUDPLY_LYRIC_MAX_LINE_SIZE);
                }
            }
#endif //_LRC_SUPPORT_
        }
        else
#endif /*(APP_SUPPORT_RGBLCD==1) ||(APP_SUPPORT_LCD==1)*/
        {
#if defined(__AT_MOD_MUSIC__)
            FS_GetEntryName(g_music_vars->location[g_play_disk].file_entry,MUSIC_MAX_TAGINFO,g_music_gui_ptr->TagInfoBuf);
            gui_unicode2local(g_music_gui_ptr->TagInfoBuf, MUSIC_MAX_TAGINFO-2);
            app_trace(APP_MUSIC_TRC, "FileName:%s",g_music_gui_ptr->TagInfoBuf);

            strcat(g_music_gui_ptr->TagInfoBuf, " ");
#endif
            mediaSendCommand(MC_GETTOTALTIME, (INT32)&(g_music_gui_ptr->TotalTime));
        }
        g_music_gui_ptr->isFileInfoGot = TRUE;
    }

    //NVRAMWrite(&g_music_vars, VM_AP_MUSIC,sizeof(g_music_vars));
    NVRAMWriteData();
    hal_HstSendEvent(SYS_EVENT,0x09220030);

    return TRUE;
}

/******************************************************************************************
input: void

output:   g_result == NULL: 退出到STOP
               g_result == 其他，按热键处理

******************************************************************************************/
static void HandlePlugandPull(UINT32 key)
{
    hal_HstSendEvent(SYS_EVENT,0x25000000 + key);

    if(key == AP_MSG_USB_PULLOUT && g_play_disk != FS_DEV_TYPE_USBDISK)
        return;
    if(key == AP_MSG_SD_OUT&& g_play_disk != FS_DEV_TYPE_TFLASH)
        return;

    if(key == AP_MSG_SD_IN)
    {
        g_next_play_disk = FS_DEV_TYPE_TFLASH;
    }
    else if( (key == AP_MSG_SD_OUT) || (AP_MSG_USB_PLUGIN) )
    {
        g_next_play_disk = FS_DEV_TYPE_USBDISK;
    }
    else
    {
        g_next_play_disk = FS_DEV_TYPE_INVALID;
    }

    //To clear all event handler that registed in music
    DeleteScreenIfPresent(SCR_ID_MUSIC_MAIN);
    if(key == AP_MSG_USB_PULLOUT)
    {
        SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_MAIN);
    }
    else
    {
        SendAppEvent(EV_UI_FW_SWITCH_MOD, RESULT_MUSIC);
    }
}

#if (APP_SUPPORT_USB)
/******************************************************************************
 * MUSICHandleUSBPlugIn:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void MUSICHandleUSBPlugIn(UINT32 param)
{
    HandlePlugandPull(AP_MSG_USB_PLUGIN);
}
/******************************************************************************
 * MUSICHandleUSBPlugOut:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static void MUSICHandleUSBPlugOut(UINT32 param)
{
    HandlePlugandPull(AP_MSG_USB_PULLOUT);
}
#endif /* APP_SUPPORT_USB */

static void MUSICHandleTFalshCardPlugInOut(UINT32 param)
{
    HandlePlugandPull((1 == param) ? AP_MSG_SD_IN : AP_MSG_SD_OUT);
}

/********************************************************************************
                    MUSIC_SelectAudio
* Description : 换曲时要做的选曲，初始化动作
* Arguments  :
* Returns     :真时表示可继续放，假时表示要停止放(播放状态)
* Notes       :
********************************************************************************/
BOOL MUSIC_SelectAudio( UINT8 Direct, Music_sel_mod_t mod )
{
    BOOL result = 0;

    //选文件
    //g_music_gui_ptr->PlayStatus = NormalPlay;
    MUSIC_ClearBreakTimePara();
//    g_music_gui_ptr->TagInfoBuf[0] = 0x0;

    if (mod == Play_key)
    {
        //play 按键的情况
        switch (g_music_vars->repeat_mode)
        {
            case FSEL_ALL_REPEATONE:
                //case FSEL_ALL_SEQUENCE:
                //case FSEL_DIR_RANDOM:
                fselSetMode( FSEL_ALL_REPEATALL );/*PLAY 时按键*/
                break;

            default:
                fselSetMode(g_music_vars->repeat_mode);
                break;
        }
    }
    else if (mod == Stop_key)
    {
        fselSetMode(FSEL_ALL_REPEATALL);//STOP时按键
    }
    else
    {
        fselSetMode(g_music_vars->repeat_mode);//正常播放的情况
    }

    switch (Direct)
    {
        case Music_Reset_Head:
        case Music_Reset_End:
            break;

        case Music_Cur:
            result = TRUE;
            break;

        case Music_Next:
            //hal_HstSendEvent(SYS_EVENT, 0x1609120b);
            //hal_HstSendEvent(SYS_EVENT, g_play_disk);

            result = fselGetNextFile(&g_music_vars->location[g_play_disk].file_entry);
            break;

        case Music_Prev:
            //hal_HstSendEvent(SYS_EVENT, 0x1609120c);
            //hal_HstSendEvent(SYS_EVENT, g_play_disk);

            result = fselGetPrevFile(&g_music_vars->location[g_play_disk].file_entry);
            break;

#if APP_SUPPORT_CHANGE_FOLDER == 1
        case Music_NextFolder:
            result = fselGetFileInNextFolder(&g_music_vars->location[g_play_disk].file_entry);
            break;

        case Music_PrevFolder:
            result = fselGetFileInPrevFolder(&g_music_vars->location[g_play_disk].file_entry);
            break;
#endif

        default:
            return FALSE;
    }

    if (!result)
    {
        //hal_HstSendEvent(SYS_EVENT, 0x1609120d);
        //hal_HstSendEvent(SYS_EVENT, g_play_disk);

        fselSetLocation(&g_music_vars->location[g_play_disk]);
        return result;
    }
    //取当前歌曲数状态
    g_music_gui_ptr->musicamount   = fselGetTotal();
    g_music_gui_ptr->musicsequence = fselGetNo();

    fselSetMode(g_music_vars->repeat_mode);
    //确保选择器与AP的目录对应

    fselGetLocation(&g_music_vars->location[g_play_disk]);
    //检测Music的类型
    g_music_gui_ptr->currentMusicType = MUSIC_CheckType(g_music_vars->location[g_play_disk].file_name);

#if (APP_SUPPORT_LCD==1) || defined(__AT_MOD_MUSIC__)
    //hal_HstSendEvent(SYS_EVENT, 0x1609120f);
    //hal_HstSendEvent(SYS_EVENT, g_play_disk);
    if( FALSE == g_music_gui_ptr->isFileInfoGot )
    {
        FS_GetEntryName(g_music_vars->location[g_play_disk].file_entry,MUSIC_MAX_TAGINFO,g_music_gui_ptr->TagInfoBuf);
        //gui_unicode2local(g_music_gui_ptr->TagInfoBuf, MUSIC_MAX_TAGINFO-2);
        app_trace(APP_MUSIC_TRC, "FileName:%s",g_music_gui_ptr->TagInfoBuf);
    }
    strcat(g_music_gui_ptr->TagInfoBuf, " ");
#endif
    g_music_gui_ptr->isFileInfoGot = FALSE;

    return TRUE;
}

/******************************************************************************
** 名字:
** 接口: INT32 UpdateFileSelector(void)
** 描述: 更新文件选择器
** 输入参数: void
** 输出参数:
            0: 更新不成功，要退出AP
            1: 更新成功，已可用
** 使用说明:
********************************************************************************/
INT32 UpdateFileSelector(void)
{
    /*1.在非正常播放下退出的情况，从上次播放地点继续播放保存变量的位置信息找到播放文件*/
    if (fselSetLocation(&g_music_vars->location[g_play_disk]))//根据location设置文件选择器参数
    {

        if (g_music_vars->location[g_play_disk].file_entry != 0)//已经有播放的文件了，并且这个文件就在当前目录
        {
            fselGetLocation(&g_music_vars->location[g_play_disk]);
#ifdef MULTI_DIR_TRAVERSAL_SUPPORT
            fselGetNoByEntry(g_music_vars->location[g_play_disk].file_entry);
#endif
            return 1;
        }
    }

    MUSIC_ClearBreakTimePara();//时间断点参数从0开始

    /*2.找不到上次播放的痕迹,找到目录的第一个文件*/
    hal_HstSendEvent(SYS_EVENT,0x09130002);

    if (fselGetNextFile(&g_music_vars->location[g_play_disk].file_entry))
    {
        //hal_HstSendEvent(SYS_EVENT, 0x16091211);
        //hal_HstSendEvent(SYS_EVENT, g_play_disk);

        fselGetLocation(&g_music_vars->location[g_play_disk]);
        return 1;
    }
    return 0;
}
/*********************************************************************************
*           void RefreshMainScreen( void )
*
* Description : 主显示的刷新函数,只有有需要时,才调用此函数
*
* Arguments   : void
*
* Returns     : void
*
* Notes       :
*
*********************************************************************************/
void RefreshMainScreen(UINT16 flag)
{


#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
    const region_t  reg_Scroll = {MUSCI_SCROLL_TXT_X, MUSCI_SCROLL_TXT_Y, LCD_WIDTH, 16};
    region_t time_rect = {MUSCI_CURRENT_TIME_X, MUSCI_CURRENT_TIME_Y, 40, 16};

    if(AP_Support_LCD())
    {
        ui_auto_update = FALSE;
        if (flag & (up_cardflag))
        {
            GUI_DisplayIconA(GUI_IMG_MMUSIC);
            //hal_HstSendEvent(SYS_EVENT, 0x16091212);
            //hal_HstSendEvent(SYS_EVENT, g_play_disk);

            //GUI_ResShowImage(GUI_IMG_DEVICE, g_play_disk, MUSCI_DEVICE_IMG_X, MUSCI_DEVICE_IMG_Y);
            //BT_DisplayIcon(POSITION_C, 0);
        }
#if APP_SUPPORT_FM_OUT
        if (flag & (up_musicfmout))
        {
            /*if (g_music_vars->fm_out) // FM output
                GUI_ResShowPic(GUI_IMG_RA_FM, POSITION_C, 0);
            else
            {
                const region_t region={POSITION_C, 0, 12, 16};
                GUI_ClearScreen(&region);
            }*/
        }
#endif
        if(g_music_gui_ptr->PlayStatus == MusicStop)
        {
            GUI_ClearScreen(&reg_Scroll);
            GUI_DisplayText(reg_Scroll.x,reg_Scroll.y, g_music_gui_ptr->TagInfoBuf);
            flag |= up_musicinformation;
        }
        else if (flag & up_musicinformation)/*Display file infomation */
        {
            GUI_SetScrollRegin(&reg_Scroll, 2);
            GUI_SetScrollStr(g_music_gui_ptr->TagInfoBuf, reg_Scroll.y/16);
            GUI_Scroll(g_music_gui_ptr->TagInfoBuf, TRUE);
        }

        if (flag & up_musicbitrate)
        {
            //先显示音乐类型图标
            if(g_music_gui_ptr->currentMusicType != Music_NON)
                GUI_ResShowImage(GUI_IMG_MUSICTYPE, g_music_gui_ptr->currentMusicType>Music_WMA?0:g_music_gui_ptr->currentMusicType-1, POSITION_B, 0);
            if(g_music_gui_ptr->BitRate >= 1000)
                GUI_DisplayNumbers(POSITION_B+3, 0, 3, GUI_IMG_BITRATENUMS, 999);
            else if(g_music_gui_ptr->BitRate > 0)
                GUI_DisplayNumbers(POSITION_B+3, 0, 3, GUI_IMG_BITRATENUMS, g_music_gui_ptr->BitRate);

        }

        if (flag & up_musicsequence) /*Display Music Number Of Total Music */
        {
            switch (g_music_gui_ptr->PlayStatus)
            {
                case MusicStop:
                case NormalPlay:
                    GUI_DisplayNumberD(g_music_gui_ptr->musicsequence, 0);
                    GUI_DisplayNumberD(g_music_gui_ptr->musicamount, 1);
                    break;
                default:
                    break;
            } //switch(g_music_gui_ptr->PlayStatus)
        }


        if (flag & up_musictotaltime)
        {
            GUI_DisplayText(LCD_WIDTH/2-4, MUSCI_CURRENT_TIME_Y, "/");
            GUI_DisplayTimeText(LCD_WIDTH/2+4, MUSCI_CURRENT_TIME_Y, GUI_TIME_DISPLAY_AUTO, g_music_gui_ptr->TotalTime);
        }

        if (flag & up_musiccurrenttime)
        {
            GUI_ClearScreen(&time_rect);
            GUI_DisplayTimeText(time_rect.x, time_rect.y, GUI_TIME_DISPLAY_AUTO, g_music_vars->CurrentTime[g_play_disk]);
            GUI_UpdateScreen(&time_rect);
        }


        if (flag & up_musicloopmode)/*Display Loop Mode*/
        {
            /* if (g_music_gui_ptr->PlayStatus == MusicStop)
             {
                 if (g_music_vars->fselmod == FSEL_TYPE_COMMONDIR)
                 {
                     g_music_vars->play_listmod = mlistnor;//改为普通目录模式
                 }
                GUI_ResShowImage(GUI_IMG_MENUMUSIC, 2, POSITION_C, 0);
                // GUI_DisplayIconC(PALYLIST_PIC[g_music_vars->play_listmod]);
             }
             else*/
            {
                GUI_ResShowImage(GUI_IMG_MUSICLOOPS, g_music_vars->repeat_mode, POSITION_F, 0);
            }
        }


        if (flag & up_musiceqmode)
        {
            GUI_ResShowImage(GUI_IMG_MUSICEQS, g_music_vars->eq_type, POSITION_E + 2, 0);
        }

        if (flag & up_followmode) /*Display Music Number Of Total Music */
        {
            if(g_music_gui_ptr->PlayStatus > NormalPlay)
                GUI_ResShowImage(GUI_IMG_MUSICRPT, g_music_gui_ptr->PlayStatus-LoopPlay, POSITION_D, 0);
        }

        if (flag & up_musicbattery)
        {
            GUI_DisplayBattaryLever();
        }

        if (flag & up_musicscroll)
        {
            INT8 nums[22];
            UINT8 num[5];
            UINT16 randnum;
            int i;
            INT32 img_x, img_y;

            //滚屏时的处理
            GUI_Scroll(g_music_gui_ptr->TagInfoBuf, FALSE);

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
            if(!g_music_gui_ptr->lyricflag)
#endif
            {
                randnum = rand();
                num[0] = (randnum>>12)%1;
                num[1] = (randnum>>9)%3;
                num[2] = (randnum>>6)%3;
                num[3] = (randnum>>3)%5;
                num[4] = (randnum)%5;

                img_x = MUSIC_FREQ_GRAPH_X;
                img_y = MUSIC_FREQ_GRAPH_Y;

                for(i=0; i<5; i++)
                {
                    img_x += MUSIC_FREQ_GRAPH_W;
                    GUI_ResShowImage(GUI_IMG_BARS, num[i], img_x, img_y);
                }
                for(i=0; i<5; i++)
                {
                    img_x += MUSIC_FREQ_GRAPH_W;
                    GUI_ResShowImage(GUI_IMG_BARS, num[i]+4, img_x, img_y);
                }

                num[0] = (randnum>>12)%5;
                num[1] = (randnum>>9)%5;
                num[2] = (randnum>>6)%3;
                num[3] = (randnum>>3)%3;
                num[4] = (randnum)%1;

                for(i=0; i<5; i++)
                {
                    img_x += MUSIC_FREQ_GRAPH_W;
                    GUI_ResShowImage(GUI_IMG_BARS, num[i]+4, img_x, img_y);
                }
                for(i=0; i<5; i++)
                {
                    img_x += MUSIC_FREQ_GRAPH_W;
                    GUI_ResShowImage(GUI_IMG_BARS, num[i], img_x, img_y);
                }

            }
        }

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
        if (flag & up_musiclyric)
        {
            if(g_music_gui_ptr->lyricflag)
            {
                const region_t region= {0, 48, LCD_WIDTH, 16};
                GUI_ClearScreen(&region);
                GUI_DisplayText(0,48, g_music_gui_ptr->lyric_buff);
            }
        }
#endif

        GUI_UpdateScreen(NULL);

        ui_auto_update = TRUE;
    }
#elif APP_SUPPORT_LED8S==1
    GUI_ClearScreen(NULL);
    GUI_ResShowPic(GUI_ICON_MUSIC|GUI_ICON_COLON,0,0);

    if (flag & (up_cardflag))
    {
        //hal_HstSendEvent(SYS_EVENT, 0x16091213);
        //hal_HstSendEvent(SYS_EVENT, g_play_disk);

        if(g_play_disk == FS_DEV_TYPE_USBDISK)
            GUI_ResShowPic(GUI_ICON_USB,0,0);
        else
            GUI_ResShowPic(GUI_ICON_SD,0,0);
    }

    if (flag & up_musicinformation)
    {
        if(g_music_gui_ptr->PlayStatus == MusicStop)
            GUI_ResShowPic(GUI_ICON_PAUSE,0,0);
        else
            GUI_ResShowPic(GUI_ICON_PLAY,0,0);
    }

    GUI_DisplayTimeLEDs(g_music_vars->CurrentTime[g_play_disk]);
    GUI_UpdateScreen(NULL);
#endif

}
/*********************************************************************************
* Description :  处理在播放状态下对按键的判断处理
*
* input : 键值 key
*
* output: 返回值 TRUE: 正常返回
*                FALSE: 不是正常返回
* Notes       :
*********************************************************************************/
BOOL ToNextMusic( Music_sel_mod_t mod )
{
    g_music_gui_ptr->Prevmusic_FLAG= FALSE;

    mediaSendCommand(MC_STOP, fast_fade);
    mediaSendCommand(MC_CLOSE, 0);
    if(!MUSIC_SelectAudio(Music_Next, mod))
        {
            return FALSE;
        }
       //取文件信息
      if(!MUSIC_GetInformation())
        {
            return FALSE;
        }
    mediaSendCommand(MC_PLAY, 0);

    //Redraw screen
    SendAppEvent(EV_UI_FW_REDRAW, MUSIC_REDRAW_FULLSCREEN);
    return TRUE;
}


BOOL PrevMusicInPlay(void)
{
    g_music_gui_ptr->Prevmusic_FLAG = TRUE;

    mediaSendCommand(MC_STOP, fast_fade);
    mediaSendCommand(MC_CLOSE, 0);
    if(!MUSIC_SelectAudio(Music_Prev, Play_key))
    {
        return FALSE;
    }

    //取文件信息
    if(!MUSIC_GetInformation())
    {
        return FALSE;
    }

    mediaSendCommand(MC_PLAY, 0);

    //Redraw screen
    SendAppEvent(EV_UI_FW_REDRAW, MUSIC_REDRAW_FULLSCREEN);
    return TRUE;
}

#if APP_SUPPORT_REMOTECONTROL==1
/******************************************************************************
 * MUSIC_HandleInputNum:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void MUSIC_HandleInputNum(UINT32 num)
{
    UINT64 LEntry=0;

    if((num <= g_music_gui_ptr->musicamount) && (num > 0))
    {
        g_music_gui_ptr->musicsequence = num;
        mediaSendCommand(MC_STOP, fast_fade);
        mediaSendCommand(MC_CLOSE, 0);
        fselGetByNO(&LEntry, num);

        g_music_vars->location[g_play_disk].file_entry=LEntry;
        FS_GetShortName(LEntry, g_music_vars->location[g_play_disk].file_name);
        g_music_gui_ptr->currentMusicType = MUSIC_CheckType(g_music_vars->location[g_play_disk].file_name);
        MUSIC_ClearBreakTimePara();
        g_music_gui_ptr->isFileInfoGot = FALSE;
        MUSIC_GetInformation(); //取文件信息

        if(NormalPlay == g_music_gui_ptr->PlayStatus) /* 播放状态 */
        {
            mediaSendCommand(MC_PLAY, 0);
        }

    }

}
#endif

/******************************************************************************
 * MUSIC_play_callback_hdlr:
 * DESCRIPTION: - Maybe callback in background
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void MUSIC_play_callback_hdlr(UINT32 result )
{
    BOOL no_error = TRUE;

    app_trace(APP_MUSIC_TRC,"MUSIC_play_callback_hdlr=%d",result);

    if (result == PLAYING_ERROR)
    {
        //显示格式错
        GUI_DisplayMessage(0,GUI_STR_FORMATERR, NULL, GUI_MSG_FLAG_WAIT);

        g_music_gui_ptr->ERROR_EXIT++;

        if(g_music_gui_ptr->ERROR_EXIT < (g_music_gui_ptr->musicamount+1))
        {
            if(FALSE == g_music_gui_ptr->Prevmusic_FLAG)
                no_error = ToNextMusic(Play_play);
            else
                no_error = PrevMusicInPlay();

            if(no_error)
            {
                if(g_comval->music_cfg.loop_mode == 0)
                {
                    g_music_vars->repeat_mode = FSEL_ALL_REPEATALL;
                }
            }
        }
        else
        {
            g_music_gui_ptr->ERROR_EXIT=0;
            //MUSIC_SwitchToNextMode();//now  exit music mode
            no_error = FALSE;
         }
    }
    else if (result == PLAYING_REACH_END)
    {
        g_music_gui_ptr->ERROR_EXIT=0;
        no_error = ToNextMusic(Play_play);
    }

    if(FALSE == no_error)
    {
        //回到停止界面
        app_trace(APP_MUSIC_TRC,"ERROR_EXIT=%d, musicamount=%d",g_music_gui_ptr->ERROR_EXIT, g_music_gui_ptr->musicamount);
        MUSIC_EnterStatus(RESULT_UI_STOP);
    }

}

/******************************************************************************
 * MUSIC_EnterStatus:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
UINT16 MUSIC_EnterStatus(INT32 status)
{
    UINT8 old_status = g_music_gui_ptr->screen_status;
    UINT8 new_status = status;
    UINT16 UpDisplayFlag = 0;

    app_trace(APP_MUSIC_TRC,"MUSIC enter status:%d, old=%d",new_status, old_status);

    //Do sth. to exit current status
    switch(old_status)
    {
        case RESULT_UI_PLAY:
        {
            mmi_cancel_timer(g_music_gui_ptr->timerid);
            g_music_gui_ptr->timerid = 0;
            ClearProtocolEventHandler(EV_AUDIO_PLAY_FINISH_IND);
            mediaSendCommand(MC_GETTIME, (INT32)&(g_music_vars->CurrentTime[g_play_disk]));
            if (new_status != RESULT_UI_FOLLOW)
            {
                mediaSendCommand(MC_STOP, slow_fade);
                mediaSendCommand(MC_CLOSE, 0);
            }
        }
        break;

#if APP_SUPPORT_FOLLOW_COMPARE==1
        case RESULT_UI_FOLLOW:
        {
            mediaSendCommand(MC_STOP, no_fade);
            mediaSendCommand(MC_CLOSE, 0);
            FS_Delete(FollowReadTempLongName);

            COS_Free_BTOverlay(s_follow_info_ptr);
            s_follow_info_ptr = NULL;
        }
        break;
#endif

        case RESULT_UI_STOP:
        {
            mmi_cancel_timer(g_music_gui_ptr->timerid);
            g_music_gui_ptr->timerid = 0;
        }
        break;


        default:
            break;

    }

    //Prepare for entry new status
    switch(new_status)
    {
        case RESULT_UI_STOP:
        {
            g_music_gui_ptr->PlayStatus = MusicStop;
        }
        break;

#if APP_SUPPORT_FOLLOW_COMPARE==1
        case RESULT_UI_FOLLOW:
        {
            UINT16 temp;

            media_status_t status_buf;

            APP_ASSERT(NULL == s_follow_info_ptr);
            s_follow_info_ptr = (FOLLOW_STATUS_INFO_PTR) COS_Malloc_BTOverlay(sizeof(FOLLOW_STATUS_INFO_T));
            memset(s_follow_info_ptr, 0x0, sizeof(FOLLOW_STATUS_INFO_T));

            //Init follow_play param
            mediaSendCommand(MC_GETSTATUS, &status_buf);
            mediaSendCommand(MC_GETSTARTPOS, &(s_follow_info_ptr->StartPos));

            s_follow_info_ptr->EndPos = status_buf.progress;

            if(TO_TIME(s_follow_info_ptr->EndPos - s_follow_info_ptr->StartPos) > g_music_vars->maxReplayTime * 1000)
            {
                s_follow_info_ptr->StartPos = s_follow_info_ptr->EndPos - (TO_PROGRESS(g_music_vars->maxReplayTime)/1000);
            }
            s_follow_info_ptr->rtc_clk_length = TO_TIME(s_follow_info_ptr->EndPos - s_follow_info_ptr->StartPos)/500 + 1;

            s_follow_info_ptr->StartTime = TO_TIME(s_follow_info_ptr->StartPos) + 500;


            s_follow_info_ptr->rec_file.file_name = FollowReadTempShortName;
            s_follow_info_ptr->rec_file.type = Music_WAV;

            hal_HstSendEvent(SYS_EVENT,0x12200020);
            hal_HstSendEvent(SYS_EVENT,s_follow_info_ptr->StartPos);
            hal_HstSendEvent(SYS_EVENT,s_follow_info_ptr->EndPos);
            hal_HstSendEvent(SYS_EVENT,s_follow_info_ptr->rtc_clk_length);
            hal_HstSendEvent(SYS_EVENT,s_follow_info_ptr->StartTime);


            //Prepare UI param
#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
            g_music_gui_ptr->lyric_next_time = 0;
#endif
            s_follow_info_ptr->rtc_clk_count = 0;

            g_music_gui_ptr->PlayStatus = MUSIC_FollowStatusMachine(NormalPlay, FALSE);
            g_music_vars->CurrentTime[g_play_disk] = s_follow_info_ptr->StartTime;

            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN | up_followmode;

        }
        break;
#endif

        case RESULT_UI_PLAY:
        {
            UINT16 StartPos = 0;

            g_music_gui_ptr->PlayStatus = NormalPlay;/* 播放状态 */

            MUSIC_GetInformation(); //取文件信息

            StartIntro(FALSE);

            if(g_music_gui_ptr->TotalTime==0)
                StartPos = 0;
            else
                StartPos = (UINT16) TO_PROGRESS(g_music_vars->CurrentTime[g_play_disk]);

            mediaSendCommand(MC_SETVOLUME, g_music_vars->volume);
            mediaSendCommand(MC_PLAY, StartPos);

            g_music_gui_ptr->timerid = mmi_timer_create(UiRefreshTimeInterval,MUSIC_PlayGuardHandler, NULL, COS_TIMER_MODE_PERIODIC);

            SetProtocolEventHandler(MUSIC_play_callback_hdlr, EV_AUDIO_PLAY_FINISH_IND);
            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;

        }
        break;

        case RESULT_UI_EXIT:
        {
            g_music_gui_ptr->PlayStatus = MusicStop;
            mediaSendCommand(MC_SETVOLUME, 0);
            mediaSendCommand(MC_CLOSE, 0);
        }
        break;

        default:
            break;

    }

    g_music_gui_ptr->screen_status = new_status;

    if(UpDisplayFlag)
    {
        SendAppEvent(EV_UI_FW_REDRAW, UpDisplayFlag);
    }

#if APP_SUPPORT_LED==1
    LED_SetPattern(GUI_LED_MUSIC_STOP + MIN(g_music_gui_ptr->PlayStatus, FollowRec), LED_LOOP_INFINITE);
#endif

    return 0;
}
/******************************************************************************
 * MUSIC_PlayStatusKeyHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/

BOOL MUSIC_PlayStatusKeyHandler(UINT32 key)
{
    UINT16 UpDisplayFlag = 0;

    switch (key)
    {
        case MUSIC_PLAY_REFRESH_UI:
        	//UI update
            UpDisplayFlag = (up_musicscroll | up_musiccurrenttime);

            // if get file information fail in open file, reget here
            if(g_music_gui_ptr->TotalTime==0)
            {
#if (APP_SUPPORT_RGBLCD==1) ||(APP_SUPPORT_LCD==1)
                if(AP_Support_LCD())
                {
                    audInfoStruct *pAudioinfo = COS_Malloc_BTOverlay(sizeof(audInfoStruct));

                    //if(Audio_GetDetailsInfo(g_music_vars->location.file_entry,&g_curaudioinfo,g_music_gui_ptr->currentMusicType,g_comval->langid))
                    if(mediaSendCommand(MC_GETFILEINFO, pAudioinfo))
                    {
                        g_music_gui_ptr->BitRate = pAudioinfo->bitRate/1000;
                        g_music_gui_ptr->TotalTime =  pAudioinfo->time;
                        UpDisplayFlag |= MUSIC_REDRAW_FULLSCREEN;
                    }

                    COS_Free_BTOverlay(pAudioinfo);
                }
                else
#endif //  APP_SUPPORT_LCD==1
                {
                    mediaSendCommand(MC_GETTOTALTIME, (INT32)&(g_music_gui_ptr->TotalTime));
                }
            }

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
		    if(g_music_gui_ptr->lyricflag)
		    {
		        if ((g_music_vars->CurrentTime[g_play_disk]) >= g_music_gui_ptr->lyric_next_time)
		        {
		            g_music_gui_ptr->lyric_next_time = g_music_vars->CurrentTime[g_play_disk];
		            if(lyricGetNext(g_music_gui_ptr->lyric_buff, g_music_vars->Lyric_Size, &(g_music_gui_ptr->lyric_next_time)))
		                UpDisplayFlag |= up_musiclyric;
		            else
		                lyricPreLoad();
		        }
		        else
		            lyricPreLoad();
		    }
#endif

            break;

        case AP_MSG_STANDBY:
            break;

        case AP_KEY_NEXT | AP_KEY_LONG: //快进
        case AP_KEY_PREV | AP_KEY_LONG: //快退
        #if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_FORE:
	 case AT_COMMON_BACK:
	 #endif

#if APP_SUPPORT_CHANGE_FOLDER == 1
    if(CHANGE_FOLDER_STATUS_NOKEY == changeFolderStatus)
    {
        mediaSendCommand(MC_STOP, fast_fade);
    }

    if((AP_KEY_NEXT | AP_KEY_LONG) == key)
    {
        changeFolderStatus = CHANGE_FOLDER_STATUS_NEXT;
    }
    else    //AP_KEY_PREV | AP_KEY_LONG) == key
    {
        changeFolderStatus = CHANGE_FOLDER_STATUS_PREV;
    }
    break;



#else   //#if SUPPORT_CHANGE_FOLDER == 0

            if(!g_music_gui_ptr->FFPlay_CNT)
            {
                mediaSendCommand(MC_STOP, fast_fade);
                g_music_gui_ptr->FFPlay_CNT = TRUE;
            }

            if((AP_KEY_NEXT | AP_KEY_LONG) == key)
            {
                g_music_vars->CurrentTime[g_play_disk] += MUSIC_FFPLAY_STEP*1000;

                if(g_music_vars->CurrentTime[g_play_disk] > g_music_gui_ptr->TotalTime)
                    g_music_vars->CurrentTime[g_play_disk] = g_music_gui_ptr->TotalTime;
            }
            else
            {
                if(g_music_vars->CurrentTime[g_play_disk] < MUSIC_FFPLAY_STEP*1000)
                    g_music_vars->CurrentTime[g_play_disk] = 0;
                else
                    g_music_vars->CurrentTime[g_play_disk] -= MUSIC_FFPLAY_STEP*1000;
            }

            UpDisplayFlag = up_musiccurrenttime;
            break;
#endif

        case AP_KEY_NEXT | AP_KEY_UP: //快进
        case AP_KEY_PREV | AP_KEY_UP: //快退
        #if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_FORE_PLAY:
	 case AT_COMMON_BACK_PLAY:
	 #endif

#if APP_SUPPORT_CHANGE_FOLDER == 1
    if(changeFolderStatus == CHANGE_FOLDER_STATUS_NEXT)
    {
        if(MUSIC_ChangeFolder( Music_NextFolder, Play_key ))
        {
            if(g_comval->music_cfg.loop_mode == 0)
                g_music_vars->repeat_mode = FSEL_ALL_REPEATONE;
        }
        else
        {
            //回到停止界面
            MUSIC_EnterStatus(RESULT_UI_STOP);
        }
    }
    else if(changeFolderStatus == CHANGE_FOLDER_STATUS_PREV)
    {
        if(MUSIC_ChangeFolder( Music_PrevFolder, Play_key ))
        {
            if(g_comval->music_cfg.loop_mode == 0)
                g_music_vars->repeat_mode = FSEL_ALL_REPEATONE;
        }
        else
        {
            //回到停止界面
            MUSIC_EnterStatus(RESULT_UI_STOP);
        }
    }
    changeFolderStatus = CHANGE_FOLDER_STATUS_NOKEY;
    break;

#else

            if(g_music_gui_ptr->FFPlay_CNT)
            {
                INT32 progress;
                if(g_music_gui_ptr->TotalTime != 0)
                    progress = (INT64) TO_PROGRESS (g_music_vars->CurrentTime[g_play_disk]);
                else
                    progress = 0;
                media_StopInternalAudio();
                if(progress > 9990)
                {
                    if(!ToNextMusic(Play_play))
                    {
                        //回到停止界面
                        MUSIC_EnterStatus(RESULT_UI_STOP);
                    }
                }
                else
                {
                    mediaSendCommand(MC_PLAY, progress);
                }

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
                if(g_music_gui_ptr->lyricflag)
                {
                    g_music_gui_ptr->lyric_next_time = g_music_vars->CurrentTime[g_play_disk];
                    if(!lyricGetNext(g_music_gui_ptr->lyric_buff, g_music_vars->Lyric_Size, &(g_music_gui_ptr->lyric_next_time)))
                    {
                        while(lyricPreLoad()); // load lyric for fast play
                        lyricGetNext(g_music_gui_ptr->lyric_buff, g_music_vars->Lyric_Size, &(g_music_gui_ptr->lyric_next_time));
                    }
                    UpDisplayFlag = up_musiclyric;
                    while(lyricPreLoad()); // load lyric for fast play
                }
#endif
                g_music_gui_ptr->FFPlay_CNT = FALSE;
            }
            break;
#endif

#if APP_SUPPORT_FM_OUT
        case MUSIC_OPEN_FMOUT:
        case AP_KEY_PLAY | AP_KEY_HOLD: // open fm out
        {
            if(g_music_vars->fm_out)
                break;
            else
                g_result = FM_OutPut(0xff, &(g_music_vars->freq), &g_music_vars->fm_out);

            if (g_result != 0)
            {
                return g_result;
            }
            mediaSendCommand(MC_SETVOLUME, g_music_vars->volume);
            //SendAppEvent(EV_UI_FW_REDRAW, MUSIC_REDRAW_FULLSCREEN);
        }
        break;
#endif
        case AP_KEY_PREV | AP_KEY_PRESS:
	      g_music_gui_ptr->ERROR_EXIT=0;
	 #if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_PREV:
	 #endif
            if(PrevMusicInPlay())
            {
                if(g_comval->music_cfg.loop_mode == 0)
                    g_music_vars->repeat_mode = FSEL_ALL_REPEATONE;
            }
            else
            {
                //回到停止界面
                MUSIC_EnterStatus(RESULT_UI_STOP);
            }
            break;

        case AP_KEY_NEXT | AP_KEY_PRESS:
		g_music_gui_ptr->ERROR_EXIT=0;//huan mode bug
         #if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_NEXT:
	 #endif
            if(ToNextMusic(Play_key))
            {
                if(g_comval->music_cfg.loop_mode == 0)
                    g_music_vars->repeat_mode = FSEL_ALL_REPEATALL;
            }
            else
            {
                //回到停止界面
                MUSIC_EnterStatus(RESULT_UI_STOP);
            }
            break;

#if APP_SUPPORT_FOLLOW_COMPARE==1
        case MUSIC_START_REPEAT:
        case AP_KEY_REC   | AP_KEY_PRESS: //start follow
#if APP_SUPPORT_FM_OUT
            if(g_music_vars->fm_out)
            {
                FM_OutPut(0xfe, NULL, &g_music_vars->fm_out);  // close fm
            }
#endif
            MUSIC_EnterStatus(RESULT_UI_FOLLOW);
            break;
#endif
        case AP_KEY_PLAY | AP_KEY_PRESS:
	 #if defined(__AT_MOD_MUSIC__)
	 case AT_MUSIC_PAUSE:
	 #endif
          #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
           play_Stop_Key_Status=1;
          #endif
            MUSIC_EnterStatus(RESULT_UI_STOP);
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
            NVRAMWriteData();   //write music data to flash before enter sleep
#endif
            break;

        default:
#if APP_SUPPORT_REMOTECONTROL==1
            if(MESSAGE_IsNumberKey(key))
            {
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
                GUI_InputNumber(key, 4, MUSIC_HandleInputNum);
#endif
                //break;
            }
            else
#endif
                if(!MUSIC_CommonKeyHandler(key))
                {
                    //Common events
                    return FALSE;
                }
            break;
    }

    if(UpDisplayFlag)
        SendAppEvent(EV_UI_FW_REDRAW, UpDisplayFlag);

    return TRUE;
}

/******************************************************************************
 * MUSIC_PlayGuardHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void MUSIC_PlayGuardHandler(void *param)
{
    UINT32 time;

    if(g_music_gui_ptr->FFPlay_CNT)
        return;

#if APP_SUPPORT_CHANGE_FOLDER
    if(changeFolderStatus != CHANGE_FOLDER_STATUS_NOKEY)
        return;
#endif

    mediaSendCommand(MC_GETTIME, (INT32)&g_music_vars->CurrentTime[g_play_disk]);
    time = g_music_vars->CurrentTime[g_play_disk];

	//Introduce mode
    if(g_music_vars->repeat_mode == FSEL_DIR_INTRO && time>MUSIC_INTRO_TIME*1000)
    {
        if(!ToNextMusic(Play_play))
        {
            //回到停止界面
            MUSIC_EnterStatus(RESULT_UI_STOP);
        }
        return;
    }

#if APP_SUPPORT_FADE_INOUT==1
	{
		static BOOL has_send = FALSE;
		//Approaching to end, fade out
		if((g_music_gui_ptr->TotalTime != 0) && ((g_music_gui_ptr->TotalTime - time) < 1000))
		{
			if(!has_send)
				AudSendFadeInOutReq(FALSE, slow_fade, NULL);
			has_send = TRUE;
		}
		else
		{
			has_send = FALSE;
		}
	}
#endif

	if(SCR_ID_MUSIC_MAIN == GetCurrentScreenId())
		SendAppEvent(MUSIC_PLAY_REFRESH_UI, 0);
}

/******************************************************************************
 * MUSIC_StopGetFileInfoTimeout:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void MUSIC_StopGetFileInfoTimeout(void *param)
{
    if(RESULT_UI_STOP == g_music_gui_ptr->screen_status)
    {
        MUSIC_GetInformation(); //取文件信息
        SendAppEvent(EV_UI_FW_REDRAW, (up_musictotaltime | up_musicbitrate | up_musicinformation));
    }
}
/******************************************************************************
 * MUSIC_StopStatusKeyHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL MUSIC_StopStatusKeyHandler(UINT32 key)
{
    UINT16 UpDisplayFlag = 0;

    switch (key)
    {
        case AP_MSG_WAIT_TIMEOUT:
            APP_Sleep();
            break;
        case AP_MSG_RTC:
            break;

        case AP_KEY_PLAY | AP_KEY_PRESS:
        case AP_KEY_PLAY | AP_KEY_LONG://add long press play for music wake up
	#if defined(__AT_MOD_MUSIC__)
	case AT_MUSIC_PLAY:
	#endif
    #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
            play_Stop_Key_Status=0;
    #endif
            StartIntro(FALSE);
            MUSIC_EnterStatus(RESULT_UI_PLAY);
            break;

//
#if APP_SUPPORT_CHANGE_FOLDER == 1
        case AP_KEY_NEXT | AP_KEY_LONG: //快进
        case AP_KEY_PREV | AP_KEY_LONG: //快退

            if((AP_KEY_NEXT | AP_KEY_LONG) == key)
            {
                changeFolderStatus = CHANGE_FOLDER_STATUS_NEXT;
            }
            else    //AP_KEY_PREV | AP_KEY_LONG) == key
            {
                changeFolderStatus = CHANGE_FOLDER_STATUS_PREV;
            }
            break;

#endif
//

        case AP_KEY_PREV | AP_KEY_PRESS:
#if APP_SUPPORT_CHANGE_FOLDER == 0
        case AP_KEY_PREV | AP_KEY_HOLD:
#endif
#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_PREV:
 #endif
            mediaSendCommand(MC_CLOSE, 0);
            MUSIC_SelectAudio(Music_Prev, Stop_key);

            //在停止状态下先不计时,到无按键动作一定时间后再开始计时
            //这样可以提高在停止状态时按next 跳歌的速度.
            mmi_cancel_timer(g_music_gui_ptr->timerid);
            g_music_gui_ptr->timerid = mmi_timer_create(CaltimeCNT_def, MUSIC_StopGetFileInfoTimeout, NULL, COS_TIMER_MODE_SINGLE);

            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;
            break;

        case AP_KEY_NEXT | AP_KEY_UP:
#if APP_SUPPORT_CHANGE_FOLDER == 1
            if(CHANGE_FOLDER_STATUS_NEXT  == changeFolderStatus)
            {
                MUSIC_ChangeFolder( Music_NextFolder, Stop_key );
            }
            changeFolderStatus = CHANGE_FOLDER_STATUS_NOKEY;
            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;
#endif
            //while(MESSAGE_Get() != 0);
            break;

        case AP_KEY_PREV | AP_KEY_UP:
#if APP_SUPPORT_CHANGE_FOLDER == 1
            if(CHANGE_FOLDER_STATUS_PREV  == changeFolderStatus)
            {
                MUSIC_ChangeFolder( Music_PrevFolder, Stop_key );
            }
            changeFolderStatus = CHANGE_FOLDER_STATUS_NOKEY;
            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;
#endif
            //while(MESSAGE_Get() != 0);
            break;

        case AP_KEY_NEXT | AP_KEY_PRESS:
#if APP_SUPPORT_CHANGE_FOLDER == 0
        case AP_KEY_NEXT | AP_KEY_HOLD:
#endif
#if defined(__AT_MOD_COMMON__)
        case AT_COMMON_NEXT:
#endif
            mediaSendCommand(MC_CLOSE, 0);
            MUSIC_SelectAudio(Music_Next, Stop_key);
            mmi_cancel_timer(g_music_gui_ptr->timerid);
            g_music_gui_ptr->timerid = mmi_timer_create(CaltimeCNT_def, MUSIC_StopGetFileInfoTimeout, NULL, COS_TIMER_MODE_SINGLE);

            UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;
            break;

        default:
#if APP_SUPPORT_REMOTECONTROL==1
            if(MESSAGE_IsNumberKey(key))
            {
#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
                GUI_InputNumber(key, 4, MUSIC_HandleInputNum);
#endif
                //break;
            }
            else
#endif
                if(!MUSIC_CommonKeyHandler(key))
                {
                    //Common events
                    return FALSE;
                }
            break;
    } //switch(key)

    if(UpDisplayFlag)
        SendAppEvent(EV_UI_FW_REDRAW, UpDisplayFlag);

    return TRUE;
}
#if APP_SUPPORT_FOLLOW_COMPARE==1
/******************************************************************************
 * MUSIC_FollowStatusMachine:
 * DESCRIPTION: -
 *
 *  NormalPlay ---(rec_key_press)----> LoopPlay ---(rec_key_press)-----> FollowRec -> CompareAB -> ComparePlay -> LoopPlay ---(max_loop_times)-----> NormalPlay
 *                                     |____________________________________________________________________^
 * Input: old status
 * Output:
 * Returns: new status
 *
 *
 ******************************************************************************/
static Replay_status_t MUSIC_FollowStatusMachine(Replay_status_t old_status, BOOL start_rec)
{
    Replay_status_t new_status = LoopPlay;

    app_trace(APP_MUSIC_TRC,"MUSIC FOLLOW exit old status:%d",old_status);

    //Stop current paly file
    mediaSendCommand(MC_STOP, no_fade);

    if(start_rec)
    {
        APP_ASSERT(FollowRec != old_status);

        mediaSendCommand(MC_CLOSE, 0);
        //open codec
        s_follow_info_ptr->rec_file.mode = MEDIA_RECORD;
        mediaSendCommand(MC_OPEN, (INT32)&(s_follow_info_ptr->rec_file));
        mediaSendCommand(MC_RECORD, 0);
        s_follow_info_ptr->loop_count = 0;
        new_status = FollowRec;
    }
    else
    {

        switch(old_status)
        {
            case FollowRec:
            {
                mediaSendCommand(MC_CLOSE, 0);
                mediaSendCommand(MC_OPEN,(INT32)&(g_music_gui_ptr->Openparam));
                mediaSendCommand(MC_PLAY, s_follow_info_ptr->StartPos);

                new_status = CompareAB;
            }
            break;

            case CompareAB:
            {
                mediaSendCommand(MC_CLOSE, 0);
                s_follow_info_ptr->rec_file.mode = MEDIA_PLAYREC;

                mediaSendCommand(MC_OPEN, (INT32)&(s_follow_info_ptr->rec_file));
                mediaSendCommand(MC_PLAY, 0);

                new_status = ComparePlay;
            }
            break;

            case ComparePlay:
            {

                mediaSendCommand(MC_CLOSE, 0);

                mediaSendCommand(MC_OPEN,(INT32)&(g_music_gui_ptr->Openparam));
                mediaSendCommand(MC_PLAY, s_follow_info_ptr->StartPos);

                new_status = LoopPlay;
                s_follow_info_ptr->loop_count = 0;
            }
            break;

            case LoopPlay:
            {
                if((++(s_follow_info_ptr->loop_count)) >=g_music_vars->replaytimes)
                {
                    new_status = NormalPlay;
                }
                else
                {
                    mediaSendCommand(MC_PLAY, s_follow_info_ptr->StartPos);
                }
            }
            break;

            case NormalPlay:
            {
                mediaSendCommand(MC_PLAY, s_follow_info_ptr->StartPos);
                new_status = LoopPlay;
            }
            break;
        }
    }

    switch(new_status)
    {
        case LoopPlay:
            LED_SetPattern(GUI_LED_MUSIC_LOOP, LED_LOOP_INFINITE);
            break;

        case NormalPlay:
            break;

        default:
            LED_SetPattern(GUI_LED_MUSIC_FOLLOW, LED_LOOP_INFINITE);
            break;
    }

    app_trace(APP_MUSIC_TRC,"MUSIC FOLLOW enter new status:%d, old=%d",new_status, old_status);

    return new_status;

}
/******************************************************************************
 * MUSIC_FollowStatusKeyHandler:
 * DESCRIPTION: -
 *在此状态下，按play/next/prev 到play状态；
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL MUSIC_FollowStatusKeyHandler(UINT32 key)
{
    BOOL is_rec_key_press = FALSE;
    UINT16 UpDisplayFlag = 0;

    switch (key)
    {
        case AP_MSG_STANDBY:
            break;

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
        // TODO: use mmi timer to implement it
        //case AP_KEY_NULL:
            if(g_music_gui_ptr->lyricflag)
            {
                if(g_music_vars->CurrentTime[g_play_disk] >= g_music_gui_ptr->lyric_next_time)
                {
                    g_music_gui_ptr->lyric_next_time = g_music_vars->CurrentTime[g_play_disk];
                    if(lyricGetNext(g_music_gui_ptr->lyric_buff, g_music_vars->Lyric_Size, &(g_music_gui_ptr->lyric_next_time)))
                        UpDisplayFlag = up_musiclyric;
                    else
                        lyricPreLoad();
                }
                else
                    lyricPreLoad();
            }
            break;
#endif

        case AP_KEY_NEXT | AP_KEY_DOWN:
        case AP_KEY_PREV | AP_KEY_DOWN:
        case AP_KEY_MODE | AP_KEY_PRESS:
            g_music_vars->CurrentTime[g_play_disk] = TO_TIME(s_follow_info_ptr->EndPos);
            MUSIC_EnterStatus(RESULT_UI_PLAY);
            break;

        case MUSIC_SET_FOLLOW:
        case AP_KEY_PLAY | AP_KEY_PRESS:
        case AP_KEY_REC | AP_KEY_PRESS: // start follow record
            if(g_music_gui_ptr->PlayStatus == FollowRec)
            {
                break;
            }
            else
            {
                is_rec_key_press = TRUE;
            }
        case AP_MSG_RTC:
            UpDisplayFlag = (up_musicscroll | up_musiccurrenttime);

            s_follow_info_ptr->rtc_clk_count ++;
            g_music_vars->CurrentTime[g_play_disk] = s_follow_info_ptr->StartTime + s_follow_info_ptr->rtc_clk_count*500;

            /*Follow status need changed*/
            if((s_follow_info_ptr->rtc_clk_count >= s_follow_info_ptr->rtc_clk_length) || is_rec_key_press)
            {
                Replay_status_t status;

                s_follow_info_ptr->rtc_clk_count = 0;
#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
                g_music_gui_ptr->lyric_next_time = 0;
#endif
                UpDisplayFlag |= up_followmode;

                status = MUSIC_FollowStatusMachine(g_music_gui_ptr->PlayStatus, is_rec_key_press);

                if(NormalPlay == status)
                {
                    //Exit to play status
                    MUSIC_EnterStatus(RESULT_UI_PLAY);
                    return TRUE;
                }
                else
                {
                    g_music_gui_ptr->PlayStatus = status;
                }

                g_music_vars->CurrentTime[g_play_disk] = s_follow_info_ptr->StartTime;
            }

            break;

        default:
            if(!MUSIC_CommonKeyHandler(key))
            {
                //Common events
                return FALSE;
            }
            break;
    } //switch(key)

    if(UpDisplayFlag)
        SendAppEvent(EV_UI_FW_REDRAW, UpDisplayFlag);

    return TRUE;
}
#endif

#if (APP_SUPPORT_CHANGE_FOLDER == 1)
/*******************************************************************************
* Description : play music file in the other folder.
*
* Arguments  :
*             direct: change to next folder or previous folder.
*             mode: Play_key or Stop_key
* Returns     :
*            TRUE: Successful,
*            FALSE: Fail.
* Notes       :
*
*******************************************************************************/
BOOL MUSIC_ChangeFolder( UINT8 direct, Music_sel_mod_t mode )
{
#if APP_SUPPORT_CHANGE_FOLDER == 1
    if( ((Music_NextFolder != direct) && (Music_PrevFolder != direct)) || (Play_play == mode) )
        return FALSE;
    if(Music_NextFolder == direct)
    {
        g_music_gui_ptr->Prevmusic_FLAG=0;
    }
    else    //Music_PrevFolder == direct
#endif
    {
        g_music_gui_ptr->Prevmusic_FLAG=1;
    }

    if(Play_key == mode)
    {
        mediaSendCommand(MC_STOP, fast_fade);
        mediaSendCommand(MC_CLOSE, 0);
    }
    else //Stop_key == mode
    {
        mediaSendCommand(MC_CLOSE, 0);
    }

    if(!MUSIC_SelectAudio(direct, mode))
    {
        return FALSE;
    }
    if(Play_key == mode)
    {
        //取文件信息
        if(!MUSIC_GetInformation())
        {
            return FALSE;
        }
        mediaSendCommand(MC_PLAY, 0);

        //Redraw screen
        SendAppEvent(EV_UI_FW_REDRAW, MUSIC_REDRAW_FULLSCREEN);
    }
    else //Stop_key == mode
    {
        //MUSIC_SelectAudio(direct, Stop_key);

    }
    return TRUE;
}
#endif


/******************************************************************************
 * MUSIC_CommonKeyHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL MUSIC_CommonKeyHandler(UINT32 key)
{
    UINT16 param = 0;
    INT32 result = 0;
    INT8 *string = NULL;
    UINT16 UpDisplayFlag = 0;

    param = param;
    result = result;
    string = string;
    switch(key)
    {
#if APP_SUPPORT_MENU==1
        case MUSIC_DELETE_FILE:
        {
            Delete_file();
        }
        break;
#endif
        case AP_KEY_MODE | AP_KEY_PRESS: //短按 MODE +
#if APP_SUPPORT_MENU==1
            if(AP_Support_MENU())
            {
                GUI_Display_Menu(GUI_MENU_MUSIC, music_menu_callback);
            }
            else
#endif
            {
                MUSIC_SwitchToNextMode();
            }
            break;

#if APP_SUPPORT_MENU==1
        case MUSIC_SELECT_FILE: // select file
            MUSIC_EnterStatus(RESULT_UI_STOP);
            //hal_HstSendEvent(SYS_EVENT, 0x16091216);
            //hal_HstSendEvent(SYS_EVENT, g_play_disk);

            GUI_EntryFileBrowser(&g_music_vars->location[g_play_disk], DIRECTORY_MUSIC, 0, MUSIC_SelectFileCb);
            break;

        case AP_KEY_MODE | AP_KEY_HOLD:
            if(AP_Support_MENU())
            {
                AppGotoMainMenu();
                break;
            }
            // else close fm out
#endif

#if APP_SUPPORT_FM_OUT
        case MUSIC_CLOSE_FMOUT:
            if(g_music_vars->fm_out)
            {
                FM_OutPut(0xfe, NULL, &g_music_vars->fm_out);  // close fm
                mediaSendCommand(MC_SETVOLUME, g_music_vars->volume);
                //UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN|up_musicfmout;
            }
            break;

        case MUSIC_SETUP_FMOUT:
            FM_OutPut(0, &g_music_vars->freq, &g_music_vars->fm_out);
            //UpDisplayFlag = MUSIC_REDRAW_FULLSCREEN;
            break;
#endif

        case AP_KEY_EQ | AP_KEY_PRESS:
            if (g_music_gui_ptr->PlayStatus == NormalPlay)
            {
                g_music_vars->eq_type = (g_music_vars->eq_type+1)%max_eq;
                mediaSendCommand(MC_SETEQ, g_music_vars->eq_type );
                UpDisplayFlag = up_musiceqmode;
            }
            break;
        case AP_KEY_LOOP | AP_KEY_PRESS:
            if (g_music_gui_ptr->PlayStatus == NormalPlay)
            {
#if APP_SUPPORT_LCD==1
                if(AP_Support_LCD())
                    g_music_vars->repeat_mode = (g_music_vars->repeat_mode+1)%FSEL_DIR_END;
                else
#endif
                {
                    if(g_music_vars->repeat_mode == FSEL_ALL_REPEATONE)
                        g_music_vars->repeat_mode = FSEL_ALL_REPEATALL;
                    else
                        g_music_vars->repeat_mode = FSEL_ALL_REPEATONE;
                }
                UpDisplayFlag = up_musicloopmode;
            }
            break;


        case AP_KEY_VOLSUB | AP_KEY_DOWN:
        case AP_KEY_VOLADD | AP_KEY_DOWN:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
	#if defined(__AT_MOD_COMMON__)
	 case AT_COMMON_VOLADD:
	 case AT_COMMON_VOLSUB:
	 #endif
            ChangeVolume(key);
            break;

#if APP_SUPPORT_MENU==1
        case MUSIC_SET_EQ:
            GUI_GetMenuResult(&param);
            g_music_vars->eq_type = param;
            mediaSendCommand(MC_SETEQ, g_music_vars->eq_type );
            UpDisplayFlag = up_musiceqmode;
            break;
        case MUSIC_SET_LOOP:
            GUI_GetMenuResult(&param);
            g_music_vars->repeat_mode = param;
            UpDisplayFlag = up_musicloopmode;
            break;

        case MUSIC_SET_RPT_TIMES:
            slider.max = 9;
            slider.min = 1;
            slider.step = 1;

            if (g_music_vars->replaytimes<1) //检测复读次数是否超出范围
            {
                g_music_vars->replaytimes = 1;
            }
            if (g_music_vars->replaytimes>9)
            {
                g_music_vars->replaytimes = 9;
            }
            slider.value = g_music_vars->replaytimes;
            string = GUI_GetString(GUI_STR_REPLAYTIMES);
            GUI_ClearScreen(NULL);
            GUI_DisplayTextCenter(0,string);

            GUI_Slider(& slider, MUSIC_SetRepeatTimesCb);
            break;

        case MUSIC_SET_RPT_PEROID:
            slider.max = 99;
            slider.min = 5;
            slider.step = 10;

            if (g_music_vars->maxReplayTime<5)//检测复读间隔是否超出范围
            {
                g_music_vars->maxReplayTime = 5;
            }
            if (g_music_vars->maxReplayTime>99)
            {
                g_music_vars->maxReplayTime = 99;
            }
            slider.value = g_music_vars->maxReplayTime;

            string = GUI_GetString(GUI_STR_REPLAYGAP);
            GUI_ClearScreen(NULL);
            GUI_DisplayTextCenter(0,string);

            GUI_Slider(& slider, MUSIC_SetRepeatPeroidCb);
            break;

#if APP_SUPPORT_LYRIC==1//支持歌词显示功能
        case MUSIC_OPEN_LYRIC:
            g_music_vars->show_lyric = TRUE;
            g_music_gui_ptr->lyricflag =lyricLoad(g_music_vars->location[g_play_disk].file_entry);
            if(g_music_gui_ptr->lyricflag)
            {
                g_music_gui_ptr->lyric_next_time = 0;
                memset(g_music_gui_ptr->lyric_buff, 0, AUDPLY_LYRIC_MAX_LINE_SIZE);
            }
            break;
        case MUSIC_CLOSE_LYRIC:
            g_music_vars->show_lyric = FALSE;
            g_music_gui_ptr->lyricflag = FALSE;
            break;
#endif
#endif // APP_SUPPORT_MENU==1

#if APP_SUPPORT_LCD
		case MUSIC_SET_ADD_TIMER:
            //回到停止界面
            MUSIC_EnterStatus(RESULT_UI_STOP);
			TIMER_API_AddAlarm(RESULT_MUSIC);
			break;
#endif
        default:
            return FALSE;
    }


    // TODO: Fix me!!
    if(0)//key == EV_BT_NOTIFY_IND)
    {
        mediaSendCommand(MC_STOP, slow_fade);
        mediaSendCommand(MC_CLOSE, 0);
    }

    if(UpDisplayFlag)
        SendAppEvent(EV_UI_FW_REDRAW, UpDisplayFlag);

    return TRUE;
}


/******************************************************************************
 * MusicMainMsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/

extern UINT8  tflash_poweron_flag1;
BOOL MusicMainMsgHandler(COS_EVENT *pEvent)
{
    UINT32 key;
    BOOL ret = TRUE;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            SetProtocolEventHandler(MUSICHandleTFalshCardPlugInOut, EV_FS_TFLASH_DETECT);
#if (APP_SUPPORT_USB)
            SetProtocolEventHandler(MUSICHandleUSBPlugOut, AP_MSG_USB_PULLOUT);
            SetProtocolEventHandler(MUSICHandleUSBPlugIn, AP_MSG_USB_PLUGIN);
#endif
            if(g_play_disk == FS_DEV_TYPE_TFLASH)
    	    {
    	       if(!(
 #if !defined(WIN32)

    	    (usb_poweron_det == 1)&&

#endif
    	    (tflash_poweron_flag1 == 1)))
        	   	{
        	   	    tflash_poweron_flag1 = 0;
        	         //media_PlayInternalAudio(GUI_AUDIO_TFCARD_PLAY, 1, 0);
                     GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_TFCARD_PLAY, GUI_MSG_FLAG_WAIT);
        	   	}
    	    }
#if APP_SUPPORT_USB
    	    else if(g_play_disk == FS_DEV_TYPE_USBDISK)
    	    {
    	         //media_PlayInternalAudio(GUI_AUDIO_MASS_STORAGE, 1, 0);
                GUI_DisplayMessageTone(0, 0, 0, GUI_AUDIO_MASS_STORAGE, GUI_MSG_FLAG_WAIT);
    	    }
#endif
            //Set current status to play
            //g_music_gui_ptr->screen_status = RESULT_UI_PLAY;
            if(pEvent->nParam1 == 0)
            {
                g_music_gui_ptr->screen_status = RESULT_UI_PLAY;
            }
            else    //force enter to tf or usb mode
            {
                g_music_gui_ptr->screen_status = RESULT_UI_EXIT;
            }
            mediaSendCommand(MC_SETEQ, g_music_vars->eq_type );
            break;

        case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
             //g_music_gui_ptr->screen_status = RESULT_UI_PLAY;
        case EV_UI_FW_ON_RESUME:
            MUSIC_EnterStatus(g_music_gui_ptr->screen_status);
            GUI_ClearScreen(NULL);
            RefreshMainScreen(MUSIC_REDRAW_FULLSCREEN);
            break;

        case EV_UI_FW_REDRAW:
            if(pEvent->nParam1)
            {
             app_trace(APP_MUSIC_TRC,"MUSIC redraw=0x%4x, Time:%d/%d",pEvent->nParam1, g_music_vars->CurrentTime[g_play_disk], g_music_gui_ptr->TotalTime);
             RefreshMainScreen((pEvent->nParam1) & MUSIC_REDRAW_MASK);
            }
            break;

        case EV_UI_FW_ON_PAUSE:
            {
                UINT8 prev_status = g_music_gui_ptr->screen_status;

                MUSIC_EnterStatus(RESULT_UI_STOP);
                //Save previous status and resume it when recv EV_UI_FW_ON_RESUME
                g_music_gui_ptr->screen_status = prev_status;
            }
            break;

        case EV_UI_FW_ON_EXIT:
            ClearProtocolEventHandler(EV_AUDIO_PLAY_FINISH_IND);
            media_StopInternalAudio();
            ClearProtocolEventHandler(EV_FS_TFLASH_DETECT);
#if (APP_SUPPORT_USB)
            ClearProtocolEventHandler(AP_MSG_USB_PULLOUT);
            ClearProtocolEventHandler(AP_MSG_USB_PLUGIN);
#endif
            hal_SysSetVocClockDefault();
            MUSIC_DeInit();
            break;

#ifdef __AT_MOD_COMMON__
        case AT_COMMON_VOLUME:
        	g_music_vars->volume = MIN(pEvent->nParam1, MUSIC_MAX_VOLUME);;
	        if(g_music_gui_ptr->PlayStatus != MusicStop)
	            mediaSendCommand(MC_SETVOLUME, g_music_vars->volume);
	        g_comval->Volume = g_music_vars->volume;
        	break;
#endif /* __AT_MOD_COMMON__ */


        default:
            //key event
            key = MESSAGE_ConvertToKeyValue(pEvent);
            switch(g_music_gui_ptr->screen_status)
            {
                case RESULT_UI_PLAY:
                    ret = MUSIC_PlayStatusKeyHandler(key);
                    break;

                case RESULT_UI_STOP:
                    ret = MUSIC_StopStatusKeyHandler(key);
                    break;
#if APP_SUPPORT_FOLLOW_COMPARE==1
                case RESULT_UI_FOLLOW:
                    ret = MUSIC_FollowStatusKeyHandler(key);
                    break;
#endif
                case RESULT_UI_EXIT:
                    if((AP_KEY_MODE | AP_KEY_PRESS) == key)//Mode key handler when play tone
                    {
                        MUSIC_SwitchToNextMode();
                    }
                    else
                    {
                        ret = FALSE;
                    }
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
/*********************************************************************************
* Description : 主函数
*
* Arguments   :进入MP3播放界面，播放时对歌词的处理等
        param:

*
* Returns     : 结果消息或0
*
* Notes       :
*
*********************************************************************************/
INT32 MUSIC_Entry(INT32 param)
{

    INT16 error = 0;

    app_trace(APP_MUSIC_TRC, "====MUSIC_Entry====");
    //MESSAGE_Initial(NULL);//开始算STANDBY 时间

    //显示请稍后
    GUI_DisplayMessage(0, GUI_STR_WAITPASS, NULL, GUI_MSG_FLAG_DISPLAY);
    error = MUSIC_Init();
    if( (0 != error) && (param == 0) )
    {
        app_trace(APP_MUSIC_TRC, "====MUSIC_Init Error_id=%d====",error);
        GUI_DisplayMessageWithCB(0,error, NULL, AppGotoMainMenu, GUI_MSG_FLAG_WAIT);
        if(NULL != g_music_gui_ptr)
        {
            COS_Free_BTOverlay(g_music_gui_ptr);
            g_music_gui_ptr = NULL;
        }
        return 1;
    }
    else
    {
        app_trace(APP_MUSIC_TRC, "====MUSIC_Init OK param=%d====",param);
        EntryNewScreen(SCR_ID_MUSIC_MAIN,MusicMainMsgHandler,NULL, param, 0);
        return 0;
    }
}

#endif
