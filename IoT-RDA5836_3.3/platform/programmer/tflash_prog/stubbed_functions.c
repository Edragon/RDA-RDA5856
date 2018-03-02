/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "cs_types.h"
#include "hal_sys.h"
#include "cos.h"

#ifndef MCU_WIFI_PLAT
#include "boot_usb_monitor.h"


#include "hal_usb.h"
#include "hal_mem_map.h"


// The purpose of the following is to avoid the inclusion of big symbols in 
// the final flash programmer lodfile.

BOOL g_usb_connected = FALSE;


// This bcpu_main replaces the regular bcpu_main present in the platform library
PROTECTED VOID bcpu_main()
{
}


//================================================
// EDRV stuff
//================================================

#include "aud_m.h"
PUBLIC AUD_ERR_T aud_AnalogSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_AnalogTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_AnalogCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}


PUBLIC AUD_ERR_T aud_MaximSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_MaximTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_MaximCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_NullSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_NullTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_NullCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}


PUBLIC AUD_ERR_T aud_FmSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_FmTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_FmCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtStreamStart(
    SND_ITF_T itf,
    CONST HAL_AIF_STREAM_T* stream,
    CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtStreamRecord(
    SND_ITF_T itf,
    CONST HAL_AIF_STREAM_T* stream,
    CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_BtStreamStop(SND_ITF_T itf) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtStreamPause(SND_ITF_T itf, BOOL pause) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtTone(SND_ITF_T itf,
    CONST SND_TONE_TYPE_T tone,
    CONST AUD_DEVICE_CFG_T* cfg,
    CONST BOOL start) {return AUD_ERR_NO;}
    
PUBLIC AUD_ERR_T aud_BtTonePause(SND_ITF_T itf, BOOL pause) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_BtCalibUpdateValues(SND_ITF_T itf) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvStreamStart(
    SND_ITF_T itf,
    CONST HAL_AIF_STREAM_T* stream,
    CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvStreamRecord(
    SND_ITF_T itf,
    CONST HAL_AIF_STREAM_T* stream,
    CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_TvStreamStop(SND_ITF_T itf) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvStreamPause(SND_ITF_T itf, BOOL pause) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvTone(SND_ITF_T itf,
    CONST SND_TONE_TYPE_T tone,
    CONST AUD_DEVICE_CFG_T* cfg,
    CONST BOOL start) {return AUD_ERR_NO;}
    
PUBLIC AUD_ERR_T aud_TvTonePause(SND_ITF_T itf, BOOL pause) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_TvCalibUpdateValues(SND_ITF_T itf) {return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_LineinSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_LineinTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_LineinCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_CodecGalliteSetup(SND_ITF_T itf, CONST AUD_DEVICE_CFG_T* cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteStreamStart(SND_ITF_T itf,
                                       CONST HAL_AIF_STREAM_T* stream,
                                       CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteStreamStop(SND_ITF_T itf){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteStreamPause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteStreamRecord(
                        SND_ITF_T itf, 
                        CONST HAL_AIF_STREAM_T* stream,
                        CONST AUD_DEVICE_CFG_T*        cfg){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteTone(
                SND_ITF_T itf, 
                CONST SND_TONE_TYPE_T  tone,
                CONST AUD_DEVICE_CFG_T*       cfg,
                CONST BOOL             start){return AUD_ERR_NO;}
PUBLIC AUD_ERR_T aud_CodecGalliteTonePause(SND_ITF_T itf, BOOL pause){return AUD_ERR_NO;}

PUBLIC AUD_ERR_T aud_CodecGalliteCalibUpdateValues(SND_ITF_T itf){return AUD_ERR_NO;}

PUBLIC VOID aud_InitEarpieceSetting(VOID) {}

PUBLIC VOID aud_EnableEarpieceSetting(BOOL enable) {}

PUBLIC AUD_ERR_T aud_CodecGalliteInit(VOID){     return AUD_ERR_NO;}
void DSM_EnqNvram(void)
{
/*	UINT16 p_UserData = NULL;
	p_UserData =NVRAMGetData(0x80, 6);
	p_UserData =NVRAMGetData(VM_SYSTEM,sizeof(comval_t));
	p_UserData =NVRAMGetData(VM_AP_BLUETOOTH,sizeof(APP_BT_FLASH_STORE));
	p_UserData =NVRAMGetData(VM_AP_MUSIC,sizeof(music_vars_t));
	p_UserData =NVRAMGetData(VM_AP_RADIO,sizeof(FM_play_status_t));*/
}
PUBLIC VOID umss_Close(VOID){}
#if 0// (CHIP_HAS_USB == 1)


// USB need a lot of stuff, so stub what can be stubbed
PUBLIC HAL_USB_EP_DESCRIPTOR_T*
hal_UsbDescriptorNewCmdEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback) {return 0;}

// The HAL lib compiled for a chip that defines HAL_USB_DRIVER_DONT_USE_ROMED_CODE
// call         hal_HostUsbRecv(); and         hal_HostUsbSend();
PUBLIC BOOL hal_HostUsbSend(VOID)
{
    return boot_HostUsbRecv();
}

PUBLIC UINT8 hal_HostUsbRecv(VOID)
{
    return boot_HostUsbSend();
}

PUBLIC HAL_USB_EP_DESCRIPTOR_T*
hal_UsbDescriptorNewBulkEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback) {return 0;}

PUBLIC HAL_USB_EP_DESCRIPTOR_T*
hal_UsbDescriptorNewInterruptEpDescriptor(UINT8 ep, UINT8 interval, HAL_USB_CALLBACK_T callback) {return 0;}

PUBLIC HAL_USB_EP_DESCRIPTOR_T*
hal_UsbDescriptorNewIsoEpDescriptor(UINT8 ep, HAL_USB_CALLBACK_T callback) {return 0;}

PUBLIC HAL_USB_INTERFACE_DESCRIPTOR_T*
hal_UsbDescriptorNewInterfaceDescriptor(
    UINT8 usbClass,
    UINT8 usbSubClass,
    UINT8 usbProto,
    UINT8 interfaceIdx,
    HAL_USB_EP_DESCRIPTOR_T** epList,
    UINT8* description,
    HAL_USB_CALLBACK_T callback) {return 0;}

PUBLIC VOID hal_UsbDescriptorCleanInterfaceListDescriptor(HAL_USB_INTERFACE_DESCRIPTOR_T** interfaces) {}

PUBLIC VOID hal_UsbDescriptorCleanDeviceDescriptor(HAL_USB_DEVICE_DESCRIPTOR_T* dev) {}

PUBLIC HAL_USB_INTERFACE_DESCRIPTOR_T*
hal_UsbDescriptorCopyInterfaceDescriptor(CONST HAL_USB_INTERFACE_DESCRIPTOR_T* interface) {return 0;}

#endif // (CHIP_HAS_USB == 1)


//#include "hal_debug.h"
//PUBLIC VOID hal_DbgPortSetup(HAL_DBG_PORT_SEL_T mode){}
//PUBLIC VOID hal_DbgPxtsSetup(UINT16 mask){}

#if 0
PUBLIC VOID hal_DbgPageProtectSetup(
        HAL_DBG_PAGE_NUM_T pageNum, HAL_DBG_PAGE_SPY_MODE_T mode,
        UINT32 startAddr, UINT32 endAddr){}

PUBLIC VOID hal_DbgPageProtectEnable(HAL_DBG_PAGE_NUM_T pageNum){}

PUBLIC VOID hal_DbgFatalTrigger(UINT8 trigId){}
PUBLIC VOID hal_DbgPxtsSendTrigger(HAL_DBG_PXTS_LEVEL_T level){}

PUBLIC VOID hal_DbgPxtsProfileFunctionEnter(HAL_DBG_PXTS_LEVEL_T level,
                                            UINT16 functionId){}
PUBLIC VOID hal_DbgPxtsProfileFunctionExit(HAL_DBG_PXTS_LEVEL_T level,
                                           UINT16 functionId){}
PUBLIC VOID hal_DbgPxtsProfileWindowEnter(HAL_DBG_PXTS_LEVEL_T level,
                                          UINT16 windowId){}
PUBLIC VOID hal_DbgPxtsProfileWindowExit(HAL_DBG_PXTS_LEVEL_T level,
                                         UINT16 windowId){}
PUBLIC VOID hal_DbgPxtsProfileTask(HAL_DBG_PXTS_LEVEL_T level,
                                   UINT16 taskId){}
PUBLIC VOID hal_DbgPxtsProfilePulse(HAL_DBG_PXTS_LEVEL_T level,
                                    UINT16 code){}
PUBLIC VOID hal_DbgPxtsProfileJobEnter(HAL_DBG_PXTS_LEVEL_T level,
                                       UINT16 jobId){}
PUBLIC VOID hal_DbgPxtsProfileJobExit(HAL_DBG_PXTS_LEVEL_T level,
                                      UINT16 jobId){}

PUBLIC VOID hal_DbgAssert(CONST CHAR* format, ...){}
PUBLIC VOID hal_DbgTrace(UINT32 level, UINT32 tsmap, CONST CHAR *fmt, ...){}


PUBLIC VOID hal_DbgPxtsSendCode(HAL_DBG_PXTS_LEVEL_T level, UINT16 code){}
PROTECTED VOID hal_DbgPxtsOpen(VOID){}
#endif

//PUBLIC VOID hal_AudForceDcdc4ClockOn(BOOL on){}

//PUBLIC VOID hal_AudOpen(VOID) {}
//PUBLIC VOID hal_AudClose(VOID) {}
//PUBLIC VOID hal_AudForcePowerDown(VOID) {}
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_GALLITE && \
     CHIP_ASIC_ID != CHIP_ASIC_ID_8808) // 8809 or later
//PUBLIC VOID hal_AudForceDcdc4ClockOn(BOOL on) {}
#endif

VOID aud_CodecCommonSetRateStg(HAL_AIF_FREQ_T sampleRate)
{}


#endif

void vPortYieldProcessor(void)
{

}

void rdabt_wait_btcpu_busy(){}

void rdabt_clear_xcpu_busy(){}


void DM_CheckPowerOnCause() {}

BOOL BAL_ATInit(VOID){return TRUE;}
BOOL AT_AsyncEventProcess(COS_EVENT* pEvent){return TRUE;}

void uart2_start(void){}
void uart2_deal_data(UINT32 param){}

INT32 tsd_GetKeyIndex(UINT16 volt){return 0;}

void PM_SetBattery(UINT8 value){}

void uart2_wakeup(){}

UINT8                        g_uaudIsoOutEp;
UINT8                        g_uaudIsoInEp;
UINT32                      g_uAudActualPacketSize;

PUBLIC VOID tsd_Debounce(VOID){}

PUBLIC VOID hal_AudEnableCodecAuxPll(BOOL on){}

PUBLIC VOID hal_AudSetDacVolume(UINT32 vol){}

PUBLIC VOID hal_AudSetAdcVolume(UINT32 vol){}
    
