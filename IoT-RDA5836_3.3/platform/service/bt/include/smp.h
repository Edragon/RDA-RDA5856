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
#ifndef BLUETOOTH_LOWENERGY_SMP_API_DECLARED
#define BLUETOOTH_LOWENERGY_SMP_API_DECLARED

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_smp_ind_msg
{
	u_int8 is_disp;
	u_int32 value;
	void *device;
}smp_ind_msg_t;

enum 
{
	LE_STATIC_ADDR = 0,
	LE_NON_RESOLVABLE_ADDR = 1,
	LE_RESOVABLE_ADDR = 2
};

APIDECL1 t_api APIDECL2 SMP_PINResponse(u_int8* pincode,t_bdaddr address);
APIDECL1 t_api APIDECL2 SMP_StartPair(t_bdaddr address);
APIDECL1 t_api APIDECL2 SMP_SecRequest(u_int8 auth_req);
APIDECL1 t_api APIDECL2 SMP_DataSign(u_int16 handle,u_int8 *input,u_int8 *output,u_int8 len);

APIDECL1 t_api APIDECL2 SMP_GenRandomAddress(t_bdaddr address, t_bdaddr *pRanddomAddress, u_int8 type);

//input buffer should contain PDU data, signature
APIDECL1 t_api APIDECL2 SMP_SignatureCheck(u_int16 handle,u_int8 *input,u_int8 len);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif