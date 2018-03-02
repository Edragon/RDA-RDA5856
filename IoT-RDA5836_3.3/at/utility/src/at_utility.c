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


// REVISION HISTORY:
// NAME              DATE                REMAKS
// Zhanggs           2008-3-22       Created initial version 1.0
//
// //////////////////////////////////////////////////////////////////////////////

#include "at_common.h"
#include "at_parser.h"
#include "sul.h"
#include "string.h"

#define ERR_BAL_INVALID_PARAMETER 0
#if (AUDIO_CALIB_VER == 1)
#define AT_UTLI_BUF_LEN 400
#else
#define AT_UTLI_BUF_LEN 255
#endif
#define AT_UTLI_U8      8
#define AT_UTLI_U16     16
#define AT_UTLI_U32     32
#define AT_UTIL_NR_PARA 16

#ifndef ERR_SUCCESS
#define ERR_SUCCESS 0
#endif

#define AT_ERROR_INFO_LEN  128

#if (AUDIO_CALIB_VER == 1)
typedef struct _PARA_POS
{
    UINT16 begin;
    UINT16 len;
} PARA_POS, *PPARA_POS, **PPPARA_POS;
#else
typedef struct _PARA_POS
{
    UINT8 begin;
    UINT8 len;
} PARA_POS, *PPARA_POS, **PPPARA_POS;
#endif

typedef struct _PARA
{
    UINT32 uParaNum;
    PARA_POS sParaPos[AT_UTIL_NR_PARA];
} PARA, *PPARA, **PPPARA;

#define IS_WHITE_SPACE(ch) ((ch) == 32 || (ch) == 9 || (ch) == 10 || (ch) == 13)

static INT32 save_para(PPARA pResult, UINT32 uOrg, UINT32 uBegin, UINT32 uForward)
{
    if (pResult->uParaNum < AT_UTIL_NR_PARA)
    {
        #if (AUDIO_CALIB_VER == 1)
        pResult->sParaPos[pResult->uParaNum].begin = (uBegin - uOrg);
        pResult->sParaPos[pResult->uParaNum].len   = (uForward - uBegin);
        #else
        pResult->sParaPos[pResult->uParaNum].begin = (UINT8)(uBegin - uOrg);
        pResult->sParaPos[pResult->uParaNum].len   = (UINT8)(uForward - uBegin);
        #endif
        pResult->uParaNum++;
        return ERR_SUCCESS;
    }
    else
    {
        return ERR_AT_UTIL_TOO_MANY_CMD_PARA;
    }
}

INT32 AT_Util_ParaParser(PPARA pResult, UINT8* pPara)
{
    UINT32 cheat   = 1;
    UINT32 iStatus = 0;
    UINT8 c;
    UINT8* begin   = (UINT8*)pPara;
    UINT8* forward = (UINT8*)pPara;
    INT32 lErrCode;

    AT_Util_TrimLeft(pPara);
    if (AT_StrLen(pPara) == 0)
    {
        pResult->uParaNum = 0;
        return ERR_SUCCESS;
    }
    while (1 == cheat)
    {
        switch (iStatus)
        {
            case 0:
                c =* forward++;
                if ('"' == c)
                {
                    iStatus = 2;
                }
                else if ('\0' == c)
                {
                    iStatus = 3;
                }
                else if (',' == c)
                {
                    iStatus = 1;
                }
                else
                {
                    iStatus = 0;
                }
                break;
            case 1:
                lErrCode = save_para(pResult, (UINT32)pPara, (UINT32)begin, (UINT32)(forward - 1));
                if (ERR_SUCCESS != lErrCode)
                {
                    return lErrCode;
                }
                iStatus = 0;
                begin = forward;
                break;
            case 2:
                c = *forward++;
                if ('"' == c)
                {
                    iStatus = 0;
                }
                else if ('\0' == c)
                {
                    iStatus = 3;
                }
                else
                {
                    iStatus = 2;
                }
                break;
            case 3:
                lErrCode = save_para(pResult, (UINT32)pPara, (UINT32)begin, (UINT32)(forward - 1));
                if (ERR_SUCCESS != lErrCode)
                {
                    return lErrCode;
                }
                cheat = 0;
                break;
        }
    }
    return ERR_SUCCESS;
}

// Trim left whitespace characters from the string
// the whitespace include ' ', '\t',<CR>,<LF>
VOID AT_Util_TrimLeft(UINT8* pString)
{
    UINT8* p; // temp point.
    // If string is NULL, do not anything.
    if (NULL == pString)
    {
        return;
    }
    p = pString;
    while ('\0' != *p)
    {
        // If the character is whitespace,move rightward the point of string,
        if ((*p == 32) || (*p == 9) || (*p == 10) || (*p == 13))
        {
            p++;
        }
        else  // is not whitespace,break the loop.
        {
            break;
        }
    }
    if (NULL != p)
    {
        AT_StrCpy(pString, p);  // copy the temp to header.
    }
}

// Trim right whitespace characters from the string
// the whitespace include ' ', '\t',<CR>,<LF>
VOID AT_Util_TrimRight(UINT8* pString)
{
    UINT8* p;
    UINT32 iLen;
    INT32 i;
    if (NULL == pString)
    {
        return;
    }
    p = pString;
    iLen = (UINT8)AT_StrLen(p);
    for (i = (UINT8)(iLen - 1); i >= 0; i--)
    {
        if (IS_WHITE_SPACE(p[i]))
        {
            p[i] = '\0';  // if is whitespace,set it with '\0'.
        }
        else
        {
            break;
        }
    }
}

// Trim left and right whitespace characters from the string
// the whitespace include ' ', '\t',<CR>,<LF>
VOID AT_Util_Trim(UINT8* pString)
{
    AT_Util_TrimLeft(pString);  // Trim the left whitespace.
    AT_Util_TrimRight(pString); // Trim the right whitespace.
}

// Trim all whitespace characters from the string
// the whitespace include ' ', '\t',<CR>,<LF>
VOID AT_Util_TrimAll(UINT8* pString)
{
    UINT8* p;
    UINT32 i, n;
    UINT32 iLen;
    if (NULL == pString)
    {
        return;
    }
    p = pString;
    iLen = (UINT8)AT_StrLen(p);
    n    = 0;
    for (i = 0; i < iLen; i++)
    {
        if ((p[i] == 32) || (p[i] == 9) || (p[i] == 10) || (p[i] == 13))
        {
            continue; // ignore whitespace.
        }
        else
        {
            // if is not whitespace,move to left tight.
            p[n] = p[i];
            n++;
        }
    }
    p[n] = '\0';
}

// Trim all whitespace characters from the string
// the whitespace include ' ', '\t',<CR>,<LF>
/*
A trim function specially for trimming space characters (includes \0x20, \0x08 characters) in parameter string.
**/
#define DOUBLE_QUOTATION_CHAR 0x22  // " character
#define SPACE_CHAR 0x20 // space character
#define TAB_CHAR 0x09 // TAB character
VOID AT_Util_TrimAll_Param(UINT8* szParam)
{
    UINT8* p    = szParam;
    UINT32 i, n = 0;
    size_t len;
    INT32 fDoubleQuotationMaker = FALSE;
    if (szParam == NULL || (len = strlen(szParam)) <= 0)
        return;
    for (i = 0; i < len; i++)
    {
        if (p[i] == DOUBLE_QUOTATION_CHAR)
            fDoubleQuotationMaker = !fDoubleQuotationMaker;
        // If space character not inner double quotaton marker then ignore it.
        if (fDoubleQuotationMaker == FALSE && (p[i] == SPACE_CHAR || p[i] == TAB_CHAR))
            continue;
        else
            p[n++] = p[i];
    }
    p[n] = '\0';
}

// Trim the left specify characters from the string.
VOID AT_Util_TrimLeftChar(UINT8* pString, CONST UINT8 cTrimChar)
{
    UINT8* p;
    if (NULL == pString)
    {
        return;
    }
    p = pString;
    while ('\0' != *p)
    {
        // if the value is specify character,move the point to right.
        if (*p == cTrimChar)
        {
            p++;
        }
        else
        {
            break;
        }
    }
    if (NULL != p)
    {
        AT_StrCpy(pString, p);
    }
}

// Trim backspace characters from the string.
// General the <BS> is 0x08.
VOID AT_Util_TrimBackspace(UINT8* pString, CONST UINT8 iBSChar)
{
    UINT8* p = NULL;
    UINT32 i, n;
    UINT32 iLen;
    if (NULL == pString)
    {
        return;
    }
    iLen = AT_StrLen(pString);
    if (iLen == 0)
    {
        return;
    }
    p = pString;
    n = 0;
    for (i = 0; i < iLen; i++)
    {
        if (pString[i] == iBSChar)  // if is backspace.
        {
            if (n > 0)  // if not arrived the header, move ahead the point.
            {
                n--;
            }
        }
        else
        {
            p[n] = pString[i];
            n++;
        }
    }
    p[n] = '\0';
}

VOID AT_Util_TrimRspStringSuffixCrLf(UINT8* buff, UINT16* plen)
{
    /*
       IF last char(s) is CR and/or LF THEN
       trim it
       END
       * */
    while (*plen > 0 && (buff[*plen - 1] == AT_CMD_CR || buff[*plen - 1] == AT_CMD_LF))
    {
        UINT8 s[21] = { 0 };
        // trace an warning information.
        memcpy(s, buff,* plen > 20 ? 20 :* plen);
        AT_TC(g_sw_UTIL, "Unwanted CR and/or LF suffix character is found in the RSP string!!! rsp=%s", s);
        buff[*plen--] = 0;
    }
}

// This function check the string if is a numeric string.
BOOL AT_Util_IsStringNumeric(CONST UINT8* pString)
{
    UINT32 i, iLen;
    if (NULL == pString)
    {
        return FALSE;
    }
    iLen = AT_StrLen(pString);
    if (0 == iLen)
    {
        return FALSE;
    }
    for (i = 0; i < iLen; i++)
    {
        // Check the character if in '0'-'9'
        if (pString[i] >= '0' && pString[i] <= '9')
        {
            continue;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

// This function convert a numeric string to BCD string.
BOOL AT_Util_Numeric2BCD(CONST UINT8* pNumeric, UINT8 * pBcd, UINT8 * pLen)
{
    UINT8 iInLen; // input string length.
    UINT8 iOutLen;  // output string length.
    UINT32 i;
    UINT8 iLocalLen;  // local used.
    // if pont of paramert is NULL,return false.
    if ((NULL == pNumeric) || (NULL == pBcd) || (NULL == pLen))
    {
        return FALSE;
    }
    AT_MemSet(pBcd, 0x00, (UINT32)(*pLen));
    iInLen = (UINT8)AT_StrLen(pNumeric);
    if (((iInLen) / 2) * 2 == iInLen) // Check the count of input is even number.
    {
        iOutLen = (UINT8)(iInLen / 2);
    }
    else  // is odd number
    {
        iOutLen = (UINT8)(iInLen / 2 + 1);  // the value of iOutLen adding one.
    }
    // if the buffer is less than the result of conversion,return false.
    if ((*pLen) < iOutLen)
    {
        return FALSE;
    }
    *pLen = iOutLen;
    iLocalLen = (UINT8)(iInLen / 2);
    for (i = 0; i < iLocalLen; i++)
    {
        // set the left number to low bit, set the right number to high bit.
        pBcd[i] = (UINT8)((pNumeric[2* i] & 0x0f)|((pNumeric[2* i + 1] & 0x0f)<<4));
    }
    // is odd number.
    if (iLocalLen < iOutLen)
    {
        // the output length adding one,
        // set the numeric to low bit,set high bit with 'F'.
        pBcd[i] = (UINT8)((pNumeric[2* i])|(0xf0));
    }
    return TRUE;
}

// This function convert a BCD string to numeric string.
BOOL AT_Util_BCD2Numeric(CONST UINT8* pBcd, UINT8 * pNumeric, UINT8 * pLen)
{
    BOOL bRet;
    UINT8 iInLen;
    UINT8 iOutLen;
    UINT32 i;
    // if pont of paramert is NULL,return false.
    if ((NULL == pNumeric) || (NULL == pBcd) || (NULL == pLen))
    {
        return FALSE;
    }
    AT_MemSet(pNumeric, 0x00, (UINT32)(*pLen));
    iInLen  = (UINT8)AT_StrLen(pBcd);
    iOutLen = 0;
    for (i = 0; i < iInLen; i++)
    {
        // check the validate for high bit
        if ((pBcd[i] & 0xf0) != 0xf0)
        {
            // get the low bit,
            pNumeric[2 * i] = (UINT8)((((pBcd[i]) >> 4) & 0x0f) | '0');
            iOutLen++;
        }
        // check the validate for low bit
        if ((pBcd[i] & 0x0f) != 0x0f)
        {
            // Get low bit
            if (iOutLen == 2 * i)
            {
                pNumeric[2 * i] = (UINT8)(((pBcd[i]) & 0x0f) | '0');
            }
            // Get high bit
            else
            {
                pNumeric[2 * i + 1] = (UINT8)(((pBcd[i]) & 0x0f) | '0');
            }
            iOutLen++;
        }
    }
    // if the buffer is less than the result of conversion,return false.
    if (*pLen < iOutLen)
    {
        bRet = FALSE;
    }
    else
    {
        *pLen = iOutLen;
        bRet  = TRUE;
    }
    return bRet;
}

// This function calls srx_malloc to allocate storage space for a copy of strSource and then copies strSource to the allocated space.
// REMARKS: must calling the fuction SFW_MemFree() to free the memory.
UINT8* AT_Util_StrDup(CONST UINT8* strSource)
{
    INT32 iLen;
    UINT8* pBuff;
    if (NULL == strSource)
    {
        return NULL;
    }
    iLen = AT_StrLen(strSource);
    pBuff = (UINT8*)AT_MALLOC(iLen + 1);
    if (NULL == pBuff)
    {
        return NULL;
    }
    AT_MemSet(pBuff, 0x00,iLen + 1);
    AT_StrCpy(pBuff, strSource);
    return pBuff;
}

// This function is compare string .
INT32 AT_Util_StrCmp(CONST UINT8* szStr1, CONST UINT8* szStr2)
{
    UINT32 i   = 0;
    BOOL bLoop = TRUE;
    if (NULL == szStr1)
    {
        if (NULL == szStr2)
        {
            return 0;
        }
        else
        {
            return- 1;
        }
    }
    else
    {
        if (NULL == szStr1)
        {
            return 1;
        }
    }
    while (bLoop)
    {
        if (szStr1[i] > szStr2[i])
        {
            return 1;
        }
        else if (szStr1[i] < szStr2[i])
        {
            return- 1;
        }
        else
        {
            if ('\0' == szStr1[i])
                return 0;
        }
        i++;
    }
    return 0;
}

BOOL AT_Util_CheckValidUn(CONST UINT8* pParam, UINT8 nType)
{
    BOOL bResult;
    UINT8 szMax[11];
    UINT32 i, iMaxLen;
    INT32 iLen;
    BOOL bRet;
    UINT8 pBuff[AT_UTLI_BUF_LEN];
    AT_MemSet(pBuff, 0x00, AT_UTLI_BUF_LEN);
    if (NULL == pParam)
    {
        return FALSE;
    }
    iLen = AT_StrLen(pParam);
    if (iLen > AT_UTLI_BUF_LEN)
    {
        return FALSE;
    }
    // Set the max value.
    AT_StrCpy(pBuff, pParam);
    // Trim the left and right whitespace.
    AT_Util_Trim(pBuff);
    // Trim the left '0' from numerical string.
    iLen = AT_StrLen(pBuff);
    for (i = 0; i < iLen; i++)
    {
        if ('0' != pBuff[i])
        {
            break;
        }
    }
    if (i == iLen)  // 数字串中全是0
    {
        AT_MemSet(pBuff, 0x00, AT_UTLI_BUF_LEN);
        pBuff[0] = '0';
    }
    else  // 数字串中有非0字符
    {
        AT_Util_TrimLeftChar(pBuff, '0');
    }
    // Check if is a numeric string.
    bResult = AT_Util_IsStringNumeric(pBuff);
    if (FALSE == bResult)
    {
        return FALSE;
    }
    AT_MemSet(szMax, 0x00, 11);
    switch (nType)
    {
        case AT_UTLI_U8:
            AT_StrCpy(szMax, (UINT8*)"255");
            break;
        case AT_UTLI_U16:
            AT_StrCpy(szMax, (UINT8*)"65535");
            break;
        case AT_UTLI_U32:
            AT_StrCpy(szMax, (UINT8*)"4294967295");
            break;
        default:
            AT_StrCpy(szMax, (UINT8*)"0");
            break;
    }
    // Check the range.
    iLen = (UINT8)AT_StrLen(pBuff);
    iMaxLen = (UINT8)AT_StrLen((UINT8*)szMax);
    if (iLen == iMaxLen)
    {
        bRet = FALSE;
        for (i = 0; i < iMaxLen; i++)
        {
            if (pBuff[i] < szMax[i])
            {
                bRet = TRUE;
                break;
            }
            else if (pBuff[i] == szMax[i])
            {
                bRet = TRUE;
                continue;
            }
            else
            {
                bRet = FALSE;
                break;
            }
        }
    }
    else if (iLen > iMaxLen)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
    }
    return bRet;
}

// This function is check the paramert if is a UINT8 numeric string .
// The range is 0-255
BOOL AT_Util_CheckValidateU8(CONST UINT8* pParam)
{
    return AT_Util_CheckValidUn(pParam, AT_UTLI_U8);
}

// This function is check the paramert if is a UINT16 numeric string .
BOOL AT_Util_CheckValidateU16(CONST UINT8* pParam)
{
    return AT_Util_CheckValidUn(pParam, AT_UTLI_U16);
}

// This function is check the paramert if is a UINT32 numeric string .
BOOL AT_Util_CheckValidateU32(CONST UINT8* pParam)
{
    return AT_Util_CheckValidUn(pParam, AT_UTLI_U32);
}

// This function is check the paramert if is a keys visible string with quotation mark .
BOOL AT_Util_CheckValidateString(CONST UINT8* pParam)
{
    //UINT32 iLen;
    //UINT8 pBuff[AT_UTLI_BUF_LEN] = { 0x00, };
    if (NULL == pParam)
    {
        return FALSE;
    }
    //AT_StrCpy(pBuff, pParam);
    //AT_Util_Trim(pBuff);
    //iLen = (UINT8)AT_StrLen(pBuff);
    //if (iLen < 2)
    //{
    // return FALSE;
    //}
    return TRUE; // Modify for supporting without '"' at string by lixp at 20130726
    //if ('\"' != pBuff[0] || '\"' != pBuff[iLen - 1])
    //{
    //    return FALSE;
    //}
    //else
    //{
    //    return TRUE;
    //}
}

INT32 AT_Util_GetParaCount(UINT8* pPara, UINT8* pCount)
{
    INT32 lErrCode;
    PARA result;
    AT_MemSet(&result, 0x00, sizeof(PARA));
    lErrCode = AT_Util_ParaParser(&result, pPara);
    if (ERR_SUCCESS != lErrCode)
    {
        *pCount = 0;
        return lErrCode;
    }
    *pCount = (UINT8)(result.uParaNum);
    return ERR_SUCCESS;
}

#if (AUDIO_CALIB_VER == 1)
INT32 AT_Util_GetPara(UINT8* pRes, CONST UINT8 iIndex, UINT8* pParam, UINT16* pLen)
#else
INT32 AT_Util_GetPara(UINT8* pRes, CONST UINT8 iIndex, UINT8* pParam, UINT8* pLen)
#endif
{
    PARA result;
    AT_MemSet(&result, 0x00, sizeof(PARA));
    if (ERR_SUCCESS != AT_Util_ParaParser(&result, pRes))
    {
        *pLen = 0;
        return ERR_AT_UTIL_PARA_ERROR;
    }
    if (result.uParaNum == 0)
    {
        *pLen = 0;
        return ERR_AT_UTIL_PARA_ERROR;
    }
    if (*pLen <= result.sParaPos[iIndex].len)
    {
        return ERR_AT_UTIL_PARA_ERROR;
    }
    if (iIndex <= result.uParaNum)
    {
        AT_MemSet(pParam, 0x00, *pLen);
        AT_MemCpy(pParam, pRes + result.sParaPos[iIndex].begin, result.sParaPos[iIndex].len);
    }
    else
    {
        return ERR_AT_UTIL_PARA_ERROR;
    }
    return ERR_SUCCESS;
}

#if (AUDIO_CALIB_VER == 1)
INT32 AT_Util_GetTrimedPara(UINT8* pRes, CONST UINT8 iIndex, UINT8* pPara, UINT16* pLen)
#else
INT32 AT_Util_GetTrimedPara(UINT8* pRes, CONST UINT8 iIndex, UINT8* pPara, UINT8* pLen)
#endif
{
    INT32 lErrCode = AT_Util_GetPara(pRes, iIndex, pPara, pLen);
    if (ERR_SUCCESS != lErrCode)
    {
        return lErrCode;
    }
    else
    {
        AT_Util_Trim(pPara);
        #if (AUDIO_CALIB_VER == 1)
        *pLen = AT_StrLen(pPara);
        #else
        *pLen = (UINT8)AT_StrLen(pPara);
        #endif
        return ERR_SUCCESS;
    }
}

// This function is get the paramert with specify index and rule.
// The index start from zero.
// Mark:
#if (AUDIO_CALIB_VER == 1)
INT32 AT_Util_GetParaWithRule(UINT8* pRes, CONST UINT8 iIndex, CONST AT_UTIL_PARA_TYPE eType, PVOID pPara, UINT16* pLen)
#else
INT32 AT_Util_GetParaWithRule(UINT8* pRes, CONST UINT8 iIndex, CONST AT_UTIL_PARA_TYPE eType, PVOID pPara, UINT8* pLen)
#endif
{
    UINT32 iNumValue32; // the value of integer32.
    UINT16 iNmbValue16; // the value of integer16.
    UINT8 iNmbValue8; // the value of integer8.
    UINT8* pNextParam;  // the next paramert.
#if (AUDIO_CALIB_VER == 1)
    UINT16 iParamLen;  // paramert length.
#else
    UINT8 iParamLen;
#endif
    INT32 lErrCode;
    if ((NULL == pRes) || (NULL == pPara) || NULL == pLen || 0 ==* pLen)
    {
        return ERR_AT_UTIL_PARA_ERROR;
    }
    // Integer type.
    if ((AT_UTIL_PARA_TYPE_UINT8 == eType) || (AT_UTIL_PARA_TYPE_UINT16 == eType) || (AT_UTIL_PARA_TYPE_UINT32 == eType))
    {
#if (AUDIO_CALIB_VER == 1)
        UINT16 iNumLen = AT_UTLI_BUF_LEN;
#else
        UINT8 iNumLen = AT_UTLI_BUF_LEN;
#endif
        UINT8 szNum[AT_UTLI_BUF_LEN];
        AT_MemSet(szNum, 0x00, (UINT32)iNumLen);
        // get the paramert by index.
        lErrCode = AT_Util_GetTrimedPara(pRes, iIndex, szNum, &iNumLen);
        if (ERR_SUCCESS != lErrCode)
        {
            return lErrCode;
        }
        if (0 == AT_StrLen(szNum))
        {
            return ERR_AT_UTIL_CMD_PARA_NULL;
        }
        if (AT_UTIL_PARA_TYPE_UINT8 == eType)
        {
            if (sizeof(UINT8) !=* pLen)
            {
                return ERR_AT_UTIL_PARA_ERROR;
            }
            if (AT_Util_CheckValidateU8(szNum))
            {
                iNmbValue8 = (UINT8)AT_StrAToI(szNum);
                *(UINT8*)pPara = (UINT8)iNmbValue8;
                return ERR_SUCCESS;
            }
        }
        else if (AT_UTIL_PARA_TYPE_UINT16 == eType)
        {
            if (sizeof(UINT16) !=* pLen)
            {
                return ERR_AT_UTIL_PARA_ERROR;
            }
            if (AT_Util_CheckValidateU16(szNum))
            {
                iNmbValue16 = (UINT16)AT_StrAToI(szNum);
                *(UINT16*)pPara = (UINT16)iNmbValue16;
                return ERR_SUCCESS;
            }
        }
        else  // COMM_PARAM_TYPE_UINT32 == eType
        {
            if (sizeof(UINT32) !=* pLen)
            {
                return ERR_AT_UTIL_PARA_ERROR;
            }
            if (AT_Util_CheckValidateU32(szNum))
            {
                iNumValue32 = AT_StrAToI(szNum);
                *(UINT32*)pPara = (UINT32)iNumValue32;
                return ERR_SUCCESS;
            }
        }
        return ERR_AT_UTIL_FAILED;
    }
    else if (AT_UTIL_PARA_TYPE_STRING == eType) // String type
    {
        // Get the paramert with index.
        pNextParam = (UINT8*)pPara;
        iParamLen  = *pLen;
        lErrCode   = AT_Util_GetTrimedPara(pRes, iIndex, pNextParam, &iParamLen);
        if (ERR_SUCCESS != lErrCode)
        {
            return lErrCode;
        }
        #if (AUDIO_CALIB_VER == 1)
        iParamLen = AT_StrLen(pNextParam);
        #else
        iParamLen = (UINT8)AT_StrLen(pNextParam);
        #endif
        if (0 == iParamLen) // if the parameter length is 0,return ISNULL.
        {
            AT_MemSet(pPara, 0x00, *pLen);
            *pLen = 0;
            return ERR_AT_UTIL_CMD_PARA_NULL;
        }
        // Check the string if validate.
        if (AT_Util_CheckValidateString(pNextParam))
        {
            if('"' == pNextParam[iParamLen - 1])
                pNextParam[iParamLen - 1] = '\0'; // Trim the tail '"'
            if('"' == pNextParam[0])
                pNextParam++; // Trim the head '"'
            if ('\0' == *pNextParam)
            {
                AT_MemSet(pPara, 0x00, *pLen);
                *pLen = 0;
                return ERR_SUCCESS;
            }
            else
            {
                AT_StrCpy((UINT8*)pPara, pNextParam);
                #if (AUDIO_CALIB_VER == 1)
                *pLen = AT_StrLen((UINT8*)pPara);
                #else
                *pLen = (UINT8)AT_StrLen((UINT8*)pPara);
                #endif
                return ERR_SUCCESS;
            }
        }
        else  // Invalid string
        {
            AT_MemSet(pPara, 0x00, *pLen);
            *pLen = 0;
            return ERR_AT_UTIL_FAILED;
        }
    }
    else
    {
        AT_TC(g_sw_DFMS, "exe     AT_Util_CheckValidateString  failed\n");
        return ERR_AT_UTIL_PARA_ERROR;
    }
}

// This function is get the response information for DCE with errorcode.
UINT32 AT_GetError_DCE(UINT8 iType, UINT32 iErrCode, UINT8* pErrStr)
{
    UINT8 szCR[] = { 0, 0 };
    UINT8 szLF[] = { 0, 0 };
    AT_ASSERT(iType == 0 || iType == 1);
    szCR[0] = CHAR_CR;
    szLF[0] = CHAR_LF;
#if (0)
    if (iErrCode == CMD_RC_INVCMDLINE)
    {
        AT_GetError_CME(ERR_AT_CME_INVALID_COMMAND_LINE, pErrStr, gATCurrentuCmee);
        return! ERR_SUCCESS;
    }
#endif /* 0 */
    if (iType == 1)
    {
        // //////////////////////////////////////////////////////
        // ATV1 format for RC is <cr><lf><verbose code><cr><lf>.
        strcpy(pErrStr, szCR);
        strcat(pErrStr, szLF);
        switch (iErrCode)
        {
            case CMD_RC_OK:
                strcat(pErrStr, "OK");
                break;
            case CMD_RC_CONNECT:
                strcat(pErrStr, "CONNECT");
                break;
            case CMD_RC_RING:
                strcat(pErrStr, "RING");
                break;
            case CMD_RC_NOCARRIER:
                strcat(pErrStr, "NO CARRIER");
                break;
            case CMD_RC_ERROR:
                strcat(pErrStr, "ERROR");
                break;
            case CMD_RC_NODIALTONE:
                strcat(pErrStr, "NO DIALTONE");
                break;
            case CMD_RC_BUSY:
                strcat(pErrStr, "BUSY");
                break;
            case CMD_RC_NOANSWER:
                strcat(pErrStr, "NO ANSWER");
                break;
            case CMD_RC_NOTSUPPORT:
                strcat(pErrStr, "NOT SUPPORT");
                break;
            case CMD_RC_INVCMDLINE:
                strcat(pErrStr, "INVALID COMMAND LINE");
                break;
            case CMD_RC_CR:
                break;
            case CMD_RC_SIMDROP:
                strcat(pErrStr, "SIM not inserted");
                break;
            default:
                return ERR_AT_UTIL_FAILED;
                break;
        }
        strcat(pErrStr, szCR);
        strcat(pErrStr, szLF);
        return ERR_SUCCESS;
    }
    else if (0 == iType)
    {
        // //////////////////////////////////////////////////////
        // ATV0 format for RC is <numeric code><cr>.
        AT_Sprintf(pErrStr, "%u", iErrCode);
        strcat(pErrStr, szCR);
        return ERR_SUCCESS;
    }
    return! ERR_SUCCESS;
}

// This function is get the response information for CME with errorcode.
INT32 AT_GetError_CME(UINT32 iErrCode, UINT8 *pErrStr, UINT8 iLevel)
{
	return AT_GetError(iErrCode, pErrStr, CMD_ERROR_CODE_TYPE_CME, iLevel);
}

// This function is get the response information for CMS with errorcode.
INT32 AT_GetError_CMS(UINT32 iErrCode, UINT8 *pErrStr, UINT8 iLevel)
{
	return AT_GetError(iErrCode, pErrStr, CMD_ERROR_CODE_TYPE_CMS, iLevel);
}

INT32 AT_GetError(UINT32 iErrCode, UINT8* pErrStr, UINT8 iType, UINT8 iLevel)
{
    //UINT8* pErr    = NULL;
    INT32 lErrCode = ERR_SUCCESS;
    UINT8 szCR[]   = { 0, 0 };
    UINT8 szLF[]   = { 0, 0 };
    szCR[0] = CHAR_CR;
    szLF[0] = CHAR_LF;
    if (0 == iLevel)
    {
        if (uRCFormatMode == 1)
        {
            strcpy(pErrStr, szCR);
            strcat(pErrStr, szLF);
        }
        strcat(pErrStr, "ERROR");
        strcat(pErrStr, szCR);
        strcat(pErrStr, szLF);
    }
    else if (1 == iLevel) // Return error code
    {
        if (uRCFormatMode == 1)
        {
            strcpy(pErrStr, szCR);
            strcat(pErrStr, szLF);
        }
        if (CMD_ERROR_CODE_TYPE_CMS == iType)
            AT_Sprintf(pErrStr + strlen(pErrStr), "+CMS ERROR:%u", iErrCode);
        else
            AT_Sprintf(pErrStr + strlen(pErrStr), "+CME ERROR:%u", iErrCode);
        strcat(pErrStr, szCR);
        strcat(pErrStr, szLF);
    }
/*    else if (2 == iLevel) // Return error string
    {
        if (uRCFormatMode == 1)
        {
            strcpy(pErrStr, szCR);
            strcat(pErrStr, szLF);
        }
        pErr = AT_GetErrorString(iErrCode, iType);
        if (NULL == pErr)
            pErr = "Unknown error";
        if (CMD_ERROR_CODE_TYPE_CMS == iType)
            AT_Sprintf(pErrStr + strlen(pErrStr), "+CMS ERROR:%s", pErr);
        if (CMD_ERROR_CODE_TYPE_CME == iType)
            AT_Sprintf(pErrStr + strlen(pErrStr), (CONST TCHAR*)"\r\n+CME ERROR:%s\r\n", pErr);
        strcat(pErrStr, szCR);
        strcat(pErrStr, szLF);
    }
*/
    else
        lErrCode = ERR_AT_UTIL_FAILED;
    return lErrCode;
}

/***************************************************************************************************
                                CSW SUL LIB PATCH SECTION
****************************************************************************************************/

UINT16 AT_UtilGbkStrCharLen(const UINT8* gbk_string)
{
    UINT16 idx = 0;
    UINT16 len = 0;
    /*
       @RUBY STYLE PSEUDOCODE@
       check parameter
       * */
    AT_ASSERT(gbk_string != NULL);
    /*
       check for NULL character only at the odd no. of bytes assuming forst byte start from zero
       * */
    while (gbk_string[idx] != 0)
    {
        len++;
        /*
           @RUBY STYLE PSEUDOCODE@
           if is an ASCII char then
           idx++
           else
           idx += 2
           end
           * */
        if (gbk_string[idx] <= 127)
            idx++;
        else
            idx += 2;
    }
    return len; /* One is added to count 0th byte */
}

#define AT_UtilGbkStrByteLen(s) (strlen((s)))

UINT16 AT_UtilUcs2StrCharLen(const UINT8* ucs2_string)
{
    UINT16 idx = 0;
    UINT16 len = 0;
    /*
       @RUBY STYLE PSEUDOCODE@
       check parameter
       * */
    AT_ASSERT(ucs2_string != NULL);
    /*
       check for NULL character only at the odd no. of bytes assuming forst byte start from zero
       * */
    while (ucs2_string[idx] != 0 || ucs2_string[idx + 1] != 0)
    {
        len++;
        idx += 2;
    }
    return len; /* One is added to count 0th byte */
}

UINT16 AT_UtilGbk2Ucs2(UINT8* gbk_string, UINT16 gbk_str_bytelen, UINT8* ucs2_string)
{
    /*
       @RUBY STYLE PSEUDOCODE@
       check parameter
       * */
    AT_ASSERT(gbk_string != NULL && gbk_str_bytelen == 0 && ucs2_string != NULL);
    /*
       @RUBY STYLE PSEUDOCODE@
       if convert is okay then
       return the length of the converted (UCS2) string, in bytes
       else
       return -1
       end
       * */
#if 0
    if (SUL_LocallanguageToUCS2(gbk_string, gbk_str_bytelen, ucs2_string) != -1)
        return (UINT16)AT_UtilUcs2StrByteLen(ucs2_string);
    else
        return 0;
#endif
    return 0;
}

UINT16 AT_UtilUcs2ToGbk(UINT8* ucs2_string, UINT16 ucs2_str_bytelen, UINT8* gbk_string)
{
    /*
       @RUBY STYLE PSEUDOCODE@
       check parameter
       * */
    AT_ASSERT(ucs2_string != NULL && ucs2_str_bytelen == 0 && gbk_string != NULL);
    /*
       @RUBY STYLE PSEUDOCODE@
       if convert is okay then
       return the length of the converted (GBK) string, in bytes
       else
       return -1
       end
       * */
#if 0
    if (SUL_UCS2ToLocallanguage(ucs2_string, ucs2_str_bytelen, gbk_string) != -1)
        return (UINT16)AT_UtilGbkStrByteLen(gbk_string);
    else
        return 0;
#endif
    return 0;
}

