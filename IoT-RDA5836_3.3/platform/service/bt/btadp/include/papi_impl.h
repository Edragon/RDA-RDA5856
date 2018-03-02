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
/***********************************************************************
 *
 * MODULE NAME:    papi_impl.h
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    PAPI eCos specific include
 * MAINTAINER:     John Sheehy
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: papi_impl.h,v 1.1 2008/12/18 01:53:15 tianwq Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 RDA Microelectronics Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/
#ifndef PAPI_IMPL_H
#define PAPI_IMPL_H

#define pNULL NULL

#ifndef __CS_TYPE_H__
#ifndef __size_t
#define __size_t   1
typedef u_int32  size_t;
#endif
#endif
typedef u_int32  t_pMutex;
typedef u_int32  t_pTimer;
typedef u_int32  t_pEvent;

#if MEMORY_POOL_SUPPORT==0
#ifndef _COS_H_
void  *COS_Malloc (u_int32 nSize);
void *COS_Malloc_NoCache (u_int32 nSize);
int COS_Free ( void *pMemBlock);
#endif
#define pMalloc COS_Malloc
#define pFree COS_Free
#endif

extern void * memcpy(void *, const void *, size_t);
extern void * memset(void *, int,  size_t);
extern int memcmp(const void *, const void *, size_t);
extern void * memchr(const void *, int,  size_t);
extern int strcmp(const char *, const char *);
extern int strncmp(const char *, const char *, size_t);
extern size_t strlen(const char *);
extern char * strcpy(char *, const char *);
extern char * strncpy(char *, const char *, size_t);


#define pMemcpy memcpy
#define pMemset memset
#define pMemcmp memcmp
#define pMemchr memchr
#define pStrcmp strcmp
#define pStrncmp strncmp
#define pStrlen strlen
#define pStrcpy strcpy
#define pStrncpy strncpy

/* platform timer is no used in mtk */
#define pSetTimer(millisec)       (0)
#define pChangeTimer(id, millisec)       (0)
#define pCancelTimer(id)       (0)
#define pGetCurrentTicks()              (0)
#define pGetTickDifference(start, end)      (0)

#define pWakeUpScheduler()              rdabt_send_notify()

 
#if CHIP_HAS_BTCPU == 1
#define  pAssert(exp)   do {\
                                            if(exp==NULL) \
                                                SYS_ASSERT2();\
                                        } while(0)
                                            
#elif !defined(WIN32)
extern void hal_DbgAssert(const char* format, ...);
#define  pAssert(exp)   do {\
                                            if(exp==NULL) \
                                                hal_DbgAssert("bt_host");\
                                        } while(0)
                                        
#endif

#ifndef CT_NO_DEBUG    
#define pDebugPrintfEX(args)                 (RDA_Debug_PrintEX args)
#define pDebugPrintf(args)                   (RDA_Debug_Print args)
#define pDebugDumpEX(level,layer,buf,len)    (pDebugDumpFuncEX((level),(layer),(buf),(len)))
#define pDebugDump(level,buf,len)            (pDebugDumpFunc((level),(buf),(len)))
#define pDebugCheck()                        (pDebugCheckFunc())
#endif


#define  RDABT_MUTEX_UNSUPPORT
#define  pMutexCreate(initState)        (0)
#define  pMutexFree(mutex)              
#define  pMutexLock(mutex)               
#define  pMutexUnlock(mutex)             
#define pEnterCriticalSection()				RDABT_Enter_CriticalSection()
#define pExitCriticalSection(status)		RDABT_Exit_CriticalSection(status)




void rdabt_send_notify(void);
int RDA_Debug_PrintEX(int logLevel,int logLayer, char *formatString, ...) ;
int RDA_Debug_Print(int logLevel, char *formatString, ... );
int pDebugDumpFunc(int logLevel, u_int8 *buffer, u_int32 len);
int pDebugDumpFuncEX(int logLevel, int logLayer, u_int8 *buffer, u_int32 len) ;
int pDebugCheckFunc(void) ;

u_int32 RDABT_Enter_CriticalSection();
void RDABT_Exit_CriticalSection(u_int32 status);

#ifndef CT_NO_DEBUG    
void SYS_SendEvent2(u_int32 event);
void SYS_SendEvent(u_int32 event);
#else
#define SYS_SendEvent2(event)
#define SYS_SendEvent(event)
#endif
#define SYS_ASSERT2()
#define SYS_ASSERT()


#endif /* PAPI_IMPL_H */
