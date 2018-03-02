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
/*                                                                            */
/* FILE NAME                                                                  */
/*      ts_win32.c                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    Trace Service service declarations, constant definitions and macros     */
/*                                                                            */
/******************************************************************************/

#include "fmd_m.h"

static UINT32 g_curr_freq;
// =============================================================================
// fmd_Open
// -----------------------------------------------------------------------------
/// This function opens the FM driver and activate the FM chip.
/// @param fmdTgtCfg    configuration from TGT module, refer to documentation 
///                     of target module for details. 
///                     (nust not be \c NULL)
/// @param band         band selection
/// @param callback     callback for #fmd_Tune and #fmd_Seek
///                     (can be \c NULL)
/// @param initValues   initial channel & sound configuration 
///                     (can be \c NULL, in that case, the FM output is muted)
/// @return             \c FMD_ERR_NO, \c FMD_ERR_ALREADY_OPENED
///                     \c FMD_ERR_BAD_PARAMETER or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Open(FMD_BAND_T band,
                          FMD_CALLBACK_T* callback,
                          FMD_INIT_T* initValues)
{
    //g_curr_freq = initValues->freqKHz;
    return FMD_ERR_NO;
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
    g_curr_freq = freqKHz;
    return FMD_ERR_NO;
}


// =============================================================================
// fmd_Seek
// -----------------------------------------------------------------------------
/// This function seeks to the next FM channel.
/// Once the seek operation has found a channel, the callback registered 
/// at #fmd_Open will be called.
/// @param seekUp   \c TRUE : increasing frequency, \c FALSE: decreasing frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Seek(BOOL seekUp)
{
    return FMD_ERR_NO;
}


// =============================================================================
// fmd_I2sOpen
// -----------------------------------------------------------------------------
/// This function open the I2S interface
/// @param fmdI2sCfg I2S configuration 
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sOpen(FMD_I2S_CFG_T fmdI2sCfg)
{
    return FMD_ERR_NO;
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
// fmd_SetVolume
// -----------------------------------------------------------------------------
/// This function sets the volume and other audio related paramters.
/// @param volume       Analog Volume
/// @param bassBoost    Enable/Disable Bass Boost
/// @param forceMono    Enable/Disable Mono output of stereo radios
/// @return             \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_SetVolume(FMD_ANA_LEVEL_T volume,
                               BOOL bassBoost,
                               BOOL forceMono)
{
    return FMD_ERR_NO;
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
    *pFreqKHz = g_curr_freq;
    return FMD_ERR_NO;
}

PUBLIC FMD_ERR_T fmd_GetRssi(UINT8* pRSSI)
{
	*pRSSI = 10;
    return FMD_ERR_NO;
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
	if((g_curr_freq % 4000) == 1700)
		*pStereoStatus = TRUE;
    else
		*pStereoStatus = FALSE;
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_ValidStop
// -----------------------------------------------------------------------------
/// @param freqKHz      the channel frequency.
/// @return             \c TRUE if a channel is found or
///                     \c FALSE otherwise
// =============================================================================
PUBLIC BOOL fmd_ValidStop(UINT32 freqKHz)
{
        g_curr_freq = freqKHz;
	if((freqKHz % 2000) == 1700)
		return TRUE;
	else
		return FALSE;
}


