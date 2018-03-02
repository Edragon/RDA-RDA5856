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
//
// FILENAME: record_api.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"
#include "cpu_share.h"
#include "mcip_debug.h"

#include "mmc.h"

//#include "cpu_sleep.h"
#include "lily_statemachine.h"

#include "mmc_fmrecorder.h"
uint32 LILY_FmrRecordStart (int8 *file_name_p) 	//	MCI_MEDIA_PLAY_REQ,			
{  
	int32 result;
	//int32 recordtype;

	diag_printf("[FMR_RECORD_API]: CALL - LILY_FmrRecordStart\n");    
	if (STA_RES_FAIL == lily_changeToState(STM_MOD_FMREC))
  {
    diag_printf("Error state!!!!!!!!!!!in bmpshow\n");
    return -1;
  }

    mmc_SetCurrMode(MMC_MODE_FM_RECORD);
	result = FmrRecordStart(0, 0, file_name_p);
	mmc_SetModeStatus(AudioRecordStatus);
	
    if (result > 0)
    {
		mmc_SetCurrMode(MMC_MODE_IDLE);
    }
	return result;
}

uint32 LILY_FmrRecordPause(void)
{
	diag_printf("[FMR_RECORD_API]: CALL - LILY_FmrRecordPause\n"); 	  
	
	FmrRecordPause();
	mmc_SetModeStatus(AudioRecordPauseStatus);
	return 0;
	//mmc_SendResult(nEvent,1);
}

uint32 LILY_FmrRecordResume(void)
{
	diag_printf("[FMR_RECORD_API]: CALL - LILY_FmrRecordResume\n");	
	
	FmrRecordResume();
	mmc_SetModeStatus(AudioRecordStatus);
	return 0;
//	mmc_SendResult(nEvent,1);

}

uint32 LILY_FmrRecordStop(void)
{
	diag_printf("[FMR_RECORD_API]: CALL - LILY_FmrRecordStop\n");
	
	FmrRecordStop();
	mmc_SetModeStatus(AudioRecordStopStatus);
	mmc_SetCurrMode(MMC_MODE_IDLE);
  lily_exitModule(STM_MOD_FMREC);
  return 0;
	//mmc_SendResult(nEvent,1);
}


