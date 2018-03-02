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

#ifndef _CBIT_H_
#define _CBIT_H_

#include "usbmscp.h"

//*****************************************************************************
// D E F I N E S
//*****************************************************************************

#define MAX_CBIT_STALL_COUNT 3

#define CBIT_COMMAND_COMPLETION_INTERRUPT   0x00

#define CBIT_STATUS_SUCCESS             0x00
#define CBIT_STATUS_FAIL                0x01
#define CBIT_STATUS_PHASE_ERROR         0x02
#define CBIT_STATUS_PERSISTENT_ERROR    0x03

//*****************************************************************************
//
// F U N C T I O N    P R O T O T Y P E S
//
//*****************************************************************************

uint32 CBIT_DataTransfer(PUSBMSC_DEVICE pUsbDevice, PTRANSPORT_COMMAND pCommand, PTRANSPORT_DATA pData, BOOL Direction);

#endif // _CBIT_H_

// EOF
