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
///     @file hal_ana_afc.c
///     Implementation of Granite's AFC Driver
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"

#include "global_macros.h"
#include "cos.h"

#ifdef BT_SHAREMEM_TRANSPORT
#include "sharemem.h"
#include "sys_ctrl.h"

#include "hal_sharemem.h"
#include "hal_mem_map.h"
#include "chip_id.h"

#define AFC_NEXT_VAL_DATE 0
HWP_AHB_SHAREMEM_T HAL_SHARED_BSS	g_hal_ShareMem;
HWP_AHB_SHAREMEM_T          *hwp_ShareMem;

static UINT32 *g_hal_read_addr;
static UINT32 *g_hal_write_addr;

// =============================================================================
// hal_AnaAfcOpen
// -----------------------------------------------------------------------------
/// Power Up and Initialize the AFC.
/// Takes a resource.
// =============================================================================
PUBLIC void hal_ShareMemInit(void)
{
	hwp_ShareMem = &g_hal_ShareMem;
    g_hal_read_addr = hwp_ShareMem->Rx_Buff;
    g_hal_write_addr = hwp_ShareMem->Tx_Buff;
    hwp_ShareMem->Rx_Buff[0] = 0;
    hwp_ShareMem->Tx_Buff[0] = 0;
}

PUBLIC UINT16 *hal_ShareMem_GetHeader(void)
{
    if(*g_hal_read_addr == 0xffffffff)
        g_hal_read_addr = hwp_ShareMem->Rx_Buff;
        
    return (UINT16*)(g_hal_read_addr);
}

PUBLIC UINT8 *hal_ShareMem_GetData(void)
{
    return (UINT8*)(g_hal_read_addr+1);
}

UINT32 *bt_read_addr[1024];
UINT32 bt_read_index;

PUBLIC void hal_ShareMem_NextData(UINT32 len)
{
#if 0 // for test
	bt_read_addr[bt_read_index++] = g_hal_read_addr;
	bt_read_index&=0x3ff;
#endif

    g_hal_read_addr += ((len+3)>>2)+1;
    //hal_HstSendEvent(g_hal_read_addr);
}

PUBLIC UINT8 hal_ShareMem_GetRxPage(void)
{
    return ((g_hal_read_addr - (UINT32*)hwp_ShareMem->Rx_Buff)>>8);
}

PUBLIC UINT8 hal_ShareMem_GetTxPage(void)
{
    return ((g_hal_write_addr - (UINT32*)hwp_ShareMem->Tx_Buff)>>6);
}

PUBLIC void hal_ShareMem_SendHeader(UINT16 *header)
{
	g_hal_write_addr[((header[1]+3)>>2)+1] = 0;
    //memcpy(g_hal_write_addr, header, 4);
    g_hal_write_addr[0] = (header[1]<<16);
    g_hal_write_addr[0] = (header[1]<<16)|header[0];
    
    g_hal_write_addr += ((header[1]+3)>>2)+1;

    //*g_hal_write_addr = 0x00;
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5856E)
	hwp_sysCtrl->BT_Cfg ^= SYS_CTRL_BT_DATA2BT;
#endif
	//*(unsigned int *)0xa1a000Bc ^= 0x800;
    //hwp_gpio->gpio_oen_set_in ^= 0x40000000; // reverse gpio 30
}

PUBLIC void hal_ShareMem_SendData(UINT8 *data, UINT32 len)
{
    if(data != NULL && len > 0)
    {
        if(g_hal_write_addr+((len+3)>>2)+3 - (UINT32*)(hwp_ShareMem->Tx_Buff) > SHAREMEM_SIZE * 2)
        {
            hwp_ShareMem->Tx_Buff[0] = 0;
            *g_hal_write_addr = 0xffffffff;
            g_hal_write_addr = hwp_ShareMem->Tx_Buff;
        }
        memcpy((UINT8*)(g_hal_write_addr+1), data, len);
    }
}

void hal_Sharemem_Interrupt_Handle(void)
{
    COS_EVENT ev = {0};
    ev.nEventId = 0;
    COS_SendEvent(MOD_BT, &ev , COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

#endif

