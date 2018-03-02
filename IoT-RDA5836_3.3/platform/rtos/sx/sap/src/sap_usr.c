//------------------------------------------------------------------------------
//  $Log: sap_usr.c,v $
//  Revision 1.6  2006/04/21 14:59:53  fch
//  Update with Bob
//
//  Revision 1.5  2006/03/24 10:31:31  fch
//  Remove scenario list (moved to MMI)
//
//  Revision 1.4  2006/03/09 11:02:48  fch
//  Update from bob
//
//  Revision 1.3  2006/01/17 13:45:00  oth
//  Update from Bob Ref v1.18
//
//  Revision 1.2  2005/12/29 17:39:16  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
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
  File       : sap_usr.c
--------------------------------------------------------------------------------

  Scope      : User specific initialisation for the system

  History    :
--------------------------------------------------------------------------------
  Jun 05 06  | OTH   | Remove the include of the ed_usr.h
  Mar 28 06  | MCE   | re-enabled call to sxs_RmcCtxInit ()
  Mar 28 06  | ADA   | Added sap_SerialSpeed()
  Sep 05 03  | ADA   | Creation
  Aug 31 05  | ADA   | Move cluster creation to sap_Main.c
  Nov 28 05  | MCE   | temporarily out-commented call to sxs_RmcCtxInit ()
================================================================================
*/


/*
  This file is given for example, every application must have his one
*/

#include "sxs_type.h"
#include "chip_id.h"

#ifndef __TARGET__
#include <malloc.h>
void pal_SimuIdleHook (void);
#else
#include "sxs_srl.h"
#include "sxs_rmt.h"
#endif  // __TARGET__

#include "sxs_lib.h"
#include "sxs_io.h"
#include "sxr_ops.h"
#include "sap_cfg.h"
#include "pal_gsm.h"
#include "sxs_spy.h"
#include "hal_sim.h"
#include "hal_map_engine.h"
#include "sxs_rmc.h"
#include "hal_debug.h"
#include "hal_signal_spy.h"



/*
==============================================================================
   Function   : sap_InitUsr
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/

void sap_InitUsr(void)
{
    //sxs_IoCtx.TraceBitMap [TGET_ID(_MM)] = 0xFFF1;
    //sxs_IoCtx.TraceBitMap [TGET_ID(_CC)] = 0xFFF5;
    //sxs_IoCtx.TraceBitMap [TGET_ID(_MMI)] = 0xFFFF; // Print all MMI trace on default.
    //sxs_IoCtx.TraceBitMap [TGET_ID(_SM)] = 1 | (1 << 4);
    //sxs_IoCtx.TraceBitMap [TGET_ID(_SND)] = 1 | (1 << 2) | (1 << 4) | (1 << 6) | (1 << 8);
    //sxs_IoCtx.TraceBitMap [TGET_ID(_LLC)] = 1 | (1 << 2) | (1 << 4);
    sxs_IoCtx.TraceBitMap [TGET_ID(_CSW)] = 1 | (1 << 1) | (1 << 3);
    sxs_IoCtx.TraceBitMap [TGET_ID(_API)] = 1;
    sxs_IoCtx.TraceBitMap [TGET_ID(_APP)] = 1 | (1 << 2) ;
    sxs_IoCtx.TraceBitMap [TGET_ID(_SIM)] = 0xFFFF;

    sxs_RmcCtxInit ();

    hal_DbgSignalSpyCfg(TRUE,TRUE,
        HAL_DBG_SIGNAL_SPY_MODE_RF,
        HAL_DBG_SIGNAL_SPY_IRQ_SYS_COM0,
        HAL_DBG_SIGNAL_SPY_IRQ_SYS_FINT,
        HAL_DBG_SIGNAL_SPY_CLK_RF);

#ifdef CHIP_HAS_AP
    syscmds_Open();
#endif

    // Print Traces for modules versions
    hal_MapEnginePrintAllModulesVersion();
}
