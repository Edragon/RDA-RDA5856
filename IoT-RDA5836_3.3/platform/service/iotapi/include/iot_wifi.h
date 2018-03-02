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

#include "cs_types.h"
#include "event.h"
#include "tcpip_netdb.h"
#include "tcpip_sockets.h"
#include "iot_base.h"

#if !defined(MCU_WIFI_PLAT)

// ============================================================================
// wifi_Open
// ----------------------------------------------------------------------------
/// WIFI module power on
/// 
/// @param VOID
// ============================================================================
VOID wifi_Open(VOID);
// ============================================================================
// wifi_Close
// ----------------------------------------------------------------------------
/// WIFI module power off
/// 
/// @param VOID
// ============================================================================
VOID wifi_Close(VOID);
// ============================================================================
// wifi_GetStatus
// ----------------------------------------------------------------------------
/// Get WIFI connection status
/// 
/// @param VOID
// ============================================================================
UINT8 wifi_GetStatus(VOID);
#endif

// ============================================================================
// iot_Connect
// ----------------------------------------------------------------------------
/// Connect to URL like "www.rda.com" or ip address like "127.0.0.1"
/// @param addr
///
/// @param portno port number
/// @return
// ============================================================================
INT32 iot_Connect(const char *addr, INT16 portno);

// ============================================================================
// iot_Send
// ----------------------------------------------------------------------------
/// Send data to server
///
/// @param VOID
// ============================================================================
INT32 iot_Send(INT32 sockfd, const INT8* buffer, UINT32 len);
// ============================================================================
// iot_Recv
// ----------------------------------------------------------------------------
/// Read data from server
///
/// @param VOID
// ============================================================================
INT32 iot_Recv(INT32 sockfd, INT8 *buffer, UINT32 len);
// ============================================================================
// wifi_GetStatus
// ----------------------------------------------------------------------------
/// Close the tcp socket
///
/// @param VOID
// ============================================================================
INT32 iot_Close(INT32 sockfd);


