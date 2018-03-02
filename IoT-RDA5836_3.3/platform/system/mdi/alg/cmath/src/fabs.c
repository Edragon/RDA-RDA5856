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
/*							fabs.c
 *
 *		Absolute value
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y;
 *
 * y = fabs( x );
 *
 *
 *
 * DESCRIPTION:
 * 
 * Returns the absolute value of the argument.
 *
 */


#include "mconf.h"
/* Avoid using UNK if possible.  */
#ifdef UNK
#if BIGENDIAN
#define MIEEE 1
#else
#define IBMPC 1
#endif
#endif

double fabs(x)
double x;
{
union
  {
    double d;
    short i[4];
  } u;

u.d = x;
#ifdef IBMPC
    u.i[3] &= 0x7fff;
#endif
#ifdef MIEEE
    u.i[0] &= 0x7fff;
#endif
#ifdef DEC
    u.i[3] &= 0x7fff;
#endif
#ifdef UNK
if( u.d < 0 )
   u.d = -u.d;
#endif
return( u.d );
}
