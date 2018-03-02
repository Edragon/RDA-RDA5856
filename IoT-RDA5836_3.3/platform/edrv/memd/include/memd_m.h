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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file memd_m.h
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



/// @defgroup memd EDRV MEMD Mainpage
/// 
/// We describe here the MEMD Driver API. Of interest are the two 
/// opening functions : memd_RamOpen and memd_FlashOpen, which set 
/// the proper settings and configure the access to the RAM and 
/// Flash chip respectively, to get the best possible performances.
/// 
/// @par Module List
/// This document is composed of:
/// - @ref flashd
/// - @ref ramd
/// 
/// @{

#ifndef _MEMD_M_H_
#define _MEMD_M_H_

#include "cs_types.h"
#include "memd_config.h"

/// @}



/// @defgroup ramd EDRV MEMD RAM Driver
/// This driver only consists in the memd_RamOpen open function,
/// which configure the RAM to offer its best possible use. Accessing
/// RAM before a call to this function is possible, but will offer
/// poor performance.
/// @{


// =============================================================================
// memd_RamOpen
// -----------------------------------------------------------------------------
/// That function configures the RAM.
/// It must be called to configure the best settings for the RAM, thus enabling
/// the fatest access.
/// @param cfg Ram configuration used to open the RAM.
///
// =============================================================================
PUBLIC VOID memd_RamOpen(CONST MEMD_RAM_CONFIG_T* cfg);

/// @} // <-- End of the ramd group

/// @defgroup flashd EDRV MEMD Flash Driver
/// 
/// This document describes the characteristics of the Flash Driver 
/// and how to use it its API
///
/// A flash chip can be used for both program and file system data. This driver 
/// provide a basic API for accessing generic flash chips to implement a file 
/// system.
///
/// @par Flash Operation
/// This driver provides an abstraction of the flash commands. Nevertheless, the 
/// sector structure of the flash must still be managed at the upper software 
/// level.
///
/// @par
/// A basic flash handling is proposed by the HAL layer. But to fully use
/// the flash at its best performance, a call to the #memd_Open function
/// must be made, to set the specific driver configuration.
///
/// @par
/// Also note that in a flash chip, bit can only go from '1' to '0' during a 
/// write operation. To get back to '1', a full sector need to be erased. This 
/// must be taken into account by the upper level software. For instance 
/// managing a block list of used sector should be coded so that it writes 
/// only '0' to change the state of a sector from free to use, and from used to 
/// erasable.
///
/// @par
/// Moreover the life time of a sector is counted in number of erases, so 
/// trying to distribute the use of sectors before erasing them will improve 
/// the global lifetime of the flash. (Anyway, a file system should also be 
/// able to mark sectors as erroneous).
///
/// @par
/// The driver basic functions access the flash as a block device. Note that 
/// the read and write functions can actually access at byte boundaries.
///
///  
/// @{
///  


// =============================================================================
//  Defines
// =============================================================================

// Macro used to put a specific function in flash boot area
#define MEMD_BOOT_FUNC __attribute__((section(".boottext")))


// =============================================================================
//  Types
// =============================================================================

// =============================================================================
//  MEMD_ERR_T
// -----------------------------------------------------------------------------
/// Error codes
/// The Flash API uses the following error codes. 
/// Those errors reflect errors from the flash chip, for consistency the current 
/// operation should be restarted.
// =============================================================================
typedef enum
{
    /// No error occured
    MEMD_ERR_NO = 0,

    /// An error occurred during the erase operation
    MEMD_ERR_ERASE = -10,

    /// An error occurred during the write operation
    MEMD_ERR_WRITE,

    /// This error occurs when a command requiring 
    /// sector aligned addresses as parameters is called with unaligned addresses. 
    /// (Those are 
    /// #memd_FlashErrors memd_FlashErase, #memd_FlashLock and #memd_FlashUnlock)
    MEMD_ERR_ALIGN,

    /// An erase or write operation was attempted on a protected sector
    MEMD_ERR_PROTECT,
    /// erase suspend.
    MEMD_ERR_SUSPEND,
    
}MEMD_ERR_T;


// =============================================================================
// MEMD_BANK_LAYOUT_T
// -----------------------------------------------------------------------------
/// Describes the layout of a flash bank. 
// =============================================================================
typedef struct
{ 
    /// Start address of the bank
    UINT32 bankStartAddr;

    /// Composition of the bank, which can be made of up to 3 different sector
    /// types. One line is for one sector type. The first column is the number
    /// of sectors of this type and the second column is the size of this type
    /// of sector.
    UINT32 sectorLayoutTable[3][2];

} MEMD_BANK_LAYOUT_T;

 
// =============================================================================
// MEMD_FLASH_LAYOUT_T
// -----------------------------------------------------------------------------
/// This structure describes a flash through its different banks.
// =============================================================================
typedef struct
{
    /// Number of banks in the flash
    UINT8 numberOfBank;

    /// Table of pointers to the structures describing the different banks
    CONST MEMD_BANK_LAYOUT_T* bankLayout;
    
    /// Size of the flash in bytes.
    UINT32 fullSize;
    
    /// User Data Field Base Address
    UINT32 userDataBase;
    
    /// User Data Field Size
    UINT32 userDataSize;

    /// User Data Sector Logical Block Size
    UINT32 userDataSectorLogicalBlockSize;
} MEMD_FLASH_LAYOUT_T;



// =============================================================================
//  Functions
// =============================================================================



// =============================================================================
// memd_FlashOpen
// -----------------------------------------------------------------------------
/// That function configures the flash.
/// It must be called before using any other function of the MEMD API.
///
/// @param cfg Pointer to the configuration to use to open the flash.
/// This parameter is ignored by the romulator driver.
/// @return Pointer towards the structure describing the flash.
// =============================================================================
PUBLIC MEMD_FLASH_LAYOUT_T* memd_FlashOpen(CONST MEMD_FLASH_CONFIG_T* cfg);

PUBLIC VOID memd_FlashRst(VOID);


// =============================================================================
// memd_FlashRead
// -----------------------------------------------------------------------------
/// This function reads data from the flash. It gets its data from \c flashAddress, 
/// copies \c bytesize bytes to \c buffer. \c preadbytesize is filled with the 
/// actual number of read bytes (Equal to \c bytesize, or less in case of error)
///
/// @param flashAddress The byte offset within the flash chip (Take care of 
/// the  positions of the code in the flash)
/// @param byteSize Number of bytes to read from the flash
/// @param pReadByteSize Filled with the number of bytes actually read
/// @param buffer Buffer to fill with data from flash
/// @return #MEMD_ERR_NO
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashRead(UINT8 *flashAddress,
        UINT32  byteSize, 
        UINT32* pReadByteSize,
        UINT8*  buffer);

// =============================================================================
// memd_FlashWrite
// -----------------------------------------------------------------------------
/// This function writes data in the flash. It gets its data from \c buffer, copies 
/// \c bytesize bytes to the flash location designed by \c flashAddress. \c 
/// pwrittenbytesize is filled with the actual number of written bytes (Equal 
/// to size, or less in case of error).
/// 
/// @param  flashAddress The byte offset within the flash chip. (Take care not 
/// to overwrite the code present at the beginning of the flash)
/// @param  byteSize Number of bytes to write in flash
/// @param  pWrittenByteSize Number of bytes actually written in flash
/// @param  buffer Buffer where to get the data to write in the flash
/// @return #MEMD_ERR_NO, #MEMD_ERR_WRITE or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed.
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashWrite(UINT8 *flashAddress,
        UINT32 byteSize, 
        UINT32 * pWrittenByteSize, 
        CONST UINT8* buffer);


// =============================================================================
// memd_FlashErase
// -----------------------------------------------------------------------------
/// This function erases contiguous flash sectors. 
/// Addresses are byte offset within the flash chip and <B> must be aligned on sectors</B>:
/// - The \c startFlashAddress is the address of the first sector to erase.
/// - The \c endFlashAddress is the address of the first sector NOT to erase.
///   If \c endFlashAddress is \c NULL, only one sector will be erased.
/// .
/// Care must be taken not to erase the code present at the beginning of the flash.
///
/// @param start_flashAddress The address of the first sector to erase
/// @param end_flashAddress The address of the first sector NOT to erase.
///   If \c NULL, only one sector will be erased 
/// @return #MEMD_ERR_NO, #MEMD_ERR_ERASE, #MEMD_ERR_ALIGN or #MEMD_ERR_PROTECT
/// whether the operation succeeded or failed
///
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashErase(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress);


PUBLIC MEMD_ERR_T memd_FlashErase2(
    UINT8 *startFlashAddress,
    UINT8 *endFlashAddress);

// =============================================================================
// memd_FlashGetGlobalAddress
// -----------------------------------------------------------------------------
/// This function returns the global address in the system memory map 
/// from the flash relative address. 
/// Using the returned address, the user will be able to read directly 
/// from the flash, without using #memd_FlashRead
///
/// Note that the flash can be read directly, but it cannot be written without 
/// using the memd driver functions. That's why the returned address is declared 
/// as \c CONST. The content of the flash can be erased without the help of the 
/// CPU (by sending a command to the flash chip), that's why the returned 
/// address is declared as \c VOLATILE.
///
/// @param flashAddress Byte address within the flash chip to be converted 
/// into a global byte address (take care to the positions of the code in flash)
/// @return The byte global address
// =============================================================================
PUBLIC VOLATILE CONST UINT8* memd_FlashGetGlobalAddress(UINT8 *flashAddress);


// =============================================================================
// memd_FlashLock
// -----------------------------------------------------------------------------
/// This function protects the sectors between \c start_flashAddress and 
/// \c end_flashAddress.  Addresses must be sector aligned.  The sector 
/// specified by \c end_flashAddress is @b _NOT_ included in the 
/// protect procedure 
///
/// @param startFlashAddress Address of the first sector to protect
/// @param end_LashAddress Address of the first sector not to protect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashLock(UINT8* startFlashAddress, UINT8 *endFlashAddress);


// =============================================================================
// memd_FlashUnlock
// -----------------------------------------------------------------------------
/// This function unlocks the protection of the flash sectors between 
/// \c start_flashAddress and \c end_flashAddress.  Addresses must be 
/// sector aligned.  The sector specified by \c end_flashAddress is 
/// _NOT_ included in the unprotect procedure 
///
/// @param start_flashAddress Address of the first sector to unprotect
/// @param end_flashAddress Address of the first sector not to unprotect
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashUnlock(UINT8* startFlashAddress, UINT8* endFlashAddress);


// =============================================================================
// memd_FlashGetLockStatus
// -----------------------------------------------------------------------------
/// This function gets the protections status of the sector starting at 
/// \c start_flashAddress.  This addresses must be sector 
/// aligned.
///
/// @param startFlashAddress Address of the beginning of the sector to check.
/// @param lockStatus Pointer to the boolean value retaining the result of the
/// get: \c TRUE if the sector is protected, \c FALSE otherwise. The value is 
/// only valid if the return value is MEMD_ERR_NO
/// @return #MEMD_ERR_NO or #MEMD_ERR_ALIGN
// =============================================================================
PUBLIC MEMD_ERR_T memd_FlashGetLockStatus(UINT8* startFlashAddress, BOOL* lockStatus);


// =============================================================================
// memd_FlashGetUserFieldInfo
// -----------------------------------------------------------------------------
/// That function get the user data info from memd.def.
///
/// @param userFieldBase The USER_DATA_BASE defined in memd.def.
/// @param userFieldSize The USER_DATA_SIZE defined in memd.def.
/// @param sectorSize The sector size of user data field, it is the large sector size.
///        Some small boot sectors can be regarded as one large sector.
/// @return VOID.
// =============================================================================
PUBLIC VOID memd_FlashGetUserFieldInfo(UINT32 *userFieldBase, UINT32 *userFieldSize, UINT32 *sectorSize, UINT32 *blockSize);


// =============================================================================
// memd_FlashRomulatorSetRamMode
// -----------------------------------------------------------------------------
/// Set the Romulator in RAM mode. This function is only effective when using
/// a romulator device. When using a real Flash Combo, the function returns 
/// \c FALSE and does nothing.
///
/// When the romualtor is in RAM mode, it can be accessed as a RAM, that is to 
/// say read and written without sending any command, but through direct 
/// addressing.
///
/// @param  enable Enable or disable the RAM mode.
/// @return \c TRUE if the operation could be done and the switch done,
///         \c FALSE otherwise.
// =============================================================================
PUBLIC BOOL memd_FlashRomulatorSetRamMode(BOOL enable);


PUBLIC VOID memd_FlashSetCtrRegionSize(UINT32 CtrSize);

PUBLIC UINT32 memd_FlashGetCtrRegionSize(VOID);

PUBLIC VOID memd_FlashSetFSSectorCTXSizeInfo(UINT32 sectorCTXSize);
PUBLIC UINT32 memd_FlashGetFSSectorCTXSizeInfo(VOID);

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
PUBLIC  MEMD_ERR_T memd_FlashErase_First(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time);
PUBLIC  MEMD_ERR_T memd_FlashErase_Continue(UINT8 *startFlashAddress, UINT8 *endFlashAddress,UINT32 time);
#endif

PUBLIC UINT32 memd_FlashErase2SuspendTime(VOID);
PUBLIC VOID memd_FlashSetLayout();


//SPI flash: SPI register bit define
#ifdef __PRJ_WITH_SPIFLSH__
/*
[16 14...13]size: 0000: 8Mbit	  0001: 16Mbit	   0010: 32Mbit      0011: 64Mbit  
			    1000:128Mbit     1001:256Mbit
[12]chip num: 0: 1 chip 	       1: 2 chip
*/
#define ADDR_SAVING_FLASHID_4_USB	0xA1A04410
#define FLASH_1_8MBIT    0x00000
#define FLASH_2_8MBIT    0x01000
#define FLASH_1_16MBIT   0x02000
#define FLASH_2_16MBIT   0x03000
#define FLASH_1_32MBIT   0x04000
#define FLASH_2_32MBIT   0x05000
#define FLASH_1_64MBIT   0x06000
#define FLASH_2_64MBIT   0x07000
#define FLASH_1_128MBIT  0x10000
#define FLASH_2_128MBIT  0x11000
#define FLASH_1_256MBIT  0x12000
#define FLASH_2_256MBIT  0x13000
#define FLASH_64_32MBIT  0x14000
#define FLASH_128_32MBIT 0x15000
#define FLASH_128_64MBIT 0x16000

//spi flash erase start to suspend time: n MILLI_SECONDS
#define FLASH_MICRON_ERASE2SUSPEND_TIME 0xc0
#define FLASH_GIGADEVICE_ERASE2SUSPEND_TIME 0x60
#define FLASH_COMMON_ERASE2SUSPEND_TIME 0x50




//mx25u6435e:c2 2537
//w25q64bv:ef 4017
//w25q64dw:ef 6017
//gd25q64b:c8 4017
//en25q64:1c 3017
//en25s64:1c 3817
//s25fl064p:01 0216
//fidelix:f8 3217

typedef enum
{
    MEMD_FLASH_SPANSION = 0x01,
    MEMD_FLASH_EON = 0x1c,
    MEMD_FLASH_MXIC = 0xc2, 
    MEMD_FLASH_GIGADEVICE = 0xc8,   
    MEMD_FLASH_BERGMICRO = 0xe0, 
    MEMD_FLASH_WINBOND = 0xef,
    MEMD_FLASH_FIDELIX = 0xf8,
    MEMD_FLASH_ESMT = 0x8c,
    MEMD_FLASH_XMCKGD = 0x20,
    MEMD_FLASH_MICRON = 0x2c,
    MEMD_FLASH_PM25L = 0x7f,
    MEMD_FLASH_FUDAN = 0xa1   ,
    MEMD_FLASH_BOYA = 0x68,
    MEMD_FLASH_PUYA = 0x85,
}MEMD_FLASH_MANUFACTURER_T;

typedef enum
{
    MEMD_FLASH_SPANSION_30V = 0x02,
    MEMD_FLASH_EON_30V = 0x30,
	MEMD_FLASH_EON_18V = 0x38,
	MEMD_FLASH_MXIC_30V = 0x5E, 
	MEMD_FLASH_MXIC_18V = 0x25, 
    MEMD_FLASH_GIGADEVICE_30V = 0x40,
    MEMD_FLASH_GIGADEVICE_18V = 0x60,  
	MEMD_FLASH_WINBOND_30V = 0x40,
	MEMD_FLASH_WINBOND_18V = 0x60,
    MEMD_FLASH_FIDELIX_30V = 0x32,
	MEMD_FLASH_FIDELIX_18V = 0x42,
	MEMD_FLASH_ESMT_30V = 0x41,
	MEMD_FLASH_ESMT_18V = 0x25,
    MEMD_FLASH_NUMONYX_30V = 0xBA,    
    MEMD_FLASH_MICRON_18V = 0xCB,
    MEMD_FLASH_FUDAN_30V = 0x40
}MEMD_FLASH_MEMORY_TYPE_T;

typedef enum
{
    MEMD_FLASH_SPANSION_64M = 0x16,
    MEMD_FLASH_EON_64M = 0x17,
    MEMD_FLASH_EON_32M = 0x16,
    MEMD_FLASH_MXIC_U_128M = 0x38, 
	MEMD_FLASH_MXIC_U_64M = 0x37,
	MEMD_FLASH_MXIC_U_32M = 0x36, 
	MEMD_FLASH_MXIC_L_32M = 0x16, 	
    MEMD_FLASH_GIGADEVICE_64M = 0x17,  
    MEMD_FLASH_GIGADEVICE_32M = 0x16,
    MEMD_FLASH_GIGADEVICE_128M = 0x18,
    MEMD_FLASH_GIGADEVICE_256M = 0x19,
	MEMD_FLASH_WINBOND_64M = 0x17,
	MEMD_FLASH_WINBOND_32M = 0x16,
    MEMD_FLASH_FIDELIX_64M = 0x17,   
    MEMD_FLASH_FIDELIX_32M = 0x16,
    MEMD_FLASH_ESMT_64M_30V = 0x17,
    MEMD_FLASH_ESMT_32M_30V = 0x16,
    MEMD_FLASH_ESMT_64M_18V = 0x37,
    MEMD_FLASH_ESMT_32M_18V = 0x36,
    MEMD_FLASH_NUMONYX_128M = 0x18,
    MEMD_FLASH_MICRON_32M = 0x16,
    MEMD_FLASH_MICRON_64M = 0x17,
    MEMD_FLASH_MICRON_128M = 0x18,
    MEMD_FLASH_FUDAN_32M = 0x16
}MEMD_FLASH_CAPACITY_TYPE_T;


typedef struct
{     
    MEMD_FLASH_MANUFACTURER_T manufacturerID;  
	MEMD_FLASH_MEMORY_TYPE_T device_memory_type_ID;
	MEMD_FLASH_CAPACITY_TYPE_T device_capacity_type_ID;
} MEMD_SPIFLSH_RDID_T;


#endif

///  @} <- End o flashd group 


#endif // _MEMD_M_H_
