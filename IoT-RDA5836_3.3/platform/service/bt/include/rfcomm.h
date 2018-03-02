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
 * MODULE NAME:    rfcomm.h
 * DESCRIPTION:    Bluetooth Host Stack RFCOMM API header
 * AUTHOR:         David Airlie, Martin Mellody
 *
 * SOURCE CONTROL: $Id: rfcomm.h,v 1.3 2008/12/18 09:19:44 tianwq Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2002 RDA Microelectronics.
 *     All rights reserved.
 *
 * REMARKS:
 *     This file should be incuded via bt.h only.
 *
 ***********************************************************************/


#ifndef BLUETOOTH_RFCOMM_API_DECLARED
#define BLUETOOTH_RFCOMM_API_DECLARED


/* 
   NOTE:	These functions are provided by the Port Entity and not by
   RFCOMM itself. These functions are concerned with Port Entity
   setup only. The actual RFCOMM transport interface is provided 
   by the Port Entity in an OS-specific manner.
*/


/* Connection states for port entity callback function newState argument*/
#define RFCOMM_PE_CONNECTED  		0
#define RFCOMM_PE_DISCONNECTED 		1
#define RFCOMM_PE_CONNECTFAILED		2


#ifdef __cplusplus
extern "C" {
#endif


/* Server side functions */
APIDECL1 int APIDECL2 PE_SrvRegisterPort(char *portName, u_int8 *serverChan, void (*callbackFunc)(u_int8 serverChan, t_bdaddr remoteDevice, u_int8 newState));
APIDECL1 int APIDECL2 PE_SrvDeregisterPort(u_int8 serverChan);
APIDECL1 int APIDECL2 PE_SrvDisconnectPort(u_int8 serverChan);


/* Client side functions */
APIDECL1 int APIDECL2 PE_CliConnectPort(t_bdaddr remoteDevice, u_int8 serverChan, char *portName, void (*callbackFunc)(u_int8 serverChan, t_bdaddr remoteDevice, u_int8 newState));
APIDECL1 int APIDECL2 PE_CliDisconnectPort(t_bdaddr remoteDevice, u_int8 serverChan);
APIDECL1 int APIDECL2 PE_GetCIDFromConnection(t_bdaddr bdaddr, u_int8 dlci, u_int16 *cid);


/* RDA Stack Entity code.. can be used to implement data-only services 
   on top of RFCOMM */

/* defines for flow indication */
#define RSE_FLOW_STOP 0
#define RSE_FLOW_GO 1

/* defines for connection confirmation */
#define RSE_CONNECT_REFUSE 0
#define RSE_CONNECT_ACCEPT 1

typedef struct st_t_RSE_Callbacks
{
  int (*connectionConfirmation)(u_int8 schan, t_bdaddr address, u_int8 status, u_int8 dlci, u_int16 max_frame_size);
  int (*connectionIndication)(u_int8 schan, t_bdaddr address, u_int8 dlci, u_int16 max_frame_size);
  int (*dataIndication)(t_bdaddr address, u_int8 dlci, t_DataBuf *p_buf);
  int (*flowIndication)(t_bdaddr address, u_int8 dlci,  u_int8 flow_state);  
  int (*disconnectionIndication)(t_bdaddr address, u_int8 dlci);
  int (*framesizeIndication)(t_bdaddr address, u_int8 dlci, u_int16 max_frame_size);
} t_RSE_Callbacks;

APIDECL1 int APIDECL2 RSE_Init(void);
APIDECL1 int APIDECL2 RSE_SrvRegisterPort(u_int8 *serv_chan, t_RSE_Callbacks *cbs);
APIDECL1 int APIDECL2 RSE_SrvDeregisterPort(u_int8 schan);
APIDECL1 int APIDECL2 RSE_SrvDisconnectPort(u_int8 schan);
APIDECL1 int APIDECL2 RSE_CliConnectPort(t_bdaddr remote_bd_addr, u_int8 rem_schan, u_int16 max_frame_size, t_RSE_Callbacks *cbs);
APIDECL1 int APIDECL2 RSE_CliDisconnectPort(t_bdaddr remote_bd_addr, u_int8 schan);
APIDECL1 int APIDECL2 RSE_Write(t_bdaddr remote_bd_addr, u_int8 dlci, t_DataBuf *p_buf);
APIDECL1 int APIDECL2 RSE_SrvConnectResponse(t_bdaddr remote_bd_addr, u_int8 dlci, u_int8 accept);
APIDECL1 int APIDECL2 RSE_GetWriteBuffer(t_bdaddr bdaddr, int dlci, int length, u_int32 flags, t_DataBuf **p_buf);

APIDECL1 int APIDECL2 RSE_FreeWriteBuffer(rdabt_host_buf_t *frame);
APIDECL1 int APIDECL2 RSE_GetCIDFromConnection(t_bdaddr bdaddr, u_int8 dlci, u_int16 *cid);

#ifdef __cplusplus
} /* extern "C" { */
#endif


#endif /* BLUETOOTH_RFCOMM_API_DECLARED */
