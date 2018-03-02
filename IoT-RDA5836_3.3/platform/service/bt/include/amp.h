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
#ifndef AMP_TEST_H
#define AMP_TEST_H

#define MAX_PHYLINK_NUM	5
#define MAX_CONTROLLER 	MAX_PHYLINK_NUM

typedef struct st_ampTestRemoteInfo
{
	u_int8		controllerNum;
	struct
	{
		u_int8	controlID;
		u_int8	controlType;
		u_int8	controlStatus;
	}controlInfo[MAX_CONTROLLER];

}ampTestRemoteInfo;

int AMPTestGetRemotePeerAddress(t_bdaddr *pAddr);

int AMPTestGetRemoteControlInfo(t_bdaddr address, ampTestRemoteInfo *pInfo);

int AMPTestWriteRemoteAMPInfo(t_bdaddr address, u_int8 controlID);

#endif