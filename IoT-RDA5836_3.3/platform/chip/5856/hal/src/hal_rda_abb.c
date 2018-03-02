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
/// @file hal_rda_abb.c                                                           //
///                                                                           //
/// This file decribes the RDA Analog Base Band driver API.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "sxr_tls.h"

#include "halp_rda_abb.h"
#include "halp_debug.h"
#include "hal_ispi.h"
#include "hal_host.h"
#include "hal_rda_abb.h"
#include "hal_rda_audio.h"
#include "rfd_xcv.h"
#include "halp_abb_regs.h"

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE) // 8808 or later
#include "hal_aif.h"
extern VOID aud_CodecCommonSetRateStg(HAL_AIF_FREQ_T sampleRate);
#endif

#include "hal_mem_map.h"
#include "hal_map.h"
#include "global_macros.h"
#include "sys_irq.h"
#include "cos.h"
#include "opal.h"

#define ABB_REG_READ_WAITING_TIME 5

#if 0
// Power control
PROTECTED UINT16 g_halAbbCodecPowerCtrl = 
// The registers of codec digital part (starting from 
// CODEC_DIG_MIN_REG_INDEX), is accessible and sustainable only
// when codec digital part is out of reset (or REF is powered on).
    ABB_RSTN_CODEC_DR|
    ABB_RSTN_CODEC;

#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)

// MIC mode
PROTECTED CONST UINT16 gc_halAbbCodecMiscSetting = 
    ABB_MIC_MODE_EN|
#ifdef MIC_CAPLESS_MODE
    ABB_MICCAPLESS_MODE_EN|
#else
    ABB_MICCAP_MODE_EN|
#endif
    ABB_MASH_EN_ADC|
    ABB_MICBIAS_SEL;

// HP cap mode
PROTECTED CONST UINT16 gc_halAbbCodecPllSetting1 = 
    ABB_PD_HP_CAPLESS_DR;

#else // 8809 or later

#ifdef ABB_DISABLE_NEG_MODE
// HP cap mode
PROTECTED CONST UINT16 gc_halAbbCodecPllSetting1 = 
    ABB_PD_NEG1_REG|ABB_PD_NEG1_DR;
#endif

// MIC mode
PROTECTED CONST UINT16 gc_halAbbCodecMicSetting = 
    ABB_MB_SEL_BIT(2)|
#ifndef MIC_CAPLESS_MODE
    ABB_MICCAP_MODE_EN|
    ABB_MIC_CHOP_EN|
#endif
    ABB_MASH_EN_ADC;

// VCOM VREF setting
PRIVATE CONST UINT16 gc_halAbbCodecLdoSetting2 = 
    ABB_VCOM_MODE(0)|
    ABB_VREF_MODE(5)|
    ABB_ISEL_BIT_B(0)|
    ABB_RESET_DAC_REG(2)|
    ABB_CLASSG_ENABLE_REG;

// HP VCOM setting
PRIVATE CONST UINT16 gc_halAbbCodecMiscSetting =
#ifdef CHIP_DIE_8809E
ABB_MIC_MODE_EN|
#else
#ifdef ABB_DISABLE_NEG_MODE
(1<<15)|
#endif
#endif
0
;

#ifndef ABB_DISABLE_NEG_MODE
PRIVATE CONST UINT16 gc_halAbbCodecNegSetting1 = 1<<9;
PRIVATE CONST UINT16 gc_halAbbCodecNegSetting2 = ~(1<<10);

#endif

#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
// Codec clock setting
PUBLIC CONST UINT16 gc_halAbbCodecClockCodec = 
    ABB_DSSI_INV(3)|
//    ABB_DCDC_CLKGEN3_DIV_BASE(11);
    ABB_DCDC_CLKGEN3_DIV_BASE(1)|(1<<13);
#else
// Codec clock setting
PUBLIC CONST UINT16 gc_halAbbCodecClockCodec = 
    ABB_DSSI_INV(3)|
    ABB_DCDC_CLKGEN3_DIV_BASE(23);
#endif

// PLL setting
PRIVATE CONST UINT16 gc_halAbbCodecPll_13h = 
    ABB_PLL_AUX_CLKOUT_EN(12)|
    ABB_PLL_AUX_CPAUX_BIT(4)|
    ABB_PLL_AUX_FILTER_IBIT(4)|
    ABB_PLL_AUX_CP_BIT(4)|
    ABB_PLL_AUX_SDM_CLK_SEL_RESET;
#ifdef __FORCE_WORK_IN_26MHZ__LOCAL
PRIVATE CONST UINT16 gc_halAbbCodecPll_14h = 
	//    ABB_PLL_AUX_SDM_CLK_TEST_EN|
	ABB_PLL_AUX_PD_DR|
	ABB_PLL_AUX_PD_REG|
	ABB_PLL_AUX_VREG_BIT(8);
#else
PRIVATE CONST UINT16 gc_halAbbCodecPll_14h = 
    ABB_PLL_AUX_SDM_CLK_TEST_EN|
    ABB_PLL_AUX_VREG_BIT(8);
#endif

PRIVATE CONST UINT16 gc_halAbbCodecSdm2Dither = 
    ABB_PLL_SDM2_DCDC4_312M_AUX_34;

#endif // 8809 or later
#endif // !Gallite

PUBLIC VOID hal_AbbOpen(VOID)
{
    static BOOL abbInitDone = FALSE;

    if (!abbInitDone)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808) && defined(FASTPF_USE_USB) /*&& defined(_FLASH_PROGRAMMER)*/
        // USB flash programmer: Not to restore the default states of USB pins for 8808 V2 and V3 chips       
        UINT32 chipId = rfd_XcvGetChipId();
        if (chipId == XCV_CHIP_ID_8808_V2 || chipId == XCV_CHIP_ID_8808_V3)
        {
            // Power off all the audio-related modules
            hal_AbbRegWrite(CODEC_LDO_SETTING1, ABB_PD_ALL_DR|ABB_PD_ALL_REG|
                                                ABB_PD_ADC_DR|ABB_PD_ADC_REG|
                                                ABB_PD_MICBIAS_DR|ABB_PD_MICBIAS_REG|
                                                ABB_PD_DAC_L_DR|ABB_PD_DAC_L_REG|
                                                ABB_PD_DAC_R_DR|ABB_PD_DAC_R_REG|
                                                ABB_PA_ENB_RECI_DR|
                                                ABB_PA_ENB_HP_DR);
            hal_AbbRegWrite(CODEC_LDO_SETTING2, ABB_VCOM_MODE(6)|
                                                ABB_VREF_MODE(0)|
                                                ABB_ISEL_BIT_B(2)|
                                                ABB_PA_EN_L_SPK_DR|
                                                ABB_PA_EN_R_SPK_DR);
        }
        else
#endif
        {
            // reset software and register
            hal_AbbRegWrite(CODEC_SOFT_RESET, ABB_REG_RESET_SET|
                                              ABB_SOFT_RESET_SET);
            hal_TimDelay(1);
            hal_AbbRegWrite(CODEC_SOFT_RESET, ABB_REG_RESET_CLR|
                                              ABB_SOFT_RESET_CLR);
        }

#if !defined(_FLASH_PROGRAMMER) && !defined(_T_UPGRADE_PROGRAMMER)

        // Codec digital part should be kept out of reset
        hal_AbbRegWrite(CODEC_POWER_CTRL, g_halAbbCodecPowerCtrl);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE)

        hal_AbbRegWrite(CODEC_MISC_SETTING,
                        ABB_SYNC_CLK_FREQ(DCDC_FREQ_DIV_VALUE_12)|
                        ABB_MASH_EN_ADC|ABB_MICBIAS_SEL);

        // Direct-reg to power off codec digital part
        g_halAbbCodecPowerCtrl |= ABB_DAC_EN_CODEC_DR;
        hal_AbbRegWrite(CODEC_POWER_CTRL, g_halAbbCodecPowerCtrl);

        hal_AbbRegWrite(0x20, 0x002e);
        hal_AbbRegWrite(0x25, 0xeffe);
        hal_AbbRegWrite(0x29, 0x0f0c);
        // Set vgain_fm_dac[7:4] to max value (for internal FM channel)
        hal_AbbRegWrite(FM_DAC_SETTING, 0x08f2);

        // Direct-reg to power on codec digital part
        g_halAbbCodecPowerCtrl |= ABB_DAC_EN_CODEC;
        hal_AbbRegWrite(CODEC_POWER_CTRL, g_halAbbCodecPowerCtrl);

#else // 8808 or later

        // Disable codec DSP to start DSP configuration
        hal_AudEnableCodecDsp(FALSE);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)

        // Enable HP cap mode (by disabling capless mode)
        hal_AbbRegWrite(CODEC_PLL_SETTING1, gc_halAbbCodecPllSetting1);
        // MIC mode
        hal_AbbRegWrite(CODEC_MISC_SETTING, gc_halAbbCodecMiscSetting);

#else // 8809 or later
#ifdef ABB_DISABLE_NEG_MODE
{
	UINT32 tmpReg=0;
	hal_AbbRegBlockingRead(CODEC_PLL_SETTING1,&tmpReg);
	// Enable HP cap mode (by disabling capless mode)
	hal_AbbRegWrite(CODEC_PLL_SETTING1, tmpReg|gc_halAbbCodecPllSetting1);
}
#else
{
	UINT32 tmpReg=0;
	hal_AbbRegBlockingRead(CODEC_PLL_SETTING1,&tmpReg);
	// set V_NEG
	hal_AbbRegWrite(CODEC_PLL_SETTING1, tmpReg&gc_halAbbCodecNegSetting2|gc_halAbbCodecNegSetting1);
}
#endif
        // MIC mode
        hal_AbbRegWrite(CODEC_MIC_SETTING, gc_halAbbCodecMicSetting);
        // VCOM VREF setting
        hal_AbbRegWrite(CODEC_LDO_SETTING2, gc_halAbbCodecLdoSetting2);
        // HP VCOM setting
        hal_AbbRegWrite(CODEC_MISC_SETTING, gc_halAbbCodecMiscSetting);
        // DSSI setting
        hal_AudEnableDSSI(FALSE);
        // Clock settings
        hal_AbbRegWrite(CODEC_CLOCK_CODEC, gc_halAbbCodecClockCodec); // divider 1/24
        hal_AbbRegWrite(CODEC_PLL_13H, gc_halAbbCodecPll_13h); // int_mode=0
        hal_AbbRegWrite(CODEC_PLL_14H, gc_halAbbCodecPll_14h); // sdm_en
        hal_AbbRegWrite(CODEC_SDM2_DITHER, gc_halAbbCodecSdm2Dither);

#endif

        // Init codec sample rate
        aud_CodecCommonSetRateStg(HAL_AIF_FREQ_8000HZ);
        // Init to non-FM mode
        hal_AudSetFmMode(FALSE);
        // Init to non-voice mode
        hal_AudSetVoiceMode(FALSE);
        // Init Dither gain
        hal_AudSetDacVolume(0);
        hal_AudSetAdcVolume(0);

        // Enable codec DSP
        hal_AudEnableCodecDsp(TRUE);

#endif // (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE)

#ifdef SPI_REG_DEBUG
        extern VOID abb_RegisterSpiRegCheckFunc(VOID);
        abb_RegisterSpiRegCheckFunc();
#endif // SPI_REG_DEBUG

#endif // !_FLASH_PROGRAMMER && !_T_UPGRADE_PROGRAMMER

        abbInitDone = TRUE;
    }
}
#endif

PRIVATE BOOL hal_AbbIspiCsActivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }

    return hal_IspiCsActivate(HAL_ISPI_CS_ABB);
}

PRIVATE BOOL hal_AbbIspiCsDeactivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }

    return hal_IspiCsDeactivate(HAL_ISPI_CS_ABB);
}

//=============================================================================
// hal_AbbIspiRead
//-----------------------------------------------------------------------------
/// Read codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @return register value                 0 write 1 read
//=============================================================================
PROTECTED UINT32 hal_AbbIspiRead(UINT32 regIdx)
{
    UINT32 wrData, rdData;
    UINT32 count;

    wrData = (1<<25) | ((regIdx & 0x1ff) << 16) | 0;
    
    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_ABB) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_ABB, wrData, TRUE) == 0);

    //wait until any previous transfers have ended
    while(!hal_IspiTxFinished(HAL_ISPI_CS_ABB));
    count = hal_IspiGetData(HAL_ISPI_CS_ABB, &rdData);
    HAL_ASSERT(1 == count, "ABB ISPI expected 1 data got %d !", count);

    rdData &= 0xffff;
    HAL_TRACE(HAL_SPI_TRC, 0, " ##### ABB reg =0x%x , value = 0x%x ",regIdx,rdData);

    return rdData;
}

//=============================================================================
// hal_AbbIspiWrite
//-----------------------------------------------------------------------------
/// Write codec register on ISPI, the CS must already be activated
/// @param regIdx register to read
/// @param value register value
//=============================================================================
PROTECTED VOID hal_AbbIspiWrite(UINT32 regIdx, UINT32 value)
{
    HAL_TRACE(HAL_SPI_TRC, 0, " ABB write addr = 0x%x data =0x%0x ",regIdx,value);

    UINT32 wrData;
    
    wrData = (0<<25) | ((regIdx & 0x1ff) << 16) | (value&0xffff);

    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_ABB) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_ABB, wrData, FALSE) == 0);

    //wait until any previous transfers have ended
    while(!hal_IspiTxFinished(HAL_ISPI_CS_ABB));    
}

PUBLIC BOOL hal_AbbRegRead(UINT32 addr, UINT32* pData)
{
    UINT32 status = hwp_sysIrq->sc;
	
    if (!hal_AbbIspiCsActivate(FALSE))
    {
        return FALSE;
    }

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8808 || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8809 || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)
    hal_AbbIspiReadOutputSelect(addr);
#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)
    UINT16 spiOutSel = hal_AbbIspiRead(CODEC_RESET_CTRL);
    spiOutSel = (spiOutSel & ~ABB_SPI_OUT_MASK);
    if ((addr < CODEC_DIG_MIN_REG_INDEX)||(addr >= 0x80 ))
    {
        spiOutSel |= ABB_SPI_OUT_ABB;
    }
    else
    {
        spiOutSel |= ABB_SPI_OUT_CODEC;
    }
    hal_AbbIspiWrite(CODEC_RESET_CTRL, spiOutSel);

#elif (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    //hal_AbbIspiReadOutputSelect(addr);

    if (addr >= CODEC_DIG_MIN_REG_INDEX)
    {
        UINT16 spiOutSel = hal_AbbIspiRead(CODEC_MISC);

        if(spiOutSel == 0)//already at codec
        {
			*pData = hal_AbbIspiRead(addr);
		}
		else
		{
	        spiOutSel |= ABB_SPI_OUT_CODEC;
			hal_AbbIspiWrite(CODEC_MISC, spiOutSel);//change to codec;
			
	    	*pData = hal_AbbIspiRead(addr);
			
	        spiOutSel &= ~ABB_SPI_OUT_CODEC;
			hal_AbbIspiWrite(CODEC_MISC, spiOutSel);//change to abb;
		}
    }
    else
    {
    	*pData = hal_AbbIspiRead(addr);
    }
#endif

    
    hal_AbbIspiCsDeactivate(FALSE);

	hwp_sysIrq->sc = status;

    return TRUE;
}

PUBLIC VOID hal_AbbRegBlockingRead(UINT32 addr, UINT32* pData)
{
    while(!hal_AbbRegRead(addr, pData))
    {
        COS_SleepByTick(ABB_REG_READ_WAITING_TIME);
    }
}

PUBLIC BOOL hal_AbbRegWrite(UINT32 addr, UINT32 data)
{
    
#if 0
    if (!hal_AbbIspiCsActivate(FALSE))
    {
        return FALSE;
    }
#endif

    hal_AbbIspiWrite(addr, data);

#if 0
    hal_AbbIspiCsDeactivate(FALSE);
#endif

    return TRUE;
}


// ======================================================
// Debug: Read and Write Registers via SPI

#define ABB_SPI_REG_VALUE_LOCATION __attribute__((section(".abb_reg_value")))

#ifdef SPI_REG_DEBUG

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_GALLITE || \
     CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
#define ABB_REG_NUM (0x36)
#else // 8809 or later
#define ABB_REG_NUM (0x48)
#endif

UINT32 ABB_SPI_REG_VALUE_LOCATION g_abbSpiRegValue[ABB_REG_NUM];

PROTECTED VOID abb_SpiRegValueInit(VOID)
{
    for (int i=0; i<ABB_REG_NUM; i++)
    {
        g_abbSpiRegValue[i] = 0;
    }
}

PUBLIC VOID abb_SpiRegCheck(VOID)
{
    static BOOL initDone = FALSE;
    if (!initDone)
    {
        abb_SpiRegValueInit();
        initDone = TRUE;
    }

    for (int i=0; i<ABB_REG_NUM; i++)
    {
        if (g_abbSpiRegValue[i] & SPI_REG_WRITE_FLAG)
        {
            if (!hal_AbbRegWrite(i, g_abbSpiRegValue[i]&SPI_REG_DATA_MASK))
            {
                continue;
            }
            g_abbSpiRegValue[i] &= ~SPI_REG_WRITE_FLAG;
        }
        if (g_abbSpiRegValue[i] & SPI_REG_READ_FLAG)
        {
            UINT32 data;
            if(!hal_AbbRegRead(i, &data))
            {
                continue;
            }
            g_abbSpiRegValue[i] &= ~(SPI_REG_READ_FLAG|SPI_REG_DATA_MASK);
            g_abbSpiRegValue[i] |= data&SPI_REG_DATA_MASK;
        }
    }
}


PUBLIC VOID abb_RegisterSpiRegCheckFunc(VOID)
{
    static BOOL registerIdleHook = FALSE;
    if (!registerIdleHook)
    {
        registerIdleHook = TRUE;
        extern bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void));
        sxs_RegisterDebugIdleHookFunc(&abb_SpiRegCheck);
    }
}
#endif // SPI_REG_DEBUG



PROTECTED VOLATILE HAL_ISPI_DEBUG_T HAL_UNCACHED_BSS g_halIspiDebug = {{0},0};

//extern UINT32 _HWradio_READNow(UINT32 data);
//extern void _HWradio_ProgNow(UINT32 data);
extern BOOL pmd_RegWrite(RDA_REG_MAP_T regIdx, UINT32 data);
extern BOOL pmd_RegRead(RDA_REG_MAP_T regIdx, UINT32 *pData);
extern BOOL rfd_XcvRegForceWrite(UINT32 page, UINT32 addr, UINT32 data);
extern BOOL rfd_XcvRegForceRead(UINT32 page, UINT32 addr, UINT32 *pData);
PUBLIC VOID hal_AllSpiRegCheck(VOID)
{
    if(0 == (g_halIspiDebug.debug.read | g_halIspiDebug.debug.write))
        return;

    UINT16 spi_addr = g_halIspiDebug.debug.address;
    UINT8 spi_type = g_halIspiDebug.debug.type;
    UINT32 value = g_halIspiDebug.data;

    if(g_halIspiDebug.debug.write)
    {
        switch(spi_type)
        {
#if (CHIP_ASIC_ID <= CHIP_ASIC_ID_8809)
        case HAL_ISPI_REG_TYPE_XCV:
	     rfd_XcvRegForceWrite(spi_addr>>12, spi_addr & 0xfff, value);
            break;
#endif
        case HAL_ISPI_REG_TYPE_ABB:
            hal_AbbRegWrite(spi_addr, value);
            break;
        case HAL_ISPI_REG_TYPE_PMD:
            pmd_RegWrite(spi_addr, value);
            break;
#if 0//defined(SPI_REG_TYPE_FMD)
        case HAL_ISPI_REG_TYPE_FMD:
			#if(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
            fmd_RegWrite(spi_addr, value);
			#endif
            break;
#endif /* SPI_REG_TYPE_FMD */
        case 4:
          //_HWradio_ProgNow((spi_addr << 16) | (value));
          break;
        }

        g_halIspiDebug.debug.write = 0;
    }
    else if(g_halIspiDebug.debug.read)
    {
        switch(spi_type)
        {
#if (CHIP_ASIC_ID <= CHIP_ASIC_ID_8809)
        case HAL_ISPI_REG_TYPE_XCV:
            rfd_XcvRegForceRead(spi_addr>>12, spi_addr & 0xfff, &value);
            break;
#endif
        case HAL_ISPI_REG_TYPE_ABB:
            hal_AbbRegRead(spi_addr, &value);
            break;
        case HAL_ISPI_REG_TYPE_PMD:
            pmd_RegRead(spi_addr, &value);
            break;
#if 0//defined(SPI_REG_TYPE_FMD)
        case HAL_ISPI_REG_TYPE_FMD:
        #if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
            fmd_RegRead(spi_addr, &value);
        #endif
            break;
#endif /* SPI_REG_TYPE_FMD */
        case 4:
          //value = _HWradio_READNow(spi_addr);
          break;
        }

        g_halIspiDebug.debug.read = 0;
        g_halIspiDebug.data = value;
    }
}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)||(CHIP_ASIC_ID == CHIP_ASIC_ID_8809P)\
	||(CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
PUBLIC VOID hal_XcvPmuRegInit(VOID)
{
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)
    hal_AbbRegWrite(0x87, 0x0041);//sdm_resv codec select pll_aux zhusn
#endif

    hal_AbbRegWrite(0xc3, 0x3c70);
    hal_AbbRegWrite(0xc9, 0x76c0);

#if 0   //Dac 1K 
    hal_AbbRegWrite(0x02, 0xe404);//spk      e401
    hal_AbbRegWrite(0x03, 0x3008);
    hal_AbbRegWrite(0x0a, 0x8624);
    hal_AbbRegWrite(0x0b, 0x4380);
    hal_AbbRegWrite(0x10, 0xf3ff);
    hal_AbbRegWrite(0x11, 0x005d);
    hal_AbbRegWrite(0x13, 0x4926);//pll int mode   4922
    hal_AbbRegWrite(0x15, 0xb58b);
    hal_AbbRegWrite(0x16, 0xfb9f);
    hal_AbbRegWrite(0x1f, 0x0001);
    hal_TimDelay(1638);
    hal_AbbRegWrite(0x29, 0x0a24);
    hal_AbbRegWrite(0x2a, 0xf960);
    hal_AbbRegWrite(0x2b, 0x0f01);

#endif

}
UINT8 xcvPmuReg_flag = 0;
UINT32 spi_cmd = 0;
UINT32 spi_val = 0;
volatile UINT32 spi_cmd_tmp = 0;
UINT32 spi_val_tmp = 0;
PUBLIC VOID hal_XcvPmuRegReadWrite(VOID)
{
    if(0 == xcvPmuReg_flag)
    {
        hal_XcvPmuRegInit();
        xcvPmuReg_flag = 1;
    }
    spi_cmd = *((UINT32*)(((UINT32)(&spi_cmd_tmp))|0x20000000));
    spi_val = *((UINT32*)(((UINT32)(&spi_val_tmp))|0x20000000));
    UINT16 spi_addr = (spi_cmd&SPI_REG_ADDR_MASK);
    UINT8 spi_type = (spi_cmd&SPI_REG_TYPE_MASK)>>16;
    UINT32 value = spi_val;

//    hal_HstSendEvent(BOOT_EVENT, 0x14061802);
//    hal_HstSendEvent(BOOT_EVENT, spi_cmd);

    if(spi_cmd & SPI_REG_WRITE_FLAG)
    {
        switch(spi_type)
        {
        case HAL_ISPI_REG_TYPE_XCV:
            rfd_XcvRegForceWrite(spi_addr/0x40, spi_addr&0x3f, value);
            break;
        case HAL_ISPI_REG_TYPE_ABB:
            hal_AbbRegWrite(spi_addr, value);
            break;
        case HAL_ISPI_REG_TYPE_PMD:
            pmd_RegWrite(spi_addr, value);
            break;
        }
    }
    else if(spi_cmd & SPI_REG_READ_FLAG)
    {
        switch(spi_type)
        {
        case HAL_ISPI_REG_TYPE_XCV:
            rfd_XcvRegForceRead(spi_addr/0x40, spi_addr&0x3f, &value);
            break;
        case HAL_ISPI_REG_TYPE_ABB:
           // hal_AbbRegRead(spi_addr, &value);
            hal_AbbRegRead(spi_addr, &value);
            break;
        case HAL_ISPI_REG_TYPE_PMD:
            pmd_RegRead(spi_addr, &value);
            break;
        }
    }
    spi_cmd = (spi_type<<16) | (spi_addr);
    spi_val = value;
}
#endif

