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
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
/// file tgt_calib_cfg.h                                                        //
///   This file include the default calibration data for this target            //
///The content can be generate by the calibration tool in RDAAudioCalibrator.exe//
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#ifndef _TGT_CALIB_CFG_H_
#define _TGT_CALIB_CFG_H_

/// Gain Calibration Config Count
#define CALIB_AUDIO_GAIN_ITF_QTY		2

/// VOC Calibration Config Count
#define CALIB_AUDIO_VOC_ITF_QTY			2

/// Music Calibration Config Count
#define CALIB_AUDIO_MUSIC_ITF_QTY		2

///itf
//AUD_ITF_RECEIVER      0,
//AUD_ITF_EAR_PIECE     1,
//AUD_ITF_LOUD_SPEAKER  2,
//AUD_ITF_BLUETOOTH     3,
//AUD_ITF_BT_EP         4,
//AUD_ITF_BT_SP         5,
//AUD_ITF_FM            6,
//AUD_ITF_LINEIN        7,

/// version of audio calib
#define CALIB_AUDIO_VERSION_TAG		0x5856e003

/// gain itf map
// calib_t.audio.audioGainItfMap
#define DEFAULT_CALIB_AUDIO_GAIN_MAP  {0, 0, 1, -1, 0, 1, 0, 0, }

// voc itf map
// calib_t.audio.audioVocItfMap
#define DEFAULT_CALIB_AUDIO_VOC_MAP   {0, 0, 1, -1, 0, 1, -1, -1, }

/// music itf map
// calib_t.audio.audioMusicItfMap
#define DEFAULT_CALIB_AUDIO_MUSIC_MAP {0, 0, 0, 1, 1, 0, 0, 0, }

///Audio gains parameters 
#define DEFAULT_CALIB_GAINS_ALL {\
		DEFAULT_CALIB_AUDIO_GAINS_EP,\
		DEFAULT_CALIB_AUDIO_GAINS_LS,\
		}

// calib_t.audio.audioGains[0]
#define DEFAULT_CALIB_AUDIO_GAINS_EP  { MIC_GAINS_EP, DEFAULT_CALIB_AUDIO_OUT_EP,  DEFAULT_CALIB_AUDIO_SIDE_EP, MIC_GAINS_RECORD_EP}

// calib_t.audio.audioGains[1]
#define DEFAULT_CALIB_AUDIO_GAINS_LS  { MIC_GAINS_LS, DEFAULT_CALIB_AUDIO_OUT_LS,  DEFAULT_CALIB_AUDIO_SIDE_LS, MIC_GAINS_RECORD_LS}

// calib_t.audio.audioGains[calib_t.audio.audioGainItfMap[itf]].inGainsCall
#define MIC_GAINS_EP { 12, 0, 0, 0 }
#define MIC_GAINS_LS { 12, 0, 0, 0 }
// calib_t.audio.audioGains[calib_t.audio.audioGainItfMap[itf]].outGains
#define DEFAULT_CALIB_AUDIO_OUT_EP  { \
		{0, -26, -45, 0, 0, -26, -45, 0},\
		{0, -25, -9, 0, 0, -25, -9, 0},\
		{0, -24, -6, 0, 0, -25, -6, 0},\
		{0, -23, -3, 0, 0, -25, -3, 0},\
		{0, -22, 0, 0, 0, -25, 0, 0},\
		{3, -21, 0, 0, 3, -23, 0, 0},\
		{3, -20, 0, 0, 3, -21, 0, 0},\
		{3, -16, 0, 0, 3, -19, 0, 0},\
		{3, -15, 0, 0, 3, -18, 0, 0},\
		{6, -13, 0, 0, 6, -17, 0, 0},\
		{6, -12, 0, 0, 6, -15, 0, 0},\
		{6, -10, 0, 0, 6, -12, 0, 0},\
		{6, -9, 0, 0, 6, -9, 0, 0},\
		{6, -7, 0, 0, 6, -7, 0, 0},\
		{6, -5, 0, 0, 6, -5, 0, 0},\
		{6, -3, 0, 0, 6, -3, 0, 0},\
		{6, -1, 0, 0, 6, -1, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0}}

#define DEFAULT_CALIB_AUDIO_OUT_LS  { \
		{0, -26, -45, 0, 0, -26, -45, 0},\
		{0, -25, -9, 0, 0, -25, -9, 0},\
		{0, -24, -6, 0, 0, -25, -6, 0},\
		{0, -23, -3, 0, 0, -25, -3, 0},\
		{0, -22, 0, 0, 0, -25, 0, 0},\
		{3, -21, 0, 0, 3, -23, 0, 0},\
		{3, -20, 0, 0, 3, -21, 0, 0},\
		{3, -16, 0, 0, 3, -19, 0, 0},\
		{3, -15, 0, 0, 3, -18, 0, 0},\
		{6, -13, 0, 0, 6, -17, 0, 0},\
		{6, -12, 0, 0, 6, -15, 0, 0},\
		{6, -10, 0, 0, 6, -12, 0, 0},\
		{6, -9, 0, 0, 6, -9, 0, 0},\
		{6, -7, 0, 0, 6, -7, 0, 0},\
		{6, -5, 0, 0, 6, -5, 0, 0},\
		{6, -3, 0, 0, 6, -3, 0, 0},\
		{6, -1, 0, 0, 6, -1, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0},\
		{6, 0, 0, 0, 6, 0, 0, 0}}

// calib_t.audio.audioGains[calib_t.audio.audioGainItfMap[itf]].sideTone
#define DEFAULT_CALIB_AUDIO_SIDE_EP  { \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, }

#define DEFAULT_CALIB_AUDIO_SIDE_LS  { \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, \
		-6, -6, -6, -6, -6, -6, -6, -6, }

// calib_t.audio.audioGains[calib_t.audio.audioGainItfMap[itf]].inGainsRecord
#define MIC_GAINS_RECORD_EP { 0, 0, 0, 0 }

#define MIC_GAINS_RECORD_LS { 0, 0, 0, 0 }

///  Audio VoC params.
#define DEFAULT_CALIB_VOC_ALL {\
		DEFAULT_CALIB_AUDIO_VOC_PARAM_EP,\
		DEFAULT_CALIB_AUDIO_VOC_PARAM_LS,\
		}

// calib_t.audio.audioVoc[0]
#define DEFAULT_CALIB_AUDIO_VOC_PARAM_EP  \
{\
DEFAULT_CALIB_AUDIO_VOC_DIGGAIN_PARAM_EP, \
DEFAULT_CALIB_AUDIO_IIR_HPF_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_AEC_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_SPEEX_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_AGC_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_FIR_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_IIR_EQ_PARAM_EP, \
DEFAULT_CALIB_AUDIO_VOC_WEBRTCAGC_PARAM_EP, \
}

// calib_t.audio.audioVoc[1]
#define DEFAULT_CALIB_AUDIO_VOC_PARAM_LS  \
{\
DEFAULT_CALIB_AUDIO_VOC_DIGGAIN_PARAM_LS, \
DEFAULT_CALIB_AUDIO_IIR_HPF_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_AEC_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_SPEEX_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_AGC_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_FIR_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_IIR_EQ_PARAM_LS, \
DEFAULT_CALIB_AUDIO_VOC_WEBRTCAGC_PARAM_LS, \
}

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].gain
#define DEFAULT_CALIB_AUDIO_VOC_DIGGAIN_PARAM_EP		{ 0x0, 0x0, 0x400, 0x400 }

#define DEFAULT_CALIB_AUDIO_VOC_DIGGAIN_PARAM_LS		{ 0x0, 0x0, 0x400, 0x400 }

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].hfp
#define DEFAULT_CALIB_AUDIO_IIR_HPF_PARAM_EP		{ 1, 0, {{14354, -28709, 14354}, {16384, 28457, -12578}, 0, 2, 180, {0}, {0, 0}} }

#define DEFAULT_CALIB_AUDIO_IIR_HPF_PARAM_LS		{ 1, 0, {{14354, -28709, 14354}, {16384, 28457, -12578}, 0, 2, 180, {0}, {0, 0}} }

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].aec
#define DEFAULT_CALIB_AUDIO_VOC_AEC_PARAM_EP { \
		1, 0, 6, 8, 4, 10, 32, 0, \
		64, 4, 13, 0, 128, 32, 16, 3, \
		2, 51, 0, 16, 0, 1023, 20, \
		{15, 15, 15, 15, 15, 15, 15, 15, 15}, \
		0, 0x7f, 128, 64, 160, 4, 256, \
		128, 64, 6, 160, 16, 16, 100, \
		64, 0, 71, 25, {500, 2000, 5000}, \
		{100, 200, 300, 600}, 0, 0, \
		128, 128, 16, 0, 0, 0, 0, 0, \
		0, 8, 4, 5, 0, 5, 0 }

#define DEFAULT_CALIB_AUDIO_VOC_AEC_PARAM_LS { \
		1, 0, 6, 8, 4, 10, 32, 0, \
		64, 4, 13, 0, 128, 32, 16, 3, \
		2, 51, 0, 16, 0, 1023, 20, \
		{15, 15, 15, 15, 15, 15, 15, 15, 15}, \
		0, 0x7f, 128, 64, 160, 4, 256, \
		128, 64, 6, 160, 16, 16, 100, \
		64, 0, 71, 25, {500, 2000, 5000}, \
		{100, 200, 300, 600}, 0, 0, \
		128, 128, 16, 0, 0, 0, 0, 0, \
		0, 8, 4, 5, 0, 5, 0 }

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].speex
#define DEFAULT_CALIB_AUDIO_VOC_SPEEX_PARAM_EP  \
{\
{0, -30, {30, 30, 30, 30}, {0, 0} },\
{1, -20, {15, 40, 40, 40}, {0, 0} },\
}

#define DEFAULT_CALIB_AUDIO_VOC_SPEEX_PARAM_LS  \
{\
{0, -30, {30, 30, 30, 30}, {0, 0} },\
{1, -20, {15, 40, 40, 40}, {0, 0} },\
}

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].agc
#define DEFAULT_CALIB_AUDIO_VOC_AGC_PARAM_EP  \
{\
{0, 0, 0, 0x3},\
{0, 0, 0, 0x9},\
}

#define DEFAULT_CALIB_AUDIO_VOC_AGC_PARAM_LS  \
{\
{0, 0, 0, 0x9},\
{0, 0, 0, 0x3},\
}

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].webrtcagc
#define DEFAULT_CALIB_AUDIO_VOC_WEBRTCAGC_PARAM_EP  \
{\
{0, 1, 12, 1, 1024, 0, -65, 500, -1000, 512, 1000, 4},\
{1, 1, 10, 1, 1024, 0, -65, 500, -1000, 512, 1000, 4},\
}

#define DEFAULT_CALIB_AUDIO_VOC_WEBRTCAGC_PARAM_LS  \
{\
{0, 1, 12, 1, 1024, 0, -65, 500, -1000, 512, 1000, 4},\
{1, 1, 10, 1, 1024, 0, -65, 500, -1000, 512, 1000, 4},\
}

// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].fir
#define DEFAULT_CALIB_AUDIO_VOC_FIR_PARAM_EP  { 0, 0, DEFAULT_CALIB_MDF_PROFILE_EP, DEFAULT_CALIB_SDF_PROFILE_EP }

#define DEFAULT_CALIB_AUDIO_VOC_FIR_PARAM_LS  { 0, 0, DEFAULT_CALIB_MDF_PROFILE_LS, DEFAULT_CALIB_SDF_PROFILE_LS }

#define DEFAULT_CALIB_MDF_PROFILE_EP { 0x4000, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0}

#define DEFAULT_CALIB_SDF_PROFILE_EP { 0x4000, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0}

#define DEFAULT_CALIB_MDF_PROFILE_LS { 0x4000, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0}

#define DEFAULT_CALIB_SDF_PROFILE_LS { 0x4000, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0, \
				0x0, 0x0, 0x0, 0x0}


// calib_t.audio.audioVoc[calib_t.audio.audioVocItfMap[itf]].eq
#define DEFAULT_CALIB_AUDIO_VOC_IIR_EQ_PARAM_EP  \
{\
1,\
0,\
{\
{0, 0, 200, {7}},\
{-3, 3, 450, {7}},\
{3, 3, 1100, {10}},\
{6, 3, 2400, {7}},\
{9, 0, 2900, {5}},\
{6, 0, 3000, {7}},\
{11, 0,3200, {7}},\
}\
}

#define DEFAULT_CALIB_AUDIO_VOC_IIR_EQ_PARAM_LS  \
{\
1,\
0,\
{\
{0, 0, 200, {7}},\
{-3, 3, 450, {7}},\
{3, 3, 1100, {10}},\
{6, 3, 2400, {7}},\
{9, 0, 2900, {5}},\
{6, 0, 3000, {7}},\
{11, 0,3200, {7}},\
}\
}

///  Audio Music params.
#define DEFAULT_CALIB_MUSIC_ALL {\
		DEFAULT_CALIB_AUDIO_MUSIC_PARAM_EP,\
		DEFAULT_CALIB_AUDIO_MUSIC_PARAM_LS,\
}
/// calib_t.audio_music.music[itf]
#define DEFAULT_CALIB_AUDIO_MUSIC_PARAM_EP \
{\
DEFAULT_CALIB_AUDIO_MUSIC_IIR_EQ_PARAM_EP,\
DEFAULT_CALIB_AUDIO_MUSIC_DRC_PARAM_EP,\
}
#define DEFAULT_CALIB_AUDIO_MUSIC_PARAM_LS \
{\
DEFAULT_CALIB_AUDIO_MUSIC_IIR_EQ_PARAM_LS,\
DEFAULT_CALIB_AUDIO_MUSIC_DRC_PARAM_LS,\
}
// calib_t.audio.audioMusic[calib_t.audio.audioMusicItfMap[itf]].eq
#define DEFAULT_CALIB_AUDIO_MUSIC_IIR_EQ_PARAM_EP  \
{\
0,\
0,\
{\
{0, 2, 50, {7}},\
{0, 0, 150, {7}},\
{0, 0, 200, {7}},\
{0, 0, 400, {7}},\
{0, 0, 1000, {7}},\
{0, 0, 2000, {7}},\
{0, 0, 4000, {7}},\
{0, 0, 8000, {7}},\
{0, 0, 16000, {7}},\
{0, 1, 20000, {7}},\
}\
}

#define DEFAULT_CALIB_AUDIO_MUSIC_IIR_EQ_PARAM_LS  \
{\
0,\
0,\
{\
{0, 2, 50, {7}},\
{0, 0, 150, {7}},\
{0, 0, 200, {7}},\
{0, 0, 400, {7}},\
{0, 0, 1000, {7}},\
{0, 0, 2000, {7}},\
{0, 0, 4000, {7}},\
{0, 0, 8000, {7}},\
{0, 0, 16000, {7}},\
{0, 1, 20000, {7}},\
}\
}

// calib_t.audio.audioMusic[calib_t.audio.audioMusicItfMap[itf]].drc
#define DEFAULT_CALIB_AUDIO_MUSIC_DRC_PARAM_EP   { 0, -10, 6, 1, 1, 2, -3, 8, 1024, 512, -70, 512, -1, 0, 2, 0 }

#define DEFAULT_CALIB_AUDIO_MUSIC_DRC_PARAM_LS   { 0, -10, 6, 1, 1, 2, -3, 8, 1024, 512, -70, 512, -1, 0, 2, 0 }

#endif //_TGT_CALIB_CFG_H_
