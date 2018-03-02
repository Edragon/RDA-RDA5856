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


#include "chip_id.h"
#include "platform.h"
#include "event.h"
#include "ap_common.h"
#include "ap_message.h"
#include "GlobalConstants.h"
#include "ts.h"
#include "cos.h"

BOOL bal_is_first_keydown()
{
    return TRUE;
}

TASK_ENTRY BAL_SysTask (void *pData)
{
	COS_EVENT ev;

	for (;;)
	{
		COS_WaitEvent(MOD_SYS, &ev, COS_WAIT_FOREVER);

		switch (ev.nEventId)
		{
#ifdef UART2_SUPPORT
			case EV_UART2_DATA:
			hal_HstSendEvent(SYS_EVENT, 0x00275854);
			uart2_deal_data(ev.nParam1);
			break;
#endif
#if defined(__AT_SUPPORT__)
			case EV_DM_UART_RECEIVE_DATA_IND:
			SA_UartRecevDataInd(&ev);
			break;
#endif /* __AT_SUPPORT__ */
		}
	}
}
