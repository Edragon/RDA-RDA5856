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

#ifndef _RDABT_A2DP_H_
#define _RDABT_A2DP_H_

#include "tgt_app_cfg.h"

#include "a2dp.h"
#include "rdabt_common.h"
#include "rdabt_hfp.h"

/* SBC configuration bit fields */
#define SBC_SAMPLING_FREQ_16000        128   /* Octet 0 */
#define SBC_SAMPLING_FREQ_32000         64
#define SBC_SAMPLING_FREQ_44100         32
#define SBC_SAMPLING_FREQ_48000         16
#define SBC_CHANNEL_MODE_MONO            8
#define SBC_CHANNEL_MODE_DUAL_CHAN       4
#define SBC_CHANNEL_MODE_STEREO          2
#define SBC_CHANNEL_MODE_JOINT_STEREO    1
#define SBC_BLOCK_LENGTH_4             128   /* Octet 1 */
#define SBC_BLOCK_LENGTH_8              64
#define SBC_BLOCK_LENGTH_12             32
#define SBC_BLOCK_LENGTH_16             16
#define SBC_SUBBANDS_4                   8
#define SBC_SUBBANDS_8                   4
#define SBC_ALLOCATION_SNR               2
#define SBC_ALLOCATION_LOUDNESS          1
#define SBC_BITPOOL_MIN                  2   /* Octet 2 (min bitpool)  /  Octet 3 (max bitpool) */
#define SBC_BITPOOL_MAX                53
#define SBC_BITPOOL_MEDIUM_QUALITY      32
#define SBC_BITPOOL_HIGH_QUALITY        50




/*!
	@brief The capability to stream media. This is manditory for advance audio
	distribution profile.
*/
#define AVDTP_SERVICE_MEDIA_TRANSPORT		(1)		
/*!
	@brief The reporting capability. This is not currently supported.
*/
#define AVDTP_SERVICE_REPORTING				(2)		
/*!
	@brief The recovery capability. This is not currently supported.
*/
#define AVDTP_SERVICE_RECOVERY				(3)		
/*!
	@brief The content protection capability. This is not currently supported.
*/
#define AVDTP_SERVICE_CONTENT_PROTECTION	(4)		
/*!
	@brief The header compression capability. This is not currently supported.
*/
#define AVDTP_SERVICE_HEADER_COMPRESSION	(5)		
/*!
	@brief The multiplexing capability. This is not currently supported.
*/
#define AVDTP_SERVICE_MULTIPLEXING			(6)		
/*!
	@brief The codec capability for the Stream End Point.
*/
#define AVDTP_SERVICE_MEDIA_CODEC			(7)		

#define A2DP_ADP_STATE_IDLE                    0
#define A2DP_ADP_STATE_CLOSE                   1
#define A2DP_ADP_STATE_PENDING             2
#define A2DP_ADP_STATE_CONNECTED         3
#define A2DP_ADP_STATE_OPEN                   4
#define A2DP_ADP_STATE_PLAY                    5
#define A2DP_ADP_STATE_PAUSE                  6
#define A2DP_ADP_STATE_STOP                    7


#define A2DP_USED                    1
#define A2DP_UNUSED                0

typedef enum
{
    BT_CLOCK=0,
    NATIVE_CLOCK
}RDABT_A2DP_CLOCK;

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
    u_int8 waiting_pause_flag;
}RDABT_A2DP_DEVICE_INFO;

typedef struct
{
	RDABT_A2DP_DEVICE_INFO *a2dp_device_info_list;
    u_int8 a2dp_max_device_num;
    u_int8 a2dp_connect_index;
    t_a2dp_stream_config_ind a2dp_config;
    t_bdaddr last_addr;
}RDABT_A2DP_ADP_INFO;

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
        u_int8* p_data;
        u_int32 timestamp;
        u_int16 seq_num;
        u_int8 payload_type;
        u_int8 marker;
        u_int8 frames; 
}RDABT_A2DP_DATA_INFO;

void rdabt_a2dp_signal_connect_cnf_cb(void *pArgs);
void rdabt_a2dp_signal_disconnect_cnf_cb(void *pArgs);
void rdabt_a2dp_media_connect_cnf_cb(void *pArgs);
void rdabt_a2dp_media_disconnect_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_discover_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_get_capabilities_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_set_config_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_reconfig_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_open_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_start_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_pause_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_close_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_abort_cnf_cb(void *pArgs);
void rdabt_a2dp_signal_connect_ind_cb(void *pArgs);
void rdabt_a2dp_signal_disconnect_ind_cb(void *pArgs);
void rdabt_a2dp_media_connect_ind_cb(void *pArgs);
void rdabt_a2dp_media_disconnect_ind_cb(void *pArgs);
void rdabt_a2dp_media_data_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_discover_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_get_capabilities_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_set_config_ind_cb(void *pArgs);
void rdabt_a2dp_sep_reconfig_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_open_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_start_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_pause_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_close_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_abort_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_sec_control_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_get_all_cap_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_delay_report_cnf_cb(void *pArgs);
void rdabt_a2dp_send_sep_sec_control_ind_cb(void *pArgs);
void rdabt_a2dp_sep_get_all_cap_ind_cb(void *pArgs);
void rdabt_a2dp_send_sep_delay_report_ind_cb(void *pArgs);

void rdabt_a2dp_msg_handle(u_int16 msg_id, void* msg_data);

void rdabt_a2dp_register_source_sdp(void);
void rdabt_a2dp_register_sink_sdp(void);
void rdabt_a2dp_send_data(RDABT_A2DP_DATA_INFO *data_info);
void rdabt_a2dp_read_syncclk_and_start(void);
u_int8 rdabt_get_soundbar_clk_role(void);
u_int32 rdabt_a2dp_check_syncclock(u_int32 milliseconds);
void rdabt_a2dp_set_audio_sync_clk(u_int32 synclk,u_int32 counternum);
RDABT_A2DP_DEVICE_INFO *rdabt_get_a2dp_active_device(void);
RDABT_A2DP_DEVICE_INFO *rdabt_get_a2dp_device_info(t_bdaddr addr);
void rdabt_a2dp_resume_sbc(t_bdaddr addr);

void rdabt_a2dp_init(void);
void rdabt_a2dp_free(void);
void rdabt_a2dp_connect(t_bdaddr addr);
void rdabt_a2dp_connect_media(t_bdaddr addr);
void rdabt_a2dp_disconnect(t_bdaddr addr);
void rdabt_a2dp_disconnect_all(void);
void rdabt_a2dp_return_adp_info(void);
void rdabt_a2dp_send_suspend(t_bdaddr addr);
void rdabt_a2dp_drop_sbc(t_bdaddr addr);
void rdabt_a2dp_send_start(t_bdaddr addr);



#endif




