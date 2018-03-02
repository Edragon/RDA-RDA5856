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
#ifndef _VOC_RAM_ASM_H_
#define _VOC_RAM_ASM_H_

//THIS FILE HAS BEEN GENERATED WITH COOLWATCHER. PLEASE EDIT WITH CARE !

#ifndef CT_ASM
//#error "You are trying to use in a normal C code the assembly H description of 'voc_ram'."
#endif


#define VOC_RAM_X_SIZE                           (0X5000)
#define VOC_RAM_Y_SIZE                           (0X5000)
#define VOC_ROM_Z_SIZE                           (0X0000)
#define VOC_RAM_I_SIZE                           (0X5000)
#define VOC_RAM_X_SPACE                          (0X8000)
#define VOC_RAM_Y_SPACE                          (0X8000)
#define VOC_ROM_Z_SPACE                          (0X10000)
#define VOC_RAM_I_SPACE                          (0X20000)

//==============================================================================
// voc_ram
//------------------------------------------------------------------------------
/// 
//==============================================================================
#define REG_VOC_RAM_BASE            0x01940000

#define REG_VOC_RAM_BASE_HI         BASE_HI(REG_VOC_RAM_BASE)
#define REG_VOC_RAM_BASE_LO         BASE_LO(REG_VOC_RAM_BASE)





#endif
