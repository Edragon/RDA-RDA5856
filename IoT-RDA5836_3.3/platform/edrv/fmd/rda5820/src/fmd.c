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
/// @file fmd.c
/// That file implement the FM driver for RDA5820 chip.
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "fmd_config.h"
#include "fmd_m.h"
#include "tgt_fmd_cfg.h"
#include "fmdp_debug.h"

#include "pmd_m.h"

#include "hal_i2c.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"

#include "sxr_tim.h"

#include "sxr_tls.h"
//#include "cmn_defs.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#endif

// =============================================================================
//  MACROS
// =============================================================================

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
#define BASE_FREQ_EAST_EUROPE     65000

#define CHAN_SPACE          g_chanSpacing[g_fmdConfig->channelSpacing]

// MACROS for PRIVATE FUNCTION USE ONLY
#define FMD_I2C_BPS             g_fmdConfig->i2cBps

#define HAL_I2C_BUS_ID 			g_fmdConfig->i2cBusId


#define WORKMODE_RECEIVER           0
#define WORKMODE_TRANSMITTER        1

// =============================================================================
//  TYPES
// =============================================================================


//=============================================================================
// FMD_STATE_T
//-----------------------------------------------------------------------------
/// State of the tune or seek pooling process (using function timers)
//=============================================================================
typedef enum
{
    /// Initial state, no operation in process
    FMD_STATE_IDLE,
    /// Pooling states for tune and seek
    FMD_STATE_TUNE_POLL,
    FMD_STATE_SEEK_POLL,
    /// Once polling is done, we need to stop, is SPI is not avalaible, this is delayed to those states
    FMD_STATE_TUNE_STOP,
    FMD_STATE_SEEK_STOP,

    FMD_STATE_QTY
} FMD_STATE_T;

// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================

PRIVATE CONST TGT_FMD_CONFIG_T*  g_fmdConfig;

PRIVATE UINT32 g_baseFreq[FMD_BAND_QTY] = {BASE_FREQ_USEUROPE, BASE_FREQ_JAPAN, BASE_FREQ_WORLD, BASE_FREQ_EAST_EUROPE};
PRIVATE UINT32 g_chanSpacing[FMD_CHANNEL_SPACING_QTY] = {100,200,50};
PRIVATE FMD_BAND_T g_band = FMD_BAND_QTY; // using FMD_BAND_QTY as a closed state marker

PRIVATE FMD_CALLBACK_T* g_callback = NULL;
PRIVATE FMD_STATE_T g_state = FMD_STATE_IDLE;

PRIVATE UINT8 g_workMode = WORKMODE_RECEIVER;


uint16 g_fm_regs[] = {
	0x0002,
	0xC001,
	0xC001,
	0x0000,	
	0x0400,
	0x860F,
};



static uint16 RDA5820NS_RX_initialization_reg[][2] = {
    {0x02, 0xC001},
    {0x03, 0x0000},
    {0x04, 0x0400},
    {0x05, 0x88EF},
    {0x14, 0x2000},
    {0x15, 0x88FE},
    {0x16, 0x4C00},
    {0x1C, 0x221c},
    {0x25, 0x0E1C},
    {0x27, 0xBB6C},
    {0x5C, 0x175C},

};

static uint16 RDA5820NS_TX_initialization_reg[][2] = {
    {0x02, 0xC001},
    {0x03, 0x0000},  
    {0x04, 0x0c00},
    {0x05, 0x860F},
    {0x14, 0x5A00},
    {0x15, 0xBCFF},
    {0x16, 0x4C00},
    {0x18, 0x8013},
    {0x19, 0x00C0},
    {0x1A, 0x0400},
    {0x21, 0x0003},
    {0x27, 0xBB6C},
    {0x36, 0x0382},
    {0x5C, 0x175C},
    {0x5D, 0xFFED},
    {0x65, 0x003B},
    {0x67, 0x2E00},
    {0x68, 0x00BF},
    {0x6A, 0x2846},
    {0x40, 0x0001},
    {0x41, 0x41FF},
    {0x03, 0x0020},  
};

//#define HAL_I2C_BUS_ID HAL_I2C_BUS_ID_2
#define FMD_CHIP_ADRESS             0x11//0010001

#define I2C_MASTER_ACK              (1<<0)
#define I2C_MASTER_RD               (1<<4)
#define I2C_MASTER_STO              (1<<8)
#define I2C_MASTER_WR               (1<<12)
#define I2C_MASTER_STA              (1<<16)

void rdafm_iic_write_data(unsigned char regaddr, unsigned char *data, unsigned char datalen)
{
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 i=0;

	hal_I2cSendRawByte(HAL_I2C_BUS_ID,(FMD_CHIP_ADRESS<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,regaddr,I2C_MASTER_WR);

	for(i=0;i<datalen-1;i++,data++)
	{
		halErr=hal_I2cSendRawByte(HAL_I2C_BUS_ID,*data,I2C_MASTER_WR);
	}
	halErr=hal_I2cSendRawByte(HAL_I2C_BUS_ID,*data,I2C_MASTER_WR|I2C_MASTER_STO);
}

void rdafm_iic_read_data(unsigned char regaddr, unsigned char *data, unsigned char datalen)
{
	UINT8 i=0;
	
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,(FMD_CHIP_ADRESS<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,regaddr,I2C_MASTER_WR);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,((FMD_CHIP_ADRESS<<1)|1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(i=0;i<datalen-1;i++,data++)//data
	{	
		(*data)=hal_I2cReadRawByte(HAL_I2C_BUS_ID,I2C_MASTER_RD);
	}
	(*data)=hal_I2cReadRawByte(HAL_I2C_BUS_ID,I2C_MASTER_RD | I2C_MASTER_ACK | I2C_MASTER_STO);
}



PRIVATE FMD_ERR_T fmd_I2cOpen(HAL_I2C_BPS_T bps) 
{
    HAL_ERR_T error;
    
#ifdef I2C_BASED_ON_GPIO
    return gpio_i2c_open() ? FMD_ERR_NO : FMD_ERR_RESOURCE_BUSY;
#else
    error = hal_I2cOpen(HAL_I2C_BUS_ID);

    if(HAL_ERR_NO != error)
    {
        FMD_TRACE(EDRV_FMD_TRC,0,"fmd_I2cOpen FAIL=%d", error);
        return FMD_ERR_RESOURCE_BUSY;
    }

    return  FMD_ERR_NO;
#endif
}

VOID fmd_I2cClose(VOID)
{
#ifdef I2C_BASED_ON_GPIO
    gpio_i2c_close();
#else
    hal_I2cClose(HAL_I2C_BUS_ID); 
#endif
}


PRIVATE FMD_ERR_T fmd_Write(UINT32 addr, UINT8* data,UINT8 length)
{
    FMD_ERR_T ret;
    
    ret = fmd_I2cOpen(FMD_I2C_BPS);
    if(ret != FMD_ERR_NO)
        return ret;

    //FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Write addr=0x%x,", addr);

#ifdef I2C_BASED_ON_GPIO    	
    gpio_i2c_write_data(FMD_CHIP_ADRESS,(unsigned char * )&addr,1, data, length);
#else
//     hal_I2cSendData(HAL_I2C_BUS_ID,FMD_CHIP_ADRESS, addr,data,length);
    rdafm_iic_write_data(addr,data, length);
#endif
    fmd_I2cClose();

    return FMD_ERR_NO;
}

PRIVATE FMD_ERR_T fmd_Read(UINT32 addr, UINT8* data,UINT8 length)
{
    FMD_ERR_T ret;
    
    ret = fmd_I2cOpen(FMD_I2C_BPS);
    if(ret != FMD_ERR_NO)
        return ret;

    //FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Read addr=0x%x,", addr);

#ifdef I2C_BASED_ON_GPIO
    gpio_i2c_read_data(FMD_CHIP_ADRESS,(UINT8* )&addr,1, data, length);
 #else
    //hal_I2cGetData(HAL_I2C_BUS_ID,FMD_CHIP_ADRESS,addr,data,length);
	rdafm_iic_read_data(addr, data, length);
  #endif
    fmd_I2cClose();

    return FMD_ERR_NO;
}




PUBLIC BOOL fmd_RegWrite(UINT16 addr, UINT16 data)
{
    UINT8 writeReg[2];
    FMD_ERR_T ret;

    writeReg[0] = data >> 8;
    writeReg[1] = data & 0x00ff;

    ret = fmd_Write(addr, writeReg,2);
    if(ret != FMD_ERR_NO)
        return ret;

    return FMD_ERR_NO;

}

PUBLIC BOOL fmd_RegRead(UINT16 addr, UINT16* pData)
{
    UINT8 readReg[2];
    FMD_ERR_T ret;
    
    ret = fmd_Read(addr, readReg, 2);
    if(ret != FMD_ERR_NO)
        return ret;

    *pData = (readReg[0] << 8) | readReg[1];
    return FMD_ERR_NO;
}




//=============================================================================
// fmd_Scheduler
//-----------------------------------------------------------------------------
/// Process tune and seek operations
/// @param data used to send register read value to next time processing
/// in case SPI is not available.
//=============================================================================
VOID fmd_Scheduler(UINT32 data)
{
    FMD_ERR_T errStatus = FMD_ERR_NO;
    UINT16 readReg = 0;

    FMD_ASSERT(g_state < FMD_STATE_QTY, "FMD state unknown: %d", g_state);

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Scheduler: state=%d", g_state);

    // the POLLING step
    if ((g_state == FMD_STATE_TUNE_POLL) || (g_state == FMD_STATE_SEEK_POLL))
    {
        fmd_RegRead(0x0a,&readReg);
        FMD_TRACE(EDRV_FMD_TRC,0,"chan(fmd_Scheduler) = %x,reg1=%x",readReg[0],readReg[1]);    
        if (errStatus == FMD_ERR_NO)
        {
            // no error, check STC bit
            if (readReg & 0x4000)
            {
                // done: next step:
                FMD_TRACE(EDRV_FMD_TRC,0,"chan(readReg0)#########");  
                if (g_state == FMD_STATE_TUNE_POLL)
                {
                    g_state = FMD_STATE_TUNE_STOP;
                }
                else if (g_state == FMD_STATE_SEEK_POLL)
                {
                    g_state = FMD_STATE_SEEK_STOP;
                }
            }
        }
    }
    g_state = FMD_STATE_IDLE;
}

PUBLIC FMD_ERR_T fmd_Open(FMD_BAND_T band,
                          FMD_CALLBACK_T* callback,
                          FMD_INIT_T* initValues)
{
    CONST TGT_FMD_CONFIG_T* fmdTgtCfg = tgt_GetFmdConfig();
	UINT8 i = 0;
	UINT16 tChipId = 0;

    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_Open=%d",g_band);

    // Check if already opened
    // -----------------------
    if (g_band != FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED;
    }
    // Check the parameters and store them
    // -----------------------------------

    g_fm_regs[3] &= 0xfff3;
    switch (band)
    {
        case FMD_BAND_US_EUROPE:
            g_fm_regs[3] &= 0xfff3;
            g_band = band;
            break;

        case FMD_BAND_JAPAN:
            g_fm_regs[3] |= 1 << 2;
            g_band = band;
            break;


        case FMD_BAND_WORLD:
            g_fm_regs[3] |= 1 << 3;
            g_band = band;
            break;


        case FMD_BAND_EAST_EUROPE:
            g_fm_regs[3] |= (1 << 2) | (1 << 3);
            g_band = band;
            break;

        default:
            // this chip has limited band support...
            return FMD_ERR_BAD_PARAMETER;
    }

    if (fmdTgtCfg == NULL)
    {
        g_band = FMD_BAND_QTY; // close marker
        return FMD_ERR_BAD_PARAMETER;
    }
    g_fmdConfig = fmdTgtCfg;
    g_callback = callback;

    g_fm_regs[3] &= 0xfffc;
    switch (g_fmdConfig->channelSpacing)
    {
        case FMD_CHANNEL_SPACE_100:
            g_fm_regs[3] &= 0xfffc;
            break;

        case FMD_CHANNEL_SPACE_200:
            g_fm_regs[3] |= 0x01;
            break;


        case FMD_CHANNEL_SPACE_50:
            g_fm_regs[3] |= 1 << 1;
            break;

        default:
            // this chip has limited band support...
            return FMD_ERR_BAD_PARAMETER;
    }


    // default : mute , stereo, no bass boost
    // set the required initial state
    if (initValues != NULL)
    {
        // compute the mute bit
        if (initValues->volume == FMD_ANA_MUTE)
        {
            g_fm_regs[2] &= ~0x4000;
        }
        else
        {
            g_fm_regs[2] |= 0x4000;
        }
        if (initValues->bassBoost == TRUE)
        {
            g_fm_regs[2] |= 0x1000;
        }
        else
        {
            g_fm_regs[2] &= ~0x1000;
        }
        if (initValues->forceMono == TRUE)
        {
            g_fm_regs[2] |= 0x2000;
        }
        else
        {
            g_fm_regs[2] &= ~0x2000;
        }
    }

    // Wait for at least powerOnTime
    COS_SleepByTick(g_fmdConfig->powerOnTime);


    // Soft reset and enable
    fmd_RegWrite(0x02, g_fm_regs[0]);
    COS_Sleep(50 );

    fmd_RegRead(0x00, &tChipId);
    hal_HstSendEvent(1, 0x16121208);
    hal_HstSendEvent(1, tChipId);

    fmd_RegWrite(0x02, g_fm_regs[1]);
    // Wait about 5ms
    COS_Sleep(600);

    if (WORKMODE_RECEIVER == g_workMode)
    {
        for(i = 0;i< ((sizeof(RDA5820NS_RX_initialization_reg))/(sizeof(RDA5820NS_RX_initialization_reg[0])));i++)
            fmd_RegWrite(RDA5820NS_RX_initialization_reg[i][0],  RDA5820NS_RX_initialization_reg[i][1]);

        g_fm_regs[4] = RDA5820NS_RX_initialization_reg[2][1];
        g_fm_regs[5] = RDA5820NS_RX_initialization_reg[3][1];
    }
    else if (WORKMODE_TRANSMITTER== g_workMode)
    {
        for(i = 0;i< ((sizeof(RDA5820NS_TX_initialization_reg))/(sizeof(RDA5820NS_TX_initialization_reg[0])));i++)
            fmd_RegWrite(RDA5820NS_TX_initialization_reg[i][0],  RDA5820NS_TX_initialization_reg[i][1]);

        g_fm_regs[4] = RDA5820NS_TX_initialization_reg[2][1];
        g_fm_regs[5] = RDA5820NS_TX_initialization_reg[3][1];
    }

    //must delay for tune
    COS_Sleep(100 );

    return FMD_ERR_NO;
}

// =============================================================================
// fmd_Tune
// -----------------------------------------------------------------------------
/// This function tunes the FM channel to the desired frequency.
/// Once the tune operation has locked the channel, the callback registered 
/// at #fmd_Open will be called.
/// @param freqKHz  FM frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Tune(UINT32 freqKHz)
{
    FMD_ERR_T errStatus;
    UINT16 chan;

    if (WORKMODE_TRANSMITTER== g_workMode)
        fmd_RegWrite(0x09, 0x0831);


    chan = (freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
	
    g_fm_regs[3] = (chan << 6) | 0x10 | (g_fm_regs[3] & 0x000f);

    if (WORKMODE_TRANSMITTER== g_workMode)
        g_fm_regs[3] |= (1 << 5);


    errStatus = fmd_RegWrite(0x03, g_fm_regs[3]);

    if (WORKMODE_TRANSMITTER== g_workMode)
        fmd_RegWrite(0x09, 0x0031);

    COS_Sleep(50 );

    return errStatus;
}

// =============================================================================
// fmd_Close
// -----------------------------------------------------------------------------
/// This function closes the FM driver and desactivate the FM chip 
/// and stops any pending #fmd_Tune or #fmd_Seek operation.
/// @return             \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Close(VOID)
{
    FMD_ERR_T errStatus;
    UINT16 writeReg = 0x0000;

    // check oppened
    if (g_band == FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED; // ALREADY_CLOSED here !!
    }

    // cancel pending actions
    if (g_state != FMD_STATE_IDLE)
    {
        g_state = FMD_STATE_IDLE;
        //sxr_StopFunctionTimer((void (*)(void *))fmd_Scheduler);
    }
    // put radio in sleep mode
    errStatus = fmd_RegWrite(0x02, writeReg); // 0xc000
    if (errStatus != FMD_ERR_NO)
    {
        return errStatus;
    }

    g_band = FMD_BAND_QTY; // close marker

    return FMD_ERR_NO;
}

// =============================================================================
// fmd_Seek
// -----------------------------------------------------------------------------
/// This function seeks to the next FM channel.
/// Once the seek operation has found a channel, the callback registered 
/// at #fmd_Open will be called.
/// @param seekUp   \c TRUE : increasing frequency, \c FALSE: decreasing frequency
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Seek(BOOL seekUp)
{
    FMD_ERR_T errStatus;

    if (WORKMODE_TRANSMITTER== g_workMode)
        return FMD_ERR_NO;


    if (g_state != FMD_STATE_IDLE)
    {
        return FMD_ERR_RESOURCE_BUSY;
    }
    if (seekUp)
    {
        // Seek up
        g_fm_regs[2] |= (1 << 9);
    }
    else
    {
        // Seek down
        g_fm_regs[2] &= (~(1 << 9));
    }
    g_fm_regs[2] |= (1 << 8); 

    errStatus = fmd_RegWrite(0x02, g_fm_regs[2]); 
    if (errStatus != FMD_ERR_NO)
    {
        return errStatus;
    }
    // schedule read & callback
    g_state = FMD_STATE_TUNE_POLL;
    sxr_StartFunctionTimer(100 MILLI_SECONDS, fmd_Scheduler, 0, 0);
    return FMD_ERR_NO;
}


// =============================================================================
// fmd_SetVolume
// -----------------------------------------------------------------------------
/// This function sets the volume and other audio related paramters.
/// @param volume       analog volume
/// @param bassBoost    enable/disable bass boost
/// @param forceMono    enable/disable mono output of stereo radios
/// @return             \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_SetVolume(FMD_ANA_LEVEL_T volume,
                               BOOL bassBoost,
                               BOOL forceMono)
{
    FMD_ERR_T errStatus=0;

	if(volume >= FMD_ANA_VOL_QTY)
		volume=FMD_ANA_VOL_7;
    FMD_ASSERT(volume < FMD_ANA_VOL_QTY, "FMD volume out of range (%d)", volume);
    // compute the mute bit
    if (volume == FMD_ANA_MUTE)
    {
        g_fm_regs[2] &= ~0x4000;
    }
    else
    {
        g_fm_regs[2]  |= 0x4000;
    }
    if (bassBoost == TRUE)
    {
        g_fm_regs[2]  |= 0x1000;
    }
    else
    {
        g_fm_regs[2]  &= ~0x1000;
    }
    if (forceMono == TRUE)
    {
        g_fm_regs[2]  |= 0x2000;
    }
    else
    {
        g_fm_regs[2]  &= ~0x2000;
    }

    errStatus = fmd_RegWrite(0x02, g_fm_regs[2]);

    g_fm_regs[5] &= 0xFFF0;
    g_fm_regs[5] |= (volume*2 & 0x0f);
    errStatus = fmd_RegWrite(0x05, g_fm_regs[5]);
//    sxr_Sleep(50 MILLI_SECONDS);
    return errStatus;
}

PUBLIC FMD_ERR_T fmd_TxPaGain(uint8 pagain) 
{
    FMD_ERR_T errStatus=0;
    UINT16 writeReg = 0;

	errStatus = fmd_RegRead(0x41, &writeReg);
	writeReg &= (~0x3f);
	writeReg |= pagain;
	errStatus = fmd_RegWrite(0x41, writeReg);
	
    return errStatus;
}

PUBLIC FMD_ERR_T fmd_TxSigGain(uint8 siggain) 
{
    FMD_ERR_T errStatus=0;
    UINT16 writeReg = 0;

	fmd_RegRead(0x68,&writeReg);
	writeReg &= 0xe3ff;
	writeReg |= ((siggain&0x07) << 10);
	fmd_RegWrite(0x68, writeReg);
	
    return errStatus;
}


// =============================================================================
// fmd_GetFreq
// -----------------------------------------------------------------------------
/// @param pFreqKHz     pointer to a UINT32 receiving the channel frequency
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pFreqKHz is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetFreq(UINT32 *pFreqKHz)
{
    UINT16 readReg = 0;
    FMD_ERR_T errStatus;

    if (pFreqKHz == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    errStatus = fmd_RegRead(0x0a, &readReg);
    FMD_TRACE(EDRV_FMD_TRC,0,"readReg=%x",readReg);   

    if (errStatus == FMD_ERR_NO)
    {
        *pFreqKHz = (readReg & 0x03ff) * CHAN_SPACE + g_baseFreq[g_band];
    }

    return errStatus;
	
}


// =============================================================================
// fmd_GetStereoStatus
// -----------------------------------------------------------------------------
/// @param pStereoStatus    pointer to a BOOL receiving the stereo status:
///                         \c TRUE the channel is received in stereo,
///                         \c FALSE the channel is recieved in mono.
/// @return                 \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                         \c FMD_ERR_BAD_PARAMETER if \p pStereoStatus is \c NULL
// =============================================================================
PUBLIC FMD_ERR_T fmd_GetStereoStatus(BOOL *pStereoStatus)
{
    UINT16 readReg = 0;
    FMD_ERR_T errStatus;
    if (pStereoStatus == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    errStatus = fmd_RegRead(0x0a, &readReg);
    FMD_TRACE(EDRV_FMD_TRC,0,"readReg=%x",readReg);   
    if (errStatus == FMD_ERR_NO)
    {
        if ((readReg & 0x0400) == 0)
        {
            *pStereoStatus = FALSE;
        }
        else
        {
            *pStereoStatus = TRUE;
        }
    }

    return errStatus;
}


// =============================================================================
// fmd_GetRssi
// -----------------------------------------------------------------------------
/// @param pRSSI        pointer to a UINT32 receiving a measure of the channel quality, should not be used as the meaning of this is dependant on the actual FM chip.
/// @return             \c FMD_ERR_NO, \c FMD_ERR_RESOURCE_BUSY or
///                     \c FMD_ERR_BAD_PARAMETER if \p pRSSI is \c NULL
// =============================================================================
DEPRECATED PUBLIC FMD_ERR_T fmd_GetRssi(BYTE* pRSSI)
{
    UINT16 readReg = 0;
    FMD_ERR_T errStatus;
    if (pRSSI == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }
    *pRSSI = 0;
	
    errStatus = fmd_RegRead(0x0b, &readReg);
    FMD_TRACE(EDRV_FMD_TRC,0,"readReg=%x",readReg);   

    *pRSSI = readReg >> 9;

    return errStatus;

}

PUBLIC BOOL fmd_ValidStop(UINT32 freqKHz)
{
    FMD_ERR_T errStatus;
    UINT16 readReg = 0;
    UINT16 chan;

    if (WORKMODE_TRANSMITTER== g_workMode)
        return FMD_ERR_NO;


    FMD_TRACE(EDRV_FMD_TRC,0,"fmd_ValidStop freqKHz= %d",freqKHz);   	

	chan = (freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
    g_fm_regs[3] = (chan << 6) | 0x10 | (g_fm_regs[3] & 0x000f);

    errStatus = fmd_RegWrite(0x03, g_fm_regs[3]);


    COS_Sleep(50 );
	
	errStatus = fmd_RegRead(0x0b, &readReg);	

	if ((readReg &0x0100)==0) 
        return 0;
		
    return 1;
}
// =============================================================================
// fmd_GetRssiThreshold
// -----------------------------------------------------------------------------
/// @return             a Threshold used to compare with RSSI measure of the channel quality, only useful with fmd_GetRssi.
// =============================================================================
DEPRECATED PUBLIC UINT32 fmd_GetRssiThreshold(VOID)
{
    return g_fmdConfig->seekRSSIThreshold;
}

PUBLIC VOID fmd_SetWorkMode(uint8 mode) 
{
    g_workMode = mode;
}




PUBLIC FMD_ERR_T fmd_I2sOpen(FMD_I2S_CFG_T fmdI2sCfg)
{
    return FMD_ERR_NO;  
}

// =============================================================================
// fmd_I2sClose
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sClose()
{
    return FMD_ERR_NO;  
}


