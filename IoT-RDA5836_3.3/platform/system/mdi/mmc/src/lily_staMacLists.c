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


#include "lily_statemachine.h"



#if 0
//STA_MAC_IDLE,p=7,f=1
const uint16 sm_idle_p[]=
{
	STA_MAC_IDLE,
	STA_MAC_MEDIA_AUD_PLAY,
	STA_MAC_MEDIA_AUD_PAUSE,
	STA_MAC_MEDIA_AUD_RESUME,
	STA_MAC_MEDIA_IMG_DECODE,
	STA_MAC_MEDIA_CAM_POWERON,
	STA_MAC_NO_CARD
	
};
const uint16 sm_idle_f[]=
{
	STA_MAC_MAX
};

//STA_MAC_NO_CARD,p=2,f=1
const uint16 sm_nocard_p[]=
{
	STA_MAC_MEDIA_CAM_POWERON,
	STA_MAC_MEDIA_CAM_PREVIEWON,
	
};
const uint16 sm_nocard_f[]=
{
	STA_MAC_MAX
};

//STA_MAC_SLEEP,p=1,f=1
const uint16 sm_sleep_p[]=
{
	STA_MAC_IDLE
	
};
const uint16 sm_sleep_f[]=
{
	STA_MAC_MAX//if none,use this instead
};

//STA_MAC_MEDIA_AUD_PLAY,p=3,f=1
const uint16 sm_audplay_p[]=
{
	STA_MAC_MEDIA_AUD_PAUSE,
	STA_MAC_MEDIA_AUD_STOP,
	STA_MAC_MEDIA_IMG_DECODE	
	
};
const uint16 sm_audplay_f[]=
{
	STA_MAC_MEDIA_AUD_RESUME
};

//STA_MAC_MEDIA_AUD_PAUSE,p=3,f=1
const uint16 sm_audpause_p[]=
{
	STA_MAC_MEDIA_AUD_RESUME,
	STA_MAC_MEDIA_AUD_STOP,
	STA_MAC_MEDIA_IMG_DECODE	
	
};
const uint16 sm_audpause_f[]=
{
	STA_MAC_MEDIA_AUD_PLAY
	
};

//STA_MAC_MEDIA_AUD_RESUME,p=0,f=0
const uint16 sm_audresume_p[]=
{
	STA_MAC_MAX	
};
const uint16 sm_audresume_f[]=
{
	STA_MAC_MAX	
};
//STA_MAC_MEDIA_AUD_STOP,p=0,f=0
const uint16 sm_audstop_p[]=
{
	STA_MAC_MAX	
};
const uint16 sm_audstop_f[]=
{
	STA_MAC_MAX	
};


//STA_MAC_MEDIA_IMG_DECODE,p=3,f=0
const uint16 sm_imgDecode_p[]=
{
	STA_MAC_MEDIA_IMG_STOP,
	STA_MAC_MEDIA_AUD_PLAY,
	STA_MAC_MEDIA_AUD_RESUME	
	
};
const uint16 sm_imgDecode_f[]=
{
	STA_MAC_MAX	
};
//STA_MAC_MEDIA_IMG_STOP,p=0,f=0
const uint16 sm_imgDecodeStop_p[]=
{
	STA_MAC_MAX
	
};
const uint16 sm_imgDecodeStop_f[]=
{
	STA_MAC_MAX	
};

//STA_MAC_MEDIA_CAM_POWERON,p=1,f=0
const uint16 sm_camPwnOn_p[]=
{
	STA_MAC_MEDIA_CAM_POWEROFF	
};
const uint16 sm_camPwnOn_f[]=
{
	STA_MAC_MAX	
};
//STA_MAC_MEDIA_CAM_POWEROFF,p=0,f=0
const uint16 sm_camPwnOff_p[]=
{
	STA_MAC_MAX	
};
const uint16 sm_camPwnOff_f[]=
{
	STA_MAC_MAX	
};





























const STA_MAC_MANAGE_LIST lilyStaMacList[]=
{
	{7,sm_idle_p,1,sm_idle_f},//STA_MAC_IDLE
	{2,sm_nocard_p,1,sm_nocard_f},//STA_MAC_NO_CARD	
	{1,sm_sleep_p,0,sm_sleep_f},//STA_MAC_SLEEP		
	{3,sm_audplay_p,1,sm_audplay_f},//STA_MAC_MEDIA_AUD_PLAY
	{3,sm_audpause_p,1,sm_audpause_f},//STA_MAC_MEDIA_AUD_PAUSE
	{0,sm_audresume_p,0,sm_audresume_f},//STA_MAC_MEDIA_AUD_RESUME
	{0,sm_audstop_p,0,sm_audstop_f},//STA_MAC_MEDIA_AUD_STOP
	{3,sm_imgDecode_p,0,sm_imgDecode_f},//STA_MAC_MEDIA_AUD_STOP
	{0,sm_imgDecodeStop_p,0,sm_imgDecodeStop_f},//STA_MAC_MEDIA_IMG_STOP	
	{1,sm_camPwnOn_p,0,sm_camPwnOn_f},//STA_MAC_MEDIA_CAM_POWERON	
	{0,sm_camPwnOff_p,0,sm_camPwnOff_f},//STA_MAC_MEDIA_CAM_POWEROFF	
};
#endif
