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



/*
================================================================================
  File         pal_flhr.h
--------------------------------------------------------------------------------

  Scope      : PAL flash Ram header file.

  History    :
--------------------------------------------------------------------------------
 Jun 13 2003 |  ADA  | Creation
 Jan 12 2006 |  ADA  | Increase Flash sector size and purge threshold.
================================================================================
*/

#ifndef __PAL_FLHR_H__
#define __PAL_FLHR_H__

#include "sxs_type.h"
#include "sxs_flsh.hp"
#include "sxs_lib.h"
#include "sxr_ops.h"

#define PAL_RAM_FLASH_NB_SECT   6
#define PAL_RAM_FLASH_SECT_SIZE 2048
#define PAL_RAM_FLASH_SIZE      (PAL_RAM_FLASH_NB_SECT * PAL_RAM_FLASH_SECT_SIZE)


u16 *pal_RamFlash;

#define PAL_SECTOR_TO_ADD(Sect) (pal_RamFlash + ((Sect * PAL_RAM_FLASH_SECT_SIZE) / 2))


void pal_RamFlhWrite (u16 *Address, u16 Data);
void pal_RamFlhEraseSector (void *Address);
void pal_RamFlhUnLock (u8 *SectorAddress);
void pal_RamFlhLock (u8 *SectorAddress);
void pal_RamFlhIdentification (sxs_FlashId_t *Flash);



u16 *pal_RamSectorToAdd (u8 Sect)
{
 return PAL_SECTOR_TO_ADD(Sect);
}


sxs_FlashDesc_t pal_RamFlashDesc =

{
 {
  0,
  0,
 },

 pal_RamSectorToAdd,

 pal_RamFlhIdentification,
 pal_RamFlhLock,
 pal_RamFlhUnLock,
 pal_RamFlhEraseSector,
 pal_RamFlhWrite,

 PAL_RAM_FLASH_NB_SECT,

 {
  5,
  1,
  0,
  PAL_RAM_FLASH_SECT_SIZE,
  0, // PAL_SECTOR_TO_ADD(5),
  0, // PAL_SECTOR_TO_ADD(6) - 1
 },
 {
  {
   4,
   1,
   PAL_RAM_FLASH_SECT_SIZE - 256,
   PAL_RAM_FLASH_SECT_SIZE,
   0, // PAL_SECTOR_TO_ADD(4),
   0, // PAL_SECTOR_TO_ADD(5) - 1
  },
  {
   3,
   1,
   PAL_RAM_FLASH_SECT_SIZE - 256,
   PAL_RAM_FLASH_SECT_SIZE,
   0, // PAL_SECTOR_TO_ADD(3),
   0, // PAL_SECTOR_TO_ADD(4) - 1
  }
 },
 {
  0,
  3,
  100,
  3 * PAL_RAM_FLASH_SECT_SIZE,
  0, // PAL_SECTOR_TO_ADD(0),
  0  // PAL_SECTOR_TO_ADD(3) - 1
 }
};



/*
==============================================================================
   Function   : pal_RamFlhIdentification
 -----------------------------------------------------------------------------
   Scope      : Flash type detection
   Parameters : Pointer on sxs_FlashId_t.
   Return     : Structure sxs_FlashId_t is filled with found data.
==============================================================================
*/
void pal_RamFlhIdentification (sxs_FlashId_t *Flash)
{
 if (pal_RamFlash == NIL)
  pal_RamFlash = (u16 *)sxr_HMalloc (PAL_RAM_FLASH_SIZE);

 pal_RamFlashDesc.StaticBnk.StartAdd      = PAL_SECTOR_TO_ADD(5);
 pal_RamFlashDesc.StaticBnk.LastAdd       = PAL_SECTOR_TO_ADD(6) - 1;

 pal_RamFlashDesc.DynamicBnk [0].StartAdd = PAL_SECTOR_TO_ADD(4);
 pal_RamFlashDesc.DynamicBnk [0].LastAdd  = PAL_SECTOR_TO_ADD(5) - 1;

 pal_RamFlashDesc.DynamicBnk [1].StartAdd = PAL_SECTOR_TO_ADD(3);
 pal_RamFlashDesc.DynamicBnk [1].LastAdd  = PAL_SECTOR_TO_ADD(4) - 1;

 pal_RamFlashDesc.StackedBnk.StartAdd     = PAL_SECTOR_TO_ADD(0);
 pal_RamFlashDesc.StackedBnk.LastAdd      = PAL_SECTOR_TO_ADD(3) - 1;

 Flash -> ManufCode = 0;
 Flash -> DeviceId  = 0;
}


/*
==============================================================================
   Function   : pal_RamFlhLock
 -----------------------------------------------------------------------------
   Scope      : Lock a sector.
   Parameters : Sector address.
   Return     : none
==============================================================================
*/
void pal_RamFlhLock (u8 *SectorAddress)
{
}


/*
==============================================================================
   Function   : pal_RamFlhUnLock
 -----------------------------------------------------------------------------
   Scope      : UnLock a sector.
   Parameters : Sector address.
   Return     : none
==============================================================================
*/
void pal_RamFlhUnLock (u8 *SectorAddress)
{
}


/*
==============================================================================
   Function   : pal_RamFlhEraseSector
 -----------------------------------------------------------------------------
   Scope      : Erase a sector.
   Parameters : Sector address.
   Return     : none
==============================================================================
*/
void pal_RamFlhEraseSector (void *Address)
{
 memset (Address, 0xff, PAL_RAM_FLASH_SECT_SIZE);
}


/*
==============================================================================
   Function   : pal_RamFlhWrite
 -----------------------------------------------------------------------------
   Scope      : Write a word data at the defined address.
   Parameters : Address and Word data.
   Return     : none
==============================================================================
*/
void pal_RamFlhWrite (u16 *Address, u16 Data)
{
 *Address = Data;
}


#endif

