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
#ifndef __GATT_TESTER_H__
#define __GATT_TESTER_H__

#ifdef __cplusplus
extern "C"
{
#endif

enum
{
	HTP_CONNECT_SUCCEED = 0,
	HTP_CONNECT_FAILED,
};

typedef struct 
{
	u_int8 result;
}gatt_tester_conn_cnf_t;

typedef struct
{
	u_int16 reqCode;
	s_int16 payLoadLen;
	u_int8	result;
	u_int8* rspPayLoad;
}gatt_tester_req_rsp_t;

int GATT_Tester_Connect(t_bdaddr address);

int GATT_Tester_Disconnect(t_bdaddr address);

int GATT_Tester_DiscoverAllService(u_int8 type, u_int8* value, u_int8 valueLen);

int GATT_Tester_DiscoverAllCharacteristic(u_int16 StartHandle, u_int16 EndHandle);

int GATT_Tester_ReadCharValue(u_int16 attHandle, u_int8 isLongAtt, u_int16 offset);

int GATT_Tester_ReadCharValueByUUID(u_int8 *uuid_l, u_int16 uuid_s, u_int16 startHandle, u_int16 endHandle);

int GATT_Tester_ReadMultiple(u_int8* attHandle, u_int8 length);

int GATT_Tester_GetAllDescriptor(u_int16 startHandle, u_int16 endHandle);

int GATT_Tester_WriteCharValue(u_int16 attHandle, void *value, u_int16 length);

int GATT_Tester_SignedWrite(u_int16 attHandle, void *value, u_int16 length);

int GATT_Tester_WriteWithoutRsp(u_int16 attHandle, void *value, u_int16 length);

int GATT_Tester_ExchangeMTU();

int GATT_Tester_FindIncludeService();

int GATT_Tester_GetManufactureName();

#ifdef __cplusplus
}
#endif

#endif