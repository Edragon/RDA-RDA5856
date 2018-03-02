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
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_irq.c
/// IRQ module driver implementation
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "global_macros.h"
#include "sys_irq.h"

#include "chip_id.h"

#include "halp_irq.h"
#include "halp_os_stub.h"
#include "halp_debug.h"
#include "halp_profile.h"
#include "halp_irq_prio.h"

#include "hal_debug.h"
#include "hal_mem_map.h"
#include "register.h"
#include "halp_sys.h"


// =============================================================================
//  MACROS
// =============================================================================


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#if (CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
extern bt_irq_status SYSirq_IRQ_State_Dispatch(bt_irq_num irq_num,bt_irq_position position);
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================
#ifdef UNIT_TEST
HAL_MODULE_IRQ_HANDLER_T test_hanlder = NULL;
UINT test_irq_id = 0;
PUBLIC void hal_IrqSetDispatchHandler(UINT id, HAL_MODULE_IRQ_HANDLER_T handle)
{
    test_irq_id = id;
    test_hanlder = handle;
}
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#ifdef CT_NO_DEBUG
UINT32 g_wdtStatus;
void hal_WdtIrqHandler(void)
	
{
    UINT32 t_adpll_setting1;
    hal_HstSendEvent(BOOT_EVENT,0x77667766);
	hwp_sysCtrl->RF_IF_CTRL = 0;
	t_adpll_setting1 = hwp_rf_if->adpll_setting1;
    t_adpll_setting1 &= ~RF_IF_SYS_PLL_CLK_DIV_MASK;
    t_adpll_setting1 |= (RF_IF_SYS_PLL_CLK_DIV(10) & RF_IF_SYS_PLL_CLK_DIV_MASK);
    t_adpll_setting1 |= RF_IF_SYS_PLL_CLK_LD_CFG;
    hwp_rf_if->adpll_setting1 = t_adpll_setting1;
    hwp_rf_if->adpll_setting1 = RF_IF_SYS_CLK_SEL_FAST | RF_IF_SYS_FAST_CLK_SEL_XTAL_26M;
	hwp_sysCtrl->per_soft_reset1 &= ~(SYS_CTRL_GLOBAL_RSTB);

}
#endif
#endif
// =============================================================================
// hal_IrqDispatch
// -----------------------------------------------------------------------------
/// IRQ dispatcher. Calls the interrupt handler according to their priorities
/// defined in the relevant macros and array.
// =============================================================================
extern  unsigned int role_switching;
unsigned int assert_done = 0;
PROTECTED VOID HAL_FUNC_INTERNAL hal_IrqDispatch(VOID)
{
#ifdef UNIT_TEST
    UINT32 cause = hwp_sysIrq->cause;
    UINT32 bit = 1;
    UINT8 i;
    UINT32 unused __attribute__ ((unused));
    UINT32 mask;

    //sxr_EnterScSchedule();
    
    // gestion Mask
    // save current Mask
    mask = hwp_sysIrq->mask_set;
    // Mask all IT
    hwp_sysIrq->mask_clear = SYS_IRQ_MASK_SET_MASK;
    // Re-enable irq    
    hwp_sysIrq->sc = 1;

    for(i=0;i<SYS_IRQ_QTY;i++)
    {
        if (cause & bit)
        {
            HAL_PROFILE_IRQ_ENTER(i);
            // clear irq bit of reenabled higher prio irqs as those will be treated 
            // by an other interrupt call
            cause &= ~g_halIrqPriorityMask[i];
            // Setup priority mask for this irq
            hwp_sysIrq->mask_set = g_halIrqPriorityMask[i];

            if(i==test_irq_id)
            {
                test_hanlder(i);
            }

            // Mask all IT
            hwp_sysIrq->mask_clear =SYS_IRQ_MASK_SET_MASK;
            
            HAL_PROFILE_IRQ_EXIT(i);
        }
        bit = bit << 1;
    }

    // Disable irq
    unused = hwp_sysIrq->sc;
    // Restore previous mask
    hwp_sysIrq->mask_set = mask; 

    //sxr_ExitScSchedule();
#else
   
    UINT32 cause = hwp_sysIrq->cause;
    
    UINT32 bit = 1;
    UINT8 i = 0;
    UINT32 unused __attribute__ ((unused));
    UINT32 mask;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#ifdef CT_NO_DEBUG
    if(g_wdtStatus)
	{
	    hal_WdtIrqHandler();
	}
#endif
#endif

    sxr_EnterScSchedule();
    
    // gestion Mask
    // save current Mask
    mask = hwp_sysIrq->mask_set;
    // Mask all IT
    hwp_sysIrq->mask_clear = SYS_IRQ_MASK_SET_MASK;
    // Re-enable irq    
    hwp_sysIrq->sc = 1;

#if (CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)
	// modify for BT core Irq.
	bit = (1<<BT_SYNC_DET_IRQ);
	for(i=BT_SYNC_DET_IRQ;i>=0;i--)
	{
        if ((cause & bit)&&!assert_done)
        {
            HAL_PROFILE_IRQ_ENTER(i);
            // clear irq bit of reenabled higher prio irqs as those will be treated 
            // by an other interrupt call
            //cause &= ~g_halIrqPriorityMask[i];
            // Setup priority mask for this irq
            //hwp_sysIrq->Mask_Set = g_halIrqPriorityMask[i];
#if 1            
			if(BT_IRQ_IGNORE == (bt_irq_status)SYSirq_IRQ_State_Dispatch(i,BT_IRQ_START))
			{
				//do nothing
			}
            else if(g_halHwModuleIrqHandler[i])
            {
                (g_halHwModuleIrqHandler[i])(i);
				 if(BT_IRQ_RELOAD == (bt_irq_status)SYSirq_IRQ_State_Dispatch(i,BT_IRQ_END))
				 {
					 cause = hwp_sysIrq->cause;
				 }
            }
#else
		 if(i==7)//pkd
            {
                //HWradio_RxComplete();
            }
            if(g_halHwModuleIrqHandler[i])
            {
                (g_halHwModuleIrqHandler[i])(i);
            }
            //if(USLCsleep_Is_Tabasco_Asleep())
                rdabt_send_notify();
            if(i==7)//pkd
            {
                //HWradio_RxComplete();
                //rdabt_send_notify();
            }

            if(i==8)//pkd-hdr
            {
                //t_packet rx_packet_type=(t_packet) HW_get_rx_type();
                if(((RDA_READ_REG(0xa1700050)>>10)&0xf)==1)//poll
                    {
                        //HWradio_RxComplete();
                    }
            }
#endif
            // Mask all IT
            hwp_sysIrq->mask_clear =SYS_IRQ_MASK_SET_MASK;
			          
            HAL_PROFILE_IRQ_EXIT(i);
        }
        bit = bit >> 1;
		if(i==0)
			break;
    }

	i=10;
	bit = (1<<10);
#endif /*(CHIP_ASIC_ID==CHIP_ASIC_ID_5856E)*/

    for(;i<SYS_IRQ_QTY;i++)
    {
        if (cause & bit)
        {
            HAL_PROFILE_IRQ_ENTER(i);
            // clear irq bit of reenabled higher prio irqs as those will be treated 
            // by an other interrupt call
#if CHIP_ASIC_ID!=CHIP_ASIC_ID_5856E
            cause &= ~g_halIrqPriorityMask[i];
            // Setup priority mask for this irq
            hwp_sysIrq->mask_set = g_halIrqPriorityMask[i];
#else
            // enable bt irq
            hwp_sysIrq->mask_set = (mask & 0xfff); 
#endif

            if(g_halHwModuleIrqHandler[i])
            {
                (g_halHwModuleIrqHandler[i])(i);
            }

            // Mask all IT
            hwp_sysIrq->mask_clear =SYS_IRQ_MASK_SET_MASK;
            
            HAL_PROFILE_IRQ_EXIT(i);
        }
        bit = bit << 1;
    }

    // Disable irq
    unused = hwp_sysIrq->sc;
    // Restore previous mask
    hwp_sysIrq->mask_set = mask; 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)||(CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)\
	||(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
 //   hwp_sysIrq->mask_clear = 0x40000000;//mask pmu interrupt
#endif

    sxr_ExitScSchedule();
#endif
}


// =============================================================================
// hal_XCpuSleep
// -----------------------------------------------------------------------------
/// Put the XCpu in sleep mode until next irq.
/// @todo see how mike used that one and named it...
// =============================================================================
PROTECTED VOID hal_XCpuSleep()
{
    hwp_sysIrq->wakeup_mask = hwp_sysIrq->mask_set;
    //hwp_sysIrq->Cpu_Sleep = (SYS_IRQ_SLEEP);
    {
        UINT32 flush_wr_buff __attribute__((unused)) = hwp_sysIrq->cpu_sleep;
    }
}


PROTECTED VOID hal_XCpuEnterSleep()
{
    //hwp_sysIrq->cpu_sleep =1;
    hal_SysXcpuSleep();
}

