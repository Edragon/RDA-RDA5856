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
// FILENAME: mmc_audiolineinRecord.c
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

#include "mmc_audiolineinRecord.h"

#include "mmc.h"
#include "cpu_share.h"

#include "audio_api.h"

#include "aud_m.h"
#include "fs_asyn.h"

#include "cos.h"
#include "mci.h"
#include "sxs_type.h"
#include "sxr_sbx.h"
#include "sxr_mem.h"

#include "string.h"
#include "hal_overlay.h"

#include "mmc_adpcm.h"
#include "resample.h"
#include "event.h"

#include "vpp_audiosbc_enc.h"

#ifdef LINEIN_RECORDER_SUPPORT

//#define SBCENC_DEBUG
#define SBCENC_DEBUG_TICKS

#define LINEIN_REC_PCM_BUF_LEN (4*1024)
#define LINEIN_IN_PCM_BUF_LEN  (4*1024)

#define LINEIN_REC_PCM_BUF_HALF_LEN (LINEIN_REC_PCM_BUF_LEN/2)

#define LINEIN_OUT_SBC_BUF_LEN (8*1024)


#define Rec_pcm_buffer pcmbuf_overlay
#define Input_pcm_buffer (pcmbuf_overlay + LINEIN_REC_PCM_BUF_LEN/4)
#define Output_sbc_buffer (Input_pcm_buffer + LINEIN_IN_PCM_BUF_LEN/4)

/* sample rate */
//#define SBC_FREQ_16000		  16000
//#define SBC_FREQ_32000		  32000
//#define SBC_FREQ_44100		  44100
//#define SBC_FREQ_48000		  48000

/* channel mode */
//#define SBC_MODE_MONO		  0x00
//#define SBC_MODE_DUAL_CHANNEL 0x01
//#define SBC_MODE_STEREO		  0x02
//#define SBC_MODE_JOINT_STEREO 0x03

/* allocation method */
//#define SBC_AM_LOUDNESS		  0x00
//#define SBC_AM_SNR		      0x01

//source set

#define SOURCE_SAMPLE  SBC_FREQ_16000
#define SOURCE_CH      HAL_AIF_MONO     

UINT8 gSoundSource =  SOURCE_MIC_IN;


//sbcenc set
#define SBC_ENC_SAMPLE_RATE      SOURCE_SAMPLE 
#define SBC_ENC_BIT_POOL         32
#define SBC_ENC_ALLOCATION       SBC_AM_LOUDNESS

//must match
#define SBC_ENC_INPUT_CH_MODE    SOURCE_CH
#define SBC_ENC_OUTPUT_CH_MODE   SBC_MODE_JOINT_STEREO //mono not support.



LINEIN_PLAY *LineInPlayer;

INT32 *G_VppSBCEncBakConstY=NULL;
extern const INT32 G_VppSBCConstX[];

UINT8* sbc_buffer;

static UINT8 sbc_will_encoding = 0;

#ifdef SBCENC_DEBUG_TICKS
    UINT32 g_SbcHalfEndTicks = 0;
#endif

#ifdef SBCENC_DEBUG
UINT32 g_read_len = 0;
short g_sin_wave[520] = 
{
#if 1
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,   
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,   
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,   
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,   
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
    0x0000, 0x4B3C, 0x79BC, 0x79BB, 0x4B3B, 0x0000, 0xB4C5, 0x8644, 0x8644,0xB4C4,	
#endif //sample:16000 fre:1600hz ch:1

#if 0
	0x0000,	0x0000,	0x12CE,	0x2534,	0x2535,	0x4732,	0x36CC,	0x630A,	
	0x4732,	0x7655,	0x560C,	0x7F67,	0x630A,	0x7D7B,	0x6DE0,	0x70B6,	
	0x7655,	0x5A37,	0x7C38,	0x3BEE,	0x7F67,	0x1878,	0x7FD6,	0xF2E4,	
	0x7D7A,	0xCE73,	0x7867,	0xAE4A,	0x70B6,	0x952E,	0x6694,	0x854C,	
	0x5A38,	0x8002,	0x4BE6,	0x85C5,	0x3BEE,	0x9617,	0x2AAA,	0xAF8F,	
	0x1879,	0xCFF7,	0x05BF,	0xF487,	0xF2E5,	0x1A13,	0xE053,	0x3D60,	
	0xCE74,	0x5B5F,	0xBDA7,	0x717C,	0xAE4A,	0x7DC9,	0xA0B3,	0x7F3C,	
	0x952E,	0x75B3,	0x8BFA,	0x61FE,	0x854B,	0x45D4,	0x8146,	0x23A2,	
	0x8001,	0xFE5C,	0x8183,	0xD93B,	0x85C6,	0xB772,	0x8CAD,	0x9BED,	
	0x9618,	0x890E,	0xA1CD,	0x8072,	0xAF8E,	0x82DC,	0xBF0F,	0x9013,	
	0xCFF8,	0xA6F5,	0xE1ED,	0xC587,	0xF487,	0xE925,	0x0762,	0x0EBD,	
	0x1A14,	0x330F,	0x2C34,	0x52F8,	0x3D5E,	0x6BB8,	0x4D37,	0x7B2A,	
	0x5B60,	0x7FF9,	0x678E,	0x79BA,	0x717C,	0x68FB,	0x78F3,	0x4F29,	
	0x7DCA,	0x2E82,	0x7FE6,	0x09D7,	0x7F3D,	0xE452,	0x7BCE,	0xC130,	
	0x75B4,	0xA37C,	0x6D07,	0x8DC5,	0x61FF,	0x81EB,	0x54D5,	0x80F3,	
	0x45D4,	0x8AF5,	0x354F,	0x9F13,	0x23A2,	0xBB8E,	0x112E,	0xDDF3,	
	0xFE5C,	0x0349,	0xEB91,	0x2855,	0xD93B,	0x49E7,	0xC7BA,	0x6517,	
	0xB773,	0x778B,	0xA8BD,	0x7FAD,	0x9BEE,	0x7CCA,	0x914A,	0x6F1F,	
	0x890D,	0x57DB,	0x8367,	0x3902,	0x8072,	0x153D,	0x8044,	0xEFA2,	
	0x82DC,	0xCB70,	0x882A,	0xABCA,	0x9014,	0x9368,	0x9A6A,	0x8467,	
	0xA6F5,	0x8012,	0xB56E,	0x86C9,	0xC587,	0x97F7,	0xD6E4,	0xB223,	
	0xE925,	0xD306,	0xFBE6,	0xF7CC,	0x0EBD,	0x1D48,	0x2143,	0x403C,	
	0x3311,	0x5DA4,	0x43C0,	0x72F6,	0x52F8,	0x7E5B,	0x6064,	0x7ED6,	
	0x6BB9,	0x745F,	0x74B5,	0x5FD9,	0x7B29,	0x430D,	0x7EF2,	0x2077,	
	0x7FF7,	0xFB13,	0x7E37,	0xD61D,	0x79BB,	0xB4C5,	0x729A,	0x99EA,	
	0x68FA,	0x87E2,	0x5D14,	0x8038,	0x4F2A,	0x8397,	0x3F86,	0x91B4,	
	0x2E82,	0xA957,	0x1C7B,	0xC876,	0x09D6,	0xEC61,	0xF6FB,	0x11FE,	
	0xE451,	0x360E,	0xD243,	0x5572,	0xC130,	0x6D74,	0xB17C,	0x7C04,	
	0xA37D,	0x7FDE,	0x977E,	0x78AD,	0x8DC5,	0x6711,	0x8687,	0x4C8F,	
	0x81EA,	0x2B6F,	0x800D,	0x0691,	0x80F4,	0xE11F,	0x849E,	0xBE59,	
	0x8AF6,	0xA13F,	0x93D7,	0x8C54,	0x9F14,	0x8165,	0xAC67,	0x8164,	
	0xBB8F,	0x8C54,	0xCC31,	0xA13F,	0xDDF3,	0xBE5A,	0xF074,	0xE120,	
	0x0348,	0x0690,	0x160E,	0x2B6F,	0x2855,	0x4C8E,	0x39BF,	0x6711,	
	0x49E8,	0x78AD,	0x5874,	0x7FDE,	0x6517,	0x7C04,	0x6F87,	0x6D74,	
	0x778C,	0x5572,	0x7CF8,	0x360E,	0x7FAD,	0x11FF,	0x7F9F,	0xEC62,	
	0x7CC9,	0xC877,	0x7740,	0xA958,	0x6F1F,	0x91B3,	0x6495,	0x8397,	
	0x57DB,	0x8037,	0x493B,	0x87E0,	0x3902,	0x99EA,	0x278E,	0xB4C4,	
	0x153C,	0xD61D,	0x0277,	0xFB14,	0xEFA2,	0x2078,	0xDD28,	0x430D,	
	0xCB71,	0x5FD9,	0xBADD,	0x745F,	0xABCA,	0x7ED6,	0x9E8A,	0x7E5B,	
	0x9367,	0x72F6,	0x8AA1,	0x5DA4,	0x8465,	0x403C,	0x80DB,	0x1D47,	
	0x8012,	0xF7CD,	0x8211,	0xD306,	0x86C9,	0xB222,	0x8E23,	0x97F8,	
	0x97F8,	0x86C9,	0xA40E,	0x8012,	0xB223,	0x8466,	0xC1E8,	0x9368,	
	0xD306,	0xABCA,	0xE51F,	0xCB71,	0xF7CD,	0xEFA2,	0x0AA8,	0x153C,	
	0x1D49,	0x3902,	0x2F45,	0x57DB,	0x403C,	0x6F1F,	0x4FCF,	0x7CCA,	
	0x5DA5,	0x7FAD,	0x6972,	0x778B,	0x72F6,	0x6516,	0x79FD,	0x49E6,	
	0x7E5B,	0x2855,	0x7FFC,	0x0348,	0x7ED6,	0xDDF3,	0x7AEF,	0xBB8F,	
	0x745E,	0x9F12,	0x6B45,	0x8AF5,	0x5FD9,	0x80F3,	0x5259,	0x81EB,	
	0x430D,	0x8DC5,	0x324E,	0xA37C,	0x2078,	0xC131,	0x0DED,	0xE452,	
	0xFB13,	0x09D7,	0xE856,	0x2E81,	0xD61D,	0x4F2A,	0xC4CC,	0x68FB,	
	0xB4C3,	0x79BB,	0xA65D,	0x7FF9,	0x99EC,	0x7B2A,	0x8FAD,	0x6BB8,	
	0x87E1,	0x52F9,	0x82B0,	0x330F,	0x8036,	0x0EBD,	0x8085,	0xE926,	
	0x8395,	0xC587,	0x895B,	0xA6F4,	0x91B3,	0x9013,	0x9C71,	0x82DC,	
	0xA958,	0x8072,	0xB81F,	0x890E,	0xC878,	0x9BED,	0xDA03,	0xB772,	
	0xEC62,	0xD93A,	0xFF2E,	0xFE5C,	0x11FE,	0x23A2,	0x246B,	0x45D4,	
	0x360D,	0x61FE,	0x4683,	0x75B1,	0x5571,	0x7F3C,	0x6285,	0x7DCA,	
	0x6D75,	0x717C,	0x7603,	0x5B5F,	0x7C04,	0x3D61,	0x7F53,	0x1A15,	
	0x7FDF,	0xF487,	0x7DA3,	0xCFF7,	0x78AD,	0xAF8E,	0x711A,	0x9617,	
	0x6712,	0x85C6,	0x5ACD,	0x8001,	0x4C8D,	0x854B,	0x3CA7,	0x952E,	
	0x2B6E,	0xAE49,	0x1946,	0xCE73,	0x0691,	0xF2E4,	0xF3B6,	0x1877,	
	0xE120,	0x3BEE,	0xCF35,	0x5A38,	0xBE59,	0x70B6,	0xAEEB,	0x7D7A,	
	0xA140,	0x7F68,	0x95A2,	0x7655,	0x8C54,	0x630B,	0x8587,	0x4732,	
	0x8165,	0x2534,	0x8000,	0x0000,	0x8164,	0xDACC,	0x8588,	0xB8CE,	
	0x8C54,	0x9CF7,	0x95A2,	0x89AB,	0xA140,	0x8098,	0xAEEC,	0x8286,	
	0xBE5A,	0x8F49,	0xCF35,	0xA5C8,	0xE11F,	0xC412,	0xF3B7,	0xE788
#endif //sample:44100 fre:L1035hz R2070hz ch:2
};


#endif

UINT8 sbc_test_timer_id = 0;
UINT8 sbc_test_flag = 0;
UINT8* test_buffer = NULL;
UINT8* sbc_test_buffer = NULL;
UINT8* pcm_test_buffer = NULL;
/**/
INT32 pcm_cnt = -1;
UINT32 sbc_cnt = 0;
UINT8 g_LineInRecHalfEnd = 0;

UINT8 bt_start_send_linein_sbc = 0;
UINT16 source_mtu = 0;


extern AUD_LEVEL_T audio_cfg;
extern AUD_ITF_T audioItf;
/*
* NAME:	MMC_AudioLineInVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioLineInVocISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]VoC INT!");
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_LINEIN_SBC_VOC);
} 

/*
* NAME:	MMC_AudioLineInVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioLineInRecHalfISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]REC HALF INT!");
#ifdef SBCENC_DEBUG_TICKS
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x11100000 + sbcenc_tick - g_SbcHalfEndTicks);
	g_SbcHalfEndTicks = sbcenc_tick;
#endif
#ifdef SBCENC_DEBUG
    memcpy((UINT8*)Rec_pcm_buffer, g_sin_wave+g_read_len, 1024);
    g_read_len += 1024/2;
	g_read_len %= 10;
	memcpy((UINT8*)Rec_pcm_buffer+1024, g_sin_wave+g_read_len, 1024);
	g_read_len += 1024/2;
	g_read_len %= 10;
#endif
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_LINEIN_REC_HALF);
} 

/*
* NAME:	MMC_AudioLineInVocISR()
* DESCRIPTION: ISR for VOC decode a frame.
*/
void MMC_AudioLineInRecEndISR(void)
{
//	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]REC END INT!");
#ifdef SBCENC_DEBUG_TICKS
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x22200000 + sbcenc_tick - g_SbcHalfEndTicks);
	g_SbcHalfEndTicks = sbcenc_tick;
#endif
#ifdef SBCENC_DEBUG
    memcpy((UINT8*)Rec_pcm_buffer+LINEIN_REC_PCM_BUF_HALF_LEN, g_sin_wave+g_read_len, 1024);
    g_read_len += 1024/2;
	g_read_len %= 10;
	memcpy((UINT8*)Rec_pcm_buffer+LINEIN_REC_PCM_BUF_HALF_LEN+1024, g_sin_wave+g_read_len, 1024);
	g_read_len += 1024/2;
	g_read_len %= 10;
#endif
  	mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_MMC_LINEIN_REC_END);
} 

UINT32 MMC_AudioLineInGetPcmLength()
{
    LINEIN_INPUT *input = &LineInPlayer->LineInInput;
    return input->length;
}

/*
* NAME:	MMC_AudioLineinDecOpen()
* DESCRIPTION: Initialize linein decoder.
*/
MCI_ERR_T MMC_AudioLineinDecOpen (HANDLE fhd, LINEIN_PLAY *pLineInPlayer)
{
    if(pLineInPlayer == NULL)
    {
        hal_HstSendEvent(SYS_EVENT, 0x13121744);
    }      
	memset(pLineInPlayer, 0, sizeof(LINEIN_PLAY));
#if 0
    if(pcm_test_buffer == NULL)
    {
        pcm_test_buffer = COS_Malloc(512*1024);
    }

    if(sbc_test_buffer == NULL)
    {
        sbc_test_buffer = COS_Malloc(2048*100);
    }
#endif
    pcm_cnt = 0;//start linein pcm record
    
#ifdef SBCENC_DEBUG
    g_read_len = 0;
#endif

    LINEIN_INPUT *input = &pLineInPlayer->LineInInput;
    LINEIN_OUTPUT *output = &pLineInPlayer->LineInOutput;        

    input->data = Input_pcm_buffer;
    output->Buffer = Output_sbc_buffer;

	memset(Rec_pcm_buffer, 0x00, LINEIN_REC_PCM_BUF_LEN);
	memset(input->data, 0x00, LINEIN_IN_PCM_BUF_LEN);
	memset(output->Buffer, 0x00, LINEIN_OUT_SBC_BUF_LEN);

    hal_HstSendEvent(SYS_EVENT, 0x13120701);
    hal_HstSendEvent(SYS_EVENT, output->Buffer);
    hal_HstSendEvent(SYS_EVENT, input->length);
    if(output->Buffer == NULL)
    {
        hal_DbgAssert("Malloc output->Buffer failed...");
    }
    
    HAL_ERR_T voc_ret = 0;

    input->is_first_frame=1;

#if 1
	pLineInPlayer->Voc_AudioEncStatus.consumedLen = 0;
	pLineInPlayer->Voc_AudioEncStatus.nbChannel = 0;
	pLineInPlayer->Voc_AudioEncStatus.output_len = 0;
	pLineInPlayer->Voc_AudioEncStatus.streamStatus= 0;
		
	pLineInPlayer->Voc_AudioEncIN.EQ_Type=-1;
	pLineInPlayer->Voc_AudioEncIN.reset=1;
  
	/*initialize I/O data struct*/
	G_VppSBCEncBakConstY = (INT32*)mmc_MemMalloc(468*4);
	
	if(G_VppSBCEncBakConstY==NULL)
	{
		mmc_MemFreeAll();
		return MCI_ERR_OUT_OF_MEMORY;		
	}

	/*open voc decoder*/
	voc_ret = vpp_AudioSbcEncOpen((HAL_VOC_IRQ_HANDLER_T)MMC_AudioLineInVocISR);
    hal_HstSendEvent(SYS_EVENT, 0x13120601);
    hal_HstSendEvent(SYS_EVENT, voc_ret);
	switch (voc_ret)
	{
		case HAL_ERR_NO:
			break;
			
		case HAL_ERR_RESOURCE_RESET:
		case HAL_ERR_RESOURCE_BUSY:  
		case HAL_ERR_RESOURCE_TIMEOUT:
		case HAL_ERR_RESOURCE_NOT_ENABLED:	  
		case HAL_ERR_BAD_PARAMETER:
		case HAL_ERR_UART_RX_OVERFLOW:
		case HAL_ERR_UART_TX_OVERFLOW:
		case HAL_ERR_UART_PARITY:
		case HAL_ERR_UART_FRAMING:
		case HAL_ERR_UART_BREAK_INT:
		case HAL_ERR_TIM_RTC_NOT_VALID:
		case HAL_ERR_TIM_RTC_ALARM_NOT_ENABLED:
		case HAL_ERR_TIM_RTC_ALARM_NOT_DISABLED:
		case HAL_ERR_COMMUNICATION_FAILED:
		case HAL_ERR_QTY:
			MCI_TRACE (MCI_AUDIO_TRC|TSTDOUT,0,"[MMC_LINEIN]Fail to Call vpp_AudioJpegDecOpen()t: %d \n", voc_ret);
			return MCI_ERR_ERROR;
		default:
			break;
	}
	MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]MMC_AudioLineinDecOpen end!");
#endif

    hal_HstSendEvent(SYS_EVENT, 0x1312060b);
    hal_HstSendEvent(SYS_EVENT, input->length);

	return MCI_ERR_NO;
	
}


VOID MMC_Audio_Side_Set(AUD_ITF_T *itf, UINT8 source)
{
    // Local variable passed to a function, but copied in the function.
    HAL_AIF_STREAM_T inAudioStream;
	HAL_AIF_STREAM_T outAudioStream;

    // Initial cfg
    //if(gpio_detect_earpiece())
    //    *itf = AUD_ITF_EAR_PIECE;
    //else
    //    *itf = AUD_ITF_LOUD_SPEAKER;
    hal_HstSendEvent(SYS_EVENT, 0x20161628);

	audioItf = AUD_ITF_EAR_PIECE;
	
    if (audioItf == AUD_ITF_LOUD_SPEAKER)
    {
        audio_cfg.micLevel  = AUD_MIC_ENABLE;
        audio_cfg.spkLevel  = AUD_SPK_VOL_7;
        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
    }
    else if (audioItf == AUD_ITF_EAR_PIECE)
  {
  
        audio_cfg.micLevel  = AUD_MIC_ENABLE;

        audio_cfg.spkLevel  = AUD_SPK_VOL_7;

        audio_cfg.sideLevel = AUD_SIDE_VOL_TEST;
    }

    inAudioStream.startAddress = Rec_pcm_buffer;
    inAudioStream.length       = LINEIN_REC_PCM_BUF_LEN;
    inAudioStream.sampleRate   = SOURCE_SAMPLE;//HAL_AIF_FREQ_8000HZ
    inAudioStream.channelNb    = SOURCE_CH;//HAL_AIF_STEREO;
    inAudioStream.voiceQuality = !TRUE;
    inAudioStream.playSyncWithRecord = FALSE;
    inAudioStream.halfHandler  = MMC_AudioLineInRecHalfISR;
    inAudioStream.endHandler   = MMC_AudioLineInRecEndISR;

	outAudioStream.startAddress = Rec_pcm_buffer;
    outAudioStream.length       = LINEIN_REC_PCM_BUF_LEN;
    outAudioStream.sampleRate   = SOURCE_SAMPLE;//HAL_AIF_FREQ_8000HZ
    outAudioStream.channelNb    = SOURCE_CH;//HAL_AIF_STEREO;
    outAudioStream.voiceQuality = !TRUE;
    outAudioStream.playSyncWithRecord = FALSE;
    outAudioStream.halfHandler  = NULL;//MMC_AudioLineInRecHalfISR;
    outAudioStream.endHandler   = NULL;//MMC_AudioLineInRecEndISR;

	switch(source)
	{
	    case  SOURCE_MIC_IN :  
							while (aud_StreamStart(audioItf, &outAudioStream, &audio_cfg) != AUD_ERR_NO)
							{
							    sxr_Sleep(1 MS_WAITING);
								hal_HstSendEvent(SYS_EVENT, 0x20161629);

							}
							break;
	    case  SOURCE_LINE_IN :  
							while (aud_StreamStart(audioItf, &outAudioStream, &audio_cfg) != AUD_ERR_NO)
						    {
						        sxr_Sleep(1 MS_WAITING);
								hal_HstSendEvent(SYS_EVENT, 0x20161630);

						    }
			                break;
		default : hal_HstSendEvent(SYS_EVENT, 0x20161631);break;
	}


    if (audio_cfg.sideLevel == AUD_SIDE_VOL_TEST)
    {
        sxr_Sleep(20 MS_WAITING);
        // The sound is replayed after 800ms (1s - 200ms)
    }

    while (aud_StreamRecord(audioItf, &inAudioStream, &audio_cfg) != AUD_ERR_NO)
    {
        sxr_Sleep(1 MS_WAITING);
		hal_HstSendEvent(SYS_EVENT, 0x20161632);
    }
    //aud_TestModeSetup(audioItf, &sideAudioStream, &audio_cfg, AUD_TEST_SIDE_TEST);
}
MCI_ERR_T MMC_AudioSbcEncVoc(LINEIN_PLAY *pLineInPlayer)
{
    static UINT32 timestamp;
    static UINT32 seq_num;
    int status = 0;
    static BOOL retry_flag = FALSE;
    
    LINEIN_INPUT *input = &pLineInPlayer->LineInInput;
    LINEIN_OUTPUT *output = &pLineInPlayer->LineInOutput;

    vpp_AudioSbc_ENC_OUT_T *pVoc_AudioDecStatus = &pLineInPlayer->Voc_AudioEncStatus;
    vpp_AudioSbc_ENC_IN_T *pVoc_Voc_AudioDecIN = &pLineInPlayer->Voc_AudioEncIN;

    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]MMC_AudioSbcEncVoc! input->length = %d", input->length);

#ifdef SBCENC_DEBUG_TICKS
    UINT32 sbcenc_tick;
	sbcenc_tick = csw_TMGetTick();
    hal_HstSendEvent(SYS_EVENT,0x33300000 + sbcenc_tick - g_SbcHalfEndTicks);
#endif
	
    if(input->is_first_frame==1)
    {
	      input->is_first_frame=0;
	   
		  sbc_will_encoding = 1;

		  MMC_Audio_Side_Set(&audioItf, gSoundSource);
		  
		  return MCI_ERR_NO;

	}  
	else
	{	    
        static int sbclen, sbcaddr;
		
		vpp_AudioSbcEncStatus(pVoc_AudioDecStatus);
		//BT_A2DP_sbc

		UINT32 sbc_length = 0;
		UINT8* sbc_output_addr =  (UINT8*) hal_VocGetPointer(SBC_Out_put_addr);
		sbc_length = pVoc_AudioDecStatus->ImgWidth;
        if(sbclen != sbc_length || sbcaddr != sbc_output_addr)
        {
			sbclen = sbc_length;
			sbcaddr = sbc_output_addr;
			
			hal_HstSendEvent(SYS_EVENT, 0x20161402);
			hal_HstSendEvent(SYS_EVENT, pVoc_AudioDecStatus->ErrorStatus);
			hal_HstSendEvent(SYS_EVENT, sbc_length);
			hal_HstSendEvent(SYS_EVENT, sbc_output_addr);
		}
		
#ifndef SBCENC_DEBUG
		memcpy((UINT8*)output->Buffer, sbc_output_addr, sbc_length);
#else

		memcpy((UINT8*)output->Buffer+sbc_cnt, sbc_output_addr, sbc_length);
        sbc_cnt += sbc_length;
        if(sbc_cnt + sbc_length >= 8*1024)
        {			
            hal_HstSendEvent(SYS_EVENT, 0x20161403);
        	hal_HstSendEvent(SYS_EVENT, sbc_cnt);
			hal_HstSendEvent(SYS_EVENT, output->Buffer);

            hal_DbgAssert("You can dump sbc data from 0x%x, length = %d...", output->Buffer, sbc_cnt);
        }
#endif            


#if 0
        //send a2dpdata
        if(sbc_will_encoding== 0)
        {
            hal_HstSendEvent(SYS_EVENT, 0x13121803);
			//hal_HstSendEvent(SYS_EVENT, output->Buffer);
             if(!retry_flag)
                {
                    status = Avdtp_Send_Data(sbc_length, (UINT8*)output->Buffer, timestamp, seq_num, 0,0, 4);
                    if(status == 0)
                    {
                        retry_flag = FALSE;
                        if(seq_num==0xFFFFFFFF)
    		    			seq_num=0;
    		    seq_num++;
    		    timestamp += 1152;        
                    }
                    else
                        retry_flag = TRUE;
                }
                else
                {
                        status = Avdtp_Send_Data(sbc_length, (UINT8*)output->Buffer, timestamp, seq_num, 0,0, 4);
                        if(status == 0)
                        {
                            retry_flag = FALSE;
			

			if(seq_num==0xFFFFFFFF)
				seq_num=0;

			seq_num++;
			timestamp += 1152;
                        }
                        else
            	    {
            		retry_flag = TRUE;	
            	    }
                }
            }
           
#endif             
 	}
	if(input->length >= 1024)//feed pcm data to voc
	{
//#ifndef SBCENC_DEBUG
#if 1
            memcpy( (UINT8*) hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR), input->data, 1024);
#else
            memcpy( (UINT8*) hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR), g_sin_wave+g_read_len, 1024);
            g_read_len += 1024/2;
			g_read_len %= 10;
#endif
        //if(input->length > 1024)
        {
			memcpy(input->data, input->data+1024, input->length - 1024);
		}
		input->length -= 1024;

		pVoc_Voc_AudioDecIN->mode=0xd;                               //sbcenc
		pVoc_Voc_AudioDecIN->BsbcEnable=1;
		pVoc_Voc_AudioDecIN->audioItf=0;
		
		pVoc_Voc_AudioDecIN->Source_width=SBC_ENC_OUTPUT_CH_MODE;
		pVoc_Voc_AudioDecIN->Source_height=SBC_ENC_ALLOCATION;
		
		pVoc_Voc_AudioDecIN->Cut_width=SBC_ENC_BIT_POOL;
		
		pVoc_Voc_AudioDecIN->Zoomed_width=SBC_ENC_INPUT_CH_MODE;     // inputch
		pVoc_Voc_AudioDecIN->Zoomed_height=SBC_ENC_SAMPLE_RATE;
		
		pVoc_Voc_AudioDecIN->inStreamUBufAddr=(INT32*)G_VppSBCConstX;
		pVoc_Voc_AudioDecIN->inStreamVBufAddr=(INT32*)G_VppSBCEncBakConstY;
		  
		vpp_AudioSbcEncScheduleOneFrame(pVoc_Voc_AudioDecIN);
		sbc_will_encoding = 0;
	}
	else
	{
		sbc_will_encoding = 1;
		//do nothing
	}
	
	return MCI_ERR_NO;

}
MCI_ERR_T MMC_AudioSbcEncPcm(UINT32 rec_half_end)
{
    INT16 i;

    if(Rec_pcm_buffer == NULL)
    {
        hal_DbgAssert("Rec_pcm_buffer is NULL in MMC_AudioSbcEncPcm...");
    }

    UINT8* pcm_tmp = rec_half_end?Rec_pcm_buffer+LINEIN_REC_PCM_BUF_HALF_LEN/4: Rec_pcm_buffer;
    
    LINEIN_INPUT *input = &LineInPlayer->LineInInput;
    LINEIN_OUTPUT *output = &LineInPlayer->LineInOutput;


    vpp_AudioSbc_ENC_IN_T *pVoc_Voc_AudioDecIN = &LineInPlayer->Voc_AudioEncIN;

    if((input->length + LINEIN_REC_PCM_BUF_HALF_LEN )> LINEIN_REC_PCM_BUF_LEN)
    {
		input->length = 0;
		hal_HstSendEvent(SYS_EVENT, 0x5bc00001);
	}
	{
		static UINT32 count = 0;
        count++;
		if(count >= 10)
		{
			memcpy(input->data + input->length, pcm_tmp, LINEIN_REC_PCM_BUF_HALF_LEN);
	        input->length += LINEIN_REC_PCM_BUF_HALF_LEN;
		}
	}
    MCI_TRACE (MCI_AUDIO_TRC,0, "[MMC_LINEIN]MMC_AudioSbcEncPcm! input->length = %d", input->length);

    if(sbc_will_encoding == 1)
    {
        if(input->length >= 1024)//start sbc encode
        {

//#ifndef SBCENC_DEBUG
#if 1
            memcpy( (UINT8*) hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR), input->data, 1024);
#else
            memcpy( (UINT8*) hal_VocGetPointer(OUTPUT_PCM_BUFFER1_ADDR), g_sin_wave+g_read_len, 1024);
            g_read_len += 1024/2;
			g_read_len %= 10;
#endif
            if(input->length > 1024)
            {
            	memcpy(input->data, input->data+1024, input->length - 1024);
			}
            input->length -= 1024;
			
            //src >= dest

			pVoc_Voc_AudioDecIN->mode=0xd;                               //sbcenc
			pVoc_Voc_AudioDecIN->BsbcEnable=1;
			pVoc_Voc_AudioDecIN->audioItf=0;

			pVoc_Voc_AudioDecIN->Source_width=SBC_ENC_OUTPUT_CH_MODE;
			pVoc_Voc_AudioDecIN->Source_height=SBC_ENC_ALLOCATION;
			
			pVoc_Voc_AudioDecIN->Cut_width=SBC_ENC_BIT_POOL;
			
			pVoc_Voc_AudioDecIN->Zoomed_width=SBC_ENC_INPUT_CH_MODE;     // inputch
			pVoc_Voc_AudioDecIN->Zoomed_height=SBC_ENC_SAMPLE_RATE;
			
			pVoc_Voc_AudioDecIN->inStreamUBufAddr=(INT32*)G_VppSBCConstX;
			pVoc_Voc_AudioDecIN->inStreamVBufAddr=(INT32*)G_VppSBCEncBakConstY;
						
            vpp_AudioSbcEncScheduleOneFrame(pVoc_Voc_AudioDecIN);
            sbc_will_encoding = 0;
        }
    }

    return MCI_ERR_NO;
}
/*
* NAME:	MMC_AudioLineinDecClose()
* DESCRIPTION: Close linein decoder.
*/
MCI_ERR_T MMC_AudioLineinDecClose (LINEIN_PLAY *pLineInPlayer)
{
	hal_HstSendEvent(SYS_EVENT,0x10250101);

	if(pLineInPlayer != NULL)
	{
    	COS_Free(pLineInPlayer);
	}
	else
	{
    	return  MCI_ERR_NO;
	}
    vpp_AudioSbcEncClose();
	
	aud_StreamStop(audioItf);
	
	mmc_MemFreeAll();

    return  MCI_ERR_NO;
	
}


int32 Audio_LineinPlay(int32 OutputPath, HANDLE fhd,int16 filetype,int32 PlayProgress)
{
    int32 result;

    if(LineInPlayer == NULL)
        LineInPlayer = COS_Malloc(sizeof(LINEIN_PLAY));
    if((result= MMC_AudioLineinDecOpen(fhd, LineInPlayer))!=MCI_ERR_NO)
    {
    	MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_LINEIN]MMC_AudioLineinDecOpen() RETURN FALSE!");
    	MMC_AudioLineinDecClose(LineInPlayer);
    	return result;
    }
    
    return  MCI_ERR_NO;
}

int32 Audio_LineinStop (void) 	
{
	MMC_AudioLineinDecClose(LineInPlayer);
	return  MCI_ERR_NO;
}


int32 Audio_LineinPause (void)	
{
        MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_LINEIN] Audio_LineinPause!!!!");
	
	return  MCI_ERR_NO;
}


int32 Audio_LineinResume ( HANDLE fhd) 
{	
	return  MCI_ERR_NO;
}


int32 Audio_LineinGetID3 (char * pFileName)  
{
	return 0;	
}

int32 Audio_LineinGetPlayInformation (MCI_PlayInf * MCI_PlayInfLineIn)  
{
	return MCI_ERR_NO;
}

int32 Audio_LineinUserMsg(COS_EVENT *ev)
{

    int32 nMsg = ev->nEventId;  

	switch (nMsg)
	{
#if 1
		case MSG_MMC_LINEIN_SBC_VOC:
			 MMC_AudioSbcEncVoc(LineInPlayer);
			break;
#endif		
		case MSG_MMC_LINEIN_REC_HALF:
			 MMC_AudioSbcEncPcm(0);
		break;
		
		case MSG_MMC_LINEIN_REC_END:
			 MMC_AudioSbcEncPcm(1);
		break;

        case MSG_MMC_LINEIN_START_SEND_SBC:
                bt_start_send_linein_sbc = 1;
                source_mtu = (u_int16)ev->nParam1;
                hal_HstSendEvent(SYS_EVENT, 0x20161104);
                hal_HstSendEvent(SYS_EVENT, source_mtu);
            break;

                    default:
                            MCI_TRACE (MCI_AUDIO_TRC,0,"[MMC_LINEIN]Audio_LineinUserMsg receives unexpected event!!!! nMsg = %d", nMsg);
                             break;

	}
    return 0;
}

#endif

