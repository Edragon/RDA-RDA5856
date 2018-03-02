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
/* FILE NAME                                                                  */
/*      sul.h                                                                 */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   System Utility service declarations, constant definitions and macros     */
/*                                                                            */
/******************************************************************************/

#define UNICODE
#define _UNICODE

#include <math.h>

#include <tchar.h>
#include <windows.h>

#include "sul.h"

unsigned char SRVAPI _SUL_tolower(unsigned char c);
unsigned char SRVAPI _SUL_toupper(unsigned char c);

/******************************************************************************/
/*                           [end] Ctype Manipulation                         */
/******************************************************************************/



/******************************************************************************/
/*                           [begin] String Manipulation                      */
/******************************************************************************/
UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
    return 0;
}

PTCHAR  SRVAPI SUL_StrCat(
                  TCHAR* strDestination, //string which append one string to.  
                  CONST TCHAR* strSource //string which append the string from. 
                  )
{
  return _tcscat(strDestination,strSource);
}
PTCHAR  SRVAPI SUL_StrNCat(
                  TCHAR *dest,      //Null-terminated destination string 
                  CONST TCHAR *src, //Null-terminated source string 
                  UINT32 count      //Number of characters to append 
                  )
{
  return _tcsncat(dest,src,count);
}
PTCHAR  SRVAPI SUL_StrChr(
                  CONST TCHAR *string,	// The string to be searched
                  TCHAR  c	            // The character to search for
                  );
INT32  SRVAPI SUL_StrCompare(
                     CONST TCHAR* stringa,
                     CONST TCHAR* stringb
                     )
{
  return _tcscmp(stringa,stringb);
}
INT32  SRVAPI SUL_StrCompareAscii(
                          CONST TCHAR* stringa,
                          CONST TCHAR* stringb
                          );
INT32  SRVAPI SUL_StrCaselessCompare(
                             CONST TCHAR* stringa,
                             CONST TCHAR* stringb
                             )
{
  return _tcsicmp(stringa,stringb);
}

PTCHAR  SRVAPI  SUL_StrCopy(
                   TCHAR* strDestination,	//the string which copy one string to. 
                   CONST TCHAR* strSource    //the string which copy the string from. 
                   )
{
  return _tcscpy(strDestination,strSource);
}

PTCHAR  SRVAPI  SUL_StrNCopy(
                    TCHAR* dest,
                    CONST TCHAR* src,
                    INT32 n
                    )
{
  return _tcsncpy(dest,src,n);
}

INT32   SRVAPI  SUL_StrIsAscii(
                     CONST TCHAR* stringa	//The string is null terminated to be tested
                     );
UINT32  SRVAPI  SUL_Strlen(
                  CONST TCHAR * string		//The string to be sized
                  )
{
  return _tcslen(string);
}

INT16  SRVAPI   SUL_StrNCompare (
                       CONST TCHAR* stringa, //One string
                       CONST TCHAR* stringb, //Another string
                       INT32 count //Number of characters to compare 
                       )
{
  return _tcsncmp(stringa,stringb,count);
}

INT16  SRVAPI   SUL_StrNCaselessCompare (
                               CONST TCHAR* stringa, //One string
                               CONST TCHAR* stringb, //Another string
                               UINT32  count //Number of characters to compare 
                               )
{
  return _tcsncicmp(stringa,stringb,count);
}

PTCHAR  SRVAPI  SUL_Strtok (
                    TCHAR* string,         // String containing token
                    CONST TCHAR* control,  // Another string
                    TCHAR** newStr        // The new string
                    );
BOOL SRVAPI SUL_StrTrim (
                  TCHAR* pszSource,           // Pointer to the null-terminated string to be trimmed.
                  CONST TCHAR* pszTrimChars  // Pointer to a null-terminated string containing the characters 
                  );
PTCHAR SRVAPI SUL_StrTrimEx(
                     TCHAR* pszSource,          // Pointer to the null-terminated string to be trimmed.
                     CONST TCHAR* pszTrimChars  // Pointer to a null-terminated string containing the characters 
                     );
PTCHAR SRVAPI SUL_StrTrimLeft (
                        TCHAR* pString	//Pointer to the null-terminated string to be trimmed.
                        );
BOOL SRVAPI SUL_StrTrimLeftChar(
                         TCHAR* pszSource,          //Pointer to the null-terminated string to be trimmed.
                         CONST TCHAR* pszTrimChars //Pointer to a null-terminated string containing the characters 
                         );
BOOL SRVAPI SUL_StrTrimRight(
                      TCHAR* pString	//Pointer to the null-terminated string to be trimmed.
                      );
void  SRVAPI     SUL_StrTrimRightChar(
                          TCHAR* pszSource,  
                          const TCHAR* pszTrimChars
                          );
BOOL  SRVAPI     SUL_StrTrimSpace(
                      TCHAR *pString	//Pointer to the null-terminated string to be trimmed.
                      );

PTCHAR SRVAPI    SUL_Strrchr (
                     const TCHAR * string,
                     INT32 ch
                     );
UINT32 SRVAPI  SUL_Strnlen(
                   const TCHAR * s, 
                   UINT32 count
                   );
PSTR  SRVAPI     SUL_CharLower(
                   PSTR string
                   );
PVOID SRVAPI SUL_MemChr(
                 CONST VOID* pSource,	//Pointer to the start of the area.
                 INT32   c,		//The byte to fill the area with
                 UINT32  count	//The size of the area.
                 );

/******************************************************************************/
/* Function name:    SUL_MemCompare        					                  */
/* Description:                                                               */
/*                   Compare two areas of memory			                  */
/* Parameters:		                                                          */
/*                  cs  One area of memory									  */
/*                  ct  Another area of memory								  */
/*                  count  The size of the area.							  */
/* Return Values:                                                             */
/*                  The return values is the the lexicographic relation of	  */
/*    						cs to ct.								          */
/*      Value		Relationship of string1 to ct						      */
/*		<0				cs less than ct					   					  */
/*		=0				cs identical to ct									  */
/*		>0				cs greater than	ct									  */
/*																			  */
/******************************************************************************/
INT16 SUL_MemCompare(
                     CONST VOID* buf1, // One area of memory
                     CONST VOID* buf2, // Another area of memory
                     UINT16 count      // The size of the area.
                     )
{
    const TCHAR *su1, *su2;
    int res = 0;
    
    for( su1 = buf1, su2 = buf2; 0 < count; ++su1, ++su2, count--)
        if ((res = *su1 - *su2) != 0)
            break;
        return (INT16)res;
}


BOOL SRVAPI SUL_MemCopy32(
                   VOID* dest,	   //Where to copy to
                   CONST VOID* src, //Where to copy from
                   UINT32 count	   //The size of the area.
                   );
//jiashuo add start
PVOID SRVAPI SUL_MemCopy8 (
                    VOID*  dest,	    //Where to copy to 
                    CONST VOID* src,  //Where to copy from
                    UINT32 count	    //The size of the area.
                    ){
	return memcpy(dest, src, count);
}
//jiashuo add end
BOOL SRVAPI SUL_MemCopyEx (
                    VOID* dest,	   //Where to copy to
                    CONST VOID* src, //Where to copy from
                    UINT16 count,	   //The size of the area.
                    UINT16 nOption   //The size of the area.
                    );
BOOL SRVAPI SUL_MemDMACopy (
                     VOID* dest,	   //Where to copy to
                     CONST VOID* src, //Where to copy from
                     UINT16 count,	   //The size of the area.
                     UINT16 nOption   
                     );
PVOID SRVAPI SUL_MemMove(
                  void* dest,	        //Where to copy to
                  CONST void 	*src,	//Where to copy from
                  unsigned int 	count	//The size of the area.
                  );
BOOL SRVAPI SUL_MemSet32 (
                   VOID* pSource, //Pointer to the start of the area.
                   INT32 c,       //The bytes to fill the area with
                   UINT16 count   //The size of the area.
                   );

BOOL SRVAPI SUL_MemSet8(
                 void* pSource,  //Pointer to the start of the area.
                 INT32 c,        //The byte to fill the area with
                 UINT16 count    //The size of the area.
                 )
{
  memset(pSource,c,count);
  return TRUE;
}

BOOL SRVAPI SUL_ZeroMemory32(
                      VOID*  pBuf,
                      UINT32 count
                      );
BOOL SRVAPI SUL_ZeroMemory8(
                     VOID*  pBuf,
                     UINT32 count
                     );

/******************************************************************************/
/*                           [END] String Manipulation						  */
/******************************************************************************/


/******************************************************************************/
/*                           [begin] vsprintf Utility                         */
/******************************************************************************/

INT32  SRVAPI    SUL_StrAToI(
                      const TCHAR *nptr
                      )
{
  return _ttoi(nptr);
}

INT32  SRVAPI   SUL_StrPrint(
                       TCHAR*   buffer, 		// Storage location for output 
                       CONST TCHAR* format,		// Format-control string  
                       ...	                        // Optional arguments 
                       ){}
INT32 SRVAPI SUL_StrVPrint(
                    TCHAR*   str,
                    CONST TCHAR* fmtstr,
                    va_list arg
                    );

/******************************************************************************/
/*                           [end] vsprintf Utility                           */
/******************************************************************************/


/******************************************************************************/
/*                           [begin] File System Utiltiy                      */
/******************************************************************************/

INT32  SRVAPI SUL_GetFileName(
                       const TCHAR* pathname, 
                       TCHAR* out_file_name
);
INT32  SRVAPI SUL_Splitpath(
                     TCHAR *fullpath, 
                     TCHAR *dir, 
                     TCHAR *filename
);
INT32  SRVAPI SUL_Makepath(
                    TCHAR *dir, 
                    TCHAR *filename,
                    TCHAR *fullpath
);


/******************************************************************************/
/*                           [END] File System Utiltiy                        */
/******************************************************************************/


/******************************************************************************/
/*                           [begin] Phone Utility		                      */
/******************************************************************************/

BOOL  SRVAPI SUL_IsValidPhoneNumber(
                             TCHAR * PhoneNumber, 
                             UINT8 nSize
);
UINT32	SRVAPI SUL_CompressPhoneString(
                                 PCSTR pSrc,	
                                 UINT8 nSrcSize, 
                                 PSTR pDest, 
                                 UINT8 nDestSize
);
UINT32	SRVAPI SUL_UnCompressPhoneString(
                                   PCSTR pSrc, 
                                   UINT8 nSrcSize, 
                                   PSTR pDest, 
                                   UINT8 nDestSize
);
UINT32	SRVAPI SUL_GetNumberString (
                              PCSTR pString, 
                              UINT8 * pArray, 
                              UINT8 nSize,  
                              UINT8 nCount, 
                              UINT8 nMode
);
UINT32	SRVAPI SUL_GetURLString (
                           PCSTR pString, 
                           UINT8 * pArray, 
                           UINT8 nSize,  
                           UINT8 nCount, 
                           UINT8 nMode
);
UINT32	SRVAPI SUL_GetEmaiString (
                            PCSTR pString, 
                            UINT8 * pArray, 
                            UINT8 nSize,  
                            UINT8 nCount, 
                            UINT8 nMode
);
BOOL SRVAPI  SUL_IsValidWebAddr(
                          PCSTR pString,
                          UINT8 nSize);
BOOL SRVAPI  SUL_IsValidEmailAddr(
                           PCSTR pString,
                           UINT8 nSize
);


/******************************************************************************/
/*                           [END] PHONE Utiltiy                              */
/******************************************************************************/

/******************************************************************************/
/*                           [begin] Image Decode                             */
/******************************************************************************/


UINT32  SRVAPI    SUL_GifDecode(
									  	UINT8 *GifData,
									  	UINT32 nLength,
									  	Gif_DecodeData *pDecodeData
									  );

UINT32  SRVAPI   SUL_JpegDecode(
									  	UINT8 * JpegData,
									  	UINT32 nLength,
									  	Jpeg_DecodeData *pDecodeData
									  );


UINT32 SRVAPI 	SUL_BmpDecode(
									  	UINT8 * BmpData,
									  	UINT32 nLength,
									  	Bmp_DecodeData *pDecodeData
									 );


/******************************************************************************/
/*                           [end] Image Decode                               */
/******************************************************************************/

INT32 SRVAPI SUL_CharacterLen(
  PCSTR   pString,		// The given SMS string.
  UINT16  nSize			//The size of the SMS string
);

INT32 SRVAPI SUL_Encode7Bit(
  PCSTR   pSrc,	       // The given SMS string.
  PSTR    pDest,         // Output encode SMS string.
  UINT16  nSrcSize      // the given SMS sting size in bytes.
)
{
}

INT32 SRVAPI SUL_Decode7Bit(
  PCSTR   pSrc,	            // The given SMS string.
  PSTR    pDest,            // Output Decode SMS string.
  UINT16  nSrcSize          // the given SMS sting size in bytes.
);

INT32 SRVAPI SUL_ASCII2GSM(
  PCSTR  pSrc,	  	    // The given ASCII string.	
  UINT16 nSrcSize,   	// the given string's length.
  PSTR   pDest      	// Output translation string.
);

INT32 SRVAPI SUL_GSM2ASCII(
  PCSTR  pSrc,	  	// The given GSM string.	
  UINT16 nSrcSize,   	// the given string's length.
  PSTR   pDest      	// Output translation string.
);

INT32 SRVAPI SUL_LocallanguageToUCS2(
  PCSTR  pSrc,	  	    // The given local language string.	
  UINT16 nSrcSize,   	// the given string's length.
  PSTR   pDest      	// Output translation string.s
);

INT32 SRVAPI SUL_UCS2ToLocallanguage(
  PCSTR  pSrc,	  	    // The given local language string.	
  UINT16 nSrcSize,   	// the given string's length.
  PSTR   pDest      	// Output translation string.
);

PSTR SRVAPI SUL_CharLower(PSTR string);

#ifndef MMI_ON_WIN32
	PSTR SRVAPI CharLowerBuff(
					   PSTR string,
					   UINT32 Count
					   );
#endif

PSTR SRVAPI SUL_CharUpper(PSTR string);

PSTR SRVAPI SUL_CharUpperBuff(
                       PSTR string,
                       UINT32 Count
                       );

#ifndef MMI_ON_WIN32
BOOL SRVAPI IsCharAlpha(
                 TCHAR ch
                 );

BOOL SRVAPI IsCharAlphaNumeric(
                        TCHAR ch
                        );

BOOL SRVAPI IsCharLower(
                 TCHAR ch 
                 );

BOOL SRVAPI IsCharUpper(
                 TCHAR ch
                 );
#endif

PTCHAR SRVAPI SUL_CharPrev(
                     const TCHAR *lpszStart, 
                     const TCHAR *lpszCurrent 
                     );

PTCHAR SRVAPI SUL_CharNext(
                    const TCHAR * lpsz 
                    );

INT32 SRVAPI SUL_CountChar(
                    const TCHAR * pStrStart 
                    );

BOOL SRVAPI SUL_SeekCodeTable(HRES hdata,
                       INT32 src, 
                       UINT8 srcByte, 
                       UINT16 *pNum
                       );

BOOL SRVAPI SUL_EncodePDU(
                   UINT8 * pSmsContent, 
                   INT8  * phoneNum, 
                   UINT8 * pPackedSubmitMsg
                   );

UINT16 SRVAPI SUL_DecodePdu(
                     CONST INT8 * pSrc, 
                     PSMS_PDU_INFO pDst
                     );

UINT16 SRVAPI SUL_GSM2UCS2(
                    UINT8 * pSrc,
                    UINT8 *pDes,
                    UINT16 nLength);

UINT16 SRVAPI SUL_String2Bytes(
                        CONST UINT8 * pSrc, 
                        UINT8 * pDst, 
                        UINT16 nSrcLength
                        );

UINT16 SRVAPI SUL_SerializeNumbers(
                            INT8 * pSrc,
                            INT8 * pDst, 
                            UINT16 nSrcLength
                            );

VOID SRVAPI SUL_SetLastError(
  UINT32 nErrCode   // error code
);

UINT32 SRVAPI SUL_GetLastError(VOID){}

//
//该函数会去掉最后的F
//0x08, 0x91, 0x68, 0x31 0x18 0x11 0x98 0x38 0xF6 --> '13811189836' 
//
BOOL SRVAPI SUL_GetSCAStruct(
						UINT8 nSCABcd[12], 
						SCA_PARAM *pParam
						);

//
// GSMBCD码,如果转化奇数个数字的话，将会在对应高位补F
// 支持'*''#'''+'p'
// +13811189836 --> 0x0D, 0x31 0x18 0x11 0x98 0x38 0xF6
// 13811189836 --> 0x31 0x18 0x11 0x98 0x38 0xF6
//
UINT16 SRVAPI SUL_AsciiToGsmBcd(
    INT8 * pNumber, // input
    UINT8 nNumberLen,
    UINT8 * pBCD          // output (The size of this buffer should >= nNumberLen/2+1)
){}

//
// 该函数会去掉最后的F，同时将A转化为‘*’将B转化为‘#’将C转化为‘p’
// 将D转化为"+"
// 0x31 0x18 0x11 0x98 0x38 0xF6 --> 13811189836 
//
UINT16 SRVAPI SUL_GsmBcdToAscii (
    UINT8 * pBcd,    // input 
    UINT8 nBcdLen,
    UINT8 * pNumber // output (The size of this buffer should >= nBcdLen*2)
){}


//
//Change the user dial number(UDN) to stack dial number(SDN).
//The space's of SDN is  alloted by the user,and the length of SDN is 2 bytes
//biger than the UDN length at least.
//Also the pUDN and pSDN can be pointed to the same pointer.
//
#if 0 
UINT32 SUL_UserDialNum2StackDialNum(
					CFW_DIALNUMBER *pNumber,
					UINT8 * pSDN,
					UINT8 * pSDNLen
					);
#endif 

// "+,001,138111" ---> "91,001,138111". // internal type
// "+,138111" ---> "91,138111".   // national
// "138111" ---> "81,138111".     // unknown type
// "86138111" ---> "81,86138111". // unknown type



BOOL SRVAPI SUL_ZeroMemory32(VOID* pBuf, UINT32 count)
{
    memset(pBuf,0,count);
	return TRUE;
}

BOOL SRVAPI SUL_ZeroMemory8(VOID* pBuf, UINT32 count)
{
    memset(pBuf,0,count);
	return TRUE;
}

/* zhuoxz,2009-4-29,start */
/* { */
UINT16* SUL_Wcscpy(UINT16* str1, const UINT16* str2)
{
  if (str2)
  {
    return str1;
  }
  return wcscpy(str1,str2);
}

UINT16* SUL_Wcscat(UINT16* dest, const UINT16* src)
{
  return wcscat(dest,src);
}

UINT16* SUL_Wcsncat(UINT16* dest, const UINT16* src, UINT32 count)
{
  if (src == NULL)
  {
    return dest;
  }
  return wcsncat(dest,src,count);
}

UINT8* SUL_Itoa(INT32 value, UINT8 *string, INT32 radix)
{
  return _itoa(value,string,radix);
}

INT32 SUL_Wcscmp(const UINT16* cs, const UINT16* ct)
{
  return wcscmp(cs,ct);
}

UINT32 SUL_FloorEX(double z)
{
  return floor(z);
}

UINT32 SUL_CeilEX(double z)
{
  return ceil(z);
}

UINT16* SUL_Wcschr(const UINT16* str, UINT16 ch)
{
  return wcschr(str,ch);
}

int SUL_Wcsnicmp(const UINT16 *string1, const UINT16 *string2, UINT32 count)
{
  return _wcsnicmp(string1,string2,count);
}

/* 
SUL_StrNCompare

  */
/* } */
/* zhuoxz,2009-4-29,end */


UINT32 tcslen(PCWSTR str)
{
	return _tcslen(str);
}

int tcscmp(PCWSTR s1, PCWSTR s2)
{
  return _tcscmp(s1,s2);
}

int tcsncmp(PCWSTR s1, PCWSTR s2, UINT32 n)
{
  return _tcsncmp(s1,s2,n);
}

WCHAR* tcscpy(PWSTR to, PCWSTR from)
{
  return _tcscpy(to,from);
}

WCHAR* tcsncpy(PWSTR dst, PCWSTR src, UINT32 n)
{
  return _tcsncpy(dst,src,n);
}

WCHAR* tcscat(PWSTR s, PCWSTR append)
{
 return _tcscat(s,append);
}

WCHAR* tcsncat(PWSTR dst, PCWSTR src, UINT32 len)
{
 return _tcsncat(dst,src,len);
}

WCHAR* tcschr(PCWSTR p, WCHAR ch)
{
 return _tcschr(p,ch);
}

WCHAR* tcsrchr(PCWSTR p, WCHAR ch)
{
 return _tcsrchr(p,ch);
}

WCHAR* tcsupr(PWSTR p)
{
 return _tcsupr(p);
}
