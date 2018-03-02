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
// File name      : ahb_icache                                            
// Author         : liqiu                                                 
// Date of created: 2016-12-21 13:55:29                                                  
// Last modified  : 2016-12-21 13:55:29                                                  
//------------------------------------------------------------------------
// Functional Description:                                                
// N/A                                                                    
//------------------------------------------------------------------------
// Reversion History:                                                     
// 2016-12-21 13:55:29 --  Create by liqiu                                               
//------------------------------------------------------------------------
#ifndef __AHB_ICACHE_H__
#define __AHB_ICACHE_H__
//Auto-gen by reg_gen
#include "base_address.h"
#include "type_def.h"

#ifndef NORMAL_CHEAD
//r_rev_id
typedef union
{
    U32 v;
    struct
    {
        U32 rev_id                         :8 ; /*7 :0 , Revision ID                   */
        U32 r_rev_id_reserved_0            :24; /*31:8 , reserved                      */
    } b;
} T_AHB_ICACHE_r_rev_id;

//cache_conf
typedef union
{
    U32 v;
    struct
    {
        U32 rsvd                           :4 ; /*3 :0 , rsvd bits                     */
        U32 cache_size                     :2 ; /*5 :4 , Cache Size                                                                                                          2'h0: 32KB Cache; no system RAM2'h1: 16KB Cache; 16KB system RAM2'h2: 8KB Cache; 24KB system RAM2'h3: 4KB Cache; 28KB system RAM*/
        U32 cache_en                       :1 ; /*6 :6 , Cache Enable                                                                                                     1'h0: No Cache, 32KB RAM as system RAM1'h1: Cache Mode*/
        U32 irq_quick_resp                 :1 ; /*7 :7 , IRQ Quick Response            */
        U32 flash_map                      :3 ; /*10:8 , Flash Mapping for Cache       */
        U32 cache_conf_reserved_1          :1 ; /*11:11, RESERVED                      */
        U32 miss_cnt_en                    :1 ; /*12:12, Miss Count Enable             */
        U32 hit_cnt_en                     :1 ; /*13:13, Hit Count Enable              */
        U32 cache_conf_reserved_0          :18; /*31:14, RESERVED                      */
    } b;
} T_AHB_ICACHE_cache_conf;

//cache_flush
typedef union
{
    U32 v;
    struct
    {
        U32 cache_line_flush               :1 ; /*0 :0 , Cache Line Flush                                                                                               Reset value is 1, so there is one flushing after power up;Write 1 to this bit will trigger manual flushingHardware clears it after flushing is done*/
        U32 cache_flush_reserved_0         :31; /*31:1 , RESERVED                      */
    } b;
} T_AHB_ICACHE_cache_flush;

//cache_hitcnt
typedef union
{
    U32 v;
    struct
    {
        U32 hit_cnt                        :32; /*31:0 , Hit Count                                                                                                                For debug purpose. When hit_cnt_en is 1, it trackes the hit number happens on cache. It will be cleared by any write to this register*/
    } b;
} T_AHB_ICACHE_cache_hitcnt;

//cache_misscnt
typedef union
{
    U32 v;
    struct
    {
        U32 miss_cnt                       :32; /*31:0 , Miss Count                                                                                                               For debug purpose. When miss_cnt_en is 1, it trackes the miss number happens on cache. It will be cleared by any write to this register*/
    } b;
} T_AHB_ICACHE_cache_misscnt;

//cache_status
typedef union
{
    U32 v;
    struct
    {
        U32 addr_range_error               :1 ; /*0 :0 , Address Range Error                                                                                              Current flash address is not mapped to cache*/
        U32 write_error                    :1 ; /*1 :1 , Write Error                                                                                                                 Attempt to write cache*/
        U32 cache_disable_error            :1 ; /*2 :2 , Cache Disable Error                                                                                                Attempt to read during cache disabled*/
        U32 flush_done                     :1 ; /*3 :3 , Cache Flush Done                                                                                               Cache tag ram flush done*/
        U32 dev_read_error                 :1 ; /*4 :4 , Device Read ErrorAHB Master Interface gets error response*/
        U32 cache_status_reserved_0        :27; /*31:5 , RESERVED                      */
    } b;
} T_AHB_ICACHE_cache_status;

//cache_irq
typedef union
{
    U32 v;
    struct
    {
        U32 addr_range_error_irq           :1 ; /*0 :0 , Masked Address Range Error    */
        U32 write_error_irq                :1 ; /*1 :1 , Masked Write Error            */
        U32 cache_disable_error_irq        :1 ; /*2 :2 , Masked Cache Disable Error    */
        U32 flush_done_irq                 :1 ; /*3 :3 , Masked Cache Flush Done       */
        U32 dev_read_error_irq             :1 ; /*4 :4 , Masked Device Read Error      */
        U32 cache_irq_reserved_0           :27; /*31:5 , RESERVED                      */
    } b;
} T_AHB_ICACHE_cache_irq;

//cache_irq_mask
typedef union
{
    U32 v;
    struct
    {
        U32 addr_range_error_mask          :1 ; /*0 :0 , Address Range Error Mask      */
        U32 write_error_mask               :1 ; /*1 :1 , Write Error Mask              */
        U32 cache_disable_error_mask       :1 ; /*2 :2 , Cache Disable Error Mask      */
        U32 flush_done_mask                :1 ; /*3 :3 , Cache Flush Done Mask         */
        U32 dev_read_error_mask            :1 ; /*4 :4 , Device Read Error Mask        */
        U32 cache_irq_mask_reserved_0      :27; /*31:5 , RESERVED                      */
    } b;
} T_AHB_ICACHE_cache_irq_mask;

typedef struct
{
    volatile           T_AHB_ICACHE_r_rev_id                       r_rev_id; /*0x0  , RO   , 0x00000010, Revision ID                   */
    volatile         T_AHB_ICACHE_cache_conf                     cache_conf; /*0x4  , RW   , 0x00000010, Cache Configuration Register  */
    volatile        T_AHB_ICACHE_cache_flush                    cache_flush; /*0x8  , RW   , 0x00000001, Cache Line Flush Register     */
    volatile                             U32                     reserved_1;
    volatile       T_AHB_ICACHE_cache_hitcnt                   cache_hitcnt; /*0x10 , RW   , 0x00000000, Cache Hit Count Register      */
    volatile      T_AHB_ICACHE_cache_misscnt                  cache_misscnt; /*0x14 , RW   , 0x00000000, Cache Miss Count Register     */
    volatile       T_AHB_ICACHE_cache_status                   cache_status; /*0x18 , RW   , 0x00000000, Cache Raw Status Register     */
    volatile          T_AHB_ICACHE_cache_irq                      cache_irq; /*0x1c , RO   , 0x00000000, Cache IRQ Register            */
    volatile     T_AHB_ICACHE_cache_irq_mask                 cache_irq_mask; /*0x20 , RW   , 0x0000001F, Cache IRQ Mask Register       */
} T_HWP_AHB_ICACHE_T;
#else
typedef struct
{
    volatile                             U32                       r_rev_id; /*0x0  , RO   , 0x00000010, Revision ID                   */
    volatile                             U32                     cache_conf; /*0x4  , RW   , 0x00000010, Cache Configuration Register  */
    volatile                             U32                    cache_flush; /*0x8  , RW   , 0x00000001, Cache Line Flush Register     */
    volatile                             U32                     reserved_2;
    volatile                             U32                   cache_hitcnt; /*0x10 , RW   , 0x00000000, Cache Hit Count Register      */
    volatile                             U32                  cache_misscnt; /*0x14 , RW   , 0x00000000, Cache Miss Count Register     */
    volatile                             U32                   cache_status; /*0x18 , RW   , 0x00000000, Cache Raw Status Register     */
    volatile                             U32                      cache_irq; /*0x1c , RO   , 0x00000000, Cache IRQ Register            */
    volatile                             U32                 cache_irq_mask; /*0x20 , RW   , 0x0000001F, Cache IRQ Mask Register       */
} T_HWP_AHB_ICACHE_T;

#define AHB_ICACHE_REV_ID(n)           (((n)&0xFF)<<0)     
#define AHB_ICACHE_REV_ID_MASK         (0xFF<<0)           
#define AHB_ICACHE_REV_ID_SHIFT        (0)                 


#define AHB_ICACHE_RSVD(n)             (((n)&0xF)<<0)      
#define AHB_ICACHE_RSVD_MASK           (0xF<<0)            
#define AHB_ICACHE_RSVD_SHIFT          (0)                 

#define AHB_ICACHE_CACHE_SIZE(n)       (((n)&0x3)<<4)      
#define AHB_ICACHE_CACHE_SIZE_MASK     (0x3<<4)            
#define AHB_ICACHE_CACHE_SIZE_SHIFT    (4)                 

#define AHB_ICACHE_CACHE_EN            (1<<6)              

#define AHB_ICACHE_IRQ_QUICK_RESP      (1<<7)              

#define AHB_ICACHE_FLASH_MAP(n)        (((n)&0x7)<<8)      
#define AHB_ICACHE_FLASH_MAP_MASK      (0x7<<8)            
#define AHB_ICACHE_FLASH_MAP_SHIFT     (8)                 

#define AHB_ICACHE_MISS_CNT_EN         (1<<12)             

#define AHB_ICACHE_HIT_CNT_EN          (1<<13)             


#define AHB_ICACHE_CACHE_LINE_FLUSH    (1<<0)              


#define AHB_ICACHE_HIT_CNT(n)          (((n)&0xFFFFFFFF)<<0)
#define AHB_ICACHE_HIT_CNT_MASK        (0xFFFFFFFF<<0)     
#define AHB_ICACHE_HIT_CNT_SHIFT       (0)                 


#define AHB_ICACHE_MISS_CNT(n)         (((n)&0xFFFFFFFF)<<0)
#define AHB_ICACHE_MISS_CNT_MASK       (0xFFFFFFFF<<0)     
#define AHB_ICACHE_MISS_CNT_SHIFT      (0)                 


#define AHB_ICACHE_ADDR_RANGE_ERROR    (1<<0)              

#define AHB_ICACHE_WRITE_ERROR         (1<<1)              

#define AHB_ICACHE_CACHE_DISABLE_ERROR (1<<2)              

#define AHB_ICACHE_FLUSH_DONE          (1<<3)              

#define AHB_ICACHE_DEV_READ_ERROR      (1<<4)              


#define AHB_ICACHE_ADDR_RANGE_ERROR_IRQ (1<<0)              

#define AHB_ICACHE_WRITE_ERROR_IRQ     (1<<1)              

#define AHB_ICACHE_CACHE_DISABLE_ERROR_IRQ (1<<2)              

#define AHB_ICACHE_FLUSH_DONE_IRQ      (1<<3)              

#define AHB_ICACHE_DEV_READ_ERROR_IRQ  (1<<4)              


#define AHB_ICACHE_ADDR_RANGE_ERROR_MASK (1<<0)              

#define AHB_ICACHE_WRITE_ERROR_MASK    (1<<1)              

#define AHB_ICACHE_CACHE_DISABLE_ERROR_MASK (1<<2)              

#define AHB_ICACHE_FLUSH_DONE_MASK     (1<<3)              

#define AHB_ICACHE_DEV_READ_ERROR_MASK (1<<4)              


#endif

#endif


