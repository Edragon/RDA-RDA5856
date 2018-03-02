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
// FILENAME: mmc_timer.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   shenh           20090721        create
//
////////////////////////////////////////////////////////////////////////////////
#ifndef MMC_TIMER_H
#define MMC_TIMER_H

#include "cos.h"

typedef enum
{
	/*
	** Start for for mmc timer.
	** Total 256 IDS.
	*/
	MMC_TIMER_ID_NONE=COS_MDI_TIMER_ID_BASE, /* Imp: Please do not modify this */ 
	
  	MMC_VID_PLY_TIMER_ID,
  	MMC_VID_REC_TIMER_ID,
	MMC_CAMERA_TIMER_ID,
	MMC_AUD_PLY_TIMER_ID,
	MMC_AUD_REC_TIMER_ID,
	    
	MMC_MAX_TIMERS=COS_MDI_TIMER_ID_END_ /* Imp: Please do not modify this */ 	 
}MMC_TIMER_IDS;

#endif


