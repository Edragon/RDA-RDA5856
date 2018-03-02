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
#include "base_prv.h"
#include "ap_common.h"
#include "ap_message.h"
#include "GlobalConstants.h"
#include "ts.h"
#include "cos.h"


TASK_ENTRY BAL_MMCTask(uint32 data)
{
    BOOL bRet;
	COS_EVENT ev_play ={0} ;

    for(;;)
    {		
		COS_WaitEvent(MOD_MED, &ev_play, COS_WAIT_FOREVER);
	}
}

INT32 MMC_Mpeg2tsDemux(UINT8 * buf, UINT8 * end, UINT8 * outbuf,UINT32 *outlen,INT32 flags,INT32 demux_pid)
{
    return 0;
}
