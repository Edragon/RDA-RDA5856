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
#ifndef REGISTER_MASK_H
#define REGISTER_MASK_H

/*
 * These registers will not be used by GCC to 
 * output mips32 code, when WITH_REDUCED_REGS 
 * is set.
 * **** DO NOT CHANGE THIS WITHOUT MODIFYING
 * **** THE IRQ HANDLER ACCORDINGLY !
 */

#ifdef __REDUCED_REGS__
/* 
* t-registers are also used as pseudo stack 
* for register save/restore in mips16
* 3 registers seem enough for most functions 
*/
   
/*
register unsigned long masked_t0 asm("$8");
register unsigned long masked_t1 asm("$9");
register unsigned long masked_t2 asm("$10");
*/

register unsigned long masked_t3 asm("$11");
register unsigned long masked_t4 asm("$12");
register unsigned long masked_t5 asm("$13");
register unsigned long masked_t6 asm("$14");
register unsigned long masked_t7 asm("$15");

/* t9 ($25) has a specific meaning from GCC's point-of-view
 * (PIC_FN_REGISTER, used in abi calls) and is used explicitly by
 * ct-gcc for mips16 compatible sibling calls in 32 bits,
 * therefore IT MUST NOT BE MASKED (if it is compilation will fail
 * anyway)
 */

register unsigned long masked_s2 asm("$18");
register unsigned long masked_s3 asm("$19");
register unsigned long masked_s4 asm("$20");
register unsigned long masked_s5 asm("$21");
register unsigned long masked_s6 asm("$22");
register unsigned long masked_s7 asm("$23");

register unsigned long masked_fp asm("$30");
#endif

#endif /* REGISTER_MASK_H */
