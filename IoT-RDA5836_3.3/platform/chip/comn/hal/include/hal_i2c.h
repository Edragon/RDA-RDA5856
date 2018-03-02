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
/// @file hal_i2c.h
/// That file describe the I2C master module driver API.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#include "cs_types.h"
#include "hal_error.h"

/// @defgroup i2c HAL I2C Driver
/// I2C is a serial bus protocol. The API provides blocking functions to send
/// an write one byte at a time, or read and write bytes to or from 
/// a register in a slave device's memory map. (The slave being connected
/// on the I2C bus.
///
/// @par
/// The I2C master has a FIFO of one byte. The API allows to write or
/// read several bytes using #hal_I2cSendData or #hal_I2cGetData.
///
/// @{


// ============================================================================
// TYPES
// ============================================================================


// ============================================================================
// HAL_I2C_BUS_ID_T
// ----------------------------------------------------------------------------
/// That type describes the supported I2C bus IDs.
// ============================================================================
typedef enum
{
    HAL_I2C_BUS_ID_1,
    HAL_I2C_BUS_ID_2,
    HAL_I2C_BUS_ID_3,

    HAL_I2C_BUS_ID_QTY,

    HAL_I2C_BUS_ID_INVALID = 0xff,

} HAL_I2C_BUS_ID_T;


// ============================================================================
// HAL_I2C_BPS_T
// ----------------------------------------------------------------------------
/// That type describes the supported frequencies for the I2C bus.
/// They are sustained as long as the driver is opened.
// ============================================================================
typedef enum
{
    HAL_I2C_BPS_100K,
    HAL_I2C_BPS_400K

} HAL_I2C_BPS_T;



// ============================================================================
// FUNCTIONS
// ============================================================================


// ============================================================================
// hal_I2cOpen
// ----------------------------------------------------------------------------
/// This function opens the I2C modules and enable the use of its API.
/// 
/// This function requests a resource of #HAL_SYS_FREQ_26M.
///
/// @param id ID of the I2C bus
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cOpen(HAL_I2C_BUS_ID_T id);


// ============================================================================
// hal_I2cClose
// ----------------------------------------------------------------------------
/// This function closes the I2C module.
///
/// This function release the resource to #HAL_SYS_FREQ_32K.
///
/// @param id ID of the I2C bus
// ============================================================================
PUBLIC VOID hal_I2cClose(HAL_I2C_BUS_ID_T id);


// ============================================================================
// hal_I2cSendByte
// ----------------------------------------------------------------------------
/// This function write one byte at a given address of a given slave.
/// This is a blocking function, unless the bus is already busy, in which
/// case the function returns #HAL_ERR_RESOURCE_BUSY immediatly.
/// 
/// @param id ID of the I2C bus
/// @param slaveAddr Address of the slave to which we write a byte.
/// @param memAddr Address on the slave's memmory map where to write the 
/// data byte.
/// @param data Byte to write
/// @return #HAL_ERR_NO or #HAL_ERR_RESOURCE_BUSY, if the I2C FIFO is full
/// at that time, or HAL_ERR_COMMUNICATION_FAILED.
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cSendByte(HAL_I2C_BUS_ID_T id, UINT32 slaveAddr, UINT32 memAddr, UINT8 data);



// ============================================================================
// hal_I2cGetByte
// ----------------------------------------------------------------------------
/// This function reads one byte from a given address of a given slave's memory
/// map.
/// This is a blocking function, unless the bus is already busy, in which
/// case the function returns #HAL_ERR_RESOURCE_BUSY immediatly.
/// 
/// @param id ID of the I2C bus
/// @param slaveAddr Address of the slave from which we read a byte.
/// @param memAddr Address in the memory map where to get the byte.
/// @param pData Pointer to the place where the read data will be written.
/// @return #HAL_ERR_NO or #HAL_ERR_RESOURCE_BUSY, if the I2C FIFO is full
/// at that time, or HAL_ERR_COMMUNICATION_FAILED.
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cGetByte(HAL_I2C_BUS_ID_T id, UINT32 slaveAddr, UINT32 memAddr, UINT8* pData);



// ============================================================================
// hal_I2cSendData
// ----------------------------------------------------------------------------
/// This function write a buffer of data to a given slave on the I2C bus.
/// A typical use of this function is to set several consecutive registers
/// in just one access for device with auto-incremented register addresses.
/// The memAddr value is the first written to the slave and is the address of 
/// the first register to set. The values to write are the length bytes of the 
/// buffer.
/// 
/// @param id ID of the I2C bus
/// @param slaveAddr Address of the slave to which we write a byte.
/// @param memAddr Address on the slave's memory map from which to write the 
/// data bytes, or (equivalent to) first value written to the slave.
/// @param pData Pointer to the buffer to write.
/// @param length Number of bytes to send.
/// @return #HAL_ERR_NO or #HAL_ERR_RESOURCE_BUSY, if the I2C FIFO is full
/// at that time, or HAL_ERR_COMMUNICATION_FAILED.
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cSendData(HAL_I2C_BUS_ID_T id, UINT32 slaveAddr,  UINT32 memAddr,
                                 CONST UINT8* pData,      UINT32 length);





// ============================================================================
// hal_I2cGetData
// ----------------------------------------------------------------------------
/// This function reads a number of bytes from a given address of a given slave's memory
/// map.
/// 
/// @param id ID of the I2C bus
/// @param slaveAddr Address of the slave from which we read a byte.
/// @param memAddr Address in the memory map where to get the byte.
/// @param pData Pointer to the place where the read data will be written.
/// @param length Number of bytes to read.
/// @return #HAL_ERR_NO or #HAL_ERR_RESOURCE_BUSY, if the I2C FIFO is full
/// at that time, or HAL_ERR_COMMUNICATION_FAILED.
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cGetData(HAL_I2C_BUS_ID_T id, UINT32 slaveAddr,   UINT32 memAddr,
                                UINT8* pData,       UINT32 length);

// ============================================================================
// hal_I2cSendRawByte
// ----------------------------------------------------------------------------
/// This function sends a single byte on the I2C interface
/// It is not designed for use in the real I2C protocol, but allows access
/// for non-standard usages such as SCCB for the Omnivision Camera control
/// 
/// @param id ID of the I2C bus
/// @param SendByte raw data byte to be sent.
/// @param CmdMask this is the command associated with this byte.  It must
/// be sent at the same time so the two actions (write to the data fifo and
/// issue the command) are done together.
/// @return HAL_ERR_NO if everything was OK, otherwise Resource Busy if the
/// bus is in use
// ============================================================================
PUBLIC HAL_ERR_T hal_I2cSendRawByte(HAL_I2C_BUS_ID_T id, UINT8 SendByte, UINT32 CmdMask);

// ============================================================================
// hal_I2cReadRawByte
// ----------------------------------------------------------------------------
/// This function completes the 2 phase read cycle as defined in the SCCB
/// spec.  The actual commands for the I2C module must be:  Stop, Read, and
/// NACK. (The SCCB requires a NACK at the end of the read byte)
/// 
/// @param id ID of the I2C bus
/// @return Data byte read by the I2C interface
// ============================================================================
PUBLIC UINT8 hal_I2cReadRawByte(HAL_I2C_BUS_ID_T id, UINT32 CmdMask);

/// @} // i2c group

#endif // _HAL_I2C_H_

