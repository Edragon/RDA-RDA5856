//------------------------------------------------------------------------
// Copyright (C) 2016 RDA microelectronic Inc.                                           
// All Rights Reserved                                                    
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of RDA microelectronic Inc.                
// The copyright notice above does not evidence any actual or intended    
// publication of such source code.                                       
// No part of this code may be reproduced, stored in a retrieval system,  
// or transmitted, in any form or by any means, electronic, mechanical,   
// photocopying, recording, or otherwise, without the prior written       
// permission of RDA microelectronic Inc.                                                 
//------------------------------------------------------------------------
// File name      : pmuc_asm                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-14 15:57:43                                                  
// Last modified  : 2016-12-14 15:57:43                                                  
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-14 15:57:43 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __PMUC_ASM_H__
#define __PMUC_ASM_H__
//Auto-gen by reg_gen
#include "base_address.h"

#define REG_test_cfg                   REG_PMUC_BASE+0x00  
#define REG_pwr_sys_ctrl               REG_PMUC_BASE+0x04  
#define REG_pm_fsm_status              REG_PMUC_BASE+0x08  
#define REG_pm_fsm_frc_ctrl1           REG_PMUC_BASE+0xc   
#define REG_xtal_clk32k_div            REG_PMUC_BASE+0x10  
#define REG_xtal_cfg_ctrl              REG_PMUC_BASE+0x14  
#define REG_ldo_ctrl                   REG_PMUC_BASE+0x18  
#define REG_bandgap_cfg                REG_PMUC_BASE+0x1c  
#define REG_buck_cp_ctrl               REG_PMUC_BASE+0x20  
#define REG_ldo_cfg                    REG_PMUC_BASE+0x24  
#define REG_charger_cfg_ctrl           REG_PMUC_BASE+0x28  
#define REG_ana_status                 REG_PMUC_BASE+0x2c  
#define REG_pmu_ana_rsvd               REG_PMUC_BASE+0x30  
#define REG_sw_rsvd                    REG_PMUC_BASE+0x34  
#define REG_switch_mode                REG_PMUC_BASE+0x38  

//test_cfg
#define PMUC_SCAN_MODE0                (1<<0)              
#define PMUC_SCAN_MODE1                (1<<1)              
#define PMUC_SCAN_RSTB                 (1<<2)              
#define PMUC_BIST_MODE0                (1<<3)              
#define PMUC_BIST_MODE1                (1<<4)              
#define PMUC_FUNC_TEST_MODE            (1<<5)              
#define PMUC_EDT_BYPASS                (1<<6)              
#define PMUC_OCC_LF_DC_MODE            (1<<7)              
#define PMUC_OCC_HF_DC_MODE            (1<<8)              
#define PMUC_SPIFLASH_TEST_MODE        (1<<9)              

//pwr_sys_ctrl
#define PMUC_KEYSENSE_ENB              (1<<0)              
#define PMUC_SELF_RESETN               (1<<1)              
#define PMUC_PM_REG(n)                 (((n)&0x7)<<2)      
#define PMUC_LP_MODE_B                 (1<<5)              
#define PMUC_KEY_THLD(n)               (((n)&0x3)<<6)      
#define PMUC_CLK_FINE_SEL              (1<<8)              
#define PMUC_CLK_RAW_SEL               (1<<9)              
#define PMUC_CLK_PMUC_SEL              (1<<10)             
#define PMUC_CHR_AC_ON_RESET_BYPASS    (1<<11)             
#define PMUC_LOWBAT_OFF_BYPASS         (1<<12)             
#define PMUC_XTAL_RDY_TIME_SEL(n)      (((n)&0x7)<<13)     
#define PMUC_CHR_AC_ON_TIME_SEL(n)     (((n)&0x3)<<16)     
#define PMUC_I2CS_DISABLE              (1<<18)             
#define PMUC_HST_CLK_DISABLE           (1<<19)             
#define PMUC_TAB_RST_BYPASS            (1<<20)             

//pm_fsm_status
#define PMUC_PM_FSM_STATE(n)           (((n)&0x7F)<<0)     
#define PMUC_PWRSWITCH_EN              (1<<7)              
#define PMUC_PU_BG_PMU                 (1<<8)              
#define PMUC_PU_XTAL_26M_LV            (1<<9)              
#define PMUC_PU_MEM_GRP1               (1<<10)             
#define PMUC_PU_MEM_GRP2               (1<<11)             
#define PMUC_PU_MEM_GRP3               (1<<12)             
#define PMUC_PU_BUCK_LDO               (1<<13)             
#define PMUC_PU_LDO_VCORE              (1<<14)             
#define PMUC_RSTB_DBB                  (1<<15)             
#define PMUC_VRTC_OUT_SEL              (1<<16)             
#define PMUC_PWRSWITCH_EN_DLY          (1<<17)             
#define PMUC_PU_BT_AON                 (1<<18)             

//pm_fsm_frc_ctrl1
#define PMUC_PWRSWITCH_EN_REG          (1<<0)              
#define PMUC_PWRSWITCH_EN_DR           (1<<1)              
#define PMUC_PU_BG_REG                 (1<<2)              
#define PMUC_PU_BG_DR                  (1<<3)              
#define PMUC_PU_26M_REG                (1<<4)              
#define PMUC_PU_26M_DR                 (1<<5)              
#define PMUC_PU_MEM_GRP1_REG           (1<<6)              
#define PMUC_PU_MEM_GRP1_DR            (1<<7)              
#define PMUC_PU_MEM_GRP2_REG           (1<<8)              
#define PMUC_PU_MEM_GRP2_DR            (1<<9)              
#define PMUC_PU_MEM_GRP3_REG           (1<<10)             
#define PMUC_PU_MEM_GRP3_DR            (1<<11)             
#define PMUC_PU_BUCK_LDO_REG           (1<<12)             
#define PMUC_PU_BUCK_LDO_DR            (1<<13)             
#define PMUC_PU_LDO_VCORE_REG          (1<<14)             
#define PMUC_PU_LDO_VCORE_DR           (1<<15)             
#define PMUC_RSTB_DBB_REG              (1<<16)             
#define PMUC_RSTB_DBB_DR               (1<<17)             
#define PMUC_VRTC_OUT_SEL_REG          (1<<18)             
#define PMUC_VRTC_OUT_SEL_DR           (1<<19)             
#define PMUC_PWRSWITCH_EN_DLY_REG      (1<<20)             
#define PMUC_PWRSWITCH_EN_DLY_DR       (1<<21)             
#define PMUC_PU_BT_AON_REG             (1<<22)             
#define PMUC_PU_BT_AON_DR              (1<<23)             
#define PMUC_MEM_GRP1_ON               (1<<24)             
#define PMUC_MEM_GRP2_ON               (1<<25)             
#define PMUC_MEM_GRP3_ON               (1<<26)             

//xtal_clk32k_div
#define PMUC_STEP_OFFSET_LP(n)         (((n)&0xFF)<<0)     
#define PMUC_STEP_OFFSET_NORMAL(n)     (((n)&0xFF)<<8)     
#define PMUC_STEP_OFFSET_UPDATE        (1<<16)             
#define PMUC_DIV_LP_MODE_H             (1<<17)             
#define PMUC_DIV_LP_MODE_H_REG         (1<<18)             
#define PMUC_DIV_LP_MODE_H_DR          (1<<19)             
#define PMUC_XTAL_VAMP_IBIT_XOSC_ACTIVE(n) (((n)&0xF)<<20)     
#define PMUC_XTAL_FIX_IBIT_XOSC_ACTIVE(n) (((n)&0x1F)<<24)    

//xtal_cfg_ctrl
#define PMUC_XTAL_VAMP_IBIT_XOSC_LP(n) (((n)&0xF)<<0)      
#define PMUC_XTAL_REG_BIT_LV(n)        (((n)&0xF)<<4)      
#define PMUC_XTAL_CAPBANK_BIT_LV(n)    (((n)&0xF)<<8)      
#define PMUC_XTAL_CAP_FINE_LV(n)       (((n)&0x3F)<<12)    
#define PMUC_XTAL_FIX_IBIT_XOSC_LP(n)  (((n)&0x1F)<<18)    
#define PMUC_XTAL_CLK_LP_EN_LV         (1<<23)             
#define PMUC_PU_XTAL_26M_REG           (1<<24)             
#define PMUC_PU_XTAL_26M_DR            (1<<25)             
#define PMUC_XTAL_LP_MODE_EN_REG       (1<<26)             
#define PMUC_XTAL_LP_MODE_EN_DR        (1<<27)             

//ldo_ctrl
#define PMUC_LDO_ULP_VPAD_VBIT(n)      (((n)&0x7)<<0)      
#define PMUC_LDO_ULP_VRTC_VBIT(n)      (((n)&0x3)<<3)      
#define PMUC_VBUCK_LDO_BIT(n)          (((n)&0x7)<<5)      
#define PMUC_LDO_VCORE_VBIT(n)         (((n)&0x7)<<8)      
#define PMUC_VOUT_SEL_BUCK_LDO         (1<<11)             
#define PMUC_LDO_VCORE_DISCHARGE_EN    (1<<12)             

//bandgap_cfg
#define PMUC_BG_CAL_TC_BIT(n)          (((n)&0x1F)<<0)     
#define PMUC_BG_FREQ_BIT(n)            (((n)&0x3)<<5)      
#define PMUC_BG_TS_I_BIT(n)            (((n)&0xF)<<7)      
#define PMUC_PU_TS_BG                  (1<<11)             
#define PMUC_LP_MODE_BG                (1<<12)             
#define PMUC_BG_CHOPPER_ENB            (1<<13)             
#define PMUC_REF_SEL_BG                (1<<14)             

//buck_cp_ctrl
#define PMUC_PU_BUCK                   (1<<0)              
#define PMUC_PFM_MODE_SEL_BUCK         (1<<1)              
#define PMUC_VBUCK_BIT(n)              (((n)&0xF)<<2)      
#define PMUC_DISCHARGE_EN_BUCK         (1<<6)              
#define PMUC_HEAVY_LOAD_BUCK           (1<<7)              
#define PMUC_ANTIRING_DISABLE_BUCK     (1<<8)              
#define PMUC_LOW_SENSE_BUCK            (1<<9)              
#define PMUC_PFM_BUCK_THRESHOLD(n)     (((n)&0x3)<<10)     
#define PMUC_OSC_FREQ_BUCK(n)          (((n)&0x3)<<12)     
#define PMUC_COUNTER_DISABLE_BUCK      (1<<14)             
#define PMUC_HYS_VBP_LOWBAT_BUCK       (1<<15)             
#define PMUC_PFM_CLK_DISABLE_BUCK      (1<<16)             
#define PMUC_NONOV_DELAY_BUCK(n)       (((n)&0x3)<<17)     
#define PMUC_BUCK_LDO_VREF_SEL         (1<<19)             
#define PMUC_PFM_VREF_ONLY_BUCK        (1<<20)             
#define PMUC_CHARGE_PUMP_LDO_EN        (1<<21)             

//ldo_cfg
#define PMUC_VUSB_VBIT(n)              (((n)&0x7)<<0)      
#define PMUC_VUSB_VSEL(n)              (((n)&0x3)<<3)      
#define PMUC_VUSB_EXCAP_EN             (1<<5)              
#define PMUC_PU_VUSB                   (1<<6)              
#define PMUC_VIO_VSEL(n)               (((n)&0x3)<<7)      
#define PMUC_VIO_VBIT(n)               (((n)&0x7)<<9)      
#define PMUC_PU_VIO                    (1<<12)             
#define PMUC_VSEL_VA                   (1<<13)             
#define PMUC_VA_VBIT(n)                (((n)&0x7)<<14)     
#define PMUC_VA_CC_DISABLE             (1<<17)             
#define PMUC_PU_VA_NORMAL              (1<<18)             
#define PMUC_PU_VA_LP                  (1<<19)             
#define PMUC_LV_DETECT_PMU             (1<<20)             
#define PMUC_VIO_EXCAP_EN              (1<<21)             

//charger_cfg_ctrl
#define PMUC_PRE_CHR_CHECK_BYPASS      (1<<0)              
#define PMUC_CHR_VFB_SEL(n)            (((n)&0xF)<<1)      
#define PMUC_CHR_VFB_SEL_REG(n)        (((n)&0xF)<<5)      
#define PMUC_CHR_VFB_SEL_DR            (1<<9)              
#define PMUC_CHR_PRE_I_BIT(n)          (((n)&0x7)<<10)     
#define PMUC_CHR_CC_I_BIT(n)           (((n)&0x7)<<13)     
#define PMUC_CHR_CC_MODE               (1<<16)             
#define PMUC_CHR_CC_MODE_REG           (1<<17)             
#define PMUC_CHR_CC_MODE_DR            (1<<18)             
#define PMUC_CHR_CV_LOWGAIN            (1<<19)             
#define PMUC_UV_SEL(n)                 (((n)&0x3)<<20)     
#define PMUC_UV_SEL_REG(n)             (((n)&0x3)<<22)     
#define PMUC_UV_SEL_DR                 (1<<24)             
#define PMUC_CHR_AUXADC_SEL(n)         (((n)&0x3)<<25)     
#define PMUC_CHR_ENABLE                (1<<27)             
#define PMUC_CHR_ENABLE_REG            (1<<28)             
#define PMUC_CHR_ENABLE_DR             (1<<29)             
#define PMUC_CHR_AC_UVLO               (1<<30)             

//ana_status
#define PMUC_CHR_CREG                  (1<<0)              
#define PMUC_CHR_VREG                  (1<<1)              
#define PMUC_CHR_TERM                  (1<<2)              
#define PMUC_CHR_AC_ON                 (1<<3)              
#define PMUC_LOWBAT_DET_OUT            (1<<4)              
#define PMUC_BAT_ON_DET_OUT            (1<<5)              

//pmu_ana_rsvd
#define PMUC_PMU_ANA_RSVD_OUT(n)       (((n)&0xF)<<0)      
#define PMUC_PMU_ANA_RSVD_IN(n)        (((n)&0x3)<<4)      

//sw_rsvd
#define PMUC_SW_RSVD(n)                (((n)&0xFFFFFFFF)<<0)

//switch_mode
#define PMUC_SWITCH_MODE               (1<<0)              

#endif


