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
 * MODULE NAME:    hfp.h
 * DESCRIPTION:    interface for Headset and Handfree profile
 * AUTHOR:         Martin Mellody
 *
 * SOURCE CONTROL: $Id: hfp.h,v 1.1 2008/01/03 06:55:12 tianwq Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2011 RDA Microelectronics.
 *     All rights reserved.
 *
 ***********************************************************************/
#ifndef __HFP_H__
#define __HFP_H__

#include "obex.h"
/*********************************************
 * HFP Class of Device                     
 *********************************************/
#define HANDS_FREE_UNIT_COD     0x200408
#define HANDS_FREE_AG_COD       0x200408
#define HEADSET_UNIT_COD        0x200404
#define HEADSET_AG_COD          0x200404


#define ACTIVE_PROFILE_HEADSET            0x01
#define ACTIVE_PROFILE_HEADSET_AG         0x02
#define ACTIVE_PROFILE_HANDSFREE          0x04
#define ACTIVE_PROFILE_HANDSFREE_AG       0x08

/*****************************************************************************/
/*                        Handsfree Feature Definitions                      */
/*****************************************************************************/
//AG supported features bit masks .
#define AG_FEATURE_THREE_WAY_CALLING                (1)
#define AG_FEATURE_NREC_FUNCTION                    (1<<1)
#define AG_FEATURE_VOICE_RECOGNITION                (1<<2)
#define AG_FEATURE_IN_BAND_RING                     (1<<3)
#define AG_FEATURE_VOICE_TAG                        (1<<4)
#define AG_FEATURE_REJECT_CALL                      (1<<5)
#define AG_FEATURE_ENHANCED_CALL_STATUS             (1<<6)
#define AG_FEATURE_ENHANCED_CALL_CONTROL            (1<<7)
#define AG_FEATURE_EXTENDED_ERROR_CODES             (1<<8)

/*Bits for HF_Modify_Remote_Supplementary_Feature(). */
//HF supported features bit masks .
#define HF_FEATURE_NREC_FUNCTION                    (1)
#define HF_FEATURE_CALL_WAIT                        (1<<1)
#define HF_FEATURE_CLIP                             (1<<2)
#define HF_FEATURE_VOICE_RECOGNITION                (1<<3)
#define HF_FEATURE_REMOTE_VOL                       (1<<4)
#define HF_FEATURE_ENHANCED_CALL_STATUS             (1<<5)
#define HF_FEATURE_ENHANCED_CALL_CONTROL            (1<<6)



/* second argument of HF_Call_Request */
enum call_type {
    /* use by HF_Call_Request */
	HF_CALL_INCOMMING,
	HF_CALL_OUTGOING,
	HF_CALL_START,
	HF_CALL_ALERT,
	HF_CALL_RELEASE,
	HF_CALL_NUM,
	HF_CALL_MEM,
	HF_CALL_REDIAL,

    /* use by HF_Call_Answer */
	HF_CALL_ACCEPT,
	HF_CALL_REJECT,

    /* use by HF_Call_Hold */
	HF_CALL_HELD,
	HF_CALL_WAIT,
	HF_CALL_ACTIVE,
    /* use by HF_Call_Status */
    HF_SERVICE_INDICATOR,
    HF_CALL_INDICATOR,
    HF_CALLSETUP_INDICATOR,
    HF_CALLHOLD_INDICATOR,
    HF_BATTCHG_INDICATOR,
    HF_SIGNAL_INDICATOR,
    HF_ROAM_INDICATOR,

    /* use by HF_Voice_Recognition */
    HF_VR_START,
    HF_VR_STOP
};

/*CODEC NEGOTIATION  CODEC ID'S*/
enum codec_id{
		HF_VOC_CVSD = 1,
		HF_VOC_MSBC,
		HF_VOC_UNKNOW
};



/*Bits for HF_Multiparty_Call_Handling() */
typedef enum{
	PARTY_CURR_ACTIVE_OTHER_DISMISS	= 0,
	PARTY_CURR_RELEASE_OTHER_ACCEPT,
	PARTY_CURR_HOLD_OTHER_ACCEPT,
	PARTY_JOIN_3WAY,
	PARTY_LEAVE_3WAY
} HF_PART_CALL_TYPE;

typedef struct
{
    t_bdaddr       bt_addr;
    u_int16        connection_id;  
    u_int8         profile;
    u_int8         result;
} t_hfp_connect_ind;

typedef struct
{
    u_int8 	       gain;
    u_int8         profile;
    u_int16        connection_id;
} t_hfp_gain_ind;

typedef struct 
{
    u_int8          type;
    u_int8		    value;
    u_int16         connection_id;
    u_int8          call_end;
} t_hfp_call_ind;

#ifdef __cplusplus
extern "C" {
#endif

// must call in the bt task
APIDECL1 t_api APIDECL2 HF_Activate(u_int8 active_profile);
APIDECL1 t_api APIDECL2 HF_Deactivate(u_int8 active_profile);
APIDECL1 t_api APIDECL2 HF_Connect_SLC(u_int8 call_active, t_bdaddr device_addr, u_int8 active_profile);
APIDECL1 t_api APIDECL2 HF_Accept_SLC(u_int8 call_active, t_bdaddr ag_bdaddr);
APIDECL1 t_api APIDECL2 HF_Reject_SLC(t_bdaddr ag_bdaddr);
APIDECL1 t_api APIDECL2 HF_Release_SLC(t_bdaddr ag_bdaddr);

APIDECL1 t_api APIDECL2 HF_ServiceDiscover(t_bdaddr address, u_int8 active_profile);

APIDECL1 t_api APIDECL2 HF_Stop_Ring(t_bdaddr *address);

APIDECL1 t_api APIDECL2 HF_Send_Raw_Data(t_bdaddr *address, u_int8 *data, int dataLen);

APIDECL1 t_api APIDECL2 HF_Send_Mic_Gain(t_bdaddr *address, u_int8 gain) ;
APIDECL1 t_api APIDECL2 HF_Send_Speaker_Gain(t_bdaddr *address, u_int8 gain);

APIDECL1 t_api APIDECL2 HF_Send_Button_Press(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Enable_Band_Ring(t_bdaddr *address, u_int8 enabled);

APIDECL1 u_int APIDECL2 HF_Get_Current_Profile(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Set_Number(t_bdaddr *address, u_int8 *number);
APIDECL1 t_api APIDECL2 HF_Get_Number(t_bdaddr *address, u_int8 *number, u_int8 *length);
APIDECL1 t_api APIDECL2 HF_Set_NetworkOperator(t_bdaddr *address, u_int8 *netop);
APIDECL1 t_api APIDECL2 HF_Get_NetworkOperator(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Get_CallStatus(t_bdaddr *address, u_int16 *status);

APIDECL1 t_api APIDECL2 HF_Call_Request(t_bdaddr *address, u_int8 type, u_int8 mode);
APIDECL1 t_api APIDECL2 HF_Call_Answer(t_bdaddr *address, u_int8 type);
APIDECL1 t_api APIDECL2 HF_Call_Release(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Call_Hold(t_bdaddr *address, u_int8 type);
APIDECL1 t_api APIDECL2 HF_Call_Status(t_bdaddr *address, u_int8 type, u_int8 value);
APIDECL1 t_api APIDECL2 HF_Terminate_Call(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Voice_Recognition(t_bdaddr *address, u_int8 type);
APIDECL1 t_api APIDECL2 HF_Get_Clcc(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Send_Cnum(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Send_BTRH(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Send_NREC(t_bdaddr *address, u_int8 type);
APIDECL1 t_api APIDECL2 HF_Send_BINP_2(t_bdaddr *address, u_int8 type);
APIDECL1 t_api APIDECL2 HF_Send_DTMF(t_bdaddr *address, char* dtmf_sequence );
APIDECL1 t_api APIDECL2 HF_Send_NoCarrier(t_bdaddr *address);
APIDECL1 t_api APIDECL2 HF_Set_PhoneBook_PATH(t_bdaddr *address, enum PhoneBook_Place which, enum PhoneBook_Type type);
APIDECL1 t_api APIDECL2 HF_Get_PhoneBook(t_bdaddr *address, u_int32 start, u_int32 num);
APIDECL1 t_api APIDECL2 HF_Activate_CLIP(t_bdaddr *address);

APIDECL1 t_api APIDECL2 RDABT_HF_Connect_SLC(t_bdaddr device_addr);
APIDECL1 t_api APIDECL2 RDABT_HF_Accept_SLC(t_bdaddr ag_bdaddr);
APIDECL1 u_int APIDECL2 RDABT_HF_Release_SLC(t_bdaddr ag_bdaddr);
//APIDECL1 t_api APIDECL2 RDABT_HF_Call_Request(u_int8 type);

APIDECL1 t_api RDABT_HF_Call_Request(t_hfp_call_ind * call_info);

APIDECL1 t_api APIDECL2 RDABT_HF_Call_Status(u_int8 type, u_int8 value);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif

