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
/*      vpp_pngdec.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*     This is the public header of VPP PNGDEC.                            */
/*                                                                       */
/*************************************************************************/
///
///   @file vpp_pngdec.h
///   This is the public header of VPP PNGDEC.
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
///  VPP PNGDEC is composed of the following division :
///  - @ref vpp_pngdec "vpp public pngdec structures and defines" \n
///

#ifndef VPP_PNGDEC_H
#define VPP_PNGDEC_H

#include "cs_types.h"
#include "hal_voc.h"


///@defgroup vpp_pngdec
///
///  This document describes the characteristics of the VPP PNGDEC module.
///
///  @par configuration :
///  @par
///         The VPP PNGDEC configuration ...
///
///  @par Resource management :
///  @par
///         The VPP PNGDEC module needs to be first opened and configured using function
///         <b> \c vpp_PngDecOpen </b>. When the module is no longer needed it should be closed using
///         <b> \c vpp_PngDecClose </b>. Not closing the module would not free the VoC resource
///         for other applications.
///
///@{

// ============================================================================
// Types
// ============================================================================

// ============================================================================
// VPP_PNGDEC_CFG_T
// ----------------------------------------------------------------------------
/// VPP PNGDEC configuration structure
// ============================================================================

typedef struct
{
 	INT16  mode;
	INT16  reset;

	UINT32  *FileContent;
	UINT32 FileSize;

	UINT32 *PNGOutBuffer;         
	UINT32 OutBufferSize;
    
	UINT32 *AlphaOutBuffer;	
	UINT32 AlphaBufferSize;

	INT32 *BkgrdBuffer;		
	INT16 BkgrdClipX1;	
	INT16 BkgrdClipX2;	
	INT16 BkgrdClipY1;	
	INT16 BkgrdClipY2;	

	INT16 BkgrdOffsetX; 
	INT16 BkgrdOffsetY; 

	INT16 BkgrdWidth;
	INT16 BkgrdHeight;
	
	INT16 ZoomWidth;	
	INT16 ZoomHeight;
	
	INT16 DecMode;
	INT16 alpha_blending_to_transparentColor;


	UINT32 *ImgWidth;
	UINT32 *ImgHeigh;

	UINT32  *Window;


	UINT32 *rgb_temp_ptr;

	UINT32 PngFileHandle;

} VPP_PNGDEC_CFG_T;


// ============================================================================
// VPP_PNGDEC_STATUS_T
// ----------------------------------------------------------------------------
/// VPP PNGDEC status structure
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
    // pngdec status
    // ----------------
    INT32 PngdecFinished;


} VPP_PNGDEC_STATUS_T;


// ============================================================================
// VPP_PNGDEC_MODE_T
// ----------------------------------------------------------------------------
/// VPP PNGDEC code mode enumerator
// ============================================================================

typedef enum
{
	VPP_PNGDEC_MODE_NO = -1,
	VPP_PNGDEC_MODE_HI =  0,
	VPP_PNGDEC_MODE_BYE

} VPP_PNGDEC_MODE_T;

// ============================================================================
// VPP_PNGDEC_WAKEUP_MODE_T
// ----------------------------------------------------------------------------
/// Defines the order in which the tasks are executed an the way these
/// tasks are started
// ============================================================================

typedef enum
{
    /// No wakeup enabled
    VPP_PNGDEC_WAKEUP_NO = 0,
    /// Wake on software event SOF0.
    VPP_PNGDEC_WAKEUP_SW0,
    /// Wake on hardware event IFC0.
    VPP_PNGDEC_WAKEUP_HW0

} VPP_PNGDEC_WAKEUP_MODE_T;


// ============================================================================
// VPP_PNGDEC_ERROR_T
// ----------------------------------------------------------------------------
/// VPP PNGDEC error enumerator
// ============================================================================

typedef enum
{
    /// There is no error
    VPP_PNGDEC_ERROR_NO                  =  0,
    /// There is error
    VPP_PNGDEC_ERROR_YES                 = -1

} VPP_PNGDEC_ERROR_T;


// ============================================================================
// VPP_PNGDEC_SAYS_T
// ----------------------------------------------------------------------------
/// VPP PNGDEC enumerator for what the VPP code said
// ============================================================================

typedef enum
{
    /// There is no error
    VPP_PNGDEC_SAYS_HI                   = 0x0111,
    /// There is error
    VPP_PNGDEC_SAYS_BYE                  = 0x0B1E

} VPP_PNGDEC_SAYS_T;



// ============================================================================
// API Functions
// ============================================================================

//=============================================================================
// vpp_PngDecOpen function
//-----------------------------------------------------------------------------
/// Initialize all VPP PNGDEC code and environment variables. It also set the
/// VoC wakeup masks necessary for the synchronization. 
/// @param vocIrqHandler : user defined handler function for the VoC interrupt.
/// If the function is set to NULL, no interrupts are used (the interrupt mask
/// is not set).
/// @param wakeupMode : Select whether the VoC module wakeup is controlled by 
/// XCPU or by IFC interrupts.
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_PngDecOpen(HAL_VOC_IRQ_HANDLER_T vocIrqHandler,
                               VPP_PNGDEC_WAKEUP_MODE_T wakeupMode);

//=============================================================================
// vpp_PngDecClose function
//-----------------------------------------------------------------------------
/// Close VPP PNGDEC, clear VoC wakeup masks.
//=============================================================================
PUBLIC VOID vpp_PngDecClose(VOID);

//=============================================================================
// vpp_PngDecStatus function
//-----------------------------------------------------------------------------
/// Return the VPP PNGDEC status structure.
/// @param pStatus : result status structure of type VPP_PNGDEC_STATUS_T
//=============================================================================
PUBLIC VOID vpp_PngDecStatus(VPP_PNGDEC_STATUS_T * pStatus);

//=============================================================================
// vpp_PngDecCfg function
//-----------------------------------------------------------------------------
/// @param cfg : configuration (codec mode, reset...)
//=============================================================================
PUBLIC VOID vpp_PngDecCfg(VPP_PNGDEC_CFG_T* pCfg);

//=============================================================================
// vpp_PngDecSchedule function
//-----------------------------------------------------------------------------
/// This function will write a software wakeup event if the wakeup mode is 
/// VPP_PNGDEC_WAKEUP_SW0
/// @return error of type HAL_ERR_T
//=============================================================================
PUBLIC HAL_ERR_T vpp_PngDecSchedule(VOID);



///  @} <- End of the vpp_pngdec group


#endif // VPP_PNGDEC_H
