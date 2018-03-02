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

/*************************************************************************
 *
 * MODULE NAME:    GPIO_ir.c
 * PROJECT CODE:   rda
 * DESCRIPTION:    rda ir drv.
 * MAINTAINER:     cj
 * CREATION DATE:  16/04/10
 *
 * SOURCE CONTROL: $Id: gpioir.c 1857 2010-09-01 11:25:00Z huazeng $
 *
 *
 * NOTES TO USERS:
 *   None.
 *
 *************************************************************************/
 
#include "chip_id.h"
#if (CHIP_ASIC_ID != CHIP_ASIC_ID_5855)&& (CHIP_ASIC_ID != CHIP_ASIC_ID_5856)

#include "cs_types.h"
#include "gpio_config.h"
#include "hal_sys.h"
#include "hal_gpio.h"
#include "cos.h"
#include "event.h"

UINT32 IRtimerval=0;
typedef enum
{
    IR_IDLE,
    IR_LEAD_CODE,
    IR_INTO_CUSTOM_CODE,
    IR_CUSTOM_CODE_1,
    IR_CUSTOM_CODE_2,
    IR_END_CODE
}IR_STATE;
IR_STATE g_irstate= IR_IDLE;
#define ONE_TICK_TO_US 61
UINT32 g_ircustomcode;
UINT32 g_ircustomcode_num;
UINT8 g_repenable;
extern const GPIO_CFG_CONFIG_T *g_gpio_config;
extern UINT32 irc_press_mode;

PROTECTED VOID gpio_IrqIRHandler()
{
    UINT32 val=0,hwtimer=0,div=0;
    COS_EVENT ev;
    extern UINT8 g_sys_sleep_flag;

    ev.nEventId  = 0;
    ev.nParam1  = 0;
//    ev.nParam2  = 0;//for delete nParam2
//    ev.nParam3  = 0;//for delete nParam3	
    ev.nEventId  = 0;
    
    val = hal_GpioGet(g_gpio_config->gpio_irc); //((hwp_gpio->gpio_val&0x10)>>4);
    hwtimer=hal_TimGetUpTime();
#if 0
    hal_HstSendEvent(SYS_EVENT,0x2583691);
    hal_HstSendEvent(SYS_EVENT,g_irstate);
    hal_HstSendEvent(SYS_EVENT,val);
    hal_HstSendEvent(SYS_EVENT,g_ircustomcode);
    hal_HstSendEvent(SYS_EVENT,g_ircustomcode_num);
#endif
    div = hwtimer - IRtimerval ;
    if((div*ONE_TICK_TO_US)>100000)//> 100ms
        g_irstate = IR_IDLE;

    switch(g_irstate)
    {
        case IR_IDLE:
            if(val==0)
            {
                if(g_sys_sleep_flag)
                    hal_SysRequestFreq(0, HAL_SYS_FREQ_104M, NULL);
                IRtimerval=hwtimer;
                g_irstate = IR_LEAD_CODE;
            }
            g_ircustomcode_num=0;
            break;
        case IR_LEAD_CODE:
            if(val==1)
            {
                div = hwtimer - IRtimerval ;
                if(1)//(div*ONE_TICK_TO_US)>8000 && (div*ONE_TICK_TO_US)<10000)//8ms to 10ms
                {
                    g_irstate = IR_INTO_CUSTOM_CODE;
                    IRtimerval=hwtimer;
                }
                else
                    g_irstate = IR_IDLE;
            }
            else
                g_irstate = IR_IDLE;
            //hal_HstSendEvent(0x2583691);
            break;
        case IR_INTO_CUSTOM_CODE:
            if(val==0)
            {
                div = hwtimer - IRtimerval ;
                if((div*ONE_TICK_TO_US)>=3500 && (div*ONE_TICK_TO_US)<6000)//4ms to 5ms
                {
                    g_irstate = IR_CUSTOM_CODE_1;
                    IRtimerval=hwtimer;
                    g_ircustomcode=0;
                }
                else if((div*ONE_TICK_TO_US)<3500)//(div*ONE_TICK_TO_US)>2000 && (div*ONE_TICK_TO_US)<3000)//2ms to 3ms
                {
                    // repeat mode
                    g_irstate = IR_END_CODE;
                    g_repenable=1;
                }
                else
                    g_irstate = IR_IDLE;
            }
            else
                g_irstate = IR_IDLE;
            //hal_HstSendEvent(0x2583692);
            break;
        case IR_CUSTOM_CODE_1:
            //if(val==1)
            {
                div = hwtimer - IRtimerval ;
                if((div*ONE_TICK_TO_US)<3000)//(div*ONE_TICK_TO_US)>300 && (div*ONE_TICK_TO_US)<800)//300us to 700us
                {
                    g_irstate = IR_CUSTOM_CODE_2;
                    IRtimerval=hwtimer;
                }
                else if(val==0)
                {
                    IRtimerval=hwtimer;
                    g_ircustomcode=g_ircustomcode<<1;
                    g_ircustomcode+=0;
                    g_ircustomcode_num++;
    //                g_irstate = IR_IDLE;

                    if(g_ircustomcode_num>31)
                        g_irstate = IR_END_CODE;
                }
                else
                    g_irstate = IR_IDLE;
            }
            //else
            {
            }
            break;
        case IR_CUSTOM_CODE_2:
            //if(val==0)
            {
                div = hwtimer - IRtimerval ;
                //hal_HstSendEvent(SYS_EVENT,(div*ONE_TICK_TO_US));
                if(/*(div*ONE_TICK_TO_US)>300 && */(div*ONE_TICK_TO_US)<900)//300us to 700us
                {
                    g_ircustomcode=g_ircustomcode<<1;
                    g_ircustomcode+=0;
                    g_ircustomcode_num++;
                }
                else if(1)//(div*ONE_TICK_TO_US)<5000)//(div*ONE_TICK_TO_US)>300 && (div*ONE_TICK_TO_US)<2000)//300us to 2ms
                {
                    g_ircustomcode=g_ircustomcode<<1;
                    g_ircustomcode+=1;
                    g_ircustomcode_num++;
                }
                else
                {
                    g_irstate = IR_IDLE;
                    break;
                }

                IRtimerval=hwtimer;

                if(g_ircustomcode_num<=31)
                {
                    if(val==0)
                        g_irstate = IR_CUSTOM_CODE_1;
                }
                else
                    g_irstate = IR_END_CODE;
            }
            /*else
            {
                g_ircustomcode=g_ircustomcode<<1;
                g_ircustomcode+=0;
                g_ircustomcode_num++;
                IRtimerval=hwtimer;
                //g_irstate = IR_IDLE;

                if(g_ircustomcode_num>31)
                    g_irstate = IR_END_CODE;
            }*/
            //hal_HstSendEvent(0x2583694);
            break;
    }
    
    if(g_irstate == IR_END_CODE)
    {
        g_irstate = IR_IDLE;
        //hal_HstSendEvent(g_ircustomcode );
    	ev.nParam1  = (((g_ircustomcode>>8)&0xff)<<16)+(((g_ircustomcode>>16)&0xff)<<8)+((~(g_ircustomcode>>16))&0xff);
        irc_press_mode = g_repenable ? 8:1;
 //   	ev.nParam2  = g_repenable ? 8:1;//for delete nParam2
//    	ev.nParam3  = 0;	//for delete nParam3
    	ev.nEventId  = EV_IRC_PRESS;
        COS_SendEvent(MOD_SYS, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
        //hal_HstSendEvent(SYS_EVENT,0x2583695);
        //hal_HstSendEvent(SYS_EVENT,ev.nParam1 );
        g_repenable=0;
    }
    
    if(g_irstate == IR_IDLE && g_sys_sleep_flag)
    {
        hal_SysRequestFreq(0, HAL_SYS_FREQ_32K, NULL);
    }
}

PUBLIC VOID gpio_IRCinit(HAL_GPIO_GPIO_ID_T gpio)
{
    HAL_GPIO_CFG_T irCfg;

    g_ircustomcode=0;
    g_ircustomcode_num=0;
    g_repenable=0;
    
    irCfg.direction = HAL_GPIO_DIRECTION_INPUT;
    irCfg.irqHandler = gpio_IrqIRHandler;
    irCfg.irqMask.rising=TRUE;
    irCfg.irqMask.falling = TRUE;
    irCfg.irqMask.debounce = FALSE;
    irCfg.irqMask.level=FALSE;
    hal_GpioOpen(gpio,&irCfg);
}

#endif

