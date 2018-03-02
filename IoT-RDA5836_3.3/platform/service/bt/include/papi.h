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
/*
 * MODULE NAME:    papi.h
 * DESCRIPTION:    PAPI Operating System Independent Header
 * AUTHOR:         Martin Mellody
 * DATE:           10 March 2000
 *
 * SOURCE CONTROL: $Id: papi.h,v 1.8 2009/08/25 05:53:21 jianguo Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000 RDA Microelectronics plc.
 *     All rights reserved.
 *
 */

#ifndef RDABT_PAPI_H
#define RDABT_PAPI_H

#include <stdarg.h>

/* 
	OS specific stuff is in here. The build mechanism 
	must make sure the correct one of these is found.
*/
#include "papi_impl.h"	

/* Error return codes */
#define pENOERROR		0 /* must be zero */
#define pENOMEMORY		1
#define pENORESOURCE	2
#define pEINVALIDPARM	3
#define pEWOULDBLOCK	4
#define pELOCKERROR		5
#define pEIOERROR		6	
#define pENOTSUPPORTED	7
#define pEUNKNOWNERROR	8

/* Logging levels for pDebugPrint */
#define pLOGFATAL		0
#define pLOGCRITICAL	1
#define pLOGERROR		2
#define pLOGWARNING	3
#define pLOGNOTICE		4
#define pLOGDEBUG		5

/*Logging layers for pDebug */
#define pLOGSYSTEM 1     /*0*/
#define pLOGHCI 2           /*1*/
#define pLOGMANAGER 4   /*2*/
#define pLOGL2CAP 8     /*3*/
#define pLOGRFCOMM 16   /*4*/
#define pLOGSDP 32  /*5*/
#define pLOGOBEX 64   /*6*/
#define pLOGTCS 128   /*7*/
#define pLOGA2DP 256  /*8*/
#define pLOGHFP    512 /*9*/
#define pLOGADP    1024 /*10*/
#define pLOGAMP 2048  /*11*/
#define pLOGPAL 4096  /*12*/
#define pLOGHCRP 8192  /*13*/
#define pLOGALWAYS 0xffff

#define LOG_EVENT_STRING_L2_PSM_INDEX  0   /*13*/
#define LOG_EVENT_STRING_L2_CONFIG_SM_INDEX 13

/* the start index num for each file*/

#define LOG_L2CAP_L2CAP_PRIMITIVES_INDEX   0  /* 1*/
#define LOG_L2CAP_L2_CONFIG_SM_INDEX          1  /*4*/
#define LOG_L2CAP_L2_CONNCECT_SM_INDEX     5 /*11*/
#define LOG_L2CAP_L2_ENC_DEC_INDEX             16/*29*/
#define LOG_L2CAP_L2_GROUPS_INDEX               45 /*5*/
#define LOG_L2CAP_L2_PKT_HANDLER_INDEX      50/*1*/  
#define LOG_L2CAP_L2_PSM_INDEX      51/*8*/                
#define LOG_L2CAP_L2_SAR_INDEX    59 /**/  

#define LOG_SYS_HOST_TIME_INDEX   0  /*3*/
#define LOG_SYS_PAPI_WIN32_INDEX 3 /*14*/  

#define LOG_A2TP_AVDTP_CALLBACKS_INDEX 0 /*5*/
#define LOG_A2TP_AVRCP_CALLBACKS_INDEX 5 /*2*/
#define LOG_A2TP_AVRCP_CORE_INDEX 7 /*3*/

#define LOG_ALWAYS_AVRCP_CORE_INDEX 0 /*3*/
#define LOG_ALWAYS_PAPI_WIN32_INDEX 3  /*1*/
#define LOG_ALWAYS_RFCOMM_UNIT  4 /*1*/

#define LOG_RFC_RFCOMM_CTRL_CHAN_INDEX 0 /*7*/
#define LOG_RFC_RFCOMM_DLC_STATE_INDEX 7  /*11*/
#define LOG_RFC_RFCOMM_FRAMES_INDEX 18 /*3*/
#define LOG_RFC_RFCOMM_L2AUTO_STATE_INDEX  21  /*4*/
#define LOG_RFC_RFCOMM_L2CAP_INDEX 25 /*1*/
#define LOG_RFC_RFCOMM_UE_API_INDEX    26/*3*/
#define LOG_RFC_STACK_ENT_INDEX  29/*1*/


#define LOG_SDP_SDP_CALLBACKS_INDEX 0/*6*/
#define LOG_SDP_SDP_DB  6/*1*/
#define LOG_SDP_SDP_LE_REQUEST_DECODER   7/*3*/
#define LOG_SDP_SDP_LE_RESPONSE_DECODER  10/*4*/
#define LOG_SDP_UPF_5  14 /*2*/

#define pTIMER_ONESHOT 1
#define pTIMER_PERIODIC 2

#define PAPI_EVENT_NUM		16

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define pNULL NULL

/* This should be used in code and provided where available for specific patforms */
#ifndef pINLINEFUNC
#define pINLINEFUNC		
#endif

/* if the functions are not defined, ignore it */
#if !pDEBUG
#define pDebugPrintfEX(args)                        
#define pDebugDumpEX(level,layer,buf,len)           
#define pDebugPrintf(args)                          
#define pDebugDump(level,buf,len)                   
#define pDebugCheck()                               
#endif

#ifndef  pAssert
    #define  pAssert(exp)                               (0)
#endif

#ifndef  pSleep
    #define  pSleep(millisec)                           (0)
#endif

#ifndef pWakeUpScheduler
    #define  pWakeUpScheduler()                         (0)
#endif

#ifndef pMutexCreate
    #define  pMutexCreate(initState)                    (0)
#endif

#ifndef pMutexFree
    #define  pMutexFree(mutex)                          (0)
#endif

#ifndef pMutexLock
    #define  RDABT_MUTEX_UNSUPPORT
    #define  pMutexLock(mutex)                          (0)
#endif

#ifndef pMutexUnlock
    #define  pMutexUnlock(mutex)                        (0)
#endif

#ifndef pEnterCriticalSection
    u_int32  pEnterCriticalSection();
#endif

#ifndef pExitCriticalSection
    void  pExitCriticalSection(u_int32 status);
#endif

#ifndef  pGetCurrentTicks
    u_int32 pGetCurrentTicks(void);
#endif

#ifndef  pGetTickDifference
    u_int32 pGetTickDifference(u_int32 start, u_int32 end);
#endif

#ifndef pMalloc
	void *pMalloc(u_int32 size);
#endif

#ifndef pFree
	void pFree(void *ptr);
#endif

#ifndef pMemcpy
	void *pMemcpy(void *dest, void *src, size_t numbytes);
#endif

#ifndef pMemset
	void *pMemset(void *buf, int byte, size_t numbytes);
#endif

#ifndef pMemcmp
	int pMemcmp(void *buf1, void *buf2, size_t numbytes);
#endif

#ifndef pMemchr
	void *pMemchr(void *buf, int byte, size_t numbytes);
#endif

#ifndef pStrlen
	size_t pStrlen(const char *s);
#endif

#ifndef pStrncmp
	int pStrncmp(const char *s1, const char *s2, size_t n);
#endif

#ifndef pStrcmp
	int pStrcmp(const char *s1, const char *s2);
#endif

#ifndef pStrcpy
	char *pStrcpy(char *s1, const char *s2);
#endif

#ifndef pStrncpy
	char *pStrncpy(char *s1, const char *s2, size_t n);
#endif


#ifndef pSendEvent
	int pSendEvent(u_int16 event_id, u_int8 *msg);
#endif


#endif	/* PAPI_H */

