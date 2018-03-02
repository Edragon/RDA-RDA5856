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
#include "ap_gui.h"
#include "tm.h"
#include "hal_overlay.h"
#include "ap_mgr.h"
#include "ap_common.h"
//#include "hal_usb.h"

typedef struct
{
    UINT32 date;
    UINT32 time;
} BOOT_SECTOR_DATE_STRUCT_T;

#ifdef WIN32
BOOT_SECTOR_DATE_STRUCT_T g_date_and_time =
{ BUILD_DATE, BUILD_TIME};
#else
PROTECTED BOOT_SECTOR_DATE_STRUCT_T g_date_and_time
__attribute__((section (".build_date"))) =
{ BUILD_DATE, BUILD_TIME};
#endif
extern PUBLIC VOID hal_TimWatchDogPause(UINT8 pause);
extern UINT8 umss_GetUsbVirtualStatus();
extern int hal_UsbGetMode(void);
/*********************************************************************************
* Description : 根据序号生成新的可用文件名
*
* Arguments   : location->file_name, 原来的文件名
*               location->file_name, 输出新的文件名
*
* Returns     :  新文件的序号, 01~99
*        如果01~99 文件存在, 返回0
*
* Notes       :文件名字格式 : xxx000 WAV, xxx000 ACT, ***000 JPG...
*        如果输入的文件不存在,则直接返回
        检查目录满最长时间15s
        顺序产生下一个目录名最长时间1s.
*
* TODO:        不要使用 atoi(), 节省128byte  (ok!)
*********************************************************************************/
INT32 GenNewName(file_location_t *location)
{
    INT32 num;
    INT32 loops=0;
    WCHAR new_name[12];
    INT32 f;

    new_name[0] = location->file_name[0];
    new_name[1] = location->file_name[1];
    new_name[2] = location->file_name[2];

    new_name[6] = '.';
    new_name[7] = location->file_name[8];
    new_name[8] = location->file_name[9];
    new_name[9] = location->file_name[10];
    new_name[10] = 0;
    // 检查文件夹是否含有001这个文件,如没有RETURN 1
    new_name[3] = '0';
    new_name[4] = '0';
    new_name[5] = '1';
    // 如果文件不存在
    f = FS_Open((PCSTR)new_name, FS_O_RDONLY,0);

    if(f < 0)
    {
        location->file_name[3] = new_name[3];
        location->file_name[4] = new_name[4];
        location->file_name[5] = new_name[5];
        return 1;
    }
    else
    {
        FS_Close(f);
    }

    // 按指定的值,继续查找.
    new_name[3] = location->file_name[3] ;
    new_name[4] = location->file_name[4] ;
    new_name[5] = location->file_name[5] ;

    num = (new_name[3] - '0')* 100 + (new_name[4] - '0')* 10 + (new_name[5] - '0');
    if(num <= 0) num = 1;
    if(num > 200) num = 1;

    //如果文件存在,数字加1
    do
    {
        new_name[3] = ((num / 100) % 10) + '0';
        new_name[4] = ((num / 10) % 10) + '0';
        new_name[5] = (num % 10) + '0';

        // 如果文件不存在
        f = FS_Open((PCSTR)new_name, FS_O_RDONLY,0);

        if(f < 0)
        {
            location->file_name[3] = new_name[3];
            location->file_name[4] = new_name[4];
            location->file_name[5] = new_name[5];
            return num;
        }
        else
        {
            FS_Close(f);
            num++;
            if(num > 200)
                num = 1;
        }

    }
    while(++loops < 1000);

    //循环1000次还找不到合适的文件
    return 0;
}




void FS_FileTimeConvert(fs_crt_time_t *pfileDateTime, UINT32 time)
{
    TM_FILETIME   fileTime;
    TM_SYSTEMTIME sysTime;

    memset(&fileTime, 0, sizeof(TM_FILETIME));
    fileTime.DateTime = time;
    memset(&sysTime, 0, sizeof(TM_SYSTEMTIME));

    TM_FileTimeToSystemTime(fileTime, &sysTime);

    pfileDateTime->day     = sysTime.uDay;
    pfileDateTime->hour    = sysTime.uHour;
    pfileDateTime->minute  = sysTime.uMinute;
    pfileDateTime->month   = sysTime.uMonth;
    pfileDateTime->second  = sysTime.uSecond;
    pfileDateTime->year    = sysTime.uYear;
}


#if defined(WIN32)
#define hal_TimWatchDogPause(x)
#endif /* WIN32 */

/************************************************************************************
        FS_GetSpace()
 *描述
    获取u盘剩余空间，根据输入参数不同选择要求返回磁盘总的扇区数还是剩
    余扇区数
 *参数
    INT8 typechar  0表示调用将返回表示当前磁盘驱动器总空间的扇区数，
                   1表示返回当前磁盘驱动器剩余空间的扇区数
 *返回
    long long diskspace
 ************************************************************************************/
UINT64   FS_GetSpace(UINT8 disk,UINT8 type)
{
    static FS_INFO psFSInfo;
    INT32 Result = -1;
#ifndef WIN32
#ifdef CT_NO_DEBUG   //Disable Watchdog in debug mode
    hal_TimWatchDogPause(TRUE);
#endif
#endif
    if(FS_DEV_TYPE_FLASH==disk)/*Tflash*/
    {
        Result=FS_GetFSInfo("vds0", &psFSInfo);
    }
    else if(FS_DEV_TYPE_TFLASH==disk)/*Tflash*/
    {
        Result=FS_GetFSInfo("mmc0", &psFSInfo);
    }
    else if(FS_DEV_TYPE_USBDISK==disk)/*USB*/
    {
        Result=FS_GetFSInfo("usb0", &psFSInfo);
    }
    else/*Unsurport Disk*/
    {
#ifndef WIN32
#ifdef CT_NO_DEBUG   //Disable Watchdog in debug mode
        hal_TimWatchDogPause(FALSE);
#endif
#endif
        return 0;
    }
#ifndef WIN32
#ifdef CT_NO_DEBUG   //Disable Watchdog in debug mode
    hal_TimWatchDogPause(FALSE);
#endif
#endif
    if(ERR_SUCCESS==Result)
    {
        if(FS_GET_SPACE_FREE==type)//返回剩余扇区数目
        {
            return (psFSInfo.iTotalSize-psFSInfo.iUsedSize);
        }
        else if(FS_GET_SPACE_TOTAL==type)//返回总的扇区数目
        {
            return psFSInfo.iTotalSize;
        }
    }
    return 0;


}

/// Return the date of build of system.
/// The format is 0xYYYYMMDD.
UINT32 AP_GetBuildDate(void)
{
    return g_date_and_time.date;//BUILD_DATE;
}

/// Return the time of build of system.
/// The format is 0xHHMMSS.  change the BUILD_TIME of compilerrules.mk
UINT32 AP_GetBuildTime(void)
{
    return g_date_and_time.time;//BUILD_TIME;
}


/// Return the version number
/// The format is 0xAABB, A is the big version, B is little version
UINT32 AP_GetVersion(void)
{
    return 0x0100; // 1.0
}


extern INT32 DSM_DevInit();
//激活文件系统分区是否存在，若存在，返回分区状态（文件系统类型，是否格式化）
//para: 逻辑驱动器号，工作模式（通常=0）
//ret:  TRUE/FALSE
BOOL MountDisk(UINT8 dev_type)
{
#if (CHIP_BTOVERLAY_SUPPORTED!=0)
	hal_BTOverlayLoad(HAL_BTOVERLAY_FILESYSTEM);
    DSM_DevInit();
    FS_PowerOn();
#endif
    
#if APP_SUPPORT_USB==1
    UINT umss_status = umss_GetUsbVirtualStatus();
    if(umss_status == 0)
    {
        if(dev_type == FS_DEV_TYPE_TFLASH && GetUsbCableStatus()&&hal_UsbGetMode() == 1) // usb disk use t flash
        {
            return FALSE;
        }
    }
#else
    if(dev_type == FS_DEV_TYPE_USBDISK)
        return FALSE;
#endif
    if(FS_MountDevice(dev_type) == ERR_SUCCESS)
        return TRUE;
    else
        return FALSE;
}

//-------------------------------------------------
CharType_e CheckCharType(INT8 data)
{
    if ((data >= '0')&&(data <= '9'))
    {
        return CharType_NUM;
    }

    if ((data >= 'A')&&(data <= 'Z'))
    {
        return CharType_UPER;
    }

    if ((data >= 'a')&&(data <= 'z'))
    {
        return CharType_DNER;
    }

    if (data == ' ')
    {
        return CharType_SPACE;
    }
    return CharType_OTHER;
}

//type must equ. 0 means 公历格式
BOOL TM_SetDateAndTime(date_t *date, ap_time_t *time)
{
    //kval_t  kval;
    TM_SYSTEMTIME systemTime/*,SysRtcTime*/;
    //UINT32 /*TotalMinute1*/TotalMinute2;
    memset(&systemTime, 0, sizeof(TM_SYSTEMTIME));

    TM_GetSystemTime(&systemTime);

    if(date!=NULL)
    {
        systemTime.uYear  =date->year ;
        systemTime.uMonth=date->month;
        systemTime.uDay    =date->day;

    }
    if(time!=NULL)
    {
        systemTime.uHour    =time->hour;
        systemTime.uMinute =time->minute;
        systemTime.uSecond=time->second;
    }

    //TotalMinute1 = ((SysRtcTime.uYear-2000)*365+SysRtcTime.uMonth*30+SysRtcTime.uDay)*24*60+SysRtcTime.uMinute;
    //TotalMinute2 = ((systemTime.uYear-2000)*365+systemTime.uMonth*30+systemTime.uDay)*24*60+systemTime.uHour*60+systemTime.uMinute;

    //if(TotalMinute1 < TotalMinute2)
    {
        //将systemtime 和 gwtime写回到VRAM中
        //NVRAMRead(&kval, VM_KERNEL, sizeof(kval_t));
        //kval.systemtime = TotalMinute2;   //全局变量
        //NVRAMWrite(&kval, VM_KERNEL, sizeof(kval_t));
        TM_SetSystemTime(&systemTime);
    }
    return TRUE;
}


INT8 TM_GetDateAndTime(date_t *date,ap_time_t *time)
{
    TM_SYSTEMTIME systemTime;
    memset(&systemTime, 0, sizeof(TM_SYSTEMTIME));

    if (TM_GetSystemTime(&systemTime))
    {
        if(date!=NULL)
        {
            date->year = systemTime.uYear;
            date->month= systemTime.uMonth;
            date->day  = systemTime.uDay;
        }
        if(time!=NULL)
        {
            time->hour  = systemTime.uHour;
            time->minute= systemTime.uMinute;
            time->second= systemTime.uSecond;
        }
        //app_trace(APP_MAIN_TRC,"uHour:%d,uMinute:%d,uSecond:%d,uDayOfWeek:%d.",systemTime.uHour,systemTime.uMinute,systemTime.uSecond,systemTime.uDayOfWeek);
        return 0;
    }
    app_trace(APP_MAIN_TRC,"Fatal Error : TM_GetDateAndTime, Return -1!");
    return -1;
}

/**************************************************************
 *
 * Function : ap_convert_volume
 *
 * Input Params : src_max - src max volume
 *             dst_max - dst max volume
 *             src_vol  - src volume value
 *
 * Return Params: dst volume value
 *
 * Description :
 * 
 *   ASSERT( src_min = dst_min = 0)
 *
 *
 **************************************************************/
UINT16 ap_convert_volume(UINT16 src_max, UINT16 dst_max, UINT16 src_vol)
{
	const UINT16 src_min =  0;
	const UINT16 dst_min = 0;
	UINT16 dst_vol;
	
	if(src_vol >= src_max)
	{
		dst_vol = dst_max;
	}
	else if(src_min == src_vol)
	{
		dst_vol = dst_min;
	}
	else
	{
		dst_vol = ((UINT32) src_vol * (dst_max + 1)) / src_max;

		if(dst_vol >= dst_max)
		{
			dst_vol = dst_max - 1;
		}
		else if(dst_min == dst_vol)
		{
			dst_vol = dst_min + 1;
		}
		else
		{
			;
		}
	}

	return dst_vol;

}


#if 0//!defined(__DISABLE__MMI_TIMER__)


#define MMI_TIMER_UNUSED        0
#define MMI_MAX_TIMERS   20

#define MMI_MUTEX_CREATE(m)		(0)
#define MMI_MUTEX_LOCK(m) 		(0)
#define MMI_MUTEX_UNLOCK(m)		(0)
#define MMI_MUTEX_FREE(m) 		(0)

#define MMI_ENTER_CRITICAL_SECTION(x)  (0)
#define MMI_EXIT_CRITICAL_SECTION(x)   (0)


static struct t_MMITimerInfo
{
    UINT8 flags;
    UINT8 next;
    UINT32 timeoutTenths;
    UINT32 timeToExpiry;
    void (*callbackFunc)(void *);
    void *callbackParam;
} *mmi_timers;//[MMI_MAX_TIMERS];

static struct st_mmi_timer_info
{
    UINT32 timeTenths;
    UINT8 curr_process_timer;
    UINT32 timer_mutex;
} mmi_timer_info;

static UINT8 mmi_base_timer_id = 0;

void mmi_timer_enlist(UINT8 timer_index);
void mmi_timer_delist(UINT8 timer_index);

BOOL mmi_timer_initialise(void)
{
    mmi_timer_info.timeTenths = 0;
    mmi_timer_info.curr_process_timer = 0;
    mmi_timer_info.timer_mutex = MMI_MUTEX_CREATE(0);

    mmi_timers=(struct t_MMITimerInfo*)COS_MALLOC(sizeof(struct t_MMITimerInfo)*MMI_MAX_TIMERS);
    memset(mmi_timers, 0, sizeof(struct t_MMITimerInfo)*MMI_MAX_TIMERS);

    return TRUE;
}

void mmi_timer_reset(void)
{
    mmi_timer_info.timeTenths = 0;
    mmi_timer_info.curr_process_timer = 0;
    memset(mmi_timers, 0, sizeof(struct t_MMITimerInfo)*MMI_MAX_TIMERS);
}

void mmi_timer_shutdown(void)
{
    COS_FREE(mmi_timers);
    MMI_MUTEX_FREE(mmi_timer_info.timer_mutex);
}

UINT8 mmi_timer_create(UINT32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags)
{
    /* Timer handles must start at 1 - timer handle 0 is unused */
    struct t_MMITimerInfo *newEntry;
    UINT8 retVal;
    UINT8 i;

    if(flags == 0)
        return 0;

    for(i=1; i<MMI_MAX_TIMERS; i++)
    {
        if(mmi_timers[i].flags == MMI_TIMER_UNUSED)
        {
            UINT32 status = MMI_ENTER_CRITICAL_SECTION();
            MMI_MUTEX_LOCK( mmi_timer_info.timer_mutex);
            if(mmi_timers[i].flags == MMI_TIMER_UNUSED)
            {
                newEntry = &(mmi_timers[i]);
                newEntry->flags=flags;
                retVal = i;
                MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
                MMI_EXIT_CRITICAL_SECTION(status);
                break;
            }
            MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
            MMI_EXIT_CRITICAL_SECTION(status);
        }
    }
    if (i==MMI_MAX_TIMERS)
    {
    	APP_ASSERT(0);
        return 0;
    }

#if 1
    if (!(flags&COS_TIMER_MODE_SINGLE||flags&COS_TIMER_MODE_PERIODIC))
    {
        app_trace(APP_MAIN_TRC,"mmi_timer_create(): Invalid flags value 0x%02X specified\n",flags);
    }
#endif
    timeoutTenths = (timeoutTenths+3)/5;
    if(!timeoutTenths)
        timeoutTenths = 1;
    newEntry->callbackFunc=callbackFunc;
    newEntry->timeoutTenths=timeoutTenths;
    newEntry->timeToExpiry=timeoutTenths + mmi_timer_info.timeTenths;
    newEntry->callbackParam=callbackArg;
    newEntry->next = 0;
    mmi_timer_enlist(i);

    return retVal;
}

BOOL mmi_change_timer(UINT8 timerHandle, UINT32 timeoutTenths)
{

    if(timerHandle !=0 && timerHandle <= MMI_MAX_TIMERS)
    {
        UINT8 timer_index = timerHandle;
        struct t_MMITimerInfo *p_timer = &mmi_timers[timer_index];

        mmi_timer_delist(timer_index);
        timeoutTenths = (timeoutTenths+3)/5;
        if(!timeoutTenths)
            timeoutTenths = 1;
        p_timer->timeoutTenths = timeoutTenths;
        p_timer->timeToExpiry = timeoutTenths + mmi_timer_info.timeTenths;
        mmi_timer_enlist(timer_index);

        return TRUE;
    }
    else
        return FALSE;
}

BOOL mmi_cancel_timer(UINT8 timerHandle)
{
    if(timerHandle !=0 && timerHandle <= MMI_MAX_TIMERS && mmi_timers[timerHandle].flags != MMI_TIMER_UNUSED)
    {
        UINT8 timer_index = timerHandle;
        mmi_timers[timer_index].flags = MMI_TIMER_UNUSED;
        mmi_timer_delist(timer_index);

        return TRUE;
    }
    else
        return FALSE;
}

void mmi_base_timer_handler(void *param)
{
    SendAppEvent(EV_UI_FW_TIMER_EXPIRED, 0);
}

void mmi_handle_expired_timers(void)
{
    UINT8 curr_index;
    struct t_MMITimerInfo *current;

    mmi_timer_info.timeTenths ++;

    current = &(mmi_timers[mmi_timer_info.curr_process_timer]);

    if(current->flags == MMI_TIMER_UNUSED)
    {
        // TODO: Stop base timer
        if(mmi_base_timer_id != 0)
        {
            COS_KillTimer(mmi_base_timer_id);
            mmi_base_timer_id = 0;
        }
        return;
    }

    /* Decrement the counter and check for expiry */
    if(mmi_timer_info.timeTenths >= current->timeToExpiry)
    {
        UINT32 status;
        void (*callbackFunc)(void *);
        void *callbackParam;


        status = MMI_ENTER_CRITICAL_SECTION();
        MMI_MUTEX_LOCK( mmi_timer_info.timer_mutex);

        current = &(mmi_timers[mmi_timer_info.curr_process_timer]);

        if(current->flags == MMI_TIMER_UNUSED)
        {
            MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
            MMI_EXIT_CRITICAL_SECTION(status);
            return;
        }
        callbackFunc=current->callbackFunc;
        callbackParam = current->callbackParam;
        curr_index = mmi_timer_info.curr_process_timer;

        mmi_timer_info.curr_process_timer = current->next;
        current->next = 0;
        if (current->flags&COS_TIMER_MODE_SINGLE)
        {
            current->flags = MMI_TIMER_UNUSED; // disable the timer
        }
        MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
        MMI_EXIT_CRITICAL_SECTION(status);
        if (current->flags&COS_TIMER_MODE_PERIODIC)
        {
            current->timeToExpiry = current->timeoutTenths + mmi_timer_info.timeTenths;
            mmi_timer_enlist(curr_index);
        }

        // call callback function
        if(callbackFunc)
            callbackFunc(callbackParam);

    }
}

/**************************************************************
 *
 * Function : mmi_timer_delist
 *
 * Input Params : timer_index - Identifies timer to delist
 *
 * Description :
 * Local proc to remove timer from ordered list of used timers.
 *
 * Context: Both Task & ISR.
 *
 **************************************************************/
void mmi_timer_delist(UINT8 timer_index)
{
    UINT8 iter;

    UINT32 status = MMI_ENTER_CRITICAL_SECTION();
    MMI_MUTEX_LOCK( mmi_timer_info.timer_mutex);

    if(timer_index == mmi_timer_info.curr_process_timer)
    {
        mmi_timer_info.curr_process_timer = mmi_timers[timer_index].next;
        mmi_timers[timer_index].next = 0;
    }
    else if(timer_index)
    {
        for(iter=mmi_timer_info.curr_process_timer; iter; iter=mmi_timers[iter].next)
        {
            if(mmi_timers[iter].next == timer_index)
            {
                mmi_timers[iter].next = mmi_timers[timer_index].next;
                mmi_timers[timer_index].next = 0;
                break;
            }
        }
    }

    MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
    MMI_EXIT_CRITICAL_SECTION(status);
}

/**************************************************************
 *
 * Function : mmi_timer_enlist
 *
 * Input Params : timer_index - Identifies timer to enlist
 *
 * Description :
 * Local proc to insert timer into ordered list of used timers.
 *
 * Context: Both Task & ISR.
 *
 **************************************************************/
void mmi_timer_enlist(UINT8 timer_index)
{
    UINT8 iter;
    UINT8 b4timer = 0;
    UINT32 status;

    if(mmi_timer_info.curr_process_timer == 0) // first timer
    {
        // TODO: Start base timer
        if( 0 == mmi_base_timer_id)
            mmi_base_timer_id = COS_SetTimer(500, mmi_base_timer_handler, NULL, COS_TIMER_MODE_PERIODIC);
    }

    status = MMI_ENTER_CRITICAL_SECTION();

    MMI_MUTEX_LOCK( mmi_timer_info.timer_mutex);

    iter = mmi_timer_info.curr_process_timer;
    while(iter && (mmi_timers[iter].timeToExpiry<mmi_timers[timer_index].timeToExpiry))
    {
        b4timer = iter;
        if(iter==mmi_timers[iter].next)
        {
            mmi_timers[iter].next = 0;
            break;
        }
        iter=mmi_timers[iter].next;
        if(iter == timer_index)
            break;
        if(b4timer == iter)
        {
            mmi_timers[iter].next = 0;
            iter = 0;
        }
    }

    if(iter != timer_index)
    {
        if(iter == mmi_timer_info.curr_process_timer)
        {
            mmi_timers[timer_index].next = mmi_timer_info.curr_process_timer;
            mmi_timer_info.curr_process_timer = timer_index;
        }
        else
        {
            mmi_timers[timer_index].next = mmi_timers[b4timer].next;
            mmi_timers[b4timer].next = timer_index;
        }
    }
    MMI_MUTEX_UNLOCK( mmi_timer_info.timer_mutex);
    MMI_EXIT_CRITICAL_SECTION(status);
}
#else


#define MMI_TIMER_UNUSED        0
#define MMI_MAX_TIMERS   20

#if (COS_MMI_TIMER_ID_BASE + MMI_MAX_TIMERS >= (COS_MMI_TIMER_ID_END_))
#error 'Error: invalid MMI_MAX_TIMERS definition!'

#endif

static struct t_MMITimerInfo
{
    UINT8 flags;
    void (*callbackFunc)(void *);
    void *callbackParam;
} *mmi_timers;//[MMI_MAX_TIMERS];

BOOL mmi_timer_initialise(void)
{
    mmi_timers=(struct t_MMITimerInfo*)COS_MALLOC(sizeof(struct t_MMITimerInfo)*MMI_MAX_TIMERS);
    memset(mmi_timers, 0, sizeof(struct t_MMITimerInfo)*MMI_MAX_TIMERS);

    return TRUE;
}


UINT8 mmi_timer_create(UINT32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags)
{
    /* Timer handles must start at 1 - timer handle 0 is unused */
    struct t_MMITimerInfo *newEntry;
    UINT8 retVal;
    UINT8 i;

    if (!(flags&COS_TIMER_MODE_SINGLE||flags&COS_TIMER_MODE_PERIODIC))
    {
        app_trace(APP_MAIN_TRC,"mmi_timer_create(): Invalid flags value 0x%02X specified\n",flags);
        return 0;
    }

    for(i=1; i<MMI_MAX_TIMERS; i++)
    {
        if(mmi_timers[i].flags == MMI_TIMER_UNUSED)
        {
            newEntry = &(mmi_timers[i]);
            newEntry->flags=flags;
            retVal = i;
        	break;
        }
    }
    if (i==MMI_MAX_TIMERS)
    {
    	APP_ASSERT(0);
        return 0;
    }


    newEntry->callbackFunc=callbackFunc;
    newEntry->callbackParam=callbackArg;

    COS_SetTimerEX(Cos_GetTaskHandler(MOD_APP),COS_MMI_TIMER_ID_BASE + retVal,flags,timeoutTenths*100);

    return retVal;
}


BOOL mmi_change_timer(UINT8 timerHandle, UINT32 timeoutTenths)
{

    if(timerHandle !=0 && timerHandle <= MMI_MAX_TIMERS)
    {
        HANDLE csStatus = COS_EnterCriticalSection();
        UINT8 timer_index = timerHandle;
        struct t_MMITimerInfo *p_timer = &mmi_timers[timer_index];

        COS_KillTimerEX(Cos_GetTaskHandler(MOD_APP), COS_MMI_TIMER_ID_BASE + timer_index);
        COS_SetTimerEX(Cos_GetTaskHandler(MOD_APP),COS_MMI_TIMER_ID_BASE + timer_index,p_timer->flags,timeoutTenths*100);
        COS_ExitCriticalSection(csStatus);

        return TRUE;
    }
    else
        return FALSE;
}

BOOL mmi_cancel_timer(UINT8 timerHandle)
{
    if(timerHandle !=0 && timerHandle <= MMI_MAX_TIMERS && mmi_timers[timerHandle].flags != MMI_TIMER_UNUSED)
    {
        HANDLE csStatus = COS_EnterCriticalSection();

        UINT8 timer_index = timerHandle;
        mmi_timers[timer_index].flags = MMI_TIMER_UNUSED;
        COS_KillTimerEX(Cos_GetTaskHandler(MOD_APP), COS_MMI_TIMER_ID_BASE + timer_index);
        COS_ExitCriticalSection(csStatus);

        return TRUE;
    }
    else
        return FALSE;
}

void mmi_handle_expired_timers(UINT32 timerHandle)
{
    timerHandle -= COS_MMI_TIMER_ID_BASE;
    
    if(timerHandle !=0 && timerHandle <= MMI_MAX_TIMERS && mmi_timers[timerHandle].flags != MMI_TIMER_UNUSED)
    {
        void (*callbackFunc)(void *);
        void *callbackParam;

        callbackFunc = mmi_timers[timerHandle].callbackFunc;
        callbackParam = mmi_timers[timerHandle].callbackParam;

        if (mmi_timers[timerHandle].flags&COS_TIMER_MODE_SINGLE)
        {
            mmi_timers[timerHandle].flags = MMI_TIMER_UNUSED; // disable the timer
        }

        // call callback function
        if(callbackFunc)
            callbackFunc(callbackParam);
    }
}
#endif /* __DISABLE__MMI_TIMER__ */
