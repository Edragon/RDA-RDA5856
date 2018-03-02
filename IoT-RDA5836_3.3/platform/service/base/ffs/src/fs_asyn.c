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
// FILENAME: fs_asyn.c
//
// DESCRIPTION:
//   This file to realize some asynchronous file access apis,as asyn reading,asyn writing etc.
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//  Caoxh 	         2008.07.28      Create
//  Caoxh            2008.07.29      fixed the bug of crash for free;clean up the warning.
////////////////////////////////////////////////////////////////////////////////
#ifndef _FS_SIMULATOR_


#include "dsm_cf.h"
#include "fs.h"
#include "fs_asyn.h"
#include "fs_asyn_local.h"
#include "ts.h"
#include "csw_mem_prv.h"
#include "event.h"

HANDLE g_hAsynFsTask = HNULL;

INT32 FS_AsynReadReq(INT32 iFd,UINT8* pBuff,UINT32 uSize,FS_ASNY_FUNCPTR pCallback)
{
	FS_ASYN_EVENT sEv = {0,};	
	FS_ASYN_PARAM  *pParam = NULL;

	//hal_HstSendEvent(0x18500000);    	
	//hal_HstSendEvent(0x18500000+uSize);    	

	if(iFd < 0 || NULL == pBuff || NULL == pCallback)
	{
		return ERR_FS_ASYN_PARA_INVALID;     
	}	
	
	pParam = (FS_ASYN_PARAM*)CSW_FFS_MALLOC(SIZEOF(FS_ASYN_PARAM));	
	if(pParam == NULL)
	{
		return ERR_FS_ASYN_MALLOC_FAILED;
	}
	pParam ->Fd = iFd;
	pParam ->pBuff = (UINT32)pBuff;
	pParam ->uSize = uSize;
	pParam ->pCallback = (UINT32)pCallback;
	
	sEv.nEventId = EV_FS_ASYN_READ_REQ;
	sEv.nParam1 = (UINT32)pParam;
//	sEv.nParam2 = 0;//for delete nParam2
//	sEv.nParam3 = 0;//for delete nParam3

	COS_SendEvent(MOD_SYS, (COS_EVENT*)&sEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
/*tianwq	TASK_HANDLE* pHTask = (TASK_HANDLE*)g_hAsynFsTask;
	if(pHTask)
	{
	    sxr_Send(&sEv, pHTask->nMailBoxId, SXR_SEND_EVT);
	}
	else
	{
	     CSW_FFS_FREE(pParam);
            return ERR_FS_ASYN_TASK_NOT_START;
    }*/
	return ERR_FS_ASYN_SUCCESS;   
}

INT32 FS_AsynWriteReq(INT32 iFd,UINT8* pBuff,UINT32 uSize, FS_ASNY_FUNCPTR pCallback)
{
	FS_ASYN_EVENT sEv = {0,};
	FS_ASYN_PARAM  *pParam = NULL;

	//hal_HstSendEvent(0x18600000);  

	if(iFd < 0 || NULL == pBuff || NULL == pCallback)
	{
		return ERR_FS_ASYN_PARA_INVALID;     
	}	
	
	pParam = (FS_ASYN_PARAM*)CSW_FFS_MALLOC(SIZEOF(FS_ASYN_PARAM));
	if(NULL == pParam)
	{
		return ERR_FS_ASYN_MALLOC_FAILED;
	}
	pParam ->Fd = iFd;
	pParam ->pBuff = (UINT32)pBuff;
	pParam ->uSize = uSize;
	pParam ->pCallback = (UINT32)pCallback;
	
	sEv.nEventId = EV_FS_ASYN_WRITE_REQ;
	sEv.nParam1 = (UINT32)pParam;
//	sEv.nParam2 = 0;//for delete nParam2
//	sEv.nParam3 = 0;//for delete nParam3
	COS_SendEvent(MOD_SYS, (COS_EVENT*)&sEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

/*tianwq	TASK_HANDLE* pHTask = (TASK_HANDLE*)g_hAsynFsTask;
	if(pHTask)
	{
	    sxr_Send(&sEv, pHTask->nMailBoxId, SXR_SEND_EVT);
	}
	else
	{
	     CSW_FFS_FREE(pParam);
            return ERR_FS_ASYN_TASK_NOT_START;
        }*/
	return ERR_FS_ASYN_SUCCESS;   
}

#if 0 // tianwq: merge to fs task
VOID BAL_AsynFsTask(void *pData)
{
	FS_ASYN_EVENT ev = {0,};
//	TASK_HANDLE* pHTask = NULL;

//       pHTask = (TASK_HANDLE*)g_hAsynFsTask;
	SUL_ZeroMemory32(&ev, SIZEOF(FS_ASYN_EVENT) );

	while (1)      
	{
//tianwq		sxr_Wait((UINT32*)&ev, pHTask ->nMailBoxId);
		switch(ev.nEventId)
		{
		case FS_ASYN_EVENT_ID_READ_REQ: 
			FS_AsynReadPrc(ev.nParam1);
			break;
		case FS_ASYN_EVENT_ID_WRITE_REQ: 
			FS_AsynWritePrc(ev.nParam1);
			break;
		default:
			break;
		}
		CSW_FFS_FREE((void*)ev.nParam1);
		//sxr_Sleep(10);
	}
	return;
}
#endif

VOID FS_AsynReadPrc(UINT32 pParam1)
{
	FS_ASYN_READ_REQ* pReq = NULL;  
	FS_ASYN_READ_RESULT  sRsp;  
	INT32 iResult = 0;

	if(0 == pParam1)
	{		
		return;
	}

	pReq = (FS_ASYN_READ_REQ*)pParam1;
	iResult = FS_Read(pReq->iFd,pReq->pBuff,pReq->uSize);
	if(iResult >= 0)
	{
		sRsp.iResult = ERR_SUCCESS;
		sRsp.pBuff = pReq->pBuff;
		sRsp.uSize = iResult;
	}
	else
	{
	// mmi_trace(TRUE,"FS_Read() Filed!iResult = %d.",iResult);
		sRsp.iResult = iResult;
		sRsp.pBuff = pReq->pBuff;
		sRsp.uSize = 0;
	}  
	//hal_HstSendEvent(sRsp.uSize);
	pReq->pCBFunc(&sRsp);	
	CSW_FFS_FREE(pReq);
	return;
}

VOID FS_AsynWritePrc(UINT32 pParam1)
{
	FS_ASYN_WRITE_REQ* pReq = NULL;  
	FS_ASYN_WRITE_RESULT  sRsp = {0,};  
	INT32 iResult;

	if(0 == pParam1)
	{		
		return;
	}

	pReq = (FS_ASYN_WRITE_REQ*)pParam1;

	iResult = FS_Write(pReq->iFd,pReq->pBuff,pReq->uSize);
	if(iResult >= 0)
	{    
		sRsp.iResult = ERR_SUCCESS;
		sRsp.pBuff = pReq->pBuff;
		sRsp.uSize = iResult;
	}
	else
	{
	// mmi_trace(TRUE,"FS_Write() Filed!iResult = %d.", iResult);
		sRsp.iResult = iResult;
		sRsp.pBuff = pReq->pBuff;
		sRsp.uSize = 0;    
	}  

	pReq->pCBFunc(&sRsp);
	CSW_FFS_FREE(pReq);
	return;
}

#endif


