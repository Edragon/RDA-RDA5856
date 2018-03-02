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
//
/// @file umss_transportboscsi_m.c
///
/// Transport Bulk Only function
//
////////////////////////////////////////////////////////////////////////////////


// =============================================================================
// HEADERS                                                                      
// =============================================================================

#include "csw.h"
#include "umss_callback.h"
#include "hal_host.h"

//typedef HAL_USB_CALLBACK_EP_TYPE_T MsgBody_t;
//#include "itf_msg.h"

#include "hal_usb.h"
#include "sxr_ops.h"
#include "sxs_io.h"
#include "uctls_m.h"


#include <string.h>

#include "umssp_debug.h"
#include "event.h"
#include "gpio_edrv.h"
// =============================================================================
// MACROS                                                                       
// =============================================================================

#define CBW_SIGNATURE 0x43425355
#define CSW_SIGNATURE 0x53425355

#define UMSS_MAX_PART               2

// =============================================================================
// TYPES                                                                        
// =============================================================================

typedef struct
{
    UINT32                   signature;
    UINT32                   tag;
    UINT32                   dataLength;
    UINT8                    flags;
    UINT8                    lun;
    UINT8                    cbLength;
    UINT8                    cb[16];
} __attribute__((packed)) UMSS_BO_COMMAND_BLOCK_WRAPPER_T;

typedef struct
{
    UINT32                   signature;
    UINT32                   tag;
    UINT32                   dataResidue;
    UINT8                    status;
} __attribute__((packed)) UMSS_BO_COMMAND_STATUS_WRAPPER_T;

// =============================================================================
// UMSS_SCSI_CMD_T
// -----------------------------------------------------------------------------
/// SCSI command
// =============================================================================
typedef enum
{
    SCSI_CMD_FORMAT_UNIT                  = 0x04,
    SCSI_CMD_INQUIRY                      = 0x12,
    SCSI_CMD_MODE_SELECT_6                = 0x15,
    SCSI_CMD_MODE_SELECT_10               = 0x55,
    SCSI_CMD_MODE_SENSE_6                 = 0x1a,
    SCSI_CMD_MODE_SENSE_10                = 0x5a,
    SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL = 0x1e,
    SCSI_CMD_READ_6                       = 0x08,
    SCSI_CMD_READ_10                      = 0x28,
    SCSI_CMD_READ_12                      = 0xa8,
    SCSI_CMD_READ_CAPACITY                = 0x25,
    SCSI_CMD_READ_FORMAT_CAPACITIES       = 0x23,
    SCSI_CMD_RELEASE                      = 0x17,
    SCSI_CMD_REQUEST_SENSE                = 0x03,
    SCSI_CMD_RESERVE                      = 0x16,
    SCSI_CMD_SEND_DIAGNOSTIC              = 0x1d,
    SCSI_CMD_START_STOP_UNIT              = 0x1b,
    SCSI_CMD_SYNCHRONIZE_CACHE            = 0x35,
    SCSI_CMD_TEST_UNIT_READY              = 0x00,
    SCSI_CMD_VERIFY                       = 0x2f,
    SCSI_CMD_WRITE_6                      = 0x0a,
    SCSI_CMD_WRITE_10                     = 0x2a,
    SCSI_CMD_WRITE_12                     = 0xaa
} UMSS_SCSI_CMD_T ;

// =============================================================================
// UMSS_BO_TRANSPORT_STATE_T
// -----------------------------------------------------------------------------
/// State of bulkonly transport
// =============================================================================
typedef enum
{
    UMSS_BO_TRANSPORT_STATE_IDLE,
    UMSS_BO_TRANSPORT_STATE_DATA,
    UMSS_BO_TRANSPORT_STATE_DATA_END,
    UMSS_BO_TRANSPORT_STATE_STATUS
} UMSS_BO_TRANSPORT_STATE_T ;

// =============================================================================
// UMSS_BO_TRANSPORT_STATUS_T
// -----------------------------------------------------------------------------
/// Status for CSW
// =============================================================================
typedef enum
{
    UMSS_BO_TRANSPORT_STATUS_PASSED       = 0x00,
    UMSS_BO_TRANSPORT_STATUS_FAILED       = 0x01,
    UMSS_BO_TRANSPORT_STATUS_PHASE        = 0x02,
} UMSS_BO_TRANSPORT_STATUS_T ;

// =============================================================================
// SCSI_REPLY_READ_CAPACITY_T
// -----------------------------------------------------------------------------
/// Read capacity data structure
// =============================================================================
typedef struct
{
    UINT32 nbBlock;
    UINT32 blockSize;
} __attribute__((packed)) SCSI_REPLY_READ_CAPACITY_T ;

// =============================================================================
// SCSI_REPLY_READ_FORMAT_CAPACITY_T
// -----------------------------------------------------------------------------
/// Read format capacity data structure
// =============================================================================
typedef struct
{
    UINT8  reserved1;
    UINT8  reserved2;
    UINT8  reserved3;
    UINT8  length;
    UINT32 nbBlock;
    UINT32 blockSize;
} __attribute__((packed)) SCSI_REPLY_READ_FORMAT_CAPACITY_T ;

// =============================================================================
// SCSI_CMD_SENSE_T
// -----------------------------------------------------------------------------
/// Structure of sense command
// =============================================================================
typedef struct
{
    UINT8 opCode;
    UINT8 lun      :3;
    UINT8 reserved :1;
    UINT8 dbd      :1;
    UINT8 reserved2:3;
    UINT8 pageCtl  :2;
    UINT8 pageCode :6;
    UINT8 reserved3;
    UINT8 length;
    UINT8 control;
} __attribute__((packed)) SCSI_CMD_SENSE_T ;

// =============================================================================
// SCSI_CMD_READ_10_T
// -----------------------------------------------------------------------------
/// Structure of read 10 command
// =============================================================================
typedef struct
{
    UINT8  opCode;
    UINT8  lun      :3;
    UINT8  dpo      :1;
    UINT8  fua      :1;
    UINT8  reserved1:2;
    UINT8  reladr   :1;
    UINT32 lba;
    UINT8  reserved2;
    UINT16 length;
    UINT8  control;
} __attribute__((packed)) SCSI_CMD_READ_10_T ;

// =============================================================================
// SCSI_CMD_READ_6_T
// -----------------------------------------------------------------------------
/// Structure of read 6 command
// =============================================================================
typedef struct
{
    UINT8  opCode;
    UINT8  lun      :3;
    UINT8  reserved1:5;
    UINT16 lba;
    UINT8  length;
    UINT8  control;
} __attribute__((packed)) SCSI_CMD_READ_6_T ;

// =============================================================================
// SCSI_CMD_WRITE_10_T
// -----------------------------------------------------------------------------
/// Structure of write 10 command
// =============================================================================
typedef struct
{
    UINT8  opCode;
    UINT8  lun      :3;
    UINT8  dpo      :1;
    UINT8  fua      :1;
    UINT8  reserved1:2;
    UINT8  reladr   :1;
    UINT32 lba;
    UINT8  reserved2;
    UINT16 length;
    UINT8  control;
} __attribute__((packed)) SCSI_CMD_WRITE_10_T ;

// =============================================================================
// SCSI_CDB_T
// -----------------------------------------------------------------------------
/// Structure of generique cmd
// =============================================================================
typedef struct
{
    UINT8  opCode;
    UINT8  lun      :3;
    UINT8  reserved1:5;
    UINT8  reserved2[2];
    UINT16 length;
    UINT8  control;
}  __attribute__((packed)) SCSI_CDB_T;

// =============================================================================
// SCSI_REPLY_REQUEST_SENSE_T
// -----------------------------------------------------------------------------
/// Structure of request sens reply
// =============================================================================
typedef struct
{
    UINT8  errorCode:7;
    UINT8  valid    :1;
    UINT8  reserved1;
    UINT8  senseKey :4;
    UINT8  reserved2:4;
    UINT32 information;
    UINT8  additionalSenseLength;
    UINT32 reserved3;
    UINT8  additionalSenseCode;
    UINT8  additionalSenseCodeQualifier;
    UINT32 reserved4;
}  __attribute__((packed)) SCSI_REPLY_REQUEST_SENSE_T;

// =============================================================================
// UMSS_BO_TRANSPORT_LUN_T
// -----------------------------------------------------------------------------
/// Description of LUN variable
// =============================================================================
typedef struct
{
    CONST UMSS_STORAGE_CALLBACK_T* callback      ;
    VOID*                          handler       ;
    UINT8                          senseKey      ;
    UINT8                          senseCode     ;
    UINT8                          senseQualifier;
    UINT32                         pos;
} UMSS_BO_TRANSPORT_LUN_T ;

#define USB_BULK_BUFF_SIZE      (512)

// =============================================================================
// PRIVATE VARIABLES                                                            
// =============================================================================

PRIVATE UMSS_BO_TRANSPORT_STATUS_T   g_umssBoTransportStatus;

PRIVATE UINT32                       g_umssBoTag;
PRIVATE UINT32                       g_umssBoResidue;

PRIVATE INT32                        g_umssBoNbLun
__attribute__ ((aligned (4)))                       = 0;


PRIVATE UINT8                        g_umssBoInEp;
PRIVATE UINT8                        g_umssBoOutEp;

PRIVATE UINT8                        *g_UsbBulkOut;
PRIVATE UINT8                        *g_UsbBulkIn;

PRIVATE UMSS_BO_TRANSPORT_STATE_T    g_umssBoState;

PRIVATE UMSS_BO_TRANSPORT_LUN_T      *g_umssBoLun;

PRIVATE UINT16                       g_umssScsiDataSize;
PRIVATE UINT16                       g_umssScsiLun;

PRIVATE UINT8						 g_StopUnit_flag = 0;

// =============================================================================
// PRIVATE FUNCTIONS                                                            
// =============================================================================

extern UINT32 gpio_detect_tcard();

PRIVATE UINT16 scsiInquiry(VOID)
{
    g_UsbBulkIn[ 0] = 0x00; // Direct access
    g_UsbBulkIn[ 1] = 0x80; // Removable
    g_UsbBulkIn[ 2] = 0x00;
    g_UsbBulkIn[ 3] = 0x01; // SCSI 1
    g_UsbBulkIn[ 4] = 0x1f; // Additionnal length
    g_UsbBulkIn[ 5] = 0x00;
    g_UsbBulkIn[ 6] = 0x00;
    g_UsbBulkIn[ 7] = 0x00;

    g_UsbBulkIn[ 8] = 'R';
    g_UsbBulkIn[ 9] = 'D';
    g_UsbBulkIn[10] = 'A';
    g_UsbBulkIn[11] = ' ';
    g_UsbBulkIn[12] = 'U';
    g_UsbBulkIn[13] = 'M';
    g_UsbBulkIn[14] = 'S';
    g_UsbBulkIn[15] = 'S';

    g_UsbBulkIn[16] = ' ';
    g_UsbBulkIn[17] = ' ';
    g_UsbBulkIn[18] = ' ';
    g_UsbBulkIn[19] = ' ';
    g_UsbBulkIn[20] = ' ';
    g_UsbBulkIn[21] = ' ';
    g_UsbBulkIn[22] = ' ';
    g_UsbBulkIn[23] = ' ';
    g_UsbBulkIn[24] = ' ';
    g_UsbBulkIn[25] = ' ';
    g_UsbBulkIn[26] = ' ';
    g_UsbBulkIn[27] = ' ';
    g_UsbBulkIn[28] = ' ';
    g_UsbBulkIn[29] = ' ';
    g_UsbBulkIn[30] = ' ';
    g_UsbBulkIn[31] = ' ';

    g_UsbBulkIn[32] = '1';
    g_UsbBulkIn[33] = '.';
    g_UsbBulkIn[34] = '0';
    g_UsbBulkIn[35] = '0';

    return 36;
}


PRIVATE BOOL scsiStorageReady(UINT8* cmd)
{
    SCSI_CDB_T*                 cdb;
	
    cdb  = (SCSI_CDB_T*) cmd;

	if(0 == g_StopUnit_flag)
	{
		//hal_HstSendEvent(USB_EVENT,0x08120001);
	    //hal_HstSendEvent(USB_EVENT,cdb->lun);
	    //hal_HstSendEvent(USB_EVENT,g_umssBoLun[cdb->lun].handler);
	    if(g_umssBoLun[cdb->lun].callback)
	    {
	        if(g_umssBoLun[cdb->lun].callback->present(
	               g_umssBoLun[cdb->lun].handler))
	        {
	            return(TRUE);
	        }
	    }
	}

    hal_HstSendEvent(USB_EVENT,0x08120003);
    g_umssBoLun[cdb->lun].senseKey        = 0x02;
    g_umssBoLun[cdb->lun].senseCode       = 0x3a;
    g_umssBoLun[cdb->lun].senseQualifier  = 0x00;

    g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
    return(FALSE);
}


PRIVATE UINT16 scsiRequestSense(UINT8* cmd)
{
    SCSI_REPLY_REQUEST_SENSE_T* sens;
    SCSI_CDB_T*                 cdb;

    cdb  = (SCSI_CDB_T*) cmd;
    sens = (SCSI_REPLY_REQUEST_SENSE_T*) g_UsbBulkIn;

    memset(sens, 0, sizeof(SCSI_REPLY_REQUEST_SENSE_T));

    if(gpio_detect_tcard() != GPIO_DETECTED || (1 == g_StopUnit_flag))//NO SD card
    {
        sens->errorCode                      = 0x70;
        sens->senseKey                       = 0x02;
        sens->additionalSenseLength          = 0x0a;
        sens->additionalSenseCode            = 0x3a;
        sens->additionalSenseCodeQualifier   = 0; 
    }
    else//SD card is detected
    {
        sens->errorCode                      = 0x70;
        sens->senseKey                       = g_umssBoLun[cdb->lun].senseKey;
        sens->additionalSenseLength          = 0x0a;
        sens->additionalSenseCode            = g_umssBoLun[cdb->lun].senseCode;
        sens->additionalSenseCodeQualifier   = g_umssBoLun[cdb->lun].senseQualifier;
    }
    g_umssBoLun[cdb->lun].senseKey       = 0x00;
    g_umssBoLun[cdb->lun].senseCode      = 0x00;
    g_umssBoLun[cdb->lun].senseQualifier = 0x00;

    return(sizeof(SCSI_REPLY_REQUEST_SENSE_T));
}

PRIVATE UINT16 scsiRead(UINT8 lun, UINT32 lba, INT32 length, UINT32 rel)
{
    UMSS_STORAGE_SIZE_T*                size;
    UINT32                              i;
    UINT32                              pos;
    //extern UINT8*g_usbTemp;
	
    g_umssScsiLun = lun;

    //hal_HstSendEvent(USB_EVENT,0xab000008);
    //hal_HstSendEvent(USB_EVENT,lun);
    //memset(g_usbTemp, 0, 4096);
    size = g_umssBoLun[lun].callback->getsize(g_umssBoLun[lun].handler);

    if(rel)
    {
        pos = g_umssBoLun[lun].pos + lba;
    }
    else
    {
        pos = lba;
    }

 //   hal_HstSendEvent(USB_EVENT, 0x10170001);
  //  hal_HstSendEvent(USB_EVENT, size->nbBlock);

    if(pos+length-1 < size->nbBlock)
    {
   //     hal_HstSendEvent(USB_EVENT, 0x10170002);
    
        g_umssScsiDataSize = length;
        i                  = USB_BULK_BUFF_SIZE/size->sizeBlock;
        if(i < length)
        {
            length = i;
        }
        length = g_umssBoLun[lun].callback->read(
            g_umssBoLun[lun].handler, g_UsbBulkIn, pos, length);

	 //sxr_Sleep(100);
	 //memcpy(g_UsbBulkIn, g_usbTemp, length*size->sizeBlock);
	 
	 //hal_HstSendEvent(USB_EVENT,0x04260001);
	 //hal_HstSendEvent(USB_EVENT,pos);
	 //hal_HstSendEvent(USB_EVENT,g_UsbBulkIn);
	 //hal_HstSendEvent(USB_EVENT,g_usbTemp);
	 //if(pos == 0x65)
	 	//while(1);
	
        if(length < 0)
        {
            // Read error
            if(length == -2)
            {
                g_umssBoLun[g_umssScsiLun].senseKey        = 0x02;
                g_umssBoLun[g_umssScsiLun].senseCode       = 0x04;
                g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
            }
            else
            {
                g_umssBoLun[g_umssScsiLun].senseKey        = 0x03;
                g_umssBoLun[g_umssScsiLun].senseCode       = 0x11;
                g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
            }
            g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
            return(0);
        }
        g_umssBoLun[lun].pos  = pos + length;
        g_umssScsiDataSize   -= length;
        return(length*size->sizeBlock);
    }
    // Out of range
    g_umssBoLun[lun].senseKey        = 0x05;
    g_umssBoLun[lun].senseCode       = 0x21;
    g_umssBoLun[lun].senseQualifier  = 0x00;
    g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;

    return(0);
}

PRIVATE UINT16 scsiRead10(UINT8* cmd)
{
    SCSI_CMD_READ_10_T*                 read;
    UINT32                              lba;
    INT32                              length;

    read = (SCSI_CMD_READ_10_T*) cmd;


    length = HAL_ENDIAN_BIG_16(read->length);
    lba    = HAL_ENDIAN_BIG_32(read->lba);

    UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: READ10 LBA=%i LENGTH=%i", lba, length);
    //hal_HstSendEvent(USB_EVENT,0x04250040);
    //hal_HstSendEvent(USB_EVENT,read->lun);
    //hal_HstSendEvent(USB_EVENT,lba);
    //hal_HstSendEvent(USB_EVENT,length);
    //hal_HstSendEvent(USB_EVENT,read->reladr);
    return scsiRead(read->lun, lba, length, read->reladr);
}

PRIVATE UINT16 scsiWrite10(UINT8* cmd)
{
    SCSI_CMD_WRITE_10_T*                write;
    UINT32                              lba;
    INT16                               length;
    UMSS_STORAGE_SIZE_T*                size;
    UINT32                              i;
    UINT32                              pos;

    write = (SCSI_CMD_WRITE_10_T*) cmd;

    g_umssScsiLun = write->lun;

    size = g_umssBoLun[write->lun].callback->
        getsize(g_umssBoLun[write->lun].handler);

    length = HAL_ENDIAN_BIG_16(write->length);
    lba    = HAL_ENDIAN_BIG_32(write->lba);

    if(write->reladr)
    {
        pos = g_umssBoLun[write->lun].pos + lba;
    }
    else
    {
        pos = lba;
    }

    UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: WRITE LBA=%i LENGTH=%i",
                   pos, length);

    if(pos+length-1 < size->nbBlock)
    {
        size = g_umssBoLun[write->lun].callback->
            getsize(g_umssBoLun[write->lun].handler);

        g_umssBoLun[write->lun].pos = pos;

        g_umssScsiDataSize = length;
        i                  = USB_BULK_BUFF_SIZE/size->sizeBlock;
        if(i < length)
        {
            length = i;
        }
        return(length*size->sizeBlock);
    }
    // Out of range
    g_umssBoLun[write->lun].senseKey        = 0x05;
    g_umssBoLun[write->lun].senseCode       = 0x21;
    g_umssBoLun[write->lun].senseQualifier  = 0x00;
    g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;

    return(0);
}

UINT32 scsiRecv(UINT8* buffer, UINT32 trans)
{
    INT32                              length;
    UMSS_STORAGE_SIZE_T*                size;

    if(g_umssScsiDataSize == 0)
    {
        return(0);
    }

    if(trans == 0)
    {
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: RECV=0 ???");
        return(0);
    }


    size = g_umssBoLun[g_umssScsiLun].callback->
        getsize(g_umssBoLun[g_umssScsiLun].handler);

    length              = trans/size->sizeBlock;
    g_umssScsiDataSize -= length;
	
    length = g_umssBoLun[g_umssScsiLun].callback->write(
        g_umssBoLun[g_umssScsiLun].handler, buffer,
        g_umssBoLun[g_umssScsiLun].pos,
        length);
    if(length < 0)
    {
        if(length == -2)
        {
            g_umssBoLun[g_umssScsiLun].senseKey        = 0x02;
            g_umssBoLun[g_umssScsiLun].senseCode       = 0x04;
            g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
        }
        else
        {
            g_umssBoLun[g_umssScsiLun].senseKey        = 0x03;
            g_umssBoLun[g_umssScsiLun].senseCode       = 0x0C;
            g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
        }
        g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
        return(0);
    }

    if(g_umssScsiDataSize == 0)
    {
        return(0);
    }

    g_umssBoLun[g_umssScsiLun].pos += length;

    length = USB_BULK_BUFF_SIZE/size->sizeBlock;
    if(g_umssScsiDataSize < length)
    {
        length = g_umssScsiDataSize;
    }

    if(hal_UsbRecv(g_umssBoOutEp, buffer, length*size->sizeBlock, 0) == -1)
    {
        UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: Usb Recv failed\n");
    }

    return length*size->sizeBlock;
}

UINT32 scsiSend(UINT8* buffer)
{
    INT32                              length;
    UMSS_STORAGE_SIZE_T*                size;

//	hal_HstSendEvent(USB_EVENT,0xab000001);
	
    size = g_umssBoLun[g_umssScsiLun].callback->
        getsize(g_umssBoLun[g_umssScsiLun].handler);

    if (size == NULL)
    {
        return(0);
    }

    length = USB_BULK_BUFF_SIZE/size->sizeBlock;

    if(g_umssScsiDataSize == 0)
    {
        return(0);
    }

    if(g_umssScsiDataSize < length)
    {
        length = g_umssScsiDataSize;
    }
    g_umssScsiDataSize -= length;

//	hal_HstSendEvent(USB_EVENT,0xab000002);

    length = g_umssBoLun[g_umssScsiLun].callback->read(
        g_umssBoLun[g_umssScsiLun].handler, buffer, 
        g_umssBoLun[g_umssScsiLun].pos, length);
    if(length < 0)
    {
        // Read error
        if(length == -2)
        {
            g_umssBoLun[g_umssScsiLun].senseKey        = 0x02;
            g_umssBoLun[g_umssScsiLun].senseCode       = 0x04;
            g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
        }
        else
        {
            g_umssBoLun[g_umssScsiLun].senseKey        = 0x03;
            g_umssBoLun[g_umssScsiLun].senseCode       = 0x11;
            g_umssBoLun[g_umssScsiLun].senseQualifier  = 0x00;
        }
        g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
        return(0);
    }

    g_umssBoLun[g_umssScsiLun].pos += length;

    return(length*size->sizeBlock);
}

PRIVATE UINT16 scsiReadFormatCapacity(UINT8* cmd, UINT32 dataLength)
{
    SCSI_REPLY_READ_FORMAT_CAPACITY_T*  capacity;
    UMSS_STORAGE_SIZE_T*                size;
    SCSI_CDB_T*                         cdb;

    cdb      = (SCSI_CDB_T*) cmd;
    capacity = (SCSI_REPLY_READ_FORMAT_CAPACITY_T*) g_UsbBulkIn;

    hal_HstSendEvent(USB_EVENT, 0x0904000b);

    size     = g_umssBoLun[cdb->lun].callback->
        getsize(g_umssBoLun[cdb->lun].handler);

    capacity->reserved1  = 0;
    capacity->reserved2  = 0;
    capacity->reserved3  = 0;
    capacity->length     = 0x08; // Size of descriptor
    capacity->nbBlock    = HAL_ENDIAN_BIG_32(size->nbBlock);
    capacity->blockSize  = HAL_ENDIAN_BIG_32(size->sizeBlock);

    if(dataLength < sizeof(SCSI_REPLY_READ_FORMAT_CAPACITY_T))
    {
        return 0;
    }
    
    memset(capacity+1, 0 ,dataLength - sizeof(SCSI_REPLY_READ_FORMAT_CAPACITY_T));

  //  return sizeof(SCSI_REPLY_READ_FORMAT_CAPACITY_T);
    return dataLength;
}

PRIVATE UINT16 scsiSense(UINT8* cmd)
{
    SCSI_CMD_SENSE_T* sens;

    sens = (SCSI_CMD_SENSE_T*) cmd;

    memset(g_UsbBulkIn, 0, sens->length);

    if(sens->pageCode == 0x3F)
    {
        g_UsbBulkIn[4] = 0x08;
        g_UsbBulkIn[5] = 0x0A;
    }
    g_UsbBulkIn[0] = sens->length-1;

    return(sens->length);
}

PRIVATE UINT16 scsiReadCapacity(UINT8* cmd)
{
    SCSI_REPLY_READ_CAPACITY_T* capacity;
    UMSS_STORAGE_SIZE_T*        size;
    SCSI_CDB_T*                 cdb;
	
    cdb      = (SCSI_CDB_T*) cmd;
    capacity = (SCSI_REPLY_READ_CAPACITY_T*) g_UsbBulkIn;

    size     = g_umssBoLun[cdb->lun].callback->
        getsize(g_umssBoLun[cdb->lun].handler);

	//hal_HstSendEvent(USB_EVENT,0x08120004);
	//hal_HstSendEvent(USB_EVENT, size->nbBlock);
	//hal_HstSendEvent(USB_EVENT, size->sizeBlock);
       capacity->nbBlock   = HAL_ENDIAN_BIG_32(size->nbBlock-1);
       capacity->blockSize = HAL_ENDIAN_BIG_32(size->sizeBlock);

    return(8);
}

PRIVATE UINT16 scsiDecode(UINT8* cmd, UINT32 dataLength, UINT8 lun)
{
    UINT16                      size = 0;
    SCSI_CDB_T*                 cdb;

    cdb                = (SCSI_CDB_T*) cmd;
    g_umssScsiDataSize = 0;

    cdb->lun           = lun;

    UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: cmd 0x%02x lun %i\n", cmd[0], cdb->lun);

    hal_HstSendEvent(USB_EVENT,0x09040008);
    //hal_HstSendEvent(USB_EVENT,cmd);
    //hal_HstSendEvent(USB_EVENT,cmd[0]);
	
    switch(cmd[0])
    {
    case SCSI_CMD_INQUIRY:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: INQUIRY\n");
        size = scsiInquiry();
        if(size > cmd[4])
        {
            size = cmd[4];
        }
        break;
    case SCSI_CMD_TEST_UNIT_READY:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: TEST UNIT\n");
        scsiStorageReady(cmd);
        break;
    case SCSI_CMD_VERIFY:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: TEST UNIT\n");
        scsiStorageReady(cmd);
        break;
    case SCSI_CMD_READ_CAPACITY:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: READ CAPACITY\n");
        if(scsiStorageReady(cmd))
        {
            size = scsiReadCapacity(cmd);
        }
        break;
    case SCSI_CMD_MODE_SENSE_6:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: MODE SENSE 6\n");
        if(scsiStorageReady(cmd))
        {
            size = scsiSense(cmd);
	     //hal_HstSendEvent(USB_EVENT,0x04250050);
	     //hal_HstSendEvent(USB_EVENT,size);
        }
        break;
    case SCSI_CMD_READ_10:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: READ 10\n");
        if(scsiStorageReady(cmd))
        {
            size = scsiRead10(cmd);
		//hal_HstSendEvent(USB_EVENT,0x99999999);	
		//hal_HstSendEvent(USB_EVENT,g_UsbBulkIn);
        }
        break;
    case SCSI_CMD_WRITE_10:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: WRITE 10\n");
        if(scsiStorageReady(cmd))
        {
            size = scsiWrite10(cmd);
        }
        break;
    case SCSI_CMD_READ_FORMAT_CAPACITIES:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: READ FORMAT CAPACITY\n");
        if(scsiStorageReady(cmd))
        {
            hal_HstSendEvent(USB_EVENT,0x09040009);
            size = scsiReadFormatCapacity(cmd, dataLength);
            hal_HstSendEvent(USB_EVENT,0x0904000a);
        }
        break;
    case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: MEDIUM REMOVAL\n");
        scsiStorageReady(cmd);
        // Add flush cache
        break;
    case SCSI_CMD_REQUEST_SENSE:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: REQUEST SENSE\n");
        size = scsiRequestSense(cmd);
        break;
	case SCSI_CMD_START_STOP_UNIT:
		UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: STOP UINT\n");
		scsiStorageReady(cmd);
		g_StopUnit_flag = 1;
		break;
    default:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI: ????\n");
        // Unsupported command
        g_umssBoLun[cdb->lun].senseKey        = 0x05;
        g_umssBoLun[cdb->lun].senseCode       = 0x20;
        g_umssBoLun[cdb->lun].senseQualifier  = 0x00;
        g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
        break;
    }
	
   // hal_HstSendEvent(USB_EVENT,0x04250003);
   // hal_HstSendEvent(USB_EVENT,cmd[0]);
   // hal_HstSendEvent(USB_EVENT,size);
    return(size);
}

void boSendCsw(void)
{
    UMSS_BO_COMMAND_STATUS_WRAPPER_T* csw;
    //hal_HstSendEvent(USB_EVENT,0x04250010);
    //hal_HstSendEvent(USB_EVENT,g_umssBoTransportStatus);
    csw = (UMSS_BO_COMMAND_STATUS_WRAPPER_T*) g_UsbBulkIn;

    csw->signature   = HAL_ENDIAN_LITTLE_32(CSW_SIGNATURE);
    csw->tag         = g_umssBoTag;
    csw->dataResidue = HAL_ENDIAN_LITTLE_32(g_umssBoResidue);
    csw->status      = g_umssBoTransportStatus;

    g_umssBoState          = UMSS_BO_TRANSPORT_STATE_STATUS;

    hal_UsbSend(g_umssBoInEp, g_UsbBulkIn,
                sizeof(UMSS_BO_COMMAND_STATUS_WRAPPER_T), 1);
}

PRIVATE HAL_USB_CALLBACK_RETURN_T
mssCallback(HAL_USB_CALLBACK_EP_TYPE_T   type,
            HAL_USB_SETUP_T*             setup)
{
    UINT8 i;
//	hal_HstSendEvent(USB_EVENT,0x08100002);
//	hal_HstSendEvent(USB_EVENT,type);
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_CMD:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Cmd\n");
        if(setup->request == 0xFE)
        {
            UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Cmd Get LUN %i\n", g_umssBoNbLun);
#ifndef DONGLE_HOST
            hal_HstSendEvent(USB_EVENT,0x13041001);
            hal_UsbSend(0, (UINT8*)&g_umssBoNbLun, 1, 0);
#endif
            for(i = 0; i < UMSS_MAX_PART; ++i)
            {
                g_umssBoLun[i].pos      = 0;
            }
            return(HAL_USB_CALLBACK_RETURN_OK);
        }
        break;
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Receive end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Transmit end\n");
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Enable\n");
        hal_UsbRecv(g_umssBoOutEp, g_UsbBulkOut, (UINT16)USB_BULK_BUFF_SIZE, 0);
        g_umssBoState = UMSS_BO_TRANSPORT_STATE_IDLE;
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_KO);
}


PRIVATE void mssSendEvent(HAL_USB_CALLBACK_EP_TYPE_T type)
{

    COS_EVENT ev;
    ev.nEventId = EV_USB_MSS_EVENT_ID;
    ev.nParam1 = type;
    COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}


PRIVATE HAL_USB_CALLBACK_RETURN_T mssCallbackEp(HAL_USB_CALLBACK_EP_TYPE_T type, HAL_USB_SETUP_T* setup)
{
	//hal_HstSendEvent(USB_EVENT,0xb1000000);
	//hal_HstSendEvent(USB_EVENT,type);
	
    switch(type)
    {
    case HAL_USB_CALLBACK_TYPE_DATA_CMD:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Cmd data\n");
        break;
    case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Receive end\n");
        mssSendEvent(HAL_USB_CALLBACK_TYPE_RECEIVE_END);
        //g_umssBoMsg.B = HAL_USB_CALLBACK_TYPE_RECEIVE_END;
        //sxr_Send(&g_umssBoMsg, g_umssBoMbx, SXR_SEND_MSG);
        break;
    case HAL_USB_CALLBACK_TYPE_CMD:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Cmd\n");
    case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Transmit end\n");
        mssSendEvent(HAL_USB_CALLBACK_TYPE_TRANSMIT_END);
        //g_umssBoMsg.B = HAL_USB_CALLBACK_TYPE_TRANSMIT_END;
        //sxr_Send(&g_umssBoMsg, g_umssBoMbx, SXR_SEND_MSG);
        break;
    case HAL_USB_CALLBACK_TYPE_ENABLE:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Enable\n");
        break;
    case HAL_USB_CALLBACK_TYPE_DISABLE:
        UMSS_TRACE(UMSS_INFO_TRC, 0, "--- Usb EP Callback Disable\n");
        break;
    }
    return(HAL_USB_CALLBACK_RETURN_OK);
}

UINT8* cmd;
static UINT8 umss_virtual_plugout = 0;
static UINT8 umss_check_timer_id = 0;

UINT8 umss_GetUsbVirtualStatus()
{
    return umss_virtual_plugout;
}

UINT8 umss_ResetUsbVirtualStatus()
{
    umss_virtual_plugout = 0;
    return 0;
}
static void umss_CheckUsbVirtualStatus()
{
    if(umss_virtual_plugout != 0)
    {
        hal_DbgAssert("Error in umss virtual status control...");
    }
    umss_virtual_plugout = 1;
}

PUBLIC VOID umss_TransportEventProcess(UINT32 nParam1)
{
    UINT32                           i;
//    Msg_t*                           msg;
  //  COS_EVENT ev;
    UMSS_BO_COMMAND_BLOCK_WRAPPER_T* cbw;
    UINT16                           size;
 //   UINT32                           evt[4];

    cbw = (UMSS_BO_COMMAND_BLOCK_WRAPPER_T*) HAL_SYS_GET_UNCACHED_ADDR(g_UsbBulkOut);
	//hal_HstSendEvent(USB_EVENT,0x08110001);
//	hal_HstSendEvent(USB_EVENT,cbw);
//	hal_HstSendEvent(USB_EVENT, nParam1);	
//	hal_HstSendEvent(USB_EVENT, g_umssBoState);

    cmd = cbw->cb; //TEST_NO_SDCARD

    if(umss_check_timer_id == 0)//start umss plug-in-out timer
    {
        umss_check_timer_id = COS_SetTimer(1500, umss_CheckUsbVirtualStatus, NULL, COS_TIMER_MODE_SINGLE);
    }
    else
    {
        COS_ChangeTimer(umss_check_timer_id, 1500);
    }
    
    //param = param;

    //while(1)
    {
        //COS_WaitEvent(MOD_SYS, &ev, COS_WAIT_FOREVER);
        //msg = sxr_Wait(evt, g_umssBoMbx);
        //if(msg == NULL){
        //    UMSS_TRACE(UMSS_INFO_TRC, 0, "unknown evt:%x:%x:%x:%x\n",evt[0],evt[1],evt[2],evt[3]);
        //    continue;
        //}
        switch(nParam1)
        {
        case HAL_USB_CALLBACK_TYPE_RECEIVE_END:
		//hal_HstSendEvent(USB_EVENT,0xa1b00004);
		//hal_HstSendEvent(USB_EVENT,g_umssBoState);
            switch(g_umssBoState)
            {
            case UMSS_BO_TRANSPORT_STATE_IDLE:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: IDLE\n");
                if(HAL_ENDIAN_LITTLE_32(cbw->signature) == CBW_SIGNATURE)
                {
                    if(0) //gpio_detect_tcard() != GPIO_DETECTED)//NO SD card
                    {                           
                            if(cmd[0] == SCSI_CMD_READ_FORMAT_CAPACITIES||
                                cmd[0] == SCSI_CMD_READ_CAPACITY||
                              //  cmd[0] == 0x0c||
                                cmd[0] == SCSI_CMD_READ_10)
                            {
                                hal_UsbSend(g_umssBoInEp, g_UsbBulkIn, 0, 0);
                                g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
                                g_umssBoResidue = 0;
                                g_umssBoTag     = cbw->tag;
                                g_umssBoState         = UMSS_BO_TRANSPORT_STATE_DATA;
                                break;
                            }
                            else if(cmd[0] == SCSI_CMD_MODE_SENSE_6)
                            {
                                hal_UsbSend(g_umssBoInEp, g_UsbBulkIn, 0, 0);
                                g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
                                g_umssBoResidue = cbw->dataLength;
                                g_umssBoTag     = cbw->tag;
                                g_umssBoState         = UMSS_BO_TRANSPORT_STATE_DATA;
                                break;
                            }else if(cmd[0] == SCSI_CMD_TEST_UNIT_READY)
                            {
                                g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_FAILED;
                                g_umssBoResidue = 0;
                                g_umssBoTag     = cbw->tag;
                                g_umssBoState         = UMSS_BO_TRANSPORT_STATE_DATA;
                                boSendCsw();
                                break;
                            }
                    }                            
                    
                    g_umssBoTransportStatus = UMSS_BO_TRANSPORT_STATUS_PASSED;
                    UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: CBW recv %i\n", cbw->lun);
                    cbw->dataLength = HAL_ENDIAN_LITTLE_32(cbw->dataLength);
                    size            = scsiDecode(cbw->cb, cbw->dataLength,
                                                 cbw->lun);
#ifdef DONGLE_HOST                                                 
                    UINT8* cmd_dongle = cbw->cb;
#endif                    
                    
                                                 
                    //hal_HstSendEvent(USB_EVENT,0x09040007);
	             //hal_HstSendEvent(USB_EVENT,cbw->dataLength); 
	             //hal_HstSendEvent(USB_EVENT, size);
			        
                    g_umssBoTag     = cbw->tag;
                    g_umssBoState         = UMSS_BO_TRANSPORT_STATE_DATA;

                    if(cbw->dataLength == 0)
                    {
                        g_umssBoResidue = 0;
                        boSendCsw();
                    }
                    else
                    {
                    	   //hal_HstSendEvent(USB_EVENT,0x04250001);
                    	   //hal_HstSendEvent(USB_EVENT,g_umssBoTransportStatus);
                        if(g_umssBoTransportStatus == UMSS_BO_TRANSPORT_STATUS_PASSED)
                        {
                            if(cbw->dataLength > size && cbw->cb[0] != 0x12)
                            {
                                g_umssBoResidue = cbw->dataLength - size;
                            }
                            else
                            {
                                size            = cbw->dataLength;
                                g_umssBoResidue = 0;
                            }

#ifdef DONGLE_HOST
                            if(cmd_dongle[0] == SCSI_CMD_READ_FORMAT_CAPACITIES)
                            {
                                g_umssBoResidue = 0;
                                hal_HstSendEvent(USB_EVENT,0x12280001);
                            }
#endif
                            if(cbw->flags & 0x80)
                            { // IN
                                
                                hal_UsbSend(g_umssBoInEp, g_UsbBulkIn, size, 1);
                            }
                            else
                            { //OUT
                                hal_UsbRecv(g_umssBoOutEp, g_UsbBulkOut, size, 1);
                            }
                        }
                        else
                        {
                            g_umssBoResidue = 0;
                            if(cbw->flags & 0x80)
                            { // IN
                                hal_UsbEpStall(g_umssBoInEp, TRUE);
                            }
                            else
                            { //OUT
                                hal_UsbEpStall(g_umssBoOutEp, TRUE);
                            }

                        }
                    }
                }
                break;
            case UMSS_BO_TRANSPORT_STATE_DATA:
		  //hal_HstSendEvent(USB_EVENT,0xa1b00005);
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA WRITE res %i\n",g_umssBoResidue);
                i = scsiRecv((UINT8*)HAL_SYS_GET_UNCACHED_ADDR(g_UsbBulkOut),hal_UsbEpRxTransferedSize(g_umssBoOutEp));
                g_umssBoResidue -= i;
                hal_HstSendEvent(USB_EVENT, 0x09200001);
                hal_HstSendEvent(USB_EVENT, g_umssBoResidue);
                hal_HstSendEvent(USB_EVENT, i);
                if(i == 0)
                {
                    if(g_umssBoResidue == 0)
                    {
                        hal_HstSendEvent(USB_EVENT,0x04250002);
                        boSendCsw();
                    }
                    else
                    {
                        UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA WRITE STALL\n");
                        hal_UsbEpStall(g_umssBoOutEp, TRUE);
                        g_umssBoState = UMSS_BO_TRANSPORT_STATE_DATA_END;
                    }
                }
                break;
            case UMSS_BO_TRANSPORT_STATE_DATA_END:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA END\n");
                boSendCsw();
                break;
            case UMSS_BO_TRANSPORT_STATE_STATUS:
                g_umssBoState = UMSS_BO_TRANSPORT_STATE_IDLE;
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: STATUS ??\n");
                break;
            }
            break;

        case HAL_USB_CALLBACK_TYPE_TRANSMIT_END:
			//hal_HstSendEvent(USB_EVENT,0xa1b00006);
			//hal_HstSendEvent(USB_EVENT,g_umssBoState);
			//hal_HstSendEvent(USB_EVENT,g_umssBoResidue);
            switch(g_umssBoState)
            {
            case UMSS_BO_TRANSPORT_STATE_IDLE:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: IDLE\n");
                break;
            case UMSS_BO_TRANSPORT_STATE_DATA:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA READ res %i\n", g_umssBoResidue);

                if(0 )// gpio_detect_tcard() != GPIO_DETECTED)//NO SD card
                {
                    if(cmd[0] == SCSI_CMD_MODE_SENSE_6)
                    {
                        boSendCsw();
                        break;
                    }
                }

                if(g_umssBoResidue != 0)
                {
                    i = scsiSend((UINT8*)HAL_SYS_GET_UNCACHED_ADDR(g_UsbBulkIn));
                    if(i == 0)
                    {
                        UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA READ STALL\n");
                        hal_UsbEpStall(g_umssBoInEp, TRUE);
                        g_umssBoState = UMSS_BO_TRANSPORT_STATE_DATA_END;
                    }
                    else
                    {
                        if(hal_UsbSend(g_umssBoInEp, g_UsbBulkIn, i, 1) == -1)
                        {
                            UMSS_TRACE(UMSS_INFO_TRC, 0,
                                        "BO: DATA READ UsbSend error\n");
                        }
                    }
                    g_umssBoResidue -= i;
                }
                else
                {
                	//hal_HstSendEvent(USB_EVENT,0x04250004);
                     boSendCsw();
                }
                break;
            case UMSS_BO_TRANSPORT_STATE_DATA_END:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: DATA END\n");
                boSendCsw();
                break;
            case UMSS_BO_TRANSPORT_STATE_STATUS:
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: STATUS\n");
                UMSS_TRACE(UMSS_INFO_TRC, 0, "BO: CSW send\n");
                g_umssBoState = UMSS_BO_TRANSPORT_STATE_IDLE;
                hal_UsbRecv(g_umssBoOutEp, g_UsbBulkOut, USB_BULK_BUFF_SIZE, 0);
                break;
            }
            break;
        case HAL_USB_CALLBACK_TYPE_DATA_CMD:
        case HAL_USB_CALLBACK_TYPE_CMD:
        case HAL_USB_CALLBACK_TYPE_ENABLE:
        case HAL_USB_CALLBACK_TYPE_DISABLE:
            break;
        }
    }
}

//UINT8 *g_usbTemp = NULL;

//PRIVATE UINT8 USB_UCBSS_INTERNAL 	g_bulkOut[512];

PUBLIC void umss_Open(void)
{
    HAL_USB_EP_DESCRIPTOR_T** epList;
    UINT8                     epNum;
    //extern uint32 *pcmbuf_overlay;
    //extern uint32 *inputbuf_overlay;

    umss_virtual_plugout = 0;
    umss_check_timer_id = 0;

	g_StopUnit_flag = 0;

    epList = (HAL_USB_EP_DESCRIPTOR_T**)
        COS_Malloc_NoCache(sizeof(HAL_USB_EP_DESCRIPTOR_T*)*3);

    //g_UsbBulkIn = (UINT8*)(inputbuf_overlay + 1280);
    //g_UsbBulkOut =(UINT8*)(inputbuf_overlay + 1280 + USB_BULK_BUFF_SIZE/4);
    //g_umssBoLun = (UINT8*)(inputbuf_overlay + USB_BULK_BUFF_SIZE/2);

	//g_UsbBulkIn = pcmbuf_overlay;
	//g_UsbBulkOut =pcmbuf_overlay + USB_BULK_BUFF_SIZE/4;
      //g_umssBoLun = pcmbuf_overlay + USB_BULK_BUFF_SIZE/2;

    if(!g_UsbBulkIn)
    {
        g_UsbBulkIn = (UINT8*)COS_Malloc_NoCache(USB_BULK_BUFF_SIZE);
    }
     if(!g_UsbBulkOut)
    {
        g_UsbBulkOut =  (UINT8*)COS_Malloc_NoCache(USB_BULK_BUFF_SIZE);
    }
    if(!g_umssBoLun)
    {
        g_umssBoLun = (UMSS_BO_TRANSPORT_LUN_T*)COS_Malloc_NoCache(sizeof(UMSS_BO_TRANSPORT_LUN_T)*UMSS_MAX_PART);
    }  
	//g_usbTemp = (UINT8*)sxr_Malloc(USB_BULK_BUFF_SIZE);

    memset(g_umssBoLun, 0, sizeof(UMSS_BO_TRANSPORT_LUN_T)*UMSS_MAX_PART);
    memset(g_UsbBulkOut, 0, USB_BULK_BUFF_SIZE);
    memset(g_UsbBulkIn, 0, USB_BULK_BUFF_SIZE);

    // tianwq: don't create task, add the process in MOD_SYS task
    /*if(g_umssBoIdTask == 0)
    {
        g_umssBoTask.TaskBody  = umss_TransportBoScsiTask;
        g_umssBoTask.TaskExit  = 0;
        g_umssBoTask.Name      = "UMSS BulkOnly SCSI";
		// changed by binbinguo		
		//g_umssBoTask.StackSize	= 512
		g_umssBoTask.StackSize	= 512 * 4;
		// end by binbinguo
        g_umssBoTask.Priority  = 50;
        g_umssBoMbx            = sxr_NewMailBox();
        g_umssBoIdTask         = sxr_NewTask(&g_umssBoTask);
        sxr_StartTask(g_umssBoIdTask, 0);
    }*/

    /*for(i = 0; i < 8; ++i)
    {
        if(storageCfg[i].storageCallback == 0)
        {
            break;
        }
        g_umssBoLun[i].pos      = 0;
        g_umssBoLun[i].callback = storageCfg[i].storageCallback;
        g_umssBoLun[i].handler  = storageHandler[i];
    }
    g_umssBoNbLun = i-1;*/

	hal_HstSendEvent(USB_EVENT,0x08100004);

	g_umssBoNbLun = -1;

    // allocate ep number
    epNum = uctls_AllocateEndPoint();
    g_umssBoInEp  = HAL_USB_EP_DIRECTION_IN (epNum);
    epNum = uctls_AllocateEndPoint();
    g_umssBoOutEp = HAL_USB_EP_DIRECTION_OUT(epNum);

    epList[0] = uctls_NewEpDescriptor(g_umssBoOutEp, 0, mssCallbackEp, HAL_USB_EP_TYPE_BULK);
    epList[1] = uctls_NewEpDescriptor(g_umssBoInEp , 0, mssCallbackEp, HAL_USB_EP_TYPE_BULK);
    epList[2] = 0;

    UMSS_TRACE(UMSS_INFO_TRC, 0, "SCSI Open\n");

    uctls_AddInterface(uctls_NewInterfaceDescriptor(HAL_USB_CLASS_MSS, 0x06, 0x50, 0x00, epList, 0, mssCallback));
}

PUBLIC  VOID umss_AddPart( UMSS_STORAGE_CALLBACK_T *storageCallback)// shaohuima 201607/08
{
    g_umssBoNbLun++;
    if(g_umssBoNbLun+1 >= UMSS_MAX_PART)
    {
    	hal_HstSendEvent(USB_EVENT,0x08100009);
        return;
    }
    g_umssBoLun[g_umssBoNbLun].pos      = 0;
    g_umssBoLun[g_umssBoNbLun].callback = storageCallback;
    g_umssBoLun[g_umssBoNbLun].handler  = storageCallback->open();
	hal_HstSendEvent(USB_EVENT,0x08100006);
//	hal_HstSendEvent(USB_EVENT,storageCallback);
}
#if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
extern BYTE g_usb_connected;
#endif
PUBLIC VOID umss_Close(VOID)
{
    int i;
    #if(CHIP_ASIC_ID==CHIP_ASIC_ID_5856)
    g_usb_connected=0;
    #endif
    for(i=0;i<g_umssBoNbLun;i++)
    {
        if(g_umssBoLun[i].callback)
        {
            g_umssBoLun[i].callback->close(g_umssBoLun[i].handler);
        }
    }
    // free interface
    // free ep
    if(g_umssBoLun !=NULL)
        {COS_Free(g_umssBoLun);g_umssBoLun=NULL;}
    if(g_UsbBulkOut !=NULL)
        {COS_Free(g_UsbBulkOut);g_UsbBulkOut=NULL;}
    if(g_UsbBulkIn !=NULL)
        {COS_Free(g_UsbBulkIn);g_UsbBulkIn=NULL;}
    g_umssBoNbLun  = -1;
	g_StopUnit_flag = 0;
}

// =============================================================================
// GLOBAL VARIABLES                                                             
// =============================================================================

PUBLIC CONST UCTLS_SERVICE_CALLBACK_T g_umssServiceCallback =
{
    //.open  = umss_TransportBoScsiOpen,
    .close   = umss_Close,
    .process = umss_TransportEventProcess,
};

// =============================================================================
// FUNCTIONS                                                                    
// =============================================================================

