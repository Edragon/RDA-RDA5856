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
#include "cswtype.h"
#include "string.h"
#include "cos.h"
#include "dbg.h"
#include "event.h"
#include "bt.h"
#include "rdabt_sharemem.h"
#include "hal_host.h"
#include "hal_bt.h"
#include "hal_timers.h"

#if CHIP_HAS_BTCPU==1
#ifndef WIN32
#include "hal_sys.h"
#include "hal_mem_map.h"
#else
#define VOLATILE volatile
#define HAL_UNCACHED_BSS 
#define hal_BtSetHostIrqHandler(x)
#define rdabt_send_notify_flash()
#define HCIT_ShareMem_Initialise()
#define SYSinit_Gpio_Write(x, y)
#endif

#define RDABT_SHAREMEM_SIZE     32

#ifndef BT_SHAREMEM_TRANSPORT

typedef struct {
	u_int8 xcpu_busy, btcpu_busy;
    u_int8 bt_head, bt_tail;
    u_int8 xcpu_head, xcpu_tail;
    COS_EVENT bt_event[RDABT_SHAREMEM_SIZE];
    COS_EVENT xcpu_event[RDABT_SHAREMEM_SIZE];
} rdabt_sharemem_t;

VOLATILE rdabt_sharemem_t HAL_SHARED_BSS g_rdabt_sharemem = {0};
extern u_int8 rdabt_send_notify_pending;
extern void HWdelay_Wait_For_ms(UINT32 units_of_m_secs, UINT32 use_native_clk);
void SYS_SendEvent2(u_int32 event);
void SYSinit_Gpio_Write(u_int8 index, u_int8 value);


void rdabt_Sharemem_Interrupt_Handle(void)
{
    COS_EVENT ev = {0};
   //rdabt_send_notify_flash();
    //hal_HstSendEvent(SYS_EVENT, 0x09170100);
    if(rdabt_send_notify_pending < 5)
    {
        rdabt_send_notify_pending++;
        ev.nEventId = EV_BT_NOTIFY_IND;
        COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
}

void rdabt_sharemem_init() // call by xcpu
{
    memset((void*)&g_rdabt_sharemem, 0, sizeof(g_rdabt_sharemem));
    //HCIT_ShareMem_Initialise(); // need it?
    hal_BtSetHostIrqHandler(rdabt_Sharemem_Interrupt_Handle);
}

void rdabt_send_msg_to_bt(COS_EVENT *event)
{
    int count = 0;
    while(g_rdabt_sharemem.bt_event[g_rdabt_sharemem.bt_head].nEventId != 0
    || ((g_rdabt_sharemem.bt_head + 1) % RDABT_SHAREMEM_SIZE) == g_rdabt_sharemem.bt_tail)
    {
        //hal_DbgAssert("BT EVENT full! head=%d", g_rdabt_sharemem.bt_head);
        hal_BtNotifyDataSent();
        hal_TimDelay(100);
        if(event->nEventId==RDABT_AVRCP_SEND_KEY_REQ || event->nEventId==RDABT_HID_TXDATA_REQ)
        {
            if(count ++ > 20)
            {
                hal_HstSendEvent(SYS_EVENT, 0x1a500000);
                hal_HstSendEvent(SYS_EVENT, event->nEventId);
                return;
            }
        }
        else
        {
            if(count ++ > 100)
            {
                hal_HstSendEvent(SYS_EVENT, 0x1a510000);
                hal_HstSendEvent(SYS_EVENT, event->nEventId);
                return;
            }
        }
    }
    g_rdabt_sharemem.bt_event[g_rdabt_sharemem.bt_head++] = *event;
    hal_BtNotifyDataSent();
    
    if(g_rdabt_sharemem.bt_head == RDABT_SHAREMEM_SIZE)
        g_rdabt_sharemem.bt_head = 0;
}

void rdabt_send_msg_to_xcpu(COS_EVENT *event)
{
    int count = 0;
    while(g_rdabt_sharemem.xcpu_event[g_rdabt_sharemem.xcpu_head].nEventId != 0)
    {
        //SYS_ASSERT2();//"XCPU EVENT full! head=%d", g_rdabt_sharemem.xcpu_head
        SYSinit_Gpio_Write(2,1); // xcpu interrupt
        HWdelay_Wait_For_ms(10, TRUE);
        
        if(count ++ > 20)
        {
            SYS_SendEvent2(0x1a520000);
            SYS_SendEvent2(event->nEventId);
            return;
        }
    }
    g_rdabt_sharemem.xcpu_event[g_rdabt_sharemem.xcpu_head++] = *event;
    SYSinit_Gpio_Write(2,1); // xcpu interrupt

    if(g_rdabt_sharemem.xcpu_head == RDABT_SHAREMEM_SIZE)
        g_rdabt_sharemem.xcpu_head = 0;
}


BOOL rdabt_get_bt_msg(COS_EVENT *event)
{
    if(g_rdabt_sharemem.bt_head==g_rdabt_sharemem.bt_tail)
        return FALSE;
    else
        {
        *event = g_rdabt_sharemem.bt_event[g_rdabt_sharemem.bt_tail];
        g_rdabt_sharemem.bt_event[g_rdabt_sharemem.bt_tail++].nEventId = 0;
        }
    if(g_rdabt_sharemem.bt_tail == RDABT_SHAREMEM_SIZE)
        g_rdabt_sharemem.bt_tail = 0;
    return TRUE;
}

BOOL rdabt_get_xcpu_msg(COS_EVENT *event)
{
    if(g_rdabt_sharemem.xcpu_head==g_rdabt_sharemem.xcpu_tail)
        return FALSE;
    else
        {
        *event = g_rdabt_sharemem.xcpu_event[g_rdabt_sharemem.xcpu_tail];
        g_rdabt_sharemem.xcpu_event[g_rdabt_sharemem.xcpu_tail++].nEventId = 0;
        }
    if(g_rdabt_sharemem.xcpu_tail == RDABT_SHAREMEM_SIZE)
        g_rdabt_sharemem.xcpu_tail = 0;
    return TRUE;
}

// wati xcpu before btcpu send event
void rdabt_wait_xcpu_busy()
{
	g_rdabt_sharemem.btcpu_busy = 1;
	while(g_rdabt_sharemem.xcpu_busy);
}

// clear btcpu status
void rdabt_clear_btcpu_busy()
{
	g_rdabt_sharemem.btcpu_busy = 0;
}

// wait btcpu before xcpu sent event
BOOL rdabt_wait_btcpu_busy()
{
	UINT32 startTime;
    BOOL   timeOut   = FALSE;
	
	startTime = hal_TimGetUpTime();
    //  wait for host sema to be free
    //  or time out.
	while(g_rdabt_sharemem.btcpu_busy)
	{
		if ((hal_TimGetUpTime() - startTime) >= HAL_HST_SEND_EVENT_DELAY)
        {
            timeOut = TRUE;
			break;
        }
	}
	
	g_rdabt_sharemem.xcpu_busy = 1;
	return timeOut;
}

// clear xcpu status
void rdabt_clear_xcpu_busy()
{
	g_rdabt_sharemem.xcpu_busy = 0;
}
#else


void rdabt_Sharemem_Interrupt_Handle(void)
{
}

void rdabt_sharemem_init() // call by xcpu
{
 
}

void rdabt_send_msg_to_bt(COS_EVENT *event)
{
}


BOOL rdabt_get_xcpu_msg(COS_EVENT *event)
{
}

// wati xcpu before btcpu send event
void rdabt_wait_xcpu_busy()
{

}

// clear btcpu status
void rdabt_clear_btcpu_busy()
{
	
}

// wait btcpu before xcpu sent event
BOOL rdabt_wait_btcpu_busy()
{
    return FALSE;
}

// clear xcpu status
void rdabt_clear_xcpu_busy()
{

}

#endif
#endif // CHIP_HAS_BTCPU==0

