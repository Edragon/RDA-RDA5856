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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_calib_m.h                                                      //
/// That file provides the TGT Data Defines for calibration.             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_CALIB_M_H_
#define _TGT_CALIB_M_H_

#include "cs_types.h"
#include "tgt_calib_cfg.h"

/// Number of coefficiens in the MDF FIR filter.
#define CALIB_VOC_MDF_QTY                        (64)
/// Number of coefficiens in the SDF FIR filter.
#define CALIB_VOC_SDF_QTY                        (64)
/// Number of audio gain steps.
#define CALIB_AUDIO_GAIN_QTY                     (AUD_MAX_LEVEL+1)
#define CALIB_AUDIO_GAIN_MAX_QTY                     (32)
#define CALIB_AUDIO_GAIN_MIN_QTY                     (8)
/// The audio gain value standing for mute.
#define CALIB_AUDIO_GAIN_VALUE_MUTE              (-128)
/// Maximum number of different Audio Interfaces supported by this calibration structure.
/// The value of CALIB_AUDIO_TOTAL_ITF_QTY must be the equal as AUD_ITF_QTY!
#define CALIB_AUDIO_TOTAL_ITF_QTY                (8)
/// The echo param number, one for speaker, on for ear-piece
#define CALIB_AUDIO_ECHO_QTY                     (2)
/// This magiv tag is used as a parameter to the boot loader to force is to run the
/// calibration stub
#define CALIB_MAGIC_TAG                         (0XCA1BCA1B)
/// Number of audio IIR bands.
#define CALIB_AUDIO_IIR_BANDS 10
/// Number of audio call IIR bands.
#define CALIB_AUDIO_CALL_IIR_BANDS 7
/// Number of audio mp3 sdf gains.
#define CALIB_VOC_MP3_QTY     64

/// Version of the calibration stub and code (on 8 bits).
#define CALIB_MAJ_VERSION                       (3)
#define CALIB_MIN_VERSION                       (8)
#define CALIB_MARK_VERSION                      (0x58560000)
#define CALIB_VERSION_NUMBER                    (( CALIB_MARK_VERSION | CALIB_MAJ_VERSION << 8 | CALIB_MIN_VERSION ))
#define CALIB_STUB_VERSION                      (0XCA5B0001)


// ============================================================================
// CALIB_PARAM_STATUS_T
// -----------------------------------------------------------------------------
/// Calibration parameter type identifier.
// =============================================================================
typedef enum
{
    CALIB_PARAM_DEFAULT                         = 0xCA11B042,
    CALIB_PARAM_DEFAULT_RF_MIS                  = 0xCA11B043,
    CALIB_PARAM_INIT_ERROR                      = 0xCA10DEAD,
    CALIB_PARAM_RF_CALIBRATED                   = 0x007F0011,
    CALIB_PARAM_AUDIO_CALIBRATED                = 0x00A0D011,
    CALIB_PARAM_AUDIO_CALIBRATED_RF_MIS         = 0x00A0D043,
    CALIB_PARAM_CALIBRATED                      = 0x00DEF011
} CALIB_PARAM_STATUS_T;

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define CALIB_MAJ_MASK(x) ((x) & 0xFF)
#define CALIB_MIN_MASK(x) (x & 0xFF)
#define CALIB_MARK_MASK(x) (x & 0xFFFF0000)
#define CALIB_VERSION(maj, min) (CALIB_MARK_VERSION | maj << 8 | min << 0)



// ============================================================================
// CALIB_VERSION_TAG_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef UINT32 CALIB_VERSION_TAG_T;

// ============================================================================
// CALIB_COMMAND_T
// -----------------------------------------------------------------------------
///
// =============================================================================
typedef enum
{
/// Command used by the Calibration Embedded Stub to inform HST that the command
/// is done.
    CALIB_CMD_DONE                              = 0xCA11B042,
    CALIB_CMD_NOT_ACCESSIBLE                    = 0xCA11B043,
    CALIB_CMD_UPDATE                            = 0xCA11B044,
    CALIB_CMD_UPDATE_ERROR                      = 0xCA11B045,
    CALIB_CMD_PA_PROFILE_GSM                    = 0xCA11B046,
    CALIB_CMD_PA_PROFILE_DCSPCS                 = 0xCA11B047,
    CALIB_CMD_RF_FLASH_BURN                     = 0xCA11B048,
    CALIB_CMD_FLASH_ERASE                       = 0xCA11B049,
    CALIB_CMD_FLASH_ERROR                       = 0xCA11B04A,
    CALIB_CMD_RF_RESET                          = 0xCA11B04B,
    CALIB_CMD_AUDIO_RESET                       = 0xCA11B04C,
    CALIB_CMD_RESET                             = 0xCA11B04D,
    CALIB_CMD_AUDIO_FLASH_BURN                  = 0xCA11B04E,
    CALIB_CMD_FLASH_BURN                        = 0xCA11B04F,
    CALIB_CMD_CFP_BURN                          = 0xCA11B050
} CALIB_COMMAND_T;


// ============================================================================
// CALIB_AUDIO_IN_GAINS_T
// -----------------------------------------------------------------------------
/// Input (MIC) gains.
// =============================================================================
typedef struct
{
    /// Input analog gain.
    INT8                           ana;                          //0x00000000
    /// Input ADC gain.
    INT8                           adc;                          //0x00000001
    /// Input algorithm gain.
    INT8                           alg;                          //0x00000002
    /// Reserved.
    INT8                           reserv;                       //0x00000003
} CALIB_AUDIO_IN_GAINS_T; //Size : 0x4



// ============================================================================
// CALIB_AUDIO_OUT_GAINS_T
// -----------------------------------------------------------------------------
/// Output gains.
// =============================================================================
typedef struct
{
    /// Output voice gains for physical interfaces, or earpiece gains for application
    /// interfaces. Output analog gain.
    INT8                           voiceOrEpAna;                 //0x00000000
    /// Output DAC gain.
    INT8                           voiceOrEpDac;                 //0x00000001
    /// Output algorithm gain.
    INT8                           voiceOrEpAlg;                 //0x00000002
    /// Reserved.
    INT8                           reserv1;                      //0x00000003
    /// Output music gains for physical interfaces, or loudspeaker gains for application
    /// interfaces. Output analog gain.
    INT8                           musicOrLsAna;                 //0x00000004
    /// Output DAC gain.
    INT8                           musicOrLsDac;                 //0x00000005
    /// Output algorithm gain.
    INT8                           musicOrLsAlg;                 //0x00000006
    /// Reserved.
    INT8                           reserv2;                      //0x00000007
} CALIB_AUDIO_OUT_GAINS_T; //Size : 0x8

// ============================================================================
// CALIB_AUDIO_VOC_FILTERS_T
// -----------------------------------------------------------------------------
/// VoC anti-distortion filters.
// =============================================================================
typedef struct
{
    /// mdf flag.
    UINT16                         mdfflag;//0x00000000
    /// sdf flag.
    UINT16                         sdfflag;//0x00000002 
    /// VoC needs the MDF coeffs to be 32-bit aligned.
    UINT16                         mdfFilter[CALIB_VOC_MDF_QTY]; //0x00000004
    /// SDF coeffs must stay right after MDF.
    UINT16                         sdfFilter[CALIB_VOC_SDF_QTY]; //0x00000084
} CALIB_AUDIO_VOC_FILTERS_T; //Size : 0x104


// ============================================================================
// CALIB_AUDIO_GAINS_T
// -----------------------------------------------------------------------------
/// Calib audio gain types.
// =============================================================================
typedef struct
{
    /// MIC gains during a voice call.
    CALIB_AUDIO_IN_GAINS_T         inGainsCall;                  //0x00000000
    /// Output gains.
    //CALIB_AUDIO_OUT_GAINS_T        outGains[CALIB_AUDIO_GAIN_QTY]; //0x00000004
    CALIB_AUDIO_OUT_GAINS_T        outGains[CALIB_AUDIO_GAIN_MAX_QTY]; //0x00000004
    //INT8                           sideTone[CALIB_AUDIO_GAIN_QTY]; //0x00000044
    INT8                           sideTone[CALIB_AUDIO_GAIN_MAX_QTY]; //0x00000044
    /// MIC gains during a sound recording.
    CALIB_AUDIO_IN_GAINS_T         inGainsRecord;                //0x0000004C
    //INT8                           reserv[20];                   //0x00000050
} CALIB_AUDIO_GAINS_T; //Size : 0x64


//#if (AUDIO_CALIB_VER == 1)
#if 1
typedef struct
{
    INT16 in_enable;            //0x00000000
    INT16 out_enable;           //0x00000002

    INT16 scal_in;              //0x00000004
    INT16 scal_out;             //0x00000006
} CALIB_AUDIO_VOC_DigGain_T; //Size : 0x00000008

typedef enum
{
    SKIP = 0,
    LPF,
    HPF,
    peakingEQ,
    lowShelf,
    highShelf,
} FILTER_TYPE;

typedef struct
{
    //INT16 num[3];           //0x00000000
    //INT16 den[3];           //0x00000006

    INT16 gain;             //0x0000000c

    // info
    INT16 type;             //0x0000000e
    INT16 freq;             //0x00000010
    union {
        INT16 qual;
    INT16 slop;
    INT16 bw;
    } param;                 //0x00000012
    //INT16 reserved[2];       //0x00000014
} CALIB_AUDIO_IIR_EQ_BAND_T; //0x00000018

typedef struct
{
    INT16 num[3];           //0x00000000
    INT16 den[3];           //0x00000006

    INT16 gain;             //0x0000000c

    // info
    INT16 type;             //0x0000000e
    INT16 freq;             //0x00000010
    union {
        INT16 qual;
    INT16 slop;
    INT16 bw;
    } param;                 //0x00000012
    INT16 reserved[2];       //0x00000014
} CALIB_AUDIO_IIR_EQ_BAND_COEF_T; //0x00000018

typedef struct
{
    INT16 flag;             //0x00000000
    INT16 reserved;         //0x00000002
    CALIB_AUDIO_IIR_EQ_BAND_COEF_T band;//0x00000004
} CALIB_AUDIO_IIR_HPF_T; //Size : 0x0000001c

typedef struct
{
    //enable flag
    INT16 AecEnableFlag;		//0x00000000
    // deprecated, use flag in agc module
    INT16 AgcEnableFlag;		//0x00000002

    //ProcessPara
    INT16 ProcessMode;			//0x00000004
    INT16 InitRegularFactor;		//0x00000006
    INT16 AFUpdateFactor;		//0x00000008
    INT16 AFCtrlSpeedUpFactor;		//0x0000000a
    INT16 AFFilterAmp;			//0x0000000c

    INT16 EchoEstMethod;		//0x0000000e
    INT16 NoiseUpdateFactor;		//0x00000010
    INT16 SNRUpdateFactor;		//0x00000012
    INT16 SNRLimit;			//0x00000014
    INT16 NoiseOverEstFactor;		//0x00000016
    INT16 NoiseGainLimit;		//0x00000018
    INT16 EchoUpdateFactor;		//0x0000001a
    INT16 SERUpdateFactor;		//0x0000001c
    INT16 SERLimit;			//0x0000001e
    INT16 EchoOverEstFactor;		//0x00000020
    INT16 EchoGainLimit;		//0x00000022
    INT16 CNGFactor;			//0x00000024
    INT16 AmpGain;			//0x00000026
    INT32 NoiseMin;			//0x00000028

    INT32 PFPrevEchoEstLimit;		//0x0000002c
    INT16 PFDTEchoOverEstFactor;	//0x00000030

    INT16 pPFEchoGainLimitAmp[9];	//0x00000032

    INT16 NoiseGainLimitStep;		//0x00000044
    INT16 AmpThr;			//0x00000046

    INT16 PFCLDataSmoothFactor;		//0x00000048
    INT16 PFCLNoiseUpdateFactor;	//0x0000004a
    INT16 PFCLThrSigDet;		//0x0000004c
    INT16 PFCLThrDT;			//0x0000004e

    INT16 DataSmoothFactor;		//0x00000050
    INT16 PFCLChanlGain;		//0x00000052

    INT16 PFCLThrNT;			//0x00000054
    INT16 PFCLThrDTDiff;		//0x00000056
    INT16 PFCLDTHoldTime;		//0x00000058
    INT16 PFCLDTStartTime;		//0x0000005a

    INT16 PFCLDTDUpdateFactor;		//0x0000005c
    INT16 PFCLDTDThreshold;		//0x0000005e
    INT16 PFCLDTD2ndThreshold;		//0x00000060

    INT16 StrongEchoFlag;		//0x00000062

    INT16 PFCLDTDThrRatio;		//0x00000064

	/* below is not in VPP_SPEECH_AEC_INIT_T */
    INT16 DelaySampleNum;		//0x00000066

    INT16 FrameCntLimitNum[3];//0x00000068
    INT16 NoiseCntLimitNum[4]; //0x0000006E
    INT16 NoiseWeightSNRLimitMax; //0x00000076
    INT16 NoiseWeightSNRLimitMin; //0x00000078
    INT16 NoiseWeightSNRTh1; //0x0000007a
    INT16 NoiseWeightSNRTh2; //0x0000007c
    INT16 NoiseWeightSNRLimitStep; //0x0000007e
    INT16 NoiseWeightSub1DecayFactor; //0x00000080
    INT16 ResidualEchoEnable; //0x00000082
    INT16 AGCRrrSpkRelation;//0x00000084
    INT16 AddAttDB;//0x00000086
    INT16 MinAttDB;//0x00000088
    INT16 MaxAttDB;//0x0000008a
    INT16 NPowRiseFactor;//0x0000008c
    INT16 WorkFactFactor;//0x0000008e
    INT16 AttExtFactor;//0x00000090
    INT16 AttRiseFactor;//0x00000092
    INT16 AttDecayFactor;//0x00000094
    INT16 Log2LinFactor;//0x00000096
} CALIB_AUDIO_VOC_AEC_T; //Size : 0x00000098

typedef struct
{
    INT16 flag;             //0x00000000

    INT16 noise_suppress_default;   //0x00000002
    INT16 noise_probe_range[4];     //0x00000004

    INT16 reserved[2];          //0x0000000c
} CALIB_AUDIO_VOC_SPEEX_PATH_T; //Size : 0x00000010

typedef struct
{
    CALIB_AUDIO_VOC_SPEEX_PATH_T rx;    //0x00000000
    CALIB_AUDIO_VOC_SPEEX_PATH_T tx;    //0x00000010
} CALIB_AUDIO_VOC_SPEEX_T; //Size : 0x00000020

typedef struct
{
    INT16 flag;             //0x00000000

    INT16 smooth_threshold;     //0x00000002
    INT16 noise_threshold;      //0x00000004
    INT16 gain;             //0x00000006
} CALIB_AUDIO_VOC_AGC_PATH_T; //Size : 0x00000008

typedef struct
{
    CALIB_AUDIO_VOC_AGC_PATH_T  rx; //0x00000000
    CALIB_AUDIO_VOC_AGC_PATH_T  tx; //0x00000008
} CALIB_AUDIO_VOC_AGC_T; //Size : 0x00000010

typedef struct
{
INT16  enable;              //0x00000000
INT16  target_level_dbfs;         //0x00000002
INT16  compression_gain_dB;    //0x00000004
INT16  limiter_enable;              //0x00000006
INT16  upper_thr;                 //0x00000008
INT16  lower_thr;                 //0x0000000A
INT16  decay_value;             //0x0000000C
INT16  capacitor_slow_factor; //0x0000000E
INT16  capacitor_fast_factor; //0x00000010
INT16  noise_compress_factor; //0x00000012
INT16  noise_gate_thr; //0x00000014
INT16  noise_std_short_term_factor; //0x00000016
} CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T;//Size : 0x00000018

typedef struct
{
    CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T    rx;    //0x00000000
    CALIB_AUDIO_VOC_WEBRTC_AGC_PATH_T    tx;    //0x00000018
} CALIB_AUDIO_VOC_WEBRTC_AGC_T; //Size : 0x00000030

typedef struct
{
    INT16 flag;                         //0x00000000
    INT16 reserved;                         //0x00000002
    CALIB_AUDIO_IIR_EQ_BAND_T   band[CALIB_AUDIO_IIR_BANDS];    //0x00000004
} CALIB_AUDIO_IIR_EQ_T; //Size : 0x00000f4

typedef struct
{
    INT16 flag;                         //0x00000000
    INT16 reserved;                         //0x00000002
    CALIB_AUDIO_IIR_EQ_BAND_T   band[CALIB_AUDIO_CALL_IIR_BANDS];    //0x00000004
} CALIB_AUDIO_CALL_IIR_EQ_T; //Size : 0x00000AC

typedef struct
{
    INT16 alc_enable;

    INT16 thres;
    INT16 width;
    INT16 R;
    INT16 R1;
    INT16 R2;
    INT16 limit;
    INT16 M;
    INT16 alpha1;
    INT16 alpha2;
    INT16 noise_gate;
    INT16 alpha_max;
    INT16 Thr_dB;
    INT16 mm_gain;
    INT16 channel;
    INT16 reserved;
} CALIB_AUDIO_MUSIC_DRC_T; //Size : 0x000000e

typedef struct
{
    CALIB_AUDIO_IIR_EQ_T       eq;      //0x00000000
    CALIB_AUDIO_MUSIC_DRC_T        drc;     //0x000000a2
} CALIB_AUD_MUSIC_ITF_T; //Size : 0x000000b0

typedef struct
{
    CALIB_AUDIO_VOC_DigGain_T       gain; //0x08  
    CALIB_AUDIO_IIR_HPF_T           hpf;  //0x1c
    CALIB_AUDIO_VOC_AEC_T           aec;  //0x98
    CALIB_AUDIO_VOC_SPEEX_T         speex;//0x20
    CALIB_AUDIO_VOC_AGC_T           agc;   //0x10
    CALIB_AUDIO_VOC_FILTERS_T       fir;     //0x104
    CALIB_AUDIO_CALL_IIR_EQ_T            eq;    //0xa2
    CALIB_AUDIO_VOC_WEBRTC_AGC_T    webrtcagc; //0x30    
} CALIB_AUD_VOC_ITF_T; // Size : 0x000002C4

#endif

// ============================================================================
// CALIB_AUDIO_ITF_T
// -----------------------------------------------------------------------------
/// Calibration of an audio interface. It gathers the audio gains and VoC calibrations
/// data
// =============================================================================
typedef struct
{
    /// Version of audio calib  
    UINT32                          audioVersionTag;
    /// value of CALIB_AUDIO_GAIN_ITF_QTY
    INT16                          audioGainItfNums;    
	/// value of CALIB_AUDIO_VOC_ITF_QTY
    INT16                          audioVocItfNums;
	/// value of CALIB_AUDIO_MUSIC_ITF_QTY
    INT16                          audioMusicItfNums; 
    /// index inf audioGains for each itf
    INT8                           audioGainItfMap[CALIB_AUDIO_TOTAL_ITF_QTY];    
    /// index inf audioVoc for each itf
    INT8                           audioVocItfMap[CALIB_AUDIO_TOTAL_ITF_QTY];     
    /// index inf audioMusic for each itf
    INT8                           audioMusicItfMap[CALIB_AUDIO_TOTAL_ITF_QTY];    
    /// Calib audio gain types.
    CALIB_AUDIO_GAINS_T            audioGains[CALIB_AUDIO_GAIN_ITF_QTY];     
    /// Calib audio voc param
    CALIB_AUD_VOC_ITF_T            audioVoc[CALIB_AUDIO_VOC_ITF_QTY];        
    /// Calib audio music param
    CALIB_AUD_MUSIC_ITF_T          audioMusic[CALIB_AUDIO_MUSIC_ITF_QTY];          
} CALIB_AUDIO_ITF_T; //Size : 0x5e4


typedef struct
{
    /// Audio calibration.
    CALIB_AUDIO_ITF_T              aud;                                      //0x00000000
    CONST CALIB_AUDIO_ITF_T *      flash;
} CALIB_CALIBRATION_T; //Size : 0x5E4


typedef struct
{
    /// Audio calibration.
    CALIB_AUDIO_ITF_T              aud;                                      //0x00000000
} CALIB_BUFFER_T; //Size : 0x5E4

EXPORT PROTECTED CALIB_CALIBRATION_T g_calibCalibration;

//=============================================================================
// tgt_GetCalibConfig
//-----------------------------------------------------------------------------
/// Access to the calibration parameters.
///
/// @return The global pointers used by the code to access the calibrated
/// parameters. When the target's flash does not contain a flash sector,
/// the calib parameters are initialized with default parameters during
/// the calibration init. Also returns the pointers used by the remote
/// calibration tools to get the version number of the calibration and to 
/// find the calibration buffers.
//=============================================================================
PUBLIC CALIB_CALIBRATION_T* tgt_GetCalibConfig(VOID);


#endif // _TGT_CALIB_M_H_

