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

#include "cs_types.h"
#include "chip_id.h"

#include "snd_types.h"
#include "calib_m.h"

#include "string.h"



PUBLIC VOID calib_ConvertStructV2p5(CALIB_BB_T *pCalibBb)
{
    for (int i = 0; i < CALIB_AUDIO_ITF_QTY; i++)
    {
        memcpy(&pCalibBb->audio[i].audioGains.inGainsRecord,
            &pCalibBb->audio[i].audioGains.inGainsCall,
            sizeof(pCalibBb->audio[i].audioGains.inGainsRecord));
    }
}


