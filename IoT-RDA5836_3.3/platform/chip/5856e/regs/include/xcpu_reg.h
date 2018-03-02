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
// File name      : xcpu_reg                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:56:05                                                  
// Last modified  : 2016-12-21 13:56:05                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:56:05 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __XCPU_REG_H__
#define __XCPU_REG_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#define NORMAL_CHEAD
#ifndef NORMAL_CHEAD
//if0_addr
typedef union
{
    U32 v;
    struct
    {
        U32 if0_addr                       :32; /*31:0 , xcpu pc addr                  */
    } b;
} T_XCPU_REG_if0_addr;

//rf0_addr
typedef union
{
    U32 v;
    struct
    {
        U32 rf0_addr                       :32; /*31:0 , program counter for the rf stage*/
    } b;
} T_XCPU_REG_rf0_addr;

//ex0_addr
typedef union
{
    U32 v;
    struct
    {
        U32 ex0_addr                       :32; /*31:0 , same value of rf0_addr, with one cycle delay*/
    } b;
} T_XCPU_REG_ex0_addr;

//dc0_addr
typedef union
{
    U32 v;
    struct
    {
        U32 dc0_addr                       :32; /*31:0 , same value of ex0_addr, with one cycle delay*/
    } b;
} T_XCPU_REG_dc0_addr;

//ap_cp0cntrl11
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_mt_PRID                    :1 ; /*0 :0 , No description.               */
        U32 cp0_mt_EPC                     :1 ; /*1 :1 , No description.               */
        U32 cp0_mt_Cause                   :1 ; /*2 :2 , No description.               */
        U32 cp0_mt_Statuts                 :1 ; /*3 :3 , No description.               */
        U32 ap_cp0cntrl11_reserved_1       :1 ; /*4 :4 , No description.               */
        U32 cp0_mt_BadVAddr                :1 ; /*5 :5 , No description.               */
        U32 ap_cp0cntrl11_reserved_0       :26; /*31:6 , No description.               */
    } b;
} T_XCPU_REG_ap_cp0cntrl11;

//ap_cp0cntr12
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_unusable_except            :1 ; /*0 :0 , No description.               */
        U32 ap_cp0cntr12_reserved_2        :6 ; /*6 :1 , No description.               */
        U32 dc0_cp3_insn                   :1 ; /*7 :7 , No description.               */
        U32 dc0_cp2_insn                   :1 ; /*8 :8 , No description.               */
        U32 dc0_cp1_insn                   :1 ; /*9 :9 , No description.               */
        U32 cp0_except                     :1 ; /*10:10, No description.               */
        U32 cp0_valid                      :1 ; /*11:11, No description.               */
        U32 ap_cp0cntr12_reserved_1        :4 ; /*15:12, No description.               */
        U32 cp0_rfe                        :1 ; /*16:16, No description.               */
        U32 cp0_mf_cp0                     :1 ; /*17:17, No description.               */
        U32 cp0_cp_sel                     :4 ; /*21:18, No description.               */
        U32 ap_cp0cntr12_reserved_0        :10; /*31:22, No description.               */
    } b;
} T_XCPU_REG_ap_cp0cntr12;

//cp0_adr_BadVAddr
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_adr_BadVAddr               :32; /*31:0 , No description.               */
    } b;
} T_XCPU_REG_cp0_adr_BadVAddr;

//cp0_cause
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_cause_reserved_3           :2 ; /*1 :0 , No description.               */
        U32 cp0_Cause_ExcCode              :4 ; /*5 :2 , 4'h0:: Int: Interrupt4'h1:: Mod: TLB modification exception4'h2:: TLBL: TLB exception (load or instruction fetch)4'h3:: TLBS: TLB exception (store)4'h4:: AdEL: Address error exception (load or instruction fetch)4'h5:: AdES: Address error exception (store) 4'h6:: IBE: Bus error exception (instruction fectch)4'h7:: DBE: Bus error exception (data reference: load or store)4'h8:: Sys: Syscall exception4'h9:: Pb: Breakpoint exception4'h10:: Ri: Reserved instruction exception4'h11:: CPU: Coprocessor Unusable exception4'h12:: Ov: Arithmetic Overflow exception*/
        U32 cp0_cause_reserved_2           :2 ; /*7 :6 , No description.               */
        U32 cp0_Cause_IP_soft              :2 ; /*9 :8 , Theses interrupt lines are software interrupts (the cpu can write in the CP0 bits to trigger and clear them)*/
        U32 cp0_Cause_IP_ext               :6 ; /*15:10, Theses interrupt lines maps to the hardware interrupt lines from the corresponding irq module*/
        U32 cp0_cause_reserved_1           :12; /*27:16, No description.               */
        U32 cp0_Cause_CE                   :2 ; /*29:28, The Coprocessor Error (CE) field indicate the coprocessor unit number referenced when a Coprocessor Unusable exception is taken*/
        U32 cp0_cause_reserved_0           :1 ; /*30:30, No description.               */
        U32 cp0_Cause_BD                   :1 ; /*31:31, The Branch Delay (BD) bit indicate whether the last exception was taken while executing in a branch delay slot1'b0:: normal1'b1:: delay_slot*/
    } b;
} T_XCPU_REG_cp0_cause;

//cp0_Status
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_Status_Iec                 :1 ; /*0 :0 , Current Interrupt Enable1'b0:: disable1'b1:: enable*/
        U32 cp0_Status_Kuc                 :1 ; /*1 :1 , Current Kernel/User mode1'b0:: kernel1'b1:: user*/
        U32 cp0_Status_IEp                 :1 ; /*2 :2 , Previous Interrupt Enable1'b0:: disable1'b1:: enable*/
        U32 cp0_Status_Kup                 :1 ; /*3 :3 , Previous Kernel/User mode1'b0:: kernel1'b1:: user*/
        U32 cp0_Status_IEo                 :1 ; /*4 :4 , Old Interrupt Enable1'b0:: disable1'b1:: enable*/
        U32 cp0_Status_Kuo                 :1 ; /*5 :5 , Old Kernel/User mode1'b0:: kernel1'b1:: user*/
        U32 cp0_Status_KUI                 :1 ; /*6 :6 , No description.               */
        U32 cp0_Status_reserved_4          :1 ; /*7 :7 , No description.               */
        U32 cp0_Status_IntMask             :8 ; /*15:8 , Interrupt Mask control the enabling of each of the external and software interrupts. (See Cause for more information on interruptions)*/
        U32 cp0_Status_SD                  :1 ; /*16:16, No description.               */
        U32 cp0_Status_CI                  :1 ; /*17:17, No description.               */
        U32 cp0_Status_reserved_3          :1 ; /*18:18, No description.               */
        U32 cp0_Status_CM                  :1 ; /*19:19, Signals that the most recent access to the cachable space resulted in cache miss*/
        U32 cp0_Status_reserved_2          :1 ; /*20:20, No description.               */
        U32 cp0_Status_TS                  :1 ; /*21:21, Signals that 2 entries in the TLB matched the virtual address. This is an error condition but the processor takes no action other than signalling it via this bit in the Status Register*/
        U32 cp0_Status_BEV                 :1 ; /*22:22, Select the location of the exception vectors in ROM or in DRAM*/
        U32 cp0_Status_reserved_1          :2 ; /*24:23, No description.               */
        U32 cp0_Status_RE                  :1 ; /*25:25, Reverse Endian in User mode   */
        U32 cp0_Status_reserved_0          :2 ; /*27:26, No description.               */
        U32 cp0_Status_CU                  :4 ; /*31:28, Control the Usability of the corresponding Coprocessor Unit*/
    } b;
} T_XCPU_REG_cp0_Status;

//cp0_EPC
typedef union
{
    U32 v;
    struct
    {
        U32 cp0_EPC                        :32; /*31:0 , Exception Program Counter. Saves the value of the program counter for the instruction  that caused the exception*/
    } b;
} T_XCPU_REG_cp0_EPC;

//ap_debug_multcntrl
typedef union
{
    U32 v;
    struct
    {
        U32 ap_debug_multcntrl             :15; /*14:0 , No description.               */
        U32 ap_debug_multcntrl_reserved_0  :17; /*31:15, No description.               */
    } b;
} T_XCPU_REG_ap_debug_multcntrl;

//regfile
typedef union
{
    U32 v;
    struct
    {
        U32 regfile                        :32; /*31:0 , No description.               */
    } b;
} T_XCPU_REG_regfile;

typedef struct
{
    volatile             T_XCPU_REG_if0_addr                       if0_addr; /*0x0  , RO   , 0x00000000, Peripheral Soft  Reset Register 1*/
    volatile             T_XCPU_REG_rf0_addr                       rf0_addr; /*0x4  , RO   , 0x00000000, rf0 Register                  */
    volatile             T_XCPU_REG_ex0_addr                       ex0_addr; /*0x8  , RO   , 0x00000000, ex0 Register                  */
    volatile             T_XCPU_REG_dc0_addr                       dc0_addr; /*0xc  , RO   , 0x00000000, dc0 Register                  */
    volatile                             U32                 reserved_1[60];
    volatile        T_XCPU_REG_ap_cp0cntrl11                  ap_cp0cntrl11; /*0x100, RO   , 0x00000000, ap_cp0 control11 Register     */
    volatile         T_XCPU_REG_ap_cp0cntr12                   ap_cp0cntr12; /*0x104, RO   , 0x00380000, ap_cp0cntrl12 Register        */
    volatile                             U32                 reserved_2[33];
    volatile     T_XCPU_REG_cp0_adr_BadVAddr               cp0_adr_BadVAddr; /*0x18c, RO   , 0x00000000, cp0_adr_BadVAddr Register     */
    volatile            T_XCPU_REG_cp0_cause                      cp0_cause; /*0x190, RO   , 0x00000000, cp0_cause Register            */
    volatile           T_XCPU_REG_cp0_Status                     cp0_Status; /*0x194, RO   , 0x00400000, cp0_Status Register           */
    volatile              T_XCPU_REG_cp0_EPC                        cp0_EPC; /*0x198, RO   , 0x00000000, cp0_EPC Register              */
    volatile                             U32                 reserved_3[57];
    volatile   T_XCPU_REG_ap_debug_multcntrl             ap_debug_multcntrl; /*0x280, RO   , 0x00000000, ap_debug_multcntrl Register   */
    volatile                             U32                 reserved_4[63];
    volatile              T_XCPU_REG_regfile                    regfile[32]; /*0x380, RO   , 0x00000000, regfile Register              */
} T_HWP_XCPU_REG_T;
#else
typedef struct
{
    volatile                             U32                       if0_addr; /*0x0  , RO   , 0x00000000, Peripheral Soft  Reset Register 1*/
    volatile                             U32                       rf0_addr; /*0x4  , RO   , 0x00000000, rf0 Register                  */
    volatile                             U32                       ex0_addr; /*0x8  , RO   , 0x00000000, ex0 Register                  */
    volatile                             U32                       dc0_addr; /*0xc  , RO   , 0x00000000, dc0 Register                  */
    volatile                             U32                 reserved_5[60];
    volatile                             U32                  ap_cp0cntrl11; /*0x100, RO   , 0x00000000, ap_cp0 control11 Register     */
    volatile                             U32                   ap_cp0cntr12; /*0x104, RO   , 0x00380000, ap_cp0cntrl12 Register        */
    volatile                             U32                 reserved_6[33];
    volatile                             U32               cp0_adr_BadVAddr; /*0x18c, RO   , 0x00000000, cp0_adr_BadVAddr Register     */
    volatile                             U32                      cp0_cause; /*0x190, RO   , 0x00000000, cp0_cause Register            */
    volatile                             U32                     cp0_Status; /*0x194, RO   , 0x00400000, cp0_Status Register           */
    volatile                             U32                        cp0_EPC; /*0x198, RO   , 0x00000000, cp0_EPC Register              */
    volatile                             U32                 reserved_7[57];
    volatile                             U32             ap_debug_multcntrl; /*0x280, RO   , 0x00000000, ap_debug_multcntrl Register   */
    volatile                             U32                 reserved_8[63];
    volatile                             U32                    regfile[32]; /*0x380, RO   , 0x00000000, regfile Register              */
} T_HWP_XCPU_REG_T;

#define XCPU_REG_IF0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_IF0_ADDR_MASK         (0xFFFFFFFF<<0)     
#define XCPU_REG_IF0_ADDR_SHIFT        (0)                 


#define XCPU_REG_RF0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_RF0_ADDR_MASK         (0xFFFFFFFF<<0)     
#define XCPU_REG_RF0_ADDR_SHIFT        (0)                 


#define XCPU_REG_EX0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_EX0_ADDR_MASK         (0xFFFFFFFF<<0)     
#define XCPU_REG_EX0_ADDR_SHIFT        (0)                 


#define XCPU_REG_DC0_ADDR(n)           (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_DC0_ADDR_MASK         (0xFFFFFFFF<<0)     
#define XCPU_REG_DC0_ADDR_SHIFT        (0)                 


#define XCPU_REG_CP0_MT_PRID           (1<<0)              

#define XCPU_REG_CP0_MT_EPC            (1<<1)              

#define XCPU_REG_CP0_MT_CAUSE          (1<<2)              

#define XCPU_REG_CP0_MT_STATUTS        (1<<3)              

#define XCPU_REG_CP0_MT_BADVADDR       (1<<5)              


#define XCPU_REG_CP0_UNUSABLE_EXCEPT   (1<<0)              

#define XCPU_REG_DC0_CP3_INSN          (1<<7)              

#define XCPU_REG_DC0_CP2_INSN          (1<<8)              

#define XCPU_REG_DC0_CP1_INSN          (1<<9)              

#define XCPU_REG_CP0_EXCEPT            (1<<10)             

#define XCPU_REG_CP0_VALID             (1<<11)             

#define XCPU_REG_CP0_RFE               (1<<16)             

#define XCPU_REG_CP0_MF_CP0            (1<<17)             

#define XCPU_REG_CP0_CP_SEL(n)         (((n)&0xF)<<18)     
#define XCPU_REG_CP0_CP_SEL_MASK       (0xF<<18)           
#define XCPU_REG_CP0_CP_SEL_SHIFT      (18)                


#define XCPU_REG_CP0_ADR_BADVADDR(n)   (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_CP0_ADR_BADVADDR_MASK (0xFFFFFFFF<<0)     
#define XCPU_REG_CP0_ADR_BADVADDR_SHIFT (0)                 


#define XCPU_REG_CP0_CAUSE_EXCCODE(n)  (((n)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_INT (((0)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_MOD (((1)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_TLBL (((2)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_TLBS (((3)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_ADEL (((4)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_ADES (((5)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_IBE (((6)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_DBE (((7)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_SYS (((8)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_PB  (((9)&0xF)<<2)      
#define XCPU_REG_CP0_CAUSE_EXCCODE_RI  (((16)&0xF)<<2)     
#define XCPU_REG_CP0_CAUSE_EXCCODE_CPU (((17)&0xF)<<2)     
#define XCPU_REG_CP0_CAUSE_EXCCODE_OV  (((18)&0xF)<<2)     
#define XCPU_REG_CP0_CAUSE_EXCCODE_MASK (0xF<<2)            
#define XCPU_REG_CP0_CAUSE_EXCCODE_SHIFT (2)                 

#define XCPU_REG_CP0_CAUSE_IP_SOFT(n)  (((n)&0x3)<<8)      
#define XCPU_REG_CP0_CAUSE_IP_SOFT_MASK (0x3<<8)            
#define XCPU_REG_CP0_CAUSE_IP_SOFT_SHIFT (8)                 

#define XCPU_REG_CP0_CAUSE_IP_EXT(n)   (((n)&0x3F)<<10)    
#define XCPU_REG_CP0_CAUSE_IP_EXT_MASK (0x3F<<10)          
#define XCPU_REG_CP0_CAUSE_IP_EXT_SHIFT (10)                

#define XCPU_REG_CP0_CAUSE_CE(n)       (((n)&0x3)<<28)     
#define XCPU_REG_CP0_CAUSE_CE_MASK     (0x3<<28)           
#define XCPU_REG_CP0_CAUSE_CE_SHIFT    (28)                

#define XCPU_REG_CP0_CAUSE_BD          (1<<31)             
#define XCPU_REG_CP0_CAUSE_BD_NORMAL   (1<<31)             
#define XCPU_REG_CP0_CAUSE_BD_DELAY_SLOT (1<<31)             


#define XCPU_REG_CP0_STATUS_IEC        (1<<0)              
#define XCPU_REG_CP0_STATUS_IEC_DISABLE (1<<0)              
#define XCPU_REG_CP0_STATUS_IEC_ENABLE (1<<0)              

#define XCPU_REG_CP0_STATUS_KUC        (1<<1)              
#define XCPU_REG_CP0_STATUS_KUC_KERNEL (1<<1)              
#define XCPU_REG_CP0_STATUS_KUC_USER   (1<<1)              

#define XCPU_REG_CP0_STATUS_IEP        (1<<2)              
#define XCPU_REG_CP0_STATUS_IEP_DISABLE (1<<2)              
#define XCPU_REG_CP0_STATUS_IEP_ENABLE (1<<2)              

#define XCPU_REG_CP0_STATUS_KUP        (1<<3)              
#define XCPU_REG_CP0_STATUS_KUP_KERNEL (1<<3)              
#define XCPU_REG_CP0_STATUS_KUP_USER   (1<<3)              

#define XCPU_REG_CP0_STATUS_IEO        (1<<4)              
#define XCPU_REG_CP0_STATUS_IEO_DISABLE (1<<4)              
#define XCPU_REG_CP0_STATUS_IEO_ENABLE (1<<4)              

#define XCPU_REG_CP0_STATUS_KUO        (1<<5)              
#define XCPU_REG_CP0_STATUS_KUO_KERNEL (1<<5)              
#define XCPU_REG_CP0_STATUS_KUO_USER   (1<<5)              

#define XCPU_REG_CP0_STATUS_KUI        (1<<6)              

#define XCPU_REG_CP0_STATUS_INTMASK(n) (((n)&0xFF)<<8)     
#define XCPU_REG_CP0_STATUS_INTMASK_MASK (0xFF<<8)           
#define XCPU_REG_CP0_STATUS_INTMASK_SHIFT (8)                 

#define XCPU_REG_CP0_STATUS_SD         (1<<16)             

#define XCPU_REG_CP0_STATUS_CI         (1<<17)             

#define XCPU_REG_CP0_STATUS_CM         (1<<19)             

#define XCPU_REG_CP0_STATUS_TS         (1<<21)             

#define XCPU_REG_CP0_STATUS_BEV        (1<<22)             

#define XCPU_REG_CP0_STATUS_RE         (1<<25)             

#define XCPU_REG_CP0_STATUS_CU(n)      (((n)&0xF)<<28)     
#define XCPU_REG_CP0_STATUS_CU_MASK    (0xF<<28)           
#define XCPU_REG_CP0_STATUS_CU_SHIFT   (28)                


#define XCPU_REG_CP0_EPC(n)            (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_CP0_EPC_MASK          (0xFFFFFFFF<<0)     
#define XCPU_REG_CP0_EPC_SHIFT         (0)                 


#define XCPU_REG_AP_DEBUG_MULTCNTRL(n) (((n)&0x7FFF)<<0)   
#define XCPU_REG_AP_DEBUG_MULTCNTRL_MASK (0x7FFF<<0)         
#define XCPU_REG_AP_DEBUG_MULTCNTRL_SHIFT (0)                 


#define XCPU_REG_REGFILE(n)            (((n)&0xFFFFFFFF)<<0)
#define XCPU_REG_REGFILE_MASK          (0xFFFFFFFF<<0)     
#define XCPU_REG_REGFILE_SHIFT         (0)                 


#endif

#endif


