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
/// @file hal_ana_gpadc.h                                                   
///                                                                         
/// Contains the GPADC driver API
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef _HAL_ANA_GPADC_H_
#define _HAL_ANA_GPADC_H_

#include "cs_types.h"

/// @defgroup ana_gpadc HAL ANA GPADC
/// This document describes the HAL GPADC API that provides access to 
/// the GPADC hardware. 
/// 
/// The features of the HAL GPADC API include:
/// - Enable continuous conversion mode ;
/// - Get latest result during continuous conversion mode.
///
/// @par GPADC Interface
/// The chip contains a General Purpose Analog to Digital Converter that can be 
/// used to measure 4 external  analog signals that are connected to the chip pins. 
/// This API performs the task of enabling or disabling the GPADC,
/// The measured analog values are repeatedly updated.
///
/// @par Operation
/// Before being used, the Gpadc driver must be opened by a call to the function
/// #hal_AnaGpadcOpen. This function takes a resource. The function's parameter 
/// configuration structure is used to choose which external
/// signal we want to measure, selecting which GPADC channel are used, and 
/// and the time between between two consecutive measurements.
/// 
/// These measures are done continuously. The latest result can be obtained
/// through a call to #hal_AnaGpadcGet, which returns the tension measured on a 
/// given channel, in mV. The measures are automatically (in hardware) updated
/// at the frequency specified during the opening. We thus have always access
/// to a valid data, once the first measures have been made. For every channel.
///
/// Consequenty, the #hal_AnaGpadcGet will return an invalid result, #HAL_ANA_GPADC_BAD_VALUE,
/// before these first measure have been gotten, or when a non-enabled channel is 
/// measured.
///
/// Once the GPADC is not needed anymore, or before its configuration can be 
/// changed, the #hal_AnaGpadcClose function must be called. This free a resource.
/// 
/// @{


// =============================================================================
// MACROS
// =============================================================================

// =============================================================================
// HAL_ANA_GPADC_BAD_VALUE
// -----------------------------------------------------------------------------
/// Value returned when the gpadc has not finished its convertion.
// =============================================================================
#define HAL_ANA_GPADC_BAD_VALUE  0xFFFF


// =============================================================================
// TYPES
// =============================================================================

// =============================================================================
// HAL_ANA_GPADC_CHAN_T
// -----------------------------------------------------------------------------
/// Channel selection.
// =============================================================================
typedef enum
{
    HAL_ANA_GPADC_CHAN_0,
    HAL_ANA_GPADC_CHAN_1,
    HAL_ANA_GPADC_CHAN_2,
    HAL_ANA_GPADC_CHAN_3,
#if (CHIP_HAS_ASYNC_TCU)
    HAL_ANA_GPADC_CHAN_4,
    HAL_ANA_GPADC_CHAN_5,
    HAL_ANA_GPADC_CHAN_6,
    HAL_ANA_GPADC_CHAN_7,
#endif
    HAL_ANA_GPADC_CHAN_QTY,
} HAL_ANA_GPADC_CHAN_T;


// =============================================================================
// HAL_ANA_GPADC_ATP_T
// -----------------------------------------------------------------------------
/// Acquisition Time Period, for continuous aquisition.
///
/// In continous mode, this defines the time between two consecutive
/// measurements.
// =============================================================================
typedef enum
{
    HAL_ANA_GPADC_ATP_122US = 0,
    HAL_ANA_GPADC_ATP_1MS,
    HAL_ANA_GPADC_ATP_10MS,
    HAL_ANA_GPADC_ATP_100MS,
    HAL_ANA_GPADC_ATP_250MS,
    HAL_ANA_GPADC_ATP_500MS,
    HAL_ANA_GPADC_ATP_1S,
    HAL_ANA_GPADC_ATP_2S,

    HAL_ANA_GPADC_ATP_QTY
} HAL_ANA_GPADC_ATP_T;


// =============================================================================
// HAL_ANA_GPADC_MV_T
// -----------------------------------------------------------------------------
/// MilliVolt type, used to return value from the GPADC.
// =============================================================================
typedef UINT16 HAL_ANA_GPADC_MV_T;


// =============================================================================
// FUNCTIONS
// =============================================================================


// =============================================================================
// hal_AnaGpadcOpen
// -----------------------------------------------------------------------------
/// Open a GPADC channel and start periodic measurements.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// That function starts measurements on one of the 4 periodic GPADC channels.
/// The ATP (Acquisition Time Period)is common, so the minimal value of atp
/// is used. Using atp value above HAL_ANA_GPADC_ATP_100MS allows to enter 
/// low power between the measure.
/// The measured channels are selected through
/// #HAL_ANA_GPADC_CFG_T parameter.
///
/// @param channel Channel to enable
/// @param atp Atp value required for that channel
// =============================================================================
PUBLIC VOID hal_AnaGpadcOpen(HAL_ANA_GPADC_CHAN_T channel, HAL_ANA_GPADC_ATP_T atp);


// =============================================================================
// hal_AnaGpadcClose
// -----------------------------------------------------------------------------
/// Close a GPADC channel. 
///
/// This function release the resource to #HAL_SYS_FREQ_32K when all channels
/// are closed.
///
/// This function stops the periodic measurements on a particular channel.
/// It close the GPADC hardware when all channels are disabled.
// =============================================================================
PUBLIC VOID hal_AnaGpadcClose(HAL_ANA_GPADC_CHAN_T channel);


// =============================================================================
// hal_AnaGpadcGet
// -----------------------------------------------------------------------------
/// Return the more up-to-date value measure for the given channel in mV. 
/// If the conversion is not finished yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
///
/// Once the first measured has been correctly done, there will always
/// be a value ready to read for all the enabled channel.
///
/// @param channel Channel whose more up to date measured value
/// will be returned.
/// @return Measured value, in mV, or #HAL_ANA_GPADC_BAD_VALUE if no result is 
/// available yet.
// =============================================================================
PUBLIC HAL_ANA_GPADC_MV_T hal_AnaGpadcGet(HAL_ANA_GPADC_CHAN_T channel);


// =============================================================================
// hal_AnaGpadcGetRaw
// -----------------------------------------------------------------------------
/// Return the more up do date value measured for the specified GPADC channel,
/// if available, as the raw value read in the GPADC register. (no conversion
/// towards voltage is done.)
/// If no result is available because the conversion is not ready
/// yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
/// 
/// @param channel Channel whose more up to date measured value will be returned.
/// @return Raw measured value for this channel by the GPADC,  or 
/// #HAL_ANA_GPADC_BAD_VALUE if no result is available yet.
// =============================================================================
PUBLIC UINT16 hal_AnaGpadcGetRaw(HAL_ANA_GPADC_CHAN_T channel);


// =============================================================================
// hal_AnaGpadcGetCalibHigh
// -----------------------------------------------------------------------------
/// Get GPADC Calib High sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibHigh(VOID);


// =============================================================================
// hal_AnaGpadcGetCalibLow
// -----------------------------------------------------------------------------
/// Get GPADC Calib low sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibLow(VOID);


// =============================================================================
// hal_AnaGpadcGetTscX
// -----------------------------------------------------------------------------
/// This function get the touch screen X coordinate value 
// =============================================================================
PUBLIC UINT16 hal_AnaGpadcGetTscX(VOID);


// =============================================================================
// hal_AnaGpadcGetTscY
// -----------------------------------------------------------------------------
/// This function get the touch screen Y coordinate value 
// =============================================================================
PUBLIC UINT16 hal_AnaGpadcGetTscY(VOID);

///  @} <- End of the gpadc group

#endif //_HAL_ANA_GPADC_H_


