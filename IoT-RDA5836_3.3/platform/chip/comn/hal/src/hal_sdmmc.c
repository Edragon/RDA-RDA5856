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
/// @file hal_sdmmc.c                                                           //
/// SDMMC driver implementation
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 


#include "cs_types.h"
#include "chip_id.h"

#if (CHIP_HAS_SDMMC == 1)

#include "global_macros.h"
#include "sdmmc.h"
#include "sys_ctrl.h"
#include "cfg_regs.h"

#include "hal_sdmmc.h"
#include "halp_sys_ifc.h"
#include "halp_sys.h"
#include "hal_host.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "pmd_m.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
#include "base_address.h"
#endif
#include "halp_debug.h"
#include "gpio.h"
#include "gpio_edrv.h"

// =============================================================================
//  Global variables
// =============================================================================


UINT8 g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;
UINT8 g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;


/// SDMMC clock frequency.
PRIVATE UINT32 g_halSdmmcFreq = 200000;
extern PUBLIC VOID gpio_ConvSdFunction(void);
extern PUBLIC VOID gpio_ConvDetFunction(void);
//=============================================================================
// hal_SdmmcUpdateDivider
//-----------------------------------------------------------------------------
/// Update the SDMMC module divider to keep the desired frequency, whatever
/// the system frequency is.
/// @param sysFreq System Frequency currently applied or that will be applied
/// if faster.
//=============================================================================
PRIVATE VOID hal_SdmmcUpdateDivider(HAL_SYS_FREQ_T sysFreq);


//=============================================================================
// hal_SdmmcOpen
//-----------------------------------------------------------------------------
/// Open the SDMMC module. Take a resource.
//=============================================================================
PUBLIC VOID hal_SdmmcOpen(UINT8 clk_adj)
{
#if (CHIP_ASIC_ID <= CHIP_ASIC_ID_5856)
    UINT32 value ;
#endif
    #if defined(__CT_CHIP_PKG_RDA5856T__) || defined(__CT_CHIP_PKG_RDA5856L__) || defined(__CT_CHIP_PKG_RDA5856Q32__) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    {
        gpio_ConvSdFunction();
    }
    #endif
    
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E)
    // Take the module out of reset
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
#endif

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
    hwp_sysCtrl->per_soft_reset2 |= SYS_CTRL_SDMMC_RSTB_UNRESET;
#else
    hwp_sysCtrl->Sys_Rst_Clr = SYS_CTRL_CLR_RST_SDMMC;
    hwp_sysCtrl->Clk_Per_Enable = SYS_CTRL_ENABLE_PER_SDMMC;
#endif
   // hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
   
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    value = hwp_sysCtrl->Clk_Per_Mode;
    value = value&(~SYS_CTRL_MODE_PER_SDMMC_MANUAL);
    hwp_sysCtrl->Clk_Per_Mode = value;
    hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_LOCK;
#endif
    // We don't use interrupts.
    hwp_sdmmc->sd_int_mask = 0x0;
    hwp_sdmmc->sd_mclk_adjust = clk_adj;
    // Take a resource (The idea is to be able to get a 25Mhz clock)
    hal_SysRequestFreq(HAL_SYS_FREQ_SDMMC, HAL_SYS_FREQ_52M, hal_SdmmcUpdateDivider);
    UINT32 scStatus = hal_SysEnterCriticalSection();
    // Make sure the last clock is set
    hal_SdmmcUpdateDivider(hal_SysGetFreq());
    hal_SysExitCriticalSection(scStatus);
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    pmd_EnablePower(PMD_POWER_SDMMC, TRUE);
#endif
}


//=============================================================================
// hal_SdmmcClose
//-----------------------------------------------------------------------------
/// Close the SDMMC module. Take a resource.
//=============================================================================
PUBLIC VOID hal_SdmmcClose(VOID)
{
    //pmd_EnablePower(PMD_POWER_SDMMC, FALSE);
    // Free a resource
    hal_SysRequestFreq(HAL_SYS_FREQ_SDMMC, HAL_SYS_FREQ_32K, NULL);
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    // Put the module in reset, as its clock is free running.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
#endif

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
    hwp_sysCtrl->per_soft_reset2 &= ~SYS_CTRL_SDMMC_RSTB_UNRESET;
#else
    hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_SDMMC;
    hwp_sysCtrl->Clk_Per_Disable = SYS_CTRL_DISABLE_PER_SDMMC;
#endif
    
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;
#endif
#if defined(__CT_CHIP_PKG_RDA5856T__) || defined(__CT_CHIP_PKG_RDA5856L__)||defined(__CT_CHIP_PKG_RDA5856Q32__)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    {
        gpio_ConvDetFunction();
    }
#endif
    
}


//=============================================================================
// hal_SdmmcWakeUp
//-----------------------------------------------------------------------------
/// This function requests a resource of #HAL_SYS_FREQ_52M. 
/// hal_SdmmcSleep() must be called before any other
//=============================================================================
PUBLIC VOID hal_SdmmcWakeUp(VOID)
{
    // Take a resource (The idea is to be able to get a 25Mhz clock)
    hal_SysRequestFreq(HAL_SYS_FREQ_SDMMC, HAL_SYS_FREQ_52M, hal_SdmmcUpdateDivider);

    UINT32 scStatus = hal_SysEnterCriticalSection();
    // Make sure the last clock is set
    hal_SdmmcUpdateDivider(hal_SysGetFreq());
    hal_SysExitCriticalSection(scStatus);
}


//=============================================================================
// hal_SdmmcSleep
//-----------------------------------------------------------------------------
/// This function release the resource to #HAL_SYS_FREQ_32K.
//=============================================================================
PUBLIC VOID hal_SdmmcSleep(VOID)
{
    // We just release the resource, because the clock gating in sdmmc controller
    // will disable the clock. We should wait for the clock to be actually disabled
    // but the module does not seam to have a status for that...

    // Free a resource
    hal_SysRequestFreq(HAL_SYS_FREQ_SDMMC, HAL_SYS_FREQ_32K, NULL);
}




PUBLIC VOID hal_SdmmcCloseClk(VOID)
{
    // Put the module in reset, as its clock is free running.
#if (CHIP_ASIC_ID < CHIP_ASIC_ID_5856E) 
    hwp_sysCtrl->Clk_Per_Mode |= SYS_CTRL_MODE_PER_SDMMC_MANUAL;
#endif
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
#else
    hwp_sysCtrl->Clk_Per_Disable &=(~SYS_CTRL_DISABLE_PER_SDMMC);
#endif
}





// =============================================================================
// hal_SdmmcSendCmd
// -----------------------------------------------------------------------------
/// Send a command to a SD card plugged to the sdmmc port.
/// @param cmd Command
/// @param arg Argument of the command
/// @param suspend Feature not implemented yet.
// =============================================================================
PUBLIC VOID hal_SdmmcSendCmd(HAL_SDMMC_CMD_T cmd, UINT32 arg, BOOL suspend)
{
    UINT32 configReg = 0;
#if !defined(GALLITE_IS_8805)
    hwp_sdmmc->sd_config = SDMMC_AUTO_FLAG_EN;
#else
    hwp_sdmmc->sd_config = 0x00000000;
#endif
    
    
    switch (cmd)
    {
        case HAL_SDMMC_CMD_GO_IDLE_STATE:
            configReg = SDMMC_SDMMC_SENDCMD;
            break;

            
        case HAL_SDMMC_CMD_ALL_SEND_CID:
            configReg = SDMMC_RSP_SEL_R2    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x51
            break;

        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SEND_IF_COND:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SET_DSR:
            configReg = SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SELECT_CARD:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SEND_CSD                :
            configReg = SDMMC_RSP_SEL_R2    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_STOP_TRANSMISSION        :
            configReg = 0; // TODO
            break;

        case HAL_SDMMC_CMD_SEND_STATUS            :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_SET_BLOCKLEN            :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD;
            break;

        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK        :
            configReg =  SDMMC_RD_WT_SEL_READ
                       | SDMMC_RD_WT_EN
                       | SDMMC_RSP_SEL_OTHER
                       | SDMMC_RSP_EN
                       | SDMMC_SDMMC_SENDCMD; // 0x111
            break;

        case HAL_SDMMC_CMD_READ_MULT_BLOCK        :
            configReg =  SDMMC_S_M_SEL_MULTIPLE
                       | SDMMC_RD_WT_SEL_READ
                       | SDMMC_RD_WT_EN
                       | SDMMC_RSP_SEL_OTHER
                       | SDMMC_RSP_EN
                       | SDMMC_SDMMC_SENDCMD; // 0x511;
            break;

        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK    :
            configReg = SDMMC_RD_WT_SEL_WRITE
                       | SDMMC_RD_WT_EN
                       | SDMMC_RSP_SEL_OTHER
                       | SDMMC_RSP_EN
                       | SDMMC_SDMMC_SENDCMD; // 0x311
            break;

        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK        :
            configReg =  SDMMC_S_M_SEL_MULTIPLE
                       | SDMMC_RD_WT_SEL_WRITE
                       | SDMMC_RD_WT_EN
                       | SDMMC_RSP_SEL_OTHER
                       | SDMMC_RSP_EN
                       | SDMMC_SDMMC_SENDCMD; // 0x711
            break;

        case HAL_SDMMC_CMD_APP_CMD                :
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SET_BUS_WIDTH            :
        case HAL_SDMMC_CMD_SWITCH:
            configReg = SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS    :
            configReg =  SDMMC_RD_WT_SEL_READ
                       | SDMMC_RD_WT_EN
                       | SDMMC_RSP_SEL_OTHER
                       | SDMMC_RSP_EN
                       | SDMMC_SDMMC_SENDCMD; // 0x111
            break;

        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT        :
            configReg =  SDMMC_RSP_SEL_OTHER | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x11
            break;

        case HAL_SDMMC_CMD_MMC_SEND_OP_COND:
        case HAL_SDMMC_CMD_SEND_OP_COND:
            configReg =  SDMMC_RSP_SEL_R3    | SDMMC_RSP_EN | SDMMC_SDMMC_SENDCMD; // 0x31
            break;

        default:
            break;
    }

    // TODO Add suspend management
    hwp_sdmmc->sd_cmd_index = SDMMC_COMMAND(cmd);
    hwp_sdmmc->sd_cmd_arg   = SDMMC_ARGUMENT(arg);

#if !defined(GALLITE_IS_8805)
    hwp_sdmmc->sd_config    = configReg |SDMMC_AUTO_FLAG_EN;
#else
    hwp_sdmmc->sd_config    = configReg ;
#endif
}


// =============================================================================
// hal_SdmmcNeedResponse
// -----------------------------------------------------------------------------
/// Tells if the given command waits for a reponse.
/// @return \c TRUE if the command needs an answer, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdmmcNeedResponse(HAL_SDMMC_CMD_T cmd)
{
    switch (cmd)
    {
        case HAL_SDMMC_CMD_GO_IDLE_STATE:
        case HAL_SDMMC_CMD_SET_DSR:
        case HAL_SDMMC_CMD_STOP_TRANSMISSION:
            return FALSE;
            break;

            
        case HAL_SDMMC_CMD_ALL_SEND_CID:
        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR:
        case HAL_SDMMC_CMD_SEND_IF_COND:

        case HAL_SDMMC_CMD_SELECT_CARD:
        case HAL_SDMMC_CMD_SEND_CSD:
        case HAL_SDMMC_CMD_SEND_STATUS:
        case HAL_SDMMC_CMD_SET_BLOCKLEN:
        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK:
        case HAL_SDMMC_CMD_READ_MULT_BLOCK:
        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK:
        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK:
        case HAL_SDMMC_CMD_APP_CMD:
        case HAL_SDMMC_CMD_SET_BUS_WIDTH:
        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS:
        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT:
        case HAL_SDMMC_CMD_MMC_SEND_OP_COND:
        case HAL_SDMMC_CMD_SEND_OP_COND:
        case HAL_SDMMC_CMD_SWITCH:
            return TRUE;
            break;
        
        default:
            HAL_ASSERT(FALSE, "Unsupported SDMMC command:%d", cmd);
            // Dummy return, for the compiler to be pleased.
            return FALSE;
            break;
    }
}


// =============================================================================
// hal_SdmmcCmdDone
// -----------------------------------------------------------------------------
/// @return \c TRUE of there is not command pending or being processed.
// =============================================================================
PUBLIC BOOL hal_SdmmcCmdDone(VOID)
{
    return (!(hwp_sdmmc->sd_status & SDMMC_NOT_SDMMC_OVER));
}


// =============================================================================
// hal_SdmmcGetCardDetectPinLevel
// -----------------------------------------------------------------------------
/// @return \c TRUE if card detect (DAT3) line is high,
///         \c FALSE if the line is low.
/// User must check with SD spec and board pull-up/down resistor to 
/// interpret this value.
// =============================================================================
PUBLIC BOOL hal_SdmmcGetCardDetectPinLevel(VOID)
{
    HAL_ASSERT(FALSE, "TODO: hal_SdmmcGetCardDetectPinLevel not implemented for your chip!");
    return FALSE;
}


//=============================================================================
// hal_SdmmcUpdateDivider
//-----------------------------------------------------------------------------
/// Update the SDMMC module divider to keep the desired frequency, whatever
/// the system frequency is.
/// @param sysFreq System Frequency currently applied or that will be applied
/// if faster.
//=============================================================================
PRIVATE VOID hal_SdmmcUpdateDivider(HAL_SYS_FREQ_T sysFreq)
{
    if (g_halSdmmcFreq != 0)
    {
        UINT32 divider = (sysFreq-1)/(2*g_halSdmmcFreq) + 1;
        if (divider>1)
        {
            divider = divider-1;
        }

        if (divider > 0xFF)
        {
            divider = 0xFF;
        }

        hwp_sdmmc->sd_trans_speed = SDMMC_SDMMC_TRANS_SPEED(divider);
    }
}


// =============================================================================
// hal_SdmmcSetClk
// -----------------------------------------------------------------------------
/// Set the SDMMC clock frequency to something inferior but close to that,
/// taking into account system frequency.
// =============================================================================
PUBLIC VOID hal_SdmmcSetClk(UINT32 clock)
{
    // TODO Add assert to stay on supported values ?
    g_halSdmmcFreq = clock;
    
    UINT32 scStatus = hal_SysEnterCriticalSection();
    // Update the divider takes care of the registers configuration
    hal_SdmmcUpdateDivider(hal_SysGetFreq());
    hal_SysExitCriticalSection(scStatus);
}


// =============================================================================
// hal_SdmmcGetOpStatus
// -----------------------------------------------------------------------------
/// @return The operational status of the SDMMC module.
// =============================================================================
PUBLIC HAL_SDMMC_OP_STATUS_T hal_SdmmcGetOpStatus(VOID)
{
    HAL_SDMMC_OP_STATUS_T status;
    status.reg = hwp_sdmmc->sd_status;
    return status;
}


// =============================================================================
// hal_SdmmcGetResp
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command. It is needed to provide the command index and its application 
/// specific status, in order to determine the type of answer expected.
///
/// @param cmd Command to send
/// @param arg Pointer to a four 32 bit word array, where the argument will be 
/// stored. Only the first case is set in case of a response of type R1, R3 or 
/// R6, 4 if it is a R2 response.
/// @param suspend Unsupported
// =============================================================================
PUBLIC VOID hal_SdmmcGetResp(HAL_SDMMC_CMD_T cmd, UINT32* arg, BOOL suspend)
{
    // TODO Check in the spec for all the commands response types
    switch (cmd)
    {
        // If they require a response, it is cargoed 
        // with a 32 bit argument.
        case HAL_SDMMC_CMD_GO_IDLE_STATE       : 
        case HAL_SDMMC_CMD_SEND_RELATIVE_ADDR  : 
        case HAL_SDMMC_CMD_SEND_IF_COND        : 
        case HAL_SDMMC_CMD_SET_DSR               : 
        case HAL_SDMMC_CMD_SELECT_CARD           : 
        case HAL_SDMMC_CMD_STOP_TRANSMISSION   : 
        case HAL_SDMMC_CMD_SEND_STATUS           : 
        case HAL_SDMMC_CMD_SET_BLOCKLEN           : 
        case HAL_SDMMC_CMD_READ_SINGLE_BLOCK   : 
        case HAL_SDMMC_CMD_READ_MULT_BLOCK       : 
        case HAL_SDMMC_CMD_WRITE_SINGLE_BLOCK  : 
        case HAL_SDMMC_CMD_WRITE_MULT_BLOCK       : 
        case HAL_SDMMC_CMD_APP_CMD               : 
        case HAL_SDMMC_CMD_SET_BUS_WIDTH       : 
        case HAL_SDMMC_CMD_SEND_NUM_WR_BLOCKS  : 
        case HAL_SDMMC_CMD_SET_WR_BLK_COUNT       :
        case HAL_SDMMC_CMD_MMC_SEND_OP_COND    :
        case HAL_SDMMC_CMD_SEND_OP_COND           :
        case HAL_SDMMC_CMD_SWITCH:
            arg[0] = hwp_sdmmc->sd_rsp_arg3;
            arg[1] = 0;
            arg[2] = 0;
            arg[3] = 0;
            break;

        // Those response arguments are 128 bits
        case HAL_SDMMC_CMD_ALL_SEND_CID           : 
        case HAL_SDMMC_CMD_SEND_CSD               :
            arg[0] = hwp_sdmmc->sd_rsp_arg0;
            arg[1] = hwp_sdmmc->sd_rsp_arg1;
            arg[2] = hwp_sdmmc->sd_rsp_arg2;
            arg[3] = hwp_sdmmc->sd_rsp_arg3;
            break;

        default:
            HAL_ASSERT(FALSE, "hal_SdmmcGetResp called with "
                              "an unsupported command: %d", cmd);
            break;
    }
}


// =============================================================================
// hal_SdmmcGetResp32BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command triggerring a response with a 32 bits argument (typically, 
/// R1, R3 or R6).
/// @return 32 bits of arguments of a 48 bits response token
// =============================================================================
PUBLIC UINT32 hal_SdmmcGetResp32BitsArgument(VOID)
{
    return hwp_sdmmc->sd_rsp_arg3;
}


// =============================================================================
// hal_SdmmcGetResp128BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command triggerring a response with a 128 bits argument (typically, 
/// R2).
/// @param Pointer to a 4 case arrays of 32 bits where the argument of the 
/// function will be stored.
// =============================================================================
PUBLIC VOID hal_SdmmcGetResp128BitsArgument(UINT32* buf)
{
    buf[0] = hwp_sdmmc->sd_rsp_arg0;
    buf[1] = hwp_sdmmc->sd_rsp_arg1;
    buf[2] = hwp_sdmmc->sd_rsp_arg2;
    buf[3] = hwp_sdmmc->sd_rsp_arg3;
}


// =============================================================================
// hal_SdmmcEnterDataTransferMode
// -----------------------------------------------------------------------------
/// Configure the SDMMC module to support data transfers
/// FIXME Find out why we need that out of the transfer functions...
// =============================================================================
PUBLIC VOID hal_SdmmcEnterDataTransferMode(VOID)
{
    hwp_sdmmc->sd_config |= SDMMC_RD_WT_EN;
}


// =============================================================================
// hal_SdmmcSetDataWidth
// -----------------------------------------------------------------------------
/// Set the data bus width
/// @param width Number of line of the SD data bus used.
// =============================================================================
PUBLIC VOID hal_SdmmcSetDataWidth(HAL_SDMMC_DATA_BUS_WIDTH_T width)
{
    switch (width)
    {
        case HAL_SDMMC_DATA_BUS_WIDTH_1:
            hwp_sdmmc->sd_data_width = 1;
            break;

        case HAL_SDMMC_DATA_BUS_WIDTH_4:
            hwp_sdmmc->sd_data_width = 4;
            break;

        default:
            HAL_ASSERT(FALSE, "hal_SdmmcSetDataWidth(%d) is an invalid width",
                    width);
    }
}


// =============================================================================
// hal_SdmmcTransfer
// -----------------------------------------------------------------------------
/// Start the ifc transfer to feed the SDMMC controller fifo.
/// @param transfer Transfer to program.
/// @return HAL_ERR_NO or HAL_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC HAL_ERR_T hal_SdmmcTransfer(HAL_SDMMC_TRANSFER_T* transfer)
{
    UINT8 channel = 0;
    HAL_IFC_REQUEST_ID_T ifcReq = HAL_IFC_NO_REQWEST;
    UINT32 length = 0;
    UINT32 lengthExp = 0;

    HAL_ASSERT((transfer->blockSize>=4) && (transfer->blockSize<=2048),
                "Block Length(%d) is invalid!\n", transfer->blockSize);

    length =  transfer->blockSize;
    
    // The block size register 
    while (length != 1)
    {
        length >>= 1;
        lengthExp++;
    }

    // Configure amount of data
    hwp_sdmmc->sd_block_cnt  = SDMMC_SDMMC_BLOCK_CNT(transfer->blockNum);
    hwp_sdmmc->sd_block_size = SDMMC_SDMMC_BLOCK_SIZE(lengthExp);

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
    // Configure Bytes reordering
    hwp_sdmmc->sd_ctrl = SDMMC_SOFT_RST_L | SDMMC_L_ENDIAN(1);
#else
    hwp_sdmmc->apbi_ctrl_sdmmc = SDMMC_SOFT_RST_L | SDMMC_L_ENDIAN(1);
#endif

    switch (transfer->direction)
    {
        case HAL_SDMMC_DIRECTION_READ:
            ifcReq = HAL_IFC_SDMMC_RX;
            break;

        case HAL_SDMMC_DIRECTION_WRITE:
            ifcReq = HAL_IFC_SDMMC_TX;
            break;

        default:
            HAL_ASSERT(FALSE, "hal_SdmmcTransfer with erroneous %d direction",
                            transfer->direction);
            break;
    }

    channel = hal_IfcTransferStart(ifcReq, transfer->sysMemAddr, 
                                   transfer->blockNum*transfer->blockSize,
                                   HAL_IFC_SIZE_32_MODE_MANUAL);
    if (channel == HAL_UNKNOWN_CHANNEL)
    {
        return HAL_ERR_RESOURCE_BUSY;
    }
    else
    {
        // Record Channel used.
        if (transfer->direction == HAL_SDMMC_DIRECTION_READ)
        {
            g_halSdmmcReadCh = channel;
        }
        else
        {
            g_halSdmmcWriteCh = channel;
        }
        return HAL_ERR_NO;
    }
}


// =============================================================================
// hal_SdmmcTransferDone
// -----------------------------------------------------------------------------
/// Check the end of transfer status.
/// Attention: This means the SDMMC module has finished to transfer data.
/// In case of a read operation, the Ifc will finish its transfer shortly 
/// after. Considering the way this function is used (after reading at least
/// 512 bytes, and flushing cache before releasing the data to the user), and
/// the fifo sizes, this is closely equivalent to the end of the transfer.
/// @return \c TRUE if a transfer is over, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdmmcTransferDone(VOID)
{
    // The link is not full duplex. We check both the
    // direction, but only one can be in progress at a time.
    if (g_halSdmmcReadCh != HAL_UNKNOWN_CHANNEL)
    {
        // Operation in progress is a read.
        // The SDMMC module itself can know it has finished
        if ((hwp_sdmmc->sd_int_status & SDMMC_DAT_OVER_INT)
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E)
         && (hwp_sysIfc->std_ch[g_halSdmmcReadCh].std_ch_tc == 0)
#else
		 && (hwp_sysIfc->std_ch[g_halSdmmcReadCh].tc == 0)
#endif
		 )
        {
            // Transfer is over
            hwp_sdmmc->sd_int_clear = SDMMC_DAT_OVER_CL;
            hal_IfcChannelRelease(HAL_IFC_SDMMC_RX, g_halSdmmcReadCh);

            // We finished a read
            g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
            //  Put the FIFO in reset state.
            hwp_sdmmc->sd_ctrl = 0 | SDMMC_L_ENDIAN(1);
#else
            //  Put the FIFO in reset state.
            hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);
#endif
            return TRUE;
        }
    }

    if (g_halSdmmcWriteCh != HAL_UNKNOWN_CHANNEL)
    {
        // Operation in progress is a write.
        // The SDMMC module itself can know it has finished
        if ((hwp_sdmmc->sd_int_status & SDMMC_DAT_OVER_INT)
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E)
         && (hwp_sysIfc->std_ch[g_halSdmmcWriteCh].std_ch_tc == 0)
#else
         && (hwp_sysIfc->std_ch[g_halSdmmcWriteCh].tc == 0)
#endif
		 )
        {
            // Transfer is over
            hwp_sdmmc->sd_int_clear = SDMMC_DAT_OVER_CL;
            hal_IfcChannelRelease(HAL_IFC_SDMMC_TX, g_halSdmmcWriteCh);

            // We finished a write
            g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
            //  Put the FIFO in reset state.
            hwp_sdmmc->sd_ctrl = 0 | SDMMC_L_ENDIAN(1);
#else
	            //  Put the FIFO in reset state.
            hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);
#endif
            return TRUE;
        }
    }
    
    // there's still data running through a pipe (or no transfer in progress ...)
    return FALSE;
}


// =============================================================================
// hal_SdmmcStopTransfer
// -----------------------------------------------------------------------------
/// Stop the ifc transfer feeding the SDMMC controller fifo.
/// @param transfer Transfer to program.
/// @return #HAL_ERR_NO
// =============================================================================
PUBLIC HAL_ERR_T hal_SdmmcStopTransfer(HAL_SDMMC_TRANSFER_T* transfer)
{
    // Configure amount of data
    hwp_sdmmc->sd_block_cnt  = SDMMC_SDMMC_BLOCK_CNT(0);
    hwp_sdmmc->sd_block_size = SDMMC_SDMMC_BLOCK_SIZE(0);
    
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5856E) 
    //  Put the FIFO in reset state.
    hwp_sdmmc->sd_ctrl = 0 | SDMMC_L_ENDIAN(1);
#else
    hwp_sdmmc->apbi_ctrl_sdmmc = 0 | SDMMC_L_ENDIAN(1);
#endif

    if (transfer->direction == HAL_SDMMC_DIRECTION_READ)
    {
        hal_IfcChannelFlush(HAL_IFC_SDMMC_RX, g_halSdmmcReadCh);
        while(!hal_IfcChannelIsFifoEmpty(HAL_IFC_SDMMC_RX, g_halSdmmcReadCh));
        hal_IfcChannelRelease(HAL_IFC_SDMMC_RX, g_halSdmmcReadCh);
        g_halSdmmcReadCh = HAL_UNKNOWN_CHANNEL;
    }
    else
    {
        hal_IfcChannelFlush(HAL_IFC_SDMMC_TX, g_halSdmmcWriteCh);
        while(!hal_IfcChannelIsFifoEmpty(HAL_IFC_SDMMC_TX, g_halSdmmcReadCh));
        hal_IfcChannelRelease(HAL_IFC_SDMMC_TX, g_halSdmmcWriteCh);
        g_halSdmmcWriteCh = HAL_UNKNOWN_CHANNEL;
    }
    return HAL_ERR_NO;
}


// =============================================================================
// hal_SdmmcIsBusy
// -----------------------------------------------------------------------------
/// Check if the SD/MMC is busy.
/// 
/// @return \c TRUE if the SD/MMC controller is busy.
///         \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdmmcIsBusy(VOID)
{
    if (g_halSdmmcReadCh    != HAL_UNKNOWN_CHANNEL
     || g_halSdmmcWriteCh   != HAL_UNKNOWN_CHANNEL
     || ((hwp_sdmmc->sd_status & (SDMMC_NOT_SDMMC_OVER | SDMMC_BUSY | SDMMC_DL_BUSY)) != 0)
       )
    {
        // SD/MMc is busy doing something.
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif // CHIP_HAS_SDMMC












