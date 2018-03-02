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
// Caoxh             2008-5-8            Create
// 
// //////////////////////////////////////////////////////////////////////////////

#ifndef __AT_PARSER_H__
#define __AT_PARSER_H__

#ifdef _MS_VC_VER_

#  include "at_cf.h"
#  include "at_define.h"

#else

#  include "at_common.h"

#endif

// The function macro define.
// Check the character if is dial digit.
#define AT_IS_DIAL_DIGIT(ch) ((('0' <= ch)&&('9' >= ch))  \
    || ('*' == ch)\
    || ('#' == ch)\
    || ('A'== ch)\
    || ('B' == ch)\
    || ('C' == ch)\
    || ('D' == ch)\
    || ('.' == ch)\
|| ('+' == ch))

// Check the character if is dial modifier.
#define AT_IS_DIAL_MODIFIER(ch) (('T' == ch)\
    || ('P' == ch)\
    || ('p' == ch)\
    || ('!' == ch)\
    || ('W' == ch)\
    || ('w' == ch)\
|| ('@' == ch))
// Check the character if is a dail character.
#define AT_IS_DIAL(ch) (AT_IS_DIAL_DIGIT(ch) || AT_IS_DIAL_MODIFIER(ch))

// Check the character if is the element at command name.
#define AT_IS_NAME_CHARACTER(ch) ((('a' <= ch)&&('z' >= ch))\
    || (('A' <= ch)&&('Z' >= ch))\
    || ('&' == ch)\
    || ('+' == ch)\
    || ('%' == ch)\
    || ('^' == ch)\
    || ('/' == ch)\
    || ('-' == ch)\
    || ('.' == ch)\
    || ('!' == ch)\
    || ('_' == ch)\
|| (':' == ch))
// Check the character if is Number.
// Digit 
#define AT_IS_DIGIT(ch) ((('0' <= ch)&&('9' >= ch)))

// Hex
#define AT_IS_HEX(ch) ((('0' <= ch)&&('9' >= ch)) ||\
    (('a' <= ch)&&('f' >= ch)) ||\
(('A' <= ch)&&('F' >= ch)))
// Bin
#define AT_IS_BIN(ch) ((('0' == ch) || ('1' == ch)))
#define AT_IS_NUM(ch)   (AT_IS_DIGIT(ch) || AT_IS_HEX(ch) || AT_IS_BIN(ch))

// Check the character if is A/a.
#define AT_IS_A(ch)       (('A' == ch) || ('a' == ch))
#define AT_IS_PLUS(ch)       ('+' == ch)
#define AT_IS_2PLUS(cha,cht) (('+' == cha) && ('+' == cht))
#define AT_IS_3PLUS(cha,cht,chp) (('+' == cha) && ('+' == cht)&& ('+' == chp))

// Check the character if is T/t.
#define AT_IS_T(ch)       (('T' == ch) || ('t' == ch))

// Check the prefix at command line if is legal.
#define AT_IS_AT(cha,cht) ((('A' == cha) && ('T' == cht)) || (('a' == cha) && ('t' == cht)))

// Check the character if is a letter.                        
#define AT_IS_LETTER(ch) ((('a' <= ch) && ('z' >= ch)) ||\
(('A' <= ch) && ('Z' >= ch)))

// Check the character if is a spacing.                                                
#define AT_IS_SPACING(ch) ((0x09 == ch) || (0x20 == ch))

// Check the character if is the prifix of extend command.
#define AT_IS_E_PRIFIX(ch) (('+' == ch) || ('^' == ch))

// Check the character if is the prifix of base command.
#define AT_IS_B_PRIFIX(ch) (('\\' == ch) || ('&' == ch)|| ('%' == ch))

// Check the character if is the command D.
#define AT_IS_D(ch) (('D' == ch) || ('d' == ch))

// Check the character if is semicolon.
#define AT_IS_SEMICOLON(ch) (';' == ch)

// Check the character if is sprit  
// add by wulc for AT\Q
// #define AT_IS_SPRIT(ch) ('\\' == ch)

// Check the character if is equit sign.
#define AT_IS_EQUITSIGN(ch) ('=' == ch)

// Check the character if is question mark.
#define AT_IS_QUESTIONMARK(ch) ('?' == ch)

// Check the character if is quotation mark.
#define AT_IS_QUOTATIONMARK(ch) ('"' == ch)

// Check the character if is comma.
#define AT_IS_COMMA(ch) (',' == ch)

// Check the character if is wall.
#define AT_IS_HASH(ch) ('#' == ch)

// Check the character if is star.
#define AT_IS_STAR(ch) ('*' == ch)

// Check the character if is character of transferred meaning.
#define AT_IS_TRANSMEAN(ch) ('\\' == ch)

#ifdef _MS_VC_VER_

#define CHAR_CR (0x0D)// carriage return
#define CHAR_LF (0x0A)// line feed
#define CHAR_BS (0x08)// back space
#define AT_CMD_NAME_SIZE            20
#define AT_CMD_PARA_SIZE            200
#else

#define CHAR_CR (AT_CMD_CR)
                                        // carriage return
#define CHAR_LF (AT_CMD_LF)
                                        // line feed
#define CHAR_BS (AT_CMD_BS)
                                        // back space

#endif

#define IS_BS(ch) (ch == CHAR_BS)
#define IS_LF(ch) (ch == CHAR_LF)
#define IS_CR(ch) (ch == CHAR_CR)

// The state of parse define.
#define ATP_STATE_INIT               1
#define ATP_STATE_A                  2
#define ATP_STATE_T                  3
#define ATP_STATE_D                  4
#define ATP_STATE_BASE               5
#define ATP_STATE_EXTENED            6
#define ATP_STATE_D_PARAM            7
#define ATP_STATE_D_SEMIC            8
#define ATP_STATE_SPARE            9
#define ATP_STATE_B_NAME            10
#define ATP_STATE_B_NAME_T          11
#define ATP_STATE_B_EQUAL           12
#define ATP_STATE_B_PARAM           13
#define ATP_STATE_E_NAME            14
#define ATP_STATE_E_READ            15
#define ATP_STATE_E_EQUAL           16
#define ATP_STATE_E_TEST            17
#define ATP_STATE_E_PARAM_NUM       18
#define ATP_STATE_E_PARAM_STR_BEGIN 19
#define ATP_STATE_E_PARAM_STR_TM    20
#define ATP_STATE_E_NEXTP           21
#define ATP_STATE_E_PARAM_STR_END   22
#define ATP_STATE_E_PARAM_NUM_T     23
#define ATP_STATE_E_NAME_T          24
#define ATP_STATE_B_READ            25
#define ATP_STATE_B_TEST            26
#define ATP_STATE_D_PARAM_STAR      27
#define ATP_STATE_SUCC_END          30
#define ATP_STATE_FAIL_END          31
#define ATP_STATE_AT_OK             32
#define ATP_STATE_CR_END            33
#define ATP_STATE_PLUS           34 // add by wulc
#define ATP_STATE_2PLUS           35  // add by wulc
#define ATP_STATE_3PLUS           36  // add by wulc

// The size define.

#ifdef CODEC_SUPPORT
#define ATP_CMD_LINE_SIZE           550
#else
#define ATP_CMD_LINE_SIZE           AT_CMD_LINE_BUFF_LEN
#endif

// error code define for at parse.
#define _ERR_ATP_INVALID_PARA       1001
#define _ERR_ATP_NO_MORE_MEMORY     1002
#define _ERR_ATP_WITHOUT_CR         1003
#define _ERR_ATP_SYNTAX_ERROR       1004
#define _ERR_ATP_NMAE_TOO_LONG      1005
#define _ERR_ATP_PARAM_TOO_LONG     1006
#define _ERR_ATP_SUCCESS            0

// The structure define.
// The base command name struct.
typedef struct _at_cmd_name
{
  UINT8* pName; // The max length is 40.
} AT_CMD_NAME;

// The command node struct.
typedef struct _at_cmd_node
{
  UINT8 szName[AT_CMD_NAME_SIZE + 1];
  UINT8 szParam[AT_CMD_PARA_SIZE + 1];
  UINT8 uType;
  struct _at_cmd_node* pNext;
  UINT8 uReserved[3];
} AT_CMD_NODE;

// The parse result struct.
typedef struct _at_parse_result
{
  UINT32 uNodeCount;
    AT_CMD_NODE* pNodeHead;
}
AT_PARSE_RESULT;

BOOL AT_IsBaseCmd(UINT8* pName);

BOOL AT_IsTermiCmd(UINT8* pName);

UINT32 AT_CmdLineSyntaxParser(UINT8* pCmdBuf, UINT32 uCmdLen, AT_PARSE_RESULT* pParseResult);

AT_CMD_NODE* AT_CreateNode(UINT8* pName, UINT8* pParam, UINT8 uType);

UINT32 AT_AddNode(UINT8* pName, UINT8* pParam, UINT8 uType, AT_CMD_NODE* pNodeChain);

UINT32 AT_AddNodeToChain(UINT8* pName, UINT8* pParam, UINT8 uType, AT_CMD_NODE** ppHeader);
UINT32 AT_ClearNode(AT_CMD_NODE* pNodeChain);

UINT32 AT_GetNodeCount(AT_CMD_NODE* pNodeChain);

#endif // __AT_PARSER_H__
 
