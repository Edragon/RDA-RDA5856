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
//***********************************************************************//
/// @file rfd.h
/// @defgroup rfd_mainpage EDRV RFD Mainpage
/// @author Laurent Bastide
/// @par Purpose
/// This document describes the structures and functions used by PAL to
/// control RF components, as well as HAL functions usable by RF Drivers.
/// @par Module List
/// This document is composed of:
/// - @ref hal_rfdAPI "HAL limited API for RFD"
/// - @ref xcv_API "Transceiver Drivers API"
/// - @ref pa_API "Power Amplifier Drivers API"
/// - @ref sw_API "RF Switch Drivers API"
///@defgroup hal_rfdAPI EDRV RFD Usage of HAL
///@{
/// @par HAL usage
/// A reduced set of HAL functions are giving access to the various HardWare
/// peripherals (RFSpi, TCU, AFC) needed to implement RFD functionalities.
/// (see HAL documentation for details).\n
/// Depending on the synchronous nature of the RFD function to be implemented,
/// the usable HAL function set is different.
///
/// Functions of RFD API can be of two types : \n
/// @par Synchronous :
///      these functions are called at frame N-1 to setup actions that
///      are taking effect at frame N.
///      Therfore these functions should only use following functions
///      to control RF Spi, RF TCOs and Afc:
///      - \c hal_RfspiNewCmd
///      - \c hal_RfspiPushData
///      - \c hal_TcuSetEvent
///      - \c hal_AfcNxtFrameValue \n
///
/// @par  Asynchronous :
///      these functions are not called synchronously to the TDMA frame
///      structure. They should not rely on TCU timed operations, and therefore
///      they should only use following functions to control RFSpi, 
///      RF TCOs and Afc:
///      - \c hal_RfspiOpen
///      - \c hal_RfspiClose
///      - \c hal_RfspiSendCmd
///      - \c hal_AfcSetup
///      - \c hal_AfcForceValue
///      - \c hal_TcuSetTco
///      - \c hal_TcuClrTco
///      - \c hal_PulseRfCtrl
///@}
///
///@defgroup agc_scheme EDRV RFD AGC scheme
///@{
/// @par AGC Scheme
/// PAL Automatic Gain Control (AGC) Scheme:
///    To get a precise estimation of the Rf power over a large dynamic
///    span, PAL can use AGC on Monitoring and Interference windows.
///    PAL AGC scheme consists in incrementing digital gain of the Xcver
///    during Rx window.\n Every gain corresponds to a @b step that lasts for
///    X symbols. \n 3 steps are performed, at each step the received power
///    is checked against a threshold : \n
///      if the threshold is reached (or last step is reached) the chain
///      gain coresponding to the step is asked through rfd_XcvGetAgcGain,
///      and the actual Power is derived. \n
///    It's Xcver responsability to fill the Spi Gain fifo with a set
///    of gains allowing for a wide dynamic range of measurments. \n
///    Gains must be sorted from the weakest to the strongest.\n\n
/// If the Xcver doesn't support this AGC scheme (no gain change during Rx),
/// then a single specific gain value must be chosen.\n This value should allow
/// measurements of rather low fields (circa -103 dBm) as this is mandatory for
/// syncronization process.\n\n
/// In any case, the #RFD_XCV_PARAMS_T::lowRla parameter must be set accordingly
/// with the Xcver's performances.
///@}
///
///@defgroup xtal_aging EDRV RFD Crystal Aging detection scheme
///@{
/// @par Crystal Aging
/// During production calibration the crystal settings (rough frequency
/// tuning) are optimized to get the most accurate reference frequency
/// for a mid-range setting of the AFC (fine frequency tuning). \n
/// But characteristics of crystals are known to variate substentially
/// over time. 
/// This can become problematic for 2 reasons:
/// - at phone init (AFC midrange) the frequency offset becomes large,
///   this implies that a large correction will be needed to get back to
///   the reference frequency. On such large corrections, errors can
///   be large too (due to AFC gain inherent error, and AFC non linearity).
///   If this residual error is too big, it can prevent the MS to synchronize
///   the network.
/// - this reduces AFC tuning range as the "0" frequency offset is not at
///   the AFC's midrange value anymore.
/// 
/// @par Aging Detection & Correction Scheme
/// - During normal operation PAL is monitoring the average AFC setting
/// through calls to #rfd_XcvGetAfcOffset. PAL is filtering those results
/// according to performance/robustness criterions, and will store the
/// resulting average value in non-volatile memory.
/// - At next boot PAL will call #rfd_XcvOpen with the AfcOffset as parameter.
/// The Xcver's driver should then modify the crystal's default settings to
/// compensate for the given offset (if possible).
///@}
//***********************************************************************//

#ifndef _RFD_H
#define _RFD_H

#warning "Are you sure you wanted to include rfd.h and not a rfd_module.h ?"

#include "rfd_defs.h"
#include "rfd_xcv.h"
#include "rfd_pa.h"
#include "rfd_sw.h"

#endif //  RFD_H 

