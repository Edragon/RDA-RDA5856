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
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file tgt_m.h                                                             //
/// That file provides the TGT API related to KEY and form factor factory
/// setting configuration.                                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _TGT_H_
#define _TGT_H_

#include "cs_types.h"

#include "tgt_pmd_cfg.h"
// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// TGT_IMEI_T
// -----------------------------------------------------------------------------
/// Type used to represent a IMEI number.
// =============================================================================
typedef UINT8 TGT_IMEI_T[9];

// =============================================================================
// TGT_MOBILE_ID_T
// -----------------------------------------------------------------------------
/// Define the mobile identification structure 
/// This structure must be included at the beginning of the factory settings sector
/// The rest of the factory settings sector can be customized to meet the customer needs
// =============================================================================
typedef struct {
    // Hold a valid status bit for the IMEI, check with a CRC, and markers
    // for the factory setting configuration step status.
    UINT32 validMarker;
    // IMEISV for the sims of the phone. (Hardcoded at most 4 sims.)
    TGT_IMEI_T imeiSv[4];
} TGT_MOBILE_ID_T;

// =============================================================================
// TGT_MOBILE_SN_T
// -----------------------------------------------------------------------------
/// The structure contains the mobile serial numbers
// =============================================================================
typedef struct
{
    // Mother board serial numbers.
    UINT8 mbSn[50];
    // Mobile phone serial numbers.
    UINT8 mpSn[50];
} TGT_MOBILE_SN_T;

// =============================================================================
// TGT_MODULE_ID_T
// -----------------------------------------------------------------------------
/// Define the module identification structure 
/// Module IDs are used to speed up LCD and Camera init.
// =============================================================================
typedef struct
{
    UINT8 lcd_id;
    UINT8 cam_id[2];
} TGT_MODULE_ID_T;

// =============================================================================
// TGT_BT_INFO_T
// -----------------------------------------------------------------------------
/// The structure contains the bluetooth device info
// =============================================================================
typedef struct
{
    UINT16 activated;
    UINT8 DEVICE_ADDR[6];
    UINT8 DEVICENAME[32];
    UINT8 PINCODE[16];
} TGT_BT_INFO_T;

// =============================================================================
// TGT_SALES_TRACK_INFO_T
// -----------------------------------------------------------------------------
/// The structure contains the sales tracking info
// =============================================================================
typedef struct
{
    UINT8  WORKMODE;
    UINT8  SWITCH;
    UINT8  SENDONCE;
    UINT8  BATLOSE;
    UINT32 INSIM_TIME;
    UINT32 NORMAL_TIME_LIMT;
    UINT32 TESTMOD_TIME_LIMT;
} TGT_SALES_TRACK_INFO_T;

// =============================================================================
// TGT_WIFI_INFO_T
// -----------------------------------------------------------------------------
/// The structure contains the wifi device info
// =============================================================================
typedef struct
{
    UINT16 activated;
    UINT8  mac_addr[6];
    UINT8  mac_ap1[6];
    UINT8  mac_ap2[6];
    UINT8  mac_ap3[6];
} TGT_WIFI_INFO_T;

// =============================================================================
// TGT_HW_MODULE_T
// -----------------------------------------------------------------------------
/// The structure contains the hardware module names
// =============================================================================
typedef struct
{
    CONST CHAR *target;
    CONST CHAR *chipAsic;
    CONST CHAR *chipDie;
    CONST CHAR *chipPkg;
    CONST CHAR *rfPa;
    CONST CHAR *flash;
    CONST CHAR *lcd;
    CONST CHAR *cam;
    CONST CHAR *bt;
    CONST CHAR *fm;
    CONST CHAR *atv;
} TGT_HW_MODULE_T;

typedef struct
{
	INT8   dbsSeed[63];
} TGT_DBS_INFO_T;




typedef struct
{
    UINT8                          sxr_NB_MAX_TASK;
    UINT8                          sxr_STACK_SIZE_MULTIPLIER;
    UINT8                          sxr_TK_STCK_HEAP;
    UINT8                          sxr_JB_STCK_HEAP;
    UINT8                          sxr_JB_CTX_HEAP;
    UINT16                         sxr_NB_MAX_MBX_ENV;
    UINT8                          sxr_EVT_MBX_SIZE;
    UINT8                          sxr_NB_MAX_MBX;
    UINT8                          sxr_NB_MAX_SEM;
    UINT8                          sxr_NB_MAX_MUTEX;
    UINT8                          sxr_TIMER_NB_INSTANCE;
    UINT16                         sxr_NB_MAX_TIMER_ENVELOPPE;
    UINT8                          sxr_TIMER_CTX_SIZE;
    UINT8                          sxr_TIMER_LIST_SIZE;
    UINT8                          sxr_REGULAR_TIMER;
    UINT8                          sxr_FRAMED_TIMER;
    UINT8                          sxr_NB_MAX_POOL;
    UINT8                          sxr_NB_HEAP_USER;
    UINT16                         sxr_NB_MAX_QUEUE;
    UINT16                         sxr_NB_MAX_USR_QUEUE;
    UINT16                         sxr_NB_QUEUE_ENVELOPPE;
    UINT16                         sxr_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_START;
    UINT16                         sxr_NB_FREE_QUEUE_ENVELOPPE_OVERLOAD_END;
    UINT8                          sxr_NB_MAX_TRACEID;
    UINT8                          sxs_FLH_NB_FILES_MAX;
    UINT8                          sxs_SPY_NB_MAX;
}SX_NB_T;


// =============================================================================
// TGT_FACTORY_SETTINGS_T
// -----------------------------------------------------------------------------
/// Define the factory settings structure for this target
// =============================================================================
typedef struct
{
    UINT32 magic;
    UINT32 len;
    TGT_PMD_VOLT_CAP_T curve[48];
} TGT_BATT_CURVE_T;

// =============================================================================
// TGT_FACTORY_SETTINGS_T
// -----------------------------------------------------------------------------
/// Define the factory settings structure for this target
// =============================================================================
typedef struct
{
    // mobile id
    TGT_MOBILE_ID_T mobileId;                   // 0x00000000
    // mobile sn
    TGT_MOBILE_SN_T mobileSn;                 //  0x00000028
    // module id
    TGT_MODULE_ID_T moduleId;                //  0x0000008c
    // encrypted checksum
    UINT32          uid;                                  //  0x00000090
    // bt device info
    TGT_BT_INFO_T   btInfo;                       //  0x00000094
    // sales tracking info
    TGT_SALES_TRACK_INFO_T trackInfo;  //  0x000000cc
    // wifi device info
    TGT_WIFI_INFO_T wifiInfo;                   //  0x000000dc
    // battery curve
    TGT_BATT_CURVE_T battCurve;            // 0x000000f8

    TGT_DBS_INFO_T dbs_info;                  // 0x000001c0

	
} TGT_FACTORY_SETTINGS_T;  // size 0x200

#ifndef WIN32
STATIC_ASSERT((sizeof(TGT_FACTORY_SETTINGS_T) <= 4*1024),
    "Error: Factory structure too large.");
#endif

PUBLIC VOID tgt_Open(VOID);
// =============================================================================
// tgt_GetBuildVerNo
// -----------------------------------------------------------------------------
/// This function is used to access the version number of this build.
// =============================================================================
PUBLIC CONST CHAR* tgt_GetBuildVerNo(VOID);

// =============================================================================
// tgt_GetBuildDateTime
// -----------------------------------------------------------------------------
/// This function is used to access the date time of this build.
// =============================================================================
PUBLIC CONST CHAR* tgt_GetBuildDateTime(VOID);

// =============================================================================
// tgt_GetHwModule
// -----------------------------------------------------------------------------
/// This function is used to access the hardware module names on this target.
// =============================================================================
PUBLIC CONST TGT_HW_MODULE_T* tgt_GetHwModule(VOID);

PUBLIC CONST SX_NB_T* tgt_GetSxNbConfig(VOID);

// =============================================================================
// tgt_GetEncryptedUid
// -----------------------------------------------------------------------------
/// This function is used to get the encrypted UID.
// =============================================================================
PUBLIC UINT32 tgt_GetEncryptedUid(VOID);

// =============================================================================
// tgt_GetBtInfo
// -----------------------------------------------------------------------------
/// This function is used to get the BT information.
// =============================================================================
PUBLIC VOID tgt_GetBtInfo(TGT_BT_INFO_T* btInfo);

// =============================================================================
// tgt_GetWifiInfo
// -----------------------------------------------------------------------------
/// This function is used to get the WIFI information.
// =============================================================================
PUBLIC VOID tgt_GetWifiInfo(TGT_WIFI_INFO_T* wifiInfo);

// =============================================================================
// tgt_GetMobileId
// -----------------------------------------------------------------------------
/// This function is used to access the Mobile ID description.
// =============================================================================
PUBLIC CONST TGT_MOBILE_ID_T* tgt_GetMobileId(VOID);

// =============================================================================
// tgt_GetFactorySettings
// -----------------------------------------------------------------------------
/// This function is used to access the factory settings.
// =============================================================================
PUBLIC TGT_FACTORY_SETTINGS_T* tgt_GetFactorySettings(VOID);

// =============================================================================
// tgt_SetFactorySettings
// -----------------------------------------------------------------------------
/// This function is used to update the new factory settings to flash.
// =============================================================================
PUBLIC BOOL tgt_SetFactorySettings(TGT_FACTORY_SETTINGS_T* factorySetting);

// =============================================================================
// tgt_GetKeyCode
// -----------------------------------------------------------------------------
/// This function returns the code of a key from its column number. It 
/// implements thus the key mapping, between the key detected and the 
/// keyboard of a given target.
/// @param key Key number as given by the HAL Key driver
/// @return The key code as mapped for a specifc target.
// =============================================================================
PUBLIC UINT8 tgt_GetKeyCode(UINT8 key);

// =============================================================================
// tgt_GetKeyMapIndex
// -----------------------------------------------------------------------------
/// This function returns the index of a key in the key map.
/// @param keyCode The key code as mapped for a specifc target.
/// @return The index in the key map.
// =============================================================================
PUBLIC UINT8 tgt_GetKeyMapIndex(UINT8 keyCode);


PUBLIC CONST UINT32 tgt_GetKeyMapCount(VOID);

// =============================================================================
// tgt_GetBootDownloadKeyList
// -----------------------------------------------------------------------------
/// This function returns the list of boot download keys.
/// @param ppList Point to the pointer of the key list.
/// @param pLen Point to the list length.
// =============================================================================
PUBLIC VOID tgt_GetBootDownloadKeyList(CONST UINT8 **ppList, UINT32 *pLen);

// =============================================================================
// tgt_GetBuildVerNo
// -----------------------------------------------------------------------------
/// This function is used to access the version number of this build.
// =============================================================================
PUBLIC CONST CHAR* tgt_GetBuildVerNo(VOID);

// =============================================================================
// tgt_GetBuildDateTime
// -----------------------------------------------------------------------------
/// This function is used to access the date time of this build.
// =============================================================================
PUBLIC CONST CHAR* tgt_GetBuildDateTime(VOID);

// =============================================================================
// tgt_GetHwModule
// -----------------------------------------------------------------------------
/// This function is used to access the hardware module names on this target.
// =============================================================================
PUBLIC CONST TGT_HW_MODULE_T* tgt_GetHwModule(VOID);

// =============================================================================
// tgt_GetMobileBoardSn
// -----------------------------------------------------------------------------
/// This function is used to get the mobile board SN.
// =============================================================================
PUBLIC VOID tgt_GetMobileBoardSn(UINT8 *pSn, UINT8 *pLen);

// =============================================================================
// tgt_GetMobilePhoneSn
// -----------------------------------------------------------------------------
/// This function is used to get the mobile phone SN.
// =============================================================================
PUBLIC VOID tgt_GetMobilePhoneSn(UINT8 *pSn, UINT8 *pLen);

// =============================================================================
// tgt_RegisterAllModules
// -----------------------------------------------------------------------------
/// This function call the register functions for each modules of platform.
// =============================================================================
PUBLIC VOID tgt_RegisterAllModules(VOID);



#endif // _TGT_H_
