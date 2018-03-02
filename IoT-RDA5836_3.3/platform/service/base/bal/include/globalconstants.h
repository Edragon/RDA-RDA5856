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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	GlobalConstants.h
 *
 * Project:
 * --------
 *  MAUI
 *
 * Description:
 * ------------
 *  Global const define
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

#ifndef _GLOBALCONSTANTS_H_
#define _GLOBALCONSTANTS_H_
//Constants from Menu.h


#define MAX_SUB_MENUS			60
#define MAX_SUBMENU_CHARACTERS	48

#define MAX_SUB_MENU_SIZE		    (48*ENCODING_LENGTH + ENCODING_LENGTH)
#define MAX_SUB_MENU_HINT_SIZE	(48*ENCODING_LENGTH + ENCODING_LENGTH)

//Queue structures
#define MAX_PRT_NODES			50

#define INVALID_HANDLER			0
#define QUEUE_NOT_YET_CREATED	0

typedef enum {
	KEY_1 = 1,	
	KEY_2,	
	KEY_3,	
	KEY_4,		
	KEY_5,	
	KEY_6,	
	KEY_7,	
	KEY_8,	
	KEY_9,	
	KEY_0= 10,
	
	KEY_MODE, // =0x0b
	KEY_PLAY,
	KEY_REC,
	KEY_PREV,
	KEY_NEXT,		
	KEY_EQ,// = 0x10
	KEY_LOOP,
	KEY_MUTE, 
	KEY_HALT,
	KEY_POWER,
	
	KEY_VOL_UP,
	KEY_VOL_DOWN,
	KEY_VOL_COMB,
	KEY_EXTRA_1,
	KEY_EXTRA_2,


	MAX_KEYS,				/* max key codes support */	

	KEY_INVALID	= 0x5a // 0xFE// 255 /* By JL, to sync with driver */
} mmi_keypads_enum;

//CSD added by JL for sync with alignment time definition
#define TIMER_IS_ALIGNMENT				0
#define TIMER_IS_NO_ALIGNMENT				1

//KEY BOARD MSG TYPES (Global to MMI, KBD and KBH tasks)
typedef enum {
	KEY_EVENT_DOWN,
	KEY_EVENT_UP,  
	KEY_LONG_PRESS,
	KEY_REPEAT,
	KEY_FULL_PRESS,
	MAX_KEY_TYPE
} mmi_key_types_enum;

//++Robin
#define KEY_EVENT_REPEAT					KEY_REPEAT
#define KEY_EVENT_LONG_PRESS			KEY_LONG_PRESS   
//--Robin


//PRT MESSAGE TYPES (Global to MMI and PRT task)
//#define PRT_SETUP_IND				1
/*  Sound media interface message types */
#define START_MELODY			1
#define STOP_MELODY				2
#define RESUME_MELODY			3


//EVENT TYPES
#define KBD_EVENT				1
#define PRT_EVENT				2
#define TMR_EVENT				3
#define HWR_EVENT				4
#define MEDIA_EVENT				5

#define KEY_IDLE		0
#define KEY_PRESS		1

//WINDOWS KEY CODES
#ifdef MMI_ON_WIN32	// amitm5
#define WM_KEYPRESS			513 // Equivalent to WM_LBUTTONDOWN
#define WM_KEYRELEASE		514 // Equivalent to WM_LBUTTONUP
#else
#define WM_KEYPRESS			0
#define WM_KEYRELEASE		1
//++Robin
#define DRV_WM_KEYLONGPRESS   2
#define DRV_WM_KEYREPEATED   3
//--Robin
#define DRV_WM_KEYFULLPRESS   4
#endif


#define WM_KEYBRD_PRESS			256 // Equivalent to WM_KEYDOWN for key board keys
#define WM_KEYBRD_RELEASE		257 // Equivalent to WM_KEYUP for key board keys

#endif

