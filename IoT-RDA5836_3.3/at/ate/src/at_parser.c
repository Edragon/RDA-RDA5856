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
// Caoxh             2008-05-08         Created initial version 1.0
// 
// //////////////////////////////////////////////////////////////////////////////
#ifdef _MS_VC_VER_

#else

#include "at_common.h"
#  include "at_sa.h"
#  include "at_module.h"

#endif
#include "at_parser.h"
#include"at_define.h"
// The base command name list. --out-of-order.
// This list is created refer "Coolsand at command set V3.0.1"
// Insert the new base command name into this list. 
const AT_CMD_NAME g_pBNameList[] = {
  {(UINT8 *)"a"},
  {(UINT8*)"e"},
  {(UINT8*)"h"},
  {(UINT8*)"i"},
  {(UINT8*)"o"},
  {(UINT8*)"q"},
  {(UINT8*)"s0"},
  {(UINT8*)"s1"},
  {(UINT8*)"s2"},
  {(UINT8*)"s3"},
  {(UINT8*)"s4"},
  {(UINT8*)"s5"},
  {(UINT8*)"v"},
  {(UINT8*)"x"},
  {(UINT8*)"z"},
  {(UINT8*)"&f"},
  {(UINT8*)"&v"},
  {(UINT8*)"&w"},
  {(UINT8*)"\\q"},
};

// The terminal command name list. --out-of-order.
// This list is created refer "Coolsand at command set V3.0.1"
// Insert the new terminal command name into this list. 
const AT_CMD_NAME g_pTNameList[] = {
  {(UINT8 *)"d"},
  {(UINT8*)"h"},
  {(UINT8*)"+dlst"},
};

// AT_IsBaseCmd()
// Function
// This function check the name if is base at command name.
// Parameters
// pName 
// [in] Specify the name of will been finded.
// Return Values
// If finded in the base list,return TRUE ,else return FALSE.
#if 0
int strnicmp(s1, s2, n)
     const char* s1;
     const char* s2;
     size_t n;
{
  if (n == 0)
    return 0;
  do
  {
    unsigned char c1 = (unsigned char)*s1++;
    unsigned char c2 = (unsigned char)*s2++;

    if (c1 != c2)
    {
      if (c1 >= 'A' && c1 <= 'Z' && c2 >= 'a' && c2 <= 'z')
        c1 += 'a' - 'A';
      else if (c1 >= 'a' && c1 <= 'z' && c2 >= 'A' && c2 <= 'Z')
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
BOOL AT_IsBaseCmd(UINT8* pName)
{
  UINT32 uCount;
  UINT32 i;

  uCount = SIZEOF(g_pBNameList) / SIZEOF(AT_CMD_NAME);
  for (i = 0; i < uCount; i++)
  {
    if (AT_StrCaselessCmp(pName, g_pBNameList[i].pName) == 0)
    {
      return TRUE;
    }
  }
  return FALSE;
}

// extern int strnicmp(const char *, const char *, size_t);

BOOL AT_IsTermiCmd(UINT8* pName)
{
  UINT32 uCount;
  UINT32 i;

  uCount = SIZEOF(g_pTNameList) / SIZEOF(AT_CMD_NAME);
  for (i = 0; i < uCount; i++)
  {
    if (AT_StrCaselessCmp(pName, g_pTNameList[i].pName) == 0)
    {
      return TRUE;
    }
  }
  return FALSE;
}

// AT_CmdLineSyntaxParser
// Function
// This function to parse the command line,output the parse result.
// Parameters
// pCmdBuf
// [in] The buffer of command line.
// uCmdLen
// [in] The length of command line string.
// pParseResult
// [out] Point to a parse resut struct,to output the result of parsed.
// Return Values
// If parse success,return ERR_SUCCESS ,else return ERR_FAIL.
// ERR_MELLOC_FAIL
// ERR_SYNTAX_ERR
// ERR_NO
UINT32 AT_CmdLineSyntaxParser(UINT8* pCmdBuf, UINT32 uCmdLen, AT_PARSE_RESULT* pParseResult)
{
  UINT32 uResult;
  UINT32 uRet                        = ERR_SUCCESS;
  UINT8 szName[AT_CMD_NAME_SIZE + 1] = { 0, };
  UINT8 szPara[AT_CMD_PARA_SIZE + 1] = { 0, };
  UINT8 uType                        = AT_CMD_EXE;
  UINT8 uState                       = ATP_STATE_INIT;
  UINT8 uChA                         = 0;
  UINT32 i                           = 0;
  UINT8 uChT                         = 0;
  UINT8 uChP                         = 0;

  UINT8* p = NULL;

  AT_CMD_NODE* pNodeH = NULL;
  BOOL bLoop = TRUE;

  // Input parameter checking.
  if (NULL == pCmdBuf || NULL == pParseResult || 0 == uCmdLen || ATP_CMD_LINE_SIZE < uCmdLen)
  {
    return _ERR_ATP_INVALID_PARA;
  }

  // Process back space. 
  AT_Util_TrimBackspace(pCmdBuf, 0x08);

  // Parsing...
  p = pCmdBuf;
  while (TRUE == bLoop)
  {
    if ((UINT32)((UINT8*)p - pCmdBuf) > uCmdLen)
    {
      uRet = _ERR_ATP_WITHOUT_CR;
      break;
    }

    // AT_TC(g_sw_GC,"*p:  %x[%c];  uState: %d, i = %d",*p,*p, uState, i);  
    switch (uState)
    {
    case ATP_STATE_INIT:
      if (IS_CR(*p))
      {
        uState = ATP_STATE_CR_END;
      }
      else if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_A(*p))
      {
        uChA   = *p;
        uState = ATP_STATE_A;
        p++;
      }

      // add by wulc begin
      else if (AT_IS_PLUS(*p))
      {
        uChA   = *p;
        uState = ATP_STATE_PLUS;
        p++;
      }

      // add by wulc end
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;

      // add by wulc begin
    case ATP_STATE_PLUS:
      if (AT_IS_PLUS(*p))
      {
        uChT = *p;
        if (AT_IS_2PLUS(uChA, uChT))
        {
          p++;
          uState = ATP_STATE_2PLUS;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_2PLUS:
      if (AT_IS_PLUS(*p))
      {
        uChP = *p;
        if (AT_IS_3PLUS(uChA, uChT, uChP))
        {
          p++;
          uState = ATP_STATE_3PLUS;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_3PLUS:
      if (IS_CR(*p))
      {
        if (NULL == pNodeH)
        {
          uType = AT_CMD_EXE;
          AT_StrCpy(szName, "+++");
          szPara[0] = '\0';
          uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
          if (ERR_SUCCESS == uResult)
          {
            uState = ATP_STATE_AT_OK;
          }
          else
          {
            uRet   = _ERR_ATP_NO_MORE_MEMORY;
            uState = ATP_STATE_FAIL_END;
          }
        }
        else
        {
          uState = ATP_STATE_SUCC_END;
        }

      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;

      // add by
      // add by wulc end
    case ATP_STATE_A:
      if (AT_IS_T(*p))
      {
        uChT = *p;
        if (AT_IS_AT(uChA, uChT))
        {
          p++;
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_T:

      // i to talk count of the character of name 
      i = 0;
      if (IS_CR(*p))
      {
        if (NULL == pNodeH)
        {
          uType = AT_CMD_EXE;
          AT_StrCpy(szName, "AT");
          szPara[0] = '\0';
          uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
          if (ERR_SUCCESS == uResult)
          {
            uState = ATP_STATE_AT_OK;
          }
          else
          {
            uRet   = _ERR_ATP_NO_MORE_MEMORY;
            uState = ATP_STATE_FAIL_END;
          }
        }
        else
        {
          uState = ATP_STATE_SUCC_END;
        }

      }
      else if (AT_IsTermiCmd(szName))
      {
        uState = ATP_STATE_SPARE;
      }
      else if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_D(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_D;
      }
      else if (AT_IS_B_PRIFIX(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_BASE;
      }
      else if (AT_IS_E_PRIFIX(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_EXTENED;
      }
      else if (AT_IS_LETTER(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_B_NAME;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_D:

      // Name is terminative.
      szName[i] = '\0';

      // i change to talk count of character of parameter.
      i = 0;

      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_STAR(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_D_PARAM_STAR;
      }
      else if (AT_IS_DIAL(*p) || AT_IS_COMMA(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_D_PARAM;
      }
      else if (IS_CR(*p))
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      else
      {
        p++;
        uState = ATP_STATE_D_PARAM;
      }
      break;
    case ATP_STATE_BASE:
      if (AT_IS_LETTER(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_B_NAME;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_EXTENED:
      if (AT_IS_LETTER(*p))
      {
        szName[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_NAME;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_D_PARAM:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_STAR(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_D_PARAM_STAR;
      }
      else if (AT_IS_HASH(*p))
      {
        if (0 == i)
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
        else
        {
          szPara[i] = *p;
          i++;
          p++;
          szPara[i] = 0;

          uType   = AT_CMD_EXE;
          uResult = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
          if (ERR_SUCCESS == uResult)
          {
            uState = ATP_STATE_SUCC_END;
          }
          else
          {
            uRet   = _ERR_ATP_NO_MORE_MEMORY;
            uState = ATP_STATE_FAIL_END;
          }
          uState = ATP_STATE_T;
        }

      }
      else if (AT_IS_DIAL(*p) || AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }

        szPara[i] = *p;
        i++;
        p++;
      }
      else if (AT_IS_SEMICOLON(*p) || IS_CR(*p))
      {
        // parameter is terminative.
        if (0 == i)
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
        else
        {
          szPara[i] = '\0';
          p++;
          uType   = AT_CMD_EXE;
          uResult = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
          if (ERR_SUCCESS == uResult)
          {
            uState = ATP_STATE_SUCC_END;
          }
          else
          {
            uRet   = _ERR_ATP_NO_MORE_MEMORY;
            uState = ATP_STATE_FAIL_END;
          }
          uState = ATP_STATE_T;
        }
      }
      else if (IS_CR(*p))
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      else
      {
        p++;
      }
      break;
    case ATP_STATE_D_PARAM_STAR:
      if (AT_IS_SEMICOLON(*p))
      {
        // parameter is terminative.
        if (0 == i)
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
        else
        {
          szPara[i] = '\0';
          p++;
          uType   = AT_CMD_EXE;
          uResult = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
          if (ERR_SUCCESS == uResult)
          {
            uState = ATP_STATE_SUCC_END;
          }
          else
          {
            uRet   = _ERR_ATP_NO_MORE_MEMORY;
            uState = ATP_STATE_FAIL_END;
          }
          uState = ATP_STATE_T;
        }
      }
      else if (IS_CR(*p))
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      else if (AT_IS_STAR(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
      }
      else if (AT_IS_DIAL(*p) || AT_IS_COMMA(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_D_PARAM;
      }
      else
      {
        p++;
        uState = ATP_STATE_D_PARAM;
      }
      break;
    case ATP_STATE_SPARE:
      uState = ATP_STATE_SUCC_END;
      break;
    case ATP_STATE_B_NAME:
      szName[i] = 0;
      if (AT_IsBaseCmd(szName))
      {
        uState = ATP_STATE_B_NAME_T;
      }
      else if (AT_IS_NUM(*p) || AT_IS_LETTER(*p))
      {
        if (i >= AT_CMD_NAME_SIZE)
        {
          uRet   = _ERR_ATP_NMAE_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szName[i] = *p;
        i++;
        p++;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_B_NAME_T:

      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_QUESTIONMARK(*p))
      {
        szName[i] = '\0';
        i         = 0;
        p++;
        uState = ATP_STATE_B_READ;
      }
      else if (AT_IS_DIGIT(*p))
      {
        i         = 0;
        szPara[i] = *p;
        i++;
        p++;
        uType  = AT_CMD_EXE;
        uState = ATP_STATE_B_PARAM;
      }
      else if (AT_IS_EQUITSIGN(*p))
      {
        p++;
        uState = ATP_STATE_B_EQUAL;
      }
      else
      {
        szPara[0] = '\0';
        i         = 0;
        uType     = AT_CMD_EXE;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }

        if (AT_IsTermiCmd(szName))
        {
          uState = ATP_STATE_SPARE;
        }
        else if (AT_IS_B_PRIFIX(*p))
        {

          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_BASE;
        }
        else if (AT_IS_D(*p))
        {

          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_D;
        }
        else if (AT_IS_E_PRIFIX(*p))
        {

          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_EXTENED;
        }
        else if (AT_IS_LETTER(*p))
        {

          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_B_NAME;
        }
        else if (AT_IS_SEMICOLON(*p))
        {
          p++;
          uState = ATP_STATE_T;
        }
        else if (IS_CR(*p))
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      break;
    case ATP_STATE_B_PARAM:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_NUM(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
      }
      else
      {
        szPara[i] = '\0';
        i         = 0;

        // uType = AT_CMD_EXE;
        uResult = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }

        if (AT_IsTermiCmd(szName))
        {
          uState = ATP_STATE_SPARE;
        }
        else if (AT_IS_B_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_BASE;
        }
        else if (AT_IS_D(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_D;
        }
        else if (AT_IS_E_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_EXTENED;
        }
        else if (AT_IS_LETTER(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_B_NAME;
        }
        else if (AT_IS_SEMICOLON(*p))
        {
          p++;
          uState = ATP_STATE_T;
        }
        else if (IS_CR(*p))
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      break;
    case ATP_STATE_B_EQUAL:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_QUESTIONMARK(*p))
      {
        szName[i] = '\0';
        i         = 0;
        p++;
        uState = ATP_STATE_B_TEST;
      }
      else if (AT_IS_NUM(*p))
      {
        i         = 0;
        szPara[i] = *p;
        i++;
        p++;
        uType  = AT_CMD_SET;
        uState = ATP_STATE_B_PARAM;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_NAME:
      if (AT_IS_SPACING(*p))
      {
        p++;
        uState = ATP_STATE_E_NAME_T;
      }
      else if (AT_IS_NAME_CHARACTER(*p) || AT_IS_DIGIT(*p))
      {
        if (i >= AT_CMD_NAME_SIZE)
        {
          uRet   = _ERR_ATP_NMAE_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szName[i] = *p;
        i++;
        p++;
      }
      else if (AT_IS_QUESTIONMARK(*p))
      {
        szName[i] = '\0';
        p++;
        uState = ATP_STATE_E_READ;
      }
      else if (AT_IS_EQUITSIGN(*p))
      {
        szName[i] = '\0';
        p++;
        uState = ATP_STATE_E_EQUAL;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szName[i] = '\0';
        szPara[0] = '\0';
        uType     = AT_CMD_EXE;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (IS_CR(*p))
      {
        szName[i] = '\0';
        szPara[0] = '\0';
        uType     = AT_CMD_EXE;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_NAME_T:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_QUESTIONMARK(*p))
      {
        szName[i] = '\0';
        p++;
        uState = ATP_STATE_E_READ;
      }
      else if (AT_IS_EQUITSIGN(*p))
      {
        szName[i] = '\0';
        p++;
        uState = ATP_STATE_E_EQUAL;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szName[i] = '\0';
        szPara[0] = '\0';
        uType     = AT_CMD_EXE;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (IS_CR(*p))
      {
        szName[i] = '\0';
        szPara[0] = '\0';
        uType     = AT_CMD_EXE;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_READ:
      if (AT_IS_SEMICOLON(*p))
      {
        szPara[0] = '\0';
        uType     = AT_CMD_READ;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (IS_CR(*p))
      {
        szPara[0] = '\0';
        uType     = AT_CMD_READ;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_EQUAL:
      i = 0;
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_NEXTP;
      }
      else if (AT_IS_DIAL(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_NUM;
      }
      else if (AT_IS_QUESTIONMARK(*p))
      {
        p++;
        uState = ATP_STATE_E_TEST;
      }
      else if (AT_IS_QUOTATIONMARK(*p))
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_STR_BEGIN;
      }
      else
      {
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_STR_BEGIN;
//        uRet   = _ERR_ATP_SYNTAX_ERROR;
//        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_TEST:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szPara[0] = '\0';
        uType     = AT_CMD_TEST;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (IS_CR(*p))
      {
        szPara[0] = '\0';
        uType     = AT_CMD_TEST;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_PARAM_NUM:
      if (AT_IS_SPACING(*p))
      {
        p++;
        uState = ATP_STATE_E_PARAM_NUM_T;
      }
      else if (AT_IS_DIAL(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_NEXTP;
      }
      else if (IS_CR(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
      uState = ATP_STATE_E_PARAM_STR_BEGIN;
       // uRet   = _ERR_ATP_SYNTAX_ERROR;
        //uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_PARAM_NUM_T:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_NEXTP;
      }
      else if (IS_CR(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_E_NEXTP:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else if (AT_IS_DIAL(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_NUM;
      }
      else if (AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
      }
      else if (AT_IS_QUOTATIONMARK(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_STR_BEGIN;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (IS_CR(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
//Lixp add for string pass support abc as a string      
#if 1
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_STR_BEGIN;
#else        
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
#endif
      }
      break;
    case ATP_STATE_E_PARAM_STR_BEGIN:
      if (AT_IS_QUOTATIONMARK(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_PARAM_STR_END;
      }
      else if (AT_IS_TRANSMEAN(*p))
      {
        p++;
        uState = ATP_STATE_E_PARAM_STR_TM;
      }
        else if (IS_CR(*p))
        {
            if (i >= AT_CMD_PARA_SIZE)
            {
              uRet   = _ERR_ATP_PARAM_TOO_LONG;
              uState = ATP_STATE_FAIL_END;
              break;
            }
#if 0
            szPara[i] = *p;
            i++;
            p++;
            uState = ATP_STATE_E_PARAM_STR_END;
            break;
#else
            szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);

        AT_TC(g_sw_UART, "Get IS_CR uResult 0x%x",uResult);
        if (ERR_SUCCESS == uResult)
        {
          uRet   = ERR_SUCCESS;
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        uState = ATP_STATE_SUCC_END;
#endif
        }

      
      else
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;

      }

      break;
    case ATP_STATE_E_PARAM_STR_TM:
      if (i >= AT_CMD_PARA_SIZE)
      {
        uRet   = _ERR_ATP_PARAM_TOO_LONG;
        uState = ATP_STATE_FAIL_END;
        break;
      }
      szPara[i] = *p;
      i++;
      p++;
      uState = ATP_STATE_E_PARAM_STR_BEGIN;
      break;
    case ATP_STATE_E_PARAM_STR_END:
      if (AT_IS_SPACING(*p))
      {
        /* 
           if(i >= AT_CMD_PARA_SIZE)
           {
           uRet = _ERR_ATP_PARAM_TOO_LONG;  
           uState = ATP_STATE_FAIL_END;
           break;
           } 
           szPara[i] = *p;                
           i++;
         */
        p++;
      }
      else if (AT_IS_SEMICOLON(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        p++;
        uState = ATP_STATE_T;
      }
      else if (AT_IS_COMMA(*p))
      {
        if (i >= AT_CMD_PARA_SIZE)
        {
          uRet   = _ERR_ATP_PARAM_TOO_LONG;
          uState = ATP_STATE_FAIL_END;
          break;
        }
        szPara[i] = *p;
        i++;
        p++;
        uState = ATP_STATE_E_NEXTP;
      }
      else if (IS_CR(*p))
      {
        szPara[i] = '\0';
        uType     = AT_CMD_SET;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }
        uState = ATP_STATE_SUCC_END;
      }
      else
      {
        /* 
           if(i >= AT_CMD_PARA_SIZE)
           {
           uRet = _ERR_ATP_PARAM_TOO_LONG;  
           uState = ATP_STATE_FAIL_END;
           break;
           } 
           szPara[i] = *p;                
           i++;
           p++;
           uState = ATP_STATE_E_PARAM_STR_BEGIN;
         */
        uRet   = _ERR_ATP_SYNTAX_ERROR;
        uState = ATP_STATE_FAIL_END;
      }
      break;
    case ATP_STATE_B_READ:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else
      {
        szPara[0] = '\0';
        uType     = AT_CMD_READ;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }

        if (AT_IS_B_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_BASE;
        }
        else if (AT_IS_D(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_D;
        }
        else if (AT_IS_E_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_EXTENED;
        }
        else if (AT_IS_LETTER(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_B_NAME;
        }
        else if (AT_IS_SEMICOLON(*p))
        {
          p++;
          uState = ATP_STATE_T;
        }
        else if (IS_CR(*p))
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      break;
    case ATP_STATE_B_TEST:
      if (AT_IS_SPACING(*p))
      {
        p++;
      }
      else
      {
        szPara[0] = '\0';
        uType     = AT_CMD_TEST;
        uResult   = AT_AddNodeToChain(szName, szPara, uType, &pNodeH);
        if (ERR_SUCCESS == uResult)
        {
          uState = ATP_STATE_T;
        }
        else
        {
          uRet   = _ERR_ATP_NO_MORE_MEMORY;
          uState = ATP_STATE_FAIL_END;
        }

        if (AT_IS_B_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_BASE;
        }
        else if (AT_IS_D(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_D;
        }
        else if (AT_IS_E_PRIFIX(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_EXTENED;
        }
        else if (AT_IS_LETTER(*p))
        {
          szName[i] = *p;
          i++;
          p++;
          uState = ATP_STATE_B_NAME;
        }
        else if (AT_IS_SEMICOLON(*p))
        {
          p++;
          uState = ATP_STATE_T;
        }
        else if (IS_CR(*p))
        {
          uState = ATP_STATE_SUCC_END;
        }
        else
        {
          uRet   = _ERR_ATP_SYNTAX_ERROR;
          uState = ATP_STATE_FAIL_END;
        }
      }
      break;
    case ATP_STATE_SUCC_END:
      bLoop = FALSE;
      break;
    case ATP_STATE_FAIL_END:
      bLoop = FALSE;
      break;
    case ATP_STATE_AT_OK:
      bLoop = FALSE;
      break;
    case ATP_STATE_CR_END:
      bLoop = FALSE;
      break;
    default:
      bLoop = FALSE;
      break;
    }
  }

  // Set the output parameter.
  if (ERR_SUCCESS == uRet)
  {
    pParseResult->pNodeHead  = pNodeH;
    pParseResult->uNodeCount = AT_GetNodeCount(pParseResult->pNodeHead);
  }
  else
  {
    // If failed,clear the node chain. 
    AT_ClearNode(pNodeH);
  }
  return uRet;
}

// AT_AddNode
// Function
// This function add a node to command node chain.
// Parameters
// pName
// [in] The name of at command.
// pParam
// [in] The parameter of at command.
// uType
// [in] The type of at command.
// pNodeChain
// [in][out] Point to the header of node chain..
// Return Values
// If parse success,return ERR_SUCCESS ,else return ERR_FAIL.
UINT32 AT_AddNode(UINT8* pName, UINT8* pParam, UINT8 uType, AT_CMD_NODE* pNodeChain)
{
  UINT32 uRet = ERR_SUCCESS;

  AT_CMD_NODE* pNode = NULL;
  BOOL bCheat = TRUE;

  // Input parameter checking.
  if (NULL == pName || NULL == pParam || NULL == pNodeChain)
  {
    return _ERR_ATP_INVALID_PARA;
  }

  // Check the type.
  if (AT_CMD_SET != uType && AT_CMD_TEST != uType && AT_CMD_READ != uType && AT_CMD_EXE != uType)
  {
    return _ERR_ATP_INVALID_PARA;
  }

  // move the point of chain to last node.
  pNode = pNodeChain;
  do
  {
    if (NULL != pNode->pNext)
    {
      pNode = pNode->pNext;
    }
    else
    {
      break;
    }
  } while (TRUE == bCheat);

  // Create a new node.
  pNode->pNext = AT_CreateNode(pName, pParam, uType);
  if (NULL != pNode->pNext)
  {
    uRet = ERR_SUCCESS;
  }
  else
  {
    uRet = _ERR_ATP_NO_MORE_MEMORY;
  }

  return uRet;

}

// AT_CreateNode
// Function
// This function create new node.
// Parameters
// pName
// [in] The name of at command.
// pParam
// [in] The parameter of at command.
// uType
// [in] The type of at command.
// Return Values
// If parse success,return the point of new node,else return NULL.
AT_CMD_NODE* AT_CreateNode(UINT8* pName, UINT8* pParam, UINT8 uType)
{
  AT_CMD_NODE* pNode = NULL;

  // Input parameter checking.
  if (NULL == pName || NULL == pParam)
  {
    return NULL;
  }

  // Check the type.
  if (AT_CMD_SET != uType && AT_CMD_TEST != uType && AT_CMD_READ != uType && AT_CMD_EXE != uType)
  {
    AT_TC(g_sw_GC, "The type of command is error.type = %d", uType);
    return NULL;
  }

  // Create a new node.
  pNode = AT_MALLOC(SIZEOF(AT_CMD_NODE));
  if (NULL != pNode)
  {
    AT_MemZero(pNode, SIZEOF(AT_CMD_NODE));
    AT_StrCpy(pNode->szName, pName);
    AT_StrCpy(pNode->szParam, pParam);
    pNode->uType = uType;
    pNode->pNext = NULL;
  }
  else
  {
    AT_TC(g_sw_GC, "Malloc failed.");
  }

  return pNode;

}

// AT_ClearNode
// Function
// This function free all of the command node in the chain.
// Parameters
// pNodeChain
// [in][out] Point to the header of node chain..
// Return Values
// If parse success,return ERR_SUCCESS ,else return ERR_FAIL.
UINT32 AT_ClearNode(AT_CMD_NODE* pNodeChain)
{
  AT_CMD_NODE* pNodeN = NULL;

  pNodeN = pNodeChain;
  while (NULL != pNodeN)
  {
    pNodeChain = pNodeN->pNext;
    AT_FREE(pNodeN);
    pNodeN = pNodeChain;
  }
  return ERR_SUCCESS;
}

// AT_AddNodeToChain
// Function
// This function add a node to the chain,if the header point is NULL,will create a header node of chain.
// Parameters
// pName
// [in] The name of at command.
// pParam
// [in] The parameter of at command.
// uType
// [in] The type of at command.
// ppHeader
// [in][out] Point to a point of the header of node chain.
// Return Values
// If parse success,return ERR_SUCCESS ,else return ERR_FAIL.
UINT32 AT_AddNodeToChain(UINT8* pName, UINT8* pParam, UINT8 uType, AT_CMD_NODE** ppHeader)
{
  UINT32 iResult;

  if (NULL != pName)
  {
    AT_StrUpr(pName);
  }
  if (NULL ==* ppHeader)
  {
    *ppHeader = AT_CreateNode(pName, pParam, uType);
    if (NULL == *ppHeader)
    {
      return _ERR_ATP_NO_MORE_MEMORY;
    }
    else
    {
      return _ERR_ATP_SUCCESS;
    }
  }
  else
  {
    iResult = AT_AddNode(pName, pParam, uType, *ppHeader);
    if (ERR_SUCCESS == iResult)
    {
      return _ERR_ATP_SUCCESS;
    }
    else
    {
      return _ERR_ATP_NO_MORE_MEMORY;
    }
  }
}

// AT_GetNodeCount
// Function
// This function get  the count of node in the chain.
// Parameters
// ppHeader
// [in] Point to the header of node chain.
// Return Values
// return the count of node in che chain,if the chain header is NULL,reutnr 0.
UINT32 AT_GetNodeCount(AT_CMD_NODE* pNodeChain)
{
  UINT32 uCount = 0;

  while (NULL != pNodeChain)
  {
    pNodeChain = pNodeChain->pNext;
    uCount++;
  }
  return uCount;
}
 
