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
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#ifdef CT_ASM
#error "You are trying to use in an assembly code the normal H description of 'keypad'."
#endif



// =============================================================================
//  MACROS
// =============================================================================
/// Number of key in the keypad
#define KEY_NB                                   (64)
/// Number of key in the low data register
#define LOW_KEY_NB                               (32)
/// Number of key in the high data register
#define HIGH_KEY_NB                              (32)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// KEYPAD_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
#define REG_KEYPAD_BASE             0x01A05000

typedef volatile struct
{
    REG32                          KP_DATA_L;                    //0x00000000
    REG32                          KP_DATA_H;                    //0x00000004
    REG32                          KP_STATUS;                    //0x00000008
    REG32                          KP_CTRL;                      //0x0000000C
    REG32                          KP_IRQ_MASK;                  //0x00000010
    REG32                          KP_IRQ_CAUSE;                 //0x00000014
    REG32                          KP_IRQ_CLR;                   //0x00000018
} HWP_KEYPAD_T;

#define hwp_keypad                  ((HWP_KEYPAD_T*) KSEG1(REG_KEYPAD_BASE))


//KP_DATA_L
#define KEYPAD_KP_DATA_L(n)         (((n)&0xFFFFFFFF)<<0)
#define KEYPAD_KP_DATA_L_MASK       (0xFFFFFFFF<<0)
#define KEYPAD_KP_DATA_L_SHIFT      (0)

//KP_DATA_H
#define KEYPAD_KP_DATA_H(n)         (((n)&0xFFFFFFFF)<<0)
#define KEYPAD_KP_DATA_H_MASK       (0xFFFFFFFF<<0)
#define KEYPAD_KP_DATA_H_SHIFT      (0)

//KP_STATUS
#define KEYPAD_KEYIN_STATUS(n)      (((n)&0xFF)<<0)
#define KEYPAD_KEYIN_STATUS_MASK    (0xFF<<0)
#define KEYPAD_KEYIN_STATUS_SHIFT   (0)
#define KEYPAD_KP_ON                (1<<31)
#define KEYPAD_KP_ON_MASK           (1<<31)
#define KEYPAD_KP_ON_SHIFT          (31)

//KP_CTRL
#define KEYPAD_KP_EN                (1<<0)
#define KEYPAD_KP_DBN_TIME(n)       (((n)&0xFF)<<2)
#define KEYPAD_KP_ITV_TIME(n)       (((n)&0x3F)<<10)
#define KEYPAD_KP_IN_MASK(n)        (((n)&0xFF)<<16)
#define KEYPAD_KP_OUT_MASK(n)       (((n)&0xFF)<<24)

//KP_IRQ_MASK
#define KEYPAD_KP_EVT0_IRQ_MASK     (1<<0)
#define KEYPAD_KP_EVT1_IRQ_MASK     (1<<1)
#define KEYPAD_KP_ITV_IRQ_MASK      (1<<2)

//KP_IRQ_CAUSE
#define KEYPAD_KP_EVT0_IRQ_CAUSE    (1<<0)
#define KEYPAD_KP_EVT1_IRQ_CAUSE    (1<<1)
#define KEYPAD_KP_ITV_IRQ_CAUSE     (1<<2)
#define KEYPAD_KP_EVT0_IRQ_STATUS   (1<<16)
#define KEYPAD_KP_EVT1_IRQ_STATUS   (1<<17)
#define KEYPAD_KP_ITV_IRQ_STATUS    (1<<18)

//KP_IRQ_CLR
#define KEYPAD_KP_IRQ_CLR           (1<<0)





#endif

