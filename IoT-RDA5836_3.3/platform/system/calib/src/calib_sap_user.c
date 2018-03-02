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
#include "sxs_srl.h"
#include "sxs_rmt.h"
#include "sxs_lib.h"
#include "sxs_io.h"
#include "sxr_ops.h"
#include "sxs_spy.h"
#include "sxs_rmc.h"
#include "hal_debug.h"
#define __SAP_CFG_VAR__
#include "calib_sap_cfg.h"
#undef __SAP_CFG_VAR__

/*
==============================================================================
   Function   : calib_SapInitUsr
 ----------------------------------------------------------------------------
   Scope      :
   Parameters : None.
   Return     : None.
==============================================================================
*/

VOID calib_SapInitUsr(VOID)
{
    //------------------------
    // Create OS Malloc clusters
    //------------------------
//    sxr_NewCluster (60, 175);
//    sxr_NewCluster (220, 255);
//    // sxr_NewCluster (550, 22);
//    sxr_NewCluster (640, 90);
//    sxr_NewCluster (1600, 8);
//    // sxr_NewCluster (6000, 3);
//    sxr_NewCluster (7000, 8);

    // -----------------------------
    // Setup default trace levels
    // -----------------------------
    sxs_IoCtx.TraceBitMap [TGET_ID(_APP)] |= 0xFFFF;


    // Create User Task
    SXS_TRACE(TSTDOUT,"creating test task ");
    g_calibStubTaskId = sxr_NewTask(&g_calibSxsCalibStubTaskDesc);
    sxr_StartTask(g_calibStubTaskId,NULL);
    g_calibStubMbx = sxr_NewMailBox();
}

