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



// FILENAME: cs_types.h
//
// DESCRIPTION:
//   This file defines all basic data types used by all files in RDA
//   development environment.
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   SHK           2006-10-29     Created initial version 1.0
//   Romuald       2007-04-25     Added PRIVATE, PROTECTED, REG16 and REG32
//   zhaoyong      2008-08-08     clean up
////////////////////////////////////////////////////////////////////////////////

#ifndef __CS_TYPE_H__
#define __CS_TYPE_H__

#define	FREERTOS               1
#define	SXR                    2
#define	KE                     3

#if	COMPILER == 2   //armcc
#pragma anon_unions
#define printf                dbg_trace
#endif

#define MAX_INT8                    127
#define MIN_INT8                    (-MAX_INT8 - 1)
#define MAX_UINT8                   255
#define MIN_UINT8                   0
#define MAX_INT16                   32767
#define MIN_INT16                   (-MAX_INT16 - 1)
#define MAX_UINT16                  65535
#define MIN_UINT16                  0
#define MAX_INT32                   2147483647L
#define MIN_INT32                   (-MAX_INT32 - 1)
#define MAX_UINT32                  4294967295U
#define MIN_UINT32                  0U
#ifdef WITH_LONG_LONG
#define INTMAX_MAX                  0x7fffffffffffffff
#define INTMAX_MIN                  -(0x7fffffffffffffff-1)
#define UINTMAX_MAX                 0xffffffffffffffff
#else
#define INTMAX_MAX                  0x7fffffff
#define INTMAX_MIN                  -(0x7fffffff-1)
#define UINTMAX_MAX                 0xffffffff
#endif
//#define INT_MAX                     (char *)(1<<31)
#define INT_MAX                     0x7fffffff
#define LONG_MAX                    0x7fffffff
#define LONG_MIN                    -(0x7fffffff-1)
#define ULONG_MAX                   0xffffffff


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
typedef unsigned long long          uint64;
#ifndef PLATFORM_CONFIG_H
typedef char                        boolean;
#endif
#define stdout						NULL
#define stderr						NULL

#define __always_inline

#define BITS_PER_LONG 				32

#ifndef CHAR_BIT
#define CHAR_BIT  8
#endif

#ifndef __user
#define __user
#endif

#ifndef __S64__
typedef long long					s64;
#define __S64__
#endif

#ifndef __U64__
typedef unsigned long long u64;
#define __U64__
#endif

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif


#ifndef __SIZE_T__
typedef unsigned int                size_t;
#define __SIZE_T__
#endif

#ifndef __SSIZE_T__
typedef int                         ssize_t;
#define __SIZE_T__
#endif

#ifndef __INT8_T__
typedef	char		int8_t;
#define __INT8_T__
#endif

#ifndef __INT16_T__
typedef	short		int16_t;
#define __INT16_T__
#endif

#ifndef __INT32_T__
typedef	int		int32_t;
#define __INT32_T__
#endif

#ifndef __UINT8_T__
typedef	unsigned char	uint8_t;
#define __UINT8_T__
#endif

#ifndef __UINT16_T__
typedef	unsigned short	uint16_t;
#define __UINT16_T__
#endif

#ifndef __UINT32_T__
typedef	unsigned int	uint32_t;
#define __UINT32_T__
#endif

typedef unsigned long long uint64_t;

#ifndef __PTRDIFF_T__
typedef int ptrdiff_t;
#define PTRDIFF(p1, p2, type)	((p1) - (p2))
#define __PTRDIFF_T__
#endif

#ifdef WITH_LONG_LONG
typedef unsigned long long          uintmax_t;
typedef long long                   intmax_t;
#else
typedef unsigned long               uintmax_t;
typedef long                        intmax_t;
#endif

typedef unsigned long long                UINT64;
typedef unsigned long               UINT32;
typedef unsigned short              UINT16;
typedef long long                   INT64;
typedef long                        INT32;
typedef short                       INT16;
typedef unsigned char               UINT8;
typedef char                        INT8;
#if defined(_M_IX86)
#pragma warning(disable:4142)
typedef unsigned long long                     UINT64;
typedef long long                     INT64;
#else
//typedef unsigned long long          UINT64;
//typedef long long                   INT64;
#endif
typedef unsigned char               BOOL;
typedef short                       WCHAR;
typedef char                        CHAR;
typedef unsigned char               BYTE;
typedef float                       FLOAT;
typedef double                      DOUBLE;
typedef UINT32                      HANDLE;

//#ifndef FILE
//#define FILE HANDLE
//#endif

typedef UINT8*                      PUINT8;
typedef UINT32*                     PUINT32;
typedef INT32*                      PINT32;
typedef UINT16*                     PUINT16;
typedef INT16*                      PINT16;
typedef CHAR *                      PCHAR;
typedef void*                       PVOID;
typedef unsigned short              WORD;
typedef unsigned long               DWORD;  /* actually long, but this should be okay */

//#if	COMPILER == 1   //gcc
typedef volatile unsigned char      REG8;
typedef volatile unsigned short     REG16;
typedef volatile unsigned int       REG32;
//#endif

typedef int                         INT;
typedef unsigned int                UINT;
typedef char                        S8;
typedef char *                      PS8;
typedef unsigned char               U8;
typedef unsigned char   *           PU8;
typedef unsigned short              pBOOL; 
typedef short int                   S16; 
typedef short int *                 PS16; 
typedef unsigned short int          U16; 
typedef unsigned short int *        PU16;
typedef int                         S32;
typedef int *                       PS32;
typedef unsigned int                U32;
typedef unsigned int *              PU32;
typedef float                       float32;
typedef unsigned long long          U64;
typedef long long                   S64;

typedef uint32_t	__u32;
typedef uint16_t	__u16;
typedef unsigned char	__u8;
typedef int		__kernel_pid_t;
typedef __kernel_pid_t		pid_t;

typedef unsigned int	__kernel_ino_t;
typedef unsigned int	__kernel_mode_t;
typedef unsigned int	__kernel_nlink_t;
typedef long		__kernel_off_t;
typedef long long	__kernel_loff_t;
typedef int		__kernel_ipc_pid_t;
typedef unsigned int	__kernel_uid_t;
typedef unsigned int	__kernel_gid_t;
typedef unsigned long	__kernel_size_t;
typedef long		__kernel_ssize_t;
typedef long		__kernel_ptrdiff_t;
typedef long		__kernel_time_t;
typedef long		__kernel_clock_t;
typedef int		__kernel_daddr_t;
typedef char *		__kernel_caddr_t;
typedef unsigned long	__kernel_sigset_t;	/* at least 32 bits */
typedef unsigned short	__kernel_uid16_t;
typedef unsigned short	__kernel_gid16_t;
typedef int		__kernel_clockid_t;
typedef int		__kernel_timer_t;

typedef __u32 __kernel_dev_t;
typedef long		__kernel_suseconds_t;

typedef __kernel_dev_t		dev_t;
typedef __kernel_ino_t		ino_t;
typedef __kernel_mode_t		mode_t;
typedef __kernel_nlink_t	nlink_t;
typedef __kernel_off_t		off_t;
typedef __kernel_daddr_t	daddr_t;
typedef __kernel_suseconds_t	suseconds_t;
typedef __kernel_timer_t	timer_t;
typedef __kernel_clockid_t	clockid_t;

typedef __kernel_uid_t __kernel_old_uid_t;
typedef __kernel_gid_t __kernel_old_gid_t;
typedef __kernel_uid_t __kernel_uid32_t;
typedef __kernel_gid_t __kernel_gid32_t;

typedef unsigned int	__kernel_old_dev_t;

typedef __kernel_uid32_t	uid_t;
typedef __kernel_gid32_t	gid_t;
typedef __kernel_uid16_t        uid16_t;
typedef __kernel_gid16_t        gid16_t;
typedef unsigned long		uintptr_t;
typedef unsigned int umode_t;
typedef __kernel_loff_t		loff_t;

#ifndef _TIME_T
#define _TIME_T
typedef __kernel_time_t		time_t;
#endif

#ifdef __CHECK_ENDIAN__
#define __bitwise __bitwise__
#else
#define __bitwise
#endif

typedef __u32 __bitwise __wsum;

#define __KERNEL__

#define PUBLIC
#define SRVAPI
#define PROTECTED
#define LOCAL                       static
#define PRIVATE                     static
#define EXPORT                      extern
#define REG                         register
#define CONST                       const
#define VOLATILE                    volatile
#define VOID                        void

#ifndef __INLINE__
#if	COMPILER == 2   //armcc
#define __INLINE__    __inline
#else
#define __INLINE__    inline
#endif
#endif

#if	COMPILER == 2   //armcc
#define INLINE                      static __inline // Do compiler directives
#else
#define INLINE                      static inline // Do compiler directives
#endif

#ifdef ENABLE_DEPRECATED
#define DEPRECATED __attribute__ ((deprecated))
#else
#define DEPRECATED
#endif /* ENABLE_DEPRECATED */

#define TRUE                        (1==1)
#define FALSE                       (1==0)
#define NULL                        0
#define CS_NULL                     (void*)0
#define NIL                         0
#define CS_NIL                      0
#define HNULL                       0
#define NULL_CHAR                   '\0'

#define OFFSETOF(s,m)                   ((UINT32)&(((s *)0)->m)) // Get the address offset of the specified member.
#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#ifndef ARRAY_SIZE 
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))
#endif
#define LAST_ELEMENT(x)                 (&x[ARRAY_SIZE(x)-1])
#define TCHAR_SIZEOF(sz)                (sizeof(sz)/sizeof(TCHAR))
#define BOUND(x, min, max)              ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )
#define ROUND_SIZEOF(t)                 ((sizeof(t)+sizeof(int)-1)&~(sizeof(int)-1))
#define IS_32BIT_ALIGN(nAdd)            ((UINT32)(((UINT32)(nAdd)) & 0x3)==0)
#define IS_16_ALIGN(nSize)              ((UINT32)(((UINT32)(nSize))& 0x0f) == 0)
#define S_LAST_MEMBER_ADDR(ptr, type)   ((UINT8*)(ptr) + SIZEOF(type))
#define S_MEMBER_ADDR(s_prt, offset)    ((UINT8*)(s_prt) + offset)
#define MSB(x)                          (((x) >> 8) & 0xff) // most signif byte of 2-byte integer
#define LSB(x)                          ((x) & 0xff)        // least signif byte of 2-byte integer
#define MAKE_WORD(p)                    (((UINT16)(p)[0] << 8) | (UINT16)(p)[1])
#define MAKE_DWORD(p)                   (((UINT32)(p)[0] << 24) | ((UINT32)(p)[1] << 16) | ((UINT32)(p)[2] << 8) | (UINT32)(p)[3])
#define SWAP16(p)                       (((UINT16)((UINT16*)(p))[1] << 8) | (UINT16)((UINT16*)(p))[0])
#define SWAP32(p)                       (((UINT32)((UINT32*)(p))[3] << 24) | ((UINT32)((UINT32*)(p))[2] << 16) | ((UINT32)((UINT32*)(p))[1] << 8) | (UINT32)((UINT32*)(p))[0])
#define SWAPT(v1,v2,typ)                {typ v; v  = v1; v1 = v2; v2 = v;}

/* align size withe the specified bits.*/
#define ALIGN_SIZE(x, align)            (((UINT32)(x)+align-1)&~(align-1))
#define Arg(arg, type)                  (*(type *)(arg))
#define NextArg(arg, type)              ((arg) = (TCHAR *)(arg) + roundedsizeof(type))
// Get the data from "arg" to "in".
#define ARG_IN(r,arg,type)              (memcpy(&(r), (type *) (arg), sizeof (arg)))
// Put the data from "w" to "arg".
#define ARG_OUT(arg, w, type)           (memcpy((type *) (arg), &(w), sizeof (w)))
#define ABS(x) ((x<0)?(-(x)):(x))
#define MAX(a,b)                        (((a) > (b)) ? (a) : (b))
#define MIN(a,b)                        (((a) < (b)) ? (a) : (b))
#define MAKEINT16(a, b)                 ((INT16)(((UINT8)(a)) | ((INT16)((UINT8)(b))) << 8))
#define MAKEINT32(a, b)                 ((INT32)(((UINT16)(a)) | ((INT32)((UINT16)(b))) << 16))
#define MAKEINT64(a, b)                ((INT64)(((UINT32)(a)) | ((INT64)((UINT32)(b))) << 32))
#define MAKEUINT16(a, b)                ((UINT16)(((UINT8)(a)) | ((UINT16)((UINT8)(b))) << 8))
#define MAKEUINT32(a, b)                ((UINT32)(((UINT16)(a)) | ((UINT32)((UINT16)(b))) << 16))
#define MAKEUINT64(a, b)                ((UINT64)(((UINT32)(a)) | ((UINT64)((UINT32)(b))) << 32))
#define LOINT8(w)                       ((INT8)(w))
#define HIINT8(w)                       ((INT8)((UINT16)(w) >> 8))
#define LOUINT8(w)                      ((UINT8)(w))
#define HIUINT8(w)                      ((UINT8)((UINT16)(w) >> 8))
#define LOINT16(l)                      ((INT16)(l))
#define HIINT16(l)                      ((INT16)((UINT32)(l) >> 16))
#define LOUINT16(l)                     ((UINT16)(l))
#define HIUINT16(l)                     ((UINT16)((UINT32)(l) >> 16))
#if COMPILER ==1
#define offsetof(TYPE, MEMBER)          ((u32) &((TYPE *)0)->MEMBER)
#endif
#define offsetofvar(VAR, MEMBER)        (((u32) &(VAR.MEMBER)) - ((u32) &VAR))
/* Remove const cast-away warnings from gcc -Wcast-qual */
#define __UNCONST(a)                    ((void *)(unsigned long)(const void *)(a))
#define __P(protos)  protos
#define ALIGN(val,exp)                  (((val) + ((exp)-1)) & ~((exp)-1))
#ifndef YOC_SUPPORT
#define ARRAY_SIZE(a)                   (sizeof(a)/sizeof(a[0]))
#endif
#define LAST_ELEMENT(x)                 (&x[ARRAY_SIZE(x)-1])
#define TCHAR_SIZEOF(sz)                (sizeof(sz)/sizeof(TCHAR))
#define BOUND(x, min, max)              ( (x) < (min) ? (min) : ((x) > (max) ? (max):(x)) )
#define roundedsizeof(t)                ((sizeof(t)+sizeof(int)-1)&~(sizeof(int)-1))
#define IS_32BIT_ALIGN(nAdd)            ((UINT32)(((UINT32)(nAdd)) & 0x3)==0)
#define IS_16_ALIGN(nSize)              ((UINT32)(((UINT32)(nSize))& 0x0f) == 0)
#define ABS(x)                          ((x<0)?(-(x)):(x))

// Set a breakpoint, to enter GDB.
#define BREAKPOINT                      asm volatile("break 1\n\tnop\n\tnop\n\tnop")
#define BKPT                            BREAKPOINT;
#define GDB_FUNC_IN_RAM                 __attribute__((section(".ram")))

// Preprocessor macros.
#define STRINGIFY_VALUE(s) STRINGIFY(s)
#define STRINGIFY(s) #s


#if defined(UNICODE)
#undef SIZEOF
#define SIZEOF(type) (sizeof(type)/sizeof(UINT16))
typedef UINT16* PSTR;
typedef CONST UINT16* PCSTR;
#error: it should not define  this type for TCHAR 
typedef UINT16 TCHAR;
#undef TEXT
#define TEXT(x) L ## x
#else
#undef SIZEOF
#define SIZEOF(type) sizeof(type)

#if COMPILER == 2
typedef char* PSTR;
typedef CONST char* PCSTR;
#else
typedef UINT8* PSTR;
typedef CONST UINT8* PCSTR;
#endif

typedef UINT8 TCHAR;
typedef UINT8* PTCHAR;
#undef TEXT
#define TEXT(x) x
#endif

#define CPP_START  extern "C" {
#define CPP_END    }
#define TSTXT(x)  x

/// From http://www.ibm.com/developerworks/linux/library/l-gcc-hacks/
/// Macro to use in a if statement to tell the compiler this branch
/// is likely taken, and optimize accordingly.
#define LIKELY(x)   __builtin_expect(!!(x), 1)
/// Macro to use in a if statement to tell the compiler this branch
/// is unlikely take, and optimize accordingly.
#define UNLIKELY(x) __builtin_expect(!!(x), 0)


/// For packing structure
#define PACKED      __attribute__((packed))

/// To describe alignment
#define ALIGNED(a)  __attribute__((aligned(a)))

#ifndef __MICRON_ALIGN__
#define MICRON_ALIGN(n) __attribute__ ((aligned (n)))
#endif

/// For possibly unused functions or variables (e.g., debugging stuff)
#define POSSIBLY_UNUSED  __attribute__((unused))


/// C preprocessor conditional check
/// --------------------------------
#define GCC_VERSION (__GNUC__ * 10000 \
                       + __GNUC_MINOR__ * 100 \
                       + __GNUC_PATCHLEVEL__)

#if defined(__GNUC__) && (GCC_VERSION >= 40600)

// GCC 4.6 or later
#define STATIC_ASSERT(e,m) _Static_assert(e,m)

#else // Not GCC or GCC pre 4.6 releases

/// FROM: http://www.pixelbeat.org/programming/gcc/static_assert.html
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
/* These can't be used after statements in c89. */
#ifdef __COUNTER__
#define STATIC_ASSERT(e,m) \
    enum { ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(!!(e)) };
#else
/* This can't be used twice on the same line so ensure if using in headers
 * that the headers are not included twice (by wrapping in #ifndef...#endif)
 * Note it doesn't cause an issue when used on same line of separate modules
 * compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e,m) \
    enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) };
#endif

#endif // Not GCC or GCC pre 4.6 releases
/// --------------------------------
// C++ needs to know that types and declarations are C, not C++.
#ifdef __cplusplus
# define EXTERN_C_START extern "C" {
# define EXTERN_C_END }
#else
# define EXTERN_C_START
# define EXTERN_C_END
#endif

#define RDA_READ_REG(REG)      				(*(volatile UINT32*)(REG))
#define RDA_WRITE_REG(REG,VAL)    			(*(volatile UINT32*)(REG)) = (UINT32)(VAL) 
 #define RDA_BIT(num)  						(1u << num)
 
#define BOOT_EVENT 0xff
#define SYS_EVENT 0x01
#define APP_EVENT 0x02
#define USB_EVENT 0x04
#define BT_EVENT 0x08

#if COMPILER == 2	// armcc
#define MIN_ARRAY_SIZE 	1
#else
#define MIN_ARRAY_SIZE 	0
#endif

/*****************************************************************************/
/* Constant Definitions                                                      */
/*****************************************************************************/
#ifndef BIT
#define BIT(x) (1 << (x))

#define BIT0                            (1ul << 0)
#define BIT1                            (1ul << 1)
#define BIT2                            (1ul << 2)
#define BIT3                            (1ul << 3)
#define BIT4                            (1ul << 4)
#define BIT5                            (1ul << 5)
#define BIT6                            (1ul << 6)
#define BIT7                            (1ul << 7)
#define BIT8                            (1ul << 8)
#define BIT9                            (1ul << 9)
#define BIT10                           (1ul << 10)
#define BIT11                           (1ul << 11)
#define BIT12                           (1ul << 12)
#define BIT13                           (1ul << 13)
#define BIT14                           (1ul << 14)
#define BIT15                           (1ul << 15)
#define BIT16                           (1ul << 16)
#define BIT17                           (1ul << 17)
#define BIT18                           (1ul << 18)
#define BIT19                           (1ul << 19)
#define BIT20                           (1ul << 20)
#define BIT21                           (1ul << 21)
#define BIT22                           (1ul << 22)
#define BIT23                           (1ul << 23)
#define BIT24                           (1ul << 24)
#define BIT25                           (1ul << 25)
#define BIT26                           (1ul << 26)
#define BIT27                           (1ul << 27)
#define BIT28                           (1ul << 28)
#define BIT29                           (1ul << 29)
#define BIT30                           (1ul << 30)
#define BIT31                           (1ul << 31)
#endif



// for armcc and armlink using. While code devide into ROM and flash, the '--symdefs' and 'weak definition' should be used, add by xzc
#if defined(CFG_RULE_FLASH) && !defined(__GNUC__)
#define __ROM_CODE__ __weak
#define __ROM_CODE_VAL__ __weak
#define __ROM_STATIC__  static
#elif (defined(CFG_RULE_ALL) || defined(CFG_RULE_AIO) )
#define __ROM_CODE__ __attribute__((section(".FAST_ACCESS_SECTION")))
#define __ROM_CODE_VAL__
#define __ROM_STATIC__  static
#define __ROM_VAL_STATIC__   static
#elif defined(__GNUC__) && defined(CFG_RULE_ROM)
// for riscv rom , by xzc
#define __ROM_CODE__ __attribute__((section(".FAST_ACCESS_SECTION")))
#define __ROM_CODE_VAL__
#define __ROM_STATIC__  
#define __ROM_VAL_STATIC__  
#elif defined(__GNUC__) && defined(CFG_RULE_FLASH)
// for riscv rom , by xzc
#define __ROM_CODE__ __attribute__((weak)) 
#define __ROM_CODE_VAL__ __attribute__((weak)) 
#define __ROM_STATIC__  
#define __ROM_VAL_STATIC__  
#else
#define __ROM_CODE__
#define __ROM_CODE_VAL__
#define __ROM_STATIC__  static
#define __ROM_VAL_STATIC__   static
#endif



#include "chip_id.h"

#endif // __CS_TYPE_H__

