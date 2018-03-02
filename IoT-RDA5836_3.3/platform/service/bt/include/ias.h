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
#ifndef _GATT_IAS_H_
#define _GATT_IAS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define IAS_ATT_MTU		23 

typedef struct 
{
    u_int8 result;
}ias_connect_cnf_t;

//APIDECL1 t_api APIDECL2 ias_connect(t_bdaddr address);

t_api rdabt_gatt_ias_process_msg(u_int16 msgId, void *data);
void ias_init();

#ifdef __cplusplus
}
#endif

#endif