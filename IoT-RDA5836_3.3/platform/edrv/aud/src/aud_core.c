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
//                                                                            //
/// @file                                                                     //
/// That file describes the audio driver interface.                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "tgt_aud_cfg.h"

#include "aud_m.h"
#include "dm_audio.h"

#include "hal_aif.h"
#include "hal_sys.h"
#include "hal_rda_audio.h"

#include "hal_timers.h"
#include "sxr_tls.h"

#include "audp_debug.h"
#include "tgt_app_cfg.h"
#define AUD_MUTEX_FOR_IRQ 0

#if (AUD_MUTEX_FOR_IRQ)
extern VOID hal_GetK1(UINT32* k1);
#else
#include "sxr_sbx.h"
#endif
#include "cos.h"

extern uint32 *pcmbuf_overlay;
// =============================================================================
//  MACROS
// =============================================================================

#if (AUD_MUTEX_FOR_IRQ)
#define AUD_TIME_SEM_WAIT_INTERVAL (5)
#endif

#define SND_ITF_HAS_PHY_SPK(itf) \
    (itf == SND_ITF_RECEIVER || itf == SND_ITF_EAR_PIECE || itf == SND_ITF_LOUD_SPEAKER || itf == SND_ITF_BT_EP || itf == SND_ITF_BT_SP)

// 0.5 second for 8K Hz sample rate (16 bits per sample)
#define AUD_SIDE_TEST_BUF_LENGTH  8000

// 64 ms for 8K Hz sample rate (16 bits per sample)
#define AUD_RECVMIC_IN_EARPIECE_OUT_BUF_LENGTH  1024

extern PUBLIC INT16 aud_CodecCommonGetOutAlgGainDb2Val(VOID);
// =============================================================================
//  GLOBALS
// =============================================================================

PUBLIC BOOL g_audSetHeadMaxDigitalGain = FALSE;
PUBLIC BOOL g_audSetMicMaxGain = FALSE;

#if defined(LOOPBACK_TEST_MODE)
PRIVATE AUD_TEST_T  g_audRunningTest        = AUD_TEST_NO;
#endif
PRIVATE SND_ITF_T   g_audEnabledRecordItf   = SND_ITF_NONE;
PRIVATE SND_ITF_T   g_audEnabledPlayItf     = SND_ITF_NONE;
PRIVATE SND_ITF_T   g_audEnabledToneItf     = SND_ITF_NONE;

#if (AUD_MUTEX_FOR_IRQ)
/// status being configured
PRIVATE VOLATILE BOOL g_audBeingConfigured  = FALSE;
#else
PRIVATE BYTE        g_audSemaphore          = 0xFF;
#endif

/// Current output device for FM/TV
PRIVATE SND_ITF_T   g_audCurOutputDevice    = SND_ITF_NONE;

/// Whether to select loud speaker output along with earpiece
PRIVATE BOOL g_audLoudspeakerWithEarpiece   = FALSE;

extern UINT8 gAudio_mode;

// Test mode record buffer 
//PRIVATE UINT8 *g_audTestModeRecordBuffer = NULL;

// Test mode buffer malloc
PRIVATE AUD_TEST_MODE_MALLOC_FUNC_T g_audTestModeMallocFuncPtr = NULL;

// Test mode buffer free
PRIVATE AUD_TEST_MODE_FREE_FUNC_T g_audTestModeFreeFuncPtr = NULL;


#if (AUDIO_CALIB_VER == 1)
SND_ITF_T musicItf = SND_ITF_LOUD_SPEAKER;
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================

extern UINT32 MCI_SetSideTone(UINT32 sample_rate, UINT32 channel);
// =============================================================================
// aud_SemTake
// -----------------------------------------------------------------------------
/// Take the semaphore protecting AUD shared variables.
///
/// @return TRUE if the semaphore was free, and thus taken by that call, or
/// FALSE if the resource is not available.
// =============================================================================
PRIVATE BOOL aud_SemTake(VOID)
{
#if (AUD_MUTEX_FOR_IRQ)
    UINT32 k1 = 0;
    hal_GetK1(&k1);

    UINT32 status = hal_SysEnterCriticalSection();
    while (g_audBeingConfigured)
    {
        if (k1 > 0)
        {
            // Interrupt can NOT sleep or wait
            hal_SysExitCriticalSection(status);
            return FALSE;
        }
        else
        {
            // Task can sleep
            COS_Sleep(AUD_TIME_SEM_WAIT_INTERVAL);
        }
    }
    g_audBeingConfigured = TRUE;
    hal_SysExitCriticalSection(status);
#else
    // Create the semaphore if it doesn't exist.
    UINT32 status = hal_SysEnterCriticalSection();
    if (g_audSemaphore == 0xFF)
    {
     #if OS_KERNEL==SXR
        g_audSemaphore = sxr_NewSemaphore(1); 
     #else
        g_audSemaphore = COS_CreateSemaphore(1);
     #endif
    }
    hal_SysExitCriticalSection(status);

 #if OS_KERNEL==SXR
    sxr_TakeSemaphore(g_audSemaphore);
 #else
    COS_WaitForSemaphore(g_audSemaphore, 1000);
 #endif
#endif

    return TRUE;
}


// =============================================================================
// aud_SemFree
// -----------------------------------------------------------------------------
/// Free the semaphore protecting AUD shared variables.
// =============================================================================
PRIVATE VOID aud_SemFree(VOID)
{
#if (AUD_MUTEX_FOR_IRQ)
    g_audBeingConfigured = FALSE;
#else
 #if OS_KERNEL==SXR
    sxr_ReleaseSemaphore(g_audSemaphore);
 #else
    COS_ReleaseSemaphore(g_audSemaphore);
 #endif
#endif
}


// =============================================================================
// aud_Setup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker, enabling of the microphone ... It can be used to change
/// 'on the fly' the input or output used to play/record a stream on the 
/// same interface, but cannot be used to change the interface during a
/// playback or a record.
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_LEVEL_T for more details.
/// @return #AUD_ERR_NO if it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_Setup(SND_ITF_T itf, CONST AUD_LEVEL_T* cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");

#if defined(AUD_SPK_ON_WITH_LCD) || defined(AUD_EXT_PA_ON_WITH_LCD)
    AUD_ASSERT(audioCfg[SND_ITF_LOUD_SPEAKER].spkSel == AUD_SPK_LOUD_SPEAKER,
                "TGT_AUD_CONFIG_LOUD_SPEAKER_OUTPUT_PATH should be set to AUD_SPK_LOUD_SPEAKER");
#endif

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_Setup begin itf: %d (spkLvl=%d, micLvl=%d)",
              itf, cfg->spkLevel, cfg->micLevel);

#if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
#endif


    aud_SemTake();

    // Only update a configuration if AUD is being used
    // Otherwise, do nothing.
    if (g_audEnabledRecordItf != SND_ITF_NONE || g_audEnabledPlayItf != SND_ITF_NONE || g_audEnabledToneItf != SND_ITF_NONE)
    {
        // Only change the AUD properties of the interface if, and only if
        // this interface is the interface being used.
        // FIXME The MMI is bogous and do not respect this constraint:
        // Fix the MMI and reenable this test. (We use a flag to avoid
        // to comment or #if 0 those line)
#ifdef THE_MMI_COMPLY_TO_AUD_SPEC
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE))
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            AUD_DEVICE_CFG_T deviceCfg;
            deviceCfg.spkSel    = audioCfg[itf].spkSel;
            deviceCfg.spkType   = audioCfg[itf].spkType;
            deviceCfg.micSel = audioCfg[itf].micSel;
            deviceCfg.level     = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (g_audLoudspeakerWithEarpiece)
            {
                if (deviceCfg.spkSel == AUD_SPK_LOUD_SPEAKER)
                {
                    deviceCfg.spkSel = AUD_SPK_LOUD_SPEAKER_EAR_PIECE;
                }
            }

            if (audioCfg[itf].setup)
            {
                // There is a setup function to call
                // for that interface
                errStatus = (audioCfg[itf].setup(itf, &deviceCfg));
            }
            else
            {
                // There is no driver for that interface,
                // ie. the interface does not exist.
                errStatus =  AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another interface is using the audio
            // ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }

#else

        // Interface switching is allowed among physical speaker
        // interfaces only
        if ( ( (g_audEnabledRecordItf == SND_ITF_NONE) ||
               (g_audEnabledRecordItf == itf) ||
               ( SND_ITF_HAS_PHY_SPK(itf) &&
                 SND_ITF_HAS_PHY_SPK(g_audEnabledRecordItf) )
             ) &&
             ( (g_audEnabledPlayItf == SND_ITF_NONE) ||
               (g_audEnabledPlayItf == itf) ||
               ( SND_ITF_HAS_PHY_SPK(itf) &&
                 SND_ITF_HAS_PHY_SPK(g_audEnabledPlayItf) )
             ) &&
             ( (g_audEnabledToneItf == SND_ITF_NONE) ||
               (g_audEnabledToneItf == itf) ||
               ( SND_ITF_HAS_PHY_SPK(itf) &&
                 SND_ITF_HAS_PHY_SPK(g_audEnabledToneItf) )
             )
           )
        {
            AUD_DEVICE_CFG_T deviceCfg;

            if(g_audEnabledRecordItf!=SND_ITF_NONE)
            {
                g_audEnabledRecordItf=itf;
            }
            
            if(g_audEnabledPlayItf!=SND_ITF_NONE)
            {
                g_audEnabledPlayItf=itf;
            }
            
            if(g_audEnabledToneItf!=SND_ITF_NONE)
            {
                g_audEnabledToneItf=itf;
            }

            deviceCfg.spkSel    = audioCfg[itf].spkSel;
            deviceCfg.spkType   = audioCfg[itf].spkType;
            if(gAudio_mode == DM_AUDIO_MODE_EARPIECE)
                deviceCfg.spkSel = audioCfg[SND_ITF_EAR_PIECE].spkSel;
            else if(gAudio_mode == DM_AUDIO_MODE_LOUDSPEAKER)
                deviceCfg.spkSel = audioCfg[SND_ITF_LOUD_SPEAKER].spkSel;
                
            deviceCfg.micSel = audioCfg[itf].micSel;
            deviceCfg.level     = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (g_audLoudspeakerWithEarpiece)
            {
                if (deviceCfg.spkSel == AUD_SPK_LOUD_SPEAKER)
                {
                    deviceCfg.spkSel = AUD_SPK_LOUD_SPEAKER_EAR_PIECE;
                }
            }

            if (audioCfg[itf].setup)
            {
                // There is a setup function to call
                // for that interface
                //HAL_AUD_SPK_CFG_T spkcfg = hal_AudGetSpkCfg();
                //BOOL nospkmute = spkcfg.noSpkMute;
                
#ifdef EXTERNAL_PA_SUPPORT
                hal_HstSendEvent(SYS_EVENT, 0x14011003);
                hal_HstSendEvent(SYS_EVENT, itf);
                hal_HstSendEvent(SYS_EVENT, g_audEnabledPlayItf);
                hal_HstSendEvent(SYS_EVENT, cfg->spkLevel);
                //hal_HstSendEvent(SYS_EVENT, nospkmute);
        //        if ( (itf == g_audEnabledPlayItf) && (cfg->spkLevel ==0) && (nospkmute!=0))//original
                if ( (itf == g_audEnabledPlayItf) && (cfg->spkLevel ==0))
                {
                    gpio_SetMute(TRUE);
                    hal_HstSendEvent(SYS_EVENT, 0x14011004);
                }
#endif

                errStatus = (audioCfg[itf].setup(itf, &deviceCfg));

#ifdef EXTERNAL_PA_SUPPORT
       //         if ( (itf == g_audEnabledPlayItf) && (cfg->spkLevel >0) && (nospkmute==0))//original
                if ( (itf == g_audEnabledPlayItf) && (cfg->spkLevel >0) )
                {
                    gpio_SetMute(FALSE);
                    hal_HstSendEvent(SYS_EVENT, 0x14011005);
                }
#endif                    

            }
            else
            {
                // There is no driver for that interface,
                // ie. the interface does not exist.
                errStatus =  AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another interface is using the audio
            // ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d",__LINE__, itf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }

#endif

    }
    else
    {
        // Nothing to do in that case, this can be 
        // interpreted as a no error call to the function.
        errStatus = AUD_ERR_NO;
    }

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_Setup end itf: %d", itf);
    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_StreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
/// 
/// This function  outputs the audio stream on the audio interface specified as 
/// a parameter, on the output selected in the AUD_LEVEL_T cfg parameter. \n
/// When the buffer runs out, the playing will wrap at the 
/// beginning of the buffer. If defined in the HAL_AIF_STREAM_T structure,
/// a user function can be called at the middle or at the end of the playback.
/// 
/// @param itf Interface on which play that stream
/// @param stream Stream to play. Handler called at the middle and end of the 
/// buffer are defined there. The length of a stream must be a multiple 
/// of 16 bytes.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamStart(
                        CONST SND_ITF_T      itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_LEVEL_T *       cfg)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T deviceCfg;
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
               "Exist on this target");
    AUD_ASSERT(stream, "Stream == 0");
    AUD_ASSERT(stream->startAddress, "Stream address == NULL");
    AUD_ASSERT(stream->length, "Stream length == 0");

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamStart itf: %d (spkLvl=%d, micLvl=%d)",
              itf, cfg->spkLevel, cfg->micLevel);

    #if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
    #endif

    MCI_SetSideTone(stream->sampleRate, stream->channelNb);

    aud_SemTake();

    if (((g_audEnabledRecordItf == itf)             || (g_audEnabledRecordItf   == SND_ITF_NONE))
     &&  (g_audEnabledPlayItf   == SND_ITF_NONE)
     && ((g_audEnabledToneItf   == itf)             || (g_audEnabledToneItf     == SND_ITF_NONE)))
    {
        // Stop tone first
        if (g_audEnabledToneItf != SND_ITF_NONE)
        {
            if (audioCfg[itf].tone)
            {
                // Only the start fields needs to be specified, 
                // set to FALSE to stop the tone generation.
                errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                if (errStatus == AUD_ERR_NO)
                {
                    // Stop the tone
                    // Set the new used interface
                    g_audEnabledToneItf = SND_ITF_NONE;
                }
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }

        if (errStatus == AUD_ERR_NO)
        {
            deviceCfg.spkSel  = audioCfg[itf].spkSel;
            deviceCfg.spkType = audioCfg[itf].spkType;
            if(gAudio_mode == DM_AUDIO_MODE_EARPIECE)
                deviceCfg.spkSel = audioCfg[SND_ITF_EAR_PIECE].spkSel;
            else if(gAudio_mode == DM_AUDIO_MODE_LOUDSPEAKER)
                deviceCfg.spkSel = audioCfg[SND_ITF_LOUD_SPEAKER].spkSel;

            deviceCfg.micSel = audioCfg[itf].micSel;
            deviceCfg.level   = cfg;

            if (!SND_ITF_HAS_PHY_SPK(itf))
            {
                if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
                {
                    deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
                }
            }

            if (g_audLoudspeakerWithEarpiece)
            {
                if (deviceCfg.spkSel == AUD_SPK_LOUD_SPEAKER)
                {
                    deviceCfg.spkSel = AUD_SPK_LOUD_SPEAKER_EAR_PIECE;
                }
            }

            if (audioCfg[itf].play)
            {
                // Interface driver exist: play
                errStatus = (audioCfg[itf].play(itf, stream, &deviceCfg));
                if (errStatus == AUD_ERR_NO)
                {
                    // The play actually started, set the (new) used interface
                    
#ifdef EXTERNAL_PA_SUPPORT
                    if (itf != g_audEnabledPlayItf)
                    {
                        hal_HstSendEvent(SYS_EVENT, 0x14011001);
                        gpio_SetMute(FALSE);
                    }
#endif

                    g_audEnabledPlayItf = itf;
                }
            }
            else
            {
                // No interface
                errStatus = AUD_ERR_NO_ITF;
            }
        }
    }
    else
    {
        // Interface is busy
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param itf Interface on which to stop the playing.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamStop(CONST SND_ITF_T itf)
{
    // Some applications will try to stop an audio stream in mutiple paths,
    // then it is possible for them to stop a SND_ITF_NONE interface.
    if (itf == SND_ITF_NONE)
    {
        return AUD_ERR_NO;
    }

    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamStop itf: %d", itf);

    aud_SemTake();

    if ((g_audEnabledRecordItf != SND_ITF_NONE) || (g_audEnabledPlayItf != SND_ITF_NONE) || (g_audEnabledToneItf != SND_ITF_NONE))
    {
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            // Stop tone first
            if (g_audEnabledToneItf != SND_ITF_NONE)
            {
                if (audioCfg[itf].tone)
                {
                    // Only the start fields needs to be specified, 
                    // set to FALSE to stop the tone generation.
                    errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                    if (errStatus == AUD_ERR_NO)
                    {
                        // Stop the tone
                        // Set the new used interface
                        g_audEnabledToneItf = SND_ITF_NONE;
                    }
                }
                else
                {
                    errStatus = AUD_ERR_NO_ITF;
                }
            }

            if (errStatus == AUD_ERR_NO)
            {
                // Stop stream then
                
#ifdef EXTERNAL_PA_SUPPORT
                if (itf == g_audEnabledPlayItf)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x14011002);
                    gpio_SetMute(TRUE);
                }
#endif

                if (audioCfg[itf].stop)
                {
                    errStatus = audioCfg[itf].stop(itf);
                    if (errStatus == AUD_ERR_NO)
                    {
                        // Freeing currently used interface
                        // Set the new freed interface
                        g_audEnabledRecordItf   = SND_ITF_NONE;
                        g_audEnabledPlayItf     = SND_ITF_NONE;
                        // Set current output device to default
                        g_audCurOutputDevice = SND_ITF_NONE;
                    }
                }
                else
                {
                    errStatus = AUD_ERR_NO_ITF;
                }
            }
        }
        else
        {
            // Another interface is in use. Ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to stop, not illegal (though useless)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();

    #if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
    #endif

    return errStatus;
}


// =============================================================================
// aud_StreamPause
// -----------------------------------------------------------------------------
/// This function pauses or resume the audio stream playback or/and record.
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver 
/// is busy with another audio command.
///
/// @param itf Interface on which to stop the playing.
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamPause(CONST SND_ITF_T itf, CONST BOOL pause)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");   
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamPause itf: %d ", itf);

    #if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
    #endif


    aud_SemTake();

    if ((g_audEnabledRecordItf != SND_ITF_NONE) || (g_audEnabledPlayItf != SND_ITF_NONE) || (g_audEnabledToneItf != SND_ITF_NONE))
    {
        if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
         && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
         && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
        {
            // Pause current interface
            if (audioCfg[itf].pause)
            {
                errStatus = (audioCfg[itf].pause(itf, pause));
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }

        }
        else
        {
            // Another interface is in use. Ignore
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to pause, not illegal (though useless)
        errStatus = AUD_ERR_NO;
    }

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamPause itf: %d, errStatus: %d", itf,errStatus);

    aud_SemFree();
    return errStatus;
}



// =============================================================================
// aud_StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
/// This function records the audio stream from the audio interface specified as 
/// a parameter, from the input selected in the AUD_LEVEL_T cfg parameter.\n
/// In normal operation, when the buffer runs out, the recording will wrap at the 
/// beginning of the buffer. If defined in the HAL_AIF_STREAM_T structure,
/// a user function can be called at the middle or at the end of the record.
///
/// @param itf Interface whose PCM flow is recorded.
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers. The length of a stream must be a multiple 
/// of 16 bytes.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_StreamRecord(
                        CONST SND_ITF_T      itf,
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_LEVEL_T *       cfg)
{
    CONST AUD_ITF_CFG_T* audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T deviceCfg;
    AUD_ERR_T errStatus = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
                "Exist on this target");

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_StreamRecord itf: %d (spkLvl=%d, micLvl=%d)",
              itf, cfg->spkLevel, cfg->micLevel);
    #if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
    #endif

    aud_SemTake();

    if ((g_audEnabledRecordItf  == SND_ITF_NONE)
     && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
     && ((g_audEnabledToneItf   == itf) || (g_audEnabledToneItf     == SND_ITF_NONE)))
    {
        // Record
        deviceCfg.spkSel    = audioCfg[itf].spkSel;
        deviceCfg.spkType   = audioCfg[itf].spkType;
        deviceCfg.micSel = audioCfg[itf].micSel;
        deviceCfg.level     = cfg;

        if (!SND_ITF_HAS_PHY_SPK(itf))
        {
            if (SND_ITF_HAS_PHY_SPK(g_audCurOutputDevice))
            {
                deviceCfg.spkSel = audioCfg[g_audCurOutputDevice].spkSel;
            }
        }

        if (g_audLoudspeakerWithEarpiece)
        {
            if (deviceCfg.spkSel == AUD_SPK_LOUD_SPEAKER)
            {
                deviceCfg.spkSel = AUD_SPK_LOUD_SPEAKER_EAR_PIECE;
            }
        }

        if (audioCfg[itf].record)
        {
            errStatus = (audioCfg[itf].record(itf, stream, &deviceCfg));
            if (errStatus == AUD_ERR_NO)
            {
                // Start recording on the (new) enabled interface
                // Set the (new) used interface
                g_audEnabledRecordItf = itf;
            }
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // AUD is activated on another interface
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledRecordItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    #if (AUDIO_CALIB_VER == 1)
    musicItf = itf;
    #endif
    return errStatus;
}


// =============================================================================
// aud_ToneStart
// -----------------------------------------------------------------------------
/// Manage the playing of a tone: DTMF or Comfort Tone. 
///
/// This function starts the output of a tone generated in the audio 
/// module on the selected interface. It a stream is being played on the same 
/// interface, the stream will continue to be played the  its sound won't be
/// output. It will be replaced instead by the generated tone.
///
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param itf AUD interface on which to play a tone.
/// @param tone The tone to generate.
/// @param cfg The configuration set applied to the audio interface.
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ToneStart(
                CONST SND_ITF_T         itf,
                CONST SND_TONE_TYPE_T   tone,
                CONST AUD_LEVEL_T*      cfg)
{
    CONST AUD_ITF_CFG_T*    audioCfg = tgt_GetAudConfig();
    AUD_DEVICE_CFG_T        deviceCfg;
    AUD_ERR_T               errStatus;
    
    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");    

    AUD_TRACE(AUD_INFO_TRC, 0, "aud_ToneStart itf: %d (spkLvl=%d, micLvl=%d)",
              itf, cfg->spkLevel, cfg->micLevel);

    aud_SemTake();

    if (((g_audEnabledRecordItf == itf) || (g_audEnabledRecordItf   == SND_ITF_NONE))
     && ((g_audEnabledPlayItf   == itf) || (g_audEnabledPlayItf     == SND_ITF_NONE)) 
     && ((g_audEnabledToneItf   == SND_ITF_NONE)))
    {
        deviceCfg.spkSel    = audioCfg[itf].spkSel;
        deviceCfg.spkType   = audioCfg[itf].spkType;
        deviceCfg.micSel    = audioCfg[itf].micSel;
        deviceCfg.level     = cfg;

        if (g_audLoudspeakerWithEarpiece)
        {
            if (deviceCfg.spkSel == AUD_SPK_LOUD_SPEAKER)
            {
                deviceCfg.spkSel = AUD_SPK_LOUD_SPEAKER_EAR_PIECE;
            }
        }

        if (audioCfg[itf].tone)
        {
            errStatus = (audioCfg[itf].tone(itf, tone, &deviceCfg, TRUE));
            if (errStatus == AUD_ERR_NO)
            {
                // Start toning on the (new) interface
                // Set the new used interface
                g_audEnabledToneItf = itf;
            }
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // AUD is activated on another interface
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_ToneStop
// -----------------------------------------------------------------------------
/// Stop the tone generation.
/// If the function returns  #AUD_ERR_RESOURCE_BUSY, it means that the driver 
/// is busy with an other audio command.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_ToneStop(CONST SND_ITF_T itf)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");
    
    AUD_TRACE(AUD_INFO_TRC, 0, "aud_ToneStop itf: %d", itf);
    
    aud_SemTake();

    if (g_audEnabledToneItf != SND_ITF_NONE)
    {
        if (g_audEnabledToneItf    == itf)
        {
            if (audioCfg[itf].tone)
            {
                // Only the start fields needs to be specified, 
                // set to FALSE to stop the tone generation.
                errStatus = (audioCfg[itf].tone(itf, 0, NULL, FALSE));
                if (errStatus == AUD_ERR_NO)
                {
                    // Stop the tone
                    // Set the new used interface
                    g_audEnabledToneItf = SND_ITF_NONE;
                }
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another AUD interface is activated
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to stop, not illegal (useless, though)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();
    return errStatus;
}


// =============================================================================
// aud_TonePause
// -----------------------------------------------------------------------------
/// This function pauses or resume a tone.
/// If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param itf Interface on which to stop the playing.
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_TonePause(CONST SND_ITF_T itf, CONST BOOL pause)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");

    aud_SemTake();

    if (g_audEnabledToneItf != SND_ITF_NONE)
    {
        if (g_audEnabledToneItf    == itf)
        {
            // Pause the tone
            if (audioCfg[itf].tonePause)
            {
                errStatus = (audioCfg[itf].tonePause(itf, pause));
            }
            else
            {
                errStatus = AUD_ERR_NO_ITF;
            }
        }
        else
        {
            // Another AUD interface is activated
            AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledToneItf);
            errStatus = AUD_ERR_RESOURCE_BUSY;
        }
    }
    else
    {
        // Nothing to pause, but not illegal (useless though)
        errStatus = AUD_ERR_NO;
    }

    aud_SemFree();
    return errStatus;
}





#if defined(LOOPBACK_TEST_MODE)
// Test mode record buffer 
PRIVATE UINT8 *g_audTestModeRecordBuffer = NULL;

// =============================================================================
// aud_TestModeSetup
// -----------------------------------------------------------------------------
/// Enable a test mode.
/// This function enables a test mode, enabling for example the side test
/// or the DAIs.
/// The audio interface must be enabled when this function is called.
/// This function disables the AIF (audio interface), sets the audio 
/// interface in the mode specified, and re-enables the AIF.
/// See the <B> Application Note 0018 </B> for more details on the DAI 
/// functionality and see 3GPP TS 44014-500 page 40 for the description of the 
/// DAI test interface.
/// 
/// @param itf AUD interface to test.
/// @param stream Depending on the test, this parameter can be ignored, or
/// used to configure it. (This is the case for the side test). This 
/// sets things relative to the streaming (sampling rate, etc)
/// @param cfg Depending on the test, this parameter can be ignored, or
/// used to configure it. (This is the case for the side test). Configure
/// the audio interface.
/// @param mode Test mode to set.
/// @return #AUD_ERR_NO if it succeeds.
// =============================================================================
PUBLIC AUD_ERR_T aud_TestModeSetup(CONST SND_ITF_T         itf,
                                   CONST HAL_AIF_STREAM_T* stream,
                                   CONST AUD_LEVEL_T *     cfg,
                                   AUD_TEST_T              mode)

{
    // FIXME How to protect that state machine (It is one, nope ?)
    switch (mode)
    {
        case AUD_TEST_SIDE_TEST:
        case AUD_TEST_RECVMIC_IN_EARPIECE_OUT:
        {
            if (g_audRunningTest != AUD_TEST_NO)
            {
                return AUD_ERR_RESOURCE_BUSY;
            }

            if (mode == AUD_TEST_RECVMIC_IN_EARPIECE_OUT &&
                itf != SND_ITF_EAR_PIECE)
            {
                return AUD_ERR_BAD_PARAMETER;
            }

            // Play 0 to add side test on it.
            UINT32 i;
            UINT32 len;

            if (mode == AUD_TEST_SIDE_TEST)
            {
                len = AUD_SIDE_TEST_BUF_LENGTH;
            }
            else // AUD_TEST_RECVMIC_IN_EARPIECE_OUT
            {
                len = AUD_RECVMIC_IN_EARPIECE_OUT_BUF_LENGTH;
            }
            
            if (g_audTestModeRecordBuffer == NULL)
            {
                g_audTestModeRecordBuffer = (UINT8*)pcmbuf_overlay;//COS_Malloc(len);
            }
            if (g_audTestModeRecordBuffer == NULL)
            {
                return AUD_ERR_UNKNOWN;
            }

            hal_HstSendEvent(BOOT_EVENT, 0x12345678);

            for (i=0 ; i<len; i++)
            {
                // Clear test buffer.
                g_audTestModeRecordBuffer[i] = 0;
            }

            if (mode == AUD_TEST_RECVMIC_IN_EARPIECE_OUT)
            {
                hal_AudForceReceiverMicSelection(TRUE);
                // Set max earpiece digital gain flag
                g_audSetHeadMaxDigitalGain = TRUE;
                // Set max MIC gain flag
                g_audSetMicMaxGain= TRUE;
            }

            // Copy configuration from parameter, 
            // but play null buffer.
            HAL_AIF_STREAM_T sideTestStream = *stream;
            sideTestStream.startAddress = (UINT32*)g_audTestModeRecordBuffer;
            sideTestStream.length       = len;

            while (aud_StreamStart(itf, &sideTestStream, cfg) != AUD_ERR_NO)
            {
                COS_Sleep(1);
            }

            if (mode == AUD_TEST_SIDE_TEST)
            {
                COS_Sleep(200);
                // The sound is replayed after 800ms (1s - 200ms)
            }

            while (aud_StreamRecord(itf, &sideTestStream, cfg) != AUD_ERR_NO)
            {
                COS_Sleep(1);
            }
            g_audRunningTest = mode;
            break;
        }

        case AUD_TEST_NO:
            switch (g_audRunningTest)
            {
                case AUD_TEST_SIDE_TEST:
                case AUD_TEST_RECVMIC_IN_EARPIECE_OUT:
                    if (g_audRunningTest == AUD_TEST_RECVMIC_IN_EARPIECE_OUT)
                    {
                        hal_AudForceReceiverMicSelection(FALSE);
                        // Reset max earpiece digital gain flag
                        g_audSetHeadMaxDigitalGain = FALSE;
                        // Reset max MIC gain flag
                        g_audSetMicMaxGain= FALSE;
                    }

                    while( aud_StreamStop(itf) != AUD_ERR_NO)
                    {
                        COS_Sleep(1);
                    }

                    if (g_audTestModeRecordBuffer != NULL)
                    {
                        COS_Free(g_audTestModeRecordBuffer);
                        g_audTestModeRecordBuffer = NULL;
                    }

                    g_audRunningTest = AUD_TEST_NO;
                    break;

                default:
                    break;
            }

        default:
            break;
    }

    return AUD_ERR_NO;
}
#endif /* LOOPBACK_TEST_MODE */


// =============================================================================
// aud_CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CalibUpdateValues(CONST SND_ITF_T itf)
{
    CONST AUD_ITF_CFG_T*    audioCfg    = tgt_GetAudConfig();
    AUD_ERR_T               errStatus   = AUD_ERR_NO;

    AUD_ASSERT(itf<SND_ITF_QTY, "This Audio interface does not"
            "Exist on this target");

    aud_SemTake();

    if (((g_audEnabledRecordItf == SND_ITF_NONE)    && (g_audEnabledPlayItf == SND_ITF_NONE))
     || ((g_audEnabledRecordItf == SND_ITF_NONE)    && (g_audEnabledPlayItf == itf ))
     || ((g_audEnabledRecordItf == itf)             && (g_audEnabledPlayItf == SND_ITF_NONE ))
     || ((g_audEnabledRecordItf == itf)             && (g_audEnabledPlayItf == itf )))
    {
        if (audioCfg[itf].calibUpdate)
        {
            errStatus = (audioCfg[itf].calibUpdate(itf));
        }
        else
        {
            errStatus = AUD_ERR_NO_ITF;
        }
    }
    else
    {
        // Another interface is activated, don't mess with it
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy, itf:%d, used itf:%d",__LINE__, itf, g_audEnabledPlayItf);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    aud_SemFree();
    return errStatus;
}


PUBLIC VOID aud_ForceReceiverMicSelection(BOOL on)
{
    aud_SemTake();

    hal_AudForceReceiverMicSelection(on);

    aud_SemFree();
}


PUBLIC VOID aud_SetCurOutputDevice(SND_ITF_T itf)
{
    if (itf == SND_ITF_NONE || SND_ITF_HAS_PHY_SPK(itf))
    {
        g_audCurOutputDevice = itf;
    }
}


PUBLIC SND_ITF_T aud_GetCurOutputDevice(VOID)
{
    return g_audCurOutputDevice;
}


PUBLIC VOID aud_LoudspeakerWithEarpiece(BOOL on)
{
    g_audLoudspeakerWithEarpiece = on;
}


PUBLIC INT16 aud_GetOutAlgGainDb2Val(VOID)
{
    return aud_CodecCommonGetOutAlgGainDb2Val();
}



PUBLIC BOOL aud_TestModeRegisterMallocFreeFunc(AUD_TEST_MODE_MALLOC_FUNC_T mallocFunc,
                                               AUD_TEST_MODE_FREE_FUNC_T freeFunc)
{
    if (g_audTestModeMallocFuncPtr == NULL &&
        g_audTestModeFreeFuncPtr == NULL)
    {
        g_audTestModeMallocFuncPtr = mallocFunc;
        g_audTestModeFreeFuncPtr = freeFunc;
        return TRUE;
    }

    return FALSE;
}


#ifdef VMIC_POWER_ON_WORKAROUND
// =============================================================================
// aud_InitEarpieceSetting
// -----------------------------------------------------------------------------
/// Configure audio components for earpiece detection and earpiece key detection
/// at system initialization time when OS is NOT ready yet.
/// The function is called by PMD only.
/// 
/// @param on TRUE if is about to enable, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_InitEarpieceSetting(VOID)
{
    extern BOOL hal_AudEarpieceDetectInit(VOID);
    hal_AudEarpieceDetectInit();
}

// =============================================================================
// aud_EnableEarpieceSetting
// -----------------------------------------------------------------------------
/// Configure audio components for earpiece detection and earpiece key detection
/// after OS is ready.
/// The function is called by PMD only.
/// 
/// @param on TRUE if is about to enable, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_EnableEarpieceSetting(BOOL enable)
{
    BOOL result;

    aud_SemTake();

    if (enable)
    {
        result = hal_AudOpen(HAL_AUD_USER_EARPIECE, NULL);
    }
    else
    {
        result = hal_AudClose(HAL_AUD_USER_EARPIECE);
    }

    if (!result)
    {
        AUD_ASSERT(FALSE, "aud_EnableEarpieceSetting(%d) failed", enable);
    }

    aud_SemFree();
}

#ifdef ABB_HP_DETECT
// =============================================================================
// aud_FmEnableHpDetect
// -----------------------------------------------------------------------------
/// Enable or disable headphone detection when FM is changing frequency.
/// The function is called by FMD only.
/// 
/// @param on TRUE if is about to enable, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_FmEnableHpDetect(BOOL enable)
{
    aud_SemTake();

    hal_AudEnableHpDetect(enable, HP_DETECT_REQ_SRC_FM);

    aud_SemFree();
}
#endif // ABB_HP_DETECT
#endif // VMIC_POWER_ON_WORKAROUND


#ifdef AUD_MUTE_FOR_BT_WORKAROUND
// =============================================================================
// aud_MuteOutputDevice
// -----------------------------------------------------------------------------
/// Power on/off the audio when it is playing.
/// The function is called by MCI only to redirect the audio output to the bluetooth device.
/// The function might be removed if MCI changes its way to switch between bluetooth and normal codec.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_MuteOutputDevice(BOOL mute)
{
    aud_SemTake();

    extern VOID hal_AudMuteOutput(BOOL);
    hal_AudMuteOutput(mute);

    aud_SemFree();
}
#endif // AUD_MUTE_FOR_BT_WORKAROUND


#if defined(RECV_USE_SPK_LINE) || defined(AUD_SPK_ON_WITH_LCD) || defined(AUD_EXT_PA_ON_WITH_LCD)
// =============================================================================
// aud_LcdPowerOnCallback
// -----------------------------------------------------------------------------
/// Power on some audio components along with LCD.
/// The function is called by LCDD only.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_LcdPowerOnCallback(VOID)
{
    aud_SemTake();

    extern VOID hal_AudLcdPowerOnCallback(VOID);
    hal_AudLcdPowerOnCallback();

    aud_SemFree();
}

// =============================================================================
// aud_LcdPowerOffCallback
// -----------------------------------------------------------------------------
/// Restore the power down state for some audio components.
/// The function is called by LCDD only.
/// 
/// @param on TRUE if power on, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_LcdPowerOffCallback(VOID)
{
    aud_SemTake();

    extern VOID hal_AudLcdPowerOffCallback(VOID);
    hal_AudLcdPowerOffCallback();

    aud_SemFree();
}
#endif // RECV_USE_SPK_LINE || AUD_SPK_ON_WITH_LCD || AUD_EXT_PA_ON_WITH_LCD


#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
#ifdef AUD_3_IN_1_SPK
// =============================================================================
// aud_BypassNotchFilterReq
// -----------------------------------------------------------------------------
/// Request to bypass notch filter.
/// 
/// @param on TRUE if is about to bypass, FALSE otherwise.
/// @return None.
// =============================================================================
PUBLIC VOID aud_BypassNotchFilterReq(BOOL bypass)
{
    aud_SemTake();

    hal_AudBypassNotchFilterReq(bypass);

    aud_SemFree();
}
#endif // AUD_3_IN_1_SPK
#endif // 8809 or later


///  @} <- End of the audio group 




