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



/* FILE NAME                                                                  */
/*      cswtype.h                                                             */
/*                                                                            */
/* DESCRIPTION                                                                */
/*      all basic types or constant definitions on CSW.                       */
/*                                                                            */
/******************************************************************************/

//#define CFW_PBK_SYNC_VER  

#if !defined(__CSWTYPE_H__)
#define __CSWTYPE_H__


#define HAVE_TM_MODULE    1
#define HAVE_LIBC_MALLOC  0

#ifdef __cplusplus
#ifdef MMI_ON_WIN32
//#include "stdafx.h"
#endif
#endif

#ifdef _WINSOCKAPI_
//#error "error: can't include winsocket!"
#else
#define _WINSOCKAPI_
#endif


//#include "phy_profile.h"
#include "cp_csw.h"



#if !defined(MAX_INT8)
#define MAX_INT8            +127
#endif

#if !defined(MIN_INT8)
#define MIN_INT8            -128
#endif

#if !defined(MAX_UINT8)
#define MAX_UINT8           255
#endif

#if !defined(MIN_UINT8)
#define MIN_UINT8           0
#endif

#if !defined(MAX_INT16)
#define MAX_INT16           +32767
#endif

#if !defined(MIN_INT16)
#define MIN_INT16           -32768
#endif

#if !defined(MAX_UINT16)
#define MAX_UINT16          65535
#endif

#if !defined(MIN_UINT16)
#define MIN_UINT16          0
#endif

#if !defined(MAX_INT32)
#define MAX_INT32           +2147483647L
#endif

#if !defined(MIN_INT32)
#define MIN_INT32           -2147483648L
#endif

#if !defined(MAX_UINT32)
#define MAX_UINT32          4294967295U
#endif

#if !defined(MIN_UINT32)
#define MIN_UINT32          0U
#endif

#if !defined(MAX)
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#if !defined(MIN)
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MAKEINT16)
#define MAKEINT16(a, b)     ((INT16)(((UINT8)(a)) | ((INT16)((UINT8)(b))) << 8))
#endif

#if !defined(MAKEINT32)
#define MAKEINT32(a, b)     ((INT32)(((UINT16)(a)) | ((INT32)((UINT16)(b))) << 16))
#endif


#if !defined(MAKEUINT16)
#define MAKEUINT16(a, b)     ((UINT16)(((UINT8)(a)) | ((UINT16)((UINT8)(b))) << 8))
#endif

#if !defined(MAKEUINT32)
#define MAKEUINT32(a, b)     ((UINT32)(((UINT16)(a)) | ((UINT32)((UINT16)(b))) << 16))
#endif


#if !defined(LOINT8)
#define LOINT8(w)           ((INT8)(w))
#endif

#if !defined(HIINT8)
#define HIINT8(w)           ((INT8)((UINT16)(w) >> 8))
#endif

#if !defined(LOUINT8)
#define LOUINT8(w)           ((UINT8)(w))
#endif

#if !defined(HIUINT8)
#define HIUINT8(w)           ((UINT8)((UINT16)(w) >> 8))
#endif

#if !defined(LOINT16)
#define LOINT16(l)          ((INT16)(l))
#endif

#if !defined(HIINT16)
#define HIINT16(l)          ((INT16)((UINT32)(l) >> 16))
#endif

#if !defined(LOUINT16)
#define LOUINT16(l)          ((UINT16)(l))
#endif

#if !defined(HIUINT16)
#define HIUINT16(l)          ((UINT16)((UINT32)(l) >> 16))
#endif

#define MSB(x)    (((x) >> 8) & 0xff) // most signif byte of 2-byte integer
#define LSB(x)    ((x) & 0xff)        // least signif byte of 2-byte integer

#define MAKE_WORD(p)    (((UINT16)(p)[0] << 8) | (UINT16)(p)[1])
#define MAKE_DWORD(p)   (((UINT32)(p)[0] << 24) | ((UINT32)(p)[1] << 16) | ((UINT32)(p)[2] << 8) | (UINT32)(p)[3])

#define SWAP16(p)    (((UINT16)((UINT16*)(p))[1] << 8) | (UINT16)((UINT16*)(p))[0])
#define SWAP32(p)    (((UINT32)((UINT32*)(p))[3] << 24) | ((UINT32)((UINT32*)(p))[2] << 16) | ((UINT32)((UINT32*)(p))[1] << 8) | (UINT32)((UINT32*)(p))[0])    
#define LE_SWAP16(v) ((((UINT16)(v) & 0xff) << 8) | (((UINT16)(v) & 0xff00) >> 8))

#define PRIVATE static // function scope
#define PUBLIC

#ifndef FALSE
#define FALSE (0 == 1)
#endif
#ifndef TRUE
#define TRUE  (0 == 0)
#endif

#define NIL   ((void *) 0)

#ifndef NULL
#define NULL 0
#endif

#define CONST   const
#define VOID    void

typedef unsigned char               u8;
typedef unsigned short              u16;
typedef unsigned long               u32;
typedef char                        s8;
typedef short                       s16;
typedef long                        s32;
#ifdef __cplusplus
#else
typedef unsigned char               bool;
#endif

typedef char                        ascii;
typedef unsigned char               byte;           /*  unsigned 8-bit data     */
typedef unsigned short              word;           /*  unsigned 16-bit data    */
typedef unsigned long               dword;          /*  unsigned 32-bit data    */
typedef unsigned char               uint8;
typedef signed char                 int8;
typedef unsigned short int          uint16;
typedef signed short int            int16;
typedef unsigned int                uint32;
typedef signed int                  int32;


#ifndef size_t
typedef unsigned int                size_t;
#endif

#ifndef ssize_t
typedef int                         ssize_t;
#endif

#ifdef MMI_ON_WIN32//******************************************************MMI_ON_WIN32

/*******************************from mmi_data_types.h***************************************************************/
typedef char					S8;
typedef unsigned char  			U8;
typedef short int				S16;
typedef unsigned short int		U16; 	
typedef unsigned int			U32;
typedef int                     S32;

//#define MMI_BOOL 		kal_bool
/*******************************from mmi_data_types.h***************************************************************/


typedef unsigned char           kal_uint8;
typedef signed char             kal_int8;
typedef char                    kal_char;
typedef unsigned short          kal_wchar;

typedef unsigned short int      kal_uint16;
typedef signed short int        kal_int16;

typedef unsigned int            kal_uint32;
typedef signed int              kal_int32;
typedef  unsigned char        boolean;



typedef unsigned char       BYTE;
typedef unsigned int       UINT32;
typedef unsigned short      UINT16;
typedef unsigned char       UINT8;
typedef int       			INT32;
typedef short      			INT16;
#ifndef _BASETSD_H_
typedef char       			INT8;
#endif
typedef char       			CHAR;
typedef short				SHORT;
typedef long				LONG;
typedef int                 BOOL;
typedef void*               PVOID;
typedef UINT16              RESID;
typedef UINT32              HANDLE_WIN32;
typedef void *              HANDLE;
typedef UINT32				HRES;

#ifndef _TIME_T_DEFINED
typedef long time_t;        /* time value */
#define _TIME_T_DEFINED     /* avoid multiple def's of time_t */
#endif


#if defined(UNICODE)
typedef char* PSTR;
typedef CONST char* PCSTR;
typedef UINT16 TCHAR;
typedef UINT16 *PTCHAR;
#else
typedef char* PSTR;
typedef CONST char* PCSTR;
typedef char TCHAR;
typedef char* PTCHAR;
#endif

#define HNULL               0
#define NULL_CHAR           '\0'
#define _T_MMI
#if 0
typedef    void *va_list_win32;


#if defined(_M_IX86)
#define SRVAPI  __declspec(dllexport) 
#else
#define SRVAPI // TODO ...
#endif

#define HNULL               0

#ifdef __GNUC__
typedef INT8 Int8_32Aligned __attribute__ ((aligned (4)));
#endif

#define CPP_START  extern "C" {
#define CPP_END    }
#endif

/* zhuoxz,2009-4-27 */
#if defined(UNICODE)
#define SIZEOF(type) (sizeof(type)/sizeof(UINT16))
#else
#define SIZEOF(type) sizeof(type)
#endif

#else //************************************************************************MMI_ON_WIN32

#ifdef __GNUC__
typedef unsigned long long  UINT64;
typedef long long  			INT64;
#else // WIN32 
typedef __int64 UINT64;
typedef __int64 INT64;
#endif //__GNUC__
typedef unsigned long       UINT32; 
typedef unsigned short      UINT16;
typedef unsigned char       UINT8;
typedef long       			INT32;
typedef short      			INT16;
typedef char       			INT8;
typedef char       			CHAR;
typedef long       			DWORD;
//#ifndef MMI_ON_WIN32
	typedef int                BOOL;
//#endif
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef void*               PVOID;
//luanyy modify start
#ifndef MMI_ON_WIN32
    typedef INT32              HANDLE;
#else
	typedef UINT32				HANDLE_WIN32;
#endif
//luanyy modify end
typedef UINT8*              PUINT8;
typedef PUINT8           PU8;

typedef UINT32*             PUINT32;
typedef INT32*              PINT32;
typedef UINT16*             PUINT16;
typedef INT16*              PINT16;
//luanyy modify start
#ifndef MMI_ON_WIN32
typedef HANDLE              HKEY;
#endif
//luanyy modify end
typedef UINT16              RESID;
//luanyy modify start
#ifndef MMI_ON_WIN32
typedef HANDLE              HRES;
#else
	typedef UINT32				HRES;
#endif
//luanyy modify end
typedef UINT32              EVID;
//luanyy modify start
#ifndef MMI_ON_WIN32
typedef UINT8               LANGID;
#endif
//luanyy modify end
typedef UINT32              EVPARAM;
typedef UINT32              HAO;


#if defined(UNICODE)
#define SIZEOF(type) (sizeof(type)/sizeof(UINT16))
typedef UINT16* PSTR;
typedef CONST UINT16* PCSTR;
typedef UINT16 TCHAR;
typedef UINT16 *PTCHAR;
#define TEXT(x) L ## x
#else
#define SIZEOF(type) sizeof(type)
typedef UINT8* PSTR;
typedef CONST UINT8* PCSTR;
typedef UINT8 TCHAR;
typedef UINT8* PTCHAR;
#define TEXT(x) x
#endif

#endif //**********************************************************************8MMI_ON_WIN32

#define IS_RESOURCE_ID(x)  (!(((UINT32)(x))>>16))
#define MAKE_RES_ID(i)     (PCSTR)((DWORD)((WORD)(i)))
#define RES_ID(i)          ((WORD)(i))

#define TSTXT(x)  x 
//
// Get the address offset of the specified member.
//
#ifndef OFFSETOF
#define OFFSETOF(s,m)       (UINT32)&(((s *)0)->m)
#endif

#define ALIGN(val,exp)      (((val) + ((exp)-1)) & ~((exp)-1))

#define ARRAY_SIZE(a)       (sizeof(a)/sizeof(a[0]))

#define LAST_ELEMENT(x)     (&x[ARRAY_SIZE(x)-1])

#define TCHAR_SIZEOF(sz)    (sizeof(sz)/sizeof(TCHAR))
#define BOUND(x, min, max)  ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )

#define ROUND_SIZEOF(t)    ((sizeof(t)+sizeof(int)-1)&~(sizeof(int)-1))

#define IS_32BIT_ALIGN(nAdd) ((UINT32)(((UINT32)(nAdd)) & 0x3)==0)
#define IS_16_ALIGN(nSize)   ((UINT32)(((UINT32)(nSize))& 0x0f) == 0)

#define S_LAST_MEMBER_ADDR(ptr, type)  ((UINT8*)(ptr) + SIZEOF(type)) 
#define S_MEMBER_ADDR(s_prt, offset)   ((UINT8*)(s_prt) + offset) 

typedef UINT32 (*PFN_VOID_PTR)(VOID*);
typedef BOOL (*PFN_BOOL)();

//
// align size withe the specified bits.
//
#define ALIGN_SIZE(x, align) (((UINT32)(x)+align-1)&~(align-1))

#define Arg(arg, type) (*(type *)(arg))

#define NextArg(arg, type)  ((arg) = (TCHAR *)(arg) + roundedsizeof(type))

//
// Get the data from "arg" to "in".
//
#define ARG_IN(r,arg,type)  (memcpy(&(r), (type *) (arg), sizeof (arg)))

//
// Put the data from "w" to "arg".
//
#define ARG_OUT(arg, w, type)  (memcpy((type *) (arg), &(w), sizeof (w)))

#define ABS(x) ((x<0)?(-(x)):(x))

typedef VOID (*PFN_CALLBACK)(VOID);
//luanyy modify start
#ifndef MMI_ON_WIN32
	typedef    void *va_list;
#else
	typedef    char *va_list;
#endif

#ifndef MMI_ON_WIN32
#ifdef va_start
#undef va_start
#endif
#define    va_start(list, name) (void) (list = (void *)((TCHAR *)&name + \
           ((sizeof (name) + (sizeof (int) - 1)) & ~(sizeof (int) - 1))))
#ifdef va_arg
#undef va_arg
#endif
#define    va_arg(list, mode) ((mode *)(list = (TCHAR *)list + sizeof (mode)))[-1]
#ifdef va_end
#undef va_end
#endif
#define    va_end(list) (void)0
#endif

#ifdef __GNUC__
typedef INT8 Int8_32Aligned __attribute__ ((aligned (4)));
#endif

#if defined(_M_IX86)
#define SRVAPI  __declspec(dllexport) 
#else
#define SRVAPI // TODO ...
#endif

typedef UINT32 (*PFN_VOID)();

//
// Optimization option for compiler
//
#ifdef __GNUC__
//#define packed __attribute__((packed))
#define packed 
#else 
#ifdef _MSC_VER
#define packed
#endif
//#pragma pack (push)
//#pragma pack (1)
#endif

#if defined(_M_IX86)
#undef INLINE
#define INLINE __inline
#else 
#if defined(__GNUC__) // for MIPS CPU LITTLE_ENDIAN
#define INLINE inline // Do compiler directives
#else
#error "Don't define INLINE!" 
#endif //__GNUC__
#endif //defined(_M_IX86) && !defined(_WIN32)

#define CPP_START  extern "C" {
#define CPP_END    }

#if defined(DEBUG) && !defined(DISABLE_ASSERTS) 
#define DEBUG_BREAK()   *((UINT32 *)0) = 55   // Brute-force debug break
#else
#undef DEBUG_BREAK
#define DEBUG_BREAK()
#endif //DEBUG

#if defined(_WIN32)
#define APPENTRY  __declspec(dllexport) 
#else
#define APPENTRY // TODO ...
#endif 

#ifdef MMI_ON_WIN32
#define FS_GenVFN_SIZE 92 /* buffer size for Virtual File (include NULL tail) */
#define snprintf _snprintf  // Add by Rain,2009-8-20
#endif


#define HAL_TICK1S   1000//16384


// =============================================================================
// MS_WAITING
// -----------------------------------------------------------------------------
/// Number of delay timer during one millisecond
// =============================================================================
#define MS_WAITING      *  HAL_TICK1S / 1000

#define SECOND        * HAL_TICK1S
#define MILLI_SECOND  SECOND / 1000
#define MILLI_SECONDS MILLI_SECOND
#define SECONDS       SECOND
#define MINUTE        * ( 60 SECOND )
#define MINUTES       MINUTE
#define HOUR          * ( 60 MINUTE )
#define HOURS         HOUR


#define MILLI_SECOND  SECOND / 1000



#define BOOT_EVENT 0xff
#define SYS_EVENT 0x01
#define APP_EVENT 0x02
#define USB_EVENT 0x04

#endif // _H_

