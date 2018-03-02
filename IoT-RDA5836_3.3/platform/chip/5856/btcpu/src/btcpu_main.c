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
/// @file btcpu_main.c                                                         //
/// That file contains the btCPU init function                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "cs_types.h"
#include "chip_id.h"

#include "global_macros.h"
#include "sys_ctrl.h"
#include "bb_sram.h"

#include "hal_host.h"
#include "hal_mem_map.h"
#include "hal_timers.h"
#include "hal_sys.h"
#include "hal_map_engine.h"
#include "hal_map.h"

#include "hal_debug.h"


#include "sys_irq.h"
#include "gpio.h"
#include "debug_host.h"
#include "rdabt_sharemem.h"

//#include "sys_config.h"
extern UINT32 _btrom_data_base;
extern UINT32 _btrom_data_start;
extern UINT32 _btrom_data_end;

extern UINT32 _btrom_bss_start;
extern UINT32 _btrom_bss_end;

extern UINT32 _bt_irq_flash_start_location;
extern UINT32 _bt_irq_flash_end_location;
extern UINT32 _bt_irq_sram_txt_start;

extern UINT32 _bt_sram_flash_start_location;
extern UINT32 _bt_sram_flash_end_location;
extern UINT32 _bt_sram_data_start;

extern UINT32 _bt_sram_bss_start;
extern UINT32 _bt_sram_bss_end;

extern UINT32 _bt_dualport_bss_end;
extern UINT32 test_pka;
extern UINT8 BTms_data_flag;

VOID  BT_CPU_Main(void);
void _bt_Irq_handler(void);

extern void* TRAhcit_Register_UART_Driver(void);
extern void* TRAhcit_Register_ShareMem_Driver(void);
extern void* TRAhcit_Register_COMBINED(void);
extern UINT32 BTms_Sched(UINT32 num_interations);


const UINT32 entry[4] = {(UINT32)BT_CPU_Main, (UINT32)_bt_Irq_handler};
extern UINT32 *DATA_BUFF;
extern void* (*TRAhcit_Register)(void);

volatile UINT8  __attribute__((section(".dualucbss"))) g_btcpu_watcherdog_count=0;


//add for temp rx/tx test

extern UINT32 baseband_test;

void rda_test_tx_continus_first(void *param);
void rda_test_bothscan_second(void *param);

UINT32 LMscan_Write_Scan_Enable(UINT32 scan_enable);
UINT8 LMtmr_Set_Timer(UINT32 timer_value, 
                       void ((*p_funct)(void*)), void* p_link, UINT8 dynamic);
UINT32 BTtimer_Set_Slots(UINT32 timeSlots);
UINT32 BTtimer_Is_Expired(UINT32 ptimer);

void  TC_BaseBand_Test_Tx_continues_Start1(UINT32 mode);
void  TC_BaseBand_Test_Stop(UINT8*p_payload, void *p_complete_event_info);


void SYSirq_Disable_Interrupts_Save_Flags(UINT32 *flags);
void SYSirq_Enable_Interrupts_Save_Flags(UINT32 *flags);
void SYSirq_Interrupts_Restore_Flags(UINT32 flags);
void SYSconfig_Initialise(void);

void BT_Initialise(void);
UINT8 BThost_Initialise();

UINT8  BThost_Sched(UINT32 num_interations);

void rda_test_tx_continus_first(void *param)
{	
	baseband_test = 1 ;
	LMscan_Write_Scan_Enable(0x0);//NO_SCANS_ENABLED
	LMtmr_Set_Timer(0x46500,rda_test_bothscan_second, NULL,0);// 0x46500 288000 slots 180s
	TC_BaseBand_Test_Tx_continues_Start1(baseband_test);

}

void rda_test_bothscan_second(void *param)
{
	TC_BaseBand_Test_Stop(NULL,NULL);
	LMtmr_Set_Timer(0x46500,rda_test_tx_continus_first, NULL,0);
	LMscan_Write_Scan_Enable(0x3);//BOTH_SCANS_ENABLED
}

//add for temp rx/tx test

void BT_CPU_Main(void)
{
    /*
     * WARNING: Following code is duplicated in SYS_HC_INit()
     * ARM ADS COMPILER BUG DOES NOT ALLOW FUNCTION CALL AT THIS POINT
     * */
    UINT32 flags;
	
	UINT32* src;    
	UINT32* dst;
    
    //extern UINT32 le_advert_Test;//,inquiry_test_scan;
    //extern UINT32 inquiry_test_scan;
    //extern UINT32 pagescan_Test;
    //extern UINT32 inquiry_test;

#ifdef CODEC_SUPPORT
    extern g_host_flash_write_req;
    //SYS_SendEvent2(0x20199001);
    test_pka = 0x31;
    if (g_host_flash_write_req) {
        test_pka = 0x32;
        HW_Wait_Flash_Write();
    }
    test_pka = 0x33;
    //SYS_SendEvent2(0x20199002);
#endif
    SYSirq_Disable_Interrupts_Save_Flags(&flags);

    test_pka = 1;
    
    // Copy booting code & data from ROM space to internal SRAM.
	for (src = (UINT32*) &_btrom_data_base,
        dst = (UINT32*) &_btrom_data_start;
	    src < (UINT32*) &_btrom_data_end;
	    src++, dst++)
	{
		*dst = *src;
	}
    test_pka = 2;
    // Clear ROM BSS in Internal SRAM.
	for (dst = (UINT32*) &_btrom_bss_start;
	    dst < (UINT32*) &_btrom_bss_end;
	    dst++)    
	{
		*dst = 0;
	}
    test_pka = 3;
    // Copy irq code from flash space to internal SRAM.
	for (src = (UINT32*) &_bt_irq_flash_start_location,
        dst = (UINT32*) &_bt_irq_sram_txt_start;
	    src < (UINT32*) &_bt_irq_flash_end_location;
	    src++, dst++)
	{
		*dst = *src;
	}
    test_pka = 4;
    // Copy data from flash space to internal SRAM.
	for (src = (UINT32*) &_bt_sram_flash_start_location,
        dst = (UINT32*) &_bt_sram_start;
	    src < (UINT32*) &_bt_sram_flash_end_location;
	    src++, dst++)
	{
		*dst = *src;
	}
    test_pka = 5;
    // Clear BSS in Internal SRAM.
	for (dst = (UINT32*) &_bt_sram_bss_start;
	    dst < (UINT32*) &_bt_sram_bss_end;
	    dst++)    
	{
		*dst = 0;
	}
   // le_advert_Test = 1;
   // inquiry_test_scan=1;
    //pagescan_Test = 1;
   //inquiry_test = 1;
    test_pka = 6;


//	hw_memset8((void*)0xa1c1c000, 0x12, 0x3ff);
//	test_pka = RDA_READ_REG(0xa1c1c3fc);
//return;

    DATA_BUFF = (&_bt_dualport_bss_end); //TODO:

    SYSconfig_Initialise();

    /*
     * Set up the HCI transport registration function (this function
     * is called on intialisation or re-initialisation of the HCI
     * transport sub-system
     */

    //TRAhcit_Set_Register_Function(TRAhcit_Register_UART_Driver);
#ifdef BT_SHAREMEM_TRANSPORT
  	TRAhcit_Register =TRAhcit_Register_ShareMem_Driver; //uart
#else
	TRAhcit_Register =TRAhcit_Register_COMBINED;
#endif
        test_pka = 8;

    BT_Initialise();

#ifndef BT_SHAREMEM_TRANSPORT
    BThost_Initialise();//uart
#endif

    test_pka = 9;

//	SYS_ASSERT();

    SYSirq_Enable_Interrupts_Save_Flags(&flags);
    test_pka = 10;

//       pagescan_Test = 1;
//       connect_Test = 1;
//inquiry_test=1;

//	LMtmr_Set_Timer(0x3E80,rda_test_tx_continus_first, NULL,0);//add for test 16000 slots 10s
	
    while(1)
    {
        g_btcpu_watcherdog_count ++;
        BTms_Sched(0x1);
#ifndef BT_SHAREMEM_TRANSPORT
        BTms_data_flag |= BThost_Sched(0x01);
#endif
//SYShal_config_Update();
    	//rdabt_test_temp();
	test_pka++;
	if(test_pka>0xffffff)
	    test_pka =1;
    }
}

#if 1
BOOL BTCPU_SendEvent2(UINT32 event)
{
    UINT32 timer = BTtimer_Set_Slots(3);
    BOOL   timeOut   = FALSE;
    //  wait for host sema to be free
    //  or time out.
    while (!timeOut&& (hwp_debugHost->event & 1) == 0)
    {
        if (BTtimer_Is_Expired(timer))
        {
            timeOut = TRUE;
        }
    }

    //  Write the byte in the host event register if possible
    if (!timeOut)
    {
        hwp_debugHost->event = event;
    }

    return timeOut;
}
#endif

void SYS_SendEvent2(UINT32 event)
{
#if CHIP_HAS_BTCPU==1
    HAL_MAP_TABLE_T *table = *((HAL_MAP_TABLE_T**)0xa1c0027c);
    if(table == NULL)
        return;
    HAL_MAP_ACCESS_T *halmap = (HAL_MAP_ACCESS_T*)(table->modules[HAL_MAP_ID_HAL].access);
    if(halmap==NULL)
        return;

    if(halmap->eventMask != NULL && ((*(halmap->eventMask))&BT_EVENT))
    {
        rdabt_wait_xcpu_busy();
        BTCPU_SendEvent2(event);
        rdabt_clear_btcpu_busy();
    }
    
#else
    hal_HstSendEvent(BT_EVENT,event);
#endif
}


PUBLIC VOID SYS_ASSERT2(VOID)
{
	SYS_SendEvent2(0xb7a55e47);
	asm volatile("break 2");
}
