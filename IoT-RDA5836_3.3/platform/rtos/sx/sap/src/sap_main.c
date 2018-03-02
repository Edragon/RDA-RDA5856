/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : sap_main.c
--------------------------------------------------------------------------------

  Scope      : Main.

  History    :
--------------------------------------------------------------------------------
================================================================================
*/

#include "chip_id.h"
#include "hal_sys.h"

#if !defined(__TARGET__) || defined (__SIMU__)
#include <malloc.h>
#ifdef __STKTRACE__
#include "sxs_udp.h"
#endif
#endif

#include "sxs_srl.h"
#include "sxs_type.h"
#include "sxs_lib.h"
#include "sxr_ops.h"
#include "sxr_csp.h"
#include "sxr_mutx.h"
#include "sxs_rmt.h"

#if defined(GSM_SUPPORT)
#define __SAP_CFG_VAR__
#include "sap_cfg.h"
#undef  __SAP_CFG_VAR__

typedef u32 MsgBody_t;
#include "itf_msg.h"

#endif /* GSM_SUPPORT */

#include "sxs_io.h"
#include "sxs_spy.h"

#define PAL_DBG_PORT_IDX 0


#include "cos.h"
#include "event.h"
#include "hal_host.h"
#include "hal_lps.h"
#include "hal_sys.h"
#include "hal_bt.h"
#include "tgt_m.h"
#include "timer.h"
#include "register.h"
#include "uart.h"
#include "global_macros.h"

#define IDLE_TASK_STACK_SIZE 128//98//128 // 256byte
#define IDLE_TASK_PRIORITY   255

void sxs_TaskIdle (void);
BOOL BAL_TaskInit(VOID);
#if defined(__AT_IN_DEBUG_HOST__)
extern void uart_check_uart_command(void);
#endif /* __AT_IN_DEBUG_HOST__ */

#if !defined(__SAP_CFG_H__)
const sxr_TaskDesc_t sxs_TaskIdleDesc =
{
 (void(*)(void *))sxs_TaskIdle,
// 0,
 "Task Idle.",
 IDLE_TASK_STACK_SIZE,
 IDLE_TASK_PRIORITY
};
#endif /* !__SAP_CFG_H__ */

extern BOOL rdabt_check_bttask();

//------------------------------------------------------------------
// Idle hook variables

void (*g_sxsFsIdleHookFuncPtr)(void) = NIL;

#define SXS_DEBUG_IDLE_HOOK_NUM 4
void (*g_sxsDebugIdleHookFuncPtr[SXS_DEBUG_IDLE_HOOK_NUM])(void) =
    { NIL, };

// To avoid linkage issue while compiling platform_test programs,
// we put the definition in sx_map.c instead of here
extern volatile u32 g_sxsDebugIdleHookEnable;

//------------------------------------------------------------------
// FS idle hook functions

void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void))
{
    g_sxsFsIdleHookFuncPtr = funcPtr;
}

void sxs_FsIdleHook (void)
{
    if (g_sxsFsIdleHookFuncPtr != NIL)
    {
        (*g_sxsFsIdleHookFuncPtr)();
    }
}

//------------------------------------------------------------------
// Debug idle hook functions

bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void))
{
    int i;
    bool result = FALSE;

    u32 status = sxr_EnterSc ();
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookFuncPtr[i] == NIL)
            break;
    }

    if (i < SXS_DEBUG_IDLE_HOOK_NUM)
    {
        g_sxsDebugIdleHookFuncPtr[i] = funcPtr;
        result = TRUE;
    }
    sxr_ExitSc (status);

    return result;
}

void sxs_DebugIdleHook (void)
{
#ifndef CHIP_HAS_NO_ISPI    
    PUBLIC VOID hal_AllSpiRegCheck(VOID);
    hal_AllSpiRegCheck(); // no check flag 
#endif    

    if (g_sxsDebugIdleHookEnable == 0)
    {
        return; // skip this debug hook
    }

    int i;
    for (i=0; i<SXS_DEBUG_IDLE_HOOK_NUM; i++)
    {
        if (g_sxsDebugIdleHookFuncPtr[i] != NIL)
        {
            (*g_sxsDebugIdleHookFuncPtr[i])();
        }
    }
}

UINT32 TEST_M=0;

#if defined(GSM_SUPPORT)
/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_Task (sxs_TaskParse_t *TaskParse)
{
 Msg_t *Msg;
 u8    SenderTaskId;
 u32   SenderAddr;
 //u32   Evt [SXR_EVT_MBX_SIZE];
 u32 *Evt = sxr_HMalloc(SXR_EVT_MBX_SIZE*sizeof(u32));


 if (TaskParse -> TaskInit != NIL)
 {
  TaskParse -> TaskInit ();
 }

 while (TRUE)
 {
//  Msg = (Msg_t *)sxr_WaitEx (Evt, TaskParse -> MailBox, &SenderTaskId, &SenderAddr);
  Msg = (Msg_t *)sxr_Wait (Evt, TaskParse -> MailBox
  #ifdef SXR_MAILBOX_TIMEOUT_SUPPORT
  , COS_WAIT_FOREVER
  #endif
  );

  if (Msg)
  {
   TaskParse -> MsgParse (Msg);
  }
  else
  {
   if (Evt [0] & SXS_TIMER_EVT_ID)
   {
    TaskParse -> TimerParse (Evt);
   }
   else
   {
    if (Evt [0] == NIL)
    {
     SXS_RAISE ((_SXR|TNB_ARG(2)|TABORT|TDB, TSTR("Invalid event received at sxs_Task. Sender task=%d, addr=0x%08x", 0x070b0002), SenderTaskId, SenderAddr));
    }
    ((void (*)(void *))Evt [0]) ((void *)Evt [1]);
   }
  }
 }
}
#endif /* GSM_SUPPORT */

/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_StartAll (void)
{
#ifdef __PROTO_REMOTE__
 u32 i;
#endif

 sxr_TaskInit();
 sxr_TkSdInit ();

 //sxr_SbxInit (SXR_NB_MAX_MBX_ENV, SXR_NB_MAX_MBX);
 sxr_SbxInit();
 sxr_MutexInit ();

 COS_Timer_Initialise();

 sxs_IoInit ();

#if defined(GSM_SUPPORT)
 sxr_JbSdInit ();
 
 for (i=0; i<CMN_LAST; i++)
 {
  sxr_NewMailBox ();
 }

 sxs_InitSpy();
 
 sxr_InitQueue ();
 sxr_NewQueue ("Cs Data");
#endif /* GSM_SUPPORT */

#ifndef NO_REMOTE_COMPUTER
 u16 SetUart = SXS_SET_NB_DATA (SXS_8_DATA)
   | SXS_SET_NB_STOP (SXS_1_STOP)
   | SXS_SET_PARITY (SXS_PARITY_DISABLE)
   | SXS_SET_RATE (SXS_R460800);

 sxs_InitSerial (SetUart, PAL_DBG_PORT_IDX, SXS_RMT_DBG_CPU_IDX);


#ifdef __PROTO_REMOTE__
 for (i=CMN_RR; i< CMN_LAST; i++)
  sxr_SetRmtMbx (i, TRUE);
#endif
#endif// #ifndef NO_REMOTE_COMPUTER

  sxr_StartTask (sxr_NewTask (&sxs_TaskIdleDesc), NIL);

#if defined(GSM_SUPPORT)
 for (i=0; i<sxs_NbStaticTasksDesc; i++)
 {
  u8 TaskId =
  sxs_CmnTaskId [sxs_TaskCfg [i].TaskParse ->MailBox] = sxr_NewTask (&sxs_TaskCfg [i].TaskDesc);
  sxr_StartTask (TaskId, (void *)sxs_TaskCfg [i].TaskParse);
 }  
#endif /* GSM_SUPPORT */
}


/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/

void sxs_TaskIdle (void)
{
 u32 Cnt = 0;
// extern UINT8 g_sys_sleep_flag;
#ifndef NO_REMOTE_COMPUTER
 u8 SerialDataAvail = FALSE;
#endif
 TEST_M=7;
 
 while (TRUE)
 {
  TEST_M++;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
#ifndef CODEC_SUPPORT

  if(!hal_IsBtDead())
#endif
#endif
  {
#ifdef CT_NO_DEBUG
        hal_TimWatchDogKeepAlive();
#endif
  }
  
#if !defined(_T_UPGRADE_PROGRAMMER)
  sxs_FsIdleHook();

#ifndef NO_REMOTE_COMPUTER
  SerialDataAvail = sxs_SerialFlush ();
#endif
//  hal_SysProcessIdle();
//  pal_WDogCntRst ();
//  sxs_SpyData ();
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
  //if(g_sys_sleep_flag)
#endif
  {
    //hal_LpsSleep();
  }
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#if(COMBINED_HOST != 0)
  if(hal_IsBtStart() && rdabt_check_bttask())
 {
    extern UINT8  rdabt_send_notify_pending;
    COS_EVENT ev = {0};
    if(rdabt_send_notify_pending == 0)
    {
        UINT32 status = hal_SysEnterCriticalSection();
        rdabt_send_notify_pending++;
        hal_SysExitCriticalSection(status);
        ev.nEventId = EV_BT_NOTIFY_IND;
        COS_SendEvent(MOD_BT,  &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }
 }
  else
 {
    //if(rdabt_send_notify_pending == 0)
       hal_LpsSleep();
 }
#endif
#endif  
#ifndef NO_REMOTE_COMPUTER
  sxs_RmtIdleHook ();
#endif
  sxs_DebugIdleHook();
#if defined(__AT_IN_DEBUG_HOST__)
  uart_check_uart_command();
#endif /* __AT_IN_DEBUG_HOST__ */
#endif
  Cnt++;
 }
}

extern void sxr_NewCluster (u16 Size, u8 NbCluster);
/*
==============================================================================
   Function   :
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/


#if defined(_T_UPGRADE_PROGRAMMER)
int sap_main (int argc, char *argv[])
#else
int main (int argc, char *argv[])
#endif
{
TEST_M=1;

#ifndef NO_REMOTE_COMPUTER
 sxs_SetFnStamp( (u32*)(((u32)_sxs_SetFnStamp) & ~3) );
#endif

TEST_M=2;

 sxs_StartAll ();
TEST_M=3;

#ifdef GSM_SUPPORT
//For stack test
sxr_NewCluster (18, 100);
sxr_NewCluster (22, 220);
sxr_NewCluster (50, 150);
sxr_NewCluster (60, 200);
sxr_NewCluster (66, 150);
sxr_NewCluster (80, 200);
sxr_NewCluster (580, 100);
#endif

 // User specific initialisation
 BAL_TaskInit();

 sxr_CheckTask();
 TEST_M=6;
 #if(COMBINED_HOST != 0)
#ifdef CT_NO_DEBUG
    hal_TimWatchDogOpen(30 SECOND);
#else
    hal_TimWatchDogClose();  //Disable Watchdog in debug mode
#endif
#endif
 sxr_StartScheduling ();

 return 0;
}



