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

/**
    @file

    Defines related to the gsm specification 
*/
#ifndef __GSM_H__
#define __GSM_H__


// GSM burst length (number of symbols)
#define GSM_BURST_LEN		157
// Burst length definitions in Qs
#define GSM_SLOT_QS_DURATION 625
#define GSM_ABURST_ACTIVE_QS_DURATION (88 * 4)
#define GSM_NBURST_ACTIVE_QS_DURATION (148 * 4)

// Windows type definition
typedef enum {
    GSM_WIN_TYPE_MONIT = 1, ///< Type for Monitoring Window
    GSM_WIN_TYPE_FCCH = 2, ///< Type for FCCH Window
    GSM_WIN_TYPE_SCH = 3, ///< Type for SCH Window
    GSM_WIN_TYPE_NBURST = 4, ///< Type for Normal Burst Window
    GSM_WIN_TYPE_INTRF = 5, ///< Type for Interference Window
    GSM_WIN_TYPE_TXABURST = 6, ///< Type for Access Burst Tx Window
    GSM_WIN_TYPE_TXNBURST = 7, ///< Type for Normal Burst Tx Window
    GSM_WIN_TYPE_MONIT_EXT = 8 ///< Type for Monitor extra windows
}GSM_WIN_TYPE_T;

// Coding Schemes
typedef enum {
    GSM_CS1=0, 
    GSM_CS2=1, 
    GSM_CS3=2, 
    GSM_CS4=3,
    EGPRS_MCS1=4, 
    EGPRS_MCS2=5, 
    EGPRS_MCS3=6, 
    EGPRS_MCS4=7,
    EGPRS_MCS5=8,
    EGPRS_MCS6=9,
    EGPRS_MCS7=10,
    EGPRS_MCS8=11,
    EGPRS_MCS9=12
} GSM_CS_T;

// Band definition
typedef enum {
  GSM_BAND_GSM850 =  0,
  GSM_BAND_GSM900 =  1,
  GSM_BAND_DCS1800 = 2,
  GSM_BAND_PCS1900 = 3,
  GSM_BAND_QTY
} GSM_RFBAND_T;


typedef enum {
    PSK8          = 0x00000000,
	GMSK          = 0x00000001
} GSM_MODULATION_T;


//Fch mode: according to the one in spc
/*typedef enum
{
/// Original Delay2 Correlation Algo
    FCCH_SEARCH_ALGO_DELAY2                     = 0x00000000,
/// New Delay4 Correlation Algo
    FCCH_SEARCH_ALGO_DELAY4                     = 0x00000001,
/// New Delay4 Correlation Algo
    FCCH_SEARCH_ALGO_FILTER                     = 0x00000002
} SPC_FCCH_SEARCH_MODE_T;
*/
typedef enum
{
/// Original Delay2 Correlation Algo
   GSM_FCCH_SEARCH_ALGO_DELAY2                     = 0x00000000,
/// New Delay4 Correlation Algo
    GSM_FCCH_SEARCH_ALGO_DELAY4                     = 0x00000001,
/// New Delay4 Correlation Algo
    GSM_FCCH_SEARCH_ALGO_FILTER                     = 0x00000002
} GSM_FCCH_SEARCH_MODE_T;

//--------------------------------------------------------------------//
//                                                                    //
//                       ARFCN limits                                 //
//                                                                    //
//--------------------------------------------------------------------//
// GSM
#define GSM_ARFCN_GSM850_MIN         128
#define GSM_ARFCN_GSM850_MAX         251
// EGSM RX
#define GSM_ARFCN_EGSM_RX_MIN1       0
#define GSM_ARFCN_EGSM_RX_MAX1       124
#define GSM_ARFCN_EGSM_RX_MIN2       975
#define GSM_ARFCN_EGSM_RX_MAX2       1023
// EGSM TX
#define GSM_ARFCN_EGSM_TX_MIN1_1     0
#define GSM_ARFCN_EGSM_TX_MAX1_1     24
#define GSM_ARFCN_EGSM_TX_MIN1_2     50
#define GSM_ARFCN_EGSM_TX_MAX1_2     124
#define GSM_ARFCN_EGSM_TX_MIN2       25
#define GSM_ARFCN_EGSM_TX_MAX2       49
#define GSM_ARFCN_EGSM_TX_MIN3       975
#define GSM_ARFCN_EGSM_TX_MAX3       1023
// DCS
#define GSM_ARFCN_DCS_MIN            512
#define GSM_ARFCN_DCS_MAX            885
// PCS
#define GSM_ARFCN_PCS_MIN            512
#define GSM_ARFCN_PCS_MAX            810


#endif //__GSM_H__
