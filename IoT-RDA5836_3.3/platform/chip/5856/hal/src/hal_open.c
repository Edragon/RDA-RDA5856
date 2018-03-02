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
///     @file   hal_open.c 
///     Implementation of the HAL layer initialization.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "page_spy.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "cfg_regs.h"

#if (CHIP_HAS_ASYNC_BCPU)
#include "bb_ctrl.h"
#endif

#include "tcu.h"

#include "tgt_m.h"
#include "tgt_hal_cfg.h"
#include "tgt_pmd_cfg.h"

#include "pmd_m.h"
#include "rfd_xcv.h"
//#include "calib_m.h"

#include "hal_rfspi.h"
#include "hal_timers.h"
#include "hal_ana_gpadc.h"
#include "hal_debug.h"
#include "hal_pwm.h"
#include "hal_mem_map.h"
#include "hal_gpio.h"
#include "hal_sys.h"
#include "hal_config.h"
#include "hal_map_engine.h"
#include "hal_speech.h"
#include "hal_usb.h"
#include "hal_camera.h"
#include "hal_ispi.h"
#include "hal_rda_abb.h"

#include "halp_irq.h"
#include "halp_page_spy.h"
#include "halp_sys.h"
#include "halp_sys_ifc.h"
#include "halp_timers.h"
//#include "halp_calib.h"
#include "halp_config.h"
#include "halp_debug.h"
#include "halp_speech.h"
#include "halp_ebc.h"
#include "halp_proxy_memory.h"
#include "halp_profile.h"

#include "boot_usb_monitor.h"
#include "halp_usb_monitor.h"
#include "tgt_app_cfg.h"
//#include "patch_m.h"

#include "stdio.h"


#ifdef ASICSIMU
#include "asicsimu_m.h"
#endif

//#include "pal_gsm.h"

// =============================================================================
//  MACROS
// =============================================================================

#if defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER)
#undef EARLY_TRACE_OPEN
#endif


// =============================================================================
//  GLOBAL VARIABLES
// =============================================================================
#ifdef SERVICE_CALIB_FROME_FLASH_SUPPORT
#ifndef _T_UPGRADE_PROGRAMMER
#if (AUDIO_CALIB_VER == 1)
extern VOID calib_DaemonInit(VOID);
#endif /* (AUDIO_CALIB_VER == 1) */
#endif /* _T_UPGRADE_PROGRAMMER */
#endif /* SERVICE_CALIB_SUPPORT */

extern VOID pmd_OpenI2cTrace(VOID);
extern VOID hal_SysSetVocClockDefault(VOID);
extern VOID hal_TcuInit(VOID);
extern VOID gpio_config(VOID);
extern VOID pmd_I2C_ExtEn(void);
extern VOID hal_BoardConfigP37P40(void);

// =============================================================================
// g_halCalibration
// -----------------------------------------------------------------------------
/// Since the calibration is managed by the Calib module, we need to use
/// an accessor function to get the pointer to the calibration structure.
/// To avoid to call that function numerous times, the g_halCalibration is
/// used as a pointer to the calibration structure, and is initialized by
/// hal_Open. g_halCalibration can be used by all driver of HAL.
// =============================================================================
//PROTECTED CALIB_CALIBRATION_T*  g_halCalibration = NULL;

#ifdef PAGE_SPY_LOG
// In 32 bits words, we keep the last 100 writes plus a stamp
#define SP_BUFFER_SIZE  (4*2048)



// =============================================================================
// g_SpBuffer
// -----------------------------------------------------------------------------
/// Page spy writes buffer
// =============================================================================
PROTECTED UINT32 g_SpBuffer[SP_BUFFER_SIZE];


// =============================================================================
// g_SpBufferTop
// -----------------------------------------------------------------------------
/// Pointer to the top position in the Page spy writes buffer
// =============================================================================
extern UINT32* g_SpBufferTop;


// =============================================================================
// g_SpBufferPtr
// -----------------------------------------------------------------------------
/// Pointer to the current position in the Page spy writes buffer
// =============================================================================
extern UINT32* g_SpBufferPtr;
#endif

#ifdef PAGE_SPY_AT_ZERO
// placeholder for protecting arrea at the beginning of ram (addr 0 and above)
PROTECTED UINT32 g_SpZero[256] __attribute__((section (".ram")));
#endif

// =============================================================================
//  FUNCTIONS
// =============================================================================

/// define CHIP_CHECK_UID_ENCRYPT in target.def
/// only if release version can work 
#if defined(CHIP_CHECK_UID_ENCRYPT) && defined(CT_NO_DEBUG)
UINT32 hal_uid_encrypt_algorithm(UINT32 num)
{
	UINT32  value;
	/// modify you encrypt algorithm here 
	/// also modify encrypt algorithm in tools 
	value=~(num<<2)+1;
	
	return value;
}
void hal_check_uid_encrypt()
{
	UINT32  val =	pmd_GetEncryptUid();
	TGT_FACTORY_SETTINGS_T *pFS = tgt_GetFactorySettings();
	UINT32  checksum =hal_uid_encrypt_algorithm(val);	
	
	if(pFS->uid != checksum)
	{
		HAL_ASSERT(0,"%x,%x,%x,The LOD file has been encrypted, please using authorized code!",val,checksum,pFS->uid);
		hal_SysSoftReset();
	}
}
#endif
// =============================================================================
// hal_Open
// -----------------------------------------------------------------------------
/// Initializes the HAL layer. 
/// This function also do the open of the PMD driver and set the idle-mode.
// =============================================================================
PUBLIC VOID hal_Open(CONST HAL_CFG_CONFIG_T* halCfg)
{
#ifdef EARLY_TRACE_OPEN
    // Enable trace before OS is ready, for the convenience of debugging in hal_Open().
    extern void sxs_HostTraceEarlyOpen (void);
    sxs_HostTraceEarlyOpen ();
#endif // EARLY_TRACE_OPEN

    // Prevent from going into test mode
    // through the test mode pin.

    // Checkers to ensures that defines used to publicly access registers are
    // coherent with the register map.
    HAL_ASSERT((UINT32)&(hwp_sysIrq->sc) == HAL_SYS_CRITICAL_SECTION_ADDRESS,
            "The critical section public access is not up to date with"
            "the memory map. Edit hal_sys.h and update "
            "HAL_SYS_CRITICAL_SECTION_ADDRESS.%08X %08X",
            (UINT32)&(hwp_sysIrq->sc),
            HAL_SYS_CRITICAL_SECTION_ADDRESS);

#ifdef GSM_SUPPORT
    // Install all patches
    patch_Open();
#endif

    // Initialize the map engine.
    hal_MapEngineOpen();

#if (CHIP_HAS_USB == 1)
    // Avoid crashing when calling hal_UsbClose() without hal_UsbOpen() before.
#ifndef _RDA5855_TODO_
    boot_UsbInitVar();
#endif
#endif // (CHIP_HAS_USB == 1)

#if defined(HAL_PROFILE_NOT_ON_ROMULATOR) || defined(HAL_PROFILE_ON_BUFFER)
    // Configure PXTS to enable record in RAM
    hal_DbgPxtsOpen();
#endif// defined(HAL_PROFILE_NOT_ON_ROMULATOR)

    // Initialize the System IFC.
    hal_IfcOpen();

#if (CHIP_HAS_PROXY_MEMORY == 1)
    // Disable access to the proxy memory from the external
    // bus. (ie. Enable the access at cpu speed)
    hal_PMemEnableExternalAccess(FALSE);
#endif // (CHIP_HAS_PROXY_MEMORY == 1)

    // Unlock following registers.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;

    // cache ram automatic disable mode
    hwp_sysCtrl->Cfg_Cpus_Cache_Ram_Disable|=
                 SYS_CTRL_XCPU_CACHE_RAM_DISABLE(1)
               | SYS_CTRL_BCPU_CACHE_RAM_DISABLE(1);
#define ENABLE_AUTO_CLOCK_GATING 1
#if (ENABLE_AUTO_CLOCK_GATING)
    // automatic clock gating enable
    hwp_sysCtrl->Clk_Sys_Mode = 0
#if (CHIP_HAS_ASYNC_TCU)
                              | SYS_CTRL_MODE_SYS_MAILBOX_MANUAL
                              | SYS_CTRL_MODE_SYS_A2A_MANUAL
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) //Fix bug:  write data to voc bus error,  maybe voc_clk : sys_clk  > 3 : 1 will okay?
                              |SYS_CTRL_MODE_SYS_VOC_AHB_MANUAL
#endif
                              ;
    hwp_sysCtrl->Clk_BB_Mode = 0
#if (CHIP_HAS_ASYNC_TCU)
                             | SYS_CTRL_MODE_BB_MAILBOX_MANUAL
                             | SYS_CTRL_MODE_BB_ROM_AHB_MANUAL
                             | SYS_CTRL_MODE_BB_COM_REGS_MANUAL
                             | SYS_CTRL_MODE_BB_A2A_MANUAL
#endif
                             ;
    hwp_sysCtrl->Clk_Other_Mode = 0;
#endif // ENABLE_AUTO_CLOCK_GATING

    // SDMMC might output voltage and clock when power-key is
    // pressed, which might cause T-card initialized to a false
    // state. SDMMC clock is disabled here to ensure T-card can
    // be initialized correctly later.
#if (CHIP_HAS_SDIO == 1) 
    hwp_sysCtrl->Clk_Per_Disable = SYS_CTRL_DISABLE_PER_SDMMC
                                 | SYS_CTRL_DISABLE_PER_SDMMC2;
    hwp_sysCtrl->Clk_Per_Mode = SYS_CTRL_MODE_PER_SDMMC_MANUAL
                              | SYS_CTRL_MODE_PER_SDMMC2_MANUAL;
#else
    hwp_sysCtrl->Clk_Per_Disable = SYS_CTRL_DISABLE_PER_SDMMC;
    hwp_sysCtrl->Clk_Per_Mode = SYS_CTRL_MODE_PER_SDMMC_MANUAL;
#endif

#ifdef DUAL_TFLASH_SUPPORT
    hwp_sysCtrl->BB_Rst_Set = SYS_CTRL_SET_RST_SDMMC2;
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
    hwp_sysCtrl->Clk_Sys_Disable2 = SYS_CTRL_DISABLE_SYS_TCU;
    hwp_sysCtrl->mbist_ctrl = SYS_CTRL_MBIST_CLK_AUTO_CTRL;
#endif

    // Lock registers.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    // Set the board specific configuration, depending on 
    // the target.
    
    hal_BoardSetup(halCfg);

    // for asic simulation configure the print after the board setup is done
#ifdef ASICSIMU
    asicsimu_Open();
#endif


#ifdef SERVICE_CALIB_FROME_FLASH_SUPPORT
#ifndef _T_UPGRADE_PROGRAMMER
#if (AUDIO_CALIB_VER == 1)
// ********************
//   Calibration Init  
// ********************

// This function check if the platform has been calibrated. If not,
// it uses the default calibration parameters that have been written
// in the Flash, with the code. If the platform has been calibrated,
// it uses the values stored in the calibration dedicated Flash sector. 
        
// If we are in PAL_CALIB mode, the global calibration buffer will 
// be stored in Ram. Jade Debug Server or the CalibTool will be able to
// write in an other buffer and swap it with the global calibration
// buffer. After the calibration is done one of these buffer will be
// burnt in Flash, in the calibration dedicated Flash sector.
    calib_DaemonInit();

    /// Init HAL internal pointer to the calibration structure.
    //g_halCalibration = calib_GetPointers();
#endif /* (AUDIO_CALIB_VER == 1) */
#endif /* _T_UPGRADE_PROGRAMMER */
#endif /* SERVICE_CALIB_SUPPORT */

    // Set the sys clock, and tcu clock, and analog clocks
    // In FPGA, the frequency doesn't change after the initial setup
    // (hal_SysChangeSystemClock is empty)
    // With the Chip, the frequency is set according to the users
    // need. For example, PAL asks for at least 39 Mhz.


#ifdef __FORCE_WORK_IN_26MHZ__LOCAL

#ifdef 	__FORCE_WORK_IN_26MHZ_LOCAL_UPTO_52MHZ__
	hal_SysSetupSystemClock(HAL_SYS_FREQ_52M);
#else
	hal_SysSetupSystemClock(HAL_SYS_FREQ_26M);
#endif

#else		
#ifdef FPGA
#ifndef FPGA_SLOW
    hal_SysSetupSystemClock(HAL_SYS_FREQ_39M);
#else // FPGA_SLOW
    hal_SysSetupSystemClock(HAL_SYS_FREQ_26M);
#endif // FPGA_SLOW
#else // !FPGA
    hal_SysSetupSystemClock(HAL_SYS_FREQ_26M);
#endif // !FPGA
#endif


    // Read XCV chip ID
    // (must be done after sys clk setup, as it needs to request freq)
    // The XCV chip ID must be used when configuring ABB, PMU and XCV
    rfd_XcvReadChipId();

    // Open ISPI with default (romed) config
    // (must be done after sys clk setup, as it needs to request freq)
    // (must be done before hal_AbbOpen/pmd_Open)
    hal_IspiOpen(NULL);

    // Initializes Power Management Driver and set
    // it as no-eco mode.
    pmd_Open(tgt_GetPmdConfig());
//8809

  #if (APP_SUPPORT_I2C_TRACE == 1)
    pmd_OpenI2cTrace();
  #else
    pmd_I2C_ExtEn();
    hal_BoardConfigP37P40();
  #endif

    gpio_config();

      // Initialize ABB (must be done before pmd_Open, as the ABB settings
    // in pmd_Open cannot be overridden or reset to defaults)
    hal_AbbOpen();

    pmd_SetPowerMode(PMD_IDLEPOWER);


    // EBC clock has been setup in boot_Sector
    hal_SysSetVocClockDefault();

    // By default, turn the CLK_OUT   *** OFF ***    The clock can be reconfigured
    // i.e. set to 32k or 26M / enabled or disabled later.
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_UNLOCK;
    hwp_sysCtrl->Clk_Other_Disable = SYS_CTRL_DISABLE_OC_CLK_OUT;
    hwp_sysCtrl->REG_DBG = SYS_CTRL_PROTECT_LOCK;

    // OS timer init. 
    hal_TimTickOpen();

//    hal_TimWatchDogOpen(5*HAL_TICK1S); // 5s default watchdog value

    // Enable and configure the page spies. 
/*    hal_DbgPageProtectSetup (HAL_DBG_PAGE_IRQ_STACKWATCH, HAL_DBG_PAGE_WRITE_TRIGGER, 
                    (UINT32) &_sys_sram_irq_stackwatch_start, (UINT32)&_sys_sram_irq_stackwatch_end);

    hal_DbgPageProtectSetup (HAL_DBG_PAGE_CODEWATCH, HAL_DBG_PAGE_WRITE_TRIGGER, 
                    (UINT32) &_ram_txt_start, (UINT32)&_ram_txt_end);

    hal_DbgPageProtectSetup (HAL_DBG_PAGE_INT_CODEWATCH, HAL_DBG_PAGE_WRITE_TRIGGER,  
                        (UINT32)&_sys_sram_txt_start, (UINT32)&_sys_sram_txt_end);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_IRQ_STACKWATCH);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_CODEWATCH);

    hal_DbgPageProtectEnable (HAL_DBG_PAGE_INT_CODEWATCH);

#ifdef PAGE_SPY_AT_ZERO
    hal_DbgPageProtectSetup(HAL_DBG_PAGE_4, HAL_DBG_PAGE_READWRITE_TRIGGER, (UINT32)g_SpZero, (UINT32)g_SpZero + sizeof(g_SpZero)-1);
    hal_DbgPageProtectEnable(HAL_DBG_PAGE_4);
#endif

#ifdef PAGE_SPY_LOG
    g_SpBufferTop = g_SpBuffer;
    g_SpBufferPtr = g_SpBufferTop;
    // The one useful page spy is set in the job thing.
#endif
*/
    // reset the speech fifo
    //hal_SpeechFifoReset();

    // Configure debugging
#ifdef ENABLE_PXTS
#ifdef ENABLE_EXL
    hal_DbgPortSetup(HAL_DBG_EXL_AND_PXTS);
#else
    // Enable PXTS
    hal_DbgPortSetup(HAL_DBG_PXTS);
#endif
    // if not extra lvl selected set no extra lvl
#ifndef ENABLE_PXTS_EXTRA_LVL
#define ENABLE_PXTS_EXTRA_LVL 0
#endif /* ENABLE_PXTS_EXTRA_LVL */
    // Enable some interesting levels by default
    hal_DbgPxtsSetup( 1<<HAL_DBG_PXTS_HAL
                    | 1<<HAL_DBG_PXTS_SX
                    | 1<<HAL_DBG_PXTS_PAL
                    | 1<<HAL_DBG_PXTS_BB_IRQ
                    | 1<<HAL_DBG_PXTS_SPC
                    | ENABLE_PXTS_EXTRA_LVL);
#else
#ifdef ENABLE_EXL
    hal_DbgPortSetup(HAL_DBG_EXL);
#endif
#endif

    // Ask Target to request every modules to register itself.
    tgt_RegisterAllModules();

#ifdef GSM_SUPPORT
    // initialise pal and rf stuff (transceiver, lps).
    pal_Open();
#else /*GSM_SUPPORT*/
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8809)
    //5855 does not using xcv, by xzc
    rfd_XcvOpen(0, tgt_GetXcvConfig()); // Also Handles RFSPI setup
#endif

    hal_TcuInit();
#endif /*GSM_SUPPORT*/

#ifdef __UPDATE_PROGRAM__
    memd_FlashOpen(tgt_GetMemdFlashConfig());
#endif
#if !(defined(_FLASH_PROGRAMMER) || defined(_T_UPGRADE_PROGRAMMER))
    // Camera: Configure the camera RST and PDN lines so that
    // the camera is off by now.
    // We trick this by opening and closing the camera drivers
    // with dummy configurations.

#if defined(CAMERA_SUPPORT)
    if (halCfg->camCfg.camUsed)
    {
        HAL_CAMERA_CFG_T cameraConfig = {0, };

        // Camera 0
        cameraConfig.rstActiveH = halCfg->camCfg.camRstActiveH;
        cameraConfig.pdnActiveH = halCfg->camCfg.camPdnActiveH;
        cameraConfig.dropFrame  = FALSE; // Irrelevant parameter.
        cameraConfig.camClkDiv  = 0xFF; // Irrelevant parameter.
        cameraConfig.endianess  = NO_SWAP; // Irrelevant parameter.
        cameraConfig.colRatio   = COL_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.rowRatio   = ROW_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.camId      = 0;

        hal_CameraOpen(&cameraConfig);
        hal_CameraClose();
    }
    if (halCfg->camCfg.cam1Used)
    {
        HAL_CAMERA_CFG_T cameraConfig = {0, };

        // Camera 1		
        cameraConfig.rstActiveH = halCfg->camCfg.cam1RstActiveH;
        cameraConfig.pdnActiveH = halCfg->camCfg.cam1PdnActiveH;
        cameraConfig.dropFrame  = FALSE; // Irrelevant parameter.
        cameraConfig.camClkDiv  = 0xFF; // Irrelevant parameter.
        cameraConfig.endianess  = NO_SWAP; // Irrelevant parameter.
        cameraConfig.colRatio   = COL_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.rowRatio   = ROW_RATIO_1_1; // Irrelevant parameter.
        cameraConfig.camId      = 1;

        hal_CameraOpen(&cameraConfig);
        hal_CameraClose();
    }
#endif
#endif // !(_FLASH_PROGRAMMER || _T_UPGRADE_PROGRAMMER)

#ifdef EARLY_TRACE_OPEN
    extern void sxs_HostTraceEarlyOpenDone (void);
    sxs_HostTraceEarlyOpenDone ();
#endif // EARLY_TRACE_OPEN
#if defined(CHIP_CHECK_UID_ENCRYPT) && defined(CT_NO_DEBUG)
	{
		hal_check_uid_encrypt();
	}
#endif

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5856) || (CHIP_ASIC_ID == CHIP_ASIC_ID_5996)
    hal_GpioInit();
#endif
}


