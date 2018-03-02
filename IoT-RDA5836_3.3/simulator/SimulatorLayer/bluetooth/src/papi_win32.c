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
 * MODULE NAME:    papi_uitron.c
 * PROJECT CODE:   host side stack
 * DESCRIPTION:    PAPI for UITRON port
 * MAINTAINER:     John Sheehy
 * CREATION DATE:  1 May 2000
 *
 * SOURCE CONTROL: $Id: papi_mtk.c,v 1.3 2008/12/30 09:58:42 tianwq Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2001 Parthus Technologies Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1.May.2000 - JS - first import
 *   03.Jul.2000 - DA - Updated with timer/semaphores
 *
 *
 * ISSUES:
 *       
 ***********************************************************************/

//#include "project_config.h"
#include "cos.h"
#include "bt.h"
#include "papi.h"
#include "rdabt_main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void RDABT_Platform_Init(void)
{
}


#if pDEBUG
#include "ap_common.h"

static const char* LOGLAYER[]=
{
    "pLOGSYSTEM",
    "pLOGHCI",
    "pLOGMANAGER",
    "pLOGL2CAP",
    "pLOGRFCOMM",
    "pLOGSDP",
    "pLOGOBEX",
    "pLOGTCS",
    "pLOGA2DP",
    "pALWAYS"
};

static const char* LOGLEVEL[]=
{
//    "pLOGDATA",	
    "pLOGFATAL",
    "pLOGCRITICAL",
    "pLOGERROR",	
    "pLOGWARNING",
    "pLOGNOTICE",
    "pLOGDEBUG"	
};


UINT8 LogLayer_Encode(UINT32 loglayer)
{
    UINT8 i=0;
    if(loglayer==0)
        return 0;//error
    if(pLOGALWAYS == loglayer)
        return 9;
    while(loglayer)
    {
         loglayer=loglayer>>1;
         i++;
    }        
    return (i-1);
    
}

int RDA_Debug_PrintEX(int logLevel,int logLayer, char *formatString, ... ) 
{
    va_list args;
    char buff[512];
    char buff2[1024];

	//if(pLOGHCI==logLayer)
	//    return RDABT_NOERROR;
	if(pLOGSYSTEM==logLayer)
		return RDABT_NOERROR;
	if(pLOGA2DP==logLayer)
		return RDABT_NOERROR;
	//if(pLOGMANAGER==logLayer)
	//	return RDABT_NOERROR;
	//if(pLOGSDP==logLayer)
	//return RDABT_NOERROR;
	//if(pLOGRFCOMM==logLayer)
		//return RDABT_NOERROR;
	//if(pLOGL2CAP==logLayer)
		//return RDABT_NOERROR;
	//if(pLOGDEBUG==logLevel)
	//	return RDABT_NOERROR;

    va_start(args,formatString);
    vsprintf(buff, formatString, args);
    va_end(args);
  
    sprintf(buff2,"[BT][%s]:%s:%s\n", LOGLAYER[LogLayer_Encode(logLayer)],LOGLEVEL[logLevel], buff);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),buff2,strlen(buff2),NULL,NULL);
    return RDABT_NOERROR;
}

int RDA_Debug_Print(int logLevel, char *formatString, ... )
{
    va_list args;
    char buff[512];
    char buff2[1024];

    va_start(args,formatString);
    vsprintf(buff, formatString, args);
    va_end(args);

    sprintf(buff2,"[BT][%s]:%s:%s\n", LOGLAYER[9], LOGLEVEL[logLevel], buff);
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),buff2,strlen(buff2),NULL,NULL);
    return RDABT_NOERROR;
}

int pDebugDumpFunc(int logLevel, u_int8 *buffer, u_int32 len)
{
  return pDebugDumpFuncEX(logLevel,pLOGALWAYS,buffer,len);
}

int pDebugDumpFuncEX(int logLevel, int logLayer, u_int8 *buffer, u_int32 len) 
{
	u_int8 printBuffer[300];
	u_int32 err,i,j,printBufferOffset,bytesThisLine,numLinesPrinted=0;

    return pENOERROR;
    
/*  This sets the number of bytes to dump per line - usually 0x10 or 0x08 */
#define PAPI_DUMP_BYTESPERLINE 0x10 

	do {
		printBufferOffset=0;
		bytesThisLine=len-numLinesPrinted*PAPI_DUMP_BYTESPERLINE;
		if (bytesThisLine>PAPI_DUMP_BYTESPERLINE)
			bytesThisLine=PAPI_DUMP_BYTESPERLINE;
		for (i=0; i<bytesThisLine; i++) {
			printBuffer[printBufferOffset]=(((buffer[i+numLinesPrinted*PAPI_DUMP_BYTESPERLINE])&0xF0)>>4)+48;
			printBuffer[printBufferOffset]+=printBuffer[printBufferOffset]>57?7:0;
			printBufferOffset++;
			printBuffer[printBufferOffset]=((buffer[i+numLinesPrinted*PAPI_DUMP_BYTESPERLINE])&0x0F)+48;
			printBuffer[printBufferOffset]+=printBuffer[printBufferOffset]>57?7:0;
			printBufferOffset++;
			printBuffer[printBufferOffset++]=' ';
        }
		printBuffer[printBufferOffset++]=' ';

		if (bytesThisLine<PAPI_DUMP_BYTESPERLINE) 
			for (j=0; j<(PAPI_DUMP_BYTESPERLINE-bytesThisLine)*3; j++)
				printBuffer[printBufferOffset++]=' ';

		for (i=0; i<bytesThisLine; i++) {
			printBuffer[printBufferOffset]=buffer[i+numLinesPrinted*PAPI_DUMP_BYTESPERLINE];
			if (printBuffer[printBufferOffset]<0x21 || printBuffer[printBufferOffset]>0x7e)
				printBuffer[printBufferOffset]='.';
			printBufferOffset++;
		}
		printBuffer[printBufferOffset++]='\0';

		if (err=pDebugPrintfEX((logLevel,pLOGALWAYS,"   0x%04X:  %s",PAPI_DUMP_BYTESPERLINE*numLinesPrinted,printBuffer)))
			return err;
	} while (bytesThisLine+PAPI_DUMP_BYTESPERLINE*numLinesPrinted++<len);
	
	return pENOERROR;
}

int pDebugCheckFunc(void) 
{
  return RDABT_NOERROR;
}

#endif


#if 0
void *pMalloc(size_t size)
{
    return COS_Malloc(size);
}

void pFree(void *buffer)
{
   COS_Free(buffer);
}
#endif

/*
 * PAPI TIMER SECTION
 * no used in MTK
 */
t_pTimer pTimerCreate(u_int32 timeoutTenths, void(*callbackFunc)(void *), void *callbackArg, u_int8 flags) 
{
    return 0;
}

int pTimerSet(unsigned int timeout, void(*callbackFunc)(void *), void *callbackParm, int *timerHandle)
{
   return 0;
}

int pTimerCancel(t_pTimer timerHandle)
{
    return 0;
}

void rdabt_send_notify(void)
{
    extern u_int8  rdabt_send_notify_pending;

    rdabt_send_notify_pending++;
}
#if 0
void *pMemcpy(void *dest, void *src, size_t numbytes)
{
	return memcpy(dest, src, numbytes);
}
#endif

int pSendEvent(u_int16 event_id, u_int8 *msg)
{
    return 0;
}


static HANDLE CriticalSection_mutex;

u_int32 RDABT_Enter_CriticalSection()
{
	if(CriticalSection_mutex==NULL)
		CriticalSection_mutex = CreateEvent(NULL,FALSE,TRUE,NULL);
	WaitForSingleObject(CriticalSection_mutex,INFINITE);

	return 0;
}

void RDABT_Exit_CriticalSection(u_int32 status)
{
	SetEvent(CriticalSection_mutex);
}

#if CHIP_HAS_BTCPU == 1
void SYS_ASSERT2(void)
{
	assert(0);
}
#endif
