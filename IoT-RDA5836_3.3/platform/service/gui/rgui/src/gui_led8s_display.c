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

/***********************************************************************
 *
 * MODULE NAME:    gui_led_display.c
 * DESCRIPTION:    gui display Interface
 * AUTHOR:         Tianwq
 *
 *
 *
 * REMARKS:
 *
 ***********************************************************************/
#include "platform.h"
#include "ap_common.h"
#include "gui.h"
#include "string.h"

#if APP_SUPPORT_LED8S==1


/*
col: ___        1
    |   |     0   3
    |___|       2
    |   |     4   5
    |___|       6
*/
#define LED8S_BIT0	0x01
#define LED8S_BIT1	0x02
#define LED8S_BIT2	0x04
#define LED8S_BIT3	0x08
#define LED8S_BIT4	0x10
#define LED8S_BIT5	0x20
#define LED8S_BIT6	0x40
#define LED8S_BIT7	0x80

static const UINT8 g_led8s_numbers[10] = {
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,              // 0
    LED8S_BIT3|LED8S_BIT5,                                                          // 1
    LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT6,                         // 2
    LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,                         // 3
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5,                                    // 4
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT5|LED8S_BIT6,                         // 5
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,              // 6
    LED8S_BIT1|LED8S_BIT3|LED8S_BIT5,                                               // 7
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // 8
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,              // 9
    };
static const UINT8 g_led8s_latters[26] = {
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5,   // a
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // b
    LED8S_BIT2|LED8S_BIT4|LED8S_BIT6,   // c
    LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // d
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT4|LED8S_BIT6,   // e
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT4,   // f
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,   // g
    
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT4|LED8S_BIT5,   // h
    LED8S_BIT3|LED8S_BIT5,   // i
    LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,   // j
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT4|LED8S_BIT6,   // k
    LED8S_BIT0|LED8S_BIT4|LED8S_BIT6,   // l
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT4|LED8S_BIT6,   // m
    LED8S_BIT2|LED8S_BIT4|LED8S_BIT5,   // n
    
    LED8S_BIT2|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // o
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4,   // p
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5,   // q
    LED8S_BIT2|LED8S_BIT4,   // r
    LED8S_BIT0|LED8S_BIT1|LED8S_BIT2|LED8S_BIT5|LED8S_BIT6,   // s
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT4,   // t
    
    LED8S_BIT0|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // u
    LED8S_BIT4|LED8S_BIT5|LED8S_BIT6,   // v
    LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,   // w
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT5,   // x
    LED8S_BIT0|LED8S_BIT2|LED8S_BIT3|LED8S_BIT5|LED8S_BIT6,   // y
    LED8S_BIT1|LED8S_BIT2|LED8S_BIT3|LED8S_BIT4|LED8S_BIT6,   // z
};
static UINT16 g_led8s_flags;
static UINT8 g_led8s_string[8];

extern void led8s_update_screen(UINT16 flag, UINT8* Data);

void gui_led8s_display_icon(UINT16 icon_id)
{
    g_led8s_flags |= icon_id;
}

void gui_led8s_display_text(UINT8 pos, UINT8 *text)
{
    strcpy(g_led8s_string + pos, text);
}


void gui_led8s_update_screen(void)
{
    INT8 code[5];
    INT i,c;
    app_trace(APP_GUI_TRC, "gui_led8s_update_screen flag=0x%x, string=%s",g_led8s_flags, g_led8s_string);
    for(i=0; i<4; i++)
    {
        c = g_led8s_string[i];
        if(c>='0' && c<='9')
            code[i] = g_led8s_numbers[c-'0'];
        else if(c>='A' && c<='Z')
            code[i] = g_led8s_latters[c-'A'];
        else
            code[i] = 0;
    }
    code[4] = 0;
    led8s_update_screen(g_led8s_flags, code);
}

void gui_led8s_clear_screen(void)
{
    g_led8s_flags = 0;
    memset(g_led8s_string, 0, 4);
}

void gui_led8s_initialse()
{
    gui_led8s_clear_screen();
}


#endif // APP_SUPPORT_LED==1

