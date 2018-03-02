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
/// @file hal_trace.c                                                         //
/// That file implements the HAL Trace Driver.                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "debug_uart.h"
#include "debug_host.h"
#include "sys_irq.h"
#include "sys_ifc.h"

#include "boot_usb_monitor.h"

#include "hal_uart.h"
#include "hal_gpio.h"
#include "hal_usb.h"
#include "hal_host.h"

#include "halp_sys_ifc.h"
#include "halp_sys.h"
#include "halp_irq.h"
#include "halp_debug.h"

#include "sxs_srl.h"
#include "sxs_rmt.h"

#include "string.h"
#include "rfd_xcv.h"

// =============================================================================
// Trace Usb Function
// =============================================================================
#ifdef	MEM_ULC_3216_SAVE_SRAM
#define HAL_TRACE_FORCE_HOST
PRIVATE          UINT8  g_halTraceUsbBuffer[16];
#else
PRIVATE          UINT8  g_halTraceUsbBuffer[4096];
#endif
PRIVATE          UINT16 g_halTraceUsbReadIndex;
PRIVATE          UINT16 g_halTraceUsbWriteIndex;
PRIVATE          UINT16 g_remainingBytesReturn       = 0;

//  ---------- Internal variables. ----------
#ifdef HAL_TRACE_FORCE_USB
PRIVATE          HAL_HST_TRACE_PORT_T g_HalTracePort         = HAL_HST_TRACE_PORT_USB;
#else
PRIVATE          HAL_HST_TRACE_PORT_T g_HalTracePort         = HAL_HST_TRACE_PORT_HOST;
#endif /* HAL_TRACE_FORCE_USB */

PRIVATE VOLATILE UINT8  g_hstTraceTxIfcCh      = HAL_UNKNOWN_CHANNEL;
PRIVATE          UINT16 g_remainingBytes       = 0;
PRIVATE CONST    UINT8* g_previousStartAddress = (UINT8*) 0xffffffff;
PRIVATE          BOOL   g_halHstTraceEnabled   = FALSE;

#if defined(HAL_TRACE_FORCE_USB) && defined(HAL_TRACE_FORCE_HOST)
  #error "You can't force USB (HAL_TRACE_FORCE_USB) and HOST (HAL_TRACE_FORCE_HOST) in same time, check your env file"
#endif

#if defined(HAL_TRACE_FORCE_USB) && !defined(CHIP_HAS_USB)
  #error "You can't force USB (HAL_TRACE_FORCE_USB) on chip without USB, check your env file"
#endif

// =============================================================================
// hal_HstTraceSwitch
// -----------------------------------------------------------------------------
/// Change trace output path
/// @param port Trace path (USB, Host)
// =============================================================================
PUBLIC VOID hal_HstTraceSwitch(HAL_HST_TRACE_PORT_T port)
{
#ifdef HAL_TRACE_FORCE_USB
    port = HAL_HST_TRACE_PORT_USB;
#endif /* HAL_TRACE_FORCE_USB */
#if defined(HAL_TRACE_FORCE_HOST) || defined(HAL_USB_PRINTF)
    port = HAL_HST_TRACE_PORT_HOST;
#endif /* HAL_TRACE_FORCE_HOST */

    if(g_HalTracePort != port)
    {
        UINT16 SetUart = SXS_SET_NB_DATA (SXS_8_DATA)
            | SXS_SET_NB_STOP (SXS_1_STOP)
            | SXS_SET_PARITY (SXS_PARITY_DISABLE)
            | SXS_SET_RATE (SXS_R460800);

        hal_HstTraceClose(g_HalTracePort);
        g_HalTracePort = port;
        sxs_InitSerial (SetUart, 0, SXS_RMT_DBG_CPU_IDX);
/*         hal_HstTraceOpen(0, g_HalTracePort); */
    }
}

// =======================================================
// hal_HstTraceOpen
// -------------------------------------------------------
/// Open and configure the host trace port.
/// The configuration is set to comply with the
/// trace tool running on the host.
/// Traces are not enabled after a call to this function.
/// A (positive) call to #hal_HstTraceEnable is needed before
/// it is possible to actually send traces.
///
/// @param settings Not taken into account
/// @param portIdx  Not taken into account
// =======================================================
PUBLIC VOID hal_HstTraceOpen(UINT16 setting, UINT8 portIdx)
{
    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
        //  set the resource as active
        // FIXME Does it need more ? Since the clock comes from the host
        //  hal_SysRequestFreq(HAL_SYS_FREQ_TRACE, HAL_SYS_FREQ_26M, NULL);
        // hal_SetResourceActivityPrivate(HAL_LP_R_UART0, TRUE);

        // Uart already open
        HAL_ASSERT(!((hwp_debug_uart->ctrl) & DEBUG_UART_ENABLE),
                   "Host trace already open");

        hwp_debug_uart->triggers = DEBUG_UART_RX_TRIGGER(7)
            | DEBUG_UART_TX_TRIGGER(12)
            | DEBUG_UART_AFC_LEVEL(10);

        // Enable IRQs:
        // It is only enabled in the IRQ module 
        // as a wake-up IRQ, and not handled otherwise
        // Enable Rx level IRQ: when in idle, if data are available, we 
        // fetch them
        // Same for Rx Timeout.
        // Tx Dma done IRQ: when a bunch of data has been send, wake up
        // to send even more...
        hwp_debug_uart->irq_mask = DEBUG_UART_RX_DATA_AVAILABLE 
                                | DEBUG_UART_RX_TIMEOUT
                                | DEBUG_UART_TX_DMA_DONE;

// FIXME configure rationaly the remaining fields
// By default use host hard coded config, but !
#if (CHIP_HAS_DBG_UART_SW_FLOW_CONTROL == 1)
        hwp_debug_uart->ctrl     = DEBUG_UART_ENABLE
                        | DEBUG_UART_SWRX_FLOW_CTRL(1)
                        | DEBUG_UART_SWTX_FLOW_CTRL(1)
                        | DEBUG_UART_BACKSLASH_EN
                        | DEBUG_UART_DMA_MODE ;
#else
        hwp_debug_uart->ctrl     = DEBUG_UART_ENABLE | DEBUG_UART_DMA_MODE | DEBUG_UART_DATA_BITS_8_BITS | DEBUG_UART_AUTO_FLOW_CONTROL_ENABLE;
#endif
        while (hwp_debug_uart->status & DEBUG_UART_ENABLE_N_FINISHED);

        // Done by the get and release channel
        // g_hstTraceTxIfcCh = HAL_UNKNOWN_CHANNEL;
        g_remainingBytes       = 0;
        g_previousStartAddress = (UINT8*) 0xffffffff;
        g_halHstTraceEnabled   = FALSE;
        break;

    case HAL_HST_TRACE_PORT_USB:
        g_halTraceUsbReadIndex  = 0;
        g_halTraceUsbWriteIndex = 0;
        g_remainingBytesReturn  = 0;
        break;
    }
}



// =======================================================
// hal_HstTraceClose
// -------------------------------------------------------
// Close the host trace port
// @param portIdx Not taken into account
// =======================================================
PUBLIC VOID hal_HstTraceClose(UINT8 portIdx)
{
    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
        //  Turn off the Uart.
        hwp_debug_uart->ctrl = DEBUG_UART_RX_FIFO_RESET
            | DEBUG_UART_TX_FIFO_RESET ;
        //  Disable the Tx IFC Channel.
        if (g_hstTraceTxIfcCh != HAL_UNKNOWN_CHANNEL)
        {
            hal_IfcChannelRelease(HAL_IFC_DEBUG_UART_TX, g_hstTraceTxIfcCh);
        }

        g_hstTraceTxIfcCh = HAL_UNKNOWN_CHANNEL;
        g_remainingBytes  = 0;

        //  Release resource
        // hal_SetResourceActivityPrivate(HAL_LP_R_UART0, FALSE);
        //    hal_SysRequestFreq(HAL_SYS_FREQ_TRACE, HAL_SYS_FREQ_32K, NULL);
        break;

    case HAL_HST_TRACE_PORT_USB:
        break;
    }
}


// =============================================================================
// hal_HstTraceEnable
// -----------------------------------------------------------------------------
/// This fonction enables the actual sending of traces.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// @param portIdx Not taken into account.
/// @return TRUE if trace sending could be enabled, FALSE otherwise.
///         Also TRUE if host clock is disabled to ensure we can go to low power.
// =============================================================================
PUBLIC BOOL hal_HstTraceEnable(VOID)
{
    UINT8 portIdx;
    UINT8 channel;

    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
        if (g_halHstTraceEnabled)
        {
            return TRUE;
        }
        
        channel = hal_IfcGetChannel(HAL_IFC_DEBUG_UART_TX, HAL_IFC_SIZE_8_MODE_MANUAL);

        if (channel == HAL_UNKNOWN_CHANNEL)
        {
            return FALSE;
        }

        hal_SysRequestFreq(HAL_SYS_FREQ_TRACE, HAL_SYS_FREQ_26M, NULL);
        g_hstTraceTxIfcCh    = channel;
        g_halHstTraceEnabled = TRUE;
        return TRUE;

    case HAL_HST_TRACE_PORT_USB:
        return TRUE;
    }

    return FALSE;
}


// =============================================================================
// hal_HstTraceDisable
// -----------------------------------------------------------------------------
/// This function disables the sending of traces.
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
/// @param portIdx Not taken into account.
// =============================================================================
PUBLIC VOID hal_HstTraceDisable(VOID)
{
    UINT8 portIdx;
    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
        if (!g_halHstTraceEnabled)
        {
            return;
        }
        hwp_debug_uart->irq_cause = DEBUG_UART_TX_DMA_DONE;
        while(!hal_IfcChannelIsFifoEmpty(HAL_IFC_DEBUG_UART_TX, g_hstTraceTxIfcCh));
        hal_TimDelay(100);
        hal_IfcChannelRelease(HAL_IFC_DEBUG_UART_TX, g_hstTraceTxIfcCh);
        g_hstTraceTxIfcCh    = HAL_UNKNOWN_CHANNEL;
        g_halHstTraceEnabled = FALSE;
        // To force to reload the start address at the next call to hal_HstTraceGetChannel,
        // in case the channel is different
        hal_SysRequestFreq(HAL_SYS_FREQ_TRACE, HAL_SYS_FREQ_32K, NULL);
        g_previousStartAddress = (UINT8*) 0xffffffff;
        break;

    case HAL_HST_TRACE_PORT_USB:
        break;
    }
}


// =======================================================
// hal_HstTraceSendData
// -------------------------------------------------------
/// Write data on the host trace port.
/// @param portIdx Not taken into account.
/// @param startAddress Pointer on location to get data to be sent.
/// @param length Number of bytes to be sent.
/// @return Number of bytes taken from the location.
// =======================================================
PUBLIC UINT32 hal_HstTraceSendData(UINT8 portIdx,
                                   CONST UINT8* startAddress, UINT32 length)
{
    UINT32 extension;
    UINT32 currentTc;
    UINT32 sentBytes;

    // Data write is only done on DMA polling
    //HAL_PROFILE_FUNCTION_ENTER(hal_HstTraceSendData);

    //  Check the address pointer.
    HAL_ASSERT((startAddress != NULL), "Unitialized addr:%#x", (UINT32) startAddress);

    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
       //  Check if the uart is open
        HAL_ASSERT(hwp_debug_uart->ctrl & DEBUG_UART_ENABLE,
                   "Host trace not Open");

        //  Check the transfer size
        HAL_ASSERT(length < EXP2(SYS_IFC_TC_LEN),
                   "Transfer size too big: %d", length);

        hwp_debug_uart->irq_cause = DEBUG_UART_TX_DMA_DONE;

        if (g_halHstTraceEnabled)
        {
            if (startAddress < g_previousStartAddress)
            {
                // We have wrapped. Meaning that the previous transfer was finish and
                // tc is 0. This is checked in the called function hal_IfcSetStartAddress
                hal_IfcSetStartAddress(g_hstTraceTxIfcCh, startAddress);
                // Extending transfer with tc = 0 is like setting it.
                hal_IfcExtendTransfer(g_hstTraceTxIfcCh, length);
                g_previousStartAddress = startAddress;
                // We start to send the full length
                g_remainingBytes       = length;
                // No bytes transfered
                return 0;
            }
            else
            {
                // Previous 'DMA still in progress',
                // ...
                // In fact, since the ifc channel is opened definitively
                // with no auto-disable, we are always carrying on a dma
                // transfer, thus the appending as explained later...
                //
                // The only trick is when the buffer wrap, at the upper (functional)
                // level: we have to restart the transfert at the beginning
                // (cf if statement above)
                //
                // We append data and return what has been sent since last call
                // to this function
                // We need an transfer extended of the new length minus what remained to be
                // transfered previously (the start address of a 'following' transfer being
                // the one of the previous transfer added with the amount of transfered bytes
                // A picture:
                // addr1:----tc----------------->length1
                //       addr2:---------------------------->length2
                // What is to be added is length2-(what remained at time when tc is measured)
                // tc is g_remainingBytes ... Without saturating Tc
                g_previousStartAddress = startAddress;

                currentTc = hal_IfcGetTc(HAL_IFC_DEBUG_UART_TX, g_hstTraceTxIfcCh);

                sentBytes = g_remainingBytes - currentTc;

                // Number of bytes to add to the transfer.
                if (EXP2(SYS_IFC_TC_LEN) - currentTc > length - g_remainingBytes)
                {
                    // We can add everything
                    extension = length - g_remainingBytes;
                }
                else
                {
                    // We only send what is available
                    extension = EXP2(SYS_IFC_TC_LEN) - currentTc;
                }

                hal_IfcExtendTransfer(g_hstTraceTxIfcCh, extension);

                g_remainingBytes = currentTc + extension;
                return sentBytes;
            }
        }
        else
        {
            return 0;
        }
        break;
    case HAL_HST_TRACE_PORT_USB:
#if (CHIP_HAS_USB == 1)

/*         boot_HostUsbSend(); */
/*         { */
            length    &= ~(0x03);
            sentBytes  = 0;
            if(length == 0)
            {
                return 0;
            }

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
            UINT32 chipId = rfd_XcvGetChipId();
            if (chipId == XCV_CHIP_ID_8808_V2 || chipId == XCV_CHIP_ID_8808_V3)
            {
                return length;
            }
#endif

            if(hal_UsbEpTransfertDone(HAL_USB_EP_DIRECTION_IN(1)) == TRUE &&
               g_remainingBytesReturn != 0)
            {
                sentBytes               = g_remainingBytesReturn;
                g_remainingBytesReturn  = 0;
            }
            else
            {
                if(hal_UsbSend(1, (UINT8*)startAddress, length, 0) != -1)
                {
                    g_remainingBytesReturn = length;
                }
            }
            return sentBytes;
/*         } */
#endif /* CHIP_HAS_USB */
            break;
    }
    return 0;
}


// =======================================================
// hal_HstTraceGetData
// -------------------------------------------------------
/// Read data from the host trace port.
///
/// In order to read data received from the tool, the debug uart
/// IRQ is used to wake up the system from sleep and thus 
/// handle received data in idle.
/// The debug uart IRQ is not used to process data in interrupt
/// mode, thus the IRQ Mask in the IRQ module is disabled 
/// explicitely for the debug uart. Only the wake-up mask
/// is set for this interrupt.
/// This is necessary in the following case: the PC Trace Tool
/// sends a large configuration packet while the chip is in sleep mode,
/// so the data from the fifo of the Trace UART won't be read, the fifo
/// gets saturated. The Host Port sets its RTS line to high. The PC
/// cannot send commands through the Host Port anymore. We are locked outside.
///
/// @param portIdx  Not taken into account.
/// @param destAddress Pointer on location to put the received data.
/// @param length Number max of data to write at the location
/// @return Number of bytes written at the location.
// =======================================================
PUBLIC UINT32 hal_HstTraceGetData(UINT8 portIdx, UINT8 *destAddress, UINT32 length)
{
    // Data read is only done by direct polling
    //HAL_PROFILE_FUNCTION_ENTER(hal_HstTraceGetData);

    UINT32 i;
    UINT32 nbAvailable = 0;
    UINT32 copySize;
    UINT32 copySizeRemainder;

    portIdx = g_HalTracePort;

    switch(portIdx)
    {
    case HAL_HST_TRACE_PORT_HOST:
        // Serial Host Incoming Data Fetch.
        //  Check the address pointer.
        HAL_ASSERT((destAddress != NULL), "Uninitialized dest addr: %#x",
                   (UINT32) destAddress);

        //  Check if the uart is open
        HAL_ASSERT(hwp_debug_uart->ctrl & DEBUG_UART_ENABLE,
                   "Host trace has not been opened");

        nbAvailable = GET_BITFIELD(hwp_debug_uart->status,
                                   DEBUG_UART_RX_FIFO_LEVEL) ;

        if (nbAvailable > length)
        {
            nbAvailable = length;
        }

        //  Get data byte by byte.
        for (i = 0; i < nbAvailable; i++)
        {
            destAddress[i] = hwp_debug_uart->rxtx_buffer;
        }
        break;

    case HAL_HST_TRACE_PORT_USB:
        // USB Host Incoming Data Fetch.
        
        nbAvailable = (g_halTraceUsbWriteIndex - g_halTraceUsbReadIndex)%sizeof(g_halTraceUsbBuffer);

        // We are asked to provide up to length bytes.
        if (nbAvailable > length)
        {
            nbAvailable = length;
        }

        // Handle buffer wrap.
        if (sizeof(g_halTraceUsbBuffer)-g_halTraceUsbReadIndex < nbAvailable)
        {
            // Wrap needed.
            copySize = sizeof(g_halTraceUsbBuffer) - g_halTraceUsbReadIndex;
        }
        else
        {
            // We can take everything in one shot.
            copySize = nbAvailable;
        }

        memcpy(destAddress, &g_halTraceUsbBuffer[g_halTraceUsbReadIndex], copySize);

        g_halTraceUsbReadIndex += copySize;

        // Wrap, only useful when copySize = sizeof(g_halTraceUsbBuffer) - g_halTraceUsbReadIndex;
        if (g_halTraceUsbReadIndex >= sizeof(g_halTraceUsbBuffer))
        {
            g_halTraceUsbReadIndex = 0;
        }

        // Take care of the remainder.
        copySizeRemainder = nbAvailable - copySize;
        if (copySizeRemainder != 0)
        {
            memcpy(&destAddress[copySize], g_halTraceUsbBuffer, copySizeRemainder);
            g_halTraceUsbReadIndex += copySizeRemainder;
        }
        break;
    }

    //HAL_PROFILE_FUNCTION_EXIT(hal_HstTraceGetData);
    return nbAvailable;
}




// =============================================================================
// hal_TraceUsbCallback
// -----------------------------------------------------------------------------
/// @todo Lilian has to add a comment here!
// =============================================================================
PUBLIC VOID hal_TraceUsbCallback(VOID* pBuffer, UINT16 size)
{
    UINT16 copySize;
    UINT8* buffer = (UINT8*) pBuffer;

    // Ensure we don't write more data than the available space.
    // (Initially, both pointers are at 0).
    HAL_ASSERT(((g_halTraceUsbReadIndex == g_halTraceUsbWriteIndex)
        ||  ((g_halTraceUsbReadIndex - g_halTraceUsbWriteIndex) %
        sizeof(g_halTraceUsbBuffer)) >= size),
        "hal_TraceUsbCallback fed with too much data: %d, r:%d, w:%d",
        size, g_halTraceUsbReadIndex, g_halTraceUsbWriteIndex);

    if(sizeof(g_halTraceUsbBuffer) - g_halTraceUsbWriteIndex > size)
    {
        copySize = size;
    }
    else
    {
        copySize = sizeof(g_halTraceUsbBuffer) - g_halTraceUsbWriteIndex;
    }

    memcpy(&g_halTraceUsbBuffer[g_halTraceUsbWriteIndex], buffer, copySize);
    size -= copySize;
    g_halTraceUsbWriteIndex    += copySize;

    // Wrap, only useful when copySize = sizeof(g_halTraceUsbBuffer) - g_halTraceUsbWriteIndex;
    if (g_halTraceUsbWriteIndex >= sizeof(g_halTraceUsbBuffer))
    {
        g_halTraceUsbWriteIndex = 0;
    }

    // We wrapped and start again from the g_halTraceUsbBuffer
    // beginning.
    if(size != 0)
    {
        memcpy(g_halTraceUsbBuffer, &buffer[copySize], size);
        g_halTraceUsbWriteIndex = size;
    }
}


