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
/* FILE NAME                                                             */
/*                                                                       */
/*      rf_names.h                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file defines the identifiers and the names of the RF chips  */
/*      that have a driver in phy_cfg: XCV, PA and SW.                   */
/*                                                                       */
/*************************************************************************/

#ifndef RF_NAMES_H
#define RF_NAMES_H

#include "calib_m.h"



/* -------------------------------------------------- */
/* ----- XCV identifiers. */

enum {
    DEFAULT_CALIB_XCV_OBSOLETE_0 = CALIB_XCV_MASK,
    DEFAULT_CALIB_XCV_OBSOLETE_1,
    DEFAULT_CALIB_XCV_OBSOLETE_2,
    DEFAULT_CALIB_XCV_OBSOLETE_3,
    DEFAULT_CALIB_XCV_OBSOLETE_4,
    DEFAULT_CALIB_XCV_OBSOLETE_5,
    DEFAULT_CALIB_XCV_RDA6220_GALLITE,
    DEFAULT_CALIB_XCV_RDA6220_GALLITE_CT1129,
    DEFAULT_CALIB_XCV_8808,
    DEFAULT_CALIB_XCV_8809,
    DEFAULT_CALIB_XCV_RDA6220E,
    DEFAULT_CALIB_XCV_8810,
    DEFAULT_CALIB_XCV_8809P,
    DEFAULT_CALIB_XCV_8850,
    DEFAULT_CALIB_XCV_QTY
};

/* -------------------------------------------------- */
/* ----- XCV names. */

#define DEFAULT_CALIB_XCV_NAMES_STR     "XCV Obsolete 0", \
                                        "XCV Obsolete 1", \
                                        "XCV Obsolete 2", \
                                        "XCV Obsolete 3", \
                                        "XCV Obsolete 4", \
                                        "XCV Obsolete 5", \
                                        "XCV RDA 6220 Gallite", \
                                        "XCV RDA 6220 Gallite CT1129", \
                                        "XCV 8808", \
                                        "XCV 8809", \
                                        "XCV RDA 6220E", \
                                        "XCV 8810", \
                                        "XCV 8809P", \
                                        "XCV 8850"

/* -------------------------------------------------- */
/* ----- PA identifiers. */

enum {
    DEFAULT_CALIB_PA_OBSOLETE_0 = CALIB_PA_MASK,
    DEFAULT_CALIB_PA_OBSOLETE_1,
    DEFAULT_CALIB_PA_OBSOLETE_2,
    DEFAULT_CALIB_PA_OBSOLETE_3,
    DEFAULT_CALIB_PA_OBSOLETE_4,
    DEFAULT_CALIB_PA_OBSOLETE_5,
    DEFAULT_CALIB_PA_OBSOLETE_6,
    DEFAULT_CALIB_PA_OBSOLETE_7,
    DEFAULT_CALIB_PA_RDA6625,
    DEFAULT_CALIB_PA_RDA6231,
    DEFAULT_CALIB_PA_8809P,
    DEFAULT_CALIB_PA_DUMMY,
    DEFAULT_CALIB_PA_QTY
};

/* -------------------------------------------------- */
/* ----- PA names. */

#define DEFAULT_CALIB_PA_NAMES_STR      "PA Obsolete 0", \
                                        "PA Obsolete 1", \
                                        "PA Obsolete 2", \
                                        "PA Obsolete 3", \
                                        "PA Obsolete 4", \
                                        "PA Obsolete 5", \
                                        "PA Obsolete 6", \
                                        "PA Obsolete 7", \
                                        "PA RDA 6625", \
                                        "PA RDA 6231", \
                                        "PA RDA 8809P", \
                                        "Dummy PA"

/* -------------------------------------------------- */
/* ----- SW identifiers. */

enum {
    DEFAULT_CALIB_SW_OBSOLETE_0 = CALIB_SW_MASK,
    DEFAULT_CALIB_SW_OBSOLETE_1,
    DEFAULT_CALIB_SW_OBSOLETE_2,
    DEFAULT_CALIB_SW_OBSOLETE_3,
    DEFAULT_CALIB_SW_OBSOLETE_4,
    DEFAULT_CALIB_SW_OBSOLETE_5,
    DEFAULT_CALIB_SW_OBSOLETE_6,
    DEFAULT_CALIB_SW_OBSOLETE_7,
    DEFAULT_CALIB_SW_OBSOLETE_8,
    DEFAULT_CALIB_SW_OBSOLETE_9,
    DEFAULT_CALIB_SW_RDA6625,
    DEFAULT_CALIB_SW_RDA6231,
    DEFAULT_CALIB_SW_8809P,
    DEFAULT_CALIB_SW_DUMMY,
    DEFAULT_CALIB_SW_QTY
};

/* -------------------------------------------------- */
/* ----- SW names. */

#define DEFAULT_CALIB_SW_NAMES_STR      "SW Obsolete 0", \
                                        "SW Obsolete 1", \
                                        "SW Obsolete 2", \
                                        "SW Obsolete 3", \
                                        "SW Obsolete 4", \
                                        "SW Obsolete 5", \
                                        "SW Obsolete 6", \
                                        "SW Obsolete 7", \
                                        "SW Obsolete 8", \
                                        "SW Obsolete 9", \
                                        "SW RDA 6625", \
                                        "SW RDA 6231", \
                                        "SW RDA 8809P", \
                                        "Dummy SW"


#endif /* RF_NAMES_H */
