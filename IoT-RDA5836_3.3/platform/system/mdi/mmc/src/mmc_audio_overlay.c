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
// FILENAME: mmc_audio_overlay.c
//
// DESCRIPTION:
//  pcm overlay for audio play. 
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   shenh
//
////////////////////////////////////////////////////////////////////////////////
#include "cswtype.h"
#include "mci.h"

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#include "tgt_calib_m.h"
#endif

//uint32 pcmbuf_overlay[1];//18*1024 bytes
#define MCI_PCM_FIfO_BUF_SIZE        (512)  // 2KB
#define MCI_PCM_Voc_BUF_SIZE         (640)  // 2.5KB
#define MCI_PCM_NS_BUF_SIZE          (2560) // 10KB
//#define MCI_PCM_Pitch_BUF_SIZE          (2560) // 10KB

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
uint32 bigbuf[0x1000];
//#ifdef __AT_MOD_SPECIAL_
uint8 AudioOverlayBuf[400*2];  //for audiocalib and plc;
//#endif
CALIB_CALIBRATION_T  g_tgtCalibCfg;
#else
uint32 bigbuf[0x1000]  __attribute__((section(".pcm_big_datamem")));
#endif

uint32 *pcmbuf_overlay=bigbuf;
uint32 *inputbuf_overlay= bigbuf+MCI_PCM_BUF_SIZE;


/////////////////////////////////////////////////////////////////
//For NS and PotchFilter
/////////////////////////////////////////////////////////////////
uint32 *pBtSpeechFifoOverlay  = bigbuf;
uint32 *pBtSpeechVocOverlay  = bigbuf + MCI_PCM_FIfO_BUF_SIZE;
uint32 *pBtSpeechNSPcmOverlay = bigbuf + MCI_PCM_FIfO_BUF_SIZE + MCI_PCM_Voc_BUF_SIZE;

//pcmover is USED 16K   
