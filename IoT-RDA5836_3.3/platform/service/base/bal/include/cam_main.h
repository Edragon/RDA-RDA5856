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

#ifndef _CAM_MAIN_H__
#define _CAM_MAIN_H__

#include "MultimediaTask.h"
#include "ap_message.h"

typedef enum
{
   CAM_EVT_NOWAIT   ,
   CAM_EVT_POWER_UP,
   CAM_EVT_POWER_DOWN     ,

#if defined(IP_CAMERA_SUPPORT)
   IPCAM_EVT_START,
   IPCAM_EVT_STOP,
   IPCAM_EVT_GET_FRAME_DATA,
#endif /* IP_CAMERA_SUPPORT */

} cam_wait_event_enum;


#define CAM_INIT_EVENT() 
#define CAM_WAIT_EVENT(evt)  COS_WaitForSemaphore(g_camSem, COS_WAIT_FOREVER)
#define CAM_SET_EVENT(evt)   COS_ReleaseSemaphore(g_camSem)

extern HANDLE g_camSem;
extern BOOL cam_init(void);
extern BOOL cam_main(MULTI_MSG_PTR ev);; 
VOID cam_set_result(UINT32 result); 
#endif /* _CAM_MAIN_H__ */

