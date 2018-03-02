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

#ifndef AP_A2DP_H
#define AP_A2DP_H

#include "tgt_app_cfg.h"

#include "bt_types.h"
#if APP_SUPPORT_A2DP==1
#include "bt_msg.h"
#include "ap_common.h"
#include "ap_mgr.h"
#include "ap_message.h"
#include "rdabt_avrcp.h"
#include "rdabt_manager.h"
#include "ap_avrcp.h"

#define MSG_AUDIODEC_A2DP   3
#define MSG_AUDIODEC_CONTROL   5
#define MSG_LINEIN_START_SEND_SBC 6



#define A2DP_ADP_STATE_IDLE                    0
#define A2DP_ADP_STATE_CLOSE                   1
#define A2DP_ADP_STATE_PENDING             2
#define A2DP_ADP_STATE_CONNECTED         3
#define A2DP_ADP_STATE_OPEN                   4
#define A2DP_ADP_STATE_PLAY                    5
#define A2DP_ADP_STATE_PAUSE                  6
#define A2DP_ADP_STATE_STOP                    7

#define MAX_CP_SCHEME_LENGTH                              (48)

typedef struct{
    u_int32 current_clk;
    u_int32 counterNum;
}SOUNBAR_AUDIO_SYNC_STRUCT;

#ifndef __BT_A2DP_CODEC_TYPES__
#define __BT_A2DP_CODEC_TYPES__

typedef struct
{
	u_int8 min_bit_pool;
	u_int8 max_bit_pool;
	u_int8 block_len; // b0: 16, b1: 12, b2: 8, b3: 4
	u_int8 subband_num; // b0: 8, b1: 4
	u_int8 alloc_method; // b0: loudness, b1: SNR
	u_int8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 16000
	u_int8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
} bt_a2dp_sbc_codec_cap_struct;

typedef struct
{
	u_int8 layer; // b0: layerIII, b1: layerII, b0: layerI
	u_int8 CRC;
	u_int8 channel_mode; // b0: joint stereo, b1: stereo, b2: dual channel, b3: mono
	u_int8 MPF; // is support MPF-2
	u_int8 sample_rate; // b0: 48000, b1: 44100, b2: 32000, b3: 24000, b4: 22050, b5: 16000
	u_int8 VBR; // is support VBR
	u_int16 bit_rate; // bit-rate index in ISO 11172-3 , b0:0000 ~ b14: 1110
} bt_a2dp_mp3_codec_cap_struct; /* all MPEG-1,2 Audio */

typedef struct
{
	u_int8 object_type; // b4: M4-scalable, b5: M4-LTP, b6: M4-LC, b7: M2-LC
	u_int16 sample_rate; // b0~b11: 96000,88200,64000,48000,44100,32000,24000,22050,16000,12000,11025,8000
	u_int8 channels; // b0: 2, b1: 1
	u_int8 VBR; // is supported VBR
	u_int32 bit_rate; // constant/peak bits per second in 23 bit UiMsbf, 0:unknown
} bt_a2dp_aac_codec_cap_struct; /* all MPEG-2,4 AAC */

typedef struct
{
	u_int8 version; // 1:ATRAC, 2:ATRAC2, 3:ATRAC3
	u_int8 channel_mode; // b0: joint stereo, b1: dual, b2: single
	u_int8 sample_rate; // b0: 48000, b1: 44100
	u_int8 VBR; // is supported VBR
	u_int32 bit_rate; // bit-rate index in ATRAC, b0: 0x0012 ~ b18: 0x0000
	u_int16 max_sul; // sound unit length in 16 bits UiMsbf
} bt_a2dp_atrac_codec_cap_struct; /* all ATRAC family */

typedef union
{
	bt_a2dp_sbc_codec_cap_struct sbc;
	bt_a2dp_mp3_codec_cap_struct mp3;
	bt_a2dp_aac_codec_cap_struct aac;
	bt_a2dp_atrac_codec_cap_struct atrac;
} bt_a2dp_audio_codec_cap_struct;

typedef struct
{
	u_int8 codec_type; // SBC, MP3
	bt_a2dp_audio_codec_cap_struct codec_cap;
} bt_a2dp_audio_cap_struct;

typedef struct
{
    u_int16 connect_id;
    t_bdaddr device_addr;
} t_a2dp_signal_connect_ind;

typedef struct
{
    u_int16 connect_id;
    u_int8 acp_seid;
    t_bdaddr addr;
} t_a2dp_capabilities_get_ind;

typedef struct
{
    u_int16 connect_id;
    u_int8 length;
    u_int8 cp_scheme[MAX_CP_SCHEME_LENGTH];
} t_a2dp_security_control_ind;

typedef struct
{
    u_int16 connect_id;
    u_int16 length;
    u_int8 cap[128];
} t_a2dp_cmd_data_ind;
#endif

typedef struct
{
    u_int8 isValid;
    u_int16 delay_value;
} bt_a2dp_delay_reporting_cap_struct;

typedef struct
{
    u_int8 losc;
    u_int16 CP_type;
} bt_a2dp_content_protection_cap_struct;

typedef struct
{
	u_int16 connect_id;
	u_int8 acp_seid;
	u_int8 int_seid;
	u_int8 stream_handle;
	bt_a2dp_audio_cap_struct audio_cap;
    bt_a2dp_content_protection_cap_struct cp_cap;
    bt_a2dp_delay_reporting_cap_struct dr_cap;
    t_bdaddr addr;
} t_a2dp_stream_config_ind;


typedef struct A2DP_DEVICE_INFO
{
    t_bdaddr a2dp_addr;
    u_int8 a2dp_device_name[MAX_BT_DEVICE_NAME];
    u_int8 seid;
    u_int16 cid;
    u_int16 media_cid;
    u_int8 a2dp_adp_state;
    u_int8 a2dp_check_timer;
    u_int8 check_cnt;
    u_int32 a2dp_param;
    u_int8 need_play;
    u_int8 send_pause_mutex_lock;
    u_int8 send_avrcp_pause_timer;
}APP_A2DP_DEVICE_INFO;

typedef struct
{
	APP_A2DP_DEVICE_INFO *a2dp_device_info_list;
    u_int8 a2dp_max_device_num;
    u_int8 a2dp_connect_index;  
    t_a2dp_stream_config_ind a2dp_config;
    t_bdaddr last_addr;
}APP_A2DP_ADP_INFO;

typedef struct
{
    t_bdaddr a2dp_addr;
    u_int32 msg_param;
    u_int32 msg_param1;
}RDABT_A2DP_MSG_INFO;

typedef struct
{
    t_bdaddr a2dp_addr;
    u_int32 length;
    u_int8 *p_data;
    u_int32 timestamp;
    u_int16 seq_num;
    u_int8 payload_type;
    u_int8 marker;
    u_int8 frames;
}RDABT_A2DP_DATA_INFO;

typedef enum {
    A2DP_IDLE,            /* 0 */    
    A2DP_PLAY_AUDIO_VOL,    
    A2DP_PLAY_AUDIO,    
    A2DP_CONNECTED,                         /* 2 */
    A2DP_PLAY,                        /* 3 */
}app_a2dp_state_t;

void app_clean_a2dp_adp_info(void);
u_int8 app_get_a2dp_state(void);
void app_set_a2dp_state(u_int8 state);
u_int8 app_check_a2dp_connection(void);
void app_a2dp_play_sbc_after(void);
int app_find_a2dp_device_state(t_bdaddr addr);
void app_cancel_create_a2dp_connection(t_bdaddr addr);
APP_A2DP_DEVICE_INFO *app_find_a2dp_need_play_device(void);
void app_a2dp_play_sbc(t_bdaddr addr);
void app_a2dp_get_adp_info_req(void);
void app_a2dp_clean_timer(void);
void app_a2dp_handle_key(u_int32 key);
void app_a2dp_bt_state_machine(COS_EVENT *ev);
void app_create_a2dp_connection(u_int32 timeoutTenths, t_bdaddr addr);
BOOL app_a2dp_check_active(t_bdaddr addr);
void app_a2dp_drop_sbc(t_bdaddr *pAddr);
void app_a2dp_stop_sbc(void);
APP_A2DP_DEVICE_INFO *app_find_a2dp_device(t_bdaddr addr);
void app_a2dp_resume_sbc(t_bdaddr *pAddr);
void app_a2dp_check_device_conneciton(void *pArgs);
APP_A2DP_ADP_INFO *app_get_a2dp_adp_info(void);
void app_a2dp_play_sbc_filter(t_bdaddr addr);

extern void MMC_SyncAudioPlay(void);
extern u_int8 a2dp_connect_role;
extern u_int32 a2dp_param;
extern INT32 g_current_module;
extern void app_a2dp_send_data_handle(void *pArgs);
extern void app_send_audio_sync_data(SOUNBAR_AUDIO_SYNC_STRUCT* audio_sync);
extern void app_connect_a2dp(t_bdaddr *pAddr);
extern void app_disconnect_a2dp(t_bdaddr *pAddr);
extern void app_a2dp_msg_handle(COS_EVENT *ev);
extern void app_avrcp_msg_handle(COS_EVENT *ev);
extern void app_a2dp_send_suspend(t_bdaddr *addr);
extern void app_a2dp_switch_mode(void);
void app_a2dp_drop_other_deivce(t_bdaddr addr);
#if APP_SUPPORT_A2DP_SOURCE==1
void app_a2dp_send_start(t_bdaddr *addr);
void app_a2dp_send_data(t_bdaddr addr, u_int32 length, 
    u_int8 *p_data, u_int32 timestamp, u_int16 seq_num,
    u_int8 payload_type, u_int8 marker, u_int8 frames);
#endif


#endif

#endif

