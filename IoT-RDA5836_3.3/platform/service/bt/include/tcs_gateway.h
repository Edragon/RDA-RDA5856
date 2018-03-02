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


#ifndef BLUETOOTH_TCSGATEWAY_API_DECLARED
#define BLUETOOTH_TCSGATEWAY_API_DECLARED


APIDECL1 int APIDECL2 TCS_GWRegister(u_int8 flag, t_TCS_callbacks *callbacks);
APIDECL1 int APIDECL2 TCS_GWCordlessSetup(u_int16 *callHandle, t_bdaddr *TLbdAddress, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_GWIntercomSetup(u_int16 *callHandle, t_bdaddr TLbdAddress, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_GWConnect(u_int16 callHandle, t_TCS_callParams *callParams);
APIDECL1 int APIDECL2 TCS_GWReleaseComplete(u_int16 callHandle, t_TCS_callParams *callParams);

#define TCS_GWSetupAcknowledge      TCS_SetupAcknowledge
#define TCS_GWAlerting              TCS_Alerting
#define TCS_GWInformation			TCS_Information
#define TCS_GWCallProceeding		TCS_CallProceeding
#define TCS_GWConnectAcknowledge	TCS_ConnectAcknowledge
#define TCS_GWDisconnect			TCS_Disconnect
#define TCS_GWRelease				TCS_Release
#define TCS_GWStartDTMF				TCS_StartDTMF
#define TCS_GWStartDTMFAcknowledge	TCS_StartDTMFAcknowledge
#define TCS_GWStartDTMFReject		TCS_StartDTMFReject
#define	TCS_GWStopDTMF				TCS_StopDTMF
#define TCS_GWStopDTMFAcknowledge	TCS_StopDTMFAcknowledge
#define TCS_GWProgress				TCS_Progress
APIDECL1 int APIDECL2 TCS_GWDisconnectFromTL(t_bdaddr TLAddress);

#endif /* BLUETOOTH_TCSTERMINAL_API_DECLARED */

