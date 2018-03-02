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
//
/// @file mcip_media.h
///
/// MCI internal media variable
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCIP_CAMERA_H_
#define _MCIP_CAMERA_H_

#include "cs_types.h"
#include "lcdd_m.h"


// =============================================================================
// MACROS                                                                       
// =============================================================================
#define MCI_CAM_MAX_PREVIEW_SIZE (320*240)
#define MCI_CAM_MAX_CAPTURE_SIZE (640*480)

// =============================================================================
// TYPES                                                                        
// =============================================================================

// =============================================================================
// MCI_CAMERA_INFO_STRUCT_T
// -----------------------------------------------------------------------------
/// Type used to store some useful information about the camera settings.
/// Nota: TODO We should be able, in a certain measure, to get rid of it.
// =============================================================================
typedef struct
{
    UINT16  startX;
	UINT16  startY;
	UINT16  endX;
	UINT16  endY;
	UINT16  imageWidth;
	UINT16  imageHeight;
	UINT16  screenWidth;
	UINT16  screenHeight;	
	UINT32  previewZoom;
	INT32   imageQuality;
	BOOL    addFrame;
} MCI_CAMERA_INFO_STRUCT_T;


// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================


// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================
PRIVATE VOID MCI_CamSaveOsd(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy);
PRIVATE VOID mci_CameraBlendOsd(LCDD_FBW_T* previewFbw, LCDD_FBW_T* osdFbw);
PRIVATE VOID mci_CameraPreviewHandler(LCDD_FBW_T* previewFbw);
PRIVATE VOID mci_CameraCaptureHandler(LCDD_FBW_T* dataFbw, LCDD_FBW_T* previewFbw);



#endif // _MCIP_CAMERA_H_
