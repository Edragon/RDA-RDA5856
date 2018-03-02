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
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_sys.h
/// That file describes the SYSTEM driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_SYS_H_
#define _HAL_SYS_H_

#include "cs_types.h"



/// @defgroup system HAL Sytem Driver
/// description
/// ...
/// @{



// =============================================================================
//  MACROS
// =============================================================================

// Chip product IDs

#define HAL_SYS_CHIP_PROD_ID_8808       0x8808

#define HAL_SYS_CHIP_PROD_ID_8808S      0x880a

#define HAL_SYS_CHIP_PROD_ID_8809       0x8809

#define HAL_SYS_CHIP_PROD_ID_8810       0x8810


#ifdef BIG_ENDIAN

#define HAL_ENDIAN_LITTLE_32(X) (((((X) >> 24) & 0xFF) <<  0)           \
                                |((((X) >> 16) & 0xFF) <<  8)           \
                                |((((X) >>  8) & 0xFF) << 16)           \
                                |((((X) >>  0) & 0xFF) << 24))

#define HAL_ENDIAN_LITTLE_24(X) (((((X) >> 16) & 0xFF) <<  0)           \
                                |((((X) >>  8) & 0xFF) <<  8)           \
                                |((((X) >>  0) & 0xFF) << 16))

#define HAL_ENDIAN_LITTLE_16(X) (((((X) >>  8) & 0xFF) <<  0)           \
                                |((((X) >>  0) & 0xFF) <<  8))

#define HAL_ENDIAN_BIG_32(X)    (X)

#define HAL_ENDIAN_BIG_24(X)    (X)

#define HAL_ENDIAN_BIG_16(X)    (X)

#else

#define HAL_ENDIAN_BIG_32(X)    (((((X) >> 24) & 0xFF) <<  0)           \
                                |((((X) >> 16) & 0xFF) <<  8)           \
                                |((((X) >>  8) & 0xFF) << 16)           \
                                |((((X) >>  0) & 0xFF) << 24))

#define HAL_ENDIAN_BIG_24(X)    (((((X) >> 16) & 0xFF) <<  0)           \
                                |((((X) >>  8) & 0xFF) <<  8)           \
                                |((((X) >>  0) & 0xFF) << 16))

#define HAL_ENDIAN_BIG_16(X)    (((((X) >>  8) & 0xFF) <<  0)           \
                                |((((X) >>  0) & 0xFF) <<  8))

#define HAL_ENDIAN_LITTLE_32(X) (X)

#define HAL_ENDIAN_LITTLE_24(X) (X)

#define HAL_ENDIAN_LITTLE_16(X) (X)

#endif /* BIG_ENDIAN */

// =============================================================================
// HAL_SYS_CRITICAL_SECTION_ADDRESS
// -----------------------------------------------------------------------------
/// Address of the interrupt enable semaphore, used to control the critical
/// section.
// =============================================================================
#define HAL_SYS_CRITICAL_SECTION_ADDRESS  CHIP_SYS_CRITICAL_SECTION_REG


// =============================================================================
// HAL_SYS_GET_CACHED_ADDR
// -----------------------------------------------------------------------------
/// That macro is used to get the cached version of a given address
// =============================================================================
#define HAL_SYS_GET_CACHED_ADDR(addr)  (((UINT32)(addr))&0xdfffffff)


// =============================================================================
// HAL_SYS_GET_UNCACHED_ADDR
// -----------------------------------------------------------------------------
/// That macro is used to get the uncached version of a given address
// =============================================================================
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5857)
#define HAL_SYS_GET_UNCACHED_ADDR(addr) (((UINT32)(addr)))
#else
#define HAL_SYS_GET_UNCACHED_ADDR(addr) (((UINT32)(addr))|0x20000000)
#endif
#ifdef __mips16
// =============================================================================
// HAL_SYS_GET_RA
// -----------------------------------------------------------------------------
/// That macro puts in store the current value of the RA register. It should 
/// be called at the beginning of the function (before any other function call)
/// to ensure the correctness of the result.
///
/// This is the macro for mips16 code. It also exists for mips32 code, in
/// which case its value would be:
///
/// <code>
/// \#define HAL_SYS_GET_RA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))
/// </code>
///
/// @param store The return address of the current function
// =============================================================================
#define HAL_SYS_GET_RA(store) asm volatile("move $2, $31\n\tsw $2, 0(%0)"::"d"((store)):"$2")
#else
// =============================================================================
// HAL_SYS_GET_RA
// -----------------------------------------------------------------------------
/// That macro puts in store the current value of the RA register. It should 
/// be called at the beginning of the function (before any other function call)
/// to ensure the correctness of the result.
///
/// This is the macro for mips32 code. It also exists for mips16 code, in
/// which case its value would be:
///
/// <code>
/// \#define HAL_SYS_GET_RA(store) asm volatile("move $2, $31\n\tsw $2, 0(%0)"::"r"((store)):"$2")
/// </code>
///
/// @param store The return address of the current function
// =============================================================================
#define HAL_SYS_GET_RA(store) asm volatile("sw $31, 0(%0)"::"r"((store)))
#endif

// =============================================================================
// HAL_SYS_GET_K1
// -----------------------------------------------------------------------------
/// That macro puts in store the current value of the K1 register.
///
/// @param store The return value of the current function
// =============================================================================
#define HAL_SYS_GET_K1(store) asm volatile("move %0, $27":"=r"((store)))


#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
/* __HAL_SYS_SWITCH_PLL_DYNAMIC__
 Disable pll when it's not needed (sys_clk & spi_clk & voc_clk < 78MHz)
*/
#define __HAL_SYS_SWITCH_PLL_DYNAMIC__


#if defined(__HAL_SYS_SWITCH_PLL_DYNAMIC__)

/*
 Use aux pll when sys pll is needed
*/
#define __HAL_SYS_USE_AUX_PLL_ALWAYS__
#endif /* __HAL_SYS_SWITCH_PLL_DYNAMIC__ */

#endif
#endif

// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_SYS_FREQ_USER_ID_T
// -----------------------------------------------------------------------------
/// Those are the different user recognized at the system scale.
/// @todo fill
// =============================================================================
typedef enum
{
    // Resource ID to be used by the platform 
    HAL_SYS_FREQ_PLATFORM_FIRST_USER = 22,
    HAL_SYS_FREQ_PAL = HAL_SYS_FREQ_PLATFORM_FIRST_USER,
    HAL_SYS_FREQ_BCPU,
    HAL_SYS_FREQ_XCV,
    HAL_SYS_FREQ_CAMS,
    HAL_SYS_FREQ_VOIS,
    HAL_SYS_FREQ_NONE_0,
    HAL_SYS_FREQ_NONE_1,
    HAL_SYS_FREQ_AVRS,
    HAL_SYS_FREQ_PLATFORM_LAST_USER = HAL_SYS_FREQ_AVRS,

    // Resource ID to be used by the application (MMI)
    HAL_SYS_FREQ_APP_FIRST_USER = 32,
    HAL_SYS_FREQ_APP_USER_0 = HAL_SYS_FREQ_APP_FIRST_USER,
    HAL_SYS_FREQ_APP_USER_1,
    HAL_SYS_FREQ_APP_USER_2,
    HAL_SYS_FREQ_APP_USER_3,
    HAL_SYS_FREQ_APP_LAST_USER = HAL_SYS_FREQ_APP_USER_3,

    // Total number of resource IDs
    HAL_SYS_FREQ_USER_QTY
} HAL_SYS_FREQ_USER_ID_T;



// =============================================================================
// HAL_SYS_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the system clock.
/// This type is used to define the minimum system frequency required 
/// by a user selected among #HAL_SYS_FREQ_USER_ID_T.
/// Some frequencies might not be supported by some chips.
// =============================================================================
typedef enum
{
#if (!CHIP_HAS_ASYNC_TCU)
    HAL_SYS_FREQ_32K     = 32768,
    HAL_SYS_FREQ_13M     = 13000000,
    HAL_SYS_FREQ_26M     = 26000000,
    HAL_SYS_FREQ_39M     = 39000000,
    HAL_SYS_FREQ_52M     = 52000000,
    HAL_SYS_FREQ_78M     = 78000000,
    HAL_SYS_FREQ_104M    = 104000000,
    HAL_SYS_FREQ_156M    = 156000000,
    HAL_SYS_FREQ_208M    = 208000000,


    
    HAL_SYS_FREQ_UNKNOWN = 0,
#else // CHIP_HAS_ASYNC_TCU
    HAL_SYS_FREQ_32K     = 32768,
    HAL_SYS_FREQ_13M     = 13000000,
    HAL_SYS_FREQ_26M     = 26000000,
    HAL_SYS_FREQ_39M     = 39000000,
    HAL_SYS_FREQ_52M     = 52000000,
    HAL_SYS_FREQ_78M     = 78000000,
    HAL_SYS_FREQ_89M     = 89142857, //624M/7
    HAL_SYS_FREQ_104M    = 104000000,
    HAL_SYS_FREQ_113M    = 113454545, //624M/5.5
    HAL_SYS_FREQ_125M    = 124800000,
    HAL_SYS_FREQ_139M    = 138666667, //624M/4.5
    HAL_SYS_FREQ_156M    = 156000000,
    HAL_SYS_FREQ_178M    = 178285714, //624M/3.5
    HAL_SYS_FREQ_208M    = 208000000,
    HAL_SYS_FREQ_250M    = 249600000, //624M/2.5
    HAL_SYS_FREQ_312M    = 312000000,

    
    // 28M * N
    HAL_SYS_FREQ_70M      = 70000000,
    HAL_SYS_FREQ_77M      = 77000000,
    HAL_SYS_FREQ_84M      = 84000000,
    HAL_SYS_FREQ_98M      = 98000000,
    HAL_SYS_FREQ_112M     = 112000000,
    HAL_SYS_FREQ_140M     = 140000000,
    HAL_SYS_FREQ_168M     = 168000000,

    // 48M * N
    HAL_SYS_FREQ_72M      = 72000000,
    HAL_SYS_FREQ_96M      = 96000000,
    HAL_SYS_FREQ_99M      = 99000000,
    HAL_SYS_FREQ_144M     = 144000000,


    HAL_SYS_FREQ_UNKNOWN = 0,
#endif // CHIP_HAS_ASYNC_TCU
} HAL_SYS_FREQ_T;


// =============================================================================
// HAL_SYS_BB_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the bb clock.
/// Some frequencies might not be supported by some chips.
// =============================================================================
typedef enum
{
    HAL_SYS_BB_FREQ_26M     = 26000000,
    HAL_SYS_BB_FREQ_39M     = 39000000,
    HAL_SYS_BB_FREQ_52M     = 52000000,
    HAL_SYS_BB_FREQ_78M     = 78000000,
    HAL_SYS_BB_FREQ_104M    = 104000000,
    HAL_SYS_BB_FREQ_156M    = 156000000,
    HAL_SYS_BB_FREQ_208M    = 208000000,
    HAL_SYS_BB_FREQ_312M    = 312000000,
} HAL_SYS_BB_FREQ_T;



// =============================================================================
// HAL_SYS_FREQ_CHANGE_HANDLER_T
// -----------------------------------------------------------------------------
/// Type for Handler called when the system frequency changes
/// as registered by function hal_SysRequestFreq()
// =============================================================================
typedef VOID (*HAL_SYS_FREQ_CHANGE_HANDLER_T)(HAL_SYS_FREQ_T);



// =============================================================================
// HAL_SYS_MEM_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the memory clock.
/// Some frequencies might not be supported by some chips.
// =============================================================================
typedef enum
{
    HAL_SYS_MEM_FREQ_13M     = 13000000,
    HAL_SYS_MEM_FREQ_26M     = 26000000,
    HAL_SYS_MEM_FREQ_39M     = 39000000,
    HAL_SYS_MEM_FREQ_52M     = 52000000,
    HAL_SYS_MEM_FREQ_78M     = 78000000,
    HAL_SYS_MEM_FREQ_89M     = 89142857, //624M/7
    HAL_SYS_MEM_FREQ_104M    = 104000000,
    HAL_SYS_MEM_FREQ_113M    = 113454545, //624M/5.5
    HAL_SYS_MEM_FREQ_125M    = 124800000,
    HAL_SYS_MEM_FREQ_139M    = 138666667, //624M/4.5
    HAL_SYS_MEM_FREQ_156M    = 156000000,
    HAL_SYS_MEM_FREQ_178M    = 178285714, //624M/3.5
    HAL_SYS_MEM_FREQ_208M    = 208000000,
    HAL_SYS_MEM_FREQ_250M    = 249600000, //624M/2.5
    HAL_SYS_MEM_FREQ_312M    = 312000000,
} HAL_SYS_MEM_FREQ_T;


// =============================================================================
// HAL_SYS_VOC_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the VoC clock.
/// Some frequencies might not be supported by some chips.
// =============================================================================
typedef enum
{
    HAL_SYS_VOC_FREQ_13M     = 13000000,
    HAL_SYS_VOC_FREQ_26M     = 26000000,
    HAL_SYS_VOC_FREQ_39M     = 39000000,
    HAL_SYS_VOC_FREQ_52M     = 52000000,
    HAL_SYS_VOC_FREQ_78M     = 78000000,
    HAL_SYS_VOC_FREQ_89M     = 89142857, //624M/7
    HAL_SYS_VOC_FREQ_104M    = 104000000,
    HAL_SYS_VOC_FREQ_113M    = 113454545, //624M/5.5
    HAL_SYS_VOC_FREQ_125M    = 124800000,
    HAL_SYS_VOC_FREQ_139M    = 138666667, //624M/4.5
    HAL_SYS_VOC_FREQ_156M    = 156000000,
    HAL_SYS_VOC_FREQ_178M    = 178285714, //624M/3.5
    HAL_SYS_VOC_FREQ_208M    = 208000000,
    HAL_SYS_VOC_FREQ_250M    = 249600000, //624M/2.5
    HAL_SYS_VOC_FREQ_312M    = 312000000,
} HAL_SYS_VOC_FREQ_T;


typedef enum
{
    HAL_SYS_ZSP_FREQ_13M     = 13000000,
    HAL_SYS_ZSP_FREQ_26M     = 26000000,
    HAL_SYS_ZSP_FREQ_39M     = 39000000,
    HAL_SYS_ZSP_FREQ_52M     = 52000000,
    HAL_SYS_ZSP_FREQ_78M     = 78000000,
    HAL_SYS_ZSP_FREQ_89M     = 89142857, //624M/7
    HAL_SYS_ZSP_FREQ_104M    = 104000000,
    HAL_SYS_ZSP_FREQ_113M    = 113454545, //624M/5.5
    HAL_SYS_ZSP_FREQ_125M    = 124800000,
    HAL_SYS_ZSP_FREQ_139M    = 138666667, //624M/4.5
    HAL_SYS_ZSP_FREQ_156M    = 156000000,
    HAL_SYS_ZSP_FREQ_178M    = 178285714, //624M/3.5
    HAL_SYS_ZSP_FREQ_208M    = 208000000,
    HAL_SYS_ZSP_FREQ_250M    = 249600000, //624M/2.5
    HAL_SYS_ZSP_FREQ_312M    = 312000000,
} HAL_SYS_ZSP_FREQ_T;


// =============================================================================
// HAL_SYS_SPIFLASH_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the SPI FLASH clock.
/// Some frequencies might not be supported by some chips.
// =============================================================================
typedef enum
{
    HAL_SYS_SPIFLASH_FREQ_13M     = 13000000,
    HAL_SYS_SPIFLASH_FREQ_26M     = 26000000,
    HAL_SYS_SPIFLASH_FREQ_39M     = 39000000,
    HAL_SYS_SPIFLASH_FREQ_52M     = 52000000,
    HAL_SYS_SPIFLASH_FREQ_78M     = 78000000,
    HAL_SYS_SPIFLASH_FREQ_89M     = 89142857, //624M/7
    HAL_SYS_SPIFLASH_FREQ_104M    = 104000000,
    HAL_SYS_SPIFLASH_FREQ_113M    = 113454545, //624M/5.5
    HAL_SYS_SPIFLASH_FREQ_125M    = 124800000,
    HAL_SYS_SPIFLASH_FREQ_139M    = 138666667, //624M/4.5
    HAL_SYS_SPIFLASH_FREQ_156M    = 156000000,
    HAL_SYS_SPIFLASH_FREQ_178M    = 178285714, //624M/3.5
    HAL_SYS_SPIFLASH_FREQ_208M    = 208000000,
    HAL_SYS_SPIFLASH_FREQ_250M    = 249600000, //624M/2.5
    HAL_SYS_SPIFLASH_FREQ_312M    = 312000000,
} HAL_SYS_SPIFLASH_FREQ_T;


// =============================================================================
// HAL_SYS_CLOCK_OUT_ID_T
// -----------------------------------------------------------------------------
/// That type defines all the possible user Id for the clock Out Pin sharing.
// =============================================================================
typedef enum
{
    /// returned when no more IDs are availlable
    HAL_SYS_CLOCK_OUT_ID_1 = 0,
    HAL_SYS_CLOCK_OUT_ID_2,
    HAL_SYS_CLOCK_OUT_ID_3,
    HAL_SYS_CLOCK_OUT_ID_4,

    HAL_SYS_CLOCK_OUT_ID_QTY,

    HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE = -1
} HAL_SYS_CLOCK_OUT_ID_T;


// =============================================================================
// HAL_SYS_CLOCK_OUT_FREQ_T
// -----------------------------------------------------------------------------
/// That type defines all the possible frequencies for the clock Out Pin.
// =============================================================================
typedef enum
{
    HAL_SYS_CLOCK_OUT_FREQ_32K  = 1,
    HAL_SYS_CLOCK_OUT_FREQ_26M
} HAL_SYS_CLOCK_OUT_FREQ_T;


// =============================================================================
// HAL_SYS_RESET_CAUSE_T
// -----------------------------------------------------------------------------
/// Describes the cause of a system reset. This type is returned by the function
/// #hal_SysGetResetCause.
// =============================================================================
typedef enum
{
    /// Normal cause, ie power up
    HAL_SYS_RESET_CAUSE_NORMAL, 

    /// The reset was caused by a watchdog
    HAL_SYS_RESET_CAUSE_WATCHDOG,

    /// The reset was caused by a soft restart, triggered by the function
    /// #hal_SysRestart.
    HAL_SYS_RESET_CAUSE_RESTART,

    /// The reset was initiated from the Host Interface.
    HAL_SYS_RESET_CAUSE_HOST_DEBUG,

    /// The reset was caused by alarm, from the calendar.
    HAL_SYS_RESET_CAUSE_ALARM,

    HAL_SYS_RESET_CAUSE_QTY

} HAL_SYS_RESET_CAUSE_T;


// =============================================================================
// HAL_SYS_CHIP_ID_T
// -----------------------------------------------------------------------------
/// Describes the cause of a system reset. This type is returned by the function
/// #hal_SysGetResetCause.
// =============================================================================
typedef enum
{
    /// Chip product ID
    HAL_SYS_CHIP_PROD_ID,

    /// Chip bond ID
    HAL_SYS_CHIP_BOND_ID,

    /// Chip metal ID
    HAL_SYS_CHIP_METAL_ID,

    /// Chip ID whole
    HAL_SYS_CHIP_ID_WHOLE,

} HAL_SYS_CHIP_ID_T;


// =============================================================================
// HAL_SYS_CHIP_METAL_ID_T
// -----------------------------------------------------------------------------
/// Those are the different metal IDs.
/// @todo fill
// =============================================================================
typedef enum
{
	HAL_SYS_CHIP_METAL_ID_U00 = 0x00,
	HAL_SYS_CHIP_METAL_ID_U01,
	HAL_SYS_CHIP_METAL_ID_U02,
	HAL_SYS_CHIP_METAL_ID_U03,
	HAL_SYS_CHIP_METAL_ID_U04,
	HAL_SYS_CHIP_METAL_ID_U05,
	HAL_SYS_CHIP_METAL_ID_U06,
	HAL_SYS_CHIP_METAL_ID_U07,
	HAL_SYS_CHIP_METAL_ID_U08,
	HAL_SYS_CHIP_METAL_ID_U09,
	HAL_SYS_CHIP_METAL_ID_U0a,
	HAL_SYS_CHIP_METAL_ID_U0b,
	HAL_SYS_CHIP_METAL_ID_U0c,
	HAL_SYS_CHIP_METAL_ID_U0d,
	HAL_SYS_CHIP_METAL_ID_U0e,
	HAL_SYS_CHIP_METAL_ID_U0f,
	HAL_SYS_CHIP_METAL_ID_U10,
}HAL_SYS_CHIP_METAL_ID_T;

#define HAL_SYS_CHIP_METAL_ID_8809E     0x02

// =============================================================================
//  FUNCTIONS
// =============================================================================


// ============================================================================
// hal_SysSetupPLL
// ----------------------------------------------------------------------------
/// Setup PLL at early system boot time.
// ============================================================================
PUBLIC VOID hal_SysSetupPLL(VOID);

PUBLIC VOID hal_SysSetupNonePLL(VOID);

// =============================================================================
// hal_SysStartBcpu
// -----------------------------------------------------------------------------
/// Start the BCPU with a stack to at the given address.
/// @param bcpuMain Main function to run on the BCPU.
/// @param stackStartAddr Stack top for the program running on the BCPU
// =============================================================================
PUBLIC VOID hal_SysStartBcpu(VOID* bcpuMain, VOID* stackStartAddr);


// =============================================================================
// hal_SysStopBcpu
// -----------------------------------------------------------------------------
/// Stop the BCPU and leave it in reset
/// Use it for debug only as it is prefereable to 
/// not send any commands to it
// =============================================================================
PUBLIC VOID hal_SysStopBcpu(VOID);


// =============================================================================
// hal_SysSetRequestFreq
// -----------------------------------------------------------------------------
/// When a given users requires a minimum frequency to be able to execute a
/// task, this function must be called.
/// @param userId Identifier of the calling user.
/// @param minFreq Minimum required frequency.
/// @param handler User function registered for this userId 
/// The function is called when the frequency is switched by any following call
/// to hal_SysSetRequestFreq() including the current one.
/// If \c NULL, no function is registered.
// =============================================================================
PUBLIC VOID hal_SysRequestFreq(HAL_SYS_FREQ_USER_ID_T userId, HAL_SYS_FREQ_T minFreq,
                                HAL_SYS_FREQ_CHANGE_HANDLER_T handler);



// =============================================================================
// hal_SysGetRequestFreq
// -----------------------------------------------------------------------------
/// Get the minimum frequency required by a given user
/// @param userId Identifier of the calling user.
/// @return The minimum required frequency for this user.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetRequestFreq(HAL_SYS_FREQ_USER_ID_T userId);



// =============================================================================
// hal_SysGetFreq
// -----------------------------------------------------------------------------
/// Get the current system clock frequency.
/// @return The current system clock frequency.
// =============================================================================
PUBLIC HAL_SYS_FREQ_T hal_SysGetFreq(VOID);



// =============================================================================
// hal_SysSetBBClock
// -----------------------------------------------------------------------------
/// Set the BB clock.
/// @param newClock New BB clock.
// =============================================================================
PUBLIC VOID hal_SysSetBBClock(HAL_SYS_BB_FREQ_T newClock);



// =============================================================================
// hal_SysSetVocClock
// -----------------------------------------------------------------------------
/// Set the VOC clock.
/// @param newClock New VOC clock.
/// @return Old VOC clock.
// =============================================================================
PUBLIC HAL_SYS_VOC_FREQ_T hal_SysSetVocClock(HAL_SYS_VOC_FREQ_T newClock);



// =============================================================================
// hal_SysGetShutdownFlag
// -----------------------------------------------------------------------------
/// Get the shutdown flag, and then reset the flag.
// =============================================================================
PUBLIC BOOL hal_SysGetShutdownFlag(VOID);



// =============================================================================
// hal_SysShutdown
// -----------------------------------------------------------------------------
/// A call to this function shuts the system down.
// =============================================================================
PUBLIC VOID hal_SysShutdown(VOID);



// =============================================================================
// hal_SysSoftReset
// -----------------------------------------------------------------------------
/// Does a soft reset on the chip.
/// Generally, you should NOT use this function, use hal_SysRestart() instead.
// =============================================================================
PUBLIC VOID hal_SysSoftReset(VOID);



// =============================================================================
// hal_SysRestart
// -----------------------------------------------------------------------------
/// A call to this function reboots the system.
/// First, this function does a power cycle on USB
// =============================================================================
PUBLIC VOID hal_SysRestart(VOID);


// =============================================================================
// hal_SysSetBootMode
// -----------------------------------------------------------------------------
/// Set Boot Mode.
// =============================================================================
PUBLIC VOID hal_SysSetBootMode(UINT8 mode);

// =============================================================================
// hal_SysGetResetCause
// -----------------------------------------------------------------------------
/// Get the cause of the last reset.
/// This function only returns the reset cause. It does not proceed to any test
/// to now if the boot was due to a press on the Power On button or because
/// the charger was plugged. Those tests are to be done by the user code, 
/// when this function returns HAL_SYS_RESET_CAUSE_ALARM.
/// @return The last reset cause
// =============================================================================
PUBLIC HAL_SYS_RESET_CAUSE_T hal_SysGetResetCause(VOID);



// =============================================================================
// hal_SysSleep 
// -----------------------------------------------------------------------------
/// Check if the system can sleep and, in that case, make it sleep.
///
/// This function is to be called by pal_Sleep, normally in
/// the idle task. It switches the system on the 32khz clock if no user requires
/// a higher clock and if it is ready to do it, ie: the LPS is operational
// =============================================================================
PUBLIC VOID hal_SysSleep(VOID);



// =============================================================================
// hal_SysResetOut
// -----------------------------------------------------------------------------
/// A call to this function resets an external device through the reset-out
/// line, if the parameter is \c TRUE.
/// @param resetOut If \c TRUE, rises the reset-out line.
///                 If \c FALSE, pulls it down.
// =============================================================================
PUBLIC VOID hal_SysResetOut(BOOL resetOut);



// =============================================================================
// hal_SysInvalidateCache
// -----------------------------------------------------------------------------
/// This function invalidates in the cache the line corresponding to the given 
/// memory area, referred by a pointer and a size.
/// It must be called with the destination buffer after a DMA copy.
/// @param buffer Pointer to the first address whose cache line must be 
/// invalidated
/// @param size Size of the buffer.
// =============================================================================
PUBLIC VOID hal_SysInvalidateCache(VOID* buffer, UINT32 size);



// =============================================================================
// hal_SysGetChipId
// -----------------------------------------------------------------------------
/// That function gives the chip ID the software is running on.
/// @param part Which part of the chip ID
/// @return The requested part of the chip ID.
// =============================================================================
PUBLIC UINT32 hal_SysGetChipId(HAL_SYS_CHIP_ID_T part);



// =============================================================================
// g_halSysCsAddress
// -----------------------------------------------------------------------------
/// This global variable stores the address of the chip critical section.
///
/// *ATTENTION* It is forbidden to use  global variables, but here we e
/// exceptionally need to have one for critical performance reasons.
/// This is not an exchange variable, it is forbidden to use it directly.
///
/// hal_SysEnterCriticalSection and hal_SysExitCriticalSection are inline 
/// built using that variable and must be used to access the critical 
/// section.
// =============================================================================
EXPORT VOLATILE UINT32* g_halSysCsAddress;



// =============================================================================
// hal_SysEnterCriticalSection
// -----------------------------------------------------------------------------
/// This function enters in a critical section (relative to the hardware 
/// interrupts) and return a status that should be given to the 
/// #hal_SysExitCriticalSection function.
/// 
/// @return  The \c status that should be given to the #hal_SysExitCriticalSection 
/// function.
// =============================================================================
UINT32 hal_SysEnterCriticalSection(VOID);
//{
    //UINT32  myLocalVar = *g_halSysCsAddress;
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    //asm volatile ("");
    //return myLocalVar;
//}




// =============================================================================
// hal_SysExitCriticalSection
// -----------------------------------------------------------------------------
/// This function leaves the critical section. The <CODE>status</CODE> 
/// parameter is the return value of the #hal_SysEnterCriticalSection call
/// that opened the critical section.
/// 
/// @param status The value returned by #hal_SysEnterCriticalSection.
// =============================================================================
VOID hal_SysExitCriticalSection(UINT32 status);
//{
    // add this empty assembly line to avoid the compiler to re-order
    // the code around the critical section call
    //asm volatile ("");
  // *g_halSysCsAddress = status;
//}

UINT32 hal_SysEnterSC(VOID); 
VOID hal_SysExitSC(UINT32 status);


// =============================================================================
// hal_SysGetSp
// -----------------------------------------------------------------------------
/// This function returns the current value of the stack register
/// @return The current value of the stack pointer.
// =============================================================================
PUBLIC UINT32 hal_SysGetSp(VOID);



// =============================================================================
// hal_SysClkOutOpen
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function requests a resource of #HAL_SYS_FREQ_26M or release it 
/// to #HAL_SYS_FREQ_32K depending on the \p freq parameter value.
///
/// @param freq the required frequency
/// @return the user id to be used for #hal_SysClkOutClose
///         or #HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE when the clock is already
///         in use at a different frequency.
// =============================================================================
PUBLIC HAL_SYS_CLOCK_OUT_ID_T hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_T freq);



// =============================================================================
// hal_SysClkOutClose
// -----------------------------------------------------------------------------
/// Manage the output of the clock-out, to an external device
///
/// This function release a resource to #HAL_SYS_FREQ_32K.
///
/// @param id the user id returned by #hal_SysClkOutOpen
// =============================================================================
PUBLIC VOID hal_SysClkOutClose(HAL_SYS_CLOCK_OUT_ID_T id);



// ==========================================================================
// hal_SysAuxClkOut
// --------------------------------------------------------------------------
/// Manage the output of the 26M AUX Clock, to an external device
/// @param enable If \c TRUE, enable AuxClk output.
///               If \c FALSE, disable AuxClk output.
// ==========================================================================
PUBLIC VOID hal_SysAuxClkOut(BOOL enable);



// ==========================================================================
// hal_Sys32kClkOut
// --------------------------------------------------------------------------
/// Manage the output of the 32K Clock, to an external device
/// @param enable If \c TRUE, enable 32K clock output.
///               If \c FALSE, disable 32K clock output.
// ==========================================================================
PUBLIC VOID hal_Sys32kClkOut(BOOL enable);



#if (!CHIP_HAS_ASYNC_TCU)    
// =============================================================================
// hal_SysQBitError
// -----------------------------------------------------------------------------
/// This function returns error on QBit due to clock switch mechanism.
/// If the error has to be compensated the error is cleared.
/// @param forceReset force to clear the error counter.
/// @return -1, 0 or 1 QBit error to be compensated.
// =============================================================================
PUBLIC INT32 hal_SysQBitError(BOOL forceReset);
#endif // (!CHIP_HAS_ASYNC_TCU)



// =============================================================================
// hal_SysIrqPending
// -----------------------------------------------------------------------------
/// This function checks the IRQ pending status. It should be called in critical
/// section to know if some IRQ are pending and then decide to release the 
/// critical section.
/// @return \c TRUE if IRQ are pending.
///         \c FALSE Otherwise.
// =============================================================================
PUBLIC BOOL hal_SysIrqPending(VOID);



// =============================================================================
PUBLIC VOID hal_SysProcessHostMonitor(VOID);
// hal_SysProcessIdle
// -----------------------------------------------------------------------------
/// This function is called in the idle task. 
/// It is used to execute in idle the function processing  possible commands
/// from the Host, and thus, for example, call the executor. etc ...
// =============================================================================
PUBLIC VOID hal_SysProcessIdle(VOID);



// =============================================================================
// hal_SysWaitMicrosecond
// -----------------------------------------------------------------------------
/// This function wait some time in microseconds.
/// It calculates CPU instruction cycles needed, depending on current system frequency.
/// Note that it does NOT have critical section operations.
/// @param us Microseconds to wait.
// =============================================================================
PUBLIC VOID hal_SysWaitMicrosecond(UINT32 us);


PUBLIC VOID hal_IsRunningInIrqNow(u32* K1);

/// @} // end of the system group

// ==========================================================================
// 
// --------------------------------------------------------------------------
/// 
/// @param 
/// 
// ==========================================================================
VOID hal_SysSetTxStbDiv(UINT32 txstbClk);
// ==========================================================================
// 
// --------------------------------------------------------------------------
/// 
/// @param 
/// 
// ==========================================================================
VOID hal_SysSetBclkDiv(UINT32 bClk);
// ==========================================================================
// 
// --------------------------------------------------------------------------
/// 
/// @param 
/// 
// ==========================================================================
VOID hal_SysSetMclkDiv(UINT32 mClk);
// ==========================================================================
// 
// --------------------------------------------------------------------------
/// 
/// @param 
/// 
// ==========================================================================
VOID hal_SysSetPerClkForceon(UINT32 clkBit);

VOID hal_SysWriteRFUsingSPI(void);
VOID hal_SysWriteRFUsingAPB(void);

UINT32 hal_SysEnterRFIFCtrlAPB(VOID);
VOID hal_SysExitRFIFCtrlAPB(UINT32 value);

VOID hal_SysPMUIntfXtalCtrl(UINT8 enable);

PUBLIC VOID hal_SysEnableClkOut(uint8 div);
PUBLIC HAL_SYS_SPIFLASH_FREQ_T hal_SysSetSpiFlashClock(HAL_SYS_SPIFLASH_FREQ_T newClock);

#endif // _HAL_SYS_H_

