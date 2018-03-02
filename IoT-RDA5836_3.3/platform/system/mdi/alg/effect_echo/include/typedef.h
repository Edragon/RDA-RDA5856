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
/*
********************************************************************************
*
*      File             : typedef.c
*      Purpose          : Basic types.
*
********************************************************************************
*/


#ifndef typedef_h
#define typedef_h "$Id $"

#define Precision16
//#define Precision32
/*
 * this is the original code from the ETSI file typedef.h
 */
   
#if 1
#ifdef Precision16
//typedef signed char Word8;
typedef short Shortword;
typedef long Longword;
typedef int Flag;
#else
typedef long Shortword;
typedef int Flag;
typedef long long Longword;
#endif

#endif
#else /* not original typedef.h */

/*
 * use (improved) type definition file typdefs.h and add a "Flag" type
 */

#endif

