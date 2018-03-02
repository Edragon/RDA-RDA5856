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

#include "tgt_app_cfg.h"
#include "ap_gui.h"
#include "ap_pbap.h"

#if APP_SUPPORT_PBAP==1

Rdabt_pbap_msg pbap_msg = {0};

static u_int32 pbap_start = 0;
static u_int32 pbap_end = 0;
static u_int32 pbap_size = 0;

void app_pbap_deal_history_data(u_int8 *data)
{
    //int result;
    int tmp_index = 0;
    //int unicode_len;
    int ret = 0;
    int call_type = 0;
    int i = 0;
    int len = 0;

    char divide[2] = {0x0d, 0x0a};
    char name_tmp[PB_LENGTH*2] = {0};
    char tel_tmp[PB_LENGTH] = {0};
    char type_tmp[PB_LENGTH] = {0};
    char time_tmp[PB_LENGTH] = {0};
    char name_local[PB_LENGTH*2] = {0};
    int p_index = 0;

    while(!(data[i] == 'B' && data[i+1] == 'E'
        && data[i+2] == 'G' && data[i+3] == 'I'
        && data[i+4] == 'N'))
    {
        i++;
    }
    
    data += i;
    i = 0;
    len = strlen(data);

    while(p_index < len)
    {
        ret = find_str(&data[p_index], "BEGIN", 5);
        if(ret < 0)
            break ;
        
        p_index += ret;
        p_index += 5;
        //查找姓名
        ret = find_str(&data[p_index], "FN", 2);
        if(ret < 0)
            break ;
        
        p_index += ret;
        p_index += 2;
        ret = find_str(&data[p_index], ":", 1);
        if(ret < 0)
            break ;
        
        p_index += ret;
        p_index++;
        tmp_index = p_index;
        ret = find_str(&data[p_index], divide, 2);
        if(ret < 0)
            break ;
            
        p_index += ret;
        if((p_index - tmp_index) > sizeof(name_local))
            strncpy(name_local, &data[tmp_index], sizeof(name_local));
        else
            strncpy(name_local, &data[tmp_index], p_index - tmp_index);
        
        p_index += 2;
        //查找电话号码
        ret = find_str(&data[p_index], "TEL", 3);
        if(ret < 0)
            break ;
        
        p_index += ret;
        p_index += 3;
        ret = find_str(&data[p_index], ":", 1);
        if(ret < 0)
            break ;
        
        p_index += ret;
        p_index ++;
        tmp_index = p_index;
        ret = find_str(&data[p_index], divide, 2);
        if(ret < 0)
            break ;
        
        p_index += ret;
        strncpy(tel_tmp, &data[tmp_index], p_index - tmp_index);
        p_index += 2;
        //查找通话类型
        ret = find_str(&data[p_index], "X-IRMC", 6);
        if(ret >= 0)
        {
            p_index += ret;
            p_index += 6;
            ret = find_str(&data[p_index], ";", 1);
            if(ret < 0)
                ret = 0;
            
            p_index += ret;
            p_index ++;
            tmp_index = p_index;
            ret = find_str(&data[p_index], ":", 1);
            if(ret < 0)
                ret = 0;
            
            p_index += ret;
            if((p_index - tmp_index) > sizeof(type_tmp))
                strncpy(type_tmp, &data[tmp_index], sizeof(type_tmp));
            else
                strncpy(type_tmp, &data[tmp_index], p_index - tmp_index);
        }
        else
        {
            strcpy(type_tmp, "UNKNOW");
        }
        
        //判断通话类型
        if(!strcmp("MISSED", type_tmp))
        {
            call_type = CALL_MISSED;
        }
        else if((!strcmp("RECEIVED", type_tmp)) || (!strcmp("INCOMING", type_tmp)))
        {
            call_type = CALL_RECEIVED;
        }
        else if((!strcmp("DIALED", type_tmp)) || (!strcmp("OUTGOING", type_tmp)))
        {
            call_type = CALL_DIALED; 
        }
        else if(!strcmp("UNKNOW", type_tmp))
        {
            call_type = CALL_UNKNOW;
        }
        p_index++;

        //查找时间
        ret = find_str(&data[p_index], divide, 2);
        if(ret < 0)//时间不属于必需信息，没找到可以继续分析其他字段
            ;
        else
        {
            if(ret > sizeof(time_tmp))
                strncpy(time_tmp, &data[p_index], sizeof(time_tmp));
            else
                strncpy(time_tmp, &data[p_index], ret);
            p_index += ret;
            p_index += 2;
        }
        
        //名字存在name_tmp数组
        //通话时间点存在time_tmp数组
        //电话号码存在tel_tmp数组
        //通话类型存在call_type
        //可以在此处使用串口发送出去

        //使用完毕清空
        memset(name_tmp, 0, PB_LENGTH*2);
        memset(name_local, 0, PB_LENGTH*2);
        memset(time_tmp, 0, PB_LENGTH);
        memset(tel_tmp, 0, PB_LENGTH);
        memset(type_tmp, 0, PB_LENGTH);
        call_type = 0;
    }
}

void app_pbap_deal_pb_data(u_int8 *data)
{
    int i = 0;
    int ret = 0;
    int pos = 0;
    int tmp_pos = 0;
    int len;
    int name_next_pos = 0;
    char name_tmp[PB_LENGTH*2] = {0};
    char tel_tmp[4][PB_LENGTH] = {{0}};
    //char type_tmp[PB_LENGTH] = {0};
    int end_pos = 0;
    char divide[2] = {0x0d, 0x0a};

    while(!(data[i] == 'B' && data[i+1] == 'E'
            && data[i+2] == 'G' && data[i+3] == 'I'
            && data[i+4] == 'N'))
    {
        i++;
    }
    
    data += i;
    i = 0;
    len = strlen(data);
    while(pos < len)
    {
        ret = find_str(data+pos, "END", 3);
        if(ret < 0)
        {
            return ;
        }
        
        end_pos = pos + ret;
        ret = find_str(data+pos, "FN", 2);
        if(ret < 0)
            return ;
        
        pos += ret;
        pos += 2;
        ret = find_str(data+pos, ":", 1);
        if(ret < 0)
            return ;
        
        pos += ret;
        pos += 1;
        tmp_pos = pos;
        ret = find_str(data+pos, &divide[0], 1);
        if(ret < 0)
            break ;
        
        pos += ret;
        if(pos >= end_pos)
        {
            continue;
        }
        
        if((pos-tmp_pos) > sizeof(name_tmp))
            strncpy(name_tmp, data+tmp_pos, sizeof(name_tmp));
        else
            strncpy(name_tmp, data+tmp_pos, pos-tmp_pos);
        
        pos += 1;
        name_next_pos = find_str(data+pos, "FN", 2);
        if(name_next_pos < 0)//没有下一条名字
        {
            ret = find_str(data+pos, "TEL", 3);
            if(ret >= 0)
            {
                pos += ret;
                pos += 3;
            }
            
            ret = find_str(data+pos, ":", 1);
            if(ret < 0)
            {
                memset(name_tmp, 0, sizeof(name_tmp));
                memset(tel_tmp[0], 0, PB_LENGTH);
                memset(tel_tmp[1], 0, PB_LENGTH);
                memset(tel_tmp[2], 0, PB_LENGTH);
                memset(tel_tmp[3], 0, PB_LENGTH);
                break;	
            }

            pos += ret;
            pos += 1;
            tmp_pos = pos;
            ret = find_str(data+pos, &divide[0], 1);
            if(ret < 0)
            {
                memset(name_tmp, 0, sizeof(name_tmp));
                memset(tel_tmp[0], 0, PB_LENGTH);
                memset(tel_tmp[1], 0, PB_LENGTH);
                memset(tel_tmp[2], 0, PB_LENGTH);
                memset(tel_tmp[3], 0, PB_LENGTH);
                break;	                        
            }

            pos += ret;
            if(pos >= end_pos)
            {
                continue;
            }
            
            if((pos-tmp_pos) > PB_LENGTH)
                strncpy(tel_tmp[0], data+tmp_pos, PB_LENGTH);
            else
                strncpy(tel_tmp[0], data+tmp_pos, pos-tmp_pos);
            
            //姓名保存在name_tmp
            //电话号码保存在tel_tmp，同一个姓名下最多保存4个电话号码
            //可以在此处使用串口发送出去

            //使用完清空
            memset(name_tmp, 0, sizeof(name_tmp));
            memset(tel_tmp[0], 0, PB_LENGTH);
            memset(tel_tmp[1], 0, PB_LENGTH);
            memset(tel_tmp[2], 0, PB_LENGTH);
            memset(tel_tmp[3], 0, PB_LENGTH);
            return ;
        }
        
        name_next_pos += pos;
        ret = find_str(data+pos, "TEL", 3);
        if(ret >= 0)
        {
            pos += ret;
            pos += 3;
        }
        
        while(ret >= 0 && i < 3)
        {
            ret = find_str(data+pos, ":", 1);
            if(ret < 0)
                break;	
            
            pos += ret;
            pos += 1;
            tmp_pos = pos;
            ret = find_str(data+pos, &divide[0], 1);
            if(ret < 0)
                break;
            
            pos += ret;
            if(pos >= end_pos)
            {
                pos = end_pos;
                pos += 3;
                break;
            }
            
            strncpy(tel_tmp[i], data+tmp_pos, pos-tmp_pos);
            pos += 1;
            i++;
            if(i < 3)
            {
                ret = find_str(data+pos, "TEL", 3);
                if(ret >= 0)
                {
                    pos += ret;
                    pos += 3;
                }
            }
        }
        
        if(i == 3)
        {
            ret = find_str(data+pos, "END", 3);
            if(ret < 0)
            {
                return ;
            }
            pos += ret;
            pos += 3;
        }
        i = 0;

        //姓名保存在name_tmp
        //电话号码保存在tel_tmp，同一个姓名下最多保存4个电话号码
        //可以在此处使用串口发送出去

        //使用完清空
        memset(name_tmp, 0, sizeof(name_tmp));
        memset(tel_tmp[0], 0, PB_LENGTH);
        memset(tel_tmp[1], 0, PB_LENGTH);
        memset(tel_tmp[2], 0, PB_LENGTH);
        memset(tel_tmp[3], 0, PB_LENGTH);
    } 
}

void app_get_pbap_size(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    pbap_msg.pb_addr = *pAddr;  
    pbap_msg.pb_type = PBAP_PHONEBOOK_TYPE;
    pbap_start = 0;
    pbap_end = 0;
    pbap_size = 0;
    event.nEventId = RDABT_PBAP_GET_SIZE_REQ;
    event.nParam1 = (UINT32)&pbap_msg;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_get_call_recends_size(t_bdaddr *pAddr)
{
    COS_EVENT event = {0};
    pbap_msg.pb_addr = *pAddr;  
    pbap_msg.pb_type = PBAP_HISTORY_TYPE;
    pbap_start = 0;
    pbap_end = 0;
    pbap_size = 0;
    event.nEventId = RDABT_PBAP_GET_SIZE_REQ;
    event.nParam1 = (UINT32)&pbap_msg;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_get_pbap_data(u_int8 type, u_int32 start, u_int32 end)
{
    COS_EVENT ev = {0};
    pbap_msg.pb_type = type;
    pbap_msg.pb_start = start;
    pbap_msg.pb_end = end;
    ev.nEventId = RDABT_PBAP_DOWNLOAD_REQ;
    ev.nParam1 = (UINT32)&pbap_msg;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_get_call_recends_data(u_int32 size)
{
    COS_EVENT event = {0};
    event.nEventId = RDABT_CALL_RECENDS_DOWNLOAD_REQ;
    event.nParam1 = size;
    COS_SendEvent(MOD_BT,  &event , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_pbap_disconnect(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = RDABT_PBAP_DISCONNECT_REQ;
    COS_SendEvent(MOD_BT,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

void app_pbap_msg_handle(COS_EVENT *ev)
{
    //COS_EVENT event = {0};
    switch(ev->nEventId)
    {	
        case RDABT_PBAP_GET_SIZE_IND:
        {
            pbap_size = ev->nParam1;
            hal_HstSendEvent(SYS_EVENT, 0x11211100);
            hal_HstSendEvent(SYS_EVENT, pbap_size);
            if(pbap_size > 0)
            {
                pbap_start = 1;
                pbap_end = 2;

                app_get_pbap_data(pbap_msg.pb_type, pbap_start, pbap_end);
            }
        }
        break;

        case RDABT_PBAP_GET_DATA_IND:
        {
            hal_HstSendEvent(SYS_EVENT, 0x11211358);
            hal_HstSendEvent(SYS_EVENT, pbap_start);
            hal_HstSendEvent(SYS_EVENT, pbap_end);
            hal_HstSendEvent(SYS_EVENT, pbap_size);
            if(pbap_msg.pb_type == PBAP_PHONEBOOK_TYPE)        
                app_pbap_deal_pb_data((u_int8 *)ev->nParam1);
            else
                app_pbap_deal_history_data((u_int8 *)ev->nParam1);

            if(pbap_end >= pbap_size)
            {
                app_pbap_disconnect();
            }
            else
            {
                pbap_start++;
                pbap_end++;
                app_get_pbap_data(pbap_msg.pb_type, pbap_start, pbap_end);
            }
        }
        break;	

        case RDABT_PBAP_GET_DATA_SERVER_ERROR_IND:
        {                    
            hal_HstSendEvent(SYS_EVENT, 0x11211359);
            if(ev->nParam1==0xd0)//peer device server error
            {                    
                hal_HstSendEvent(SYS_EVENT, 0x1121135a);                        
                hal_HstSendEvent(SYS_EVENT, pbap_start);                        
                hal_HstSendEvent(SYS_EVENT, 0x1121135b);
                pbap_start++;
                pbap_end++;
                app_get_pbap_data(pbap_msg.pb_type, pbap_start, pbap_end);
            }
            else                        
                app_pbap_disconnect();
        }
        break;
    }
}

#endif
