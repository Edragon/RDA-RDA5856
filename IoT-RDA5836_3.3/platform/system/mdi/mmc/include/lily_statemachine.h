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


//chenhe create for lily states management,20071226




#ifndef _LILY_STATEMACHINE_H
#define _LILY_STATEMACHINE_H
#include "cs_types.h"

#define STM_USE_BIT

//#define ENABLE_LILY_SLEEP

#ifdef STM_USE_BIT


typedef enum
{
	STA_MAC_MOD_IDLE,
	STA_MAC_MOD_SLEEP,
	STA_MAC_MOD_AUD_PLAY,
	STA_MAC_MOD_AUD_PAUSE,
	STA_MAC_MOD_AUDREC,		
	STA_MAC_MOD_IMG,		
	STA_MAC_MOD_CAM,	
	STA_MAC_MOD_FS,	
	STA_MAC_MOD_VID,	
	STA_MAC_MOD_VIDREC,		
	STA_MAC_MOD_USB,
	STA_MAC_MOD_FMREC,
	STA_MAC_MOD_FM,
	STA_MAC_MOD_NONE,	
}STA_MAC_MODULE;

typedef enum
{
	STA_RES_OK,
	STA_RES_FAIL
}STA_RESULT;

#define STATE_BIT1 (1L)



#define STM_MOD_IDLE (STATE_BIT1)
#define STM_MOD_SLEEP (STATE_BIT1<<(STA_MAC_MOD_SLEEP))
#define STM_MOD_AUD_PLAY (STATE_BIT1<<(STA_MAC_MOD_AUD_PLAY))
#define STM_MOD_AUD_PAUSE (STATE_BIT1<<(STA_MAC_MOD_AUD_PAUSE))
#define STM_MOD_AUDREC (STATE_BIT1<<(STA_MAC_MOD_AUDREC))
#define STM_MOD_IMG (STATE_BIT1<<(STA_MAC_MOD_IMG))
#define STM_MOD_CAM (STATE_BIT1<<(STA_MAC_MOD_CAM))
#define STM_MOD_FS (STATE_BIT1<<(STA_MAC_MOD_FS))
#define STM_MOD_VID (STATE_BIT1<<(STA_MAC_MOD_VID))
#define STM_MOD_VIRECD (STATE_BIT1<<(STA_MAC_MOD_VIDREC))
#define STM_MOD_USB (STATE_BIT1<<(STA_MAC_MOD_USB))
#define STM_MOD_FMREC (STATE_BIT1<<(STA_MAC_MOD_FMREC))
#define STM_MOD_FM (STATE_BIT1<<(STA_MAC_MOD_FM))









#else
#define STATE_MACHINE_IDLE_BASE (0x0)
#define STATE_MACHINE_MEDIA_BASE (STATE_MACHINE_IDLE_BASE+10)
#define STATE_MACHINE_USB_BASE (STATE_MACHINE_MEDIA_BASE+200)


typedef enum
{
	STA_MAC_MOD_IDLE,
	STA_MAC_MOD_SLEEP,
	STA_MAC_MOD_AUD,
	STA_MAC_MOD_AUDREC,		
	STA_MAC_MOD_IMG,		
	STA_MAC_MOD_CAM,	
	STA_MAC_MOD_FS,	
	STA_MAC_MOD_VID,	
	STA_MAC_MOD_VIDREC,		
	STA_MAC_MOD_USB,
	STA_MAC_MOD_NONE,	
}STA_MAC_MODULE;




































typedef enum
{
	STA_RES_OK,
	STA_RES_FAIL
}STA_RESULT;





typedef enum
{
	STA_MAC_IDLE = STATE_MACHINE_IDLE_BASE,
	STA_MAC_NO_CARD,
	STA_MAC_SLEEP,
	
/*********all  is media State,start****/	
	/*audio module states*/
	STA_MAC_MEDIA_AUD_PLAY ,//= STATE_MACHINE_MEDIA_BASE,
	STA_MAC_MEDIA_AUD_PAUSE,
	STA_MAC_MEDIA_AUD_RESUME,	
	STA_MAC_MEDIA_AUD_STOP,	
/*********all  is media State,end****/	

/********image start	***/
	STA_MAC_MEDIA_IMG_DECODE,
	STA_MAC_MEDIA_IMG_STOP,	
/********image end	***/
/********camera start	***/
	STA_MAC_MEDIA_CAM_POWERON,
	STA_MAC_MEDIA_CAM_PREVIEWON,
	STA_MAC_MEDIA_CAM_PREVIEWOFF,
	STA_MAC_MEDIA_CAM_CAPTURE,
	STA_MAC_MEDIA_CAM_POWEROFF,
/********camera end	***/	
	STA_MAC_USB,// = STATE_MACHINE_USB_BASE,
	STA_MAC_MAX
}STA_MAC_STATES;

typedef struct
{
	uint16 permissionNum;
	const uint16 *permission;
	uint16 forbiddanceNum;
	const uint16 *forbiddance;	
}STA_MAC_MANAGE_LIST;

typedef struct
{
	uint16 parallelState1;
	uint16 parallelState2;
	uint16 parallelState3;	
}STA_MAC_STATES_RECORD;













//function declaration
STA_MAC_STATES lily_stmTransferTmpstate(uint16 *state);
void lily_stateMachineInit(void);
boolean lily_getModuleState(uint16 module);

#endif


STA_RESULT lily_changeToState(uint16 module);
STA_RESULT lily_exitModule(uint16 module);



#endif //_LILY_STATEMACHINE_H
