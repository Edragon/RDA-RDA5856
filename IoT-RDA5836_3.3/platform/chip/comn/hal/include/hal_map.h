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
//                                                                              
//==============================================================================
//
/// @file
/// This file contains the portion of the module's memory map that will be accessible
/// through CoolWatcher (or any CoolXml tool). It is also used for the get version
/// mechanism.
//
//==============================================================================

#ifndef _HAL_MAP_H_
#define _HAL_MAP_H_


#include "hal_speech.h"

// =============================================================================
//  MACROS
// =============================================================================
#define HAL_VERSION_NUMBER                       (11)
#define HAL_SYS_RESOURCE_NUMBER                  (64)

// ============================================================================
// HAL_LPS_STATE_2_T
// -----------------------------------------------------------------------------
/// Record of LPS State
// =============================================================================
typedef enum
{
    HAL_LPS_IDLE_2                              = 0x00000000,
    HAL_LPS_CALIBRATED_2                        = 0x00000001,
    HAL_LPS_CALIB_RUNNING_2                     = 0x00000002
} HAL_LPS_STATE_2_T;


// ============================================================================
// PXTSLEVELS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef enum
{
    ENABLE_PXTS_TAG_RESERVED_0                  = 0x00000000,
    ENABLE_PXTS_TAG_BOOT                        = 0x00000001,
    ENABLE_PXTS_TAG_HAL                         = 0x00000002,
    ENABLE_PXTS_TAG_SX                          = 0x00000003,
    ENABLE_PXTS_TAG_PAL                         = 0x00000004,
    ENABLE_PXTS_TAG_EDRV                        = 0x00000005,
    ENABLE_PXTS_TAG_SVC                         = 0x00000006,
    ENABLE_PXTS_TAG_STACK                       = 0x00000007,
    ENABLE_PXTS_TAG_CSW                         = 0x00000008,
    ENABLE_PXTS_TAG_RESERVED_9                  = 0x00000009,
    ENABLE_PXTS_TAG_RESERVED_10                 = 0x0000000A,
    ENABLE_PXTS_TAG_RESERVED_11                 = 0x0000000B,
    ENABLE_PXTS_TAG_BB_IRQ                      = 0x0000000C,
    ENABLE_PXTS_TAG_SPAL                        = 0x0000000D,
    ENABLE_PXTS_TAG_SPP                         = 0x0000000E,
    ENABLE_PXTS_TAG_SPC                         = 0x0000000F
} PXTSLEVELS_T;

#define HAL_GDB_SWITCH_CTX_CMD_START             (0XF0)
#define HAL_GDB_SWITCH_CTX_CMD_OK                (0XE0)
#define HAL_GDB_SWITCH_CTX_CMD_ERR               (0XE1)
#define HAL_GDB_SWITCH_CTX_CMD_RESTORED          (0XE2)

// =============================================================================
//  TYPES
// =============================================================================

// ============================================================================
// HAL_MAP_GLOBALS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32              reserved;
} HAL_MAP_GLOBALS_T; //Size : 0x0



// ============================================================================
// HAL_SYS_FREQ_REGISTRY_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef UINT8 HAL_SYS_FREQ_REGISTRY_T[HAL_SYS_RESOURCE_NUMBER];


// ============================================================================
// HAL_SYS_ACCESS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================
typedef struct
{
    UINT32*                        halSysSystemFreq;             //0x00000000
    BOOL*                          halSysLpsLock;                //0x00000004
    HAL_SYS_FREQ_REGISTRY_T*       halSysFreqScalRegistry;       //0x00000008
} HAL_SYS_ACCESS_T; //Size : 0xC



// ============================================================================
// HAL_LPS_CONTEXT_2_T
// -----------------------------------------------------------------------------
/// Record of LPS content
// =============================================================================
typedef struct
{
    /// LPS State
    HAL_LPS_STATE_2_T              lpsState;                     //0x00000000
    /// Calibration invalidation request
    BOOL                           lpsInvalidateCalib;           //0x00000004
    /// Accuracy
    UINT8                          lpsAccuracy;                  //0x00000005
    /// Low Power Synchronizer Rate value
    UINT32                         lpsRate;                      //0x00000008
    /// Low Power Calibration frequency
    UINT32                         calibFreq;                    //0x0000000C
    /// Low Power Synchronizer previous Rate value
    UINT32                         lpsRateLast;                  //0x00000010
    /// Low Power Calibration frequency
    UINT32                         calibFreqLast;                //0x00000014
    /// Allow Frames to be skipped
    BOOL                           lpsSkipFrameAllowed;          //0x00000018
    /// Last event programmed in active frame
    UINT16                         lpsFrameLastEvt;              //0x0000001A
    /// Number of Frame skipped
    UINT16                         lpsSkipFrameNumber;           //0x0000001C
    /// Pu State machine configuration
    UINT32                         lpsPuCtrl;                    //0x00000020
    /// Number of frames neccessary to wake up
    UINT8                          lpsWakeUpFrameNbr;            //0x00000024
} HAL_LPS_CONTEXT_2_T; //Size : 0x28



// ============================================================================
// HAL_PROFILE_CONTROL_T
// -----------------------------------------------------------------------------
/// This is used only when the code is compiled with HAL_PROFILE_ON_MULTI in cool_profile
/// release.
// =============================================================================
typedef struct
{
    UINT32                         config;                       //0x00000000
    /// Start Address of the Ram Buffer used to record PXTS tags. It is allocted
    /// by CoolWatcher through a remote execution of malloc. The size is defined
    /// by the size field of this structure.
    UINT32                         startAddr;                    //0x00000004
    /// It must be of a sized multiple of 64 bits.
    UINT32                         size;                         //0x00000008
    /// The next PXTS tag/time stamp will be written at this address
    UINT32                         writePointer;                 //0x0000000C
    /// The next PXTS tag/time stamp will be read at this address by CoolWatcher.
    /// (Unused)
    UINT32                         readPointer;                  //0x00000010
    /// Remaining size in the buffer, in byte.
    INT32                          remainingSize;                //0x00000014
    /// Status of the profiling recording
    UINT32                         status;                       //0x00000018
    /// Function to execute remotely to allocate the buffer used by the profile in
    /// RAM mechanism.
    VOID*                          mallocRamBuffer;              //0x0000001C
    /// Function to execute remotely to free the buffer used by the profile in RAM
    /// mechanism.
    VOID*                          freeRamBuffer;                //0x00000020
} HAL_PROFILE_CONTROL_T; //Size : 0x24

//config
#define HAL_PROFILE_CONTROL_T_GLOBAL_ENABLE_RAM (1<<0)
#define HAL_PROFILE_CONTROL_T_GLOBAL_ENABLE_TRACE (1<<1)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_RESERVED_0 (1<<16)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_BOOT (1<<17)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_HAL (1<<18)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SX (1<<19)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_PAL (1<<20)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_EDRV (1<<21)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SVC (1<<22)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_STACK (1<<23)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_CSW (1<<24)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_RESERVED_9 (1<<25)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_RESERVED_10 (1<<26)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_RESERVED_11 (1<<27)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_BB_IRQ (1<<28)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SPAL (1<<29)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SPP (1<<30)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SPC (1<<31)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG(n) (((n)&0xFFFF)<<16)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_MASK (0xFFFF<<16)
#define HAL_PROFILE_CONTROL_T_ENABLE_PXTS_TAG_SHIFT (16)

//status
#define HAL_PROFILE_CONTROL_T_WRAPPED (1<<0)
#define HAL_PROFILE_CONTROL_T_OVERFLOWED (1<<1)



// ============================================================================
// HAL_GDB_SWITCH_CTX_T
// -----------------------------------------------------------------------------
/// To set the parameters and the result of the context switching.
// =============================================================================
typedef struct
{
    /// Whether to start switching, or the result after switching.
    UINT8                          cmd;                          //0x00000000
    /// TRUE if switching job's context.
    UINT8                          isJob;                        //0x00000001
    /// The task or job ID.
    UINT16                         id;                           //0x00000002
} HAL_GDB_SWITCH_CTX_T; //Size : 0x4

// ============================================================================
// HAL_ISPI_DEBUG_T
// -----------------------------------------------------------------------------
/// Read or write the ISPI in idle task simply.
// =============================================================================
typedef struct
{
    struct
    {
      UINT32 address      :16;
      UINT32 type         :8;
      UINT32 reserved     :4;
      UINT32 force_read   :1;
      UINT32 force_write  :1;
      UINT32 read         :1;
      UINT32 write        :1;
    }debug;                                                      //0x00000000
    UINT32                          data;                        //0x00000004
} HAL_ISPI_DEBUG_T; //Size : 0x8

#define SPI_REG_WRITE_FLAG (1<<31)
#define SPI_REG_READ_FLAG (1<<30)
#define SPI_REG_FORCE_WRITE_FLAG (1<<29)
#define SPI_REG_FORCE_READ_FLAG (1<<28)
#define SPI_REG_ACTION_MASK (0xff000000)
#define SPI_REG_TYPE_MASK   (0x00ff0000)
#define SPI_REG_ADDR_MASK   (0x0000ffff)
#define SPI_REG_DATA_MASK   (0x0000ffff)

//#define SPI_REG_TYPE_XCV   0
//#define SPI_REG_TYPE_ABB   1
//#define SPI_REG_TYPE_PMD   2
//#define SPI_REG_TYPE_FMD   3

typedef enum
{
  HAL_ISPI_REG_TYPE_XCV = 0x0,
  HAL_ISPI_REG_TYPE_ABB,
  HAL_ISPI_REG_TYPE_PMD,
  HAL_ISPI_REG_TYPE_FMD,

} HAL_ISPI_REG_T;

// ============================================================================
// HAL_MAP_ACCESS_T
// -----------------------------------------------------------------------------
/// Type used to define the accessible structures of the module.
// =============================================================================
typedef struct
{
    HAL_SYS_ACCESS_T               sys;                          //0x00000000
    HAL_LPS_CONTEXT_2_T*           lpsCtx;                       //0x0000000C
    UINT32*                        lpsForceNoSleepPtr;           //0x00000010
    HAL_SPEECH_FIFO_T*             halSpeechFifo;                //0x00000014
    /// This is used only when the code is compiled with HAL_PROFILE_ON_MULTI in
    /// cool_profile release.
    HAL_PROFILE_CONTROL_T          profileControl;               //0x00000018
    VOID*                          keypadPulseHandler;           //0x0000003C
    HAL_GDB_SWITCH_CTX_T*          gdbSwitchCtxPtr;              //0x00000040
    UINT32*                        gdbEnabledPtr;                //0x00000044
    UINT32*                        traceSwitchPtr;               //0x00000048
    UINT32*                        eventMask;                    //0x0000004C
    HAL_ISPI_DEBUG_T*              ispiDebug;                    //0x00000050
} HAL_MAP_ACCESS_T; //Size : 0x48

//config
#define HAL_MAP_ACCESS_T_GLOBAL_ENABLE_RAM (1<<0)
#define HAL_MAP_ACCESS_T_GLOBAL_ENABLE_TRACE (1<<1)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_RESERVED_0 (1<<16)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_BOOT (1<<17)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_HAL (1<<18)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SX (1<<19)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_PAL (1<<20)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_EDRV (1<<21)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SVC (1<<22)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_STACK (1<<23)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_CSW (1<<24)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_RESERVED_9 (1<<25)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_RESERVED_10 (1<<26)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_RESERVED_11 (1<<27)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_BB_IRQ (1<<28)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SPAL (1<<29)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SPP (1<<30)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SPC (1<<31)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG(n) (((n)&0xFFFF)<<16)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_MASK (0xFFFF<<16)
#define HAL_MAP_ACCESS_T_ENABLE_PXTS_TAG_SHIFT (16)

//status
#define HAL_MAP_ACCESS_T_WRAPPED    (1<<0)
#define HAL_MAP_ACCESS_T_OVERFLOWED (1<<1)







// =============================================================================
// g_halMapAccess
// -----------------------------------------------------------------------------
/// This global variable is the shared structure of HAL.
// =============================================================================
EXPORT PROTECTED HAL_MAP_ACCESS_T g_halMapAccess;


// =============================================================================
// hal_RegisterYourself
// -----------------------------------------------------------------------------
/// This function registers the module itself to HAL so that the version and
/// the map accessor are filled. Then, the CSW get version function and the
/// CoolWatcher get version command will work.
// =============================================================================
PUBLIC VOID hal_RegisterYourself(VOID);

    

#endif

