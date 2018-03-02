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


#include "stdio.h"                  /* Basic C I/O functions */
#include "rdabt_drv.h"
#include "cs_types.h"
#include "hal_timers.h"
#include "hal_sys.h"
#include "hal_sharemem.h"
#include "hal_bt.h"

#include "btd_config.h"
#include "tgt_btd_cfg.h"
#include "tgt_pmd_cfg.h"
#include "pmd_m.h"
#include "pmd_config.h"

extern CONST TGT_BTD_CONFIG_T* g_btdConfig;

#if COMBINED_HOST==0

#ifdef GPIO_2_INTF_GPIO_3_WORKAROUND
extern VOID pmd_EarDetectStable(BOOL stable);
#endif // GPIO_2_INTF_GPIO_3_WORKAROUND
#if defined(__RDA_CHIP_R16_5876P__) || defined(__RDA_CHIP_R12_5876__)
extern PUBLIC BOOL g_FMOpen_Flag;
extern PUBLIC BOOL g_BTOpen_Flag;
#endif
unsigned char rdabt_chip_sel = RDA_NONE_CHIP_ENUM;
//kal_uint8 bt_sleephdl =0xff;

const uint8 rdabt_hard_flow_ctrl[] = {0x01,0x02,0xfd,0x0a,0x00,0x01,0x10,0x00,0x00,0x50,0x22,0x01,0x00,0x00}; //hard flow ctrl enable 
const uint8 rdabt_CTS_flow_ctrl[] = {0x01,0x02,0xfd,0x0a,0x00,0x01,0x44,0x00,0x20,0x40,0x5f,0x00,0x00,0x00}; //hard flow ctrl enable 
const uint8 rdabt_aotu_baudrate_data[] = {0x01,0x77,0xfc,0x00};

void rdabt_audio_setting(UINT8 audioPath, UINT8 audioDataType);
void rda_bt_pin_to_high(void);


////////////////////////////////////////io/////////////////////////////////
void RDA_bt_Power_On_Reset(void)
{   
	hal_GpioClr(g_btdConfig->pinReset);
	RDABT_DELAY(10);   
	hal_GpioSet(g_btdConfig->pinReset);
}

void RDA_bt_Power_Off(void)
{
    ////kal_prompt_trace(0,"RDA_bt_Power_Off");
#ifdef __RDA_SHARE_CRYSTAL__
//   GPIO_ModeSetup(rda_bt_clkreq,0);
//   GPIO_InitIO(0,rda_bt_clkreq);
#endif

   hal_GpioClr(g_btdConfig->pinReset);
#if defined(__RDA_CHIP_R16_5876P__) || defined(__RDA_CHIP_R12_5876__)
   if(g_FMOpen_Flag == FALSE)
   {
       pmd_EnablePower(PMD_POWER_BT, FALSE);
   }
   else
   {   //FM on
       RDABT_5876p_btoff_fmon_patch();
   }
   g_BTOpen_Flag = FALSE;
#else
    pmd_EnablePower(PMD_POWER_BT, FALSE);
#if CHIP_HAS_BTCPU==1
    hal_BtStop();
#endif
#endif
#ifdef __RDABT_USE_IIC__
//   RDA_bt_SerialCommdeInit(); 
#endif   
}







#if defined( __RDA_AUTO_CHIP_5868__)
unsigned int rda5868_chipid = 0;
const unsigned int chipidaddr = 0x40200028;
uint8 rda5868_read_chip[10] = {0x01,0x01,0xfd,0x06,0x00,0x01,0x28,0x00,0x20,0x40};

uint8 rdabt_get_chipid(void)
{
	rdabt_iic_core_read_data(chipidaddr,&rda5868_chipid,1);
	//kal_prompt_trace(0,"rda5868_chipid = %x",rda5868_chipid);
	if((rda5868_chipid & 0x00ffff) == 0x5868)
	{
	    if(((rda5868_chipid>>16)&0x000000ff) == 0x11)
			return RDA_BT_R7_5868PLUS_ENUM;
	    else if(((rda5868_chipid>>16)&0x000000ff) == 0x03 || ((rda5868_chipid>>16)&0x000000ff) == 0x01 )
			return RDA_BT_R5_5868_ENUM;
           else 
		   	return RDA_NONE_CHIP_ENUM;
	}
	else if((rda5868_chipid & 0x00ffff) == 0x5872)
	{
	    if(((rda5868_chipid>>16)&0x000000ff) == 0x00)
			return RDA_BT_R10_5868E_ENUM;
	    else if(((rda5868_chipid>>16)&0x000000ff) == 0x01)
			return RDA_BT_R10_AL_5868H_ENUM;
	    else 
			return RDA_NONE_CHIP_ENUM;
	}
	else
	{
	    return RDA_NONE_CHIP_ENUM;
       }

}


#elif defined(__RDA_AUTO_CHIP_587x__)

unsigned short rdabt_chipid,rdabt_revid;
unsigned char pageaddr = 0x3f;
unsigned short pagevalue = 0x0001;
unsigned short pagevalue1 = 0x0000;

const unsigned char chipidaddr = 0x20;
const unsigned char revidaddr = 0x21;

bool rdabt_read_rssi(int8 *rssi)
{
    uint32 rssiaddr = 0x8000043c;//0x34;
    uint32 rssi_raw;
	//rdabt_iic_rf_write_data(pageaddr,&pagevalue,1);
	rdabt_iic_core_read_data(rssiaddr,&rssi_raw,1);
	//rdabt_iic_rf_write_data(pageaddr,&pagevalue1,1);
    EDRV_TRACE(EDRV_BTD_TRC, 0, "rssi_raw = %x", rssi_raw);
    if((rssi_raw&0xffffff00) == 0x5500)
    {
        *rssi = (int8)(rssi_raw&0xff);
        return TRUE;
    }
    return FALSE;
}

uint8 rdabt_get_chipid(void)
{
	rdabt_iic_rf_write_data(pageaddr,&pagevalue,1);
	rdabt_iic_rf_read_data(chipidaddr,&rdabt_chipid,1);
	rdabt_iic_rf_read_data(revidaddr,&rdabt_revid,1);	
	rdabt_iic_rf_write_data(pageaddr,&pagevalue1,1);
    EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_chipid = %x",rdabt_chipid);
	hal_HstSendEvent(SYS_EVENT,0xaabbccdd);
	hal_HstSendEvent(SYS_EVENT,rdabt_chipid);

    if(rdabt_chipid == 0x5872)
    {
        if((rdabt_revid & 0x001f)<0x10)
            return RDA_BT_R10_5872_ENUM;
        else if((rdabt_revid & 0x0018)==0x10)
            return RDA_BT_R10_AL_5872H_ENUM;
        else if((rdabt_revid & 0x0018)==0x18)
            return RDA_BT_R10_5870E_ENUM;
        else
            return RDA_NONE_CHIP_ENUM;
    }
    else if(rdabt_chipid == 0x5875)
    {
#if 0
#ifdef __RDA_CHIP_R17_8809__
        return RDA_BT_R17_8809_ENUM;
#else
        return RDA_BT_R11_5875_ENUM;
#endif
#else
        const unsigned int chipId2Addr = 0x40200028;
        unsigned int chipId2;
        rdabt_iic_core_read_data(chipId2Addr,&chipId2,1);
        EDRV_TRACE(EDRV_BTD_TRC, 0, "chipId2 = %x",chipId2);
        
        if(chipId2 == 0x12005990)
            return RDA_BT_R18_8809E_ENUM;
        else if(chipId2 == 0x11005990)
            return RDA_BT_R17_8809_ENUM;
        return RDA_BT_R11_5875_ENUM;
#endif
    }
    else if(rdabt_chipid == 0x5876)
        return RDA_BT_R12_5876_ENUM;
	else if(rdabt_chipid == 0x587f)
		return RDA_BT_R16_5876P_ENUM;	
    else if(rdabt_chipid == 0x5990)
        return RDA_BT_R12_5990_ENUM;
    else if(rdabt_chipid == 0xffff || rdabt_chipid == 0xff00)
        return RDA_BT_R11_5872P_ENUM;
    else
        return RDA_BT_R8_5870_ENUM;
}


#elif defined(__RDA_AUTO_CHIP_5875__)

unsigned short rdabt_chipid = 0,rdabt_revid = 0;
 unsigned char pageaddr = 0x3f;
 unsigned short pagevalue = 0x0001;

const unsigned char chipidaddr = 0x20;
const unsigned char revidaddr = 0x21;



uint8 rdabt_get_chipid(void)
{
    rdabt_iic_rf_write_data(pageaddr,&pagevalue,1);
    rdabt_iic_rf_read_data(chipidaddr,&rdabt_chipid,1);
    rdabt_iic_rf_read_data(revidaddr,&rdabt_revid,1);	
    //kal_prompt_trace(0," __RDA_AUTO_CHIP_5875__ rdabt_chipid = %x",rdabt_chipid);

    if(rdabt_chipid == 0x5875)
        return RDA_BT_R11_5875_ENUM; 
    else if(rdabt_chipid == 0x5876)
        return RDA_BT_R12_5876_ENUM; 
	else if(rdabt_chipid == 0x587f)
	    return RDA_BT_R16_5876P_ENUM;	
    else if(rdabt_chipid == 0x5990)
        return RDA_BT_R12_5990_ENUM; 
    else
        return RDA_NONE_CHIP_ENUM;         
}
#endif


#ifdef __RDA_SOFT_RESET__
const unsigned short rda5400_swreset[][2] = 
{
{0x30,0x28},
{0x30,0x29}
};

void rdabt_sw_reset_write(void)
{
	rdabt_iic_rf_write_data(rda5400_swreset[0][0],&rda5400_swreset[0][1],1);
	RDABT_DELAY(30);
	rdabt_iic_rf_write_data(rda5400_swreset[1][0],&rda5400_swreset[1][1],1);
	RDABT_DELAY(10);
}

#endif

void rda_bt_32k_on(void)
{
// xiaoyifeng must note
}


void rda_bt_pin_setup(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
	hal_GpioSet(g_btdConfig->pinReset);
	
	RDABT_DELAY(5);

	pmd_EnablePower(PMD_POWER_BT, TRUE);
	I2C_Open();
}

void rda_bt_pin_to_high(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
	RDABT_DELAY(5);   
	hal_GpioSet(g_btdConfig->pinReset);
	RDABT_DELAY(70);   
}

void rda_bt_ldoon_toggle_high(void)
{
	pmd_EnablePower(PMD_POWER_BT, TRUE);
	RDABT_DELAY(1);   
}
void rda_bt_reset_to_high(void)
{
	hal_GpioSet(g_btdConfig->pinReset);
	RDABT_DELAY(1);   
}


void rda_bt_pin_to_low(void)
{
#if defined(__RDA_CHIP_R16_5876P__) || defined(__RDA_CHIP_R12_5876__)
    //FM open, LDO must not be off.         
    if(g_FMOpen_Flag == FALSE)
#endif
    {
	    pmd_EnablePower(PMD_POWER_BT, FALSE);
    }
	RDABT_DELAY(1);
	hal_GpioClr(g_btdConfig->pinReset);
	RDABT_DELAY(5);
}


uint8 rda_bt_get_chip_sel(void)
{
#ifdef __RDABT_USE_IIC__   
    rdabt_chip_sel = 0;
#if defined( __RDA_AUTO_CHIP_5868__) || defined(__RDA_AUTO_CHIP_587x__)
    rdabt_chip_sel = rdabt_get_chipid();
    rdabt_chip_sel = rdabt_get_chipid();
    hal_HstSendEvent(SYS_EVENT,0x20160101);
	hal_HstSendEvent(SYS_EVENT,rdabt_chip_sel);
    EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_chip_sel = %d",rdabt_chip_sel);
    RDABT_DELAY(1);       
#else
#ifdef __RDA_CHIP_R5_5868__
	rdabt_chip_sel = RDA_BT_R5_5868_ENUM;
#endif///__RDA_CHIP_R5_5868__

#ifdef __RDA_CHIP_R7_5868plus__
	rdabt_chip_sel = RDA_BT_R7_5868PLUS_ENUM;	
#endif//__RDA_CHIP_R7_5868plus__

#ifdef __RDA_CHIP_R8_5870__
	rdabt_chip_sel = RDA_BT_R8_5870_ENUM;	
#endif//__RDA_CHIP_R8_5870__

#ifdef __RDA_CHIP_R10_5868E__
	rdabt_chip_sel = RDA_BT_R10_5868E_ENUM;	
#endif//__RDA_CHIP_R10_5868E__

#ifdef __RDA_CHIP_R10_5872__
	rdabt_chip_sel = RDA_BT_R10_5872_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5872H__
	rdabt_chip_sel = RDA_BT_R10_AL_5872H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5868H__
	rdabt_chip_sel = RDA_BT_R10_AL_5868H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5870E__
	rdabt_chip_sel = RDA_BT_R10_5870E_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5872P__
	rdabt_chip_sel = RDA_BT_R11_5872P_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5875__
	rdabt_chip_sel = RDA_BT_R11_5875_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5876__
	rdabt_chip_sel = RDA_BT_R12_5876_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5870P__
	rdabt_chip_sel = RDA_BT_R12_5870P_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5876M__
	rdabt_chip_sel = RDA_BT_R12_5876M_ENUM;
#endif
#ifdef __RDA_CHIP_R16_5876P__
	rdabt_chip_sel = RDA_BT_R16_5876P_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5990__
	rdabt_chip_sel = RDA_BT_R12_5990_ENUM;
#endif
#ifdef __RDA_CHIP_R17_8809__
	rdabt_chip_sel = RDA_BT_R17_8809_ENUM;
#endif
#ifdef __RDA_CHIP_R18_8809E__
	rdabt_chip_sel = RDA_BT_R18_8809E_ENUM;
#endif
#endif//__RDA_AUTO_CHIP__
	   
#ifdef __RDA_SOFT_RESET__
    if(rdabt_chip_sel == RDA_BT_R7_5868PLUS_ENUM|| rdabt_chip_sel == RDA_BT_R5_5868_ENUM)
        rdabt_sw_reset_write();
#endif

#else //__RDABT_USE_IIC__

#ifdef __RDA_CHIP_R5_5868__
	rdabt_chip_sel = RDA_BT_R5_5868_ENUM;
#endif///__RDA_CHIP_R5_5868__

#ifdef __RDA_CHIP_R7_5868plus__
	rdabt_chip_sel = RDA_BT_R7_5868PLUS_ENUM;	
#endif//__RDA_CHIP_R7_5868plus__

#ifdef __RDA_CHIP_R8_5870__
	rdabt_chip_sel = RDA_BT_R8_5870_ENUM;	
#endif//__RDA_CHIP_R8_5870__

#ifdef __RDA_CHIP_R10_5868E__
	rdabt_chip_sel = RDA_BT_R10_5868E_ENUM;	
#endif//__RDA_CHIP_R10_5868E__

#ifdef __RDA_CHIP_R10_5872__
	rdabt_chip_sel = RDA_BT_R10_5872_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5872H__
	rdabt_chip_sel = RDA_BT_R10_AL_5872H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5868H__
	rdabt_chip_sel = RDA_BT_R10_AL_5868H_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R10_5870E__
	rdabt_chip_sel = RDA_BT_R10_5870E_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5872P__
	rdabt_chip_sel = RDA_BT_R11_5872P_ENUM;
#endif//__RDA_CHIP_R10_5872__

#ifdef __RDA_CHIP_R11_5875__
	rdabt_chip_sel = RDA_BT_R11_5875_ENUM;
#endif //__RDA_CHIP_R11_5875__

#ifdef __RDA_CHIP_R12_5876__
	rdabt_chip_sel = RDA_BT_R12_5876_ENUM;
#endif ////__RDA_CHIP_R12_5876__
#ifdef __RDA_CHIP_R12_5870P__
	rdabt_chip_sel = RDA_BT_R12_5870P_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5876M__
	rdabt_chip_sel = RDA_BT_R12_5876M_ENUM;
#endif
#ifdef __RDA_CHIP_R16_5876P__
	rdabt_chip_sel = RDA_BT_R16_5876P_ENUM;
#endif
#ifdef __RDA_CHIP_R12_5990__
	rdabt_chip_sel = RDA_BT_R12_5990_ENUM;
#endif ////__RDA_CHIP_R12_5990__

#endif   //__RDABT_USE_IIC__

    if(rdabt_chip_sel == RDA_NONE_CHIP_ENUM)
    {
//   	ASSERT(0);	//xiaoyifeng
    }

    return rdabt_chip_sel;
}


void RDA_5990_reg_read(void)
{
#ifdef __RDA_CHIP_R12_5990__

	unsigned short data_temp[6] = {0,0,0,0,0,0};

	rdabt_iic_rf_write_data(pageaddr,&pagevalue,1);
	rdabt_iic_rf_read_data(0x26,&data_temp[0],1);
	rdabt_iic_rf_read_data(0x28,&data_temp[1],1);	
	rdabt_iic_rf_write_data(pageaddr,&pagevalue1,1);
	rdawifi_iic_rf_write_data(pageaddr,&pagevalue1,1);
	rdawifi_iic_rf_read_data(0x30,&data_temp[2],1);	
	
	rdawifi_iic_rf_write_data(pageaddr,&pagevalue,1);
	rdawifi_iic_rf_read_data(0x39,&data_temp[3],1);		
	rdawifi_iic_rf_write_data(pageaddr,&pagevalue1,1);
	
	hal_HstSendEvent(SYS_EVENT,0x58685876);
	hal_HstSendEvent(SYS_EVENT,data_temp[0]);	
	hal_HstSendEvent(SYS_EVENT,data_temp[1]);	
	hal_HstSendEvent(SYS_EVENT,data_temp[2]);	
	hal_HstSendEvent(SYS_EVENT,data_temp[3]);
#endif
}

extern void RDA_5990_bt_dig_reset(void);

#if CHIP_HAS_BTCPU==1
extern VOID _bt_Irq_handler(VOID);
extern int bt_sys_main (void);
//static UINT32 btcpu_stack[512];
static struct {
	UINT32 flag;
	UINT32 epc;
	UINT32 cause;
	UINT32 ra;
	UINT32 bad;
} BT_FUNC_INTERNAL btcpu_status;
//extern UINT8 *DATA_BUFF;


VOID BT_FUNC_INTERNAL bt_irq(UINT32 epc, UINT32 cause, UINT32 ra, UINT32 bad)
{
	int count = 0;

    //mon_Event(0xbbbbbbb0);
    //mon_Event(cause);
    //mon_Event(epc);
	//mon_Event(ra);
	//mon_Event(bad);
	btcpu_status.flag = 1;
	btcpu_status.epc =epc;
	btcpu_status.cause =cause;
	btcpu_status.ra =ra;
	btcpu_status.bad =bad;
	while(1);
	
    count++;
    //if (count >= 5) while(1);
}


#endif

void rda_bt_power_on(void)
{
#if CHIP_HAS_BTCPU==0
    rda_bt_pin_setup();
#endif	
#if CHIP_HAS_BTCPU==1
	pmd_EnablePower(PMD_POWER_BT, TRUE);

	hal_TimDelay(200 MS_WAITING);
	//memset(btcpu_stack, 0xfdfdfdfd, sizeof(btcpu_stack));
	//if(DATA_BUFF == NULL)
	//	DATA_BUFF = COS_Malloc_NoCache(10*1024);
	hal_BtStart(NULL, NULL, NULL);
#ifdef BT_SHAREMEM_TRANSPORT
	hal_BtSetHostIrqHandler(hal_Sharemem_Interrupt_Handle);
#endif
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5855)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_5856)
	rdabt_chip_sel = RDA_BT_R20_5855_ENUM;
#else
	rdabt_chip_sel = RDA_BT_R19_8809P_ENUM;
#endif
	hal_TimDelay(200 MS_WAITING);

	EDRV_TRACE(EDRV_BTD_TRC, 0, "rda_bt_power_on CHIP_HAS_BTCPU rdabt_chip_sel:%d", rdabt_chip_sel);
	
	return;
#endif

#ifndef __RDA_CHIP_R17_8809__
    rda_bt_32k_on();
#ifdef __RDABT_USE_IIC__   
//   RDA_bt_SerialCommInit(); 
    RDABT_DELAY(1);
#endif
    rda_bt_ldoon_toggle_high();
    rda_bt_pin_to_low();
#ifdef __RDA_BT_5868_SERIES__
    rda_bt_pin_to_high();
#endif
#endif // !__RDA_CHIP_R17_8809__

    RDABT_DELAY(4);
    rda_bt_get_chip_sel();

#ifdef __RDA_BT_5868_SERIES__
    RDA_bt_Power_On_Reset();
    RDABT_DELAY(70);
#endif
#ifdef __RDA_CHIP_R12_5990__
	RDA_5990_power_off();
     /*to make sure bt use ldo when fm is on*/
      	RDA5990bt_wf_dcdc_ldo_trans();	 
	RDA_5990_power_on();	
    RDABT_DELAY(10);
	RDA_5990_clock_en();
     RDABT_DELAY(20);
   /*add bt dig reset to ensure bt dig powerup safely*/
    RDA_5990_bt_dig_reset();
     RDABT_DELAY(400);

	RDA_5990_reg_read();
#endif

#ifdef USE_VOC_CVSD
	rdabt_audio_setting(1,1);
#else
	rdabt_audio_setting(1,0);
#endif
}


void rda_bt_rf_init(void)
{
#ifdef __RDABT_USE_IIC__   
    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R7_5868plus__	   
    case RDA_BT_R7_5868PLUS_ENUM:
        RDABT_rf_Intialization_r7(); 		
	 break;
#endif
#ifdef __RDA_CHIP_R5_5868__	   
    case RDA_BT_R5_5868_ENUM:
       RDABT_rf_Intialization_r5();
	break;
#endif
#ifdef __RDA_CHIP_R8_5870__	   
    case RDA_BT_R8_5870_ENUM:
        RDABT_rf_Intialization_r8();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868E__	   
    case RDA_BT_R10_5868E_ENUM:
        RDABT_rf_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5872__	   
    case RDA_BT_R10_5872_ENUM:
        RDABT_rf_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868H__
    case RDA_BT_R10_AL_5868H_ENUM:
        RDABT_rf_Intialization_r10_al();   	   
        break;
#endif
#ifdef __RDA_CHIP_R10_5872H__	   
    case RDA_BT_R10_AL_5872H_ENUM:
        RDABT_rf_Intialization_r10_al();
        break;
#endif
#ifdef __RDA_CHIP_R10_5870E__
    case RDA_BT_R10_5870E_ENUM:
        RDABT_rf_Intialization_r10_e();
        break;
#endif
#ifdef __RDA_CHIP_R11_5872P__
    case RDA_BT_R11_5872P_ENUM:
	 break; 
#endif

#ifdef __RDA_CHIP_R11_5875__
    case RDA_BT_R11_5875_ENUM:
	 RDABT_rf_Intialization_r11();	
	 break; 
#endif
#ifdef __RDA_CHIP_R12_5876__
    case RDA_BT_R12_5876_ENUM:
	 RDABT_rf_Intialization_r12();	
	 break; 
#endif
#ifdef __RDA_CHIP_R12_5870P__
    case RDA_BT_R12_5870P_ENUM:
	 RDABT_rf_Intialization_r12();	
	 break; 
#endif
#ifdef __RDA_CHIP_R12_5876M__
    case RDA_BT_R12_5876M_ENUM:
	 break; 
#endif
#ifdef __RDA_CHIP_R16_5876P__
    case RDA_BT_R16_5876P_ENUM:
        if(g_FMOpen_Flag == FALSE)
        {   //FM off
            pmd_EnablePower(PMD_POWER_BT, TRUE);
            RDABT_5876p_bton_fmoff_patch();
        }
        else
        {   //FM on, not need LDO on again
            RDABT_5876p_bton_fmon_patch();
        }
        g_BTOpen_Flag = TRUE;
        break;
#endif
#ifdef __RDA_CHIP_R12_5990__
    case RDA_BT_R12_5990_ENUM:
	 RDABT_rf_Intialization_r12();	
	 break; 
#endif

#ifdef __RDA_CHIP_R17_8809__
	case RDA_BT_R17_8809_ENUM:
	 RDABT_rf_Intialization_r17();	
	 break; 
#endif

#ifdef __RDA_CHIP_R18_8809E__
	case RDA_BT_R18_8809E_ENUM:
	 RDABT_rf_Intialization_r18();	
	 break; 
#endif

#ifdef __RDA_CHIP_R19_8809P__
	case RDA_BT_R19_8809P_ENUM:
	 //RDABT_rf_Intialization_r19();	
	 break; 
#endif

#ifdef __RDA_CHIP_R20_5855__
	case RDA_BT_R20_5855_ENUM:
	 //RDABT_rf_Intialization_r20();	
	 break; 
#endif

    default:
        break;

    }

#endif//__RDA5868_USE_IIC__

#if defined( __RDA_BT_587x_SERIES__ ) || defined(__RDA_BT_5875_SERIES__) || defined(__RDA_BT_5876M_SERIES__)
	rda_bt_pin_to_high();
#endif



}

void rda_bt_uart_init(void)
{
    rdabt_uart_start();
    rdabt_uart_configure(115200,FALSE);
}

int rdabt_poweron_init(void)
{
	EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_poweron_init");
	EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_poweron_init  start %d", rdabt_chip_sel);

#ifdef GPIO_2_INTF_GPIO_3_WORKAROUND
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
    if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) != HAL_SYS_CHIP_PROD_ID_8808 &&
        hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) >= HAL_SYS_CHIP_METAL_ID_U02)
    {
        pmd_EarDetectStable(FALSE);
    }
#endif // 8808
#endif // GPIO_2_INTF_GPIO_3_WORKAROUND

#ifdef __RDA_SHARE_CRYSTAL__
//	if(bt_sleephdl == 0xff)
//		bt_sleephdl = L1SM_GetHandle();
//       L1SM_SleepDisable(bt_sleephdl);
#endif
	rda_bt_power_on();
	rda_bt_rf_init();
//	rda_bt_uart_init();


    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R7_5868plus__	   
    case RDA_BT_R7_5868PLUS_ENUM:
        RDABT_core_Intialization_r7(); 		
	 break;
#endif
#ifdef __RDA_CHIP_R5_5868__	   
    case RDA_BT_R5_5868_ENUM:
       RDABT_core_Intialization_r5();
	break;
#endif
#ifdef __RDA_CHIP_R8_5870__	   
    case RDA_BT_R8_5870_ENUM:
        RDABT_core_Intialization_r8();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868E__	   
    case RDA_BT_R10_5868E_ENUM:
        RDABT_core_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5872__	   
    case RDA_BT_R10_5872_ENUM:
        RDABT_core_Intialization_r10();
        break;
#endif
#ifdef __RDA_CHIP_R10_5868H__
    case RDA_BT_R10_AL_5868H_ENUM: 
        RDABT_core_Intialization_r10_al();   	   
        break;
#endif
#ifdef __RDA_CHIP_R10_5872H__	   
    case RDA_BT_R10_AL_5872H_ENUM:
        RDABT_core_Intialization_r10_al();
        break;
#endif
#ifdef __RDA_CHIP_R10_5870E__
    case RDA_BT_R10_5870E_ENUM:
        RDABT_core_Intialization_r10_e();
        break;
#endif
#ifdef __RDA_CHIP_R11_5872P__
    case RDA_BT_R11_5872P_ENUM:
        RDABT_core_Intialization_r11_p();
        break;
#endif

#ifdef __RDA_CHIP_R11_5875__
    case RDA_BT_R11_5875_ENUM:
        RDABT_core_Intialization_r11();
        break;
#endif

#ifdef __RDA_CHIP_R12_5876__
    case RDA_BT_R12_5876_ENUM:
        RDABT_core_Intialization_r12();
        break;
#endif
#ifdef __RDA_CHIP_R12_5870P__
    case RDA_BT_R12_5870P_ENUM:
	 	RDABT_core_Intialization_r12();	
	 break; 
#endif
#ifdef __RDA_CHIP_R12_5876M__
    case RDA_BT_R12_5876M_ENUM:
        RDABT_core_Intialization_r12_m();
	 break; 
#endif
#ifdef __RDA_CHIP_R16_5876P__
	case RDA_BT_R16_5876P_ENUM:
		RDABT_core_Intialization_r16();
	 break; 
#endif
#ifdef __RDA_CHIP_R12_5990__
	case RDA_BT_R12_5990_ENUM:
        RDABT_core_Intialization_r12();
		break;
#endif

#ifdef __RDA_CHIP_R17_8809__
	case RDA_BT_R17_8809_ENUM:
        RDABT_core_Intialization_r17();
		break;
#endif

#ifdef __RDA_CHIP_R18_8809E__
    case RDA_BT_R18_8809E_ENUM:
        RDABT_core_Intialization_r18();
        break;
#endif

    default:
       	return (-1);	
        break;

    }
	//just to ensure the I2C return to normal
	{
		unsigned int chipidaddress = 0x40200028;
		unsigned int rda5868_chipid = 0;
		rdabt_iic_core_read_data(chipidaddress,&rda5868_chipid,1);
	}

	// rdabt_uart_stop();    
	RDABT_DELAY(5);     

#ifdef GPIO_2_INTF_GPIO_3_WORKAROUND
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
    if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) != HAL_SYS_CHIP_PROD_ID_8808 &&
        hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) >= HAL_SYS_CHIP_METAL_ID_U02)
    {
        // Stable time is about 400 ms for most chips, and 2000 ms 
        // should work for all of them.
        COS_Sleep(2000);
        pmd_EarDetectStable(TRUE);
    }
#endif // 8808
#endif // GPIO_2_INTF_GPIO_3_WORKAROUND
#ifdef __RDA_SHARE_CRYSTAL__
//       L1SM_SleepEnable(bt_sleephdl);
#endif

		EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_poweron_init  end %d", rdabt_chip_sel);


	return (0);	
}

void rda_bt_phone_init(void)
{
    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R11_5875__
	case RDA_BT_R11_5875_ENUM:
		RDABT_phone_Intialization_r11();	
		break; 
#endif
#ifdef __RDA_CHIP_R12_5876__
	case RDA_BT_R12_5876_ENUM:
		RDABT_phone_Intialization_r12();
		break; 
#endif
#ifdef __RDA_CHIP_R16_5876P__
	case RDA_BT_R16_5876P_ENUM:
		RDABT_phone_Intialization_r16();
		break; 
#endif
#ifdef __RDA_CHIP_R12_5990__
	case RDA_BT_R12_5990_ENUM:

	RDA_5990_power_up_setting();
	RDA_5990_power_off();	
    RDABT_DELAY(10);
	RDA_5990_power_on();	
    RDABT_DELAY(10);
	RDA_5990_clock_en();	
    RDABT_DELAY(10);
	RDABT_phone_Intialization_r12();	
    RDABT_DELAY(10);
	RDA_5990_power_off();	
	break; 
#endif

    default:
        break;
    }
}

void rdabt_poweronphone_init(void)
{
    pmd_EnablePower(PMD_POWER_BT, TRUE);
    RDABT_DELAY(4);
    hal_GpioSet(g_btdConfig->pinReset);

#ifdef GPIO_2_BT_HOSTWAKE_WORKAROUND
    if (g_btdConfig->pinSleep.gpioId == HAL_GPIO_2)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
        if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) != HAL_SYS_CHIP_PROD_ID_8808 &&
            hal_SysGetChipId(HAL_SYS_CHIP_METAL_ID) < HAL_SYS_CHIP_METAL_ID_U02)
        {
            // GPIO_2 should output low level on 8808S U01
            hal_GpioSetOut(g_btdConfig->pinSleep.gpioId);
        }
        else
#endif
        {
            // GPIO_2 on other chips should be configured as input
            hal_GpioSetIn(g_btdConfig->pinSleep.gpioId);
        }
        hal_GpioClr(g_btdConfig->pinSleep);
    }
#endif // GPIO_2_BT_HOSTWAKE_WORKAROUND

#if defined(__RDA_CHIP_R11_5875__) || defined(__RDA_CHIP_R12_5876__) || defined(__RDA_CHIP_R12_5990__)
    I2C_Open();
    rda_bt_get_chip_sel();
    rda_bt_phone_init();
    I2C_Close();
#endif // __RDA_CHIP_R11_5875__ || __RDA_CHIP_R12_5876__
    hal_GpioClr(g_btdConfig->pinReset);
    pmd_EnablePower(PMD_POWER_BT, FALSE);
}

void rdabt_common_baudrate_flow_ctrl(void)
{
    char port = 1;

    uint16 num_send;	
    hal_HstSendEvent(SYS_EVENT,0x5868ffff);
    

#ifdef __RDA_CHIP_R12_5990__
    rdabt_uart_tx(rdabt_CTS_flow_ctrl,sizeof(rdabt_CTS_flow_ctrl)/sizeof(rdabt_CTS_flow_ctrl[0]),&num_send); 
#endif
    rdabt_adp_uart_tx(port, rdabt_aotu_baudrate_data,sizeof(rdabt_aotu_baudrate_data)/sizeof(rdabt_aotu_baudrate_data[0]),&num_send); 
    COS_Sleep(100);
    //DEVICE *pDev=&Uart_Dev[BT_UART_PORT];
    rdabt_adp_uart_configure(port, BT_UART_BAUDRATE/*460800*//*230400*/,FALSE);
    COS_Sleep(200);
    //pDev->uartCfg.afc = HAL_UART_AFC_MODE_RX_TRIG_16;
    rdabt_adp_uart_tx(port, rdabt_hard_flow_ctrl,sizeof(rdabt_hard_flow_ctrl)/sizeof(rdabt_hard_flow_ctrl[0]),&num_send); 
    COS_Sleep(200);
    rdabt_adp_uart_configure(port, BT_UART_BAUDRATE/*921600*//*460800*//*230400*/,TRUE);
}

void rdabt_baudrate_ctrl(void)
{
	//kal_prompt_trace(0,"rdabt_baudrate_ctrl");

    switch(rdabt_chip_sel)
    {
#ifdef __RDA_CHIP_R7_5868plus__	   
    case RDA_BT_R7_5868PLUS_ENUM:
	break;
#endif
#ifdef __RDA_CHIP_R5_5868__	   
    case RDA_BT_R5_5868_ENUM:
	break;
#endif
#ifdef __RDA_CHIP_R8_5870__	   
    case RDA_BT_R8_5870_ENUM:
    break;
#endif
#ifdef __RDA_CHIP_R10_5868E__	   
    case RDA_BT_R10_5868E_ENUM:
#endif
#ifdef __RDA_CHIP_R10_5872__	   
    case RDA_BT_R10_5872_ENUM:
#endif
#ifdef __RDA_CHIP_R10_5868H__
    case RDA_BT_R10_AL_5868H_ENUM: 
#endif
#ifdef __RDA_CHIP_R10_5872H__	   
    case RDA_BT_R10_AL_5872H_ENUM:
#endif
#ifdef __RDA_CHIP_R10_5870E__
    case RDA_BT_R10_5870E_ENUM:
#endif
#ifdef __RDA_CHIP_R11_5872P__
    case RDA_BT_R11_5872P_ENUM:
#endif
#ifdef __RDA_CHIP_R11_5875__
    case RDA_BT_R11_5875_ENUM:
#endif
#ifdef __RDA_CHIP_R12_5876__
    case RDA_BT_R12_5876_ENUM:
#endif
#ifdef __RDA_CHIP_R12_5870P__
	case RDA_BT_R12_5870P_ENUM:
#endif
#ifdef __RDA_CHIP_R12_5876M__
	case RDA_BT_R12_5876M_ENUM:
#endif
#ifdef __RDA_CHIP_R16_5876P__
	case RDA_BT_R16_5876P_ENUM:
#endif
#ifdef __RDA_CHIP_R12_5990__
	case RDA_BT_R12_5990_ENUM:
#endif
#ifdef __RDA_CHIP_R17_8809__
	case RDA_BT_R17_8809_ENUM:
#endif
    default:
	rdabt_common_baudrate_flow_ctrl();
	break;
    }
}

const uint32 rda_voice_over_hw_pcm[][2] =
{
	{0x80000070,0x00002000},
	{0x80000040,0x00001000},
};

const uint32 rda_voice_over_hw_uart[][2] =
{
	{0x80000070,0x00002080},
	{0x80000040,0x00001000},
};

const uint32 rda_voice_data_pcm[][2] =
{
	{0x80000084,0x9098c007},
	{0x80000040,0x00020000},
};

const uint32 rda_voice_data_cvsd[][2] =
{
	{0x80000084,0x9098c008},
	{0x80000040,0x00020000},
};

const uint8 rda_voice_over_hw_pcm_r18[] = {0x00,0x20,0x08,0x00};//pcm
const uint8 rda_voice_over_hw_uart_r18[] = {0x80,0x20,0x08,0x00};//UART

const uint8 rda_voice_data_pcm_r18[] = {0x07,0xc0,0x98,0x90};	//SHOUJI
const uint8 rda_voice_data_cvsd_r18[] = {0x08,0xc0,0x98,0x90};
void rdabt_audio_setting(UINT8 audioPath, UINT8 audioDataType)
{
	hal_HstSendEvent(SYS_EVENT,0x19892000);
	uint16 i=0;
		
	switch(rdabt_chip_sel)
	{
#ifdef __RDA_CHIP_R18_8809E__
	case RDA_BT_R18_8809E_ENUM:
		if (0==audioPath)
		{
			rdabt_write_pskey(0x15, rda_voice_over_hw_pcm_r18, sizeof(rda_voice_over_hw_pcm_r18));
		}
		else if (1==audioPath)
		{
			rdabt_write_pskey(0x15, rda_voice_over_hw_uart_r18, sizeof(rda_voice_over_hw_uart_r18));
		}

		if (0==audioDataType)
		{
			rdabt_write_pskey(0x17, rda_voice_data_pcm_r18, sizeof(rda_voice_data_pcm_r18));

		}
		else if (1==audioDataType)
		{
			rdabt_write_pskey(0x17, rda_voice_data_cvsd_r18, sizeof(rda_voice_data_cvsd_r18));
		}		
		break;
#endif		
	default:
		if (0==audioPath)
		{
			for(i=0;i<sizeof(rda_voice_over_hw_pcm)/sizeof(rda_voice_over_hw_pcm[0]);i++)
				rdabt_wirte_memory(rda_voice_over_hw_pcm[i][0],&rda_voice_over_hw_pcm[i][1],1,0x0);
		}
		else if (1==audioPath)
		{
			for(i=0;i<sizeof(rda_voice_over_hw_uart)/sizeof(rda_voice_over_hw_uart[0]);i++)
				rdabt_wirte_memory(rda_voice_over_hw_uart[i][0],&rda_voice_over_hw_uart[i][1],1,0x0);
		}

		if (0==audioDataType)
		{
			for(i=0;i<sizeof(rda_voice_data_pcm)/sizeof(rda_voice_data_pcm[0]);i++)
				rdabt_wirte_memory(rda_voice_data_pcm[i][0],&rda_voice_data_pcm[i][1],1,0x0);
		}
		else if (1==audioDataType)
		{
			for(i=0;i<sizeof(rda_voice_data_cvsd)/sizeof(rda_voice_data_cvsd[0]);i++)
				rdabt_wirte_memory(rda_voice_data_cvsd[i][0],&rda_voice_data_cvsd[i][1],1,0x0);
		}  
		break;
	}
}

#else

void rdabt_poweronphone_init(void)
{
}

void rdabt_Sharemem_Interrupt_Handle(void);

void rda_bt_power_on(void)
{
        if(!tgt_GetPmdConfig()->power[PMD_POWER_BT].powerOnState)
        {
	    pmd_EnablePower(PMD_POWER_BT, TRUE);

	    hal_TimDelay(200 MS_WAITING);
	    hal_BtStart(NULL, NULL, NULL);
	    hal_TimDelay(200 MS_WAITING);
         }
#ifdef BT_SHAREMEM_TRANSPORT
	 hal_BtSetHostIrqHandler(hal_Sharemem_Interrupt_Handle);
#elif CHIP_HAS_BTCPU==1
        hal_BtSetHostIrqHandler(rdabt_Sharemem_Interrupt_Handle);
#endif

	return;
}

int rdabt_poweron_init(void)
{
	EDRV_TRACE(EDRV_BTD_TRC, 0, "rdabt_poweron_init");

	rda_bt_power_on();

	RDABT_DELAY(5);     

	return (0);	
}
void RDA_bt_Power_Off(void)
{
   hal_GpioClr(g_btdConfig->pinReset);
    pmd_EnablePower(PMD_POWER_BT, FALSE);

    hal_BtStop();

}

#endif

