
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


/***********************************************************************
 *
 * MODULE NAME:    gui_display.h
 * DESCRIPTION:    gui display Interface
 * AUTHOR:         Tianwq
 *
 *
 * REMARKS:
 *       this file will be used by resedit, let every line start withoue blank, and use hex 0x define the values
 *
 ***********************************************************************/
 
#ifndef _GUI_CONST_H_
#define _GUI_CONST_H_

// language defines
#define GUI_LANG_ENGLISH                        0x0001
#define GUI_LANG_SM_CHINESE                     0x0002
#define GUI_LANG_TR_CHINESE                     0x0003
#define GUI_LANG_JAPANESE       				0x0004
#define GUI_LANG_KOREAN         				0x0005
#define GUI_LANG_SPANISH	                    0x0006
#define GUI_LANG_FRENCH		                    0x0007
#define GUI_LANG_GERMAN		                    0x0008
#define GUI_LANG_ITALIAN		                0x0009
#define GUI_LANG_RUSSIAN	                    0x000A  
#define GUI_LANG_THAI		                    0x000B	
#define GUI_LANG_TURKISH	                    0x000C	
#define GUI_LANG_MALAY		                    0x000D
#define GUI_LANG_VIETNAMESE	                    0x000E
#define GUI_LANG_DANISH		                    0x000F
#define GUI_LANG_PORTUGUESE	                    0x0010
#define GUI_LANG_INDONESIAN	                    0x0011
#define GUI_LANG_ARABIC	                        0x0012

// image defines
#define GUI_IMG_LOGO                            0x0001

#define GUI_IMG_MENUMAIN                        0x0005
#define GUI_IMG_MENUMAINA                       0x0006

#define GUI_IMG_UDISK                           0x0008
#define GUI_IMG_UAUDIO                          0x0009
#define GUI_IMG_LINEIN                          0x000A

// common use image
#define GUI_IMG_SMALL_NUM                       0x0010
#define GUI_IMG_BIG_NUM                         0x0011
#define GUI_IMG_BARS                            0x0012
#define GUI_IMG_BATTLVLS                        0x0013
#define GUI_IMG_BITRATENUMS                     0x0014

/*music images */
#define GUI_IMG_MUSICEQS	                    0x0020
#define GUI_IMG_MENUEQA	                        0x0021
#define GUI_IMG_MUSICLOOPS	                    0x0022
#define GUI_IMG_MENULOOPA	                    0x0023

#define GUI_IMG_MENUMUSIC	                    0x0024
#define GUI_IMG_MENUMUSICA	                    0x0025

#define GUI_IMG_DEVICE                          0x0026
#define GUI_IMG_MUSICTYPE                       0x0027
#define GUI_IMG_MUSICRPT                        0x0028


#define GUI_IMG_MMUSIC_FMOUT                    0x006F
#define GUI_IMG_MMUSIC		                    0x008F

/*FM pic*/
#define GUI_IMG_RA_FM		                    0x0090
#define GUI_IMG_MENU_FM		                    0x0091
#define GUI_IMG_MENU_FMA		                0x0092
#define GUI_IMG_FQBAR		                    0x0093
#define GUI_IMG_FQBAR_JP		                0x0094
#define GUI_IMG_DWNARROW		                0x0095
#define GUI_IMG_RA_STERE                        0x0096

/*BT pic */
#define GUI_IMG_MENU_BT		                    0x0097
#define GUI_IMG_MENU_BTA		                0x0098

/*GUI PIC*/
#define GUI_IMG_PBKGRD		                    0x0121
#define GUI_IMG_PBAR		                    0x0122
#define GUI_IMG_SBAR		                    0x0123
#define GUI_IMG_SBKGRD		                    0x0124
#define GUI_IMG_SSLIDER		                    0x0125
#define GUI_IMG_CARROWL		                    0x0126
#define GUI_IMG_CARROWN		                    0x0127
#define GUI_IMG_CARROWR		                    0x0128
#define GUI_IMG_VOLUMEBK		                0x0129
#define GUI_IMG_LISTDER	                        0x012A
#define GUI_IMG_LISTDERBG	                    0x012B

/* record */
#define GUI_IMG_MREC		                    0x0180    
#define GUI_IMG_MENU_REC		                0x0181
#define GUI_IMG_MENU_RECA		                0x0182
#define GUI_IMG_PLAY		                    0x0183

#define GUI_IMG_MENU_BTDEVICEOPTION             0x0184
#define GUI_IMG_MENU_BTDEVICEOPTIONA		    0x0185
#define GUI_IMG_BTDEVICELIST                    0x0186
#define GUI_IMG_BTDEVICELISTA		            0x0187


/*system Setting*/
#define GUI_IMG_MENU_SETUP		                0x0190
#define GUI_IMG_MENU_SETUPA		                0x0191
#define GUI_IMG_MENU_LANG		                0x0192
#define GUI_IMG_MENU_LANGA		                0x0193
#define GUI_IMG_MENU_LCD		                0x0194
#define GUI_IMG_MENU_LCDA		                0x0195
#define GUI_IMG_MENU_INFO		                0x0196
#define GUI_IMG_MENU_INFOA		                0x0197
#define GUI_IMG_MENU_ALARM		                0x0198
#define GUI_IMG_MENU_ALARMA		                0x0199
#define GUI_IMG_MENU_ALARM_AUDIO                0x019a
#define GUI_IMG_MENU_ALARM_AUDIOA               0x019b

#define GUI_IMG_CLOCKICON		                0x01F0
#define GUI_IMG_CLOCKBG		                    0x01F1

#define GUI_IMG_CONTRSTW		                0x01F6
#define GUI_IMG_CONTRSTB		                0x01F7

#define GUI_IMG_BKLIGHT		                    0x01FC
#define GUI_IMG_BKLIGHTA		                0x01FD

#define GUI_IMG_BKCOLOR		                    0x01FE
#define GUI_IMG_BKCOLORA		                0x01FF

#define GUI_IMG_POWER                           0x0203
#define GUI_IMG_ALARM_PICS                      0x0222

#define GUI_IMG_CDEL_NO                         0x0240
#define GUI_IMG_CDEL_YES                        0x0241
#define GUI_IMG_LOCK		                    0x025A    
#define GUI_IMG_FWBCODE		                    0x025C  
#define GUI_IMG_SELECTICONS                     0x0264 
#define GUI_IMG_CALL_TYPE                       0x0265
#define GUI_IMG_CALL_TYPEA                      0x0266
#define GUI_IMG_MENU_CALENDAR                   0x0300
#define GUI_IMG_MENU_CALENDARA                  0x0301

#define GUI_IMG_FOLDER_LIST                     0x0310
#define GUI_IMG_FOLDER_LISTA                    0x0311

#define GUI_IMG_BLUETOOTH                       0x0320
#define GUI_IMG_BT_CONNECT                      0x0321

/*USB PlugIn*/
#define GUI_IMG_USBPLUGIN                       0x0330
#define GUI_IMG_USBPLUGINA                      0x0331

/*background image*/
#define GUI_IMG_BG_MAIN							0x0340
#define GUI_IMG_BG_LIST                         0x0341
#define GUI_IMG_BG_WEEK							0x0342

/*********************************************
 *
 *  Led pattern defines
 *
 *********************************************/
#define GUI_LED_POWEROFF                        0x01
#define GUI_LED_POWERON                         0x02
#define GUI_LED_LOWPOWER                        0x03
#define GUI_LED_IDLE                            0x04
#define GUI_LED_LIST                            0x05
#define GUI_LED_SLIDER                          0x06
#define GUI_LED_NONE                            0x07

#define GUI_LED_MUSIC_STOP                      0x10
#define GUI_LED_MUSIC_PLAY                      0x11
#define GUI_LED_MUSIC_LOOP                      0x12
#define GUI_LED_MUSIC_FOLLOW                    0x13

#define GUI_LED_FM_PLAY                         0x20
#define GUI_LED_FM_SEEK                         0x21
#define GUI_LED_FM_NOSTATION                    0x22

#define GUI_LED_LINEIN_STOP                     0x28
#define GUI_LED_LINEIN_PLAY                     0x29

#define GUI_LED_REC_STOP                        0x30
#define GUI_LED_REC_PAUSE                       0x31
#define GUI_LED_REC_MIC                         0x32
#define GUI_LED_REC_FM                          0x33
#define GUI_LED_REC_PLAY                        0x34

#define GUI_LED_BT_CLOSED                       0x40
#define GUI_LED_BT_IDLE                         0x41
#define GUI_LED_BT_SCAN                         0x42
#define GUI_LED_BT_CONNECT                      0x43
#define GUI_LED_BT_CALL                         0x44
#define GUI_LED_BT_MUSIC                        0x45
#define GUI_LED_BT_FILE                         0x46
#define GUI_LED_BT_CALL_INCOMING                0x47

#define GUI_LED_CALENDAR                        0x50
#define GUI_LED_ALARM                           0x51
#define GUI_LED_CALIB_KEY1                      0x52
#define GUI_LED_CALIB_KEYN                      0x53

#define GUI_LED_USB_DISK                        0x58
#define GUI_LED_USB_AUDIO                       0x59

#define GUI_LED_TEST_PATTERN1                   0x61
#define GUI_LED_TEST_PATTERN2                   0x62
#define GUI_LED_TEST_SUCESS                     0x63
#define GUI_LED_TEST_FAIL                       0x64
#define GUI_LED_BT_MODE                         0x65

/*******************************************************
*                String Defines                        *
*                                                      *
*                                                      *
********************************************************/
#define     GUI_STR_MUSIC                       0x0001       
#define     GUI_STR_FM                          0x0002       
#define     GUI_STR_BT                          0x0003       
#define     GUI_STR_RECORD                      0x0004       
#define     GUI_STR_SETTING                     0x0005       
#define     GUI_STR_EXIT                        0x0006       
#define     GUI_STR_CALENDAR                    0x0007
#define     GUI_STR_STANDBY                     0x0008       

#define     GUI_STR_KANS                        0x0020 /*   甲、乙、丙、丁、戊、己、庚、辛、壬、癸*/
#define     GUI_STR_CHIS                        0x0021 /*子、丑、寅、卯、辰、巳、午、未、申、酉、戌、亥*/      
#define     GUI_STR_TERMS                       0x0022 /*24节气*/
#define     GUI_STR_NUMBERS                     0x0023 /*汉字数字*/
#define     GUI_STR_LUNAR                       0x0024 /* 年月日闰初廿 */
#define     GUI_STR_CHUXI                       0x0025 /* 除夕 */
#define     GUI_STR_EASTER                      0x0026 /* 复活节 */
#define     GUI_STR_FESTIVALDAY                 0x0027

#define     GUI_STR_SUNDAY                      0x0040       
#define     GUI_STR_MONDAY                      0x0041       
#define     GUI_STR_TUESDAY                     0x0042       
#define     GUI_STR_WEDNESDAY                   0x0043       
#define     GUI_STR_THURSDAY                    0x0044       
#define     GUI_STR_FRIDAY                      0x0045       
#define     GUI_STR_SATURDAY                    0x0046       

#define     GUI_STR_BACKTODAY                   0x0050
#define     GUI_STR_SETDAY                      0x0051
#define     GUI_STR_LOADDATES                   0x0052
#define     GUI_STR_DATELIST                    0x0053
#define     GUI_STR_FESTIVAL                    0x0054
#define     GUI_STR_TERMLIST                    0x0055

#define     GUI_STR_RESERROR                    0x0056

//setting
#define     GUI_STR_ALARMSETTING		        0x008f
#define     GUI_STR_DISPLAYSET  		        0x0090
#define     GUI_STR_SYSTEMINFO  		        0x0091
#define     GUI_STR_SYSTEMCLOCK		            0x0092
#define     GUI_STR_RECORDTYPE		            0x0093
#define     GUI_STR_BACKLIGHTTIME	            0x0094
#define     GUI_STR_BACKLIGHT		            0x0095
#define     GUI_STR_BACKLIGHTCOLOR	            0x0096
#define     GUI_STR_LANGUAGESELECT	            0x0097
#define     GUI_STR_SHUTDOWNSETTING             0x0098
#define     GUI_STR_REPLAYMODE		            0x0099
#define     GUI_STR_CONTRAST		            0x009A
#define     GUI_STR_BTSET		                0x009B
#define     GUI_STR_ONLINEDEVICE	            0x009C
#define     GUI_STR_MEMORYINFO                  0x009D
#define     GUI_STR_FIRMWAREVERSION             0x009E
#define     GUI_STR_FIRMWAREUPGRADE		        0x009F
#define     GUI_STR_DATEFORMATSET  		        0x00A0

#define     GUI_STR_FMSET		                0x00A1
#define     GUI_STR_DISKFMT		                0x00A2

#define     GUI_STR_CALIBKEY                    0x00A3
#define     GUI_STR_CALIBKEY_1                  0x00A4
#define     GUI_STR_CALIBKEY_N                  0x00A5
#define     GUI_STR_CALIBKEY_SUCC               0x00A6
#define     GUI_STR_CALIBKEY_FAIL               0x00A7

#define     GUI_STR_POWEOFFTIME		            0x00A8
#define     GUI_STR_MANUALREPLAY		        0x00A9
#define     GUI_STR_AUTOREPLAY		            0x00AA

#define     GUI_STR_VISIABLEOPEN                0x00AC     /*BT 可见(开)*/
#define     GUI_STR_VISIABLECLOSE               0x00AD     /*BT 可见(关)*/
#define     GUI_STR_DEVICEINFO                  0x00AE     /*BT 设备信息*/


#define     GUI_STR_VOICERECORD	                0x00B0
#define     GUI_STR_MUSICECORD		            0x00B1
#define     GUI_STR_AMRECORD		            0x00B2
#define     GUI_STR_REDLIGHT		            0x00B3
#define     GUI_STR_GREENLIGHT		            0x00B4
#define     GUI_STR_YELLOWLIGHT		            0x00B5
#define     GUI_STR_BLUELIGHT		            0x00B6
#define     GUI_STR_PURPLELIGHT		            0x00B7
#define     GUI_STR_CYANLIGHT		            0x00B8
#define     GUI_STR_WHITELIGHT                  0x00B9

#define     GUI_STR_FMT16		                0x00BA
#define     GUI_STR_FMT32		                0x00BB
#define     GUI_STR_WAITPASS		            0x00BC
#define     GUI_STR_MREADERR		            0x00BD
#define     GUI_STR_MWRIERR		                0x00BE
#define     GUI_STR_CDELFILE	                0x00BF
#define     GUI_STR_CDELALL		                0x00C0
#define     GUI_STR_FMERROR		                0x00C1
#define     GUI_STR_TIMEAFFECT		            0x00C2
#define     GUI_STR_PLAYINGRIGHTHS 		        0x00C3
#define     GUI_STR_SETTIME		                0x00C4
#define     GUI_STR_ADJUSTCLOCK		            0x00C5
#define     GUI_STR_ANTICLOCKWISE		        0x00C6
#define     GUI_STR_SOMELICENSE		            0x00C7
#define     GUI_STR_FREQLIST		            0x00C8
#define     GUI_STR_AUTOSEARCH		            0x00C9
#define     GUI_STR_SAVEFREQ	                0x00CA
#define     GUI_STR_FREQRECORD		            0x00CB
#define     GUI_STR_FREQDELET		            0x00CC
#define     GUI_STR_FMCLOCK  		            0x00CD
#define     GUI_STR_NORMALBAND		            0x00CE
#define     GUI_STR_JAPANBAND		            0x00CF
#define     GUI_STR_CDELING		                0x00D0
#define     GUI_STR_CDELFAIL	                0x00D1
#define     GUI_STR_CARDPULL		            0x00D2
#define     GUI_STR_FORMATERR		            0x00D3
#define     GUI_STR_NOLICENS		            0x00D4
#define     GUI_STR_REQLICEN		            0x00D5
#define     GUI_STR_FMTIMER 		            0x00D6

#define     GUI_STR_SYSTEMRESET                 0x00D7  
#define     GUI_STR_UPGRADING                   0x00D8
#define     GUI_STR_UPGRADE_COMPLETE            0x00D9
#define     GUI_STR_RESET_COMPLETE              0x00DA
#define     GUI_STR_RESTART                     0x00DB

/*Playlist ref*/
#define     GUI_STR_PLAYLIST		            0x00E0
#define     GUI_STR_ARTISTLIST		            0x00E1
#define     GUI_STR_ALBUMLIST		            0x00E2
#define     GUI_STR_GENRE		                0x00E3
#define     GUI_STR_COMPOSERS		            0x00E4
#define     GUI_STR_LICENSEINFO		            0x00E5
#define     GUI_STR_RINIT		                0x00E6
#define     GUI_STR_MDIRFULL		            0x00E7
#define     GUI_STR_MPWROUT		                0x00E8
#define     GUI_STR_USBDISK		                0x00E9
#define     GUI_STR_MEDIADEV		            0x00EA
#define     GUI_STR_OPLAYCNT		            0x00EB
#define     GUI_STR_EXPDATE		                0x00EC
#define     GUI_STR_REMAINTM		            0x00ED
#define     GUI_STR_REMAINST		            0x00EE
#define     GUI_STR_DELCKBCK		            0x00EF
#define     GUI_STR_DISCKBCK		            0x00F0
#define     GUI_STR_TDAY		                0x00F1
#define     GUI_STR_TTIMES		                0x00F2
#define     GUI_STR_HOURS		                0x00F3
#define     GUI_STR_NOLIMIT		                0x00F4
#define     GUI_STR_REQSETDT		            0x00F5
#define     GUI_STR_PLAYCNT		                0x00F6
#define     GUI_STR_BEGINDAT		            0x00F7
#define     GUI_STR_MFILEOUT		            0x00F8
#define     GUI_STR_DFILEOUT		            0x00F9
#define     GUI_STR_CFILEOUT		            0x00FA
#define     GUI_STR_CREADERR		            0x00FB
#define     GUI_STR_RSAVING		                0x00FC

// MUSIC MENU
#define     GUI_STR_REPEAT                      0x0100
#define     GUI_STR_EQUALIZER                   0x0101
#define     GUI_STR_FOLLOW                      0x0102
#define     GUI_STR_REPLAY                      0x0103
#define     GUI_STR_REPLAYTIMES                 0x0104
#define     GUI_STR_REPLAYGAP                   0x0105
#define     GUI_STR_LOCALFOLDER                 0x0106
#define     GUI_STR_CARDFOLDER                  0x0107
#define     GUI_STR_SELECTFILE                  0x0108
#define     GUI_STR_OPENFMOUT                   0x0109
#define     GUI_STR_CLOSEFMOUT                  0x010A
#define     GUI_STR_SETFMOUT	                0x010B
#define     GUI_STR_OPENLYRIC                   0x010C
#define     GUI_STR_CLOSELYRIC                  0x010D
#define     GUI_STR_SET_ALARM                   0x010E
#define     GUI_STR_FILE_DELETE                 0x010F

// EQ MODE
#define     GUI_STR_NORMAL 		                0x0110
#define     GUI_STR_BASS		                0x0111
#define     GUI_STR_DANCE		                0x0112
#define     GUI_STR_CLASSIC		                0x0113
#define     GUI_STR_TREBLE		                0x0114
#define     GUI_STR_PARTY	    	            0x0115
#define     GUI_STR_POP	        	            0x0116
#define     GUI_STR_ROCK	    	            0x0117
// LOOP MODE
#define     GUI_STR_SEQENCE		                0x0118
#define     GUI_STR_REPEATONE		            0x0119
#define     GUI_STR_FOLDERPLAY		            0x011A
#define     GUI_STR_REPEATFOLDE		            0x011B
#define     GUI_STR_REPEATALL		            0x011C
#define     GUI_STR_RANDOM		                0x011D
#define     GUI_STR_INTRO		                0x011E


//BT
#define     GUI_STR_BTACTIVE                    0x011F    /*Bt Activing...*/
#define     GUI_STR_BTDEACTIVE                  0x0120    /*Bt Activing...*/

#define     GUI_STR_BTACTIVEING                 0x0121    /*Bt Activing...*/
#define     GUI_STR_BTACTIVEFAILED              0x0122    /*Bt Active Failed!*/
#define     GUI_STR_BTACTIVESUCCESS             0x0123    /*Bt Active Sucess!*/
#define     GUI_STR_BTCLOSEING                  0x0124    /*Bt Closing...*/
#define     GUI_STR_BTCLOSESUCESS               0x0125    /*Bt Closed!*/

#define     GUI_STR_BTVISIABLE                  0x0126    /*Bt Visiable...*/
#define     GUI_STR_BTINVISIABLE                0x0127    /*Bt Invisiable...*/

#define     GUI_STR_BTPAIREDSUCCESS             0x0128    /*Bt Paired Success!*/
#define     GUI_STR_BTPAIREDFAILED              0x0129    /*Bt Paired Failed!*/

#define     GUI_STR_BTHFPCONNECTED              0x012A    /*HFP Connect Success...*/
#define     GUI_STR_BTA2DPCONNECTED             0x012B    /*A2DP Connect Success...*/

#define     GUI_STR_BT_CONNECTING               0x012C
#define     GUI_STR_BT_CONNECT_FAIL             0x012D

#define     GUI_STR_DEVICELIST                  0x012E     /*BT 设备列表*/
#define     GUI_STR_SETPATH                     0x012F     /*BT 设置路径*/

#define     GUI_STR_NODEVICELIST                0x0131     /*BT 没有设备*/

#define     GUI_STR_BTHFPCONNECT                0x0133     /*BT 连接hfp */
#define     GUI_STR_BTA2DPCONNECT               0x0134     /*BT 连接a2dp */
#define     GUI_STR_BTSWITCHAUDIO               0x0135     /*BT switch audio */
#define     GUI_STR_BTDISCONNECT                0x0136     /*BT 断开*/
#define     GUI_STR_BTSETDEFAULT                0x0137     /*BT 设置默认设备*/
#define     GUI_STR_BTDELETE                    0x0138     /*BT 删除当前设备*/
#define     GUI_STR_BTDELETEALL                 0x0139     /*BT 删除全部*/
#define     GUI_STR_BTRECORD                    0x013A     /*BT 电话录音 */
#define     GUI_STR_BTHID                       0x013B     /*BT 远程控制 */

#define     GUI_STR_BTHIDCONNECT                0x013C     /*BT 远程控制已连接 */
#define     GUI_STR_BTHIDCONTROL                0x013D     /*BT 远程控制 */


/* bt state */
#define    GUI_STR_BT_CLOSED                    0x0140
#define    GUI_STR_BT_IDLE                      0x0141
#define    GUI_STR_BT_SCAN                      0x0142
#define    GUI_STR_BT_CONNECT                   0x0143
#define    GUI_STR_BT_CALL                      0x0144
#define    GUI_STR_BT_PLAY                      0x0145
#define    GUI_STR_BT_FILE                      0x0146
#define    GUI_STR_BT_AUDIO                     0x0147

#define    GUI_STR_CALL_INCOMING                0x0148
#define    GUI_STR_CALL_OUTGOING                0x0149

#define    GUI_STR_CALL_UNKNOW_NUMBER           0x014a

//add by wuxiang
#define    GUI_STR_CALL_END                     0x150
#define    GUI_STR_BTDISCONNECTING              0x151
#define    GUI_STR_BT_FILE_ABORT                0x152
#define    GUI_STR_SD_OUT                       0x153
#define    GUI_STR_FILE_COMPLETE                0x154
#define    GUI_STR_PHONE_BOOK                   0x155
#define    GUI_STR_CALL_HISTORY                 0x156
#define    GUI_STR_BT_BONDING                   0x157
#define    GUI_STR_GET_PHONE_BOOK               0x158
#define    GUI_STR_PB_DOWNLOADING               0x159
#define    GUI_STR_PB_DOWNLOAD_SUCCESS          0x15a
#define    GUI_STR_PB_DOWNLOAD_FAILED           0x15b
#define    GUI_STR_BT_INQUIRY                   0x15c
#define    GUI_STR_BT_INQUIRING                 0x15d
#define    GUI_STR_BT_PAIRING                   0x15e
#define    GUI_STR_BT_INQUIRY_NO_DEVICE         0x15f
#define    GUI_STR_BT_MAP                       0x160
#define    GUI_STR_BT_PB_SYNC                   0x161
#define    GUI_STR_PB_SEARCH                    0x162
#define    GUI_STR_NO_PB_DATA					0x163
#define    GUI_STR_CHARGING						0x164
#define    GUI_STR_FULL_CHARGING				0x165
#define    GUI_STR_SEND_MSG                     0x166
//add end
/* record */
#define     GUI_STR_SELECTFOLDER                0x016C
#define     GUI_STR_MICREC                      0x016D
#define     GUI_STR_FMREC                       0x016E
#define     GUI_STR_LONGREC                     0x016F
#define     GUI_STR_MIDREC		                0x0170
#define     GUI_STR_FINEREC		                0x0171
#define     GUI_STR_RECORDERROR                 0x0172
#define     GUI_STR_RECORDOK                    0x0173

/*alarm seting*/
#define     GUI_STR_ALARMOPEN                   0x0180  /*alarm open*/
#define     GUI_STR_ALARMCLOSE                  0x0181  /*alarm close*/
#define     GUI_STR_ALARMCLOCK                  0x0183  /*alarm clock*/
#define     GUI_STR_ALARMTIMES                  0x0184  /*alarm times*/
#define     GUI_STR_ALARMFILE                   0x0185  /*alarm ring*/
#define     GUI_STR_ALARMFREQ                   0x0186  /*alarm ring*/
#define     GUI_STR_ALARMMUSIC                  0x0187  /*music alarm*/
#define     GUI_STR_ALARMFM                     0x0188  /*time fm*/
#define     GUI_STR_ALARMRECORD                 0x0189  /*time fm record*/
#define     GUI_STR_ALARMNOREPEAT               0x018a  
#define     GUI_STR_ALARMEVERYDAY               0x018b  
#define     GUI_STR_ALARMWORKDAY                0x018c  
#define     GUI_STR_ALARMDAYS                   0x018d  
#define     GUI_STR_ALARM_AUDIO1                0x018e
#define     GUI_STR_ALARM_AUDIO2                0x018f  
#define     GUI_STR_ALARM_AUDIO3                0x0190  

// folder list
#define     GUI_STR_OPENFOLDER                  0x0200
//#define     GUI_STR_SELECTFOLDER                0x0201
#define     GUI_STR_UPFOLDER                    0x0202
#define     GUI_STR_TFLASHROOT                  0x0203
#define     GUI_STR_USBROOT                     0x0204


/*USB PlugIn*/
#define GUI_STR_USBPLUGIN_UMASS                 0x0210
#define GUI_STR_USBPLUGIN_UAUDIO                0x0211

/* LANGUAGE STRING DEFINE */
#define     GUI_STR_SIMPLECHINESE	            0x0301
#define     GUI_STR_ENGLISH		                0x0302
#define     GUI_STR_TRADCHINESE		            0x0303
#define     GUI_STR_JAPANESE		            0x0304
#define     GUI_STR_KOREAN		                0x0305
#define     GUI_STR_LANG_FRENCH                 0x0306

// menu defines
#define     GUI_MENU_MAIN                       0x01

#define     GUI_MENU_MUSIC                      0x02
#define     GUI_MENU_MUSIC_EQ                   0x04
#define     GUI_MENU_MUSIC_LOOP                 0x05

#define     GUI_MENU_FM                         0x08

#define     GUI_MENU_BT                         0x0a
#define     GUI_MENU_BTDEVICEOPTION             0x0b

#define     GUI_MENU_RECORD                     0x0e

#define     GUI_MENU_CONFIG                     0x10
#define     GUI_MENU_CFG_LCD                    0x11
#define     GUI_MENU_CFG_LANG                   0x12
#define     GUI_MENU_DEVICE_INFO                0x13

#define     GUI_MENU_DATE_FORMAT                0x14

#define     GUI_MENU_SEPARATOR                  0x15

#define     GUI_MENU_TIMER                      0x20
#define     GUI_MENU_TIMERTYPE                  0x21
#define     GUI_MENU_TIMERAUDIO                 0x22

#define     GUI_MENU_CALENDAR                   0x24

#define     GUI_MENU_FOLDERLIST                 0x28

#define     GUI_MENU_USBPLUGIN                  0x30


#define GUI_AUDIO_LOWPOWER                      0x01
#define GUI_AUDIO_POWERON                       0x02
#define GUI_AUDIO_POWEROFF                      0x03
#define GUI_AUDIO_VOL_MAXORMIN                  0x04
#define GUI_AUDIO_REJECT_CALL                   0x05
#define GUI_AUDIO_RECALL                        0x06
#define GUI_AUDIO_LANGUAGE                      0x07
#define GUI_AUDIO_ALARM1                        0x11
#define GUI_AUDIO_ALARM2                        0x12
#define GUI_AUDIO_ALARM3                        0x13

#define GUI_AUDIO_MUSIC_FAST_FORWARD            0x21

#define GUI_AUDIO_FM_SEARCH                     0x31

#define GUI_AUDIO_NUMBER_0                     0x32
#define GUI_AUDIO_NUMBER_1                     0x33
#define GUI_AUDIO_NUMBER_2                     0x34
#define GUI_AUDIO_NUMBER_3                     0x35
#define GUI_AUDIO_NUMBER_4                     0x36
#define GUI_AUDIO_NUMBER_5                     0x37
#define GUI_AUDIO_NUMBER_6                     0x38
#define GUI_AUDIO_NUMBER_7                     0x39
#define GUI_AUDIO_NUMBER_8                     0x3A
#define GUI_AUDIO_NUMBER_9                     0x3B
#define GUI_AUDIO_INCOMING_CALL                0x3C

#define GUI_AUDIO_BT_POWERON                    0x51
#define GUI_AUDIO_BT_POWEROFF                   0x52
#define GUI_AUDIO_BT_RING                       0x53
#define GUI_AUDIO_BT_SCAN                       0x54
#define GUI_AUDIO_BT_CONNECT                    0x55

#define GUI_AUDIO_BT_DISCONNECT                 0x56
#define GUI_AUDIO_LINEIN_PLAY                   0x57
#define GUI_AUDIO_TFCARD_PLAY                   0x58
#define GUI_AUDIO_FM_PLAY                       0x59
#define GUI_AUDIO_MASS_STORAGE                  0x5a
#define GUI_AUDIO_SWITCHOFF_CHARGE              0x5b
#define GUI_AUDIO_CALL_WARNING                  0x5c
#define GUI_AUDIO_BT_ANSWERCALL                 0x5d

#define GUI_AUDIO_FT_CODEC                      0x5e
#define GUI_AUDIO_BT_MAXVOL                     0x5f
#define GUI_AUDIO_BT_MINVOL                     0x60
#define GUI_AUDIO_BT_RECOGNITION                0x61

#endif/*_GUI_CONST_H_*/

