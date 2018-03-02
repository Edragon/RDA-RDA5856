/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 2003 CII Technologies, Inc.                      */
/*                                                                       */
/* PROPRIETARY RIGHTS of CII Technology are involved in the              */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*   dm_tone.c                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/* AUTHOR                                                                */
/*       simon  yang                                    */
/* FUNCTIONS                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      DATE  2007/6            REMARKS                                  */
/*************************************************************************/


#include "chip_id.h"
#include "ars_m.h"
#include "aud_m.h"
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
#include "mps_m.h"
#endif
#include "apbs_m.h"

//#include "vois_m.h"
//#include "vpp_speech.h"

#include "sxr_tls.h"
#include "sxr_tksd.h"
#include "csw_csp.h"
#include "csw_debug.h"

#include "errorcode.h"
#include <cos.h>
#include <base_prv.h>
#include <cswtype.h>
#include "dm.h"
#include "dm_audio_ex.h"
#include <event.h>
#include "ts.h"
#include "stdkey.h"
#include "cmn_defs.h"
#include "assert.h"
#include "tgt_app_cfg.h"

#define PAL_TICK1S 16384  // time constant of the timer - 16384 ticks/s
//#define SYS_EV_MASK_AUDIO_TONE            0xF0000001+4

/***********************************************************
for c1000, use this define to play tone,use the global variable to judge if play music or not
***********************************************************/

//#define AUDIO_DELAY(a) sxr_Sleep( (a) * 16384 / 1000 )
#define AUDIO_DELAY(a) dm_sysMSleep(a)

BOOL Play_music = FALSE;
BOOL audioBusy = FALSE;
BOOL audioBusy_stream = FALSE;
UINT32 gTone_Audio_Total_time = 300;
UINT32 g_playToneAudioTempTime = 0;
UINT8 g_mic_gain         = AUD_MIC_ENABLE;
UINT8 g_spk_gain ;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
MPS_HANDLE_T audioHandle = NULL;
MPS_CONFIG_T audioStreamConfig =
{
    .audioPath               = SND_ITF_RECEIVER,
    .windows                 = { {0, 0}, 0, 0},
    .volume                  = SND_SPK_VOL_4,
    .equalizerMode           = SND_EQUALIZER_NORMAL
};
#endif
#define __SUPPORT_BT_PHONE__
#include "vois_m.h"

#define    VPP_SPEECH_MUTE   0 
#define    VPP_SPEECH_UNMUTE  0x400 

BOOL isSPKMute = FALSE;

BOOL mic_record = FALSE;

typedef enum
{
    DTMF_PLAYING,
    DTMF_IDLE
} CII_DTMF_STATE;


/* Tone types. */
//reference GSM 02.40  or TS 22.001
typedef enum
{
    CII_TONE_DTMF_0,
    CII_TONE_DTMF_1,
    CII_TONE_DTMF_2,
    CII_TONE_DTMF_3,
    CII_TONE_DTMF_4,
    CII_TONE_DTMF_5,
    CII_TONE_DTMF_6,
    CII_TONE_DTMF_7,
    CII_TONE_DTMF_8,
    CII_TONE_DTMF_9,
    CII_TONE_DTMF_A,
    CII_TONE_DTMF_B,
    CII_TONE_DTMF_C,
    CII_TONE_DTMF_D,
    CII_TONE_DTMF_S,//'*'
    CII_TONE_DTMF_P,//'#'
    CII_DIAL_TONE,// Comfort tones #1
    CII_SUBSCRIBER_BUSY,//Comfort tones #2
    CII_CONGESTION,//Comfort tones #4
    CII_RADIO_PATHACKNOWLEDGEMENT,//Comfort tones #5
    CII_CALL_DROPPED,//Comfort tones #6
    CII_SPECIAL_INFORMATION,//Comfort tones #7
    //( Error/Special Information;Number Unobtainable ;Authentication Failure )
    CII_CALL_WAITING_TONE,//Comfort tones #8
    CII_RINGING_TONE,
    CII_TEST_TONE,
    CII_TONE_END
} CII_ToneType;


typedef enum
{
    CII_TONE_0dB,
    CII_TONE_m3dB,
    CII_TONE_m9dB,
    CII_TONE_m15dB,
    CII_TONE_DEFAULT
} CII_volume;

typedef struct
{
    CII_volume level;
    CII_ToneType Frequency;
    UINT16 Duration;
} CII_AUDIO_TONE;


/*add some scenario book here */
typedef struct
{
    AUD_TONE_TYPE_T tone;
    UINT32  tone_term;
} cii_DTMF_term,*cii_DTMF_pterm;

typedef struct
{
    UINT32  term_num;
    cii_DTMF_pterm DTMF_term;
} cii_DTMF_tone,*cii_DTMF_ptone;



const static cii_DTMF_term term_0_P[2] =
{
    {HAL_AUDIO_DTMF_0,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_1_P[2] =
{
    {HAL_AUDIO_DTMF_1,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_2_P[2] =
{
    {HAL_AUDIO_DTMF_2,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_3_P[2] =
{
    {HAL_AUDIO_DTMF_3,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_4_P[2] =
{
    {HAL_AUDIO_DTMF_4,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_5_P[2] =
{
    {HAL_AUDIO_DTMF_5,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_6_P[2] =
{
    {HAL_AUDIO_DTMF_6,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_7_P[2] =
{
    {HAL_AUDIO_DTMF_7,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_8_P[2] =
{
    {HAL_AUDIO_DTMF_8,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_9_P[2] =
{
    {HAL_AUDIO_DTMF_9,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_A_P[2] =
{
    {HAL_AUDIO_DTMF_0,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_B_P[2] =
{
    {HAL_AUDIO_DTMF_B,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_C_P[2] =
{
    {HAL_AUDIO_DTMF_C,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_D_P[2] =
{
    {HAL_AUDIO_DTMF_D,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_S_P[2] =
{
    {HAL_AUDIO_DTMF_S,330},
    {HAL_AUDIO_SILENCE,330}
};
const static cii_DTMF_term term_P_P[2] =
{
    {HAL_AUDIO_DTMF_P,330},
    {HAL_AUDIO_SILENCE,330}
};


const  static cii_DTMF_tone DTMF_0_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_0_P
};
const  static cii_DTMF_tone DTMF_1_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_1_P
};
const  static cii_DTMF_tone DTMF_2_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_2_P
};
const  static cii_DTMF_tone DTMF_3_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_3_P
};
const  static cii_DTMF_tone DTMF_4_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_4_P
};
const  static cii_DTMF_tone DTMF_5_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_5_P
};
const  static cii_DTMF_tone DTMF_6_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_6_P
};
const  static cii_DTMF_tone DTMF_7_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_7_P
};
const  static cii_DTMF_tone DTMF_8_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_8_P
};
const  static cii_DTMF_tone DTMF_9_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_9_P
};
const  static cii_DTMF_tone DTMF_A_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_A_P
};
const  static cii_DTMF_tone DTMF_B_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_B_P
};
const  static cii_DTMF_tone DTMF_C_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_C_P
};
const  static cii_DTMF_tone DTMF_D_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_D_P
};
const  static cii_DTMF_tone DTMF_S_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_S_P
};
const  static cii_DTMF_tone DTMF_P_P=
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_P_P
};

const static cii_DTMF_term term_DIAL[1] =
{
    {HAL_AUDIO_COMFORT_950,1000}
};

const static cii_DTMF_tone DTMF_DIAL =
{
    0,
    (cii_DTMF_pterm)&term_DIAL
};

const static cii_DTMF_term term_SUBSCRIBER_BUSY[2] =
{
    {HAL_AUDIO_COMFORT_425,500},
    {HAL_AUDIO_SILENCE,500}
};


const static cii_DTMF_tone DTMF_SUBSCRIBER_BUSY =
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_SUBSCRIBER_BUSY
};


const static cii_DTMF_term term_CONGESTION[2] =
{
    {HAL_AUDIO_COMFORT_425,200},
    {HAL_AUDIO_SILENCE,200}
};

const static cii_DTMF_tone DTMF_CONGESTION =
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_CONGESTION
};


const static cii_DTMF_term term_RADIO_PATHACKNOWLEDGEMENT[1] =
{
    {HAL_AUDIO_COMFORT_425,200}
};

const static cii_DTMF_tone DTMF_RADIO_PATHACKNOWLEDGEMENT =
{
    0,
    (cii_DTMF_pterm)&term_RADIO_PATHACKNOWLEDGEMENT
};

const static cii_DTMF_term term_CALL_DROPPED[2] =
{
    {HAL_AUDIO_COMFORT_425,200},
    {HAL_AUDIO_SILENCE,200}
};

const static cii_DTMF_tone DTMF_CALL_DROPPED =
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_CALL_DROPPED
};

const static cii_DTMF_term term_SPECIAL_INFORMATION[4] =
{
    {HAL_AUDIO_COMFORT_950,330},
    {HAL_AUDIO_COMFORT_1400,330},
    {HAL_AUDIO_COMFORT_1800,330},
    {HAL_AUDIO_SILENCE,1000}
};

const static cii_DTMF_tone DTMF_SPECIAL_INFORMATION =
{
    3,//changed by CII
    (cii_DTMF_pterm)&term_SPECIAL_INFORMATION
};

const static cii_DTMF_term term_CALL_WAITING_TONE[7] =
{
    {HAL_AUDIO_COMFORT_425,200},
    {HAL_AUDIO_SILENCE,600},
    {HAL_AUDIO_COMFORT_425,200},
    {HAL_AUDIO_SILENCE,3000},
    {HAL_AUDIO_COMFORT_425,600},
    {HAL_AUDIO_SILENCE,600},
    {HAL_AUDIO_COMFORT_425,200}
};

const static cii_DTMF_tone DTMF_CALL_WAITING_TONE =
{
    6,//changed by CII
    (cii_DTMF_pterm)&term_CALL_WAITING_TONE
};

const static cii_DTMF_term term_RINGING_TONE[2] =
{
    {HAL_AUDIO_COMFORT_425,1000},
    {HAL_AUDIO_SILENCE,4000}//changed by CII,refer GSM 02.40 version 7.0.1

};

const static cii_DTMF_tone DTMF_RINGING_TONE =
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_RINGING_TONE
};

const static cii_DTMF_term term_TEST_TONE[2] =
{
    {HAL_AUDIO_DTMF_1,2000},
    {HAL_AUDIO_SILENCE,1000}
};

const static cii_DTMF_tone DTMF_TEST_TONE =
{
    1,//changed by CII
    (cii_DTMF_pterm)&term_TEST_TONE
};

//level , frequency , duration

CONST cii_DTMF_ptone DTMF_ARRAY[CII_TONE_END]=
{
    (cii_DTMF_ptone)&DTMF_0_P,
    (cii_DTMF_ptone)&DTMF_1_P,
    (cii_DTMF_ptone)&DTMF_2_P,
    (cii_DTMF_ptone)&DTMF_3_P,
    (cii_DTMF_ptone)&DTMF_4_P,
    (cii_DTMF_ptone)&DTMF_5_P,
    (cii_DTMF_ptone)&DTMF_6_P,
    (cii_DTMF_ptone)&DTMF_7_P,
    (cii_DTMF_ptone)&DTMF_8_P,
    (cii_DTMF_ptone)&DTMF_9_P,
    (cii_DTMF_ptone)&DTMF_A_P,
    (cii_DTMF_ptone)&DTMF_B_P,
    (cii_DTMF_ptone)&DTMF_C_P,
    (cii_DTMF_ptone)&DTMF_D_P,
    (cii_DTMF_ptone)&DTMF_S_P,
    (cii_DTMF_ptone)&DTMF_P_P,
    (cii_DTMF_ptone)&DTMF_DIAL,
    (cii_DTMF_ptone)&DTMF_SUBSCRIBER_BUSY,
    (cii_DTMF_ptone)&DTMF_CONGESTION,
    (cii_DTMF_ptone)&DTMF_RADIO_PATHACKNOWLEDGEMENT,
    (cii_DTMF_ptone)&DTMF_CALL_DROPPED,
    (cii_DTMF_ptone)&DTMF_SPECIAL_INFORMATION,
    (cii_DTMF_ptone)&DTMF_CALL_WAITING_TONE,
    (cii_DTMF_ptone)&DTMF_RINGING_TONE,
    (cii_DTMF_ptone)&DTMF_TEST_TONE,
};

extern BOOL COS_SendSysEvent(COS_EVENT* pEvent,UINT16 nOption);
BOOL DM_GetSPKMuteFlag(VOID);



#ifdef __SUPPORT_BT_PHONE__
extern BOOL BTSco_AudCallStarted(VOID);
extern VOIS_ERR_T BTSco_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg);
extern VOIS_ERR_T BTSco_Stop(VOID);
extern VOID BTSco_AudCallReset(VOID);
extern VOIS_ERR_T BTSco_Start(
                        CONST AUD_ITF_T      itf,
                        CONST VOIS_AUDIO_CFG_T* cfg);


#endif /* __SUPPORT_BT_PHONE__ */


CII_DTMF_STATE cii_tone_sate = DTMF_IDLE ;
CII_AUDIO_TONE cii_currentDTMF ;
UINT32  term_num =0;

UINT8 gAudio_mode=DM_AUDIO_MODE_EARPIECE;
UINT8 g_speaker_vlume=5;
UINT8 tone_slot = 0;


AUD_ITF_T   audioItf = AUD_ITF_RECEIVER;
AUD_LEVEL_T audio_cfg =
{
    AUD_SPK_VOL_4,
    AUD_MIC_ENABLE,
    AUD_SIDE_MUTE,
    AUD_TONE_m3dB
};
HAL_AIF_STREAM_T audioStream =
{
    NULL,
    0,
    HAL_AIF_FREQ_8000HZ,
    HAL_AIF_MONO,
    TRUE,
    NULL,
    NULL
};
AUD_ITF_T audioItfBackup = AUD_ITF_RECEIVER;
AUD_SPK_LEVEL_T spkLevelBackup = AUD_SPK_VOL_4;
AUD_MIC_LEVEL_T micLevelBackup = AUD_MIC_ENABLE;

#if 0
static struct hal_AudioCfg audio_cfg_mute =
{
    HAL_AUDIO_SPK_MUTE,
    HAL_AUDIO_SPK,
    HAL_AUDIO_MIC_30dB,
    HAL_AUDIO_MIC,
    HAL_AUDIO_SIDE_MUTE
};
#endif

UINT32 g_audioRecBufStart   = 0;
UINT32 g_audioRecBufLen     = 0;
// Current offset from start
UINT32 g_audioRecBufOffset  = 0;

UINT32 g_audioPlayBufStart   = 0;
UINT32 g_audioPlayBufLen     = 0;
// Current offset from start
UINT32 g_audioPlayBufOffset  = 0;

#define DM_AUD_SYNC_EVENT_ID (0xffffffff)
#define DM_AUD_SYNC_SLOT_NUM (3)
#define DM_AUD_SYNC_DEBUG_VERBOSE (0)

typedef enum
{
    DM_AUD_SYNC_SLOT_FREE = 0,
    DM_AUD_SYNC_SLOT_ALLOC = 1,
    DM_AUD_SYNC_SLOT_WAIT = 2,
    DM_AUD_SYNC_SLOT_ACK = 3,
} DM_AUD_SYNC_SLOT_STATUS_T;

typedef struct
{
    UINT8 status;
    UINT8 taskId;
    UINT16 count;
    UINT32 result;
#if (DM_AUD_SYNC_DEBUG_VERBOSE)
    UINT32 callerAddr;
    UINT32 resumeCnt;
#endif
} DM_AUD_SYNC_SLOT_T;

PRIVATE VOLATILE DM_AUD_SYNC_SLOT_T g_dmAudSyncSlotAry[DM_AUD_SYNC_SLOT_NUM];

PRIVATE CONST char *g_dmAudSyncAssertStrBadSlotIndex = 
    "DM AUD SYNC: bad slot index";
PRIVATE CONST char *g_dmAudSyncAssertStrBadSlotStatus = 
    "DM AUD SYNC: Bad slot status";
PRIVATE CONST char *g_dmAudSyncAssertStrBadTaskId = 
    "DM AUD SYNC: bad task ID";


PUBLIC VOID DM_AudInit(VOID)
{
    for (UINT32 i = 0; i < DM_AUD_SYNC_SLOT_NUM; i++)
    {
        g_dmAudSyncSlotAry[i].status = DM_AUD_SYNC_SLOT_FREE;
    }
}

PROTECTED UINT32 DM_GetAudSyncSlot(VOID)
{
    UINT32 scStatus;
    UINT32 slot;
#if (DM_AUD_SYNC_DEBUG_VERBOSE)
    UINT32 callerAddr;
    HAL_SYS_GET_RA(&callerAddr);
#endif

    while (1)
    {
        scStatus = hal_SysEnterCriticalSection();
        for (slot = 0; slot < DM_AUD_SYNC_SLOT_NUM; slot++)
        {
            if (g_dmAudSyncSlotAry[slot].status == DM_AUD_SYNC_SLOT_FREE)
            {
                g_dmAudSyncSlotAry[slot].status = DM_AUD_SYNC_SLOT_ALLOC;
                g_dmAudSyncSlotAry[slot].taskId = COS_GetCurrentTaskId();
                g_dmAudSyncSlotAry[slot].count++;
#if (DM_AUD_SYNC_DEBUG_VERBOSE)
                g_dmAudSyncSlotAry[slot].callerAddr = callerAddr;
#endif
                break;
            }
        }
        hal_SysExitCriticalSection(scStatus);
        if (slot < DM_AUD_SYNC_SLOT_NUM)
        {
            break;
        }
        else
        {
            COS_Sleep(2 );
        }
    }

    return slot;
}

PROTECTED UINT32 DM_WaitAudOperResult(UINT32 slot)
{
    UINT32 scStatus;
    UINT32 result;

    if (slot >= DM_AUD_SYNC_SLOT_NUM)
    {
        __assert(g_dmAudSyncAssertStrBadSlotIndex);
    }

    scStatus = hal_SysEnterCriticalSection();

    if (g_dmAudSyncSlotAry[slot].taskId != COS_GetCurrentTaskId())
    {
        __assert(g_dmAudSyncAssertStrBadTaskId);
    }

    if (g_dmAudSyncSlotAry[slot].status == DM_AUD_SYNC_SLOT_ALLOC)
    {
        g_dmAudSyncSlotAry[slot].status = DM_AUD_SYNC_SLOT_WAIT;
        //sxr_SuspendTask(g_dmAudSyncSlotAry[slot].taskId);
        CSW_TRACE(CSW_DM_TS_ID, "Suspend Task for status of slot [%d] is DM_AUD_SYNC_SLOT_ALLOC" ,slot);
        // Here the task has been resumed
    }

    if (g_dmAudSyncSlotAry[slot].status != DM_AUD_SYNC_SLOT_ACK)
    {
        //__assert(g_dmAudSyncAssertStrBadSlotStatus);
    }

    result = g_dmAudSyncSlotAry[slot].result;
    g_dmAudSyncSlotAry[slot].status = DM_AUD_SYNC_SLOT_FREE;
    g_dmAudSyncSlotAry[slot].taskId = 0xFF;
    g_dmAudSyncSlotAry[slot].result = 0;
#if (DM_AUD_SYNC_DEBUG_VERBOSE)
    g_dmAudSyncSlotAry[slot].callerAddr = 0;
#endif

    hal_SysExitCriticalSection(scStatus);
 
    return result;
}

PUBLIC VOID DM_SendAudOperResult(UINT32 slot, UINT32 result)
{
    UINT32 scStatus;
    UINT8 slotStatus;
    UINT8 taskId;

    if (slot >= DM_AUD_SYNC_SLOT_NUM)
    {
        __assert(g_dmAudSyncAssertStrBadSlotIndex);
    }

    scStatus = hal_SysEnterCriticalSection();

    slotStatus = g_dmAudSyncSlotAry[slot].status;
    if (slotStatus != DM_AUD_SYNC_SLOT_ALLOC &&
        slotStatus != DM_AUD_SYNC_SLOT_WAIT)
    {
        __assert(g_dmAudSyncAssertStrBadSlotStatus);
    }
#if (DM_AUD_SYNC_DEBUG_VERBOSE)
    if (slotStatus == DM_AUD_SYNC_SLOT_WAIT)
    {
        g_dmAudSyncSlotAry[slot].resumeCnt++;
    }
#endif

    taskId = g_dmAudSyncSlotAry[slot].taskId;
    if (taskId == 0xFF)
    {
        __assert(g_dmAudSyncAssertStrBadTaskId);
    }

    g_dmAudSyncSlotAry[slot].result = result;
    g_dmAudSyncSlotAry[slot].status = DM_AUD_SYNC_SLOT_ACK;

    hal_SysExitCriticalSection(scStatus);

    if (slotStatus == DM_AUD_SYNC_SLOT_WAIT)
    {
        COS_ResumeTask(taskId);
    }
}

// LOL CAT
void Delay_for_audio(UINT32 time)
{
    UINT32 i,j,k;

    for (i=0; i<time; i++)
        for (j=0; j<20; j++)
            for (k=0; k<30; k++)
            {
                __asm("nop");
            }
}

// LOL CAT
VOID dm_sysMSleep(UINT32 n)
{
    hal_TimDelay(n);
}


#if 0
void dm_sysMSleep(UINT32 uMilliSecond)
{
    u32 RTC_Start;
    u32 RTC_End;
    // Very basic delay for non-critical timing
    // This will wait n/256 sec +/- 1/256 sec, so
    // for a delay of at _least_ m/256, use (m+1)
    u32 n=(u32)(uMilliSecond*256/1000);
    RTC_Start = hal_timRTC_GetTime();

    if ((RTC_Start & 0x80000000) != 0)
    {
        // In upper half of the range - readjust...
        RTC_Start &= ~(0x80000000);
        RTC_End = RTC_Start + n;

        while ((hal_timRTC_GetTime() + 0x80000000) < RTC_End);
    }
    else
    {
        RTC_End = RTC_Start + n;

        while (hal_timRTC_GetTime() < RTC_End);
    }
}



CII_AUDIO_TONE  * Audio_Wait(UINT32  time, u8 msgBox)
{
    UINT32  tick_time = (time * 16) / 100;
    CII_AUDIO_TONE      *msg=0;
    UINT32  evt[2];
    sxs_StartTimer(tick_time,PRV_DM_AUDIO_TIMER_ID, NULL, FALSE, msgBox);
msg = sxr_Wait(evt, msgBox); :

    if (msg!=0)
    {
        hal_audio_StopTone();
        sxs_StopTimer(PRV_DM_AUDIO_TIMER_ID, NULL, toneMsgBox);
    }

    return msg;
}
#endif



/*
================================================================================
Function   : audio_Init
--------------------------------------------------------------------------------
Scope      : NOTHING!!!
Parameters : none
Return     : none
================================================================================
*/

#define CLOSED_SPEAKER 0
#define OPENED_SPEAKER 1
#define CLOSED_MIC 0
#define OPENED_MIC 1


BOOL gSpeakerOpen_stasus = CLOSED_SPEAKER;
BOOL gMicOpen_stasus = CLOSED_MIC;

BOOL Speeching = FALSE;

BOOL Speaker_Close(VOID)
{
    if (gSpeakerOpen_stasus == OPENED_SPEAKER)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### speaker  close sucessed  \n"));
        gSpeakerOpen_stasus = CLOSED_SPEAKER;
        Delay_for_audio(5);
        return TRUE;
    }
    else
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### speaker  already close by other user \n"));
        return FALSE;
    }
}
BOOL Speaker_Open(VOID)
{
    if (gSpeakerOpen_stasus == CLOSED_SPEAKER)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  speaker open sucessed  \n"));
        gSpeakerOpen_stasus = OPENED_SPEAKER;
        Delay_for_audio(5);
        return TRUE;
    }
    else
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  speaker already open by other user \n"));
        return FALSE;
    }
}

BOOL Mic_close(VOID)
{
    if (gMicOpen_stasus == OPENED_MIC)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####   close mic sucessed  \n"));
        gMicOpen_stasus = CLOSED_MIC;
        Delay_for_audio(5);
        return TRUE;
    }
    else
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### mic  already close by other user \n"));
        return FALSE;
    }
}
BOOL Mic_open(VOID)
{
    if (gMicOpen_stasus == CLOSED_MIC)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####  mic open sucessed  \n"));
        gMicOpen_stasus = OPENED_MIC;
        Delay_for_audio(5);
        return TRUE;
    }
    else
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### mic  already open by other user \n"));
        return FALSE;
    }
}

UINT16 DTMF_duration = 0;
UINT8 short_time_keytone=0;

#if 0 // tianwq
/*
This function set speaker level for tone play.
*/
LOCAL BOOL DM_SetToneVolumeEx(DM_SpeakerGain nVolume)
{
    /* Check the volume argument. */
    if (nVolume >= DM_AUDIO_SPK_GAIN_QTY)
    {
        nVolume = DM_AUDIO_SPK_18dB;
    }

    if (DM_GetSPKMuteFlag())
    {
        g_spk_gain = nVolume;
        return TRUE;
    }

    CSW_TRACE(CSW_DM_TS_ID, TSTXT("   #### Tone nVolume =%d   \n"), nVolume);
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    audioStreamConfig.volume = nVolume;

    if (audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }

#else

    // save previous audio interface, speaker level, and mic level
    if (cii_tone_sate == DTMF_IDLE)
    {
        audioItfBackup = audioItf;
        spkLevelBackup = audio_cfg.spkLevel;
        micLevelBackup = audio_cfg.micLevel;
    }

    audio_cfg.spkLevel  = nVolume;
    // Mute MIC for keypad tones (and not for other tones) during a call
    if (Speeching == TRUE &&
        cii_currentDTMF.Frequency >= CII_TONE_DTMF_0 &&
        cii_currentDTMF.Frequency <= CII_TONE_DTMF_P)
    {
        audio_cfg.micLevel  = AUD_MIC_MUTE;
    }

    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetToneVolume: spkLevel=%d(%d), micLevel=%d(%d), Speeching=%d"),
        audio_cfg.spkLevel, spkLevelBackup,
        audio_cfg.micLevel, micLevelBackup,
        Speeching);

    /* Update the audio parameters in the drivers. */
    if (Speeching == TRUE)
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
        {
            AUDIO_DELAY(2);

            if (AUD_ERR_NO != vois_Setup(audioItf, &voisCfg))
            {
                return FALSE;
            }
        }
    }
    else
    {
        if (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
        {
            AUDIO_DELAY(2);

            if (AUD_ERR_NO != aud_Setup(audioItf, &audio_cfg))
            {
                return FALSE;
            }
        }
    }

#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

    Delay_for_audio(3);
    return TRUE;
}

LOCAL VOID DM_ToneEndEx(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_ToneEndEx: audioItf=%d(%d), spkLevel=%d(%d), micLevel=%d(%d)"),
        audioItf, audioItfBackup,
        audio_cfg.spkLevel, spkLevelBackup,
        audio_cfg.micLevel, micLevelBackup);

    while (AUD_ERR_NO != aud_ToneStop(audioItf))
    {
        COS_Sleep(1);
    }

    // If previous audio interface, speaker level, or mic level is
    // different, recover them when stopping tone
    if (audioItfBackup != audioItf ||
        spkLevelBackup != audio_cfg.spkLevel ||
        micLevelBackup != audio_cfg.micLevel)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("recover previous audio itf or spk/mic level"));

        audioItf = audioItfBackup;
        audio_cfg.spkLevel = spkLevelBackup;
        audio_cfg.micLevel = micLevelBackup;

        if (Speeching == TRUE)
        {
            VOIS_AUDIO_CFG_T voisCfg =
            {
                .spkLevel = audio_cfg.spkLevel,
                .micLevel = audio_cfg.micLevel,
                .sideLevel = audio_cfg.sideLevel,
                .toneLevel = audio_cfg.toneLevel,
                .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
                .decMute = VPP_SPEECH_UNMUTE
            };

            while (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
            {
                COS_Sleep(1);
            }
        }
        else
        {
            while (aud_Setup(audioItf, &audio_cfg) != VOIS_ERR_NO)
            {
                COS_Sleep(1);
            }
        }
    }

    if ((Speeching == FALSE) && (Play_music ==FALSE))
    {
        Speaker_Close();
    }

    COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
    cii_tone_sate = DTMF_IDLE;
    g_playToneAudioTempTime = 0;
    short_time_keytone=0;
}

VOID DM_ToneTimerHandlerEx(COS_EVENT* pEvent)
{
    UINT32 tick_time = 0;
    BOOL finish = FALSE;

    if (cii_tone_sate == DTMF_IDLE)
    {
        return;
    }

    if (short_time_keytone == 1)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("TIMER-short time tone"));
        DM_ToneEndEx();
        return;
    }

    // for long time key tone
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("TIMER-long time tone"));

    term_num++;
    // check if we finish
    if (term_num > DTMF_ARRAY[cii_currentDTMF.Frequency]->term_num)
    {
        // increase the duration
        DTMF_duration++;
        if (DTMF_duration < cii_currentDTMF.Duration)
        {
            term_num = 0 ;
        }
        else
        {
            finish = TRUE;
        }
    }

    if (g_playToneAudioTempTime >= gTone_Audio_Total_time)
    {
        finish = TRUE;
    }

    if (finish)
    {
        DM_ToneEndEx();
    }
    else
    {
        if (0 == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
        {
            audio_cfg.toneLevel = cii_currentDTMF.level;
            AUD_ERR_T err = aud_ToneStart(audioItf, cii_currentDTMF.Frequency, &audio_cfg);

            if (err != AUD_ERR_NO)
            {
                AUDIO_DELAY(2);
                aud_ToneStart(audioItf, cii_currentDTMF.Frequency, &audio_cfg);
            }
        }
        else if (HAL_AUDIO_SILENCE == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
        {
            aud_TonePause(audioItf, TRUE);
        }
        else
        {
            audio_cfg.toneLevel = cii_currentDTMF.level;
	     aud_ToneStop(audioItf);
            AUD_ERR_T err = aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                          &audio_cfg);

            if (err != AUD_ERR_NO)
            {
                AUDIO_DELAY(2);
		   aud_ToneStop(audioItf);
                aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                              &audio_cfg);
            }
        }

        tick_time = (DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone_term)MILLI_SECOND; //changed by Shawn fix timer use stack define 2005-3-20
        g_playToneAudioTempTime += tick_time ;
        COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_DM_AUDIO_TIMER_ID, COS_TIMER_MODE_SINGLE,tick_time);
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("tone play time: %d/%d/%d"),
                    tick_time*1000/16384,
                    g_playToneAudioTempTime*1000/16384,
                    gTone_Audio_Total_time*1000/16384);
    }
}

VOID DM_ToneMsgHandlerEx(COS_EVENT* pEvent)
{
    UINT32 tick_time = 0;
    DM_SpeakerGain SpeakerLevel = 0;

    // play the firt one term
    switch (pEvent->nParam1>>24)
    {
    case CII_TONE_END:

        if (DTMF_PLAYING == cii_tone_sate)
        {
            CSW_TRACE(CSW_DM_TS_ID, TSTXT("MSG-tone end"));
            DM_ToneEndEx();
        }

        break;

    default:

        // Start a dtmf tone
        if (DTMF_PLAYING == cii_tone_sate)
        {
            COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),PRV_DM_AUDIO_TIMER_ID);
        }

        g_playToneAudioTempTime = 0;
        gTone_Audio_Total_time = pEvent->nParam1&0x00FFFFFF;
        cii_currentDTMF.Duration=100;
        cii_currentDTMF.Frequency=pEvent->nParam1>>24;
        cii_currentDTMF.level=(pEvent->nParam2 & 0xFFFF0000)>>16;
        SpeakerLevel = pEvent->nParam2 & 0x0000FFFF;
        // Set spk level and MIC status
        // NOTE: Must be called AFTER setting cii_currentDTMF.Frequency,
        // as we only mute keypad tones in a speeck call
        DM_SetToneVolumeEx(SpeakerLevel);
        cii_tone_sate = DTMF_PLAYING;
        term_num = 0;
        DTMF_duration = 0;
        // Play the first term of a DTMF
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("EVENT-tone play: playtime=%d,cii_currentDTMF.level=%d,SpeakerLevel=%d"),
            gTone_Audio_Total_time*1000/16384,cii_currentDTMF.level,SpeakerLevel);

        if (gTone_Audio_Total_time >= ((100)MILLI_SECOND)&&gTone_Audio_Total_time<=((300)MILLI_SECOND))
        {
            CSW_TRACE(CSW_DM_TS_ID, TSTXT("start play the short tone time=%d") ,gTone_Audio_Total_time*1000/16384);
            short_time_keytone=1;
        }
        else
        {
            short_time_keytone=0;
        }

        if (0 == DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone)
        {
            audio_cfg.toneLevel = cii_currentDTMF.level;
            AUD_ERR_T err = aud_ToneStart(audioItf, cii_currentDTMF.Frequency,
                                          &audio_cfg);

            if (err != AUD_ERR_NO)
            {
                AUDIO_DELAY(2);
                aud_ToneStart(audioItf, cii_currentDTMF.Frequency,
                              &audio_cfg);
            }
        }
        else
        {
            audio_cfg.toneLevel = cii_currentDTMF.level;
	     aud_ToneStop(audioItf);
            AUD_ERR_T err = aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                                          &audio_cfg);

            if (err != AUD_ERR_NO)
            {
                AUDIO_DELAY(2);
		   aud_ToneStop(audioItf);
                aud_ToneStart(audioItf, DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone,
                              &audio_cfg);
            }
        }

        if (short_time_keytone == 0)
        {
            tick_time = (DTMF_ARRAY[cii_currentDTMF.Frequency]->DTMF_term[term_num].tone_term)MILLI_SECOND;//changed by Shawn fix timer use stack define 2005-3-20
            g_playToneAudioTempTime += tick_time ;
        }
        else
        {
            tick_time = gTone_Audio_Total_time;
            g_playToneAudioTempTime += tick_time ;
        }

        COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,PRV_DM_AUDIO_TIMER_ID, COS_TIMER_MODE_SINGLE,tick_time);
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("tone play time: %d/%d/%d"),
                    tick_time*1000/16384,
                    g_playToneAudioTempTime*1000/16384,
                    gTone_Audio_Total_time*1000/16384);

        break;
    } // end of switch

    DM_SendAudOperResult(pEvent->nParam3, 0);
}

#endif    

/*
================================================================================
Function   : DM_SpeakerOpen
--------------------------------------------------------------------------------
Scope      : Open the speaker.
Parameters : none
Return     : Always success.
================================================================================
*/
BOOL DM_SpeakerOpen(void)
{
    //hal_audio_SpeakerOpen();
    Speaker_Open();
    return TRUE;
}



BOOL DM_MicOpen(void)
{
    //hal_audio_MicOpen();
    Mic_open();
    Delay_for_audio(1);
    return TRUE;
}


/*
================================================================================
Function   : DM_SpeakerClose
--------------------------------------------------------------------------------
Scope      : Close the speaker.
Parameters : none
Return     : Always success.
================================================================================
*/
BOOL DM_SpeakerClose(void)
{
    //hal_audio_SpeakerClose();
    Speaker_Close();
    return TRUE;
}



BOOL DM_MicClose(void)
{
    //hal_audio_MicClose();
    Mic_close();
    return TRUE;
}

BOOL DM_SetMicGain(DM_MicGain nGain)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioVolume\n"));
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    tone_slot = DM_GetAudSyncSlot() ;
    COS_EVENT ev = { EV_DM_SET_MIC_GAIN_IND, nGain };
    COS_SendEvent(MOD_SYS,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    DM_WaitAudOperResult(tone_slot);
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    return TRUE;
}
BOOL DM_SetMicGainEX(DM_MicGain nGain)
{
    BOOL ret=FALSE;

    if (nGain >= DM_AUDIO_MIC_GAIN_QTY)
    {
        return FALSE;
    }

    AUD_ERR_T err;
    audio_cfg.micLevel = nGain;
    err = aud_Setup(audioItf, &audio_cfg);

    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_Setup(audioItf, &audio_cfg);
        ret = (err == AUD_ERR_NO);
    }

    Delay_for_audio(1);
    return ret;
}

/* ********   the follow is the  api supported for the csw  **********************************/


/*The DM_SetAudioMode function set the audio mode required for the connected equipment.

*/
//#define DM_AUDIO_MODE_AUX                    0   // earpiece
//#define DM_AUDIO_MODE_MAIN                1   //main speaker
//#define DM_AUDIO_MODE_LOUD                2   // louder speaker


BOOL SRVAPI DM_SetAudioMode_From_Calibration(UINT8 nAudioMode)
{
    // AUD integrates all that already
    return TRUE;
}

BOOL SRVAPI DM_SetAudioMode(UINT8 nAudioMode)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioMode\n"));
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    tone_slot = DM_GetAudSyncSlot();
    COS_EVENT ev = { EV_DM_SET_AUDIO_MODE_IND, nAudioMode};
    COS_SendEvent(MOD_SYS ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    DM_WaitAudOperResult(tone_slot);
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    return TRUE;
}

/*The DM_SetAudioMode function set the audio mode required for the connected equipment.

*/
//#define DM_AUDIO_MODE_AUX                 0   // earpiece
//#define DM_AUDIO_MODE_MAIN                1   // main speaker
//#define DM_AUDIO_MODE_LOUD                2   // louder speaker

BOOL SRVAPI DM_SetAudioModeEX(UINT8 nAudioMode)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioMode);
    AUD_ITF_T newItf = SND_ITF_RECEIVER;

    /* Check the volume argument. */
    if (nAudioMode > 3) //guohui add blue tooth
    {
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
        return FALSE;
    }

    /* Configure the mode. */
    switch (nAudioMode)
    {
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    case DM_AUDIO_MODE_EARPIECE:
        if(audioItf<=SND_ITF_LOUD_SPEAKER)
            newItf = SND_ITF_EAR_PIECE;
        else if(audioItf==SND_ITF_BT_EP || audioItf==SND_ITF_BT_SP)
            newItf = SND_ITF_BT_EP;
        else
            newItf = audioItf;
        break;
    case DM_AUDIO_MODE_HANDSET:
        newItf = SND_ITF_RECEIVER;
        break;
    case DM_AUDIO_MODE_LOUDSPEAKER:
        if(audioItf<=SND_ITF_LOUD_SPEAKER)
            newItf = SND_ITF_LOUD_SPEAKER;
        else if(audioItf==SND_ITF_BT_EP || audioItf==SND_ITF_BT_SP)
            newItf = SND_ITF_BT_SP;
        else
            newItf = audioItf;
        break;
        //guoh add new audio path
    case DM_AUDIO_MODE_BLUETOOTH:
        newItf = SND_ITF_BLUETOOTH;
        break;
#else
    case DM_AUDIO_MODE_EARPIECE:
        if(audioItf<=AUD_ITF_LOUD_SPEAKER)
            newItf = AUD_ITF_EAR_PIECE;
        else if(audioItf==AUD_ITF_BT_EP || audioItf==AUD_ITF_BT_SP)
            newItf = AUD_ITF_BT_EP;
        else
            newItf = audioItf;
        break;
    case DM_AUDIO_MODE_HANDSET:
        newItf = AUD_ITF_RECEIVER;
        break;
    case DM_AUDIO_MODE_LOUDSPEAKER:
        if(audioItf<=AUD_ITF_LOUD_SPEAKER)
            newItf = AUD_ITF_LOUD_SPEAKER;
        else if(audioItf==AUD_ITF_BT_EP || audioItf==AUD_ITF_BT_SP)
            newItf = AUD_ITF_BT_SP;
        else
            newItf = audioItf;
        break;
        //guoh add new audio path
    case DM_AUDIO_MODE_BLUETOOTH:
        newItf = AUD_ITF_BLUETOOTH;
        break;
#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */
    default:
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
        return FALSE;
    }

    gAudio_mode = nAudioMode;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("set audio mode = %d"),nAudioMode);

    if (cii_tone_sate == DTMF_PLAYING)
    {
        audioItfBackup = newItf;
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
        return TRUE;
    }

    audioItf = newItf;

    /* Update the audio parameters in the drivers. */
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    audioStreamConfig.audioPath = audioItf;

    if (audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }
#else
#ifdef __SUPPORT_BT_PHONE__	
    if(BTSco_AudCallStarted())
    {
	        VOIS_AUDIO_CFG_T voisCfg =
	        {
	            .spkLevel = audio_cfg.spkLevel,
	            .micLevel = audio_cfg.micLevel,
	            .sideLevel = audio_cfg.sideLevel,
	            .toneLevel = audio_cfg.toneLevel,
	            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
	            .decMute = VPP_SPEECH_UNMUTE
	        };

	        while (BTSco_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
	        {
	            COS_Sleep(1);
	        }
    }    
#endif

    /*if (Speeching == TRUE)
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        while (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
        {
            COS_Sleep(1);
        }
    }
    else*/
    {
        int count=0;
        while (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
        {
            if(count++>10)
            {
                CSW_TRACE(0, TSTXT("set audio mode fail, mode = %d, itf = %d"),nAudioMode, audioItf);
                break;
            }
            COS_Sleep(10);
        }
    }
#endif  /* CSW_EXTENDED_API_AUDIO_VIDEO */

    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioMode);
    return TRUE;
}



/*The CFW_GetAudioMode function select the audio hardware set.*/
BOOL SRVAPI DM_GetAudioMode(UINT8* pAudioMode)
{
    *pAudioMode=gAudio_mode;
    return TRUE;
}


BOOL SRVAPI DM_GetSpeakerVolume(UINT8* pVolume)
{
    *pVolume=audio_cfg.spkLevel;
    return TRUE;
}


/*The DM_GetLoudspeakerVolume function retrieves the loudspeaker volume level of the phone.
*/

BOOL SRVAPI DM_GetSpeakerVolume_From_Calibration(UINT8* pVolume)
{
    *pVolume=g_speaker_vlume;
    return TRUE;
}


/*This function adjust speaker volume for voice call purpose.

*/
BOOL SRVAPI DM_SetAudio_From_Calibration_Volume(DM_SpeakerGain nVolume)
{
    // AUD already use the calibrated values
    return TRUE;
}

BOOL SRVAPI DM_SetAudioVolume(DM_SpeakerGain nVolume)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_SetAudioVolume\n"));
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);
    //COS_EVENT ev = { EV_DM_SET_AUDIO_VOLUME_IND, nVolume, 0, DM_GetAudSyncSlot() };
	COS_EVENT ev = { EV_DM_SET_AUDIO_VOLUME_IND,nVolume };
    COS_SendEvent(MOD_SYS ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    //DM_WaitAudOperResult(ev.nParam3);
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    return TRUE;
}

/*This function adjust speaker volume for voice call purpose.

*/
BOOL SRVAPI DM_SetAudioVolumeEX(DM_SpeakerGain nVolume)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_SetAudioVolume);

    /* Check the volume argument. */
    if (nVolume >= AUD_MAX_LEVEL)
    {
        nVolume = AUD_MAX_LEVEL; // DM_AUDIO_SPK_18dB;
        //CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
        //return FALSE;
    }

    CSW_TRACE(CSW_DM_TS_ID, TSTXT("#### nVolume=%d"), nVolume);

    if (DM_GetSPKMuteFlag())
    {
        g_spk_gain = nVolume;
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
        return TRUE;
    }

    if (cii_tone_sate == DTMF_PLAYING)
    {
        spkLevelBackup = nVolume;
        CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
        return TRUE;
    }

#ifdef EXTERNAL_PA_SUPPORT
    hal_HstSendEvent(SYS_EVENT, 0x14011005);
    hal_HstSendEvent(SYS_EVENT, audio_cfg.spkLevel);
    hal_HstSendEvent(SYS_EVENT, nVolume);
    if(audio_cfg.spkLevel == nVolume)
    {
        gpio_SetMute(TRUE);
    }
    if(nVolume == 0)
    {
        gpio_SetMute(TRUE);
    }
    if(audio_cfg.spkLevel == 0&&nVolume == 1)
    {
        gpio_SetMute(TRUE);
    }
#endif

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    audioStreamConfig.volume = nVolume;

    if (audioHandle)
    {
        mps_SetConfig(audioHandle, &audioStreamConfig);
    }

#else
    audio_cfg.spkLevel  = nVolume;
#ifdef __SUPPORT_BT_PHONE__	
    if(BTSco_AudCallStarted())
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        if (BTSco_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
        {
            AUDIO_DELAY(2);

            if (AUD_ERR_NO != BTSco_Setup(audioItf, &voisCfg))
            {
                CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
                return FALSE;
            }
        }
    }
	else
#endif

#if 0// tianwq
    /* Update the audio parameters in the drivers. */
    if (Speeching == TRUE)
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        if (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO)
        {
            AUDIO_DELAY(2);

            if (AUD_ERR_NO != vois_Setup(audioItf, &voisCfg))
            {
                CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
                return FALSE;
            }
        }
    }
    else if (cii_tone_sate == DTMF_IDLE)
#endif   
    {
        if (aud_Setup(audioItf, &audio_cfg) != AUD_ERR_NO)
        {
            AUDIO_DELAY(2);

            if (AUD_ERR_NO != aud_Setup(audioItf, &audio_cfg))
            {
                CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
                return FALSE;
            }
        }
    }

#endif /* CSW_EXTENDED_API_AUDIO_VIDEO */

    Delay_for_audio(3);
    CSW_PROFILE_FUNCTION_EXIT(DM_SetAudioVolume);
    return TRUE;
}

// ARS use event passing
// These DM functions are not used by the MMI
// and thus not maintained. Use MRS/MPS instead
// through MCI if needed.

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
VOID stream_end_Handler(MPS_HANDLE_T handle, MPS_STATE_T state)
{
    COS_EVENT ev;

    if (state == MPS_STATE_EOF)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####               stream_end_Handler    \n"));
        ev.nEventId = EV_CSW_PRV_STREAM_END;
        COS_SendSysEvent(&ev, 0);
        Play_music = FALSE;
    }
}
#else
VOID stream_end_Handler(APBS_STATUS_T status)
{
    if (status == APBS_STATUS_END_BUFFER_REACHED)
    {
        COS_EVENT ev;
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("   ####               stream_end_Handler    \n"));
        ev.nEventId = EV_CSW_PRV_STREAM_END;
        COS_SendSysEvent(&ev, 0);
        Play_music = FALSE;
    }
}
#endif

/*
================================================================================
Function   : CII_MicRecord
--------------------------------------------------------------------------------
Scope      : Start recording a stream.
Parameters : "buffer" is the memory buffer where to record. "uiLen" is the max
number of unsigned int to record.
Return     : ERR_SUCCESS, ERR_DM_INVALID_PARAMETER or ERR_BUSY_DRIVE.
If ERR_DM_INVALID_PARAMETER, the pointer if null, try with an other argument.
If ERR_BUSY_DRIVE, the command cannot be processed now, try later.
================================================================================
*/



BOOL DM_MicStartRecord(UINT32 *pBuffer, UINT32 nBufferSize,DM_PlayRingType codec,BOOL loop_mode)
{
    UINT32 ret_val;
    CSW_PROFILE_FUNCTION_ENTER(DM_MicStartRecord);

    if (pBuffer == NULL)
    {
        return FALSE;
    }

    if ((nBufferSize == 0) || (codec > 13))
    {
        return FALSE;
    }

    //hal_audio_MicOpen();
    mic_record = TRUE;
    Mic_open();
    ret_val = TRUE;
    Delay_for_audio(1);
    CSW_PROFILE_FUNCTION_EXIT(DM_MicStartRecord);

    if (ret_val)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*
================================================================================
Function   : CII_MicStopRecord
--------------------------------------------------------------------------------
Scope      : Stop recording a stream.
Parameters : none
Return     : ERR_SUCCESS or ERR_BUSY_DRIVE.
If ERR_BUSY_DRIVE, the command cannot be processed now, try later.
================================================================================
*/
BOOL DM_MicStopRecord(void)
{
    mic_record = FALSE;

#if (CSW_EXTENDED_API_MRS == 0)
    if (ars_Stop() != ARS_ERR_NO)
    {
        AUDIO_DELAY(2);
        return (ars_Stop() == ARS_ERR_NO);
    }
#endif
    return TRUE;
}

VOID DM_SetSPKMuteFlag(BOOL muteflag)
{
    isSPKMute = muteflag;
}
BOOL DM_GetSPKMuteFlag()
{
    return isSPKMute;
}
BOOL DM_MuteSpeaker(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_MuteSpeaker \n"));
    DM_SetSPKMuteFlag(TRUE);

    if (cii_tone_sate == DTMF_PLAYING)
    {
        g_spk_gain = spkLevelBackup;
        spkLevelBackup = AUD_SPK_MUTE;
        return TRUE;
    }

    g_spk_gain = audio_cfg.spkLevel;
    audio_cfg.spkLevel = AUD_SPK_MUTE;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#else
    APBS_AUDIO_CFG_T apsAudioCfg =
    {
        .spkLevel = audio_cfg.spkLevel,
        .filter = NULL
    };
    return (apbs_Setup(audioItf,&apsAudioCfg) == AUD_ERR_NO);
    // return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#endif
}

BOOL DM_ResumSpeaker(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_ResumSpeaker \n"));
    DM_SetSPKMuteFlag(FALSE);

    if (cii_tone_sate == DTMF_PLAYING)
    {
        spkLevelBackup = g_spk_gain;
        return TRUE;
    }

    audio_cfg.spkLevel = g_spk_gain;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#else
    APBS_AUDIO_CFG_T apsAudioCfg =
    {
        .spkLevel = audio_cfg.spkLevel,
        .filter = NULL
    };
    return (apbs_Setup(audioItf,&apsAudioCfg) == AUD_ERR_NO);
    //return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
#endif
}

BOOL dm_MuteMic(VOID)
{
    if (cii_tone_sate == DTMF_PLAYING)
    {
        g_mic_gain = micLevelBackup;
        micLevelBackup = AUD_MIC_MUTE;
        return TRUE;
    }

    g_mic_gain = audio_cfg.micLevel;
    audio_cfg.micLevel = AUD_MIC_MUTE;
#ifdef __SUPPORT_BT_PHONE__	
    if(BTSco_AudCallStarted())
	{
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        while (BTSco_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);

        return TRUE;
    }
    else 
#endif
	
#if 0 // tianwq
    if (Speeching == TRUE)
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        while (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);

        return TRUE;
    }
    else
#endif
    {
        return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
    }
}

BOOL dm_ResumMic(VOID)
{
    if (cii_tone_sate == DTMF_PLAYING)
    {
        micLevelBackup = g_mic_gain;
        return TRUE;
    }

    audio_cfg.micLevel = g_mic_gain;
	#ifdef __SUPPORT_BT_PHONE__		
    if(BTSco_AudCallStarted())
	{
	        VOIS_AUDIO_CFG_T voisCfg =
	        {
	            .spkLevel = audio_cfg.spkLevel,
	            .micLevel = audio_cfg.micLevel,
	            .sideLevel = audio_cfg.sideLevel,
	            .toneLevel = audio_cfg.toneLevel,
	            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
	            .decMute = VPP_SPEECH_UNMUTE
	        };

	        while (BTSco_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);

	        return TRUE;
	}
    else
#endif
#if 0 // tianwq
    if (Speeching == TRUE)
    {
        VOIS_AUDIO_CFG_T voisCfg =
        {
            .spkLevel = audio_cfg.spkLevel,
            .micLevel = audio_cfg.micLevel,
            .sideLevel = audio_cfg.sideLevel,
            .toneLevel = audio_cfg.toneLevel,
            .encMute = (audio_cfg.micLevel)?VPP_SPEECH_UNMUTE:VPP_SPEECH_MUTE,
            .decMute = VPP_SPEECH_UNMUTE
        };

        while (vois_Setup(audioItf, &voisCfg) != VOIS_ERR_NO);

        return TRUE;
    }
    else
#endif
    {
        return (aud_Setup(audioItf, &audio_cfg) == AUD_ERR_NO);
    }
}


//
// Control Audio.
// Mute Mic .
//
BOOL DM_SetAudioControl(UINT8 nCmd, UINT16 nValue)
{
    if (DM_AUD_IOCTL_MUTE_MIC == nCmd)
    {
        dm_MuteMic();
    }
    else if (DM_AUD_IOCTL_UNMUTE_MIC == nCmd)
    {
        dm_ResumMic();
    }
    else if (DM_AUD_IOCTL_MUTE_SPK == nCmd)
    {
        DM_MuteSpeaker();
    }
    else if (DM_AUD_IOCTL_UNMUTE_SPK == nCmd)
    {
        DM_ResumSpeaker();
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//
// Close Speaker and Mic
//
BOOL DM_StopAudio(VOID)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_StopAudio\n"));
    CSW_PROFILE_FUNCTION_ENTER(DM_StopAudio);
#if (CFW_AUDIO_CONTROL_BYPASS_SYNC)
    COS_EVENT ev = { EV_DM_AUDIO_CONTROL_IND, 0};
    COS_SendEvent(MOD_SYS ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#else
    tone_slot = DM_GetAudSyncSlot(); 
    COS_EVENT ev = { EV_DM_AUDIO_CONTROL_IND, 0};
    COS_SendEvent(MOD_SYS ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    DM_WaitAudOperResult(tone_slot);
#endif
    CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
    return TRUE;
}

BOOL DM_StopAudioEX(VOID)
{
    COS_EVENT ev;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_StopAudioEX \n"));
    CSW_PROFILE_FUNCTION_ENTER(DM_StopAudio);
  #ifdef __SUPPORT_BT_PHONE__	
    if(BTSco_AudCallStarted())
    	{
	    while (BTSco_Stop() != VOIS_ERR_NO)
	    {
	        COS_Sleep(1);
	    }
		BTSco_AudCallReset();

	    //hal_audio_SpeakerClose();
	    Speaker_Close();
	    //hal_audio_MicClose();
	    Mic_close();
	    Delay_for_audio(1);
	    audioBusy = 0;
	    CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
	    return TRUE;
	}
#endif	

    if (Speeching == FALSE)
    {
        return TRUE;
    }

    Speeching = FALSE;

// tianwq    while (vois_Stop() != VOIS_ERR_NO)
    {
        COS_Sleep(1);
    }

    //hal_audio_SpeakerClose();
    Speaker_Close();
    //hal_audio_MicClose();
    Mic_close();
    Delay_for_audio(1);
    audioBusy = 0;
    ev.nEventId = EV_DM_SPEECH_IND;//// when stop call, this ind will send to mmi task
    ev.nParam1  = 0;
//    ev.nParam2  = 0;//for delete nParam2
//    ev.nParam3  = 0;//for delete nParam3
    COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    CSW_PROFILE_FUNCTION_EXIT(DM_StopAudio);
    return TRUE;
}

BOOL DM_StartAudio(VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_StartAudio);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_StartAudio\n"));
#if (CFW_AUDIO_CONTROL_BYPASS_SYNC)
    COS_EVENT ev = { EV_DM_AUDIO_CONTROL_IND, 1};
    COS_SendEvent(MOD_SYS ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
#else
    tone_slot = DM_GetAudSyncSlot();
    COS_EVENT ev = { EV_DM_AUDIO_CONTROL_IND, 1};
    COS_SendEvent(MOD_SYS  ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    DM_WaitAudOperResult(tone_slot);//modified for delete nParam3
#endif
    CSW_PROFILE_FUNCTION_EXIT(DM_StartAudio);
    return TRUE;
}

int sco_ret = 0;

BOOL DM_StartAudioEX(VOID)
{
    BOOL ret = TRUE;
    UINT8 times_delay = 0;
    COS_EVENT ev;
    CSW_PROFILE_FUNCTION_ENTER(DM_StartAudio);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" csw DM_StartAudioEX \n"));
    //hal_audio_SpeakerOpen();
    Speaker_Open();
    //hal_audio_MicOpen();
    Mic_open();
    // hal_audio_StopTone();
    Delay_for_audio(1);
#ifdef __SUPPORT_BT_PHONE__	
    if(BTSco_AudCallStarted())
    	{
    	    VOIS_AUDIO_CFG_T voisCfg = 
                {
                    .spkLevel = audio_cfg.spkLevel,
                    .micLevel = audio_cfg.micLevel,
                    .sideLevel = audio_cfg.sideLevel,
                    .toneLevel = audio_cfg.toneLevel,
                    .encMute = VPP_SPEECH_UNMUTE,
                    .decMute = VPP_SPEECH_UNMUTE
                };
                sco_ret = BTSco_Start(audioItf, &voisCfg);
	    //while (BTSco_Start(audioItf, &voisCfg) != VOIS_ERR_NO)
                while(sco_ret != VOIS_ERR_NO)
                {
	        times_delay++;
            COS_Sleep(5);

	        if (times_delay > 10)
	        {
	            ret = FALSE;
	            break;
	        }
                        sco_ret = BTSco_Start(audioItf, &voisCfg);
	    }

	    AUDIO_DELAY(2);
	    Delay_for_audio(5);

	    CSW_PROFILE_FUNCTION_EXIT(DM_StartAudio);
	    return ret;
	}
#endif	
#if 0 // tianwq
    VOIS_AUDIO_CFG_T voisCfg =
    {
        .spkLevel = audio_cfg.spkLevel,
        .micLevel = audio_cfg.micLevel,
        .sideLevel = audio_cfg.sideLevel,
        .toneLevel = audio_cfg.toneLevel,
        .encMute = VPP_SPEECH_UNMUTE,
        .decMute = VPP_SPEECH_UNMUTE
    };

    while (vois_Start(audioItf, &voisCfg) != VOIS_ERR_NO)
    {
        times_delay++;
        sxr_Sleep(5 * 16384 /1000);

        if (times_delay > 10)
        {
            ret = FALSE;
            break;
        }
    }
#endif    
    AUDIO_DELAY(2);
    Delay_for_audio(5);

    if (ret == TRUE)
    {
        Speeching = TRUE;
        audioBusy = 1;
        ev.nEventId = EV_DM_SPEECH_IND;// when start call, this ind will send to mmi task
        ev.nParam1  = 1;
//        ev.nParam2  = 0;//for delete nParam2
 //       ev.nParam3  = 0;//for delete nParam3
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }

    CSW_PROFILE_FUNCTION_EXIT(DM_StartAudio);
    return ret;
}


/*
================================================================================
Function   : DM_Audio_StartStream
--------------------------------------------------------------------------------
Scope      :  Manager the play of stream
Parameters :
Return     : True or False.
================================================================================
*/

BOOL    DM_Audio_StartStream(UINT32*    buffer, UINT32  len, DM_PlayRingType codec, BOOL loop_mode)
{
    if (Speeching == TRUE)
    {
        return TRUE;
    }

    CSW_PROFILE_FUNCTION_ENTER(DM_Audio_StartStream);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" DM_Audio_StartStream \n"));
    Play_music = TRUE;
    Speaker_Open();
    BOOL ret=FALSE;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)

    if (len == 0)
#else
    if ((len == 0) || (codec > 13))
#endif
    {
        return FALSE;
    }

#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    mps_Open();
    g_audioPlayBufStart   = (UINT32) buffer;
    g_audioPlayBufLen     = len*4;

    if (audioHandle == 0)
    {
        mps_OpenBuffer(&audioHandle, (VOID*)buffer, len*4, stream_end_Handler, &audioStreamConfig);

        if (audioHandle != 0)
        {
            if (mps_Play(audioHandle, (loop_mode) ? 0xFFFF : 0) == MPS_ERR_NO)
            {
                ret = TRUE;
            }
        }
    }

#else
    g_audioPlayBufStart   = (UINT32) buffer;
    g_audioPlayBufLen     = len*4;
    // Current offset from start
    g_audioPlayBufOffset  = 0;
    APBS_ENC_STREAM_T stream;
    APBS_ERR_T streamError;
    APBS_AUDIO_CFG_T apsAudioCfg =
    {
        .spkLevel = audio_cfg.spkLevel,
        .filter = NULL
    };
    stream.startAddress = (UINT32)buffer;
    stream.length = len*4; // ? Modem2g apiuses bytes to define buffer
    // size.
    stream.sampleRate = HAL_AIF_FREQ_8000HZ;
    stream.mode = codec;
    stream.channelNb = HAL_AIF_MONO;
    stream.voiceQuality = TRUE;

    if (loop_mode)
    {
        // In loop mode, No interrupt is expected
        stream.handler = NULL;
    }
    else
    {
        stream.handler = stream_end_Handler;
    }

    streamError = apbs_Play(audioItf, &stream, &apsAudioCfg, loop_mode);

    if (streamError != APBS_ERR_NO)
    {
        AUDIO_DELAY(2);
        streamError = apbs_Play(audioItf, &stream, &apsAudioCfg, loop_mode);
    }

    ret = (streamError == APBS_ERR_NO);
#endif
    Delay_for_audio(1);

    if (ret == TRUE)
    {
        audioBusy = 1;
    }

    CSW_PROFILE_FUNCTION_EXIT(DM_Audio_StartStream);
    return ret;
}

VOID DM_Set_SpeechingFlag(BOOL flag)  // according guojian's advice. for bug10462
{
    Speeching = flag;
}
BOOL    DM_Audio_StopStream(void)
{
    if (Speeching == TRUE)
    {
        return TRUE;
    }

    CSW_PROFILE_FUNCTION_ENTER(DM_Audio_StopStream);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT(" DM_Audio_StopStream \n"));
    BOOL ret=FALSE;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 1)
    ret = (MPS_ERR_NO == mps_CloseBuffer(audioHandle));
    audioHandle = 0;
#else
    APBS_ERR_T streamError;
    streamError = apbs_Stop();

    if (streamError != APBS_ERR_NO)
    {
        AUDIO_DELAY(2);
        streamError = apbs_Stop();
    }

    ret = (streamError == APBS_ERR_NO);
#endif
    //hal_audio_SpeakerClose();
    Speaker_Close();
    //#ifdef c1000
    Play_music = FALSE;
    //#endif
    Delay_for_audio(1);
    audioBusy = 0;
    CSW_PROFILE_FUNCTION_EXIT(DM_Audio_StopStream);
    return ret;
}

/*This function plays the tone specified.
The caller can request the sound to be repeated, and the time between repeats.

*/

BOOL SRVAPI DM_PlayTone(UINT8 nToneType, UINT8 nToneAttenuation,UINT16 nDuration,DM_SpeakerGain volumn)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_PlayTone);
    COS_EVENT mtone;
    BOOL ret=0;
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_PlayTone"));

    if (TRUE == Play_music)
    {
        return FALSE;
    }

    if (nToneType > CII_TONE_END)
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Unknown nToneType in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }

    if (nDuration < 100)
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Too small  nDuration in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }

    if (nToneAttenuation >= DM_TONE_GAIN_QTY || volumn >= DM_AUDIO_SPK_GAIN_QTY)
    {
        CSW_TRACE(CSW_DM_TS_ID, ("Unknown   nToneAttenuation or volume level in AUDIO\n"));
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return FALSE;
    }

    if (volumn == DM_AUDIO_SPK_MUTE)
    {
        // Do nothing since the tone will be muted
        CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);
        return TRUE;
    }

    mtone.nEventId=SYS_EV_MASK_AUDIO_TONE;
    mtone.nParam1=((nDuration)MILLI_SECOND)&0x00FFFFFF;
    mtone.nParam1|=(nToneType<<24);
//    mtone.nParam2 = (nToneAttenuation<<16) | volumn;//for delete nParam2

//    mtone.nParam3 = DM_GetAudSyncSlot();//for delete nParam3
    tone_slot = DM_GetAudSyncSlot();
    Speaker_Open();
    ret=COS_SendEvent(MOD_SYS,&mtone,COS_WAIT_FOREVER,COS_EVENT_PRI_NORMAL);
//    DM_WaitAudOperResult(mtone.nParam3);//for delete nParam3
    DM_WaitAudOperResult(tone_slot);
    CSW_PROFILE_FUNCTION_EXIT(DM_PlayTone);

    return TRUE;
}


/*This function stops the current playing tone.

 */
BOOL DM_StopTone(VOID)
{
    CSW_PROFILE_FUNCTION_ENTER(DM_StopTone);
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_StopTone"));
    COS_EVENT mtone;
    mtone.nEventId=SYS_EV_MASK_AUDIO_TONE;
    mtone.nParam1=(CII_TONE_END<<24);
//    mtone.nParam3 = DM_GetAudSyncSlot();//for delete nParam3
    tone_slot = DM_GetAudSyncSlot();
    COS_SendEvent(MOD_SYS ,&mtone, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
//    DM_WaitAudOperResult(mtone.nParam3);//for delete nParam3
    DM_WaitAudOperResult(tone_slot);
    CSW_PROFILE_FUNCTION_EXIT(DM_StopTone);
    return TRUE;
}

#if defined(LOOPBACK_TEST_MODE)
extern AUD_ERR_T aud_TestModeSetup(CONST SND_ITF_T         itf,
                                   CONST HAL_AIF_STREAM_T* stream,
                                   CONST AUD_LEVEL_T *     cfg,
                                   AUD_TEST_T              mode);

VOID DM_Audio_Side_test(UINT8 mode)
{
    CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test"));
    // Local variable passed to a function, but copied in the function.
    HAL_AIF_STREAM_T sideAudioStream;

    if (mode == DM_AUDIO_MODE_LOUDSPEAKER)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_LOUDSPEAKER"));
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_LOUD_SPEAKER;
    }
    else if (mode == DM_AUDIO_MODE_EARPIECE)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_EARPIECE"));
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_EAR_PIECE;
    }
    else // (mode == DM_AUDIO_MODE_HANDSET)
    {
        CSW_TRACE(CSW_DM_TS_ID, TSTXT("DM_Audio_Side_test mode DM_AUDIO_MODE_HANDSET"));
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
        audioItf            = AUD_ITF_RECEIVER;
    }

    sideAudioStream.startAddress = NULL;
    sideAudioStream.length       = 0;
    sideAudioStream.sampleRate   = HAL_AIF_FREQ_8000HZ;
    sideAudioStream.channelNb    = HAL_AIF_MONO;
    sideAudioStream.voiceQuality = TRUE;
    sideAudioStream.playSyncWithRecord = FALSE;
    sideAudioStream.halfHandler  = NULL;
    sideAudioStream.endHandler   = NULL;
    aud_TestModeSetup(audioItf, &sideAudioStream, &audio_cfg, AUD_TEST_SIDE_TEST);
}
#endif /* LOOPBACK_TEST_MODE */


#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)

BOOL DM_GetStreamStatus(VOID)
{
    return Play_music;
}

BOOL DM_GetAudioStatus(VOID)
{
    return audioBusy;
}

#endif

/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words yet to be played in the stream buffer.
Parameters : none
Return     : The number of words yet to be played in the stream buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingStream(VOID)
{
    UINT32 ret;
#if (CSW_EXTENDED_API_AUDIO_VIDEO == 0)
    UINT32 bufPos = (UINT32) apbs_GetBufPosition();
    UINT32 newOffset;
    ret = g_audioPlayBufStart + g_audioPlayBufLen - bufPos;
    newOffset = bufPos - g_audioPlayBufStart;
    UINT32 virtualAddedData = 0;

    if (newOffset > g_audioPlayBufOffset)
    {
        virtualAddedData = newOffset - g_audioPlayBufOffset;
        g_audioPlayBufOffset = newOffset;
    }
    else
    {
        virtualAddedData = g_audioPlayBufLen - g_audioPlayBufOffset + newOffset;
        g_audioPlayBufOffset = newOffset;
    }

#if 1
    apbs_AddedData(virtualAddedData);
#endif
#else
    ret = 0;
#endif
    return ret;
}


/*
================================================================================
Function   : DM_Audio_RemainingStream
--------------------------------------------------------------------------------
Scope      : Returns the number of words that can still be written in the record buffer.
Parameters : none
Return     : The number of words that can still be written in the record buffer.
================================================================================
*/
UINT32 DM_Audio_RemainingRecord(VOID)
{
    UINT32 ret = 0;
#if (CSW_EXTENDED_API_MRS == 0)
    UINT32 readData = ars_AvailableData();
    ars_ReadData(readData);
    g_audioRecBufOffset = (g_audioRecBufOffset + readData) % g_audioRecBufLen;
    ret = g_audioRecBufLen - g_audioRecBufOffset;
#endif
    return ret;
}

BOOL DM_Audio_PauseStream(VOID)
{
    AUD_ERR_T err;
    err = aud_StreamPause(audioItf, TRUE);

    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_StreamPause(audioItf, TRUE);
    }

    return (err == AUD_ERR_NO);
}

BOOL DM_Audio_ResumeStream(VOID)
{
    AUD_ERR_T err;
    err = aud_StreamPause(audioItf, FALSE);

    if (err != AUD_ERR_NO)
    {
        AUDIO_DELAY(2);
        err = aud_StreamPause(audioItf, FALSE);
    }

    return (err == AUD_ERR_NO);
}

BOOL DM_Close_All_Audio(VOID)
{
    BOOL ret=FALSE;
    DM_StopAudio();
    Speeching = FALSE;
    ret = DM_Audio_StopStream();
    return ret;
}

#if 0

// -------------------------------------------------------
// DM Audio Implementaions that are needed to 
// synchronize in CSW task

BOOL DM_AudSetupEx(CONST AUD_LEVEL_T* cfg)
{
    AUD_ERR_T errStatus;
    errStatus = aud_Setup(audioItf, cfg);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudStreamStartEx(CONST DM_AUD_STREAM_START_PARAM_T* param)
{
    AUD_ERR_T errStatus;
    errStatus = aud_StreamStart(audioItf, param->stream, param->cfg);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudStreamStopEx(VOID)
{
    AUD_ERR_T errStatus;
    errStatus = aud_StreamStop(audioItf);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudStreamPauseEx(BOOL pause)
{
    AUD_ERR_T errStatus;
    errStatus = aud_StreamPause(audioItf, pause);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudStreamRecordEx(CONST DM_AUD_STREAM_RECORD_PARAM_T* param)
{
    AUD_ERR_T errStatus;
    errStatus = aud_StreamRecord(audioItf, param->stream, param->cfg);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudTestModeSetupEx(CONST DM_AUD_TEST_MODE_SETUP_PARAM_T* param)
{
    AUD_ERR_T errStatus;
    errStatus = aud_LineinRecordSetup(audioItf, param->stream, param->cfg, param->mode);
    return (errStatus == AUD_ERR_NO);
}

BOOL DM_AudForceReceiverMicSelectionEx(BOOL on)
{
    aud_ForceReceiverMicSelection(on);
    return TRUE;
}

BOOL DM_AudSetCurOutputDeviceEx(SND_ITF_T itf)
{
    aud_SetCurOutputDevice(itf);
    return TRUE;
}

BOOL DM_AudMuteOutputDeviceEx(BOOL mute)
{
#ifdef AUD_MUTE_FOR_BT_WORKAROUND
    extern VOID aud_MuteOutputDevice(BOOL mute);
    aud_MuteOutputDevice(mute);
#endif
    return TRUE;
}

BOOL DM_AudLoudspeakerWithEarpieceEx(BOOL on)
{
    aud_LoudspeakerWithEarpiece(on);
    return TRUE;
}

BOOL DM_VoisRecordStartEx(CONST DM_VOIS_RECORD_START_PARAM_T* param)
{
    VOIS_ERR_T errStatus;
    errStatus = vois_RecordStart(param->startAddress, param->length, param->handler);
    return (errStatus == VOIS_ERR_NO);
}

BOOL DM_VoisRecordStopEx(VOID)
{
    VOIS_ERR_T errStatus;
    errStatus = vois_RecordStop();
    return (errStatus == VOIS_ERR_NO);
}

BOOL DM_ArsSetupEx(CONST ARS_AUDIO_CFG_T* cfg)
{
    ARS_ERR_T errStatus;
    errStatus = ars_Setup(audioItf, cfg);
    return (errStatus == ARS_ERR_NO);
}

BOOL DM_ArsRecordEx(CONST DM_ARS_RECORD_PARAM_T* param)
{
    ARS_ERR_T errStatus;
    errStatus = ars_Record(audioItf, param->stream, param->cfg, param->loop);
    return (errStatus == ARS_ERR_NO);
}

BOOL DM_ArsPauseEx(BOOL pause)
{
    ARS_ERR_T errStatus;
    errStatus = ars_Pause(pause);
    return (errStatus == ARS_ERR_NO);
}

BOOL DM_ArsStopEx(VOID)
{
    ARS_ERR_T errStatus;
    errStatus = ars_Stop();
    return (errStatus == ARS_ERR_NO);
}

// -------------------------------------------------------
// APIs for MMI/MDI

BOOL DM_AudSetup(CONST AUD_LEVEL_T* cfg)
{
    COS_EVENT ev = { EV_DM_AUD_SETUP_IND, (UINT32)cfg, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudStreamStart(CONST HAL_AIF_STREAM_T* stream, CONST AUD_LEVEL_T* cfg)
{
    DM_AUD_STREAM_START_PARAM_T param = { stream, cfg };
    COS_EVENT ev = { EV_DM_AUD_STREAM_START_IND,(UINT32)&param, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudStreamStop(VOID)
{
    COS_EVENT ev = { EV_DM_AUD_STREAM_STOP_IND, 0, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudStreamPause(BOOL pause)
{
    COS_EVENT ev = { EV_DM_AUD_STREAM_PAUSE_IND, pause, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudStreamRecord(CONST HAL_AIF_STREAM_T* stream, CONST AUD_LEVEL_T* cfg)
{
    DM_AUD_STREAM_RECORD_PARAM_T param = { stream, cfg };
    COS_EVENT ev = { EV_DM_AUD_STREAM_RECORD_IND, (UINT32)&param, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudTestModeSetup(CONST HAL_AIF_STREAM_T* stream, CONST AUD_LEVEL_T* cfg, AUD_TEST_T mode)
{
    DM_AUD_TEST_MODE_SETUP_PARAM_T param = { stream, cfg, mode };
    COS_EVENT ev = { EV_DM_AUD_TEST_MODE_SETUP_IND, (UINT32)&param, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudForceReceiverMicSelection(BOOL on)
{
    COS_EVENT ev = { EV_DM_AUD_FORCE_RECV_MIC_SEL_IND, on, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudSetCurOutputDevice(SND_ITF_T itf)
{
    COS_EVENT ev = { EV_DM_AUD_SET_CUR_OUTPUT_DEVICE_IND, (UINT32)itf, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudMuteOutputDevice(BOOL mute)
{
    COS_EVENT ev = { EV_DM_AUD_MUTE_OUTPUT_DEVICE_IND, mute, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_AudLoudspeakerWithEarpiece(BOOL on)
{
    COS_EVENT ev = { EV_DM_AUD_LOUDSPK_WITH_EP_IND, on, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_VoisRecordStart(INT32 *startAddress, INT32 length, VOIS_USER_HANDLER_T handler)
{
    DM_VOIS_RECORD_START_PARAM_T param = { startAddress, length, handler };
    COS_EVENT ev = { EV_DM_VOIS_RECORD_START_IND, (UINT32)&param, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_VoisRecordStop(VOID)
{
    COS_EVENT ev = { EV_DM_VOIS_RECORD_STOP_IND, 0, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_ArsSetup(CONST ARS_AUDIO_CFG_T* cfg)
{
    COS_EVENT ev = { EV_DM_ARS_SETUP_IND, (UINT32)cfg, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_ArsRecord(CONST ARS_ENC_STREAM_T* stream, CONST ARS_AUDIO_CFG_T* cfg, BOOL loop)
{
    DM_ARS_RECORD_PARAM_T param= { stream, cfg, loop };
    COS_EVENT ev = { EV_DM_ARS_RECORD_IND, (UINT32)&param, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_ArsPause(BOOL pause)
{
    COS_EVENT ev = { EV_DM_ARS_PAUSE_IND, pause, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}

BOOL DM_ArsStop(VOID)
{
    COS_EVENT ev = { EV_DM_ARS_STOP_IND, 0, 0, DM_GetAudSyncSlot() };
    COS_SendEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,&ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return DM_WaitAudOperResult(ev.nParam3);
}


// -------------------------------------------------------
// APIs for MMI/MDI that do NOT need to run in CSW task

SND_ITF_T DM_AudGetCurOutputDevice(VOID)
{
    return aud_GetCurOutputDevice();
}

UINT32* DM_ArsGetBufPosition(VOID)
{
    return ars_GetBufPosition();
}

BOOL DM_ArsReadData(UINT32 addedDataBytes)
{
    ARS_ERR_T errStatus;
    errStatus = ars_ReadData(addedDataBytes);
    return (errStatus == ARS_ERR_NO);
}

UINT32 DM_ArsAvailableData(VOID)
{
    return ars_AvailableData();
}

#endif
