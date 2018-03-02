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
//------------------------------------------------------------------------
// File name      : bt_core                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:33                                                  
// Last modified  : 2016-12-21 13:55:33                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:33 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __BT_CORE_H__
#define __BT_CORE_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//common_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 common_ctrl_reserved_1         :1 ; /*0 :0 , No description.               */
        U32 rf_ext_mode                    :1 ; /*1 :1 , Set to 1 when using plug-in RF board in FPGA mode or working as a plug-in RF board. Set to 0 in all other modes*/
        U32 common_ctrl_reserved_0         :30; /*31:2 , No description.               */
    } b;
} T_BT_CORE_common_ctrl;

//tport_ctrl
typedef union
{
    U32 v;
    struct
    {
        U32 tport_data0_sel                :4 ; /*3 :0 , select tport data0            */
        U32 tport_ctrl_reserved_3          :4 ; /*7 :4 , No description.               */
        U32 tport_data1_sel                :4 ; /*11:8 , select tport data1            */
        U32 tport_ctrl_reserved_2          :4 ; /*15:12, No description.               */
        U32 tport_trig_sel                 :4 ; /*19:16, select tport trigger          */
        U32 tport_ctrl_reserved_1          :4 ; /*23:20, No description.               */
        U32 tport_clk_sel                  :4 ; /*27:24, select tport clock            */
        U32 tport_ctrl_reserved_0          :4 ; /*31:28, No description.               */
    } b;
} T_BT_CORE_tport_ctrl;

//trx_on_force
typedef union
{
    U32 v;
    struct
    {
        U32 rxon_value                     :1 ; /*0 :0 , No description.               */
        U32 rxon_force                     :1 ; /*1 :1 , force rxon for rf to rxon_value when rxon_force is 1*/
        U32 txon_value                     :1 ; /*2 :2 , No description.               */
        U32 txon_force                     :1 ; /*3 :3 , force txon for rf to txon_value when txon_force is 1*/
        U32 rx_cal_en                      :1 ; /*4 :4 , rx calibration enable         */
        U32 tx_cal_en                      :1 ; /*5 :5 , tx calibration enable         */
        U32 trx_on_force_reserved_0        :26; /*31:6 , No description.               */
    } b;
} T_BT_CORE_trx_on_force;

//trx_on_timing
typedef union
{
    U32 v;
    struct
    {
        U32 modem_rf_rxoff_delay           :8 ; /*7 :0 , delay time in us to disable modem&amp;rf rx after link layer rxon disable*/
        U32 modem_rxon_delay               :8 ; /*15:8 , delay time in us to enable modem rx after link layer rxon enable*/
        U32 modem_rf_txoff_delay           :8 ; /*23:16, delay time in us to disable modem&amp;rf tx after link layer txon disable*/
        U32 modem_txon_delay               :8 ; /*31:24, delay time in us to enable modem tx after link layer txon enable*/
    } b;
} T_BT_CORE_trx_on_timing;

typedef struct
{
    volatile           T_BT_CORE_common_ctrl                    common_ctrl; /*0x0  , RW   , 0x00000000, No description.               */
    volatile            T_BT_CORE_tport_ctrl                     tport_ctrl; /*0x4  , RW   , 0x00000000, No description.               */
    volatile                             U32                  reserved_1[3];
    volatile          T_BT_CORE_trx_on_force                   trx_on_force; /*0x14 , RW   , 0x00000000, No description.               */
    volatile         T_BT_CORE_trx_on_timing                  trx_on_timing; /*0x18 , RW   , 0x30103000, No description.               */
} T_HWP_BT_CORE_T;
#else
typedef struct
{
    volatile                             U32                    common_ctrl; /*0x0  , RW   , 0x00000000, No description.               */
    volatile                             U32                     tport_ctrl; /*0x4  , RW   , 0x00000000, No description.               */
    volatile                             U32                  reserved_2[3];
    volatile                             U32                   trx_on_force; /*0x14 , RW   , 0x00000000, No description.               */
    volatile                             U32                  trx_on_timing; /*0x18 , RW   , 0x30103000, No description.               */
} T_HWP_BT_CORE_T;

#define BT_CORE_RF_EXT_MODE            (1<<1)              


#define BT_CORE_TPORT_DATA0_SEL(n)     (((n)&0xF)<<0)      
#define BT_CORE_TPORT_DATA0_SEL_MASK   (0xF<<0)            
#define BT_CORE_TPORT_DATA0_SEL_SHIFT  (0)                 

#define BT_CORE_TPORT_DATA1_SEL(n)     (((n)&0xF)<<8)      
#define BT_CORE_TPORT_DATA1_SEL_MASK   (0xF<<8)            
#define BT_CORE_TPORT_DATA1_SEL_SHIFT  (8)                 

#define BT_CORE_TPORT_TRIG_SEL(n)      (((n)&0xF)<<16)     
#define BT_CORE_TPORT_TRIG_SEL_MASK    (0xF<<16)           
#define BT_CORE_TPORT_TRIG_SEL_SHIFT   (16)                

#define BT_CORE_TPORT_CLK_SEL(n)       (((n)&0xF)<<24)     
#define BT_CORE_TPORT_CLK_SEL_MASK     (0xF<<24)           
#define BT_CORE_TPORT_CLK_SEL_SHIFT    (24)                


#define BT_CORE_RXON_VALUE             (1<<0)              

#define BT_CORE_RXON_FORCE             (1<<1)              

#define BT_CORE_TXON_VALUE             (1<<2)              

#define BT_CORE_TXON_FORCE             (1<<3)              

#define BT_CORE_RX_CAL_EN              (1<<4)              

#define BT_CORE_TX_CAL_EN              (1<<5)              


#define BT_CORE_MODEM_RF_RXOFF_DELAY(n) (((n)&0xFF)<<0)     
#define BT_CORE_MODEM_RF_RXOFF_DELAY_MASK (0xFF<<0)           
#define BT_CORE_MODEM_RF_RXOFF_DELAY_SHIFT (0)                 

#define BT_CORE_MODEM_RXON_DELAY(n)    (((n)&0xFF)<<8)     
#define BT_CORE_MODEM_RXON_DELAY_MASK  (0xFF<<8)           
#define BT_CORE_MODEM_RXON_DELAY_SHIFT (8)                 

#define BT_CORE_MODEM_RF_TXOFF_DELAY(n) (((n)&0xFF)<<16)    
#define BT_CORE_MODEM_RF_TXOFF_DELAY_MASK (0xFF<<16)          
#define BT_CORE_MODEM_RF_TXOFF_DELAY_SHIFT (16)                

#define BT_CORE_MODEM_TXON_DELAY(n)    (((n)&0xFF)<<24)    
#define BT_CORE_MODEM_TXON_DELAY_MASK  (0xFF<<24)          
#define BT_CORE_MODEM_TXON_DELAY_SHIFT (24)                


#endif

#endif


