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
////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                    //
///                                                                                                                                  //
///     @file hal_ir.c                                                                                                           //
///                                                                                                                                  //
///     This file contains RDA's IR driver implementation                                                       //
///                                                                                                                                  //
///                                                                                                                                  //
////////////////////////////////////////////////////////////////////////////////
#include "chip_id.h"

#if defined(__IRC_SUPPORT__)

#include "cs_types.h"
#include "global_macros.h"
#include "sys_irq.h"
#include "ir.h"
#include "cos.h"
#include "event.h"
#include "sys_ctrl.h"
#include "hal_host.h"

//#include "halp_ir.h"
extern UINT32 irc_press_mode;

void IR_Init()
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
     hwp_sysCtrl->Clk_Sys_Mode |= SYS_CTRL_MODE_SYSD_RF_SPI_MANUAL;
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    hwp_sysCtrl->per_module_en |= SYS_CTRL_IR_EN;
#endif
}

PROTECTED VOID hal_IRhandle(UINT8 interruptId)
{
    COS_EVENT ev; 
    static UINT repeat_count  = 0;    

    ev.nEventId  = 0;
    ev.nParam1  = 0;

    ev.nParam1  = hwp_ir->ir_code; 

    ev.nEventId  = EV_IRC_PRESS;     
    repeat_count++; 

    if(10 == repeat_count  || 8 != (hwp_ir->ir_fsm & 0xf))
    	{
    	       hal_HstSendEvent(SYS_EVENT,0x90049004 );  
              repeat_count = 0;
		COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
              hal_HstSendEvent(SYS_EVENT,ev.nParam1 );       
	}
    
    
	
}
#endif
