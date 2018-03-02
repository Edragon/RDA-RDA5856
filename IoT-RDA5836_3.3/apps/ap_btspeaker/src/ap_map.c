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

#include "ap_map.h"
#if APP_SUPPORT_MAP==1

Rdabt_map_msg map_msg = {0};

static u_int32 map_start = 0;
static u_int32 map_end = 0;
static u_int32 map_size = 0;

void app_get_map_size(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};

    map_msg.map_addr = *pAddr;
    event.nEventId = RDABT_MAP_GET_SIZE_REQ;
    event.nParam1 = &map_msg;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_get_map_data(u_int32 start, u_int32 end)
{
    COS_EVENT ev = {0};

    map_msg.map_start = start;
    map_msg.map_end = end;
    ev.nEventId = RDABT_MAP_DOWNLOAD_REQ;
    ev.nParam1 = &map_msg;

    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_set_mns(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};

    map_msg.map_addr = *pAddr;
    event.nEventId = RDABT_MAP_SET_MNS_REQ;
    event.nParam1 = &map_msg;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_get_msg_data(int msg_handle)
{
    COS_EVENT ev = {0};

    ev.nEventId = RDABT_MAP_GET_DATA_REQ;
    ev.nParam1 = msg_handle;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_mas_disconnect(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_MAS_DISCONNECT_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_mns_disconnect(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_MNS_DISCONNECT_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

int my_atoi(char * str)
{
    int value = 0;
    int sign = 1;
    int radix;

    if(*str == '-')
    {
        sign = -1;
        str++;
    }
    
    if(*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
    {
        radix = 16;
        str += 2;
    }
    else if(*str == '0')      // 八进制首字符为0
    {
        radix = 8;
        str++;
    }
    else
        radix = 10;
    
    while(*str)
    {
        if(radix == 16)
        {
            if(*str >= '0' && *str <= '9')
                value = value * radix + *str - '0';
            else
                value = value * radix + (*str | 0x20) - 'a' + 10;
        }
        else
            value = value * radix + *str - '0';
        
        str++;
    }
    
    return sign*value;
}

int app_map_deal_msglist_data(u_int8 *msg_data)
{
    int ret = 0;
    int tmp_pos = 0;
    int p_index = 0;
    int msg_handle = -1;
    char *msg_head = msg_data;
    char tmp[32] = {0};
    char time_tmp[MAP_LENGTH] = {0};
    char name_tmp[MAP_LENGTH * 2] = {0};
    char name_local[MAP_LENGTH * 2] = {0};
    char num_tmp[MAP_LENGTH] = {0};
    char msg_local[MSG_BUFF_LEN] = {0};
    u_int16 result_length;
    char *result_data;

    while(*msg_head != 0x49)
    {
        msg_head++;
        p_index++;
        result_data = &msg_data[p_index];
        if(p_index >= RDABT_MSG_BUF_SIZE)
            return -1;
    }
    
    result_length = strlen(&result_data);
    p_index = 0;
    
    while(p_index < result_length)
    {
        //查找subject
        ret = find_str(&result_data[p_index], "subject", 7);
        if(ret > 0)
        {
            p_index += ret;
            p_index += 7;
            
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret > 0)
            {
                p_index += ret;
                p_index += 1;
                tmp_pos = p_index;
                
                ret = find_str(&result_data[p_index], "\"", 1);
                if(ret > 0)
                {
                    p_index += ret;
                    if((p_index - tmp_pos) > MSG_BUFF_LEN)
                        strncpy(msg_local, &result_data[tmp_pos], MSG_BUFF_LEN);
                    else
                        strncpy(msg_local, &result_data[tmp_pos], p_index - tmp_pos);        
                    msg_handle = 0;
                }
            }

        }
        else
        {
            //查找message handle
            ret = find_str(&result_data[p_index], "handle", 6);
            if(ret < 0)
            {
                break;
            }
            p_index += ret;
            p_index += 6;
            
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret < 0)
            {
                break;
            }
            
            p_index += ret;
            p_index += 1;
            tmp_pos = p_index;
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret < 0)
            {
                break;
            }
            
            p_index += ret;
            tmp[0] = '0';
            tmp[1] = 'x';
            if((p_index - tmp_pos) > 30)
                strncpy(&tmp[2], &result_data[tmp_pos], 30);
            else
                strncpy(&tmp[2], &result_data[tmp_pos], p_index - tmp_pos);
            p_index += 1;
            tmp[31] = 0;
            msg_handle = my_atoi(tmp);
            memset(tmp, 0, sizeof(tmp));
        }
        
        //查找短信时间
        ret = find_str(&result_data[p_index], "datetime", 8);
        if(ret >= 0)//时间查不到没关系
        {
            p_index += ret;
            p_index += 8;
            
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret >= 0)
            {
                p_index += ret;
                p_index += 1;
                tmp_pos = p_index;
                
                ret = find_str(&result_data[p_index], "\"", 1);
                if(ret >= 0)
                {
                    p_index += ret;
                    if((p_index - tmp_pos) > MAP_LENGTH)
                    {
                        strncpy(time_tmp, &result_data[tmp_pos], MAP_LENGTH);
                    }
                    else
                    {
                        strncpy(time_tmp, &result_data[tmp_pos], p_index - tmp_pos);
                    }
                    p_index += 1;
                }
            }
        }

        //查找发送者姓名
        ret = find_str(&result_data[p_index], "sender_name", 11);
        if(ret >= 0)
        {
            p_index += ret;
            p_index += 11;
            
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret >= 0)
            {
                p_index += ret;
                p_index += 1;
                tmp_pos = p_index;
                
                ret = find_str(&result_data[p_index], "\"", 1);
                if(ret >= 0)
                {
                    p_index += ret;
                    strncpy(name_tmp, &result_data[tmp_pos], p_index - tmp_pos);
                    p_index += 1;
                }
            }
        }
        
        //查找发送者号码
        ret = find_str(&result_data[p_index], "sender_addressing", 17);
        if(ret >= 0)
        {
            p_index += ret;
            p_index += 17;
            ret = find_str(&result_data[p_index], "\"", 1);
            if(ret >= 0)
            {
                p_index += ret;
                p_index += 1;
                tmp_pos = p_index;
                
                ret = find_str(&result_data[p_index], "\"", 1);
                if(ret >= 0)
                {
                    p_index += ret;
                    if((p_index - tmp_pos) > MAP_LENGTH)
                    {
                        strncpy(num_tmp, &result_data[tmp_pos], MAP_LENGTH);
                    }
                    else
                    {
                        strncpy(num_tmp, &result_data[tmp_pos], p_index - tmp_pos);
                    }
                    p_index += 1;
                }
            }
        }

        //发送者姓名存在name_local数组
        //发送者号码存在num_tmp数组
        //接收时间存在time_tmp数组
        return msg_handle;
    }
    
    return -1;
}

int app_map_deal_msg_data(u_int8 *result_data)
{
    int tmp_index = 0;
    int unicode_len;
    int ret = 0;
    int result = -1;
    char msg[MSG_BUFF_LEN] = {0};
    char msg_local[MSG_BUFF_LEN] = {0};
    char divide[2] = {0x0d, 0x0a};
    int p_index = 0;
    u_int16 result_length = strlen(result_data);

    while(p_index < result_length)
    {
        //查找信息内容
        ret = find_str(&result_data[p_index], "BEGIN:MSG", 9);
        if(ret < 0)
        {
            break;
        }
        
        p_index += ret;
        p_index += 9;
        ret = find_str(&result_data[p_index], divide, 2);
        if(ret < 0)
        {
            break;
        }
        
        p_index += ret;
        p_index += 2;
        tmp_index = p_index;
        ret = find_str(&result_data[p_index], divide, 2);
        if(ret < 0)
        {
            break;
        }
        
        p_index += ret;
        if(p_index - tmp_index > MSG_BUFF_LEN)
        {
            strncpy(msg, &result_data[tmp_index], MSG_BUFF_LEN);
        }
        else
        {
            strncpy(msg, &result_data[tmp_index], p_index - tmp_index);
        }
        p_index += 2;
        //短信保存在msg_local数组内
        result = 0;
    }
    
    return result;
}

int app_map_deal_mns_data(u_int8 *result_data)
{
        
}

void app_map_msg_handle(COS_EVENT *ev)
{
    COS_EVENT event = {0};
    int msg_handle = 0;

    switch(ev->nEventId)
    {
        case RDABT_MAP_GET_SIZE_IND:
        {
            map_size = ev->nParam1;
            if(map_size > 0)
            {
                map_start = 1;
                map_end = 2;
                app_get_map_data(map_start, map_end);
            }
        }
        break;
        
        case RDABT_MAP_GET_DATA_IND:
        {       
            msg_handle = app_map_deal_msglist_data((u_int8 *)ev->nParam1);
            if(-1 != msg_handle)
            {
                //app_map_deal_msg_data((u_int8 *)ev->nParam1);
                if(map_end >= map_size)
                {
                    app_mas_disconnect();
                }
                else
                {
                    map_start++;
                    map_end++;
                    app_get_map_data(map_start, map_end);
                }
                //app_get_msg_data(msg_handle);
            }
            else
            {

            }
        }
        break;	
        
        case RDABT_MNS_NOTIFY_IND:
        {
            app_map_deal_mns_data((u_int8 *)ev->nParam1);
        }
        break;        
    }
}

#endif

