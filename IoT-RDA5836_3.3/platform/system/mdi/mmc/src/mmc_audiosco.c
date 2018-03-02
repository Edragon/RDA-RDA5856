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

////////////////////////////////////////////////////////////////////////////////
// FILENAME: mmc_audioSCO.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cswtype.h"
#include "mcip_debug.h"
#include "fs.h"



#include "mmc.h"
#include "cpu_share.h"

#include "audio_api.h"

#include "aud_m.h"
#include "fs_asyn.h"

#include "cos.h"
#include "mmc_audiosco.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "ars_m.h"
#include "event.h"

#include "dm_audio.h"

extern AUD_ITF_T   audioItf;
extern PUBLIC UINT32 gpio_detect_earpiece(VOID);
//extern PUBLIC VOIS_ERR_T BTSco_RecordStart( INT32 file);

/*
* NAME:	MMC_AudioSCODecClose()
* DESCRIPTION: Close SCO decoder.
*/
static BOOL BTScoCallStarted = FALSE;
static UINT16 BTSco_connectHandle;

BOOL BTSco_AudCallStarted(void)
{
	return BTScoCallStarted;
}
VOID BTSco_AudCallReset(void)
{
	BTScoCallStarted = FALSE;
}

UINT16 BTSco_AudFileHandle(void)
{
	return BTSco_connectHandle;
}

BOOL BTSco_AudCallStart(void)
{
    if(!BTScoCallStarted)
    {
        // Initial cfg
        if(gpio_detect_earpiece())
            audioItf = AUD_ITF_BT_EP;
        else
            audioItf = AUD_ITF_BT_SP;

        BTScoCallStarted = TRUE;
        DM_StartAudioEX();  
    }
    
    return TRUE;
}

BOOL BTSco_AudCallStop(void)
{
    if(BTScoCallStarted)
    {
        DM_StopAudioEX();  
    }
    
    return TRUE;
}

int32 Aduio_SCORecord(HANDLE fhd)
{
       //BTSco_RecordStart(fhd);  //not support;
	return  MCI_ERR_NO;
}

int32 Audio_SCOPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{

    hal_HstSendEvent(SYS_EVENT, 0x19820100);
    hal_HstSendEvent(SYS_EVENT, fhd);

    BTSco_connectHandle = fhd;
    BTSco_AudCallStart();
    return  MCI_ERR_NO;
}

int32 Audio_SCOStop (void) 	
{
    //dbg_TraceOutputText(0,"Audio_SCOStop BTScoCallStarted?%d",BTScoCallStarted);

    BTSco_AudCallStop();
    return  MCI_ERR_NO;
}


int32 Audio_SCOPause (void)	
{

	return  MCI_ERR_NO;
}


int32 Audio_SCOResume ( HANDLE fhd) 
{

	return  MCI_ERR_NO;
}


int32 Audio_SCOGetID3 (char * pFileName)  
{
	return 0;	
}

int32 Audio_SCOGetPlayInformation (MCI_PlayInf * MCI_PlayInfSCO)  
{
	MCI_PlayInfSCO->PlayProgress=0;
	
	//dbg_TraceOutputText(0,"[MCI_SCO]PlayProgress:%d",MCI_PlayInfSCO->PlayProgress);
	
	return MCI_ERR_NO;
}

int32 Audio_SCOUserMsg(COS_EVENT *ev)
{
	int32 nMsg = ev->nEventId;
	switch (nMsg)
	{
		default:
			
			//dbg_TraceOutputText(0,"[MMC_SCO_ERR]ERROR MESSAGE!");
			
			break;
	}
    return 0;
}

