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
/// @file hal_gpio.c                                                          //
/// This file contains HAL GPIO driver implementation                         //
//                                                                            //
//////////////////////////////////////////////////////////////////////////////// 


#include "cs_types.h"

#include "chip_id.h"

#include "global_macros.h"
#include "gpio.h"
#include "hal_config.h"
#include "hal_gpio.h"
#include "halp_gpio.h"
#include "hal_tcu.h"
#include "hal_mem_map.h"
#include "halp_debug.h"
#include "gpio_config.h"
#include "halp_irq.h"
#include "register.h"
#include "iomux.h"
#include "tgt_board_cfg.h"
// =============================================================================
// MACROS
// =============================================================================

#undef HAL_FUNC_INTERNAL
#define HAL_FUNC_INTERNAL

// GPIO use for charger detect
#ifdef CHIP_GPIO_NUM_CHARGER_DETECT
    #define HAL_GPIO_NUM_CHARGER_DETECT CHIP_GPIO_NUM_CHARGER_DETECT
#else
    #error "Charger detect GPIO must be integrated to this chip def"
#endif // CHIP_GPIO_NUM_CHARGER_DETECT

#define HAL_GPIO_SET_NUM_CHARGER_DETECT2(X) (HAL_GPIO_##X)
#define HAL_GPIO_SET_NUM_CHARGER_DETECT(X) HAL_GPIO_SET_NUM_CHARGER_DETECT2(X)

#define HAL_GPIO_CHARGER_DETECT HAL_GPIO_SET_NUM_CHARGER_DETECT(HAL_GPIO_NUM_CHARGER_DETECT)

#define SWAP_8808S_GPIO_6_AND_13(gpio)
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)
#ifndef USE_8808S_GPIO_6_IRQ
#undef SWAP_8808S_GPIO_6_AND_13
#define SWAP_8808S_GPIO_6_AND_13(gpio) \
    if (hal_SysGetChipId(HAL_SYS_CHIP_PROD_ID) != HAL_SYS_CHIP_PROD_ID_8808) \
    { \
        if (gpio == HAL_GPIO_6) { gpio = HAL_GPIO_13; } \
        else if (gpio == HAL_GPIO_13) { gpio = HAL_GPIO_6; } \
    }
#endif // USE_8808S_GPIO_6_IRQ
#endif // (CHIP_ASIC_ID == CHIP_ASIC_ID_8808)

#define SWAP_GPIO_24_AND_25(gpio) \
    if (gpio == HAL_GPIO_24) { gpio = HAL_GPIO_25; } \
    else if (gpio == HAL_GPIO_25) { gpio = HAL_GPIO_24; }


// =============================================================================
// GLOBAL VARIABLE
// =============================================================================
#ifndef HAL_NO_ASSERT
//PRIVATE CONST CHAR* gc_errNonIntGpio
//        = "Trying to set interrupt on non interruptible gpio (not between 0 and 7).";

PRIVATE CONST CHAR* gc_errWrongGpioType
        = "Wrong gpio type";
#endif
HAL_GPIO_IRQ_HANDLER_T g_halGpioRegistry[16]
        = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}; 


PUBLIC VOID hal_GpioInitAbbLibView(VOID)
{
	/*
	ABB SPI对应GPIO P1<1> P1<3> P1<4> P1<5>  DO为P15.
	CFG: A1A0303c: 0x222x2x,  x为don’t’care
	RUEN: A1A03028: 0x3axx 
	OENB: A1A03010  0x1axx
	*/
//	hwp_gpio->gpio_cfg[1] = 0x222020 | (hwp_gpio->gpio_cfg[1] & 0xFF000F0F);
//	hwp_gpio->gpio_ruen[0] |= 0x3a00;
//	hwp_gpio->gpio_oenb[0] = (hwp_gpio->gpio_oenb[0] |0x1a00) & (~0x2000);
}



extern PROTECTED BoardSetup_GPIO_CFG BoardsetupGpioCfg[HAL_GPIO_QTY];

PUBLIC VOID hal_GpioInit(VOID)
{

}


// =============================================================================
// FUNCTIONS
// =============================================================================

// =============================================================================
// hal_GpioOpen
// -----------------------------------------------------------------------------
/// Opens the a GPIO and sets its initial configuration. The GPIO's state can
/// then be changed by the others functions of this API.
/// It also sets the corresponding pin in GPIO mode.
///
/// @param gpio Identifier of the GPIO to open
/// @param cfg Initial configuration of this GPIO.
// =============================================================================
PUBLIC VOID hal_GpioOpen(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_CFG_T* cfg)
{
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);
 
    //Set direction and value (if direction is out)
    if(cfg->direction == HAL_GPIO_DIRECTION_INPUT)
    {
        hwp_gpio->low_oenb |= GPIO_GPIO_LOW_OENB(1 << apo.id);
    }
    else if(cfg->direction == HAL_GPIO_DIRECTION_OUTPUT)
    {
        hwp_gpio->low_oenb &= ~(GPIO_GPIO_LOW_OENB(1 << apo.id));
    }

    if(cfg->drv != NONEED)
    {
	    hwp_iomux->cfg[apo.id] |= IOMUX_DRV(cfg->drv);
    }
	
    if(cfg->updown != HAL_GPIO_UP_DOWN_QTY)
    {
        if(cfg->updown == HAL_GPIO_UP)
        {
            hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PU_REG;
        }
        else
        {
            hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PD_REG;
        }
    } 
}

// =============================================================================
// hal_GpioClose
// -----------------------------------------------------------------------------
/// Closes a GPIO driver. The GPIO is returned to its reset state
/// @param gpio GPIO to close.
// =============================================================================
PUBLIC VOID hal_GpioClose(HAL_GPIO_GPIO_ID_T gpio)
{    
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);

    hwp_gpio->low_oenb |= GPIO_GPIO_LOW_OENB(1 << apo.id);

    if(BoardsetupGpioCfg[apo.id].enint == TRUE)
    {
	    hal_GpioIrqDisable(BoardsetupGpioCfg[apo.id].gpio_intnum);
    }
	
}

// =============================================================================
// hal_GpioSetOut
// -----------------------------------------------------------------------------
/// This function sets one GPIO to output and leaves the others'directions
/// unchanged.
// =============================================================================
PUBLIC VOID hal_GpioSetOut(HAL_GPIO_GPIO_ID_T gpio)
{
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);

    hwp_gpio->low_oenb &= ~(GPIO_GPIO_LOW_OENB(1 << apo.id));
}



// =============================================================================
// hal_GpioSetIn
// -----------------------------------------------------------------------------
/// This function sets one GPIO to input and leaves the others'directions
/// unchanged.
// =============================================================================
PUBLIC VOID hal_GpioSetIn(HAL_GPIO_GPIO_ID_T gpio)
{
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);

    hwp_gpio->low_oenb |= GPIO_GPIO_LOW_OENB(1 << apo.id);

}



// =============================================================================
// hal_GpioGet
// -----------------------------------------------------------------------------
/// This function reads a GPIO, and returns its value. It 
/// returns the input value of GPIO configured as input and the output value 
/// for GPIO configured as output.
///
/// @return The input value for a GPIO configured as input and 
/// the output value for a GPIO configured as output.
// =============================================================================
PUBLIC UINT32 hal_GpioGet(HAL_GPIO_GPIO_ID_T gpio)
{
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return 0;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);
    if (hwp_gpio->low_in & (1 << apo.id))
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


// =============================================================================
// hal_GpioSet
// -----------------------------------------------------------------------------
/// This function sets to '1' the value of the given GPIO, GPO or TCO and leaves
/// the others unchanged
/// @param apo A GPIO, a GPO or a TCO set to '1', others being unchanged.
// =============================================================================
VOID HAL_FUNC_INTERNAL hal_GpioSet(HAL_APO_ID_T apo)
{
    switch (apo.type)
    {
        case HAL_GPIO_TYPE_NONE:
            break;
        case HAL_GPIO_TYPE_IO:
          //  hwp_gpio->gpio_set = 1 << apo.id;
            hwp_gpio->low_out |= GPIO_GPIO_LOW_OUT(1 << apo.id);
            break;
        default:
            HAL_ASSERT(FALSE, "Trying to use an invalid type of gpio %08x", apo.gpioId);
            break;
    }
}



// =============================================================================
// hal_GpioClr
// -----------------------------------------------------------------------------
/// This function sets to '0' the value of the given GPIO, GPO or TCO and leaves
/// the others unchanged
/// @param apo A GPIO, a GPO or a TCO set to '0', others being unchanged.
// =============================================================================
VOID HAL_FUNC_INTERNAL hal_GpioClr(HAL_APO_ID_T apo)
{
    switch (apo.type)
    {
        case HAL_GPIO_TYPE_NONE:
            break;
        case HAL_GPIO_TYPE_IO:
          //  hwp_gpio->gpio_clr = 1 << apo.id;
            hwp_gpio->low_out &= ~(GPIO_GPIO_LOW_OUT(1 << apo.id));
            break;
        default:
       //     HAL_ASSERT(FALSE, "Trying to use an invalid type of gpio %08x", apo.gpioId);
            break;
    }
}



// =============================================================================
// hal_GpioResetIrq
// -----------------------------------------------------------------------------
/// Clears the gpio IRQ, 
/// This function should only be called to avoid unwanted IRQ. \n
/// For regular usage, IRQ clearing is done automatically by the driver. \n
/// Rise and fall cause are cleared simultaneously
/// 
/// @param gpio A GPIO whose interrupt cause is cleared.
// ==========================================================================
PUBLIC VOID hal_GpioResetIrq(HAL_GPIO_GPIO_ID_T gpio)
{
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    
    hwp_gpio->int_clear |= (1<<BoardsetupGpioCfg[apo.id].gpio_intnum);
}

// =============================================================================
// hal_GpioIrqSetHandler
// -----------------------------------------------------------------------------
/// Sets the user IRQ handler called when an interruption is triggerd on a
/// given GPIO.
///
/// @param gpio The GPIO whose interrupt will trig the handler
/// @param handler The user function called on an interruption on this GPIO.
PUBLIC VOID hal_GpioIrqSetHandler(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_IRQ_HANDLER_T handler)
{
    UINT8 i=0,id=0;
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;
    
    for(i=0;i<TGT_HAL_PINNUM;i++)
    {
         if(BoardsetupGpioCfg[i].id == gpio)
         {
		     id = i;
		     break;
         }
    }
    g_halGpioRegistry[BoardsetupGpioCfg[id].gpio_intnum] = handler;
}

// =============================================================================
// hal_GpioIrqDisable
// -----------------------------------------------------------------------------
/// Sets the user IRQ handler called when an interruption is triggerd on a
/// given GPIO.
///
/// @param gpio is the gpio which disable 
// =============================================================================
PUBLIC VOID hal_GpioIrqDisable(uint8 int_num)
{
    g_halGpioRegistry[int_num] = NULL ;
    hwp_gpio->int_clear |= (1<<int_num);
}

// =============================================================================
// hal_GpioIrqSetMask
// -----------------------------------------------------------------------------
/// Set the condition under which an interrupt is triggered on a GPIO
/// @param gpio The GPIO
/// @param cfg Pointer to the configuration defining the condition 
/// under which the interrupts are triggered, ie the interrupt mask.
// =============================================================================
PUBLIC VOID hal_GpioIrqConfig(HAL_GPIO_GPIO_ID_T gpio, CONST HAL_GPIO_IRQ_MASK_T config,uint8 id)
{
    HAL_APO_ID_T apo;
    UINT8 reg_index = 0;  
    UINT8 offset_index = 0;
    HAL_GPIO_CFG_T cfg;
    
    apo.gpioId = gpio;
    reg_index = apo.id/8;   
    offset_index = apo.id%8;
    
    hwp_iomux->cfg[apo.id] &= ~ IOMUX_FUNC_SEL(0xF);
    hwp_iomux->cfg[apo.id] |= IOMUX_FUNC_SEL(HAL_GPIO_ALTFUNC_CFG_11);
    cfg.direction = BoardsetupGpioCfg[id].direction;
    cfg.drv    = BoardsetupGpioCfg[id].drv;
    cfg.updown  =BoardsetupGpioCfg[id].updown;
    hal_GpioOpen(BoardsetupGpioCfg[id].id, &cfg);
    hal_GpioIrqSetMask(BoardsetupGpioCfg[id].id, &config);
    g_halGpioRegistry[BoardsetupGpioCfg[id].gpio_intnum] = BoardsetupGpioCfg[id].irqHandler;
}

// =============================================================================
// hal_GpioIrqSetMask
// -----------------------------------------------------------------------------
/// Set the condition under which an interrupt is triggered on a GPIO
/// @param gpio The GPIO
/// @param cfg Pointer to the configuration defining the condition 
/// under which the interrupts are triggered, ie the interrupt mask.
// =============================================================================

PUBLIC VOID hal_GpioIrqSetMask(HAL_GPIO_GPIO_ID_T gpio, CONST HAL_GPIO_IRQ_MASK_T* cfg)
{
    UINT32 cfg1_ctrlSet = 0,cfg2_ctrlSet=0; 
    UINT32 cfg1_ctrlClr = 0,cfg2_ctrlClr=0; 
    UINT8 i=0,id=0;
    HAL_APO_ID_T apo;
    UINT8 int_num;
    
    apo.gpioId = gpio;
    for(i=0;i<TGT_HAL_PINNUM;i++)
    {
         if(BoardsetupGpioCfg[i].id == gpio)
         {
		 id = i;
		 break;
         }
    }
    int_num = BoardsetupGpioCfg[id].gpio_intnum;
    hwp_gpio->int_clear |= (1<<int_num);
    
    if(cfg->rising)
    {
        cfg1_ctrlSet |= GPIO_SET_GPINT_CTRL_R(1 << int_num);
    }
    else
    {
        cfg1_ctrlClr |= GPIO_CLR_GPINT_CTRL_R(1 << int_num);
    }
    if(cfg->falling)
    {
        cfg1_ctrlSet |= GPIO_SET_GPINT_CTRL_F(1 << int_num);
    }
    else
    {
        cfg1_ctrlClr |= GPIO_CLR_GPINT_CTRL_F(1 << int_num);
    }

    if(cfg->debounce)
    {
        cfg2_ctrlSet |= GPIO_SET_GPINT_DBN_EN(1 << int_num);
    }
    else
    {
        cfg2_ctrlClr |= GPIO_CLR_GPINT_DBN_EN(1 << int_num);
    }

    if(cfg->level)
    {
        cfg2_ctrlSet |= GPIO_SET_GPINT_MODE(1 << int_num);
    }
    else
    {
        cfg2_ctrlClr |= GPIO_CLR_GPINT_MODE(1 << int_num);
    }
   
/*   mon_Event(0x90);
   mon_Event(cfg1_ctrlSet);
    mon_Event(cfg1_ctrlClr);
	 mon_Event(0x91);
	 mon_Event(cfg2_ctrlSet);
	  mon_Event(cfg2_ctrlClr);*/
   hwp_gpio->int_cfg1_set = cfg1_ctrlSet;
    hwp_gpio->int_cfg1_clr = cfg1_ctrlClr;     

    hwp_gpio->int_cfg2_set = cfg2_ctrlSet;
   hwp_gpio->int_cfg2_clr = cfg2_ctrlClr;
}



// =============================================================================
// hal_GpioGetCfg
// -----------------------------------------------------------------------------
/// Return the configuration of a GPIO.
/// @param gpio Identifier of the GPIO.
/// @param cfg Current configuration of this GPIO.
// =============================================================================
PUBLIC VOID hal_GpioGetCfg(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_CFG_T* cfg)
{
#if 0
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;

    UINT8 reg_index = 0, i=0;
    UINT8 offset_index = 0;
    
    reg_index = apo.id/32;
    offset_index = apo.id%32;
    
    HAL_ASSERT(reg_index < 2, "reg_index %d error in hal_GpioGetCfg", reg_index);
    if(apo.type == HAL_GPIO_TYPE_NONE)
    {
        return;
    }
    HAL_ASSERT(apo.type == HAL_GPIO_TYPE_IO, gc_errWrongGpioType);

    if (hwp_gpio->gpio_oenb[reg_index] & (1 << offset_index))
    {
        cfg->direction = HAL_GPIO_DIRECTION_INPUT;
        cfg->value = ((hwp_gpio->gpio_in[reg_index] & (1 << offset_index)) != 0);
    }
    else
    {
        cfg->direction = HAL_GPIO_DIRECTION_OUTPUT;
        cfg->value = ((hwp_gpio->gpio_out[reg_index] & (1 << offset_index)) != 0);
    }

   
    if(BoardsetupGpioCfg[apo.id].enint == TRUE)
    {
         
       if(BoardsetupGpioCfg[apo.id].gpio_intnum>=8)
       {
           cfg->irqMask.rising = 
            ((hwp_gpio->gpint_int_p15_8_cfg_set & GPIO_GPINT_R_SET(1 << (BoardsetupGpioCfg[apo.id].gpio_intnum-8))) != 0);
           cfg->irqMask.falling = 
            ((hwp_gpio->gpint_int_p15_8_cfg_set & GPIO_GPINT_F_SET(1 << (BoardsetupGpioCfg[apo.id].gpio_intnum-8))) != 0);
           cfg->irqMask.debounce = 
            ((hwp_gpio->gpint_int_p15_8_cfg_set & GPIO_DBN_EN_SET(1 << (BoardsetupGpioCfg[apo.id].gpio_intnum-8))) != 0);
           cfg->irqMask.level = 
            ((hwp_gpio->gpint_int_p15_8_cfg_set & GPIO_GPINT_MODE_SET(1 <<  (BoardsetupGpioCfg[apo.id].gpio_intnum-8))) != 0);
       }
       else
       {
           cfg->irqMask.rising = 
            ((hwp_gpio->gpint_int_p7_0_cfg_set & GPIO_GPINT_R_SET(1 << BoardsetupGpioCfg[apo.id].gpio_intnum)) != 0);
           cfg->irqMask.falling = 
            ((hwp_gpio->gpint_int_p7_0_cfg_set & GPIO_GPINT_F_SET(1 << BoardsetupGpioCfg[apo.id].gpio_intnum)) != 0);
           cfg->irqMask.debounce = 
            ((hwp_gpio->gpint_int_p7_0_cfg_set & GPIO_DBN_EN_SET(1 << BoardsetupGpioCfg[apo.id].gpio_intnum)) != 0);
           cfg->irqMask.level = 
            ((hwp_gpio->gpint_int_p7_0_cfg_set & GPIO_GPINT_MODE_SET(1 << BoardsetupGpioCfg[apo.id].gpio_intnum)) != 0);
       }
       cfg->irqHandler = g_halGpioRegistry[BoardsetupGpioCfg[apo.id].gpio_intnum];
    }
#endif
}



// =============================================================================
// hal_GpioGetIntStatus
// -----------------------------------------------------------------------------
/// Return the interrupt status of a GPIO.
/// @param gpio Identifier of the GPIO.
/// @return The interrupt status of this GPIO.
// =============================================================================
PUBLIC BOOL hal_GpioGetIntStatus(HAL_GPIO_GPIO_ID_T gpio)
{

    UINT8 int_num;
    HAL_APO_ID_T apo;
    apo.gpioId = gpio;

    int_num =  BoardsetupGpioCfg[apo.id].gpio_intnum;

    UINT32 status = hwp_gpio->int_status;
	
    return ((status & (1 << int_num)) != 0);

}

// =============================================================================
// hal_GpioSetIdleLevel
// -----------------------------------------------------------------------------
/// This function sets level in idle state of specified gpio in 5855.
/// @param gpio Identifier of the GPIO.
/// @param upDown Value of raising/falling
/// @return void
// =============================================================================
PUBLIC VOID hal_GpioSetIdleLevel(HAL_GPIO_GPIO_ID_T gpio, 
                                        HAL_GPIO_UP_DOWN_ID_T upDown)
{
#if 0
    HAL_APO_ID_T apo;
    UINT8 reg_index = 0;  
    UINT8 offset_index = 0;

    apo.gpioId = gpio;
    reg_index = apo.id/8;   
    offset_index = apo.id%8;
	
    if(upDown == HAL_GPIO_UP)
    {
        hwp_iomux->IOMUX_PXX_CTRL[reg_index][offset_index] |= IOMUX_PXX_ENABLE_FORCE | IOMUX_PXX_UP_ACTIVE_FORCE | IOMUX_PXX_DIR_FORCE;
    }
    else
    {
        hwp_iomux->IOMUX_PXX_CTRL[reg_index][offset_index] |= IOMUX_PXX_ENABLE_FORCE | IOMUX_PXX_DN_ACTIVE_FORCE | IOMUX_PXX_DIR_FORCE;
    }
#endif
    return ;
}

// =============================================================================
// hal_GpioAltFuncConfig
// -----------------------------------------------------------------------------
/// This function sets specified gpio as alt func in 5855.
/// @param gpio Identifier of the GPIO.
/// @return void
// =============================================================================
PUBLIC VOID hal_GpioAltFuncConfig(HAL_GPIO_GPIO_ID_T gpio, 
                                        HAL_GPIO_ALTFUNC_CFG_ID_T cfg,
                                        UINT8 drv_strength,
                                        HAL_GPIO_UP_DOWN_ID_T updown)
{
    HAL_APO_ID_T apo;
	
    apo.gpioId = gpio;

    hwp_iomux->cfg[apo.id] &= ~ IOMUX_FUNC_SEL(0xF);
    hwp_iomux->cfg[apo.id] |= IOMUX_FUNC_SEL(cfg);
	
    if(drv_strength != NONEED)
        hwp_iomux->cfg[apo.id] |= IOMUX_DRV(drv_strength);

	if(updown != HAL_GPIO_UP_DOWN_QTY)
	{
        if(updown == HAL_GPIO_UP)
        {
            hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PU_REG;
        }
        else
        {
            hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PD_REG;
        }
    }
	  
    return ;
}

// =============================================================================
// hal_GpioVioSel
// -----------------------------------------------------------------------------
/// This function sets specified gpio as alt func in 5855.
/// @param gpio Identifier of the GPIO.
/// @return void
// =============================================================================
PUBLIC VOID hal_GpioVioSel(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_VIO_SEL_T sel)
{
#if 0
    HAL_APO_ID_T apo;
    UINT8 reg_index = 0;  
    UINT8 offset_index = 0;

    apo.gpioId = gpio;
    reg_index = apo.id/8;   
    offset_index = apo.id%8;
    
    hwp_iomux->IOMUX_PXX_CTRL[reg_index][offset_index] |= IOMUX_PXX_VIO2P8_SEL(sel);
#endif
    return ;
}

// =============================================================================
// hal_GpioIrqHandler
// -----------------------------------------------------------------------------
/// GPIO module IRQ handler
/// 
///     Clear the IRQ and call the IRQ handler
///     user function
///     @param interruptId The interruption ID
// =============================================================================  
PROTECTED VOID hal_GpioIrqHandler(UINT8 interruptId)
{

    // Get cause
    UINT32 status = hwp_gpio->int_status;

    // Clear IRQ
    hwp_gpio->int_clear = status;

    // Call user handler
    UINT32 i;

    for (i=0; i<TGT_HAL_PINNUM; i++)
    {
        if (status & (1<<i))
        {
//            hal_HstSendEvent(SYS_EVENT,i);
            if (g_halGpioRegistry[i])
            {
                g_halGpioRegistry[i]();
            }
        }
    }
}

// =============================================================================
// hal_GpioSetPinMux
// -----------------------------------------------------------------------------
/// This function sets one GPIO as ADC function
// =============================================================================
PUBLIC VOID hal_GpioUpOrDn(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_UP_DOWN_ID_T updown)
{
    HAL_APO_ID_T apo;

    apo.gpioId = gpio;
    if(updown != HAL_GPIO_UP_DOWN_QTY)
	{
        if(updown == HAL_GPIO_UP)
        {
        	hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PU_REG;
        }
        else
        {
        	hwp_iomux->cfg[apo.id] |= IOMUX_PULL_FORCE | IOMUX_PD_REG;
        }
    }
}

// =============================================================================
// hal_GpioSetPinMux
// -----------------------------------------------------------------------------
/// This function sets one GPIO as ADC function
// =============================================================================
PUBLIC VOID hal_GpioSetPinMux(HAL_GPIO_GPIO_ID_T gpio, HAL_GPIO_ALTFUNC_CFG_ID_T cfg)
{
    HAL_APO_ID_T apo;

    apo.gpioId = gpio;
    hwp_iomux->cfg[apo.id] &= ~ IOMUX_FUNC_SEL(0xf);
    hwp_iomux->cfg[apo.id] |= IOMUX_FUNC_SEL(cfg);
}

// =============================================================================
// hal_GpioSetPinMux
// -----------------------------------------------------------------------------
/// This function sets one GPIO as ADC function
// =============================================================================
PUBLIC UINT32  hal_GpioGetPinMux(HAL_GPIO_GPIO_ID_T gpio )
{
    HAL_APO_ID_T apo;

    apo.gpioId = gpio;
    return hwp_iomux->cfg[apo.id];
}

PUBLIC VOID hal_GpioPd(HAL_GPIO_GPIO_ID_T gpio, uint8 ispd)
{
    HAL_APO_ID_T apo;
 
    apo.gpioId = gpio;
    if(ispd == TRUE)
    {
     hwp_iomux->cfg[apo.id] &=~IOMUX_PDN;
    }
    else
    {
     hwp_iomux->cfg[apo.id] &=~IOMUX_PDN;
    }
}



