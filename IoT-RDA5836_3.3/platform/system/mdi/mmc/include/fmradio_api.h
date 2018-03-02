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
//
// FILENAME: fmradio_api.h
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   jiashuo
//
////////////////////////////////////////////////////////////////////////////////
#include "cs_types.h"



uint32 LILY_FM_RDA5800_init (uint8 scl,uint8 sda) ;			

uint32 LILY_FM_RDA5800_close(void);

uint32 LILY_FM_RDA5800_open(uint32 bUseLastFreq);

uint32 LILY_FM_RDA5800_tune(uint32 iFrequency,uint32 isStereo);

uint32 LILY_FM_RDA5800_mute(uint32 isMute);

uint32 LILY_FM_RDA5800_setVolume(uint32 iVol);

uint32 LILY_FM_RDA5800_seek(uint32 isSeekUp);

uint32 LILY_FM_RDA5800_I2S(void);

uint32 LILY_FM_RDA5800_STOPI2S(void);




