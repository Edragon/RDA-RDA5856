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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * TaskInit.c
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   Initilize the task structure for the entire application. Functions to allocate task resources, 
 *   create task communication interfaces and synchronization mechanism for common resource sharing.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "tgt_app_cfg.h"
#include "platform.h"
#include "event.h"
#include "cos.h"
#include "hal_host.h"

comval_t         *g_comval; //系统总的环境变量

#ifndef BT_SUPPORT
#if APP_SUPPORT_BLUETOOTH==1
#error platform not support bluetooth
#endif
#endif

// Task的stack大小定义.
#if (CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
#define SYS_TASK_STACK_SIZE         180
#if APP_SUPPORT_RGBLCD==1
#define APP_TASK_STACK_SIZE		    768
#else
#define APP_TASK_STACK_SIZE		    448
#endif
#define MED_TASK_STACK_SIZE	        320
#define BT_TASK_STACK_SIZE  	   	128
#define MULTI_TASK_STACK_SIZE  	   	384
#else /*(CHIP_ASIC_ID!=CHIP_ASIC_ID_5856E)*/
#define SYS_TASK_STACK_SIZE         256//256//512 	// 2k
#if APP_SUPPORT_RGBLCD==1
#define APP_TASK_STACK_SIZE		   768 	   // 3k
#else
#define APP_TASK_STACK_SIZE		    400//384//512 	// 2k
#endif
#if defined(__ZBAR_SUPPORT__)
#define MED_TASK_STACK_SIZE	        1024    //4k
#else
#define MED_TASK_STACK_SIZE	        320//320//512		// 2k
#endif /* __ZBAR_SUPPORT__ */
#define BT_TASK_STACK_SIZE  	   	480//340//512 	// 2k
#define MULTI_TASK_STACK_SIZE  	   	384//384//512
#endif /* (CHIP_ASIC_ID==CHIP_ASIC_ID_5856) */
 
#ifdef CFW_TCPIP_SUPPORT
#define APS_TASK_STACK_SIZE  	   	512
#endif
#define CFW_TASK_STACK_SIZE  	   	5*1024

// Task的优先级定义.
#if OS_KERNEL == FREERTOS

#define MED_TASK_PRIORITY	        	mainBLOCK_Q_PRIORITY
#define SYS_TASK_PRIORITY		        mainBLOCK_Q_PRIORITY
#define BT_TASK_PRIORITY			mainBLOCK_Q_PRIORITY
#define APP_TASK_PRIORITY		        mainBLOCK_Q_PRIORITY
#define APS_TASK_PRIORITY		        mainBLOCK_Q_PRIORITY
#define CFW_TASK_STACK_PRIORITY         mainBLOCK_Q_PRIORITY
#define MULTI_TASK_PRIORITY		      mainBLOCK_Q_PRIORITY
#else

#define MED_TASK_PRIORITY	        	230 //30
#define SYS_TASK_PRIORITY		        230 //10
#define BT_TASK_PRIORITY				230 //80
#define APP_TASK_PRIORITY		        230 //200
#define MULTI_TASK_PRIORITY		        230 //200
#ifdef CFW_TCPIP_SUPPORT
#define APS_TASK_PRIORITY		        230 //200
#endif
#define CFW_TASK_STACK_PRIORITY         230
#endif // endif OS_KERNEL == FREERTOS

extern TASK_ENTRY BAL_SysTask (void *pData);
extern TASK_ENTRY BAL_FSTaskEntry (void *pData);
extern TASK_ENTRY BAL_MMCTask (void *pData);
extern TASK_ENTRY BAL_BTTaskEntry (void *pData);
extern TASK_ENTRY BAL_MainTaskEntry (void *pData);
extern TASK_ENTRY BAL_MultimediaTaskEntry(void *pData);
#if defined(CFW_TCPIP_SUPPORT)
extern TASK_ENTRY BAL_CFWApsTask ( VOID* pData);
#endif /* CFW_TCPIP_SUPPORT */

#ifdef RDA_DLNA
extern TASK_ENTRY BAL_HTTP_ServerTask(void* pData);
extern TASK_ENTRY BAL_SSDP_ServerTask(void* pData);
extern TASK_ENTRY BAL_SSDP_AdvertiseTask(void* pData);
extern TASK_ENTRY BAL_HTTP_ServerTask_Shift(void* pData);
#endif

#if defined(GSM_SUPPORT)
extern TASK_ENTRY cfw_TaskEntry(void *pData);
#endif

extern BOOL BAL_Initalise(void );
BOOL pm_BatteryInit(VOID);
//extern u8 sxr_NewSemaphore (u8 InitValue);
#ifdef WIFI_SUPPORT
TASK_ENTRY BAL_WIFIMGR_TaskEntry(void *pData);
extern VOID wifi_task_init(VOID);
#endif

BOOL BAL_TaskInit ( VOID )
{
	HANDLE hTask;
	hal_HstSendEvent(BOOT_EVENT,0x11111);
 //	sxr_NewSemaphore(1); 
	BAL_Initalise();
 #ifdef  MCD_MGC_USB			
	//hal_musb_start();
 #endif
	hal_HstSendEvent(BOOT_EVENT,0x11113);
	hTask = COS_CreateTask(BAL_SysTask, MOD_SYS,
				NULL, NULL,
				SYS_TASK_STACK_SIZE, 
				SYS_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "System Task");  
	hal_HstSendEvent(BOOT_EVENT,0x11115);

#ifndef TARGET_MEDIA_SUPPORT_OFF
	hTask = COS_CreateTask(BAL_MMCTask, MOD_MED,
				NULL, NULL,
				MED_TASK_STACK_SIZE, 
				MED_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "Media Task");  
	hal_HstSendEvent(BOOT_EVENT,0x11117);
#endif

#if defined(WIFI_SUPPORT)
	hTask = COS_CreateTask(BAL_WIFIMGR_TaskEntry, MOD_WIFI_MGR,
					NULL, NULL,
					1024,
					APP_TASK_PRIORITY,
					COS_CREATE_DEFAULT, 0, "BAL_WIFIMGR_TaskEntry");


	wcnd_Main();
	marlin_download_init();

	wifi_wcnd_init();


#endif

#ifdef CFW_TCPIP_SUPPORT
   hTask = COS_CreateTask(BAL_CFWApsTask, MOD_APS,
				NULL, NULL,
				512, 
				APS_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "BAL_CFWApsTask");
#endif

#ifdef RDA_DLNA
	hTask = COS_CreateTask(BAL_HTTP_ServerTask_Shift, MOD_HTTP_SERVER_SHIFT,
				 NULL, NULL,
				 1024+512, 
				 230, 
				 COS_CREATE_DEFAULT, 0, "BAL_HTTP_Server_Client_Task");

	hTask = COS_CreateTask(BAL_HTTP_ServerTask, MOD_HTTP_SERVER,
				 NULL, NULL,
				 1024, 
				 230, 
				 COS_CREATE_DEFAULT, 0, "BAL_HTTPServerTask");


    hTask = COS_CreateTask(BAL_SSDP_AdvertiseTask, MOD_SSDP_ADVERTISE,
			     NULL, NULL,
			     512, 
			     230, 
			     COS_CREATE_DEFAULT, 0, "BAL_SSDPAdvertiseTask");

	hTask = COS_CreateTask(BAL_SSDP_ServerTask, MOD_SSDP_SERVER,
				 NULL, NULL,
				 512, 
				 230, 
			 	COS_CREATE_DEFAULT, 0, "BAL_SSDPServerTask");

#endif

#ifdef BT_SUPPORT
#if APP_SUPPORT_BLUETOOTH
   hTask = COS_CreateTask(BAL_BTTaskEntry, MOD_BT,
				NULL, NULL,
				BT_TASK_STACK_SIZE, 
				BT_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "Bluetooth Task");  

#endif	  
#endif	
	hal_HstSendEvent(BOOT_EVENT,0x11118);

#if 0//defined(GSM_SUPPORT)
	  hTask = COS_CreateTask(cfw_TaskEntry, MOD_CFW,
				NULL, NULL,
				CFW_TASK_STACK_SIZE,
				CFW_TASK_STACK_PRIORITY,
				COS_CREATE_DEFAULT, 0, "CFW Task");
#endif

#ifndef TARGET_MEDIA_SUPPORT_OFF
	  hTask = COS_CreateTask(BAL_MultimediaTaskEntry, MOD_MULTI,
				NULL, NULL,
				MULTI_TASK_STACK_SIZE, 
				MULTI_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "Multimedia Task"); 
				
	hal_HstSendEvent(BOOT_EVENT,0x11119);
#endif    


#ifndef COMPILE_ERROR_MOD
    hTask = COS_CreateTask(BAL_MainTaskEntry, MOD_APP,
                NULL, NULL,
                APP_TASK_STACK_SIZE, 
                APP_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "App Main Task"); 
#endif

#ifdef YOC_SUPPORT
	{
		int yoc_main_service_entry(void);
		yoc_main_service_entry();
	}
#endif

#if defined(MMI_ON_WIN32)
	/*send power on ind to simulator, mp3host, 0120 20:27*/
	COS_SendMessageEvent(MOD_APP,EV_DM_POWER_ON_IND, 0);
#endif

    return TRUE;
}





