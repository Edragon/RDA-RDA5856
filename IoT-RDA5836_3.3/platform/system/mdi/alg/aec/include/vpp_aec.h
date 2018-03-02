/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      vpp_aec.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is the public header of VPP AEC.                            */
/*                                                                       */
/*************************************************************************/
///
///   @file vpp_aec.h
///   This is the public header of VPP AEC.
///
///
///  @mainpage Granite VOC Processing Control API
///  Granite VOC Processing Control (VPP) API description.

///  @version 1.0
///  @date    oct 2008
///
///  @par Purpose
///
///  This document describes the Granite software VPP layer.
///
///  @par
///
///  VPP AEC is composed of the following division :
///  - @ref vpp_aec "vpp public aec structures and defines" \n
///

#ifndef VPP_AEC_H
#define VPP_AEC_H

#include "cs_types.h"
#include "hal_voc.h"


///@defgroup vpp_aec
///
///  This document describes the characteristics of the VPP AEC module.
///
///  @par configuration :
///  @par
///         The VPP AEC configuration ...
///
///  @par Resource management :
///  @par
///         The VPP AEC module needs to be first opened and configured using function
///         <b> \c vpp_AECOpen </b>. When the module is no longer needed it should be closed using
///         <b> \c vpp_AECClose </b>. Not closing the module would not free the VoC resource
///         for other applications.
///
///@{

// ============================================================================
// Types
// ============================================================================

// ============================================================================
// VPP_AEC_CFG_T
// ----------------------------------------------------------------------------
/// VPP AEC configuration structure
// ============================================================================

typedef struct
{
 	INT16  mode;
	INT16  reset;

} VPP_AEC_CFG_T;


// ============================================================================
// VPP_AEC_STATUS_T
// ----------------------------------------------------------------------------
/// VPP AEC status structure
// ============================================================================

//struct of output parameters
typedef struct
{
    // ----------------
    // common status
    // ----------------
	INT16 mode;
	INT16 errorStatus;

    // ----------------
    // aec status
    // ----------------
    INT32 aecStatus;
    
} VPP_AEC_STATUS_T;

typedef struct {
	INT32* inStreamMICBufAddr;         //Input stream MIC buffer address
	INT32* inStreamSPKBufAddr;         //Input stream SPK buffer address
	INT32* outStreamMICBufAddr;         //Input stream MIC buffer address
	INT16 length;				 // length of stream buf 

} vpp_AEC_IN_T;

// ============================================================================
// VPP_AEC_MODE_T
// ----------------------------------------------------------------------------
/// VPP AEC code mode enumerator
// ============================================================================

typedef enum
{
	VPP_AEC_MODE_NO = -1,
	VPP_AEC_MODE_HI =  0,
	VPP_AEC_MODE_BYE

} VPP_AEC_MODE_T;

// ============================================================================
// VPP_AEC_WAKEUP_MODE_T
// ----------------------------------------------------------------------------
/// Defines the order in which the tasks are executed an the way these
/// tasks are started
// ============================================================================

typedef enum
{
    /// No wakeup enabled
    VPP_AEC_WAKEUP_NO = 0,
    /// Wake on software event SOF0.
    VPP_AEC_WAKEUP_SW0,
    /// Wake on hardware event IFC0.
    VPP_AEC_WAKEUP_HW0

} VPP_AEC_WAKEUP_MODE_T;


// ============================================================================
// VPP_AEC_ERROR_T
// ----------------------------------------------------------------------------
/// VPP AEC error enumerator
// ============================================================================

typedef enum
{
    /// There is no error
    VPP_AEC_ERROR_NO                  =  0,
    /// There is error
    VPP_AEC_ERROR_YES                 = -1

} VPP_AEC_ERROR_T;


// ============================================================================
// VPP_AEC_SAYS_T
// ----------------------------------------------------------------------------
/// VPP AEC enumerator for what the VPP code said
// ============================================================================

typedef enum
{
    /// There is no error
    VPP_AEC_SAYS_HI                   = 0x0111,
    /// There is error
    VPP_AEC_SAYS_BYE                  = 0x0B1E

} VPP_AEC_SAYS_T;



// ============================================================================
// API Functions
// ============================================================================

//=============================================================================
// vpp_AECOpen function
//-----------------------------------------------------------------------------
/// Initialize all VPP AEC code and environment variables. It also set the
/// VoC wakeup masks necessary for the synchronization. 
/// @param vocIrqHandler : user defined handler function for the VoC interrupt.
/// If the function is set to NULL, no interrupts are used (the interrupt mask
/// is not set).
/// @param wakeupMode : Select whether the VoC module wakeup is controlled by 
/// XCPU or by IFC interrupts.
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_AECOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler);

//=============================================================================
// vpp_AECClose function
//-----------------------------------------------------------------------------
/// Close VPP AEC, clear VoC wakeup masks.
//=============================================================================
PUBLIC VOID vpp_AECClose(VOID);

//=============================================================================
// vpp_AECStatus function
//-----------------------------------------------------------------------------
/// Return the VPP AEC status structure.
/// @param pStatus : result status structure of type VPP_AEC_STATUS_T
//=============================================================================
PUBLIC VOID vpp_AECGetStatus(VPP_AEC_STATUS_T * pStatus);

//=============================================================================
// vpp_AECScheduleOneFrame function
//-----------------------------------------------------------------------------
/// This function will write a software wakeup event if the wakeup mode is 
/// VPP_AEC_WAKEUP_SW0
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_AECScheduleOneFrame(VPP_AEC_CFG_T *pVoc_ACE_CFG, vpp_AEC_IN_T *pVoc_ACEIN);

///  @} <- End of the vpp_aec group


#endif // VPP_AEC_H
