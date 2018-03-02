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

#ifndef _MATHHALF_H_
#define _MATHHALF_H_
#include "typedef.h"
#include "constant.h"

#define EPS 1

#define saturate(a) (((a)>STW_MAX)?(STW_MAX):((a)<STW_MIN)?(STW_MIN):((Shortword)(a))) 
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a)   ((a == STW_MIN)?(STW_MAX):((a>0)?(a):(-a)))
//#define add(a,b) (Shortword)(a + b)
//#define sub(a,b) (Shortword)(a - b)

#define add(a,b) (Shortword)saturate((Longword)(a + b))
#define sub(a,b) (Shortword)saturate((Longword)(a - b))

#define L_mult(a,b) (Longword)(((a)*(b))<<1)
#define extract_h(a) (Shortword)(0x0000ffffL & (a >> 16))
#define extract_l(a) (Shortword)(0x0000ffffL & a)
#define shl(a,b) (Shortword)((((a)<<(b))>STW_MAX)?(STW_MAX):(((a)<<(b))<STW_MIN)?(STW_MIN):((a)<<(b))) 
#define shr(a,b) (Shortword)((((a)>>(b))>STW_MAX)?(STW_MAX):(((a)>>(b))<STW_MIN)?(STW_MIN):((a)>>(b))) 

#define L_add(a,b) (Longword)((a)+(b))

#define L_round(a) (Shortword)(extract_h(L_add(a, 0x00008000L)))
#define mult(a,b) (Shortword)(extract_h(L_mult(a, b)))

/* addition */

//Shortword	add(Shortword var1, Shortword var2);                     /* 1 ops */

//Shortword	sub(Shortword var1, Shortword var2);                     /* 1 ops */

//Longword	L_add(Longword L_var1, Longword L_var2);                 /* 2 ops */

Longword	L_sub(Longword L_var1, Longword L_var2);                 /* 2 ops */

/* multiplication */

//Shortword mult(Shortword var1, Shortword var2);                    /* 1 ops */

//Longword	L_mult(Shortword var1, Shortword var2);                  /* 1 ops */

/* multiply accumulate	*/

Longword	L_mac(Longword L_var3, Shortword var1, Shortword var2);	  /* 1 op */

Longword	L_msu(Longword L_var3, Shortword var1, Shortword var2);   /* 1 op */

Shortword	msu_r(Longword L_var3, Shortword var1, Shortword var2);   /* 2 op */

/* L_round */

//Shortword	L_round(Longword L_var1);                                  /* 1 ops */

/* negation  */
Shortword	negate(Shortword var1);                                  /* 1 ops */

Longword	L_negate(Longword L_var1);                               /* 2 ops */

/* Accumulator manipulation */

Longword	L_deposit_l(Shortword var1);                             /* 1 ops */

Longword	L_deposit_h(Shortword var1);                             /* 1 ops */

//Shortword	extract_l(Longword L_var1);                              /* 1 ops */

//Shortword	extract_h(Longword L_var1);                              /* 1 ops */

/* Normalization */

Shortword	norm_l(Longword L_var1);                                /* 30 ops */

Shortword	norm_s(Shortword var1);                                 /* 15 ops */

/* Division */

Shortword	divide_s(Shortword var1, Shortword var2);               /* 18 ops */

/* arithmetic shifts */

//Shortword	shr(Shortword var1, Shortword var2);                     /* 1 ops */
//Shortword	shl(Shortword var1, Shortword var2);                     /* 1 ops */
Shortword	shift_r(Shortword var, Shortword var2);                  /* 2 ops */
Longword	L_shr(Longword L_var1, Shortword var2);                  /* 2 ops */


Longword	L_shift_r(Longword L_var, Shortword var2);               /* 3 ops */


/* Normalization */

Shortword	norm_l(Longword L_var1);                                /* 30 ops */
Shortword	norm_s(Shortword var1);                                 /* 15 ops */
Longword	L_shl(Longword L_var1, Shortword var2);                  /* 2 ops */


/* -------------------------------------------------------------------------- */
Shortword abs_s(Shortword var1);
/* 40-Bit Routines....added by Andre 11/23/99 */

/* new 40 bits basic operators */

/*Word64 L40_add(Word64 acc, Longword L_var1);

Word64 L40_sub(Word64 acc, Longword L_var1);

Word64 L40_mac(Word64 acc, Shortword var1, Shortword var2);

Word64 L40_msu(Word64 acc, Shortword var1, Shortword var2);

Word64 L40_shl(Word64 acc, Shortword var1);

Word64 L40_shr(Word64 acc, Shortword var1);

Word64 L40_negate(Word64 acc);

Shortword norm32(Word64 acc);

Longword L_sat32(Word64 acc);*/

void inc_saturation();

#endif

