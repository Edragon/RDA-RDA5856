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
/// That file implement the FM driver for RDA5870 chip.
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
#include "cmn_defs.h"

#ifdef I2C_BASED_ON_GPIO
#include "gpio_i2c.h"
#endif


#ifdef FM_LDO_WORKAROUND
extern VOID pmd_EnableDcdcPower(BOOL on);
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

// =============================================================================
//  TYPES
// =============================================================================
typedef enum{
	RDA_NONE_CHIP_ENUM = 0,
	RDA_BT_R5_5868_ENUM,
	RDA_BT_R7_5868PLUS_ENUM,
	RDA_BT_R8_5870_ENUM,
	RDA_BT_R10_5868E_ENUM,
	RDA_BT_R10_5872_ENUM,
	RDA_BT_R10_AL_5868H_ENUM,
	RDA_BT_R10_AL_5872H_ENUM,
	RDA_BT_R10_5870E_ENUM,
	RDA_BT_R11_5872P_ENUM,
	RDA_BT_R11_5875_ENUM,
	RDA_BT_R12_5876_ENUM,
	RDA_BT_R12_5870P_ENUM,
 	RDA_BT_R12_5876M_ENUM,
 	RDA_BT_R16_5876P_ENUM,	
	RDA_BT_R12_5990_ENUM,
	RDA_BT_R17_8809_ENUM,
}RDABT_CHIP_ENUM;

extern unsigned char rdabt_chip_sel;
extern PUBLIC BOOL g_BTOpen_Flag;
extern PUBLIC BOOL g_FMOpen_Flag;
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
PRIVATE UINT16 g_fmRegSysCtrl; // BassBoost ForceMono and Mute state
PRIVATE FMD_CALLBACK_T* g_callback = NULL;
PRIVATE FMD_STATE_T g_state = FMD_STATE_IDLE;
PRIVATE HAL_SYS_CLOCK_OUT_ID_T g_fmdClockOutId;

/// when using SPI to avoid conflicts
//PRIVATE BOOL g_spiInUse = FALSE;

//extern void rdafm_iic_write_data(unsigned char regaddr, const unsigned short *data, unsigned char datalen);
//extern void rdafm_iic_read_data(unsigned char regaddr, unsigned short *data, unsigned char datalen);


#define FM_REG_VCO_BAND_SETTING 0x15

#define FM_LOW_FREQ_RATE 95000

#define HIGH_FREQ_VCO_BAND_SETTING 0xf831
#define LOW_FREQ_VCO_BAND_SETTING 0x8831

uint16 current_FREQ_VCO_SETTING[2]={FM_REG_VCO_BAND_SETTING,LOW_FREQ_VCO_BAND_SETTING};

uint8 RDA5876_initialization_reg[]={
0xc0,0x05,//02h
0x00,0x00,
0x04,0x00,
0xc8,0xcf, // determined by hardware N:0x884f P:0x888f
0x60,0x00,
0x7e,0x22, //[7:2]:5802e seek th
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0x00,0x00,
0xf0,0x17,//10h
0x01,0x39,
0x14,0x01,
0xb0,0x42,
0x20,0x01,
0xf8,0x31,//15h
0xc3,0xe4,
0x21,0xd1,
0x94,0x12,//18h
0x00,0xa8,
0xc4,0x00,
0xe7,0xdf,
0x35,0x9c,
0x81,0x6a,
0x46,0x08,
0x00,0x86,
0x06,0x61,//20h
0x00,0x00,//21h
0x10,0x9e,
0x23,0xc8,
0x04,0x06,
0x06,0x0b,//25h
0xe1,0x05,
0xbb,0x6c,
0x2b,0xec,
0x09,0x0f,
0x34,0x14,
0x14,0x50,
0x09,0x6d,
0x2d,0x96,
0x01,0xda,
0x2a,0x7b,
0x08,0x21,//30h
0x13,0xd5,
0x48,0xf1,
0x00,0xbc,
0x08,0x20,
0x0a,0x24,//35h
0x0b,0x82,
0x25,0xc7,
0x2b,0xe0,
0x3e,0xdd,
0x2c,0xb9,
0x0c,0x95,
0x08,0x8d,
0x04,0x85,
0x00,0x80,
0x00,0x00,
0x58,0x04,//40h
0x58,0x04,
0x38,0x35,
0x2f,0x2b,
0x27,0x22,
0x1b,0x15,
0x0f,0x08,
0x03,0xfd,
0xfd,0xfd,
0x17,0x8a,//49h
0x00,0xff,
0x00,0x00,
0x31,0x40,
0xc8,0x06,
0x12,0xfe,
0xd4,0x98
};

uint8 RDA5876p_initialization_reg[]={
0xc0,0x01,//02h
0x1B,0x80,
0x04,0x00,
0xc8,0xcf, // determined by hardware N:0x884f P:0x888f
0x60,0x00,
0x42,0x22, //[7:2]:5802e seek th
0x00,0x00,
0x00,0x00,
0x40,0x6e,//0ah
0x0e,0x80,
0x58,0x03,
0x58,0x04,
0x58,0x01,
0x58,0x04,
0x00,0x00,//10h
0x00,0x19,
0x1a,0x0f,
0xb0,0x00,
0x2a,0x11,
0xf8,0x31,//15h
0xc3,0xd4,
0x23,0x66,
0x94,0x12,//18h
0x00,0xa8,
0xc4,0x00,
0xd7,0xdf,
0x22,0xdc,
0x81,0x6a,
0x46,0x08,
0x00,0x86,
0x06,0x61,//20h
0x00,0x00,//21h
0x10,0x9e,
0x23,0xc8,
0x04,0x06,
0x06,0x0b,//25h
0xe1,0x05,
0xbb,0x6c,
0x2b,0xec,
0x09,0x0f,
0x34,0x14,
0x14,0x50,
0x09,0x6d,
0x2d,0x96,
0x01,0xda,
0x2a,0x7b,
0x08,0x21,//30h
0x13,0xd5,
0x48,0xb1,
0x00,0x7d,
0x08,0xa0,
0x0a,0x24,//35h
0x0b,0x82,
0x25,0xc7,
0x2b,0xe0,
0x3e,0xdd,
0x2c,0xb9,
0x0c,0x95,
0x08,0x8d,
0x04,0x85,
0x00,0x80,
0x00,0x00,
0x58,0x04,//40h
0x58,0x04,
0x38,0x35,
0x2f,0x2b,
0x27,0x22,
0x1b,0x15,
0x0f,0x08,
0x03,0xfd,
0xfd,0xfd,
0x17,0x8a,//49h
0x00,0x00,
0x00,0xff,
0x32,0x40,
0xc8,0x02,
0x13,0xf4,
0xd4,0x98,
0x00,0x00,//50h
0x1f,0x20,
0x00,0x00,
0x4a,0x80,
0x4e,0x00,
0x58,0x04,
0xae,0xc0,//56h
0x58,0x04,
0x58,0x04,
0x58,0x04,
0x58,0x04,
0x68,0x00,//5bh
0x09,0xa3,
0x00,0x2d,
0x58,0x04,
0x58,0x04,
0x80,0x00,//60h
0x02,0x00,
0x00,0x00
};

//#define HAL_I2C_BUS_ID HAL_I2C_BUS_ID_2
#define FMD_CHIP_ADRESS 0x11//0010001
#define FMD_CHIP_ADRESS_T 0x11

#define I2C_MASTER_ACK              (1<<0)
#define I2C_MASTER_RD               (1<<4)
#define I2C_MASTER_STO              (1<<8)
#define I2C_MASTER_WR               (1<<12)
#define I2C_MASTER_STA              (1<<16)

void rdafm_iic_write_data(unsigned char regaddr, unsigned char *data, unsigned char datalen)
{
	HAL_ERR_T halErr = HAL_ERR_RESOURCE_BUSY;
	UINT8 Data_half[2];
	UINT8 i=0;
	
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,(FMD_CHIP_ADRESS_T<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,regaddr,I2C_MASTER_WR);

	for(i=0;i<datalen-1;i++,data++)//data
	{
		halErr=hal_I2cSendRawByte(HAL_I2C_BUS_ID,*data,I2C_MASTER_WR);
	}
	halErr=hal_I2cSendRawByte(HAL_I2C_BUS_ID,*data,I2C_MASTER_WR|I2C_MASTER_STO);

//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_write_data: %d\n.",halErr);
}

void rdafm_iic_read_data(unsigned char regaddr, unsigned char *data, unsigned char datalen)
{
	UINT8 Data_half[2];
	UINT8 i=0;
	
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,(FMD_CHIP_ADRESS_T<<1),I2C_MASTER_WR | I2C_MASTER_STA);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,regaddr,I2C_MASTER_WR);
	hal_I2cSendRawByte(HAL_I2C_BUS_ID,((FMD_CHIP_ADRESS_T<<1)|1),I2C_MASTER_WR | I2C_MASTER_STA);

	for(i=0;i<datalen-1;i++,data++)//data
	{	
		(*data)=hal_I2cReadRawByte(HAL_I2C_BUS_ID,I2C_MASTER_RD);
	}
	(*data)=hal_I2cReadRawByte(HAL_I2C_BUS_ID,I2C_MASTER_RD | I2C_MASTER_ACK | I2C_MASTER_STO);

//	SXS_TRACE(TSTDOUT, "rdabt_iic_rf_read_data: %d\n.");
}



PRIVATE FMD_ERR_T fmd_I2cOpen(HAL_I2C_BPS_T bps) 
{
#ifdef I2C_BASED_ON_GPIO
        gpio_i2c_open();
#else
	  hal_I2cOpen(HAL_I2C_BUS_ID);
#endif
    return FMD_ERR_NO;
}

 VOID fmd_I2cClose(VOID)
{
#ifdef I2C_BASED_ON_GPIO
       gpio_i2c_close();
#else
	 hal_I2cClose(HAL_I2C_BUS_ID); 
#endif
}

 VOID fmd_WriteSingle(UINT32 addr, UINT16 data)
{
#ifdef I2C_BASED_ON_GPIO
     gpio_i2c_write_data(FMD_CHIP_ADRESS,(unsigned char * )&addr,1, (unsigned char * )&data, 1);   
#else	
     hal_I2cSendByte(HAL_I2C_BUS_ID,FMD_CHIP_ADRESS,addr,data);
#endif
        sxs_fprintf(TSTDOUT,"fmd_WriteSingle  addr=%x,data=%x",addr,data);
		
}

PRIVATE FMD_ERR_T fmd_Write(UINT32 addr, UINT8* data,UINT8 length)
{

    fmd_I2cOpen(FMD_I2C_BPS);

#ifdef I2C_BASED_ON_GPIO    	
    gpio_i2c_write_data(FMD_CHIP_ADRESS,(unsigned char * )&addr,1, data, length);
#else
//     hal_I2cSendData(HAL_I2C_BUS_ID,FMD_CHIP_ADRESS, addr,data,length);
         rdafm_iic_write_data(addr,data, length);

#endif
       fmd_I2cClose();

    sxs_fprintf(TSTDOUT,"fmd_Write  addr=%x,data[0]=%x,data[1]=%x",addr,data[0],data[1]);
    return FMD_ERR_NO;
}

PRIVATE FMD_ERR_T fmd_Read(UINT32 addr, UINT8* data,UINT8 length)
{
    fmd_I2cOpen(FMD_I2C_BPS);
#ifdef I2C_BASED_ON_GPIO
    gpio_i2c_read_data(FMD_CHIP_ADRESS,(UINT8* )&addr,1, data, length);
 #else
 //     hal_I2cGetData(HAL_I2C_BUS_ID,FMD_CHIP_ADRESS,addr,data,length);
	rdafm_iic_read_data(addr, data, length);
  #endif
     fmd_I2cClose();
    sxs_fprintf(TSTDOUT,"fmd_Read  addr=%x,data[0]=%x,data[1]=%x",addr,data[0],data[1]);
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
    UINT8 readReg[2] = {0};

    FMD_ASSERT(g_state < FMD_STATE_QTY, "FMD state unknown: %d", g_state);
    // the POLLING step
    if ((g_state == FMD_STATE_TUNE_POLL) || (g_state == FMD_STATE_SEEK_POLL))
    {
        //errStatus = fmd_Read(0x0a, &rdReg);
        fmd_Read(0x0a,readReg,2);
        SXS_TRACE(TSTDOUT,"chan(fmd_Scheduler) = %x,reg1=%x",readReg[0],readReg[1]);    
        if (errStatus == FMD_ERR_NO)
        {
            // no error, check STC bit
            if (readReg[0] & 0x40)
            {
                // done: next step:
                SXS_TRACE(TSTDOUT,"chan(readReg0)#########");  
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
    FMD_ERR_T errStatus;
    UINT8 dacVol;
    UINT16 chan;
    UINT8 readReg[8] = {0};
    UINT8 writeReg[8] = {0};
	//UINT8 TestReg[2] = {0};
    
    // Check if already opened
    // -----------------------
    if (g_band != FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED;
    }
    // Check the parameters and store them
    // -----------------------------------
    switch (band)
    {
        case FMD_BAND_US_EUROPE:
        case FMD_BAND_JAPAN:
        case FMD_BAND_WORLD:
        case FMD_BAND_EAST_EUROPE:
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
    // default : mute , stereo, no bass boost
    g_fmRegSysCtrl = 0;
    dacVol = 0;
    chan = 0;
    // set the required initial state
    // ------------------------------
    if (initValues != NULL)
    {
        // set given parameters 
        dacVol = g_fmdConfig->volumeVal[initValues->volume];
        // compute the mute bit
        if (initValues->volume == FMD_ANA_MUTE)
        {
            g_fmRegSysCtrl &= ~0x4000;
        }
        else
        {
            g_fmRegSysCtrl |= 0x4000;
        }
        if (initValues->bassBoost == TRUE)
        {
            g_fmRegSysCtrl |= 0x1000;
        }
        else
        {
            g_fmRegSysCtrl &= ~0x1000;
        }
        if (initValues->forceMono == TRUE)
        {
            g_fmRegSysCtrl |= 0x2000;
        }
        else
        {
            g_fmRegSysCtrl &= ~0x2000;
        }
        chan = (initValues->freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
    }

    // enable the clock
    g_fmdClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_32K);
    if (HAL_SYS_CLOCK_OUT_RESOURCE_UNAVAILABLE == g_fmdClockOutId)
    {
        return FMD_ERR_RESOURCE_BUSY;
    }

    // enable the POWER
    pmd_EnablePower(PMD_POWER_FM, TRUE);
#ifdef FM_LDO_WORKAROUND
	pmd_EnableDcdcPower(FALSE); 
#endif

    COS_Sleep(25 );
    if(rdabt_chip_sel == RDA_BT_R16_5876P_ENUM)
    {
        if(g_BTOpen_Flag == FALSE)
        {   //BT off, need LDO on first.
            pmd_EnablePower(PMD_POWER_BT, TRUE);
            RDABT_5876p_btoff_fmon_patch();
        }
        else
        {   //BT on, not need LDO on again            
            RDABT_5876p_bton_fmon_patch();
        }
        g_FMOpen_Flag = TRUE;
    }
    else
    {
	    RDABT_5870e_5876_fm_patch_on(); 
        EDRV_TRACE(EDRV_FMD_TRC,0,"RDABT_5870e_5876_fm_patch_on");   

    }
    COS_Sleep(25);

    // Wait for at least powerOnTime
    COS_SleepByTick(g_fmdConfig->powerOnTime);
    // Soft Reset
    writeReg[0] = 0;
    writeReg[1] = 0x02;
    errStatus = fmd_Write(0x02, writeReg,2);

    // Wait about 5ms
    COS_Sleep(5);

    fmd_Read(0x0c,(UINT8*)readReg,4);
    //readReg = ((readReg & 0x3ff) * CHAN_SPACE) + g_baseFreq[g_band];
    EDRV_TRACE(EDRV_FMD_TRC,0,"read the register is 0x0A=%x,0x0B=%x,0x0C=%x,0x0D=%x",readReg[0],readReg[1],readReg[2],readReg[3]);
#if 0
	hal_HstSendEvent(SYS_EVENT,HAL_I2C_BUS_ID);
		
	hal_HstSendEvent(SYS_EVENT,0xFF00001F);
	hal_HstSendEvent(SYS_EVENT,readReg[0]);
	hal_HstSendEvent(SYS_EVENT,readReg[1]);
	hal_HstSendEvent(SYS_EVENT,readReg[2]);
	hal_HstSendEvent(SYS_EVENT,readReg[3]);
	hal_HstSendEvent(SYS_EVENT,0xF100002F);
	
	fmd_Read(0x0e,(UINT8*)TestReg,2);
	hal_HstSendEvent(SYS_EVENT,0xFF00001F);
	hal_HstSendEvent(SYS_EVENT,TestReg[0]);
	hal_HstSendEvent(SYS_EVENT,TestReg[1]);
	hal_HstSendEvent(SYS_EVENT,0xF100002F);
#endif

 //   g_fmRegSysCtrl |= 0x2000;
 //   g_fmRegSysCtrl |= 0x4000;
//    g_fmRegSysCtrl |= 0x8000;

    // Register configuration
    // System control    
    if(rdabt_chip_sel == RDA_BT_R16_5876P_ENUM)
    {
        errStatus = fmd_Write(0x02, RDA5876p_initialization_reg,sizeof(RDA5876p_initialization_reg)/sizeof(RDA5876p_initialization_reg[0]));
    }
    else
    {
        RDA5876_initialization_reg[0] = (UINT8)(0x80 | ((g_fmRegSysCtrl&0xFF00)>>8));//0x02
        RDA5876_initialization_reg[1] = (UINT8)(0x01 | (g_fmRegSysCtrl&0x00FF));
        RDA5876_initialization_reg[2] = (UINT8)(((chan & 0x03ff)>>2) & 0xFF);//0x03
        RDA5876_initialization_reg[3] = (UINT8)((((chan & 0x03ff) << 6)&0x00C0) | 0x10 | ((g_band & 3) << 2) | (g_fmdConfig->channelSpacing & 3));
        RDA5876_initialization_reg[6] = (UINT8)((RDA5876_initialization_reg[6] & 0xf0) | (g_fmdConfig->seekRSSIThreshold & 0x0f));//0x05
        RDA5876_initialization_reg[7] = (RDA5876_initialization_reg[7] & 0xf0) | (dacVol & 0x0f);

        EDRV_TRACE(EDRV_FMD_TRC,0,"NEW reg0=%x,reg1=%x,reg2=%x,reg3=%x,reg6=%x,reg7=%x,",RDA5876_initialization_reg[0],RDA5876_initialization_reg[1],RDA5876_initialization_reg[2],RDA5876_initialization_reg[3],RDA5876_initialization_reg[6],RDA5876_initialization_reg[7]);   
        errStatus = fmd_Write(0x02, RDA5876_initialization_reg,sizeof(RDA5876_initialization_reg)/sizeof(RDA5876_initialization_reg[0]));
    }
    
    //must delay for tune
    COS_Sleep(100);

    // Wait about 5ms

    
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

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
PUBLIC FMD_ERR_T fmd_Tune(UINT32 freqKHz)
{
    FMD_ERR_T errStatus;
    UINT8 readReg[2] = {0};
    UINT8 writeReg[2] = {0};
    UINT16 chan;
    UINT16 g_FREQ_VCO_SETTING=0;

    if ( freqKHz < FM_LOW_FREQ_RATE )
    {
    	g_FREQ_VCO_SETTING=LOW_FREQ_VCO_BAND_SETTING;
    }
    else
    {
    	g_FREQ_VCO_SETTING=HIGH_FREQ_VCO_BAND_SETTING;
    }

    if (current_FREQ_VCO_SETTING[1]!=g_FREQ_VCO_SETTING)
    {
    	current_FREQ_VCO_SETTING[1]=g_FREQ_VCO_SETTING;
    	writeReg[0]=( current_FREQ_VCO_SETTING[1]&0xff00 )>>8;
    	writeReg[1]=current_FREQ_VCO_SETTING[1] & 0x00ff;
    	fmd_Write(current_FREQ_VCO_SETTING[0], writeReg,2);
    }

    COS_Sleep(10 );
    errStatus = fmd_Read(0x03, readReg,2);

    chan = (freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
    writeReg[0] = (UINT8)(((chan & 0x03ff)>>2) & 0xFF);//0x03
    //open I2S ,must use |0x30 ;if colse I2s ,use |0x10
    writeReg[1] = (UINT8)((((chan & 0x03ff) << 6)&0x00C0) | (readReg[1]&0x20)| 0x10 | ((g_band & 3) << 2) | (g_fmdConfig->channelSpacing & 3));
    errStatus = fmd_Write(0x03, writeReg,2);
    
    EDRV_TRACE(EDRV_FMD_TRC,0,"fmd_Tune write register is writeReg[0]=%x,writeReg[1]=%x",writeReg[0],writeReg[1]);

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
    UINT8 writeReg[2] = {0};

    // check oppened
    if (g_band == FMD_BAND_QTY)
    {
        //FMD_ASSERT(g_spiInUse, "closing FMD while not open");
        return FMD_ERR_ALREADY_OPENED; // ALREADY_CLOSED here !!
    }

    // cancel pending actions
    if (g_state != FMD_STATE_IDLE)
    {
        g_state = FMD_STATE_IDLE;
        sxr_StopFunctionTimer((void (*)(void *))fmd_Scheduler);
    }
    // put radio in sleep mode (+mute and high Z ?)
    errStatus = fmd_Write(0x02, writeReg,2); // 0xc000
    if (errStatus != FMD_ERR_NO)
    {
        return errStatus;
    }
    
    if(rdabt_chip_sel == RDA_BT_R16_5876P_ENUM)
    {
        if(g_BTOpen_Flag == FALSE)
        {   //BT off, need LDO off
            pmd_EnablePower(PMD_POWER_BT, FALSE);
        }
        else
        {   //BT on, not need LDO off 
            RDABT_5876p_bton_fmoff_patch();
        }
        g_FMOpen_Flag = FALSE;
    }
    else
    {
	    RDABT_5870e_5876_fm_patch_off();
    }
    // close
    // cut the power, cut the clock
#ifdef FM_LDO_WORKAROUND
	pmd_EnableDcdcPower(TRUE);
#endif
    pmd_EnablePower(PMD_POWER_FM, FALSE);
    hal_SysClkOutClose(g_fmdClockOutId);

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
    UINT8 writeReg[2] = {0};
    if (g_state != FMD_STATE_IDLE)
    {
        return FMD_ERR_RESOURCE_BUSY;
    }
    if (seekUp)
    {
        // Seek up
	 writeReg[0] = (UINT8)(0x83 | ((g_fmRegSysCtrl&0xFF00)>>8));//0x02
        writeReg[1] = (UINT8)(0x01 | (g_fmRegSysCtrl&0x00FF));
    }
    else
    {
        // Seek down
	 writeReg[0] = (UINT8)(0x81 | ((g_fmRegSysCtrl&0xFF00)>>8));//0x02
        writeReg[1] = (UINT8)(0x01 | (g_fmRegSysCtrl&0x00FF));
    }
    
    errStatus = fmd_Write(0x02, writeReg,2); 
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
    UINT8 writeReg[2] = {0};
    UINT8 dacVol;
    FMD_ASSERT(volume < FMD_ANA_VOL_QTY, "FMD volume out of range (%d)", volume);

    // compute the mute bit
    if (volume == FMD_ANA_MUTE)
    {
        g_fmRegSysCtrl &= ~0x4000;
    }
    else
    {
        g_fmRegSysCtrl |= 0x4000;
    }
    if (bassBoost == TRUE)
    {
        g_fmRegSysCtrl |= 0x1000;
    }
    else
    {
        g_fmRegSysCtrl &= ~0x1000;
    }
    if (forceMono == TRUE)
    {
        g_fmRegSysCtrl |= 0x2000;
    }
    else
    {
        g_fmRegSysCtrl &= ~0x2000;
    }

    writeReg[0] = (UINT8)(0x80 | ((g_fmRegSysCtrl&0xFF00)>>8));//0x02
    writeReg[1] = (UINT8)(0x01 | (g_fmRegSysCtrl&0x00FF));
    errStatus = fmd_Write(0x02, writeReg, 2);
    COS_Sleep(50 );

    // get volume value from config
    dacVol = g_fmdConfig->volumeVal[volume];
    if(rdabt_chip_sel == RDA_BT_R16_5876P_ENUM)
    {
        writeReg[0] = (RDA5876p_initialization_reg[6] & 0xf0) | (g_fmdConfig->seekRSSIThreshold & 0x0f);//0x05
        writeReg[1] = (RDA5876p_initialization_reg[7] & 0xf0) | (dacVol & 0x0f);
    }
    else
    {
        writeReg[0] = (RDA5876_initialization_reg[6] & 0xf0) | (g_fmdConfig->seekRSSIThreshold & 0x0f);//0x05
        writeReg[1] = (RDA5876_initialization_reg[7] & 0xf0) | (dacVol & 0x0f);
    }
    
    //writeReg[0] = (RDA5876_initialization_reg[6] & 0xf0) | (g_fmdConfig->seekRSSIThreshold & 0x0f);//0x05
    //writeReg[1] = (RDA5876_initialization_reg[7] & 0xf0) | (0x0f);

    errStatus = fmd_Write(0x05, writeReg, 2);
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
    UINT8 readReg[2] = {0};
    FMD_ERR_T errStatus;
    if (pFreqKHz == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }
    errStatus = fmd_Read(0x0a, readReg,2);
    SXS_TRACE(TSTDOUT,"reg0=%x,reg1=%x",readReg[0],readReg[1]);   
    if (errStatus == FMD_ERR_NO)
    {
        *pFreqKHz = (((readReg[0]<<8) |readReg[1]) & 0x3ff) * CHAN_SPACE + g_baseFreq[g_band];
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
    UINT8 readReg[2] = {0};
    FMD_ERR_T errStatus;
    if (pStereoStatus == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    errStatus = fmd_Read(0x0a, readReg,2);
    SXS_TRACE(TSTDOUT,"reg0=%x,reg1=%x",readReg[0],readReg[1]);   
    if (errStatus == FMD_ERR_NO)
    {
        if ((readReg[0] & 0x04) == 0)
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
DEPRECATED PUBLIC FMD_ERR_T fmd_GetRssi(UINT8* pRSSI)
{
    UINT8 readReg[2] = {0};
    FMD_ERR_T errStatus;

    if (pRSSI == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    } 

    errStatus = fmd_Read(0x0b, readReg, 2);
    SXS_TRACE(TSTDOUT,"reg0=%x,reg1=%x",readReg[0],readReg[1]);

    *pRSSI = ((readReg[0] & 0xfe) >> 1); 

    return errStatus;
}

PUBLIC BOOL fmd_ValidStop(UINT32 freqKHz)
{
    FMD_ERR_T errStatus;
    UINT8 readReg[4] = {0};
    UINT8 writeReg[2] = {0};
    UINT16 chan;
	UINT8 i = 0;
	
	UINT16 g_FREQ_VCO_SETTING=0;
	
	if ( freqKHz < FM_LOW_FREQ_RATE )
	{
		g_FREQ_VCO_SETTING=LOW_FREQ_VCO_BAND_SETTING;
	}
	else
	{
		g_FREQ_VCO_SETTING=HIGH_FREQ_VCO_BAND_SETTING;
	}

	
	if (current_FREQ_VCO_SETTING[1]!=g_FREQ_VCO_SETTING)
	{
		current_FREQ_VCO_SETTING[1]=g_FREQ_VCO_SETTING;
		writeReg[0]=( current_FREQ_VCO_SETTING[1] & 0xff00 )>>8;
		writeReg[1]=current_FREQ_VCO_SETTING[1] & 0x00ff;
		fmd_Write(current_FREQ_VCO_SETTING[0], writeReg,2);
	}

    SXS_TRACE(TSTDOUT,"fmd_ValidStop freqKHz= %d",freqKHz);

    errStatus = fmd_Read(0x03, readReg,2);

    chan = (freqKHz - g_baseFreq[g_band])/CHAN_SPACE;
    writeReg[0] = (UINT8)(((chan & 0x03ff)>>2) & 0xFF);//0x03
    //open I2S ,must use |0x30 ;if colse I2s ,use |0x10
    writeReg[1] = (UINT8)((((chan & 0x03ff) << 6)&0x00C0) | (readReg[1]&0x20)| 0x10 | ((g_band & 3) << 2) | (g_fmdConfig->channelSpacing & 3));
    errStatus = fmd_Write(0x03, writeReg,2);



    COS_Sleep(35 );
	//waiting for FmReady
	do
	{
		i++;
		if(i>10) return 0; 
		COS_Sleep(10 );
		//read REG0A&0B	
		errStatus = fmd_Read(0x0a, readReg,4);
        //add some log
        SXS_TRACE(TSTDOUT,"fmd_ValidStop readReg[0]=0x%x",readReg[0]);   	
        SXS_TRACE(TSTDOUT,"fmd_ValidStop readReg[1]=0x%x",readReg[1]);   	
        SXS_TRACE(TSTDOUT,"fmd_ValidStop readReg[2]=0x%x",readReg[2]);   	
        SXS_TRACE(TSTDOUT,"fmd_ValidStop readReg[3]=0x%x",readReg[3]);   	
	}while((readReg[3]&0x80)==0);	

	if ((readReg[2] &0x01)==0) return 0;
		
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


// =============================================================================
// fmd_I2sOpen
// -----------------------------------------------------------------------------
/// This function open the I2S interface
/// @param fmdI2sCfg I2S configuration 
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sOpen(FMD_I2S_CFG_T fmdI2sCfg)
{
#if 0	
    UINT8 readReg[2] = {0};
    UINT8 writeReg[2] = {0};
    FMD_ERR_T errStatus;
    writeReg[0] =  0x12;    
    // writeReg[1] =  0x80;
    writeReg[1] =((fmdI2sCfg.freq & 0xF)<<4);
    errStatus = fmd_Write(0x06, writeReg,2);  //i2s slave,  48k

    writeReg[0] = 0x00; 
    writeReg[1] = 0x40;                                  //enable i2s 
    errStatus = fmd_Write(0x04, writeReg,2);
    sxr_Sleep(50 MILLI_SECONDS);

   // close ear 
    errStatus = fmd_Read(0x03, readReg,2);
    //  SXS_TRACE(TSTDOUT,"fmd_Read=%x,fmd_Read=%x",readReg[0],readReg[1]);    
    writeReg[0] = readReg[0];
    writeReg[1] = readReg[1]|(1<<5);   
    errStatus = fmd_Write(0x03, writeReg,2);

    sxr_Sleep(50 MILLI_SECONDS);
    errStatus = fmd_Read(0x09, readReg,2);
    // SXS_TRACE(TSTDOUT,"fmd_Read=%x,fmd_Read=%x",readReg[0],readReg[1]);  	
    writeReg[0] = readReg[0];
    writeReg[1] = readReg[1]|(1<<0);
    errStatus = fmd_Write(0x09, writeReg,2);
hal_HstSendEvent(SYS_EVENT,0xFFFF000A);
    return errStatus;
#else
    return FMD_ERR_NO;
#endif    
}

// =============================================================================
// fmd_I2sClose
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sClose()
{
#if 0 
    UINT8 readReg[2] = {0};
    UINT8 writeReg[2] = {0};
    FMD_ERR_T errStatus;

    errStatus = fmd_Read(0x09, readReg,2);
    writeReg[0] = readReg[0];
    writeReg[1] = readReg[1]&0xfe;
    errStatus = fmd_Write(0x09, writeReg,2);
    //enable earphone
    sxr_Sleep(50 MILLI_SECONDS);
    errStatus = fmd_Read(0x03, readReg,2);
    writeReg[0] = readReg[0];
    writeReg[1] = readReg[1]&0xDF;
    errStatus = fmd_Write(0x03, writeReg,2);
   //disable  i2s
    writeReg[0] = 0x00;
    writeReg[1] = 0x00;
    errStatus = fmd_Write(0x04, writeReg,2);
    sxr_Sleep(50 MILLI_SECONDS);
	hal_HstSendEvent(SYS_EVENT,0xFFFF000B);
    return errStatus;
#else
    return FMD_ERR_NO;
#endif    
}

// =============================================================================
// fmd_PreInit
// -----------------------------------------------------------------------------
/// This function initial fm in handset power on.
// =============================================================================
PROTECTED  VOID  fmd_PreInit(VOID)
{
    UINT8 writeReg[2] = {0};

    // enable the clock
    g_fmdClockOutId = hal_SysClkOutOpen(HAL_SYS_CLOCK_OUT_FREQ_32K);
    g_fmdConfig = tgt_GetFmdConfig();

    // enable the POWER
    pmd_EnablePower(PMD_POWER_FM, TRUE);

    // Wait for at least powerOnTime
    hal_TimDelay(30 MS_WAITING);

    // Soft Reset
    writeReg[0] = 0;
    writeReg[1] = 0x02;
    fmd_Write(0x02, writeReg,2);

    // Wait about 5ms
    hal_TimDelay(5 MS_WAITING);

    // put radio in sleep mode
    writeReg[0] = 0x00;
    writeReg[1] = 0x00;
    fmd_Write(0x02, writeReg,2); 

    // cut the power, cut the clock
    pmd_EnablePower(PMD_POWER_FM, FALSE);
    hal_SysClkOutClose(g_fmdClockOutId);

}

