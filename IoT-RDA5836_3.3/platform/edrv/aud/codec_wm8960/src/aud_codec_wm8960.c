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

//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file aud_codec_gallite.c                                                        //
/// That file provides the implementation of the AUD interface for the CodecWM8960 //
//  chip.                                                                     //
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// 
// -----------------------------------------------------------------------------
// =============================================================================

#include "cs_types.h"

#include "hal_rda_abb.h"
#include "aud_codec_common.h"
#include "aud_codec_gallite.h"
#include "aud_codec_wm8960.h"
#include "audp_debug.h"

#include "hal_error.h"
#include "hal_sys.h"

#include "hal_rda_audio.h"
#include "hal_ispi.h"
#include "pmd_m.h"
#include "sxr_tls.h"
#include "tgt_calib_m.h"
//#include "rfd_xcv.h"
#include "global_macros.h"
#include "sys_ctrl.h"
#include "tgt_aud_cfg.h"
#include "audp_debug.h"
#include "hal_i2c.h"
#include "aif.h"

// aud_StreamStart() and aud_StreamStop(), aud_Setup() are of mutually exclusive access,
// with the help of semaphore. Then here we do not need mutex any more.


// =============================================================================
//  MACROS
// =============================================================================
//Master or slave mode 1:master 0:slave
#define WM8960_MS_MODE  0

// =============================================================================
//  TYPES
// =============================================================================

extern PUBLIC BOOL HAL_BOOT_FUNC hal_HstSendEvent(UINT32 level,UINT32 ch);
extern PUBLIC HAL_ERR_T hal_AifSerialOpen(CONST HAL_AIF_CONFIG_T* config);
// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

#define WM8960_I2C_ID  HAL_I2C_BUS_ID_1

HAL_AIF_CONFIG_T g_audCodecWM8960AifCfg;

PUBLIC  BOOL  g_audWM8960Playing;

/// Recording status 
PUBLIC BOOL  g_audWM8960Recording;

/// Openness status 
PUBLIC BOOL  g_audWM8960Opened;

UINT8 halErr;

#define WM8960_WR(reg,val)   /*\
	halErr = HAL_ERR_RESOURCE_BUSY; \
	while (halErr == HAL_ERR_RESOURCE_BUSY) \
	{ \
           halErr = hal_I2cSendByte(WM8960_I2C_ID, WM8960_ADDR, ((reg)<<1)|(((val)&0x100)>>8), (val)&0xff); \
	} \
	if (halErr != HAL_ERR_NO) \
	{ \
	}*/
	
	
PROTECTED AUD_LEVEL_T g_audWM8960Level = 
{       
        .spkLevel   = AUD_SPK_MUTE,
        .micLevel   = AUD_MIC_ENABLE,
        .sideLevel  = AUD_SIDE_MUTE,
        .toneLevel  = AUD_TONE_0dB,
};

PROTECTED AUD_DEVICE_CFG_T g_audWM8960Cfg = 
{
        .spkSel = AUD_SPK_EAR_PIECE,
        .micSel = AUD_MIC_RECEIVER,
        .level  = &g_audWM8960Level
};

PUBLIC VOID HAL_BOOT_FUNC hal_SysEnable8KSampleRate(VOID);
// =============================================================================
//  FUNCTIONS
// =============================================================================

HAL_AIF_SERIAL_CFG_T I2SAifConfig8960;

PRIVATE VOID aud_CodecWM8960I2SAifCfg(CONST HAL_AIF_STREAM_T* stream)
{
    // AIF Serial Interface configuration
    I2SAifConfig8960.mode=HAL_SERIAL_MODE_I2S;
	if(WM8960_MS_MODE)
	{
    	I2SAifConfig8960.aifIsMaster=FALSE;
	}
	else
	{
    	I2SAifConfig8960.aifIsMaster=TRUE;
	}

    I2SAifConfig8960.lsbFirst=FALSE;
    I2SAifConfig8960.polarity=FALSE;
    I2SAifConfig8960.rxDelay=HAL_AIF_RX_DELAY_1;
    I2SAifConfig8960.txDelay=HAL_AIF_TX_DELAY_ALIGN;
    if(stream->channelNb == HAL_AIF_MONO)
    {
        I2SAifConfig8960.rxMode=HAL_AIF_RX_MODE_STEREO_MONO_FROM_L; 
        I2SAifConfig8960.txMode=HAL_AIF_TX_MODE_MONO_STEREO_DUPLI;
    }
    else
    {
        I2SAifConfig8960.rxMode=HAL_AIF_RX_MODE_STEREO_STEREO; 
        I2SAifConfig8960.txMode=HAL_AIF_TX_MODE_STEREO_STEREO;
    }
    I2SAifConfig8960.fs=stream->sampleRate;
    I2SAifConfig8960.bckLrckRatio=32; //64; // 2x32
    I2SAifConfig8960.invertBck=TRUE;
    I2SAifConfig8960.outputHalfCycleDelay=FALSE;
    I2SAifConfig8960.inputHalfCycleDelay=FALSE;
    I2SAifConfig8960.enableBckOutGating=TRUE;

    g_audCodecWM8960AifCfg.interface=HAL_AIF_IF_SERIAL_IN_PARALLEL_OUT;

    // AIF configuration
    g_audCodecWM8960AifCfg.sampleRate=stream->sampleRate;
    g_audCodecWM8960AifCfg.channelNb=stream->channelNb;
    g_audCodecWM8960AifCfg.serialCfg=&I2SAifConfig8960;
}

PUBLIC AUD_ERR_T aud_CodecWM8960VolSet(U16 vol)
{
	U16 temp=0;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
       return AUD_ERR_NO;
#endif	   
   if(vol == 0)
   {
        WM8960_WR(WM8960_LOUT1, 0x100);
        WM8960_WR(WM8960_ROUT1, 0x100);
   }
   else
   {
		vol *=5;
		vol += 0x35;
		if(vol > 0x7f) vol = 0x7f;
		else if (vol<=0x30) vol =0x30;

		temp = vol;
		WM8960_WR(WM8960_LOUT1, temp);
		WM8960_WR(WM8960_ROUT1, temp);
		temp = 0x100 | vol;
		WM8960_WR(WM8960_LOUT1, temp);
		WM8960_WR(WM8960_ROUT1, temp);
	}
    return HAL_ERR_NO;
}

PUBLIC AUD_ERR_T aud_CodecWM8960Init(VOID)
{
    HAL_ERR_T err;
    err = hal_I2cOpen(WM8960_I2C_ID);
    if(err == HAL_ERR_NO)
    {
         hal_HstSendEvent(SYS_EVENT,0x20160211); 
	     return HAL_ERR_NO;
     }

     return HAL_ERR_RESOURCE_TIMEOUT;
}

extern PUBLIC VOID hal_AudI2SSetSampleRate(HAL_AIF_FREQ_T sampleRate);
PUBLIC AUD_ERR_T aud_CodecWM8960ChipConfig(
                                        HAL_AIF_FREQ_T sampleRate,
                                        BOOL voice,
                                        BOOL mono)
{
       U8 dacdiv,bclkdiv;
   
     /*
	* Reset all registers
	*/

	WM8960_WR(WM8960_RESET, 0x00);
	/*
	* VMID=50K, Enable VREF, AINL, AINR, ADCL and ADCR
	* I2S_IN (bit 0), I2S_OUT (bit 1), DAP (bit 4), DAC (bit 5), ADC (bit 6) are powered on
	*/
      WM8960_WR(WM8960_POWER1, 0xFE);
	/*
	 * Enable DACL, DACR, LOUT1, ROUT1, PLL ON
	 */
	WM8960_WR(WM8960_POWER2, 0x1FF);

	/*
	 * Enable left and right channel input PGA, left and right output mixer
	 */
	WM8960_WR(WM8960_POWER3, 0x3C);
	
	if((sampleRate == HAL_AIF_FREQ_11025HZ) || (sampleRate == HAL_AIF_FREQ_22050HZ) || (sampleRate == HAL_AIF_FREQ_44100HZ))
	{
	    //sysclk 11.2896
	    #if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
		WM8960_WR(WM8960_PLL1, 0x26);
		WM8960_WR(WM8960_PLL2, 0xf2);
		WM8960_WR(WM8960_PLL3, 0x8b);
		WM8960_WR(WM8960_PLL4, 0xd4);
	    #else
		WM8960_WR(WM8960_PLL1, 0x36);
		WM8960_WR(WM8960_PLL2, 0xf2);
		WM8960_WR(WM8960_PLL3, 0x8b);
		WM8960_WR(WM8960_PLL4, 0xd4);
		#endif

		switch(sampleRate)
		{
	     	#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
			case HAL_AIF_FREQ_44100HZ:
				 dacdiv = 0; bclkdiv=18; break;
				 
			case HAL_AIF_FREQ_22050HZ:
				 dacdiv = 2; bclkdiv=37;break;
				 
			case HAL_AIF_FREQ_11025HZ:
				 dacdiv = 4; bclkdiv=74;break;
				 
			default:
				 dacdiv = 0; bclkdiv=74;break;
			#else
			case HAL_AIF_FREQ_44100HZ:
				 dacdiv = 0; bclkdiv=30; break;
				 
			case HAL_AIF_FREQ_22050HZ:
				 dacdiv = 2; bclkdiv=62;break;
				 
			case HAL_AIF_FREQ_11025HZ:
				 dacdiv = 4; bclkdiv=126;break;
				 
			default:
				 dacdiv = 0; bclkdiv=30;break;
			#endif
		}

	}
	else
	{
		//sysclk 12.288
	
		#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
		WM8960_WR(WM8960_PLL1, 0x27);
		WM8960_WR(WM8960_PLL2, 0x8f);
		WM8960_WR(WM8960_PLL3, 0xd5);
		WM8960_WR(WM8960_PLL4, 0x25);
		#else
		WM8960_WR(WM8960_PLL1, 0x37);
		WM8960_WR(WM8960_PLL2, 0x8f);
		WM8960_WR(WM8960_PLL3, 0xd5);
		WM8960_WR(WM8960_PLL4, 0x25);
		#endif
		switch(sampleRate)
		{
			#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
			case HAL_AIF_FREQ_48000HZ:
				 dacdiv = 0;bclkdiv=17; break;
				 
			case HAL_AIF_FREQ_32000HZ:
				 dacdiv = 1; bclkdiv=25; break;
				 
			case HAL_AIF_FREQ_24000HZ:
				 dacdiv = 2; bclkdiv=34;break;
				 
			case HAL_AIF_FREQ_16000HZ:
				 dacdiv = 3; bclkdiv=51;break;	
				 
			case HAL_AIF_FREQ_8000HZ:
				 dacdiv = 6; bclkdiv=102; break;
				 
			default:
				 dacdiv = 0; bclkdiv=102;break;
			#else
			case HAL_AIF_FREQ_48000HZ:
				 dacdiv = 0;bclkdiv=30; break;
				 
			case HAL_AIF_FREQ_32000HZ:
				 dacdiv = 1; bclkdiv=46; break;
				 
			case HAL_AIF_FREQ_24000HZ:
				 dacdiv = 2; bclkdiv=62;break;
				 
			case HAL_AIF_FREQ_16000HZ:
				 dacdiv = 3; bclkdiv=94;break;	
				 
			case HAL_AIF_FREQ_8000HZ:
				 dacdiv = 6; bclkdiv=190; break;
				 
			default:
				 dacdiv = 0; bclkdiv=30;break;
			#endif
		}

	}

	#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
	hwp_sysCtrl->audio_clk_cfg &= ~SYS_CTRL_AIF_BCLK_DIV_MASK;
	hwp_sysCtrl->audio_clk_cfg |= SYS_CTRL_AIF_BCLK_DIV(bclkdiv) |SYS_CTRL_I2S_MCLK_DIV_LD_CFG|SYS_CTRL_AIF_BCLK_DIV_LD_CFG    \
	 	                                    |SYS_CTRL_I2S_MCLK_DIV_EN_ENABLE | SYS_CTRL_I2S_MCLK_DIV_DIV2|SYS_CTRL_AIF_BCLK_DIV_EN_ENABLE;
	#else
	hal_AudI2SSetSampleRate(sampleRate);
	hal_SysEnableClkOut(bclkdiv);
	#endif
       hal_HstSendEvent(0xff, 0x20161431);
	hal_HstSendEvent(0xff, sampleRate);
	hal_HstSendEvent(0xff, dacdiv);

	/* Configure SYS_FS clock, MCLK_FREQ to 256*Fs, SYSCLK derived from MCLK input */
	WM8960_WR(WM8960_CLOCK1, 0x185 |((dacdiv<<3)&0x38));

	/*
	 * Audio data length = 16bit, Left justified data format
	 */
	 if(WM8960_MS_MODE)
	 {
		WM8960_WR(WM8960_IFACE1, 0x41);
		WM8960_WR(WM8960_CLOCK2, 0x07);
		WM8960_WR(WM8960_IFACE2, 0x40);
	 }
	 else
	 {
		WM8960_WR(WM8960_IFACE1, 0x01);
	 }



	/*
	 * LMICBOOST = 0dB, Connect left and right PGA to left and right Input Boost Mixer
	 */
       WM8960_WR(WM8960_LINPATH, 0x178);
	WM8960_WR(WM8960_RINPATH, 0x178);

	/*
	 * Left and right input boost, LIN3BOOST and RIN3BOOST = 0dB
	 */
	WM8960_WR(WM8960_INBMIX1, 0x00);
	WM8960_WR(WM8960_INBMIX2, 0x00);

      
	/*
	 * Left DAC and LINPUT3 to left output mixer, LINPUT3 left output mixer volume = 0dB
	 */
	WM8960_WR(WM8960_LOUTMIX, 0x180);

	/*
	 * Right DAC and RINPUT3 to right output mixer, RINPUT3 right output mixer volume = 0dB
	 */
	WM8960_WR(WM8960_ROUTMIX, 0x180);

	/*
	 * ADC volume, 0dB
	 */
	WM8960_WR(WM8960_LADC, 0x1FF); //0dB
	WM8960_WR(WM8960_RADC, 0x1FF); //0dB

	/*
	 * Digital DAC volume, 0dB
	 */
	WM8960_WR(WM8960_LDAC, 0x1FF); //0dB
	WM8960_WR(WM8960_RDAC, 0x1FF); //0dB

       WM8960_WR(WM8960_LINVOL, 0x120);
	WM8960_WR(WM8960_RINVOL, 0x113);//micgain,0x3f:+30db step:0.75db
 	WM8960_WR(WM8960_DACCTL1, 0);
	
	/*
	 * Headphone volume, LOUT1 and ROUT1, 0dB
	 */
       WM8960_WR(WM8960_LOUT1, 0x150);//max:0-0x7f; step:0.75db
	WM8960_WR(WM8960_ROUT1, 0x150);


       //loopback
       //WM8960_WR(WM8960_IFACE2, 0x1);
	  // while(1);
	

	return AUD_ERR_NO;
}

PUBLIC AUD_ERR_T aud_CodecWM8960AifConfig(VOID)
{
    if (hal_AifSerialOpen(&g_audCodecWM8960AifCfg) != HAL_ERR_NO)
    {
        return AUD_ERR_RESOURCE_BUSY;
    }

    return AUD_ERR_NO;
}

PUBLIC AUD_ERR_T aud_CodecWM8960Open(HAL_AIF_FREQ_T sampleRate, BOOL voice, BOOL mono)
{
    AUD_ERR_T errStatus;

    errStatus = aud_CodecWM8960ChipConfig(sampleRate, voice, mono);
    if (errStatus == AUD_ERR_NO)
    {
        errStatus = aud_CodecWM8960AifConfig();
        if (errStatus == AUD_ERR_NO)
        {
            g_audWM8960Opened = TRUE;

        }
    }

    return errStatus;
}


PUBLIC AUD_ERR_T aud_CodecWM8960Close(VOID)
{
   hal_AifClose();
   WM8960_WR(WM8960_POWER1, 0);
   WM8960_WR(WM8960_POWER2, 0);
   //hal_I2cClose(WM8960_I2C_ID);
 
   g_audWM8960Opened = FALSE;

    return AUD_ERR_NO;
}

// =============================================================================
// aud_TiSetup
// -----------------------------------------------------------------------------
/// Configure the audio. 
/// 
/// This functions configures an audio interface: gain for the side tone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker.
/// 
/// @param cfg The configuration set applied to the audio interface. See 
/// #AUD_DEVICE_CFG_T for more details.
/// @return #AUD_ERR_NO if it can execute the configuration.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960Setup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg)
{
    /*
    if ((g_audWM8960Cfg.level)->spkLevel == AUD_SPK_MUTE)
    {
	    WM8960_WR(WM8960_DACCTL1, 0x04);
    }
    else
    {
        WM8960_WR(WM8960_DACCTL1, 0);
    }

    if ((g_audWM8960Cfg.level)->micLevel == AUD_MIC_MUTE)
    {
       WM8960_WR(WM8960_LINVOL, 0x180);
	WM8960_WR(WM8960_RINVOL, 0x180);
    }
    else
    {
       WM8960_WR(WM8960_LINVOL, 0x120);
	WM8960_WR(WM8960_RINVOL, 0x117);//micgain,0x3f:+30db step:0.75db
    }
    */
    aud_CodecWM8960VolSet(cfg->level->spkLevel);
		
    return AUD_ERR_NO;
}

void aud_CodecWM8960HalfPcmISR(void)
{
    //hal_HstSendEvent(0xff, 0x20168888);
}

void aud_CodecWM8960EndPcmISR(void)
{
    //hal_HstSendEvent(0xff, 0x20166666);
}

void aud_CodecWM8960Test(void)
{
      extern HAL_AIF_STREAM_T audioStream;
      extern UINT32 *pcmbuf_overlay;
	extern  AUD_ITF_T   audioItf;
	extern AUD_LEVEL_T audio_cfg;
	UINT8 i=0;
      audioStream.startAddress  	= pcmbuf_overlay;
      audioStream.length        	= 32;
      audioStream.channelNb     	= HAL_AIF_STEREO;//HAL_AIF_MONO;

      audioStream.voiceQuality   = !TRUE;    
      audioStream.halfHandler   	= aud_CodecWM8960HalfPcmISR;
      audioStream.endHandler    = aud_CodecWM8960EndPcmISR;
   
       audioItf = AUD_ITF_EAR_PIECE;

  	audioStream.sampleRate    = HAL_AIF_FREQ_48000HZ;
	hal_HstSendEvent(0xff, 0x20160930);

	for (i=0 ; i<32; i++)
       {
                // Clear test buffer.
                pcmbuf_overlay[i] = 0x3300aa00+i;
       }

	aud_StreamStart(audioItf, &audioStream, &audio_cfg);
       
}

// =============================================================================
// aud_CodecWM8960StreamStart
// -----------------------------------------------------------------------------
/// Start the playing of a stream. 
/// 
/// This function  outputs the 
/// audio on the audio interface specified as a parameter, on the output selected in 
/// the #aud_Setup function. \n
/// In normal operation, when 
/// the buffer runs out, the playing will wrap at the beginning. Here, there are two ways 
/// you can handle your buffer: HAL can call a user function at the middle or 
/// at the end of the playback or, depending 
/// on your application, you can simply poll the playing state using the "reached half"
/// and "reached end functions" TODO Ask if needed, and then implement !
/// 
/// @param stream Stream to play. Handler called at the middle and end of the buffer
/// are defined there.
/// @param cfg The configuration set applied to the audio interface
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command,
///         #AUD_ERR_NO it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960StreamStart(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;
	
    aud_CodecWM8960I2SAifCfg(stream);

    if (g_audWM8960Playing)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audWM8960Opened)
    {
        errStatus = aud_CodecWM8960Open(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }
    errStatus = aud_CodecWM8960Setup(itf, cfg);
    if (errStatus != AUD_ERR_NO) needCloseAud = TRUE;
	
    // Send the stream through the IFC
    if (hal_AifPlayStream(stream) != HAL_ERR_NO)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }

    if (errStatus == AUD_ERR_NO)
    {
        needCloseAud = FALSE;
        g_audWM8960Playing = TRUE;
    }

    if (needCloseAud)
    {
        aud_CodecWM8960Close();
    }

    return errStatus;
}
	
// =============================================================================
// aud_CodecWM8960StreamStop
// -----------------------------------------------------------------------------
/// This function stops the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960StreamStop(SND_ITF_T itf)
{	
    if (g_audWM8960Playing)
    {
        hal_AifStopPlay();
        g_audWM8960Playing = FALSE;
    }

    if (g_audWM8960Recording)
    {
        hal_AifStopRecord();
        g_audWM8960Recording = FALSE;
    }
  
    if (g_audWM8960Opened)
    {
        aud_CodecWM8960Close();
    }
    else
    {
        return AUD_ERR_NO;
    }

    return AUD_ERR_NO;
}



// =============================================================================
// aud_CodecWM8960StreamPause
// -----------------------------------------------------------------------------
/// This function pauses the audio stream playback or/and record.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the stream. If \c FALSE, resumes a paused
/// stream.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960StreamPause(SND_ITF_T itf, BOOL pause)
{
    if (hal_AifPause(pause) == HAL_ERR_NO)
    {
        return AUD_ERR_NO;
    }

    return AUD_ERR_NO;
}



// =============================================================================
// aud_CodecWM8960StreamRecord
// -----------------------------------------------------------------------------
/// Manage the recording of a stream. 
/// 
///
/// The #aud_StreamRecord send a message to the driver which begins the 
/// dataflow from the audio and the compression. If the function returns \c 
/// FALSE, it means that the driver cannot handle the record. The value \c len 
/// should be the maximum size of the recording buffer and when the buffer is 
/// full, the recording will automatically wrap. A 
/// user function can be called, either when the middle or the end of the 
/// buffer is reached. (TODO add iatus about polling if available)
/// 
/// @param stream Stream recorded (describe the buffer and the possible interrupt
/// of middle-end buffer handlers.
/// @param cfg The configuration set applied to the audio interface.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver cannot handle the stream,
///         #AUD_ERR_NO if the stream ca be recorded.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960StreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T *       cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    BOOL needCloseAud = FALSE;
	

    aud_CodecWM8960I2SAifCfg(stream);
    
    if (g_audWM8960Recording)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        return AUD_ERR_RESOURCE_BUSY;
    }

    if (!g_audWM8960Opened)
    {
        errStatus = aud_CodecWM8960Open(stream->sampleRate, stream->voiceQuality,
            (stream->channelNb == HAL_AIF_MONO));

        if (errStatus != AUD_ERR_NO)
        {
            return errStatus;
        }
        else
        {
            needCloseAud = TRUE;
        }
    }
	
   // hal_SysEnable8KSampleRate();
	
    errStatus = aud_CodecWM8960Setup(itf, cfg);
    if (errStatus != AUD_ERR_NO) needCloseAud = TRUE;
	
    // Send the stream through the IFC
    if (hal_AifRecordStream(stream) != HAL_ERR_NO)
    {
        AUD_TRACE(AUD_INFO_TRC, 0,
            "AUD: " __FILE__ ", line:%d, Resource Busy",__LINE__);
        errStatus = AUD_ERR_RESOURCE_BUSY;
    }
 

    if (errStatus == AUD_ERR_NO)
    {
        needCloseAud = FALSE;
        g_audWM8960Recording = TRUE;
    }

    if (needCloseAud)
    {       
       aud_CodecWM8960Close();
    }

    return errStatus;
}



// =============================================================================
// aud_CodecWM8960Tone
// -----------------------------------------------------------------------------
//  Manage the playing of a tone: DTMF or Comfort Tone. 
/// 
/// Outputs a DTMF or comfort tone
///
/// When the audio output is enabled, a DTMF or a comfort tones can be output 
/// as well. This function starts the output of a tone generated in the audio 
/// module. \n
/// You can call this function several times without calling the 
/// #aud_ToneStop function or the #aud_TonePause function in 
/// between, if you just need to change the attenuation or the tone type. \n
/// If the function returns #AUD_ERR_RESOURCE_BUSY, it means that the driver is 
/// busy with an other audio command.
///
/// @param tone The tone to generate
/// @param attenuation The attenuation level of the tone generator
/// @param cfg The configuration set applied to the audio interface
/// @param start If \c TRUE, starts the playing of the tone.
///              If \c FALSE, stops the tone. 
///
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960Tone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T        tone,
                CONST AUD_DEVICE_CFG_T*             cfg,
                CONST BOOL                   start)
{
    return AUD_ERR_NO;
}



// =============================================================================
// aud_CodecWM8960TonePause
// -----------------------------------------------------------------------------
/// This function pauses the audio tone.
/// But it doesn't free the audio out resource. If the function returns 
/// #AUD_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @param pause If \c TRUE, pauses the tone. If \c FALSE, resumes a paused
/// tone.
/// @return #AUD_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #AUD_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960TonePause(SND_ITF_T itf, BOOL pause)
{
    return AUD_ERR_NO;
}



// =============================================================================
// aud_CodecWM8960CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update values depending on calibration parameters.
/// 
/// @param itf AUD interface on which to stop the tone.
/// @return #AUD_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #AUD_ERR_NO otherwise
// =============================================================================
PUBLIC AUD_ERR_T aud_CodecWM8960CalibUpdateValues(SND_ITF_T itf)
{
    return AUD_ERR_NO;
}





