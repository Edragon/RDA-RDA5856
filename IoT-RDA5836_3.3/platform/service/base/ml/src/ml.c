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


#include "ml.h"
#ifdef WIN32

#else
#include "errorcode.h"

#endif

#ifdef _FS_SIMULATOR_

#else

#include "sul.h"
//#include "cfw_prv.h"
#endif
#include "bal_resource.h"
//#include "sul.h"
//#include "cfw_prv.h"

static struct ML_Table ML_table;
static struct ML_Table* pML_table = &ML_table;

BOOL g_Flag           = FALSE;
UINT8 g_Character[12] = { 0, };


UINT32 ML_RegisterTable(struct ML_Table* ML)
{
  struct ML_Table** p = &pML_table;

  if (!p)
  {
    return ERR_INVALID_HANDLE;
  }

  if (ML->next)
  {
    return ERR_BUSY_DEVICE;
  }

  if (1 == g_Flag)
  {
    return ERR_BUSY_DEVICE;
  }
  else
  {
    g_Flag = TRUE;
  }

  while (*p)
  {
    if (ML == *p)
    {
      g_Flag = FALSE;
      return ERR_BUSY_DEVICE;
    }

    p = &(*p)->next;
  }

  ML->next = pML_table;

  pML_table = ML;
  g_Flag    = FALSE;

  return ERR_SUCCESS;
}

UINT32 ML_UnRegisterTable(struct ML_Table* ML)
{
  return ERR_SUCCESS;
}

#if 0
static struct ML_Table* ML_FindTable(UINT8* charset)
{
  struct ML_Table* p;

  if (NULL == charset)
  {
    return NULL;
  }
  for (p = pML_table; p; p = p->next)
  {
    if( NULL == p->charset )
    {
      return NULL;
    }
    if (!SUL_StrCaselessCompare((TCHAR*)p->charset, (TCHAR*)charset))
      break;
  }
  return p;
}
#endif

UINT32 ML_SetCodePage(UINT8 nCharset[12])
{
  SUL_StrCopy((TCHAR*)g_Character,(TCHAR*) nCharset);
  return ERR_SUCCESS;
}

UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  UINT32 o;
  //UINT16 uni = 0x0000;
  INT32 unilen, utflen;
  UINT8* result;
  //struct ML_Table* pTable = NULL;
  int maxlen;

  unilen = utflen = 0x00;

  //
  //Add "\0"
  //
  maxlen = in_len + 1;

//  CSW_TRACE(CFW_ML_TS_ID,"---> ML_Unicode2LocalLanguage()\n");
  maxlen = ALIGN(maxlen,4);

  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result, 0x00,(UINT16)maxlen);
  SUL_MemCopy8(result, in, in_len);
#if 0
  //Add by lixp at 20090211 for mult task conflicting
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
	  CSW_TC_MEMBLOCK(CFW_ML_TS_ID, (UINT8 *)nCharset,12 ,16);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
	  CSW_TC_MEMBLOCK(CFW_ML_TS_ID, (UINT8 *)g_Character,12 ,16);
  }
  if( NULL == pTable )
  {
    
      goto conv_err;  
  }
  for (i = o = 0; i < in_len; i += 2, o += unilen)
  {

    /* convert from Unicode to locallanguage */
    unilen = pTable->uni2char((*(UINT16*)(&in[i])), &result[o], in_len - o);
    if (unilen < 0)
    {
      CSW_TRACE(CFW_ML_TS_ID,"Input Data[0x%x]\n",(*(UINT16*)(&in[i])));
      goto conv_err;
    }
  }
#endif
  o = gui_unicode2local(result, maxlen);
  result[o] = '\0';
  *out_len  = o;

 // CSW_TRACE(CFW_ML_TS_ID,"<--- ML_Unicode2LocalLanguage()\n");

  return ERR_SUCCESS;

/*conv_err:
  CSW_TRACE(CFW_ML_TS_ID,"conv_err <--- ML_Unicode2LocalLanguage()\n");
  CSW_ML_FREE(result);
  *out = NULL;
  return ERR_INVALID_HANDLE;*/
}

UINT32 ML_LocalLanguage2Unicode(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  //struct ML_Table* pTable = NULL;
  INT32 i, o;
  //UINT16 uni;
  INT32 unilen, utflen;
  UINT8* result;
  int maxlen;
  unilen = utflen = 0;

  maxlen = (3 * in_len)+1;

  CSW_TRACE(CFW_ML_TS_ID,"---> ML_LocalLanguage2Unicode()\n");

  maxlen = ALIGN(maxlen,4);
  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    CSW_TRACE(CFW_ML_TS_ID,"ML_LocalLanguage2Unicode() cannot allocate memory\n");
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result,0x00,maxlen);
#if 0
  //Add by lixp at 20090211 for mult task conflicting
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
  }  
  if( NULL == pTable )
  {
    
      CSW_TRACE(0,"Err nCharset %s g_Character %s\n",nCharset,g_Character);
      goto conv_err;  
  }
  for (i = o = 0; i < (INT32)in_len; i += unilen, o += 2)
  {

    /* convert from locallanguage to unicode */
    unilen = pTable->char2uni((UINT8*)&in[i], in_len - i, &uni);
    if (unilen < 0)
    {
      CSW_TRACE(0,"Err 1 nCharset %s g_Character %s\n",nCharset,g_Character);
      goto conv_err;
    }
    result[o] = uni >> 8;
    result[o + 1] = uni & 0xFF;

  }
#else
  for (i = o = 0; i < (INT32)in_len; i ++, o += 2)
  {
    result[o + 1] = in[i] ;
    result[o] = 0;

  }
#endif
  result[o] = '\0';
  result[o+1] = '\0';
  *out_len  = o+1;

  CSW_TRACE(CFW_ML_TS_ID,"<--- ML_LocalLanguage2Unicode()\n");

  return ERR_SUCCESS;

/*conv_err:
  CSW_TRACE(CFW_ML_TS_ID,"conv_err<--- ML_LocalLanguage2Unicode()\n");
  CSW_ML_FREE(result);
    *out = NULL;
  return ERR_INVALID_HANDLE;*/
}

UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12])
{
  //struct ML_Table* pTable = NULL;
  INT32 i, o;
  //UINT16 uni;
  INT32 unilen, utflen;
  UINT8* result;
  int maxlen = (3 * in_len)+1;

  unilen = utflen = 0;

//  CSW_TRACE(CFW_ML_TS_ID,"---> ML_LocalLanguage2UnicodeBigEnding()\n");
  maxlen = ALIGN(maxlen,4);
  *out = result = (UINT8*)CSW_ML_MALLOC(maxlen);
  if (!*out)
  {
    CSW_TRACE(CFW_ML_TS_ID,"ML_LocalLanguage2UnicodeBigEnding() cannot allocate memory\n");
    *out_len = 0;
    return ERR_INVALID_HANDLE;
  }
  SUL_MemSet8(result,0x00,maxlen);

  //Add by lixp at 20090211 for mult task conflicting
#if 0
  if( NULL != nCharset )
  {
    pTable = ML_FindTable(nCharset);
  }
  else
  {
    pTable = ML_FindTable(g_Character);
  }  
  if( NULL == pTable )
  {
      CSW_TRACE(CFW_ML_TS_ID,"Table NULL g_Character %s\n",g_Character);
      CSW_TRACE(CFW_ML_TS_ID,"Table NULL g_Character %s\n",nCharset);
      goto conv_err;  
  }

  for (i = o = 0; i < (INT32)in_len; i += unilen, o += 2)
  {

    unilen = pTable->char2uni((UINT8*)&in[i], in_len - i, &uni);
    if (unilen < 0)
    {
      CSW_TRACE(CFW_ML_TS_ID,"char2uni NULL in %d\n",in);
      goto conv_err;
    }

//    CSW_TRACE(CFW_ML_TS_ID,"The uni is 0x%x\r\n",uni);
    result[o] = uni & 0xFF ;
    result[o + 1] = uni >> 8;

  }
#else  // no ml table support for local to unicode
  for (i = o = 0; i < (INT32)in_len; i ++, o += 2)
  {
    result[o] = in[i] ;
    if(result[o] <= 127)
        result[o + 1] = 0;
    else
        result[o + 1] = in[++i];
  }
#endif

  result[o] = '\0';
  result[o+1] = '\0';
  *out_len  = o+1;

//  CSW_TRACE(CFW_ML_TS_ID,"<--- ML_LocalLanguage2UnicodeBigEnding()\n");

  return ERR_SUCCESS;

/*conv_err:
  CSW_ML_FREE(result);
  *out = NULL;
  return ERR_INVALID_HANDLE;*/
}
UINT32 ML_InitCP0(void);
UINT32 ML_InitCP1256(void);
UINT32 ML_InitCP1258(void);
UINT32 ML_InitCP874(void);
UINT32 ML_InitCP936(void);
UINT32 ML_InitISO8859_1(void);
UINT32 ML_InitISO8859_5(void);
UINT32 ML_InitISO8859_6(void);

UINT32 ML_Init()
{
//#ifdef ISO8859_1
  ML_InitISO8859_1();
//#endif

  #ifdef CP437 
  ML_InitCP437();
  #endif   
      
  #ifdef CP737  
  ML_InitCP737(); 
  #endif   
     
  #ifdef CP775  
  ML_InitCP775();         
  #endif  

  #ifdef CP850
  ML_InitCP850();       
  #endif 
           
  #ifdef CP852
  ML_InitCP852();       
  #endif          
          
  #ifdef CP855
  ML_InitCP855();       
  #endif          
                  
  #ifdef CP857
  ML_InitCP857();       
  #endif          
                  
  #ifdef CP860
  ML_InitCP860();       
  #endif          
                  
  #ifdef CP861
  ML_InitCP861();       
  #endif          
                  
  #ifdef CP862
  ML_InitCP862();       
  #endif          
                  
  #ifdef CP863
  ML_InitCP863();       
  #endif          
                  
  #ifdef CP864
  ML_InitCP864();       
  #endif          
                  
  #ifdef CP865
  ML_InitCP865();       
  #endif          
                  
  #ifdef CP869
  ML_InitCP869();       
  #endif          
                  
  #ifdef CP874
  ML_InitCP874();       
  #endif          
                  
  #ifdef CP932
  ML_InitCP932();       
  #endif          
                  
  #ifdef CP936
  ML_InitCP936();       
  #endif          
                  
  #ifdef CP950
  ML_InitCP950();       
  #endif          
                  
  #ifdef CP1250
  ML_InitCP1250();       
  #endif          
          				
  #ifdef CP1251
  ML_InitCP1251();       
  #endif          
                 
  #ifdef CP1255
  ML_InitCP1255();       
  #endif          
                 
  #ifdef CP1256
  ML_InitCP1256();       
  #endif          
                 
  #ifdef CP1258
  ML_InitCP1258();       
  #endif          
                 
  #ifdef ISO8859_1
  //ML_InitISO8859_1();       
  #endif          
              
  #ifdef ISO8859_2
  ML_InitISO8859_2();       
  #endif          
              
  #ifdef ISO8859_3
  ML_InitISO8859_3();       
  #endif          
              
  #ifdef ISO8859_4
  ML_InitISO8859_4();       
  #endif          
              
  #ifdef ISO8859_5
  ML_InitISO8859_5();       
  #endif          
              
  #ifdef ISO8859_6
  ML_InitISO8859_6();       
  #endif          
              
  #ifdef ISO8859_7
  ML_InitISO8859_7();       
  #endif          
             
  #ifdef ISO8859_9
  ML_InitISO8859_9();       
  #endif          
              
  #ifdef ISO8859_13 
  ML_InitISO8859_13();      
  #endif          
            
  #ifdef ISO8859_14 
  ML_InitISO8859_14();       
  #endif          
            
  #ifdef ISO8859_15   
  ML_InitISO8859_15();       
  #endif   
  return ERR_SUCCESS;
}

