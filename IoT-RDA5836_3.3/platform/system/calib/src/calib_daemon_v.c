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
//  $HeadURL$ 
//    $Author$                                                         
//    $Date$                        
//    $Revision$                                                            
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file calib_daemon.c 
/// This file implements the calibration daemon functionality, for both lab
/// and production modes.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"
#include "string.h"

//#include "calib_m.h"
//#include "calibp_defaults.h"
#include "calibp_debug.h"
//#include "calibp_calibration.h"
#include "tgt_calib_cfg.h"
#include "tgt_calib_m.h"

//#include "pal_gsm.h"
#include "hal_mem_map.h"
#include "hal_dma.h"
#include "hal_host.h"
#include "hal_rda_audio.h"
#include "hal_bt.h"

#ifdef CHIP_HAS_AP
#include "hal_ap_comm.h"
#include "syscmds_m.h"
#endif

#include "sxr_mem.h"
#include "sxr_tls.h"

#include "memd_m.h"
#include "sxs_io.h"



#define STR(x) #x



// =============================================================================
// Definitions
// =============================================================================

#define CALIB_CHECK_RF_NAME 1

#define CALIB_CHECK_MINOR_VERSION 1

#define CALIB_UNCACHED_BSS __attribute__((section(".ucbss")))


// =============================================================================
// _calib_start
// -----------------------------------------------------------------------------
/// Start address of the calibration sector, in the flash.
/// Come from the linker.
// =============================================================================
extern CALIB_BUFFER_T _calib_start;


// =============================================================================
// _calib_size
// -----------------------------------------------------------------------------
/// Size of the calibration data
/// Come from the linker.
// =============================================================================
extern UINT32 _calib_size;


//  -------------------------------------------------- 
//  ----- Address of the calibration sector in flash. 

/// Calibrated parameters buffer (used when the platform has been calibrated).
/// This buffer is in flash and can be re-burnt ONLY using CalibTool.
/// The symbol "_calib_start" is in a dedicated sector of the flash and is
/// not erased while programming the flash. It is defined by LD script. 
CONST CALIB_BUFFER_T* CONST g_calibCalibSector = &_calib_start;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
extern CALIB_CALIBRATION_T  g_tgtCalibCfg;
extern CONST CALIB_CALIBRATION_T  g_tgtCalibCfgFlash;
#else
extern CALIB_CALIBRATION_T  g_tgtCalibCfg;
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================

extern PUBLIC VOID boot_InitFlashDriver(VOID);

INLINE VOID *calib_memcpy(VOLATILE VOID *dest, CONST VOLATILE VOID *src, UINT32 n)
{
    return memcpy((VOID *)dest, (CONST VOID *)src, n);
}



//=============================================================================
// calib_GetPointers
//-----------------------------------------------------------------------------
/// Access to the calibration parameters.
///
/// @return The global pointers used by the code to access the calibrated
/// parameters. When the target's flash does not contain a flash sector,
/// the calib parameters are initialized with default parameters during
/// the calibration init. Also returns the pointers used by the remote
/// calibration tools to get the version number of the calibration and to 
/// find the calibration buffers.
//=============================================================================
PUBLIC CALIB_CALIBRATION_T* calib_GetPointers(VOID)
{
    //return (&g_calibCalibration);
	return &g_tgtCalibCfg;
}




//  -------------------------------------------------- 
//  ----- Calibration trace print. 

//  Display information about the current calibration through the trace. 
#define _PAL  _HAL

#define calib_Print(x)         CALIB_TRACE(_PAL | TSTDOUT, 0, "Calibration: "x)
#define calib_Print1(x, y)     CALIB_TRACE(_PAL | TSTDOUT, 0, "Calibration: "x, y)
#define calib_PrintVal(x)      CALIB_TRACE(_PAL | TSTDOUT, 0, \
                                "Calibration: " #x " = %d", x)
#define calib_PrintUVal(x)     CALIB_TRACE(_PAL | TSTDOUT, 0,\
                                "Calibration: " #x " = %u", x)
#define calib_PrintXVal(x)     CALIB_TRACE(_PAL | TSTDOUT, 0,\
                                "Calibration: " #x " = 0x%x", x)


PRIVATE CALIB_PARAM_STATUS_T calib_InitAudioBuffer(VOID)
{

    CALIB_PARAM_STATUS_T paramStatus = CALIB_PARAM_DEFAULT;
    UINT32  audioTag = g_calibCalibSector->aud.audioVersionTag;
    UINT32  audioMark = CALIB_MARK_MASK(audioTag);
    UINT8   audioMajVer = CALIB_MAJ_MASK(audioTag);

    if ((audioMark == CALIB_MARK_VERSION) &&
        (audioMajVer == CALIB_MAJ_VERSION))
    {
        paramStatus = CALIB_PARAM_AUDIO_CALIBRATED;
        
        hal_HstSendEvent(SYS_EVENT, 0x5856eca1);
        hal_HstSendEvent(BOOT_EVENT, 0x5856eca2);

        calib_memcpy(&g_tgtCalibCfg.aud,
                     g_calibCalibSector,
                     sizeof(CALIB_AUDIO_ITF_T));       
    }
    return paramStatus;
}


//  -------------------------------------------------- 
//  ----- Calibration mechanism and structures init (USE CALIB). 

// At boot time, uses the the HST parameter Ram buffer 0 and
// copy the default parameters in it. 
PUBLIC VOID calib_DaemonInit(VOID)
{
    CALIB_PARAM_STATUS_T audioStatus;

    audioStatus = calib_InitAudioBuffer();
}

#if 0
//=============================================================================
// calib_DaemonTracePrint
//-----------------------------------------------------------------------------
/// Display in the trace the current calibration status, used for debug.
//=============================================================================
PUBLIC VOID calib_DaemonTracePrint(VOID)
{


}



//  -------------------------------------------------- 
//  ----- Calibration mechanism and structures init. 

//  Get the default calibration parameters values and set the HST calib
//  * buffers with it. This is used to init the calibration buffers and when
//  * the reset calib command is called. 

//  After this, calib_UpdateBuffer() function must be called to copy the
//  * default parameters from HST buffers to used buffers. 

PRIVATE VOID calib_SetRfBufferToDefault(VOID)
{

}


PRIVATE VOID calib_SetAudioBufferToDefault(VOID)
{

}


/// ----- Updating the HST buffer from the used buffers (USE CALIB). 
/// Copy the calib parameters from the HST RAM buffer to the used RAM
/// buffer. This function is called when the calib parameters are modified
/// by HST. Note: it does not update VoC or the bandgap cal. 
PRIVATE VOID calib_UpdateRfBuffer(VOID)
{
}


PRIVATE VOID calib_UpdateAudioBuffer(VOID)
{
}


PRIVATE VOID calib_UpdateBuffer(VOID)
{
    calib_UpdateRfBuffer();
    calib_UpdateAudioBuffer();
}


PUBLIC UINT8 calib_CheckRfCrc(VOID)
{


}


PRIVATE UINT32 calib_GenerateRfCrc(CONST VOID *data, UINT16 len)
{
     return 0;
}

PRIVATE CALIB_PARAM_STATUS_T calib_InitRfBuffer(VOID)
{

    
}

// =============================================================================
// calib_DaemonSetContext
// -----------------------------------------------------------------------------
/// Defines the context structure that HST and JCES must use.
/// @param ctx Pointer to the context to use.
// =============================================================================
PUBLIC VOID calib_DaemonSetContext(VOID* ctx)
{
    //g_calibCalibration.stubCtx = ctx;
}
PUBLIC VOID calib_DaemonSetNsftContext(VOID* ctx)
{
    //g_calibCalibration.stubNsftCtx = ctx;
}
//  -------------------------------------------------- 
//  ----- Calibration command execution. 
PRIVATE VOID calib_PrepareRfData(CALIB_BUFFER_T *buffer)
{
    CALIB_VERSION_TAG_T tagConfigured = CALIB_VERSION_NUMBER;
}


PRIVATE VOID calib_PrepareAudioData(CALIB_BUFFER_T *buffer)
{
    CALIB_VERSION_TAG_T tagConfigured = CALIB_VERSION_NUMBER;

}

PRIVATE BOOL calib_BurnBuffer(CONST VOID *buffer)
{
    CONST CALIB_BUFFER_T *calibBuffer = buffer;
    BOOL ok = TRUE;

    return ok;
}

PRIVATE BOOL calib_BurnCurUsedData(VOID)
{
    CALIB_VERSION_TAG_T tagConfigured = CALIB_VERSION_NUMBER;
    BOOL ok = TRUE;
    return ok;
}
#endif

/// Function used to program the flash calibration sector.
/// Hyp: MEMD Flash Driver opened before hand.
PRIVATE BOOL burn(CONST VOID* fshPptr, CONST VOID* srcPtr, UINT32 size)
{
    if (size == 0)
    {
        return TRUE;
    }

#ifdef CHIP_HAS_AP

    memcpy((UINT8*)HAL_SYS_GET_UNCACHED_ADDR(fshPptr), srcPtr, size);

#else // !CHIP_HAS_AP

    MEMD_ERR_T flashError;
    UINT32 len, addrFsh;

    //  Find the address local in the flash. 
    addrFsh = (UINT32)fshPptr - (UINT32)memd_FlashGetGlobalAddress(0);
    addrFsh &= 0x0fffffff;

    // Write calib flash sector. 
    // This function is in RAM, so it can be executed no matter
    // which sector is accessed to burn the calibration sector.
    // Moreover, this code is executed during the Fint interrupt,
    // which has a very high priority, so no other code (That could
    // come from the same bank) is executed.
    flashError = memd_FlashWrite((UINT8*)addrFsh, size, &len, (CONST UINT8*)srcPtr);

    //  Check flash writting. 
    if ((flashError != MEMD_ERR_NO) || (len != size))
    {
        //  Here we are screwed-up. Just return false. 
        calib_PrintVal(flashError);
        return FALSE;
    }

#endif // !CHIP_HAS_AP

    return TRUE;
}



#if (AUDIO_CALIB_VER == 1)
PRIVATE BOOL calib_BurnCurUsedAudioData(CALIB_VERSION_TAG_T tag)
{
    //CALIB_VERSION_TAG_T tagConfigured = CALIB_VERSION_NUMBER;
    BOOL ok = TRUE;
	
    hal_HstSendEvent(BOOT_EVENT, 0x2017ca3d);
    hal_HstSendEvent(BOOT_EVENT, sizeof(g_tgtCalibCfg.aud));
	
    ok &= burn(g_calibCalibSector, 
                &g_tgtCalibCfg.aud,
                sizeof(g_tgtCalibCfg.aud));
    hal_HstSendEvent(BOOT_EVENT, 0x2017ca3f);
    hal_HstSendEvent(BOOT_EVENT, ok);

    return ok;
}

extern volatile UINT8 HAL_UNCACHED_BSS g_flash_write_req;
PUBLIC BOOL calib_BurnFlash(VOID)
{
    UINT32 calibSectorStart;
    UINT32 calibSectorEnd;
    CALIB_VERSION_TAG_T tag = g_calibCalibSector->aud.audioVersionTag;

#ifdef CHIP_HAS_AP

    calibSectorStart = HAL_SYS_GET_UNCACHED_ADDR(g_calibCalibSector);
    calibSectorEnd = HAL_SYS_GET_UNCACHED_ADDR(&_factory_start);
    memset((UINT8*)calibSectorStart, 0xFF, calibSectorEnd-calibSectorStart);

#else // !CHIP_HAS_AP

    // The dst_ptr is a local flash address, this is UINT32. 
    MEMD_ERR_T flashError;

    // Erase calib flash sector.
    calibSectorStart = HAL_SYS_GET_UNCACHED_ADDR(g_calibCalibSector) -
                HAL_SYS_GET_UNCACHED_ADDR((UINT32)&_flash_start);

    calibSectorEnd = HAL_SYS_GET_UNCACHED_ADDR(&_factory_start) -
                HAL_SYS_GET_UNCACHED_ADDR((UINT32)&_flash_start);

    hal_HstSendEvent(SYS_EVENT, 0x2017ca5d);
    hal_HstSendEvent(SYS_EVENT, (UINT32)&_factory_start);	
    hal_HstSendEvent(SYS_EVENT, (UINT32)g_calibCalibSector);	
    hal_HstSendEvent(SYS_EVENT, (UINT32)&_flash_start);	

    
    hal_HstSendEvent(SYS_EVENT, calibSectorStart);	
    hal_HstSendEvent(SYS_EVENT, calibSectorEnd);	

#if !defined(_FLASH_PROGRAMMER)
#if CHIP_HAS_BTCPU==1

    hal_HstSendEvent(SYS_EVENT,g_flash_write_req);

    g_flash_write_req=1;
	
    hal_HstSendEvent(SYS_EVENT,g_flash_write_req);

    hal_TimDelay(5);

	if(hal_IsBtStart())
	{
		if(hal_IsBtSleep())
			hal_BtNotifyDataSent();
    	while(g_flash_write_req!=2);
   	}
#endif
#endif

    UINT32 status = hal_SysEnterCriticalSection();
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)
    hal_HstSendEvent(SYS_EVENT, 0x2017ca5e);
    //if(hal_VocIsClose())
    {
        boot_InitFlashDriver();
    }
#endif

    flashError = memd_FlashErase((UINT8*)calibSectorStart, (UINT8*)calibSectorEnd);

    if (flashError != MEMD_ERR_NO)
    {
        // In case of error, just say it to HST. 
        calib_Print("ERROR! Calib flash sector erase problem!");
        calib_PrintVal(flashError);

        g_flash_write_req = 0;
        hal_SysExitCriticalSection(status);

        return FALSE;
    }
    //Flash_CalibWriteFlashFlag(TRUE);
#endif
    if (!calib_BurnCurUsedAudioData(tag))
    {
        // TODO: we should say it to HST in case of error
        calib_Print("ERROR! Calib flash sector burn problem!");
    #ifndef CHIP_HAS_AP
        //Flash_CalibWriteFlashFlag(FALSE);
    #endif

           g_flash_write_req = 0;
	    hal_SysExitCriticalSection(status);

        return FALSE;
    }

#ifdef CHIP_HAS_AP
    hal_ApCommSetFactoryUpdateCmd(HAL_AP_COMM_FACT_UPDATE_CALIB);
#else
    //Flash_CalibWriteFlashFlag(FALSE);
#endif

    g_flash_write_req = 0;
    hal_SysExitCriticalSection(status);

    return TRUE;
}

PUBLIC VOID calib_UpdateBufferData(CALIB_BUFFER_T *buffer)
{

   calib_memcpy(&g_tgtCalibCfg.aud, &buffer, sizeof(CALIB_BUFFER_T));
}

#endif


//  Function used to update the Platform's used calibration buffers when the
//  user modifies them with HST. It also executes calib cmd. 
PUBLIC VOID calib_DaemonProcess(VOID)
{

    return;
}


// =============================================================================
// calib_DaemonUsrDataProcess
// -----------------------------------------------------------------------------
/// Apply and burn HST calibration buffer.
/// @return TRUE if success, and FALSE otherwise.
// =============================================================================
PUBLIC BOOL calib_DaemonUsrDataProcess(VOID)
{


    //UINT32 scStatus;
    BOOL result = TRUE;
    CALIB_COMMAND_T cmd;
    cmd = CALIB_CMD_FLASH_BURN;
    return result;
}


// =============================================================================
// calib_GetRfCalibStatus
// -----------------------------------------------------------------------------
/// Get the status of RF calibration data.
/// @return 0 if calibrated with good CRC, 1 if calibrated with bad CRC,
///         2 if not calibrated.
// =============================================================================
PUBLIC UINT8 calib_GetRfCalibStatus(VOID)
{
    // RF not calibrated
    return 2;
}


// =============================================================================
// calib_GetAudioCalibStatus
// -----------------------------------------------------------------------------
/// Get the status of audio calibration data.
/// @return 0 if calibrated, 2 if not calibrated.
// =============================================================================
PUBLIC UINT8 calib_GetAudioCalibStatus(VOID)
{
    // Audio not calibrated
    return 2;
}


#ifdef CHIP_HAS_AP
// =============================================================================
// calib_DaemonSetAutoCallMode
// -----------------------------------------------------------------------------
/// Tell calibration daemon process whether current mode is autocall.
// =============================================================================
PUBLIC VOID calib_DaemonSetAutoCallMode(BOOL autocall)
{
    g_calibAutoCallMode = autocall;
}


// =============================================================================
// calib_DaemonHandleApResponse
// -----------------------------------------------------------------------------
/// Receive the flash burning result from AP bootloader.
// =============================================================================
PUBLIC VOID calib_DaemonHandleApResponse(BOOL result)
{
    CALIB_COMMAND_T POSSIBLY_UNUSED tag =
        *((CALIB_COMMAND_T *)HAL_SYS_GET_UNCACHED_ADDR(
            (UINT32)(&g_calibCalibration.command)));
    //  --- Calibration is done --- 
    // Inform HST that the command has been processed
    if (result)
    {
        calib_Print1("AP has finished the flash operation successfully: 0x%08x", tag);
        g_calibCalibration.command = CALIB_CMD_DONE;
    }
    else
    {
        calib_Print1("ERROR! AP failed to do the flash operation: 0x%08x", tag);
        g_calibCalibration.command = CALIB_CMD_FLASH_ERROR;
    }

    // Re-enable calib process
    pal_SetCalibProcess(TRUE);
}
#endif



