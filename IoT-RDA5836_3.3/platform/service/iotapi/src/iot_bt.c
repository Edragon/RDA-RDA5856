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

#include "tgt_app_cfg.h"
#include "cswtype.h"
#include "cos.h"
#include "iot_bt.h"

// =============================================================================
// bt_Open
// -----------------------------------------------------------------------------
/// Open bluetooth stack  
///
/// @return No  
// =============================================================================
u_int8 bt_Open(u_int32 timeout)
{
    return 0;
}

// =============================================================================
// bt_Close
// -----------------------------------------------------------------------------
/// Close bluetooth stack 
///
// =============================================================================
VOID bt_Close(u_int32 timeout)
{
    
}

// =============================================================================
// bt_SetLocalName
// -----------------------------------------------------------------------------
/// Set bluetooth local name 
///
/// @param name Bluetooth local name, "\" terminate string  
/// @return  
// =============================================================================
VOID bt_SetLocalName(UINT8* name)
{

}

// =============================================================================
// bt_SetVisable
// -----------------------------------------------------------------------------
/// Set bluetooth visiable 
///
/// @True: visble, False: invisable 
/// @return  
// =============================================================================
// =============================================================================
// bt_ScanDevice
// -----------------------------------------------------------------------------
/// Bt scan device.
///
/// @param max_device  
/// @return \c >0 means scanned nearby bt, <=0 means error 
// =============================================================================
void bt_ScanDevice(UINT8 max_device) 
{
    
}

// =============================================================================
// bt_BoneDevice
// -----------------------------------------------------------------------------
/// Bt scan device.
///
/// @param timeout
/// @return \c >0 means scanned nearby bt, <=0 means error
// =============================================================================
void bt_BoneDevice(t_bdaddr device_addr, UINT32 timeout)
{

}
// =============================================================================
//  bt_GetBonedDevice
// -----------------------------------------------------------------------------
/// transfer is finished.
///
/// @param index, input 
/// @param device, output  
/// @return \c TRUE, Get device successful False, Get failed.   
// =============================================================================
BOOL  bt_GetBonedDevice()
{
    return TRUE;
}

#if APP_SUPPORT_SPP==1

// =============================================================================
// bt_SppConnect
// -----------------------------------------------------------------------------
/// transfer is finished.
///
/// @param name The transfer configuration.
/// @param timeout The transfer configuration.
/// @return DMA transfer error status: #HAL_ERR_NO.
// =============================================================================
BOOL bt_SppConnect(t_bdaddr device_addr, u_int32 timeout)
{
    return TRUE;
}

// =============================================================================
// bt_SppDisconnec
// -----------------------------------------------------------------------------
/// transfer is finished.
///
// =============================================================================
VOID bt_SppDisconnect(t_bdaddr device_addr, u_int32 timeout)
{
    
}

// =============================================================================
// bt_SppSend
// -----------------------------------------------------------------------------
/// transfer is finished.
///
/// @param buf The transfer configuration.
/// @param size The transfer configuration.
/// @return DMA transfer error status: #HAL_ERR_NO.
// =============================================================================
INT32 bt_SppSend(UINT8* buf, UINT32 size, u_int32 timeout)
{
    return 0;
}

// =============================================================================
// bt_SppRecv
// -----------------------------------------------------------------------------
/// transfer is finished.
///
/// @param timeout The transfer configuration.
/// @param buf The transfer configuration.
/// @return DMA transfer error status: #HAL_ERR_NO.
// =============================================================================
INT32  bt_SppRecv(u_int32 timeout)
{
    return 0;
}

// =============================================================================
// bt_SppAccept
// -----------------------------------------------------------------------------
/// transfer is finished.
///
/// @param timeout The transfer configuration.
/// @return DMA transfer error status: #HAL_ERR_NO.
// =============================================================================
BOOL bt_SppAccept(VOID)
{
    return TRUE;
}

#endif
