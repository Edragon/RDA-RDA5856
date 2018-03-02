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


/*******************************************************************************
 * Filename:
 * ---------
 *	MMITaskProt.h
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   This file is intends for MMI task function interface.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *------------------------------------------------------------------------------
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *==============================================================================
 *******************************************************************************/


#ifndef _MMITASKPROT_H
#define _MMITASKPROT_H

#include "ap_common.h"

/***************************************************************************** 
* Define
*****************************************************************************/
#define IS_MUSIC_MODE()		(RESULT_MUSIC  == g_current_module)
#define IS_FM_MODE()		(RESULT_RADIO  == g_current_module)
#define IS_RECORD_MODE()	(RESULT_RECORD_START  == g_current_module)
#define IS_BT_MODE()		(RESULT_BT     == g_current_module)
#define IS_USB_MODE()		(RESULT_UDISK  == g_current_module)
#define IS_LINEIN_MODE()       (RESULT_LINE_IN  == g_current_module)
/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef enum
{
	FUN_IDLE,          /*Idle Status*/
	FUN_LINEIN,
	FUN_CALENDAR,      /*Calendar Display*/
	FUN_RECORD, 
	FUN_MUSIC,
	FUN_FM,
	FUN_BT,
	FUN_USB,
	FUN_TIMER,
	FUN_SYSTEM_SETTING,
	FUN_CHARGING,
	MAX_FUNCTION_MOD
}FUN_MOD;


/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/
extern INT32           g_last_reslt;
extern INT32           g_current_module;
/***************************************************************************** 
* Extern Global Function
*****************************************************************************/

extern void BAL_MmiTask (void *pData);
extern void BAL_MultimediaTaskEntry(void *pData);
extern TASK_ENTRY BAL_MainTaskEntry(void *pData);
extern TASK_ENTRY BAL_IDLETaskEntry (void *pData);
extern void BAL_FSTaskEntry(void *pData);
extern void BAL_USBTaskEntry(void *pData);
extern void InitializeAll(void);
extern void InitFramework(void);
extern void ProtocolEventHandler(UINT32 eventID,void* MsgStruct,INT32 mod_src, void *peerBuf);
extern void InitializeChargingScr(void);
extern void mmi_frm_fetch_msg_from_extQ_to_circularQ(void);
extern VOID tp_task_main(VOID *pData);      //chenqiang add
extern void MainMenu_Entry(void);

INT32 APP_StandBy(void);

#endif /* end of _MMITASKPROT_H */


