/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 2003 CII Technologies, Inc.                                      */
/*                                                                       */
/* PROPRIETARY RIGHTS of CII Technology are involved in the                  */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*     dm_mmi.c                                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/* AUTHOR                                                                */
/*       jinsong                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      DATE  2006/8/7            REMARKS                                  */
/*************************************************************************/

#include "chip_id.h"
#include <errorcode.h>
#include <cos.h>
#include <base_prv.h>
#include <dm.h>
#include <drv.h>

#include "hal_pwm.h"
#include "hal_sys.h"
#include "hal_lps.h"
#include "dbg.h"

#include "csw_csp.h"



// =============================================================================
// csw_SetResourceActivity
// -----------------------------------------------------------------------------
/// Require a minimum system frequency for a given resource.
/// To release the resource, allowing for lowpower, you have to require a
/// frequenct of 32kHz, ie: #CSW_SYS_FREQ_32K.
/// Any other requirement will take the resource.
///
/// @param resource Resource that requests a minimum system frequency.
/// @param freq Minimum system frequency required. The actual system fequency
/// may be faster.
// =============================================================================
PUBLIC VOID csw_SetResourceActivity(CSW_LP_RESOURCE_T resource, CSW_SYS_FREQ_T freq)
{
    // Modem2G class or above
    DBG_ASSERT((HAL_SYS_FREQ_APP_USER_0+resource) <= HAL_SYS_FREQ_APP_LAST_USER,"Illegal resource number %d",resource);
    hal_SysRequestFreq((HAL_SYS_FREQ_USER_ID_T)(HAL_SYS_FREQ_APP_USER_0+resource), (HAL_SYS_FREQ_T)freq, NULL);
}

PUBLIC VOID csw_SetVOCFreqActivity(CSW_VOC_FREQ_T freq)
{
    // Modem2G class or above
    hal_SysSetVocClock((HAL_SYS_VOC_FREQ_T)freq);
}


UINT8 uLcdLevel = 0xff;
UINT8 uKeyLevel = 0xff;
BOOL bSetClkCalTimer  = FALSE;

#define BAL_TH(x)   1

VOID DM_KeyBackLightOpen()
{
  DRV_SetKeyBacklight(uKeyLevel);
}

VOID DM_KeyBackLightClose()
{
  DRV_SetKeyBacklight(0);
}

VOID DM_LpwMmiActive()
{
    if(bSetClkCalTimer)
    {
        COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY), PRV_DM_LPCLKCALIB_TIMER_ID);
        bSetClkCalTimer = FALSE;
    }
}

VOID DM_LpwMmiInactive()
{
    if(bSetClkCalTimer)
    {
        COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY), PRV_DM_LPCLKCALIB_TIMER_ID);
        bSetClkCalTimer = FALSE;
    }
    if(COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY), PRV_DM_LPCLKCALIB_TIMER_ID, COS_TIMER_MODE_SINGLE, MMI_LPCLKCALIB_TIMER))
    {
        bSetClkCalTimer = TRUE;
    }
}

