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

#ifndef AT_CFG_H
#define AT_CFG_H

// ///////////////////////////////////////////////////////////////////////
// Gather definition from other ats module
// ///////////////////////////////////////////////////////////////////////

typedef enum
{
    GC_RESULTCODE_PRESENTATION_ENABLE = 0,
    GC_RESULTCODE_PRESENTATION_DISABLE
} GC_RESULTCODE_PRESENTATION;

typedef enum
{
    GC_RESULTCODE_FORMAT_LIMITED = 0,
    GC_RESULTCODE_FORMAT_VERBOSE
} GC_RESULTCODE_FORMAT;

typedef enum
{
    GC_COMMANDECHO_MODE_DISABLE = 0,
    GC_COMMANDECHO_MODE_ENABLE
} GC_COMMANDECHO_MODE;

typedef struct
{
	UINT16 ind_ctrl_mask; // indicator control mask
	UINT16 sind_ctrl_mask;  // system indicator control mask, for ^SIND command
	at_chset_t te_chset;
	at_chset_t mt_chset;
} cfg_info_t;

typedef enum
{
    PBK_CHARACTER_SET_TRANSPARENT = 0,
    PBK_CHARACTER_SET_HEX
}
PBK_CHARACTER_SET;

typedef enum
{
    PBK_LISTENTRY_CALLER_CPBR = 0,
    PBK_LISTENTRY_CALLER_CPBF,
    PBK_LISTENTRY_CALLER_CNUM
}
PBK_LISTENTRY_CALLER;

typedef struct _AT_CNMI
{
	UINT8 nMode;
	UINT8 nMt;
	UINT8 nBm;
	UINT8 nDs;
	UINT8 nBfr;
} AT_CNMI;

typedef struct _AT_CONFIG_INFO
{
	AT_CNMI sCnmi;
	UINT8 nStorage1;
	UINT8 nStorage2;
	UINT8 nStorage3;

	UINT8 nTotal1;
	UINT8 nTotal2;
	UINT8 nTotal3;

	UINT8 nUsed1;
	UINT8 nUsed2;
	UINT8 nUsed3;

	UINT8 nServType;
	UINT8 nOverflowCtrl;
	UINT8 nOverflowMode;

} AT_CONFIG_INFO;

#define NW_PREFERRED_OPERATOR_FORMAT_NUMERIC   2
#define NW_GETPREFERREDOPERATORS_CALLER_READ 	0
#define GSM_SET			0
#define UCS2_SET		1
#define INSTANCE		0
#define NONE_INSTANCE	1

#define AT_AUDIO_TEST_HANDSET 	0
// 耳机
#define AT_AUDIO_TEST_EARPIECE 	1
// 听筒
#define AT_AUDIO_TEST_LOUDSPEAKER	2
// 免提

// ///////////////////////////////////////////////////////////////////////////
// New at global variables
// ///////////////////////////////////////////////////////////////////////////
// sms related settings
#if (AUDIO_CALIB_VER == 1)
#define NUMBER_OF_SIM 1 //tmp add
#endif
struct at_sms_settings
{
	// sms related
	UINT8 g_CSMP_fo;  // [+]2007.11.20 bug 7000
	UINT8 g_CSDH_show;  // show header
	AT_CONFIG_INFO g_AtSMSInfo[NUMBER_OF_SIM]; // CPMS命令相关的存储器信息
	UINT8 g_SMSFormat;  // SMS Foramt: 0: PDU 1:text
};

typedef struct at_sms_settings AT_SMS_SETTINGS, * P_ATSMSETTING;

struct at_general_settings
{
	// //////////////////////////////////
	// gc related
	// //////////////////////////////////
	// whether or not the DCE transmits result codes to the DTE (ATQ)
	GC_RESULTCODE_PRESENTATION g_eResultcodePresentation;

	// Determine the contents of the header and trailer transmitted with result codes
	// and information responses (ATV)
	GC_RESULTCODE_FORMAT g_eResultcodeFormat;

	// Determine whether or not the DCE echoes characters received from the DTE
	// during command state and online command state (ATE).
	GC_COMMANDECHO_MODE g_eCommandEchoMode;

	// Response formatting character (S3,S4)
	UINT8 g_nS3;
	UINT8 g_nS4;

	// Command line editing character (S5)
	UINT8 g_nS5;

	// Disable or enables the use of result code +CME ERROR (AT+CMEE)
	UINT8 g_uCmee;

	// ndicator event reporting way (AT+CMER)
	UINT8 g_uCmer_ind[NUMBER_OF_SIM];

	// Save 4 config parameters:
	// ind_ctrl_mask:     not used
	// sind_ctrl_mask:  not used
	// which character set <chset> is used by the TE (GSM or UCS2)
	// not used
	cfg_info_t g_CfgInfo;

	// /////////////////////////////////////////////////
	// cc related
	// /////////////////////////////////////////////////
	// Duration of the tone in 1/10 second, which is the para of cmd VTD
	UINT8 g_cc_VTD;

	// This S-parameter control the automatic answering feature of the DCE.
	// 0:disable automatic answering
	// non-zero: the number of times (inticated by the value) to answer an incoming call
	UINT8 g_cc_s0;

	// Operator select the auto answer status (0-3)
	// ken
	UINT8 g_gprs_auto;
	UINT32 g_baud_rate; // add by yangtt 08-04-18
	// ////////////////////////////////////////////////////////////
	// nw related
	// /////////////////////////////////////////////////////////////
	// Operator select format (AT+COPS)
	UINT8 g_OperFormat;

	// Operator selcet mode (AT+COPS)
	UINT8 g_COPS_Mode_Temp;

	// Parameter used in cmd (AT+CPOL)
	UINT8 g_nPreferredOperFormat;

	// TODO..
	// UINT8 g_arrPreferredOperater[6];
	UINT8 *g_pArrPreferredOperater;
	UINT8 g_nPreferredOperatorIndex;

	// Save the operator caller
	UINT8 g_nGetPreferredOperatorsCaller;

	// /////////////////////////////////////////////////////
	// pbk related
	// /////////////////////////////////////////////////////
	// Read current phonebook entries used in cmd (AT+CPBR)
	PBK_LISTENTRY_CALLER g_pbkListEntryCaller;

	// para used in cmd (AT+WPCS)
	PBK_CHARACTER_SET g_pbkCharacterSet;

	// Select memory storage  (AT+CPBS)
	UINT32 g_nTempStorage;

	// ////////////////////////////////////////////////
	// sat related
	// ////////////////////////////////////////////////
	// AT^STA
	UINT8 g_Alphabet;

	// ////////////////////////////////////////////////
	// sms related
	// ///////////////////////////////////////////////
	AT_SMS_SETTINGS g_AtSmsSetting;

	// ///////////////////////////////////////////////
	// ss related
	// ///////////////////////////////////////////////
	UINT8 g_ucSsCallWaitingPresentResultCode; // CCWA 命令用到的cfg
	UINT8 g_ss_ussd;  // URC control signal
	UINT8 ucClip;
	UINT8 ucClir;
	UINT8 ucColp;
	UINT8 ucCSSU;
	UINT8 ucCSSI;

	// ///////////////////////////////////////////////
	// special related
	// ///////////////////////////////////////////////
	// Retrieve information of the serving/dedicated cell automatically every n seconds
	UINT8 g_u8nMONIPara;

	// Retrieve information of up to six neighbour cells automatically every n seconds
	UINT8 g_u8nMONPPara;

	// Flag of whether the URC ^CBCI:<bcs>,<bcl>,<Bat_state>,<Bat_volt> is reported
	UINT8 g_nPMIndMark;

	// Receive gain selection
	UINT8 g_VGR_Volume;

	// Transmit gain selection
	UINT8 g_VGT_Volume;

	// enable or disable the uplink voice muting during a voice call
	UINT8 g_CMUT_mute;

	// Select and execute audio cycle test mode :0--aux,1--mic,2--loud (AT^AUST)
	UINT8 g_nAudioTestMode;

	// g_stGain;                          //audio_cfg???

	// [[ yy [add] at 2008-5-27 for TCPIP
	// /////////////////////////////////////////////////////
	// TCPIP related
	// /////////////////////////////////////////////////////
	UINT8 g_u8TcpipConIPorDName;
	UINT8 g_u8TcpipPrompt;
	UINT8 g_u8TcpipAddIPHeader;
	UINT8 g_u8TcpipDispIPandPort;

	// ]] yy [add] at 2008-5-27 for TCPIP
};

typedef struct at_general_settings AT_GENERAL_SETTINGS, AT_USER_SETTINGS, AT_DEFAULT_SETTINGS, AT_CURRENT_SETTINGS;

// define  in at_cmd_cfg.c
extern AT_USER_SETTINGS gAtUserSettings[];  // user settings
extern AT_CURRENT_SETTINGS gAtCurrentSetting; // current settings

// ///////////////////////////////////////////////////////////////////////////
// Macro definition of configuration setttings
// ///////////////////////////////////////////////////////////////////////////
// Flag of switching AT current settings
#define MANUFACTURER_DEFALUT_SETING 0
#define USER_SETTING_1 1

// /////////////////////////////////////////////////////////////////////////////////////////
// Macro to facilitate the use of Current AT settings
// /////////////////////////////////////////////////////////////////////////////////////////
// gc related
#define gATCurrenteResultcodePresentation 	gAtCurrentSetting.g_eResultcodePresentation
#define gATCurrenteResultcodeFormat 	gAtCurrentSetting.g_eResultcodeFormat
#define gATCurrenteCommandEchoMode 	gAtCurrentSetting.g_eCommandEchoMode
#define gATCurrentnS3 			gAtCurrentSetting.g_nS3
#define gATCurrentnS4 			gAtCurrentSetting.g_nS4
#define gATCurrentnS5 			gAtCurrentSetting.g_nS5
#define gATCurrentuCmee 		gAtCurrentSetting.g_uCmee
#define gATCurrentuCmer_ind 		gAtCurrentSetting.g_uCmer_ind
// TODO..
#define gATCurrentCfgInfoInd_ctrl_mask 	gAtCurrentSetting.g_CfgInfo.ind_ctrl_mask
#define gATCurrentCfgInfoTe_chset	gAtCurrentSetting.g_CfgInfo.te_chset
#define gATCurrentCfgInfoMt_chset	gAtCurrentSetting.g_CfgInfo.mt_chset

// cc related
#define gATCurrentcc_VTD 	gAtCurrentSetting.g_cc_VTD
#define gATCurrentcc_s0 	gAtCurrentSetting.g_cc_s0

// nw related
#define gATCurrentOperFormat 		gAtCurrentSetting.g_OperFormat
#define gATCurrentCOPS_Mode_Temp 	gAtCurrentSetting.g_COPS_Mode_Temp
#define gATCurrentnPreferredOperFormat 	gAtCurrentSetting.g_nPreferredOperFormat
#define gATCurrentpArrPreferredOperater 	gAtCurrentSetting.g_pArrPreferredOperater
#define gATCurrentnPreferredOperatorIndex 	gAtCurrentSetting.g_nPreferredOperatorIndex
#define gATCurrentnGetPreferredOperatorsCaller 	gAtCurrentSetting.g_nGetPreferredOperatorsCaller

// pbk related
#define gATCurrentpbkListEntryCaller 	gAtCurrentSetting.g_pbkListEntryCaller
#define gATCurrentpbkCharacterSet 	gAtCurrentSetting.g_pbkCharacterSet
#define gATCurrentnTempStorage 		gAtCurrentSetting.g_nTempStorage

// sat related
#define gATCurrentAlphabet	gAtCurrentSetting.g_Alphabet

// sms related
#define gATCurrentAtSmsSettingSCSMP_fo 		gAtCurrentSetting.g_AtSmsSetting.g_CSMP_fo
#define gATCurrentAtSmsSettingSCSDH_show 	gAtCurrentSetting.g_AtSmsSetting.g_CSDH_show
#define gATCurrentAtSmsSettingSg_SMSFormat 	gAtCurrentSetting.g_AtSmsSetting.g_SMSFormat
#define gATCurrentAtSMSInfo 	gAtCurrentSetting.g_AtSmsSetting.g_AtSMSInfo

// ss related
#define gATCurrentucSsCallWaitingPresentResultCode gAtCurrentSetting.g_ucSsCallWaitingPresentResultCode
#define gATCurrentss_ussd 	gAtCurrentSetting.g_ss_ussd
#define gATCurrentucClip 	gAtCurrentSetting.ucClip
#define gATCurrentucClir 	gAtCurrentSetting.ucClir
#define gATCurrentucColp 	gAtCurrentSetting.ucColp
#define gATCurrentucCSSU 	gAtCurrentSetting.ucCSSU
#define gATCurrentucCSSI 	gAtCurrentSetting.ucCSSI

// special related
#define gATCurrentu8nMONIPara 	gAtCurrentSetting.g_u8nMONIPara
#define gATCurrentu8nMONPPara 	gAtCurrentSetting.g_u8nMONPPara
#define gATCurrentnPMIndMark	gAtCurrentSetting.g_nPMIndMark
#define gATCurrentVGR_Volume 	gAtCurrentSetting.g_VGR_Volume
#define gATCurrentVGT_Volume 	gAtCurrentSetting.g_VGT_Volume
#define gATCurrentCMUT_mute 	gAtCurrentSetting.g_CMUT_mute
#define gATCurrentnAudioTestMode gAtCurrentSetting.g_nAudioTestMode

// gprs related
#define gATCurrentu8nURCAuto gAtCurrentSetting.g_gprs_auto

// added by yangtt at 04-18
#define gATCurrentu32nBaudRate gAtCurrentSetting.g_baud_rate

/*TCPIP related*/
// yy [add] for TCPIP
#define gATCurrentu8TCPIPConnIPorDName  gAtCurrentSetting.g_u8TcpipConIPorDName
#define gATCurrentu8TCPIPPrompt                gAtCurrentSetting.g_u8TcpipPrompt
#define gATCurrentu8TCPIPAddIPHeader       gAtCurrentSetting.g_u8TcpipAddIPHeader
#define gATCurrentu8TCPIPDispIPandPort     gAtCurrentSetting.g_u8TcpipDispIPandPort

// /////////////////////////////////////////////////////////////////////////////////
// API to operate at settings
// /////////////////////////////////////////////////////////////////////////////////
// API to save at settings
extern BOOL at_CfgSetAtSettings(UINT8 set_id);

// API to get at setttings
extern BOOL at_CfgGetAtSettings(UINT8 flag, UINT8 set_id);

#endif
