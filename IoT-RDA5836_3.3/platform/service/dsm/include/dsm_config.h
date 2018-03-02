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

#ifndef	_DSM_CONFIG_H_
#define	_DSM_CONFIG_H_


// =============================================================================
// DSM_CONFIG_T
// -----------------------------------------------------------------------------
// This structue discripte the DSM partition config information,include partition number and partition table.
// dsmPartitionNumber: Partition nmuber.
// dsmPartitionInfo: Array of partition information.
// =============================================================================
typedef struct _dsm_config_t
{
    // Partition number.
    UINT32                  dsmPartitionNumber;
    
    // Partiton table.
#ifdef WIN32    
    DSM_PARTITION_CONFIG    dsmPartitionInfo[16];   
#else
    DSM_PARTITION_CONFIG    dsmPartitionInfo[6];   
#endif
} DSM_CONFIG_T;
CONST DSM_CONFIG_T* tgt_GetDsmCfg(VOID);
#endif

