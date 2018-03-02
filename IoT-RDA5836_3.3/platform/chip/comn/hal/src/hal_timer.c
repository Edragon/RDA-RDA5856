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
/// @file hal_timer.c
/// That file contains the timers'module driver 
/// implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "timer.h"
#include "calendar.h"

#include "halp_debug.h"
#include "halp_timers.h"

#include "hal_mem_map.h"
#include "hal_timers.h"
#include "hal_error.h"
#include "hal_sys.h"
#include "hal_debug.h"
#include "hal_host.h"
#include "boot.h"

#include "sxr_tim.h"
#include "sxs_rmt.h"
#include "register.h"
// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#define SUPPORT_WDT  1
#else
#define SUPPORT_WDT  0

#endif

// =============================================================================
// g_halWatchdogCtx
// -----------------------------------------------------------------------------
/// Watchdog context saving structure.
// =============================================================================
HAL_DBG_WATCHDOG_CTX_T HAL_DATA_BACKUP g_halWatchdogCtx;



// =============================================================================
// g_halTickRegistry
// -----------------------------------------------------------------------------
/// Variable to store the user OS tick irq handler
// ===========================================================================
PRIVATE HAL_TIM_TICK_IRQ_HANDLER_T g_halTickRegistry
         = NULL ;



// =============================================================================
// g_halTickOverflow
// -----------------------------------------------------------------------------
/// Extra time not measurable by the OS timer
// =============================================================================
PRIVATE VOLATILE UINT32 g_halTickOverflow  ;


// =============================================================================
// g_halTickLastPeriod
// -----------------------------------------------------------------------------
/// Last period of the OS timer.
// =============================================================================
PRIVATE VOLATILE UINT32 g_halTickLastPeriod  ;




// ============================================================================
//  FUNCTIONS
// ============================================================================




#if (SUPPORT_WDT ==0)

// =============================================================================
// hal_TimWatchDogOpen
// -----------------------------------------------------------------------------
/// This function starts a watchdog timer which will start with an initial value
/// of \c delay and decrement by one every 2 periods of a 32 khz clock. When 
/// the watchdog timer reaches 0, a reset is generated.
///
/// @param delay Number of 16384 Hz clock cycles the WatchDog timer counts down 
/// before reaching 0. 
// =============================================================================
PUBLIC VOID hal_TimWatchDogOpen(UINT32 delay)
{
    // The delay time is multiplied by two to keep the tick format consistent
    // throughout all the HAL API. The watchdog is running on a 32 kHz clock.
    HAL_ASSERT( ((delay * 2) & (TIMER_LOADVAL_MASK >> TIMER_LOADVAL_SHIFT)) == delay * 2,
                "Delay overflow: %d", delay );
   #if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
   hwp_sysCtrl->per_soft_reset1 &= ~SYS_CTRL_WDT_TIMER_RSTB;
   hwp_sysCtrl->per_soft_reset1 |= SYS_CTRL_WDT_TIMER_RSTB;
   #endif
    hwp_timer->wdtimer_loadval = TIMER_LOADVAL(delay * 2);
    hwp_timer->wdtimer_ctrl = TIMER_START;
    //while ((hwp_timer->wdtimer_ctrl & TIMER_WDENABLED) == 0);
}

PUBLIC VOID hal_TimWatchDogPause(UINT8 pause)
{
    if(pause==1)
        hal_TimWatchDogStop();
    else
        hwp_timer->wdtimer_ctrl = TIMER_START;
}


// =============================================================================
// hal_TimWatchDogSetPeriod
// -----------------------------------------------------------------------------
/// Setup the watchdog timer which will start at \c delay and decrement by one
/// every 2 clocks 32kHz, after #. Will generate a reset when zero is reached 
/// If the WD timer is already started, the timer will be reseted to the new 
/// value.
/// If the WD timer is not started yet, this will only setup the configuration.
/// The WD timer will be started the next time #hal_TimWatchDogKeepAlive is 
/// called 
/// @param delay Number of 16384 Hz clock cycles the WatchDog timer counts down 
/// before reaching 0. 
// =============================================================================
PUBLIC VOID hal_TimWatchDogSetPeriod(UINT32 delay)
{
    // this does not start the watchdog but initalizes the delay
    // if the WD is already started it will be reseted
    HAL_ASSERT( ((delay * 2) & (TIMER_LOADVAL_MASK >> TIMER_LOADVAL_SHIFT)) == delay * 2,
                "Delay overflow: %d", delay );

    hwp_timer->wdtimer_loadval = TIMER_LOADVAL(delay * 2);

    hwp_timer->wdtimer_ctrl = TIMER_RELOAD;
}



// =============================================================================
// hal_TimWatchDogKeepAlive
// -----------------------------------------------------------------------------
/// Reloads the watchdog timer
/// This function restarts the watchdog timer with the \c delay used to start 
/// it (#hal_TimWatchDogOpen) or to set it up (#hal_TimWatchDogSetPeriod), 
/// preventing the reset from occurring.
// =============================================================================
PUBLIC VOID hal_TimWatchDogKeepAlive(VOID)
{
    // this resets the watchdog and starts it if not done yet
    if (hwp_timer->wdtimer_ctrl & TIMER_WDENABLED)
    {
        hwp_timer->wdtimer_ctrl = TIMER_RELOAD;
    }
}



// =============================================================================
// hal_TimWatchDogClose
// -----------------------------------------------------------------------------
/// Disables the watchdog timer 
/// This function disables the watchdog timer. A reset will not occur because
/// of the watchdog, even if #hal_TimWatchDogKeepAlive is not called. The 
/// watchdog can be re-enabled by #hal_TimWatchDogOpen.
// =============================================================================
PUBLIC VOID hal_TimWatchDogClose(VOID)
{
    hwp_timer->wdtimer_ctrl = TIMER_STOP;    
    hwp_timer->wdtimer_loadval = 0;
}



// =============================================================================
// hal_TimWatchDogStop
// -----------------------------------------------------------------------------
/// Stop the watchdog timer 
/// This function stops the watchdog timer. A reset will not occur because
/// of the watchdog, even if #hal_TimWatchDogKeepAlive is not called. The 
/// watchdog can be re-enabled by a call to #hal_TimWatchDogKeepAlive.
// =============================================================================
PROTECTED VOID hal_TimWatchDogStop(VOID)
{
    hwp_timer->wdtimer_ctrl = TIMER_STOP;    
}
#else
// =============================================================================
// hal_TimWatchDogOpen
// -----------------------------------------------------------------------------
/// This function starts a watchdog timer which will start with an initial value
/// of \c delay and decrement by one every 2 periods of a 32 khz clock. When 
/// the watchdog timer reaches 0, a reset is generated.
///
/// @param delay Number of 16384 Hz clock cycles the WatchDog timer counts down 
/// before reaching 0. 
// =============================================================================
PUBLIC VOID hal_TimWatchDogOpen(UINT32 delay)
{
    hwp_sysCtrl->per_module_en |= SYS_CTRL_WDT_EN;
	hwp_wdt->wdogloada = WDT_LOAD_VALUE(delay * 2);
    hwp_wdt->wdogcontrola = WDT_INT_EN ;
}

PUBLIC VOID hal_TimWatchDogPause(UINT8 pause)
{
    if(pause==1)
         hwp_sysCtrl->per_module_en &= ~SYS_CTRL_WDT_EN;
    else
         hwp_sysCtrl->per_module_en |= SYS_CTRL_WDT_EN;
}


// =============================================================================
// hal_TimWatchDogSetPeriod
// -----------------------------------------------------------------------------
/// Setup the watchdog timer which will start at \c delay and decrement by one
/// every 2 clocks 32kHz, after #. Will generate a reset when zero is reached 
/// If the WD timer is already started, the timer will be reseted to the new 
/// value.
/// If the WD timer is not started yet, this will only setup the configuration.
/// The WD timer will be started the next time #hal_TimWatchDogKeepAlive is 
/// called 
/// @param delay Number of 16384 Hz clock cycles the WatchDog timer counts down 
/// before reaching 0. 
// =============================================================================
PUBLIC VOID hal_TimWatchDogSetPeriod(UINT32 delay)
{
    // this does not start the watchdog but initalizes the delay
    // if the WD is already started it will be reseted
    HAL_ASSERT( ((delay * 2) & (TIMER_LOADVAL_MASK >> TIMER_LOADVAL_SHIFT)) == delay * 2,
                "Delay overflow: %d", delay );
	hwp_wdt->wdogloada = WDT_LOAD_VALUE(delay * 2);
}



// =============================================================================
// hal_TimWatchDogKeepAlive
// -----------------------------------------------------------------------------
/// Reloads the watchdog timer
/// This function restarts the watchdog timer with the \c delay used to start 
/// it (#hal_TimWatchDogOpen) or to set it up (#hal_TimWatchDogSetPeriod), 
/// preventing the reset from occurring.
// =============================================================================
PUBLIC VOID hal_TimWatchDogKeepAlive(VOID)
{
    // this resets the watchdog and starts it if not done yet
    hwp_wdt->wdogloada = WDT_LOAD_VALUE(30 SECOND * 2);
   
}



// =============================================================================
// hal_TimWatchDogClose
// -----------------------------------------------------------------------------
/// Disables the watchdog timer 
/// This function disables the watchdog timer. A reset will not occur because
/// of the watchdog, even if #hal_TimWatchDogKeepAlive is not called. The 
/// watchdog can be re-enabled by #hal_TimWatchDogOpen.
// =============================================================================
PUBLIC VOID hal_TimWatchDogClose(VOID)
{
	hwp_wdt->wdogcontrola = 0;
	hwp_sysCtrl->per_module_en &= ~SYS_CTRL_WDT_EN;
	hwp_wdt->wdogloada = 0;

}



// =============================================================================
// hal_TimWatchDogStop
// -----------------------------------------------------------------------------
/// Stop the watchdog timer 
/// This function stops the watchdog timer. A reset will not occur because
/// of the watchdog, even if #hal_TimWatchDogKeepAlive is not called. The 
/// watchdog can be re-enabled by a call to #hal_TimWatchDogKeepAlive.
// =============================================================================
PROTECTED VOID hal_TimWatchDogStop(VOID)
{
    hwp_wdt->wdogcontrola = 0;
    hwp_wdt->wdogloada = 0;
    hwp_sysCtrl->per_module_en &= ~SYS_CTRL_WDT_EN;
}
#endif


// ============================================================================
// hal_TimTickIrqHandler
// ----------------------------------------------------------------------------
/// OS tick IRQ handler, used by the IRQ module to clear the cause and call 
/// the 'user' function.
// ============================================================================ 
PROTECTED VOID hal_TimTickIrqHandler(UINT8 interruptId)
{
    // Clear the Os tick timer IRQ
    hwp_timer->timer_irq_clr =TIMER_OSTIMER_CLR;

    // call user handler
    if (g_halTickRegistry)
    {
        g_halTickRegistry();
    }
}



// ============================================================================
// hal_TimTickGetVal
// ----------------------------------------------------------------------------
/// @return The value of the OS timer.
// ============================================================================
INLINE UINT32 hal_TimTickGetVal(VOID)
{
    return (hwp_timer->ostimer_curval);
}

PUBLIC INT32 hal_TimTickGetValSigned(VOID)
{
    return (INT32) (hwp_timer->ostimer_curval);
}

// ============================================================================
// hal_TimTickStart
// ----------------------------------------------------------------------------
/// Start the OS timer in the specified mode
/// @param loadVal Initial value of the OS timer
/// @param mode Timer mode.
// ============================================================================
INLINE VOID hal_TimTickStart(UINT32 loadVal)
{
    hwp_timer->ostimer_ctrl = TIMER_ENABLE | TIMER_LOAD | TIMER_LOADVAL(loadVal) | TIMER_WRAP;
}



// ============================================================================
// hal_TimTickStop
// ----------------------------------------------------------------------------
/// Stops the OS tick timer.
// ============================================================================
INLINE VOID hal_TimTickStop(VOID)
{
    hwp_timer->ostimer_ctrl = 0;
}



// ============================================================================
// hal_TimTickIrqSetHandler
// ----------------------------------------------------------------------------
/// Set the function called whent the OS timer wraps.
/// @param handler The function called when the OS timer reaches 0.
// ============================================================================
INLINE VOID hal_TimTickIrqSetHandler(HAL_TIM_TICK_IRQ_HANDLER_T handler)
{
    g_halTickRegistry = handler;
}



// ============================================================================
// hal_TimTickIrqSetMask
// ----------------------------------------------------------------------------
/// Set the OS timer IRQ mask.
/// @param mask OS timer IRQ mask. If \c TRUE, the IRQ is triggable.
// ============================================================================
INLINE VOID hal_TimTickIrqSetMask(BOOL mask)
{
    if (mask)
    {
        hwp_timer->timer_irq_mask_set = TIMER_OSTIMER_MASK;
    }
    else
    {
        hwp_timer->timer_irq_mask_clr = TIMER_OSTIMER_MASK;
    }
}


// ============================================================================
// hal_TimTickIrqGetMask
// ----------------------------------------------------------------------------
/// @return The OS IRQ mask
// ============================================================================
INLINE BOOL hal_TimTickIrqGetMask(VOID)
{
    if (hwp_timer->timer_irq_mask_set & TIMER_OSTIMER_MASK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



// ============================================================================
// hal_TickIrqHandler
// ----------------------------------------------------------------------------
/// IRQ handler for the OS timer, called when this timer wrap.
// ============================================================================
PRIVATE VOID hal_TickIrqHandler(VOID)
{
    
    UINT32 scStatus;
    INT32 tickError=0;
    UINT32 reportedPeriod;

    scStatus=hal_SysEnterCriticalSection();
    
    // save error
    tickError = (INT32) hal_TimTickGetVal();

    // disable timer
    hal_TimTickStop();

    reportedPeriod = g_halTickLastPeriod-tickError;

    hal_SysExitCriticalSection(scStatus);

#ifdef TIM_VERBOSE
    HAL_TRACE(_SXR | TSTDOUT, 0, "TickIRQ: Tampax %d, Error %d", reportedPeriod, tickError);
#endif

#if OS_KERNEL == SXR 
    sxr_Tampax(reportedPeriod, 0);
#endif
}


PUBLIC VOID hal_TickPreTampax(VOID)
{
    
    UINT32 scStatus;
    INT32 tickError=0;
    UINT32 reportedPeriod;

    scStatus=hal_SysEnterCriticalSection();
    
    // save error
    tickError = (INT32) hal_TimTickGetVal();

    HAL_ASSERT((tickError > 0), "hal_TickPreTampax error");
    // disable timer
    hal_TimTickStop();

    reportedPeriod = g_halTickLastPeriod-tickError;

    hal_SysExitCriticalSection(scStatus);

#ifdef TIM_VERBOSE
    HAL_TRACE(_SXR | TSTDOUT, 0, "TickIRQ: Tampax %d, Error %d", reportedPeriod, tickError);
#endif

#if OS_KERNEL == SXR 
    sxr_Tampax(reportedPeriod, 0);
#endif
}

// ============================================================================
// hal_TickIrqHandler
// ----------------------------------------------------------------------------
/// IRQ handler for the OS timer, called when this timer wrap.
// ============================================================================
PROTECTED VOID hal_SelectIrqHandler(UINT8 interruptId)
{
    UINT32 cause = (hwp_timer->timer_irq_cause)&0x7;
    switch(cause)
    {
        case TIMER_OSTIMER_CAUSE:
            hwp_timer->timer_irq_clr = TIMER_OSTIMER_CLR;
	     hal_TimTickIrqHandler(interruptId);
            break;

        case TIMER_HWTIMER_WRAP_CAUSE:
            hwp_timer->timer_irq_clr = TIMER_HWTIMER_WRAP_CLR;
            hal_HstSendEvent(0xff, 0x20162017);
            break;              

        case TIMER_HWTIMER_ITV_CAUSE:
            hwp_timer->timer_irq_clr = TIMER_HWTIMER_ITV_CLR;
            hal_HstSendEvent(0xff, 0x20162018);
            break;
        default:
            break;
    }
}

// ============================================================================
// hal_TickInit
// ----------------------------------------------------------------------------
/// Initializes the OS tick timer and configure its driver.
/// This function is called by hal_Open.
// ============================================================================
PROTECTED VOID hal_TimTickOpen(VOID)
{
    //HAL_PROFILE_FUNCTION_ENTER(hal_TimTickOpen);
   #if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
   uint32 readdata;

   readdata = hwp_sysCtrl->per_module_en;
   if ((readdata&SYS_CTRL_TIMER_EN) == 0)
   {
       hwp_sysCtrl->per_module_en |= SYS_CTRL_TIMER_EN;
   }
   #endif
    
    g_halTickOverflow=0;
    g_halTickLastPeriod = 0;
    hal_TimTickIrqSetHandler(hal_TickIrqHandler);
    hal_TimTickIrqSetMask(TRUE);
    //HAL_PROFILE_FUNCTION_EXIT(hal_TimTickOpen);
}


// ============================================================================
// hal_TimSetExpirationTime
// ----------------------------------------------------------------------------
/// Set the next time when the OS timer will wrap.
/// @param nextPeriod The next period before the new wrap.
// ============================================================================
PUBLIC VOID hal_TimSetExpirationTime(UINT32 nextPeriod)
{
    HAL_PROFILE_FUNCTION_ENTER(hal_TimSetExpirationTime);
    UINT32 status;
    UINT32 period = nextPeriod;

    status=hal_SysEnterCriticalSection();
#ifdef TIM_VERBOSE
        HAL_TRACE(_SXR|TSTDOUT,0,"TickNotify: Next_Period %d, TickError %d",Next_Period,TickError);
#endif

    if (period>HAL_TIM_MAX_PER)
    {
        g_halTickOverflow = period-HAL_TIM_MAX_PER;
        g_halTickLastPeriod = HAL_TIM_MAX_PER;
    }
    else
    {
        g_halTickOverflow = 0;
        g_halTickLastPeriod = period;

    }

#ifdef TIM_VERBOSE
        HAL_TRACE(_SXR|TSTDOUT,0, "TickNotify: g_halTickLastPeriod %d",g_halTickLastPeriod);
#endif

    hal_TimTickStart(g_halTickLastPeriod);
   
    hal_SysExitCriticalSection(status);
    HAL_PROFILE_FUNCTION_EXIT(hal_TimSetExpirationTime);
}


// ============================================================================
// hal_TimGetReminderTime
// ----------------------------------------------------------------------------
/// Return the remaining time to count before the previously programmed period
/// expires.
/// @return Time before the previously programmed period expires.
// ============================================================================
PUBLIC UINT32 hal_TimGetReminderTime(VOID)
{
    HAL_PROFILE_FUNCTION_ENTER(hal_TimGetReminderTime);
    UINT32 status;
    UINT32 timVal;


    status=hal_SysEnterCriticalSection();

//   // the timer counts negatively once wrapped, so ...
//     // overflow + wrap time = remaining time to count
//     if ((hwp_timer->irq_cause & TIMER_STATUS(OSTIM_IRQ)))
//     {
//         // The counter wrapped and the irq is pending or under treatment
//         tim_val=g_halTickOverflow;
// #ifdef TIM_VERBOSE
//         hal_Printf((_SXR | TSTDOUT,"TickGetVal : Irq Pending"));
// #endif
//     }
//     else 
        timVal = hal_TimTickGetVal() + g_halTickOverflow;
    
#ifdef TIM_VERBOSE
    HAL_TRACE(_SXR | TSTDOUT, 0, "TickGetVal: %d",timVal);
#endif

    hal_SysExitCriticalSection(status);
    HAL_PROFILE_FUNCTION_EXIT(hal_TimGetReminderTime);
    return (timVal);
}



// =============================================================================
// hal_TimDelay
// -----------------------------------------------------------------------------
/// This function is to be used when we want to wait for a delay.
/// Attention must be paid to the fact that this is a blocking function
/// and that it could be avoided as possible, and OS function should be
/// used instead. But sometimes, we cannot use the OS functions ...
///
/// @param delay The delay to wait, in number of 16 kHz periods.
// =============================================================================
PUBLIC VOID HAL_BOOT_FUNC hal_TimDelay(UINT32 delay)
{
    UINT32 initialValue;

    initialValue = hwp_timer->hwtimer_curval;

    // Wait
    while ((hwp_timer->hwtimer_curval - initialValue) < delay); 

}



// =============================================================================
// hal_TimGetUpTime
// -----------------------------------------------------------------------------
/// Return the up time of the system in 16384Hz ticks.
/// The uptime is given as a 32 bits numbers, therefore the user of the
/// function must take care of the count wrap, that will occurs every 
/// three days.
///
/// Put this function into RAM as single-bank flash driver will use it when erasing sectors.
///
/// NOTE: to implement timeouts user can calculate (newtime - starttime) 
/// and then compare to the desired delay
/// see http://atlas/twiki/bin/view/CoolWiki/CodeAcademyWrapUpTime for details.
/// 
///
/// @return Up time, in 16384Hz ticks.
// =============================================================================
PUBLIC UINT32 HAL_BOOT_FUNC_INTERNAL hal_TimGetUpTime(VOID)
{
    return hwp_timer->hwtimer_curval;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//---------  Non Hal functions, accessed through hal_timers.h  ----------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

extern VOID hal_GdbWatchDogHandler(VOID);

// =============================================================================
// hal_TimeWdRecover
// -----------------------------------------------------------------------------
// Called early in the boot process when watchdog occured, so running in the memory
// context when the WDog occured. Displays the watchdog context.
// =============================================================================
PUBLIC VOID hal_TimeWdRecover(VOID)
{
    HAL_PROFILE_PULSE(XCPU_ERROR);
    hal_DbgFatalTrigger(HAL_DBG_EXL_FATAL);

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E)
    if (g_bootResetCause & SYS_CTRL_WDT_RST)
#else
    if (g_bootResetCause & SYS_CTRL_WATCHDOG_RESET_HAPPENED)
#endif
    {
        HAL_DBG_ERROR_CTX_T *errCtx = &(g_halWatchdogCtx.errorContext);

        // WatchDog detected
        // Do not clear watchdog bit or hal_TimWatchDogHappened wont work
        // anyway it is not required to clear it as other reset cause clears it:
        //  - full soft reset clears it
        //  - external hardware reset clears it
        // hwp_reg->sys_rst_clr = WD_HAPPENED;
        HAL_TRACE(_SXR|TSTDOUT, 0,"########################");
        HAL_TRACE(_SXR|TSTDOUT, 0,"WATCH DOG RESET DETECTED\n");
        HAL_TRACE(_SXR|TSTDOUT, 0,"Registers:");
        HAL_TRACE(_SXR|TSTDOUT, 0,"ze=0x00000000  at=0x%08X  v0=0x%08X  v1=0x%08X",
            errCtx->at, errCtx->v0, errCtx->v1);
        HAL_TRACE(_SXR|TSTDOUT, 0,"a0=0x%08X  a1=0x%08X  a2=0x%08X  a3=0x%08X",
            errCtx->a0, errCtx->a1,
            errCtx->a2, errCtx->a3);
        HAL_TRACE(_SXR|TSTDOUT, 0,"t0=0x%08X  t1=0x%08X  t2=0x%08X  t3=0x%08X",
            errCtx->t0, errCtx->t1,
            errCtx->t2, errCtx->t3);
        HAL_TRACE(_SXR|TSTDOUT, 0,"t4=0x%08X  t5=0x%08X  t6=0x%08X  t7=0x%08X",
            errCtx->t4, errCtx->t5,
            errCtx->t6, errCtx->t7);
        HAL_TRACE(_SXR|TSTDOUT, 0,"s0=0x%08X  s1=0x%08X  s2=0x%08X  s3=0x%08X",
            errCtx->s0, errCtx->s1,
            errCtx->s2, errCtx->s3);
        HAL_TRACE(_SXR|TSTDOUT, 0,"s4=0x%08X  s5=0x%08X  s6=0x%08X  s7=0x%08X",
            errCtx->s4, errCtx->s5,
            errCtx->s6, errCtx->s7);
        HAL_TRACE(_SXR|TSTDOUT, 0,"t8=0x%08X  t9=0x%08X  k0= < N/A >    k1=0x%08X",
            errCtx->t8, errCtx->t9, errCtx->k1);
        HAL_TRACE(_SXR|TSTDOUT, 0,"gp=0x%08X  sp=0x%08X  fp=0x%08X  ra=0x%08X",
            errCtx->gp, errCtx->sp,
            errCtx->fp, errCtx->ra);

        HAL_TRACE(_SXR|TSTDOUT, 0,"PC when WDog occured : 0x%08X" , errCtx->pc | 0x80000000);
        #if OS_KERNEL == SXR 
        sxs_BackTrace(errCtx->pc | 0x80000000, (UINT32*)errCtx->sp);
        
        sxs_Boot -> WDog = TRUE;
//      sxs_Boot -> ExceptionExpected = TRUE;
//      sxs_Boot -> ExceptionCount++;
        sxs_SaveRmtBuffer ();
        #endif
    }
}






// ============================================================================
// hal_TimTimersIrqHandler
// ----------------------------------------------------------------------------
/// Timers module IRQ handler.
// ============================================================================
PROTECTED VOID hal_TimTimersIrqHandler(UINT8 interruptId)
{
    // get status
    UINT16 status = hwp_timer->timer_irq_cause;
    // OS Timer is on another line and use another module handler
    // so we get rid of its cause.
    #if(CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    status &= TIMER_OTHER_TIMS_IRQ_MASK;
    #endif
        
    // clear irq  
    hwp_timer->timer_irq_clr = status;
   

    // According to the status bit, call the appropriate 
    // user handler


    // Delay timer, if ever used as an interrupt
    // generator, will lie here :
}






