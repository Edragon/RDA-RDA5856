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


/*************************************************************************/
/*                                                                       */
/// @file rfd_defs.h
/// @author Laurent BASTIDE
/// @par Purpose
/// This document describes the structures shared between PAL and
/// RF Drivers
/*                                                                       */
/*************************************************************************/


#ifndef _RFD_DEFS_H
#define _RFD_DEFS_H

#include "hal_tcu.h"

//--------------------------------------------------------------------//
//                                                                    //
//               Memory Location definitions for                      //
//                time-critical text and data                         //
//                                                                    //
//--------------------------------------------------------------------//
#define RFD_FUNC_INTERNAL           // __attribute__((section(".sramtext")))
#define RFD_DATA_INTERNAL            __attribute__((section(".sramdata")))
#define RFD_UNCACHED_DATA_INTERNAL   __attribute__((section(".sramucdata")))

//--------------------------------------------------------------------//
//                                                                    //
//                       ARFCN limits                                 //
//                                                                    //
//--------------------------------------------------------------------//
// GSM
#define RFD_ARFCN_GSM850_MIN         128
#define RFD_ARFCN_GSM850_MAX         251
// EGSM RX
#define RFD_ARFCN_EGSM_RX_MIN1       0
#define RFD_ARFCN_EGSM_RX_MAX1       124
#define RFD_ARFCN_EGSM_RX_MIN2       975
#define RFD_ARFCN_EGSM_RX_MAX2       1023
// EGSM TX
#define RFD_ARFCN_EGSM_TX_MIN1_1     0
#define RFD_ARFCN_EGSM_TX_MAX1_1     24
#define RFD_ARFCN_EGSM_TX_MIN1_2     50
#define RFD_ARFCN_EGSM_TX_MAX1_2     124
#define RFD_ARFCN_EGSM_TX_MIN2       25
#define RFD_ARFCN_EGSM_TX_MAX2       49
#define RFD_ARFCN_EGSM_TX_MIN3       975
#define RFD_ARFCN_EGSM_TX_MAX3       1023
// DCS
#define RFD_ARFCN_DCS_MIN            512
#define RFD_ARFCN_DCS_MAX            885
// PCS
#define RFD_ARFCN_PCS_MIN            512
#define RFD_ARFCN_PCS_MAX            810

//--------------------------------------------------------------------//
//                                                                    //
//                       AGC Related Values                           //
//                                                                    //
//--------------------------------------------------------------------//
#define RFD_AGC                 0xff

//--------------------------------------------------------------------//
//                                                                    //
//                      TYPES and ENUMs                               //
//                                                                    //
//--------------------------------------------------------------------//

/// @brief Flags for Rx/Tx Window selection
typedef enum {
    RFD_DIR_RX, ///< Rx Window
    RFD_DIR_TX, ///< Tx Window
    RFD_DIR_RX_DOWN, ///< End of Rx Window
    RFD_DIR_TX_DOWN, ///< End of Tx Window

    RFD_DIR_QTY
} RFD_RFDIR_T;

/// @brief Flags for Tx ramp direction
typedef enum {
    RFD_RAMP_UP, ///< Rising Ramp
    RFD_RAMP_DOWN, ///< Falling Ramp

    RFD_RAMP_QTY
} RFD_PA_RAMPDIR_T;

/// @brief Flags for AFC Calibration control
typedef enum {
    RFD_AFC_BOUND_CENTER = 0, ///< Ask for AFC midrange value
    RFD_AFC_BOUND_NEG = 0x1, ///< Ask for AFC negative value
    RFD_AFC_BOUND_POS = 0x2, ///< Ask for AFC positive value

    RFD_AFC_BOUND_QTY
} RFD_AFC_BOUND_T;

typedef enum {
    RFD_NO_DCOC = 0,
    RFD_HW_DCOC_ENABLED = 0x1,
    RFD_SW_DCOC_ENABLED = 0x2
} RFD_DCOC_SUPPORT_T;

/// Low Power modes
typedef enum {
    /// Low power mode/setting. \n
    /// Module in normal power on state
    RFD_POWER_ON,
    /// Module's power supplies will not be shut down in this mode
    RFD_LOW_POWER,
    /// Ultra low power. \n Module's power supplies will be shut down in this mode
    /// (module Vdd switched off but not Vi/o)
    RFD_POWER_OFF,
    RFD_LP_MODE_QTY
} RFD_LP_MODE_T;

typedef enum {
    RFD_DIGRF_NORM_CLK_POL = 0,
    RFD_DIGRF_INV_CLK_POL = 1
} RFD_DIGRF_CLKPOL_T;

typedef enum {
    RFD_DIGRF_1_SAMPLE_PER_SYMB = 0,
    RFD_DIGRF_2_SAMPLE_PER_SYMB = 1
} RFD_DIGRF_OVERSAMP_T;

typedef enum {
    RFD_DIGRF_TX_STREAM = 0,
    RFD_DIGRF_TX_BLOCK = 1
} RFD_DIGRF_TX_MODE_T;

typedef enum {
    RFD_DIGRF_SAMPLE_ALIGN_MSB= 0,
    RFD_DIGRF_SAMPLE_ALIGN_LSB= 1
} RFD_DIGRF_RX_SAMPLE_ALIGNMENT_T;

typedef enum
{
    /// Normal behaviour: V_RF active high and low during low power.
    RFD_LDO_BEHAVIOR_NORMAL = 0,
    /// Always high behaviour: V_RF Active high and high during low power.
    RFD_LDO_BEHAVIOR_ALWAYS_HIGH,
    /// The V_RF from pmd is not used by the transceiver (no LDO).
    RFD_LDO_BEHAVIOR_DONT_USE,
    
    RFD_LDO_BEHAVIOR_QTY
} RFD_LDO_BEHAVIOR_T;

typedef enum {
    RFD_TX_GMSK_MOD= 0,
    RFD_TX_8PSK_MOD= 1
} RFD_TX_MODULATION_T;

//======================================================================
/// RFD RF Window structure
/// This structure contains the window descriptor filled by PAL.
/// All fields are input fields except @b swapIQ that must be filled by
/// the Xcver's driver for each opened window.
//======================================================================
typedef struct
{
	///  Window Type
    GSM_WIN_TYPE_T type;
	///  Window start time in Qb
    INT32 date;
	///  RF Channel number
    UINT16 arfcn;
	///  RF Channel Band
    GSM_RFBAND_T band;
	
	/// @brief Window's slots Gain. 
	///  - for Rx windows : Expected power at antenna in -dBm
	///  - for Tx windows : PCL power level */
    UINT8 gain[4];
	///  TimeSlot bitmap allocation
    UINT8 bitmap;
	///  TimeSlot bitmap modulation description
    UINT8 txModBitmap;
	///  Index of window's first TCU event
    UINT8 firstTcu;
	///  Index of window's last TCU event
    UINT8 lastTcu;
	///  Index of the window
    UINT8 winIdx;
	///  if TRUE, I and Q channels will be swapped for this window
    BOOL swapIQ;
} RFD_WIN_T;

//======================================================================
/// RFD RF Windows boundary times
/// This structure contains the windows setup and hold times that must
/// be respected for proper function.
/// PAL derives overall setup and holds from values returned by the
/// Xcver, the PA and the RF switch. 
//======================================================================
typedef struct
{
    /// Time offset (from on-air bit[0]) of the earliest event
    /// constituting the Rx Window
    INT16   rxSetup;

    /// Time offset (from on-air bit[last]) of the latestest event
    /// constituting the Rx Window
    INT16   rxHold;

    /// Time offset (from on-air bit[0]) of the earliest event
    /// constituting the Tx Window
    INT16   txSetup;

    /// Time offset (from on-air bit[last]) of the latestest event
    /// constituting the Tx Window
    INT16   txHold;
} RFD_WIN_BOUNDS_T;

#define RF_CTRL0            TCO(0)
#define RF_CTRL1            TCO(1)
#define RF_CTRL2            TCO(2)
#define RF_CTRL3            TCO(3)
#define RF_CTRL4            TCO(4)
#define RF_CTRL5            TCO(5)
#define RF_CTRL6            TCO(6)
#define RF_CTRL7            TCO(7)
#define RF_CTRL8            TCO(8)
#define RF_CTRL9            TCO(9)


#endif // _RF_DEFS_H
