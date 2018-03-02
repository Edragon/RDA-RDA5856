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

                                                                           
//==============================================================================
//                                                                              
//    THIS FILE WAS GENERATED FROM ITS CORRESPONDING XML VERSION WITH COOLXML.  
//                                                                              
//                       !!! PLEASE DO NOT EDIT !!!                                                                                                         
//==============================================================================
//
/// @file
//
//==============================================================================

#ifndef _OPAL_H_
#define _OPAL_H_

// =============================================================================
//  MACROS
// =============================================================================

typedef enum {
    RDA_ADDR_SYS_CTRL                         = 0x00,
    RDA_ADDR_IRQ_SETTINGS                     = 0x01,
    RDA_ADDR_LDO_SETTINGS                     = 0x02,
    RDA_ADDR_LDO_ACTIVE_SETTING1              = 0x03,
    RDA_ADDR_LDO_ACTIVE_SETTING2              = 0x04,
    RDA_ADDR_LDO_ACTIVE_SETTING3              = 0x05,
    RDA_ADDR_LDO_ACTIVE_SETTING4              = 0x06,
    RDA_ADDR_LDO_ACTIVE_SETTING5              = 0x07,
    RDA_ADDR_LDO_LP_SETTING1                  = 0x08,
    RDA_ADDR_LDO_LP_SETTING2                  = 0x09,
    RDA_ADDR_LDO_LP_SETTING3                  = 0x0A,
    RDA_ADDR_LDO_LP_SETTING4                  = 0x0B,
    RDA_ADDR_LDO_LP_SETTING5                  = 0x0C,
    RDA_ADDR_LDO_POWER_ON_SETTING1            = 0x0D,
    RDA_ADDR_LDO_POWER_ON_SETTING2            = 0x0E,
    RDA_ADDR_LDO_POWER_ON_SETTING3            = 0x0F,
    RDA_ADDR_LDO_POWER_ON_SETTING4            = 0x10,
    RDA_ADDR_HP_DETECT_SETTING                = 0x11,
    RDA_ADDR_CHARGER_SETTING1                 = 0x12,
    RDA_ADDR_CHARGER_SETTING2                 = 0x13,
    RDA_ADDR_CHARGER_STATUS                   = 0x14,
    RDA_ADDR_CHARGER_CONTROL                  = 0x15,
    RDA_ADDR_CALIBRATION_SETTING1             = 0x16,
    RDA_ADDR_CALIBRATION_SETTING2             = 0x17,
    RDA_ADDR_MISC_CONTROL                     = 0x18,
    RDA_ADDR_LED_SETTING1                     = 0x19,
    RDA_ADDR_LED_SETTING2                     = 0x1A,
    RDA_ADDR_LED_SETTING3                     = 0x1B,
    RDA_ADDR_AUDIO_CONTROL                    = 0x1C,
    RDA_ADDR_TOUCH_SCREEN_CONTROL             = 0x1D,
    RDA_ADDR_TOUCH_SCREEN_RESULTS1            = 0x1E,
    RDA_ADDR_TOUCH_SCREEN_RESULTS2            = 0x1F,
    RDA_ADDR_TOUCH_SCREEN_RESULTS3            = 0x20,
    RDA_ADDR_EFUSE_OPT_SETTING1               = 0x21,
    RDA_ADDR_EFUSE_OPT_SETTING2               = 0x22,
    RDA_ADDR_EFUSE_OPT_SETTING3               = 0x23,
    RDA_ADDR_EFUSE_OPT_SETTING4               = 0x24,
    RDA_ADDR_EFUSE_OPT_SETTING5               = 0x25,
    RDA_ADDR_DEBUG1                           = 0x26,
    RDA_ADDR_SPEAKER_PA_SETTING1              = 0x27,
    RDA_ADDR_SIM_ACTIVE_SETTING               = 0x28,
    RDA_ADDR_SIM_LP_SETTING                   = 0x29,
    RDA_ADDR_SIM_POWER_ON_SETTING             = 0x2A,
    RDA_ADDR_SIM_POWER_OFF_SETTING            = 0x2B,
    RDA_ADDR_DEBUG2                           = 0x2C,
    RDA_ADDR_LDO_BUCK1                        = 0x2D,
    RDA_ADDR_LDO_BUCK2                        = 0x2E,
    RDA_ADDR_DCDC_BUCK                        = 0x2F,
    RDA_ADDR_PMU_RESET                        = 0x30,
    RDA_ADDR_LDO_POWER_OFF_SETTING1           = 0x31,
    RDA_ADDR_LDO_POWER_OFF_SETTING2           = 0x32,
    RDA_ADDR_LDO_POWER_OFF_SETTING3           = 0x33,
    RDA_ADDR_LDO_POWER_OFF_SETTING4           = 0x34,
    RDA_ADDR_LDO_POWER_OFF_SETTING5           = 0x35,
    RDA_ADDR_THERMAL_CALIBRATION              = 0x36,
    RDA_ADDR_DEBUG3                           = 0x37,
    RDA_ADDR_LED_SETTING4                     = 0x38,
    RDA_ADDR_DEBUG4                           = 0x39,
    RDA_ADDR_GPADC1                           = 0x3A,
    RDA_ADDR_GPADC2                           = 0x3B,
    RDA_ADDR_POWER_TIMING1                    = 0x3C,
    RDA_ADDR_POWER_TIMING2                    = 0x3D,
    RDA_ADDR_LED_SETTING5                     = 0x3E,
    RDA_ADDR_EFUSE_OUT                        = 0x3F,
    RDA_ADDR_SPEAKER_PA_SETTING2              = 0x40,
    RDA_ADDR_SPEAKER_PA_SETTING3              = 0x41,
    RDA_ADDR_SPEAKER_PA_SETTING4              = 0x42,
    RDA_ADDR_SPEAKER_PA_SETTING5              = 0x43,
    RDA_ADDR_TSC_KEY_CONTROL                  = 0x46,
    RDA_ADDR_REG48                            = 0x48,
    RDA_ADDR_REG49                            = 0x49,
    RDA_ADDR_XTAL_SET_LP                      = 0x60,
    RDA_ADDR_XTAL_CTRL1                       = 0x61,
    RDA_ADDR_XTAL_CTRL2                       = 0x62,
    RDA_ADDR_XTAL_CTRL3                       = 0x63,
    RDA_ADDR_32K_CTRL                         = 0x64,
    RDA_ADDR_XTAL_SET_NOR                     = 0x65,
    RDA_ADDR_XTAL_DOUBLER_SETTING             = 0x66,
    RDA_ADDR_XTAL_CTRL4                       = 0x67,
    RDA_ADDR_REG68                            = 0x68,
    RDA_ADDR_REG69                            = 0x69,
    RDA_ADDR_REG6A                            = 0x6A,

    
} RDA_REG_MAP_T;

//sys_ctrl  0X0
#define RDA_PMU_SYS_CTRL_SCAN_MODE             (1<<0)              
#define RDA_PMU_SYS_CTRL_BIST_MODE             (1<<1)              
#define RDA_PMU_SYS_CTRL_TST_FUNC_HST          (1<<2)              
#define RDA_PMU_SYS_CTRL_IOMUX_ABB_PMU_TST     (1<<3)              
#define RDA_PMU_SYS_CTRL_IOMUX_ABB_USB_MON     (1<<4)              
#define RDA_PMU_SYS_CTRL_IOMUX_ABB_USB_TST     (1<<5)              
#define RDA_PMU_SYS_CTRL_IOMUX_ABB_AUDIO_SPK_TST (1<<6)              
#define RDA_PMU_SYS_CTRL_IOMUX_ABB_AUDIO_MIC_TST (1<<7)              
#define RDA_PMU_SYS_CTRL_FLASH_DOWNLOAD        (1<<8)              
#define RDA_PMU_SYS_CTRL_PMU_I2C_EN            (1<<9)              
#define RDA_PMU_SYS_CTRL_IOMUX_EFUSE_WREN      (1<<10)             
#define RDA_PMU_SYS_CTRL_IOMUX_JTAG            (1<<11)             
#define RDA_PMU_SYS_CTRL_I2C_HOST_EN           (1<<12)             
#define RDA_PMU_SYS_CTRL_I2C_EXT_EN            (1<<13)             
#define RDA_PMU_SYS_CTRL_EDT_BYPASS            (1<<14)             
#define RDA_PMU_SYS_CTRL_IO_KEYSENSE_ENB       (1<<15)             


// IRQ settings 0x1/////////////////////////////////////
#define RDA_PMU_PD_MODE_SEL             (1<<0)
#define RDA_PMU_HOLD_PRECHARGER_EFUSE   (1<<2)
#define RDA_PMU_PD_RESETN_EFUSE          (1<<3)
#define RDA_PMU_INT_CHR_MASK            (1<<4)
#define RDA_PMU_INT_CHR_CLEAR           (1<<5)
#define RDA_PMU_KEYIRQ_MASK             (1<<6)
#define RDA_PMU_INT_CHR_ON              (1<<7)
#define RDA_PMU_PENIRQ_MASK             (1<<8)
#define RDA_PMU_EOMIRQ_MASK             (1<<9)
#define RDA_PMU_PENIRQ_CLEAR            (1<<10)
#define RDA_PMU_EOMIRQ_CLEAR            (1<<11)
#define RDA_PMU_KEYIRQ_CLEAR            (1<<12)
#define RDA_PMU_KEYIRQ                  (1<<13)
#define RDA_PMU_PENIRQ                  (1<<14)
#define RDA_PMU_EOMIRQ                  (1<<15)
// end IRQ settings

//LDO settings 0x02
#define RDA_PMU_FM_ENABLE                    (1<<0)
#define RDA_PMU_BT_ENABLE                    (1<<1)
#define RDA_PMU_KEY_ENABLE                   (1<<2)
#define RDA_PMU_TSC_ENABLE                   (1<<3)
#define RDA_PMU_VLCD_ENABLE                  (1<<4)
//#define RDA_PMU_VCAM_ENABLE                  (1<<5)//
#define RDA_PMU_VIO1P8_ENABLE                (1<<5)
#define RDA_PMU_VMIC_ENABLE                  (1<<6)
#define RDA_PMU_VIBR_ENABLE                  (1<<7)
//#define RDA_PMU_VRF_ENABLE                   (1<<9)//
#define RDA_PMU_VASW_ENABLE                   (1<<9)//
//#define RDA_PMU_VABB_ENABLE                  (1<<10)//
#define RDA_PMU_VA_ENABLE                  (1<<10)//
#define RDA_PMU_VMMC_ENABLE                  (1<<11)
#define RDA_PMU_VSIM4_ENABLE                 (1<<12)
#define RDA_PMU_VSIM3_ENABLE                 (1<<13)
#define RDA_PMU_VSIM2_ENABLE                 (1<<14)
#define RDA_PMU_VSIM1_ENABLE                 (1<<15)
#define RDA_PMU_VSIM_ENABLE_MASK             (0xf<<12)
// end LDO settings

//LDO active/lp/pon setting1 0x03/0x08/0x0d
#define RDA_PMU_NORMAL_MODE               (1<<0)
#define RDA_PMU_VSPIMEM_ON                (1<<1)
#define RDA_PMU_VBLLED_OFF                (0<<2)
#define RDA_PMU_VMIC_OFF                  (0<<3)
#define RDA_PMU_VUSB_ON                   (1<<4)
#define RDA_PMU_VIBR_OFF                  (0<<5)
#define RDA_PMU_VMMC_OFF                  (0<<6)
#define RDA_PMU_VLCD_OFF                  (0<<7)
#define RDA_PMU_VIO1P8_ON                  (1<<8)
#define RDA_PMU_VASW_OFF                   (0<<9)
#define RDA_PMU_VA_ON                      (1<<10)
#define RDA_PMU_VIO2P8_ON                  (1<<11)
#define RDA_PMU_VMEM_OFF                  (0<<12)
#define RDA_PMU_BUCK1_LDO_OFF             (1<<13)
#define RDA_PMU_BUCK1_PFM_ON              (1<<14)
#define RDA_PMU_BUCK1_ON                  (1<<15)


#define RDA_PMU_LDO_EN(n)              (((n)&0xFFFF)<<0)
#define RDA_PMU_LDO_EN_MASK            (0xFFFF<<0)
#define RDA_PMU_LDO_EN_SHIFT           (0)
// end LDO active/lp/pon setting1

// LDO active/lp/pon setting2  0x04/0x09/0x0e
//#define RDA_PMU_RGB_LED_OFF       (1<<0) //
#define RDA_PMU_RES_VBIT(n)       (((n)&0x3)<<0)
#define RDA_PMU_BUCK2_LDO_OFF     (1<<2)
#define RDA_PMU_BUCK2_PFM_ON      (1<<3)
#define RDA_PMU_BUCK2_ON          (1<<4)
#define RDA_PMU_VBOOST_ON         (1<<5)
#define RDA_PMU_VRF28_OFF        (1<<6)

#define RDA_PMU_VIBR_SEL_MASK     (1<<7)
#define RDA_PMU_VIBR_VSEL_1_8     (1<<7)
#define RDA_PMU_VIBR_VSEL_2_8     (0<<7)

#define RDA_PMU_VMMC_SEL_MASK     (1<<8)
#define RDA_PMU_VMMC_VSEL_2_8     (1<<8)
#define RDA_PMU_VMMC_VSEL_3_2     (0<<8)

#define RDA_PMU_VLCD_SEL_MASK     (1<<9)
#define RDA_PMU_VLCD_VSEL_1_8     (1<<9)
#define RDA_PMU_VLCD_VSEL_2_8     (0<<9)

#define RDA_PMU_VIO1P8_SEL_MASK     (1<<10)
#define RDA_PMU_VIO1P8_VSEL_1_8     (1<<10)
#define RDA_PMU_VIO1P8_VSEL_2_8     (0<<10)

#define RDA_PMU_VASW_SEL_MASK      (1<<11)
#define RDA_PMU_VASW_VSEL_1_8      (1<<11)
#define RDA_PMU_VASW_VSEL_2_8      (0<<11)

#define RDA_PMU_VIO2P8_SEL_MASK     (1<<12)
#define RDA_PMU_VIO2P8_VSEL_1_8     (1<<12)
#define RDA_PMU_VIO2P8_VSEL_2_8     (0<<12)

#define RDA_PMU_VRTC_VBIT_SHIFT   (13)
#define RDA_PMU_VRTC_VBIT_MASK    (0x7<<13)
#define RDA_PMU_VRTC_VBIT(n)      (((n)&0x7)<<13)

// end LDO active/lp/pon setting2

// LDO ACT/LP/PON setting3 0x5/0xa/0xf
#define RDA_PMU_PU_RES_IBIT_SHIFT       (0)
#define RDA_PMU_PU_RES_IBIT_MASK        (0x7<<0)
#define RDA_PMU_PU_RES_IBIT(n)          (((n)&0x7)<<0)

#define RDA_PMU_VIO2P8_IBIT_SHIFT       (3)
#define RDA_PMU_VIO2P8_IBIT_MASK        (0x7<<3)
#define RDA_PMU_VIO2P8_IBIT(n)          (((n)&0x7)<<3)

#define RDA_PMU_VMEM_IBIT_SHIFT       (6)
#define RDA_PMU_VMEM_IBIT_MASK        (0x7<<6)
#define RDA_PMU_VMEM_IBIT(n)          (((n)&0x7)<<6)

#define RDA_PMU_VSPIMEM_IBIT_SHIFT    (9)
#define RDA_PMU_VSPIMEM_IBIT_MASK     (0x7<<9)
#define RDA_PMU_VSPIMEM_IBIT(n)       (((n)&0x7)<<9)

#define RDA_PMU_VBACKUP_VBIT_SHIFT    (12)
#define RDA_PMU_VBACKUP_VBIT_MASK     (0x7<<12)
#define RDA_PMU_VBACKUP_VBIT(n)       (((n)&0x7)<<12)
// end LDO ACT/LP/PON setting3

// LDO ACT setting4 0x6
// NOTE:
//   These settings are valid for all profiles
#define RDA_PMU_VIBR_IBIT_SHIFT       (0)
#define RDA_PMU_VIBR_IBIT_MASK        (0x7<<0)
#define RDA_PMU_VIBR_IBIT(n)          (((n)&0x7)<<0)

#define RDA_PMU_VMMC_IBIT_SHIFT       (3)
#define RDA_PMU_VMMC_IBIT_MASK        (0x7<<3)
#define RDA_PMU_VMMC_IBIT(n)          (((n)&0x7)<<3)

#define RDA_PMU_VLCD_IBIT_SHIFT       (6)
#define RDA_PMU_VLCD_IBIT_MASK        (0x7<<6)
#define RDA_PMU_VLCD_IBIT(n)          (((n)&0x7)<<6)

#define RDA_PMU_VIO1P8_IBIT_SHIFT       (9)
#define RDA_PMU_VIO1P8_IBIT_MASK        (0x7<<9)
#define RDA_PMU_VIO1P8_IBIT(n)          (((n)&0x7)<<9)

#define RDA_PMU_VASW_IBIT_SHIFT        (12)
#define RDA_PMU_VASW_IBIT_MASK         (0x7<<12)
#define RDA_PMU_VASW_IBIT(n)           (((n)&0x7)<<12)
// end LDO ACT setting4

// LDO ACT/LP setting5 0x7/0xc
// NOTE:
//   VRGB_LED/VABB bits are located in ACT setting5 only
//   and are valid for all profiles
#define RDA_PMU_VRGB_LED_IBIT_SHIFT  (0)
#define RDA_PMU_VRGB_LED_IBIT_MASK   (0x7<<0)
#define RDA_PMU_VRGB_LED_IBIT(n)     (((n)&0x7)<<0)

#define RDA_PMU_VRGB_LED_VSEL_SHIFT  (3)
#define RDA_PMU_VRGB_LED_VSEL_MASK   (0x7<<3)
#define RDA_PMU_VRGB_LED_VSEL(n)     (((n)&0x7)<<3)

#define RDA_PMU_VABB_IBIT_SHIFT      (6)
#define RDA_PMU_VABB_IBIT_MASK       (0x7<<6)
#define RDA_PMU_VABB_IBIT(n)         (((n)&0x7)<<6)
// End of NOTE

#define RDA_PMU_VUSB_IBIT_SHIFT      (9)
#define RDA_PMU_VUSB_IBIT_MASK       (0x7<<9)
#define RDA_PMU_VUSB_IBIT(n)         (((n)&0x7)<<9)

#define RDA_PMU_VA_CC_DISABLE		 (0<<12)
#define RDA_PMU_VUSB_CC_DISABLE		 (0<<13)
#define RDA_PMU_VSIM2_VSEL_ACT		 (1<<14)
#define RDA_PMU_VSIM1_VSEL_ACT		 (1<<15)
// end LDO ACT/LP/PON setting5

// LDO LP setting4 0xb
// NOTE:
//   These settings are valid for all profiles
#define RDA_PMU_VRF28_IBIT_SHIFT    (0)
#define RDA_PMU_VRF28_IBIT_MASK     (0x7<<0)
#define RDA_PMU_VRF28_IBIT(n)       (((n)&0x7)<<0)

#define RDA_PMU_VRF28_SEL_MASK      (1<<3)
#define RDA_PMU_VRF28_VSEL_1_8      (1<<3)//
#define RDA_PMU_VRF28_VSEL_2_8      (0<<3)//0 == select?
// end LDO LP setting4

// LDO LP state ldo setting 5
// NOTE:
//   VUSB/VSIM PON bits are located in LP setting5 only
//   and are valid for power-on profile
#define RDA_PMU_VUSB_IBIT_PON_SHIFT  (0)//////delete?
#define RDA_PMU_VUSB_IBIT_PON_MASK   (0x7<<0)//
#define RDA_PMU_VUSB_IBIT_PON(n)     (((n)&0x7)<<0)

#define RDA_PMU_VSIM4_VSEL_PON_1_8   (1<<3)
#define RDA_PMU_VSIM4_VSEL_PON_2_8   (0<<3)
#define RDA_PMU_VSIM4_SEL_PON_MASK   (1<<3)
#define RDA_PMU_VSIM3_VSEL_PON_1_8   (1<<4)
#define RDA_PMU_VSIM3_VSEL_PON_2_8   (0<<4)
#define RDA_PMU_VSIM3_SEL_PON_MASK   (1<<4)
#define RDA_PMU_VSIM2_VSEL_PON_1_8   (1<<5)
#define RDA_PMU_VSIM2_VSEL_PON_2_8   (0<<5)
#define RDA_PMU_VSIM2_SEL_PON_MASK   (1<<5)
#define RDA_PMU_VSIM1_VSEL_PON_1_8   (1<<6)
#define RDA_PMU_VSIM1_VSEL_PON_2_8   (0<<6)
#define RDA_PMU_VSIM1_SEL_PON_MASK   (1<<6)//////

//#define RDA_PMU_VUSB_IBIT_SHIFT      (9)//same as in 0x7/0xc
//#define RDA_PMU_VUSB_IBIT_MASK       (0x7<<9)
//#define RDA_PMU_VUSB_IBIT(n)         (((n)&0x7)<<9)

#define RDA_PMU_VSIM4_VSEL_1_8       (1<<12)/////////delete?
#define RDA_PMU_VSIM4_VSEL_2_8       (0<<12)
#define RDA_PMU_VSIM4_SEL_MASK       (1<<12)
#define RDA_PMU_VSIM3_VSEL_1_8       (1<<13)
#define RDA_PMU_VSIM3_VSEL_2_8       (0<<13)
#define RDA_PMU_VSIM3_SEL_MASK       (1<<13)
#define RDA_PMU_VSIM2_VSEL_1_8       (1<<14)
#define RDA_PMU_VSIM2_VSEL_2_8       (0<<14)
#define RDA_PMU_VSIM2_SEL_MASK       (1<<14)
#define RDA_PMU_VSIM1_VSEL_1_8       (1<<15)
#define RDA_PMU_VSIM1_VSEL_2_8       (0<<15)
#define RDA_PMU_VSIM1_SEL_MASK       (1<<15)/////////
// End of NOTE

// 0x10
#define RDA_PMU_SIM4_EXCAP_EN       (1<<0)
#define RDA_PMU_SIM3_EXCAP_EN       (1<<1)
#define RDA_PMU_SIM2_EXCAP_EN       (1<<2)
#define RDA_PMU_SIM1_EXCAP_EN       (1<<3)
#define RDA_PMU_VRF28_EXCAP_EN      (1<<4)
#define RDA_PMU_SPIMEM_EXCAP_EN     (1<<5)
#define RDA_PMU_MMC_EXCAP_EN       (1<<6)
#define RDA_PMU_MEM_EXCAP_EN       (1<<7)
#define RDA_PMU_LCD_EXCAP_EN       (1<<8)
#define RDA_PMU_VIP2P8_EXCAP_EN       (1<<9)
#define RDA_PMU_VIO1P8_EXCAP_EN       (1<<10)
#define RDA_PMU_ASW_EXCAP_EN       (1<<11)
#define RDA_PMU_VFB_USB_SEL_CP       (1<<12)
#define RDA_PMU_USB_MODE_CP       (1<<13)
#define RDA_PMU_LED_MODE_CP       (1<<14)
#define RDA_PMU_VS_SEL_BUCK1       (1<<15)

// HP Detect setting 0x11
#define RDA_PMU_HP_OUT_MASK          (1<<0)
#define RDA_PMU_HP_IN_MASK           (1<<1)
#define RDA_PMU_HP_OUT_CLEAR         (1<<2)
#define RDA_PMU_HP_IN_CLEAR          (1<<3)
//#define RDA_PMU_HP_OUT               (1<<4)
//#define RDA_PMU_HP_IN                (1<<5)
#define RDA_PMU_VUSB_ON_IN			 (1<<4)
#define RDA_PMU_MASTER_MODE_VUSB	 (1<<5)
#define RDA_PMU_HP_OUT               (1<<6)//read only
#define RDA_PMU_HP_IN                (1<<7)//read only
#define RDA_PMU_HP_DETECT            (1<<8)//read only
// end HP Detect setting

// Charger setting1 0x12
#define RDA_PMU_BG_CAL_TC_BIT2(n)     (((n)&0x1f)<<0)
#define RDA_PMU_BG_CAL_TC_BIT2_MASK   (0x1f<<0)
#define RDA_PMU_BG_CAL_TC_BIT2_SHIFT  (0)

#define RDA_PMU_BG_DIS_CHOPPER        (1<<5)
//#define RDA_PMU_BG_DIS_CHOPPER_FILT   (1<<6)//reserved
#define RDA_PMU_IREF_CTL_BG_PMU       (1<<7)

#define RDA_PMU_UV_SEL(n)             (((n)&0x3)<<8)
#define RDA_PMU_UV_SEL_MASK           (0x3<<8)
#define RDA_PMU_UV_SEL_SHIFT          (8)

#define RDA_PMU_CHR_BYPASS_CHOPPER    (1<<11)

#define RDA_PMU_CHR_AUXADC_SEL(n)     (((n)&0x3)<<12)
#define RDA_PMU_CHR_AUXADC_SEL_MASK   (0x3<<12)
#define RDA_PMU_CHR_AUXADC_SEL_SHIFT  (12)

#define RDA_PMU_CHR_CV_LOWGAIN        (1<<14)
#define RDA_PMU_CHR_ALWAYS_CC         (1<<15)
// End of charger setting1

// Charger setting2 0x13
#define RDA_PMU_EFUSE_CHR_VFB_SEL_REG(n)     (((n)&0xf)<<2)
#define RDA_PMU_EFUSE_CHR_VFB_SEL_REG_MASK   (0xf<<2)
#define RDA_PMU_EFUSE_CHR_VFB_SEL_REG_SHIFT  (2)

#define RDA_PMU_CHR_PRE_IBIT_REG(n)    (((n)&0x7)<<4)
#define RDA_PMU_CHR_PRE_IBIT_REG_MASK  (0x7<<4)
#define RDA_PMU_CHR_PRE_IBIT_REG_SHIFT (4)

#define RDA_PMU_CHR_CC_IBIT_REG(n)     (((n)&0x7)<<7)
#define RDA_PMU_CHR_CC_IBIT_REG_MASK   (0x7<<7)
#define RDA_PMU_CHR_CC_IBIT_REG_SHIFT  (7)
#define RDA_PMU_CHR_CC_IBIT_100MA      RDA_PMU_CHR_CC_IBIT_REG(0)
#define RDA_PMU_CHR_CC_IBIT_200MA      RDA_PMU_CHR_CC_IBIT_REG(1)
#define RDA_PMU_CHR_CC_IBIT_300MA      RDA_PMU_CHR_CC_IBIT_REG(2)
#define RDA_PMU_CHR_CC_IBIT_400MA      RDA_PMU_CHR_CC_IBIT_REG(3)
#define RDA_PMU_CHR_CC_IBIT_450MA      RDA_PMU_CHR_CC_IBIT_REG(4)
#define RDA_PMU_CHR_CC_IBIT_500MA      RDA_PMU_CHR_CC_IBIT_REG(5)
#define RDA_PMU_CHR_CC_IBIT_600MA      RDA_PMU_CHR_CC_IBIT_REG(6)
#define RDA_PMU_CHR_CC_IBIT_700MA      RDA_PMU_CHR_CC_IBIT_REG(7)

#define RDA_PMU_CHR_VFB_SEL_DR         (1<<10)
#define RDA_PMU_CHR_PRE_IBIT_DR        (1<<11)
#define RDA_PMU_CHR_CC_IBIT_DR         (1<<12)

#define RDA_PMU_CHR_DISABLE_OSC_CHR	   (1<<13)
#define RDA_PMU_CHR_CHOPPER_CLK_SEL	   (1<<14)
#define RDA_PMU_CHR_CC_ITRL_EN_HIGHAC  (1<<15)
// End of charger setting 2

// Charger status 0x14
#define RDA_PMU_CHR_VREG   (1<<2)
#define RDA_PMU_CHR_AC_ON   (1<<7)
#define RDA_PMU_PENIRQ_B   (1<<11)
 // end charger status

// Charger control 0x15

#define RDA_PMU_CHR_GPADC_CHN_SEL(n)	 ((1<<((n)&0x7))<<0)//[07:0]
#define RDA_PMU_CHR_TERM_CTRL(n)         (((n)&0x7)<<8)//[10:8]
//#define RDA_PMU_CHR_R15_RESERVED		 (1<<11)//r15 reserved

#define RDA_PMU_CHR_CC_MODE_REG          (1<<12)
#define RDA_PMU_CHR_CC_MODE_DR           (1<<13)
#define RDA_PMU_CHR_ENABLE_REG           (1<<14)
#define RDA_PMU_CHR_ENABLE_DR            (1<<15)
// end charger control

// Charger calib setting1 0x16
#define RDA_PMU_BGAP_CAL_BIT_REG(n)      (((n)&0x3f)<<0)
#define RDA_PMU_BGAP_CAL_BIT_REG_MASK    (0x3f<<0)
#define RDA_PMU_BGAP_CAL_BIT_REG_SHIFT   (0)

#define RDA_PMU_BGAP_CAL_BIT_DR          (1<<6)
#define RDA_PMU_RESETN_BGAP_CAL_REG      (1<<7)
#define RDA_PMU_RESETN_BGAP_CAL_DR       (1<<8)
#define RDA_PMU_LP_MODE_BG_REG           (1<<10)
#define RDA_PMU_LP_MODE_BG_DR            (1<<11)
#define RDA_PMU_BGAP_CAL_POLARITY        (1<<12)
#define RDA_PMU_BGAP_CAL_CLOCK_INV       (1<<13)
#define RDA_PMU_VBAT_OVER_3P2_BYPASS     (1<<14)
#define RDA_PMU_BGAP_CAL_BYPASS          (1<<15)
// End of charget calib setting1

// Charger calib setting2 0x17
#define RDA_PMU_PU_CHARGE_LDO            (1<<0)
#define RDA_PMU_BG_FREQ_BIT(n)			 (((n)&0x3)<<1)
#define RDA_PMU_REF_SEL_BG				 (1<<3)
//#define RDA_PMU_R17_RESERVED			 (0110001<<4)

#define RDA_PMU_TS_I_CTRL_BATTERY(n)     (((n)&0xf)<<11)
#define RDA_PMU_TS_I_CTRL_BATTERY_MASK   (0xf<<11)
#define RDA_PMU_TS_I_CTRL_BATTERY_SHIFT  (11)

#define RDA_PMU_PU_TS_BATTERY            (1<<15)
// End of charget calib setting2

// MISC Control 0x18
#define RDA_PMU_LDO_AVDD3_BIT(n)         (((n)&7)<<0)
#define RDA_PMU_LDO_AVDD3_BIT_MASK       (7<<0)
#define RDA_PMU_LDO_AVDD3_BIT_SHIFT      (0)
#define RDA_PMU_CLK2M_FTUN_BIT(n)        (((n)&7)<<3)
#define RDA_PMU_CLK2M_FTUN_BIT_MASK      (7<<3)
#define RDA_PMU_CLK2M_FTUN_BIT_SHIFT     (3)
#define RDA_PMU_PD_LDO_AVDD3_REG         (1<<6)
#define RDA_PMU_PD_LDO_AVDD3_DR          (1<<7)
#define RDA_PMU_PU_CLK_4M_REG            (1<<8)
#define RDA_PMU_PU_CLK_4M_DR             (1<<9)
#define RDA_PMU_PU_CLK_32K_REG           (1<<10)
#define RDA_PMU_PU_CLK_32K_DR            (1<<11)
// End of MISC Control

// LED setting1 0x19
#define RDA_PMU_DIM_BL_REG               (1<<0)
#define RDA_PMU_DIM_BL_DR                (1<<1)
#define RDA_PMU_DIM_LED_B_REG            (1<<2)
#define RDA_PMU_DIM_LED_B_DR             (1<<3)
#define RDA_PMU_DIM_LED_G_REG            (1<<4)
#define RDA_PMU_DIM_LED_G_DR             (1<<5)
#define RDA_PMU_DIM_LED_R_REG            (1<<6)
#define RDA_PMU_DIM_LED_R_DR             (1<<7)
#define RDA_PMU_PWM_RGB_PMU_MODE         (1<<8)
#define RDA_PMU_PWM_BL_ENABLE            (1<<9)

#define RDA_PMU_PWM_RGB_FREQ(n)          (((n)&0xf)<<12)
#define RDA_PMU_PWM_RGB_FREQ_MASK        (0xf<<12)
#define RDA_PMU_PWM_RGB_FREQ_SHIFT       (12)
// end LED setting1 0x19

// LED setting2 0x1a
#define RDA_PMU_BL_OFF_LP                  (1<<1)
#define RDA_PMU_BL_OFF_ACT                 (1<<2)
#define RDA_PMU_BL_OFF_PON                 (1<<3)
#define RDA_PMU_BL_IBIT_LP(n)              (((n)&0xf)<<4)
#define RDA_PMU_BL_IBIT_LP_MASK            (0xf<<4)
#define RDA_PMU_BL_IBIT_LP_SHIFT           (4)
#define RDA_PMU_BL_IBIT_ACT(n)             (((n)&0xf)<<8)
#define RDA_PMU_BL_IBIT_ACT_MASK           (0xf<<8)
#define RDA_PMU_BL_IBIT_ACT_SHIFT          (8)
#define RDA_PMU_BL_IBIT_PON(n)             (((n)&0xf)<<12)
#define RDA_PMU_BL_IBIT_PON_MASK           (0xf<<12)
#define RDA_PMU_BL_IBIT_PON_SHIFT          (12)
// end LED setting2 0x1a

// LED setting3 0x1b
#define RDA_PMU_PWM_BL_FREQ_SEL            (1<<5)

#define RDA_PMU_PWM_BL_FREQ(n)             (((n)&0xf)<<6)
#define RDA_PMU_PWM_BL_FREQ_MASK           (0xf<<6)
#define RDA_PMU_PWM_BL_FREQ_SHIFT          (6)

#define RDA_PMU_PWM_BL_DUTY_CYCLE(n)       (((n)&0x1f)<<10)
#define RDA_PMU_PWM_BL_DUTY_CYCLE_MASK     (0x1f<<10)
#define RDA_PMU_PWM_BL_DUTY_CYCLE_SHIFT    (10)
// end LED setting3 0x1b

// audio control 0x1c
#define RDA_PMU_TIMER_SAMP_NEG_TSC         (1<<1)
#define RDA_PMU_TIMER_SAMP_POS_TSC         (1<<2)

#define RDA_PMU_TIMER_X_TO_Y_SEL(n)        (((n)&0x3)<<3)
#define RDA_PMU_TIMER_X_TO_Y_SEL_MASK      (0x3<<3)
#define RDA_PMU_TIMER_X_TO_Y_SEL_SHIFT     (3)

#define RDA_PMU_TIMER_INT_TO_X_SEL(n)      (((n)&0x7)<<5)
#define RDA_PMU_TIMER_INT_TO_X_SEL_MASK    (0x7<<5)
#define RDA_PMU_TIMER_INT_TO_X_SEL_SHIFT   (5)

#define RDA_PMU_TIMER_PEN_IRQ_STABLE(n)    (((n)&0x7)<<8)
#define RDA_PMU_TIMER_PEN_IRQ_STABLE_MASK  (0x7<<8)
#define RDA_PMU_TIMER_PEN_IRQ_STABLE_SHIFT (8)

#define RDA_PMU_PWM_CLK_DIV2_ENABLE        (1<<11)

#define RDA_PMU_DELAY_BEFORE_SAMP(n)       (((n)&3)<<12)
#define RDA_PMU_DELAY_BEFORE_SAMP_MASK     (3<<12)
#define RDA_PMU_DELAY_BEFORE_SAMP_SHIFT    (12)
#define RDA_PMU_DELAY_BEFORE_SAMP_2US      (0<<12)
#define RDA_PMU_DELAY_BEFORE_SAMP_4US      (1<<12)
#define RDA_PMU_DELAY_BEFORE_SAMP_8US      (2<<12)
#define RDA_PMU_DELAY_BEFORE_SAMP_16US     (3<<12)

#define RDA_PMU_BYPASS_BB_READ             (1<<14)
#define RDA_PMU_MEAS_WAIT_CLR_PENIRQ       (1<<15)
// end audio control 0x1c

// touch screen control 0x1d
#define RDA_PMU_CONV_CLK_INV               (1<<2)
#define RDA_PMU_SAMP_CLK_INV               (1<<3)
#define RDA_PMU_SAR_ADC_MODE               (1<<4)
#define RDA_PMU_SAR_OUT_POLARITY           (1<<5)
#define RDA_PMU_WAIT_BB_READ_TIMESL(n)     (((n)&3)<<6)
#define RDA_PMU_WAIT_BB_READ_TIMESL_MASK   (3<<6)
#define RDA_PMU_WAIT_BB_READ_TIMESL_SHIFT  (6)
#define RDA_PMU_SAR_VERF_BIT(n)            (((n)&3)<<8)
#define RDA_PMU_SAR_VERF_BIT_MASK          (3<<8)
#define RDA_PMU_SAR_VERF_BIT_SHIFT         (8)
#define RDA_PMU_TSC_SAR_SEL(n)             (((n)&3)<<10)
#define RDA_PMU_TSC_SAR_SEL_MASK           (3<<10)
#define RDA_PMU_TSC_SAR_SEL_SHIFT          (10)
#define RDA_PMU_TSC_CLK_DIV_EN             (1<<13)
#define RDA_PMU_TSC_YP_YN_INV             (1<<14)//different from 8809
#define RDA_PMU_TSC_XP_XN_INV              (1<<15)
// end touch screen control 

// touch screen results1 0x1e
#define RDA_PMU_TSC_X_VALUE_BIT_MASK       (0x3ff<<0)
#define RDA_PMU_TSC_X_VALUE_BIT_SHIFT      (0)
#define RDA_PMU_TSC_X_VALUE_VALID          (1<<10)
// end touch screen results1

// touch screen results2 0x1f
#define RDA_PMU_TSC_Y_VALUE_BIT_MASK       (0x3ff<<0)
#define RDA_PMU_TSC_Y_VALUE_BIT_SHIFT      (0)
#define RDA_PMU_TSC_Y_VALUE_VALID          (1<<10)
// end touch screen results2

// touch screen results3 0x20
#define RDA_PMU_GPADC_VALUE_BIT_MASK       (0x3ff<<0)
#define RDA_PMU_GPADC_VALUE_BIT_SHIFT      (0)
#define RDA_PMU_GPADC_VALUE_VALID          (1<<10)
// end touch screen results3

// Effuse and OTP setting1 0x21
#define RDA_PMU_EFFUSE_SENSE_EN_REG			(1<<0)
#define RDA_PMU_EFFUSE_SENSE_EN_DR			(1<<1)

#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2(n)     (((n)&0xf)<<2)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2_MASK   (0xf<<2)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2_SHIFT  (2)

#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1(n)     (((n)&0xf)<<6)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1_MASK   (0xf<<6)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1_SHIFT  (6)

#define RDA_PMU_EFUSE_SEL_WORD_REG(n)        (((n)&0xf)<<10)
#define RDA_PMU_EFUSE_SEL_WORD_REG_MASK      (0xf<<10)
#define RDA_PMU_EFUSE_SEL_WORD_REG_SHIFT     (10)
/*
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2(n)     (((n)&0x3)<<8)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2_MASK   (0x3<<8)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_2_SHIFT  (8)

#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1(n)     (((n)&0x3)<<10)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1_MASK   (0x3<<10)
#define RDA_PMU_EFUSE_SEL_WORD_AUTO_1_SHIFT  (10)

#define RDA_PMU_EFUSE_SEL_WORD_REG(n)        (((n)&0x3)<<12)
#define RDA_PMU_EFUSE_SEL_WORD_REG_MASK      (0x3<<12)
#define RDA_PMU_EFUSE_SEL_WORD_REG_SHIFT     (12)
*/

#define RDA_PMU_EFUSE_SENSE_REG              (1<<14)
#define RDA_PMU_EFUSE_SENSE_DR               (1<<15)
// end of effuse and OTP setting1

// Effuse and OTP setting2 0x22
#define RDA_PMU_PD_OTP_REG                   (1<<0)
#define RDA_PMU_LP_MODE_REG                  (1<<1)
#define RDA_PMU_LP_MODE_DR                   (1<<2)
#define RDA_PMU_R_PROG_EFFUSE_EN			 (1<<3)
//#define RDA_PMU_R22_RESERVED				 (0x3<<4)
#define RDA_PMU_EFUSE_REF_RES_BIT_SHIFT      (6)
#define RDA_PMU_EFUSE_REF_RES_BIT_MASK       (0x3<<6)
#define RDA_PMU_EFUSE_REF_RES_BIT(n)		 (((n)&0x3)<<6)

#define RDA_PMU_NONOV_DELAY_BUCK2_SHIFT      (8)
#define RDA_PMU_NONOV_DELAY_BUCK2_MASK       (0x3<<8)
#define RDA_PMU_NONOV_DELAY_BUCK2(n)		 (((n)&0x3)<<8)

#define RDA_PMU_NONOV_DELAY_BUCK1_SHIFT      (10)
#define RDA_PMU_NONOV_DELAY_BUCK1_MASK       (0x3<<10)
#define RDA_PMU_NONOV_DELAY_BUCK1(n)		 (((n)&0x3)<<10)


#define RDA_PMU_SW_NMOS_CTRL_CP_SHIFT        (12)
#define RDA_PMU_SW_NMOS_CTRL_CP_MASK         (0x3<<12)
#define RDA_PMU_SW_NMOS_CTRL_CP(n)           (((n)&0x3)<<12)

#define RDA_PMU_SW_PMOS_CTRL_CP_SHIFT        (14)
#define RDA_PMU_SW_PMOS_CTRL_CP_MASK         (0x3<<14)
#define RDA_PMU_SW_PMOS_CTRL_CP(n)           (((n)&0x3)<<14)
// end of effuse and OTP setting2

//Effuse and OTP setting3 0x23
#define RDA_PMU_EFFUSE_SEL(n)		         (((n)&0xff)<<0)
//#define RDA_PMU_R23_RESERVED				 (0xff<<8)

// Effuse and OTP setting4 0x24
#define RDA_PMU_EFFUSE_OUT_READ_1_MASK       (0xffff<<0)
#define RDA_PMU_EFFUSE_OUT_READ_1_SHIFT      (0)
// end of effuse and OTP setting4

// Effuse and OTP setting4 0x25
#define RDA_PMU_EFFUSE_OUT_READ_2_MASK       (0xffff<<0)
#define RDA_PMU_EFFUSE_OUT_READ_2_SHIFT      (0)
// end of effuse and OTP setting4

//MISC 0x26
#define RDA_PMU_BYPASS_CLK_4M_GATE		 (1<<0)
#define RDA_PMU_BYPASS_CLK_32K_GATE		 (1<<1)
#define RDA_PMU_TIMER_BB_RESETN_SEL(n)	 (((n)&0x3)<<2)
//#define RDA_PMU_R26_RESERVED			 (0<<4)		
#define RDA_PMU_TIME_CLK_4M_STABLE(n)	 (((n)&0x3)<<8)
#define RDA_PMU_CHR_AC_ON_TIME_SEL(n)	 (((n)&0x7)<<10)
#define RDA_PMU_CHR_OUT_TIME_SEL(n)		 (((n)&0x7)<<13)

// Speaker PA setting1 0x27
#define RDA_PMU_BYPASS_CP_REG            (1<<0)
#define RDA_PMU_BYPASS_CP_DR             (1<<1)
#define RDA_PMU_PU_CP_REG                (1<<2)
#define RDA_PMU_PU_CP_DR                 (1<<3)

#define RDA_PMU_VBP_CTRL_CP_SHIFT        (4)
#define RDA_PMU_VBP_CTRL_CP_MASK         (0x3<<4)
#define RDA_PMU_VBP_CTRL_CP(n)           (((n)&0x3)<<4)

#define RDA_PMU_CLG_DET_OUT_TIME_SHIFT   (6)
#define RDA_PMU_CLG_DET_OUT_TIME_MASK    (0x3<<6)
#define RDA_PMU_CLG_DET_OUT_TIME(n)      (((n)&0x3)<<6)

#define RDA_PMU_CLG_DET_OUT_POLL         (1<<8)
#define RDA_PMU_CLk_SEL_CP               (1<<9)

#define RDA_PMU_VCP_OUT_BIT_SHIFT        (10)
#define RDA_PMU_VCP_OUT_BIT_MASK         (0xf<<10)
#define RDA_PMU_VCP_OUT_BIT(n)           (((n)&0xf)<<10)

#define RDA_PMU_DELAY_BIT_CP_SHIFT       (14)
#define RDA_PMU_DELAY_BIT_CP_MASK        (0x3<<14)
#define RDA_PMU_DELAY_BIT_CP(n)          (((n)&0x3)<<14)
// end of speaker PA setting1

// sim ACT/LP/PON/POFF setting 0x28/0x29/0x2a/0x2b
#define RDA_PMU_VSIM4_IBIT_SHIFT         (0)
#define RDA_PMU_VSIM4_IBIT_MASK          (7<<0)
#define RDA_PMU_VSIM4_IBIT(n)            (((n)&7)<<0)

#define RDA_PMU_VSIM3_IBIT_SHIFT         (3)
#define RDA_PMU_VSIM3_IBIT_MASK          (7<<3)
#define RDA_PMU_VSIM3_IBIT(n)            (((n)&7)<<3)

#define RDA_PMU_VSIM2_IBIT_SHIFT         (6)
#define RDA_PMU_VSIM2_IBIT_MASK          (7<<6)
#define RDA_PMU_VSIM2_IBIT(n)            (((n)&7)<<6)

#define RDA_PMU_VSIM1_IBIT_SHIFT         (9)
#define RDA_PMU_VSIM1_IBIT_MASK          (7<<9)
#define RDA_PMU_VSIM1_IBIT(n)            (((n)&7)<<9)

#define RDA_PMU_VSIM4_OFF                (1<<12)
#define RDA_PMU_VSIM3_OFF                (1<<13)
#define RDA_PMU_VSIM2_OFF                (1<<14)
#define RDA_PMU_VSIM1_OFF                (1<<15)
// end sim interface2

//0x2c
#define RDA_PMU_PU_BT_REG				 (1<<0)
#define RDA_PMU_PU_BT_DR				 (1<<1)
#define RDA_PMU_PU_FM_REG				 (1<<2)
#define RDA_PMU_PU_FM_DR				 (1<<3)
//#define RDA_PMU_R2C_RESERVED_1		 (0<<4)
#define RDA_PMU_RESETN_ABB_REG			 (1<<6)
#define RDA_PMU_RESETN_ABB_DR			 (1<<7)
#define RDA_PMU_RESETN_TRANSC_REG		 (1<<8)
#define RDA_PMU_RESETN_TRANSC_DR		 (1<<9)
#define RDA_PMU_ABB_RESETN_TIME_SEL(n)	 (((n)&0x3)<<10)
#define RDA_PMU_TRANSC_RESETN_TIME_SEL(n) (((n)&0x3)<<12)
//#define RDA_PMU_R2C_RESERVED_2		 (0<<14)
#define RDA_PMU_PA_ON_BYPASS			 (1<<15)	
//end of 0x2c

// LDO buck1/buck2 0x2d/0x2e
#define RDA_PMU_ANTIRING_DIS_BUCK        (1<<0)
#define RDA_PMU_HEAVY_LOAD               (1<<1)
#define RDA_PMU_DISCHARGE_EN             (1<<2)
#define RDA_PMU_LOW_SENSE                (1<<3)

#define RDA_PMU_OSC_FREQ(n)              (((n)&3)<<4)
#define RDA_PMU_OSC_FREQ_MASK            (3<<4)
#define RDA_PMU_OSC_FREQ_SHIFT           (4)

#define RDA_PMU_PFM_THRESHOLD(n)         (((n)&3)<<6)
#define RDA_PMU_PFM_THRESHOLD_MASK       (3<<6)
#define RDA_PMU_PFM_THRESHOLD_SHIFT      (6)

#define RDA_PMU_COUNTER_DISABLE          (1<<8)
#define RDA_PMU_PFM_CLK_DIS_BUCK_REG     (1<<9)
#define RDA_PMU_PFM_CLK_DIS_BUCK_DR      (1<<10)

#define RDA_PMU_PFM_CLK_PRD_BUCK(n)      (((n)&0x3)<<11)
#define RDA_PMU_PFM_CLK_PRD_BUCK_MASK    (0x3<<11)
#define RDA_PMU_PFM_CLK_PRD_BUCK_SHIFT   (11)

#define RDA_PMU_VBUCK_LDO_BIT(n)         (((n)&7)<<13)
#define RDA_PMU_VBUCK_LDO_BIT_MASK       (7<<13)
#define RDA_PMU_VBUCK_LDO_BIT_SHIFT      (13)
// end of LDO buck1/buck2

// DCDC buck1/buck2 0x2f
#define RDA_PMU_VBUCK2_BIT_LP(n)         (((n)&0xf)<<0)
#define RDA_PMU_VBUCK2_BIT_LP_MASK       (0xf<<0)
#define RDA_PMU_VBUCK2_BIT_LP_SHIFT      (0)

#define RDA_PMU_VBUCK2_BIT_NLP(n)        (((n)&0xf)<<4)
#define RDA_PMU_VBUCK2_BIT_NLP_MASK      (0xf<<4)
#define RDA_PMU_VBUCK2_BIT_NLP_SHIFT     (4)

#define RDA_PMU_VBUCK1_BIT_LP(n)         (((n)&0xf)<<8)
#define RDA_PMU_VBUCK1_BIT_LP_MASK       (0xf<<8)
#define RDA_PMU_VBUCK1_BIT_LP_SHIFT      (8)

#define RDA_PMU_VBUCK1_BIT_NLP(n)        (((n)&0xf)<<12)
#define RDA_PMU_VBUCK1_BIT_NLP_MASK      (0xf<<12)
#define RDA_PMU_VBUCK1_BIT_NLP_SHIFT     (12)
// end of DCDC buck1/buck2

// PMU reset 0x30
#define RDA_PMU_SOFT_RESETN              (1<<0)
#define RDA_PMU_REGISTER_RESETN          (1<<15)
// end of PMU reset

//Power_on state ldo setting1 0x31 
#define RDA_PMU_LP_MODE_B_POFF			(1<<0)
#define RDA_PMU_PU_SPIMEM_POFF			(1<<1)
#define RDA_PMU_PU_BL_LED_POFF			(1<<2)
#define RDA_PMU_PU_VMIC_POFF			(1<<3)
#define RDA_PMU_PU_VUSB_POFF			(1<<4)
#define RDA_PMU_PU_VIBR_POFF			(1<<5)
#define RDA_PMU_PU_VMC_POFF				(1<<6)
#define RDA_PMU_PU_VLCD_POFF			(1<<7)
#define RDA_PMU_PU_VIO1P8_POFF			(1<<8)
#define RDA_PMU_PU_VASW_POFF			(1<<9)
#define RDA_PMU_PU_VA_POFF				(1<<10)
#define RDA_PMU_PU_VIO2P8_POFF			(1<<11)
#define RDA_PMU_PU_VM_POFF				(1<<12)
#define RDA_PMU_PU_BUCK1_LDO_POFF		(1<<13)
#define RDA_PMU_PFM_MODE_SEL_BUCK1_POFF	(1<<14)
#define RDA_PMU_PU_BUCK1_POFF			(1<<15)
//End of Power_on state ldo setting1
 
//Power_on state ldo setting2 0x32
#define RDA_PMU_V_RES_BIT_POFF(n)			(((n)&0x3)<<0)
#define RDA_PMU_PU_BUCK2_LDO_POFF			(1<<2)
#define RDA_PMU_PFM_MODE_SEL_BUCK2_POFF		(1<<3)
#define RDA_PMU_PU_BUCK2_POFF				(1<<4)
#define RDA_PMU_PU_VBOOST_POFF				(1<<5)
#define RDA_PMU_PU_VRF28_POFF				(1<<6)
#define RDA_PMU_VIBR_VSEL_POFF				(1<<7)
#define RDA_PMU_VMC_VSEL_POFF				(1<<8)
#define RDA_PMU_VLCD_VSEL_POFF				(1<<9)
#define RDA_PMU_VIO1P8_VSEL_POFF			(1<<10)
#define RDA_PMU_VASW_VSEL_POFF				(1<<11)
#define RDA_PMU_VIO2P8_VSEL_POFF			(1<<12)
#define RDA_PMU_VRTC_VBIT_POFF(n)			(((n)&0x7)<<13)
//End of Power_on state ldo setting2
 
//Power_on state ldo setting3 0x33
#define RDA_PMU_PU_RES_BIT_POFF(n)			(((n)&0x7)<<0)
#define RDA_PMU_VIO2P8_IBIT_POFF(n)			(((n)&0x7)<<3)
#define RDA_PMU_VM_IBIT_POFF(n)				(((n)&0x7)<<6)
#define RDA_PMU_VSPIMEM_IBIT_POFF(n)		(((n)&0x7)<<9)
#define RDA_PMU_VBACKUP_VBIT_POFF(n)		(((n)&0x7)<<12)
#define RDA_PMU_PU_BL_POFF					(1<<15)
//End of Power_on state ldo setting3
 
//Power_on state ldo setting4 0x34
#define RDA_PMU_PU_LED_B_POFF				(1<<0)
#define RDA_PMU_PU_LED_G_POFF				(1<<1)
#define RDA_PMU_PU_LED_R_POFF				(1<<2)
#define RDA_PMU_PU_LED_B_PON				(1<<3)
#define RDA_PMU_PU_LED_G_PON				(1<<4)
#define RDA_PMU_PU_LED_R_PON				(1<<5)
#define RDA_PMU_CHP_CLK_CHR_S26M_SEL(n)		(((n)&0x3)<<6)
#define RDA_PMU_CHP_CLK_CHR_S4M_SEL(n)		(((n)&0x3)<<8)
#define RDA_PMU_CHOPPER_CHR_4M_26M_SEL		(1<<10)
#define RDA_PMU_CHP_CLK_BG_S26M_SEL(n)		(((n)&0x3)<<11)
#define RDA_PMU_CHP_CLK_BG_S4M_SEL(n)		(((n)&0x3)<<13)
#define RDA_PMU_CHOPPER_BG_4M_26M_SEL		(1<<15)
//End of Power_on state ldo setting4 
 
//Power_on state ldo setting5 0x35
#define RDA_PMU_VSIM4_VSEL_POFF				(1<<0)
#define RDA_PMU_VSIM3_VSEL_POFF				(1<<1)
#define RDA_PMU_VSIM2_VSEL_POFF				(1<<2)
#define RDA_PMU_VSIM1_VSEL_POFF				(1<<3)
//#define RDA_PMU_R35_RESERVED				(0x10<<4)
#define RDA_PMU_VUSB_IBIT_POFF(n)			(((n)&0x7)<<9)
#define RDA_PMU_I_BIT_BL_POFF(n)			(((n)&0xf)<<12)
//End of Power_on state ldo setting5

// Thermal Calibration 0x36//Some bits here are removed to other registers in 5855
#define RDA_PMU_BUCK_HEAVY_LOAD          (1<<8)
#define RDA_PMU_CHR_CALMODE_EN_BG_REG    (1<<9)
#define RDA_PMU_CHR_CALMODE_EN_NONCAL    (1<<10)
#define RDA_PMU_CALMODE_EN_BG_CAL        (1<<11)

#define RDA_PMU_THERMO_SEL(n)            (((n)&0x3)<<12)
#define RDA_PMU_THERMO_SEL_MASK          (0x3<<12)
#define RDA_PMU_THERMO_SEL_SHIFT         (12)

#define RDA_PMU_PU_THERMO_PROTECT        (1<<14)
#define RDA_PMU_PD_VD_LDO                (1<<15)
// end Thermal Calibration

// Debug3 0x37
#define RDA_PMU_RESETN_FM_REG            (1<<0)
#define RDA_PMU_RESETN_FM_DR             (1<<1)

#define RDA_PMU_RESETN_FM_TIME_SEL(n)    (((n)&0x3)<<2)
#define RDA_PMU_RESETN_FM_TIME_SEL_MASK  (0x3<<2)
#define RDA_PMU_RESETN_FM_TIME_SEL_SHIFT (2)

#define RDA_PMU_RESETN_BT_REG            (1<<4)
#define RDA_PMU_RESETN_BT_DR             (1<<5)

#define RDA_PMU_RESETN_BT_TIME_SEL(n)    (((n)&0x3)<<6)
#define RDA_PMU_RESETN_BT_TIME_SEL_MASK  (0x3<<6)
#define RDA_PMU_RESETN_BT_TIME_SEL_SHIFT (6)

#define RDA_PMU_OVER_TEMP_BYPASS         (1<<9)
#define RDA_PMU_PU_BGAP_REG              (1<<10)
#define RDA_PMU_PU_BGAP_DR               (1<<11)
#define RDA_PMU_RESETN_BB_REG            (1<<12)
#define RDA_PMU_RESETN_BB_DR             (1<<13)
#define RDA_PMU_RESETN_TSC_REG           (1<<14)
#define RDA_PMU_RESETN_TSC_DR            (1<<15)
// end Debug3

// LED Setting4 0x38
#define RDA_PMU_PWM_B_DUTY_CYCLE(n)         (((n)&0x1f)<<1)
#define RDA_PMU_PWM_B_DUTY_CYCLE_MASK       (0x1f<<1)
#define RDA_PMU_PWM_B_DUTY_CYCLE_SHIFT      (1)

#define RDA_PMU_PWM_G_DUTY_CYCLE(n)         (((n)&0x1f)<<6)
#define RDA_PMU_PWM_G_DUTY_CYCLE_MASK       (0x1f<<6)
#define RDA_PMU_PWM_G_DUTY_CYCLE_SHIFT      (6)

#define RDA_PMU_PWM_R_DUTY_CYCLE(n)         (((n)&0x1f)<<11)
#define RDA_PMU_PWM_R_DUTY_CYCLE_MASK       (0x1f<<11)
#define RDA_PMU_PWM_R_DUTY_CYCLE_SHIFT      (11)
//End of LED Setting4

//0x39
#define RDA_PMU_BAT_VOL_MAX_REG(n)			(((n)&0x1ff)<<0)
#define RDA_PMU_BAT_VOL_MAX_DR				(1<<10)
//#define RDA_PMU_R39_RESERVED				(0x18<<11)
//End of 0x39

//0x3a   
#define RDA_PMU_RESETN_GPADC_REG			(1<<0)
#define RDA_PMU_RESETN_GPADC_DR				(1<<1)
#define RDA_PMU_GPADC_START_REG					(1<<2)
#define RDA_PMU_GPADC_START_DR					(1<<3)
#define RDA_PMU_TIME_SAMP_NEG_GPADC			(1<<4)
#define RDA_PMU_TIME_SAMP_POS_GPADC			(1<<5)
#define RDA_PMU_TIMER_SAR_STABLE_SEL(n)		(((n)&0x7)<<6)
#define RDA_PMU_DELAY_BEFORE_SAMP_GPADC(n)	(((n)&0x3)<<9)
#define RDA_PMU_SAR_OUT_POLARITY_GPADC		(1<<11)
#define RDA_PMU_SAR_ADC_MODE_GPADC			(1<<12)
#define RDA_PMU_CONV_CLK_INV_GPADC			(1<<13)
#define RDA_PMU_SAMP_CLK_INV_GPADC			(1<<14)
#define RDA_PMU_GPADC_EOC_PHASE_SEL			(1<<15)
//End of 0x3a

// GPADC Setting2 / 0x3b
#if 0
#define RDA_PMU_GPADC_ENABLE                (1<<0)
#define RDA_PMU_GPADC_ENABLE_CHANNEL(n)     ((1<<((n)&0x7))<<1)
#define RDA_PMU_GPADC_ENABLE_CHANNEL_MASK   (0xff<<1)
#define RDA_PMU_GPADC_ENABLE_CHANNEL_SHIFT  (1)
#endif
#define RDA_PMU_GPADC_ENABLE_CHANNEL(n)     ((1<<((n)&0x7))<<0)
#define RDA_PMU_GPADC_ENABLE_CHANNEL_MASK   (0xff<<0)
#define RDA_PMU_GPADC_ENABLE_CHANNEL_SHIFT  (0)

#define RDA_PMU_GPADC_ENABLE_DR               (1<<8)

#define RDA_PMU_GPADC_VREF_BIT(n)           (((n)&0x3)<<9)
#define RDA_PMU_GPADC_VREF_BIT_MASK         (0x3<<9)
#define RDA_PMU_GPADC_VREF_BIT_SHIFT        (9)

#define RDA_PMU_GPADC_ENABLE                (1<<11)
#define RDA_PMU_GPADC_CLK_DIV_EN			(1<<12)

#define RDA_PMU_GPADC_START_TIME(n)         (((n)&0x7)<<13)
#define RDA_PMU_GPADC_START_TIME_MASK       (0x7<<13)
#define RDA_PMU_GPADC_START_TIME_SHIFT      (13)
//End of GPADC Setting2

//0x3c
#define RDA_PMU_VOTHER1_POFF_TIME_SEL(n)	(((n)&0x3)<<0)
#define RDA_PMU_VOTHER1_PON_TIME_SEL(n)		(((n)&0x3)<<2)
#define RDA_PMU_VMEM_POFF_TIME_SEL(n)		(((n)&0x3)<<4)
#define RDA_PMU_VMEM_PON_TIME_SEL(n)		(((n)&0x3)<<6)
#define RDA_PMU_VIO_POFF_TIME_SEL(n)		(((n)&0x3)<<8)
#define RDA_PMU_VIO_PON_TIME_SEL(n)			(((n)&0x3)<<10)
#define RDA_PMU_VCORE_POFF_TIME_SEL(n)		(((n)&0x3)<<12)
#define RDA_PMU_VCORE_PON_TIME_SEL(n)		(((n)&0x3)<<14)
//End of 0x3c

//0x3d
#define RDA_PMU_LV_DETECT_REG				(1<<0)
#define RDA_PMU_LV_DETECT_DR				(1<<1)
#define RDA_PMU_PU_DVDD_BT					(1<<2)
#define RDA_PMU_PU_DVDD_FM					(1<<3)
#define RDA_PMU_PU_ABB_REG					(1<<4)
#define RDA_PMU_PU_ABB_DIR					(1<<5)
#define RDA_PMU_TRANSC_REG					(1<<6)
#define RDA_PMU_TRANSC_DIR					(1<<7)
#define RDA_PMU_R3D_RESERVED				(1<<8)
#define RDA_PMU_TIMER_THERMO_CHECK(n)		(((n)&0x7)<<9)
#define RDA_PMU_VOTHER2_POFF_TIME_SEL(n)	(((n)&0x3)<<12)
#define RDA_PMU_VOTHER2_PON_TIME_SEL(n)		(((n)&0x3)<<14)
//End of 0x3d

// LED Setting5 / 0x3e
#define RDA_PMU_LED_B_IBIT(n)         (((n)&0x7)<<0)
#define RDA_PMU_LED_B_IBIT_MASK       (0x7<<0)
#define RDA_PMU_LED_B_IBIT_SHIFT      (0)

#define RDA_PMU_LED_G_IBIT(n)         (((n)&0x7)<<3)
#define RDA_PMU_LED_G_IBIT_MASK       (0x7<<3)
#define RDA_PMU_LED_G_IBIT_SHIFT      (3)

#define RDA_PMU_LED_R_IBIT(n)         (((n)&0x7)<<6)
#define RDA_PMU_LED_R_IBIT_MASK       (0x7<<6)
#define RDA_PMU_LED_R_IBIT_SHIFT      (6)

#define RDA_PMU_LED_B_OFF_LP          (1<<9)
#define RDA_PMU_LED_G_OFF_LP          (1<<10)
#define RDA_PMU_LED_R_OFF_LP          (1<<11)
#define RDA_PMU_LED_B_OFF_ACT         (1<<12)
#define RDA_PMU_LED_G_OFF_ACT         (1<<13)
#define RDA_PMU_LED_R_OFF_ACT         (1<<14)
#define RDA_PMU_GBIT_ABB_EN           (1<<15)
// end LED Setting5

// Speaker PA setting2 / 0x40
#define RDA_PMU_NCN_EN_R              (1<<0)
#define RDA_PMU_NCN_EN_L              (1<<1)
#define RDA_PMU_NO_DETECT_EN          (1<<2)
#define RDA_PMU_INTERFACE_MODE_EN     (1<<3)

#define RDA_PMU_VCLG_BIT(n)           (((n)&0x3)<<4)
#define RDA_PMU_VCLG_BIT_MASK         (0x3<<4)
#define RDA_PMU_VCLG_BIT_SHIFT        (4)

#define RDA_PMU_VNCN_BIT(n)           (((n)&0x3)<<6)
#define RDA_PMU_VNCN_BIT_MASK         (0x3<<6)
#define RDA_PMU_VNCN_BIT_SHIFT        (6)

#define RDA_PMU_GBIT_R_CLG_REG(n)     (((n)&0x7)<<8)
#define RDA_PMU_GBIT_R_CLG_REG_MASK   (0x7<<8)
#define RDA_PMU_GBIT_R_CLG_REG_SHIFT  (8)

#define RDA_PMU_GBIT_L_CLG_REG(n)     (((n)&0x7)<<11)
#define RDA_PMU_GBIT_L_CLG_REG_MASK   (0x7<<11)
#define RDA_PMU_GBIT_L_CLG_REG_SHIFT  (11)

#define RDA_PMU_I_BIT_CLG_REG(n)      (((n)&0x3)<<14)
#define RDA_PMU_I_BIT_CLG_REG_MASK    (0x3<<14)
#define RDA_PMU_I_BIT_CLG_REG_SHIFT   (14)
// end of speaker PA setting2

// Speaker PA setting3 / 0x41
#define RDA_PMU_SETUP_TMCTRL_BIT(n)         (((n)&0x3)<<0)
#define RDA_PMU_SETUP_TMCTRL_BIT_MASK       (0x3<<0)
#define RDA_PMU_SETUP_TMCTRL_BIT_SHIFT      (0)

#define RDA_PMU_DMODE_EN_CLG(n)             (((n)&0x7)<<2)
#define RDA_PMU_DMODE_EN_CLG_MASK           (0x7<<2)
#define RDA_PMU_DMODE_EN_CLG_SHIFT          (2)
#define RDA_PMU_DMODE_R_CLAB_L_CLAB         RDA_PMU_DMODE_EN_CLG(1)
#define RDA_PMU_DMODE_R_CLAB_L_CLD          RDA_PMU_DMODE_EN_CLG(3)
#define RDA_PMU_DMODE_R_CLD_L_CLAB          RDA_PMU_DMODE_EN_CLG(5)
#define RDA_PMU_DMODE_R_CLD_L_CLD           RDA_PMU_DMODE_EN_CLG(7)

#define RDA_PMU_MONO_L_R_SEL                (1<<5)
#define RDA_PMU_TEST_N_CP                   (1<<6)
#define RDA_PMU_TEST_P_CP                   (1<<7)
#define RDA_PMU_R_ENABLE_CLG                (1<<8)
#define RDA_PMU_L_ENABLE_CLG                (1<<9)
#define RDA_PMU_OTHER_DELEY_NEG             (1<<10)
#define RDA_PMU_OTHER_DELEY_POS             (1<<11)

#define RDA_PMU_RESET_CLG_DELAY_NEG(n)      (((n)&0x3)<<12)
#define RDA_PMU_RESET_CLG_DELAY_NEG_MASK    (0x3<<12)
#define RDA_PMU_RESET_CLG_DELAY_NEG_SHIFT   (12)

#define RDA_PMU_RESET_CLG_DELAY_POS(n)      (((n)&0x3)<<14)
#define RDA_PMU_RESET_CLG_DELAY_POS_MASK    (0x3<<14)
#define RDA_PMU_RESET_CLG_DELAY_POS_SHIFT   (14)
// end of speaker PA setting3

// Speaker PA setting4 / 0x42
#define RDA_PMU_RESET_R_CLG_REG       (1<<0)
#define RDA_PMU_RESET_R_CLG_DR        (1<<1)
#define RDA_PMU_RESET_L_CLG_REG       (1<<2)
#define RDA_PMU_RESET_L_CLG_DR        (1<<3)
#define RDA_PMU_RESET_CLG_REG         (1<<4)
#define RDA_PMU_RESET_CLG_DR          (1<<5)
#define RDA_PMU_PU_O_R_CLG_REG        (1<<6)
#define RDA_PMU_PU_O_R_CLG_DR         (1<<7)
#define RDA_PMU_PU_R_CLG_REG          (1<<8)
#define RDA_PMU_PU_R_CLG_DR           (1<<9)
#define RDA_PMU_PU_O_L_CLG_REG        (1<<10)
#define RDA_PMU_PU_O_L_CLG_DR         (1<<11)
#define RDA_PMU_PU_L_CLG_REG          (1<<12)
#define RDA_PMU_PU_L_CLG_DR           (1<<13)
#define RDA_PMU_PU_CLG_REG            (1<<14)
#define RDA_PMU_PU_CLG_DR             (1<<15)
// end of speaker PA setting4

//0x43
#define RDA_PMU_DOUBLE_MODE_EN_CLG		(1<<8)
//End of 0x43

//0x44
#define RDA_PMU_IOMUX_ABB_SPK_CFG_0_15(n)	(((n)&0xffff)<<0)
//End of 0x44

//0x45
#define RDA_PMU_IOMUX_ABB_SPK_CFG_16_17(n)	(((n)&0x3)<<0)
//End of 0x45

// TSC and key control / 0x46
#define RDA_PMU_SAR_VREF_MS_EN_GPADC  (1<<0)
#define RDA_PMU_SAR_VREF_MS_EN_TSC    (1<<1)
#define RDA_PMU_PU_SAR_REG            (1<<2)
#define RDA_PMU_PU_SAR_DR             (1<<3)
#define RDA_PMU_TSC_EN_ANA_REG        (1<<4)
#define RDA_PMU_TSC_EN_ANA_DR         (1<<5)
#define RDA_PMU_KEY_EN_REG            (1<<6)
#define RDA_PMU_KEY_EN_DR             (1<<7)
#define RDA_PMU_KEY_INT_EN_REG        (1<<8)
#define RDA_PMU_KEY_INT_EN_DR         (1<<9)
#define RDA_PMU_PU_TSC_Y_REG          (1<<10)
#define RDA_PMU_PU_TSC_Y_DR           (1<<11)
#define RDA_PMU_PU_TSC_X_REG          (1<<12)
#define RDA_PMU_PU_TSC_X_DR           (1<<13)
#define RDA_PMU_PU_TSC_INT_REG        (1<<14)
#define RDA_PMU_PU_TSC_INT_DR         (1<<15)
// end of TSC and key control

//xtal_set_lp  0x60
#define RDA_PMU_XTAL_SET_LP_FIX_IBIT_XOSC_LP(n) (((n)&0x1F)<<0)     
#define RDA_PMU_XTAL_SET_LP_IBIT_XOSC_LP(n) (((n)&0xF)<<5)      
#define RDA_PMU_XTAL_SET_LP_CDAC_LP(n) (((n)&0x3F)<<10)    
//End of 0x60

//0x61
#define RDA_PMU_LP_MODE_H_REG			(1<<0)
#define RDA_PMU_LP_MODE_H_DR			(1<<1)
#define RDA_PMU_LP_MODE_EN_REG			(1<<2)
#define RDA_PMU_LP_MODE_EN_DR			(1<<3)
#define RDA_PMU_ENABLE_CLK_6P5M			(1<<4)
#define RDA_PMU_AUX_BUF_DELAY			(1<<5)
#define RDA_PMU_PD_LDO_REG				(1<<6)
#define RDA_PMU_PD_LDO_DR				(1<<7)
#define RDA_PMU_XTAL_REG_TIME_SEL(n)	(((n)&0x7)<<8)
#define RDA_PMU_LDO_PWR_TIME_SEL(n)		(((n)&0x7)<<11)
#define	RDA_PMU_GPIO_AUXCLK_EN_REG		(1<<14)
#define	RDA_PMU_GPIO_AUXCLK_EN_DR		(1<<15)
//End of 0x61

//0x62
#define RDA_PMU_STEP_OFFSET_LP(n)		(((n)&0xff)<<0)
#define RDA_PMU_STEM_OFFSET_NORMAL(n)	(((n)&0xff)<<8)
//End of 0x62

//0x63
#define RDA_PMU_STEM_OFFSET_UPDATE	(1<<0)
//End of 0x63

//0x64
#define RDA_PMU_XEN_BT_ENABLE			(1<<0)
#define RDA_PMU_CLK_26M_DIFF_EN			(1<<1)
//#define RDA_PMU_R64_RESERVED			(0x030<<2)
#define RDA_PMU_PU_XTAL_32K				(1<<11)
#define RDA_PMU_PU_LPO_32K_REG			(1<<12)
#define RDA_PMU_CK32K_XTAL_CLK_EN		(1<<13)
#define RDA_PMU_CK32K_LPO_CLK_EN		(1<<14)
#define RDA_PMU_CK32K_26MXTAL_CLK_EN	(1<<15)
//End of 0x64

//xtal_set_nor  0x65
//  0101100_0000_10000
#define RDA_PMU_XTAL_SET_NOR_FIX_IBIT_XOSC_NOR(n) (((n)&0x1F)<<0)     
#define RDA_PMU_XTAL_SET_NOR_IBIT_XOSC_NOR(n) (((n)&0xF)<<5)      
#define RDA_PMU_XTAL_SET_NOR_CDAC_NOR(n) (((n)&0x3F)<<10)    
//End of 0x65

//xtal_doubler_setting  0x66
#define RDA_PMU_XTAL_DOUBLER_SETTING_XTAL_DOUBLER_EN (1<<0)              
#define RDA_PMU_XTAL_DOUBLER_SETTING_DOUBLER_R_CTRL(n) (((n)&0x7)<<1)      
#define RDA_PMU_XTAL_DOUBLER_SETTING_DOUBLER_VBIAS_CTRL(n) (((n)&0x3)<<4)      
#define RDA_PMU_XTAL_DOUBLER_SETTING_DOUBLER_IBIAS_CTRL (1<<6)              

//0x68
#define RDA_PMU_XTAL_SET_ANA(n)		(((n)&0xf)<<0)
#define RDA_PMU_XTAL_SET_ANA_MASK		(0xf<<0)
#define RDA_PMU_XTAL_SET_ANA_SHIFT		0
#define RDA_PMU_XDRV_AUX_REG_BIT(n)	(((n)&0x7)<<4)
#define RDA_PMU_XDRV_AUX_REG_MASK	(0X7<<4)
#define RDA_PMU_XDRV_AUX_REG_SHIFT 	4
#define RDA_PMU_XDRV_AUX_STEP_EN(n)	(((n)&0xf)<<7)
#define RDA_PMU_XDRV_AUX_STEP_EN_MASK	(0xf<<7)
#define RDA_PMU_XDRV_AUX_STEP_EN_SHIFT	7
#define RDA_PMU_XTAL_RDAC_HLSEL(n)	(((n)&0x3)<<11)
#define RDA_PMU_XTAL_RDAC_HLSEL_MASK	(0x3<<11)
#define RDA_PMU_XTAL_RDAC_HLSEL_SHIFT	11
#define RDA_PMU_XTAL_REG_BIT(n)		(((n)&0x7)<<13)
#define RDA_PMU_XTAL_REG_BIT_MASK		(0x7<<13)
#define RDA_PMU_XTAL_REG_BIT_SHIFT		13
//End of 0x68                    
             
//0x6a			        
#define RDA_PMU_DIGI_AFC_BIT(n)		(((n)&0x3fff)<<0)
//End of 0x6a
// =============================================================================
//  TYPES
// =============================================================================
// ============================================================================
// RDA_PMU_CHR_STATUS_T
// -----------------------------------------------------------------------------
/// 
// =============================================================================

typedef enum {
    CHR_STATUS_UNKNOWN,
    CHR_STATUS_AC_OFF,
    CHR_STATUS_AC_ON,
} RDA_PMU_CHR_STATUS_T;


// =============================================================================
//  MACROS
// =============================================================================

/// to extract bitfield from register value
#define PMU_GET_BITFIELD(dword, bitfield) (((dword) & (bitfield ## _MASK)) >> (bitfield ## _SHIFT))
#define PMU_SET_BITFIELD(dword, bitfield, value) (((dword) & ~(bitfield ## _MASK)) | (bitfield(value)))

/// to build commands for pmd_OpalSpiSendCommand
#define PMU_SPI_BUILD_WRITE_COMMAND(cmd,regIdx,val) \
{ (*cmd) = (0<<25) | ((regIdx & 0x1ff) << 16) | (val & 0xffff); }

#define PMU_SPI_BUILD_READ_COMMAND(cmd,regIdx) \
{ (*cmd) = (1<<25) | ((regIdx & 0x1ff) << 16) | (val & 0xffff); }

#define PMU_SPI_EXTRACT_DATA(data) ((UINT16) (((data)[1] << 8) | (data)[2]))

// =============================================================================
//  FUNCTIONS
// =============================================================================

//=============================================================================
// pmd_OpalWrite
//-----------------------------------------------------------------------------
/// Read Opal register
/// @param regIdx register to read
/// @param value register value
///
/// Note: this function completely handles the SPI control
//=============================================================================
PUBLIC VOID pmd_RDAWrite(RDA_REG_MAP_T regIdx, UINT16 data);

//=============================================================================
// pmd_OpalRead
//-----------------------------------------------------------------------------
/// Read Opal register
/// @param regIdx register to read
/// @return register value
///
/// Note: this function completely handles the SPI control
//=============================================================================
PUBLIC UINT16 pmd_RDARead(RDA_REG_MAP_T regIdx);


// =============================================================================
//  Special API with lower level SPI handling (for multiple register access)
// =============================================================================

//=============================================================================
// pmd_OpalSpiActivateCs
//-----------------------------------------------------------------------------
/// Activate Opal SPI. refer to hal_SpiActivateCs() for details.
/// @return \c TRUE if the cs has been successfully activated
/// \c FALSE when another cs is already active
// =============================================================================
PUBLIC BOOL pmd_OpalSpiActivateCs(VOID);

//=============================================================================
// pmd_OpalSpiDeactivateCs
//-----------------------------------------------------------------------------
/// Deactivate Opal SPI. refer to hal_SpiDeActivateCs() for details.
//=============================================================================
PUBLIC VOID pmd_OpalSpiDeactivateCs(VOID);

//=============================================================================
// pmd_OpalSpiSendCommand
//-----------------------------------------------------------------------------
/// Send a burst of commands to Opal over SPI. SPI is configured in non DMA mode
/// caller must ensure the SPI FIFO will not overflow.
/// (reading results ensure the command has been effectively send).
/// @param cmd array of bytes containing the command, use #PMD_OPAL_SPI_BUILD_WRITE_COMMAND and #PMD_OPAL_SPI_BUILD_READ_COMMAND.
/// @param length number of SPI command to send (cmd array must by 3 time that)
//=============================================================================
PUBLIC VOID pmd_OpalSpiSendCommand(CONST UINT32* cmd, UINT32 length);

//=============================================================================
// pmd_OpalSpiGetData
//-----------------------------------------------------------------------------
/// Read a burst of data from Opal over SPI. SPI is configured in non DMA mode
/// caller must ensure the SPI FIFO will not overflow.
/// (reading results before sending too much new commands).
/// @param data array of bytes to receive the data, use #PMD_OPAL_SPI_EXTRACT_DATA
/// @param length number of SPI command recieve (data array must by 3 time that)
//=============================================================================
PUBLIC VOID pmd_OpalSpiGetData(UINT32* data, UINT32 length);


PUBLIC BOOL pmd_SpiCsNonblockingActivate(BOOL singleWrite);
PUBLIC VOID pmd_OpalCsDeactivate(BOOL singleWrite);
PUBLIC UINT16 pmd_OpalSpiRead(RDA_REG_MAP_T regIdx);
PUBLIC VOID pmd_OpalSpiWrite(RDA_REG_MAP_T regIdx, UINT16 value);
// -------------------------------------------------------------
// All the write access to RDA_ADDR_IRQ_SETTINGS must go through
// pmd_RegIrqSettingSet() or pmd_RegIrqSettingClr()
// -------------------------------------------------------------
PUBLIC VOID pmd_RegIrqSettingSet(UINT32 flags);
// -------------------------------------------------------------
// All the write access to RDA_ADDR_IRQ_SETTINGS must go through
// pmd_RegIrqSettingSet() or pmd_RegIrqSettingClr()
// -------------------------------------------------------------
PUBLIC VOID pmd_RegIrqSettingClr(UINT32 flags);


VOID pmd_InitRdaPmu(VOID);

BOOL pmd_SelectSimCard(UINT8 sim_card);

BOOL pmd_TSDGetSPI(VOID);
BOOL pmd_TSDReleaseSPI(VOID);
BOOL pmd_TsdReadCoordinatesInternal(UINT16* gpadc_x, UINT16* gpadc_y);
VOID pmd_TsdClearTouchIrq(VOID);
BOOL pmd_ResetTouch(VOID);
VOID pmd_TsdClearEomIrqInternal(VOID);
BOOL pmd_TsdEnableIrq(VOID);
BOOL pmd_TsdDisableIrq(VOID);

PROTECTED RDA_PMU_CHR_STATUS_T pmd_GetChargerHwStatus(VOID);

#endif
