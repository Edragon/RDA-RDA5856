/******************************************************************\
 * This code is derived from NetBSD code, for which the following *
 * copyright notice applies.                                      *
 ******************************************************************/

#include "string.h"

/*
 * Copyright (c) 1994 Christian E. Hopps
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Christian E. Hopps.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


UINT32 tcslen(PCWSTR str)
{
    PCWSTR s;
    
    for (s = str; *s; ++s);
    return(s - str);
}


WCHAR* tcscpy(PWSTR to, PCWSTR from)
{
	WCHAR* save = to;

	for (; (*to = *from) != '\0'; ++from, ++to);
	return(save);
}


WCHAR* tcsncpy(PWSTR dst, PCWSTR src, UINT32 n)
{
	if (n != 0)
	{
		WCHAR* d = dst;
		PCWSTR s = src;

		do
		{
			if ((*d++ = *s++) == 0)
			{
				/* NUL pad the remaining n-1 bytes */
				while (--n != 0)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return (dst);
}


WCHAR* tcscat(PWSTR s, PCWSTR append)
{
	WCHAR* t = s;

	for (; *t; ++t)
		;
	while ((*t++ = *append++) != '\0')
		;
	return (s);
}


WCHAR* tcsncat(PWSTR dst, PCWSTR src, UINT32 len)
{
	WCHAR* d = dst;
	PCWSTR s = src;
	UINT32 n = len;

	while (*d != '\0')
		d++;

	if (n == 0)
		return dst;
	while (*s != '\0')
	{
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return dst;
}


INT32 tcscmp(PCWSTR s1, PCWSTR s2)
{
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*s1 - *--s2);
}


INT32 tcsncmp(PCWSTR s1, PCWSTR s2, UINT32 n)
{

	if (n == 0)
		return (0);
	do
	{
		if (*s1 != *s2++)
			return (*s1 - *--s2);
		if (*s1++ == 0)
			break;
	} while (--n != 0);
	return (0);
}

#if 0
int tcsicmp(PCWSTR s1, PCWSTR s2, UINT32 n)
{
	if (n == 0)
		return 0;

	do {
		WCHAR c1 = *s1++;
        WCHAR c2 = *s2++;

		if (c1 != c2) {
			if (c1 >= 'A' && c1 <= 'Z' && 
			    c2 >= 'a' && c2 <= 'z')
				c1 += 'a' - 'A';
			else if (c1 >= 'a' && c1 <= 'z' && 
				 c2 >= 'A' && c2 <= 'Z')
				c2 += 'a' - 'A';
			if (c1 != c2)
				return c1 - c2;
		}
		if (c1 == 0)
			break;
	} while (--n != 0);

	return 0;
}
#endif


WCHAR* tcschr(PCWSTR p, WCHAR ch)
{
    for (;; ++p)
    {
        if (*p == (char) ch)
        {
            return p;
        }
        if (!*p)
        {
            return NULL;
        }
        
    }
    /* NOTREACHED */
}


WCHAR* tcsrchr(PCWSTR p, WCHAR ch)
{	
    WCHAR* save;
    
    for (save = NULL;; ++p)
    {
        if (*p == ch)
        {
            save = p;
        }
        if (!*p)
        {
            return(save);
        }
    }
    /* NOTREACHED */
}


WCHAR* tcsupr(PWSTR p)
{
    int n;

    for (n = 0; p[n] != 0; n++)
    {
        if (p[n] >= 'a' && p[n] <= 'z')
        {
            p[n] -= 32;
        }
    }
    return p;
}



