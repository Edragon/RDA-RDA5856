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
#include "csw_cfg_default.h"
#include "error_id.h"
#include "vds_api.h"
#include "reg.h"
#include "reg_local.h"
#include "dsm_dbg.h"

// Global variable.
extern const char *default_registry ;           // The default cfg string. it stroraged the value of csw module 
                                                                   // registry variable. define in csw_cfg_default.h.
struct REG_Global_Var g_RegGlobalVar = {0,0,0};                    // The medial information and semaphor handle.

REG_H_INFO g_sHInfo = {0,0,0,0,0,0,0,0,0,0,0,{0,}};                            // The reg header information.
REG_RK_OFFSET g_sRKOffset[REG_ROOTKEY_COUNT];   // 
REG_RSKD      g_sRSK[REG_OPENKEY_MAX];

/***************************************************************/
// Function: This function to get the default value for a key entry.The default values is 
//               leave factory values,it is in resource code.
// Parameter: 
// hRootKey: [in]The root key handle,as follows:
//                     REG_HKEY_SYSTEM_ROOT        
//                     REG_HKEY_LOCAL_MACHINE     
//                     REG_HKEY_CURRENT_COMM     
//                     REG_HKEY_CURRENT_USERS 
//  pcSubKey:[in]The sub-key name.
//  pcValueName:[in]Pointer to a string containing the name of the value to query. 
//                This parameter can not be NULL or a pointer to an empty string. 
//  pcType:[out] Pointer to a variable that receives the type of data associated with the specified value. 
//                The following table shows the possible values of the type. Value Description 
//                    REG_TYPE_BINARY Specifies a binary data in any form 
//                    REG_TYPE_INT32 Specifies a 32-bit number 
//                    REG_TYPE_INT8 Specifies a 8-bit number 
//                    REG_TYPE_STRING Specifies a null-terminated string 
//                    The pcType parameter can be NULL if the type is not required
//  pvData:[out] Pointer to a buffer that receives the value's data. This parameter can be NULL if the data is not required. 
//  pDataSize[in,out]Pointer to a variable that specifies the size, in bytes. This parameter can be NULL only if pvData is NULL. 
//                If pvData is NULL, and pDataSize non-NULL, the function returns ERR_SUCCESS, and stores the size of the data,
//                in bytes, in the variable pointed to by pcDataSize.   
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//                If the function fails, the return value as follows:
//                ERR_REG_INVALID_PARAMETER: parameter is invalid.
//                ERR_REG_INVALID_HANDLE: hRootKey is invalid.       
//		          ERR_REG_DEFAULT_DATA_ERROR: the data is invalid in the buffer of default_registry.
//                ERR_REG_NO_ENTRY: Can't find the value name 
//                ERR_REG_NO_KEY: Can't fine the sub key.
/**************************************************************/
INT32 REG_QueryDefaultValue(
                            HANDLE hRootKey,
                            PCSTR pcSubKey,
                            PCSTR pcValueName,
                            UINT8 *pcType,
                            PVOID pvData,
                            UINT32 *pDataSize
                            )
{
    INT32 iRet = ERR_SUCCESS;    
    UINT32 iDefLen,iReadedLen;                   
    INT32 iResult;
    const UINT8* p = (const UINT8*)default_registry; 
    REG_DEF_LINE_INFO sLineInfo;
    REG_DEF_KEY_LINE sKeyInfo;
    UINT32 iLineSize = 0; 
    UINT32 iRecordNum = 0;
   
    reg_Down(); 
    // Check input parameter
    if((NULL == pcSubKey)    ||
        (NULL == pcValueName) ||
        (NULL == pcType)      ||
        (NULL == pDataSize))
    {
        D((DL_REGERROR, "REG_QueryDefaultValue(): parameter error.pcSubkey = 0x%x,pcValueName = 0x%x,pcType= 0x%x,pDataSize = 0x%x.",
            pcSubKey,
            pcValueName,
            pcType,
            pDataSize));          
        
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _query_default_value_end;
    }
    
    if((REG_HKEY_SYSTEM_ROOT != hRootKey)   &&
        (REG_HKEY_LOCAL_MACHINE != hRootKey) &&
        (REG_HKEY_CURRENT_COMM != hRootKey)  &&
        (REG_HKEY_CURRENT_USERS != hRootKey))
    {
        D((DL_REGERROR,
            "in REG_QueryDefaultValue() root key error,return!hRootKey= 0x%x.",
            hRootKey
            ));
        
        iRet = ERR_REG_INVALID_HANDLE;
        goto _query_default_value_end;
    }    
    // Check input parameter end.
    iDefLen = DSM_StrLen(p);
    iReadedLen = 0;
    DSM_MemSet(&sKeyInfo,0,SIZEOF(REG_DEF_KEY_LINE));
    // read default cfg line and write to vds.
    while(iReadedLen < iDefLen)
    {    
        iLineSize = 0;
        DSM_MemSet(&sLineInfo,0,SIZEOF(REG_DEF_LINE_INFO));
    
        iResult = reg_GetLineInfo(p,&sLineInfo,&iLineSize); // Read a line.
        if(ERR_SUCCESS == iResult)
        {
            if(REG_DEF_STR_LINE_TYPE_KEY == sLineInfo.iType)
            {
                sKeyInfo = sLineInfo.uLine.sKeyLine;
            }
            else if(REG_DEF_STR_LINE_TYPE_VALUE == sLineInfo.iType)
            {
                if((sKeyInfo.iRKey == hRootKey) && 
                   (DSM_StrCaselessCmp(sKeyInfo.szSK,pcSubKey) == 0))                   
                {
                    
                    if(DSM_StrCaselessCmp(sLineInfo.uLine.sValueLine.szVN,pcValueName) == 0)
                    {
                        
                        if((NULL != pvData) && (0 != *pDataSize))
                        {                   
                            if(REG_TYPE_BINARY == sLineInfo.uLine.sValueLine.iDType)
                            {                                                    
                                 *pDataSize = sLineInfo.uLine.sValueLine.iDSize;
                                  DSM_MemCpy(pvData,sLineInfo.uLine.sValueLine.u_data.szStr,*pDataSize);
                            }
                            else if(REG_TYPE_INT32 == sLineInfo.uLine.sValueLine.iDType)
                            {                                                         
                                *pDataSize = sLineInfo.uLine.sValueLine.iDSize;  
                                *((INT32*)pvData) = sLineInfo.uLine.sValueLine.u_data.iInt32;
                            }
                            else if(REG_TYPE_INT8 == sLineInfo.uLine.sValueLine.iDType)
                            {
                                *pDataSize = sLineInfo.uLine.sValueLine.iDSize;
                                *((INT8*)pvData) = sLineInfo.uLine.sValueLine.u_data.iInt8;
                            }
                            else if(REG_TYPE_STRING== sLineInfo.uLine.sValueLine.iDType)
                            {
                                *pDataSize = sLineInfo.uLine.sValueLine.iDSize;
                                DSM_StrNCpy(pvData,sLineInfo.uLine.sValueLine.u_data.szStr,*pDataSize);
                            }
                            else
                            {
                                D((DL_REGERROR,
                                    "REG_QueryDefaultValue(): default cfg string data type error,data type  = 0x%x,record number= %d.",
                                    sLineInfo.uLine.sValueLine.iDType,
                                    iRecordNum
                                    ));
                                iRet = ERR_REG_DEFAULT_DATA_ERROR;
                                break;
                            }
                            iRet = ERR_SUCCESS;
                            goto _query_default_value_end;
                        }
                        else
                        {  
                            *pDataSize = sLineInfo.uLine.sValueLine.iDSize;
                            iRet = ERR_SUCCESS;
                            goto _query_default_value_end;
                        }
                    }      
                    else
                    {
                        // do noting.
                        // get next line continue.
                    }
                }
                else
                {
                    // do noting.
                    // get next line continue.
                }
                iRecordNum++;  
            }
            else
            {
                D((DL_REGWARNING, "REG_QueryDefaultValue(): default cfg line type wrong. type = %d.",sLineInfo.iType));
                iRet = ERR_REG_DEFAULT_DATA_ERROR;
                break;
            }
                
            iReadedLen += iLineSize;
            p += iLineSize;
        }
        else
        {
            D((DL_REGWARNING, "REG_QueryDefaultValue(): get line info failing. iResult = %d.",iResult));
            iRet = ERR_REG_DEFAULT_DATA_ERROR;
            break;
        }
    }
       
    D((DL_REGWARNING, "REG_QueryDefaultValue(): can't find the line. read size = %d,default size = %d,iRecordNum = %d.",
           iReadedLen,
           iDefLen,
           iRecordNum));
    
    iRet = ERR_REG_DEFAULT_DATA_ERROR;
   
_query_default_value_end:
    reg_Up();
    if(iRet != ERR_SUCCESS)
    {
        D((DL_REGERROR, "REG_QueryDefaultValue():failed. pcSubkey = %s,pcValueName = %s,*pcType= 0x%x,*pDataSize = 0x%x.return %d.",
            pcSubKey,
            pcValueName,
            *pcType,
            *pDataSize,
            iRet)); 
    }
    return iRet;
}


/***************************************************************/
// Function: This function is Open a sub key,return to caller the offset of the subkey first entry.
// Parameter: 
// hKey: [in]The root key handle,as follows:
//                     REG_HKEY_SYSTEM_ROOT        
//                     REG_HKEY_LOCAL_MACHINE     
//                     REG_HKEY_CURRENT_COMM     
//                     REG_HKEY_CURRENT_USERS 
//  pcSubKey:[in]The sub-key name.
//  phKey:[out]Output the offset of the subkey first entry.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//                If the function fails, the return value as follows:   
//		          ERR_REG_NO_KEY: can't find the sub key in the reg. 
/**************************************************************/
INT32  REG_OpenKey(
                   HANDLE hKey,
                   PCSTR pcSubKey,
                   HANDLE* phKey
                   )
{
    INT32 iRet;  
    INT32 iIndex;
    
    reg_Down();    
    // Check the input parameter.
    if((REG_HKEY_SYSTEM_ROOT != hKey)   &&
        (REG_HKEY_LOCAL_MACHINE != hKey) &&
        (REG_HKEY_CURRENT_COMM != hKey)  &&
        (REG_HKEY_CURRENT_USERS != hKey))
    {
        reg_Up();
        return ERR_REG_INVALID_TYPE;   
    }
    if(NULL == pcSubKey || NULL == phKey)
    {
        reg_Up();
        return ERR_REG_INVALID_PARAMETER;
    }
    if(0 == pcSubKey[0])
    {
        reg_Up();
        return ERR_REG_INVALID_PARAMETER;
    }
    iRet = ERR_SUCCESS;  
    iIndex = reg_GetFreeRSK();
    if(-1==iIndex)
    {
        reg_Up();
        return ERR_REG_NO_SPACE;
    }
    iRet = reg_SetRSK(hKey,pcSubKey,iIndex,phKey);
    if(_ERR_REG_OVER_POSITION == iRet)
    {
        reg_Up();
        return ERR_REG_INVALID_PARAMETER;
    }
    reg_Up();
    return iRet;
}


/***************************************************************/
// Function: The REG_SetValue function sets the data and type of a specified value under a registry key.
// Parameter: 
// hKey 
//      [in] Handle to an opened key, which is returned by REG_OpenKey function.
// pcValueName 
//      [in] Pointer to a string containing the name of the value to set. If a value with this name is not already present in the key, the function adds it to the key. 
//      The maximum size of a value name is 16characters. 
// iType 
//      [in] Type of information to be stored as the value's data. The following table shows the possible values for iType. Value Description 
//      REG_TYPE_BINARY Specifies a binary data in any form 
//      REG_TYPE_INT32 Specifies a 32-bit number 
//      REG_TYPE_INT8 Specifies a 8-bit number 
//      REG_TYPE_STRING Specifies a null-terminated string 
// pvData
//      [in] Pointer to a buffer containing the data to be stored with the specified value name. 
// iDataSize 
//      [in] Size, in bytes, of the information pointed to by the pvData parameter.The maximum size of data is 64bytes.
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//      If the function fails, the return value as follows:   
//	       ERR_REG_INVALID_HANDLE: The input hKey is not exist. 
//          ERR_REG_INVALID_PARAMETER: The input parameter pcValueName or pvData is invalid such as a null pointer. 
//          ERR_REG_INVALID_TYPE: The input parameter cType is invalid. 
//          ERR_REG_NO_ENTRY: The value is not found.
//          ERR_REG_MEDIA_READ_FAIL:  Suddenness occurs when the storage media is being read.. 
//          ERR_REG_MEDIA_WRITE_FAIL: Suddenness occurs the moment storage media is being written.. 
/**************************************************************/
INT32  REG_SetValue(
                    HANDLE hKey,
                    PCSTR pcValueName,
                    UINT8 iType,
                    PVOID pvData,
                    UINT8 iDataSize
                    )
{
    INT32 iRet = ERR_SUCCESS;
    INT32 iOffset,iRecOffset;
    UINT32 iWriteLen, iReadLen;
    REG_R_INFO sRInfo;
    REG_R_CTR sRCtr;
    
    reg_Down();  
    iOffset = 0;
    iRecOffset = 0;
    iWriteLen = 0;
    iReadLen = 0;
    DSM_MemZero(&sRInfo,SIZEOF(REG_R_INFO));
    DSM_MemZero(&sRCtr,SIZEOF(REG_R_CTR));
    
    // D((DL_DETAIL, "IN REG_SetValue, hKey = %d\n",hKey));
    // D((DL_DETAIL, "IN REG_SetValue, pcValueName = %s\n",pcValueName));
    // D((DL_DETAIL, "IN REG_SetValue, iType = %d\n",iType));
    
    // Check the input parameter.
    if((hKey<0)||(hKey>REG_OPENKEY_MAX))
    {
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_SetValue;
    }
    
    if ((NULL == pcValueName) ||
        (NULL == pvData)      ||
        (0 == iDataSize))
    {
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_SetValue;       
    }
    
    if ((REG_TYPE_INT8 != iType)   &&
        (REG_TYPE_INT32 != iType)  &&
        (REG_TYPE_STRING != iType) &&
        (REG_TYPE_BINARY != iType))
    {
        iRet = ERR_REG_INVALID_TYPE;
        goto _End_REG_SetValue;
    }     
    if (0 == *pcValueName)
    {
        D((DL_REGERROR, "reg_FindValueName ERROR,pcValueName is NULL."));
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_SetValue;
    }
    iRet = ERR_SUCCESS;
    DSM_MemSet(&sRCtr, 0, REG_R_CTR_LEN);
    iOffset = reg_FindValueName(g_sRSK[hKey].iRKey,g_sRSK[hKey].szSK,&sRCtr, pcValueName, iRecOffset);
    if (0 > iOffset)
    {
        D((DL_REGERROR, "reg_FindValueName ERROR,errorcode = %d.",iOffset));
        iRet = ERR_REG_FIND_NAME_WRONG;
        goto _End_REG_SetValue;
    }    
    if (iType != sRCtr.iDType)
    {
        D((DL_REGERROR, "reg_FindValueName ERROR,iType = %d.",iType));
        D((DL_REGERROR, "reg_FindValueName ERROR,sRCtr.iDType = %d.",sRCtr.iDType));
        iRet = ERR_REG_INVALID_TYPE;
        goto _End_REG_SetValue;
    }
    iRet = REG_Read(iOffset, (UINT8*)(&sRInfo), REG_R_LEN,&iReadLen);
    if (REG_R_LEN != iReadLen || iRet != ERR_SUCCESS)
    { 
        D((DL_REGERROR, "read media error,iReadLent = %d.",iReadLen));
        iRet = ERR_REG_READMEDIA_FAILED;        
        goto _End_REG_SetValue;
    }
    if(
        (sRInfo.iDType != iType)||
        (sRInfo.iRKey != g_sRSK[hKey].iRKey)||
        (DSM_StrLen(sRInfo.szSK) != DSM_StrLen(g_sRSK[hKey].szSK))||
        (0 != DSM_StrCmp(sRInfo.szSK,g_sRSK[hKey].szSK))||
        (DSM_StrLen(sRInfo.szVN) != DSM_StrLen(pcValueName))||
        (0 != DSM_StrCmp(sRInfo.szVN,pcValueName))
        )
    {
        
        D((DL_REGERROR, "reg_buff2rinfo error."));
        iRet = ERR_REG_FIND_NAME_WRONG;            //2006-9-29
        goto _End_REG_SetValue;
    }
    if ( REG_TYPE_INT8 == iType )
    {
        sRInfo.iDSize = 1;
    }
    else if ( REG_TYPE_INT32 == iType )
    {
        sRInfo.iDSize = 4;
    }
    else if ( REG_TYPE_STRING == iType )
    {
        if (REG_VALUE_LEN_MAX > iDataSize)
        {
            if ( NULL_CHAR != *(((char*)pvData) + iDataSize - LEN_FOR_NULL_CHAR) )
            {
                iDataSize++;
            }
            sRInfo.iDSize = iDataSize;
        }
        else
        {
            D((DL_REGERROR, "data error,iDataSize = %d.",iDataSize));
            
            iRet = ERR_REG_MORE_DATA;
            goto _End_REG_SetValue;
        }
    }
    else if ( REG_TYPE_BINARY == iType )
    {
        if (REG_VALUE_LEN_MAX >= iDataSize)
        {
            sRInfo.iDSize = iDataSize;
        }
        else
        {
            D((DL_REGERROR, " error2."));
            iRet = ERR_REG_MORE_DATA;
            goto _End_REG_SetValue;
        }
    }
    DSM_MemCpy(&(sRInfo.u_data), pvData, sRInfo.iDSize);
    iRet = REG_Write(iOffset,(UINT8*)(&sRInfo), REG_R_LEN,&iWriteLen);
    if (REG_R_LEN != iWriteLen || iRet != ERR_SUCCESS)
    {
        D((DL_REGERROR, "1write media error,iWriteLen = %d,iRet = 0x%x.",iWriteLen,iRet));
        iRet = ERR_REG_WRITEMEDIA_FAILED;        
        goto _End_REG_SetValue;
    }     
_End_REG_SetValue:
    reg_Up();
    return iRet;
}


/***************************************************************/
// Function: The REG_GetValue function retrieves the type and data for a specified value name associated with an open registry key. 
// Parameter: 
// hKey 
//      [in] Handle to an opened key, which is returned by REG_OpenKey.
// pcValueName 
//      [in] Pointer to a string containing the name of the value to query. This parameter can not be NULL or a pointer to an empty string. 
//      The maximum size of a value name is 16characters. 
// pcType 
//      [out] Pointer to a variable that receives the type of data associated with the specified value. The following table shows the possible values of the type. Value Description 
//      REG_TYPE_BINARY Specifies a binary data in any form 
//      REG_TYPE_INT32 Specifies a 32-bit number 
//      REG_TYPE_INT8 Specifies a 8-bit number 
//      REG_TYPE_STRING Specifies a null-terminated string 
//      The pcType parameter can be NULL if the type is not required. 
// pvData 
//      [out] Pointer to a buffer that receives the value's data. This parameter can not be NULL. 
// pDataSize 
//      [in][out] Pointer to a variable that specifies the size of the buffer, in bytes. If buffer is enough, the function stores the size of the data, in bytes, in the variable pointed to by pcDataSize.   
// Return Values
//      If the function succeeds, the return value is ERR_SUCCESS.
//      If the function fails, the return value as follows:
//      ERR_REG_INVALID_HANDLE: The input hKey is not exist. 
//      ERR_REG_INVALID_PARAMETER: The input parameter pcValueName is invalid such as a null pointer or a pointer to an empty string. 
//      ERR_REG_READMEDIA_FAILED:  Suddenness occurs when the storage media is being read.. 
//      ERR_REG_DATASIZE_TOOSHORT: The buf what use output data of value is too short.
/**************************************************************/
INT32 REG_GetValue(
                   HANDLE hKey,
                   PCSTR pcValueName,
                   UINT8 *pcType,
                   PVOID pvData,
                   UINT8 *pDataSize
                   )
{
    INT32 iRet = _ERR_REG_SUCCESS;
    INT32 iOffset = 0;
    INT32 iRecOffset =0;
    UINT32 iReadLen = 0;
    REG_R_CTR sRCtr;
    
    
    reg_Down();
    D((DL_DETAIL, "IN REG_GetValue, hKey = %d.",hKey));
    D((DL_DETAIL, "IN REG_GetValue, pcValueName = %s.",pcValueName));
    if((hKey<0)||(hKey>REG_OPENKEY_MAX))
    {
        D((DL_REGERROR, ("in REG_GetValue , hKey = %d."),hKey));
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_GetValue;
    }   
    if (!pcValueName || !pvData || !pDataSize)
    {
        D((DL_REGERROR, ("in REG_GetValue , parameter1 error.")));
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_GetValue;
    }
    if (0 == *pcValueName)
    {
        D((DL_REGERROR, ("in REG_GetValue , parameter2 error.")));
        iRet = ERR_REG_INVALID_PARAMETER;
        goto _End_REG_GetValue;
    }
    
    DSM_MemSet(&sRCtr, 0, REG_R_CTR_LEN);
    iOffset = reg_FindValueName(g_sRSK[hKey].iRKey,g_sRSK[hKey].szSK,&sRCtr,pcValueName,iRecOffset);
    if (0 > iOffset)
    {
        D((DL_REGERROR, "in REG_GetValue , err code = %d,iRecOffet = %d.",iOffset,iRecOffset));
        iRet = ERR_REG_FIND_NAME_WRONG;
        goto _End_REG_GetValue;
    }
    
    *pcType = sRCtr.iDType;
    D((DL_REGDETAIL, "DataSize = %d.",*pDataSize));
    D((DL_REGDETAIL, "DataSize = %d.",sRCtr.iDSize));
    if (*pDataSize < sRCtr.iDSize)
    {
        D((DL_REGERROR, "1read media error,*pDataSize = %d, sRCtr.iDSize = %d.",*pDataSize,sRCtr.iDSize));        
        iRet = ERR_REG_DATASIZE_TOOSHORT;
        goto _End_REG_GetValue;
    }    
    else                                                                                                        
    {                     
        // if parameter *pDataSize is lager than the lenth of  the data in flash,               
        // the parameter returned will become the true lenth of the data
        *pDataSize = sRCtr.iDSize;
    }
    
    iRet = REG_Read(iOffset+REG_R_CTR_LEN,pvData,sRCtr.iDSize,&iReadLen);
    if (sRCtr.iDSize != iReadLen || iRet != ERR_SUCCESS)
    {
        D((DL_REGERROR, "1read media error,sRCtr.iDSize = %d,iReadLen = %d,iRet = 0x%x.",sRCtr.iDSize,iReadLen,iRet));
        iRet = ERR_REG_READMEDIA_FAILED;
        goto _End_REG_GetValue;
    }
    
    
_End_REG_GetValue:        
    
    reg_Up();  
    return iRet;
    
}


/***************************************************************/
// Function: The REG_CloseKey function releases the handle of the specified key .
// Any handle to an open key need to be closed if you opened.
// Parameter: 
// hKey 
//      [in] Pointer to the handle to the opened key to close. The handle must have been opened by the REG_OpenKey or REG_CreateKey function. 
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//      If the function fails, error code is returned.
/**************************************************************/
INT32 REG_CloseKey(HANDLE hKey)
{
    INT32 iRet;
    
    reg_Down();
    if((hKey>REG_OPENKEY_MAX)||(hKey<0))
    {
        D((DL_REGERROR, "in REG_CloseKey , hKey = %d.",hKey));
        reg_Up();
        return ERR_REG_INVALID_HANDLE;
    }
    
    iRet= ERR_SUCCESS;
    iRet = reg_FreeRSK(hKey);
    if(iRet!=ERR_SUCCESS)
    {
        D((DL_REGERROR, "in REG_CloseKey , iRet = %d.",iRet));
        iRet = ERR_REG_NO_KEY  ;
    }
    reg_Up();
    return iRet;
}

/***************************************************************/
// Function: This function is initalize the reg,if the reg file 
//           is not existed,will create reg file and setup the the
//           root key,the sub key, the value name and values, else do nothing.
// Parameter: 
// None
//     
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//      If the function fails,  the return value as follows:
//      ERR_REG_WRITEMEDIA_FAILED:  write the falsh error.
//      ERR_REG_MEDIA_OPEN_FAIL:    open the reg file error.
//      ERR_REG_DEFAULT_DATA_ERROR: the default data error. 
/**************************************************************/
INT32 REG_Init(VOID)
{
    
    INT32 iRet = ERR_SUCCESS;
    iRet = reg_Init_Local(0x01);
    return iRet;
}


/***************************************************************/
// Function: This function setup the default value to the reg.
// Parameter: 
// None
// Return value: If the function succeeds, the return value is ERR_SUCCESS.
//      If the function fails,  the return value as follows:
//      ERR_REG_WRITEMEDIA_FAILED:  write the falsh error.
//      ERR_REG_MEDIA_OPEN_FAIL:    open the reg file error.
//      ERR_REG_DEFAULT_DATA_ERROR: the default data error. 
/**************************************************************/
INT32 REG_SetDefault(VOID)
{    
    return reg_Init_Local(0x02);
}


/**************************************************************************************/
// Function: This function get the  version of reg module.
// Parameter: 
// None.
// Return value: return the version of reg module..
/***************************************************************************************/
UINT32 REG_GetVersion(VOID)
{
    return REG_VERSION;
}


/**************************************************************************************/
// Function: This function get the  size of reg module.
// Parameter: 
// None.
// Return value: return the size in byte .
/***************************************************************************************/
UINT32 REG_GetRegSize(VOID)
{
    return reg_GetRegSize();
}


