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

#ifndef _AP_UDISK_H_
#define _AP_UDISK_H_


// =============================================================================
// g_umssStorageCallbackMc
// -----------------------------------------------------------------------------
/// Callback of MC storage
// =============================================================================
//EXPORT PUBLIC CONST UMSS_STORAGE_CALLBACK_T     g_umssStorageMcCallback;

INT32 USB_Entry(INT32 param);
//获取U盘是否插线  0:没插线   其他:插线
BOOL GetUsbCableStatus(void);


#endif /*_UDISK_H_*/



