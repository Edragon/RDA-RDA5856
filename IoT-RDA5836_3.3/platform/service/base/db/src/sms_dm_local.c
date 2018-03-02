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
#include "error_id.h"
#include "dsm_cf.h"
#include "sms_dm_cd.h"
#include "sms_dm_local.h"
#include "vds_api.h"
#include "dsm_dbg.h"


// Global Variable
UINT16  g_SMS_DM_InitFlag;                               // 是否初始化
HANDLE g_SMS_DM_Sem;                                      // 短信息数据库访问信号量


INT32 SMS_DM_VersionCompare(INT8 *pResult)
{
    INT32 iRet;
    SMS_DM_H_INFO sHInfo;

    iRet = SMS_DM_ReadHeadInfo(&sHInfo);
    if(iRet != ERR_SUCCESS)
    {
        return iRet;
    }

    if(SMS_DM_VERSION == sHInfo.iVer)
    {
        *pResult = 0;
    }
    else if(SMS_DM_VERSION > sHInfo.iVer)
    {
        *pResult = 1;
    }
    else
    {
        *pResult = -1;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_RSTableInit(VOID)
{
    UINT16 iRecordStatus[SMS_DM_NR_RST_GROUP] = {0,};
    UINT16 i;
    UINT32 ulWriteSize;
    INT32 iRet;

    DSM_MemSet((PVOID)iRecordStatus, 0x00, SMS_DM_NR_RST_GROUP * SIZEOF(iRecordStatus[0]));
    for (i = 0; i < SMS_DM_MAX_REC_COUNT / SMS_DM_NR_RST_GROUP; ++i )
    {
        iRet = DM_Write(SMS_DM_RST_ADDR + i * SMS_DM_NR_RST_GROUP \
                        * SMS_DM_SZ_RECORD_STATUS, (CONST UINT8 *)iRecordStatus,
                        SMS_DM_NR_RST_GROUP * SMS_DM_SZ_RECORD_STATUS, &ulWriteSize);
        if (iRet != ERR_SUCCESS || ulWriteSize != SMS_DM_NR_RST_GROUP * SMS_DM_SZ_RECORD_STATUS)
        {
            return ERR_SMS_DM_MEDIA_WRITE_FAILED;
        }
    }

    if (SMS_DM_MAX_REC_COUNT % SMS_DM_NR_RST_GROUP)
    {
        iRet = DM_Write(SMS_DM_RST_ADDR + (SMS_DM_MAX_REC_COUNT / SMS_DM_NR_RST_GROUP) \
                        * SMS_DM_NR_RST_GROUP * SMS_DM_SZ_RECORD_STATUS, (CONST UINT8 *)iRecordStatus,
                        ((SMS_DM_MAX_REC_COUNT % SMS_DM_NR_RST_GROUP) * SMS_DM_SZ_RECORD_STATUS), &ulWriteSize);
        if (iRet != ERR_SUCCESS
                || ulWriteSize != (UINT32)((SMS_DM_MAX_REC_COUNT % SMS_DM_NR_RST_GROUP) * SMS_DM_SZ_RECORD_STATUS))
        {
            return ERR_SMS_DM_MEDIA_WRITE_FAILED;
        }
    }

    return ERR_SUCCESS;

}


INT32 SMS_DM_Create(VOID)
{
    INT32 iRet;
    SMS_DM_H_INFO sHInfo;
    SMS_DM_H_INFO sHInfoRead;
    UINT32 uDevNo;
    UINT32 uNrBlock = 0;
    UINT32 uBlockSize = 0;
    UINT32 uSmsBaseAddr = 0;

    // Check the device space.
#ifdef _REG_SMS_MERGE

    uSmsBaseAddr = DSM_SMS_DM_ADDR;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);

#else
    uSmsBaseAddr = 0;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_SMS);
#endif
    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        return ERR_SMS_DM_NOT_REGISTER;
    }

    iRet = DRV_GET_DEV_INFO(uDevNo, &uNrBlock, &uBlockSize);
    if(ERR_SUCCESS != iRet)
    {
        return ERR_SMS_DM_NOT_REGISTER;
    }

    if((uNrBlock * uBlockSize - uSmsBaseAddr) < SMS_DM_SZ_MEDIA_USE)
    {
        return ERR_SMS_DM_NOT_ENOUGH_MEDIA_SPACE;
    }


    DSM_MemSet(&sHInfo, 0, SMS_DM_SZ_H_INFO);
    sHInfo.iHSize = SMS_DM_SZ_H_INFO;
    sHInfo.iVer = SMS_DM_VERSION;
    sHInfo.iMagic = SMS_DM_MAGIC;

    iRet = SMS_DM_WriteHeadInfo((VOID *) &sHInfo);
    ERR_RETURN(iRet != ERR_SUCCESS, ERR_SMS_DM_MEDIA_WRITE_FAILED, NULL);

    iRet = SMS_DM_ReadHeadInfo((VOID *)&sHInfoRead);
    ERR_RETURN(iRet != ERR_SUCCESS, ERR_SMS_DM_MEDIA_READ_FAILED, NULL);

    if (SMS_DM_SZ_H_INFO != sHInfoRead.iHSize || SMS_DM_VERSION != sHInfoRead.iVer
            || SMS_DM_MAGIC != sHInfo.iMagic)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    iRet = SMS_DM_RSTableInit();
    ERR_RETURN(iRet != ERR_SUCCESS, iRet, NULL);

    iRet = SMS_DM_ClearLog();
    ERR_RETURN(iRet != ERR_SUCCESS, iRet, NULL);

    return ERR_SUCCESS;

}



INT32 SMS_DM_SetRecordStatus(UINT16 iIndex, UINT16 iStatus )
{
    UINT32 ulOffset;
    UINT32 ulWriteSize;
    UINT16 iReadStatus;
    INT32 iRet;

    if (iIndex >= SMS_DM_MAX_REC_COUNT)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    ulOffset = iIndex * SMS_DM_SZ_RECORD_STATUS;
    iRet = DM_Write((SMS_DM_RST_ADDR + ulOffset), (CONST UINT8 *)&iStatus, SMS_DM_SZ_RECORD_STATUS, &ulWriteSize);
    ERR_RETURN(iRet != ERR_SUCCESS || ulWriteSize != SMS_DM_SZ_RECORD_STATUS, iRet, NULL);

    iRet = SMS_DM_GetRecordStatus(iIndex, &iReadStatus);
    ERR_RETURN(iRet != ERR_SUCCESS, iRet, NULL);

    if (iReadStatus != iStatus)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    return ERR_SUCCESS;

}



INT32 SMS_DM_GetRecordStatus(UINT16 iIndex, UINT16 *pStatus)
{
    UINT32 ulOffset;
    UINT32 ulReadSize;
    INT32 iRet;

    if (iIndex >= SMS_DM_MAX_REC_COUNT)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    ulOffset = iIndex * SMS_DM_SZ_RECORD_STATUS;

    iRet = DM_Read(SMS_DM_RST_ADDR + ulOffset, (UINT8 *)pStatus, SMS_DM_SZ_RECORD_STATUS, &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SMS_DM_SZ_RECORD_STATUS)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    return ERR_SUCCESS;

}



INT32 SMS_DM_GetDatadStatus(UINT16 iIndex, UINT8 *pStatus)
{
    UINT32 ulRecordOffset;
    UINT32 ulReadSize;
    INT32 iRet;

    if (iIndex >= SMS_DM_MAX_REC_COUNT)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    ulRecordOffset = iIndex * SMS_DM_SZ_RECORD_DATA;

    iRet = DM_Read(( SMS_DM_RECORD_DATA_START_ADDR + ulRecordOffset + SMS_DM_DATA_STATUS_POS),
                   pStatus, SMS_DM_SZ_DATA_STATUS, &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SMS_DM_SZ_DATA_STATUS)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_ReadRecordData(UINT16 iIndex, PVOID pData)
{
    UINT32 ulRecordOffset;
    UINT32 ulReadSize;
    INT32 iRet;

    if (iIndex >= SMS_DM_MAX_REC_COUNT || NULL == pData)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    ulRecordOffset = iIndex * SMS_DM_SZ_RECORD_DATA;

    iRet = DM_Read(SMS_DM_RECORD_DATA_START_ADDR + ulRecordOffset,
                   (UINT8 *)pData, SMS_DM_SZ_RECORD_DATA, &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != (UINT32)SMS_DM_SZ_RECORD_DATA)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_WriteRecordData(UINT16 iIndex, PVOID pData)
{
    UINT32 ulRecordOffset;
    UINT32 ulWriteSize;
    INT32 iRet;

    if (iIndex >= SMS_DM_MAX_REC_COUNT || NULL == pData)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    ulRecordOffset = iIndex * SMS_DM_SZ_RECORD_DATA;

    iRet = DM_Write(SMS_DM_RECORD_DATA_START_ADDR + ulRecordOffset,
                    (CONST UINT8 *)pData, SMS_DM_SZ_RECORD_DATA, &ulWriteSize);
    if (iRet != ERR_SUCCESS || ulWriteSize != (UINT32)SMS_DM_SZ_RECORD_DATA)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_ReadHeadInfo(SMS_DM_H_INFO *psHeadInfo)
{
    UINT32 ulReadSize;
    INT32 iRet;

    if (NULL == psHeadInfo)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    iRet = DM_Read(SMS_DM_BASE_ADDR, (UINT8 *)psHeadInfo,
                   SIZEOF(SMS_DM_H_INFO), &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SIZEOF(SMS_DM_H_INFO))
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_WriteHeadInfo(SMS_DM_H_INFO *psHeadInfo)
{
    UINT32 ulWriteSize;
    INT32 iRet;

    if (NULL == psHeadInfo)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    iRet = DM_Write(SMS_DM_BASE_ADDR, (CONST UINT8 *)psHeadInfo,
                    SIZEOF(SMS_DM_H_INFO), &ulWriteSize);

    ERR_RETURN(iRet != ERR_SUCCESS || ulWriteSize != SIZEOF(SMS_DM_H_INFO), ERR_SMS_DM_MEDIA_WRITE_FAILED, NULL);

    return ERR_SUCCESS;

}



BOOL SMS_DM_Existed()
{
    UINT32 magic = 0;
    SMS_DM_H_INFO sHeadInfo;
    INT32 iRet;

    iRet = SMS_DM_ReadHeadInfo(&sHeadInfo);
    if (iRet != ERR_SUCCESS)
    {
        return FALSE;
    }

    magic = sHeadInfo.iMagic;

    if(SMS_DM_MAGIC == magic)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


//------------------------------------------------------------------------------------
//description: this function output the start index and end index for searching loop by input SIM ID.
//-------------------------------------------------------------------------------------
//author:  Hameina   || date: 2009.1.22
//-------------------------------------------------------------------------------------
//parameter:		nSimId		[in] 	 SIM ID
//				nStartIndex	[out] output start index to loop
//				nEndIndex	[out] output end index to loop
//-----------------------------------------------------------------------------------
//return value:	success		ERR_SUCCESS
//				fail			ERR_SMS_DM_INVALID_PARAMETER
//-----------------------------------------------------------------------------------
INT32 SMS_DM_GetRange(
    CFW_SIM_ID nSimId,
    UINT16 *nStartIndex,
    UINT16 *nEndIndex
)
{
	#ifdef PHONE_SMS_STORAGE_SPACE_COMBINATE    
	*nStartIndex = (UINT16)0;
	*nEndIndex = (UINT16)SMS_DM_MAX_REC_COUNT;
	#else
	*nStartIndex = (nSimId-CFW_SIM_0)*(SMS_DM_MAX_REC_COUNT/CFW_SIM_COUNT);
	*nEndIndex = (nSimId-CFW_SIM_0+1)*(SMS_DM_MAX_REC_COUNT/CFW_SIM_COUNT);
	#endif
    return ERR_SUCCESS;
}


INT32 SMS_DM_GetFreeIndex(
    UINT16 *pIndex
    , CFW_SIM_ID nSimId
)
{
    UINT16 iRecordStatus[SMS_DM_NR_RST_GROUP] = {0,};
    UINT16 iIndex;
    UINT16 iIndexOfGroup;
    UINT32 ulReadSize;
    INT32 iRet;
    //[[Hameina[+] 2009.1.22
    UINT16 iStartId = 0;
    UINT16 iEndId = 0;
    //]]Hameina[+]
    if (NULL == pIndex)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    //[[Hameina [mod] 2009.1.22
    iRet = SMS_DM_GetRange(nSimId, &iStartId, &iEndId) ;
    if(ERR_SUCCESS != iRet)
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }
    for ( iIndex = iStartId, iIndexOfGroup = 0; iIndex < iEndId; iIndex++ )
        //for ( iIndex = 0, iIndexOfGroup = 0; iIndex < SMS_DM_MAX_REC_COUNT; iIndex++ )
        //]]Hameina [mod]
    {
        if ( 0 == iIndexOfGroup )
        {
            iRet = DM_Read(SMS_DM_RST_ADDR + iIndex * SMS_DM_SZ_RECORD_STATUS, (UINT8 *)iRecordStatus,
                           SMS_DM_NR_RST_GROUP * SMS_DM_SZ_RECORD_STATUS, &ulReadSize);
            if ( iRet != ERR_SUCCESS || ulReadSize != SMS_DM_NR_RST_GROUP * SMS_DM_SZ_RECORD_STATUS)
            {
                return ERR_SMS_DM_MEDIA_READ_FAILED;
            }
        }

        if ( SMS_DM_STATUS_REC_FREE == iRecordStatus[iIndexOfGroup])
        {
            *pIndex = iIndex;
            return ERR_SUCCESS;
        }

        iIndexOfGroup++;
        iIndexOfGroup %= SMS_DM_NR_RST_GROUP;
    }

    return ERR_SMS_DM_SPACE_FULL;
}



INT32 SMS_DM_MakeLog(UINT16 iIndex, UINT16 iStatus, PVOID pData)
{
    SMS_DM_LOG sLog;
    SMS_DM_LOG sLogRead;
    UINT32 ulLogAddr = SMS_DM_LOG_STRAT_ADDR;
    UINT32 ulWriteSize, ulReadSize;
    INT32 iRet;

  #ifdef PHONE_SMS_STORAGE_SPACE_COMBINATE    
  UINT16 iActStatus = iStatus & 0x00ff;
  
  if (iIndex >= SMS_DM_MAX_REC_COUNT 
      || (iActStatus != SMS_DM_STATUS_REC_FREE && iActStatus != SMS_DM_STATUS_REC_VALID)) 
  #else	
  if (iIndex >= SMS_DM_MAX_REC_COUNT 
      || (iStatus != SMS_DM_STATUS_REC_FREE && iStatus != SMS_DM_STATUS_REC_VALID))
  #endif
    {
        return ERR_SMS_DM_INVALID_PARAMETER;
    }

    sLog.iLogFlag = SMS_DM_FG_LOG_STILL;
    sLog.iRecordIndex = iIndex;
    sLog.iRecordNewStatus = iStatus;

    iRet = DM_Write(ulLogAddr, (CONST UINT8 *)&sLog, SIZEOF(SMS_DM_LOG), &ulWriteSize);
    if (ERR_SUCCESS != iRet || ulWriteSize != SIZEOF(SMS_DM_LOG))
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    iRet = DM_Read(ulLogAddr, (UINT8 *)&sLogRead, SIZEOF(SMS_DM_LOG), &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SIZEOF(SMS_DM_LOG))
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    if (sLogRead.iLogFlag != sLog.iLogFlag || sLogRead.iRecordIndex != sLog.iRecordIndex
            || sLogRead.iRecordNewStatus != sLog.iRecordNewStatus)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    if (pData != NULL)
    {
        iRet = DM_Write(ulLogAddr + SMS_DM_LOG_DATA_POS, (CONST UINT8 *)pData, SMS_DM_SZ_RECORD_DATA, &ulWriteSize);
        if (ERR_SUCCESS != iRet || ulWriteSize != (UINT32)SMS_DM_SZ_RECORD_DATA)
        {
            return ERR_SMS_DM_MEDIA_WRITE_FAILED;
        }
    }

    sLog.iLogFlag = SMS_DM_FG_LOG_ACTIVA;
    iRet = DM_Write(ulLogAddr + SMS_DM_LOG_FLAG_POS, (CONST UINT8 *)&sLog, SMS_DM_SZ_LOG_FLAG, &ulWriteSize);
    if (ERR_SUCCESS != iRet || ulWriteSize != SMS_DM_SZ_LOG_FLAG)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    iRet = DM_Read(ulLogAddr, (UINT8 *)&sLogRead, SIZEOF(SMS_DM_LOG), &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SIZEOF(SMS_DM_LOG))
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    if (sLogRead.iLogFlag != sLog.iLogFlag || sLogRead.iRecordIndex != sLog.iRecordIndex
            || sLogRead.iRecordNewStatus != sLog.iRecordNewStatus)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    return ERR_SUCCESS;
}



INT32 SMS_DM_ClearLog(VOID)
{
    UINT16 iLogFlag = SMS_DM_FG_LOG_STILL;
    UINT32 ulWriteSize, ulReadSize;
    INT32 iRet;

    iRet = DM_Write(SMS_DM_LOG_STRAT_ADDR + SMS_DM_LOG_FLAG_POS, (CONST UINT8 *)&iLogFlag,
                    SMS_DM_SZ_LOG_FLAG, &ulWriteSize);
    if (ERR_SUCCESS != iRet || ulWriteSize != SMS_DM_SZ_LOG_FLAG)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    iRet = DM_Read(SMS_DM_LOG_STRAT_ADDR + SMS_DM_LOG_FLAG_POS, (UINT8 *)&iLogFlag, SMS_DM_SZ_LOG_FLAG, &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SMS_DM_SZ_LOG_FLAG)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    if (iLogFlag != SMS_DM_FG_LOG_STILL)
    {
        return ERR_SMS_DM_MEDIA_WRITE_FAILED;
    }

    return ERR_SUCCESS;

}



INT32 SMS_DM_IsLogActive(BOOL *pResult)
{
    UINT16 iLogFlag = SMS_DM_FG_LOG_STILL;
    UINT32 ulReadSize;
    INT32 iRet;

    iRet = DM_Read(SMS_DM_LOG_STRAT_ADDR + SMS_DM_LOG_FLAG_POS, (UINT8 *)&iLogFlag, SMS_DM_SZ_LOG_FLAG, &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SMS_DM_SZ_LOG_FLAG)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    if (SMS_DM_FG_LOG_ACTIVA == iLogFlag)
    {
        *pResult = TRUE;
    }
    else
    {
        *pResult = FALSE;
    }

    return ERR_SUCCESS;

}



INT32 SMS_DM_LogRecove(VOID)
{
    SMS_DM_LOG sLog;
    PVOID pDataBuffer = NULL;
    UINT32 ulReadSize;
    INT32 iRet;

    iRet = DM_Read(SMS_DM_LOG_STRAT_ADDR, (UINT8 *)&sLog, SIZEOF(SMS_DM_LOG), &ulReadSize);
    if (iRet != ERR_SUCCESS || ulReadSize != SIZEOF(SMS_DM_LOG))
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    ERR_RETURN(sLog.iLogFlag != SMS_DM_FG_LOG_ACTIVA, ERR_SMS_DM_ERROR, "log flag does not active.");
    ERR_RETURN(sLog.iRecordIndex >= SMS_DM_MAX_REC_COUNT, ERR_SMS_DM_ERROR, "log's index does invalid.");

    if (SMS_DM_STATUS_REC_FREE == sLog.iRecordNewStatus)
    {
        iRet = SMS_DM_SetRecordStatus(sLog.iRecordIndex, (UINT16)SMS_DM_STATUS_REC_FREE);
        ERR_GOTO(iRet != ERR_SUCCESS, iRet, step0_failed, "SMS_DM_SetRecordStatus() return failed.");
    }
  #ifdef PHONE_SMS_STORAGE_SPACE_COMBINATE    
  else if (SMS_DM_STATUS_REC_VALID == (sLog.iRecordNewStatus & 0x00ff))
  #else
  else if (SMS_DM_STATUS_REC_VALID == sLog.iRecordNewStatus)
  #endif	
  {
  #ifdef PHONE_SMS_STORAGE_SPACE_COMBINATE    
    iRet = SMS_DM_SetRecordStatus(sLog.iRecordIndex, (UINT16)((sLog.iRecordNewStatus & 0xff00)|SMS_DM_STATUS_REC_VALID));
  #else
    iRet = SMS_DM_SetRecordStatus(sLog.iRecordIndex, (UINT16)SMS_DM_STATUS_REC_VALID);
  #endif
        ERR_GOTO(iRet != ERR_SUCCESS, iRet, step0_failed, "SMS_DM_SetRecordStatus() return failed.");

        pDataBuffer = (PVOID)DSM_MAlloc(SMS_DM_SZ_RECORD_DATA);
        if(NULL == pDataBuffer)
        {
            DSM_ASSERT(0, "SMS_DM_LogRecove: 1.malloc(0x%x) failed.", SMS_DM_SZ_RECORD_DATA);
        }
        ERR_GOTO(NULL == pDataBuffer, ERR_SMS_DM_NOT_ENOUGH_MEMORY, step0_failed, "alloc memory failed.");

        DSM_MemSet(pDataBuffer, 0x00, SMS_DM_SZ_RECORD_DATA);

        iRet = DM_Read(SMS_DM_LOG_STRAT_ADDR + SMS_DM_LOG_DATA_POS, (UINT8 *)pDataBuffer, SMS_DM_SZ_RECORD_DATA, &ulReadSize);
        ERR_GOTO(iRet != ERR_SUCCESS || ulReadSize != (UINT32)SMS_DM_SZ_RECORD_DATA,
                 ERR_SMS_DM_MEDIA_READ_FAILED, step1_failed, "DM_Read() failed.");

        iRet = SMS_DM_WriteRecordData(sLog.iRecordIndex, pDataBuffer);
        ERR_GOTO(iRet != ERR_SUCCESS, iRet, step1_failed, "SMS_DM_WriteRecordData() return failed.");
    }
    else
    {
        return ERR_SMS_DM_ERROR;
    }

    iRet = SMS_DM_ClearLog();

step1_failed:
    DSM_Free(pDataBuffer);

step0_failed:
    return iRet;
}



INT32 DM_Read(UINT32 ulAddrOffset, UINT8 *pBuffer, UINT32 ulBytesToRead, UINT32 *pBytesRead)
{
    INT32 uDevNo;
    UINT32 uSmsBaseAddr = 0;

#ifdef _REG_SMS_MERGE
    uSmsBaseAddr = DSM_SMS_DM_ADDR;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);
#else
    uSmsBaseAddr = 0;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_SMS);
#endif
    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }

    return DSM_Read(uDevNo, ulAddrOffset + uSmsBaseAddr, pBuffer, ulBytesToRead, pBytesRead);

}

INT32 DM_Write(UINT32 ulAddrOffset, CONST UINT8 *pBuffer, UINT32 ulBytesToWrite, UINT32  *pBytesWritten)
{
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;
    INT32 uDevNo;
    UINT32 uTimes = 0;
    UINT32 uSmsBaseAddr = 0;

#ifdef _REG_SMS_MERGE
    uSmsBaseAddr = DSM_SMS_DM_ADDR;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_CSW);
#else
    uSmsBaseAddr = 0;
    uDevNo = DSM_GetDevNrOnModuleId(DSM_MODULE_SMS);
#endif
    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        return ERR_SMS_DM_MEDIA_READ_FAILED;
    }
    do
    {
        iResult = DSM_Write(uDevNo, ulAddrOffset + uSmsBaseAddr, pBuffer, ulBytesToWrite, pBytesWritten);
        if(iResult != ERR_SUCCESS)
        {
            D((DL_SMSDMERROR, "DSM_Write failed. err code = %d,times = %d.\n", iResult, uTimes));
            iRet = iResult;
        }
        else
        {
            iRet = ERR_SUCCESS;
            break;
        }
        uTimes ++;
    }
    while(uTimes < 3);

    return iRet;
}



