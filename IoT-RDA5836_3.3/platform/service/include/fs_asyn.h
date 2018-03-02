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
// FILENAME: fs_asyn.h
//
// DESCRIPTION:
//   This file to define asynchronization access file apis,include funcation declaring,
//   struct defineing,macro defining etc..
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//  Caoxh 	         2008.07.28      Create
////////////////////////////////////////////////////////////////////////////////

#ifndef _FS_ASYN_H_
#define _FS_ASYN_H_

// FS asynchronization api error code defining.

// Base
#define ERR_FS_ASYN_BASE                           100000

// Success.
#define ERR_FS_ASYN_SUCCESS                     0

// Parameter is invalid.
#define ERR_FS_ASYN_PARA_INVALID            ERR_FS_ASYN_BASE + 1

// Malloc failed.
#define ERR_FS_ASYN_MALLOC_FAILED         ERR_FS_ASYN_BASE + 2

// Send event failed.
#define ERR_FS_ASYN_SENDEVENT_FAILED   ERR_FS_ASYN_BASE + 3 

// fs asyn task not start.
#define ERR_FS_ASYN_TASK_NOT_START      ERR_FS_ASYN_BASE + 4 
// Event Id define.

// Base.
#define FS_ASYN_EVENT_ID_BASE     0x1000

// Read request.
#define FS_ASYN_EVENT_ID_READ_REQ       (FS_ASYN_EVENT_ID_BASE + 1)

// Write request.
#define FS_ASYN_EVENT_ID_WRITE_REQ      (FS_ASYN_EVENT_ID_BASE + 2)

// Callback function define.
typedef VOID (*FS_ASNY_FUNCPTR)(void*); 

// fs asyn read requst stucture.
typedef struct _fs_asyn_read_req
{
  INT32 iFd;                                      // File desciption,received call the function FS_Open.
  UINT8* pBuff;                                // Point to a buffer of to read.
  UINT32 uSize;                               // the size of to read.
  FS_ASNY_FUNCPTR pCBFunc;          // call back fuction, it shall been called when completed reading.
}FS_ASYN_READ_REQ;   

// fs asyn write requst stucture.
typedef struct _fs_asyn_write_req
{
  INT32 iFd;                                     // File desciption,received call the function FS_Open.
  UINT8* pBuff;                               // Point to a buffer of to write.
  UINT32 uSize;                              // the size of to write.
  FS_ASNY_FUNCPTR pCBFunc;       // call back fuction, it shall been called when completed writing.
}FS_ASYN_WRITE_REQ;

// fs asyn read result structure. 
typedef struct _fs_asyn_read_result
{  
    UINT32   uSize;		                 // size of really readed.
    UINT8*   pBuff;                        // point to the buffer of to read.
    INT32 	 iResult;                       // read option process result.if success, it is zero,else,it is the return value of FS_Read.
} FS_ASYN_READ_RESULT;

// fs asyn write result structure. 
typedef struct _fs_asyn_write_result
{  
    UINT32   uSize;		                // size of really writed.
    UINT8*   pBuff;                        // point to the buffer of to write.
    INT32 	 iResult;                        // write option process result.if success, it is zero,else,it is the error code  of FS_Write.
} FS_ASYN_WRITE_RESULT;

//fs asyn task entry.
VOID BAL_AsynFsTask(void *pData);


//-----------------------------------------------------------------------------------------
// FS_AsynReadReq
// Function:
//    Asynchronization read a file.
// Parameter:
//     iFd[in]:  File description,received FS_Open.
//     pBuff[in,out]: point to a buffer to read.
//     uSize[in]: read size.
//     pCallback[in]:callback function.
// Return value:
//     Upon successed completion ,ERR_FS_ASYN_SUCCESS shall be return, else the err code shall be return.
//-----------------------------------------------------------------------------------------
INT32 FS_AsynReadReq(INT32 iFd,UINT8* pBuff,UINT32 uSize,FS_ASNY_FUNCPTR pCallback);

//-----------------------------------------------------------------------------------------
// FS_AsynWriteReq
// Function:
//    Asynchronization write a file.
// Parameter:
//     iFd[in]:  File description,received FS_Open.
//     pBuff[in,out]: point to a buffer to write.
//     uSize[in]: write size.
//     pCallback[in]:callback function.
// Return value:
//     Upon successed completion ,ERR_FS_ASYN_SUCCESS shall be return, else the err code shall be return.
//-----------------------------------------------------------------------------------------
INT32 FS_AsynWriteReq(INT32 iFd,UINT8* pBuff,UINT32 uSize,FS_ASNY_FUNCPTR pCallback);


#endif
