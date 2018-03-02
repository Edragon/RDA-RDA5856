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
/*                                                                            */
/* FILE NAME                                                                  */
/*       event.h                                                              */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   event constant definitions and macros.                                   */
/*                                                                            */
/******************************************************************************/

#ifndef __EVENT_H__
#define __EVENT_H__
//#include "cfw_multi_sim.h"
//
// System level Events
//
#define EV_SYS_BASE                   0
#define EV_SYS_END_                   50

#define EV_SHL_CFW_STATUS_IND         (EV_SYS_BASE+10)
#define EV_UART2_DATA                 (EV_SYS_BASE+11)
#define EV_OS_EXCLUSIVE               (EV_SYS_BASE+12)

//
// DM Events.
//
#define EV_DM_BASE                   (50)
#define EV_DM_END_                   (100)

//
// PM Evetns.
//
#define EV_PM_BASE                   (100)
#define EV_PM_END_                   (150)

//
// FS Events
//
#define EV_FS_BASE                   (200)
#define EV_FS_END_                   (300)

#define EV_USB_BASE                  (300)
#define EV_USB_END_                  (350)

#define EV_CFW_START_                (500)

#define EV_CFW_NW_RSP_BASE           (500)
#define EV_CFW_NW_RSP_END_           (550)

#define EV_CFW_CC_RSP_BASE           (550)
#define EV_CFW_CC_RSP_END_           (600)

#define EV_CFW_SS_RSP_BASE           (600)
#define EV_CFW_SS_RSP_END_           (650)

#define EV_CFW_SIM_RSP_BASE          (700)
#define EV_CFW_SIM_RSP_END_          (760)

#define EV_CFW_SMS_RSP_BASE          (760)
#define EV_CFW_SMS_RSP_END_          (800)

#define EV_CFW_PBK_RSP_BASE          (800)
#define EV_CFW_PBK_RSP_END_          (850)

#define EV_CFW_GPRS_RSP_BASE         (850)
#define EV_CFW_GPRS_RSP_END_         (900)

#define EV_CFW_FAX_RSP_BASE          (900)
#define EV_CFW_FAX_RSP_END_          (950)

#define EV_CFW_TCPIPUDP_RSP_BASE     (1000)
#define EV_CFW_TCPIPUDP_RSP_END_     (1050)

#define EV_CFW_HTTP_RSP_BASE         (1050)
#define EV_CFW_HTTP_RSP_END_         (1100)

#define EV_CFW_SHELL_RSP_BASE        (1100)
#define EV_CFW_SHELL_RSP_END_        (1150)

#define EV_CFW_TM_RSP_BASE           (1150) // Test Module
#define EV_CFW_TM_RSP_END_           (1200)

#define EV_RMC_BASE                  (1200) // Remote Control Module
#define EV_RMC_END_                  (1300)

#ifdef WIFI_SUPPORT
#define EV_CFW_WIFI_BASE           (1300)
#define EV_CFW_WIFI_END_           (1350)
#endif

#if defined(__SLIMMMI_TCPIP__)
#define EV_CFW_SOC_BASE           (1350)
#define EV_CFW_SOC_END_           (1400)
#endif /* __SLIMMMI_TCPIP__ */

#ifdef BT_TETHER_SUPPORT
#define EV_BT_TETHER_BASE           (1400) 
#define EV_BT_TETHER_END_           (1450)
#endif

#define EV_DLNA_BASE				(1500)
#define EV_DLNA_END_				(1600)

#define EV_WIFIMGR_BASE				(1600)
#define EV_WIFIMGR_END_				(1650)


#define EV_YOC_BASE					(1651)
#define EV_YOC_END_					(1700)

#if defined(SPEAKER_SUPPORT)
#define EV_SPEAKER_BASE				(1701)
#define EV_SPEAKER_END				(1750)
#endif

// ... 
// 1200 -- 5000 reserved.
//

#define EV_UART_BASE                 (2000)
#define EV_UART_END_				 (3000)	

#define EV_CFW_IND_BASE              (5000)
#define EV_CFW_IND_END_              (5050)

#define EV_CFW_END_                  (6000)

//
// AFW event.
//
#define AFW_EV_BASE                  (7000)
#define AFW_EV_END_                  (8000)

//
// UI FW event
//
#define UI_FW_EV_BASE               (9000)
#define UI_FW_EV_END_               (10000)
// Bluetooth event
#define EV_BT_BASE                      (11000)
#define EV_BT_END                       (12000)

// User Event
#define EV_MMI_EV_BASE              (13000) //2 NOTE: Need to modify msg id in res.bin
#define EV_MMI_EV_END_              (15000) 

#define EV_BT_STACK_BASE              (0x8000)
#define EV_BT_STACK_END               (0x8fff)


#define CSW_SRV_EV_END_             (0x000FFFFF)

//
// User Event: 0x00100000 -- 0xA0000000
//
//#define EV_MMI_EV_BASE              0x00100000
//#define EV_MMI_EV_END_              0xA0000000

// ... 
// 0xA0000000 -- 0xF0000000 reserved by system.
// 

//
// CSW Private Event 0xF0000001 --  0xF000F000
// used by CSW internal only
// 
#define EV_CSW_PRV_BASE             0xF0000001
#define EV_CSW_PRV_END_             0xF000F000

//
// CSW Private Event 0xF000F001 --  0xFFFFF000
// 
#define EV_CSW_REQ_BASE             0xF000F001
#define EV_CSW_REQ_END_             0xFFFFF000 // don't change this value!


// ... 
// 0xFFFFF000 -- 0xFFFFFFF reserved by system.
//

#define EV_EMU_BASE                 0xF0000000

//
// Helper Macros
//
#define IS_CFW_EVNET(evId)                       (((evId)>=EV_CFW_START_) && (evId)<EV_CFW_END_)

//
// DM & TIM Event.
// 
#define EV_DM_POWER_ON_IND                       (EV_DM_BASE+1)
#define EV_DM_POWER_KEY                            (EV_DM_BASE+2)
#define EV_DM_POWER_OFF_IND                      (EV_DM_BASE+5) 
#define EV_TIM_ALRAM_IND                         (EV_DM_BASE+6)
#define EV_KEY_DOUBLE_CLICK                  (EV_DM_BASE+7)
#define EV_DM_UART_RECEIVE_DATA_IND              (EV_DM_BASE+8)
#define EV_DM_EXTI_IND                           (EV_DM_BASE+9) 
#define EV_DM_GPIO_IND                           (EV_DM_BASE+10)
#define EV_DM_RECORD_END_IND                     (EV_DM_BASE+11) 
#define EV_DM_STREAM_END_IND                     (EV_DM_BASE+12)
#define EV_DM_GPADC_IND                          (EV_DM_BASE+13)
#define EV_TIM_SET_TIME_IND                      (EV_DM_BASE+14)
			
//guoh add speech indication, if start call, this indication will send to mmi task 			
#define EV_DM_SPEECH_IND                         (EV_DM_BASE+15)

#define EV_DM_AUDIO_OPER_MSG_START               (EV_DM_BASE+16)

#define EV_DM_AUDIO_CONTROL_IND                  (EV_DM_BASE+16)
#define EV_DM_SET_AUDIO_VOLUME_IND               (EV_DM_BASE+17)
#define EV_DM_SET_MIC_GAIN_IND                   (EV_DM_BASE+18)
#define EV_DM_SET_AUDIO_MODE_IND                 (EV_DM_BASE+19)

#define EV_DM_AUD_SETUP_IND                      (EV_DM_BASE+20)
#define EV_DM_AUD_STREAM_START_IND               (EV_DM_BASE+21)
#define EV_DM_AUD_STREAM_STOP_IND                (EV_DM_BASE+22)
#define EV_DM_AUD_STREAM_PAUSE_IND               (EV_DM_BASE+23)
#define EV_DM_AUD_STREAM_RECORD_IND              (EV_DM_BASE+24)
#define EV_DM_AUD_TEST_MODE_SETUP_IND            (EV_DM_BASE+25)
#define EV_DM_AUD_FORCE_RECV_MIC_SEL_IND         (EV_DM_BASE+26)
#define EV_DM_AUD_SET_CUR_OUTPUT_DEVICE_IND      (EV_DM_BASE+27)
#define EV_DM_AUD_MUTE_OUTPUT_DEVICE_IND         (EV_DM_BASE+28)
#define EV_DM_AUD_LOUDSPK_WITH_EP_IND            (EV_DM_BASE+29)

#define EV_DM_VOIS_RECORD_START_IND         (EV_DM_BASE+30)
#define EV_DM_VOIS_RECORD_STOP_IND         (EV_DM_BASE+31)

#define EV_DM_EAR_PEACE_DETECT_IND                (EV_DM_BASE+32)

#define EV_DM_ARS_SETUP_IND                      (EV_DM_BASE+33)
#define EV_DM_ARS_RECORD_IND                     (EV_DM_BASE+34)
#define EV_DM_ARS_PAUSE_IND                      (EV_DM_BASE+35)
#define EV_DM_ARS_STOP_IND                       (EV_DM_BASE+36)

#define EV_DM_AUDIO_OPER_MSG_END                 (EV_DM_BASE+37)

#define EV_DM_USB_PLUG_IN                   (EV_DM_BASE+38)

// don't change the following definition
#define EV_KEY_CALIB                             (EV_DM_BASE+39)  
#define EV_KEY_DOWN                              (EV_DM_BASE+40)  
#define EV_KEY_PRESS                             (EV_DM_BASE+41)
#define EV_KEY_UP                                (EV_DM_BASE+42)
#define EV_KEY_LONG_PRESS                        (EV_DM_BASE+43)
#define EV_RTC                                   (EV_DM_BASE+44)  
#define EV_TIMER                                 (EV_DM_BASE+45)
#define EV_IRC_PRESS							 (EV_DM_BASE+46)
#define EV_ALARM                                 (EV_DM_BASE+47)
#define EV_LINE_IN							     (EV_DM_BASE+48)
#define EV_DM_XXX_END                            (EV_DM_BASE+49)

#if (EV_DM_XXX_END >= (EV_DM_END_))
#error 'Error: invalid EV_DM_XXX_END definition!'
#endif

//
// PM Event
//
#define EV_PM_BC_IND                             (EV_PM_BASE+1)
#define EV_PM_XXX_IND_END                        (EV_PM_BASE+2)

#if (EV_PM_XXX_IND_END >= (EV_PM_END_))
#error 'Error: invalid EV_CFW_XXX_IND definition!'
#endif
//
// FS Events
//
#define EV_FS_TFLASH_DETECT                      (EV_FS_BASE+1)
#define EV_FS_USB_PLUG_DETECT                    (EV_FS_BASE+2)
#define EV_FS_ASYN_READ_REQ                      (EV_FS_BASE+3)
#define EV_FS_ASYN_WRITE_REQ                     (EV_FS_BASE+4)
#define EV_FS_UMSS_TRANSPORT_ID                  (EV_FS_BASE+5)  
#define EV_FS_UTLS_EVENT_ID                      (EV_FS_BASE+6)  
#define EV_FS_INIT_FILESYSTEM                    (EV_FS_BASE+7)
#define EV_FS_UMSS_DETECT                       (EV_FS_BASE+8)
#define EV_FS_UMSS_PLUG_OUT                      (EV_FS_BASE+9)
#define EV_FS_UDISK_READY						 (EV_FS_BASE+10)
#define EV_FS_UDISK_REMOVE						 (EV_FS_BASE+11)
#define EV_FS_USB_INIT							 (EV_FS_BASE+12)
#define EV_FS_USB_TRANSFERRED					 (EV_FS_BASE+13)
#define EV_FS_USB_PLUG_OUT                		 (EV_FS_BASE+14)
#define EV_FS_USB_VOL_CHANGE                	 (EV_FS_BASE+15)
#ifdef LINEIN_RECORDER_SUPPORT
#define EV_FS_LINEIN_RECORD                             (EV_FS_BASE+16)
#endif

#define EV_FS_XXX_END                            (EV_FS_BASE+20)
#if (EV_FS_XXX_END >= (EV_FS_END_))
#error 'Error: invalid EV_FS_XXX_END definition!'
#endif


//
// USB Events
//
#define EV_USB_UTLS_EVENT_ID                     (EV_USB_BASE+1)
#define EV_USB_MSS_EVENT_ID                      (EV_USB_BASE+2)
#define EV_USB_AUD_EVENT_ID                      (EV_USB_BASE+3)

#define EV_USB_XXX_END                           (EV_USB_BASE+4)
#if (EV_USB_XXX_END >= (EV_USB_END_))
#error 'Error: invalid EV_USB_XXX_END definition!'
#endif

//
// CFW IND Event.
//
#define EV_CFW_INIT_IND                          (EV_CFW_IND_BASE+1)
#define EV_CFW_SRV_STATUS_IND                    (EV_CFW_IND_BASE+2) 
#define EV_CFW_NW_SIGNAL_QUALITY_IND             (EV_CFW_IND_BASE+3)
#define EV_CFW_NW_REG_STATUS_IND                 (EV_CFW_IND_BASE+5)
#define EV_CFW_CC_SPEECH_CALL_IND                (EV_CFW_IND_BASE+6)
#define EV_CFW_CC_CRSSINFO_IND                   (EV_CFW_IND_BASE+7)
#define EV_CFW_NEW_SMS_IND                       (EV_CFW_IND_BASE+8)
#define EV_CFW_SMS_INFO_IND                      (EV_CFW_IND_BASE+9)
#define EV_CFW_CC_CALL_INFO_IND                  (EV_CFW_IND_BASE+10)
#define EV_CFW_SIM_STATUS_IND                    (EV_CFW_IND_BASE+11)
#define EV_CFW_SS_USSD_IND                       (EV_CFW_IND_BASE+12)
#define EV_CFW_CC_RELEASE_CALL_IND               (EV_CFW_IND_BASE+13)
#define EV_CFW_CC_ERROR_IND                      (EV_CFW_IND_BASE+14)
#define EV_CFW_SAT_CMDTYPE_IND                   (EV_CFW_IND_BASE+15)
#define EV_CFW_PBK_IND                           (EV_CFW_IND_BASE+16)
#define EV_CFW_NW_NETWORKINFO_IND                (EV_CFW_IND_BASE+17)
#define EV_CFW_GPRS_CXT_ACTIVE_IND               (EV_CFW_IND_BASE+18)
#define EV_CFW_GPRS_CXT_DEACTIVE_IND             (EV_CFW_IND_BASE+19)
#define EV_CFW_GPRS_MOD_IND                      (EV_CFW_IND_BASE+20) 
#define EV_CFW_GPRS_STATUS_IND                   (EV_CFW_IND_BASE+21)
#define EV_CFW_GPRS_DATA_IND                     (EV_CFW_IND_BASE+22)
#define EV_CFW_TCPIP_REV_DATA_IND                     (EV_CFW_IND_BASE+23)  // TCP/IP IND
#define EV_CFW_TCPIP_CLOSE_IND                     (EV_CFW_IND_BASE+24)  // TCP/IP close IND
#define EV_CFW_TCPIP_ERR_IND                     (EV_CFW_IND_BASE+25)  // TCP/IP ERROR IND
#define EV_CFW_TCPIP_ACCEPT_IND                     (EV_CFW_IND_BASE+26)  // TCP/IP accept IND
#define EV_CFW_DNS_RESOLV_SUC_IND                (EV_CFW_IND_BASE+27)
#define EV_CFW_DNS_RESOLV_ERR_IND                (EV_CFW_IND_BASE+28)
#define EV_CFW_PPP_DATA_IND                      (EV_CFW_IND_BASE+29)
#define EV_CFW_PPP_TERMINATE_IND                 (EV_CFW_IND_BASE+30)
//Frank add used MMS/PUSH
#define EV_CFW_NEW_EMS_IND                       (EV_CFW_IND_BASE+31)
#define EV_CFW_ATT_STATUS_IND                    (EV_CFW_IND_BASE+32)
#define EV_CFW_CC_STATUS_IND                    (EV_CFW_IND_BASE+33)
#define EV_CFW_CB_MSG_IND                      (EV_CFW_IND_BASE+34)
#define EV_CFW_CB_PAGE_IND                      (EV_CFW_IND_BASE+35)
#ifdef WIFI_SUPPORT
//#define EV_CFW_WIFI_CONNECTED_IND                    (EV_CFW_IND_BASE+34)
//#define EV_CFW_WIFI_DATA_IND                    (EV_CFW_IND_BASE+35)
//#define EV_CFW_WIFI_TEST_MODE_DATA_IND (EV_CFW_IND_BASE+36)
#define EV_CFW_EXIT_IND                          (EV_CFW_IND_BASE+37)
#define EV_CFW_XX_IND_END                        (EV_CFW_IND_BASE+38)
#else
#define EV_CFW_EXIT_IND                          (EV_CFW_IND_BASE+36)
#define EV_CFW_XX_IND_END                        (EV_CFW_IND_BASE+37)

#endif
#define EV_CFW_REG_UPDATE_IND                          (EV_CFW_IND_BASE+40)

#ifdef __SUPPORT_SMSCLASS__
#define EV_CFW_NEW_SMS_PDU_IND                    (EV_CFW_IND_BASE+41)
#endif

#define EV_CFW_GPRS_CTRL_RELEASE_IND                     (EV_CFW_IND_BASE+42)
#define EV_CFW_CC_PROGRESS_IND             (EV_CFW_IND_BASE+43)
#if (EV_CFW_XX_IND_END >= (EV_CFW_IND_END_)) 
#error 'Error: invalid EV_CFW_XXX_IND definition!'.
#endif

//UART EVENT
#define UART_BT_VISIBLE      (EV_UART_BASE + 1)
#define UART_BT_INVISIBLE    (EV_UART_BASE + 2)
#define UART_XX_END          (EV_UART_BASE + 3)
#define UART_RAWDATA_REC_FINISH    (EV_UART_BASE + 4)

#if (UART_XX_END >= (EV_UART_END_)) 
#error 'Error: invalid EV_UART_END definition!'.
#endif

#ifdef __AT_GPS_AGPS__
#define EV_CFW_GPS_AGPS_INIT_IND                (EV_CFW_IND_BASE+46)
#define EV_CFW_GPS_AGPS_START_IND                (EV_CFW_IND_BASE+47)
#define EV_CFW_GPS_AGPS_RUN_IND                (EV_CFW_IND_BASE+48)
#define EV_CFW_GPS_AGPS_END_IND                (EV_CFW_IND_BASE+49)
#define EV_CFW_GPS_AGPS_RESERVED                (EV_CFW_IND_BASE+50)
#endif

#ifdef __CORE__
#define EV_CFW_AT_2_CORE_IND                (EV_CFW_IND_BASE+51)
#define EV_CFW_GPS_2_CORE_IND                (EV_CFW_IND_BASE+52)
#define EV_CFW_CORE_RESERVED_1              (EV_CFW_IND_BASE+53)
#define EV_CFW_CORE_RESERVED_2              (EV_CFW_IND_BASE+54)
#endif

#ifdef __AT_GPS__
#define EV_CFW_GPS_2_BB_IND                (EV_CFW_IND_BASE+55)
#define EV_CFW_BB_WAKEUP_GPS_IND                (EV_CFW_IND_BASE+56)
#endif
#if (EV_CFW_XX_IND_END >= (EV_CFW_IND_END_))
#error 'Error: invalid EV_CFW_XXX_IND definition!'.
#endif

//
// Emulator Event 
//
#define EV_EMU_SCM_REQ                       (EV_EMU_BASE+1)
#define EV_EMU_SCL_RSP                       (EV_EMU_BASE+2)
#define EV_EMU_SCL_REQ                       (EV_EMU_BASE+3)
#define EV_EMU_SCL_TST                       (EV_EMU_BASE+4)
#define EV_EMU_TRACE                         (EV_EMU_BASE+5)
#define EV_EMU_SYSTEM                        (EV_EMU_BASE+6)
#define EV_EMU_EXIT                          (EV_EMU_BASE+7)
#define EV_EMU_SCL_AUDIO                     (EV_EMU_BASE+10)
#define EV_EMU_LCD                           (EV_EMU_BASE+11)
#define EV_EMU_INPUT                         (EV_EMU_BASE+12)

//Frank add used MMS/PUSH
#define EV_CFW_NEW_EMS_IND                       (EV_CFW_IND_BASE+31)

//
// NW Response Event.
//
#define EV_CFW_NW_SET_BAND_RSP               (EV_CFW_NW_RSP_BASE+1)
#define EV_CFW_NW_GET_IMEI_RSP               (EV_CFW_NW_RSP_BASE+2)
#define EV_CFW_NW_GET_SIGNAL_QUALITY_RSP     (EV_CFW_NW_RSP_BASE+3)
#define EV_CFW_NW_SET_REGISTRATION_RSP       (EV_CFW_NW_RSP_BASE+4)
#define EV_CFW_NW_DEREGISTER_RSP             (EV_CFW_NW_RSP_BASE+5)
#define EV_CFW_NW_GET_AVAIL_OPERATOR_RSP     (EV_CFW_NW_RSP_BASE+6)
#define EV_CFW_NW_ABORT_LIST_OPERATORS_RSP   (EV_CFW_NW_RSP_BASE+7)
#define EV_CFW_SET_COMM_RSP                                  (EV_CFW_NW_RSP_BASE + 8)
#define EV_CFW_NW_STORELIST_IND   (EV_CFW_NW_RSP_BASE+9)



//
// GPRS Response Event.
//
#define EV_CFW_GPRS_CTX_MODIFY_ACC_RSP       (EV_CFW_GPRS_RSP_BASE+1)
#define EV_CFW_GPRS_ATT_RSP                  (EV_CFW_GPRS_RSP_BASE+2)
#define EV_CFW_GPRS_ACT_RSP                  (EV_CFW_GPRS_RSP_BASE+3)
#define EV_CFW_GPRS_CXT_ACTIVE_RSP           (EV_CFW_GPRS_RSP_BASE+4)
#define EV_CFW_GPRS_MOD_RSP                  (EV_CFW_GPRS_RSP_BASE+5)
#define EV_CFW_WIFI_CONNECTED_RSP      (EV_CFW_GPRS_RSP_BASE+6)

//
// CC Response Event
//
#define EV_CFW_CC_ACCEPT_SPEECH_CALL_RSP     (EV_CFW_CC_RSP_BASE+1)
#define EV_CFW_CC_CALL_HOLD_MULTIPARTY_RSP   (EV_CFW_CC_RSP_BASE+2)
#define EV_CFW_CC_INITIATE_SPEECH_CALL_RSP   (EV_CFW_CC_RSP_BASE+3)
#define EV_CFW_CC_RELEASE_CALL_RSP           (EV_CFW_CC_RSP_BASE+4)

//
// SS Response Event
//
#define EV_CFW_SS_QUERY_CLIP_RSP             (EV_CFW_SS_RSP_BASE+1)
#define EV_CFW_SS_SET_CALL_WAITING_RSP       (EV_CFW_SS_RSP_BASE+2)
#define EV_CFW_SS_QUERY_CALL_WAITING_RSP     (EV_CFW_SS_RSP_BASE+3)
#define EV_CFW_SS_SET_CALL_FORWARDING_RSP    (EV_CFW_SS_RSP_BASE+4)
#define EV_CFW_SS_QUERY_CALL_FORWARDING_RSP  (EV_CFW_SS_RSP_BASE+5)
#define EV_CFW_SS_SET_FACILITY_LOCK_RSP      (EV_CFW_SS_RSP_BASE+6)
#define EV_CFW_SS_GET_FACILITY_LOCK_RSP      (EV_CFW_SS_RSP_BASE+7)
#define EV_CFW_SS_CHANGE_PWD_RSP             (EV_CFW_SS_RSP_BASE+8)
#define EV_CFW_SS_QUERY_FACILITY_LOCK_RSP    (EV_CFW_SS_RSP_BASE+9)
#define EV_CFW_SS_QUERY_CLIR_RSP             (EV_CFW_SS_RSP_BASE+10)
#define EV_CFW_SS_QUERY_COLP_RSP             (EV_CFW_SS_RSP_BASE+11)
#define EV_CFW_SS_SEND_USSD_RSP              (EV_CFW_SS_RSP_BASE+12)
#define EV_CFW_SS_TERMINATE_RSP              (EV_CFW_SS_RSP_BASE+13)
#define EV_CFW_SS_QUERY_COLR_RSP             (EV_CFW_SS_RSP_BASE+14)

//
// SIM Response Event
//
#define EV_CFW_SIM_ADD_PBK_RSP                   (EV_CFW_SIM_RSP_BASE+1)
#define EV_CFW_SIM_DELETE_PBK_ENTRY_RSP          (EV_CFW_SIM_RSP_BASE+2)
#define EV_CFW_SIM_GET_PBK_ENTRY_RSP             (EV_CFW_SIM_RSP_BASE+3)
#define EV_CFW_SIM_LIST_PBK_ENTRY_RSP            (EV_CFW_SIM_RSP_BASE+4)
#define EV_CFW_SIM_GET_PBK_STRORAGE_RSP          (EV_CFW_SIM_RSP_BASE+5)
#define EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP     (EV_CFW_SIM_RSP_BASE+6)
#define EV_CFW_SIM_GET_PROVIDER_ID_RSP           (EV_CFW_SIM_RSP_BASE+7)
#define EV_CFW_SIM_CHANGE_PWD_RSP                (EV_CFW_SIM_RSP_BASE+8)
#define EV_CFW_SIM_GET_AUTH_STATUS_RSP           (EV_CFW_SIM_RSP_BASE+9)
#define EV_CFW_SIM_ENTER_AUTH_RSP                (EV_CFW_SIM_RSP_BASE+11)
#define EV_CFW_SIM_SET_FACILITY_LOCK_RSP         (EV_CFW_SIM_RSP_BASE+12)
#define EV_CFW_SIM_GET_FACILITY_LOCK_RSP         (EV_CFW_SIM_RSP_BASE+13)
#define EV_CFW_SIM_DELETE_MESSAGE_RSP            (EV_CFW_SIM_RSP_BASE+15)
#define EV_CFW_SIM_WRITE_MESSAGE_RSP             (EV_CFW_SIM_RSP_BASE+16)
#define EV_CFW_SIM_READ_MESSAGE_RSP              (EV_CFW_SIM_RSP_BASE+17)
#define EV_CFW_SIM_LIST_MESSAGE_RSP              (EV_CFW_SIM_RSP_BASE+18)
#define EV_CFW_SIM_GET_SMS_STORAGE_INFO_RSP      (EV_CFW_SIM_RSP_BASE+19)
#define EV_CFW_SIM_READ_BINARY_RSP               (EV_CFW_SIM_RSP_BASE+20)
#define EV_CFW_SIM_UPDATE_BINARY_RSP             (EV_CFW_SIM_RSP_BASE+21)
#define EV_CFW_SIM_GET_SMS_PARAMETERS_RSP        (EV_CFW_SIM_RSP_BASE+22)
#define EV_CFW_SIM_SET_SMS_PARAMETERS_RSP        (EV_CFW_SIM_RSP_BASE+23)
#define EV_CFW_SIM_GET_MR_RSP                    (EV_CFW_SIM_RSP_BASE+24)
#define EV_CFW_SIM_SET_MR_RSP                    (EV_CFW_SIM_RSP_BASE+25)
#define EV_CFW_SIM_COMPOSE_PDU_RSP               (EV_CFW_SIM_RSP_BASE+26)
#define EV_CFW_SAT_ACTIVATION_RSP                (EV_CFW_SIM_RSP_BASE+27)
#define EV_CFW_SAT_GET_STATUS_RSP                (EV_CFW_SIM_RSP_BASE+28)
#define EV_CFW_SAT_RESPONSE_RSP                  (EV_CFW_SIM_RSP_BASE+29)
#define EV_CFW_SAT_GET_INFORMATION_RSP           (EV_CFW_SIM_RSP_BASE+30)
#define EV_CFW_SIM_SET_PREF_OPERATOR_LIST_RSP    (EV_CFW_SIM_RSP_BASE+31)
#define EV_CFW_SIM_GET_PREF_OPERATOR_LIST_RSP    (EV_CFW_SIM_RSP_BASE+32)
#define EV_CFW_SIM_GET_PREF_OPT_LIST_MAXNUM_RSP  (EV_CFW_SIM_RSP_BASE+33)
#define EV_CFW_SIM_SET_PBKMODE_RSP               (EV_CFW_SIM_RSP_BASE+34)
#define EV_CFW_SIM_GET_PBKMODE_RSP               (EV_CFW_SIM_RSP_BASE+35)
#define EV_CFW_SIM_INIT_SMS_RSP                  (EV_CFW_SIM_RSP_BASE+36)
#define EV_CFW_SIM_LIST_COUNT_PBK_ENTRY_RSP      (EV_CFW_SIM_RSP_BASE+37)
#define EV_CFW_SIM_RESET_RSP                     (EV_CFW_SIM_RSP_BASE+38)
#define EV_CFW_SIM_GET_PUCT_RSP                  (EV_CFW_SIM_RSP_BASE+39)
#define EV_CFW_SIM_SET_PUCT_RSP                  (EV_CFW_SIM_RSP_BASE+40)
#define EV_CFW_SIM_GET_ACMMAX_RSP                (EV_CFW_SIM_RSP_BASE+41)
#define EV_CFW_SIM_SET_ACMMAX_RSP                (EV_CFW_SIM_RSP_BASE+42)
#define EV_CFW_SIM_SET_ACM_RSP                   (EV_CFW_SIM_RSP_BASE+43)
#define EV_CFW_SIM_GET_ACM_RSP                   (EV_CFW_SIM_RSP_BASE+44)
#define	EV_CFW_SIM_READ_EF_RSP                   (EV_CFW_SIM_RSP_BASE+45)	
#define EV_CFW_SIM_GET_FILE_STATUS_RSP           (EV_CFW_SIM_RSP_BASE+46)
#define EV_CFW_SIM_READ_RECORD_RSP               (EV_CFW_SIM_RSP_BASE+47)
#define EV_CFW_SIM_UPDATE_RECORD_RSP             (EV_CFW_SIM_RSP_BASE+48)
#define EV_CFW_SIM_GET_SPN_RSP                   (EV_CFW_SIM_RSP_BASE+49)	
#define EV_CFW_SIM_GET_SMS_TOTAL_NUM_RSP         (EV_CFW_SIM_RSP_BASE+50)	
#define EV_CFW_SIM_GET_ICCID_RSP                 (EV_CFW_SIM_RSP_BASE+51)	

//
// SMS Response Event.
//
#define EV_CFW_SMS_DELETE_MESSAGE_RSP            (EV_CFW_SMS_RSP_BASE+1)
#define EV_CFW_SMS_GET_STORAGE_RSP               (EV_CFW_SMS_RSP_BASE+2)
#define EV_CFW_SMS_SET_STORAGE_RSP               (EV_CFW_SMS_RSP_BASE+3)
#define EV_CFW_SMS_LIST_MESSAGE_RSP              (EV_CFW_SMS_RSP_BASE+4)
#define EV_CFW_SMS_READ_MESSAGE_RSP              (EV_CFW_SMS_RSP_BASE+5)
#define EV_CFW_SMS_SEND_MESSAGE_RSP              (EV_CFW_SMS_RSP_BASE+6)
#define EV_CFW_SMS_SET_CENTER_ADDR_RSP           (EV_CFW_SMS_RSP_BASE+7)
#define EV_CFW_SMS_GET_CENTER_ADDR_RSP           (EV_CFW_SMS_RSP_BASE+8)
#define EV_CFW_SMS_SET_TEXT_MODE_PARAM_RSP       (EV_CFW_SMS_RSP_BASE+9)
#define EV_CFW_SMS_GET_TEXT_MODE_PARAM_RSP       (EV_CFW_SMS_RSP_BASE+10)
#define EV_CFW_SMS_WRITE_MESSAGE_RSP             (EV_CFW_SMS_RSP_BASE+11)
#define EV_CFW_SMS_LIST_MESSAGE_HRD_RSP          (EV_CFW_SMS_RSP_BASE+12)
#define EV_CFW_SMS_COPY_MESSAGE_RSP              (EV_CFW_SMS_RSP_BASE+13)
#define EV_CFW_SMS_SET_UNREAD2READ_RSP           (EV_CFW_SMS_RSP_BASE+14)
#define EV_CFW_SMS_SET_UNSENT2SENT_RSP           (EV_CFW_SMS_RSP_BASE+15)
#define EV_CFW_SMS_SET_READ2UNREAD_RSP           (EV_CFW_SMS_RSP_BASE+16)
#define EV_CFW_SMS_SET_PARAM_RSP                 (EV_CFW_SMS_RSP_BASE+17)
#define	EV_CFW_SIM_PATCH_RSP				     (EV_CFW_SMS_RSP_BASE+18)
#define EV_CFW_CB_READ_SIM_RSP                   (EV_CFW_SMS_RSP_BASE+19)
#define EV_CFW_SMS_MOVE_RSP                      (EV_CFW_SMS_RSP_BASE+20)
#define EV_CFW_CB_SET_MID_SIM_RSP                (EV_CFW_SMS_RSP_BASE+21)
#define	EV_CFW_SIM_PATCHEX_RSP				     (EV_CFW_SMS_RSP_BASE+22)
//
// PBK Response Event
//
#define EV_CFW_PBK_ADD_RSP                        (EV_CFW_PBK_RSP_BASE+1)
#define EV_CFW_PBK_UPDATE_ENTRY_RSP               (EV_CFW_PBK_RSP_BASE+2)
#define EV_CFW_PBK_DELETE_RSP                     (EV_CFW_PBK_RSP_BASE+3)
#define EV_CFW_PBK_DELETE_BATCH_ENTRIES_RSP       (EV_CFW_PBK_RSP_BASE+4)
#define EV_CFW_PBK_FIND_ENTRY_RSP                 (EV_CFW_PBK_RSP_BASE+5)
#define EV_CFW_PBK_LIST_ENTRIES_RSP               (EV_CFW_PBK_RSP_BASE+6)
#define EV_CFW_PBK_GET_ENTRY_RSP                  (EV_CFW_PBK_RSP_BASE+7)
#define EV_CFW_PBK_COPY_ENTRIES_RSP               (EV_CFW_PBK_RSP_BASE+8)
#define EV_CFW_PBK_ADD_ENTRY_ITEM_RSP             (EV_CFW_PBK_RSP_BASE+9)
#define EV_CFW_PBK_GET_ENTRY_ITEMS_RSP            (EV_CFW_PBK_RSP_BASE+10)
#define EV_CFW_PBK_DELETE_ENTRY_ITEM_RSP          (EV_CFW_PBK_RSP_BASE+11)
#define EV_CFW_PBK_ADD_CALLLOG_ENTTRY_RSP         (EV_CFW_PBK_RSP_BASE+12)
#define EV_CFW_PBK_GET_CALLLOG_ENTRY_RSP          (EV_CFW_PBK_RSP_BASE+13)
#define EV_CFW_PBK_LIST_CALLLOG_ENTRIES_RSP       (EV_CFW_PBK_RSP_BASE+14)
#define EV_CFW_PBK_DELETE_CALLLOG_ALL_ENTRIES_RSP (EV_CFW_PBK_RSP_BASE+15)
#define EV_CFW_PBK_CANCEL_RSP                     (EV_CFW_PBK_RSP_BASE+16)
#define EV_CFW_PBK_DELETE_CALLLOG_OLDEST_ENTRY_RSP (EV_CFW_PBK_RSP_BASE+17)
//
// Shell Response Event
//
#define EV_CFW_ME_SET_FACILITY_LOCK_RSP           (EV_CFW_SHELL_RSP_BASE+1)
#define EV_CFW_ME_GET_FACILITY_LOCK_RSP           (EV_CFW_SHELL_RSP_BASE+2)
#define EV_CFW_ME_CHANGE_PWD_RSP                  (EV_CFW_SHELL_RSP_BASE+3)


//
//TCP(UDP) / IP Event
//
#define EV_CFW_TCPIP_SOCKET_CONNECT_RSP           (EV_CFW_TCPIPUDP_RSP_BASE+1)
#define EV_CFW_TCPIP_SOCKET_CLOSE_RSP             (EV_CFW_TCPIPUDP_RSP_BASE+2)
#define EV_CFW_TCPIP_SOCKET_SEND_RSP              (EV_CFW_TCPIPUDP_RSP_BASE+3)

//
#define EV_CFW_FTP_LONGIN_RSP           (EV_CFW_TCPIPUDP_RSP_BASE+4)
#define EV_CFW_FTP_FILE_DOWNLOAD_RSP             (EV_CFW_TCPIPUDP_RSP_BASE+5)
#define EV_CFW_FTP_LIST_RSP              (EV_CFW_TCPIPUDP_RSP_BASE+6)
#define EV_CFW_FTP_CHANGEDIR_RSP              (EV_CFW_TCPIPUDP_RSP_BASE+7)
#define EV_CFW_FTP_REC_DATA_IND              (EV_CFW_TCPIPUDP_RSP_BASE+8)

// xiaoyifeng add
#define EV_SOC_TCPIP_SOCKET_TIMEOUT_RSP           (EV_CFW_TCPIPUDP_RSP_BASE+4)

//
//test mode event
//
#define EV_CFW_TSM_INFO_IND							(EV_CFW_TM_RSP_BASE+1)
#define EV_CFW_TSM_INFO_END_RSP	                    (EV_CFW_TM_RSP_BASE+2)
#define EV_CFW_TSM_SYNC_INFO_IND					(EV_CFW_TM_RSP_BASE+3)
#define EV_CFW_TSM_SYNC_INFO_END_RSP	            (EV_CFW_TM_RSP_BASE+4)
#define EV_RMT_STTPC_REQ                            (EV_RMC_BASE+1)
#define EV_RMT_STTPC_RSP                            (EV_RMC_BASE+2)

#define EV_RMT_APP_REQ                              (EV_RMC_BASE+51)
#define EV_RMT_APP_RSP                              (EV_RMC_BASE+52)

#ifdef WIFI_SUPPORT
#define EV_CFW_WIFI_INIT_RSP						(EV_CFW_WIFI_BASE +1)
#define EV_CFW_WIFI_DEINIT_RSP						(EV_CFW_WIFI_BASE +2)
#define EV_CFW_WIFI_SCAN_RSP                 		(EV_CFW_WIFI_BASE +3)
#define EV_CFW_WIFI_CONN_RSP						(EV_CFW_WIFI_BASE +4)
#define EV_CFW_WIFI_DISCONN_RSP						(EV_CFW_WIFI_BASE +5)
#define EV_CFW_WIFI_DISCONNECT_IND    				(EV_CFW_WIFI_BASE +6)

#define EV_CFW_WIFI_SCAN_REQ  						(EV_CFW_WIFI_BASE +7)
#define EV_CFW_WIFI_CONNECT_REQ 				    (EV_CFW_WIFI_BASE +8)
#define EV_CFW_WIFI_POWER_ON_REQ    				(EV_CFW_WIFI_BASE +9)
#define EV_MMI_WIFI_IPADDR_CHANGE_REQ			    (EV_CFW_WIFI_BASE +10)


#define EV_CFW_WIFI_CONNECTED_IND                   (EV_CFW_WIFI_BASE+11)
#define EV_CFW_WIFI_DATA_IND                    	(EV_CFW_WIFI_BASE+12)
#define EV_CFW_WIFI_TEST_MODE_DATA_IND 				(EV_CFW_WIFI_BASE+13)
#define EV_MAC_WIFI_DATA_IND 						(EV_CFW_WIFI_BASE+14)
#define EV_MAC_WIFI_DATA_REQ 						(EV_CFW_WIFI_BASE+15)
#define EV_CFW_WIFI_EAPOL_IND 						(EV_CFW_WIFI_BASE+16)
#define EV_MMI_WIFI_CONNECTED_IND					(EV_CFW_WIFI_BASE+17)
#define EV_MMI_WIFI_NEED_POWEROFF_IND  				(EV_CFW_WIFI_BASE+18)
#define EV_CFW_WIFI_IPADDR_UPDATE_IND  				(EV_CFW_WIFI_BASE+19)

#define EV_CFW_WIFI_IPADDR_CONFLICT_IND  			(EV_CFW_WIFI_BASE+20)

#define EV_CFW_WIFI_CURR_AP_INFO_IND				(EV_CFW_WIFI_BASE+21)


#define EV_WIFI_WIFI_RESET_REQ 						(EV_CFW_WIFI_BASE+22)

#define EV_WIFI_MMI_RESET_IND 						(EV_CFW_WIFI_BASE+23)

#define EV_CFW_WIFI_DISCONNECT_REQ    				(EV_CFW_WIFI_BASE+24)
#define EV_WIFI_WIFI_POWERON_CONTINUE_REQ 			(EV_CFW_WIFI_BASE+25)
#define EV_CFW_WIFI_POWER_OFF_REQ 					(EV_CFW_WIFI_BASE+26)
#define EV_MAC_WIFI_KET_SET_REQ 					(EV_CFW_WIFI_BASE+27)

#define EV_CFW_WIFI_TEST_MODE_INFO_IND 				(EV_CFW_WIFI_BASE+28)
#define EV_CFW_WIFI_GET_NETWORKINFO_REQ 			(EV_CFW_WIFI_BASE+29)
#endif

#define EV_WIFID_SCAN_COMP                          (EV_CFW_WIFI_BASE+30)
#define EV_WIFID_CONN_STATUS						(EV_CFW_WIFI_BASE+31)
#define EV_WIFID_SET_MAC_ADDR_COMP					(EV_CFW_WIFI_BASE+32)
#define EV_WIFID_SMARTLINK_PROCESS					(EV_CFW_WIFI_BASE+33)

#if defined(__SLIMMMI_TCPIP__)
/* MSGS from TCPIP */
#define MSG_ID_SOC_TCPIP_ACCEPT_IND               (EV_CFW_SOC_BASE + 1)
#define MSG_ID_SOC_TCPIP_RECEIVE_IND              (EV_CFW_SOC_BASE + 2)
#define MSG_ID_SOC_TCPIP_ATTACH_CNF               (EV_CFW_SOC_BASE + 3)
#define MSG_ID_SOC_TCPIP_DETACH_CNF               (EV_CFW_SOC_BASE + 4)
#define MSG_ID_SOC_TCPIP_LISTEN_CNF               (EV_CFW_SOC_BASE + 5)
#define MSG_ID_SOC_TCPIP_BIND_CNF                 (EV_CFW_SOC_BASE + 6)
#define MSG_ID_SOC_TCPIP_CONNECT_CNF              (EV_CFW_SOC_BASE + 7)
#define MSG_ID_SOC_TCPIP_DISCONNECT_CNF           (EV_CFW_SOC_BASE + 8)
#define MSG_ID_SOC_TCPIP_ERRMSG_IND               (EV_CFW_SOC_BASE + 9)
#define MSG_ID_SOC_TCPIP_MTU_UPDATE_IND           (EV_CFW_SOC_BASE + 10)
#define MSG_ID_SOC_TCPIP_CLOSE_IND                (EV_CFW_SOC_BASE + 11)
#define MSG_ID_SOC_TCPIP_OPTION_CNF               (EV_CFW_SOC_BASE + 12)
#define MSG_ID_SOC_TCPIP_SHUTDOWN_CNF             (EV_CFW_SOC_BASE + 13)

/* MSG to APP */
#define MSG_ID_APP_SOC_NOTIFY_IND                 (EV_CFW_SOC_BASE + 14)
#define MSG_ID_APP_SOC_GET_HOST_BY_NAME_IND       (EV_CFW_SOC_BASE + 15)
#define MSG_ID_APP_SOC_GET_HOST_BY_ADDR_IND       (EV_CFW_SOC_BASE + 16)
#define MSG_ID_APP_SOC_GET_HOST_BY_SRV_IND        (EV_CFW_SOC_BASE + 17)
#define MSG_ID_APP_SOC_GET_HOST_BY_NAPTR_IND      (EV_CFW_SOC_BASE + 18)
#define MSG_ID_APP_SOC_BEARER_INFO_IND            (EV_CFW_SOC_BASE + 19)
#define MSG_ID_APP_SOC_DEACTIVATE_CNF             (EV_CFW_SOC_BASE + 20)
#endif /* __SLIMMMI_TCPIP__ */


#define EV_HTTP_SERVER_START_REQ                     (EV_DLNA_BASE + 1)
#define EV_HTTP_SERVER_LOOP_REQ                      (EV_DLNA_BASE + 2)
#define EV_SSDP_SERVER_START_REQ                     (EV_DLNA_BASE + 3)
#define EV_SSDP_ADVERTISE_START_REQ                  (EV_DLNA_BASE + 4)
#define EV_SSDP_ADVERTISE_LOOP_REQ                   (EV_DLNA_BASE + 5)
#define EV_SSDP_ADVERTISE_STOP_REQ                   (EV_DLNA_BASE + 6)
#define EV_HTTP_SERVER_SHIFT_START_REQ               (EV_DLNA_BASE + 7)
#define EV_HTTP_PLAY_START_REQ                       (EV_DLNA_BASE + 8)
#define EV_HTTP_CLIENT_THREAD_EXIT_IND               (EV_DLNA_BASE + 9)



//
// Bluetooth event
//
#define EV_BT_POWERON_REQ                                   (EV_BT_BASE+1)
#define EV_BT_READY_IND                                         (EV_BT_BASE+2)
#define EV_BT_POWEROFF_REQ                                   (EV_BT_BASE+3)
#define EV_BT_POWEROFF_CNF                                   (EV_BT_BASE+4)
#define EV_BT_NOTIFY_IND                                   (EV_BT_BASE+5)
#define EV_BT_TIMER_EXPIRY_IND                                   (EV_BT_BASE+6)
#define EV_BT_READY_TO_READ_IND                                   (EV_BT_BASE+7)
#define EV_BT_WAKE_UP_REQ                                   (EV_BT_BASE+8)
#define EV_BT_MESSAGE_IND                                       (EV_BT_BASE+9)
#define EV_BT_CALL_IND                                       (EV_BT_BASE+10)
#define EV_BT_PLAY_IND                                       (EV_BT_BASE+11)
#define EV_BT_FILE_RSV_IND                                       (EV_BT_BASE+12)
#define EV_BT_A2DP_PLAY_IND                                       (EV_BT_BASE+13)
#define EV_BT_SHOW_DEVICE 										(EV_BT_BASE+14)			
#define EV_BT_HID_CONNECT_IND   								(EV_BT_BASE+15)			
#define EV_BT_SHOW_MSG 											(EV_BT_BASE+16)	
#define EV_BT_HUMAN_VOICE                                       (EV_BT_BASE+17)
#define EV_BT_HUMAN_VOICE_PLAY                                  (EV_BT_BASE+18)
#define EV_BT_CONNECTED_IND                                                     (EV_BT_BASE+19)
#define EV_BT_SCO_REC                                            (EV_BT_BASE+20)
#define EV_BT_INQUIRY_CNF                                            (EV_BT_BASE+21)
#define EV_BT_PAIR_CNF                                            (EV_BT_BASE+22)
#define EV_BT_SPP_CONNECT_CNF                             (EV_BT_BASE+23)
#define EV_BT_SPP_DISCONNECT_CNF                             (EV_BT_BASE+24)
#define EV_BT_SPP_CONNECT_IND                            (EV_BT_BASE+25)
#define EV_BT_SPP_DATA_IND                            (EV_BT_BASE+26)
#define EV_BT_XXX_END                                           (EV_BT_BASE+30)

#if (EV_BT_XXX_END >= (EV_BT_END)) 
#error 'Error: invalid EV_BT_XXX definition!'.
#endif



#define EV_UI_FW_START                                           (UI_FW_EV_BASE)
/*Switch to new module form any screen.
Before switch to new module, we'll exit all history screen except root screen.*/
#define EV_UI_FW_SWITCH_MOD                                      (UI_FW_EV_BASE + 1)

#define EV_UI_FW_TIMER_EXPIRED                                    (UI_FW_EV_BASE + 2)

#define EV_UI_A2TP_PLAY_MSG                                    (UI_FW_EV_BASE + 3)

/*Event will received by msgHandler of NewScreen after call function EntryNewScreen*/
#define EV_UI_FW_ON_START                                       (UI_FW_EV_BASE + 10)
/*The event will execute before NewScreen's ON_START event, and current screen will be push into history*/
#define EV_UI_FW_ON_PAUSE                                       (UI_FW_EV_BASE + 11)
/*Exit form SUB screen with return value<result> in nParam1
NOTE: If nParam1 equals '0',meaning common return value from popup display, GUI menu ignore etc. 
	And at most of time we just need to redraw current screen.*/
#define EV_UI_FW_ON_RESUME                                      (UI_FW_EV_BASE + 12)
/*exit current screen*/
#define EV_UI_FW_ON_EXIT                                        (UI_FW_EV_BASE + 13)

/*redraw screen*/
#define EV_UI_FW_REDRAW											(UI_FW_EV_BASE + 20)

#define EV_UI_FW_END                                             (UI_FW_EV_BASE + 30)

#if (EV_UI_FW_END >= (UI_FW_EV_END_)) 
#error 'Error: invalid EV_UI_FW_XXX definition!'.
#endif

#define EV_MEDIA_START									(AFW_EV_BASE)
#define EV_MEDIA_MMC_API_MSG							(AFW_EV_BASE + 1)

/*Audio event*/
#define EV_AUDIO_CMD_REQ								(AFW_EV_BASE + 5)

#define EV_AUDIO_PLAY_FINISH_IND						(AFW_EV_BASE + 10)
#define EV_AUDIO_INTERNAL_PLAY_FINISH_IND				(AFW_EV_BASE + 11)
#define EV_AUDIO_RECORD_FINISH_IND						(AFW_EV_BASE + 12)
#define EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND		    (AFW_EV_BASE + 13)

#define EV_AUDIO_PLAY_STREAM_IND		                (AFW_EV_BASE + 14)

#define EV_AUDIO_FADE_INOUT_REQ							(AFW_EV_BASE + 20)
#define EV_AUDIO_FADE_INOUT_FINISH_IND					(AFW_EV_BASE + 21)

/*Camera event*/
#define EV_CAMERA_POWER_UP_REQ                              (AFW_EV_BASE + 100)
#define EV_CAMERA_POWER_DOWN_REQ                            (AFW_EV_BASE + 101)
#if defined(__ZBAR_SUPPORT__)
#define EV_CAMERA_GET_QRCODE_DATA_IND                       (AFW_EV_BASE + 102)
#endif /* __ZBAR_SUPPORT__ */

#if defined(IP_CAMERA_SUPPORT)
#define EV_CAMERA_IPCAM_START_REQ                            (AFW_EV_BASE + 200)
#define EV_CAMERA_IPCAM_GETFRAME_REQ                            (AFW_EV_BASE + 201)
#define EV_CAMERA_IPCAM_GETFRAME_RSP                            (AFW_EV_BASE + 202)
#define EV_CAMERA_IPCAM_STOP_REQ                            (AFW_EV_BASE + 203)
#endif /* IP_CAMERA_SUPPORT */

/*Video event*/
#define EV_MEIDA_END										(AFW_EV_BASE+300)



#define EV_YOC_CLASS	                                         (EV_YOC_BASE+1)
#define EV_YOC_TASKEXIT					                         (EV_YOC_BASE+2)
#define EV_YOC_WIFI_CONNECT_TO_AP                                (EV_YOC_BASE+5)
#define EV_YOC_VOICE_TRIGGER_IND                                 (EV_YOC_BASE+8)


#if defined(SPEAKER_SUPPORT)
#define EV_SPEAKER_START_REQ                 (EV_SPEAKER_BASE + 1)
#define EV_SPEAKER_BT_SPP                    (EV_SPEAKER_BASE + 2)
#define EV_SPEAKER_SPEECH                    (EV_SPEAKER_BASE + 3)
#define EV_SPEAKER_SOCKET                    (EV_SPEAKER_BASE + 4)
#define EV_SPEAKER_STOP_REQ                  (EV_SPEAKER_BASE + 20)
#endif /* SPEAKER_SUPPORT */


#define EV_WIFI_INIT_COMPL                  (EV_WIFIMGR_BASE+1)
#define EV_WIFI_SET_MAC_ADDR_COMPL          (EV_WIFIMGR_BASE+2)
#define EV_WIFI_CONNECT_COMPL               (EV_WIFIMGR_BASE+3)
#define EV_WIFI_SMART_CONFIG_COMPL          (EV_WIFIMGR_BASE+4)
#define EV_WIFI_5991H_SMART_CONFIG_DONE     (EV_WIFIMGR_BASE+5)

#if (EV_MEIDA_END >= (AFW_EV_END_)) 
#error 'Error: invalid EV_MEIDA_xxx definition!'.
#endif

//MMI user define msg
//NOTE: Need to modify msg id in res.bin
#define AP_MSG_USER_MUSIC    		(EV_MMI_EV_BASE + 100) 

#define MUSIC_START_REPEAT              (AP_MSG_USER_MUSIC+1)
#define MUSIC_SET_FOLLOW                (AP_MSG_USER_MUSIC+2)
#define MUSIC_SELECT_FILE               (AP_MSG_USER_MUSIC+3)
#define MUSIC_SET_EQ                    (AP_MSG_USER_MUSIC+4)
#define MUSIC_SET_LOOP                  (AP_MSG_USER_MUSIC+5)
#define MUSIC_SET_RPT_TIMES             (AP_MSG_USER_MUSIC+6)
#define MUSIC_SET_RPT_PEROID            (AP_MSG_USER_MUSIC+7)
#define MUSIC_OPEN_FMOUT                (AP_MSG_USER_MUSIC+8)
#define MUSIC_CLOSE_FMOUT               (AP_MSG_USER_MUSIC+9)
#define MUSIC_SETUP_FMOUT               (AP_MSG_USER_MUSIC+10)
#define MUSIC_OPEN_LYRIC                (AP_MSG_USER_MUSIC+11)
#define MUSIC_CLOSE_LYRIC               (AP_MSG_USER_MUSIC+12)
#define MUSIC_DELETE_FILE               (AP_MSG_USER_MUSIC+13)
#define MUSIC_REINIT                    (AP_MSG_USER_MUSIC+14)
#define MUSIC_PLAY_REFRESH_UI           (AP_MSG_USER_MUSIC+15)
#define MUSIC_SET_ADD_TIMER             (AP_MSG_USER_MUSIC+16)

#define AT_MUSIC_PLAY                    (AP_MSG_USER_MUSIC+17)
#define AT_MUSIC_PAUSE                 (AP_MSG_USER_MUSIC+18)
#define AT_COMMON_VOLADD           (AP_MSG_USER_MUSIC+19)
#define AT_COMMON_VOLSUB           (AP_MSG_USER_MUSIC+20)
#define AT_COMMON_NEXT               (AP_MSG_USER_MUSIC+21)
#define AT_COMMON_PREV               (AP_MSG_USER_MUSIC+22)
#define AT_COMMON_FORE               (AP_MSG_USER_MUSIC+23)
#define AT_COMMON_FORE_PLAY     (AP_MSG_USER_MUSIC+24)
#define AT_COMMON_BACK               (AP_MSG_USER_MUSIC+25)
#define AT_COMMON_BACK_PLAY     (AP_MSG_USER_MUSIC+26)
#define AT_COMMON_VOLUME          (AP_MSG_USER_MUSIC+27)
#define AT_COMMON_PLAYTONE      (AP_MSG_USER_MUSIC+28)

#define AP_MSG_USER_FM        		(EV_MMI_EV_BASE + 300)	//13300
#define AT_FM_AUTO_SEARCH			(AP_MSG_USER_FM+9)
#define AT_FM_MODE					(AP_MSG_USER_FM+10)
#define AT_FM_STOP_SEARCH			(AP_MSG_USER_FM+11)
#define AT_FM_SETFREQ			    (AP_MSG_USER_FM+11)
#define AT_FM_SETTHRESHOLD 	        (AP_MSG_USER_FM+11)

#define AP_MSG_USER_BT        		(EV_MMI_EV_BASE + 500)	//13500
#define AP_MSG_USER_CALENDAR		(EV_MMI_EV_BASE + 700)	//13700
#define AP_MSG_USER_RECORDER		(EV_MMI_EV_BASE + 900)	//13900
#define AT_RECORD_START            (AP_MSG_USER_RECORDER+4)
#define AT_RECORD_PAUSE            (AP_MSG_USER_RECORDER+5)
#define AT_RECORD_OVER             (AP_MSG_USER_RECORDER+6)
#define AT_RECORD_OVER_CLEAR       (AP_MSG_USER_RECORDER+7)


#define AP_MSG_USER_SETTING			(EV_MMI_EV_BASE + 1100)	//14100
#define AP_MSG_USER_TIMER			(EV_MMI_EV_BASE + 1300)	//14300
#define AP_MSG_USER_USBDISK			(EV_MMI_EV_BASE + 1500)	//14500
#define AP_MSG_USER_DIRECTORY		(EV_MMI_EV_BASE + 1700)	//14700

#if defined(__WIFI_RADIO_BOX__)
#define AP_MSG_USER_WIFI_RADIOBOX		(EV_MMI_EV_BASE + 1800)	//14800
#endif /* __WIFI_RADIO_BOX__ */
#ifdef IP_CAMERA_SUPPORT
#define AP_MSG_USER_IP_CAMERA		(EV_MMI_EV_BASE + 1900)	//14900
#endif /* IP_CAMERA_SUPPORT */
#if (AP_MSG_USER_DIRECTORY >= (EV_MMI_EV_END_)) 
#error 'Error: invalid AP_MSG_USER_xxx definition!'.
#endif

#endif // _H_

