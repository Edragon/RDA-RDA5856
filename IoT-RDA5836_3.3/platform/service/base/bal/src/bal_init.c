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



#include "platform.h"
#include "ap_common.h"
#ifndef WIN32
#include "hal_rda_audio.h"
#endif

#include "hal_host.h"
#include "ts.h"
#include "gpio_edrv.h"

#ifndef ERR_SUCCESS
#define ERR_SUCCESS                                             0
#endif

extern INT32 VDS_Init();
extern INT32 DSM_DevInit();
#if defined(__AT_SUPPORT__)
extern BOOL BAL_ATInit(VOID);
#endif

BOOL BAL_Initalise()
{
#if defined(VDS_SUPPORT ) || (CHIP_BTOVERLAY_SUPPORTED==0) // tianwq
    INT32 err_code = 0x00;
#endif
    gpio_initialise();

#if defined(GSM_SUPPORT)
    DRV_FlashPowerUp();
    //mabo deleted,20070813
    //DRV_FlashInit(CSW_DVR_FLASH_INIT_PARA) ;
    //romuald added 20080425
    DRV_FlashInit();
#endif /* GSM_SUPPORT */

#if defined(VDS_SUPPORT) // tianwq
    err_code = VDS_Init();   // Initialize VDS. added bye nie. 20070322

    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID,"VDS_Init() OK.\n");        
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID,"VDS_Init() ERROR, Error code: %d.\n", err_code);
    }
#endif
hal_HstSendEvent(BOOT_EVENT,0x11123);
#if (CHIP_BTOVERLAY_SUPPORTED==0)
    err_code = DSM_DevInit();
    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID,"DSM_DevInit OK.\n");
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID,"DSM_DevInit ERROR, Error code: %d. \n",err_code);
    }
    hal_HstSendEvent(BOOT_EVENT,0x11127);
#endif    
#if defined(GSM_SUPPORT)
    err_code = REG_Init();
    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("REG_Init() OK.\n"));
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("REG_Init() Fail!Error code:%d.\n"), err_code);
    }

    err_code = CFW_CfgInit();
    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("CFW_CfgInit OK.\n"));
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("CFW_CfgInit ERROR, Error code: 0x%08x \n"), err_code);
    }

    err_code = SMS_DM_Init();
    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("SMS_DM_Init OK.\n"));
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID, TSTXT("SMS_DM_Init ERROR, Error code: %d.\n"), err_code);
    }
#endif /* GSM_SUPPORT */
hal_HstSendEvent(BOOT_EVENT,0x11129);
#if (CHIP_BTOVERLAY_SUPPORTED==0)
#ifdef MCD_TFCARD_SUPPORT
    err_code = FS_PowerOn();
    if(ERR_SUCCESS == err_code)
    {
        CSW_TRACE(BASE_BAL_TS_ID,"FS Power On Check OK.\n");
    }
    else
    {
        CSW_TRACE(BASE_BAL_TS_ID,"FS Power On Check ERROR, Error code: %d. \n",err_code);
    }
#endif
#endif
hal_HstSendEvent(BOOT_EVENT,0x11124);
    //DM_Audio_Side_test(2); // loopback mode for test audio
#if defined(__AT_SUPPORT__)
	BAL_ATInit();
#endif /* __AT_SUPPORT__ */
     hal_AudSetDacVolume(0);
     hal_AudSetAdcVolume(0);
	 hal_HstSendEvent(BOOT_EVENT,0x20160927);
    return TRUE;
}


