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
/// @file                                                                     //
/// That file Implements the functions to register the module.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "rf_spi.h"
#include "sys_ifc.h"
#include "timer.h"
#include "debug_host.h"

#include "hal_rfspi.h"
#include "hal_sys.h"
#include "hal_timers.h"

#include "drv_xcv.h"


#define XCV_INIT_ASSERT(cond, ...) \
    do { if (UNLIKELY(!(cond))) asm volatile("break 2"); } while (0)


// ================================================================================
// TIMER
// ================================================================================

PRIVATE VOID xcvInitTimDelay(UINT32 delay)
{
    UINT32 initialValue;

    initialValue = hwp_timer->hwtimer_curval;

    // Wait
    while ((hwp_timer->hwtimer_curval - initialValue) < delay);

}

// ================================================================================
// HOST
// ================================================================================

PRIVATE BOOL POSSIBLY_UNUSED xcvInitHstSendEvent(UINT32 ch)
{
    UINT32 startTime = hwp_timer->hwtimer_curval;
    BOOL   timeOut   = FALSE;

    while ((hwp_debugHost->event & 1) == 0 && !timeOut)
    {
        if ((hwp_timer->hwtimer_curval - startTime) >= (2 MS_WAITING))
        {
            timeOut = TRUE;
        }
    }

    //  Write the byte in the host event register if possible
    if (!timeOut)
    {
        hwp_debugHost->event = ch;
        xcvInitTimDelay(2 MS_WAITING);
    }

    return timeOut;
}

// ================================================================================
// SYS
// ================================================================================

PRIVATE VOID xcvInitSysSwitchTo26M(BOOL enable)
{
#if 0
    xcvInitTimDelay(10 MS_WAITING);

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    if (enable)
    {
        hwp_sysCtrl->Sel_Clock |= SYS_CTRL_SYS_SEL_FAST_SLOW;
    }
    else
    {
        hwp_sysCtrl->Sel_Clock &= ~SYS_CTRL_SYS_SEL_FAST_SLOW;
    }

    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    xcvInitTimDelay(10 MS_WAITING);
#endif
}

// ================================================================================
// RFSPI
// ================================================================================

// Macro to get remaining space in cmd fifo
#define CMD_FIFO_SPACE  (CMD_FIFO_LEN - \
                        ((hwp_rfSpi->Status & RF_SPI_CMD_LEVEL_MASK) \
                         >>RF_SPI_CMD_LEVEL_SHIFT))
#define DATA_FIFO_SPACE (CMD_DATA_FIFO_LEN - \
                        ((hwp_rfSpi->Status & RF_SPI_CMD_DATA_LEVEL_MASK) \
                         >>RF_SPI_CMD_DATA_LEVEL_SHIFT))
#define IFC_BUFFER_SPACE (RFSPI_SW_FIFO_SIZE - hwp_sysIfc->ch_rfspi_tc)

PRIVATE HAL_RFSPI_CMD_QUEUE_T g_spiCmdQueue;

PRIVATE HAL_RFSPI_SW_FIFO_T g_spiDataFifo;

PRIVATE UINT32 g_spiDivider;

PRIVATE VOID xcvInitRfspiSetDivider(VOID)
{
    g_spiDivider = hwp_rfSpi->Divider;
    hwp_rfSpi->Divider = RF_SPI_DIVIDER(0);
}

PRIVATE VOID xcvInitRfspiRestoreDivider(VOID)
{
    hwp_rfSpi->Divider = g_spiDivider;
}

PRIVATE VOID xcvInitRfspiPushData(HAL_RFSPI_CMD_T* cmd, CONST UINT8* data, UINT8 byteQty)
{
    XCV_INIT_ASSERT((g_spiCmdQueue.dataIdx+byteQty <= RFSPI_DATA_BUF_SIZE),
                  "CmdQueue Data Overflow, with index=%d",
                  g_spiCmdQueue.dataIdx);
    XCV_INIT_ASSERT((cmd->byteQty+byteQty <= 255),
                "Spi command too large, size=%d", cmd->byteQty+byteQty);

    UINT32 i;
    for (i=0; i<byteQty; i++)
    {
        cmd->data[cmd->byteQty+i] = data[i];
    }
    cmd->byteQty += byteQty;
    g_spiCmdQueue.dataIdx += byteQty;
}

PRIVATE HAL_RFSPI_CMD_T * xcvInitRfspiNewSingleCmd(VOID)
{
    HAL_RFSPI_CMD_T* new = (HAL_RFSPI_CMD_T*)&(g_spiCmdQueue.cmdBuf[0]);

    // Init the structure, directlly use the SW fifo as only one cmd
    // will be programmed and sent
    new->data = (UINT8 *)&(g_spiDataFifo.data[0]);
    new->byteQty = 0;
    new->date = 0;
    new->winIdx = 0;

    return (new);
}

PRIVATE VOID xcvInitRfspiInitQueue(VOID)
{
    // Reset command Queue
    g_spiCmdQueue.firstCmd = NULL;
    g_spiCmdQueue.cmdIdx = 0;
    g_spiCmdQueue.dataIdx = 0;
}

PRIVATE VOID xcvInitRfspiSetupFifo(VOID)
{
    // Reset SW Fifo
    g_spiDataFifo.curPos = 0;
    g_spiDataFifo.addedData = 0;

    // Reset HW
    hwp_sysIfc->ch_rfspi_control = SYS_IFC_DISABLE;
    hwp_sysIfc->ch_rfspi_tc = 0;
    hwp_rfSpi->Command = RF_SPI_FLUSH_CMD_FIFO;

    // Setup IFC Data SW Fifo
    hwp_sysIfc->ch_rfspi_start_addr = (UINT32) &(g_spiDataFifo.data[0]);
    hwp_sysIfc->ch_rfspi_end_addr = (UINT32) &(g_spiDataFifo.data[RFSPI_SW_FIFO_SIZE]);
    hwp_sysIfc->ch_rfspi_control = SYS_IFC_ENABLE;
}

PRIVATE VOID xcvInitRfspiGetStatus(HAL_RFSPI_STATUS_T* status, BOOL clear)
{
    UINT32 hwStatus = hwp_rfSpi->Status;
    status->active = (hwStatus & RF_SPI_ACTIVE_STATUS)?1:0;
    status->errCmd =  (hwStatus & RF_SPI_ERROR_CMD)?1:0;
    status->tableOvf = (hwStatus & RF_SPI_TABLE_OVF)?1:0;
    status->tableUdf = (hwStatus & RF_SPI_TABLE_UDF)?1:0;
    status->cmdOvf = (hwStatus & RF_SPI_CMD_OVF)?1:0;
    status->cmdUdf = (hwStatus & RF_SPI_CMD_UDF)?1:0;
    status->dataOvf = (hwStatus & RF_SPI_CMD_DATA_OVF)?1:0;
    status->dataUdf = (hwStatus & RF_SPI_CMD_DATA_UDF)?1:0;
    status->rxOvf = (hwStatus & RF_SPI_RX_OVF)?1:0;
    status->rxUdf = (hwStatus & RF_SPI_RX_UDF)?1:0;
    status->cmdLevel = GET_BITFIELD(hwStatus, RF_SPI_CMD_LEVEL);
    status->dataLevel = GET_BITFIELD(hwStatus, RF_SPI_CMD_DATA_LEVEL);
    status->rxLevel = GET_BITFIELD(hwStatus, RF_SPI_RX_LEVEL);

    if (clear)
    {
        //  Reset 
        hwp_rfSpi->Status = RF_SPI_ERROR_CMD | RF_SPI_TABLE_OVF
        | RF_SPI_TABLE_UDF | RF_SPI_CMD_OVF | RF_SPI_CMD_UDF
        | RF_SPI_CMD_DATA_OVF | RF_SPI_CMD_DATA_UDF | RF_SPI_RX_OVF
        | RF_SPI_RX_UDF;
    }
}

// =============================================================================
// xcvInitRfspiImmReadWriteAvail
// -----------------------------------------------------------------------------
/// Check if RFSPI is ready for immediate read or write operation.
/// @return TRUE if ready.
// =============================================================================
PRIVATE  BOOL xcvInitRfspiImmReadWriteAvail()
{
    if (hwp_sysIfc->ch_rfspi_tc != 0 ||
            (hwp_sysIfc->ch_rfspi_status & SYS_IFC_FIFO_EMPTY) == 0)
    {
        return FALSE;
    }

    HAL_RFSPI_STATUS_T status;
    xcvInitRfspiGetStatus(&status, FALSE);
    if (status.active != 0 || status.cmdLevel != 0 ||
            status.dataLevel != 0 || status.rxLevel != 0)
    {
        return FALSE;
    }

    return TRUE;
}

PRIVATE VOID xcvInitRfspiSendCmd(HAL_RFSPI_CMD_T* spiCmd)
{
    // Directly fill the SW Fifo, the size limit comes from the HW command fifo
    // that is only 8 bits wide -> 255 is max
    BOOL wait = TRUE;

    // Reset H
    // Disable the IFC before flushing. (Medow)
    hwp_sysIfc->ch_rfspi_control = SYS_IFC_DISABLE;
    hwp_sysIfc->ch_rfspi_tc = 0;
    hwp_rfSpi->Command = RF_SPI_FLUSH_CMD_FIFO;

    // Setup IFC Xfer
    hwp_sysIfc->ch_rfspi_start_addr = (UINT32) &(g_spiDataFifo.data[0]);
    hwp_sysIfc->ch_rfspi_end_addr = (UINT32) &(g_spiDataFifo.data[RFSPI_SW_FIFO_SIZE]);
    hwp_sysIfc->ch_rfspi_control = SYS_IFC_ENABLE;
    hwp_sysIfc->ch_rfspi_tc = spiCmd->byteQty;

    // And send the command
    hwp_rfSpi->Cmd_Size = spiCmd->byteQty;
    // Command sending is deported after the writing of the reset value of spiCmd->byteQty
    // so that there is something else than APB access on the bus. This way, the
    // IFC will have time to feed some data to the RF-SPI and thing will work way
    // better.
    // hwp_rfSpi->Command = RF_SPI_SEND_CMD;

    // Wait for the TC to have started going down, ie 
    // the RFSPI has filled the Fifo.
    while (hwp_sysIfc->ch_rfspi_tc == spiCmd->byteQty);

    // Clear byteQty to "free" the command
    spiCmd->byteQty = 0;

    // End of send command.
    hwp_rfSpi->Command = RF_SPI_SEND_CMD;

    // Wait for the command to be sent
    while(wait)
    {
        HAL_RFSPI_STATUS_T status;
        xcvInitRfspiGetStatus(&status, FALSE);
        wait=((status.dataLevel==0) && (status.active == 0))?FALSE:TRUE;
    }

    // Re-init the queue and the SW fifo mechanism
    xcvInitRfspiInitQueue();
    xcvInitRfspiSetupFifo();
}


// ================================================================================
// XCV
// ================================================================================

CONST PRIVATE UINT16 gc_xcvMicrocodes[] =
{
    #include "xcv_microcodes.tab"
};

// -------------------------------------------------------------------//
//                      Macros for SPI timings                        //
// -------------------------------------------------------------------//
#define RFSPI_CLOCK HAL_RFSPI_13M_CLK
#define CMD_DUR_FACTOR 1300

// 1 Qb = 12/13 us
// 1 Cmd = 35 clocks
//       = (3500/CMD_DUR_FACTOR) / (12/13) Qb
#define CMD_DURATION(n) (((n) * 3500 * 13) / ((CMD_DUR_FACTOR) * 12) + 1)

#define XCV_SEND_CMD_MAX_NUM                20

//--------------------------------------------------------------------//
//                                                                    //
//                RF transceiver Serial link management               //
//                                                                    //
//--------------------------------------------------------------------//

// Generates commands according to the Xcver SPI serial format.
//
// Rda6220 RF Transceiver:
//  1 write command is:
//  1 bit equal to 0 for write + 18 bits for data + 6 bits for address = 25 bits
//           0AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1     Byte 2      Byte 3  LSB
//      
//  1 read command is:
//  1 bit equal to 1 for read + 6 bits for address + 2 bits for clk turnaround + 18 bits for data = 27 bits
//           1AAAAAAD | DDDDDDDD | DDDDDDDD | D0000000
//      MSB    Byte0     Byte 1       Byte 2     Byte 3  LSB

PRIVATE VOID xcvInitBuildSpiCmd (HAL_RFSPI_CMD_T *cmd, UINT8 address, UINT32 data)
{
    UINT8 array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord = RDA6220_WRITE_FMT(address, data);

    // Hardcoded
    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;

    xcvInitRfspiPushData(cmd,array,RDA6220_ONE_CMD_BYTE_QTY);
}

// =============================================================================
// xcvInitRfspiDigRfRead
// -----------------------------------------------------------------------------
/// Performs an SPI Read access inDigRf Mode
/// @param Cmd Formatted read command (read bit, address and dummy data)
/// @param dataOut Buffer where the read data are stored.
// =============================================================================
PRIVATE VOID xcvInitRfspiDigRfRead(UINT8 *Cmd,UINT8* dataOut)
{
    UINT32 loop;
    UINT32 savedCfg = hwp_rfSpi->Ctrl; // save the original configuration
    UINT32 readCfg = savedCfg & ~(RF_SPI_FRAME_SIZE(0x1F));
    UINT8  FrameSize = ((RF_SPI_FRAME_SIZE_MASK & savedCfg) >>
            RF_SPI_FRAME_SIZE_SHIFT)+1;
    UINT8  InputSize = ((RF_SPI_INPUT_FRAME_SIZE_MASK & savedCfg) >>
            RF_SPI_INPUT_FRAME_SIZE_SHIFT)+1;
    UINT8  WriteSize = FrameSize-InputSize;
    UINT8  remBits = (InputSize%8);
    UINT8  CmdSize = !(WriteSize%8)?(WriteSize/8):(WriteSize/8 + 1);
    UINT8  RdSize = !(remBits)?(InputSize/8):(InputSize/8 + 1);
    UINT8  tmpOut[4] = {0,0,0,0};
    UINT32* tmpOut_p32 = (UINT32*)tmpOut;

    // Change the SPI mode to input and change the frame size
    readCfg |= RF_SPI_FRAME_SIZE(FrameSize-InputSize-1);
    readCfg |= RF_SPI_INPUT_MODE;
    hwp_rfSpi->Ctrl = readCfg;

    //HAL_ASSERT(CmdSize<CMD_DATA_FIFO_LEN, "RF SPI command size too large");

    // Fill the Tx fifo
    hwp_rfSpi->Command = RF_SPI_FLUSH_CMD_FIFO | RF_SPI_FLUSH_RX_FIFO;
    for (loop=0; loop<CmdSize; loop++)
    {
        hwp_rfSpi->Cmd_Data = Cmd[loop];
    }
    // Set the readout size
    hwp_rfSpi->Cmd_Size = RF_SPI_CMD_SIZE(CmdSize);

    // And send the command
    hwp_rfSpi->Command = RF_SPI_SEND_CMD;

    // Wait for the SPI to finish
    while((hwp_rfSpi->Status & RF_SPI_ACTIVE_STATUS) != 0);

    // Get the results
    for (loop=RdSize; loop>0; loop--)
    {
        // Wait until at least one byte is ready, otherwise Rx FIFO will be underflow
        while(GET_BITFIELD(hwp_rfSpi->Status, RF_SPI_RX_LEVEL) == 0);
        tmpOut[loop-1] = (UINT8)hwp_rfSpi->Rx_Data;
    }

    // Handle non-multiple of byte data readouts
    if (remBits)
    {
        // last byte is screwed because of missing shifts
        tmpOut[0] = tmpOut[0] << (8-remBits);
        // repack
        tmpOut_p32[0] = tmpOut_p32[0] >> (8-remBits);
    }

    // Copy to byte output
    for (loop=0; loop<RdSize; loop++)
    {
        dataOut[loop] = tmpOut[loop];
    }

    // Change the SPI mode back to output
    hwp_rfSpi->Ctrl = savedCfg;
}

PRIVATE VOID xcvInitxcvReadReg (UINT16 address, UINT8 *output)
{
    UINT8  array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord;

    cmdWord = RDA6220_READ_FMT(address, 0);

    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;
    xcvInitRfspiDigRfRead(array,output);
}

PUBLIC UINT32 xcvInitDownloadCodes(VOID)
{
    UINT32 i;
    INT8 loop = 2;
    UINT16 regdata;
    BOOL m_8051dlok = FALSE;
    UINT32 m_xcv8051dltimes = 0; // 0-7bit for download times, the 8 bit for down ok or fail flag, 1 stand for ok and 0 is error

    xcvInitRfspiInitQueue();
    xcvInitRfspiSetupFifo();

    xcvInitSysSwitchTo26M(TRUE);

    xcvInitRfspiSetDivider();

    HAL_RFSPI_CMD_T* confCmd = xcvInitRfspiNewSingleCmd();

    do
    {
        m_xcv8051dltimes++; 

        // Read when 8051 is running on 26M
        xcvInitBuildSpiCmd (confCmd, 0x30, 0x1183);
   //     xcvInitBuildSpiCmd (confCmd, 0xba, 0x0700); // 8051 26MHz 
        xcvInitBuildSpiCmd (confCmd, 0x30, 0x0301); // DL=1, rst=1
        xcvInitRfspiSendCmd(confCmd);
        xcvInitTimDelay(1 MS_WAITING);

        // Download XCV microcodes
        for (i = 0; i < ARRAY_SIZE(gc_xcvMicrocodes); i++)
        {
            xcvInitBuildSpiCmd (confCmd, 0xFF, gc_xcvMicrocodes[i]);
            if (i % XCV_SEND_CMD_MAX_NUM == (XCV_SEND_CMD_MAX_NUM - 1))
            {
                xcvInitRfspiSendCmd(confCmd);
            }
        }
        if (i % XCV_SEND_CMD_MAX_NUM != 0)
        {
            xcvInitRfspiSendCmd(confCmd);
        }

        xcvInitBuildSpiCmd (confCmd, 0x30, 0x0101); // DL=0, rst=1
        xcvInitRfspiSendCmd(confCmd);
        xcvInitTimDelay(1 MS_WAITING);

        // Read when 8051 is running on 156M
        xcvInitBuildSpiCmd (confCmd, 0x30, 0x1183);
    //    xcvInitBuildSpiCmd (confCmd, 0xba, 0x1f00); // 8051  156MHz
        xcvInitBuildSpiCmd (confCmd, 0x30, 0x0301); // DL=1, rst=1
        xcvInitRfspiSendCmd(confCmd);

                xcvInitHstSendEvent(0x11111111);
                xcvInitHstSendEvent(ARRAY_SIZE(gc_xcvMicrocodes));
        for (i = 0; i < ARRAY_SIZE(gc_xcvMicrocodes); i++)
        {
            while (!xcvInitRfspiImmReadWriteAvail());
            xcvInitTimDelay(2);

            xcvInitxcvReadReg(0xff, (UINT8 *)&regdata);
            if (regdata == gc_xcvMicrocodes[i])
            { 
                if(i == 50)
                { 
                xcvInitHstSendEvent(0x12121212);
                xcvInitHstSendEvent(regdata);
                xcvInitHstSendEvent(gc_xcvMicrocodes[i]);
                }
if(i == 51)
                { 
                xcvInitHstSendEvent(0x12121212);
                xcvInitHstSendEvent(regdata);
                xcvInitHstSendEvent(gc_xcvMicrocodes[i]);
                }

                m_8051dlok = TRUE;
                continue;
            }
            else
            {
                xcvInitHstSendEvent(0xe0220000|i);
                xcvInitHstSendEvent((regdata<<16)|gc_xcvMicrocodes[i]);
                m_8051dlok = FALSE;
                break;
            }
        }
        xcvInitTimDelay(1 MS_WAITING);

        xcvInitBuildSpiCmd (confCmd, 0x30, 0x0101); // DL=0, rst=1
        xcvInitRfspiSendCmd(confCmd);
        xcvInitTimDelay(1 MS_WAITING);

        if (m_8051dlok)
        { 
            m_xcv8051dltimes |= 0x80;
            break;
        }
        loop --;
    } while (loop > 0);

    xcvInitTimDelay(1 MS_WAITING);
    xcvInitBuildSpiCmd (confCmd, 0x30, 0x0181); // DL=0, rst=1
    xcvInitRfspiSendCmd(confCmd);

    xcvInitRfspiRestoreDivider();

    xcvInitSysSwitchTo26M(FALSE);
    xcvInitHstSendEvent(0xd1d51000|m_xcv8051dltimes);

    return m_xcv8051dltimes;
}

