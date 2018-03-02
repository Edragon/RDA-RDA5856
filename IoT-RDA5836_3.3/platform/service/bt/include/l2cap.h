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
#ifndef COMMON_L2CAP_H
#define COMMON_L2CAP_H

#ifdef __cplusplus
extern "C"
{
#endif

#define RDABT_L2CAP_EXTENDED_FEATURE_FLOWCONTROL_MODE                       0x00000001
#define RDABT_L2CAP_EXTENDED_FEATURE_RETRANSMISSION_MODE                    0x00000002
#define RDABT_L2CAP_EXTENDED_FEATURE_BIDIRECTIONAL_QOS                      0x00000004
#define RDABT_L2CAP_EXTENDED_FEATURE_ENHANCED_RETRANSMISSION_MODE           0x00000008
#define RDABT_L2CAP_EXTENDED_FEATURE_STREAMING_MODE                         0x00000010
#define RDABT_L2CAP_EXTENDED_FEATURE_FCS_OPTION                             0x00000020
#define RDABT_L2CAP_EXTENDED_FEATURE_EXTENDED_FLOW_SPECIFICATION            0x00000040
#define RDABT_L2CAP_EXTENDED_FEATURE_FIXED_CHANNELS                         0x00000080
#define RDABT_L2CAP_EXTENDED_FEATURE_EXTENDED_WINDOW_SIZE                   0x00000100
#define RDABT_L2CAP_EXTENDED_FEATURE_UNICAST_CONNECTIONLESS_DATA_RECEPTION  0x00000200

#define RDABT_L2CAP_EXTENDED_FEATURE_RESERVED                               0x80000000


typedef struct
{
	u_int32 result;
	u_int16 mtu;
	u_int16 handle;
}l2cap_conn_cnf_t;

typedef struct
{
	u_int16 length;
	u_int16 handle;
	u_int8* data;
}l2cap_data_ind_t;


APIDECL1 t_api APIDECL2 L2_GetInfo(t_bdaddr BDAddress, u_int16 infoType, u_int8 srcModule); 
APIDECL1 t_api APIDECL2 L2_Ping(t_bdaddr BDAddress,u_int8 *echoData, u_int16 length, void (*callbackFunc)(t_bdaddr BDAddress, u_int16 result, u_int8 *echoData, u_int16 length, u_int16 handle));
APIDECL1 t_api APIDECL2 L2_Echo_Disconnect(u_int16 handle);

// for l2cap bqb test
int L2CAP_ConnectToDevice(t_bdaddr address);
void L2CAP_Disconnect();
void L2CAP_SendData(u_int8 *buff, u_int32 length);
void L2CAP_LocalBusyDetected();
void L2CAP_LocalBusyClear();

#ifdef __cplusplus
}
#endif

#endif
