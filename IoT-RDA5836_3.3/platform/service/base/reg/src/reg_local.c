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


#include "dsm_cf.h"
#include <error_id.h>
#include "reg.h"
#include "reg_local.h"
#include "dsm_dbg.h"
#include "vds_api.h"


extern struct REG_Global_Var g_RegGlobalVar;
extern char *default_registry;
extern REG_H_INFO g_sHInfo;
extern REG_RK_OFFSET g_sRKOffset[];
extern  REG_RSKD     g_sRSK[];


INT32 reg_MediaInit()
{
    INT32 i;
    INT32 iErrCode = 0;    
    UINT32 uDevNr;
    UINT32 uNrBlock;
    UINT32 uBlockSize;
    
#ifdef _REG_SMS_MERGE
    uDevNr = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);
#else 
    uDevNr = DSM_GetDevNrOnModuleId(DSM_MODULE_REG);
#endif
    if(uDevNr == INVALID_DEVICE_NUMBER)
    {
         return _ERR_REG_DEV_NOT_EXIST;
    }

    g_RegGlobalVar.iMediaSize= 0;

   iErrCode = DRV_GET_DEV_INFO( uDevNr,&uNrBlock,&uBlockSize);   
   if(iErrCode == ERR_SUCCESS)
   {
       g_RegGlobalVar.iMediaSize =  (uBlockSize * uNrBlock);
   }
   else
   {
       g_RegGlobalVar.iMediaSize= 0;
   }


    for(i = 0;i < REG_OPENKEY_MAX;i++)
    {
        g_sRSK[i].iFlag = REG_INDEX_FREE;
    }
    reg_SemInit();        

    return ERR_SUCCESS;
}


BOOL reg_Existed()
{
    UINT32 magic = 0;
    UINT32 rsize;
    UINT32 addr;
    
    addr = 0;    
    
    REG_Read(addr, (UINT8*)&magic, REG_MAGIC_SIZE, &rsize);
    if(magic == REG_MAGIC)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


INT32 reg_GetFreeRSK()
{
    INT32 i;
    
    for(i=0;i<REG_OPENKEY_MAX;i++)
    {
        if(REG_INDEX_FREE == g_sRSK[i].iFlag)
        {
            return i;
        }
    }
    
    return -1; 
}


INT32 reg_SetRSK(HANDLE hKey,PCSTR pcSubKey,INT32 iIndex,HANDLE* phKey)
{
    if ((REG_INDEX_INUSE !=g_sRSK[ iIndex].iFlag)&&(REG_INDEX_FREE!=g_sRSK[ iIndex].iFlag))
    {
        return _ERR_REG_OVER_POSITION ;
    }
    g_sRSK[iIndex].iFlag = REG_INDEX_INUSE;
    g_sRSK[iIndex].iRKey = hKey;
    DSM_StrCpy (g_sRSK[iIndex].szSK, pcSubKey);
    *phKey = iIndex;
    return ERR_SUCCESS;
}


INT32 reg_FreeRSK(INT32 RSKIndex)
{
    if(REG_INDEX_INUSE != g_sRSK[RSKIndex].iFlag)
    {
        return _ERR_REG_OVER_POSITION;
    }
    g_sRSK[RSKIndex].iFlag = REG_INDEX_FREE ;
    g_sRSK[RSKIndex].iRKey = 0;
    DSM_MemSet(g_sRSK[RSKIndex].szSK,0,REG_SUBKEY_LEN_MAX);
    return ERR_SUCCESS;
}


#if 0
INT32 reg_Buff2RInfo(UINT8* pStr,REG_R_INFO* pRInfo)
{
    UINT32 i; 
    
    if((NULL == pRInfo) ||(NULL ==pStr))
    {
        return _ERR_REG_INVALID_PARAMETER;                                  //2006-9-29
    }
    DSM_MemSet(pRInfo,0,SIZEOF(REG_R_INFO));
    i = 0;
    pRInfo->iRKey = pStr[i];
    i++;
    pRInfo->iSKSize = pStr[i] ;
    i++;
    pRInfo->iVNSize = pStr[i] ;
    i++;
    pRInfo->iDType = pStr[i] ;
    i++;
    pRInfo->iDSize = pStr[i];
    i++;
    DSM_MemCpy(pRInfo->szSK,(pStr+i),pRInfo->iSKSize);
    i += REG_SUBKEY_LEN_MAX;
    DSM_MemCpy(pRInfo->szVN,(pStr+i),pRInfo->iVNSize);
    i += REG_VALUENAME_LEN_MAX;
    reg_BuffToInt(pStr+i,&pRInfo->iCheckSum,REG_CHECKSUM_SIZE);
    //D((DL_REGWARNING, "ichecksum = %d", pRInfo->iCheckSum));
    
    i += REG_CHECKSUM_SIZE;
    DSM_MemCpy(pRInfo->u_data.szStr,(pStr+i),pRInfo->iDSize);
    return ERR_SUCCESS;
}



INT32 reg_RInfo2Buff(REG_R_INFO* pRInfo,UINT8* pStr)
{
    
    UINT32 i; 
    
    if((NULL == pRInfo) ||(NULL ==pStr))
    {
        return _ERR_REG_INVALID_PARAMETER;                                  //2006-9-29
    }
    DSM_MemSet(pStr,0,REG_R_LEN);
    i = 0;
    pStr[i] = pRInfo->iRKey;
    
    i++;
    pStr[i] = pRInfo->iSKSize;
    
    i++;
    pStr[i] = pRInfo->iVNSize;
    
    i++;
    pStr[i] = pRInfo->iDType;
    
    i++;
    pStr[i] = pRInfo->iDSize;
    
    i++;
    DSM_MemCpy((pStr+i),pRInfo->szSK,pRInfo->iSKSize);
    
    i += REG_SUBKEY_LEN_MAX;
    DSM_MemCpy((pStr+i),pRInfo->szVN,pRInfo->iVNSize);
    
    i += REG_VALUENAME_LEN_MAX;
    reg_IntToBuff(pRInfo->iCheckSum,pStr+i,4);
    
    i += REG_CHECKSUM_SIZE;
    DSM_MemCpy((pStr+i),pRInfo->u_data.szStr,pRInfo->iDSize);
    return ERR_SUCCESS;
}



INT32 reg_Buff2CTR(UINT8* pStr,REG_R_CTR* pCtr)
{
    UINT32 i; 
    if((NULL == pStr)||(NULL == pCtr))
    {
        return _ERR_REG_INVALID_PARAMETER;        //2006-09-29
    }
    DSM_MemSet(pCtr,0,REG_R_CTR_LEN);
    i = 0;
    pCtr->iRKey = pStr[i];
    i++;
    pCtr->iSKSize = pStr[i];
    i++;
    pCtr->iVNSize = pStr[i];
    i++;
    pCtr->iDType = pStr[i];
    i++;
    pCtr->iDSize = pStr[i];
    i++;
    pCtr->iSKSize = (UINT8)(pCtr->iSKSize > REG_SUBKEY_LEN_MAX ? REG_SUBKEY_LEN_MAX : pCtr->iSKSize);
    DSM_MemCpy(pCtr->szSK,(pStr+i),pCtr->iSKSize);
    
    i += REG_SUBKEY_LEN_MAX;
    pCtr->iVNSize = (UINT8)(pCtr->iVNSize > REG_VALUENAME_LEN_MAX ? REG_VALUENAME_LEN_MAX : pCtr->iVNSize);
    DSM_MemCpy(pCtr->szVN,(pStr+i),pCtr->iVNSize);
    
    i += REG_VALUENAME_LEN_MAX;
    reg_BuffToInt(pStr+i, &pCtr->iCheckSum, 4);
    
    return ERR_SUCCESS     ;              //2006-09-29
}



INT32 reg_CTR2Buff(REG_R_CTR* pCtr,UINT8* pStr)
{
    UINT32 i;
    
    if((NULL == pStr)||(NULL == pCtr))
    {
        return _ERR_REG_INVALID_PARAMETER;        //2006-09-29
    }
    i = 0;
    //pStr = (UINT8*)pCtr;
    //DSM_MemSet()
    pStr[i] = pCtr->iRKey ;
    i++;
    pStr[i] = pCtr->iSKSize;
    i++;
    pStr[i] = pCtr->iVNSize;
    i++;
    pStr[i] = pCtr->iDType;
    i++;
    pStr[i] = pCtr->iDSize; 	
    i++;
    DSM_MemCpy((pStr+i),pCtr->szSK,pCtr->iSKSize);
    
    i += REG_SUBKEY_LEN_MAX;
    DSM_MemCpy((pStr+i),pCtr->szVN,pCtr->iVNSize);
    
    i += REG_VALUENAME_LEN_MAX;
    reg_IntToBuff(pCtr->iCheckSum, pStr+i, 4);
    return ERR_SUCCESS;
}

#endif


INT32 reg_FindValueName(HANDLE hKey,PCSTR pcSubKey,REG_R_CTR* pRCtr, PCSTR pValueName,INT32 iRecOffset)
{
    INT32 iRet;
    REG_R_CTR sRCtr;
    BOOL bLoop;
    UINT32 iLastOffset;
    UINT32 iOffset;
    UINT32 iReadLen;    
    UINT32 iValueNameLen;
    UINT32 iSubKeyLen;

    
    if((REG_HKEY_SYSTEM_ROOT !=hKey)&&(REG_HKEY_CURRENT_USERS!=hKey)&&
        (REG_HKEY_LOCAL_MACHINE!=hKey)&&(REG_HKEY_CURRENT_COMM!=hKey))
    {
        return _ERR_REG_NOENTRY;
    }
    if((NULL == pRCtr) ||(NULL == pValueName)||(NULL == pcSubKey))
    {
        return  _ERR_REG_NOENTRY;  
    }
    iOffset = iRecOffset+REG_H_SIZE;
    iRet = _ERR_REG_NOENTRY;
    iLastOffset = g_sHInfo.iRegSize; //g_sHInfo.iRecCount*REG_R_LEN+REG_H_SIZE;
    bLoop = TRUE;
    iReadLen = 0;
    
    //here ,iValueNameLen is only the lenth of the input_name
    iValueNameLen = DSM_StrLen(pValueName);
    iSubKeyLen = DSM_StrLen(pcSubKey);
    while(TRUE == bLoop)
    {
        if(iOffset>= iLastOffset)
        {
            D((DL_REGERROR, "in reg_FindValueName ERROR,iOffset = %d,iLastOffet = %d.",iOffset,iLastOffset));
            iRet = _ERR_REG_NOENTRY;
            break;
        }
        DSM_MemSet(&sRCtr,0,REG_R_CTR_LEN);
        iRet = REG_Read(iOffset,(UINT8*)(&sRCtr),REG_R_CTR_LEN,&iReadLen);
        if(REG_R_CTR_LEN == iReadLen||( iRet != ERR_SUCCESS))
        {
            if(sRCtr.iRKey == 0xff)
            {
                D((DL_REGERROR, "sRCtr.iRKey == 0xff."));
                return _ERR_REG_NOENTRY;
            }
            
            if((iValueNameLen != DSM_StrLen(sRCtr.szVN)) ||
                (sRCtr.iSKSize != iSubKeyLen ))
            {
                iOffset += REG_R_LEN;
                continue;
            }
            if((DSM_StrCmp(sRCtr.szSK,pcSubKey) == 0) &&
                (hKey== sRCtr.iRKey) &&
                (DSM_StrCmp(sRCtr.szVN,pValueName)== 0))                
            {
                DSM_MemCpy(pRCtr,&sRCtr,sizeof(REG_R_CTR));
                iRet = iOffset;
                break;
            }
            else
            {
                iOffset += REG_R_LEN;
            }
        }
        else
        {
            iRet = _ERR_REG_RMEDIA_FAILED;
            break;
        }
    }   
    return iRet;
}



// Parse the line , return the result of parsed and line size.
INT32 reg_GetLineInfo(const UINT8* pPos,REG_DEF_LINE_INFO* pLineInfo,UINT32* piSize)
{
        
    UINT8* p  = NULL; // point to the pPos parameter initalized,it will move from the line header to line tail. 
    
   // UINT8 szSubKeyName[REG_SUBKEY_LEN_MAX + LEN_FOR_NULL_CHAR];
   // UINT8 szValueName[REG_VALUENAME_LEN_MAX + LEN_FOR_NULL_CHAR];
   // UINT8 szNum[REG_DATA_TYPE_INT32_NUM_LEN + LEN_FOR_NULL_CHAR]; 
   // UINT8 szValueType[REG_DEF_STR_TYPE_LEN + LEN_FOR_NULL_CHAR];
   // UINT8 szRootKeyName[REG_DEF_STR_ROOT_KEY_LEN + LEN_FOR_NULL_CHAR];    
    UINT8 szBuff[REG_DEF_STR_ROOT_KEY_LEN + LEN_FOR_NULL_CHAR];
    //INT32 i;
    INT32 n = 0;
    UINT32 iUnitSize = 0;  
    INT32 iRet = _ERR_REG_SUCCESS;
    UINT32 iState = REG_PARSE_STATE_INIT;    
    BOOL bLoop = TRUE;
    

    
    if(NULL == pPos || NULL == pLineInfo || 0 == piSize)
    {
        return _ERR_REG_INVALID_PARAMETER;
    }
    
    DSM_MemSet(pLineInfo,0,SIZEOF(REG_DEF_LINE_INFO));
    p = (UINT8*)pPos;
    *piSize = 0;

    while(TRUE == bLoop)
    {
        if(' ' == *p)
        {
            p++;
            continue;
        }
        if('\n' == *p)
        {           
            if((REG_PARSE_STATE_FINISH != iState) &&
               (REG_PARSE_STATE_VALUE_HEX != iState) &&
               (REG_PARSE_STATE_VALUE_INT8 != iState) &&
               (REG_PARSE_STATE_VALUE_INT32 != iState) &&
               (REG_PARSE_STATE_END != iState))
            {
                iRet = _ERR_REG_CFG_ORDER_WRONG;
                break;                
            }
        }
        switch(iState)
        {            
            case REG_PARSE_STATE_INIT:
                if('[' == *p)
                {
                    iState = REG_PARSE_STATE_ROOTKEY;
                    DSM_MemSet(szBuff,0,REG_DEF_STR_ROOT_KEY_LEN + LEN_FOR_NULL_CHAR);  
                    pLineInfo->iType = REG_DEF_STR_LINE_TYPE_KEY;
                }
                else if('\"' == *p)
                {
                    iState = REG_PARSE_STATE_VALUENAME;   
                    pLineInfo->iType = REG_DEF_STR_LINE_TYPE_VALUE;
                }
                else
                {
                    iRet = _ERR_REG_CFG_ORDER_WRONG;
                    bLoop = FALSE;
                    break;  
                }
                break;
            case REG_PARSE_STATE_ROOTKEY:
                if(iUnitSize > REG_DEF_STR_ROOT_KEY_LEN)
                {
                    iRet = _ERR_REG_CFG_ROOT_KEY_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }

                if('/' == *p)
                {                  
                    szBuff[iUnitSize] = '\0';
                    iUnitSize = 0;
                    iState = REG_PARSE_STATE_SUB_KEY; 
                    if(DSM_StrCaselessCmp(szBuff,"REG_HKEY_SYSTEM_ROOT") == 0)
                    {
                        pLineInfo->uLine.sKeyLine.iRKey = REG_HKEY_SYSTEM_ROOT;
                        
                    }
                    else if(DSM_StrCaselessCmp(szBuff,"REG_HKEY_LOCAL_MACHINE") == 0)
                    {
                        pLineInfo->uLine.sKeyLine.iRKey = REG_HKEY_LOCAL_MACHINE;
                    }
                    else if(DSM_StrCaselessCmp(szBuff,"REG_HKEY_CURRENT_COMM") == 0)
                    {
                        pLineInfo->uLine.sKeyLine.iRKey = REG_HKEY_CURRENT_COMM;
                    }
                    else if(DSM_StrCaselessCmp(szBuff,"REG_HKEY_CURRENT_USERS") == 0)
                    {
                        pLineInfo->uLine.sKeyLine.iRKey = REG_HKEY_CURRENT_USERS;
                    }
                    else
                    {
                        iRet = _ERR_REG_CFG_ROOT_KEY_UNKNOW;
                        bLoop = FALSE;
                        break;  
                    }                    
                }                
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;
                }                
                break;
            case REG_PARSE_STATE_SUB_KEY:
                if(iUnitSize > REG_SUBKEY_LEN_MAX)
                {
                    iRet = _ERR_REG_CFG_SUB_KEY_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }

                if(']' == *p)
                {
                    iState = REG_PARSE_STATE_FINISH; 
                    szBuff[iUnitSize] = '\0';
                    pLineInfo->uLine.sKeyLine.iSKSize = (UINT8)iUnitSize;
                    DSM_StrCpy(pLineInfo->uLine.sKeyLine.szSK, szBuff);
                    iUnitSize = 0;
                }                
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;                    
                }
                break;
            case REG_PARSE_STATE_VALUENAME:
                if(iUnitSize > REG_VALUENAME_LEN_MAX)
                {
                    iRet = _ERR_REG_CFG_VALUE_NAME_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }

                if('\"' == *p)
                {
                    iState = REG_PARSE_STATE_EQUAL_TO;
                    szBuff[iUnitSize] = 0;
                    DSM_StrCpy(pLineInfo->uLine.sValueLine.szVN,szBuff);
                    pLineInfo->uLine.sValueLine.iVNSize = (UINT8)iUnitSize;                    
                    iUnitSize = 0;
                }                
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;   
                }
                
                break;
            case REG_PARSE_STATE_EQUAL_TO:
                if('=' == *p)
                {
                    iState = REG_PARSE_STATE_VALUE_TYPE_BEGIN;                    
                }                
                else
                {
                    iRet = _ERR_REG_CFG_ORDER_WRONG;
                    bLoop = FALSE;
                    break;  
                }
                break;
            case REG_PARSE_STATE_VALUE_TYPE_BEGIN:
                if('\"' == *p)
                {
                    iState = REG_PARSE_STATE_VALUE_STR;                   
                }              
                else
                {
                    iState = REG_PARSE_STATE_VALUE_TYPE;
                    DSM_MemSet(szBuff,0,REG_DEF_STR_TYPE_LEN + LEN_FOR_NULL_CHAR);
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;
                }
                break;
            case REG_PARSE_STATE_VALUE_TYPE:
                if(iUnitSize > REG_DEF_STR_TYPE_LEN)
                {
                    iRet = _ERR_REG_CFG_VALUE_TYPE_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }
                if(':' == *p)
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;
                    szBuff[iUnitSize] = '\0';
                    if(DSM_StrCaselessCmp(szBuff,REG_TYPE_STR_BINARY) == 0)
                    {
                        iState = REG_PARSE_STATE_VALUE_HEX; 
                        n = 0;
                    }
                    else if(DSM_StrCaselessCmp(szBuff,REG_TYPE_STR_INT8) == 0)
                    {
                        iState = REG_PARSE_STATE_VALUE_INT8;    
                        DSM_MemSet(szBuff,0,REG_DATA_TYPE_INT32_NUM_LEN + LEN_FOR_NULL_CHAR);
                    }
                    else if(DSM_StrCaselessCmp(szBuff,REG_TYPE_STR_INT32) == 0)
                    {
                        iState = REG_PARSE_STATE_VALUE_INT32;                    
                        DSM_MemSet(szBuff,0,REG_DATA_TYPE_INT32_NUM_LEN + LEN_FOR_NULL_CHAR);
                    }
                    else
                    {
                        iRet = _ERR_REG_CFG_VALUE_TYPE_UNKNOW;
                        bLoop = FALSE;
                        break;    
                    }
                    iUnitSize = 0;
                    
                }              
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;                    
                }
                break;
            case REG_PARSE_STATE_VALUE_STR:
                if(iUnitSize > REG_VALUE_LEN_MAX)
                {
                    iRet = _ERR_REG_CFG_VALUE_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }
                if('\"' == *p)
                {
                    iState = REG_PARSE_STATE_FINISH;  
                    pLineInfo->uLine.sValueLine.u_data.szStr[iUnitSize] = '\0';
                    pLineInfo->uLine.sValueLine.iDSize = (UINT8)iUnitSize;
                    pLineInfo->uLine.sValueLine.iDType = REG_TYPE_STRING;                    
                }              
                else
                {
                    pLineInfo->uLine.sValueLine.u_data.szStr[iUnitSize] = *p;
                    iUnitSize ++;
                }
                break;
            case REG_PARSE_STATE_VALUE_HEX:                
                if(iUnitSize > REG_VALUE_LEN_MAX - 1)
                {
                    iRet = _ERR_REG_CFG_VALUE_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }
                if(n > 2)
                {
                    iRet = _ERR_REG_CFG_STR2HEX_WRONG;
                    bLoop = FALSE;
                    break;
                }
                
                if('\n' == *p)
                {
                    if(2 == n)
                    {
                        pLineInfo->uLine.sValueLine.u_data.szStr[iUnitSize] = (UINT8)((szBuff[0] << 4) | szBuff[1]);
                        iUnitSize ++;
                    }
                    if(0 >= iUnitSize)
                    {
                        iRet = _ERR_REG_CFG_STR2HEX_WRONG;
                        bLoop = FALSE;
                        break;
                    }                    
                    pLineInfo->uLine.sValueLine.iDSize = (UINT8)iUnitSize;
                    pLineInfo->uLine.sValueLine.iDType = REG_TYPE_BINARY;
                    iState = REG_PARSE_STATE_END;
                    // continue;
                }              
                else if(',' == *p || ' ' == *p)
                {                    
                    if(2 == n)
                    {
                        pLineInfo->uLine.sValueLine.u_data.szStr[iUnitSize] = (UINT8)((szBuff[0] << 4) | szBuff[1]);
                        iUnitSize ++;                        
                    }
                    n = 0;                    
                }
                else if('0' <= *p && '9' >= *p)
                {
                    szBuff[n] = (UINT8)(*p - '0');
                    n++;
                }
                else if('A' <= *p && 'F' >= *p)
                {
                    szBuff[n] = (UINT8)(*p - 'A' + 0x0A);
                    n++;
                }
                else if('a' <= *p && 'f' >= *p)
                {
                    szBuff[n] = (UINT8)(*p - 'a' + 0x0A);
                    n++;
                }
                break;
            case REG_PARSE_STATE_VALUE_INT8:
                if(iUnitSize > REG_DATA_TYPE_INT8_NUM_LEN)
                {
                    iRet = _ERR_REG_CFG_VALUE_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }
                if('\n' == *p)
                {
                    pLineInfo->uLine.sValueLine.u_data.iInt8 = (UINT8)DSM_StrAToI((INT8*)szBuff);
                    pLineInfo->uLine.sValueLine.iDType= REG_TYPE_INT8;
                    pLineInfo->uLine.sValueLine.iDSize = SIZEOF(INT8);
                    iState = REG_PARSE_STATE_END;
                   // continue;
                }              
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;
                }
                break;
            case REG_PARSE_STATE_VALUE_INT32:
                if(iUnitSize > REG_DATA_TYPE_INT32_NUM_LEN)
                {
                    iRet = _ERR_REG_CFG_VALUE_TOO_LONG;
                    bLoop = FALSE;
                    break;
                }
                if('\n' == *p)
                {
                    pLineInfo->uLine.sValueLine.u_data.iInt32 = DSM_StrAToI((INT8*)szBuff);
                    pLineInfo->uLine.sValueLine.iDType= REG_TYPE_INT32;
                    pLineInfo->uLine.sValueLine.iDSize = SIZEOF(INT32);
                    iState = REG_PARSE_STATE_END;              
                }              
                else
                {
                    szBuff[iUnitSize] = *p;
                    iUnitSize ++;
                }
                break;
            case REG_PARSE_STATE_FINISH:
                if('\n' == *p)
                {
                    iState = REG_PARSE_STATE_END;                  
                }              
                else
                {
                    iRet = _ERR_REG_CFG_ORDER_WRONG;
                    bLoop = FALSE;
                    break;
                }
                break;
            case REG_PARSE_STATE_END:
                iRet = _ERR_REG_SUCCESS;
                *piSize = p - pPos; 
                bLoop = FALSE;
                break;
            default:
                iRet = _ERR_REG_CFG_ORDER_WRONG;
                bLoop = FALSE;                
                break;
        }
        p++;
    }
    
    return iRet;
}



INT32 reg_GetHeaderInfo(VOID)
{
    INT32 iRet;
    UINT32 iReadLen;
    INT32 iOffset;
    
    iRet = ERR_SUCCESS;
    iOffset = 0;
    iReadLen = 0;
    
    iRet = REG_Read(iOffset,(UINT8*)&g_sHInfo,SIZEOF(REG_H_INFO),&iReadLen); 
    if(SIZEOF(REG_H_INFO) == iReadLen && iRet == ERR_SUCCESS)
    {
        g_sRKOffset[0].hKey = REG_HKEY_CURRENT_COMM;
        g_sRKOffset[0].iOffset = g_sHInfo.iOffset_RHCC;
        
        g_sRKOffset[1].hKey = REG_HKEY_CURRENT_USERS;
        g_sRKOffset[1].iOffset = g_sHInfo.iOffset_RHCU;
        
        g_sRKOffset[2].hKey = REG_HKEY_LOCAL_MACHINE;
        g_sRKOffset[2].iOffset = g_sHInfo.iOffset_RHLM;
        
        g_sRKOffset[3].hKey = REG_HKEY_SYSTEM_ROOT;
        g_sRKOffset[3].iOffset= g_sHInfo.iOffset_RHSR;
        reg_SortRK(g_sRKOffset);
    }
    else 
    {
        iRet = _ERR_REG_READMEDIA_FAILED;
    }  
    return iRet;
}


INT32 reg_CreateReg(VOID)
{
    INT32 iRet = ERR_SUCCESS;
    REG_R_INFO sRInfo;
    UINT32 iLen,iReadedLen,iWriteLen;                   
    INT32 iResult;
    UINT32 iOffset;
    UINT8* p = NULL;
    REG_DEF_LINE_INFO sLineInfo;
    REG_DEF_KEY_LINE sKeyInfo;
    UINT32 iLineSize = 0;

    

    //default headerinfo  
    g_sHInfo.iMagic = REG_MAGIC;
    g_sHInfo.iVer   =  REG_VERSION;
    g_sHInfo.iHOffset = 0;
    g_sHInfo.iRecSize = REG_R_LEN;
    g_sHInfo.iHSize = SIZEOF(REG_H_INFO);
    g_sHInfo.iRegSize = 0;
    g_sHInfo.iRecCount = 0;
    g_sHInfo.iOffset_RHCC = 0;
    g_sHInfo.iOffset_RHCU = 0;
    g_sHInfo.iOffset_RHLM = 0;
    g_sHInfo.iOffset_RHSR = 0;
    

    p = (UINT8*)default_registry;
    iOffset = SIZEOF(REG_H_INFO);
    iLen = DSM_StrLen((INT8*)p);
    iReadedLen = 0;
    iWriteLen = 0;
    
    if(NULL == p)
    {
        return ERR_REG_DEFAULT_DATA_ERROR;
    }      
    DSM_MemSet(&sKeyInfo,0,SIZEOF(REG_DEF_KEY_LINE));
    
    // read default cfg line and write to vds.
    while(iReadedLen < iLen)
    {    
        iLineSize = 0;
        DSM_MemSet(&sLineInfo,0,SIZEOF(REG_DEF_LINE_INFO));

        // Read a line.
        iResult = reg_GetLineInfo(p,&sLineInfo,&iLineSize);
        if(ERR_SUCCESS == iResult)
        {
            if(REG_DEF_STR_LINE_TYPE_KEY == sLineInfo.iType)
            {
                sKeyInfo = sLineInfo.uLine.sKeyLine;
            }
            else if(REG_DEF_STR_LINE_TYPE_VALUE == sLineInfo.iType)
            {
                if(0 == sKeyInfo.iRKey)
                {
                    D((DL_REGERROR, "RKey error. sKeyInfo.iRKey = 0."));
                    iRet = ERR_REG_DEFAULT_DATA_ERROR;      
                    goto _End_reg_CreateReg;
                }
                sRInfo.iCheckSum = REG_CHECKSUM;
                sRInfo.iRKey   = sKeyInfo.iRKey;
                sRInfo.iSKSize = sKeyInfo.iSKSize;
                DSM_StrCpy(sRInfo.szSK,sKeyInfo.szSK);
                
                sRInfo.iDSize  = sLineInfo.uLine.sValueLine.iDSize;
                sRInfo.iDType  = sLineInfo.uLine.sValueLine.iDType;                
                sRInfo.iVNSize = sLineInfo.uLine.sValueLine.iVNSize;                
                DSM_StrCpy(sRInfo.szVN,sLineInfo.uLine.sValueLine.szVN);
                DSM_MemCpy(sRInfo.u_data.szStr,&sLineInfo.uLine.sValueLine.u_data,SIZEOF(sLineInfo.uLine.sValueLine.u_data));                
                // Write to midium.
                if( (iOffset+REG_R_LEN) > (UINT32)g_RegGlobalVar.iMediaSize )
                {
                    D((DL_REGERROR,"in reg_CreateReg, iOffset > MediaSize : iOffset = 0x%x,MediaSize = 0x%x.",iOffset,g_RegGlobalVar.iMediaSize ));
                    iRet = ERR_REG_NO_SPACE;
                    goto _End_reg_CreateReg;
                }
               
                iResult =  REG_Write(iOffset, (UINT8*)(&sRInfo),REG_R_LEN,&iWriteLen);
                if(iWriteLen!= REG_R_LEN || ERR_SUCCESS != iResult)
                {
                    D((DL_REGERROR, "write reg recorde failed.iOffset = 0x%x,len = 0x%x,err_code = %d.",iOffset,REG_R_LEN,iResult));
                    iRet = ERR_REG_WRITEMEDIA_FAILED;
                    goto _End_reg_CreateReg;
                }
                else
                {
                    iOffset += REG_R_LEN;
                }    
                
                g_sHInfo.iRecCount++;         
                D((DL_REGDETAIL, " write a reg record,index = %d,the root key = %d,sub key = %s,value name = %s,value type = 0x%x.",
                                        g_sHInfo.iRecCount,
                                        sRInfo.iRKey,
                                        sRInfo.szSK,
                                        sRInfo.szVN,
                                        sRInfo.iDType));   
            }
            else
            {
                D((DL_REGERROR, "default cfg line type error. type = %d.",sLineInfo.iType));
                iRet = ERR_REG_DEFAULT_DATA_ERROR;
                goto _End_reg_CreateReg;
            }                
            iReadedLen += iLineSize;
            p += iLineSize;
        }
        else
        {
            D((DL_REGERROR, " get the line info failing. iResult = %d.",iResult));
            iRet = ERR_REG_DEFAULT_DATA_ERROR;
            goto _End_reg_CreateReg;
        }
    }
    
    // check the readed length.
    if(iReadedLen != iLen)
    {
        // readed length not equal to default cfg length.
        D((DL_REGERROR, " get the line info failing. read size = %d,default size = %d.",iReadedLen,iLen));
        iRet = ERR_REG_DEFAULT_DATA_ERROR;
        goto _End_reg_CreateReg;
        
    }
    else
    {
        // write the record midium end.
        // begin to write the header.
        g_sHInfo.iRegSize = ((g_sHInfo.iRecSize)*g_sHInfo.iRecCount + SIZEOF(REG_H_INFO)); 
        iOffset = 0;
        D((DL_REGWARNING, " Create reg,header info:")); 
        iResult = REG_Write(iOffset, (VOID*)&g_sHInfo, SIZEOF(REG_H_INFO),&iWriteLen);
        if(SIZEOF(REG_H_INFO) != iWriteLen || ERR_SUCCESS != iResult)
        {
            D((DL_REGERROR, "write reg header failed.iOffset = 0x%x,len = 0x%x,err_code = %d.",iOffset,SIZEOF(REG_H_INFO),iResult));
            iRet = ERR_REG_WRITEMEDIA_FAILED;
            goto _End_reg_CreateReg;
        }
        // write the header end.
    }
    
_End_reg_CreateReg:    
    return iRet;
}


VOID reg_SortRK(REG_RK_OFFSET pRKOffset[])
{
    REG_RK_OFFSET sRKOffset;
    INT32 i,j;   
    BOOL bEexchage;
    
    for(i = 0; i < REG_ROOTKEY_COUNT; i++)
    {
        bEexchage = FALSE;
        j = 0;
        for(j = REG_ROOTKEY_COUNT-1; j >= i; j--)
        {
            if((pRKOffset[j].iOffset) <= (pRKOffset[j-1].iOffset))
            {
                sRKOffset = pRKOffset[j];
                pRKOffset[j] = pRKOffset[j - 1];
                pRKOffset[j-1] = sRKOffset;   
                bEexchage = TRUE;
                // break;
            }            
        }
        if(!bEexchage)
            break;        
    }
}


INT32 reg_Init_Local(UINT8 iMode)
{
    INT32 iRet;
    BOOL bResult; 
    INT32 iResult;
    
    iRet = ERR_SUCCESS;
    
    switch(iMode)
    {
    case 0x01:
        reg_MediaInit();
        bResult = reg_Existed();
        if(FALSE == bResult)
        {        
            iRet = reg_CreateReg();           
        }
        else
        {
            iResult =  reg_GetHeaderInfo();
            if(ERR_SUCCESS != iResult)
            {        
                return ERR_REG_UNINIT;
            }
            else
            {
                if(g_sHInfo.iVer != REG_VERSION)    //Version now is not the same as the system's,delete the reg file,and recreate 
                {
                
                    iResult = reg_CreateReg();
                    CSW_TRACE(BASE_DSM_TS_ID, "create reg success.max record number= %d.",g_sHInfo.iRecCount);
                    if(ERR_SUCCESS != iResult)
                    {
                        iRet = ERR_REG_DEFAULT_DATA_ERROR;
                    }
                }
                else
                {
                      CSW_TRACE(BASE_DSM_TS_ID, "Reg has existed.record count = %d, reg size = %d.",g_sHInfo.iRecCount,g_sHInfo.iRegSize);
                }                
            }            
        }
        break;
        
    case 0x02:
        reg_MediaInit();
         iResult = reg_CreateReg(); 
         if(ERR_SUCCESS != iResult)
        {
            iRet = ERR_REG_DEFAULT_DATA_ERROR;
        }

        break;
        
    default:
        reg_MediaInit();
        break;
    }    
    g_RegGlobalVar.iHasInited = 1;
    return iRet;
}



INT32 reg_BuffToInt(UINT8* pIn,INT32* iOut,INT32 iNum)
{
    INT32	i;
    INT32 cMid[10];
    INT32 iMid = 0;
    for (i=0; i<iNum; i++) 
    {
        cMid[i] = pIn[iNum-i-1]<<(8*(iNum-i-1));
        iMid = iMid|(cMid[i]);
    }
    *iOut = iMid;
    return ERR_SUCCESS;
}



INT32 reg_IntToBuff(INT32 iIn, UINT8*pOut, INT32 iNum)
{
    INT32 i;
    UINT8 cMid[10];
    for(i=0; i<iNum; i++)
    {
        cMid[i] =  (UINT8)(iIn>>(8*i));
        pOut[i] = cMid [i];
    }
    return ERR_SUCCESS;
}

INT32 REG_Read(UINT32 ulAddrOffset,UINT8* pBuffer, UINT32 ulBytesToRead, UINT32* pBytesRead)
{
    INT32 uDevNo;
    UINT32 uRegBaseAddr = 0;

#ifdef _REG_SMS_MERGE
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);
    uRegBaseAddr = DSM_REG_ADDR;
#else 
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_REG);
    uRegBaseAddr = 0;
#endif    

    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }
    
    return DSM_Read(uDevNo,ulAddrOffset + uRegBaseAddr,pBuffer,ulBytesToRead,pBytesRead);
    
}

INT32 REG_Write(UINT32 ulAddrOffset,CONST UINT8* pBuffer,UINT32 ulBytesToWrite,UINT32*  pBytesWritten)
{
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;
    INT32 uDevNo;
    UINT32 uTimes = 0;   
    UINT32 uRegBaseAddr = 0;

#ifdef _REG_SMS_MERGE
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);
    uRegBaseAddr = DSM_REG_ADDR;
#else 
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_REG);
    uRegBaseAddr = 0;
#endif           
    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }
    do
    {
        iResult = DSM_Write(uDevNo,ulAddrOffset+ uRegBaseAddr,pBuffer,ulBytesToWrite,pBytesWritten);
        if(iResult != ERR_SUCCESS)
        {
            D((DL_REGERROR, "Reg_Write failed. err code = %d,times = %d.",iResult,uTimes));
            iRet = iResult;
        }
        else
        {
            iRet = ERR_SUCCESS;
            break;
        } 
        uTimes ++;

   }while(uTimes < 3);
   return iRet;
}

UINT32 reg_GetRegSize(VOID)
{    
    return  g_RegGlobalVar.iHasInited == 1 ? (((g_sHInfo.iRegSize+1023)/1024)*1024):g_RegGlobalVar.iMediaSize;
}


