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
#if APP_SUPPORT_LE==1
#include "ap_gui.h"

#include "bt_types.h"
#include "bt_msg.h"
#include "bt_config.h"
#include "bt_ctrl.h"

#include "ap_mgr.h"
#include "ap_ble.h"
#include "ap_message.h"
#include "mci.h"

#include "gatt.h"
#include "ias.h"

#define ATT_UUID_IAS	0x1802

#define ATT_UUID_ALERT_LEVEL	0x2A06
const char ias_dn[] = "IAS";

const u_int8 ias_ias_service_uuid[2] = {ATT_UUID_IAS&0xff, ATT_UUID_IAS>>8};
gatt_chara_def_short_t ias_al_char = {{ATT_CHARA_PROP_WWP, 0, 0, ATT_UUID_ALERT_LEVEL&0xff, ATT_UUID_ALERT_LEVEL>>8}};
char alert_level[10];

//Immediate Alert Service
const gatt_element_t ias_ias[] =
{
    {2, ATT_PM_READABLE, {ATT_UUID_PRIMARY}, ATT_FMT_SHORT_UUID|ATT_FMT_GROUPED|ATT_FMT_FIXED_LENGTH, (void*)ias_ias_service_uuid, NULL},

    {sizeof(ias_al_char), ATT_PM_READABLE, {ATT_UUID_CHAR}, ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)&ias_al_char, NULL},
    {sizeof(alert_level), ATT_PM_WRITEABLE, {ATT_UUID_ALERT_LEVEL}, ATT_FMT_WRITE_NOTIFY|ATT_FMT_SHORT_UUID|ATT_FMT_FIXED_LENGTH, (void*)alert_level, NULL},
};

void app_bt_start_ias_server_req(void)
{
    hal_HstSendEvent(SYS_EVENT, 0x20150918);

    app_bt_set_adv_data("ias", NULL, NULL);

    app_bt_add_service_req((void*)ias_ias, sizeof(ias_ias)/sizeof(gatt_element_t));

    app_bt_set_visible_req(NULL);
}

t_api rdabt_gatt_ias_process_msg(u_int16 msgId, void *data)
{
    switch(msgId)
    {
        case GATT_CONNECT_CNF:
        {
            gatt_connect_cnf_t *pMsg = (gatt_connect_cnf_t*)data;
            ias_connect_cnf_t msg;
            msg.result = pMsg->result;
            RDABT_Send_Message(GATT_FMP_CONNECT_CNF, RDABT_GATT_FMP, RDABT_ADP, sizeof(msg), &msg);
        }
        break;

        case GATT_DISCONNECT_IND:
        {
        }
        break;

        default:
        break;
    }
    return 0;
}
#endif
