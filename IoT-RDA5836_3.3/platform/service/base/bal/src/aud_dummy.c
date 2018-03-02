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

#include "cs_types.h"
#include "hal_sys.h"

#include "hal_usb.h"
#include "hal_mem_map.h"
#include "hal_ispi.h"
#include "hal_gpio.h"
#include "opal.h"


#include "aud_m.h"
#include "vois_m.h"

#ifdef TARGET_MEDIA_SUPPORT_OFF

uint32 bigbuf[0x1000]; // 16k

PUBLIC AUD_ERR_T aud_Setup(SND_ITF_T itf, CONST AUD_LEVEL_T* cfg)
{
    AUD_ERR_T errStatus = AUD_ERR_NO;
    return errStatus;
}

PUBLIC VOIS_ERR_T BTSco_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg)
{
    AUD_ERR_T audErr = AUD_ERR_NO;
    return audErr;
}

PUBLIC VOIS_ERR_T BTSco_Start(
                        CONST AUD_ITF_T      itf,
                        CONST VOIS_AUDIO_CFG_T* cfg)
{
    AUD_ERR_T audErr = AUD_ERR_NO;
    return audErr;
}

PUBLIC VOIS_ERR_T BTSco_Stop(VOID)
{
    AUD_ERR_T audErr = AUD_ERR_NO;
    return audErr;
}
VOID BTSco_AudCallReset()
{
}

INT16 *MMC_SCOGetRXBuffer(UINT8 flag)
{
     return (INT16*)bigbuf;
}
VOID MMC_SCOReceiveData(INT32 length)
{
}

BOOL BTSco_AudCallStarted()
{
	return FALSE;
}

PUBLIC AUD_ERR_T aud_CodecGalliteInit(VOID)
{
     return AUD_ERR_NO;
}

#endif

