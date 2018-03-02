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



// 
// 印地语言
// 
//#include "stdafx.h"
#include "ml.h"

#ifdef CP0

 
//许多语言环境（例如格鲁吉亚语和印地语）都没有代码页，因为他们仅使用 Unicode 排序规则


static INT32 uni2char(UINT16 uni, UINT8* out, UINT32 boundlen)
{
//Add by lixp 
//Neednot check 
//
#if 0
  if( 2 != boundlen )
  {
    return ERR_ML_INVALID_CHARACTER;
  }
#endif
  out[0] = uni & 0xFF;
  out[1] = (uni >> 8) & 0xFF;
  
  return 2;
  
}

static INT32 char2uni(UINT8* rawstring, UINT32 boundlen, UINT16* uni)
{
//
//Add by lixp 
//Neednot check 
#if 0
  if( 2 != boundlen )
  {
    return ERR_ML_INVALID_CHARACTER;
  }
#endif
  *uni = ( rawstring[0] << 8 ) +  rawstring[1];
  return 2;
}

static struct ML_Table table = {
  (UINT8 *)"CP0",
  uni2char,
  char2uni,
  NULL,
  NULL,
};

UINT32 ML_InitCP0(void)
{
  return ML_RegisterTable(&table);
}

VOID ML_ExitCP0(void)
{
  ML_UnRegisterTable(&table);
}

#endif


