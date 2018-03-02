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
/// @file hal_ana_gpadc.h
///                                                                           //
/// Contains the GPADC driver Implementation.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// FIXME Implement for Gallite !!

#include "chip_id.h"
#include "cs_types.h"

#include "global_macros.h"
#if (!CHIP_HAS_ASYNC_TCU)
#include "gpadc.h"
#else // CHIP_HAS_ASYNC_TCU
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8810)
#include "cfg_regs.h"
#else
#include "pwm.h"
#endif
#endif // CHIP_HAS_ASYNC_TCU

#include "hal_sys.h"
#include "hal_ana_gpadc.h"
#include "hal_gpio.h"

#include "halp_ana_gpadc.h"
#include "halp_sys.h"
#include "halp_debug.h"
//#include "halp_calib.h"

//#include "calib_m.h"
#include "pmd_m.h"
#include "pwm.h"
#include "hal_pwm.h"
#include "hal_host.h"


#if (CHIP_HAS_ASYNC_TCU)
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
BOOL pmd_TsdTscEnabled(VOID);
VOID pmd_TsdEnableTsc(BOOL enable);
#endif
#endif


#define GPADC_FAST_CLOSE 1

// Battery voltage = gpadc voltage * 3
#define HAL_ANA_GPADC_MVOLT_A   900

#define HAL_ANA_GPADC_MVOLT_B   1400


PRIVATE INT32 g_halGpadcSlope = 0;
PRIVATE INT32 g_halGpadcIntcpt = 0;
PRIVATE BOOL  g_halGpadcCalibrated = FALSE;

PRIVATE UINT32 g_halgpadcCalibHigh = 0;
PRIVATE UINT32 g_halgpadcCalibLow = 0;

//#ifdef CHIP_IS_MODEM
#if (defined(CHIP_IS_MODEM)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_8809P))
PRIVATE UINT32 g_halGpadcWakeupTime = 0;
#endif
#if (!CHIP_HAS_ASYNC_TCU)
PRIVATE UINT16 g_halGpadcLastVal[HAL_ANA_GPADC_CHAN_QTY] = 
    {
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE,
        HAL_ANA_GPADC_BAD_VALUE,
    };
#endif

// using HAL_ANA_GPADC_ATP_QTY value for disabled channels 
// (so min atp is HAL_ANA_GPADC_ATP_QTY when all disabled!)
PRIVATE UINT8 g_halGpadcAtpConfig[HAL_ANA_GPADC_CHAN_QTY] = 
    {
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
#if (CHIP_HAS_ASYNC_TCU)
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
        HAL_ANA_GPADC_ATP_QTY,
#endif
    };


#if !(defined(FPGA))
// =============================================================================
// hal_AnaGpadcPowerOn
// -----------------------------------------------------------------------------
/// Power on the GPADC hardware.
// =============================================================================
PRIVATE BOOL hal_AnaGpadcPowerOn(VOID)
{
    // Request freq resource
    hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_26M, NULL);

    UINT32 status = hal_SysEnterCriticalSection();
    hwp_sysCtrl->per_module_en |= SYS_CTRL_GPADC_EN;
    hal_SysExitCriticalSection(status);

    return TRUE;
}


// =============================================================================
// hal_AnaGpadcPowerOff
// -----------------------------------------------------------------------------
/// Power off the GPADC hardware.
// =============================================================================
PRIVATE VOID hal_AnaGpadcPowerOff(VOID)
{
    UINT32 status = hal_SysEnterCriticalSection();
       
    hal_SysExitCriticalSection(status);
    hwp_sysCtrl->per_module_en &= ~SYS_CTRL_GPADC_EN;
    // Release freq resource.
    hal_SysRequestFreq(HAL_SYS_FREQ_GPADC, HAL_SYS_FREQ_32K, NULL);
}
#endif // !(FPGA || CHIP_HAS_ASYNC_TCU)


// =============================================================================
// hal_AnaGpadcMinAtp
// -----------------------------------------------------------------------------
/// @param pCh Pointer to store the GPADC channel with minimum Atp value.
/// @return minimum Atp value for configured channels, 
/// #HAL_ANA_GPADC_ATP_QTY when all chnnels are closed.
///
/// Use the global configuration variable \c g_halGpadcAtpConfig.
// =============================================================================
PRIVATE UINT8 hal_AnaGpadcMinAtp(HAL_ANA_GPADC_CHAN_T* pCh)
{
    HAL_ANA_GPADC_CHAN_T ch;
    UINT8 minAtp = HAL_ANA_GPADC_ATP_QTY;

    for (ch=HAL_ANA_GPADC_CHAN_0; ch<HAL_ANA_GPADC_CHAN_QTY; ch++)
    {
        if (g_halGpadcAtpConfig[ch] < minAtp)
        {
            minAtp = g_halGpadcAtpConfig[ch];
            if (pCh != NULL)
            {
                *pCh = ch;
            }
        }
    }

    return minAtp;
}


// =============================================================================
// hal_AnaGpadcGpadc2Volt
// -----------------------------------------------------------------------------
/// Convert the value measured by the GPADC into mV, using the calibration
/// parameters.
///
/// @param gpadcVal Value read from the GPADC hardware module.
/// @return The corresponding measured tension, in mV.
// =============================================================================
PROTECTED HAL_ANA_GPADC_MV_T hal_AnaGpadcGpadc2Volt(UINT16 gpadcVal)
{
    INT32 y1, y2, x1, x2;
    INT32 voltage;
    HAL_ANA_GPADC_MV_T rVoltage;

    if (gpadcVal == HAL_ANA_GPADC_BAD_VALUE)
    {
        // Bad values from the GPADC are still Bad Values
        // for the voltage-speaking user.
        return HAL_ANA_GPADC_BAD_VALUE;
    }
    else
    {
        if (!g_halGpadcCalibrated)
        {
	        y1 = HAL_ANA_GPADC_MVOLT_A*1000;
	        y2 = HAL_ANA_GPADC_MVOLT_B*1000;
            x1 = g_halgpadcCalibLow;
            x2 = g_halgpadcCalibHigh;

            if(g_halgpadcCalibLow<395||g_halgpadcCalibLow>455) // calib low invalid 425+-30
            {
                x1 = 12;
                y1 = 0;
            }
            if(g_halgpadcCalibHigh<625 || g_halgpadcCalibHigh>695) // calib high invalid 660+-35
            {
                x2 = 12;
                y2 = 0;
            }

            if(y1!=0&&y2!=0&& // both valid but diff invlid
                (g_halgpadcCalibLow+220>g_halgpadcCalibHigh || 
                g_halgpadcCalibLow+270<g_halgpadcCalibHigh)) // diff invalid 100+-35
            {
                if(g_halgpadcCalibLow<415||g_halgpadcCalibLow>435) // calib low invalid 425+-15
                {
                    x1 = 12;
                    y1 = 0;
                }
                if(g_halgpadcCalibHigh<645 || g_halgpadcCalibHigh>675) // calib high invalid 660+-15
                {
                    x2 = 12;
                    y2 = 0;
                }
            }
            
            if((y1==0&&y2==0)) // both invalid
            {
                g_halGpadcSlope = 2040; // according to (4.2,0xaa) and (2.7,0x6d)
                g_halGpadcIntcpt = 12;
            }
            else
            {
                g_halGpadcSlope = (y2-y1)/(x2-x1);
                g_halGpadcIntcpt = ((y1*x2)-(x1*y2))/((x2-x1)*1000);
            }


            g_halGpadcCalibrated = TRUE;

#if 0
#if defined(GALLITE_IS_8805)
	        //x1 = g_halCalibration->bb->gpadc.sensorGainA;
	        //x2 = g_halCalibration->bb->gpadc.sensorGainB;
#else // 8806 or later
#if defined(GALLITE_IS_8806)
	        if((pmd_GetBatteryGpadcChannel() == HAL_ANA_GPADC_CHAN_1) && g_halgpadcCalibHigh)
#else // 8808 or later
	        if((pmd_GetBatteryGpadcChannel() == HAL_ANA_GPADC_CHAN_7) && g_halgpadcCalibHigh)
#endif
	        {
	            x1 = g_halgpadcCalibLow;
	            x2 = g_halgpadcCalibHigh;

        		//x1 &= 0x3ff;
        		//x2 &= 0x3ff;			
	        }
	        else
	        {
	            //x1 = g_halCalibration->bb->gpadc.sensorGainA;
	            //x2 = g_halCalibration->bb->gpadc.sensorGainB;
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)||(CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
				x1 = 600;
				x2 = 700;
#else
				x1 = 0x26b;
				x2 = 0x2fe;
#endif
	        }
#endif // !GALLITE_IS_8805

            HAL_ASSERT(x1 < x2, "Invalid GPADC calib value: %d, %d", x1, x2);

			//=========================================================//
			// @ By ZM
			// Equation 1: x1*g_halGpadcSlope + g_halGpadcIntcpt = y1
			// Equation 2: x2*g_halGpadcSlope + g_halGpadcIntcpt = y2
			//=========================================================//		
            g_halGpadcSlope = (y2-y1)/(x2-x1);
            g_halGpadcIntcpt = ((y1*x2)-(x1*y2))/((x2-x1)*1000);
            g_halGpadcCalibrated = TRUE;
#endif
        }

        voltage = (((g_halGpadcSlope*gpadcVal)/1000)+(g_halGpadcIntcpt));
        if (voltage < 0)
        {
            rVoltage = 0;   // Should never be negative
        }
        else
        {
            rVoltage = (HAL_ANA_GPADC_MV_T)voltage;
        }
        //HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGpadc2Volt raw=%d val=%dmV",gpadcVal, voltage);
        return rVoltage;
    }
}



// =============================================================================
// hal_AnaGpadcDisable
// -----------------------------------------------------------------------------
/// Really close the GPADC hardware and stop periodic measurements.
/// parameters.
// =============================================================================
PRIVATE VOID hal_AnaGpadcDisable(VOID)
{
#if defined(FPGA)

    // GPADC not implemented on FPGA

#else

    // Power off ABB GPADC
    hal_AnaGpadcPowerOff();

#endif // !FPGA
    return;
}


// =============================================================================
// hal_AnaGpadcEnable
// -----------------------------------------------------------------------------
/// Really open the GPADC hardware and start periodic measurements.
/// If no channels are enabled, it will call #hal_AnaGpadcDisable to ensure
/// the GPADC hardware is disabled.
///
/// Use the global configuration variable \c g_halGpadcConfig.
// =============================================================================
PRIVATE VOID hal_AnaGpadcEnable(VOID)
{
#if defined(FPGA)

    // GPADC not implemented on FPGA

#else

    // If already enabled, the driver handle that situtation by
    // becoming blocking as long as the GPADC is not ready for the new
    // setting. (few 26Mhz clock cycles)
    //UINT32 gpadcControl = 0;
    UINT8 minAtp = hal_AnaGpadcMinAtp(NULL);

    // Only open the GPADC if at least one channel is required.
    if (minAtp != HAL_ANA_GPADC_ATP_QTY)
    {
        // If a channel is already enabled, wait for the fitted time
        // to write the register.

        // Only needed for the first time
        // Power on ABB GPADC.
        hal_AnaGpadcPowerOn();

        // Because hardware will actually change it's ATP after the current perriod
        // is elapsed, switching from slow to fast will actually be long:
        // for example switching from GPADC_ATP_2S to GPADC_ATP_1MS will take 2s
        // Limit the atp to 10ms to hide some of this effect
        /// @todo: put this limit in HAL TGT config
        if (minAtp > HAL_ANA_GPADC_ATP_10MS)
        {
            minAtp = HAL_ANA_GPADC_ATP_122US; //HAL_ANA_GPADC_ATP_10MS;
        }   
    }
    else
    {
        // No channels are enabled, so ensure the GPADC module is disabled.
        hal_AnaGpadcDisable();
    }
    

#endif // !FPGA
    return;
}


// =============================================================================
// hal_AnaGpadcOpen
// -----------------------------------------------------------------------------
/// Open a GPADC channel and start periodic measurements.
///
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// That function starts measurements on one of the 4 periodic GPADC channels.
/// The ATP (Acquisition Time Period)is common, so the minimal value of atp
/// is used. Using atp value above HAL_ANA_GPADC_ATP_100MS allows to enter 
/// low power between the measure.
/// The measured channels are selected through
/// #HAL_ANA_GPADC_CFG_T parameter.
///
/// @param channel Channel to enable
/// @param atp Atp value required for that channel
// =============================================================================
PUBLIC VOID hal_AnaGpadcOpen(HAL_ANA_GPADC_CHAN_T channel, HAL_ANA_GPADC_ATP_T atp)
{
    //HAL_ASSERT(g_halCalibration == calib_GetPointers(), 
        //    "GPADC Opening with HAL calib pointer inconsistency");

    if (channel >= HAL_ANA_GPADC_CHAN_QTY)
    {
        return;
    }

    hwp_pmu_intf->gpadc_ctrl1 |= PMU_INTF_RESETN_GPADC_REG|PMU_INTF_RESETN_GPADC_DR;
    hwp_pmu_intf->gpadc_ctrl1 |= PMU_INTF_RESETN_GPADC;

    // store the atp to the configuration array:
    g_halGpadcAtpConfig[channel] = atp;

    // enable the GPADC hardware

    hal_AnaGpadcEnable();

    // Init GPADC: Set the measurement interval to the minimum value: 125 us
    pmd_GpadcSetInterval(0);
    pmd_GpadcEnableChannel(channel);
    // Wait for 4 ticks ~= 244 us
    hal_TimDelay(4);
}


// =============================================================================
// hal_AnaGpadcClose
// -----------------------------------------------------------------------------
/// Close a GPADC channel. 
///
/// This function release the resource to #HAL_SYS_FREQ_32K when all channels
/// are closed.
///
/// This function stops the periodic measurements on a particular channel.
/// It close the GPADC hardware when all channels are disabled.
// =============================================================================
PUBLIC VOID hal_AnaGpadcClose(HAL_ANA_GPADC_CHAN_T channel)
{
    if (channel >= HAL_ANA_GPADC_CHAN_QTY)
    {
        return;
    }

    // store HAL_ANA_GPADC_ATP_QTY to the configuration array to disable the channel:
    g_halGpadcAtpConfig[channel] = HAL_ANA_GPADC_ATP_QTY;


    // Disable the GPADC if no channel is required
    UINT8 minAtp = hal_AnaGpadcMinAtp(NULL);
    if (minAtp == HAL_ANA_GPADC_ATP_QTY)
    {
        pmd_GpadcDisable();
    }

    hal_AnaGpadcDisable();
}


// =============================================================================
// hal_AnaGpadcGet
// -----------------------------------------------------------------------------
/// Return the more up-to-date value measure for the given channel in mV. 
/// If the conversion is not finished yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
///
/// Once the first measured has been correctly done, there will always
/// be a value ready to read for all the enabled channel.
///
/// @param channel Channel whose more up to date measured value
/// will be returned.
/// @return Measured value, in mV, or #HAL_ANA_GPADC_BAD_VALUE if no result is 
/// available yet.
// =============================================================================
PUBLIC HAL_ANA_GPADC_MV_T hal_AnaGpadcGet(HAL_ANA_GPADC_CHAN_T channel)
{
    HAL_ANA_GPADC_MV_T mv = hal_AnaGpadcGpadc2Volt(hal_AnaGpadcGetRaw(channel));
    HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGet ch=%d val=%dmV",channel, mv);
    return (mv);
}

// =============================================================================
// hal_AnaGpadcGetRaw
// -----------------------------------------------------------------------------
/// Return the more up do date value measured for the specified GPADC channel,
/// if available, as the raw value read in the GPADC register. (no conversion
/// towards voltage is done.)
/// If no result is available because the conversion is not ready
/// yet, #HAL_ANA_GPADC_BAD_VALUE is returned.
/// 
/// @param channel Channel whose more up to date measured value will be returned.
/// @return Raw measured value for this channel by the GPADC,  or 
/// #HAL_ANA_GPADC_BAD_VALUE if no result is available yet.
// =============================================================================
UINT32 g_debugDumpGpadcRawValue = 0;
PUBLIC UINT16 hal_AnaGpadcGetRaw(HAL_ANA_GPADC_CHAN_T channel)
{
    if (channel >= HAL_ANA_GPADC_CHAN_QTY)
    {
        return HAL_ANA_GPADC_BAD_VALUE;
    }

#if defined(FPGA)

    // GPADC not implemented on FPGA
    return HAL_ANA_GPADC_BAD_VALUE;

#else

#if (CHIP_HAS_ASYNC_TCU)

    if (g_halGpadcAtpConfig[channel] == HAL_ANA_GPADC_ATP_QTY)
    {
        return HAL_ANA_GPADC_BAD_VALUE;
    }

    UINT32 reg;
    UINT16 rdata = HAL_ANA_GPADC_BAD_VALUE;
    UINT32 scStatus, keysense, int_measure_reg, per_module_en_reg;

    scStatus = hal_SysEnterCriticalSection();
    //disable keysense first
    keysense = hwp_pmu_intf->keysense_ctrl1;
    int_measure_reg = hwp_pmu_intf->pmu_intf_imr;
    per_module_en_reg = hwp_sys_ctrl->per_module_en;
    
    hwp_pmu_intf->keysense_ctrl1 &= ~PMU_INTF_KEY_ENABLE;
    hwp_pmu_intf->pmu_intf_imr   |= PMU_INTF_KEY_MEASURE_INT_MASK;
    
    hwp_sys_ctrl->per_module_en |= SYS_CTRL_GPADC_EN_ENABLE;
    
    hwp_pmuc->pmu_ana_rsvd = 0x6;
    
    for(int i=0; i<HAL_ANA_GPADC_CHAN_QTY; i++)
    {
        hwp_gpadc->gpadc_data_ch[i] |= GPADC_GPADC_EN;//enable gpadc, this is a hardware bug
    }
    
    hwp_pmu_intf->misc_ctrl |= PMU_INTF_VBAT_MONITOR_EN;
    pmd_GpadcEnableChannel(channel);
    hwp_pmu_intf->gpadc_ctrl1 |= PMU_INTF_GPADC_START_REG|PMU_INTF_GPADC_START_DR;
    hwp_pmu_intf->gpadc_ctrl1 |= PMU_INTF_GPADC_ENABLE;

    hal_TimDelay(4);
    
    do
    {   
        reg = hwp_gpadc->gpadc_data_ch[HAL_ANA_GPADC_CHAN_0]; //only channel0 is ok, this is a hardware bug

        if (reg & GPADC_GPADC_VALUE_VALID)
        {
            rdata = (UINT16)GET_BITFIELD(reg, GPADC_GPADC_VALUE_BIT);
        }
        else
        {
            rdata = HAL_ANA_GPADC_BAD_VALUE;
        }
    } while (rdata == HAL_ANA_GPADC_BAD_VALUE);

    //hal_HstSendEvent(SYS_EVENT, 0x20180812);
    //hal_HstSendEvent(SYS_EVENT, rdata);
    //after measure, we restore origin status
    for(int i=0; i<HAL_ANA_GPADC_CHAN_QTY; i++)
    {
        hwp_gpadc->gpadc_data_ch[i] &= ~GPADC_GPADC_EN;
    }

    hwp_pmu_intf->gpadc_ctrl1 &= ~(PMU_INTF_GPADC_START_REG|PMU_INTF_GPADC_START_DR);
    hwp_pmu_intf->gpadc_ctrl1 &= ~PMU_INTF_GPADC_ENABLE;
    hwp_pmu_intf->gpadc_ctrl1 &= ~(PMU_INTF_GPADC_CHANNEL_EN_DR|PMU_INTF_GPADC_CHANNEL_EN_REG_MASK);
    hwp_pmu_intf->misc_ctrl &= ~PMU_INTF_VBAT_MONITOR_EN;
    hwp_pmuc->pmu_ana_rsvd = 0x7;

    //restore keysense
    hwp_pmu_intf->keysense_ctrl1 = keysense;
    hwp_pmu_intf->pmu_intf_imr = int_measure_reg;
    hwp_sys_ctrl->per_module_en = per_module_en_reg;
    hal_SysExitCriticalSection(scStatus);

    if (g_debugDumpGpadcRawValue)
    {
        HAL_TRACE(HAL_IO_TRC,0,"hal_AnaGpadcGetRaw ch=%d val=0x%x",channel, rdata);
    }

    return rdata;

#endif // CHIP_HAS_ASYNC_TCU

#endif // !FPGA
}

// =============================================================================
// hal_AnaGpadcCalibUpdateBandgap
// -----------------------------------------------------------------------------
/// Update the bandgap of the GPADC macro.
// =============================================================================
PUBLIC VOID hal_AnaGpadcCalibUpdateBandgap(VOID)
{
}


// =======================================================
// hal_AnaGpadcSleep
// -------------------------------------------------------
/// called in #hal_LpsSleep to try to shut down the GPADC
/// and release the resource. (only if atp >= 100MS)
// =======================================================
PROTECTED VOID hal_AnaGpadcSleep(VOID)
{
#if defined(FPGA)

    // GPADC not implemented in FPGA
    return;

#else

#if (CHIP_HAS_ASYNC_TCU)

    pmd_GpadcDisable();
    hal_AnaGpadcPowerOff();
#endif // CHIP_HAS_ASYNC_TCU

#endif // !FPGA
}


// =======================================================
// hal_AnaGpadcWakeUp
// -------------------------------------------------------
/// called in #hal_LpsSleep to resume the GPADC operation
/// can take back the resource (if GPADC is open)
// =======================================================
PROTECTED VOID hal_AnaGpadcWakeUp(VOID)
{
#if defined(FPGA)

    // GPADC not implemented in FPGA
    return;

#else

#if (CHIP_HAS_ASYNC_TCU)

    HAL_ANA_GPADC_CHAN_T ch;

    // Only enable the GPADC if at least one channel is required.
    UINT8 minAtp = hal_AnaGpadcMinAtp(&ch);
    if (minAtp != HAL_ANA_GPADC_ATP_QTY)
    {
        pmd_GpadcEnableChannel(ch);
    }
    hal_AnaGpadcPowerOn();
#ifdef CHIP_IS_MODEM
    g_halGpadcWakeupTime = hal_TimGetUpTime();
#endif

#endif // CHIP_HAS_ASYNC_TCU

#endif // !FPGA
}


// =============================================================================
// hal_AnaGpadcGetCalibHigh
// -----------------------------------------------------------------------------
/// Get GPADC Calib High sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibHigh(VOID)
{
    return &g_halgpadcCalibHigh;
}


// =============================================================================
// hal_AnaGpadcGetCalibLow
// -----------------------------------------------------------------------------
/// Get GPADC Calib low sample
// =============================================================================
PUBLIC UINT32 * hal_AnaGpadcGetCalibLow(VOID)
{
    return &g_halgpadcCalibLow;
}

#ifndef CHIP_5856E
#if (CHIP_HAS_ASYNC_TCU)
// =============================================================================
// hal_AnaGpadcGetTscX
// -----------------------------------------------------------------------------
/// This function get the touch screen X coordinate value 
// =============================================================================
PUBLIC UINT16 hal_AnaGpadcGetTscX(VOID)
{
    UINT32 reg;
    UINT16 data;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857 && CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8810)
    reg = hwp_configRegs->TSC_DATA;

    if (reg & CFG_REGS_TSC_X_VALUE_VALID)
    {
        data = (UINT16)GET_BITFIELD(reg, CFG_REGS_TSC_X_VALUE_BIT);
    }
    else
    {
        data = HAL_ANA_GPADC_BAD_VALUE;
    }
#else // !8810
    reg = hwp_pwm->TSC_DATA;

    if (reg & PWM_TSC_X_VALID)
    {
        data = (UINT16)GET_BITFIELD(reg, PWM_TSC_X_VALUE);
    }
    else
    {
        data = HAL_ANA_GPADC_BAD_VALUE;
    }
#endif

	return data;
#endif
}


// =============================================================================
// hal_AnaGpadcGetTscY
// -----------------------------------------------------------------------------
/// This function get the touch screen Y coordinate value 
// =============================================================================
PUBLIC UINT16 hal_AnaGpadcGetTscY(VOID)
{
    UINT32 reg;
    UINT16 data;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5857 && CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8810)
    reg = hwp_configRegs->TSC_DATA;

    if (reg & CFG_REGS_TSC_Y_VALUE_VALID)
    {
        data = (UINT16)GET_BITFIELD(reg, CFG_REGS_TSC_Y_VALUE_BIT);
    }
    else
    {
        data = HAL_ANA_GPADC_BAD_VALUE;
    }
#else // !8810
    reg = hwp_pwm->TSC_DATA;

    if (reg & PWM_TSC_Y_VALID)
    {
        data = (UINT16)GET_BITFIELD(reg, PWM_TSC_Y_VALUE);
    }
    else
    {
        data = HAL_ANA_GPADC_BAD_VALUE;
    }
#endif

	return data;
#endif
}
#endif
#endif


