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

#ifndef _AP_MEDIA_H_
#define _AP_MEDIA_H_

#include "aud_main.h"
#if defined(CAMERA_SUPPORT)
#include "cam_main.h"
#if defined(IP_CAMERA_SUPPORT)
#include "cpu_share.h"
#endif /* IP_CAMERA_SUPPORT */
#endif /* CAMERA_SUPPORT */

BOOL mediaSendBTCommand(UINT8 cmd, UINT32 param);
BOOL mediaSendCommand(UINT8 cmd, INT32 param);
UINT32 media_PlayInternalAudio(UINT8 audio_id, UINT8 times, BOOL waitKey); //times: loop times, 0 for continuous loop
void media_StopInternalAudio(void);

#if APP_SUPPORT_BLUETOOTH
BOOL mediaSendBTCommand(UINT8 cmd, UINT32 param);
#endif


#if defined(CAMERA_SUPPORT)
void cam_set_pair_results(UINT32 result, UINT32 value);
BOOL media_CameraPowerOn(UINT32 mode);
BOOL media_CameraPowerDown(void);
#if defined(IP_CAMERA_SUPPORT)
BOOL media_IPCameraStart(CAM_CAPTURE_STRUCT *capture);
BOOL media_IPCameraStop(void);
UINT32 media_IPCameraGetFramBuffer(UINT8 *buffer, UINT32 max_len);
#endif /* IP_CAMERA_SUPPORT */
#endif /* CAMERA_SUPPORT */

#endif  /* _AP_MEDIA_H_ */

