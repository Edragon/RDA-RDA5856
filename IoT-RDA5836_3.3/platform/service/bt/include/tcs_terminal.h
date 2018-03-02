/***********************************************************************
 *
 * MODULE NAME:    tcs_terminal.h
 * DESCRIPTION:    Bluetooth Host Stack TCS Terminal API Header File
 * AUTHOR:         Thomas Howley
 *
 /* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#ifndef BLUETOOTH_TCSTERMINAL_API_DECLARED
#define BLUETOOTH_TCSTERMINAL_API_DECLARED


APIDECL1 int APIDECL2 TCS_TLRegister(u_int8 flag, t_TCS_callbacks *callbacks, t_bdaddr *GWBDAddress, u_int8 connectToGW, void (*callback)(int status));
APIDECL1 int APIDECL2 TCS_TLConnectToTL(t_bdaddr TLbdAddress, void (*callbackFunc)(t_bdaddr address, int status));
APIDECL1 int APIDECL2 TCS_TLConnectToGW(t_bdaddr GWbdAddress, void (*callbackFunc)(t_bdaddr address, int status));
APIDECL1 int APIDECL2 TCS_TLIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_TLCordlessSetup(u_int16 *callHandle, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_TLRegisterRecall(u_int16 callHandle);
APIDECL1 int APIDECL2 TCS_TLConnect(u_int16 callHandle, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_TLReleaseComplete(u_int16 callHandle, t_TCS_callParams *callParams);

#define TCS_TLSetupAcknowledge      TCS_SetupAcknowledge
#define TCS_TLAlerting              TCS_Alerting
#define TCS_TLInformation			TCS_Information
#define TCS_TLCallProceeding		TCS_CallProceeding
#define TCS_TLConnectAcknowledge	TCS_ConnectAcknowledge
#define TCS_TLDisconnect			TCS_Disconnect
#define TCS_TLRelease				TCS_Release
#define TCS_TLStartDTMF				TCS_StartDTMF
#define TCS_TLStartDTMFAcknowledge	TCS_StartDTMFAcknowledge
#define TCS_TLStartDTMFReject		TCS_StartDTMFReject
#define	TCS_TLStopDTMF				TCS_StopDTMF
#define TCS_TLStopDTMFAcknowledge	TCS_StopDTMFAcknowledge
#define TCS_TLProgress				TCS_Progress

APIDECL1 int APIDECL2 TCS_TLDisconnectFromGWorTL(void);
#define TCS_TLDisconnectFromTL TCS_TLDisconnectFromGWorTL
#define TCS_TLDisconnectFromGW TCS_TLDisconnectFromGWorTL




/*
APIDECL1 t_api APIDECL2 TCS_TLDisconnectFromTL();
APIDECL1 t_api APIDECL2 TCS_TLDisconnectFromGW();
*/


#endif /* BLUETOOTH_TCSTERMINAL_API_DECLARED */

