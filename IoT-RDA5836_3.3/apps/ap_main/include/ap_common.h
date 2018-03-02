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


#ifndef _AP_COMMON_H_
#define _AP_COMMON_H_
#include "tgt_app_cfg.h"
#include "string.h"

#include "platform.h"
#include "device.h"
#include "globalconstants.h"
#include "event.h"
#include "fs.h"
#include "assert.h"
#include "stdio.h"

#ifndef PCWSTR
#define PCWSTR const UINT16*
#endif

#ifndef PWSTR
#define PWSTR UINT16*
#endif

#ifndef WCHAR
#define WCHAR UINT16
#endif

#define FILEDATABUF             0x7200
#define LISTNAME_DISP_LENNGTH   512     //必须是2的倍数，因为unicode是2BYTE/字符

//主要的应用模块，增加时可以在这里增加
#define RESULT_NULL             0
#define RESULT_CALENDAR         1
#define RESULT_STANDBY          2
#define RESULT_IDLE             3
#define RESULT_UDISK            4
#define RESULT_MUSIC            5
#define RESULT_BT               6
#define RESULT_RADIO            7
#define RESULT_SYSTEM           8
#define RESULT_RECORD_START     9
#define RESULT_RECORD_NOSTART   10
#define RESULT_MAIN             11
#define RESULT_TIMER            12
#define RESULT_FMREC_START      13
#define RESULT_FMREC_NOSTART    14
#define RESULT_BT_ACTIVE        15
#define RESULT_RECORD_SWITCH    16
#define RESULT_ALARM            17
#define RESULT_LINE_IN          18
//#define RESULT_EXIT_UDISK       19
#define RESULT_CHARGING         20
#define RESULT_KEYCALIB         21
#define RESULT_MUSIC_TF             22
#define RESULT_MUSIC_USB             23
#define RESULT_UART_PLAY        24
#define RESULT_UART_REC         25

//return message id
#define RESULT_REDRAW           50    //界面需要重画
#define RESULT_POWERON          51    //从standby返回
#define RESULT_IGNORE           52    //控件取消选择
#define RESULT_TIMEOUT          53    //控件取消选择
#define RESULT_SD_IN            54
#define RESULT_SD_OUT           55
#define RESULT_BT_PENDING       56

//user define id
#define RESULT_USER1            100

//controls constant
#define DELETE_FILE             0
#define DELETE_MUSIC            1
#define DELETE_VOICE            2
#define DELETE_ALL              3    //this was not impletemented

#define DIRECTORY_MUSIC         FSEL_TYPE_MUSIC
#define DIRECTORY_VOICE         FSEL_TYPE_VOICE
//#define DIRECTORY_ALL           2

#define MAX_FILE_LEN            12

//#define __OGG_SUPPORT

//常量
#define PLAYING_STOP            0x00
#define PLAYING_PLAYING         0x01 //play file/stream can paly sync with side tone
#define PLAYING_PAUSE           0x02
#define PLAYING_FM              0x03
#define PLAYING_LINEIN          0x04
#define PLAYING_REACH_END       0x06
#ifdef KALAOK_SUPPORT
#define PLAYING_MICROPHONE      0x0a
#endif
#define PLAYING_ERROR           0x0C
#define PLAYING_INTERNAL        0x0f

#define RECORDING_STOP          0x10
#define RECORDING_RECORDING     0x11
#define RECORDING_PAUSE         0x12
#define RECORDING_WAITING       0x13
#define RECORDING_EXCEPTION     0x14
#define RECORDING_OUT_SPACE     0x15
#define RECORDING_DISK_ERROR    0x16
#define RECORDING_VOR_RECORDING 0x17
#define RECORDING_VOR_WAITING   0x18

#define SIGNAL_NON              0x00//没有信号
#define SIGNAL_REACH_V          0x01//到V点
#define SIGNAL_REACH_B          0x02//到B点
#define SIGNAL_CLEAR_B          0x03//到B点，但同时要清B点

//定义淡入淡出类型
#define FadeNon_def              0
#define FadeIn_def               1
#define FadeOut_def              2
#define FadeOut2_def             3//即时fadeout

/* ------ 文件选择操作 ------*/
//file system layer
#define TOTALDIRLAYER            8
//file selector defines
#define FSEL_MAX_FILENUM        999
#define FSEL_MAX_DIRNUM         100
#define PLAYLIST_PATH           L"/playlist.bin"

// select type defines
#define FSEL_TYPE_MUSIC         0
#define FSEL_TYPE_VOICE         1
#define FSEL_TYPE_ALLFILE       2
#define DIRECTORY_ALL           3

// select playlist or common dir
#define FSEL_TYPE_COMMONDIR     0
#define FSEL_TYPE_PLAYLIST      1
#define FSEL_TYPE_NEW_PLAYLIST  2


// select mode defines
#define FSEL_ALL_SEQUENCE       0
#define FSEL_ALL_REPEATONE      1
#define FSEL_DIR_SEQUENCE       2
#define FSEL_DIR_REPEATALL      3
#define FSEL_ALL_REPEATALL      4
#define FSEL_DIR_RANDOM         5
#define FSEL_DIR_INTRO          6
#define FSEL_DIR_END            7


// delete type defines
#define FSEL_DEL_FILE           0
#define FSEL_DEL_DIRFILE        1

// error information defines
#define FSEL_ERR_OUTOF          1
#define FSEL_ERR_EMPTY          2
#define FSEL_ERR_SETDIR         3
#define FSEL_ERR_SETFILE        4
#define FSEL_ERR_EXCEPTION      5
#define FSEL_ERR_FS             20
#define FSEL_ERR_FAIL           21
#define FSEL_ERR_DISK           22
#define FSEL_ERR_PLAYLIST       23


#define CP_ASCII    1         //上面注释掉的#define是错误的!由于原来ASCII和CHS用同一个驱动而没有体现出来.
#define CP_CHS      0         //在加入Latin1之后,此BUG将浮现!故必须做此修正!
#define CP_CHT      2

//headset and hid exist at the same.
#if APP_SUPPORT_HID_MODE==1
#define BT_MODE_HEADSET 0
#define BT_MODE_HID     1
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// file system type
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define FS_DIR_FILE             0x80   //  表示（根据输入扩展名）从当前目录首DIR第一个文件
#define FS_DIR_DIR              0x81   //  表示（根据输入扩展名）从当前目录首DIR第一个目录
#define FS_DIR_FILE_CONT        0x00   //  表示（根据输入扩展名）DIR下一个文件
#define FS_DIR_DIR_CONT         0x01   //  表示（根据输入扩展名）DIR下一个目录

#define FS_OPEN_NORMAL          0x0   //  表示正常打开文件
#define FS_OPEN_LN_NORMAL       0x80  //  表示输入长文件名正常打开文件

#define FS_CD_ROOT              L"/"   //  表示改变当前目录到根目录（当前FAT16支持）
#define FS_CD_PARENT            L".."  //  表示改变当前目录到上一级父目录（当前FAT16不支持）

#define FS_GET_SPACE_TOTAL      0x00  //  表示获取描述当前驱动器总空间的扇区数
#define FS_GET_SPACE_FREE       0x01  //  表示获取描述当前驱动器剩余空间的扇区数

//ret
#define FS_INFO_RW_ERR          1   //   磁盘读写错误
#define FS_INFO_WRITE_PROTECT   2   //   磁盘写保护
#define FS_INFO_UNFORMATTED     3   //   磁盘未格式化
#define FS_INFO_OVEFLOW         4   //   文件操作超出文件边界，或目录操作超出目录边界
#define FS_INFO_NOT_EXIST       5   //   文件操作的目标文件，或目录操作的目录项
#define FS_INFO_SPACE_OUT       6   //   表示文件操作或目录操作没有磁盘空间，不能写数据或者扩展子目录
#define FS_INFO_DIR_OUT         7   //   文件操作根目录目录项满
#define FS_INFO_NOT_EMPTY       8   //   删除目录时返回，表示删除的目录为非空目录

// AppSupportFlag
#define AP_SUPPORT_LCD                   0x01
#define AP_SUPPORT_LED                   0x02
#define AP_SUPPORT_MENU                0x04
#define AP_SUPPORT_RECORD              0x08
#define AP_SUPPORT_FM                0x10
#define AP_SUPPORT_FMREC                0x20
#define AP_SUPPORT_BT                0x40
#define AP_SUPPORT_BT_A2DP               0x80
#define AP_SUPPORT_BT_OPP                0x100

// =============================================================================
//  TRACE Level mapping
// -----------------------------------------------------------------------------
#define APP_MAIN_TRC            1
#define APP_GUI_TRC             2
#define APP_BT_TRC              3
#define APP_MUSIC_TRC           4
#define APP_RECORD_TRC          5
#define APP_FM_TRC              6
#define APP_SETTING_TRC         7
#define APP_MEDIA_TRC			8
#define APP_USB_TRC			    9
#define APP_LINEIN_TRC			10
#define APP_TCPIP_TRC			14
#define APP_ADP_TRC				15

#ifdef WIN32
#define EXFAT_DIR_INODE_CACHE_SIZE              10
#endif
/*
 * APP_ASSERT(value, message), 确认value的值为真，否则停住，并显示文件名和行号 and message
 */

#define APP_ASSERT assert

/*
FUNC:
	Output trace information.
PARAM:
[level]
	Output level.
[fmt]
	The trace information that you want to output. Refer the usage of ANSI C printf().
**/
#ifndef APP_NO_TRACE
void app_trace(UINT16 level, char* fmt,...);
#else
#define app_trace(level, fmt, ...)
#endif

//#define hal_HstSendEvent(SYS_EVENT,ch)

typedef enum
{
    CharType_NUM,    //数字（0-9）
    CharType_UPER,    //大写字母（A-Z）
    CharType_DNER,    //小写字母（a-z）
    CharType_SPACE,    //空格 (" ")
    CharType_OTHER    //其他值
}CharType_e;

typedef struct
{
	UINT16 year;
	UINT8 month;
	UINT8 day;
}date_t;

typedef struct
{
	UINT8 hour;
	UINT8 minute;
	UINT8 second;
}ap_time_t;




//当前磁盘,目录,文件
typedef struct
{
    UINT8   disk;             //FS_DEV_TYPE，卡或者usb设备
    UINT8 exfat_dir_cache_length;
    UINT8 exfat_dir_cache_head;
    UINT8 exfat_dir_cache_tail;
    UINT64 exfat_dir_cache_buffer[EXFAT_DIR_INODE_CACHE_SIZE];
    UINT64  subdir_entry;     //subdir no file_inode
    UINT64  file_entry;       //cluster no,保存当前目录所在的族号，或者播放列表的所在族,file_inode
#ifdef APP_SUPPORT_PLAYLIST
    UINT8   DirNo[TOTALDIRLAYER];
    UINT8   dirLayer;
#endif
    INT8 subdir_name[MAX_FILE_LEN];        //8.3+'\0'
    INT8 file_name[MAX_FILE_LEN];    //8.3+'\0'
}file_location_t;


#if APP_SUPPORT_BLUETOOTH==1     /*Surport bt*/

#define SIZE_OF_BDADDR  6
#define SIZE_OF_LINKKEY 16

#define MAX_BT_DEVICE_LIST      10
#define MAX_BT_DEVICE_NAME      32

typedef struct
{
    UINT8 password[SIZE_OF_LINKKEY];
    UINT8 local_name[MAX_BT_DEVICE_NAME];
    UINT16 fix_address;
    UINT8 bt_address[SIZE_OF_BDADDR];
}btconfig_t;

#endif


#define SERIAL_NUMBER_MAX_SIZE              32

typedef struct {
	UINT32 AppSupportFlag;
    UINT8  serial_no[SERIAL_NUMBER_MAX_SIZE];
} sysconfig_t;

typedef struct {
   UINT8 lcd_width;
   UINT8 lcd_heigth;
   // display number
   UINT8 big_num_width;
   UINT8 small_num_width;
   // display time, width of ':'
   UINT8 big_colon_width;
   UINT8 small_colon_width;
   // battary icon postion
   UINT8 position_batt;
   // message position
   UINT8 message_y;
   // log image
   UINT8 log_image_count;
   UINT8 log_image_time;
   // color
   UINT16 back_color;
   UINT16 font_color;
} displayconfig_t;


typedef struct
{
    INT8    ListDesc[32];       //list文件类型描述：MYLIST / ARTIST / ALBUM / GENRE / COMPOSER / RATED
    UINT16    CatMembNum;         //类成员数
    UINT16    ListItemNum;        //list项总数
    UINT8    Reserved[512-36];   //头信息占用1扇区
} LISTHEAD;


typedef struct
{
    UINT8 switchflag;//开关如下各项(为1的位表示对应项打开，为0时表示关闭)
    INT32 RemainPlayCnt;//余下可播放的次数
    INT32 originalPlayCnt;//原始可播放次数

  	date_t BeginDate;//开始播放日期
    ap_time_t BeginTime;

    date_t ExpDate;//截止播放日期(2004-12-30)
    ap_time_t ExpTime;

    INT32 RemainTime;//(第一次用过之后还能用的小时数，对同一首歌是固定值)
    INT32 RemainStoreTime;//余下可存储的时长    (天数)//现在保留此接口，但暂时不加
    INT8 DealOnClkBck;//0: 时钟回调时删除 1：时钟回调失效
}licensinf_t;

typedef struct                  //16字节
{
	UINT32    stroffset;          //类字串文件内偏移，字节为单位
	UINT16	  strlength;          //类字串长度, 字节为单位, 但内容是unicode
	UINT16    MembItemNum;        //该类成员共有多少list项
	UINT32    FistItemOffset;     //该类成员首list项在文件的偏移,字节为单位
	INT8    reserved[4];
} CATMEMB;


typedef struct
{
    UINT32    FirstCluster;       //所在目录的首簇号
    UINT32    FileEntry;          //文件目录项号
} LISTITEM;                   //每ITEM占用8个字节


//文件建立时间
typedef struct
{
    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 hour;
    UINT8 minute;
    UINT8 second;
}fs_crt_time_t;

typedef struct
{
    UINT8   TIT2_length;
    INT8     *TIT2_buffer;
    UINT8   TPE1_length;
    INT8     *TPE1_buffer;
    UINT8   TALB_length;
    INT8     *TALB_buffer;
    UINT8   TENC_length;
    INT8     *TENC_buffer;
}ID3Info_t;


extern comval_t         *g_comval;
extern sysconfig_t      g_sysconfig;
extern displayconfig_t  g_displayconfig;

UINT64 FS_GetSpace(UINT8 disk,UINT8 type);

/// Return the date of build of system.
/// The format is 0xYYYYMMDD.
UINT32 AP_GetBuildDate(void);
UINT32 AP_GetBuildTime(void);

/// Return the version number
/// The format is 0xAABB, A is the big version, B is little version
UINT32 AP_GetVersion(void);

INT32 ResShowMultiLongStr(UINT16 id, UINT8 string_id, INT32 timeCount);
INT8  TM_GetDateAndTime(date_t *date,ap_time_t *time);//获取年月日，小时分
//时间为24小时制公历格式
BOOL  TM_SetDateAndTime(date_t *date, ap_time_t *time);
//设置格林威治时间，格式:seconds pass 1970/01/01 00:00:00公历格式
BOOL  TM_SetGWDateAndTime(date_t *date,ap_time_t *time);


//激活文件系统分区是否存在，若存在，返回分区状态（文件系统类型，是否格式化）
//para: 逻辑驱动器号，工作模式（通常=0）
//ret:  TRUE/FALSE
BOOL MountDisk(UINT8 disk);


// Initialize fsel Module, Must call it before call other fsel Module functions
BOOL fselInit(UINT8 type,  UINT8 mode, UINT8  PlayListOrCommonDir, UINT8 CardOrFlash);
//Initialize the playlist file
BOOL fselPlayListInit();
//open the playlist file
BOOL fselPlayListEnt(TCHAR *filename);
//close the playlist file and reset the fsel parament
BOOL fselPlayListExit(void);
//get the fsel next file
BOOL fselGetNextFile(UINT64 *entry);
//get the fsel prev file
BOOL fselGetPrevFile(UINT64 *entry);
//get the next group
BOOL fselGetNextGroup(TCHAR *groupname);
//get the prev group
BOOL fselGetPrevGroup(TCHAR *groupname);
//get the group long name
BOOL fselGetCurGroupL(TCHAR *grouplongname, UINT16 iLen);
//get the long filename
BOOL fselGetCurFNameL(TCHAR *longfilename, UINT8 iLen);
// Set  location of fsel module
UINT8 fselSetLocation(file_location_t  *location);
// Get location of fsel module
BOOL fselGetLocation(file_location_t  *location);
// Enter the current dir
BOOL fselEntCurrentDir(TCHAR *strDir);
//Return the parent dir
BOOL fselRetParentDir(TCHAR *strDir);
// HANDLE and match the case of current file delected.
#ifdef MULTI_DIR_TRAVERSAL_SUPPORT
BOOL fselDelFileByNo(UINT8 type, const UINT16 Num);
#endif
BOOL fselDelFile(UINT8 type);
// Set select mode
BOOL fselSetMode( UINT8 mode);
// Get total of files in currect directory
UINT16 fselGetTotal(void);
//Get the number of currect selected file
#ifdef MULTI_DIR_TRAVERSAL_SUPPORT
UINT16 fselGetNoByEntry(UINT64 strFile);
#endif
UINT16 fselGetNo(void);
// Get error id, it is the fail information of last called fsel module function.
UINT8 fselGetErrID(void);
// Get the next file, location changed
BOOL fselGetNext(UINT64 *entry);
//Get the previous file, location changed
BOOL fselGetPrev(UINT64 *entry);
// Get file according to file number, location changed
BOOL fselGetByNO( UINT64 *strFile, const UINT16 Num);
// Seek for next valid directory
BOOL fselNextDir(TCHAR *strDir);
// Seek for previous valid directory
BOOL fselPrevDir(TCHAR *strDir);
//if location->filename is not exists, just return
INT32 GenNewName(file_location_t *loation);
//del the list
BOOL playlistdel(TCHAR *filename);
BOOL playlistclassNext(TCHAR *classname);
BOOL playlistclassPrev(TCHAR *classname);
BOOL playlistnext(TCHAR *filename);
BOOL playlistprev(TCHAR *filename);
//common file:Set location of fsel module
UINT8 fselSetLocationC(file_location_t  *location);
// common file :Get location of fsel module
BOOL fselGetLocationC(file_location_t  *location);

CharType_e CheckCharType(INT8 data);

#if !defined(__DISABLE__MMI_TIMER__)
BOOL mmi_timer_initialise(void);
UINT8 mmi_timer_create(UINT32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, UINT8 flags);
BOOL mmi_change_timer(UINT8 timerHandle, UINT32 timeoutTenths);
BOOL mmi_cancel_timer(UINT8 timerHandle);
void mmi_handle_expired_timers(UINT32 param);

#endif /* __DISABLE__MMI_TIMER__ */

#ifndef STRING_H
UINT32 tcslen(PCWSTR str);
INT32  tcscmp(PCWSTR s1, PCWSTR s2);
INT32  tcsncmp(PCWSTR s1, PCWSTR s2, UINT32 n);
WCHAR* tcscpy(PWSTR to, PCWSTR from);
WCHAR* tcsncpy(PWSTR dst, PCWSTR src, UINT32 n);
WCHAR* tcscat(PWSTR s, PCWSTR append);
WCHAR* tcsncat(PWSTR dst, PCWSTR src, UINT32 len);
WCHAR* tcschr(PCWSTR p, WCHAR ch);
WCHAR* tcsrchr(PCWSTR p, WCHAR ch);
WCHAR* tcsupr(PWSTR p);
#endif

void FS_FileTimeConvert(fs_crt_time_t *pfileDateTime, UINT32 time);
UINT16 ap_convert_volume(UINT16 src_max, UINT16 dst_max, UINT16 src_vol);


#define AP_Support_LCD()          (g_sysconfig.AppSupportFlag&AP_SUPPORT_LCD)
#define AP_Support_LED()          (g_sysconfig.AppSupportFlag&AP_SUPPORT_LED)
#define AP_Support_MENU()         (g_sysconfig.AppSupportFlag&AP_SUPPORT_MENU)
#define AP_Support_RECORD()       (g_sysconfig.AppSupportFlag&AP_SUPPORT_RECORD)
#define AP_Support_FM()           (g_sysconfig.AppSupportFlag&AP_SUPPORT_FM)
#define AP_Support_FMREC()        (g_sysconfig.AppSupportFlag&AP_SUPPORT_FMREC)
#define AP_Support_BT()           (g_sysconfig.AppSupportFlag&AP_SUPPORT_BT)
#define AP_Support_A2DP()         (g_sysconfig.AppSupportFlag&AP_SUPPORT_BT_A2DP)
#define AP_Support_OPP()          (g_sysconfig.AppSupportFlag&AP_SUPPORT_BT_OPP)

#endif  /* _AP_COMMON_H_*/

