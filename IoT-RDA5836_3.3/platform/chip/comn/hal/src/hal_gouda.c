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
/// @file hal_gouda.c
/// This file contains the GOUDA module driver 
/// implementation.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#if (CHIP_HAS_GOUDA==1)

#include "chip_id.h"

#include "cs_types.h"

#include "global_macros.h"
#include "gouda.h"
#include "sys_irq.h"

#include "hal_gouda.h"
#include "hal_sys.h"
#include "halp_sys.h"
#include "hal_error.h"
#include "hal_gpio.h"
#include "sxr_tls.h"
#include "pmd_m.h"

#include "halp_debug.h"
#include "halp_sys_ifc.h"
#include "halp_config.h"
#include "halp_gouda.h"


// ============================================================================
//  DEFINES
// ============================================================================

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
#define DEFERRED_TIMINGS_SET hal_GoudaDeferredTimingsSet()
#else
#define DEFERRED_TIMINGS_SET
#endif

#undef HAL_FUNC_INTERNAL
#define HAL_FUNC_INTERNAL

// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================

/// Store the LCD configuration from tgt to add command value in blit
PRIVATE HAL_GOUDA_LCD_CONFIG_T g_halGoudaLcdConfig;
PRIVATE UINT32 g_halGoudaEbcCsAddr;

/// Store the timings
PRIVATE CONST HAL_GOUDA_LCD_TIMINGS_T* g_halGoudaLcdTimings = NULL;

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
/// Store the current timing index
PRIVATE UINT32 g_halGoudaCurTimingIdx = 0;
/// Store the newly requested timing index
PRIVATE UINT32 g_halGoudaNewTimingIdx = 0;
#endif // GOUDA_UPDATE_TIMINGS_WORKAROUND

#if (CHIP_HAS_ASYNC_TCU)
/// Store the serial LCD configuration from tgt to
/// add command value in blit
PRIVATE HAL_GOUDA_SERIAL_LCD_CONFIG_T g_halGoudaSlcdCfg = { .reg = 0 };
/// Store the SPI frequency for the serial LCD
PRIVATE UINT32 g_halGoudaSpiFreq = 0;
#endif

// =============================================================================
// g_halGoudaIrqHandler
// -----------------------------------------------------------------------------
/// Variable to store the user irq handler for the lcdc
/// interruption.
// =============================================================================
PRIVATE HAL_GOUDA_IRQ_HANDLER_T g_halGoudaIrqHandler = NULL ;


// ============================================================================
//  FUNCTIONS
// ============================================================================

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
// =============================================================================
// hal_GoudaDeferredTimingsSet
// -----------------------------------------------------------------------------
/// Set the timings when Gouda is idle.
// =============================================================================
PRIVATE VOID hal_GoudaDeferredTimingsSet(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();

    if (g_halGoudaNewTimingIdx != g_halGoudaCurTimingIdx)
    {
        if (!hal_GoudaIsActive())
        {
#ifdef USE_SPI_LCD
            g_halGoudaSlcdCfg.clkDivider = g_halGoudaNewTimingIdx;
            hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
#else
            hwp_gouda->gd_lcd_timing = g_halGoudaLcdTimings[g_halGoudaNewTimingIdx].reg;
#endif
            g_halGoudaCurTimingIdx = g_halGoudaNewTimingIdx;
        }
    }

    hal_SysExitCriticalSection(status);
}
#endif // GOUDA_UPDATE_TIMINGS_WORKAROUND


// =============================================================================
// hal_GoudaUpdateTimings
// -----------------------------------------------------------------------------
/// Update the timings used by the Gouda module to fit optimally with the 
/// system frequency. After the call to the #hal_GoudaOpen() function, an 
/// array of timings, one for each system frequency, is registered in to the 
/// HAL GOUDA LCD driver.
/// @param sysFreq (New) System frequency for which new timings must be 
/// configured.
// =============================================================================
PRIVATE VOID hal_GoudaUpdateTimings(HAL_SYS_FREQ_T sysFreq)
{
    // Index in the timing registry as to where to find
    // the settings to apply at that given frequency.
    UINT32 index=0;
    switch (sysFreq)
    {
        case HAL_SYS_FREQ_32K  :
            index = 0;
            break;

        case HAL_SYS_FREQ_13M  :
            index = 1;
            break;

        case HAL_SYS_FREQ_26M  :
            index = 2;
            break;

        case HAL_SYS_FREQ_39M  :
            index = 3;
            break;

        case HAL_SYS_FREQ_52M  :
            index = 4;
            break;

        case HAL_SYS_FREQ_78M  :
            index = 5;
            break;

        case HAL_SYS_FREQ_104M :
            index = 6;
            break;

        case HAL_SYS_FREQ_156M :
            index = 7;
            break;

#if (CHIP_HAS_ASYNC_TCU)
        case HAL_SYS_FREQ_178M :
            index = 8;
            break;

        case HAL_SYS_FREQ_208M :
            index = 9;
            break;

        case HAL_SYS_FREQ_250M :
            index = 10;
            break;

        case HAL_SYS_FREQ_312M :
            index = 11;
            break;
#endif
        default:
            HAL_ASSERT(FALSE, "HAL: LCDC: Unsupported system "
                    "frequency %d", sysFreq);
    }

    HAL_ASSERT(index<HAL_GOUDA_LCD_TIMINGS_QTY, "HAL: GOUDA LCD: Timing "
            "For this index don't exist: %d", index);

    HAL_ASSERT(g_halGoudaLcdTimings!= NULL, "HAL: GOUDA LCD: No Timings"
            "registered yet");

    /// @todo update to >= when hardware is updated
    HAL_ASSERT( g_halGoudaLcdTimings[index].pwh > g_halGoudaLcdTimings[index].tas + g_halGoudaLcdTimings[index].tah, "HAL: GOUDA Timing error pwh (%d) > tas(%d) + tah(%d) not respected!", g_halGoudaLcdTimings[index].pwh, g_halGoudaLcdTimings[index].tas, g_halGoudaLcdTimings[index].tah);

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
    // If Gouda is running, the timing values are compared in equality with internal counters.
    // There is a race condition that decreasing timing values will miss the comparison forever,
    // because Gallite h/w compares the values in equality only and never wraps the counters.
    // The workaround can be removed when future chips fix this issue.

    // NOTE:
    // As a requirement of the workaround,
    // the following 5 values should be monotonically increased with the index:
    // tas, tah, pwl, pwh, pwh-(tas+tah)

    UINT32 status = hal_SysEnterCriticalSection();

    g_halGoudaNewTimingIdx = index;
    if (index < g_halGoudaCurTimingIdx && hal_GoudaIsActive())
    {
        // Update the timings later
        hal_SysExitCriticalSection(status);
        return;
    }
    g_halGoudaCurTimingIdx = index;
#endif // GOUDA_UPDATE_TIMINGS_WORKAROUND

    // Timings are cleverly represented by
    // bitfields:
    hwp_gouda->gd_lcd_timing = g_halGoudaLcdTimings[index].reg;

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
    hal_SysExitCriticalSection(status);
#endif // GOUDA_UPDATE_TIMINGS_WORKAROUND
}


#if (CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_GoudaUpdateSerialTiming
// -----------------------------------------------------------------------------
/// Update the timing used by the Gouda module to fit optimally with the 
/// system frequency. After the call to the #hal_GoudaOpen() function,
/// the timing is registered in to the HAL GOUDA LCD driver.
/// @param sysFreq (New) System frequency for which new timing must be 
/// configured.
// =============================================================================
PRIVATE VOID hal_GoudaUpdateSerialTiming(HAL_SYS_FREQ_T sysFreq)
{
    UINT32 status = hal_SysEnterCriticalSection();

    UINT32 divider = sysFreq / g_halGoudaSpiFreq;
    if (divider == 0)
    {
        divider = 1;
    }
    else if (divider > GD_MAX_SLCD_CLK_DIVIDER)
    {
        divider = GD_MAX_SLCD_CLK_DIVIDER;
    }

#ifdef GOUDA_UPDATE_TIMINGS_WORKAROUND
    g_halGoudaNewTimingIdx = divider;
    if (divider < g_halGoudaCurTimingIdx && hal_GoudaIsActive())
    {
        // Update the timings later
        hal_SysExitCriticalSection(status);
        return;
    }
    g_halGoudaCurTimingIdx = divider;
#endif
    g_halGoudaSlcdCfg.clkDivider = divider;

    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    hal_SysExitCriticalSection(status);
}
#endif

// =============================================================================
// hal_GoudaOpen
// -----------------------------------------------------------------------------
/// Common operations to open Gouda for a normal LCD or a serial LCD.
/// @param handler Function to update Gouda timings.
// =============================================================================
#ifdef DUAL_LCD
PRIVATE VOID hal_GoudaCommonOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, UINT32 ebcCsAddr, HAL_SYS_FREQ_CHANGE_HANDLER_T handler,BOOL Rst)
#else
PRIVATE VOID hal_GoudaCommonOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, UINT32 ebcCsAddr, HAL_SYS_FREQ_CHANGE_HANDLER_T handler)
#endif
{
    // Power on LCD
    pmd_EnablePower(PMD_POWER_LCD, TRUE);
    // Request a working frequency to avoid enter deep sleep, and
    // register a clock update function
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_26M,handler);

    UINT32 scStatus = hal_SysEnterCriticalSection();
    // Apply timings for the current system clock frequency:
    handler(hal_SysGetFreq());
    hal_SysExitCriticalSection(scStatus);

    // Reset LCD after requesting freq since it cannot be interrupted by deep sleep
#ifdef DUAL_LCD
    if(Rst)
#endif
    {
        if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
            g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
        {
            if (g_halCfg->goudaCfg.lcdResetActiveLow)
            {
                hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
                COS_Sleep(15 );
                hal_GpioSet(g_halCfg->goudaCfg.lcdResetPin);
            }
            else
            {
                hal_GpioSet(g_halCfg->goudaCfg.lcdResetPin);
                COS_Sleep(15);
                hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
            }
        }
    }
    // This describes the way the LCD is plugged to the 
    // controller.
    g_halGoudaLcdConfig.reg = config->reg;
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;

    g_halGoudaEbcCsAddr = ebcCsAddr;
    hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
}


// =============================================================================
// hal_GoudaOpen
// -----------------------------------------------------------------------------
/// Open the LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param config Pointer to the controller configuration.
/// @param timings Controller timing array (must be provided and valid,
///   even if unused in case of #HAL_GOUDA_LCD_MEMORY_IF)
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================
#ifdef DUAL_LCD
PUBLIC VOID hal_GoudaOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, CONST HAL_GOUDA_LCD_TIMINGS_T* timings, UINT32 ebcCsAddr,BOOL Rst)
#else
PUBLIC VOID hal_GoudaOpen(CONST HAL_GOUDA_LCD_CONFIG_T* config, CONST HAL_GOUDA_LCD_TIMINGS_T* timings, UINT32 ebcCsAddr)
#endif
{
    // TODO Add parameter validity check.

    // Set configuration register
#if (CHIP_HAS_ASYNC_TCU)
    g_halGoudaSlcdCfg.isSlcd = FALSE;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
#endif

    // Register the timings
    g_halGoudaLcdTimings = timings;

    // Common open operations
 #ifdef DUAL_LCD   
    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateTimings, Rst);
 #else
    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateTimings);
 #endif
}


#if (CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_GoudaSerialOpen
// -----------------------------------------------------------------------------
/// Open the serial LCD controller of Gouda
/// Parameters are used to describe the screen connexion configuration and 
/// the controller timings to use depending on the system clock frequency.
/// @param spiLineType The SPI line type of the serial LCD.
/// @param spiFreq The SPI frequency of the serial LCD.
/// @param config Pointer to the controller configuration.
/// @param ebcCsAddr address of ebc CS for #HAL_GOUDA_LCD_MEMORY_IF,
///   can be 0 for gouda CS.
// =============================================================================
 #ifdef DUAL_LCD   
PUBLIC VOID hal_GoudaSerialOpen(HAL_GOUDA_SPI_LINE_T spiLineType, UINT32 spiFreq, CONST HAL_GOUDA_LCD_CONFIG_T* config, UINT32 ebcCsAddr, BOOL Rst)
#else
PUBLIC VOID hal_GoudaSerialOpen(HAL_GOUDA_SPI_LINE_T spiLineType, UINT32 spiFreq, CONST HAL_GOUDA_LCD_CONFIG_T* config, UINT32 ebcCsAddr)
#endif
{
    // TODO Add parameter validity check.
    HAL_ASSERT(config->outputFormat == HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565,
               "Serial interface only accepts 16-bit RGB565 output format");

    // Set configuration register
    g_halGoudaSlcdCfg.isSlcd = TRUE;
    g_halGoudaSlcdCfg.clkDivider = 10;
    g_halGoudaSlcdCfg.lineType = spiLineType;
    g_halGoudaSlcdCfg.isRead = FALSE;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;

    // Register the timings
    g_halGoudaSpiFreq = spiFreq;

    // Common open operations
 #ifdef DUAL_LCD       
    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateSerialTiming, Rst);
 #else
    hal_GoudaCommonOpen(config, ebcCsAddr, hal_GoudaUpdateSerialTiming);
 #endif
}
#endif


PUBLIC VOID hal_GoudaSleep(VOID)
{
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_32K,NULL);
}

PUBLIC VOID hal_GoudaWakeup(VOID)
{
    // Register a clock update function
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_26M,hal_GoudaUpdateSerialTiming);
}


// =============================================================================
// hal_GoudaClose
// -----------------------------------------------------------------------------
/// Close Gouda.
// =============================================================================
PUBLIC VOID hal_GoudaClose(VOID)
{
    while(hal_GoudaIsActive());
    // FIXME Is this enough ?
#if (CHIP_HAS_ASYNC_TCU)
    g_halGoudaSlcdCfg.reg = 0;
    hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
#endif
    g_halGoudaLcdConfig.reg = 0;
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;
    g_halGoudaIrqHandler = NULL;
    hal_SysRequestFreq(HAL_SYS_FREQ_LCD,HAL_SYS_FREQ_32K,NULL);
    // Output low level on LCD reset pin to avoid leaking current after powering off LCD
    if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
        g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
    {
        hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
    }
    // Power off LCD
    pmd_EnablePower(PMD_POWER_LCD, FALSE);
}


// =============================================================================
// hal_GoudaWriteReg
// -----------------------------------------------------------------------------
/// Write a command and then a data to the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to write, or command to send
/// to the LCD screen
/// @param data Corresponding data to write to the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteReg(UINT16 addr, UINT16 data)
{
    DEFERRED_TIMINGS_SET;
    
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);

        // Unavoidable wait for the previous
        // command to be sent.
        while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_TYPE|GOUDA_LCD_DATA(data);
        
        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_GoudaReadData
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @param len Data length to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadData(UINT16 addr, UINT8 *pData, UINT32 len)
{
    DEFERRED_TIMINGS_SET;

    HAL_ASSERT(len > 0, "Invalid LCD read length: %d", len);

    int i;
    HAL_ERR_T errorStatus = HAL_ERR_NO;

#if (CHIP_HAS_ASYNC_TCU)
    if (g_halGoudaSlcdCfg.isSlcd)
    {
        HAL_ASSERT(len <= GD_MAX_SLCD_READ_LEN,
                   "Invalid SLCD read length: %d (max: %d)",
                   len, GD_MAX_SLCD_READ_LEN);
    }
    else
#endif
    if (g_halCfg->goudaCfg.lcdData16Bit)
    {
        HAL_ASSERT((len&0x1) == 0,
                   "Invalid LCD 16-bit read length: %d",
                   len);
    }

    UINT32 status = hal_SysEnterCriticalSection();

    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;

#if (CHIP_HAS_ASYNC_TCU)
        if (g_halGoudaSlcdCfg.isSlcd)
        {
            HAL_GOUDA_SERIAL_LCD_CONFIG_T slcdCfg;
            slcdCfg.reg = g_halGoudaSlcdCfg.reg;
            slcdCfg.rxByte = len;
            slcdCfg.isRead = TRUE;
            hwp_gouda->gd_spilcd_config = slcdCfg.reg;
        }
#endif

        // Send register address
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);
        while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));

#if (CHIP_HAS_ASYNC_TCU)
        if (g_halGoudaSlcdCfg.isSlcd)
        {
            i = 0;
            while(i < len)
            {
                pData[i] = (hwp_gouda->gd_spilcd_rd>>(8*i)) & 0xff;
                i++;
            }
            // Recover the original register context
            hwp_gouda->gd_spilcd_config = g_halGoudaSlcdCfg.reg;
        }
        else
#endif
        {
            // Start to read
            hwp_gouda->gd_lcd_single_access=GOUDA_START_READ|GOUDA_TYPE;
            while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));

            // Get the data
            i = 0;
            while (i < len)
            {
                if (g_halCfg->goudaCfg.lcdData16Bit)
                {
                    pData[i] = hwp_gouda->gd_lcd_single_access & 0xff;
                    pData[i+1] = (hwp_gouda->gd_lcd_single_access>>8) & 0xff;
                    i += 2;
                }
                else
                {
                    pData[i] = hwp_gouda->gd_lcd_single_access & 0xff;
                    i++;
                }
                hwp_gouda->gd_lcd_single_access=GOUDA_START_READ|GOUDA_TYPE;
                while((hwp_gouda->gd_status & GOUDA_LCD_BUSY));
            }
        }

        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}


// =============================================================================
// hal_GoudaReadReg
// -----------------------------------------------------------------------------
/// Read a data from the LCD screen (Or a value at a register
/// address, depending on the point of view).
/// 
/// @param addr Address of the register to read
/// @param pData Pointer of the corresponding data to read from the LCD.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the GOUDA LCD controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaReadReg(UINT16 addr, UINT16 *pData)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    UINT32 len;

    *pData = 0;
#if (CHIP_HAS_ASYNC_TCU)
    if (g_halGoudaSlcdCfg.isSlcd)
    {
        len = 1;
    }
    else
#endif
    {
        len = 2;
    }

    errorStatus = hal_GoudaReadData(addr, (UINT8 *)pData, len);
    if (errorStatus == HAL_ERR_NO)
    {
        // Construct 16-bit register content for 8-bit interface
        if (!g_halCfg->goudaCfg.lcdData16Bit)
        {
            // Swap the byte order
            *pData = (((*pData)&0xff)<<8) | (((*pData)>>8)&0xff);
        }
    }

    return errorStatus;
}

PUBLIC HAL_ERR_T hal_GoudaReadRegLength(UINT16 addr, UINT8 *pData,UINT8 length)
{
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    UINT32 len;

    *pData = 0;
#if (CHIP_HAS_ASYNC_TCU)
    if (g_halGoudaSlcdCfg.isSlcd)
    {
        len = 1;
    }
    else
#endif
    {
        len = length;
    }

    errorStatus = hal_GoudaReadData(addr, (UINT8 *)pData, len);

    return errorStatus;
}


// =============================================================================
// hal_GoudaWriteCmd
// -----------------------------------------------------------------------------
/// Write a command to the LCD.
///
/// @param addr Command to write.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteCmd(UINT16 addr)
{
    DEFERRED_TIMINGS_SET;
    
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_LCD_DATA(addr);
        errorStatus = HAL_ERR_NO;
    }
    
    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_GoudaWriteData
// -----------------------------------------------------------------------------
/// Write a data to the LCD screen.
///
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaWriteData(UINT16 data)
{
    DEFERRED_TIMINGS_SET;
    
    HAL_ERR_T errorStatus = HAL_ERR_NO;
    
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_gouda->gd_status & GOUDA_LCD_BUSY) != 0)
    {
        // GOUDA LCD is busy
        errorStatus = HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Set configuration register and commands
        hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg; // from tgt
        // GOUDA LCD is available, we use it
        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        hwp_gouda->gd_lcd_single_access=GOUDA_START_WRITE|GOUDA_TYPE|GOUDA_LCD_DATA(data);
        errorStatus = HAL_ERR_NO;
    }

    hal_SysExitCriticalSection(status);
    return errorStatus;
}

// =============================================================================
// hal_GoudaBlitRoi
// -----------------------------------------------------------------------------
/// Send a block of pixel data to the LCD screen.
/// 
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param nbCmd number of commands to send to LCD before pixel data
/// @param pCmds commands to send to LCD before pixel data can be \c NULL if \p nbCmd is 0
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi(CONST HAL_GOUDA_WINDOW_T* pRoi, CONST UINT32 nbCmd, CONST HAL_GOUDA_LCD_CMD_T* pCmds, HAL_GOUDA_IRQ_HANDLER_T handler)
{
    DEFERRED_TIMINGS_SET;
    
    // check ROI
    HAL_ASSERT(pRoi != NULL, "hal_GoudaBlitRoi pRoi must not be NULL!");
    HAL_ASSERT(((pRoi->tlPX < pRoi->brPX) && (pRoi->tlPY < pRoi->brPY)),
            "hal_GoudaBlitRoi invalid, top point and bottom right point not ordered");

    // check Ready
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        // GOUDA is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        UINT32 i;
        HAL_GOUDA_LCD_CONFIG_T goudaLcdConfig;
        // GOUDA LCD is available, we use it

        hwp_gouda->gd_lcd_mem_address = g_halGoudaEbcCsAddr;
        // configure ROI in Gouda
        hwp_gouda->gd_roi_tl_ppos = GOUDA_X0(pRoi->tlPX) | GOUDA_Y0(pRoi->tlPY);
        hwp_gouda->gd_roi_br_ppos = GOUDA_X1(pRoi->brPX) | GOUDA_Y1(pRoi->brPY);

        // Set configuration register and commands
        goudaLcdConfig.reg = g_halGoudaLcdConfig.reg; // from tgt
        goudaLcdConfig.nbCmd = nbCmd; // from driver
        hwp_gouda->gd_lcd_ctrl = goudaLcdConfig.reg;
        for(i = 0; i < nbCmd; i++)
        {
            // it usses addressing to configure the command/data bit in Gouda
            if(pCmds[i].isData)
            {
                // datas are after workbuf and commands
                ((UINT16*)hwp_goudaSram)[GD_NB_WORKBUF_WORDS+GD_NB_LCD_CMD_WORDS+i] = pCmds[i].value;
            }
            else
            {
                // commands are after workbuf
                ((UINT16*)hwp_goudaSram)[GD_NB_WORKBUF_WORDS+i] = pCmds[i].value;
            }
        }

        // Record handler and enable interrupt if needed.
        g_halGoudaIrqHandler = handler;
        if (g_halGoudaIrqHandler != NULL)
        {
            // Set Mask
            hwp_gouda->gd_eof_irq_mask = GOUDA_EOF_MASK;
        }
        else
        {
            // Disable IRQ
            hwp_gouda->gd_eof_irq_mask = 0;
        }

        // Run GOUDA
        hwp_gouda->gd_command = GOUDA_START;
    
        // that's it
        hal_SysExitCriticalSection(status);
//until gouda work complete qiff ,if have problem call me        
        while(hal_GoudaIsActive());
    
        return HAL_ERR_NO;
    }
}

// =============================================================================
// hal_GoudaBlitRoi2Ram
// -----------------------------------------------------------------------------
/// Send a block of pixel data to a flat ram buffer in RGB565 format
/// WARNING: only even pixel number are supported, that is to say size must
/// be a mutiple of 4 bytes.
/// 
/// @param pRoi Pointer to the roi description to send to the screen.
/// @param handler If not NULL, user function that is called when the transfer
/// is finished.
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the LCDC controller is busy with another command.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaBlitRoi2Ram(UINT32* pBuffer, CONST UINT16 width, CONST HAL_GOUDA_WINDOW_T* pRoi, HAL_GOUDA_IRQ_HANDLER_T handler)
{
    DEFERRED_TIMINGS_SET;
    
    INT32 offset = 0; // default to 0 when width == 0

    // check ROI
    HAL_ASSERT(pRoi != NULL, "hal_GoudaBlitRoi2Ram pRoi must not be NULL!");
    HAL_ASSERT(((pRoi->tlPX < pRoi->brPX) && (pRoi->tlPY < pRoi->brPY)),
            "hal_GoudaBlitRoi2Ram roi invalid, top point and bottom right point not ordered");

    // compute offset
    if (width != 0)
    {
        INT32 roiWidth=pRoi->brPX - pRoi->tlPX + 1;
        HAL_ASSERT(width >= roiWidth, "hal_GoudaBlitRoi2Ram width (%d) must be greater or equal to width of roi (%d)", width, roiWidth);
        // offset is in byte
        offset = (width-roiWidth)*2;
    }

    // check Ready
    UINT32 status = hal_SysEnterCriticalSection();
    
    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        // GOUDA is busy, abort
        hal_SysExitCriticalSection(status);
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        HAL_GOUDA_LCD_CONFIG_T goudaLcdConfig;
        // GOUDA LCD is available, we use it

        // configure ROI in Gouda
        hwp_gouda->gd_roi_tl_ppos = GOUDA_X0(pRoi->tlPX) | GOUDA_Y0(pRoi->tlPY);
        hwp_gouda->gd_roi_br_ppos = GOUDA_X1(pRoi->brPX) | GOUDA_Y1(pRoi->brPY);

        // Set configuration register
        goudaLcdConfig.reg = g_halGoudaLcdConfig.reg; // from tgt
        goudaLcdConfig.cs = HAL_GOUDA_LCD_IN_RAM;
        goudaLcdConfig.outputFormat = HAL_GOUDA_LCD_OUTPUT_FORMAT_16_bit_RGB565;
        hwp_gouda->gd_lcd_ctrl = goudaLcdConfig.reg;
        
        // Record handler and enable interrupt if needed.
        g_halGoudaIrqHandler = handler;
        if (g_halGoudaIrqHandler != NULL)
        {
            // Set Mask
            hwp_gouda->gd_eof_irq_mask = GOUDA_EOF_MASK;
        }
        else
        {
            // Disable IRQ
            hwp_gouda->gd_eof_irq_mask = 0;
        }


        hwp_gouda->gd_lcd_mem_address = (UINT32)pBuffer;
        hwp_gouda->gd_lcd_stride_offset = offset;
        // Run GOUDA
        hwp_gouda->gd_command = GOUDA_START;
    
        // that's it
        hal_SysExitCriticalSection(status);
        return HAL_ERR_NO;
    }
}


// =============================================================================
// hal_GoudaIsActive
// -----------------------------------------------------------------------------
/// Get the activity status
/// @return \c TRUE when active
// =============================================================================
PUBLIC BOOL hal_GoudaIsActive(VOID)
{
    if ((hwp_gouda->gd_status & (GOUDA_IA_BUSY | GOUDA_LCD_BUSY)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


// =============================================================================
// hal_GoudaSetBgColor
// -----------------------------------------------------------------------------
/// Set the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC VOID hal_GoudaSetBgColor(UINT16 color)
{
    hwp_gouda->gd_roi_bg_color = color;
}

// =============================================================================
// hal_GoudaGetBgColor
// -----------------------------------------------------------------------------
/// Get the background color
/// @param color in RGB565 format
// =============================================================================
PUBLIC UINT16 hal_GoudaGetBgColor(VOID)
{
    return hwp_gouda->gd_roi_bg_color;
}

// =============================================================================
// hal_GoudaOvlLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure an overlay layer.
///
/// @param id the layer Id
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaOvlLayerOpen(HAL_GOUDA_OVL_LAYER_ID_T id, HAL_GOUDA_OVL_LAYER_DEF_T* def)
{
    DEFERRED_TIMINGS_SET;
    
    HAL_ASSERT(id < HAL_GOUDA_OVL_LAYER_ID_QTY, "hal_GoudaOvlLayerOpen invalid ID:%d", id);
    if ((hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt & GOUDA_ACTIVE) != 0)
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
    UINT8 wpp = (def->fmt==HAL_GOUDA_IMG_FORMAT_RGBA)?2:1;
    // Note that stride could be different in case of padding
    UINT16 stride;
    if (def->stride)
    {
        stride = def->stride;
    }
    else
    {
        stride = (def->pos.brPX - def->pos.tlPX + 1) *wpp;
    }

    HAL_ASSERT(!((def->pos.tlPX > def->pos.brPX) || (def->pos.tlPY > def->pos.brPY)),
            "hal_GoudaOvlLayerOpen pos invalid, top point and bottom right point not ordered");

    // TODO check supported formats 
    hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt = 
            ((def->fmt==HAL_GOUDA_IMG_FORMAT_RGB565)?GOUDA_FORMAT(0):
                                                     GOUDA_FORMAT(1)) |
            GOUDA_STRIDE(stride) |
            GOUDA_ACTIVE;
    hwp_gouda->Overlay_Layer[id].gd_ol_tl_ppos = 
            GOUDA_X0(def->pos.tlPX) | GOUDA_Y0(def->pos.tlPY);
    hwp_gouda->Overlay_Layer[id].gd_ol_br_ppos = 
            GOUDA_X1(def->pos.brPX) | GOUDA_Y1(def->pos.brPY);
    hwp_gouda->Overlay_Layer[id].gd_ol_blend_opt =
            GOUDA_ALPHA(def->alpha) |
            GOUDA_CHROMA_KEY_COLOR(def->cKeyColor) |
            ((def->cKeyEn)?GOUDA_CHROMA_KEY_ENABLE:0) |
            GOUDA_CHROMA_KEY_MASK(def->cKeyMask);
    hwp_gouda->Overlay_Layer[id].gd_ol_rgb_src = (UINT32)def->addr;

    return HAL_ERR_NO;
}

// =============================================================================
// hal_GoudaOvlLayerClose
// -----------------------------------------------------------------------------
/// Close an overlay layer.
// =============================================================================
PUBLIC VOID hal_GoudaOvlLayerClose(HAL_GOUDA_OVL_LAYER_ID_T id)
{
    DEFERRED_TIMINGS_SET;
    
    HAL_ASSERT(id < HAL_GOUDA_OVL_LAYER_ID_QTY, "hal_GoudaOvlLayerClose invalid ID:%d", id);
    hwp_gouda->Overlay_Layer[id].gd_ol_input_fmt = 0;
}


// =============================================================================
// hal_GoudaVidLayerOpen
// -----------------------------------------------------------------------------
/// Open and configure the video layer.
///
/// @param def the configuration
/// @return #HAL_ERR_NO when everything is fine, #HAL_ERR_RESOURCE_BUSY when
/// the layer is already enabled.
// =============================================================================
PUBLIC HAL_ERR_T hal_GoudaVidLayerOpen(HAL_GOUDA_VID_LAYER_DEF_T* def)
{
    DEFERRED_TIMINGS_SET;
    
    if ((hwp_gouda->gd_vl_input_fmt & GOUDA_ACTIVE) != 0)
    {
        return HAL_ERR_RESOURCE_BUSY;
    }

    UINT32 xpitch, ypitch;
    UINT8 bpp = (def->fmt==HAL_GOUDA_IMG_FORMAT_IYUV)?1:2;
    // Note that stride could be different in case of padding
    UINT16 stride = def->stride?def->stride:def->width*bpp;
    UINT16 outWidth = def->pos.brPX - def->pos.tlPX + 1;
    UINT16 outHeight = def->pos.brPY - def->pos.tlPY + 1;

    HAL_ASSERT(((def->pos.tlPX < def->pos.brPX) && (def->pos.tlPY < def->pos.brPY)),
            "hal_GoudaVidLayerOpen pos invalid, top point and bottom right point not ordered");

    xpitch = (def->width<<8) / outWidth;
    ypitch = (def->height<<8) / outHeight;
    hwp_gouda->gd_vl_extents = GOUDA_MAX_COL(def->width-1) | GOUDA_MAX_LINE(def->height-1);
    hwp_gouda->gd_vl_y_src = (UINT32)def->addrY;
    hwp_gouda->gd_vl_u_src = (UINT32)def->addrU;
    hwp_gouda->gd_vl_v_src = (UINT32)def->addrV;
#if (CHIP_HAS_ASYNC_TCU)
    HAL_ASSERT( ((xpitch < 0x800) && (ypitch < 0x800)), "hal_GoudaVidLayerOpen image zoom ratio out of range (0x%0x,0x%0x)",xpitch,ypitch);
#else
    HAL_ASSERT( ((xpitch < 0x400) && (ypitch < 0x400)), "hal_GoudaVidLayerOpen image zoom ratio out of range (0x%0x,0x%0x)",xpitch,ypitch);
#endif
#ifndef HAL_NO_PRINTF
    ///@todo check this limits
    if((xpitch < 0x10) || (ypitch < 0x10))
    {
        // could check if ratio is exact:(xpitch*outWidth == def->height<<8)
        HAL_TRACE(HAL_LCD_TRC, 0, "hal_GoudaVidLayerOpen image zoom ratio will introduce precision errors (%d,%d)",xpitch,ypitch);
    }
#endif

#ifdef GALLITE_IS_8805
    hwp_gouda->gd_vl_resc_ratio = GOUDA_XPITCH(xpitch) | GOUDA_YPITCH(ypitch);
#else
    hwp_gouda->gd_vl_resc_ratio = GOUDA_XPITCH(xpitch) | GOUDA_YPITCH(ypitch) | GOUDA_YPITCH_SCALE_ENABLE;
#endif

    // TODO check supported formats 
    hwp_gouda->gd_vl_input_fmt = 
            ((def->fmt==HAL_GOUDA_IMG_FORMAT_RGB565)?GOUDA_FORMAT(0):
            (def->fmt==HAL_GOUDA_IMG_FORMAT_UYVY)?GOUDA_FORMAT(1):
            (def->fmt==HAL_GOUDA_IMG_FORMAT_YUYV)?GOUDA_FORMAT(2):
            (def->fmt==HAL_GOUDA_IMG_FORMAT_IYUV)?GOUDA_FORMAT(3):0) |
            GOUDA_STRIDE(stride) |
            GOUDA_ACTIVE;
    hwp_gouda->gd_vl_tl_ppos = 
            GOUDA_X0(def->pos.tlPX) | GOUDA_Y0(def->pos.tlPY);
    hwp_gouda->gd_vl_br_ppos = 
            GOUDA_X1(def->pos.brPX) | GOUDA_Y1(def->pos.brPY);
    hwp_gouda->gd_vl_blend_opt =
            GOUDA_DEPTH(def->depth) |
            GOUDA_ALPHA(def->alpha) |
            GOUDA_CHROMA_KEY_COLOR(def->cKeyColor) |
            ((def->cKeyEn)?GOUDA_CHROMA_KEY_ENABLE:0) |
            GOUDA_CHROMA_KEY_MASK(def->cKeyMask);


    return HAL_ERR_NO;
}

// =============================================================================
// hal_GoudaVidLayerClose
// -----------------------------------------------------------------------------
/// Close the video layer.
// =============================================================================
PUBLIC VOID hal_GoudaVidLayerClose(VOID)
{
    DEFERRED_TIMINGS_SET;
    
    hwp_gouda->gd_vl_input_fmt = 0;
}

// =============================================================================
// hal_GoudaVidLayerState
// -----------------------------------------------------------------------------
/// Get an video layer state.
// =============================================================================
PUBLIC UINT32 hal_GoudaVidLayerState(VOID)
{
    DEFERRED_TIMINGS_SET;
    
    return hwp_gouda->gd_vl_input_fmt;
}

// =============================================================================
// hal_GoudaInitResetPin
// -----------------------------------------------------------------------------
/// Initialize the status of LCD reset pin.
// =============================================================================
PUBLIC VOID hal_GoudaInitResetPin(VOID)
{
    if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO ||
        g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_O)
    {
        if (g_halCfg->goudaCfg.lcdResetPin.type == HAL_GPIO_TYPE_IO)
        {
            hal_GpioSetOut(g_halCfg->goudaCfg.lcdResetPin.gpioId);
        }
        // Keep LCD in reset state
        if (g_halCfg->goudaCfg.lcdResetActiveLow)
        {
            hal_GpioClr(g_halCfg->goudaCfg.lcdResetPin);
        }
        else
        {
            hal_GpioSet(g_halCfg->goudaCfg.lcdResetPin);
        }
    }
}


// =============================================================================
// hal_GoudaIrqHandler
// -----------------------------------------------------------------------------
/// Handles IRQ.
// =============================================================================
PROTECTED VOID hal_GoudaIrqHandler(UINT8 interruptId)
{
    UINT32 tmpRead;
    hwp_gouda->gd_eof_irq = GOUDA_EOF_CAUSE; // Clear interrupt
    tmpRead = hwp_gouda->gd_eof_irq;

    DEFERRED_TIMINGS_SET;
    
    // Call user handler
    if (g_halGoudaIrqHandler)
    {
        g_halGoudaIrqHandler();
    }
}


// =============================================================================
// hal_GoudaGetTiming
// -----------------------------------------------------------------------------
/// Get lcd timing.
// =============================================================================
PUBLIC UINT32 HAL_FUNC_INTERNAL hal_GoudaGetTiming(VOID)
{
    return hwp_gouda->gd_lcd_timing;
}



// =============================================================================
// hal_GoudaSetTiming
// -----------------------------------------------------------------------------
/// Set lcd timing.
// =============================================================================
PUBLIC VOID HAL_FUNC_INTERNAL hal_GoudaSetTiming(UINT32 gouda_timing)
{
    hwp_gouda->gd_lcd_timing = gouda_timing;
}
// =============================================================================
// hal_GoudaSetCsPin
// -----------------------------------------------------------------------------
/// Change LCD CS Pin .
// =============================================================================
#ifdef DUAL_LCD
PUBLIC VOID hal_GoudaSetCsPin(BOOL cs)
{
#ifndef __SUBLCD_WITH_SPILCD__
	HAL_GOUDA_LCD_CONFIG_T old_halGoudaLcdConfig;
	old_halGoudaLcdConfig.reg=g_halGoudaLcdConfig.reg;
	if(cs)
		{
		g_halGoudaLcdConfig.cs = HAL_GOUDA_LCD_CS_0;
		}
	else
		{
		g_halGoudaLcdConfig.cs =HAL_GOUDA_LCD_CS_1;
		}

	if(old_halGoudaLcdConfig.reg!=g_halGoudaLcdConfig.reg) 
    hwp_gouda->gd_lcd_ctrl = g_halGoudaLcdConfig.reg;
    //hwp_gouda->gd_lcd_mem_address = 0;
 #endif 

}
#endif  // DUAL_LCD
#endif  // CHIP_HAS_GOUDA


