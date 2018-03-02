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
 * MODULE NAME:    hcrp.h
 * DESCRIPTION:    hcrp profile platform interface
 * MAINTAINER:      Zhu Jianguo
 *
 * 
 *
 * REVISION HISTORY:
 * 
 *
 * ISSUES:
 * 
 ***********************************************************************/
#ifndef __RDABT_HCRP_H__
#define __RDABT_HCRP_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_HCRP_BUFFER_SIZE 4096
#define PJL_HEAD_LENGTH    1200


typedef struct 
{
    u_int16 cid;
    u_int16 result;
    t_bdaddr bdaddr;
}hcrp_connect_cnf_msg_t;

typedef struct 
{
    u_int16 result;
    u_int32 credit_num;
}hcrp_credit_request_rsp_t;

int Hcrp_Start_Signal_Connect(t_bdaddr address);
u_int8 Hcrp_Start_Data_Connect(void);
int Hcrp_Disconnect_Signal_Channel(void);
void Hcrp_Disconnect_Data_Channel(void);

APIDECL1 t_api APIDECL2  Hcrp_CR_DataChannelCreditGrant(void);
APIDECL1 t_api APIDECL2  Hcrp_CR_DataChannelCreditRequest(void);
APIDECL1 t_api APIDECL2  Hcrp_CR_GetLPTStatus(void);


int Hcrp_Send_Data(u_int32 length, u_int8* p_data);


#ifdef __cplusplus
}
#endif

#endif //__RDABT_HCRP_H__

