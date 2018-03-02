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
//
/// @file mcdp_sdmmc.h
///
/// Structures privatly used by the SDMMC module implementation of MCD
/// 
//
////////////////////////////////////////////////////////////////////////////////



// =============================================================================
// MCD_CARD_STATE_T
// -----------------------------------------------------------------------------
/// The state of the card when receiving the command. If the command execution 
/// causes a state change, it will be visible to the host in the response to 
/// the next command. The four bits are interpreted as a binary coded number 
/// between 0 and 15.
// =============================================================================
typedef enum
{
    MCD_CARD_STATE_IDLE    = 0,
    MCD_CARD_STATE_READY   = 1,
    MCD_CARD_STATE_IDENT   = 2,
    MCD_CARD_STATE_STBY    = 3,
    MCD_CARD_STATE_TRAN    = 4,
    MCD_CARD_STATE_DATA    = 5,
    MCD_CARD_STATE_RCV     = 6,
    MCD_CARD_STATE_PRG     = 7,
    MCD_CARD_STATE_DIS     = 8
} MCD_CARD_STATE_T;


// =============================================================================
// MCD_CARD_STATUS_T
// -----------------------------------------------------------------------------
/// Card status as returned by R1 reponses (spec V2 pdf p.)
// =============================================================================
typedef union
{
    UINT32 reg;
    struct
    {
        UINT32                          :3;
        UINT32 akeSeqError              :1;
        UINT32                          :1;
        UINT32 appCmd                   :1;
        UINT32                          :2;
        UINT32 readyForData             :1;
        MCD_CARD_STATE_T currentState   :4;
        UINT32 eraseReset               :1;
        UINT32 cardEccDisabled          :1;
        UINT32 wpEraseSkip              :1;
        UINT32 csdOverwrite             :1;
        UINT32                          :2;
        UINT32 error                    :1;
        UINT32 ccError                  :1;
        UINT32 cardEccFailed            :1;
        UINT32 illegalCommand           :1;
        UINT32 comCrcError              :1;
        UINT32 lockUnlockFail           :1;
        UINT32 cardIsLocked             :1;
        UINT32 wpViolation              :1;
        UINT32 eraseParam               :1;
        UINT32 eraseSeqError            :1;
        UINT32 blockLenError            :1;
        UINT32 addressError             :1;
        UINT32 outOfRange               :1;
    } fields;
} MCD_CARD_STATUS_T;



