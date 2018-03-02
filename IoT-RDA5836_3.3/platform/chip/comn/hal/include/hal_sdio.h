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
/// @file hal_sdio.h                                                           //
/// SDMMC2 HAL implementation
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 
#ifndef _HAL_SDIO_H_
#define _HAL_SDIO_H_

#include "cs_types.h"
#include "hal_error.h"


/// @defgroup sdio HAL SD/IO Driver
/// This document describes the characteristics of the SD/IO module and how 
/// to use them via its Hardware Abstraction Layer API.
/// @{


// =============================================================================
// MACROS
// =============================================================================

// =============================================================================
// HAL_SDIO_ACMD_SEL
// -----------------------------------------------------------------------------
/// Macro to mark a command as application specific
// =============================================================================
#define HAL_SDIO_ACMD_SEL         0x80000000


// =============================================================================
// HAL_SDIO_CMD_MASK
// -----------------------------------------------------------------------------
/// Mask to get from a HAL_SDIO_CMD_T value the corresponding 
/// command index
// =============================================================================
#define HAL_SDIO_CMD_MASK 0x3F

typedef struct
{
	uint32		opcode;
	uint32		arg;
	uint32		resp[4];
	uint32		flags;
	uint32		error;
}mmc_command;
typedef struct
{
	uint32		blksz;
	uint32		blocks;
	uint32		error;
	uint32		flags;
	uint32		btes_xfered;
	uint8       *buffer;
}mmc_data;
#define MMC_DATA_WRITE	(1 << 8)
#define MMC_DATA_READ	(1 << 9)
#define MMC_DATA_STREAM	(1 << 10)

#define MMC_RSP_PRESENT	(1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */
#define MMC_CMD_MASK	(3 << 5)		/* non-SPI command type */
#define MMC_CMD_AC	(0 << 5)
#define MMC_CMD_ADTC	(1 << 5)
#define MMC_CMD_BC	(2 << 5)
#define MMC_CMD_BCR	(3 << 5)
#define MMC_RSP_SPI_S1	(1 << 7)		/* one status byte */
#define MMC_RSP_SPI_S2	(1 << 8)		/* second byte */
#define MMC_RSP_SPI_B4	(1 << 9)		/* four data bytes */
#define MMC_RSP_SPI_BUSY (1 << 10)		/* card may send busy */
#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1B	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE|MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_SPI_R1	(MMC_RSP_SPI_S1)
#define MMC_RSP_SPI_R1B	(MMC_RSP_SPI_S1|MMC_RSP_SPI_BUSY)
#define MMC_RSP_SPI_R2	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R3	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R4	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_RSP_SPI_R5	(MMC_RSP_SPI_S1|MMC_RSP_SPI_S2)
#define MMC_RSP_SPI_R7	(MMC_RSP_SPI_S1|MMC_RSP_SPI_B4)
#define MMC_CARD_BUSY   0x80000000


// =============================================================================
//  TYPES
// =============================================================================

// =============================================================================
// HAL_SDIO_IRQ_HANDLER_T
// -----------------------------------------------------------------------------
/// Type of the user fonction called when a SDIO interrupt occurs
// =============================================================================
typedef VOID (*HAL_SDIO_IRQ_HANDLER_T)(VOID);


// =============================================================================
// HAL_SDIO_CMD_T
// -----------------------------------------------------------------------------
// SD commands
// =============================================================================
typedef enum
{
    HAL_SDIO_CMD_GO_IDLE_STATE         = 0,
    HAL_SDIO_CMD_IO_SEND_OP_COND      = 1,
    HAL_SDIO_CMD_ALL_SEND_CID            = 2,
    HAL_SDIO_CMD_SEND_RELATIVE_ADDR    = 3,
    HAL_SDIO_CMD_SET_DSR                = 4,
    HAL_SDIO_CMD_SWITCH           = 6,
    HAL_SDIO_CMD_SELECT_CARD            = 7,
    HAL_SDIO_CMD_SEND_IF_COND          = 8,
    HAL_SDIO_CMD_SEND_CSD                = 9,
    HAL_SDIO_CMD_STOP_TRANSMISSION        = 12,
    HAL_SDIO_CMD_SEND_STATUS            = 13,
    HAL_SDIO_CMD_SET_BLOCKLEN            = 16,
    HAL_SDIO_CMD_READ_SINGLE_BLOCK        = 17,
    HAL_SDIO_CMD_READ_MULT_BLOCK        = 18,
    HAL_SDIO_CMD_WRITE_SINGLE_BLOCK    = 24,
    HAL_SDIO_CMD_WRITE_MULT_BLOCK        = 25,
    HAL_SDIO_CMD_IO_RW_DIRECT           = 52,
    HAL_SDIO_CMD_IO_RW_EXTENDED         = 53,
    HAL_SDIO_CMD_APP_CMD                = 55,
    HAL_SDIO_CMD_SET_BUS_WIDTH            = (6 | HAL_SDIO_ACMD_SEL),
    HAL_SDIO_CMD_SEND_NUM_WR_BLOCKS    = (22| HAL_SDIO_ACMD_SEL),
    HAL_SDIO_CMD_SET_WR_BLK_COUNT        = (23| HAL_SDIO_ACMD_SEL),
    HAL_SDIO_CMD_SEND_OP_COND            = (41| HAL_SDIO_ACMD_SEL)
} HAL_SDIO_CMD_T;

// =============================================================================
// HAL_SDIO_OP_STATUS_T
// -----------------------------------------------------------------------------
/// This structure is used the module operation status. It is different from the 
/// IRQ status.
// =============================================================================
typedef union
{
    UINT32 reg;
    struct
    {
        UINT32 operationNotOver     :1;
        /// Module is busy ?
        UINT32 busy                 :1;
        UINT32 dataLineBusy         :1;
        /// '1' means the controller will not perform the new command when SDIO_SENDCMD= '1'.
        UINT32 suspend              :1;
        UINT32                      :4;
        UINT32 responseCrcError     :1;
        /// 1 as long as no reponse to a command has been received.
        UINT32 noResponseReceived   :1;
        UINT32                      :2;
        /// CRC check for SD/IO write operation
        /// "101" transmission error
        /// "010" transmission right
        /// "111" flash programming error
        UINT32 crcStatus            :3;
        UINT32                      :1;
        /// 8 bits data CRC check, "00000000" means no data error, 
        /// "00000001" means DATA0 CRC check error, "10000000" means 
        /// DATA7 CRC check error, each bit match one data line.
        UINT32 dataError            :8;
    } fields;
} HAL_SDIO_OP_STATUS_T;

// =============================================================================
// HAL_SDIO_DIRECTION_T
// -----------------------------------------------------------------------------
/// Describe the direction of a transfer between the SDio controller and a 
/// pluggued card.
// =============================================================================
typedef enum
{
    HAL_SDIO_DIRECTION_READ,
    HAL_SDIO_DIRECTION_WRITE,

    HAL_SDIO_DIRECTION_QTY
} HAL_SDIO_DIRECTION_T;


// =============================================================================
// HAL_SDIO_TRANSFER_T
// -----------------------------------------------------------------------------
/// Describe a transfer between the SDio module and the SD card
// =============================================================================
typedef struct
{
    /// This address in the system memory
    UINT8* sysMemAddr;
    /// Address in the SD card
    UINT8* sdCardAddr;
    /// Quantity of data to transfer, in blocks
    UINT32 blockNum;
    /// Block size
    UINT32 blockSize;
    HAL_SDIO_DIRECTION_T direction;
} HAL_SDIO_TRANSFER_T;


// =============================================================================
// HAL_SDIO_DATA_BUS_WIDTH_T
// -----------------------------------------------------------------------------
/// Cf spec v2 pdf p. 76 for ACMD6 argument
/// That type is used to describe how many data lines are used to transfer data
/// to and from the SD card.
// =============================================================================
typedef enum
{
    HAL_SDIO_DATA_BUS_WIDTH_1 = 0x0,
    HAL_SDIO_DATA_BUS_WIDTH_4 = 0x2
} HAL_SDIO_DATA_BUS_WIDTH_T;


// =============================================================================
//  FUNCTIONS
// =============================================================================

//=============================================================================
// hal_SdioOpen
//-----------------------------------------------------------------------------
/// Open the SDIO module.
///
/// This function requests a resource of #HAL_SYS_FREQ_52M. 
//=============================================================================
PUBLIC VOID hal_SdioOpen(UINT8 clk_adj, HAL_SDIO_IRQ_HANDLER_T handler);



//=============================================================================
// hal_SdioClose
//-----------------------------------------------------------------------------
/// Close the SDIO module.
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
//=============================================================================
PUBLIC VOID hal_SdioClose(VOID);



//=============================================================================
// hal_SdioWakeUp
//-----------------------------------------------------------------------------
/// This function requests a resource of #HAL_SYS_FREQ_52M. 
/// hal_SdioSleep() must be called before any other
//=============================================================================
PUBLIC VOID hal_SdioWakeUp(VOID);



//=============================================================================
// hal_SdioSleep
//-----------------------------------------------------------------------------
/// This function release the resource to #HAL_SYS_FREQ_32K.
//=============================================================================
PUBLIC VOID hal_SdioSleep(VOID);



// =============================================================================
// hal_SdioSendCmd
// -----------------------------------------------------------------------------
// Send a command to a SD card plugged to the sdio port.
// @param cmd Command
// @param arg Argument of the command
// @param suspend Feature not implemented yet.
// =============================================================================
#ifdef USE_SPRD_WIFI
PUBLIC VOID hal_SdioSendCmd(mmc_command *cmd, mmc_data *data, HAL_SDIO_TRANSFER_T* transfer);
#else
PUBLIC VOID hal_SdioSendCmd(HAL_SDIO_CMD_T cmd, UINT32 arg, BOOL suspend, UINT8 direct,BOOL multiblock);
#endif

// =============================================================================
// hal_SdioCmdDone
// -----------------------------------------------------------------------------
/// @return \c TRUE of there is not command pending or being processed.
// =============================================================================
PUBLIC BOOL hal_SdioCmdDone(VOID);



// =============================================================================
// hal_SdioSetClk
// -----------------------------------------------------------------------------
/// Set the SDIO clock frequency to something inferior but close to that,
/// taking into account system frequency.
// =============================================================================
PUBLIC VOID hal_SdioSetClk(UINT32 clock);


// =============================================================================
// hal_SdioGetOpStatus
// -----------------------------------------------------------------------------
/// @return The operational status of the SDIO module.
// =============================================================================
PUBLIC HAL_SDIO_OP_STATUS_T hal_SdioGetOpStatus(VOID);



// =============================================================================
// hal_SdioGetResp
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command. It is needed to provide the command index and its application 
/// specific status, in order to determine the type of answer expected.
///
/// @param cmd Command to send
/// @param arg Pointer where the argument will be stored. Only the first case
/// is set in case of a response of type R1, R3 or R6, 4 if it is a R2 response.
/// @param suspend Unsupported
// =============================================================================
#ifdef USE_SPRD_WIFI
PUBLIC VOID hal_SdioGetResp(mmc_command *cmd);
#else
PUBLIC VOID hal_SdioGetResp(HAL_SDIO_CMD_T cmd, UINT32* arg, BOOL suspend);
#endif

// =============================================================================
// hal_SdioGetCardDetectPinLevel
// -----------------------------------------------------------------------------
/// @return \c TRUE if card detect (DAT3) line is high,
///         \c FALSE if the line is low.
/// User must check with SD spec and board pull-up/down resistor to 
/// interpret this value.
// =============================================================================
PUBLIC BOOL hal_SdioGetCardDetectPinLevel(VOID);

// =============================================================================
// hal_SdioGetResp32BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command triggerring a response with a 32 bits argument (typically, 
/// R1, R3 or R6).
/// @return 32 bits of arguments of a 48 bits response token
// =============================================================================
PUBLIC UINT32 hal_SdioGetResp32BitsArgument(VOID);


// =============================================================================
// hal_SdioGetResp128BitsArgument
// -----------------------------------------------------------------------------
/// This function is to be used to get the argument of the response of a 
/// command triggerring a response with a 128 bits argument (typically, 
/// R2).
/// @param buf to a 4 case arrays of 32 bits where the argument of the 
/// function will be stored.
// =============================================================================
PUBLIC VOID hal_SdioGetResp128BitsArgument(UINT32* buf);


// =============================================================================
// hal_SdioEnterDataTransferMode
// -----------------------------------------------------------------------------
/// Configure the SDIO module to support data transfers
/// FIXME Find out why we need that out of the transfer functions...
// =============================================================================
PUBLIC VOID hal_SdioEnterDataTransferMode(VOID);


// =============================================================================
// hal_SdioSetDataWidth
// -----------------------------------------------------------------------------
/// Set the data bus width
/// @param width Number of line of the SD data bus used.
// =============================================================================
PUBLIC VOID hal_SdioSetDataWidth(HAL_SDIO_DIRECTION_T width);


// =============================================================================
// hal_SdioTransfer
// -----------------------------------------------------------------------------
/// Start the ifc transfer to feed the SDIO controller fifo.
/// @param transfer Transfer to program.
/// @return HAL_ERR_NO or HAL_ERR_RESOURCE_BUSY.
// =============================================================================
PUBLIC HAL_ERR_T hal_SdioTransfer(HAL_SDIO_TRANSFER_T* transfer);



// =============================================================================
// hal_SdioTransferDone
// -----------------------------------------------------------------------------
/// Check the end of transfer status.
/// Attention: This means the SDIO module has finished to transfer data.
/// In case of a read operation, the Ifc will finish its transfer shortly 
/// after. Considering the way this function is used (after reading at least
/// 512 bytes, and flushing cache before releasing the data to the user), and
/// the fifo sizes, this is closely equivalent to the end of the transfer.
/// @return \c TRUE if a transfer is over, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdioTransferDone(VOID);



// =============================================================================
// hal_SdioNeedResponse
// -----------------------------------------------------------------------------
/// Tells if the given command waits for a reponse.
/// @return \c TRUE if the command needs an answer, \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdioNeedResponse(HAL_SDIO_CMD_T cmd);




// =============================================================================
// hal_SdioStopTransfer
// -----------------------------------------------------------------------------
/// Stop the ifc transfer feeding the SDIO controller fifo.
/// @param transfer Transfer to program.
/// @return #HAL_ERR_NO
// =============================================================================
PUBLIC HAL_ERR_T hal_SdioStopTransfer(HAL_SDIO_TRANSFER_T* transfer);



// =============================================================================
// hal_SdioIsBusy
// -----------------------------------------------------------------------------
/// Check if the SD/IO is busy.
/// 
/// @return \c TRUE if the SD/IO controller is busy.
///         \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL hal_SdioIsBusy(VOID);



#endif // _HAL_SDIO_H_

