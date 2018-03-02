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
/// That file implement the FM driver for RDA5802 chip.
//
////////////////////////////////////////////////////////////////////////////////


#include "cs_types.h"

#include "fmd_config.h"
#include "fmd_m.h"
#include "tgt_fmd_cfg.h"
#include "fmdp_debug.h"

#include "pmd_m.h"

#include "hal_ispi.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "hal_timers.h"

#include "sxr_tim.h"

#include "sxr_tls.h"
#include "cmn_defs.h"


extern VOID pmd_EnableDcdcPower(BOOL on);


// =============================================================================
//  MACROS
// =============================================================================

#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef SPI_REG_DEBUG
#endif

#define BASE_FREQ_USEUROPE  87000
#define BASE_FREQ_JAPAN     76000
#define BASE_FREQ_WORLD     76000
#define BASE_FREQ_EAST_EUROPE     65000

#define CHAN_SPACE          g_chanSpacing[g_fmdConfig->channelSpacing]

// MACROS for PRIVATE FUNCTION USE ONLY
#define FMD_SPI_FRAME_SIZE  25

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
// TSD config
PRIVATE CONST TGT_FMD_CONFIG_T*  g_fmdConfig;


PRIVATE UINT32 g_baseFreq[FMD_BAND_QTY] = {BASE_FREQ_USEUROPE, BASE_FREQ_JAPAN, BASE_FREQ_WORLD, BASE_FREQ_EAST_EUROPE};
PRIVATE UINT32 g_chanSpacing[FMD_CHANNEL_SPACING_QTY] = {100,200,50};
PRIVATE FMD_BAND_T g_band = FMD_BAND_QTY; // using FMD_BAND_QTY as a closed state marker
PRIVATE UINT16 g_fmRegSysCtrl; // BassBoost ForceMono and Mute state
PRIVATE FMD_CALLBACK_T* g_callback = NULL;
PRIVATE FMD_STATE_T g_state = FMD_STATE_IDLE;

// =============================================================================
//  FUNCTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// PRIVATE FUNCTIONS
// -----------------------------------------------------------------------------

//=============================================================================
// fmd_SpiOpen
//-----------------------------------------------------------------------------
/// Open and activate a the ISPI bus for the FM chip.
/// A parameter is used to precise if this opening is aimed at reading
/// or writing a register in the FM chip memory map.
/// @param rwCfg selects to open for read or write
/// @return      \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY when spi is already 
///  in use (either by another interrupted fmd function or by another spi user)
//=============================================================================
PRIVATE FMD_ERR_T fmd_SpiOpen(VOID)
{
    // hal_IspiOpen has been moved to hal_Open
    return FMD_ERR_NO;
}

#if 0
//=============================================================================
// fmd_SpiClose
//-----------------------------------------------------------------------------
/// Deactivate and close the SPI, no matter how it was opened.
//=============================================================================
PRIVATE VOID fmd_SpiClose(VOID)
{
    // Never close ISPI
}
#endif

PRIVATE BOOL fmd_SpiCsActivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }
    
    return hal_IspiCsActivate(HAL_ISPI_CS_FM);
}

PRIVATE BOOL fmd_SpiCsDeactivate(BOOL singleWrite)
{
    // No need to activate CS for ISPI single data frame write-only operation
    if(singleWrite)
    {
        return TRUE;
    }

    return hal_IspiCsDeactivate(HAL_ISPI_CS_FM);
}

//=============================================================================
// fmd_WriteSingle
//-----------------------------------------------------------------------------
/// Write a Data at an address in the FM chip memory space
/// SPI must already be oppened for write
/// this function will wait only for SPI FIFO space to be available to write
/// the command
/// @param addr Register of the FM chip where to write
/// @param data Data to write at that address.
//=============================================================================
PRIVATE VOID fmd_WriteSingle(UINT16 addr, UINT16 data)
{
    UINT32 wrData;

    wrData = 0;
#ifdef FPGA
    wrData = ((addr & 0x01f0) << (21-4)) | (0<<20) | ((addr & 0x000f) << 16) | (data);
#else
    wrData = (0<<25) | ((addr & 0x01ff) << 16) | (data);
#endif

    // ensure there is enought space in SPI FIFO, wait by pooling...
    // and then write data
    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_FM) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_FM, wrData, FALSE) == 0);

    //wait until any previous transfers have ended
    while(!hal_IspiTxFinished(HAL_ISPI_CS_FM));

    FMD_TRACE(FMD_INFO_TRC,0,"FMD 5802 write 0x%02x : 0x%04x",addr, data);
}


//=============================================================================
// fmd_Write
//-----------------------------------------------------------------------------
/// Write a data at an address in the FM chip memory space
/// This function open, write, wait for end of command and close
/// @param addr register of the FM chip where to write
/// @param data data to write at that address.
/// @return     \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY when spi is already 
///  in use (either by another interrupted fmd function or by another spi user)
//=============================================================================
PRIVATE FMD_ERR_T fmd_Write(UINT16 addr, UINT16 data)
{
#if 0
    if (!fmd_SpiCsActivate(TRUE))
    {
        return FMD_ERR_RESOURCE_BUSY;
    }
#endif

    fmd_WriteSingle(addr, data);

#if 0
    fmd_SpiCsDeactivate(TRUE);
#endif

    return FMD_ERR_NO;
}



//=============================================================================
// fmd_Read
//-----------------------------------------------------------------------------
/// Read a register of the FM chip
/// @param addr register of the FM chip where to read
/// @param data pointer to load with the read value
/// @return     \c FMD_ERR_NO or \c FMD_ERR_RESOURCE_BUSY when spi is already 
///  in use (either by another interrupted fmd function or by another spi user)
//=============================================================================
PRIVATE FMD_ERR_T fmd_Read(UINT16 addr, UINT32* data)
{
    UINT32 wrData;
    UINT32 rdData;

    wrData = 0;
#ifdef FPGA
    wrData = ((addr & 0x01f0) << (21-4)) | (1<<20) | ((addr & 0x000f) << 16) | 0;
#else
    wrData = (1<<25) | ((addr & 0x01ff) << 16) | 0;
#endif

    if (!fmd_SpiCsActivate(FALSE))
    {
        return FMD_ERR_RESOURCE_BUSY;
    }
    
    // Write the address and stuffing stuff
    while(hal_IspiTxFifoAvail(HAL_ISPI_CS_FM) < 1 ||
            hal_IspiSendData(HAL_ISPI_CS_FM, wrData, TRUE) == 0);
    // no check on the return value as the FIFO is deep enought for 4 bytes
    
    // wait for the transfer to finish, so all data are there
    while(!hal_IspiTxFinished(HAL_ISPI_CS_FM));

    // Read the data
    hal_IspiGetData(HAL_ISPI_CS_FM, &rdData);
    // no check on the return value as the data are recieved in the FIFO

    fmd_SpiCsDeactivate(FALSE);

    // Data is 16 bit at the proper place.
    *data = (rdData & 0xFFFF);

    FMD_TRACE(FMD_INFO_TRC,0,"FMD 5802 read 0x%02x : 0x%04x",addr, *data);
    return FMD_ERR_NO;
}

//=============================================================================
// fmd_Scheduler
//-----------------------------------------------------------------------------
/// Process tune and seek operations
/// @param data used to send register read value to next time processing
/// in case SPI is not available.
//=============================================================================
PRIVATE VOID fmd_Scheduler(UINT32 data)
{
    UINT32 rdReg = data; // get back the previous read value
    FMD_ERR_T errStatus = FMD_ERR_NO;

    FMD_ASSERT(g_state < FMD_STATE_QTY, "FMD state unknown: %d", g_state);
    // the POLLING step
    if ((g_state == FMD_STATE_TUNE_POLL) || (g_state == FMD_STATE_SEEK_POLL))
    {
        data = 0;
        errStatus = fmd_Read(0x0a, &rdReg);
        if (errStatus == FMD_ERR_NO)
        {
            // no error, check STC bit
            if (rdReg & 0x4000)
            {
                // done: next step:
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
    // the STOP state
    if ((g_state == FMD_STATE_TUNE_STOP) || (g_state == FMD_STATE_SEEK_STOP))
    {
        //UINT32 rdRegRSSI;
        //errStatus = fmd_Read(0x0b, &rdRegRSSI);
        errStatus = fmd_Write(0x02, g_fmRegSysCtrl);
        if (errStatus == FMD_ERR_NO)
        {
            BOOL stereoStatus;
            UINT32 freqKHz = ((rdReg & 0x3ff) * CHAN_SPACE) + g_baseFreq[g_band];
            if ((rdReg & 0x400) == 0 )
            {
                stereoStatus = FALSE;
            }
            else
            {
                stereoStatus = TRUE;
            }
            // no error, it's finished, do the callback and stop
            if (g_state == FMD_STATE_TUNE_STOP)
            {
                g_state = FMD_STATE_IDLE;
                if (g_callback)
                {
                    g_callback(freqKHz, TRUE, stereoStatus);
                }
            }
            else if (g_state == FMD_STATE_SEEK_STOP)
            {
                g_state = FMD_STATE_IDLE;
                BOOL found;
                if ((rdReg & 0x2000) == 0 )
                {
                    found = TRUE;
                }
                else
                {
                    found = FALSE;
                }
                if (g_callback)
                {
                    g_callback(freqKHz, found, stereoStatus);
                }
            }
        }
    }

    // rescheduling if needed
    if (g_state != FMD_STATE_IDLE)
    {
        // send the read value (of POLLING step) to the next processing
        sxr_StartFunctionTimer(40 MILLI_SECONDS, fmd_Scheduler, rdReg, 0);
    }
}


// -----------------------------------------------------------------------------
// PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------

// =============================================================================
// fmd_Open
// -----------------------------------------------------------------------------
/// This function opens the FM driver and activate the FM chip.
/// @param fmdTgtCfg    configuration from TGT module, refer to documentation 
///                     of target module for details. 
///                     (nust not be \c NULL)
/// @param band         band selection
/// @param callback     callback for #fmd_Tune and #fmd_Seek
///                     (can be \c NULL)
/// @param initValues   initial channel & sound configuration 
///                     (can be \c NULL, in that case, the FM output is muted)
/// @return             \c FMD_ERR_NO, \c FMD_ERR_ALREADY_OPENED
///                     \c FMD_ERR_BAD_PARAMETER or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_Open(CONST TGT_FMD_CONFIG_T* fmdTgtCfg,
                          FMD_BAND_T band,
                          FMD_CALLBACK_T* callback,
                          FMD_INIT_T* initValues)
{
#ifdef SPI_REG_DEBUG
    extern VOID fmd_RegisterSpiRegCheckFunc(VOID);
    fmd_RegisterSpiRegCheckFunc();
#endif // SPI_REG_DEBUG

    FMD_ERR_T errStatus;
    UINT32 chan;
    
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
    chan = 0;
    // set the required initial state
    // ------------------------------
    if (initValues != NULL)
    {
        // set given parameters 
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

    // do the init sequence of the radio chip
    // --------------------------------------

    // enable the POWER (e.g., LNA controller)
    pmd_EnablePower(PMD_POWER_FM, TRUE);
#ifdef GALLITE_IS_8806
    pmd_EnableDcdcPower(FALSE);
#endif    

    // Wait for at least 30 ns + powerOnTime
    COS_SleepByTick(1+g_fmdConfig->powerOnTime);

    errStatus = fmd_SpiOpen();
    if (errStatus != FMD_ERR_NO)
    {
        // cut the POWER (e.g., LNA controller)
        pmd_EnablePower(PMD_POWER_FM, FALSE);

        g_band = FMD_BAND_QTY; // close marker
        return errStatus;
    }
    
    // Soft Reset
    fmd_Write(0x02, 0x8003);

    // System control, enable earphone and power
    g_fmRegSysCtrl |= 0x8001;
    fmd_Write(0x02, g_fmRegSysCtrl);
    fmd_Write(0x04, 0x0600);
    fmd_Write(0x05, 0x804f | ((g_fmdConfig->seekRSSIThreshold&0x7f) << 8));
    fmd_Write(0x07, 0x8628);
    fmd_Write(0x11, 0x4000);
    fmd_Write(0x12, 0x6387);
    fmd_Write(0x13, 0x030c);
    fmd_Write(0x15, 0x4180);
    fmd_Write(0x18, 0x5814);
    fmd_Write(0x1c, 0x20dc);
    fmd_Write(0x1d, 0x4008);
    fmd_Write(0x1e, 0x84cf);
    fmd_Write(0x21, 0x2fc0);
    fmd_Write(0x22, 0x1434);
    fmd_Write(0x23, 0xe4a4);
    fmd_Write(0x25, 0x1422);
    fmd_Write(0x27, 0x000c);
    fmd_Write(0x29, 0x13c9);
    fmd_Write(0x2a, 0x34d4);
    fmd_Write(0x2b, 0xda04);
    fmd_Write(0x33, 0x2048);
    fmd_Write(0x35, 0x0eda);
    fmd_Write(0x3a, 0x0015);
    fmd_Write(0x40, 0xd808);
    fmd_Write(0x03, ((chan & 0x003ff) << 6) | 0x0010 
            | ((g_band & 3) << 2) | (g_fmdConfig->channelSpacing & 3));
    
    COS_Sleep(100 );

    // initial tune if required
    // ------------------------------
    if (initValues != NULL)
    {
        fmd_Tune(initValues->freqKHz);
        // ignore return value... if tune fail callback will never be called
        // but the radio is open anyway...
        // moreover we just closed SPI, so it's probably still available 
        // (unless we got an IRQ)
    }

    return FMD_ERR_NO;
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
    // check oppened
    if (g_band == FMD_BAND_QTY)
    {
        return FMD_ERR_ALREADY_OPENED; // ALREADY_CLOSED here !!
    }

    // cancel pending actions
    if (g_state != FMD_STATE_IDLE)
    {
        g_state = FMD_STATE_IDLE;
        sxr_StopFunctionTimer((void (*)(void *))fmd_Scheduler);
    }

    // put radio in sleep mode (+mute and high Z)
    g_fmRegSysCtrl = 0x0000;
    fmd_Write(0x02, g_fmRegSysCtrl);

#ifdef GALLITE_IS_8806
    pmd_EnableDcdcPower(TRUE);
#endif    
    // cut the POWER (e.g., LNA controller)
    pmd_EnablePower(PMD_POWER_FM, FALSE);

    g_band = FMD_BAND_QTY; // close marker
    
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
    UINT16 channel;
    UINT32 chan; 
	
    if (g_state != FMD_STATE_IDLE)
    {
        return FMD_ERR_RESOURCE_BUSY;
    }

    chan = (freqKHz - g_baseFreq[g_band])/CHAN_SPACE;

    channel = ((chan & 0x003ff) << 6) | 0x10
            | ((g_band & 3) << 2) | (g_fmdConfig->channelSpacing & 3);

    fmd_Write(0x03, channel);

    // schedule read & callback
    g_state = FMD_STATE_TUNE_POLL;
    sxr_StartFunctionTimer(40 MILLI_SECONDS, fmd_Scheduler, 0, 0);
    return FMD_ERR_NO;
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
    UINT16 i2sCfgReg = 0;

    FMD_ASSERT(g_band != FMD_BAND_QTY, "fmd_I2sOpen: FM has NOT been opened yet");
    
    i2sCfgReg =     ((fmdI2sCfg.slave)     ?(1<<12):0)
                |   ((fmdI2sCfg.polarity)  ?(1<<11):0)
                |   ((fmdI2sCfg.edge)      ?(1<<10):0)
                |   ((fmdI2sCfg.dataSigned)?(1<<9):0)
                |   ((fmdI2sCfg.freq & 0xF)<<4)
                |   ((fmdI2sCfg.wsInv)     ?(1<<3):0)
                |   ((fmdI2sCfg.slckInv)   ?(1<<2):0)
                |   ((fmdI2sCfg.leftDelay) ?(1<<1):0)
                |   ((fmdI2sCfg.rightDelay)?(1<<0):0);

    // Configure I2S interface
    fmd_Write(0x06, i2sCfgReg);
    
    // Enable I2S
    fmd_Write(0x04, 0x0600 |(1<<6));
    
    return FMD_ERR_NO;
}

// =============================================================================
// fmd_I2sClose
// -----------------------------------------------------------------------------
/// This function close the I2S interface
/// @return         \c FMD_ERR_NO, or \c FMD_ERR_RESOURCE_BUSY
// =============================================================================
PUBLIC FMD_ERR_T fmd_I2sClose(VOID)
{
    // Disable I2S
    fmd_Write(0x04, 0x0600 | (0<<6));

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
    if (g_state != FMD_STATE_IDLE)
    {
        return FMD_ERR_RESOURCE_BUSY;
    }
    if (seekUp)
    {
        // Seek up
        errStatus = fmd_Write(0x02, 0x0300 | g_fmRegSysCtrl);
    }
    else
    {
        // Seek down
        errStatus = fmd_Write(0x02, 0x0100 | g_fmRegSysCtrl);
    }
    if (errStatus != FMD_ERR_NO)
    {
        return errStatus;
    }
    // schedule read & callback
    g_state = FMD_STATE_TUNE_POLL;
    sxr_StartFunctionTimer(40 MILLI_SECONDS, fmd_Scheduler, 0, 0);
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

    // Update sysCtrl
    if (g_state == FMD_STATE_IDLE)
    {   
        // only if tune or seek is not in progress, in that case,
        // the end of it will write to sysCtrl
        fmd_Write(0x02, g_fmRegSysCtrl);
    }

    return FMD_ERR_NO;
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
    UINT32 rdReg;
    FMD_ERR_T errStatus;
    if (pFreqKHz == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    errStatus = fmd_Read(0x0a, &rdReg);
    
    if (errStatus == FMD_ERR_NO)
    {
        *pFreqKHz = ((rdReg & 0x3ff) * CHAN_SPACE) + g_baseFreq[g_band];
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
    UINT32 rdReg;
    FMD_ERR_T errStatus;
    if (pStereoStatus == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }

    errStatus = fmd_Read(0x0a, &rdReg);
    
    if (errStatus == FMD_ERR_NO)
    {
        if ((rdReg & 0x400) == 0 )
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
// fmd_ValidStop
// -----------------------------------------------------------------------------
/// @param freqKHz      the channel frequency.
/// @return             \c TRUE if a channel is found or
///                     \c FALSE otherwise
// =============================================================================
PUBLIC BOOL fmd_ValidStop(UINT32 freqKHz)
{
    FMD_ERR_T errStatus = FMD_ERR_NO;
    UINT16 channel;
    UINT32 chan; 
    UINT32 rdReg;
    UINT8 i;
    BOOL ret = FALSE;

    //SXS_TRACE(TSTDOUT,"fmd_ValidStop=%d", freqKHz);   

    i = 0;
    rdReg = 0;
    while(i++ < 3)
    {
        errStatus = fmd_Read(0x03, &rdReg);
        if (errStatus == FMD_ERR_NO)
        {
            break;
        }
        COS_Sleep(10 );
    }
    if (errStatus != FMD_ERR_NO)
    {
        return ret;
    }

    chan = (freqKHz - g_baseFreq[g_band]) / CHAN_SPACE;

    channel = ((chan&0x003ff) << 6) | 0x10 |(rdReg&0x20)
                | ((g_band&0x3) << 2) | (g_fmdConfig->channelSpacing&0x3);

    errStatus = fmd_Write(0x03, channel);

    if (errStatus == FMD_ERR_NO)
    {
        COS_Sleep(50 );
        //waiting for FmReady
        i = 0;
        rdReg = 0;
        while (i++ < 10)
        {
            //read REG 0x0b
            errStatus = fmd_Read(0x0b, &rdReg);
            if (errStatus == FMD_ERR_NO && (rdReg&0x0080) != 0)
            {
                // FM seek is ready
                if ((rdReg&0x0100) != 0)
                {
                     UINT32 curFreq;
                     UINT32 fixFreq[]={96000, 100000, 103200};
                     UINT8 i;
                     fmd_Read(0x0a, &rdReg);
                     curFreq=((rdReg & 0x3ff) * CHAN_SPACE) + g_baseFreq[g_band];
                     for (i=0;i<sizeof(fixFreq)/sizeof(fixFreq[0]);i++)
                    {
                          if (curFreq==fixFreq[i] && ((rdReg&0x0400) == 0))    
                                return FALSE;
                    }
                     
                    // current channel is a station
                    ret = TRUE;
                    break;
                }
                else
                {
                    // current channel is not a station
                    break;
                }
            }
            COS_Sleep(10 );
        }
    }

    return ret;
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
    UINT32 rdReg;
    FMD_ERR_T errStatus;
    if (pRSSI == NULL)
    {
        return FMD_ERR_BAD_PARAMETER;
    }
	
	errStatus = fmd_Read(0x0b, &rdReg);

	*pRSSI = ((rdReg & 0xfe00) >> 9);
    
    sxs_fprintf(TSTDOUT,"fmd_GetRssi=%d",*pRSSI);   
       
    return errStatus ;
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


#ifdef SPI_REG_DEBUG
// ======================================================
// Debug: Read and Write Registers via SPI

#define FMD_SPI_REG_VALUE_LOCATION __attribute__((section(".fm_reg_value")));

#define SPI_REG_WRITE_FLAG (1<<31)
#define SPI_REG_READ_FLAG (1<<30)
#define SPI_REG_FORCE_WRITE_FLAG (1<<29)
#define SPI_REG_FORCE_READ_FLAG (1<<28)
#define SPI_REG_DATA_MASK (0xffff)

#define FM_REG_NUM (0x40)
UINT32 g_fmdSpiRegValue[FM_REG_NUM] FMD_SPI_REG_VALUE_LOCATION;

PROTECTED VOID fmd_SpiRegValueInit(VOID)
{
    for (int i=0; i<FM_REG_NUM; i++)
    {
        g_fmdSpiRegValue[i] = 0;
    }
}

PUBLIC VOID fmd_SpiRegCheck(VOID)
{
    static BOOL initDone = FALSE;
    if (!initDone)
    {
        fmd_SpiRegValueInit();
        initDone = TRUE;
    }
    
    for (int i=0; i<FM_REG_NUM; i++)
    {
        if (g_fmdSpiRegValue[i] & SPI_REG_WRITE_FLAG)
        {
            if (FMD_ERR_NO != fmd_Write(i, g_fmdSpiRegValue[i]&SPI_REG_DATA_MASK))
            {
                continue;
            }
            g_fmdSpiRegValue[i] &= ~SPI_REG_WRITE_FLAG;
        }
        if (g_fmdSpiRegValue[i] & SPI_REG_READ_FLAG)
        {
            UINT32 data;
            if(FMD_ERR_NO != fmd_Read(i, &data))
            {
                continue;
            }
            g_fmdSpiRegValue[i] &= ~(SPI_REG_READ_FLAG|SPI_REG_DATA_MASK);
            g_fmdSpiRegValue[i] |= data&SPI_REG_DATA_MASK;
        }
    }
}

PUBLIC VOID fmd_RegisterSpiRegCheckFunc(VOID)
{
    static BOOL registerIdleHook = FALSE;
    if (!registerIdleHook)
    {
        registerIdleHook = TRUE;
        extern bool sxs_RegisterDebugIdleHookFunc(void (*funcPtr)(void));
        sxs_RegisterDebugIdleHookFunc(&fmd_SpiRegCheck);
    }
}
#endif // SPI_REG_DEBUG


