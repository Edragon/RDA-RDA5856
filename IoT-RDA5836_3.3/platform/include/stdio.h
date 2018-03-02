/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#ifndef STDIO_H
#define	STDIO_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stddef.h"
// Don't touch the next line. This is to use the compiler
// dependent header
#include <stdarg.h>

/* Standard sprintf() function. Work as the libc one. */
int sprintf(char * buf, const char *fmt, ...);
/* Standard snprintf() function from BSD, more secure... */
int snprintf(char * buf, size_t len, const char *fmt, ...);
/* Standard sscanf() function. Work as the libc one. */
int sscanf(const char * buf, const char * fmt, ...);
/* If you need to code your own printf... */
int vsprintf(char *buf, const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
int vsscanf (const char *fp, const char *fmt0, va_list ap);
int printf(const char *fmt, ...);
int vprintf(const char* fmt, va_list ap);


#ifdef __cplusplus
}
#endif

#endif /* STDIO_H */



