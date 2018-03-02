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


#ifndef PN544_H
#define PN544_H

#include "cs_types.h"

/* Pipes */
#define NFC_HCI_INVALID_PIPE    0x80
#define NFC_HCI_LINK_MGMT_PIPE  0x00
#define NFC_HCI_ADMIN_PIPE      0x01
///////////////////////////////////
#define PN544_IDMGT_PIPE         0x02

 /* Host IDs */
 #define NFC_HCI_HOST_CONTROLLER_ID      0x00
 #define NFC_HCI_TERMINAL_HOST_ID        0x01
 #define NFC_HCI_UICC_HOST_ID            0x02
 
 /* Host Controller Gates and registry indexes */
 #define NFC_HCI_ADMIN_GATE 0x00
 #define NFC_HCI_ADMIN_SESSION_IDENTITY  0x01
 #define NFC_HCI_ADMIN_MAX_PIPE          0x02
 #define NFC_HCI_ADMIN_WHITELIST         0x03
 #define NFC_HCI_ADMIN_HOST_LIST         0x04
 
 #define NFC_HCI_LOOPBACK_GATE           0x04
 
 #define NFC_HCI_ID_MGMT_GATE            0x05
 #define NFC_HCI_ID_MGMT_VERSION_SW      0x01
 #define NFC_HCI_ID_MGMT_VERSION_HW      0x03
 #define NFC_HCI_ID_MGMT_VENDOR_NAME     0x04
 #define NFC_HCI_ID_MGMT_MODEL_ID        0x05
 #define NFC_HCI_ID_MGMT_HCI_VERSION     0x02
 #define NFC_HCI_ID_MGMT_GATES_LIST      0x06
 
 #define NFC_HCI_LINK_MGMT_GATE          0x06
 #define NFC_HCI_LINK_MGMT_REC_ERROR     0x01
 
 #define NFC_HCI_RF_READER_B_GATE                        0x11
 #define NFC_HCI_RF_READER_B_PUPI                        0x03
 #define NFC_HCI_RF_READER_B_APPLICATION_DATA            0x04
 #define NFC_HCI_RF_READER_B_AFI                         0x02
 #define NFC_HCI_RF_READER_B_HIGHER_LAYER_RESPONSE       0x01
 #define NFC_HCI_RF_READER_B_HIGHER_LAYER_DATA           0x05
 
 #define NFC_HCI_RF_READER_A_GATE                0x13
 #define NFC_HCI_RF_READER_A_UID                 0x02
 #define NFC_HCI_RF_READER_A_ATQA                0x04
 #define NFC_HCI_RF_READER_A_APPLICATION_DATA    0x05
 #define NFC_HCI_RF_READER_A_SAK                 0x03
 #define NFC_HCI_RF_READER_A_FWI_SFGT            0x06
 #define NFC_HCI_RF_READER_A_DATARATE_MAX        0x01
 
 #define NFC_HCI_TYPE_A_SEL_PROT(x)              (((x) & 0x60) >> 5)
 #define NFC_HCI_TYPE_A_SEL_PROT_MIFARE          0
 #define NFC_HCI_TYPE_A_SEL_PROT_ISO14443        1
 #define NFC_HCI_TYPE_A_SEL_PROT_DEP             2
 #define NFC_HCI_TYPE_A_SEL_PROT_ISO14443_DEP    3
 
 /* Generic events */
 #define NFC_HCI_EVT_HCI_END_OF_OPERATION        0x01
 #define NFC_HCI_EVT_POST_DATA                   0x02
 #define NFC_HCI_EVT_HOT_PLUG                    0x03
 
 /* Reader RF gates events */
 #define NFC_HCI_EVT_READER_REQUESTED    0x10
 #define NFC_HCI_EVT_END_OPERATION       0x11
 
 /* Reader Application gate events */
 #define NFC_HCI_EVT_TARGET_DISCOVERED   0x10

#define RESET_FRAME              0x00
#define ANY_SET_PARAMETER  0x01
#define ANY_GET_PARAMETER  0x02
#define ANY_OPEN_PIPE	   0x03
#define ANY_CLOSE_PIPE     0x04

#define ADM_CREATE_PIPE    		0x10
//#define ADM_NOTIFY_PIPE_CREATED  	0x11
#define ANY_DELETE_PIPE    		0x11
#define ADM_NOTIFY_PIPE_DELETED 	0x13
#define ANY_CLEAR_ALL_PIPE    		0x14
#define ANY_NOTIFY_ALL_PIPE_CLEARED     0x15

#define HCP_COMMAND_TYPE   0x00
#define HCP_EVENTS_TYPE 	 0x01
#define HCP_RESPONSES_TYPE 0x02

#define CREATE_SUCCESS 0x00
#define CREATE_ERROR   0xff

#define PN544_CHIP_ADDR 0x28



/* HCP headers */
#define NFC_HCI_HCP_PACKET_HEADER_LEN   1
#define NFC_HCI_HCP_MESSAGE_HEADER_LEN  1
#define NFC_HCI_HCP_HEADER_LEN          2

/* HCP types */
 #define NFC_HCI_HCP_COMMAND     0x00
 #define NFC_HCI_HCP_EVENT       0x01
 #define NFC_HCI_HCP_RESPONSE    0x02
 
 /* Generic commands */
 #define NFC_HCI_ANY_SET_PARAMETER       0x01
 #define NFC_HCI_ANY_GET_PARAMETER       0x02
 #define NFC_HCI_ANY_OPEN_PIPE           0x03
 #define NFC_HCI_ANY_CLOSE_PIPE          0x04
 
 /* Reader RF commands */
 #define NFC_HCI_WR_XCHG_DATA            0x10
 
 /* Admin commands */
 #define NFC_HCI_ADM_CREATE_PIPE                 0x10
 #define NFC_HCI_ADM_DELETE_PIPE                 0x11
 #define NFC_HCI_ADM_NOTIFY_PIPE_CREATED         0x12
 #define NFC_HCI_ADM_NOTIFY_PIPE_DELETED         0x13
 #define NFC_HCI_ADM_CLEAR_ALL_PIPE              0x14
 #define NFC_HCI_ADM_NOTIFY_ALL_PIPE_CLEARED     0x15
 
 /* Generic responses */
 #define NFC_HCI_ANY_OK                          0x00
 #define NFC_HCI_ANY_E_NOT_CONNECTED             0x01
 #define NFC_HCI_ANY_E_CMD_PAR_UNKNOWN           0x02
 #define NFC_HCI_ANY_E_NOK                       0x03
 #define NFC_HCI_ANY_E_PIPES_FULL                0x04
 #define NFC_HCI_ANY_E_REG_PAR_UNKNOWN           0x05
 #define NFC_HCI_ANY_E_PIPE_NOT_OPENED           0x06
 #define NFC_HCI_ANY_E_CMD_NOT_SUPPORTED         0x07
 #define NFC_HCI_ANY_E_INHIBITED                 0x08
 #define NFC_HCI_ANY_E_TIMEOUT                   0x09
 #define NFC_HCI_ANY_E_REG_ACCESS_DENIED         0x0a
 #define NFC_HCI_ANY_E_PIPE_ACCESS_DENIED        0x0b



#define HCP_HEADER(type, instr) ((((type) & 0x03) << 6) | ((instr) & 0x3f))
#define NFC_HCI_FRAGMENT        0x7f



typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT8 window_len;
	UINT8 endpoint;
	UINT8 crc[2];
}Reset_packet;

typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT32 cb:1;
	UINT32 pid:7;
	UINT32 type:2;
	UINT32 ins:6;
	UINT8 crc[2];
}Hcp_packet;

typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT32 cb:1;
	UINT32 pid:7;
	UINT32 type:2;
	UINT32 ins:6;
	UINT8 data;
	UINT8 crc[2];
}Hcp_data1_packet;

typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT32 cb:1;
	UINT32 pid:7;
	UINT32 type:2;
	UINT32 ins:6;
	UINT8 data[2];
	UINT8 crc[2];
}Hcp_data2_packet;

typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT32 cb:1;
	UINT32 pid:7;
	UINT32 type:2;
	UINT32 ins:6;
	UINT8 data[3];
	UINT8 crc[2];
}Hcp_data3_packet;

typedef struct
{
	UINT8 llc_len;
	UINT8 llc_header;
	UINT32 cb:1;
	UINT32 pid:7;
	UINT32 type:2;
	UINT32 ins:6;
	UINT8 data[4];
	UINT8 crc[2];
}Hcp_data4_packet;

#endif

