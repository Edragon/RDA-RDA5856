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
/*************************************************************************/
/*                                                                       */
/* FILE NAME                                                             */
/*      xcv.c                                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*      This file contain the RDA6220  related functions.                */
/*                                                                       */
/*************************************************************************/

#include "cs_types.h"

#include "hal_rfspi.h"
#include "hal_tcu.h"
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_host.h"

#include "drv_xcv.h"
#include "drv_xcv_calib.h"

#include "sxs_io.h"
#include "sxr_tls.h"

#include "rfd_cfg.h"
#include "rfd_xcv.h"
#include "rfd_defs.h"

#include "gsm.h"
#include "baseband_defs.h"

//#include "calib_m.h"


extern PUBLIC VOID pmd_Enable26MXtal32K(VOID);


#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef SPI_REG_DEBUG
#undef FM_LDO_WORKAROUND
#endif

#define XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA 1

#define XCV_624M_PLL_DITHER_ENABLE 0


//#define XCV_DIGRF_RX_CLK_POL                     RFD_DIGRF_INV_CLK_POL
#define XCV_DIGRF_RX_CLK_POL                     RFD_DIGRF_NORM_CLK_POL
#define XCV_SAMPLE_WIDTH                                             12
#define XCV_ADC2DBM_GAIN                                             50

#define XCV_SET_PDN                                   SET_TCO(XCV->PDN)
#define XCV_CLR_PDN                                   CLR_TCO(XCV->PDN)

#define CDAC_REG_VALUE_BASE (0x8014)

#define MAX_PIGGYBACK_WAIT_COUNT 3
#define RX_PLL_FREQ_REG_DCDC_ON ((1<<17)|(1<<15))
#define RX_PLL_FREQ_REG_LDO_ON ((1<<16)|(1<<14))

// Forward declaration of internal functions
VOID xcvBuildSpiCmd (HAL_RFSPI_CMD_T *cmd, UINT8 address, UINT32 data);
VOID xcvSetArfcn (RFD_RFDIR_T dir, GSM_RFBAND_T band, UINT16 arfcn);
VOID xcvReadReg (UINT16 address, UINT8 *output);

PROTECTED VOID rfd_XcvRegForceWriteSingle(UINT32 addr, UINT32 data);

INLINE VOID xcvConfigure(VOID);
CONST XCV_CONFIG_T* RFD_DATA_INTERNAL XCV = NULL;


//--------------------------------------------------------------------//
//                                                                    //
//                        Internal Structures                         //
//                                                                    //
//--------------------------------------------------------------------//

// GLOBAL variables
UINT32 RFD_DATA_INTERNAL g_xcvChipId = 0;
UINT16 g_xcvbattstablevolt = 0;
INT32 RFD_DATA_INTERNAL g_CDAC;
INT32 RFD_DATA_INTERNAL g_AfcOffset = 0;
INT16 RFD_DATA_INTERNAL g_afcDacVal = RDA6220_AFC_MID;
INT16 RFD_DATA_INTERNAL g_CdacDelta = 0;
RFD_LP_MODE_T RFD_DATA_INTERNAL g_lastSleepMode = RFD_POWER_OFF;
UINT32 RFD_DATA_INTERNAL g_RxPllFreqReg_05h = 0x2a654;
#ifdef FM_LDO_WORKAROUND
VOLATILE UINT32 g_xcvDcdcLdoModeReqStatus = 0;
#endif

//PROTECTED CALIB_CALIBRATION_T* RFD_DATA_INTERNAL g_xcvCalibPtr     = 0 ;

CONST XCV_RX_GAIN_T AgcTab_EGSM[AGC_QTY]       = EGSM_AGC_DEFAULT;
CONST XCV_RX_GAIN_T AgcTab_DCS[AGC_QTY]        = DCS_AGC_DEFAULT;
CONST XCV_FREQUENCY_T Gsm850_Tx_Fre[GSM850_ARFCN_NUM] = GSM850_TX_FREQUENCY;
CONST XCV_FREQUENCY_T Gsm850_Rx_Fre[GSM850_ARFCN_NUM] = GSM850_RX_FREQUENCY;
CONST XCV_FREQUENCY_T Gsm900_Tx_Fre[GSM900_ARFCN_NUM] = GSM900_TX_FREQUENCY;
CONST XCV_FREQUENCY_T Gsm900_Rx_Fre[GSM900_ARFCN_NUM] = GSM900_RX_FREQUENCY;

CONST XCV_FREQUENCY_T Dcs_Tx_Fre[DCS_ARFCN_NUM] = DCS_TX_FREQUENCY;
CONST XCV_FREQUENCY_T Dcs_Rx_Fre[DCS_ARFCN_NUM] = DCS_RX_FREQUENCY;
CONST XCV_FREQUENCY_T Pcs_Tx_Fre[PCS_ARFCN_NUM] = PCS_TX_FREQUENCY;
CONST XCV_FREQUENCY_T Pcs_Rx_Fre[PCS_ARFCN_NUM] = PCS_RX_FREQUENCY;
// RF Commands Context
XCV_COMMANDS_T RFD_DATA_INTERNAL g_xcvCmdCtx = {0, 0, 0, 0};

BOOL RFD_DATA_INTERNAL g_xcvCalibSetPaRamp = FALSE;
UINT16 RFD_DATA_INTERNAL g_xcvCalibPaRampValue[4];

#if (XCV_624M_PLL_DITHER_ENABLE)
BOOL RFD_DATA_INTERNAL g_Xcv624MPllDitherEnabled = FALSE;
#endif

// -------------------------------------------------------------------//
//                      Macros for SPI timings                        //
// -------------------------------------------------------------------//
#define RFSPI_CLOCK_26M

#if defined(RFSPI_CLOCK_26M)
#define RFSPI_CLOCK HAL_RFSPI_26M_CLK
#define CMD_DUR_FACTOR 2600
#elif defined(RFSPI_CLOCK_19_5M)
#define RFSPI_CLOCK HAL_RFSPI_19_5M_CLK
#define CMD_DUR_FACTOR 1950
#elif defined(RFSPI_CLOCK_13M)
#define RFSPI_CLOCK HAL_RFSPI_13M_CLK
#define CMD_DUR_FACTOR 1300
#elif defined(RFSPI_CLOCK_6_5M)
#define RFSPI_CLOCK HAL_RFSPI_6_5M_CLK
#define CMD_DUR_FACTOR 650
#elif defined(RFSPI_CLOCK_3_25M)
#define RFSPI_CLOCK HAL_RFSPI_3_25M_CLK
#define CMD_DUR_FACTOR 325
#else
#error "RFSPI clock is not specified"
#endif

// 1 Qb = 12/13 us
// 1 Cmd = 35 clocks
//       = (3500/CMD_DUR_FACTOR) / (12/13) Qb
#define CMD_DURATION(n) (((n) * 3500 * 13) / ((CMD_DUR_FACTOR) * 12) + 1)


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

VOID RFD_FUNC_INTERNAL xcvBuildSpiCmd (HAL_RFSPI_CMD_T *cmd, UINT8 address, UINT32 data)
{
    UINT8 array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord = RDA6220_WRITE_FMT(address, data);

#ifdef SPI_REG_DEBUG
#ifdef XCV_REG_BUFFER_READ_WRITE
    if (address == 0x3f)
    {
        g_rfdXcvCurPage = data&0x3;
    }
    if (address < XCV_REG_NUM_PER_PAGE)
    {
        g_rfdXcvRegLastWriteValue[g_rfdXcvCurPage][address] = data&XCV_REG_DATA_MASK;
    }
#endif // XCV_REG_BUFFER_READ_WRITE
#endif // SPI_REG_DEBUG

    // Hardcoded
    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;

    hal_RfspiPushData(cmd,array,RDA6220_ONE_CMD_BYTE_QTY);
}

VOID RFD_FUNC_INTERNAL xcvReadReg (UINT16 address, UINT8 *output)
{
    UINT8  array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord;

    cmdWord = RDA6220_READ_FMT(address, 0);

    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;
    hal_RfspiDigRfRead(array,output);
}

#if 0 // no RF support
//--------------------------------------------------------------------//
//                                                                    //
//                        RF Commands Generation                      //
//                                                                    //
//--------------------------------------------------------------------//
INT16 RFD_FUNC_INTERNAL Pcl2dBm (GSM_RFBAND_T band,UINT16 arfcn,UINT8 Pcl)
{
    INT16 dBm_level;
    UINT16 nb_arfcn_in_band;
    CALIB_ARFCN_T *pcl2dbm_o_arfcn;
    INT16 arfcn_s = arfcn;

    if ((band == GSM_BAND_GSM900)||(band == GSM_BAND_GSM850))
    {
        // GSM
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnG;
        // GSM-P band (0->124)
        if (    (arfcn_s >= RFD_ARFCN_EGSM_RX_MIN1) &&
                (arfcn_s <= RFD_ARFCN_EGSM_RX_MAX1))
        {
            arfcn_s -= RFD_ARFCN_EGSM_RX_MIN1;
        }
        // GSM-E band (975->1023)
        else if (   (arfcn >= RFD_ARFCN_EGSM_TX_MIN3) &&
                (arfcn <= RFD_ARFCN_EGSM_TX_MAX3))
        {
            arfcn_s -= (RFD_ARFCN_EGSM_TX_MAX3 + 1);
        }
        // TODO: Tx offset not handled for now in GSM850 (set to the min)
        // GSM850 (128->251)
        else if((arfcn_s >= RFD_ARFCN_GSM850_MIN) &&
                (arfcn_s <= RFD_ARFCN_GSM850_MAX))
        {
            arfcn_s = RFD_ARFCN_EGSM_RX_MAX1;
        }
        nb_arfcn_in_band = RFD_ARFCN_EGSM_RX_MAX1 - RFD_ARFCN_EGSM_RX_MIN1;

        if (Pcl < 5)
            Pcl = 5;
        else if (Pcl > 19)
            Pcl = 19;
        Pcl -= 5;
    }
    else if (band == GSM_BAND_DCS1800)
    {
        // DCS
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnD;
        arfcn_s -= RFD_ARFCN_DCS_MIN;
        nb_arfcn_in_band = RFD_ARFCN_DCS_MAX - RFD_ARFCN_DCS_MIN;

        if (Pcl >= 29)
        {
            Pcl = CALIB_DCS_PCL_QTY - 1;
        }
        else
        {
            if (Pcl > 15) Pcl = 15;
        }
    }
    else
    {
        // PCS
        pcl2dbm_o_arfcn = (CALIB_ARFCN_T *)
            g_xcvCalibPtr->pa->palcust.pcl2dbmArfcnP;
        arfcn_s -= RFD_ARFCN_PCS_MIN;
        nb_arfcn_in_band = RFD_ARFCN_PCS_MAX - RFD_ARFCN_PCS_MIN;

        if ((Pcl >=22) && (Pcl <= 30))
        {
            Pcl = CALIB_PCS_PCL_QTY - 2;
        }
        else if (Pcl == 31)
        {
            Pcl = CALIB_PCS_PCL_QTY - 1;
        }
        else
        {
            if (Pcl > 15) Pcl = 15;
        }
    }
    dBm_level = pcl2dbm_o_arfcn[Pcl][0] + // Min level for lower PCL
        ((arfcn_s * (pcl2dbm_o_arfcn[Pcl][1] - pcl2dbm_o_arfcn[Pcl][0]) /
          nb_arfcn_in_band)); // Linear interpolation upon arfcn

    return dBm_level;
}


VOID RFD_FUNC_INTERNAL xcvSetArfcn (RFD_RFDIR_T dir, GSM_RFBAND_T band, UINT16 arfcn)
{
    // According to the band set the corresponding bit
    UINT32 selBand  = ( (band == GSM_BAND_GSM850)  ? 0 :
            (band == GSM_BAND_GSM900)  ? 1 :
            (band == GSM_BAND_DCS1800) ? 2 :
            3 ) << 12; // GSM_BAND_PCS1900

    // If rx set the 11th bit, if tx the 10th bit
    UINT32 rxTxMode = ( (dir == RFD_DIR_RX) ? 2 : 1 ) << 10;

    // Choose different MDLL divider number for some Arfcn at TX because of Modulation Sqectrum
    UINT32 freqReg = 0x18000; //divider=9
    if (dir == RFD_DIR_TX)
    {
        switch (band)
        {
            case GSM_BAND_GSM850:
                if (arfcn > 244 && arfcn < 252)
                {
                    freqReg = 0x14000;
                }
                break;
            case GSM_BAND_GSM900:
                if (arfcn > 79 && arfcn < 89)
                {
                    freqReg = 0x14000;
                }
                break;
            case GSM_BAND_DCS1800:
                if (arfcn > 731 && arfcn < 741)
                {
                    freqReg = 0x14000;
                }
                break;
            case GSM_BAND_PCS1900:
                if (arfcn > 616 && arfcn < 626)
                {
                    freqReg = 0x10000;
                }
                break;
            default:
                break;
        }
    }
    else if (dir == RFD_DIR_RX)
    {
        freqReg = 0x10000; //divider=7
        switch (band)
        {
            case GSM_BAND_GSM850:
                break;
            case GSM_BAND_GSM900:
                break;
            case GSM_BAND_DCS1800:
                break;
            case GSM_BAND_PCS1900:
                if (arfcn==611)
                {
                    freqReg = 0x14000; //divider=8
                }
                break;
            default:
                break;
        }
    }
    g_xcvCmdCtx.freqCtrl = (freqReg | selBand | rxTxMode | (arfcn & RDA6220_ARFCN_MASK));
}
int agcindex = 2;  // 6
int m_ganagain[10] = { 0};
    INLINE
UINT8 RFD_FUNC_INTERNAL xcvSetRxGain (UINT8 gain, GSM_RFBAND_T band, UINT16 arfcn)
{
    UINT8 iLoss = 0;
    UINT8 XcvGain = 0;
    UINT8 GainIdx;
    CONST XCV_RX_GAIN_T* agcTable;

    agcTable=((band == GSM_BAND_GSM850)|| (band == GSM_BAND_GSM900))?
        AgcTab_EGSM:
        AgcTab_DCS;
    switch (band)
    {
        // GSM850 uses 5 points          128  -- 251

        case GSM_BAND_GSM850:
            if (arfcn > 128 && arfcn < 141 ) // 128 - 140
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 166 && arfcn > 140) // 141 - 165
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
            else if (arfcn < 191 && arfcn > 165) // 166 - 190
                iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
            else if (arfcn < 226 && arfcn > 190) // 191 - 225
                iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
            else
                iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;;
            break;
            // GSM uses 5 points
        case GSM_BAND_GSM900:
            if (arfcn < 42) // 0-40
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 107 && arfcn > 41) // 41-82
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
            else if (arfcn < 125 && arfcn > 106) // 83-124
                iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
            else if (arfcn < 1006 && arfcn > 974) // 975-999
                iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
            else // 1000-1023
                iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;
            break;
            // DCS uses 8 points
        case GSM_BAND_DCS1800:
            if (arfcn < 553) // 512-558
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 583 && arfcn > 552) // 559-604
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
            else if (arfcn < 663 && arfcn > 582) // 605-650
                iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
            else if (arfcn < 693 && arfcn > 662) // 651-696
                iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
            else if (arfcn < 718 && arfcn > 692) // 697-742
                iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;
            else if (arfcn < 735 && arfcn > 717) // 743-788
                iLoss = (XCV_PARAM(ILOSS(band)) >> 20)& 0xf;
            else if (arfcn < 848 && arfcn > 734) // 789-834
                iLoss = (XCV_PARAM(ILOSS(band)) >> 24)& 0xf;
            else // 835-885
                iLoss = (XCV_PARAM(ILOSS(band)) >> 28)& 0xf;
            break;
            // PCS uses 8 points
        case GSM_BAND_PCS1900:
            if (arfcn < 550) // 512-549
                iLoss = XCV_PARAM(ILOSS(band))& 0xf;
            else if (arfcn < 587 && arfcn > 549) // 550-586
                iLoss = (XCV_PARAM(ILOSS(band)) >> 4)& 0xf;
            else if (arfcn < 623 && arfcn > 586) // 587-622
                iLoss = (XCV_PARAM(ILOSS(band)) >> 8)& 0xf;
            else if (arfcn < 660 && arfcn > 622) // 623-659
                iLoss = (XCV_PARAM(ILOSS(band)) >> 12)& 0xf;
            else if (arfcn < 697 && arfcn > 659) // 660-696
                iLoss = (XCV_PARAM(ILOSS(band)) >> 16)& 0xf;
            else if (arfcn < 734 && arfcn > 696) // 697-733
                iLoss = (XCV_PARAM(ILOSS(band)) >> 20)& 0xf;
            else if (arfcn < 771 && arfcn > 733) // 734-770
                iLoss = (XCV_PARAM(ILOSS(band)) >> 24)& 0xf;
            else // 771-810
                iLoss = (XCV_PARAM(ILOSS(band)) >> 28)& 0xf;
            break;

        default:
            break;
    }

    if (gain == RFD_AGC)
    {
        // FIXE ME: What is XCVER_AGC_TARGET
        gain = XCVER_AGC_TARGET + iLoss;

    }
    else
    {
        gain += iLoss;
        // Boundaries check
        gain = ((gain>RDA6220_MIN_IN_POWER)?RDA6220_MIN_IN_POWER:gain);
        gain = ((gain<RDA6220_MAX_IN_POWER)?RDA6220_MAX_IN_POWER:gain);
GainIdx = (gain-15);
XcvGain = agcTable[GainIdx].totGain + agcindex;
m_ganagain[0] = agcTable[GainIdx].magic;
m_ganagain[1] = XcvGain;
m_ganagain[2] = GainIdx;
m_ganagain[2] = gain;
    }
    //    if(gain > 80)
    //        gain = 80;
    GainIdx = (gain-15);
    XcvGain = agcTable[GainIdx].totGain + agcindex;

    g_xcvCmdCtx.magic = agcTable[GainIdx].magic;
    g_xcvCmdCtx.diggain = agcTable[GainIdx].digGain;

    return (XcvGain - iLoss);
}
PRIVATE UINT32 HAL_DATA_BACKUP g_halSysStateShutdownFlag;

//--------------------------------------------------------------------//
//                                                                    //
//                    Internal Functions                              //
//                                                                    //
//--------------------------------------------------------------------//

// *** xcvBuildRxSpiCmd (UINT16 arfcn, UINT8 gain) ***
// Prepare the transceiver commands for reception
    INLINE
UINT8 RFD_FUNC_INTERNAL xcvBuildRxSpiCmd(UINT8 modmode/*1 gmsk, 0 8psk*/,UINT16 arfcn, GSM_RFBAND_T band, UINT8 gain, HAL_RFSPI_CMD_T *spiCmd)
{
    UINT8 XcvGain;

    xcvBuildSpiCmd (spiCmd, 0x30, 0x0083);  // 8820 deal with rfspi
    // Set Gain
    XcvGain = xcvSetRxGain (gain, band, arfcn);
    UINT32 reg_90h = 0;    // 90 91 reg PLL
    UINT32 reg_40h = 0;    // 40 41 DSP
    UINT32 reg_42h = 0;    // 42 43
    UINT16 magic = g_xcvCmdCtx.magic;
    switch(band)
    {
        case GSM_BAND_GSM850:
            reg_90h = Gsm850_Rx_Fre[arfcn-128].pllregvalue & 0xffff;
            reg_40h = (Gsm850_Rx_Fre[arfcn-128].dspregvalue)>>16 & 0xffff;
            reg_42h = (Gsm850_Rx_Fre[arfcn-128].dspregvalue) & 0x3ffff;
            if(1 == modmode)
            {
                //      xcvBuildSpiCmd (spiCmd, 0x1a, 0x8008);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8800|(magic ));
                //     xcvBuildSpiCmd (spiCmd, 0x0a, 0x8800|(magic));
            }
            else if(0 == modmode)
            {
                //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8000);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8800|(magic ));
                //   xcvBuildSpiCmd (spiCmd, 0x0a, 0x8800|(magic ));
            }
            break;
            // GSM uses 5 points
        case GSM_BAND_GSM900:

            if(1 == modmode)
            {
                //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8008);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8800|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8800|(magic ));
            }
            else if(0 == modmode)
            {
                //       xcvBuildSpiCmd (spiCmd, 0x1a, 0x8000);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8800|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8800|(magic ));
            }            
            if(arfcn <=124)
            {
                reg_90h = Gsm900_Rx_Fre[arfcn].pllregvalue & 0xffff;
                reg_40h = (Gsm900_Rx_Fre[arfcn].dspregvalue)>> 16 & 0xffff;
                reg_42h = Gsm900_Rx_Fre[arfcn].dspregvalue & 0xffff;
            }
            else
            {
                reg_90h = Gsm900_Rx_Fre[arfcn-850].pllregvalue & 0xffff;
                reg_40h = (Gsm900_Rx_Fre[arfcn-850].dspregvalue)>> 16 & 0xffff;
                reg_42h = Gsm900_Rx_Fre[arfcn-850].dspregvalue & 0xffff;

            }
            break;
            // DCS uses 8 points
        case GSM_BAND_DCS1800:
            reg_90h = Dcs_Rx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Dcs_Rx_Fre[arfcn-512].dspregvalue) >> 16 & 0xffff;
            reg_42h = Dcs_Rx_Fre[arfcn-512].dspregvalue & 0xffff;
            if(1 == modmode)
            {
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8400|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8400|(magic ));
            }
            else if(0 == modmode)
            {
                //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8000);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8400|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8400|(magic ));
            }
            break;
            // PCS uses 8 points
        case GSM_BAND_PCS1900:
            reg_90h = Pcs_Rx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Pcs_Rx_Fre[arfcn-512].dspregvalue) & 0xffff;
            reg_42h = Pcs_Rx_Fre[arfcn-512].dspregvalue & 0xffff;
            if(1 == modmode)
            {
                //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8008);
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8400|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8400|(magic ));
            }
            else if(0 == modmode)
            {
                //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8000);
                xcvBuildSpiCmd (spiCmd, 0x38, 0x8400|(magic ));
                xcvBuildSpiCmd (spiCmd, 0x0a, 0x8400|(magic ));
            }
            break;
        default:
            reg_90h = Dcs_Rx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Dcs_Rx_Fre[arfcn-512].dspregvalue) >> 16 & 0xffff;
            reg_42h = Dcs_Rx_Fre[arfcn-512].dspregvalue & 0xffff;
            xcvBuildSpiCmd (spiCmd, 0x38, 0x9000|(magic ));
            break;
    }


    xcvBuildSpiCmd (spiCmd, 0x90, reg_90h);
    xcvBuildSpiCmd (spiCmd, 0x40, reg_40h);
    xcvBuildSpiCmd (spiCmd, 0x42, reg_42h);

    g_xcvCmdCtx.digAfc = ((g_afcDacVal))&0x3fff;
    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG, g_xcvCmdCtx.digAfc);
    if(arfcn == 586)
    { 
        if(band == GSM_BAND_DCS1800)
        { 
            xcvBuildSpiCmd (spiCmd, 0x04, 0x722a);  // dc  0x742a, 0624 0x722a
            xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);   // mdll rx div 9
            xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll rx div 9
        }
    }
    else if((arfcn == 521)||(arfcn == 651)||(arfcn == 716)||(arfcn == 781))
    { 

        if(band == GSM_BAND_DCS1800)
        { 
            xcvBuildSpiCmd (spiCmd, 0x04, 0x540a);  // dc  0x742a, 0624 0x722a
            xcvBuildSpiCmd (spiCmd, 0x4c, 0x0700);   // mdll rx div 7
            xcvBuildSpiCmd (spiCmd, 0xe8, 0x0070);   // Mdll rx div 7
        }
    }
    else
    {
        xcvBuildSpiCmd (spiCmd, 0x04, 0x722a);  // dc  0x742a, 0624 0x722a
        xcvBuildSpiCmd (spiCmd, 0x4c, 0x0700);   // mdll rx div 7
        xcvBuildSpiCmd (spiCmd, 0xe8, 0x0070);   // Mdll rx div 7
    }
    xcvBuildSpiCmd (spiCmd, 0xa7, 0xfbea);
    xcvBuildSpiCmd (spiCmd, 0xa7, 0xebea);
    xcvBuildSpiCmd (spiCmd, 0x5a, 0x11ff&g_xcvCmdCtx.diggain);
    xcvBuildSpiCmd (spiCmd, 0x30, 0x0003);   // 8051 deal with rfspi
    // Set arfcn & band
    //xcvSetArfcn (RFD_DIR_RX, band, arfcn);

    // Set DigitalAfc, 14 bit is set for digital AFC

    return XcvGain;

}

VOID xcv_SetStableVolt(UINT16 volt)
{
    g_xcvbattstablevolt = volt;
}
// *** xcvBuildTxSpiCmd  ***
// Prepare the transceiver commands for emission
    INLINE
VOID RFD_FUNC_INTERNAL xcvBuildTxSpiCmd (UINT8 modmode/*1 gmsk, 0 8psk*/,RFD_WIN_T *Win, UINT16 arfcn, GSM_RFBAND_T band, HAL_RFSPI_CMD_T *spiCmd)
{

    UINT32 reg_90h = 0;    // 90 91 reg PLL
    UINT32 reg_40h = 0;    // 40 41 DSP
    UINT32 reg_42h = 0;    // 42 43
    xcvBuildSpiCmd (spiCmd, 0x30, 0x5183); // deal with spi command
#if 1
    switch(band)
    {
        case GSM_BAND_GSM850:
            reg_90h = Gsm850_Tx_Fre[arfcn-128].pllregvalue & 0xffff;
            reg_40h = (Gsm850_Tx_Fre[arfcn-128].dspregvalue)>>16 & 0xffff;
            reg_42h = (Gsm850_Tx_Fre[arfcn-128].dspregvalue) & 0x3ffff;

            xcvBuildSpiCmd (spiCmd, 0x38, 0x4800);
            xcvBuildSpiCmd (spiCmd, 0x0a, 0x4800);
            xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
            xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9
            break;
            // GSM uses 5 points
        case GSM_BAND_GSM900:

            xcvBuildSpiCmd (spiCmd, 0x38, 0x4800);
            xcvBuildSpiCmd (spiCmd, 0x0a, 0x4800);
            if(arfcn <=124)
            {
                reg_90h = Gsm900_Tx_Fre[arfcn].pllregvalue & 0xffff;
                reg_40h = (Gsm900_Tx_Fre[arfcn].dspregvalue)>> 16 & 0xffff;
                reg_42h = Gsm900_Tx_Fre[arfcn].dspregvalue & 0xffff;
                if((arfcn > 79)&&(arfcn < 89))
                {
                    xcvBuildSpiCmd (spiCmd, 0x4c, 0x4700);  // Mdll 8
                    xcvBuildSpiCmd (spiCmd, 0xe8, 0x0080);   // Mdll tx div 8
                }
                else
                {
                    xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
                    xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9
                }
            }
            else
            {
                reg_90h = Gsm900_Tx_Fre[arfcn-850].pllregvalue & 0xffff;
                reg_40h = (Gsm900_Tx_Fre[arfcn-850].dspregvalue)>> 16 & 0xffff;
                reg_42h = Gsm900_Tx_Fre[arfcn-850].dspregvalue & 0xffff;
                xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
                xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9

            }
            break;
            // DCS uses 8 points
        case GSM_BAND_DCS1800:
            xcvBuildSpiCmd (spiCmd, 0x38, 0x4400);
            xcvBuildSpiCmd (spiCmd, 0x0a, 0x4400);
            reg_90h = Dcs_Tx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Dcs_Tx_Fre[arfcn-512].dspregvalue) >> 16 & 0xffff;
            reg_42h = Dcs_Tx_Fre[arfcn-512].dspregvalue & 0xffff;
            if((arfcn > 731)&&(arfcn < 741))
            {
                xcvBuildSpiCmd (spiCmd, 0x4c, 0x4700);  // Mdll 8
                xcvBuildSpiCmd (spiCmd, 0xe8, 0x0080);   // Mdll tx div 8
            }
            else
            {

                xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
                xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9
            }
            break;
            // PCS uses 8 points
        case GSM_BAND_PCS1900:
            reg_90h = Pcs_Tx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Pcs_Tx_Fre[arfcn-512].dspregvalue) & 0xffff;
            reg_42h = Pcs_Tx_Fre[arfcn-512].dspregvalue & 0xffff;

            xcvBuildSpiCmd (spiCmd, 0x38, 0x4400);
            xcvBuildSpiCmd (spiCmd, 0x0a, 0x4400);
            xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
            xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9
            break;
        default:
            reg_90h = Dcs_Tx_Fre[arfcn-512].pllregvalue & 0xffff;
            reg_40h = (Dcs_Tx_Fre[arfcn-512].dspregvalue) >> 16 & 0xffff;
            reg_42h = Dcs_Tx_Fre[arfcn-512].dspregvalue & 0xffff;
            xcvBuildSpiCmd (spiCmd, 0x38, 0x4400);
            xcvBuildSpiCmd (spiCmd, 0x0a, 0x4400);
            xcvBuildSpiCmd (spiCmd, 0x4c, 0x8700);  // Mdll 9
            xcvBuildSpiCmd (spiCmd, 0xe8, 0x0090);   // Mdll tx div 9
            break;
    }

    xcvBuildSpiCmd (spiCmd, 0x90, reg_90h);
    xcvBuildSpiCmd (spiCmd, 0x40, reg_40h);
    xcvBuildSpiCmd (spiCmd, 0x42, reg_42h);

    xcvBuildSpiCmd (spiCmd, 0xa7, 0xfbea);
    xcvBuildSpiCmd (spiCmd, 0xa7, 0xebea);
#endif

    //     xcvBuildSpiCmd (spiCmd, 0x68, 0xC0DA); //shift_ct_lo=4,shift_ct_hi=3;
    //    xcvBuildSpiCmd (spiCmd1, 0xE6, 0xB001); //ramp
    //    xcvBuildSpiCmd (spiCmd, 0xB0, 0x7F06); //pu
    //    xcvBuildSpiCmd (spiCmd, 0xB2, 0x0F00);
    g_xcvCmdCtx.digAfc = ((g_afcDacVal))&0x3fff;

    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG, g_xcvCmdCtx.digAfc);

    UINT8  TabRampReg[4] = {RDA6220_TX_RAMP_0_REG, RDA6220_TX_RAMP_1_REG,
        RDA6220_TX_RAMP_2_REG, RDA6220_TX_RAMP_3_REG};
    UINT16 rampFactor[4] = {0};
    UINT8 WinBitMap = Win->bitmap;
    int count = 0;
    if (g_xcvCalibSetPaRamp)
    {
#if 0
        RFD_TRACE(0,0,"WinBitMap = %d, %d, %d, %d,%d",WinBitMap,
                g_xcvCalibPaRampValue[0],
                g_xcvCalibPaRampValue[1],
                g_xcvCalibPaRampValue[2],
                g_xcvCalibPaRampValue[3]);
#endif
        g_xcvCalibSetPaRamp = FALSE;
        while(WinBitMap)
        {
            // If active slot
            if (WinBitMap & 0x1)
            {
                rampFactor[count] = (((g_xcvCalibPaRampValue[count] & RDA6220_RAMP_FACTOR_MASK)
                            << RDA6220_RAMP_FACTOR_OFFSET));
            }

            WinBitMap >>= 1;
            count++;
        }
        if (count > 0)
        {
            // Ramp_mode_sel_gsm=0
            for(; count>0; count--)
            {
                xcvBuildSpiCmd (spiCmd, TabRampReg[count/2], ((rampFactor[count-1]&0xff)<<8)|
                        (rampFactor[count]&0xff));
                count -= 2;
            }
        }

        xcvBuildSpiCmd (spiCmd, 0x30, 0x0073); // 8051 deal with spi command
        return;
    }



    int pclValue;
    int curvernum = 0;
    CONST UINT16 *pDacTable;
    INT16 dBmMax, dBmMin, dBmValue;
    int dBmOffset;
    UINT32 dacValue;

    CONST UINT16 DAC_MAX = RDA6220_RAMP_FACTOR_MASK;

    if (band == GSM_BAND_GSM850 || band == GSM_BAND_GSM900)
    {
        pDacTable = (UINT16 *)g_xcvCalibPtr->pa->palcust.profileG;
        dBmMax = g_xcvCalibPtr->pa->palcust.profileDbmMaxG;
        dBmMin = g_xcvCalibPtr->pa->palcust.profileDbmMinG;
    }
    else
    {
        pDacTable = (UINT16 *)g_xcvCalibPtr->pa->palcust.profileDp;
        dBmMax = g_xcvCalibPtr->pa->palcust.profileDbmMaxDp;
        dBmMin = g_xcvCalibPtr->pa->palcust.profileDbmMinDp;
    }

    while(WinBitMap)
    {
        // If active slot
        if (WinBitMap & 0x1)
        {
            pclValue = Win->gain[count];

            dBmValue = Pcl2dBm(band, arfcn, pclValue);
            if (dBmValue < dBmMin) dBmValue = dBmMin;
            else if (dBmValue > dBmMax) dBmValue = dBmMax;

            dBmOffset = 1023 * (dBmValue-dBmMin) / (dBmMax - dBmMin);

            dacValue = (pDacTable[dBmOffset]  > DAC_MAX) ? DAC_MAX :  pDacTable[dBmOffset];
            if(g_xcvbattstablevolt > 3600)
            {
                if((band == GSM_BAND_GSM850 || band == GSM_BAND_GSM900)&&(pclValue == 5))
                {
                    UINT32 pcl6dacvalue;
                    INT16 Pcl6dBmValue = Pcl2dBm(band, arfcn, 6);
                    if (Pcl6dBmValue < dBmMin) Pcl6dBmValue = dBmMin;
                    else if (Pcl6dBmValue > dBmMax) Pcl6dBmValue = dBmMax;

                    dBmOffset = 1023 * (Pcl6dBmValue-dBmMin) / (dBmMax - dBmMin);

                    pcl6dacvalue = (pDacTable[dBmOffset]  > DAC_MAX) ? DAC_MAX :  pDacTable[dBmOffset];
                    if(g_xcvbattstablevolt > 4000)
                    {
                        dacValue -=((dacValue - pcl6dacvalue)*800)/1200;  // 1db   -> 4.0-4.2
                    }
                    else if((g_xcvbattstablevolt > 3800)&&(g_xcvbattstablevolt < 4001))
                    {
                        dacValue -=((dacValue - pcl6dacvalue)*650)/1200;  // 0.8db -> 3.8-4.0
                    }
                    else if((g_xcvbattstablevolt > 3600)&&(g_xcvbattstablevolt < 3801))
                    {
                        dacValue -=((dacValue - pcl6dacvalue)*370)/1200;  // 0.5db -> 3.6-3.8
                    }
                }
            }
            if(band == GSM_BAND_GSM850 || band == GSM_BAND_GSM900)
            {

                if(pclValue < 8)
                    curvernum = (0x0 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if((7 < pclValue) && (pclValue < 12))
                    curvernum = (0x1 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if((11 < pclValue) &&(pclValue < 16))
                    curvernum = (0x2 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if(15 < pclValue)
                    curvernum = (0x3 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
            }
            else
            {
                if(pclValue < 4)
                    curvernum = (0x0 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if((3 < pclValue) && (pclValue < 8))
                    curvernum = (0x1 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if((7 < pclValue) &&(pclValue < 12))
                    curvernum = (0x2 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;
                else if(11 < pclValue)
                    curvernum = (0x3 & RDA6220_RAMP_CURVE_MASK) << RDA6220_RAMP_CURVE_OFFSET;

            }
            rampFactor[count] = (((dacValue & RDA6220_RAMP_FACTOR_MASK)
                        << RDA6220_RAMP_FACTOR_OFFSET));
        }

        WinBitMap >>= 1;
        count++;
    }

    if (count > 0)
    {
        // Ramp_mode_sel_gsm=0
        // rampFactor[0] |= 0x00000 | (((count-1) & 0x3) << 13);
        for(; count>0; )
        {
            xcvBuildSpiCmd (spiCmd, TabRampReg[count/2], (rampFactor[count-1]<<8)|(rampFactor[count]));
            count -= 2;
        }
    }
    xcvBuildSpiCmd (spiCmd, 0x30, 0x0003); // 8051 deal with spi command
}


//--------------------------------------------------------------------//
//                                                                    //
//               Functions for GLOBAL PAL API                         //
//                                                                    //
//--------------------------------------------------------------------//

RFD_XCV_PARAMS_T RFD_DATA_INTERNAL g_xcvParams =
{
    // Misc Modem params
    .dcocMode= RFD_SW_DCOC_ENABLED,
    //.dcocMode= RFD_NO_DCOC,
    .lowRla = 108,
    .dtxTcoSettings = 0,
    .rxOnTcoSettings = 0,

    // PowerUp params
    .wakeUpFrameNbr = 0, //1, // activating low power optimisation
    .pu2VcoEnDelay = 1,
    .vcoEn2VcoStableDelay = 10,
    .vrfBehaviour = RFD_LDO_BEHAVIOR_DONT_USE,

    // Window bounds
    // NOTE : these timings will be wrong if changed by a calibration
    // file, they should be updated at execution time
    .winBounds = {
        .rxSetup = DEFAULT_CALIB_SPI_CMD - CMD_DURATION(7),
        .rxHold  = DEFAULT_CALIB_PDN_RX_DN,
        .txSetup = DEFAULT_CALIB_SPI_CMD - CMD_DURATION(11),
        .txHold  = DEFAULT_CALIB_PDN_TX_DN
    },

    // DigRf params
    .digRf = {
        .used           = TRUE,
        .rxRate         = RFD_DIGRF_1_SAMPLE_PER_SYMB,
        .rxSampleWidth  = XCV_SAMPLE_WIDTH,
        .rxSampleAlign  = RFD_DIGRF_SAMPLE_ALIGN_MSB,
        .rxAdc2DbmGain  = XCV_ADC2DBM_GAIN,
        .rxClkPol       = XCV_DIGRF_RX_CLK_POL,
        .txClkPol       = RFD_DIGRF_INV_CLK_POL,
#ifdef __TX_BLOCK_MODE__
        .txMode         = RFD_DIGRF_TX_BLOCK
#else
            .txMode         = RFD_DIGRF_TX_STREAM
#endif
    },

    .txBstMap = {
        .preamble  = 4,
        .guardStart= 4,
        .postamble = 4
    }

};

const RFD_XCV_PARAMS_T* rfd_XcvGetParameters(VOID)
{


    // Get the calibration pointer
    g_xcvCalibPtr = calib_GetPointers();

    g_xcvParams.dtxTcoSettings    = MASK_TCO(XCV->PDN) | HAL_TCU_MASK_TCO_GMSK_ON | HAL_TCU_MASK_DIGRF_STROBE | HAL_TCU_MASK_RF_SPI_MARKED_CMD;
    g_xcvParams.rxOnTcoSettings = MASK_TCO(XCV->PDN) | HAL_TCU_MASK_TCO_RX_DEC_ON;
    g_xcvParams.winBounds.rxSetup = XCV_TIME(SPI_CMD) - CMD_DURATION(7);
    g_xcvParams.winBounds.rxHold  = XCV_TIME(PDN_RX_DN);
    g_xcvParams.winBounds.txSetup = XCV_TIME(SPI_CMD) - CMD_DURATION(11);
    g_xcvParams.winBounds.txHold  = XCV_TIME(PDN_TX_DN);
    return &g_xcvParams;
}
#endif

const HAL_RFSPI_CFG_T g_spiCfg = {
    TRUE,  // Enable
    TRUE,  // csActiveHi
    TRUE,  // modeDigRfRead
    FALSE, // modeClockBack2Back
    FALSE, // inputEn
    FALSE, // clkFallEdge
    HAL_RFSPI_HALF_CLK_PERIOD_1, // clkDelay
    HAL_RFSPI_HALF_CLK_PERIOD_2, // doDelay
    HAL_RFSPI_HALF_CLK_PERIOD_3, // diDelay
    HAL_RFSPI_HALF_CLK_PERIOD_2, // csDelay
    HAL_RFSPI_HALF_CLK_PERIOD_0, // csEndHold
    HAL_RFSPI_HALF_CLK_PERIOD_3, // csEndPulse
    2,  // turnAroundCycles
    24, // frameSize
    15, // inputFrameSize
};
int xcvgainvalue = 0;
//-----------------------------//
// Window Open/Close Functions //
//-----------------------------//
#if 0
UINT8 RFD_FUNC_INTERNAL rfd_XcvRxOn(RFD_WIN_T *Win, INT16 start)
{
    UINT8 XcvGain;
    UINT8 temgain = 0;
    //if(xcvgainvalue++ > 2)
    //if(xcvgainvalue++ != 2)
    //    return 70;
    // TO BE REMOVED
    //  if(1) // GMSK
    {
        HAL_RFSPI_CMD_T *spiCmd = NULL;
        HAL_RFSPI_CMD_T *filterResetCmd = NULL;
        UINT8 i =0;
        UINT8 Winbitmap = (Win->bitmap & 0xf) << 1;
        UINT8 nbSlots = (Winbitmap & 0x10)? 4:
            (Winbitmap & 0x8)? 3:
            (Winbitmap & 0x4)? 2: 1;

        // Allocate the spi command
        spiCmd = hal_RfspiNewCmd(  (start + XCV_TIME(SPI_CMD) - CMD_DURATION(22)),
                Win->winIdx );
#if 0
        if(  Win->arfcn == 1)
            temgain = 5;
        Win->arfcn = 1;
#endif      // Build the command
        XcvGain = xcvBuildRxSpiCmd (1,/*modulmode,*/
                Win->arfcn,
                Win->band,
                Win->gain[0],
                spiCmd);

        spiCmd = NULL;
#if 0
        filterResetCmd = hal_RfspiNewCmd(  (start + XCV_TIME(PDN_RX_UP)-0 - CMD_DURATION(20)),
                Win->winIdx );
        xcvBuildSpiCmd (filterResetCmd, 0xb0, 0x7d7b);
        xcvBuildSpiCmd (filterResetCmd, 0xb2, 0x00da);
        xcvBuildSpiCmd (filterResetCmd, 0xb4, 0xfe2a);
        xcvBuildSpiCmd (filterResetCmd, 0x96, 0xc608);
        xcvBuildSpiCmd (filterResetCmd, 0x22, 0x8003);
        xcvBuildSpiCmd (filterResetCmd, 0xb8, 0x3c04);
        xcvBuildSpiCmd (filterResetCmd, 0x24, 0x025c);
        xcvBuildSpiCmd (filterResetCmd, 0x24, 0x024c);
        xcvBuildSpiCmd (filterResetCmd, 0x28, 0x7123);
        xcvBuildSpiCmd (filterResetCmd, 0x28, 0x7133);
        xcvBuildSpiCmd (filterResetCmd, 0xbe, 0x8440);
        xcvBuildSpiCmd (filterResetCmd, 0xbe, 0x84c0);
        xcvBuildSpiCmd (filterResetCmd, 0x92, 0x4900);
        xcvBuildSpiCmd (filterResetCmd, 0x92, 0xc900);
        xcvBuildSpiCmd (filterResetCmd, 0x44, 0x3000);
        xcvBuildSpiCmd (filterResetCmd, 0x44, 0xb000);
        xcvBuildSpiCmd (filterResetCmd, 0x50, 0x4227);
        xcvBuildSpiCmd (filterResetCmd, 0x50, 0xc227);
        xcvBuildSpiCmd (filterResetCmd, 0x52, 0x0000);
        xcvBuildSpiCmd (filterResetCmd, 0x52, 0x8000);
#endif
        filterResetCmd =  NULL;
        for(i = 0; i<nbSlots; i++)
        {
            UINT32 prev_slot = Winbitmap & 0x1;
            UINT32 curr_slot = Winbitmap & 0x2;
            // If current slot active
            if (curr_slot)
            {
                //If previous slot not active
                if(!prev_slot)
                {
                    hal_TcuSetEvent(HAL_TCU_START_RFIN_RECORD, start + XCV_TIME(PDN_RX_UP));
                    hal_TcuSetEvent (XCV_SET_PDN, start + XCV_TIME(PDN_RX_UP));

                    if(Win->type == GSM_WIN_TYPE_FCCH)
                    {
                        // FCCH contains consecutive multiple slots only. No need to stop here.
                        break;
                    }
                }
                // The last active slot will be stopped by rfd_XcvRxOff()
            }
            else
            {
                //If previous slot active
                if(prev_slot)
                {
                    // BCPU does NOT expect to receive the last Qb
                    hal_TcuSetEvent(XCV_CLR_PDN, start + XCV_TIME(PDN_RX_DN));
                    hal_TcuSetEvent(HAL_TCU_STOP_RFIN_RECORD, start + XCV_TIME(PDN_RX_DN));

                }
            }

            Winbitmap >>= 1;
            start += GSM_SLOT_QS_DURATION;
        }

        Win->swapIQ = FALSE;

        RFD_TRACE(RFD_INFO_TRC,0,"xcvRxOn @ %d Qb",start);


    }
    return XcvGain - temgain;

}

VOID RFD_FUNC_INTERNAL rfd_XcvRxOff(RFD_WIN_T *Win,INT16 stop)
{
    // return;
    // TO BE REMOVED
    // Transceiver in PowerDown
    //if(xcvgainvalue != 3)
    //if(xcvgainvalue > 3)
    //    return;
#if 0
    HAL_RFSPI_CMD_T *spiCmd;
    spiCmd = hal_RfspiNewCmd(  (stop + XCV_TIME(PDN_RX_DN) + XCV_TIME(SPI_CMD)- CMD_DURATION(26)),
            Win->winIdx);
    xcvBuildSpiCmd (spiCmd, 0xb0, 0x0000);
    spiCmd = NULL;
#endif
    hal_TcuSetEvent (XCV_CLR_PDN, stop + XCV_TIME(PDN_RX_DN));
    // stop the recording
    hal_TcuSetEvent (HAL_TCU_STOP_RFIN_RECORD, stop+ XCV_TIME(PDN_RX_DN));
    RFD_TRACE(RFD_INFO_TRC,0,"xcvRxOff @ %d Qb",stop);
}

VOID RFD_FUNC_INTERNAL rfd_XcvTxOn(RFD_WIN_T *Win, INT16 start)
{
    // TO BE REMOVED
    //   if(1)  // for gmsk
    {
        HAL_RFSPI_CMD_T *spiCmd;
        //     HAL_RFSPI_CMD_T *spiCmd1;

        UINT8 i =0;
        UINT16 stop = 0;
        UINT8 Winbitmap = (Win->bitmap & 0xf) << 1;
        UINT8 nbSlots = (Winbitmap & 0x10)? 4:
            (Winbitmap & 0x8)? 3:
            (Winbitmap & 0x4)? 2: 1;

        // Needs an Spi Command
        spiCmd = hal_RfspiNewCmd(  (start + XCV_TIME(SPI_CMD)- CMD_DURATION(26)),
                Win->winIdx);
        // mark for DTX
        spiCmd->maskable = TRUE;
        // Build the command
        xcvBuildTxSpiCmd (1,/*modulmode,*/
                Win,
                Win->arfcn,
                Win->band,
                spiCmd);
#ifndef _TX_BLOCK_MODE
        hal_TcuSetEvent(HAL_TCU_START_GMSK, start + XCV_TIME(TX_GMSK_ON));
#endif
        hal_TcuSetEvent(XCV_SET_PDN, start + XCV_TIME(PDN_TX_UP));
        for(i =0; i<nbSlots; i++)
        {
            UINT32 curr_slot = Winbitmap & 0x2;
            if (curr_slot)
            {
                hal_TcuSetEvent (HAL_TCU_DIGRF_STB, start + XCV_TIME(TX_STROBE_ON));
                stop = ((Win->type == GSM_WIN_TYPE_TXABURST)?
                        (start + GSM_ABURST_ACTIVE_QS_DURATION + 4*g_xcvParams.txBstMap.postamble):
                        (start + GSM_NBURST_ACTIVE_QS_DURATION + 4*g_xcvParams.txBstMap.postamble));
                hal_TcuSetEvent (HAL_TCU_DIGRF_STB, stop + XCV_TIME(TX_STROBE_OFF));
            }
            start += (Win->type == GSM_WIN_TYPE_TXABURST)?
                (GSM_ABURST_ACTIVE_QS_DURATION + 4*8):  // active access burst + 8 guard bits.
                GSM_SLOT_QS_DURATION;
            Winbitmap >>= 1;
        }

#ifndef _TX_BLOCK_MODE
        hal_TcuSetEvent(HAL_TCU_STOP_GMSK, stop + XCV_TIME(TX_GMSK_OFF));
#endif
        hal_TcuSetEvent (XCV_CLR_PDN, stop + XCV_TIME(PDN_TX_DN));
        Win->swapIQ = FALSE;

        RFD_TRACE(RFD_INFO_TRC,0,"xcvTxOn @ %d Qb",start);

    }
}

VOID RFD_FUNC_INTERNAL rfd_XcvTxOff(RFD_WIN_T *Win,INT16 stop)
{
    // TO BE REMOVED
    // Put Transceiver in Powerdown
    if (Win->type==GSM_WIN_TYPE_TXABURST)
        stop+=16;
    RFD_TRACE(RFD_INFO_TRC,0,"xcvTxOff @ %d Qb",stop);
}

// *** UINT8 xcvGetAgcGain
UINT8 RFD_FUNC_INTERNAL rfd_XcvGetAgcGain(UINT8 step, GSM_RFBAND_T band, UINT16 arfcn)
{
    UINT8 xcver_gain = 0;
    UINT8 gain = XCVER_AGC_TARGET + XCV_PARAM(ILOSS(band));
    UINT8 index = (gain - 15);

    CONST XCV_RX_GAIN_T* agcTable =  ((band == GSM_BAND_GSM850)|| (band == GSM_BAND_GSM900))?
        AgcTab_EGSM:
        AgcTab_DCS;

    xcver_gain = agcTable[index].totGain - (XCV_PARAM(ILOSS(band))& 0xf);

    return xcver_gain;
}

VOID RFD_FUNC_INTERNAL rfd_XcvFrequencyTuning(INT32 FOf, GSM_RFBAND_T band)
{
    // Frequency(Hz) of CDAC Offset * DacOffset_1Hz(900MHzBand)
    // DacOffset_1Hz(900MHzBand) = 1000*1Hz/XCV_PARAM(AFC_GAIN(GSM_BAND_GSM900))
    //#define DELTA_AFC_VALUE ((XCV_PARAM(AFC_FREQ(GSM_BAND_GSM900))-XCV_PARAM(CDAC_FREQ))*1000/XCV_PARAM(AFC_GAIN2(GSM_BAND_GSM900)))
    extern UINT8 g_Calib_OnSite;
    if(g_Calib_OnSite)
    {
        if (band==GSM_BAND_GSM850 || band==GSM_BAND_GSM900 )
        {
            if(ABS(FOf) >= 5000)
            {
                g_CdacDelta += FOf/1200;
                if((g_CDAC + g_CdacDelta) >  RDA6220_CDAC_MAX)
                    g_CdacDelta = RDA6220_CDAC_MAX - g_CDAC;
                else if((g_CDAC + g_CdacDelta) <  RDA6220_CDAC_MIN)
                    g_CdacDelta = g_CDAC - RDA6220_CDAC_MIN;
                if (g_xcvCalibPtr->hstXcv != NULL)
                    HST_XCV_PARAM(XTAL) = g_CDAC + g_CdacDelta;

                return;
            }

        }
        else
        {
            if(ABS(FOf) >= 10000)
            {

                g_CdacDelta += FOf/2400;
                if((g_CDAC + g_CdacDelta) >  RDA6220_CDAC_MAX)
                    g_CdacDelta = RDA6220_CDAC_MAX - g_CDAC;
                else if((g_CDAC + g_CdacDelta) <  RDA6220_CDAC_MIN)
                    g_CdacDelta = g_CDAC - RDA6220_CDAC_MIN;
                if (g_xcvCalibPtr->hstXcv != NULL)
                    HST_XCV_PARAM(XTAL) = g_CDAC + g_CdacDelta;

                return;
            }
        }

    }
#if 1

    INT32 DacOffset = (1000*FOf) / XCV_PARAM(AFC_GAIN(band));
    g_afcDacVal += DacOffset;
#else
    UINT16 afcDacVal = g_afcDacVal;
    INT16 CdacDelta = g_CdacDelta;

    INT16 AfcGain = 0;
    if (CdacDelta == 0)
    {
        AfcGain = XCV_PARAM(AFC_GAIN(band));
        if(AfcGain < 150)
            AfcGain = AfcGain * 100;
    }
    else
    {
        AfcGain = XCV_PARAM(AFC_GAIN2(band));
#if (XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA)
        if(AfcGain == 0)
        {
            if (band==GSM_BAND_GSM850 || band==GSM_BAND_GSM900 )
            {
                AfcGain = XCV_PARAM(AFC_GAIN(band))*100;
            }
            else
            {
                AfcGain =  XCV_PARAM(AFC_GAIN(band))*100;
            }
        }
#endif // XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA
    }
    // AFC DAC gain is in thousend's of Hertz
    INT32 DacOffset = (1000*FOf) / AfcGain;
    INT32 DeltaAfcValue = 0;
    if (band==GSM_BAND_GSM850 || band==GSM_BAND_GSM900 )
    {
#if (XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA)
        if(!XCV_PARAM(AFC_GAIN2(band)))
            XCV_PARAM(AFC_GAIN2(band)) =  XCV_PARAM(AFC_GAIN(band))*100;
#endif // XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA
        DeltaAfcValue = ((XCV_PARAM(AFC_FREQ(band))-XCV_PARAM(CDAC_FREQ))*1000/XCV_PARAM(AFC_GAIN2(band)));
    }
    else
    {
#if (XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA)
        if(!XCV_PARAM(AFC_GAIN2(band)))
            XCV_PARAM(AFC_GAIN2(band)) =  XCV_PARAM(AFC_GAIN(band))*100;
#endif // XCV_AFC_COMPATIBLE_WITH_OLD_CALIB_DATA
        DeltaAfcValue = ((XCV_PARAM(AFC_FREQ(band))-2*XCV_PARAM(CDAC_FREQ))*1000/XCV_PARAM(AFC_GAIN2(band)));
    }

    INT32 afcValue = afcDacVal + DacOffset;
    if (CdacDelta == 0)
    {
        if (afcValue < RDA6220_AFC_MIN)
        {
            afcValue += DeltaAfcValue;
            CdacDelta -= RDA6220_CDAC_OFFSET;
        }
        else if (afcValue > RDA6220_AFC_MAX)
        {
            afcValue -= DeltaAfcValue;
            CdacDelta = RDA6220_CDAC_OFFSET;
        }
    }
    else if (CdacDelta > 0)
    {
        if (afcValue < RDA6220_AFC_MIN)
        {
            afcValue += DeltaAfcValue;
            CdacDelta = 0;
        }
        else if (afcValue > RDA6220_AFC_MAX)
        {
            afcValue = RDA6220_AFC_MAX;
        }
    }
    else // (CdacDelta < 0)
    {
        if (afcValue > RDA6220_AFC_MAX)
        {
            afcValue -= DeltaAfcValue;
            CdacDelta = 0;
        }
        else if (afcValue < RDA6220_AFC_MIN)
        {
            afcValue = RDA6220_AFC_MIN;
        }
    }

    g_afcDacVal = afcValue;
    g_CdacDelta = CdacDelta;
    //RFD_TRACE(RFD_INFO_TRC|TSTDOUT,0,"g_afcDacVal: %#x ", g_afcDacVal);
#endif
}
#endif

//-----------------------------------//
// Crystal Aging detection stuff     //
//-----------------------------------//
#define AGING_DAC_LIMIT     0x9F // 1.5 CDAC unit ~= 159 Dac units
#define DAC_MID_RANGE       RDA6220_AFC_MID
INT32 RFD_FUNC_INTERNAL rfd_XcvGetAfcOffset(VOID)
{
    //return (g_AfcOffset + (g_afcDacVal - DAC_MID_RANGE));
#ifdef __USE_DIGITAL_AFC__
#define AGING_LIMIT  0xff

    INT32 AfcOffset = 0xffff / AGING_LIMIT;

    if (AfcOffset != 0)
    {
        return (AfcOffset);
    }
    else
    {
        return (PAL_XTAL_NO_AGING);
    }
#endif
    return 0;
}

//-----------------------------------//
// Read XCV Chip ID from the chip                  //
//-----------------------------------//
VOID rfd_XcvReadChipId(VOID)
{
    if (g_xcvChipId == 0)
    {
        // Read chip metal ID from HAL SYS
        g_xcvChipId = hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID);
    }
}

//-----------------------------------//
// Get XCV Chip ID from the driver variable               //
//-----------------------------------//
UINT32 rfd_XcvGetChipId(VOID)
{
    return g_xcvChipId;
}

#if defined(ABB_HP_DETECT) && defined(ABB_HP_DETECT_IN_LP)
VOID rfd_XcvRegPuXtalEnable(BOOL enable)
{
    UINT32 status = hal_SysEnterCriticalSection();

    // Check why force-write takes 3 ms to finish in some case (when BT playing music?)
    while (!(hal_RfspiImmReadWriteAvail() && hal_LpsRemainingFramesReady(2)))
    {
        COS_Sleep(10);
    }

    // Now we can directly write to XCV
    if(enable)
    {
        rfd_XcvRegForceWriteSingle(0x20, 0x310a1);// enabling 6p5m to avoid 26M clock burr
    }
    else
    {
        rfd_XcvRegForceWriteSingle(0x20,0x110a1);// disabling 6p5m to avoid 26M clock burr
    }

    hal_SysExitCriticalSection(status);
}
#endif

//-----------------------------------//
// Open/Close & Low Power Functions  //
//-----------------------------------//
#define BBPLL_SDM1_FREQ 0xBF0E055

PUBLIC UINT32 xcvInitDownloadCodes(VOID);

INLINE VOID xcvConfigure(VOID)
{
   
    hal_HstSendEvent(BOOT_EVENT,0x51d151d1);
    xcvInitDownloadCodes();
	rfd_XcvRfInit();
	return;
    HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();

    xcvBuildSpiCmd (confCmd, 0x30, 0x0082); // Soft Resetn down
    hal_RfspiSendCmd(confCmd);
    // wait 1ms
    hal_TimDelay(1 MS_WAITING);
    xcvBuildSpiCmd (confCmd, 0x30, 0x0083); // Soft Resetn up

    xcvBuildSpiCmd (confCmd, 0xae, 0x00c1);  // abc1
    xcvBuildSpiCmd (confCmd, 0xac, 0x0000);
    xcvBuildSpiCmd (confCmd, 0xa0, 0xe100); // Enable_clk_6p5m=1
    // Restore pu_xtal setting after enabling 6p5m
    xcvBuildSpiCmd (confCmd, 0xae, 0x0041);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(100);
    //pmd_Enable26MXtal32K();

    INT16 lp_CDAC = g_CDAC;

    lp_CDAC += 46;

    if (lp_CDAC < 0)
        lp_CDAC = 0;
    else if(lp_CDAC > 128)
        lp_CDAC = 128;

    xcvBuildSpiCmd (confCmd, 0xa8,
            0x8003 | ((lp_CDAC & RDA6220_LPCDAC_REG_MASK) << RDA6220_LPCDAC_REG_OFFSET));
    //    xcvBuildSpiCmd (confCmd, 0x30, 0x0003);
    hal_RfspiSendCmd(confCmd);
}

VOID rfd_SDM1Config(BOOL reset)
{
    HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
    return; // RDABT_5855_TODO
    if(reset)
    {
#if 0
        xcvBuildSpiCmd (confCmd, 0x3f, 0x00004);
        xcvBuildSpiCmd (confCmd, 0x01, 0x04922);
        hal_RfspiSendCmd(confCmd);
        xcvBuildSpiCmd (confCmd, 0x05, (0xc000000&0xFFFF000)>>12); 
        xcvBuildSpiCmd (confCmd, 0x06, ((0xc000000&0xFFF)<<4)|0xC);
        hal_RfspiSendCmd(confCmd);
#else
        xcvBuildSpiCmd (confCmd, 0x01, 0x04926);
        hal_RfspiSendCmd(confCmd);
#endif
        hal_HstSendEvent(SYS_EVENT, 0x14070203);
    }
    else
    {       
        xcvBuildSpiCmd (confCmd, 0x3f, 0x00004);
        xcvBuildSpiCmd (confCmd, 0x01, 0x04922);
        hal_RfspiSendCmd(confCmd);
        xcvBuildSpiCmd (confCmd, 0x05, (BBPLL_SDM1_FREQ&0xFFFF000)>>12); 
        xcvBuildSpiCmd (confCmd, 0x06, ((BBPLL_SDM1_FREQ&0xFFF)<<4)|0xC);
        hal_RfspiSendCmd(confCmd);

        hal_HstSendEvent(SYS_EVENT, 0x14070204);
    }
}


VOID rfd_XcvOpen(INT32 AfcOffset, CONST XCV_CONFIG_T* xcvConfig)
{
#ifdef SPI_REG_DEBUG
    extern VOID rfd_RegisterXcvRegCheckFunc(VOID);
    rfd_RegisterXcvRegCheckFunc();
#endif // SPI_REG_DEBUG

    XCV = xcvConfig;
    // Get the calibration pointer
    //g_xcvCalibPtr = calib_GetPointers();

    // XCV Chip ID has been read in pmd_Open()

    // Derive Aging compensated CDAC value
    // Xtal triming capacitor register to default (calibrated)
    g_CDAC = RDA6220_CDAC_DEFAULT;
    g_AfcOffset = AfcOffset;
#if 0
    // Replace the default only if aging compensation happened
    // TODO: to be tested
    if (g_AfcOffset > AGING_DAC_LIMIT)
    {
        g_CDAC += (g_AfcOffset/AGING_DAC_LIMIT);
    }
    else if (g_AfcOffset < AGING_DAC_LIMIT)
    {
        g_CDAC -= (g_AfcOffset/AGING_DAC_LIMIT);
    }
#endif
    // turn on AFC DAC - mid range    
    g_afcDacVal = RDA6220_AFC_MID;
    g_CdacDelta = 0;

    // Open RF Spi, ask for a 19.5MHz spi clock (Max speed)
    hal_RfspiOpen((HAL_RFSPI_CFG_T*)&g_spiCfg, RFSPI_CLOCK);
    //hal_HstSendEvent(0xca1bca1b);
    xcvConfigure();
}

#if 0
INLINE VOID rfd_xcvRamp0Set(HAL_RFSPI_CMD_T* confCmd)
{
    for(int i = 0; i < 16; i++)
        xcvBuildSpiCmd (confCmd, 0x32, ((15-i) | 0x000 | (g_xcvCalibPtr->pa->palcust.rampLow[i]<<8))); //  Ramp Curve000; y=(sinx)^1.5
    hal_RfspiSendCmd(confCmd);
}

INLINE VOID rfd_xcvRamp1Set(HAL_RFSPI_CMD_T* confCmd)
{
    for(int i = 0; i < 16; i++)
        xcvBuildSpiCmd (confCmd, 0x32, ((15-i) | 0x0010 | (g_xcvCalibPtr->pa->palcust.rampLow[16+i]<<8)));
    hal_RfspiSendCmd(confCmd);
}

INLINE VOID rfd_xcvRamp2Set(HAL_RFSPI_CMD_T* confCmd)
{
    for(int i = 0; i < 16; i++)
        xcvBuildSpiCmd (confCmd, 0x32, ((15-i) | 0x0020 | (g_xcvCalibPtr->pa->palcust.rampHigh[i]<<8)));
    hal_RfspiSendCmd(confCmd);
}

INLINE VOID rfd_xcvRamp3Set(HAL_RFSPI_CMD_T* confCmd)
{
    for(int i = 0; i < 16; i++)
        xcvBuildSpiCmd (confCmd, 0x32, ((15-i) | 0x0030 | (g_xcvCalibPtr->pa->palcust.rampHigh[16+i]<<8)));
    hal_RfspiSendCmd(confCmd);
}
#endif

VOID rfd_XcvRfInit(VOID)
{
    // TO BE REMOVED
    hal_HstSendEvent(SYS_EVENT,0x88886666);
    hal_SysRequestFreq(HAL_SYS_FREQ_XCV, HAL_SYS_FREQ_26M, NULL);

    HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();

    xcvBuildSpiCmd (confCmd, 0x30, 0x1183);
#if 1
    xcvBuildSpiCmd (confCmd, 0x30, 0x5182);   //  Spi_16bit_mode=1,register resetn=0
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(20000);  // delay 20 ms
    xcvBuildSpiCmd (confCmd, 0x30, 0x5183);   //  Spi_16bit_mode=1
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(20000);  // delay 20 ms
    //===========RTC setting============================
    xcvBuildSpiCmd (confCmd, 0xA0, 0x0904);
    xcvBuildSpiCmd (confCmd, 0xA1, 0xE410);  //auxclk_en
    xcvBuildSpiCmd (confCmd, 0xA2, 0x8080);
    xcvBuildSpiCmd (confCmd, 0xA3, 0x0000);
    xcvBuildSpiCmd (confCmd, 0xA4, 0x58C3);
    xcvBuildSpiCmd (confCmd, 0xA5, 0x0104 | (g_CDAC<<10));   // cdac
    xcvBuildSpiCmd (confCmd, 0xA6, 0x5c04);
    hal_RfspiSendCmd(confCmd);
    xcvBuildSpiCmd (confCmd, 0xA7, 0xFBEA);
    xcvBuildSpiCmd (confCmd, 0xA7, 0xEBEA);
    xcvBuildSpiCmd (confCmd, 0xA8, 0x1EF8);
    xcvBuildSpiCmd (confCmd, 0xAA, 0xE000);

    hal_RfspiSendCmd(confCmd);
#endif

    //===========analog setting==========================
    xcvBuildSpiCmd (confCmd, 0x04, 0x692a);
    xcvBuildSpiCmd (confCmd, 0x06, 0x3c05);
    xcvBuildSpiCmd (confCmd, 0x08, 0x0a01);
    xcvBuildSpiCmd (confCmd, 0x10, 0x4600);
    xcvBuildSpiCmd (confCmd, 0x12, 0x0288);
    xcvBuildSpiCmd (confCmd, 0x14, 0xD688);
    xcvBuildSpiCmd (confCmd, 0x16, 0x8890);
    xcvBuildSpiCmd (confCmd, 0x18, 0x0256);    //Rxpll_r_bit=01;Rxpll_cp_bit=0101
    xcvBuildSpiCmd (confCmd, 0x1A, 0x022D);
    xcvBuildSpiCmd (confCmd, 0x1C, 0x6400);
    xcvBuildSpiCmd (confCmd, 0x1E, 0x8088);
    xcvBuildSpiCmd (confCmd, 0x20, 0x8890);
    xcvBuildSpiCmd (confCmd, 0x22, 0x803B);
    xcvBuildSpiCmd (confCmd, 0x24, 0x0244);      
    xcvBuildSpiCmd (confCmd, 0x26, 0x8434);
    xcvBuildSpiCmd (confCmd, 0x28, 0x7133);
    xcvBuildSpiCmd (confCmd, 0x2A, 0x8888);
    xcvBuildSpiCmd (confCmd, 0x2C, 0x9080);
    xcvBuildSpiCmd (confCmd, 0x2E, 0xD000);
    xcvBuildSpiCmd (confCmd, 0x34, 0xB66D);     //filter_cal_clk=26/128,else=26/16
    xcvBuildSpiCmd (confCmd, 0x36, 0x7B00);

    hal_RfspiSendCmd(confCmd);
    //===========DSP setting ==============================
    xcvBuildSpiCmd (confCmd, 0x44, 0xB000);
    xcvBuildSpiCmd (confCmd, 0x4A, 0xB00F);
    xcvBuildSpiCmd (confCmd, 0x4C, 0x4700);
    xcvBuildSpiCmd (confCmd, 0x50, 0xC239);  //c22d
    xcvBuildSpiCmd (confCmd, 0x52, 0x8000);
    xcvBuildSpiCmd (confCmd, 0x54, 0x0004);
    xcvBuildSpiCmd (confCmd, 0x56, 0x0229);     //dcc1 setting    gsm digrf_diff=1,8psk digrf_diff=0;
    xcvBuildSpiCmd (confCmd, 0x58, 0x2BDB);     //dcc2 setting
    xcvBuildSpiCmd (confCmd, 0x5A, 0x0100);
    xcvBuildSpiCmd (confCmd, 0x62, 0x0080);
    xcvBuildSpiCmd (confCmd, 0x64, 0x0800);
    xcvBuildSpiCmd (confCmd, 0x66, 0x0C84);
    xcvBuildSpiCmd (confCmd, 0x68, 0xB0DA);
    xcvBuildSpiCmd (confCmd, 0x6A, 0x12C9);
    xcvBuildSpiCmd (confCmd, 0x6C, 0x8000);
    xcvBuildSpiCmd (confCmd, 0x6E, 0xFCD0);
    xcvBuildSpiCmd (confCmd, 0x7A, 0x00A0);
    xcvBuildSpiCmd (confCmd, 0x7C, 0x6000);
    xcvBuildSpiCmd (confCmd, 0x7E, 0x9580);
    xcvBuildSpiCmd (confCmd, 0x96, 0x2700);    // tx digrf
    // xcvBuildSpiCmd (confCmd, 0x96, 0xC600);  //   rx digrf
    //xcvBuildSpiCmd (confCmd, 0x98, 0x4000);

    hal_RfspiSendCmd(confCmd);
    //rfd_xcvRamp0Set(confCmd);
    //rfd_xcvRamp1Set(confCmd);
    //rfd_xcvRamp2Set(confCmd);
    //rfd_xcvRamp3Set(confCmd);
    //===========Pllbb setting============================
    xcvBuildSpiCmd (confCmd, 0xB8, 0x3C04);
    xcvBuildSpiCmd (confCmd, 0xBA, 0x3040);
    xcvBuildSpiCmd (confCmd, 0xBC, 0x8080);
    xcvBuildSpiCmd (confCmd, 0xBE, 0x84C0);
    hal_RfspiSendCmd(confCmd);
#if 0

    // Set Gain
    xcvSetRxGain (80, 1, 32);
    UINT32 reg_90h = 0;    // 90 91 reg PLL
    UINT32 reg_40h = 0;    // 40 41 DSP
    UINT32 reg_42h = 0;    // 42 43
    UINT16 magic = g_xcvCmdCtx.magic;

    //     xcvBuildSpiCmd (spiCmd, 0x1a, 0x8008);
    xcvBuildSpiCmd (confCmd, 0x38, 0x8800|(magic ));
    xcvBuildSpiCmd (confCmd, 0x0a, 0x8800|(magic ));
    reg_90h = Gsm900_Rx_Fre[32].pllregvalue & 0xffff;
    reg_40h = (Gsm900_Rx_Fre[32].dspregvalue)>> 16 & 0xffff;
    reg_42h = Gsm900_Rx_Fre[32].dspregvalue & 0xffff;

    xcvBuildSpiCmd (confCmd, 0x90, reg_90h);
    xcvBuildSpiCmd (confCmd, 0x40, reg_40h);
    xcvBuildSpiCmd (confCmd, 0x42, reg_42h);

    g_xcvCmdCtx.digAfc = ((g_afcDacVal))&0x3fff;
    xcvBuildSpiCmd (confCmd, RDA6220_AFC_REG, g_xcvCmdCtx.digAfc);
    xcvBuildSpiCmd (confCmd, 0x04, 0x722a);  // dc  0x742a, 0624 0x722a
    xcvBuildSpiCmd (confCmd, 0x4c, 0x0700);   // mdll rx div 7
    xcvBuildSpiCmd (confCmd, 0xe8, 0x0070);   // Mdll rx div 7

    xcvBuildSpiCmd (confCmd, 0x5a, 0x11ff&g_xcvCmdCtx.diggain);
    hal_RfspiSendCmd(confCmd);


#endif
#if 0

    xcvBuildSpiCmd (confCmd, 0x30, 0x0003);
    hal_RfspiSendCmd(confCmd);
    xcvBuildSpiCmd (confCmd, 0x00, 0xff7f);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(1 MS_WAITING);

    xcvBuildSpiCmd (confCmd, 0x00, 0xffff);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(500 MS_WAITING);
    xcvBuildSpiCmd (confCmd, 0x00, 0xfffe);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(1 MS_WAITING);

    xcvBuildSpiCmd (confCmd, 0x00, 0xffff);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(5 MS_WAITING);
    xcvBuildSpiCmd (confCmd, 0x00, 0xfffd);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(1 MS_WAITING);

    xcvBuildSpiCmd (confCmd, 0x00, 0xffff);
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(20 MS_WAITING);
    xcvBuildSpiCmd (confCmd, 0x30, 0x0073);
    hal_RfspiSendCmd(confCmd);
#endif
    hal_HstSendEvent(SYS_EVENT,0x11112222);
#if 0
    xcvBuildSpiCmd (confCmd, 0x30, 0x5182);         //Spi_16bit_mode=1,register resetn=0
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(20000);  // delay 20 ms
    xcvBuildSpiCmd (confCmd, 0x30, 0x5183);         //Spi_16bit_mode=1
    hal_RfspiSendCmd(confCmd);
    hal_SysWaitMicrosecond(20000);  // delay 20 ms
    // ===========RTC setting============================
#if 1
    xcvBuildSpiCmd (confCmd, 0xA0, 0x0900);
    xcvBuildSpiCmd (confCmd, 0xA1, 0x2410);
    xcvBuildSpiCmd (confCmd, 0xA2, 0x8080);
    xcvBuildSpiCmd (confCmd, 0xA3, 0x0000);
    xcvBuildSpiCmd (confCmd, 0xA4, 0x58C3);
    xcvBuildSpiCmd (confCmd, 0xA5, 0x5904);
    xcvBuildSpiCmd (confCmd, 0xA6, 0x9004);
    xcvBuildSpiCmd (confCmd, 0xA7, 0xFBEA);
    xcvBuildSpiCmd (confCmd, 0xA7, 0xEBEA);
    xcvBuildSpiCmd (confCmd, 0xA8, 0x9448);
    xcvBuildSpiCmd (confCmd, 0xAA, 0xE000);  //0xE000
    xcvBuildSpiCmd (confCmd, 0x10, 0x4600);
    hal_RfspiSendCmd(confCmd);
#endif
#if 0
    int k = 0;
    { 

        for(k = 0; k < 0x10; k++)
        { 
            xcvBuildSpiCmd (confCmd, 0x50+k, 0x6565);     //SW_freq=4.8M
        }
        hal_RfspiSendCmd(confCmd);
        for(k = 0; k < 0x10; k++)
        { 
            xcvBuildSpiCmd (confCmd, 0x60+k, 0x6565);     //SW_freq=4.8M
        }
        hal_RfspiSendCmd(confCmd);
    }
#endif
#if 0 
    int i = 0,j = 0;
    for(j = 0; j < 0xf0;)
    { 
        if(j == 0x30)
        { 
            for(i = 0; i < 0x10; )
            { 
                xcvBuildSpiCmd (confCmd, j+i, 0x5183);     //SW_freq=4.8M
                i += 2;
            }
        }
        else
        { 
            for(i = 0; i < 0x10; )
            { 
                xcvBuildSpiCmd (confCmd, j+i, 0x6565);     //SW_freq=4.8M
                i += 2;
            }
        }
        j = j + 0x10;
        hal_RfspiSendCmd(confCmd);
    }
#endif
    // ===========analog setting==========================
    xcvBuildSpiCmd (confCmd, 0x12, 0x0288);
    xcvBuildSpiCmd (confCmd, 0x14, 0xD688);
    xcvBuildSpiCmd (confCmd, 0x16, 0x8890);
    xcvBuildSpiCmd (confCmd, 0x18, 0x0256);       // Rxpll_r_bit=01;Rxpll_cp_bit=0101
    xcvBuildSpiCmd (confCmd, 0x1A, 0x022D);
    xcvBuildSpiCmd (confCmd, 0x1C, 0xA480);
    xcvBuildSpiCmd (confCmd, 0x1E, 0xE088);
    xcvBuildSpiCmd (confCmd, 0x20, 0x8890);
    xcvBuildSpiCmd (confCmd, 0x22, 0x8003);
    xcvBuildSpiCmd (confCmd, 0x24, 0x0244);
    xcvBuildSpiCmd (confCmd, 0x26, 0x8434);
    xcvBuildSpiCmd (confCmd, 0x28, 0x7133);
    xcvBuildSpiCmd (confCmd, 0x2A, 0x8888);
    xcvBuildSpiCmd (confCmd, 0x2C, 0x9080);
    xcvBuildSpiCmd (confCmd, 0x2E, 0xD000);
    xcvBuildSpiCmd (confCmd, 0x34, 0x24C9);
    xcvBuildSpiCmd (confCmd, 0x36, 0xFE00);
    hal_RfspiSendCmd(confCmd);
    //===========DSP setting ==============================
    xcvBuildSpiCmd (confCmd, 0x44, 0x3000);
    xcvBuildSpiCmd (confCmd, 0x4A, 0xB00F);
    xcvBuildSpiCmd (confCmd, 0x4C, 0x4700);
    xcvBuildSpiCmd (confCmd, 0x50, 0x0253);
    xcvBuildSpiCmd (confCmd, 0x52, 0x0000);
    xcvBuildSpiCmd (confCmd, 0x54, 0x0004);
    xcvBuildSpiCmd (confCmd, 0x56, 0x0250);      //dcc1 setting    gsm digrf_diff=1,8psk digrf_diff=0;
    xcvBuildSpiCmd (confCmd, 0x58, 0xBBDB);      //dcc2 setting
    xcvBuildSpiCmd (confCmd, 0x5A, 0x1108);
    xcvBuildSpiCmd (confCmd, 0x62, 0x0080);
    xcvBuildSpiCmd (confCmd, 0x64, 0x0800);
    xcvBuildSpiCmd (confCmd, 0x66, 0x0C84);
    xcvBuildSpiCmd (confCmd, 0x68, 0xB0DA);
    xcvBuildSpiCmd (confCmd, 0x6A, 0x12C9);
    xcvBuildSpiCmd (confCmd, 0x6C, 0x8000);
    xcvBuildSpiCmd (confCmd, 0x6E, 0xFCD0);
    xcvBuildSpiCmd (confCmd, 0x7A, 0x00A0);
    xcvBuildSpiCmd (confCmd, 0x7C, 0x6000);
    xcvBuildSpiCmd (confCmd, 0x7E, 0x9580);
    xcvBuildSpiCmd (confCmd, 0xB8, 0x3CC4);
    xcvBuildSpiCmd (confCmd, 0xBA, 0x3050);
    xcvBuildSpiCmd (confCmd, 0xBC, 0x8080);
    xcvBuildSpiCmd (confCmd, 0xBE, 0xF680);
    hal_RfspiSendCmd(confCmd);
    rfd_xcvRamp0Set(confCmd);
    rfd_xcvRamp1Set(confCmd);
    rfd_xcvRamp2Set(confCmd);
    rfd_xcvRamp3Set(confCmd);
#if 0
    // ========Pa_ramp_point==========
    //pa ramp curve0 for PCL5~12
    xcvBuildSpiCmd (confCmd, 0x32, 0x0B00);
    xcvBuildSpiCmd (confCmd, 0x32, 0x0F01);
    xcvBuildSpiCmd (confCmd, 0x32, 0x1702);
    xcvBuildSpiCmd (confCmd, 0x32, 0x2703);
    xcvBuildSpiCmd (confCmd, 0x32, 0x3704);
    xcvBuildSpiCmd (confCmd, 0x32, 0x4B05);
    xcvBuildSpiCmd (confCmd, 0x32, 0x5F06);
    xcvBuildSpiCmd (confCmd, 0x32, 0x7307);
    xcvBuildSpiCmd (confCmd, 0x32, 0x8B08);
    xcvBuildSpiCmd (confCmd, 0x32, 0xA709);
    xcvBuildSpiCmd (confCmd, 0x32, 0xBF0A);
    xcvBuildSpiCmd (confCmd, 0x32, 0xD70B);
    xcvBuildSpiCmd (confCmd, 0x32, 0xE70C);
    xcvBuildSpiCmd (confCmd, 0x32, 0xF30D);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFB0E);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF0F);

    //pa ramp curve1 for PCL6~17
    xcvBuildSpiCmd (confCmd, 0x32, 0x0B10);
    xcvBuildSpiCmd (confCmd, 0x32, 0x0F11);
    xcvBuildSpiCmd (confCmd, 0x32, 0x1712);
    xcvBuildSpiCmd (confCmd, 0x32, 0x2713);
    xcvBuildSpiCmd (confCmd, 0x32, 0x3714);
    xcvBuildSpiCmd (confCmd, 0x32, 0x5315);
    xcvBuildSpiCmd (confCmd, 0x32, 0x7316);
    xcvBuildSpiCmd (confCmd, 0x32, 0x8F17);
    xcvBuildSpiCmd (confCmd, 0x32, 0xA818);
    xcvBuildSpiCmd (confCmd, 0x32, 0xCB19);
    xcvBuildSpiCmd (confCmd, 0x32, 0xE71A);
    xcvBuildSpiCmd (confCmd, 0x32, 0xF71B);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF1C);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF1D);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF1E);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF1F);

    //pa ramp curve2 for PCL9~19
    xcvBuildSpiCmd (confCmd, 0x32, 0x2B20);
    xcvBuildSpiCmd (confCmd, 0x32, 0x4721);
    xcvBuildSpiCmd (confCmd, 0x32, 0x5B22);
    xcvBuildSpiCmd (confCmd, 0x32, 0x6F23);
    xcvBuildSpiCmd (confCmd, 0x32, 0x8724);
    xcvBuildSpiCmd (confCmd, 0x32, 0xA325);
    xcvBuildSpiCmd (confCmd, 0x32, 0xBF26);
    xcvBuildSpiCmd (confCmd, 0x32, 0xD327);
    xcvBuildSpiCmd (confCmd, 0x32, 0xE328);
    xcvBuildSpiCmd (confCmd, 0x32, 0xEF29);
    xcvBuildSpiCmd (confCmd, 0x32, 0xF72A);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF2B);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF2C);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF2D);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF2E);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF2F);

    //pa ramp curve3 for PCL14~19
    xcvBuildSpiCmd (confCmd, 0x32, 0x5B30);
    xcvBuildSpiCmd (confCmd, 0x32, 0x6F31);
    xcvBuildSpiCmd (confCmd, 0x32, 0x7F32);
    xcvBuildSpiCmd (confCmd, 0x32, 0x8F33);
    xcvBuildSpiCmd (confCmd, 0x32, 0x9F34);
    xcvBuildSpiCmd (confCmd, 0x32, 0xB335);
    xcvBuildSpiCmd (confCmd, 0x32, 0xCB36);
    xcvBuildSpiCmd (confCmd, 0x32, 0xDF37);
    xcvBuildSpiCmd (confCmd, 0x32, 0xEB38);
    xcvBuildSpiCmd (confCmd, 0x32, 0xF739);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3A);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3B);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3C);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3D);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3E);
    xcvBuildSpiCmd (confCmd, 0x32, 0xFF3F);
#endif

    // ================mode setting ========
    xcvBuildSpiCmd (confCmd, 0x58, 0x0BDB);         // dcc2 setting
    hal_RfspiSendCmd(confCmd);
#if 0
    // ===============BS &GPIO setting=======
    //item:XCV_TX_Hband_GMK_digrf
    xcvBuildSpiCmd (confCmd, 0x12, 0x0788);
    xcvBuildSpiCmd (confCmd, 0x1A, 0x322D);
    xcvBuildSpiCmd (confCmd, 0x22, 0x8003);
    xcvBuildSpiCmd (confCmd, 0x2C, 0x9181);
    xcvBuildSpiCmd (confCmd, 0x68, 0xB0DA);        //shift_ct_lo=4,shift_ct_hi=3;
    xcvBuildSpiCmd (confCmd, 0xB6, 0x0001);        //pu_dac_ramp
    xcvBuildSpiCmd (confCmd, 0x56, 0x0250);        //digrf_diff=1,GMSK
    xcvBuildSpiCmd (confCmd, 0x96, 0x2700);

    hal_RfspiSendCmd(confCmd);
    // item:XCV_TX_Lband_GMK_digrf
    xcvBuildSpiCmd (confCmd, 0x12, 0x0388);
    xcvBuildSpiCmd (confCmd, 0x1A, 0x322D);
    xcvBuildSpiCmd (confCmd, 0x20, 0x8891);
    xcvBuildSpiCmd (confCmd, 0x22, 0x8103);
    xcvBuildSpiCmd (confCmd, 0x68, 0xC0DA);        //shift_ct_lo=4,shift_ct_hi=3;
    xcvBuildSpiCmd (confCmd, 0xB6, 0x0001);        //pu_dac_ramp
    xcvBuildSpiCmd (confCmd, 0x56, 0x0250);        //digrf_diff=1,GMSK
    xcvBuildSpiCmd (confCmd, 0x96, 0x2700);

    hal_RfspiSendCmd(confCmd);

    // item:XCV_RX_test_HBAND_digrf
    xcvBuildSpiCmd (confCmd, 0x12, 0x0588);    //Vco_txbuf_en=0
    xcvBuildSpiCmd (confCmd, 0x1A, 0x322D);
    xcvBuildSpiCmd (confCmd, 0x22, 0x8001);    //reserved_analog<1>=0
    xcvBuildSpiCmd (confCmd, 0x24, 0x024C);
    xcvBuildSpiCmd (confCmd, 0x26, 0x8434);    //Aux_dac_out_enable=0
    xcvBuildSpiCmd (confCmd, 0xB8, 0x3DD4);    //pu_lna=1,lna_hband_sel=1
    xcvBuildSpiCmd (confCmd, 0xBA, 0x30D3);    //pu_mixer=1,mixer_hband_sel=1,pu_pga
    xcvBuildSpiCmd (confCmd, 0x96, 0xC608);
    xcvBuildSpiCmd (confCmd, 0x28, 0x73B3);    //dac_mux_en=11;
    xcvBuildSpiCmd (confCmd, 0xBE, 0x8442);    //pu_filter=1, Filter_lp_mode, Filter_rstn=0
    xcvBuildSpiCmd (confCmd, 0xBE, 0x84C2);    //pu_filter=1, Filter_lp_mode, Filter_rstn=1

    hal_RfspiSendCmd(confCmd);
    //item:XCV_RX_test_LBAND_digrf
    xcvBuildSpiCmd (confCmd, 0x12, 0x0188);    //Vco_txbuf_en=0
    xcvBuildSpiCmd (confCmd, 0x1A, 0x322D);
    xcvBuildSpiCmd (confCmd, 0x22, 0x8002);    //reserved_analog<0>=0
    xcvBuildSpiCmd (confCmd, 0x24, 0x024C);
    xcvBuildSpiCmd (confCmd, 0x26, 0x8434);    //Aux_dac_out_enable=0
    xcvBuildSpiCmd (confCmd, 0xB8, 0x3DE4);    //pu_lna=1,lna_lband_sel=1
    xcvBuildSpiCmd (confCmd, 0xBA, 0x30D5);    //pu_mixer=1,mixer_lband_sel=1,pu_pga
    xcvBuildSpiCmd (confCmd, 0x96, 0xC608);
    xcvBuildSpiCmd (confCmd, 0x28, 0x73B3);    //dac_mux_en=11;
    xcvBuildSpiCmd (confCmd, 0xBE, 0x8442);    //pu_filter=1, Filter_lp_mode, Filter_rstn=0
    xcvBuildSpiCmd (confCmd, 0xBE, 0x84C2);    //pu_filter=1, Filter_lp_mode, Filter_rstn=1
#endif


    //item:XCV_setting for PMU initial 
    //    xcvBuildSpiCmd (confCmd, 0x30, 0x5182);     //Spi_16bit_mode=1,register resetn=0
    // thehdw.Wait 0.02
    hal_SysWaitMicrosecond(20000);  // delay 10 ms
    //  xcvBuildSpiCmd (confCmd, 0x30, 0x5183);     //Spi_16bit_mode=1
    //  xcvBuildSpiCmd (confCmd, 0xc2, 0x0237);     //[5:4]=11
    //  xcvBuildSpiCmd (confCmd, 0xcc, 0x1209);     //[14:13]=00 clkgen=pll_624M;[14:13]=10 clkgen=clk_xtal;
    //  xcvBuildSpiCmd (confCmd, 0xcd, 0x0201);     //SW_freq=4.8M
#endif
    hal_SysRequestFreq(HAL_SYS_FREQ_XCV, HAL_SYS_FREQ_32K, NULL);
}

VOID rfd_XcvClose(VOID)
{
    // Nothing to be done for the Xcver...
    hal_RfspiClose();
}

#if 0
VOID RFD_FUNC_INTERNAL rfd_XcvWakeUp (VOID)
{
    return;
    if (g_lastSleepMode == RFD_POWER_OFF)
    {
        HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
        UINT32 bitfield = CDAC_REG_VALUE_BASE | (((g_CDAC + g_CdacDelta)&RDA6220_CDAC_REG_MASK) << RDA6220_CDAC_REG_OFFSET);
        // Clear 8051 reset
        xcvBuildSpiCmd (confCmd, 0x30, 0x0083);
        xcvBuildSpiCmd  (confCmd, RDA6220_CDAC_REG, bitfield);
        // 8051 takes the control
        //       xcvBuildSpiCmd (confCmd, 0x30, 0x0003);
        hal_RfspiSendCmd(confCmd);

        g_lastSleepMode = RFD_POWER_ON;
    }
}

// *** xcvSleep (bool toDeepPD) ***
VOID RFD_FUNC_INTERNAL rfd_XcvSleep (RFD_LP_MODE_T lp_mode)
{
    return;
    if(RFD_LOW_POWER == lp_mode)
    {
        hal_TcuClrTco(HAL_TCU_TCO_RX_DEC_ON);
        hal_TcuClrTco(XCV->PDN);
    }
    else if(RFD_POWER_OFF== lp_mode)
    {
        INT32 afc_offset_ppm;
        UINT16 step_offset_lp = 0;
        INT16 lp_afcDacValue = g_afcDacVal + 0x2000;
        INT16 lp_afcDacOffset = (1<<9)-(lp_afcDacValue&0x1ff)+(1<<7);
        if(lp_afcDacValue <= (RDA6220_AFC_MID + 0x2000 - RDA6220_AFC_HRANGE))
        {
            afc_offset_ppm = (lp_afcDacOffset * 2 / 3) * XCV_PARAM(AFC_GAIN(GSM_BAND_GSM900)) / (1000 * 900);
        }
        else if(lp_afcDacValue <= (RDA6220_AFC_MID + 0x2000 + RDA6220_AFC_HRANGE))
        {
            afc_offset_ppm = (lp_afcDacOffset) * XCV_PARAM(AFC_GAIN(GSM_BAND_GSM900)) / (1000 * 900);
        }
        else
        {
            afc_offset_ppm = (lp_afcDacOffset * 4 / 3) * XCV_PARAM(AFC_GAIN(GSM_BAND_GSM900)) / (1000 * 900);
        }

        if (afc_offset_ppm<0)
        {
            step_offset_lp = 0;
        }
        else
        {
            step_offset_lp = (afc_offset_ppm * 13) >> 4 ;//afc_offset_ppm *  6.5 / 8
        }

        // It is safe to send RFSPI commands when ready to enter deep sleep mode
        HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
        // Reset 8051, and RFSPI takes the full control
        xcvBuildSpiCmd (confCmd, 0x30, 0x0183);

        xcvBuildSpiCmd (confCmd, 0xa0, 0xc100); // Enable_clk_6p5m=1
        //xcvBuildSpiCmd (confCmd, 0x03, 0x0);
        //xcvBuildSpiCmd (confCmd, 0x02, (step_offset_lp)&0xff);
        xcvBuildSpiCmd (confCmd, 0xae, 0x0041);  // abc1
        xcvBuildSpiCmd (confCmd, 0xac, (step_offset_lp)&0xff);

        xcvBuildSpiCmd (confCmd, 0xa0, 0xe100); // Enable_clk_6p5m=1
        hal_RfspiSendCmd(confCmd);
    }

    g_lastSleepMode = lp_mode;
}
#endif

//======================================================================
// rfd_XcvGetAfcDacOffset
// This function used to get the difference between current g_afcDacVal and AFC_MID
//----------------------------------------------------------------------
PUBLIC INT16 rfd_XcvGetAfcDacOffset(VOID)
{
    return g_afcDacVal - RDA6220_AFC_MID;
}

//======================================================================
// rfd_XcvGetCdacOffset
// This function used to get current CDAC offset
//----------------------------------------------------------------------
PUBLIC INT16 rfd_XcvGetCdacOffset(VOID)
{
    return g_CdacDelta;
}

//======================================================================
// rfd_XcvSetAfcDacOffset
// This function used to set g_afcDacVal
//----------------------------------------------------------------------
PUBLIC VOID rfd_XcvSetAfcDacOffset(INT16 offset)
{
    g_afcDacVal = RDA6220_AFC_MID + offset;
}

//======================================================================
// rfd_XcvSetCdacOffset
// This function used to set CDAC offset
//----------------------------------------------------------------------
PUBLIC VOID rfd_XcvSetCdacOffset(INT16 offset)
{
    g_CdacDelta = offset;
}

#ifdef FM_LDO_WORKAROUND
PUBLIC VOID rfd_XcvSetDcdcLdoMode(BOOL dcdcOn, BOOL ldoOn)
{
    UINT32 count = 0;
    UINT32 status = hal_SysEnterCriticalSection();

    if (dcdcOn)
    {
        g_RxPllFreqReg_05h |= RX_PLL_FREQ_REG_DCDC_ON;
    }
    else
    {
        g_RxPllFreqReg_05h &= ~RX_PLL_FREQ_REG_DCDC_ON;
    }
    if (ldoOn)
    {
        g_RxPllFreqReg_05h |= RX_PLL_FREQ_REG_LDO_ON;
    }
    else
    {
        g_RxPllFreqReg_05h &= ~RX_PLL_FREQ_REG_LDO_ON;
    }

    // Never directly write to XCV if PAL is working, otherwise it
    // might conflict with XCV Rx/Tx commands when CHANNEL_FREQUENCY_MODE
    // is defined
    while (!(hal_RfspiImmReadWriteAvail() && hal_LpsRemainingFramesReady(2)))
    {
        // Piggyback dcdc commands with XCV Rx commonds
        g_xcvDcdcLdoModeReqStatus = 1;
        count = 0;
        while (count++ < MAX_PIGGYBACK_WAIT_COUNT)
        {
            if (count == 1)
            {
                COS_Sleep(10);
            }
            else
            {
                COS_Sleep(5);
            }

            if (g_xcvDcdcLdoModeReqStatus == 4)
            {
                // Dcdc piggyback commands have been sent to XCV
                hal_SysExitCriticalSection(status);
                return;
            }
        }
        // Cancel dcdc piggyback commands
        g_xcvDcdcLdoModeReqStatus = 0;
    }

    // Now we can directly write to XCV
    rfd_XcvRegForceWriteSingle(0x05, g_RxPllFreqReg_05h);

    hal_SysExitCriticalSection(status);
}
#endif

//----------------------//
// Calibration Related  //
//----------------------//
#if 0
#define CALIB_DONE      0
#define CALIB_NOTDONE   1
#define CALIB_ERROR     0xFF
// State for Xtal calibration
enum {GUESS_CDAC,SEARCH_CDAC,DONE_CDAC};
UINT8 xtal_state = GUESS_CDAC;
// State of the iloss calibration
BOOL g_rfdILossInit = TRUE;

const CALIB_XCV_T xcv_calib_default =
{
    DEFAULT_CALIB_TIMES,
    DEFAULT_CALIB_PARAM,
    DEFAULT_CALIB_PALCUST
};

const CALIB_XCV_T * rfd_XcvCalibGetDefault(VOID)
{
    return &xcv_calib_default;
}

VOID rfd_XcvCalibUpdateValues(VOID)
{
    return;
}

VOID rfd_XcvCalibResetValues(VOID)
{
    // When the current CDAC is different from the init CDAC value,
    // we reset the CDAC value.
    HAL_RFSPI_CMD_T* spiCmd = hal_RfspiNewSingleCmd();
    if (g_CDAC != RDA6220_CDAC_DEFAULT)
    {
        // In this case, we reset the CDAC to default value.

        // Xtal triming capacitor register to default (calibrated)
        g_CDAC = RDA6220_CDAC_DEFAULT;
        UINT32 bitfield = CDAC_REG_VALUE_BASE | ((g_CDAC&RDA6220_CDAC_REG_MASK) << RDA6220_CDAC_REG_OFFSET);
        xcvBuildSpiCmd (spiCmd, RDA6220_CDAC_REG, bitfield);
    }
    rfd_xcvRamp0Set(spiCmd);
    rfd_xcvRamp1Set(spiCmd);
    rfd_xcvRamp2Set(spiCmd);
    rfd_xcvRamp3Set(spiCmd);

    // Reset Xtal calib state
    xtal_state = GUESS_CDAC;
    // Reset iloss calib state
    g_rfdILossInit = TRUE;
    return;
}

UINT8 rfd_XcvCalibXtalFreqOffset(INT32 FOf)
{

    // 1 CDAC unit =~ 1900Hz
#define CDAC_UNIT 1900
#define FIRST_FOF 0x7FFF
#define OPPOSITE_SIGN(n,m) (((n>0)&&(m<0))||((m>0)&&(n<0)))
    //#define ABS(x)   (((x)>0)?(x):-(x))

    static INT16 last_FOf = FIRST_FOF;
    static UINT8 last_Cdac = 0;

    HAL_RFSPI_CMD_T *spiCmd;

    switch (xtal_state)
    {
        case GUESS_CDAC:
            last_FOf = FIRST_FOF;
            last_Cdac = g_CDAC;

            g_CDAC -= (FOf / CDAC_UNIT);
            xtal_state = SEARCH_CDAC;

            // Check CDAC boundaries
            if ((g_CDAC > RDA6220_CDAC_MAX)||(g_CDAC < RDA6220_CDAC_MIN))
            {
                //xcv_fprintf((RFD_INFO_TRC,"xcvCDAC @ %d Qb",g_CDAC));
                return CALIB_ERROR;
            }
            break;

        case SEARCH_CDAC:
            // Check if we changed sign
            if (OPPOSITE_SIGN(FOf,last_FOf) && (last_FOf != FIRST_FOF))
            {
                // We have the good CDAC
                if (ABS(FOf) < ABS(last_FOf))
                {
                    XCV_PARAM(CDAC_FREQ) = (INT32)FOf;
                    if (g_xcvCalibPtr->hstXcv != NULL)
                        HST_XCV_PARAM(CDAC_FREQ) = (INT32)FOf;
                    return CALIB_DONE;
                }
                else // Last CDAC was the good one
                {
                    XCV_PARAM(CDAC_FREQ) = (INT32)last_FOf;
                    if (g_xcvCalibPtr->hstXcv != NULL)
                        HST_XCV_PARAM(CDAC_FREQ) = (INT32)last_FOf;
                    g_CDAC = last_Cdac;
                    xtal_state = DONE_CDAC;
                }
            }
            else // still searching...
            {
                last_Cdac = g_CDAC;
                if (FOf < 0) g_CDAC++;
                else g_CDAC--;

                // Check CDAC boundaries
                if ((g_CDAC > RDA6220_CDAC_MAX)||(g_CDAC < RDA6220_CDAC_MIN))
                    return CALIB_ERROR;
            }
            last_FOf = FOf;
            break;
    }

    // Program CDAC change early next frame
    //RFD_TRACE(TSTDOUT,0,"XTAL xcvCDAC @ %d Qb",g_CDAC);
    spiCmd = hal_RfspiNewCmd(0,0);
    UINT32 bitfield = CDAC_REG_VALUE_BASE | ((g_CDAC&RDA6220_CDAC_REG_MASK) << RDA6220_CDAC_REG_OFFSET);
    xcvBuildSpiCmd (spiCmd, 0x30, 0x0083);
    xcvBuildSpiCmd (spiCmd, RDA6220_CDAC_REG, bitfield);
    //    xcvBuildSpiCmd (spiCmd, 0x30, 0x0003);
    // Update calibration structure
    XCV_PARAM(XTAL) = g_CDAC;
    if (g_xcvCalibPtr->hstXcv != NULL)
        HST_XCV_PARAM(XTAL) = g_CDAC;

    // We can stop right now are we are sure that at least one
    // FINT will happen util the stub goes to another task, so
    // the CDAC value will be changed.
    if (xtal_state == DONE_CDAC) return CALIB_DONE;

    return CALIB_NOTDONE;
}

VOID rfd_XcvCalibSetAfcBound(RFD_AFC_BOUND_T bound)
{
    HAL_RFSPI_CMD_T *spiCmd = hal_RfspiNewCmd(0,0);

    if (RFD_AFC_BOUND_CENTER == bound)
        g_afcDacVal = RDA6220_AFC_MID;
    else if (RFD_AFC_BOUND_POS == bound)
        g_afcDacVal = RDA6220_AFC_BOUND_HIGH;
    else
        g_afcDacVal = RDA6220_AFC_BOUND_LOW;

    // Set DigitalAfc, 14 bit is set for digital AFC
    g_xcvCmdCtx.digAfc = ((g_afcDacVal))&0x3fff;
    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG, g_xcvCmdCtx.digAfc);
    g_CdacDelta = 0;
    //#endif
}

VOID rfd_XcvCalibCalcAfcGain(INT32 FreqOffset, GSM_RFBAND_T band)
{
    INT32 gain = (FreqOffset * 1000) / (2 * RDA6220_AFC_HRANGE);

    // Update calibration structure
    XCV_PARAM(AFC_GAIN(band)) = gain;
    if (g_xcvCalibPtr->hstXcv != NULL)
        HST_XCV_PARAM(AFC_GAIN(band)) = gain;
}

VOID rfd_XcvCalibSetChangeDAC(void)
{
    HAL_RFSPI_CMD_T *spiCmd = hal_RfspiNewCmd(0,0);
    //hal_HstSendEvent(g_CDAC); 
    UINT32 bitfield = CDAC_REG_VALUE_BASE | (((g_CDAC+RDA6220_CDAC_OFFSET)&RDA6220_CDAC_REG_MASK) << RDA6220_CDAC_REG_OFFSET);
    xcvBuildSpiCmd (spiCmd, RDA6220_CDAC_REG, bitfield);
    //hal_HstSendEvent(g_CDAC+RDA6220_CDAC_OFFSET); 
    g_CdacDelta = RDA6220_CDAC_OFFSET;
}

VOID rfd_XcvCalibSetAfcBoundChangeDAC(RFD_AFC_BOUND_T bound)
{
    HAL_RFSPI_CMD_T *spiCmd = hal_RfspiNewCmd(0,0);
    rfd_XcvCalibSetChangeDAC();
    //hal_HstSendEvent(0xca11afc); 

    if (RFD_AFC_BOUND_CENTER == bound)
        g_afcDacVal = RDA6220_AFC_MID;
    else if (RFD_AFC_BOUND_POS == bound)
        g_afcDacVal = RDA6220_AFC_BOUND_HIGH;
    else
        g_afcDacVal = RDA6220_AFC_BOUND_LOW;

    // Set DigitalAfc, 14 bit is set for digital AFC
    g_xcvCmdCtx.digAfc = ((g_afcDacVal))&0x3fff;
    xcvBuildSpiCmd (spiCmd, RDA6220_AFC_REG, g_xcvCmdCtx.digAfc);
}

VOID rfd_XcvCalibCalcAfcGainChangeDAC(INT32 FreqOffset, GSM_RFBAND_T band)
{
    INT32 gain = (FreqOffset * 1000) / (2 * RDA6220_AFC_HRANGE);

    // Update calibration structure
    XCV_PARAM(AFC_GAIN2(band)) = gain;
    if (g_xcvCalibPtr->hstXcv != NULL)
        HST_XCV_PARAM(AFC_GAIN2(band)) = gain;
    g_CdacDelta = 0;
}

VOID rfd_XcvCalibCalcAfcFreqChangeDAC(INT32 FreqOffset, GSM_RFBAND_T band)
{
    // Update calibration structure
    XCV_PARAM(AFC_FREQ(band)) = FreqOffset;
    if (g_xcvCalibPtr->hstXcv != NULL)
        HST_XCV_PARAM(AFC_FREQ(band)) = FreqOffset;
}

// ILoss ARFCN Table
PRIVATE UINT16 g_rfdILossArfcnTab850[6] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_850;
PRIVATE UINT16 g_rfdILossArfcnTabGSM[6] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_GSM;
PRIVATE UINT16 g_rfdILossArfcnTabDCS[9] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_DCS;
PRIVATE UINT16 g_rfdILossArfcnTabPCS[9] = RFD_DEFAULT_CALIB_ILOSS_ARFCN_PCS;

PRIVATE UINT8    g_rfdILossNum;
PRIVATE UINT16*  g_rfdTmpArfcn;

PRIVATE UINT32   g_rfdTmpOffset;

UINT8 rfd_XcvCalibSetILoss(INT8 offset, GSM_RFBAND_T band, UINT16* nextArfcn)
{
    // First call of this function after the restart of the system or after
    // after a call to the xcv_CalibResetValues function.


    if (TRUE == g_rfdILossInit)
    {
        // iloss calibration initialization. Choose the band which need calibration.
        g_rfdTmpArfcn = (band == GSM_BAND_GSM850 )? g_rfdILossArfcnTab850:
            (band == GSM_BAND_DCS1800)? g_rfdILossArfcnTabDCS:
            (band == GSM_BAND_PCS1900)? g_rfdILossArfcnTabPCS:
            g_rfdILossArfcnTabGSM;

        if(GSM_BAND_DCS1800 == band)
            XCV_PARAM(ILOSS(band)) = 0x44444444;
        else if(GSM_BAND_PCS1900 == band)
            XCV_PARAM(ILOSS(band)) = 0x55555555;
        else
            XCV_PARAM(ILOSS(band)) = 0x33333;
        g_rfdTmpOffset = 0;
        g_rfdILossNum = 0;
        g_rfdILossInit = FALSE;
        *nextArfcn = g_rfdTmpArfcn[g_rfdILossNum];
        return CALIB_PROCESS_CONTINUE;
    }

    offset += ((XCV_PARAM(ILOSS(band)) >> (4*g_rfdILossNum)) & 0xF);
    g_rfdTmpOffset |= (offset << (4*g_rfdILossNum));
    *nextArfcn = g_rfdTmpArfcn[++g_rfdILossNum];

    // Judge the sign of iloss calibration.
    if(*nextArfcn == 0)
    {
        g_rfdILossInit = TRUE;

        XCV_PARAM(ILOSS(band)) = g_rfdTmpOffset;
        if (g_xcvCalibPtr->hstXcv != NULL)
            HST_XCV_PARAM(ILOSS(band)) = g_rfdTmpOffset;

        return CALIB_PROCESS_STOP;
    }

    return CALIB_PROCESS_CONTINUE;
}

PUBLIC VOID rfd_XcvCalibSetPaRamp (UINT16* dacValue)
{
    g_xcvCalibSetPaRamp = TRUE;
    int i = 0;
    for(i = 0; i < 4;i++)
        g_xcvCalibPaRampValue[i] = dacValue[i];
}
#endif

PROTECTED VOID rfd_XcvRegForceWriteSingle(UINT32 addr, UINT32 data)
{
    UINT8 array[RDA6220_ONE_CMD_BYTE_QTY];
    UINT32 cmdWord = RDA6220_WRITE_FMT(addr, data);

    // Hardcoded
    array[0] = (cmdWord >> (3 * BYTE_SIZE)) & BYTE_MASK;
    array[1] = (cmdWord >> (2 * BYTE_SIZE)) & BYTE_MASK;
    array[2] = (cmdWord >> (BYTE_SIZE)) & BYTE_MASK;
    array[3] =  cmdWord & BYTE_MASK;

    hal_RfspiImmWrite(array, RDA6220_ONE_CMD_BYTE_QTY);
}

VOID rfd_XcvPowerDown(VOID)
{
    HAL_RFSPI_CMD_T* confCmd = hal_RfspiNewSingleCmd();
    xcvBuildSpiCmd (confCmd, 0x3F, 0x00003); // Page3
    xcvBuildSpiCmd (confCmd, 0x04, 0x143f1); // 26M off, LPO on
    xcvBuildSpiCmd (confCmd, 0x01, 0x3e400); // Enable_clk_6p5m=0
    xcvBuildSpiCmd (confCmd, 0x3F, 0x00003); // Page0
    hal_RfspiSendCmd(confCmd);
}

#ifdef SPI_REG_DEBUG
// ======================================================
// Debug: Read and Write Registers via SPI

//#define XCV_REG_BUFFER_READ_WRITE

#define XCV_REG_VALUE_LOCATION __attribute__((section(".xcv_reg_value")))

#define SPI_REG_WRITE_FLAG (1<<31)
#define SPI_REG_READ_FLAG (1<<30)

#ifdef XCV_REG_BUFFER_READ_WRITE
#define SPI_REG_FORCE_WRITE_FLAG (1<<29)
#define SPI_REG_FORCE_READ_FLAG (1<<28)
#else // XCV_REG_BUFFER_READ_WRITE
#define SPI_REG_FORCE_WRITE_FLAG SPI_REG_WRITE_FLAG
#define SPI_REG_FORCE_READ_FLAG SPI_REG_READ_FLAG
#endif // ! XCV_REG_BUFFER_READ_WRITE

#define XCV_REG_DATA_MASK (RDA6220_DATA_MASK)

#define XCV_REG_PAGE_NUM (1)
#define XCV_REG_NUM_PER_PAGE (0xE8/2)


UINT32 XCV_REG_VALUE_LOCATION g_rfdXcvRegValue[XCV_REG_PAGE_NUM][XCV_REG_NUM_PER_PAGE];

UINT32 g_rfdXcvRegForceWriteError = 0;
UINT32 g_rfdXcvRegForceReadError = 0;

#ifdef XCV_REG_BUFFER_READ_WRITE
UINT32 g_rfdXcvCurPage = 0;
UINT32 g_rfdXcvRegLastWriteValue[XCV_REG_PAGE_NUM][XCV_REG_NUM_PER_PAGE];
#define XCV_WRITE_LIST_LEN (0xa)
UINT32 RFD_DATA_INTERNAL g_rfdXcvRegWriteList[XCV_WRITE_LIST_LEN];

PROTECTED BOOL rfd_XcvRegWriteListInsert(UINT32 page, UINT32 addr, UINT32 data)
{
    int i;
    for (i=0; i<XCV_WRITE_LIST_LEN; i++)
    {
        if (g_rfdXcvRegWriteList[i] == 0) break;
    }

    if (i == XCV_WRITE_LIST_LEN) return FALSE;

    g_rfdXcvRegWriteList[i] = SPI_REG_WRITE_FLAG |
        (addr&RDA6220_ADDR_MASK)<<24 |
        (page&0x3)<<20 |
        (data&RDA6220_DATA_MASK);
    return TRUE;
}
#endif // XCV_REG_BUFFER_READ_WRITE

PROTECTED BOOL rfd_XcvRegForceWrite(UINT32 page, UINT32 addr, UINT32 data)
{
    BOOL result = FALSE;
    page &= 0x7;
    UINT32 status = hal_SysEnterCriticalSection();
    if (hal_RfspiImmReadWriteAvail())
    {
        rfd_XcvRegForceWriteSingle(addr<<1, data);
        result = TRUE;
    }
    hal_SysExitCriticalSection(status);
    return result;
}

PROTECTED BOOL rfd_XcvRegForceRead(UINT32 page, UINT32 addr, UINT32 *pData)
{
    BOOL result = FALSE;
    page &= 0x7;
    UINT32 status = hal_SysEnterCriticalSection();
    if (hal_RfspiImmReadWriteAvail())
    {
        xcvReadReg(addr<<1, (UINT8 *)pData);
        result = TRUE;
    }
    hal_SysExitCriticalSection(status);
    return result;
}

PROTECTED VOID rfd_XcvRegValueInit(VOID)
{
    for (int page=0; page<XCV_REG_PAGE_NUM; page++)
    {
        for (int i=0; i<XCV_REG_NUM_PER_PAGE; i++)
        {
            g_rfdXcvRegValue[page][i] = 0;
        }
    }
}

PROTECTED VOID rfd_XcvRegCheckSinglePage(UINT32 page)
{
    if (page >= XCV_REG_PAGE_NUM)
    {
        return;
    }

    for (int i=0; i<XCV_REG_NUM_PER_PAGE; i++)
    {
#ifdef XCV_REG_BUFFER_READ_WRITE
        if (g_rfdXcvRegValue[page][i] & SPI_REG_WRITE_FLAG)
        {
            if (!rfd_XcvRegWriteListInsert(page, i, g_rfdXcvRegValue[page][i]&XCV_REG_DATA_MASK))
            {
                continue;
            }
            g_rfdXcvRegValue[page][i] &= ~SPI_REG_WRITE_FLAG;
        }
        if (g_rfdXcvRegValue[page][i] & SPI_REG_READ_FLAG)
        {
            g_rfdXcvRegValue[page][i] &= ~(SPI_REG_READ_FLAG|XCV_REG_DATA_MASK);
            g_rfdXcvRegValue[page][i] |= g_rfdXcvRegLastWriteValue[page][i]&XCV_REG_DATA_MASK;
        }
#endif //XCV_REG_BUFFER_READ_WRITE

        if (g_rfdXcvRegValue[page][i] & SPI_REG_FORCE_WRITE_FLAG)
        {
            if (!rfd_XcvRegForceWrite(page, i, g_rfdXcvRegValue[page][i]&XCV_REG_DATA_MASK))
            {
                g_rfdXcvRegForceWriteError++;
                continue;
            }
#ifdef XCV_REG_BUFFER_READ_WRITE
            g_rfdXcvRegLastWriteValue[page][i] = g_rfdXcvRegValue[page][i]&XCV_REG_DATA_MASK;
#endif // XCV_REG_BUFFER_READ_WRITE
            g_rfdXcvRegValue[page][i] &= ~SPI_REG_FORCE_WRITE_FLAG;
        }
        if (g_rfdXcvRegValue[page][i] & SPI_REG_FORCE_READ_FLAG)
        {
            UINT32 data;
            if (!rfd_XcvRegForceRead(page, i, &data))
            {
                g_rfdXcvRegForceReadError++;
                continue;
            }
#ifdef XCV_REG_BUFFER_READ_WRITE
            g_rfdXcvRegLastWriteValue[page][i] = data&XCV_REG_DATA_MASK;
#endif // XCV_REG_BUFFER_READ_WRITE
            g_rfdXcvRegValue[page][i] &= ~(SPI_REG_FORCE_READ_FLAG|XCV_REG_DATA_MASK);
            g_rfdXcvRegValue[page][i] |= data&XCV_REG_DATA_MASK;
        }
    }
}

PUBLIC VOID rfd_XcvRegCheck(VOID)
{
    static BOOL initDone = FALSE;
    if (!initDone)
    {
        rfd_XcvRegValueInit();
        initDone = TRUE;
    }

    for (UINT32 page=0; page<XCV_REG_PAGE_NUM; page++)
    {
        rfd_XcvRegCheckSinglePage(page);
    }
}

PUBLIC VOID rfd_RegisterXcvRegCheckFunc(VOID)
{
    static BOOL registerIdleHook = FALSE;
    if (!registerIdleHook)
    {
        registerIdleHook = TRUE;
        extern bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void));
        sxs_RegisterDebugIdleHookFunc(&rfd_XcvRegCheck);
    }
}
#endif // SPI_REG_DEBUG

