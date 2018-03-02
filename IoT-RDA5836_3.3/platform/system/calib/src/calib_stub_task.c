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



#define  LCDD_GFX_TYPES

#include "cs_types.h"
#include "chip_id.h"

#include "calib_m.h"

//#include "fmg_m.h"
#include "lcdd_m.h"
#include "memd_m.h"
#include "tgt_memd_cfg.h"


#include "calibp_debug.h"
#include "calibp_calibration.h"
#include "calibp_stub.h"
#include "pal_gsm.h"


#include "sxr_ops.h"
#include "sxs_io.h"
#include "sxr_mem.h"



/// Needs to be defined before we include itf_msg.h 
typedef UINT8 MsgBody_t;
#include "itf_msg.h"


#include "uctls_m.h"
#include "pmd_m.h"
#ifdef CHIP_HAS_AP
#include "syscmds_m.h"
#endif


PROTECTED UINT8 g_calibStubTaskId = 0xFF;
PROTECTED UINT8 g_calibStubMbx = 0xFF;

/// Timer ID and durations. 
#define DISP_TIMER              0x42
#define DISP_TIME               500


#ifdef CES_DISPLAY

#define NCHAN_16                1
#define IMAGE_WIDTH             128
#define IMAGE_HEIGHT            60
#define IMAGE_FORMAT            LCDD_COLOR_FORMAT_RGB_565

/// Cool Logo for drawing on the phone (warning heavy metal!)
CONST UINT16 g_calibLogoRgb[(IMAGE_WIDTH*IMAGE_HEIGHT)*NCHAN_16] =
{
#include "logo_128x60_16.dat"
};

/// A frame buffer window containing the logo.
/// The region of interest is the full image.
CONST GFX_FBW_T g_calibLogoFbw =
{
    {(UINT16*)&g_calibLogoRgb, IMAGE_WIDTH, IMAGE_HEIGHT, IMAGE_FORMAT},
    {{0, 0}, IMAGE_WIDTH, IMAGE_HEIGHT}
};
#endif


#if (CHIP_HAS_USB == 1) && !defined(CALIB_WITHOUT_USB)
PRIVATE VOID calib_ChargerHandler(PMD_CHARGER_STATUS_T status)
{
    // In all modes where the charger has been plugged, start the USB service.
    switch(status)
    {
    case PMD_CHARGER_UNKNOWN:
        break;
    case PMD_CHARGER_UNPLUGGED:
        uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
        break;
    default:
        uctls_ChargerStatus(UCTLS_CHARGER_STATUS_CONNECTED);
        break;
    }
}
#endif // CHIP_HAS_USB && !defined(CALIB_WITHOUT_USB)


// =============================================================================
// calib_StubTaskInit
// -----------------------------------------------------------------------------
/// Calib Stub OS task.
// =============================================================================
PROTECTED VOID calib_StubTaskInit(VOID)
{
    UINT32  evt[4];
    Msg_t   *msg;
#ifdef CES_DISPLAY
    LCDD_SCREEN_INFO_T screenInfo;
    GFX_ROI_T fullScreenRoi;
#endif

    CALIB_PROFILE_FUNCTION_ENTER(calib_StubTestTaskInit);

    /// Some initialization.

    /// Enable full-speed access to flash and ram.
#ifndef CHIP_HAS_AP
    memd_FlashOpen(tgt_GetMemdFlashConfig());
#endif
#if !(defined(CHIP_HAS_AP) && defined(RUN_IN_DDR))
    memd_RamOpen(tgt_GetMemdRamConfig());
#endif

    // Not to charge during calibration
    pmd_CutOffCharger();

#if (CHIP_HAS_USB == 1) && !defined(CALIB_WITHOUT_USB)
    uctls_Open(/*g_calibStubMbx,*/ 0, 0, 0, "USB Calib");
    uctls_SetMode(UCTLS_ID_TRACE);

    // Force usb power cycle
    uctls_ChargerStatus(UCTLS_CHARGER_STATUS_DISCONNECTED);
    COS_SleepByTick(8000);

    // Initiate charger status
    calib_ChargerHandler(pmd_GetChargerStatus());
    // Configure PMD to warn the calib when a charger is plugged.
    pmd_SetChargerStatusHandler(calib_ChargerHandler);
#endif // CHIP_HAS_USB && !defined(CALIB_WITHOUT_USB)

#ifdef CHIP_HAS_AP
    syscmds_Open();
#endif

#ifdef CES_DISPLAY
    /// Setup the display.
    lcdd_Open();
    lcdd_SetStandbyMode(FALSE);
    lcdd_GetScreenInfo(&screenInfo);

    // Fill the whole screen with white.
    fullScreenRoi.start.x      = 0;
    fullScreenRoi.start.y      = 0;
    fullScreenRoi.width        = screenInfo.width;
    fullScreenRoi.height       = screenInfo.height;
    while (LCDD_ERR_NO != lcdd_FillRect16(&fullScreenRoi, 0xffff));

    /// Draw a cool logo.
    while (lcdd_Blit16(&g_calibLogoFbw, 0, 0) != LCDD_ERR_NO);
    lcdd_SetBrightness(6);

    fmg_PrintfInit(0, 8, screenInfo.width, screenInfo.height);

    SXS_TRACE(TSTDOUT, "Running code: Calib Embedded Stub");

    /// Switch ON/OFF PAL traces. 
    sxs_SetTraceLevel(_PAL, 0x1);
#endif

    // Init PAL
    pal_Initialisation(PAL_INIT_GLOBAL, PAL_GSM900 | PAL_DCS1800);

    // Start the calibration stub
    calib_StubOpen();

    /// Update the display. 
    calib_DispState(TRUE);

    /// Start the display refresh and battery monitoring timers. 
    sxs_StartTimer(DISP_TIME, DISP_TIMER, NULL, FALSE, g_calibStubMbx);

    /// Main loop. 
    while (1)
    {
        /// Wait for a timer event or a key message. 
        msg = sxr_Wait(evt, g_calibStubMbx);

        if (msg == 0)
        {
            /// Refresh screen. 
            if (evt[0] == DISP_TIMER)
            {
                CALIB_PROFILE_PULSE(calib_StubTimDisp);

                /// Update the display. 
                calib_DispState(FALSE);

                /// Wakeup the task to update the display. 
                sxs_StartTimer(DISP_TIME, DISP_TIMER, NULL, FALSE, g_calibStubMbx);
            }

            /// Skip the message handling. 
            continue;
        }
        else
        {
            /// Free the message.
            sxr_Free(msg);
            /// Don't handle the key pressing.
        }
    }
}

