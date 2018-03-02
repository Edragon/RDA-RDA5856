//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name: 

        BOT.H

Abstract:

        Bulk-Only Transport
        USB Revison 1.0, Sept. 31, 1999

--*/

#ifndef _BOT_H_
#define _BOT_H_

#include "usbmscp.h"

//*****************************************************************************
// D E F I N E S
//*****************************************************************************

#define MAX_BOT_STALL_COUNT 2

#define MAX_CBWCB_SIZE 16

// Command Block Wrapper Signature 'USBC'
#define CBW_SIGNATURE               0x43425355
#define CBW_FLAGS_DATA_IN           0x80
#define CBW_FLAGS_DATA_OUT          0x00

//
// Command Status Wrapper Signature 'USBS'
//
#define CSW_SIGNATURE               0x53425355
#define CSW_STATUS_GOOD             0x00
#define CSW_STATUS_FAILED           0x01
#define CSW_STATUS_PHASE_ERROR      0x02


//*****************************************************************************
// T Y P E D E F S
//*****************************************************************************

//
// Command Block Wrapper
//
typedef struct _CBW
{
    uint32   dCBWSignature; 
    uint32   dCBWTag;    
    uint32   dCBWDataTransferLength; 
    uint8   bmCBWFlags; 
    uint8   bCBWLUN; 
    uint8   bCBWCBLength; 
    uint8   CBWCB[MAX_CBWCB_SIZE]; 
} __attribute__((packed))CBW, *PCBW;

//
// Command Status Wrapper
//
typedef struct _CSW
{
    uint32   dCSWSignature; 
    uint32   dCSWTag; 
    uint32   dCSWDataResidue; 
    uint8   bCSWStatus; 
}__attribute__((packed))CSW, *PCSW;


//*****************************************************************************
//
// F U N C T I O N    P R O T O T Y P E S
//
// Direction, TRUE = Data-In, else Data-Out
//*****************************************************************************
//
uint32 BOT_DataTransfer(PUSBMSC_DEVICE pUsbDevice, PTRANSPORT_COMMAND pCommand, PTRANSPORT_DATA pData, BOOL Direction );

uint32 BOT_GetMaxLUN(PUSBMSC_DEVICE pUsbDevice, uint8* pLun);

#endif // _BOT_H_

// EOF
