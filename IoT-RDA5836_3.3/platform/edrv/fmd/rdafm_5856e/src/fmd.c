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
//       
/// @file fmd.c
/// That file implement the FM driver for RDAFM8809P chip.
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "fmd_config.h"
#include "fmd_m.h"
#include "tgt_fmd_cfg.h"
#include "fmdp_debug.h"

#include "pmd_m.h"

#include "hal_i2c.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"
#include "hal_host.h"
#include "hal_rf.h"

#include "sxr_tim.h"

#include "sxr_tls.h"
//#include "cmn_defs.h"
#include "base_address.h"
#include "register.h"
#include "rf_if.h"
#include "cos.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#endif


#ifdef ABB_HP_DETECT
extern VOID aud_FmEnableHpDetect(BOOL enable);
extern VOID rfd_XcvRegPuXtalEnable(BOOL enable);
#endif

#ifdef FM_LDO_WORKAROUND
extern VOID pmd_EnableDcdcPower(BOOL on);
#endif

// =============================================================================
//  MACROS
// =============================================================================

#define ADJUST_DAC_GAIN 0

#define PREP_WRITE_REG_DATA(a) {writeReg[0]=((a)&0xff00)>>8;writeReg[1]=(a)&0xff;}

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
#define BASE_FREQ_EAST_EUROPE     65000

#define CHAN_SPACE          g_chanSpacing[g_fmdConfig->channelSpacing]

// MACROS for PRIVATE FUNCTION USE ONLY
#define FMD_I2C_BPS             g_fmdConfig->i2cBps

#define HAL_I2C_BUS_ID 			g_fmdConfig->i2cBusId

// =============================================================================
//  TYPES
// =============================================================================


//=============================================================================
// FMD_STATE_T
//-----------------------------------------------------------------------------
/// State of the tune or seek pooling process (using function timers)
//=============================================================================
typedef enum
{
    /// Initial state, no operation in process
    FMD_STATE_IDLE,
    /// Pooling states for tune and seek
    FMD_STATE_TUNE_POLL,
    FMD_STATE_SEEK_POLL,
    /// Once polling is done, we need to stop, is SPI is not avalaible, this is delayed to those states
    FMD_STATE_TUNE_STOP,
    FMD_STATE_SEEK_STOP,

    FMD_STATE_QTY
} FMD_STATE_T;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE CONST TGT_FMD_CONFIG_T*  g_fmdConfig;

PRIVATE UINT32 g_baseFreq[FMD_BAND_QTY] = {BASE_FREQ_USEUROPE, BASE_FREQ_JAPAN, BASE_FREQ_WORLD, BASE_FREQ_EAST_EUROPE};
PRIVATE UINT32 g_chanSpacing[FMD_CHANNEL_SPACING_QTY] = {100,200,50};
PRIVATE FMD_BAND_T g_band = FMD_BAND_QTY; // using FMD_BAND_QTY as a closed state marker
PRIVATE UINT16 g_fmRegSysCtrl; // BassBoost ForceMono and Mute state
PRIVATE FMD_CALLBACK_T* g_callback = NULL;
extern void hal_Fm_Dsp_Datapath_Init(void);

void rdafm_Start_Fm_Chn(uint32 band, uint32 chn)
{
    double internal_hex,freq_adc,step_19k,freq_vco,freq;
    uint8 div_fm_lo_clk,div_fm_lo_clk2;
    uint16 div;
    //uint32 readdata;
    hwp_rf_if->fm_control &= ~RF_IF_FM_TUNE;
    
    hwp_rf_if->fm_control  &= ~RF_IF_FM_BAND_SEL_MASK;
    hwp_rf_if->fm_control  |= RF_IF_FM_BAND_SEL(band);

    div= (chn -g_baseFreq[g_band])/100*4;
    hwp_rf_if->fm_control  &= ~RF_IF_FM_CHAN_REG_MASK;
    hwp_rf_if->fm_control |= RF_IF_FM_CHAN_REG(div);

    freq=chn/1000.0;
    
    if(freq>=93) div_fm_lo_clk=6;
    else if((freq<93) && (freq>=81) )  div_fm_lo_clk=7;
    else if((freq<81) && (freq>=72) ) div_fm_lo_clk=8;
    else div_fm_lo_clk=9;
    
    freq_vco = (freq-0.125)*(div_fm_lo_clk)*4;
    
    if(freq_vco>=2537) div_fm_lo_clk2=30;
    else if((freq_vco<2537) && (freq_vco>=2451) )  div_fm_lo_clk2=29;
    else if((freq_vco<2451) && (freq_vco>=2365) )  div_fm_lo_clk2=28;
    else if((freq_vco<2365) && (freq_vco>=2279) )  div_fm_lo_clk2=27;
    else div_fm_lo_clk2=26;

    freq_adc = freq_vco/(div_fm_lo_clk2*2);
    internal_hex = 524288/freq_adc;
    step_19k = 1024*256*19*128/(freq_adc*1000);
    
    //datapath_ctrl3 interval 
    hwp_fm_dsp->datapath_ctrl3 &= ~(FM_DSP_INTERVAL_REG_MASK|FM_DSP_STEP19K_REG_MASK);
    hwp_fm_dsp->datapath_ctrl3 |= FM_DSP_INTERVAL_REG((uint32)internal_hex)|FM_DSP_STEP19K_REG((uint32)step_19k);
    hwp_rf_if->fm_control |= RF_IF_FM_TUNE;
}


#if (ADJUST_DAC_GAIN)
PRIVATE VOID fmd_SetDacGain(UINT8 db)
{
    UINT8 writeReg[2];
    UINT32 tmp6db, tmp1db;

    // DAC gain bits <5:0>:
    // <5:3> x 6dB, <2> x 2dB, <1> x 2dB, <0> x 1dB

    // The default value is 7 (5dB).
    // This value can be varied in +-3dB. The gains smaller than 2dB will
    // lead to SNR degradation, and the gains larger than 8dB will lead
    // to sound distortion (due to signal saturation).

    if (db > 15) db = 15;
    tmp6db = db / 6;
    tmp1db = db % 6;

    if (tmp1db >= 4) tmp1db |= 0x2;

    PREP_WRITE_REG_DATA(0x0080|((tmp6db&0x7)<<3)|(tmp1db&0x7));
    fmd_Write(0x1f, writeReg, 2);
}
#endif


PUBLIC FMD_ERR_T fmd_Open(FMD_BAND_T band,
                          FMD_CALLBACK_T* callback,
                          FMD_INIT_T* initValues)
{
    CONST TGT_FMD_CONFIG_T* fmdTgtCfg = tgt_GetFmdConfig();
    UINT8 dacVol;
    UINT16 chan;

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Open=%d",g_band);

    // Check if already opened
    // -----------------------
    if (g_band != FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED;
    }
    // Check the parameters and store them
    // -----------------------------------
    switch (band)
    {
        case FMD_BAND_US_EUROPE:
        case FMD_BAND_JAPAN:
        case FMD_BAND_WORLD:
        case FMD_BAND_EAST_EUROPE:
            g_band = band;
            break;
        default:
            // this chip has limited band support...
            return FMD_ERR_BAD_PARAMETER;
    }
    if (fmdTgtCfg == NULL)
    {
        g_band = FMD_BAND_QTY; // close marker
        return FMD_ERR_BAD_PARAMETER;
    }
    g_fmdConfig = fmdTgtCfg;
    g_callback = callback;
    // default : mute , stereo, no bass boost
    g_fmRegSysCtrl = 0;
    dacVol = 0;
    chan = 0;
    // set the required initial state
    // ------------------------------
    if (initValues != NULL)
    {
        // set given parameters 
        dacVol = g_fmdConfig->volumeVal[initValues->volume];
        // compute the mute bit
        if (initValues->volume == FMD_ANA_MUTE)
        {
            g_fmRegSysCtrl &= ~0x4000;
        }
        else
        {
            g_fmRegSysCtrl |= 0x4000;
        }
        if (initValues->bassBoost == TRUE)
        {
            g_fmRegSysCtrl |= 0x1000;
        }
        else
        {
            g_fmRegSysCtrl &= ~0x1000;
        }
        if (initValues->forceMono == TRUE)
        {
            g_fmRegSysCtrl |= 0x2000;
        }
        else
        {
            g_fmRegSysCtrl &= ~0x2000;
        }
        chan = (initValues->freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
    }

    // enable the POWER
    pmd_EnablePower(PMD_POWER_FM, TRUE);
   
#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(FALSE);
#endif

    hal_SysWriteRFUsingAPB();
    hal_RF_Init();
    hal_Fm_Dsp_Datapath_Init();
    //hal_HstSendEvent(BOOT_EVENT,0x11116);
    hal_Rf_Set_Rf_Mode(1);
 
    // Wait for at least powerOnTime
  //  COS_SleepByTick(g_fmdConfig->powerOnTime);
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_Tune
// -----------------------------------------------------------------------------
/// This function tunes the FM channel to the desired frequency.
/// Once the tune operation has locked the channel, the callback registered 
/// at #fmd_Open will be called.
/// @param freqKHz  FM frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Tune(UINT32 freqKHz)
{
    FMD_ERR_T errStatus = FMD_ERR_NO;

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Tune: freqKHz=%d",freqKHz);

#ifdef ABB_HP_DETECT
    aud_FmEnableHpDetect(FALSE);
#endif

    rdafm_Start_Fm_Chn(g_band,freqKHz);
	
#ifdef ABB_HP_DETECT
    COS_Sleep(50);
    aud_FmEnableHpDetect(TRUE);
#endif

    return errStatus;
}

// =============================================================================
// fmd_Close 
// -----------------------------------------------------------------------------
/// This function closes the FM driver and desactivate the FM chip 
/// and stops any pending #fmd_Tune or #fmd_Seek operation.
/// @return             \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Close(VOID)
{
    FMD_ERR_T errStatus = FMD_ERR_NO;
    //UINT8 writeReg[2] = {0};
    
    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Close=%d",g_band);

    // check oppened
    if (g_band == FMD_BAND_QTY)
    {
        //FMD_ASSERT(g_spiInUse, "closing FMD while not open");
        return FMD_ERR_ALREADY_OPENED; // ALREADY_CLOSED here !!
    }

#if defined(ABB_HP_DETECT) && defined(ABB_HP_DETECT_IN_LP)
   // rfd_XcvRegPuXtalEnable(FALSE);
#endif
    
#ifdef FM_LDO_WORKAROUND
    pmd_EnableDcdcPower(TRUE);
#endif

    // cut the power, cut the clock
    pmd_EnablePower(PMD_POWER_FM, FALSE);

    g_band = FMD_BAND_QTY; // close marker
    
    return errStatus;
}

// =============================================================================
// fmd_SetVolume
// -----------------------------------------------------------------------------
/// This function sets the volume and other audio related paramters.
/// @param volume       analog volume
/// @param bassBoost    enable/disable bass boost
/// @param forceMono    enable/disable mono output of stereo radios
/// @return             \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_SetVolume(FMD_ANA_LEVEL_T volume,
                               BOOL bassBoost,
                               BOOL forceMono)
{
    FMD_ERR_T errStatus=0;
    //UINT8 writeReg[2] = {0};
    FMD_ASSERT(volume < FMD_ANA_VOL_QTY, "FMD volume out of range (%d)", volume);

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_SetVolume: vol=%d, boost=%d, mono=%d",
        volume, bassBoost, forceMono);

    // compute the mute bit
    if (volume == FMD_ANA_MUTE)
    {
        g_fmRegSysCtrl &= ~0x4000;
    }
    else
    {
        g_fmRegSysCtrl |= 0x4000;
    }
    if (bassBoost == TRUE)
    {
        g_fmRegSysCtrl |= 0x1000;
    }
    else
    {
        g_fmRegSysCtrl &= ~0x1000;
    }
    if (forceMono == TRUE)
    {
        g_fmRegSysCtrl |= 0x2000;
    }
    else
    {
        g_fmRegSysCtrl &= ~0x2000;
    }
    
#if (ADJUST_DAC_GAIN)
    COS_Sleep(50);

    // get volume value from config
    UINT8 dacVol;
    dacVol = g_fmdConfig->volumeVal[volume];
    fmd_SetDacGain(dacVol);
#endif

    return errStatus;
}



// =============================================================================
// fmd_GetFreq
// -----------------------------------------------------------------------------
/// @param pFreqKHz     pointer to a UINT32 receiving the channel frequency
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pFreqKHz is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetFreq(UINT32 *pFreqKHz)
{
    FMD_ERR_T errStatus;
    UINT8 readReg[2] = {0};
    
    if (pFreqKHz == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }
    * (UINT16*)readReg = (hwp_rf_if->fm_control>>1) & (0x7ff);
    FMD_TRACE(EDRV_FMD_TRC,0,"reg0=%x,reg1=%x",readReg[0],readReg[1]);   

    *pFreqKHz = (((readReg[1]<<8) |readReg[0]) & 0x7ff) * 25 + g_baseFreq[g_band];
    return errStatus;
}


// =============================================================================
// fmd_GetStereoStatus
// -----------------------------------------------------------------------------
/// @param pStereoStatus    pointer to a BOOL receiving the stereo status:
///                         \c TRUE the channel is received in stereo,
///                         \c FALSE the channel is recieved in mono.
/// @return                 \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                         \c FMD_ERR_BAD_PARAMETER if \p pStereoStatus is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetStereoStatus(BOOL *pStereoStatus)
{
    UINT8 readReg=0;
    FMD_ERR_T errStatus = FMD_ERR_NO;
    
    if (pStereoStatus == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    readReg = (hwp_fm_dsp->status1 & FM_DSP_SK_CMP_GRP_FLAG_MASK)>>FM_DSP_SK_CMP_GRP_FLAG_SHIFT;

    if (errStatus == FMD_ERR_NO)
    {
        if ((readReg & 0x10) == 0)
        {
            *pStereoStatus = FALSE;
        }
        else
        {
            *pStereoStatus = TRUE;
        }
    }

    return errStatus;
}


// =============================================================================
// fmd_GetRssi
// -----------------------------------------------------------------------------
/// @param pRSSI        pointer to a UINT32 receiving a measure of the channel quality, should not be used as the meaning of this is dependant on the actual FM chip.
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pRSSI is \c NULL
// =============================================================================
DEPRECATED PUBLIC FMD_ERR_T fmd_GetRssi(UINT8* pRSSI)
{
    UINT8 readReg=0;
    FMD_ERR_T errStatus;

    if (pRSSI == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    } 

    readReg = hwp_fm_dsp->status0 & FM_DSP_RSSI_DB2_MASK;

    *pRSSI = (readReg >> FM_DSP_RSSI_DB2_SHIFT); 

    return errStatus;
}

PUBLIC BOOL fmd_ValidStop(UINT32 freqKHz)
{
    BOOL result;
    FMD_ERR_T errStatus;
	UINT8 i = 0;

#ifdef ABB_HP_DETECT
    aud_FmEnableHpDetect(FALSE);
#endif

    rdafm_Start_Fm_Chn(g_band,freqKHz);

	//waiting for FmReady
	do
	{
		i++;
		if(i>10) return 0; 
		COS_Sleep(10);
		errStatus = hwp_fm_dsp->status0;
	} while((errStatus&FM_DSP_SEEK_READY)==0);	

#ifdef ABB_HP_DETECT
    aud_FmEnableHpDetect(TRUE);
#endif

    if ((errStatus & FM_DSP_SEEK_DONE) == 0)
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_ValidStop: freqKHz=%d, result=%d",
        freqKHz, result);

    return result;
}

// =============================================================================
// fmd_GetRssiThreshold
// -----------------------------------------------------------------------------
/// @return             a Threshold used to compare with RSSI measure of the channel quality, only useful with fmd_GetRssi.
// =============================================================================
DEPRECATED PUBLIC UINT32 fmd_GetRssiThreshold(VOID)
{
    return g_fmdConfig->seekRSSIThreshold;
}


// =============================================================================
// fmd_I2sClose
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sClose(VOID)
{
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_I2sOpen
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sOpen(FMD_I2S_CFG_T fmdI2sCfg)
{
    return FMD_ERR_NO;
}
