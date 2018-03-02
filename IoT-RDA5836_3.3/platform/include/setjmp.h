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
/* Filename:  setjmp.h														  */
/* Description:                                                               */
/*   Setjmp and longjmp prototype. POSIX standard.                            */
/*   Define jump buffer layout for MIPS setjmp/longjmp.                       */
/********************************a**********************************************/

// Attention: these functions are implemented in HAL

#ifndef SETJMP_H
#define SETJMP_H
#ifdef __cplusplus
extern "C" {
#endif


#ifdef FPU
typedef unsigned long jmp_buf[22];

#else
typedef unsigned long jmp_buf[12];
#endif

int  setjmp (jmp_buf env);
 
volatile void longjmp (jmp_buf env,  int value);
 
#ifdef __cplusplus
}
#endif
#endif //SETJMP_H

