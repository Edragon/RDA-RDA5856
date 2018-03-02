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
//
/// @file hal_sim.c  
/// This file contain the SIM driver code
//
////////////////////////////////////////////////////////////////////////////////

/// @todo  Add the support for badTs and atrStart interruptions
/// or remove them from the interface ?


#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "sci.h"

#include "halp_sys_ifc.h"
#include "halp_sys.h"
#include "halp_debug.h"

#include "hal_sim.h"
#include "hal_timers.h"

#include "pmd_m.h"

#include "sxr_sbx.h"
#include "sxr_tim.h"
#include "sxr_tls.h"

#ifdef GALLITE_IS_8806
#include "hal_host.h"
#include "sys_ctrl.h"
// Dual sim includes
#include "drv_dualsim.h"

extern BOOL pmd_SelectSimCard(UINT8 sim_card);
extern PUBLIC DUALSIM_ERR_T foursim_SetSimVoltage_8806(DUALSIM_SIM_VOLTAGE_T sim0Level,DUALSIM_SIM_VOLTAGE_T sim1Level,
							DUALSIM_SIM_VOLTAGE_T sim2Level, DUALSIM_SIM_VOLTAGE_T sim3Level);
#endif


// ============================================================================
//  MACROS
// ============================================================================
#define N_TX_CMD_BYTES          5
#define MAX_RX_BYTES            1023 

#define SCI_MAINDIV_372_1       0x174
#define SCI_DIV16_372_1         0x16

#define SCI_MAINDIV_372_8       0x2e
#define SCI_DIV16_372_8         0x2

#define SCI_MAINDIV_372_12      0x1f
#define SCI_DIV16_372_12        0x1

#define SCI_MAINDIV_372_16      0x17
#define SCI_DIV16_372_16        0x1


#define SCI_MAINDIV_512_8       0x040
#define SCI_DIV16_512_8         0x03

#define SCI_MAINDIV_512_12      0x02a
#define SCI_DIV16_512_12        0x02

#define SCI_MAINDIV_512_16      0x020
#define SCI_DIV16_512_16        0x01

#define SCI_MAINDIV_512_32      0x0f
#define SCI_DIV16_512_32        0x00

#define SCI_CLK_MAX             3250000

/// Time to wait after a change of the ETU duration, in number of ms.
#define  HAL_SIM_ETU_WAIT       100


// ============================================================================
//  TYPES
// ============================================================================

// ============================================================================
// HAL_SIM_IRQ_INTERNAL_CAUSE_T
// ----------------------------------------------------------------------------
/// Sim irq cause, as in the hardware registers
// ============================================================================
typedef struct
{
    UINT32 rxDone:1;
    UINT32 rxHalf:1;
    UINT32 wwtTimeout:1;
    UINT32 extraRx:1;
    UINT32 resendOvfl:1;
    UINT32 argEnd:1;
} HAL_SIM_IRQ_INTERNAL_CAUSE_T;


// ============================================================================
//  GLOBAL VARIABLES
// ============================================================================
PRIVATE UINT8  g_halReceivingAtr;
PRIVATE UINT8  g_halIfcChan;
PRIVATE UINT8  g_halRxDmaInProgress;
PRIVATE UINT8* g_halRxBuf;

PRIVATE UINT16 g_halRxBytes;
//PRIVATE volatile BOOL SplitXfer, SplitAutoClockMode;

PRIVATE UINT16 g_halRxExpected;
PRIVATE UINT32 g_halSciMainDivVal;

/// Variable to store the user handler
PRIVATE HAL_SIM_IRQ_HANDLER_T g_halSimRegistry = NULL;

PRIVATE HAL_SIM_IRQ_STATUS_T g_halSimUserMask  = {1,1,1,1,1,1,1,1};

PRIVATE HWP_SCI_T g_sci_reg;

#ifdef GALLITE_IS_8806
PRIVATE UINT32 g_ResetIndex = 0;
#endif


// ============================================================================
//  FUNCTIONS
// ============================================================================


PUBLIC UINT32 hal_SimGetRegConfig()
{
    return (UINT32)hwp_sci->Config;
}

PUBLIC UINT32 hal_SimGetRegClkDiv()
{
    return (UINT32)hwp_sci->ClkDiv;
}

PUBLIC UINT32 hal_SimGetRegTimes()
{
    return (UINT32)hwp_sci->Times;
}

PUBLIC UINT32 hal_SimGetRegChFilt()
{
    return (UINT32)hwp_sci->Ch_Filt;
}

PUBLIC UINT32 hal_SimGetRegIntMask()
{
    return (UINT32)hwp_sci->Int_Mask;
}

PUBLIC UINT32 hal_SimGetRegIntCause()
{
    return (UINT32)hwp_sci->Int_Cause;
}

PUBLIC UINT32 hal_SimGetRegRxCnt()
{
    return (UINT32)hwp_sci->RxCnt;
}

PUBLIC UINT32 hal_SimGetRegStatus()
{
    return (UINT32)hwp_sci->Status;
}

PUBLIC VOID hal_simParseReg()
{
    g_sci_reg.Config = hal_SimGetRegConfig();
    g_sci_reg.Status = hal_SimGetRegStatus();
    g_sci_reg.ClkDiv = hal_SimGetRegClkDiv();
    g_sci_reg.RxCnt = hal_SimGetRegRxCnt();
    g_sci_reg.Times = hal_SimGetRegTimes();
    g_sci_reg.Ch_Filt = hal_SimGetRegChFilt();
    g_sci_reg.Int_Cause = hal_SimGetRegIntCause();
    g_sci_reg.Int_Mask = hal_SimGetRegIntMask();
}

PUBLIC VOID hal_SimRestoreRegConfig(UINT32 Restore)
{
    // Don't write the ARG_H or AFD_EN_H bits
    hwp_sci->Config = Restore & ~(SCI_ARG_H | SCI_AFD_EN_H) ;
}

PUBLIC VOID hal_SimRestoreRegClkDiv(UINT32 Restore)
{
    hwp_sci->ClkDiv = Restore;
}

PUBLIC VOID hal_SimRestoreRegTimes(UINT32 Restore)
{
    hwp_sci->Times = Restore;
}

PUBLIC VOID hal_SimRestoreRegChFilt(UINT32 Restore)
{
    hwp_sci->Ch_Filt = Restore;
}

PUBLIC VOID hal_SimRestoreRegIntMask(UINT32 Restore)
{
    hwp_sci->Int_Mask = Restore;
}


PUBLIC UINT32 hal_SimGetResetPol()
{
    return (UINT32)hwp_sci->Config;
}


// ============================================================================
// hal_SimUpdateDivider
// ----------------------------------------------------------------------------
/// Update the SIM main divider when the system clock change
/// @param newSysFreq New system Frequency.
// ============================================================================
PRIVATE VOID hal_SimUpdateDivider(HAL_SYS_FREQ_T newSysFreq)
{
    UINT32 clkReg = hwp_sci->ClkDiv & ~SCI_MAINDIV_MASK;

    g_halSciMainDivVal = (newSysFreq/(2*SCI_CLK_MAX)) - 1;  

    hwp_sci->ClkDiv = clkReg | SCI_MAINDIV(g_halSciMainDivVal);
//    HAL_TRACE(HAL_SIM_TRC | TSTDOUT, 0, "SIM clock sys:%d div:%d", newSysFreq, g_halSciMainDivVal);
}


// ============================================================================
// hal_SimSetEtuBitflied
// ----------------------------------------------------------------------------
/// Set the ETU duration bitfield
/// 
/// @param F, the clock divider param F for generating the baud clock 
/// from SCI clock
/// @param D, the clock divider param D
// ============================================================================
INLINE VOID hal_SimSetEtuBitfield(UINT16 F, UINT8 D, UINT8 SciInvFlag)
{
    // Need to change the SCI clock frequency and maybe also
    // the internal SCI dividers
    UINT32 InvFlag;
    InvFlag = 	(SciInvFlag == 1) ? SCI_CLK_INV : 0;

	
    if (F == 372)
    {
        if(D == 16)
        {
            hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_372_16) |
                            SCI_CLKDIV(SCI_MAINDIV_372_16) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
        else if(D == 12)
        {
            hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_372_12) |
                            SCI_CLKDIV(SCI_MAINDIV_372_12) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
        else if(D == 8)
        {
            hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_372_8) |
                            SCI_CLKDIV(SCI_MAINDIV_372_8) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
        else 
        {
            hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_372_1) |
                            SCI_CLKDIV(SCI_MAINDIV_372_1) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
    }
    else if(F == 512)
    {
        if (D == 16)
	 {
	    hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_512_16) |
                            SCI_CLKDIV(SCI_MAINDIV_512_16) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
        else if(D == 12)
        {
          hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_512_12) |
                            SCI_CLKDIV(SCI_MAINDIV_512_12) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
        else
        {
          hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_512_8) |
                            SCI_CLKDIV(SCI_MAINDIV_512_8) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
        }
    }

    else // default case
    {
          hwp_sci->ClkDiv =   SCI_CLKDIV_16(SCI_DIV16_512_8) |
                            SCI_CLKDIV(SCI_MAINDIV_512_8) |
                            SCI_MAINDIV(g_halSciMainDivVal)|
                            InvFlag;
    }

}


// ============================================================================
// hal_SimClrConfigBit
// ----------------------------------------------------------------------------
/// Clear the config bits in the mask parameter.
/// @param mask Bit to '1' will be cleared.
// ============================================================================
INLINE VOID hal_SimClrConfigBit(UINT32 mask)
{
    hwp_sci->Config &= ~(mask);
}


// ============================================================================
// hal_SimSetConfigBit
// ----------------------------------------------------------------------------
/// Set the selected bit in the config register.
///
/// @param mask Bitfield where the bit whose 
/// value is '1' in the mask will be set in the config
/// register. The other bits are unchanged
// ============================================================================
INLINE VOID hal_SimSetConfigBit(UINT32 mask)
{
    hwp_sci->Config |= mask;
}

// TODO: Add a enum type for different data convention formats
PUBLIC UINT32 hal_SimGetDataConvFormat(VOID)
{
    UINT32 simConfig = (UINT32)hwp_sci->Config;
    if((simConfig & SCI_LLI)!=0 && (simConfig & SCI_MSBH_LSBL)!=0)
    {
        return 1;
    }
    else if((simConfig & SCI_LLI)==0 && (simConfig & SCI_MSBH_LSBL)==0)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}


PUBLIC VOID hal_SimSetDataConvFormat(UINT32 Ts)
{
    // Ensure AFD is disabled to support different SIM data convetions in dual SIM mode.
    hal_SimClrConfigBit(SCI_AFD_EN_H);

    if(Ts == 0x3B)
    {
        hal_SimClrConfigBit(SCI_LLI | SCI_MSBH_LSBL);
    }
    else
    {
        hal_SimSetConfigBit(SCI_LLI | SCI_MSBH_LSBL);
    }
}


// ============================================================================
// hal_SimTxByte
// ----------------------------------------------------------------------------
/// Send a byte to the sim card
/// @param txByte The byte to transmit.
// ============================================================================
INLINE VOID hal_SimTxByte(UINT8 txByte)
{
    hwp_sci->Data = txByte;
}


// ============================================================================
// hal_SimSendNTxBytes
// ----------------------------------------------------------------------------
/// Send a bunch of byte to the SIM using the IFC
///
/// @param nTx Number of bytes to send.
/// @param pTxBytes Buffer of bytes to send.
/// @return Channel affected to the transfer.
// ============================================================================
INLINE UINT8 hal_SimSendNTxBytes(UINT8 nTx, CONST UINT8 *pTxBytes)
{
    return (hal_IfcTransferStart(HAL_IFC_SCI_TX, (UINT8*)pTxBytes, nTx, HAL_IFC_SIZE_8_MODE_AUTO));
}


// ============================================================================
// hal_SimSetNRxBytes
// ----------------------------------------------------------------------------
/// Configure the reception of a bunch of data from the sim
/// @param nRx Number of bytes to be expected by the Rx fifo 
/// @param nRxIfc Number of bytes to read through the IFC if used
/// @param pRxBytes Buffer where read data will be written
/// @param useIfc  \c TRUE if IFC is to be used \n
///                 \c FALSE otherwise.
/// @param stopClock If \c TRUE and in auto-mode, the clock is
/// stopped after this transfer.
/// @return The IFC channel used, if any.
// ============================================================================
PRIVATE UINT8 hal_SimSetNRxBytes(UINT16 nRx, UINT16 nRxIfc, UINT8* pRxBytes,
                                BOOL useIfc, BOOL stopClock)
{
    UINT32 channel;
    if (useIfc == TRUE)
    {
        channel = hal_IfcTransferStart(HAL_IFC_SCI_RX, pRxBytes, nRxIfc, HAL_IFC_SIZE_8_MODE_AUTO);
    }
    else
    {
        channel =  HAL_UNKNOWN_CHANNEL;
    }
    g_halRxExpected = nRxIfc;

    if (stopClock)
    {
        hwp_sci->RxCnt = nRx; //clk_persist bit set to 0
    }
    else
    {
        hwp_sci->RxCnt = nRx | SCI_CLK_PERSIST; // clk_persit bit set to 1
    }
    
    return channel;
}


// ============================================================================
// hal_SimAutoReset
// ----------------------------------------------------------------------------
/// Does the basic write to begin the Auto
/// Reset sequence
/// 
/// The interrupt should be previously enabled to handle the end of ARG
/// The bits are set according to the following
/// 00: SCI_ENABLE = 1;\n
/// 01: SCI_PARITY = 0;\n
/// 02: SCI_PERF = 0;\n
/// 03: SCI_FILTER_DISABLE = 0;\n\n
/// 
/// 04: SCI_CLOCKSTOP = 0\n;
/// 05: SCI_AUTOSTOP_EN_H = 0\n;
/// 06: SCI_MSBH_LSBL = <unchanged>\n;
/// 07: SCI_LLI = <unchanged>\n\n;
/// 
/// 08: SCI_PEGEN_LEN = 1;\n\n
/// 
/// 16: SCI_ARG_H = 1\n;
/// 17: SCI_AFD_EN_H = 0\n;
/// 18: SCI_TX_RESEND_EN_H = 1;\n\n
/// 
/// 20: SCI_RESET = 1;\n\n
/// 
/// Don't enable parity checking on Rst, use Manual Mode for Clk_SCI
/// Keep the default value for parity offset.
// ============================================================================
INLINE VOID hal_SimAutoReset(VOID)
{
    // Clear all but upper bits and SCI_RESET/SCI_LLI/SCI_MSBH_LSBL
    // AFD must be disabled to support different SIM data convention in dual SIM mode
    hal_SimClrConfigBit(SCI_ENABLE |
                        SCI_PARITY_ODD_PARITY |
                        SCI_PERF |
                        SCI_FILTER_DISABLE |
                        SCI_CLOCKSTOP |
                        SCI_AUTOSTOP_EN_H |
                        //SCI_MSBH_LSBL |
                        //SCI_LLI |
                        SCI_PEGEN_LEN |
                        SCI_PARITY_EN |
                        SCI_STOP_LEVEL |
                        SCI_ARG_H |
                        SCI_AFD_EN_H |
                        SCI_TX_RESEND_EN_H |
                        //SCI_RESET |
                        SCI_DLY_SEL);

    // ARG is enabled in auto reset, but if ARG fails, it will be disabled in power down process.
    // SCI_RESET must be set to ensure SIM card is NOT reset even when disabling ARG.
    // SCI_PARITY must be cleared in reset process.
    hal_SimSetConfigBit(SCI_ENABLE |
                        SCI_PEGEN_LEN |
                        SCI_ARG_H |
                        SCI_TX_RESEND_EN_H |
                        SCI_RESET);
}

PUBLIC VOID hal_SimRegReset()
{
     hal_SimSetConfigBit(SCI_RESET);	 
     hal_SimClrConfigBit(SCI_ARG_H);	 
}

PUBLIC VOID hal_SimDeactive(VOID)
{
    hal_SimClrConfigBit(SCI_ARG_H);
    hal_SimClrConfigBit(SCI_RESET);
    hal_TimDelay(50);

    hal_SimClrConfigBit(SCI_AUTOSTOP_EN_H);
    hal_SimSetConfigBit(SCI_CLOCKSTOP);
    hal_TimDelay(50);
}
// ============================================================================
// hal_SimSetWwi
// ----------------------------------------------------------------------------
/// Set the WWI factor defining the WWT
///     
/// It is the Work Waiting Time factor.  A timeout will be generated 
/// when the WWT is exceeded.  The WWT is calculated by:
/// <code> WWT = 960 x WI x (F/Fi) </code> \n
/// where \c Fi is the main SCI clock frequency (3-5 MHz) and 
/// \c F is 372 before an enhanced PPS and 512 after an enhanced PPS.
/// \n
/// The SCI_WI value must be calculated as follows: \n
///   <code> SCI_WI = WI * D </code> \n
/// Thus, by default (WI = 10) this value needs to be set to 10 before 
/// an EPPS, but needs to be scaled to WI*D=80 after the EPPS procedure.
///
/// @param WI WWI value to set
// ============================================================================
INLINE VOID hal_SimSetWwi(UINT8 wi)
{
#if (CHIP_HAS_SIM_T0T1 == 0)
    hwp_sci->Times &= ~(SCI_WI_MASK);   // Clear old WI value
    hwp_sci->Times |= SCI_WI(wi);
#else
    hwp_sci->Bwt_Wwt = 960*wi;
#endif
}


// ============================================================================
// hal_SimGetArgStatus
// ----------------------------------------------------------------------------
/// Returns the status of the automatic reset procedure:
/// @return 0 = ARG detection has failed. \n
/// 1 = ARG detection has detected that the SIM has responded to the reset 
// ============================================================================
INLINE BOOL hal_SimGetArgStatus(VOID)
{
    
    if ((hwp_sci->Status & SCI_ARG_DET) == 0)
    {
        // SCI_ARG_DET bit = 0, failed
        return(FALSE);
    }
    else
    {
        return(TRUE);
    }
}


// ============================================================================
// hal_SimGetClkStatus
// ----------------------------------------------------------------------------
/// Returns the status of the clock:
/// @return 0 = clock on. \n
/// 1 = clock off 
// ============================================================================
PUBLIC BOOL hal_SimGetClkStatus(VOID)
{
    
    if ((hwp_sci->Status & SCI_CLK_OFF) == 0)
    {
        // SCI_CLK_OFF bit = 0, clock on
        return(TRUE);
    }
    else
    { // clock off
        return(FALSE);
    }
}

// ============================================================================
// hal_SimGetClkStopStatus
// ----------------------------------------------------------------------------
/// Returns the status of the clock stop mode:
/// @return 0 = manual mode. \n
/// 1 = auto stop mode 
// ============================================================================
PUBLIC BOOL hal_SimGetClkStopStatus(VOID)
{
  
    if ((hwp_sci->Status & SCI_AUTOSTOP_STATE_MASK) == 0  ) //manual clock mode 
    {
        return(FALSE);
    }
    else
    { // auto stop mode
        return(TRUE);
    }
}

// ============================================================================
// hal_SimGetResetStatus
// ----------------------------------------------------------------------------
/// Returns the status of the reset pin after automatic reset procedure.  Since
/// the card is _not_ in reset after a successful ATR, the reset pin (and the
/// status bit) is in the _NON-ACTIVE RESET_ state.
/// @return 1 = Reset pin at 0 (Active high Reset) \n
/// 1 = Reset pin at 0 (Active low Reset)
// ============================================================================
PUBLIC UINT8 hal_SimGetResetStatus(VOID)
{
    if ((hwp_sci->Status & SCI_RESET_DET) == 0)
    {
        return(0);
    }
    else
    {
        return(1);
    }
}

#ifdef GALLITE_IS_8806
PUBLIC VOID hal_SimSetDbg(VOID)
{
    if ((hwp_sci->Status  & SCI_AUTOSTOP_RXCOUNTER_MASK) != 0)
    {
        hal_HstSendEvent(SYS_EVENT,0x22110000 );
    }
    else
    {
        hwp_sci->dbg = 0x1;
		while( (hwp_sci->Status & SCI_AUTOSTOP_RXCOUNTER_MASK) != 0)
		{
		    hwp_sci->dbg = 0x1;
        };
	
        hwp_sci->dbg = 0x0;
        hwp_sci->dbg = 0x0;
	}
}


PUBLIC VOID hal_SimResetSys(VOID)
{
 #if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
	hwp_sysCtrl->REG_DBG     = SYS_CTRL_PROTECT_UNLOCK;
	hwp_sysCtrl->Sys_Rst_Set = SYS_CTRL_SET_RST_SCI;
	hwp_sysCtrl->Sys_Rst_Clr  = SYS_CTRL_CLR_RST_SCI;
	hwp_sysCtrl->REG_DBG    = SYS_CTRL_PROTECT_LOCK;
 #endif   
}
#endif


// ============================================================================
// hal_SimGetRxCnt
// ----------------------------------------------------------------------------
/// @return The number of byte yet to receive.
// ============================================================================
INLINE UINT32 hal_SimGetRxCnt(VOID)
{
    return (g_halRxExpected - hwp_sci->RxCnt);
}


// ============================================================================
// hal_SimReleaseIFC
// ----------------------------------------------------------------------------
///  Release the IFC channel used by the sci
// ============================================================================
INLINE VOID hal_SimReleaseIFC(VOID)
{   
    hal_IfcChannelFlush(HAL_IFC_SCI_RX, g_halIfcChan);
    hal_IfcChannelRelease(HAL_IFC_SCI_RX, g_halIfcChan);
}


// ============================================================================
// hal_SimGetRxByte
// ----------------------------------------------------------------------------
/// Read a byte receive from the SIM card.
/// @return A byte read from the SIM card.
// ============================================================================
INLINE UINT8 hal_SimGetRxByte(VOID)
{
    return (hwp_sci->Data);
}


// ============================================================================
// hal_SimEnableParity
// ----------------------------------------------------------------------------
/// Selects the parity generation/detection
///
/// @param enable If \c FALSE, even parity is selected.
///               If \c TRUE, odd parity is selected.
// ============================================================================
INLINE VOID hal_SimEnableParity(BOOL enable)
{
    if (enable == TRUE)
    {
        hal_SimSetConfigBit(SCI_PARITY_EN);
    }
    else
    {
        hal_SimClrConfigBit(SCI_PARITY_EN);
    }
}


// ============================================================================
// hal_SimOpen
// ----------------------------------------------------------------------------
/// Open the SIM driver and enables the use of its API,
/// and initializes the SIM.
// ============================================================================
PUBLIC VOID hal_SimOpen(VOID)
{
    g_halReceivingAtr = 0;
    g_halRxDmaInProgress = 0;

    // Set the physical IRQ mask to let the driver properly work
    hwp_sci->Int_Mask = SCI_RX_DONE | SCI_WWT_TIMEOUT | SCI_EXTRA_RX
                        |SCI_RESEND_OVFL | SCI_ARG_END;

    // Set main divider according to the system frequency.
    g_halSciMainDivVal = (g_halSysSystemFreq/(2*SCI_CLK_MAX)) - 1;
    g_halRxExpected = 0;

    // Set some other stuff?
    hal_SimSetEtuBitfield(372, 1, 1);

    // Set the resource as inactive
    hal_SysRequestFreq(HAL_SYS_FREQ_SIM, HAL_SYS_FREQ_32K, hal_SimUpdateDivider);

    UINT32 scStatus = hal_SysEnterCriticalSection();
    // Update the divider
    hal_SimUpdateDivider(hal_SysGetFreq());
    hal_SysExitCriticalSection(scStatus);
}


// ============================================================================
// hal_SimClose
// ----------------------------------------------------------------------------
/// Close the SIM driver and disables the use of its API.
/// #hal_SimPowerDown has been called before that function.
// ============================================================================
PUBLIC VOID hal_SimClose(VOID)
{
    // Clear all but upper bits
    hal_SimClrConfigBit(SCI_ENABLE |
                        SCI_PARITY_ODD_PARITY |
                        SCI_PERF |
                        SCI_FILTER_DISABLE |
                        SCI_CLOCKSTOP |
                        SCI_AUTOSTOP_EN_H |
                        SCI_MSBH_LSBL |
                        SCI_LLI |
                        SCI_PEGEN_LEN |
                        SCI_PARITY_EN |
                        SCI_STOP_LEVEL |
                        SCI_ARG_H |
                        SCI_AFD_EN_H |
                        SCI_TX_RESEND_EN_H |
                        SCI_RESET |
                        SCI_DLY_SEL);
}


// ============================================================================
// hal_SimSciEnabled
// ----------------------------------------------------------------------------
/// Check whether the SIM card interface hardware module has been enabled.
/// @return TRUE if enabled, FALSE otherwise.
// ============================================================================
INLINE BOOL hal_SimSciEnabled(VOID)
{
    return ((hwp_sci->Config & SCI_ENABLE) != 0);
}


// ============================================================================
// hal_SimEnableSci
// ----------------------------------------------------------------------------
/// Enalbe the SIM card interface hardware module.
// ============================================================================
PUBLIC VOID hal_SimEnableSci(VOID)
{
    if (!hal_SimSciEnabled())
    {
        // Clear previous IRQs if any
        hwp_sci->Int_Clr = hwp_sci->Int_Cause;

        // Recover some config bits, to avoid messing up SIM state when
        // switching to another SIM card.
        // If failing to reset previous SIM card (ARG failure), the
        // power down process will clear most config bits.
        hal_SimSetConfigBit(SCI_ENABLE |
                            SCI_PEGEN_LEN |
                            SCI_PARITY_EN |
                            SCI_TX_RESEND_EN_H |
                            SCI_RESET);
    }
}


// ============================================================================
// hal_SimDisableSci
// ----------------------------------------------------------------------------
/// Disable the SIM card interface hardware module.
// ============================================================================
PUBLIC VOID hal_SimDisableSci(VOID)
{
    hal_SimClrConfigBit(SCI_ENABLE);
}


// ============================================================================
// hal_SimReset
// ----------------------------------------------------------------------------
/// This function starts the reset sequence, during which 
/// the timing of each signal clock, data, vcc are directly controlled by 
/// the driver. At the end of the reset, an ATR success interrupt
/// is sent
///
/// @param pRxByte Pointer to the buffer used to store the reset
/// command response.
// ============================================================================
PUBLIC VOID hal_SimReset(UINT8* pRxBytes)
{
#ifdef GALLITE_IS_8806
  g_ResetIndex = 1;
#endif
  g_halRxDmaInProgress = 1;
  HAL_TRACE(HAL_SIM_TRC | TSTDOUT, 0, "SIM reset buffer: %#x", pRxBytes);
  g_halIfcChan = hal_SimSetNRxBytes(0xff, 0xff, pRxBytes, TRUE, FALSE);
  hal_SimAutoReset();
  g_halRxBuf = pRxBytes;
}


// ============================================================================
// hal_SimPowerDown
// ----------------------------------------------------------------------------
/// This function does a power down of the SIM. The timings of each signals
/// are controlled by the driver, which switch off the clock and Vcc.
// ============================================================================
PUBLIC VOID hal_SimPowerDown(VOID)
{
    // Clear all but upper bits and SCI_RESET/SCI_LLI/SCI_MSBH_LSBL
    // AFD must be disabled to support different SIM data convention in dual SIM mode
    hal_SimClrConfigBit(SCI_ENABLE |
                        SCI_PARITY_ODD_PARITY |
                        SCI_PERF |
                        SCI_FILTER_DISABLE |
                        SCI_CLOCKSTOP |
                        SCI_AUTOSTOP_EN_H |
                        //SCI_MSBH_LSBL |
                        //SCI_LLI |
                        SCI_PEGEN_LEN |
                        SCI_PARITY_EN |
                        SCI_STOP_LEVEL |
                        SCI_ARG_H |
                        SCI_AFD_EN_H |
                        SCI_TX_RESEND_EN_H |
                        //SCI_RESET |
                        SCI_DLY_SEL);

    // Disable the SCI clock (auto clock control has been disabled)
    hal_SimSetConfigBit(SCI_CLOCKSTOP);
}


// ============================================================================
// hal_SimInterfaceActive
// ----------------------------------------------------------------------------
/// This is used to determine when the interface is no longer busy.  It does
/// that by checking whether the SCI clock is off.  If it is off, the interface
/// has completed its current transaction including the shutdown delay.
/// @return TRUE if the interface is active and the clock is still on, FALSE
/// if the interface is inactive and the clock is off.
// ============================================================================
PUBLIC BOOL hal_SimInterfaceActive(VOID)
{
    // SCI_CLK_OFF = 0 => SCI Clock is on (active)
    // SCI_CLK_OFF = 1 => SCI Clock is off (inactive)
    return (((hwp_sci->Status) & SCI_CLK_OFF)==0);
}


// ============================================================================
// hal_SimCheckClockStatus
// ----------------------------------------------------------------------------
/// This function checks the clock status of the SIM modules. When it is not
/// needed, the request for a clock is given up (ie request for no clock
/// is done.)
// ============================================================================
PROTECTED VOID hal_SimCheckClockStatus(VOID)
{
    // No more need for a clock
    if((hwp_sci->Status) & SCI_CLK_OFF)
    {
        // Set the resource as inactive
        hal_SysRequestFreq(HAL_SYS_FREQ_SIM, HAL_SYS_FREQ_32K, hal_SimUpdateDivider);
    }
}


// ============================================================================
// hal_SimSetClockStopMode
// ----------------------------------------------------------------------------
/// This function is used to indicate to the driver how the sim clock should be
/// switch off, depending on the ATR response.
///
/// @param mode This is the stop mode to be set.
// ============================================================================
PUBLIC VOID hal_SimSetClockStopMode(HAL_SIM_CLOCK_STOP_MODE_T mode)
{
    HAL_TRACE(HAL_SIM_TRC, 0, "hal_SimSetClockStopMode %d", mode);
    if (mode == HAL_SIM_CLOCK_NO_STOP)
    {
        // This will be called when switching from any Auto mode to Manual
        // mode.  Normally, this should not be done.
        // Immediately switch to _MANUAL ON_ mode only and no timings will
        // be respected.
        hal_SimClrConfigBit(SCI_AUTOSTOP_EN_H | SCI_CLOCKSTOP);
    }
    else
    {
        // This will be called when switching from manual mode to
        // enable automatic clock stop.  The clock should normally
        // be on already and calling this will go to Auto Shutdown, then
        // Auto Off states.  If the clock is off, the SCI will go directly to
        // Auto Off.
        if (mode == HAL_SIM_CLOCK_STOP_L)
        {
            hal_SimClrConfigBit(SCI_STOP_LEVEL);
        }
        else // HAL_SIM_CLOCK_STOP_H
        {
            hal_SimSetConfigBit(SCI_STOP_LEVEL);
        }
        hal_SimSetConfigBit(SCI_AUTOSTOP_EN_H | SCI_CLOCKSTOP);
    }
}


// ============================================================================
// hal_SimTxByteRxByte
// ----------------------------------------------------------------------------
/// Starts a SIM transfer.
/// A sim transfer consists in sending nTxByte and receiving an answer of 
/// nRxByte.
/// At the end of the transfer, an interrupt is sent, which is recognized
/// by the user handler as #HAL_SIM_DMA_SUCCESS.
/// 
/// @param nTxByte Number of byte to send (Size of the command)
/// @param pTxByte Pointer to the buffer containing the command to send
/// @param nRxByte Number of byte to receive (Size of the answer)
/// @param pRxByte Pointer to the buffer where to store the answer
/// @param stopClock In case of an operation consisting in two commands,
/// we do not want to stop the sim clock between the two commands. In that
/// case we set this parameter to \c FALSE for the first command. 
/// Setting it to \c TRUE will stop the clock after the command (For the 
/// second command of the pair or a standalone command).
// ============================================================================
PUBLIC VOID hal_SimTxByteRxByte(
             UINT16 nTxByte, CONST UINT8* pTxByte,
             UINT16 nRxByte, UINT8* pRxByte,
             BOOL stopClock)

{
    UINT32 txCnt;

    HAL_PROFILE_FUNCTION_ENTER(hal_SimTxByteRxByte);
    // Set resource as active
    // Rq: The setting of the divider is handled by the frequency 
    // scaling mechanism.
    hal_SysRequestFreq(HAL_SYS_FREQ_SIM, HAL_SYS_FREQ_26M, hal_SimUpdateDivider);
    HAL_TRACE(HAL_SIM_TRC, 0, "hal_SimTxByteRxByte nTxByte=%d, nRxByte=%d, stopClock=%d", nTxByte, nRxByte, stopClock);

#ifdef GALLITE_IS_8806
#ifdef CT1129_OLD_SIM
    if(!g_ResetIndex)
	{
	extern UINT8 g_SimIndex ;

	extern UINT16 g_palEtuParaF[];
	extern UINT8   g_palEtuParaD[];
	extern UINT8  g_palEtuClkInvFlag[];
	extern HAL_SIM_CLOCK_STOP_MODE_T g_palSimClkStopMode[] ;
	extern UINT8 g_TS[];

	while( (hwp_sci->Status & SCI_AUTOSTOP_RXCOUNTER_MASK) != 0);

    // Set resource as active
    // Rq: The setting of the divider is handled by the frequency 
    // scaling mechanism.

    UINT32 cri_status = hal_SysEnterCriticalSection();

	pmd_SelectSimCard( (g_SimIndex +2) & 3);

	hal_SimResetSys();

	hal_SimOpen();

	hal_SimSetEtuDuration(g_palEtuParaF[g_SimIndex], g_palEtuParaD[g_SimIndex],  g_palEtuClkInvFlag[g_SimIndex]);
	hal_SimSetClockStopMode(g_palSimClkStopMode[g_SimIndex]);

	hal_SimSetDataConvFormat(g_TS[g_SimIndex]);

	hal_HstSendEvent(SYS_EVENT,hwp_sci->Status & SCI_AUTOSTOP_RXCOUNTER_MASK);

	pmd_SelectSimCard(g_SimIndex);

	hal_SysExitCriticalSection(cri_status); 
    }
#endif    
#endif

    if (nTxByte <= N_TX_CMD_BYTES)
    {
        if (nRxByte <= 4)
        {
            HAL_PROFILE_FUNCTION_ENTER(hal_SimTxByteRxByte_nRxBytebl4);
            g_halRxDmaInProgress = 0;
            g_halIfcChan = hal_SimSetNRxBytes(nRxByte, nRxByte, pRxByte, FALSE, stopClock);
            HAL_PROFILE_FUNCTION_EXIT(hal_SimTxByteRxByte_nRxBytebl4);	
        }
        else
        {
            HAL_PROFILE_FUNCTION_ENTER(hal_SimTxByteRxByte_nRxByteab4);
            // Hardware is not supposed to be able to cope with such bigs xfers ...
            // MAX_RX_BYTES being big enough to handle all requets ...
            HAL_ASSERT(nRxByte<=MAX_RX_BYTES,"Rx size too big: %d", nRxByte);
            
            g_halRxDmaInProgress = 1;
            g_halIfcChan = hal_SimSetNRxBytes(nRxByte, nRxByte, pRxByte, TRUE, stopClock);
            HAL_PROFILE_FUNCTION_EXIT(hal_SimTxByteRxByte_nRxByteab4);
        }

        // Don't use DMA for Tx
        for (txCnt = 0; txCnt < nTxByte; ++txCnt)
        {
            hal_SimTxByte(pTxByte[txCnt]);
        }
    }
    else
    {
        g_halRxDmaInProgress = 0;
        hal_SimSetNRxBytes(nRxByte, nRxByte, pRxByte, FALSE, stopClock);
        g_halIfcChan = hal_SimSendNTxBytes(nTxByte, pTxByte);
    }
    g_halRxBuf = pRxByte;

#ifdef GALLITE_IS_8806
    g_ResetIndex = 1;
#endif	
    HAL_PROFILE_FUNCTION_EXIT(hal_SimTxByteRxByte);
}


// ============================================================================
// hal_SimSetWorkWaitingTime
// ----------------------------------------------------------------------------
/// Set the work waiting time.
/// A timeout will be generated when the WWT is exceeded.  The WWT is calculated 
/// by:
/// <code> WWT = 960 x WI x (F/Fi) </code>,
/// where Fi is the main SCI clock frequency (3-5 MHz) and F is 372 before an 
/// enhanced PPS and 512 after an enhanced PPS. \n
/// The SCI_WI value must be calculated as follows:
/// <code> SCI_WI = WI * D </code>
/// Thus, by default (WI = 10) this value needs to be set to 10 before an EPPS,
/// but needs to be scaled to WI*D=80 after the EPPS procedure.
/// 
/// The hal driver is in charge of checking the time between each received sim
/// character. The default value (10) is not initialised by HAL.
/// 
/// @param wi WI factor used to set the WWT according to the previous formula.
// ============================================================================
PUBLIC VOID hal_SimSetWorkWaitingTime(UINT8 wi)
{
   hal_SimSetWwi(wi);
}


// ============================================================================
// hal_SimSetEtuDuration
// ----------------------------------------------------------------------------
/// Select the clock rate.
/// @param etuDuration F param to set.
/// @param etuDuration D param to set.
// ============================================================================
PUBLIC VOID hal_SimSetEtuDuration(UINT16 F, UINT8 D, UINT8 SciInvFlag)
{
    if (F == 372)
    {
        if(D == 8)
          hal_SimSetWwi(80);
	 else if(D == 12)
	   hal_SimSetWwi(120);
	 else if(D == 16)
	   hal_SimSetWwi(160);
        else
            hal_SimSetWwi(10);
    }
    else
    {
        if(D == 12)
            hal_SimSetWwi(120);  // the register only 8 bit
        else if(D == 16)
            hal_SimSetWwi(160);
        else
            hal_SimSetWwi(80);
    }
    
    hal_SimSetEtuBitfield(F, D, SciInvFlag);
}


// ============================================================================
// hal_SimDelayAfterEpps
// ----------------------------------------------------------------------------
/// Delay after EPPS procedure required for some SIM cards
// ============================================================================
PUBLIC VOID hal_SimDelayAfterEpps()
{
    // Wait randomly for about 100ms, that is 1640 cycles of the 16kHz delay clock.
    hal_TimDelay(HAL_SIM_ETU_WAIT MS_WAITING);
}


// ============================================================================
// hal_SimGetNumRxBytes
// ----------------------------------------------------------------------------
/// That function lets us know the number of bytes received from the SIM.
/// @return The number of bytes read from the SIM
// ============================================================================
// To know the number of bytes received by
PUBLIC UINT16 hal_SimGetNumRxBytes(VOID)
{
    return g_halRxBytes;
}


// ============================================================================
// hal_SimClockStart
// ----------------------------------------------------------------------------
/// Start the clock sent to the SIM card.
/// This function should not be necessary anymore since the
/// automatic clock stop feature should be useable.  However, this function
/// would be used when the SIM clock configuration is in _MANUAL OFF_
/// mode and will go to _AUTOMATIC ON_ mode via the _AUTO STARTUP_ state,
/// meaning that the startup timing will be respected.  This function will
/// BLOCK during the startup.  Maybe in the future, a parameter can be added
/// to the interface to select blocking or non-blocking mode.
// ============================================================================
PUBLIC VOID hal_SimClockStart(VOID)
{
    // Set resource as active
    hal_SysRequestFreq(HAL_SYS_FREQ_SIM, HAL_SYS_FREQ_26M, hal_SimUpdateDivider);

    // Configure clock mode as AUTO OFF
    // Manual Clock Enable must be _OFF_ while auto mode is used
    hal_SimSetConfigBit(SCI_CLOCKSTOP);
    hal_SimSetConfigBit(SCI_AUTOSTOP_EN_H);

    // Use the manual start pulse to start the clock
    hwp_sci->dbg = 0x00010000;

    // Wait here until the SCI clock is valid
    while ((hwp_sci->Status & SCI_CLK_RDY_H) == 0);
}


// -------------------------------------------------------------------------//
// ----------------------  Irq handling related things  --------------------//
// -------------------------------------------------------------------------//


// ============================================================================
// hal_SimIrqSetHandler
// ----------------------------------------------------------------------------
/// Set the user function called when an interrupt is generated by the SIM 
/// driver.
/// @param handler Handler function to set.
// ============================================================================
// set the user function to call when a fint occurs
PUBLIC VOID hal_SimIrqSetHandler(HAL_SIM_IRQ_HANDLER_T handler)
{
    g_halSimRegistry = handler;
}


// ============================================================================
// hal_SimIrqSetMask
// ----------------------------------------------------------------------------
/// Set the mask for the interruption. A '1' in a field of the structure means
/// an interruption must be triggered in the case if the corresponding
/// condition occurs (cf the type details for more information).
///
/// @param mask Mask to set. 
// ============================================================================
PUBLIC VOID hal_SimIrqSetMask(HAL_SIM_IRQ_STATUS_T mask)
{
    g_halSimUserMask = mask;
}


// ============================================================================
// hal_SimIrqGetMask
// ----------------------------------------------------------------------------
/// Get the IRQ mask for the SIM driver.
/// @return The Sim mask
// ============================================================================
PUBLIC HAL_SIM_IRQ_STATUS_T hal_SimIrqGetMask(VOID)
{
    return (g_halSimUserMask);
}

// ============================================================================
// hal_SimDMAReleaseIFC
// ----------------------------------------------------------------------------
/// release IFC when dma in progress
// ============================================================================
PUBLIC VOID hal_SimDMAReleaseIFC()
{
    if (g_halRxDmaInProgress == 1)
    {
        hal_SimReleaseIFC();
    }
	
    hwp_sci->Int_Clr = 1;
}

#if 0
// ============================================================================
// hal_Switch2Manual
// ----------------------------------------------------------------------------
/// This function should be called after a successful ATR ONLY when using the
/// ARG.  After an ARG, the LLI and MSBH_LSBL control lines are internally
/// driven by the ARG state machine.  When in Dual SIM mode, the state machine
/// is restarted so the internal signals can no longer be used.  This function
/// parses the Ts value and switches to manual control for the format params.
/// This should only be necessary for Dual SIM drivers.
/// @param Ts is the Ts byte that is read and which will be used to determine
/// the transfer format.
// ============================================================================
PUBLIC UINT32 hal_SimSwitch2Manual(UINT8 Ts)
{
    // Need to switch the format detection to manual mode since the state will be lost
    // when switching to the second SIM
    if (Ts == 0x23)
    {
        hal_SimClrConfigBit(SCI_LLI | SCI_MSBH_LSBL);
    }
    else
    {
        hal_SimSetConfigBit(SCI_LLI | SCI_MSBH_LSBL);
    }
    hal_SimSetConfigBit(SCI_ARG_H | SCI_AFD_EN_H);
    return (UINT32)hwp_sci->Config;
}




// FIXME: The three following functions should not be useful.
// ============================================================================
// hal_SimSetConfig
// ----------------------------------------------------------------------------
/// Set the config for the SIM driver.
/// @return The Sim config
// ============================================================================

PUBLIC VOID hal_SimSetConfig()
{
    // Clear all but upper bits
    hal_SimClrConfigBit(SCI_ENABLE |
                        SCI_PARITY_ODD_PARITY |
                        SCI_PERF |
                        SCI_FILTER_DISABLE |
                        SCI_CLOCKSTOP |
                        SCI_AUTOSTOP_EN_H |
                        SCI_MSBH_LSBL |
                        SCI_LLI |
                        SCI_PEGEN_LEN |
                        SCI_PARITY_EN |
                        SCI_STOP_LEVEL);

    hal_SimSetConfigBit(SCI_TX_RESEND_EN_H |SCI_PEGEN_LEN |SCI_ENABLE|SCI_RESET|SCI_AUTOSTOP_EN_H | SCI_CLOCKSTOP);
}

// ============================================================================
// hal_SimSetConfig
// ----------------------------------------------------------------------------
/// Set the config for the SIM driver.
/// @return The Sim config
// ============================================================================

PUBLIC VOID hal_SimSetConfInvTS()
{
    // Clear all but upper bits
    hal_SimClrConfigBit(SCI_ENABLE |
                        SCI_PARITY_ODD_PARITY |
                        SCI_PERF |
                        SCI_FILTER_DISABLE |
                        SCI_CLOCKSTOP |
                        SCI_AUTOSTOP_EN_H |
                        SCI_MSBH_LSBL |
                        SCI_LLI |
                        SCI_PEGEN_LEN |
                        SCI_PARITY_EN |
                        SCI_STOP_LEVEL);

    hal_SimSetConfigBit(SCI_TX_RESEND_EN_H |SCI_PEGEN_LEN |SCI_ENABLE|SCI_LLI |SCI_MSBH_LSBL|
                        SCI_RESET|SCI_AUTOSTOP_EN_H | SCI_CLOCKSTOP); 	
}
#endif

// ============================================================================
// hal_SimIrqHandler
// ----------------------------------------------------------------------------
/// Sim module IRQ handler, to be used exclusively by the private internal
/// IRQ module.
/// @param interruptId IRQ line causing the IRQ Id
// ============================================================================
PROTECTED VOID hal_SimIrqHandler(UINT8 interruptId)
{
    union
    {
        UINT32 cause;
        HAL_SIM_IRQ_INTERNAL_CAUSE_T formatedCause;
    } u;

    HAL_SIM_IRQ_STATUS_T usrCause = {0,0,0,0,0,0,0,0};

    UINT32 idx=0;

    HAL_TRACE(HAL_SIM_TRC, 0, "HAL_SIM:  hal_SimIrqHandler -> 0x%08x\n",hwp_sci->Int_Cause);
    //  get cause 
    u.cause = hwp_sci->Int_Cause;
    //  to clear irq
    hwp_sci->Int_Clr = u.cause;

    // "Mecanical treatment needed before calling
    // the user handler

    // Status is the _interrupt_ status
    if (u.formatedCause.argEnd)
    {
        HAL_PROFILE_FUNCTION_ENTER(hal_SimIrqHandler_argEnd);
        if (hal_SimGetArgStatus())
        {
            //HAL_TRACE(HAL_SIM_TRC | TSTDOUT, 0, "SIM:  argEnd Receiving ATR");
            g_halReceivingAtr = 1;
        } else
        {
            hal_SimReleaseIFC();
            //sim_ISR(HAL_SIM_RESET_TIMEOUT, 0);
            if (g_halSimUserMask.resetTimeout)
            {
                    usrCause.resetTimeout = 1;
                    HAL_TRACE(HAL_SIM_TRC | TSTDOUT, 0, "SIM:  argEnd Timeout");

            }
        }
        HAL_PROFILE_FUNCTION_EXIT(hal_SimIrqHandler_argEnd)	;
    }


    if (u.formatedCause.rxDone)
    {
        HAL_PROFILE_FUNCTION_ENTER(hal_SimIrqHandler_rxDone);
        g_halRxBytes = hal_SimGetRxCnt();
        if (g_halRxDmaInProgress == 0)
        {
            // Need to get the data from SCI to Rx Pointer
            for (idx = 0; idx < g_halRxBytes; ++idx)
            {
                g_halRxBuf[idx] = hal_SimGetRxByte();
            }
        }
        else
        {
            // Need to update the XCPU cache after DMA
            hal_SysInvalidateCache(g_halRxBuf, g_halRxBytes);
        }
        g_halRxDmaInProgress = 0;
        //sim_ISR(HAL_SIM_DMA_SUCCESS, g_halRxBytes);
        if (g_halSimUserMask.dmaSuccess)
        {
            usrCause.dmaSuccess = 1;
        }
        HAL_PROFILE_FUNCTION_EXIT(hal_SimIrqHandler_rxDone);
    }
    
    
    if (u.formatedCause.wwtTimeout)
    {
        // Flush the IFC here!!!
       HAL_PROFILE_FUNCTION_ENTER(hal_SimIrqHandler_wwtTimeout);
       if (g_halRxDmaInProgress == 1)
        {
            hal_SimReleaseIFC();
        }
        g_halRxBytes = hal_SimGetRxCnt(); 
        // Previous statement incorrect if multi-transfer
        // but no multitransfert here as tc is big enough
        if (g_halReceivingAtr == 1)
        {
            // Need to update the XCPU cache after DMA
            hal_SysInvalidateCache(g_halRxBuf, g_halRxBytes);

            if (g_halSimUserMask.atrSuccess)
            {
                //while (1);
                usrCause.atrSuccess = 1;
            }
            hal_SimEnableParity(TRUE);
            g_halReceivingAtr = 0;
        }
        else
        {
            if (g_halRxDmaInProgress == 0)
            {
                for (idx = 0; idx < g_halRxBytes; ++idx)
                {
                    g_halRxBuf[idx] = hal_SimGetRxByte();
                }
            }
            else
            {
                // Need to update the XCPU cache after DMA
                hal_SysInvalidateCache(g_halRxBuf, g_halRxBytes);
            }
            g_halRxDmaInProgress = 0;
            usrCause.wwtTimeout = 1;
        }
        HAL_PROFILE_FUNCTION_EXIT(hal_SimIrqHandler_wwtTimeout);
    }

    if (u.formatedCause.resendOvfl)
    {
        HAL_PROFILE_FUNCTION_ENTER(hal_SimIrqHandler_resendOvfl);
        hal_SimClrConfigBit(SCI_TX_RESEND_EN_H);

        if (g_halRxDmaInProgress == 1)
        {
            hal_SimReleaseIFC();
        }

        if (g_halSimUserMask.parityError)
        {
            usrCause.parityError = 1;
        }
        HAL_PROFILE_FUNCTION_EXIT(hal_SimIrqHandler_resendOvfl);
    }
    
    if (u.formatedCause.extraRx)
    {
        HAL_PROFILE_FUNCTION_ENTER(hal_SimIrqHandler_extraRx);
#ifdef GALLITE_IS_8806
        if(g_halRxBuf==0)
        {
            hal_SimGetRxByte();
        }
        else
#endif
        {
            g_halRxBuf[g_halRxBytes] = hal_SimGetRxByte();
            ++g_halRxBytes;
        }

        if (g_halRxDmaInProgress == 1)
        {
            hal_SimReleaseIFC();
        }

        if (g_halSimUserMask.extraRx)
        {
            usrCause.extraRx = 1;
        }
        HAL_PROFILE_FUNCTION_EXIT(hal_SimIrqHandler_extraRx);
    }

    if (g_halSimRegistry)
        g_halSimRegistry(usrCause);
}


#if 0
// That is what the stack handler will have to do ...
 // "Mecanical treatment needed before calling
    // the user handler
    
    // Status is the _interrupt_ status
    if (formatedCause.argEnd)
    {
        if (hal_SimGetArgStatus())
        {
            g_halReceivingAtr = 1;
        } else
        {
            hal_SimReleaseIFC();
            //sim_ISR(SIM_RESET_TIMEOUT, 0);
            usrCause = SIM_RESET_TIMEOUT;
        }
    }
    if (formatedCause.rxDone)
    {
        // CT_sci_ReleaseIFC();
        g_halRxBytes = hal_SimGetRxCnt();
        if (g_halRxDmaInProgress == 0)
        {
            // Need to get the data from SCI to Rx Pointer
            for (idx = 0; idx < g_halRxBytes; ++idx)
            {
                g_halRxBuf[idx] = hal_SimGetRxByte();
            }
        }
        else
        {
            // Need to update the XCPU cache after DMA
            ///  @todo Change this with the invalidate 
///             cache instruction 
            for (idx = 0; idx < g_halRxBytes; ++idx)
            {
                g_halRxBuf[idx] = *(UINT8*)KSEG1((UINT32)g_halRxBuf + idx);
            }
        }
        g_halRxDmaInProgress = 0;
        //sim_ISR(SIM_DMA_SUCCESS, g_halRxBytes);
        usrCause = SIM_DMA_SUCCESS;
    }
    
    
    if (formatedCause.wwtTimeout)
    {
        // Flush the IFC here!!!
        if (g_halRxDmaInProgress == 1)
        {
            hal_SimReleaseIFC();
        }
        g_halRxBytes = hal_SimGetRxCnt(); /// @todo incorrect if multi-transfer
        if (g_halReceivingAtr == 1)
        {
            for (idx = 0; idx < g_halRxBytes; ++idx)
            {
                g_halRxBuf[idx] = *(UINT8*)KSEG1((UINT32)g_halRxBuf + idx);
            }
            usrCause = SIM_ATR_SUCCESS;
            hal_SimEnableParity(TRUE);
            g_halReceivingAtr = 0;
        } else
        {
            if (g_halRxDmaInProgress == 0)
            {
                for (idx = 0; idx < g_halRxBytes; ++idx)
                {
                    g_halRxBuf[idx] = hal_SimGetRxByte();
                }
            }
            else
            {
                for (idx = 0; idx < g_halRxBytes; ++idx)
                {
                    g_halRxBuf[idx] = *(UINT8*)KSEG1((UINT32)g_halRxBuf + idx);
                }
            }
            g_halRxDmaInProgress = 0;
            usrCause = SIM_WWT_TIMEOUT;
        }
    }
    if (formatedCause.resendOvfl)
    {
        hal_SimClrConfigBit(SCI_TX_RESEND_EN_H);
        usrCause = SIM_PARITY_ERROR;
    }
    
    if (formatedCause.extraRxX)
    {
        g_halRxBuf[g_halRxBytes] = hal_SimGetRxByte();
        ++g_halRxBytes;
        usrCause = SIM_EXTRA_RX;    
    }
#endif

