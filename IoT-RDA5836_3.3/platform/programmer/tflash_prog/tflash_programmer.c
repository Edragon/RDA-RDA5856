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
#include "cs_types.h"
#include "cos.h"

#include "tflash_prog_map.h"

#define SYS_TASK_STACK_SIZE         512 	// 2k
#define APP_TASK_STACK_SIZE		    512 	// 2k
#define SYS_TASK_PRIORITY		        230 //10
#define APP_TASK_PRIORITY		        230 //200

extern TASK_ENTRY BAL_SysTask (void *pData);

TASK_ENTRY TFlash_MainTaskEntry(void *pData)
{
    DSM_TFlash_Upgrade_Step2();
}
// This xcpu_main replaces the regular xcpu_main present in the platform library
PROTECTED int main(VOID)
{
    hal_HstSendEvent(BOOT_EVENT, 0x92ade200);
    sxs_StartAll ();
    hal_HstSendEvent(BOOT_EVENT, 0x92ade201);
    BAL_Initalise();
    hal_HstSendEvent(BOOT_EVENT, 0x92ade202);
	COS_CreateTask(BAL_SysTask, MOD_SYS,
				NULL, NULL,
				SYS_TASK_STACK_SIZE, 
				SYS_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "System Task");  
	hal_HstSendEvent(BOOT_EVENT,0x11115);

	 COS_CreateTask(TFlash_MainTaskEntry, MOD_APP,
				NULL, NULL,
				APP_TASK_STACK_SIZE, 
				APP_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "App Main Task"); 

    sxr_StartScheduling ();
}


