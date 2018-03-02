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



#if (CSW_EXTENDED_API == 1)

#include "mci.h"

// Init
BOOL    MCI_TaskInit(VOID){ return TRUE; }

// Video Record
UINT32  MCI_VideoRecordAdjustSetting(INT32 adjustItem, INT32 value) { return 0; }
UINT32  MCI_VideoRecordPause(VOID){ return 0; }
UINT32  MCI_VideoRecordPreviewStart(MMC_VDOREC_SETTING_STRUCT *previewPara){ return 0; }
UINT32  MCI_VideoRecordPreviewStop(VOID){ return 0; }
UINT32  MCI_VideoRecordResume(VOID){ return 0; }
UINT32  MCI_VideoRecordStart(char *pfilename){ return 0; }
UINT32  MCI_VideoRecordStop(VOID){ return 0; }
UINT32  MCI_DisplayVideoInterface (VOID){ return 0; }
VOID vdorec_cancel_saveFile(VOID){}

// Camera
VOID set_mmc_camera_preview(){}
VOID set_multicaputure(INT32 i){}

#endif // CSW_EXTENDED_API


