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
// FILENAME: fs_ansy_local.h
//
// DESCRIPTION:
//   This file to define asynchronous access file local structures,local functions etc..
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//  Caoxh 	         2009.09.28      Create
////////////////////////////////////////////////////////////////////////////////

#ifndef _FS_ASYN_LOCAL_H_
#define _FS_ASYN_LOCAL_H_

typedef struct  _FS_ASYN_PARAM
{
	UINT32 Fd;
	UINT32 pBuff;
	UINT32 uSize;
	UINT32 pCallback;
}FS_ASYN_PARAM;

typedef struct _FS_ASYN_EVENT 
{
  UINT32 nEventId;
  UINT32 nParam1;
 // UINT32 nParam2;//for delete nParam2
//  UINT32 nParam3;//for delete nParam3
} FS_ASYN_EVENT;

VOID FS_AsynReadPrc(UINT32 pParam1);
VOID FS_AsynWritePrc(UINT32 pParam1);


#endif






