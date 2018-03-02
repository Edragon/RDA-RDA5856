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


// REVISION HISTORY:
// NAME              DATE                REMAKS
// Chenhe      2010-3-2       Created for pinrt trace to t-card
// 
// //////////////////////////////////////////////////////////////////////////////
#if 0
#include "sxs_cfg.h"
#include "hal_timers.h"
#include "itf_msgc.h"
#include "fs.h"
#include "cfw.h"
#include "csw.h"
#include "fstraces.h"
#include "sxs_io.h"
#include "cs_types.h"
#include "dbg.h"
#include "hal_host.h"
#include "base_prv.h"
#include "dsm_cf.h"
#include "bal_fstrace.h"

// global variable

INT32 g_tracefileHandle;

UINT8 *g_trace_Data = NULL;

PRIVATE INT32 _fsTraceFileOpen(CONST CHAR *name,  UINT32 iFlag, UINT32 iMode)
{
    INT32 result;
    INT32 fd = -1;
    UINT32 uni_len = 0;
    UINT8* uni_name = NULL;                
    if((result = ML_LocalLanguage2UnicodeBigEnding(name,  DSM_StrLen(name),&uni_name,&uni_len,NULL) )!= ERR_SUCCESS){
	if(uni_name){
	    CSW_Free(uni_name);
	    uni_name = NULL;
	}                       
	CSW_TRACE(BASE_BAL_TS_ID,"fstrace:%s:%u:ML_LocalLanguage2UnicodeBigEnding ret %d",__FUNCTION__, __LINE__,result);
	return -1;
    }
    
    fd = FS_Open(uni_name,iFlag,iMode);
    
    if(uni_name){
        CSW_Free(uni_name);
        uni_name = NULL;
    }
    
    return fd;
            
}


PRIVATE INT32 _fsTraceFileClose(INT32 fd)
{
     if(fd < 0)
     {
        CSW_TRACE(BASE_BAL_TS_ID,"%s:%d: 0x%x not a FILE pointer",__FUNCTION__, __LINE__, fd);
        return -1;
     }
     return FS_Close(fd);
}

// =============================================================================
// _fsTraceSetLevel
// -----------------------------------------------------------------------------
/// set trace level
// =============================================================================
PRIVATE VOID _fsTraceSetLevel(void)
{
	UINT16 traceLevel[SXS_NB_ID] = {0, };

	UINT16 cnt                         = 0;
	UINT8 targetTraceLevelFileName[30] = "/t/csd_fstracelevel.tfg";

	INT32 iResult                      = -1;
	INT32 fhd;
	UINT8 tmpTransfer = 0,*tmpTransfer1;
	traceLevel[__MMI] = 0xffff;
	traceLevel[__CSW] = 0xffff;

	CSW_TRACE(BASE_BAL_TS_ID, TSTXT("fsTrace_setTraceLevel OK.%d,%d\n"), __MMI, SXS_NB_ID);

	if((fhd =_fsTraceFileOpen(targetTraceLevelFileName, FS_O_RDONLY, 0)) >= 0)
	{
		iResult = FS_Read(fhd, (UINT8*)(traceLevel), SXS_NB_ID* 2);

		CSW_TRACE(BASE_BAL_TS_ID, TSTXT("fsTrace_setTraceLevel read result is %d\n"), iResult);

		cnt = 0;
		while (cnt < SXS_NB_ID)
		{

			/******CONVERT H8 AND L8,START ****/
			tmpTransfer1 = (UINT8*)(&traceLevel[cnt]);
			tmpTransfer =*tmpTransfer1;
			*tmpTransfer1 = *(tmpTransfer1+1);
			*(tmpTransfer1+1) = tmpTransfer;
			/******CONVERT H8 AND L8, END ****/

			CSW_TRACE(BASE_BAL_TS_ID, TSTXT("fstraces_setLevel(0x%x 0x%x"), TID(cnt), traceLevel[cnt]);
			fstraces_setLevel(TID(cnt), traceLevel[cnt]);
			++cnt;
		}

		iResult = _fsTraceFileClose(fhd);
		CSW_TRACE(BASE_BAL_TS_ID, TSTXT("fsTrace_setTraceLevel close result is %d\n"), iResult);

	}
	else  // set default level
	{
		while (cnt < SXS_NB_ID)
		{
			fstraces_setLevel(TID(cnt), traceLevel[cnt]);
			++cnt;
		}
	}

}

// =============================================================================
// bal_fsTraceProcess
// -----------------------------------------------------------------------------
/// Read a bunch of data from the debug remote buffer and output to tFlash card.
// =============================================================================
PUBLIC VOID bal_fsTraceProcess(VOID)
{
  INT32 trace_len, result = 0;
  
//  hal_HstSendEvent(199);    
  
  if(g_trace_Data == NULL){
      return;
  }
  
  do
  {
	trace_len = fstraces_Read(g_trace_Data, MAX_TRACE_LEN);
	if ((trace_len > 0) && (g_tracefileHandle >= 0))
	{
		result = FS_Write(g_tracefileHandle, g_trace_Data, trace_len);
		if(result == ERR_FS_DISK_FULL)
		{
			FS_Truncate(g_tracefileHandle,0);
		}
		else if(result<0)
		{
			bal_fstraceStop();
		}
	}
  } while (trace_len > 0);

  COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_CFW_TRACE_TO_FLASH_TIMER_ID, COS_TIMER_MODE_SINGLE,FS_TRACE_TIMER_CNT); 
    
}

// =============================================================================
// bal_fstraceStart
// -----------------------------------------------------------------------------
/// start tFlash trace recorsing
// =============================================================================
PUBLIC VOID  bal_fstraceStart(VOID)
{
  UINT8 targetTraceName[20] = "/t/fstrace.bin";

  if(g_trace_Data != NULL){
      CSW_TRACE(BASE_BAL_TS_ID, TSTXT("bal_fstraceStart:fs trace already opened!"));        	
      return;
  }

  if((g_tracefileHandle = _fsTraceFileOpen(targetTraceName, FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC, 0)) < 0){
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("bal_fstraceStart:file %s open fail, ret %x"),targetTraceName,  g_tracefileHandle);    
        return;
    }

  if((g_trace_Data = CSW_Malloc(MAX_TRACE_LEN)) == NULL){
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("bal_fstraceStart:malloc fail!"));      	
  	_fsTraceFileClose(g_tracefileHandle);
	return;
  }
  
  _fsTraceSetLevel();  
  
  fstraces_Open();

  COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_CFW_TRACE_TO_FLASH_TIMER_ID, COS_TIMER_MODE_SINGLE,FS_TRACE_TIMER_CNT); 
  
}


// =============================================================================
// bal_fstraceStop
// -----------------------------------------------------------------------------
/// stop tFlash trace recorsing
// =============================================================================
PUBLIC VOID  bal_fstraceStop(VOID)
{
  
  fstraces_Close();
  COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_CFW_TRACE_TO_FLASH_TIMER_ID);
  _fsTraceFileClose(g_tracefileHandle);
  CSW_Free(g_trace_Data);
  g_trace_Data = NULL;
}

#endif
