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
  
#ifndef BT_DRV
#define BT_DRV

//ACCEL Packet Format
#define ASCP_MAX_PAYLOAD_LENGTH 240
#define FLAG_START 0xc0
#define FLAG_END 0xc0
#define FLAG_ESCAPE 0x7e

/*ADP->DRV*/
//BASE FUNCTION
#define BT_POWERON						0x01
#define BT_SET_DISCOVER				0x02
#define BT_SET_NAME					0x03
#define BT_SET_SERVICE					0x04
#define BT_INQUIRY						0x05
#define BT_INQ_CANCEL					0x15
#define BT_NAME_REQ					0x07
#define BT_NAME_REQ_CANCEL			0x17
#define BT_PAIR							0x0A
#define BT_UNPAIR						0x09
#define BT_PAIR_PASSKEY				0x0B
#define BT_SERVICE_DISCOVERY			0x0C
#define BT_SET_DEV_ADDR				0x0D
#define BT_SEND_LINKKEY				0x0F
#define BT_RESET						0x41
#define BT_POWEROFF					0x42
#define BT_ENTER_LPMODE				0x43
#define BT_EXIT_LPMODE					0x44
#define DRV_LPM
#ifdef DRV_LPM
#define BT_LPM_SYNC_REQ				0x46
#endif
//HANDSET FUNCTION
#define BT_CONNECT_HF					0x61
#define BT_DISCONNECT_HF				0x62
#define BT_SCO_CONNECT					0x63
#define BT_SCO_DISCONNECT				0x64
#define BT_HF_TO_HANDSET				0x65
#define BT_HANDSET_TO_HF				0x66
#define BT_CALL_ACTIVED					0x67
#define BT_TERMINATE_CALL				0x68
#define BT_SEND_CALL_RING				0x69
#define BT_INT_OUTGOING_CALL			0x6B
#define BT_REMOTE_ALERT				0x6C
//OPP FUNCTION
#define BT_CONNECT_SER					0x31
#define BT_DISCONNECT_SER				0x32
#define BT_CLI_PULL_OBJ					0x33
#define BT_CLI_PUSH_OBJ_NAME			0x34
#define BT_CLI_ABORT_REQ				0x35
#define BT_NAMETO_PULL_REQ			0x36
#define BT_SEND_OBJ_DATA				0x37
#define BT_OPP_SRESP_CONNECT			0x38


/*CHIP->DRV*/
//BASE FUNCTION
#define COMMAND_STATUS_NOTIFY     		0x81
#define INQUIRY_RESULT                     		0x82
#define INQUIRY_COMPLETE                 		0x83
#define PASSKEY_REQUEST_NOTIFY 		0x84
#define PAIR_RESULT						0x85
#define NAME_REQUEST_RESULT			0x86
#define SERVICE_RESULT_NOTIFY 			0x87
#define ASCP_STATE_NOTIFY				0x88
#define LINKKEY_READ_IND				0x89
#define LINKKEY_WRITE_IND				0x8A
//
#define APPLICATION_STATE_NOTIFY		0x91
#define CALL_STATUS_NOTIFY				0x92
#define CALLER_INFORMATION_NOTIFY 	0x93
//OPP FUNCTION
#define OPP_APPLICATION_STSTE_NOTIFY	0xB1
#define OPP_SERVER_PULL_IND			0xB2
#define OPP_CLIENT_REC_PUSH_RESP_IND	0xB3
#define OPP_CLIENT_PULL_MORE_IND		0xB4
#define OPP_OBJECT_NAME_IND			0xB5
#define OPP_OBJECT_DATA_IND			0xB6
#define OPP_CLIENT_CONNECT_REQ_IND	0xBA
#define OPP_CLIENT_CONNECT_IND		0xBB
//
#define FTP_APPLICATION_STSTE_NOTIFY	0xC1
#define FTP_CLIENT_PULL_LIST_IND		0xC2
#define FTP_CLIENT_SET_PATH_ROOT_IND	0xC5
#define FTP_PULL_FILE_NAME_IND		0xC6
#define FTP_CLIENT_PULL_FILE_IND		0xC7
#define FTP_PUSH_FILE_NAME_IND		0xC8
#define FTP_CLIENT_PUSH_FILE_IND		0xC9
#define FTP_CLIENT_SET_PATH_IND		0xCC
#define FTP_CLIENT_CONNECT_REQ_IND	0xCD
//HANDSET FUNCTION
#define AG_APPLICATION_STATE_NOTIFY	0xE1
#define REMOTE_BUTTON_IND				0xE2
#define DATA_PASS_TO_HOST				0xE3


#define COMMAND_REC       1
#define COMMAND_SEND     2

#define AG_CONNECTED					3
#define HANDSET							2
#define HANDFREE						4

#define UART_RECBUF_SIZE				512

#define BT_DURATION						16384
#define BT_TIMER_EVT_ID					0x40000000+0xc002

#define LAST_PACKET_HEADER			3
#define MIDDLE_PACKET_HEADER			2
#define MIDDLE_SEGMENT_PACKET			1
#define LAST_SEGMENT_PACKET			0

#define BT_MSG_RSP_BASE				6000
#define BT_MSG_IND_BASE				6100

#define AG_CMD_RSP_MASK				0x01
#define AG_CMD_IND_MASK				0x02
#define OPP_REC_RSP_MASK				0x04
#define OPP_SEND_RSP_MASK				0x08
#define AG_DISCON_RSP_MASK			0x10
#define LINKKEY_RSP_MASK				0x20
#define OPP_SER_DISC_MASK				0x40

typedef struct 
{
	BOOL chip_power;
	BOOL chip_uart;
	BOOL inquiry_status;
	BOOL ag_state;
	BOOL opp_state;
	BOOL g_agconnectresult;
	BOOL g_oppconnectresult;
	BOOL ring_state;
}bt_state_machine;
//#define IRQ_ADJ_FLAG
#ifdef IRQ_ADJ_FLAG
#define IRQ_DMA 0
#define IRQ_TIMEOUT 1
#define HW_ERROR 4
#define SEND_OVER 3
typedef struct 
{
	BOOL buffer_select;
	
}bt_state_opp;
#endif

typedef struct 
{
	UINT32 EventId;
	UINT32 Param1;
	UINT32 Param2;
	UINT32 Param3;
} sendEvent;

typedef struct
{
	UINT8	status; 
	UINT8	idx;
	UINT8	name[21];
	UINT8	bd_addr[13];//the address is 6byte, last 7byte is no use for mmi
	UINT8	linkkey[22];
	UINT8	type;
} bt_drv_pair_ind_struct;//bt_drv_inq_ind_struct,bt_drv_passkey_ind_struct;
typedef  bt_drv_pair_ind_struct bt_drv_inq_ind_struct;
typedef  bt_drv_pair_ind_struct bt_drv_passkey_ind_struct;


typedef struct
{
	UINT8	result;
	UINT8	bd_addr[6];
	UINT8	link_key[16];
	UINT16	mtu;
}bt_connect_result_struct;

typedef  bt_connect_result_struct bt_drv_linkkey_ind_struct;
typedef  bt_connect_result_struct bt_drv_AG_ind_struct;

typedef enum
{
	BT_DRV_INQ_COMPLETE,
	BT_DRV_INQ_CANCEL,
	BT_DRV_INQ_MAX_RECV, 
	BT_DRV_INQ_NOT_FINISH,
	BT_DRV_INQ_FAIL
}bt_drv_inq_status_enum;

typedef enum
{
	BT_DRV_PAIR_OK,
	BT_DRV_PAIR_TIMEOUT,
	BT_DRV_PAIR_FAIL, 
	BT_DRV_PAIR_LINKKEY,
	BT_DRV_UNPAIR_OK
}bt_drv_pair_result_enum;

typedef enum 
{
	BT_PROFILE_OPP = 4,
	BT_PROFILE_FTP = 8,
	BT_PROFILE_AG =16
}BT_SERVICE_T;


enum BT_MESSAGE_RSP
{
	BT_POWERON_RSP = BT_MSG_RSP_BASE,
	BT_SET_DISCOVER_RSP,
	BT_SET_NAME_RSP,
	BT_SET_SERVICE_RSP,
	BT_INQUIRY_RSP,
	BT_INQ_CANCEL_RSP,					
	BT_NAME_REQ_RSP,					
	BT_NAME_REQ_CANCEL_RSP,			
	BT_PAIR_RSP,						
	BT_UNPAIR_RSP,						
	BT_PAIR_PASSKEY_RSP,			
	BT_SERVICE_DISCOVERY_RSP,			
	BT_SET_DEV_ADDR_RSP,				
	BT_CONNECT_HF_RSP,					
	BT_DISCONNECT_HF_RSP,				
	BT_HF_TO_HANDSET_RSP,				
	BT_HANDSET_TO_HF_RSP,				
	BT_CALL_ACTIVED_RSP,				
	BT_TERMINATE_CALL_RSP,				
	BT_SEND_CALL_RING_RSP,				
	BT_INT_OUTGOING_CALL_RSP,			
	BT_REMOTE_ALERT_RSP,				
	BT_OPP_SRESP_SEND_RSP,			
	BT_CONNECT_SER_RSP,					
	BT_DISCONNECT_SER_RSP,				
	BT_CLI_PULL_OBJ_RSP,					
	BT_SEND_NAME_RSP,			
	BT_SEND_OBJ_DATA_RSP,				
	BT_CLI_ABORT_RSP,				
	BT_STAT_ERR_RSP,
	#ifdef DRV_LPM
	BT_NAMETO_PULL_RSP,
	BT_LPM_SYNC_RSP
	#else
	BT_NAMETO_PULL_RSP			
	#endif
};

enum BT_MESSAGE_IND
{
	BT_COMMAND_STATUS_IND = BT_MSG_IND_BASE,
	BT_INQUIRY_RESULT_IND,
	BT_INQUIRY_COMPLETE_IND,
	BT_PASSKEY_REQUEST_IND,
	BT_LINKKEY_READ_IND, //for trust list
	BT_LINKKEY_WRITE_IND,
	BT_PAIR_RESULT_IND,
	BT_NAME_REQUEST_RESULT_IND,
	BT_SERVICE_RESULT_IND,
	BT_OPP_STSTE_IND,
	BT_OPP_SERVER_PULL_IND,
	BT_OPP_OBJECT_NAME_IND,
	BT_OPP_OBJECT_DATA_IND,
	BT_OPP_CLI_CONNECT_REQ_IND,
	BT_OPP_CLI_CONNECT_IND,
	BT_AUDIO_STATE_IND,
	BT_REMOTE_BUTTON_IND,
	BT_DISCONNECT_SER_IND,
	BT_DISCONNECT_HF_IND,
	BT_HW_ERROR_IND
};


typedef struct
{
	BOOL discoverable;
	BOOL unpairall;
	BOOL allowconnect;
	BOOL rsp;
	UINT16 length;
	UINT8 seq;
	UINT8 *file_buf ;
	UINT8 name[20];//device name
	UINT8 file_name[120];//file name
	UINT8 address[10];//the address is 6byte, last 4byte is no use for mmi
	UINT8 passkey[16];
	UINT8 data[120];
	UINT16 name_len;
	UINT32 total_len;
	BT_SERVICE_T service;
}BT_CMDSTRUCT_T;

typedef struct
{
	UINT32 para1;
	UINT32 para2;
	BT_CMDSTRUCT_T* para3;
}MsgBody;

typedef struct 
{
	UINT32    Id ;        // Identifier Unique for whole stack
}MsgHead;

typedef struct 
{
	MsgHead   H;
	MsgBody   B;
}Msg;


typedef VOID (*bt_sendcmd)(sendEvent* message);
typedef VOID (*bt_cpydata)(UINT8* src,UINT16 len,UINT32 ishead,UINT32 *for_len,UINT32* back_len,UINT8 is_complete);
typedef VOID (*bt_recv_name)(UINT8* name,UINT8 name_len);
typedef VOID (*bt_recv_data)(UINT8 is_last_pkt,UINT16 back_len,UINT16 for_len,BOOL is_tail);

VOID bt_CreateTask(bt_sendcmd callback,bt_cpydata cpycallback);
VOID bt_SendMessage(UINT8 msgid,BT_CMDSTRUCT_T* cmd);
BOOL bt_Inialize(VOID);
VOID btd_Restart(VOID);
BOOL bt_Inquiry(VOID);
BOOL bt_InquiryCancel(VOID);
BOOL bt_NameReq(UINT8* name);
BOOL bt_NameReqCancel(UINT8* address);
BOOL bt_Pair(UINT8* address);
BOOL bt_Unpair(BOOL unpairall,UINT8* addess);
BOOL bt_PairPasskey(UINT8 length,UINT8* passkey);
BOOL bt_ServiceDiscovery(UINT8* address);
BOOL bt_SetDevAddress(UINT8* address);
BOOL bt_ConnectAg(UINT8* addess);
BOOL bt_DisConnectAg(VOID);
BOOL bt_HfToAg(VOID);
BOOL bt_AgToHf(VOID);
BOOL bt_ConnectHf(UINT8* addess);
BOOL bt_DisConnectHf(VOID);
BOOL bt_ScoConnect(VOID);
BOOL bt_SetService(BT_SERVICE_T service);
BOOL bt_SetDiscover(BOOL discoverable);
BOOL bt_SetName(UINT8 length,UINT8* name);
BOOL bt_SendCallRing(VOID);
BOOL bt_IntialOutGoingCall(VOID);
BOOL bt_RemoteBeAlert(VOID);
BOOL bt_CallActived(VOID);
BOOL bt_TerminateCall(VOID);
BOOL bt_ServiceDiscovery(UINT8* address);
BOOL bt_RemoteBeAlert(VOID);
BOOL bt_ConnectPushServer(UINT8* address);
BOOL bt_DisConnectPushServer(VOID);
BOOL bt_ClientPushObjectName(UINT32 total_len,UINT16 name_len,UINT8* nameptr,UINT16 pkt_len);
BOOL bt_OppServerResponseConnect(BOOL allowconnect);
BOOL bt_NameToPullRequest(UINT8 length,UINT8* data);
BOOL bt_SendObjectData(UINT8 num,UINT16 length,UINT8* data);
BOOL bt_ClientPullObject(VOID);
BOOL bt_ClientAbortRequest(VOID);
BOOL bt_FtpServerResponseConnect(BOOL allowconnect);
BOOL bt_FtpServerSendData(UINT8 num,UINT8 length,UINT8* address);
BOOL bt_FtpServerResponse(UINT8* response);
BOOL bt_FtpServerSendPullReq(UINT8* objlength);
BOOL bt_ScoDisconnect(VOID);
VOID btd_BtCfgCmd(VOID);
VOID btd_EnterLpMode(VOID);
VOID btd_ExitLpMode(VOID);
VOID btd_FuncRegister(bt_recv_name name, bt_recv_data data );
BOOL bt_LinkkeyReply(UINT8* addrWithLinkKey,UINT8 length);
VOID btd_free(VOID* msg);


#endif
