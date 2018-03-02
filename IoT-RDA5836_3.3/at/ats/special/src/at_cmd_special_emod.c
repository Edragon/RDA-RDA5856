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

/*********************************************************
 *
 * File Name
 *	?
 * Author
 * 	?
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

/**************************************************************************************************
 *                                           INCLUDES
 **************************************************************************************************/
#if defined(__AT_MOD_SPECIAL__)
// #include <hal.h>
#include "at_common.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_emod.h"
#include "at_cfg.h"
#if (AUDIO_CALIB_VER == 1)
#include "calibaud_m.h"
//#ifdef SYS_VPP //will remove later
#include "vpp_speech_bt.h"
#include "vpp_sbc_dec.h"
#include "vpp_audiojpeg_dec.h"

#include "vois_m.h"
//#include "aud_codec_common.h"
//#endif
#include "sxr_mem.h"
#include "cfw.h"
#include "snd_types.h"
#include "aud_m.h"
#include "tgt_calib_m.h"
#include "hal_host.h"
#include "string.h"


#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
extern uint8 AudioOverlayBuf[400*2];
#endif 

#endif

#define CFG_SENDDATALEN 	512

#define	 ERROR_RETURN(pResult,reason,nDLCI) do{ pResult = AT_CreateRC(CMD_FUNC_FAIL,\
        CMD_RC_ERROR, \
        reason,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        NULL,\
        0,\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult	= NULL;	\
    }\
    return;\
}while(0)
#define	 OK_RETURN(pResult,pString,nDLCI) do{	pResult	= AT_CreateRC(	CMD_FUNC_SUCC,\
        CMD_RC_OK, \
        CMD_ERROR_CODE_OK,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        pString,\
        strlen(pString),\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult	= NULL;	\
    }\
    return;\
}while(0)

#if (AUDIO_CALIB_VER == 1)

#define  ERROR_CALIB_RETURN(pResult,reason,nDLCI) do{ pResult = AT_CreateRC(CMD_FUNC_FAIL,\
        CMD_RC_ERROR, \
        reason,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        NULL,\
        0,\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult = NULL; \
        }\
        goto _exit; \
}while(0)
#define  OK_CALIB_RETURN(pResult,pString,nDLCI) do{ pResult = AT_CreateRC(  CMD_FUNC_SUCC,\
        CMD_RC_OK, \
        CMD_ERROR_CODE_OK,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        pString,\
        strlen(pString),\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult = NULL; \
        }\
        goto _exit;  \
}while(0)

#define  OK_CALIB_RESET_RETURN(pResult,pString,nDLCI) do{   pResult = AT_CreateRC(  CMD_FUNC_SUCC,\
        CMD_RC_OK, \
        CMD_ERROR_CODE_OK,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        pString,\
        strlen(pString),\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult = NULL; \
        }\
}while(0)

#define  ERROR_CALIB_RESET_RETURN(pResult,reason,nDLCI) do{ pResult = AT_CreateRC(CMD_FUNC_FAIL,\
        CMD_RC_ERROR, \
        reason,\
        CMD_ERROR_CODE_TYPE_CME,\
        0,\
        NULL,\
        0,\
        nDLCI);\
    AT_Notify2ATM(pResult,nDLCI);\
    if(pResult != NULL){ \
        AT_FREE(pResult);\
        pResult = NULL; \
        }\
}while(0)


extern AUD_ITF_T        audioItf;
extern SND_ITF_T        musicItf;

#endif

INT32 EX_EmodeClearSMSInfo()
{
	return 1;
}

INT32 EX_EmodeClearREGInfo()
{
	return 1;
}

INT32 EX_EmodeClearPBKInfo()
{
	return 1;
}

INT32 EX_EmodeClearFFSInfo()
{
	return 1;
}


//CFW_TSM_FUNCTION_SELECT *g_pSelecFUN_t = NULL;

//PRIVATE BOOL EX_EmodeOFTStartSerCell(UINT8 nFreq);
//PRIVATE BOOL EX_EmodeOFTStopSerCell(VOID);
//PRIVATE BOOL EX_EmodeOFTStartNerCell(UINT8 nFreq);
//PRIVATE BOOL EX_EmodeOFTStopNeiCell(VOID);

#ifndef MMI_WITH_AT
BOOL g_auto_calib = FALSE;
#endif
// AT_20071107_CAOW_B
extern UINT16 CSW_ATCcGsmBcdToAscii(UINT8 *pBcd,  // input
                                    UINT8 nBcdLen, UINT8 *pNumber // output
                                   );
extern UINT16 SMS_PDUAsciiToBCD(UINT8 *pNumber, UINT16 nNumberLen, UINT8 *pBCD);

extern VOID CSW_TC_MEMBLOCK(UINT16 uMask, UINT8 *buffer, UINT16 len, UINT8 radix);

extern PUBLIC VOID BTSco_SetMsdfReloadFlag(VOID);
extern PUBLIC VOIS_ERR_T BTSco_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg);
extern PUBLIC VOIS_AUDIO_CFG_T* BTSco_GetAudioCfg(VOID);

extern PUBLIC AUD_LEVEL_T* aud_CodecCommonGetLatestLevel(VOID);

// extern UINT32 CFW_SimGetICCID(UINT16 nUTI);

#if 0

VOID AT_Emod_AsyncEventProcess(COS_EVENT *pEvent)
{
	CFW_EVENT CFWev;

	CFW_EVENT *pcfwEv = &CFWev;
	PAT_CMD_RESULT pResult = NULL;

	// UINT8 tLen;
	// UINT8 i = 0;
	UINT8 res[100];
	UINT8 *pSimID   = NULL;
	UINT8 uAsciiLen = 0;
	UINT8 SIMID[22];

#ifdef AT_DUAL_SIM
	UINT8 nSim;
#endif
	AT_CosEvent2CfwEvent(pEvent, pcfwEv);
#ifdef AT_DUAL_SIM
	nSim = pcfwEv->nFlag;
#endif
	switch (pcfwEv->nEventId)
	{
#if 1
	case EV_CFW_SIM_GET_ICCID_RSP:
		AT_TC(g_sw_SPEC, "%%%%%%%%%%%%%%%%%%%%%%%%");

		if (pcfwEv->nType == 0)
		{
			AT_MemZero(SIMID, 22);
			AT_MemZero(res, 100);
			AT_Sprintf(res, "+SCID: SIM Card ID: ");  // added by yangtt an 08-05-19 for bug 8507
			pSimID    = (UINT8 *)(pcfwEv->nParam1);
			uAsciiLen = CSW_ATCcGsmBcdToAscii(pSimID, 10, SIMID);

			/*
			   for(i=0;i<uAsciiLen;i++)
			   {
			   if(i%2)
			   AT_TC(g_sw_SPEC, "BCD:0x%x\r",pSimID[i/2]);
			   //(UINT8*)(pcfwEv->nParam1)++;
			   AT_TC(g_sw_SPEC, "ASCII:%d\r",SIMID[i]);
			   //pSimID++;

			   //AT_Sprintf(res+tLen,"%u ",*pSimID);
			   //tLen=AT_StrLen(res);
			   }
			 */
			AT_StrCat(res, SIMID);
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pcfwEv->nUTI);
		}
		else
		{
			/*
			   TODO: 该处改动是为了解决CFW的一个bug，即它不能识别SIM Not Inserted这种情况。等CFW解决了这个问题后
			   要去掉下面的补丁代码。
			   * */
#  if 1
			// PATCH CODE
			// AT_20071123_CAOW_B   for bug#7088, but whether it's reasonable? wt's situation about the rsp "ERR_CME_SIM_UNKNOW"?
			if (pcfwEv->nParam1 == ERR_CME_SIM_UNKNOW)
				pcfwEv->nParam1 = ERR_CME_SIM_NOT_INSERTED;

			// AT_20071123_CAOW_E
#endif

			AT_EX_FAIL_CreateRC(pResult,
			                    pcfwEv->nParam1 - ERR_CME_PHONE_FAILURE, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pcfwEv->nUTI);

		}

		break;
#endif
	default:
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pcfwEv->nUTI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}

// AT_20071107_CAOW_E

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeOFTStartSerCell(UINT8 nFreq)
{
	BOOL result;

	result = COS_SetTimer(CSW_AT_TASK_HANDLE, AT_EX_MONI_TIM_ID, COS_TIMER_MODE_PERIODIC, (UINT32)(nFreq * 16384));
	return result;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeOFTStopSerCell(VOID)
{
	BOOL result;

	result = COS_KillTimer(CSW_AT_TASK_HANDLE, AT_EX_MONI_TIM_ID);
	return result;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeOFTStartNerCell(UINT8 nFreq)
{
	BOOL result;

	result = COS_SetTimer(CSW_AT_TASK_HANDLE, AT_EX_MONP_TIM_ID, COS_TIMER_MODE_PERIODIC, (UINT32)(nFreq * 16384));
	return result;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeOFTStopNeiCell(VOID)
{
	BOOL result;

	result = COS_KillTimer(CSW_AT_TASK_HANDLE, AT_EX_MONP_TIM_ID);
	return result;
}

#if 0

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeClearPBKInfo(void)
{
	/*
		UINT8 i;
		BOOL result = FALSE;
		UINT8 nNmb = 0;
		UINT32 addr = 0;
		DRV_FLASH_DEV_INFO *pParm = NULL;
		pParm = (DRV_FLASH_DEV_INFO*)AT_MALLOC(sizeof(DRV_FLASH_DEV_INFO));

		if (pParm == NULL)
			return FALSE;

		UINT32 ret = DRV_FlashRegionMap(DRV_FLASH_REGION_PBK, pParm);

		if (ret != ERR_SUCCESS)
		{
			AT_FREE(pParm);
			return FALSE;
		}

		nNmb = (pParm->total_size) / (pParm->sector_size);

		addr = (UINT32)pParm->pBaseAddr;

		for (i = 0;i < nNmb;i++)
		{
			//result= DRV_EraseFlash(addr);
			addr += pParm->sector_size;

			if (result == HAL_FLASH_OK)
				result = TRUE;
			else {
				result = FALSE;
				AT_FREE(pParm);
				return result;
			}
		}

		AT_FREE(pParm);
	*/
	return TRUE;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeClearSMSInfo(void)
{
	UINT8 i;
	BOOL result = FALSE;
	UINT8 nNmb  = 0;
	UINT32 addr = 0;

	DRV_FLASH_DEV_INFO *pParm = NULL;
	pParm = (DRV_FLASH_DEV_INFO *)AT_MALLOC(sizeof(DRV_FLASH_DEV_INFO));

	if (pParm == NULL)
		return FALSE;

	UINT32 ret = DRV_FlashRegionMap(DRV_FLASH_REGION_SMS, pParm);

	if (ret != ERR_SUCCESS)
	{
		AT_FREE(pParm);
		return FALSE;
	}

	nNmb = (pParm->total_size) / (pParm->sector_size);

	addr = (UINT32)pParm->pBaseAddr;

	for (i = 0; i < nNmb; i++)
	{
		// result= DRV_EraseFlash(addr);
		addr += pParm->sector_size;

		if (result == HAL_FLASH_OK)
			result = TRUE;
		else
		{
			result = FALSE;
			AT_FREE(pParm);
			return result;
		}
	}

	return TRUE;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeClearFFSInfo(void)
{
	UINT8 i;
	BOOL result = FALSE;
	UINT8 nNmb  = 0;
	UINT32 addr = 0;

	DRV_FLASH_DEV_INFO *pParm = NULL;
	pParm = (DRV_FLASH_DEV_INFO *)AT_MALLOC(sizeof(DRV_FLASH_DEV_INFO));

	if (pParm == NULL)
		return FALSE;

	UINT32 ret = DRV_FlashRegionMap(DRV_FLASH_REGION_FFS, pParm);

	if (ret != ERR_SUCCESS)
	{
		AT_FREE(pParm);
		return FALSE;
	}

	nNmb = (pParm->total_size) / (pParm->sector_size);

	addr = (UINT32)pParm->pBaseAddr;

	for (i = 0; i < nNmb; i++)
	{
		// result= DRV_EraseFlash(addr);
		addr += pParm->sector_size;

		if (result == HAL_FLASH_OK)
			result = TRUE;
		else
		{
			result = FALSE;
			AT_FREE(pParm);
			return result;
		}
	}

	return TRUE;
}

/*********************************************************************
 * @fn
 * @brief
 * @param
 * @return
 */
PRIVATE BOOL EX_EmodeClearREGInfo(void)
{
	UINT8 i;
	bool result = FALSE;
	UINT8 nNmb  = 0;
	UINT32 addr = 0;

	DRV_FLASH_DEV_INFO *pParm = NULL;
	pParm = (DRV_FLASH_DEV_INFO *)AT_MALLOC(sizeof(DRV_FLASH_DEV_INFO));

	if (pParm == NULL)
		return FALSE;

	UINT32 ret = DRV_FlashRegionMap(DRV_FLASH_REGION_REG, pParm);

	if (ret != ERR_SUCCESS)
	{
		AT_FREE(pParm);
		return FALSE;
	}

	nNmb = (pParm->total_size) / (pParm->sector_size);

	addr = (UINT32)pParm->pBaseAddr;

	for (i = 0; i < nNmb; i++)
	{
		// result= DRV_EraseFlash(addr);
		addr += pParm->sector_size;

		if (result == HAL_FLASH_OK)
			result = TRUE;
		else
		{
			result = FALSE;
			AT_FREE(pParm);
			return result;
		}
	}

	AT_FREE(pParm);

	return TRUE;
}
#endif
/**************************************************************************************************
**************************************************************************************************/

/*----------------------------------------PRIVATE FUNCTION----------------------------------------*/

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_MONI(AT_CMD_PARA *pParam)
{
	INT32 eResult = 0;
	UINT8 iParaCount;
	UINT8 nParaLen         = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 tnMONIPara;
	UINT8 res[100];

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif
	AT_TC(g_sw_SPEC, "MONI CmdType : %d\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_SET: // AT^MONI=<period>  1-127
		// check parameter count
		eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

		if ((eResult == ERR_SUCCESS) && (1 == iParaCount))
		{
			// Get parameter
			nParaLen = 1;
			eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &tnMONIPara, &nParaLen);

			// Get para success

			if (eResult == ERR_SUCCESS)
			{
				// check para valid or invalid
				if ((127 < tnMONIPara) || (1 > tnMONIPara))
				{
					// para invalid,Create error result
					eResult = ERR_INVALID_PARAMETER;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					// para valid,Create success result
					gATCurrentu8nMONIPara = tnMONIPara;
					AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);
				}
			}
			else
			{
				// Get parameter error,Create error result
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}

		}
		else
		{
			// Get parameter count error,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	case AT_CMD_READ:  // AT+MONI?
		// copy result info & Create success result
		AT_MemZero(res, 100);
		AT_Sprintf(res, "^MONI:%u", gATCurrentu8nMONIPara);
		AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		break;

	case AT_CMD_EXE: // AT+MONI

		if (TRUE == EX_EmodeOFTStartSerCell(gATCurrentu8nMONIPara))
		{
			AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);  // Return ok
		}
		else
		{
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);  // Return fall
		}

		break;

	case AT_CMD_TEST:  // AT+MONI=?
		// copy result info & Create success result
		AT_MemZero(res, 100);
		AT_Sprintf(res, "^MONI:1-127");
		AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		break;

	default: // error Type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "----------MONI Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/

VOID AT_EX_TimerFunc_MONI(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT16 eUTI            = pParam->nDLCI;
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	AT_TC(g_sw_SPEC, "AABB------------------------- MONI TIMER FUNCTION -------------------");

	// Get free UTI
	eResult = AT_GetFreeUTI(CFW_EMOD_SRV_ID, (UINT8 *)&eUTI);

	if (eResult == ERR_SUCCESS)
	{
		g_pSelecFUN_t->nServingCell  = 1;
		g_pSelecFUN_t->nNeighborCell = 0;

		// call csw function to execute this cmd

		if (ERR_SUCCESS == CFW_EmodOutfieldTestStart(g_pSelecFUN_t, eUTI, nSim))
		{
			// Create success result nead wait asynchronism
			AT_EX_SUCC_ASYN_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}
	}
	else
	{
		// Get TTI fall,Create error result
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_MONIEND(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif
	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^MONIEND
		eResult = (UINT32)EX_EmodeOFTStopSerCell();

		if (eResult)
		{
			AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);  // Return ok
		}
		else
		{
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EX_TimerFunc_MONIEND(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT16 eUTI            = pParam->nDLCI;
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	// Get free UTI
	eResult = AT_GetFreeUTI(CFW_EMOD_SRV_ID, (UINT8 *)&eUTI);

	if (eResult == ERR_SUCCESS)
	{
		// call csw function to execute this cmd
		if (ERR_SUCCESS == CFW_EmodOutfieldTestEnd(eUTI, nSim))
		{
			// Create success result nead wait asynchronism
			AT_EX_SUCC_ASYN_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}
	}
	else
	{
		// Get TTI fall,Create error result
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_MONP(AT_CMD_PARA *pParam)
{
	INT32 eResult = 0;
	UINT8 iParaCount;
	UINT8 nParaLen         = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 tnMONPPara;
	UINT8 res[100];

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	switch (pParam->iType)
	{

	case AT_CMD_SET: // AT^MONP=<period>  1-127
		// check parameter count
		eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

		if ((eResult == ERR_SUCCESS) && (1 == iParaCount))
		{
			// Get parameter
			nParaLen = 1;
			eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &tnMONPPara, &nParaLen);

			// Get para success

			if (eResult == ERR_SUCCESS)
			{
				// check para valid or invalid
				if ((127 < tnMONPPara) || (1 > tnMONPPara))
				{
					// para invalid,Create error result
					eResult = ERR_INVALID_PARAMETER;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					// para valid,Create success result
					gATCurrentu8nMONPPara = tnMONPPara;
					AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);
				}
			}
			else
			{
				// Get parameter error,Create error result
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}

		}
		else
		{
			// Get parameter count error,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	case AT_CMD_EXE: // AT^MONP

		if (TRUE == EX_EmodeOFTStartNerCell(gATCurrentu8nMONPPara))
		{
			AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);  // Return ok
		}
		else
		{
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);  // Return fall
		}

		break;

	case AT_CMD_READ:  // AT^MONP?
		// copy result info & Create success result
		AT_MemZero(res, 100);
		AT_Sprintf(res, "^MONP: %u", gATCurrentu8nMONPPara);
		AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		break;

	case AT_CMD_TEST:  // AT^MONP=?
		// copy result info & Create success result
		AT_MemZero(res, 100);
		AT_Sprintf(res, "^MONP: 1-127");
		AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		break;

	default: // error Type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EX_TimerFunc_MONP(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT16 eUTI            = pParam->nDLCI;
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	// Get free UTI
	eResult = AT_GetFreeUTI(CFW_EMOD_SRV_ID, (UINT8 *)&eUTI);

	if (eResult == ERR_SUCCESS)
	{
		g_pSelecFUN_t->nServingCell  = 0;
		g_pSelecFUN_t->nNeighborCell = 1;

		// call csw function to execute this cmd

		if (ERR_SUCCESS == CFW_EmodOutfieldTestStart(g_pSelecFUN_t, eUTI, nSim))
		{
			// Create success result nead wait asynchronism
			AT_EX_SUCC_ASYN_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}
	}
	else
	{
		// Get TTI fall,Create error result
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_MONPEND(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^MONPEND
		eResult = (UINT32)EX_EmodeOFTStopNeiCell();

		if (eResult)
		{
			AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);  // Return ok
		}
		else
		{
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);  // Return fall
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EX_TimerFunc_MONPEND(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT16 eUTI            = pParam->nDLCI;
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	// Get free UTI
	eResult = AT_GetFreeUTI(CFW_EMOD_SRV_ID, (UINT8 *)&eUTI);

	if (eResult == ERR_SUCCESS)
	{
		// call csw function to execute this cmd
		if (ERR_SUCCESS == CFW_EmodOutfieldTestEnd(eUTI, nSim))
		{
			// Create success result nead wait asynchronism
			AT_EX_SUCC_ASYN_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}
	}
	else
	{
		// Get TTI fall,Create error result
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
}

/**************************************************************************************************
* @fn
*
* @brief
*
* @param
*
* @return
**************************************************************************************************/
// this command it the same as CIMI

VOID AT_EMOD_CmdFunc_SCID(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	AT_TC(g_sw_SPEC, "AABB----------SCID CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{
		// AT_20071107_CAOW_B

	case AT_CMD_EXE: // AT^SCID?
		// Get free UTI

		// call csw function to execute this cmd
		eResult = CFW_SimGetICCID(pParam->nDLCI, nSim);

		AT_TC(g_sw_SPEC, "AABB----------SCID eResult: %d----------\n", eResult);
		if (eResult == ERR_SUCCESS)
		{
			// Create success result need wait asynchronism
			AT_EX_SUCC_ASYN_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);
		}
		else
		{
			AT_TC(g_sw_SPEC, "AABB----55555------SCID CmdType : %d----------\n", pParam->iType);  // good trace  for bug 8527
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error Type
		// CMD type error, Create error result
		eResult = ERR_AT_CME_PARAM_INVALID;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
	AT_TC(g_sw_SPEC, "AABB----------SCID Cmd END------------\n");
	return;

	// AT_20071107_CAOW_E
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_DPBK(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	AT_TC(g_sw_SPEC, "AABB----------DPBK CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^MONIEND
		eResult = (UINT32)EX_EmodeClearPBKInfo();

		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "+DPBK:Flash PBK was cleared");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------DPBK Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_DREG(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	AT_TC(g_sw_SPEC, "AABB----------DREG CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^DREG
		eResult = EX_EmodeClearREGInfo();

		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "+DREG:REG was cleared");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------DREG Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_DFFS(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	switch (pParam->iType)
	{
	case AT_CMD_EXE: // AT^DFFS
		eResult = EX_EmodeClearFFSInfo();
		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "+DFFS:FS was cleared");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}
		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
	AT_TC(g_sw_SPEC, "AABB----------DFFS Cmd END-----pParam->iType = %d--nDLCI =%d-----\n", pParam->iType, pParam->nDLCI);
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_DSMS(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	AT_TC(g_sw_SPEC, "----------DSMS CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^DSMS
		eResult = EX_EmodeClearSMSInfo();

		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "^SMS:Flash SMS was cleared");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------DSMS Cmd END------------\n");
}

// Add by wulc

VOID AT_EMOD_CmdFunc_SLCD(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	AT_TC(g_sw_SPEC, "----------DSMS CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^DSMS
		eResult = EX_EmodeClearSMSInfo();

		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "+SLCD");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------DSMS Cmd END------------\n");

}
VOID AT_EMOD_CmdFunc_STCD(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[50];

	AT_TC(g_sw_SPEC, "----------DSMS CmdType : %d----------\n", pParam->iType);

	switch (pParam->iType)
	{

	case AT_CMD_EXE: // AT^DSMS
		eResult = EX_EmodeClearSMSInfo();

		if (eResult)
		{
			AT_MemZero(res, 50);
			AT_Sprintf(res, "+STCD");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
		}
		else
		{
			// Csw Function execute fall,Create error result
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		}

		break;

	default: // error type
		// CMD type error, Create error result
		eResult = ERR_INVALID_TYPE;
		AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		break;
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------DSMS Cmd END------------\n");

}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_SBBI(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT8 iParaCount;
	UINT8 nParaLen         = 0;
	PAT_CMD_RESULT pResult = NULL;
	BOOL nSIMMark;
	BOOL nNWMark;
	UINT8 tLen;
	UINT8 res[100];

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	AT_TC(g_sw_SPEC, "AABB----------SBBI CmdType : %d----------\n", pParam->iType);

	if (pParam == NULL)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET: // AT^SBBI=<bsimbindmark>,<bnwbindmark>
			// check parameter count
			eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

			if ((eResult == ERR_SUCCESS) && (2 == iParaCount))
			{
				// Get parameter
				nParaLen = 1;

				if ((ERR_SUCCESS == AT_Util_GetParaWithRule(pParam->pPara,
				        0,
				        AT_UTIL_PARA_TYPE_UINT8,
				        &nSIMMark,
				        &nParaLen))
				        && (ERR_SUCCESS == AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT8, &nNWMark, &nParaLen)))
				{
					// Get all para success
					// check para valid or invalid
					if ((1 < nNWMark) || (0 > nNWMark) || (1 < nSIMMark) || (0 > nSIMMark))
					{
						// para invalid,Create error result
						eResult = ERR_AT_CME_PARAM_INVALID;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}
					else
					{
						// para valid, call csw function to process,
						// Create success result
						if (ERR_SUCCESS == CFW_EmodSetBindInfo(nSIMMark, nNWMark))
							AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);
						else
						{
							// Csw Function execute fall,Create error result
							eResult = ERR_AT_CME_EXE_FAIL;
							AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						}
					}
				}
				else
				{
					// Get parameter error,Create error result
					eResult = ERR_AT_CME_PARAM_INVALID;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
			}
			else
			{
				// Get parameter count error,Create error result
				eResult = ERR_AT_CME_PARAM_INVALID;
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}

			break;

		case AT_CMD_READ:  // AT^SBBI?
			// call csw function to execute this cmd
			CFW_EmodGetBindInfo(&nSIMMark, &nNWMark);

			// Create success result
			AT_MemZero(res, 100);

			if (nSIMMark == TRUE)
				AT_StrCpy(res, "^SBBI: SIM Bind:Enable,");
			else
				AT_StrCpy(res, "^SBBI: SIM Bind:Disable,");

			tLen = AT_StrLen(res);

			if (nNWMark == TRUE)
				AT_StrCpy(res + tLen, "NetWork Bind:Enable");
			else
				AT_StrCpy(res + tLen, "NetWork Bind:Disable");

			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

			break;

		case AT_CMD_TEST:  // AT^SBBI=?
			// copy result info & Create success result
			AT_MemZero(res, 100);

			AT_Sprintf(res, "^SBBI: SIM Bind:(0,1); NW Bind:(0,1)");

			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

			break;

		default: // error Type
			// CMD type error, Create error result
			eResult = ERR_AT_CME_EXE_NOT_SURPORT;

			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

			break;
		}
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_SBBR(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT8 iParaCount;
	UINT8 nParaLen         = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[400];
	UINT8 rTemp[50];
	CFW_EMOD_READ_BIND_SIM bindInfo;

	CFW_EMOD_BIND_SIM *pSim = NULL;
	void *phandle     = NULL;
	UINT8 nStartIndex = 0;
	UINT8 nEndIndex   = 0;
	UINT8 i;
	UINT8 j;
	UINT8 tflag = 0;
	UINT16 len  = 0;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	AT_TC(g_sw_SPEC, "AABB----------SBBR CmdType : %d----------\n", pParam->iType);

	if (pParam == NULL)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET: // READ
			eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

			if ((eResult != ERR_SUCCESS) || (iParaCount > 2) || (iParaCount < 1))
			{
				eResult = ERR_AT_CME_PARAM_INVALID;
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}
			else
			{
				if (iParaCount == 1)
				{
					nParaLen = 1;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nStartIndex, &nParaLen);
				}
				else if (iParaCount == 2)
				{
					nParaLen = 1;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nStartIndex, &nParaLen);

					if (eResult == ERR_SUCCESS)
					{
						nParaLen = 1;
						eResult  = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT8, &nEndIndex, &nParaLen);
					}
				}

				if (ERR_SUCCESS != eResult)
				{
					eResult = ERR_AT_CME_PARAM_INVALID;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					if (1 == iParaCount)
					{
						CFW_EmodReadBindSIM(&bindInfo);
						pSim    = AT_MALLOC(sizeof(CFW_EMOD_BIND_SIM) * bindInfo.nNumber);
						phandle = pSim;

						if ((pSim != NULL) && (0 < nStartIndex) && (10 >= nStartIndex))
						{
							pSim = (CFW_EMOD_BIND_SIM *)bindInfo.nPointer;

							for (j = 0; j < bindInfo.nNumber; j++)
							{
								if (nStartIndex == pSim->nIndex)
								{
									AT_MemZero(res, 400);
									AT_MemZero(rTemp, 50);
									AT_Sprintf(rTemp, "+SBBR:  ");  // yangtt 2008-05-07 for bug 8301
									AT_StrCat(res, rTemp);
									AT_Sprintf(rTemp, "%u,  \"", pSim->nIndex);
									AT_StrCat(res, rTemp);

									for (i = 0; i < pSim->nImsiLen; i++)
									{
										AT_Sprintf(rTemp, "%u", pSim->pImsiNum[i] - 0x30);
										AT_StrCat(res, rTemp);
									}

									AT_Sprintf(rTemp, "\"");

									AT_StrCat(res, rTemp);
									tflag = 1;
									break;
								}

								pSim++;

							}

							// if no  pSim->nIndex = nStartIndex
							if (tflag == 0)
							{
								AT_MemZero(res, 400);

								// AT_Sprintf(  res,  "^SBBR: NULL"  );
							}

							AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

							AT_FREE(phandle);
							phandle = NULL;

						}
						else
						{

							if (pSim != NULL)
							{
								AT_FREE(phandle);
								phandle = NULL;
							}

							eResult = ERR_AT_CME_PARAM_INVALID;

							AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						}
					}
					else if (2 == iParaCount)
					{
						CFW_EmodReadBindSIM(&bindInfo);
						pSim = AT_MALLOC(sizeof(CFW_EMOD_BIND_SIM) * bindInfo.nNumber);
						AT_TC(g_sw_SPEC, "AABB----------bindInfo.nNumber =  %d----------\n", bindInfo.nNumber);
						phandle = pSim;

						if ((pSim != NULL)
						        && (0 < nStartIndex)
						        && (11 > nStartIndex) && (0 < nEndIndex) && (11 > nEndIndex) && (nEndIndex > nStartIndex))
						{
							pSim = (CFW_EMOD_BIND_SIM *)bindInfo.nPointer;
							AT_MemZero(res, 400);
							AT_MemZero(rTemp, 50);

							// AT_Sprintf(  rTemp,  "^SBBR:  "  );
							// AT_StrCat(  res,  rTemp  );

							for (j = 0; j < bindInfo.nNumber; j++)
							{
								if ((pSim->nIndex >= nStartIndex) && (pSim->nIndex <= nEndIndex))
								{
									AT_Sprintf(rTemp, "^SBBR:   %u,  \"", pSim->nIndex);
									AT_StrCat(res, rTemp);

									for (i = 0; i < pSim->nImsiLen; i++)
									{
										AT_Sprintf(rTemp, "%u", pSim->pImsiNum[i] - 0x30);
										AT_StrCat(res, rTemp);
									}

									// AT_20071207_CAOW_B
									// AT_20071224_CAOW_B for bug#7272
#if 0
									if (j + 1 != bindInfo.nNumber)
#else
									if (j + 1 != nEndIndex)
#endif
										// AT_20071224_CAOW_E
									{
										AT_Sprintf(rTemp, "\"\r\n");
										AT_StrCat(res, rTemp);
									}
									else
									{
										AT_Sprintf(rTemp, "\"\r");
										AT_StrCat(res, rTemp);
									}

									// AT_20071207_CAOW_E
									tflag = 1;
								}

								pSim++;
							}

							// if no  pSim->nIndex  in[ nStartIndex,nEndIndex]
							if (tflag == 0)
							{
								AT_MemZero(res, 400);
								AT_Sprintf(res, "\n");  // add "\n"
								// AT_Sprintf(  res,  "^SBBR:\r\n  NULL\n"  ); //add "\n"
							}

							// delect the laster "\n"
							AT_MemZero(res + AT_StrLen(res) - 1, 1);

							len = strlen(res);

							AT_Util_TrimRspStringSuffixCrLf(res, &len);

							AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

							AT_FREE(phandle);

							phandle = NULL;
						}
						else
						{
							if (pSim != NULL)
							{
								AT_FREE(phandle);
								phandle = NULL;
							}

							eResult = ERR_AT_CME_PARAM_INVALID;

							AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						}

					}
					else
					{
						eResult = ERR_AT_CME_PARAM_INVALID;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}

				}

			}

			break;

		case AT_CMD_TEST:  // AT^SBBR=?
			// copy result info & Create success result
			AT_MemZero(res, 400);
			AT_Sprintf(res, "+SBBR: 1-10,15");  // yangtt 2008-05-07 for bug 8301
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
			break;

		default: // error Type
			// CMD type error, Create error result
			eResult = ERR_AT_CME_OPERATION_NOT_ALLOWED;
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			break;
		}
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------SBBR Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_NBBR(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT8 iParaCount;
	UINT8 nParaLen = 0;
	UINT8 res[400];
	UINT8 rTemp[50];
	UINT8 nStartIndex = 0;
	UINT8 nEndIndex   = 0;
	UINT8 i;
	UINT8 j;
	UINT8 tflag   = 0;
	void *phandle = NULL;
	CFW_EMOD_READ_BIND_NW bindInfo;

	CFW_EMOD_BIND_NW *pNw = NULL;
	PAT_CMD_RESULT pResult = NULL;
	UINT16 len             = 0;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	AT_TC(g_sw_SPEC, "AABB----------NBBR CmdType : %d----------\n", pParam->iType);

	if (pParam == NULL)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET: // READ

			eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

			if ((eResult != ERR_SUCCESS) || (iParaCount > 2) || (iParaCount < 1))
			{
				eResult = ERR_AT_CME_PARAM_INVALID;
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}
			else
			{
				if (iParaCount == 1)
				{
					nParaLen = 1;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nStartIndex, &nParaLen);
				}
				else if (iParaCount == 2)
				{
					nParaLen = 1;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nStartIndex, &nParaLen);

					if (eResult == ERR_SUCCESS)
					{
						nParaLen = 1;
						eResult  = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT8, &nEndIndex, &nParaLen);
					}
				}

				if (ERR_SUCCESS != eResult)
				{
					eResult = ERR_AT_CME_PARAM_INVALID;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					if (1 == iParaCount)
					{
						CFW_EmodReadBindNW(&bindInfo);
						pNw     = AT_MALLOC(sizeof(CFW_EMOD_BIND_NW) * bindInfo.nNumber);
						phandle = pNw;

						if ((pNw != NULL) && (0 < nStartIndex) && (5 >= nStartIndex))
						{
							pNw = (CFW_EMOD_BIND_NW *)bindInfo.nPointer;

							for (j = 0; j < bindInfo.nNumber; j++)
							{
								if (nStartIndex == pNw->nIndex)
								{
									AT_MemZero(res, 400);
									AT_MemZero(rTemp, 50);
									AT_Sprintf(rTemp, "^NBBR:  ");
									AT_StrCat(res, rTemp);
									AT_Sprintf(rTemp, "%u,  \"", pNw->nIndex);
									AT_StrCat(res, rTemp);

									for (i = 0; i < 3; i++) // 3//3 adapt to the array length :pNw->pNccNum[3]
									{
										AT_Sprintf(rTemp, "%u", pNw->pNccNum[i] - 0x30);
										AT_StrCat(res, rTemp);
									}

									AT_Sprintf(rTemp, "\"");

									AT_StrCat(res, rTemp);
									tflag = 1;
									break;
								}

								pNw++;

							}

							// if no  pNw->nIndex = nStartIndex
							if (tflag == 0)
							{
								AT_MemZero(res, 400);

								// AT_Sprintf(  res,  "^SBBR:\r\n  NULL\n"  );
							}

							len = strlen(res);

							AT_Util_TrimRspStringSuffixCrLf(res, &len);
							AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
							AT_FREE(phandle);
							phandle = NULL;

						}
						else
						{
							if (pNw != NULL)
							{
								AT_FREE(phandle);
								phandle = NULL;
							}

							eResult = ERR_AT_CME_PARAM_INVALID;

							AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						}
					}
					else if (2 == iParaCount)
					{
						CFW_EmodReadBindNW(&bindInfo);
						pNw     = AT_MALLOC(sizeof(CFW_EMOD_BIND_NW) * bindInfo.nNumber);
						phandle = pNw;

						if ((pNw != NULL)
						        && (0 < nStartIndex)
						        && (6 > nStartIndex) && (0 < nEndIndex) && (6 > nEndIndex) && (nEndIndex > nStartIndex))
						{
							pNw = (CFW_EMOD_BIND_NW *)bindInfo.nPointer;
							AT_MemZero(res, 400);
							AT_MemZero(rTemp, 50);

							// AT_Sprintf(  rTemp,  "^NBBR:  "  );
							// AT_StrCat(  res,  rTemp  );

							for (j = 0; j < bindInfo.nNumber; j++)
							{
								if ((pNw->nIndex >= nStartIndex) && (pNw->nIndex <= nEndIndex))
								{
									AT_Sprintf(rTemp, "^NBBR:   %u,  \"", pNw->nIndex);
									AT_StrCat(res, rTemp);

									for (i = 0; i < 3; i++) // 3//3 adapt to the array length :pNw->pNccNum[3]
									{
										AT_Sprintf(rTemp, "%u", pNw->pNccNum[i] - 0x30);
										AT_StrCat(res, rTemp);
									}

									// AT_20071207_CAOW_B
									// AT_20071224_CAOW_B for bug#7272
#if 0
									if (j + 1 != bindInfo.nNumber)
#else
									if (j + 1 != nEndIndex)
#endif
										// AT_20071224_CAOW_E
									{
										AT_Sprintf(rTemp, "\"\r\n");
										AT_StrCat(res, rTemp);
									}
									else
									{
										AT_Sprintf(rTemp, "\"\r");
										AT_StrCat(res, rTemp);
									}

									// AT_20071207_CAOW_E
									tflag = 1;
								}

								pNw++;
							}

							// if no  pNw->nIndex  in[ nStartIndex,nEndIndex]
							if (tflag == 0)
							{
								AT_MemZero(res, 400);
								AT_Sprintf(res, "\n");  // add "\n"
								// AT_Sprintf(  res,  "^SBBR:\r\n  NULL\n"  ); //add "\n"
							}

							// delect the laster "\n"
							AT_MemZero(res + AT_StrLen(res) - 1, 1);

							len = strlen(res);

							AT_Util_TrimRspStringSuffixCrLf(res, &len);

							AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

							AT_FREE(phandle);

							phandle = NULL;
						}
						else
						{
							if (pNw != NULL)
							{
								AT_FREE(phandle);
								phandle = NULL;
							}

							eResult = ERR_AT_CME_PARAM_INVALID;

							AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						}

					}
					else
					{
						eResult = ERR_AT_CME_PARAM_INVALID;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}
				}
			}

			break;

		case AT_CMD_TEST:  // AT^NBBR=?
			// copy result info & Create success result
			AT_MemZero(res, 400);
			AT_Sprintf(res, "^NBBR: 1-5,3");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
			break;

		default: // error Type
			// CMD type error, Create error result
			eResult = ERR_AT_CME_OPERATION_NOT_ALLOWED;
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			break;
		}
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------NBBR Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_SBBW(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT8 iParaCount;
	UINT8 nParaLen = 0;
	UINT8 i;
	UINT8 nLen;
	UINT8 *pBindSIM        = NULL;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[100];
	UINT8 nIndex;
	UINT8 nBSimNum[30];
	BOOL tFlag = 1;
	CFW_EMOD_UPDATE_RETURN ret;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	AT_TC(g_sw_SPEC, "AABB----------SBBW CmdType : %d----------\n", pParam->iType);

	if (pParam == NULL)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET: // AT^SBBW=<nIndex>, <numbers>
			// get para num.
			eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

			if ((eResult != ERR_SUCCESS) || (iParaCount != 2))
			{
				eResult = ERR_AT_CME_PARAM_INVALID;
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}
			else
			{
				// get para value.
				nParaLen = 1;
				eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nIndex, &nParaLen);

				if (eResult == ERR_SUCCESS)
				{
					nParaLen = 30;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, nBSimNum, &nParaLen);

					// check para num

					if (nParaLen != 15)
						tFlag = 0;

					// check para valid or invalid.
					for (i = 0; i < nParaLen; i++)
					{
						if (0 > (nBSimNum[i] - 0x30) || 9 < (nBSimNum[i] - 0x30))
							tFlag = 0;
					}

					if (10 < nIndex)
						tFlag = 0;
				}

				// process
				if ((ERR_SUCCESS != eResult) || (tFlag == 0))
				{
					eResult = ERR_AT_CME_PARAM_INVALID;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					// AT_Util_Trim(nBSimNum);
					nLen     = AT_StrLen(nBSimNum);
					pBindSIM = (UINT8 *)AT_MALLOC(nLen);

					if (pBindSIM == NULL)
					{

						eResult = ERR_AT_CME_MEMORY_FAILURE;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
						break;
					}

					AT_MemZero(pBindSIM, nLen);

					for (i = 0; i < nLen; i++)
					{
						*((UINT8 *)pBindSIM + i) = *((UINT8 *)nBSimNum + i) - 0x30;
					}

					AT_TC(g_sw_SPEC, "AABB----------nIndex = %d----------\n", nIndex);

					// call csw function to write the information.
					ret = CFW_EmodUpdateSimBind(nIndex, nBSimNum, AT_StrLen(nBSimNum));

					AT_FREE(pBindSIM);
					pBindSIM = NULL;

					if (ERR_SUCCESS == ret.nErrCode)
					{
						AT_MemZero(res, 100);
						AT_Sprintf(res, "^SBBW: nIndex=%u", ret.nIndex);
						AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);

						AT_TC(g_sw_SPEC, "AABB----------ret.nIndex = %d----------\n", ret.nIndex);
					}
					else
					{
						eResult = ERR_AT_CME_EXE_FAIL;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}
				}
			}

			break;

		case AT_CMD_TEST:  // AT^SBBW=?
			// copy result info & Create success result
			AT_MemZero(res, 100);
			AT_Sprintf(res, "^SBBW: 1-10,15");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
			break;

		default: // error Type
			// CMD type error, Create error result
			eResult = ERR_AT_CME_OPERATION_NOT_ALLOWED;
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			break;
		}
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------SBBW Cmd END------------\n");
}

/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/
VOID AT_EMOD_CmdFunc_NBBW(AT_CMD_PARA *pParam)
{
	INT32 eResult;
	UINT8 iParaCount;
	UINT8 nParaLen = 0;
	UINT8 i;
	UINT8 nLen;
	UINT8 *pBindNW         = NULL;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 res[100];
	UINT8 nIndex;
	UINT8 nBNwNum[10];
	BOOL tFlag = 1;
	CFW_EMOD_UPDATE_RETURN ret;

	//#ifdef AT_DUAL_SIM
	//  UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	//#endif

	AT_TC(g_sw_SPEC, "AABB----------NBBW CmdType : %d----------\n", pParam->iType);

	if (pParam == NULL)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);

	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET: // AT^NBBW=<nIndex>, <numbers>
			// get para num.
			eResult = AT_Util_GetParaCount(pParam->pPara, &iParaCount);

			if ((eResult != ERR_SUCCESS) || (iParaCount != 2))
			{
				eResult = ERR_AT_CME_PARAM_INVALID;
				AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			}
			else
			{
				// get para value.
				nParaLen = 1;
				eResult  = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nIndex, &nParaLen);

				if (eResult == ERR_SUCCESS)
				{
					nParaLen = 10;
					eResult  = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, nBNwNum, &nParaLen);

					// check para num

					if (nParaLen != 3)
						tFlag = 0;

					// check para valid or invalid.
					for (i = 0; i < nParaLen; i++)
					{
						if (0 > (nBNwNum[i] - 0x30) || 9 < (nBNwNum[i] - 0x30))
							tFlag = 0;
					}

					if (5 < nIndex)
						tFlag = 0;
				}

				// process
				if ((ERR_SUCCESS != eResult) || (tFlag == 0))
				{
					eResult = ERR_AT_CME_PARAM_INVALID;
					AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
				}
				else
				{
					// AT_Util_Trim(nBNwNum);
					nLen    = AT_StrLen(nBNwNum);
					pBindNW = (UINT8 *)AT_MALLOC(nLen);

					if (pBindNW == NULL)
					{
						eResult = ERR_AT_CME_MEMORY_FAILURE;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}

					AT_MemZero(pBindNW, nLen);

					for (i = 0; i < nLen; i++)
					{
						*((UINT8 *)pBindNW + i) = *((UINT8 *)pBindNW + i) - 0x30;
					}

					AT_TC(g_sw_SPEC, "AABB----------nIndex = %d----------\n", nIndex);

					// call csw function to write the information.
					ret = CFW_EmodUpdateNwBind(nIndex, nBNwNum, AT_StrLen(nBNwNum));

					AT_FREE(pBindNW);
					pBindNW = NULL;

					if (ERR_SUCCESS == ret.nErrCode)
					{
						AT_MemZero(res, 100);
						AT_Sprintf(res, "^NBBW: nIndex=%u", ret.nIndex);
						AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
						AT_TC(g_sw_SPEC, "AABB----------ret.nIndex = %d----------\n", ret.nIndex);
					}
					else
					{
						eResult = ERR_AT_CME_EXE_FAIL;
						AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
					}
				}
			}

			break;

		case AT_CMD_TEST:  // AT^SBBW=?
			// copy result info & Create success result
			AT_MemZero(res, 100);
			AT_Sprintf(res, "^NBBW: 1-5, 3");
			AT_EX_SUCC_CreateRC(pResult, 0, res, AT_StrLen(res), pParam->nDLCI);
			break;

		default: // error Type
			// CMD type error, Create error result
			eResult = ERR_AT_CME_OPERATION_NOT_ALLOWED;
			AT_EX_FAIL_CreateRC(pResult, eResult, CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
			break;
		}
	}

	// send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	AT_TC(g_sw_SPEC, "AABB----------SBBW Cmd END------------\n");
}

VOID reg_BytesToString(UINT8 *bytes, UINT32 bytesSize, UINT8 *pString)
{
	UINT32 i = 0;

	memset(pString, 0x00, bytesSize * 2);

	for(i = 0; i < bytesSize; i++)
	{
		sprintf(pString, "%s%02x", pString, bytes[i]);
	}
}

UINT8 charToUint8(UINT8 c)
{
	if ( c >= '0' && c <= '9')
	{
		return  c - '0';
	}
	else if ( c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	else if ( c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else
		return 0xff;
}

INT32 reg_StringToBytes(UINT8 *pString, UINT32 strSize, UINT8 *Hex)
{
	UINT8 tmp[3] = {0x0};
	UINT32 i = 0;
	UINT8 *p = Hex;
	UINT8 low4bit, high4bit;

	if ((strSize % 2) != 0)
	{
		return -1;
	}

	for (i = 0 ; i < strSize; i += 2)
	{
		strncpy(tmp, pString + i, 2);
		high4bit = charToUint8(tmp[0]);
		low4bit = charToUint8(tmp[1]);

		if ((high4bit == 0xff) || (low4bit == 0xff))
		{
			return -1;
		}

		*p++ = ((high4bit << 4) & 0xf0) | (low4bit & 0x0f);
		//AT_TC(g_sw_SPEC, "*p=%x", *(p-1));
	}
	//AT_TC_MEMBLOCK(g_sw_PBK, Hex, p-Hex, 16);
	return (p - Hex);
}

INT32 AT_CFGINIT_GetPara(UINT8 *pRes, INT32 *Count, INT32 *Index, UINT8 *pStr, UINT32 *pStrLen)
{
	INT32 iResult;
	UINT8 *pBeg = NULL;
	UINT8 *pEnd = NULL;
	UINT8 uParamCount = 0;
	UINT8 NumBuf[10] = {0,};
	//    INT32 ParaNum = 0;

	pBeg = pEnd = pRes;
	iResult = AT_Util_GetParaCount(pRes, &uParamCount);
	if ((ERR_SUCCESS != iResult) || (uParamCount != 3))
	{
		AT_TC(g_sw_GPRS, "AT+CFGINIT:NULL == pParam\n");
		return -1;
	}

	//get total count
	while(*pEnd != ',') pEnd++ ;
	AT_StrNCpy(NumBuf, pBeg, (pEnd - pBeg));
	*Count = atoi(NumBuf);

	//get index
	pBeg = ++pEnd;
	while(*pEnd != ',') pEnd++ ;
	AT_StrNCpy(NumBuf, pBeg, (pEnd - pBeg));
	*Index = atoi(NumBuf);

	//get get date from ap
	pEnd += 2;
	pBeg = pEnd;
	while (*pEnd != '"') pEnd++;

	if ((pEnd - pBeg) < *pStrLen)
	{
		*pStrLen = pEnd - pBeg;
	}

	AT_StrNCpy(pStr, pBeg, *pStrLen);
	return ERR_SUCCESS;

}

//add by lijingyu  Wed Jan  9 15:54:00 CST 2013
VOID AT_EMOD_CFGUPDATE_Indictation(COS_EVENT *pParam)
{
	UINT16 count = 0;
	UINT16 tmp = 0;
	UINT16 i = 0;
	INT32 iRet = 0;
	UINT32 read_size = 0;
	UINT16 index = 0;
	UINT8 nDLCI = 1;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 read_data[CFG_SENDDATALEN + 1] = {0,};
	UINT8 send_data[CFG_SENDDATALEN + 1] = {0,};
	UINT8 CallBackData[CFG_SENDDATALEN + 25] = {0,};

	UINT32 uOffset = pParam->nParam1;
	UINT32 upd_size = pParam->nParam2;

	tmp = uOffset % (CFG_SENDDATALEN / 2);
	if (tmp != 0)
	{
		uOffset -= tmp;
		upd_size += tmp;
	}

	count += upd_size / (CFG_SENDDATALEN / 2);
	count += (upd_size % (CFG_SENDDATALEN / 2)) ? 1 : 0;
	index = uOffset / (CFG_SENDDATALEN / 2);

	for (i = 0; i < count; i++)
	{
		iRet = REG_Read(index * (CFG_SENDDATALEN / 2), read_data, CFG_SENDDATALEN / 2, &read_size);
		if (iRet != ERR_SUCCESS || &read_size < 0)
		{
			AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, nDLCI);
			AT_TC(g_sw_SPEC, "REG_Read failed. uOffset = %d", uOffset);
			break;
		}

		reg_BytesToString(read_data, read_size, send_data);
		sprintf(CallBackData, "+CFGIND:%d,%s", index, send_data);

		pResult = AT_CreateRC(CSW_IND_NOTIFICATION, CMD_RC_CR, CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, 0 , CallBackData, AT_StrLen(CallBackData), nDLCI);

		AT_Notify2ATM(pResult, nDLCI);
		if (pResult != NULL)
		{
			AT_FREE(pResult);
			pResult = NULL;
		}

		index++;
	}
}


VOID AT_EMOD_CmdFunc_CFGINIT(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult = NULL;
	INT32 Count = 0;
	INT32 Index = 0;
	INT32 Ret = 0;
	UINT32 LenWrited  = 0;
	UINT32 len = CFG_SENDDATALEN;
	UINT8 CallBackData[CFG_SENDDATALEN + 25] = {0,};
	UINT8 buf[CFG_SENDDATALEN + 1] = {0,};
	UINT8 WriteBuf[CFG_SENDDATALEN] = {0x00};

	Ret = AT_CFGINIT_GetPara(pParam->pPara, &Count, &Index, buf, &len);
	if (Ret != ERR_SUCCESS)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		goto end;
	}

	INT32 WriteBufSize = reg_StringToBytes(buf, len, WriteBuf);
	if (WriteBufSize < 0 || WriteBufSize != (len / 2))
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		goto end;
	}

	Ret = REG_Write(Index * CFG_SENDDATALEN / 2, WriteBuf, WriteBufSize, &LenWrited);
	if (WriteBufSize != LenWrited || Ret != ERR_SUCCESS)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		goto end;
	}

	//    sprintf(CallBackData,"+CFGINIT:%s",buf);
	//    AT_EX_SUCC_CreateRC(pResult, 0, CallBackData, AT_StrLen(CallBackData), pParam->nDLCI);
	AT_EX_SUCC_CreateRC(pResult, 0, 0, 0, pParam->nDLCI);

	if (Count == Index)
	{
		REG_SetInd2AT(1);
	}

end:
	//send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}
	return ;
}


/**************************************************************************************************
 * @fn
 *
 * @brief
 *
 * @param
 *
 * @return
 **************************************************************************************************/

VOID AT_EMOD_CmdFunc_CFGGET(AT_CMD_PARA *pParam)
{
	INT32 Count = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT32 read_size = 0;
	UINT8 read_data[CFG_SENDDATALEN + 1] = {0,};
	UINT8 send_data[CFG_SENDDATALEN + 1] = {0,};
	UINT8 CallBackData[CFG_SENDDATALEN + 25] = {0,};
	INT32 index = atoi(pParam->pPara);
	INT32 iRet = 0;

	UINT32 REG_Size = reg_GetRegSize();
	AT_TC(2, "reg_GetRegSize = %d", REG_Size);
	Count = (REG_Size * 2) / CFG_SENDDATALEN + (((REG_Size * 2 % CFG_SENDDATALEN) > 0) ? 0 : -1);

	if (Count < index)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		goto end;
	}

	iRet = REG_Read(index * (CFG_SENDDATALEN / 2), read_data, CFG_SENDDATALEN / 2, &read_size);
	if (iRet != ERR_SUCCESS || &read_size < 0)
	{
		AT_EX_FAIL_CreateRC(pResult, ERR_AT_CME_PARAM_INVALID , CMD_ERROR_CODE_TYPE_CME, 0, NULL, 0, pParam->nDLCI);
		goto end;
	}

	reg_BytesToString(read_data, read_size, send_data);
	sprintf(CallBackData, "+CFGGET:%d,%d,%s", (INT)Count, (INT)index, send_data);
	AT_EX_SUCC_CreateRC(pResult, 0, CallBackData, AT_StrLen(CallBackData), pParam->nDLCI);

	if (Count == index)
	{
		AT_TC(g_sw_SPEC, "CFGINIT success,REG_SetIndAT 1");
		REG_SetInd2AT(1);
	}

end:
	//send result to atm
	if (NULL != pResult)
	{
		AT_Notify2ATM(pResult, pParam->nDLCI);
		AT_FREE(pResult);
		pResult = NULL;
	}

	return ;
}

static UINT8 g_bt_status = 0;

VOID AT_EMOD_CmdFunc_BTSET(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult = NULL;
	UINT8 uParaCount = 0;
	UINT8 bt_state = 0;
	UINT8 param_len = 1;
	UINT8 Rsp[20] = {0,};

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_Sprintf(Rsp, "+BTSET: (0,1)");
		OK_RETURN(pResult, Rsp, pParam->nDLCI);
		return;

	}
	else if (pParam->iType == AT_CMD_READ)
	{
		AT_Sprintf(Rsp, "+BTSET: %d", g_bt_status);
		OK_RETURN(pResult, Rsp, pParam->nDLCI);
		return;
	}
	else if (pParam->iType == AT_CMD_SET)
	{
		if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (uParaCount != 1)
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &bt_state, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (bt_state == 0)
		{
#ifdef __BT_RDABT__
			g_bt_status = 0;
			rdabt_antenna_off();
#endif
			OK_RETURN(pResult, "+BTSET: 0", pParam->nDLCI);
			return;
		}
		else if (bt_state == 1)
		{
			g_bt_status = 1;
#ifdef __BT_RDABT__
			rdabt_init();
			rdabt_set_uart_rx_cb(uart_SendDataBTCallBack);
			rdabt_antenna_on();
#endif
			OK_RETURN(pResult, "+BTSET: 1", pParam->nDLCI);
			return;
		}
		else
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}
	}
	else
	{
		ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
		return;
	}
}

//
//Only for test STK command
//Simulate sim message
//

#include "itf_api.h"
#include "itf_msgc.h"

VOID AT_EMOD_CmdFunc_TSTK(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult = NULL;
	UINT8 uParaCount = 0;
	UINT8 nTestStkCmdStr[250] = {0x00,};
	UINT8 nTestStkCmdStrLen = 250;
	UINT8 Rsp[20] = {0,};

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_Sprintf(Rsp, "+TSTK: (0,1)");
		OK_RETURN(pResult, Rsp, pParam->nDLCI);
		return;

	}
	else if (pParam->iType == AT_CMD_READ)
	{
		AT_Sprintf(Rsp, "+TSTK: ");
		OK_RETURN(pResult, Rsp, pParam->nDLCI);
		return;
	}
	else if (pParam->iType == AT_CMD_SET)
	{
		if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (uParaCount != 1)
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, nTestStkCmdStr, &nTestStkCmdStrLen))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		AT_TC(g_sw_SPEC, "TSTK Command simulate sim message\n ");
		AT_TC(g_sw_SPEC, "TSTK Command nTestStkCmdStrLen %d %s\n ", nTestStkCmdStrLen, nTestStkCmdStr);

		UINT8 nSim = AT_SIM_ID(pParam->nDLCI);

		api_SimFetchInd_t *pFetchInd = NULL;

		UINT16 nStruSize = SIZEOF(api_SimFetchInd_t);

		//Systen will Free pSendSmsReq

		pFetchInd = (api_SimFetchInd_t *)AT_MALLOC(nStruSize);

		if(!pFetchInd)
		{
			AT_TC(g_sw_SPEC, "TSTK Command  malloc error!!! \n ");
			return;
		}


		SUL_ZeroMemory8(pFetchInd, nStruSize);

		AT_TC(g_sw_SPEC, "TSTK Command nStruSize %d\n ", nStruSize);

		pFetchInd->Length = SMS_PDUAsciiToBCD(nTestStkCmdStr, nTestStkCmdStrLen, pFetchInd->Data);

		CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pFetchInd->Data, pFetchInd->Length, 16);

#ifdef CFW_MULTI_SIM
		CFW_BalSendMessage(11, API_SIM_FETCH_IND, pFetchInd, nStruSize, nSim);
#else
		//CFW_BalSendMessage(11, HVY_API_RESERVED,  NULL,  1);
#endif
		AT_EX_SUCC_CreateRC(pResult, 0, NULL, 0, pParam->nDLCI);  // Return ok
		if (NULL != pResult)
		{
			AT_Notify2ATM(pResult, pParam->nDLCI);
			AT_FREE(pResult);
			pResult = NULL;
		}
		AT_FREE(pFetchInd);
	}
	else
	{
		ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
		return;
	}
}

#endif


#if (AUDIO_CALIB_VER == 1)

//0~5:     ApMode Sel = CALL
//0:BT Call


//10~15：  ApMode Sel = Music Play
//10:Tflash Music
//11:BT Music
//12:FM
//13:LINEIN
BOOL _CheckITFValue(UINT8* pnITF)
{
	hal_HstSendEvent(SYS_EVENT, 0x20161453);
	hal_HstSendEvent(SYS_EVENT, *pnITF);

    switch(*pnITF)
    {
        case 0  : *pnITF = AUD_ITF_BT_EP;
				 break;
				 
		case 10 : *pnITF = SND_ITF_EAR_PIECE + 10;
		         break;
				 
	    case 11 : *pnITF = SND_ITF_BT_EP + 10;
		         break; 
				 
 	    case 12 : *pnITF = SND_ITF_FM + 10;
 				 break;  
				 
 	    case 13 : *pnITF = SND_ITF_LINEIN + 10;
 				 break;  

		default : *pnITF = SND_ITF_NONE;
				 break;  
	}

	hal_HstSendEvent(SYS_EVENT, *pnITF);

    if(*pnITF == SND_ITF_NONE)
        return FALSE;
    else
        return TRUE;
}


VOID AT_EMOD_CmdFunc_CAVCP(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 iIndex = 0;
    UINT8 nITF = 0;
    UINT8 nCtrl = 0;
    UINT16 param_len = 1;
    UINT8 Rsp[20] = {0,} ;

    UINT8 *nParam = NULL;

    hal_HstSendEvent(SYS_EVENT, 0x20161401);

    //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
    nParam = AudioOverlayBuf;
#else
    nParam = (UINT8*)AT_MALLOC(AT_EMOD_BUFFER_SIZE);
#endif

    if(!nParam)
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);

    memset(nParam,0x00,AT_EMOD_BUFFER_SIZE);

    if (pParam->iType == AT_CMD_TEST)
    {
        OK_CALIB_RETURN(pResult, "+CAVCP ", pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        strcpy(Rsp, "Not Support");
        OK_CALIB_RETURN(pResult, Rsp, pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_SET)
    {
        if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if (uParaCount != 3)
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nITF, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        param_len = 1;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nCtrl, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = AT_EMOD_BUFFER_SIZE;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_STRING, nParam, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
        //AT_TC(g_sw_SPEC, "------%s, nParam len=%d", __FUNCTION__, strlen(nParam));

        if(!_CheckITFValue(&nITF))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
		
            hal_HstSendEvent(SYS_EVENT, 0x20161459);
             hal_HstSendEvent(SYS_EVENT, nCtrl);
	      hal_HstSendEvent(SYS_EVENT, nITF);
		hal_HstSendEvent(SYS_EVENT, musicItf);
		hal_HstSendEvent(SYS_EVENT, audioItf);
		
        if(!((0 <= nCtrl)&&(nCtrl <= 23)))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        
        if(nCtrl % 2 == 0)
        {
            if ((nITF) != (audioItf))
            {
                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_ITF_DIFFERENT, pParam->nDLCI);
            }
        }        

        UINT8 *pParambuff;
        //CALIB_CALIBRATION_T* calibPtr = calib_GetPointers();
        CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();

        switch(nCtrl)
        {
            case 0:
                calib_SetAudioVocHPFParam(nITF, nParam, param_len);
                vpp_SpeechBTSetHpfPara(  &(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].hpf));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 1:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocHPFParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 2:
                calib_SetAudioVocFIRTXParam(nITF, nParam, param_len);
                BTSco_SetMsdfReloadFlag();
		        vpp_SpeechBTSetMSDFPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].fir));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 3:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocFIRTXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 4:
                calib_SetAudioVocFIRRXParam(nITF, nParam, param_len);
                BTSco_SetMsdfReloadFlag();
		        vpp_SpeechBTSetMSDFPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].fir));

                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 5:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocFIRRXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 6:
                calib_SetAudioVocAECParam(nITF, nParam, param_len);
                vpp_SpeechBTSetAecAllPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].aec));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 7:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocAECParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 8:
                calib_SetAudioVocNSTXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetNoiseSuppressParaTX(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].speex.tx));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 9:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocNSTXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 10:
                calib_SetAudioVocNSRXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetNoiseSuppressParaRX(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].speex.rx));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 11:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocNSRXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 12:
                calib_SetAudioVocAGCTXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetNonClipParaTX(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].agc.tx));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 13:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocAGCTXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 14:
                calib_SetAudioVocAGCRXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetNonClipParaRX(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].agc.rx));
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 15:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocAGCRXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 16:
                calib_SetAudioVocEQParam(nITF, nParam, param_len);
                vpp_SpeechBTSetEQPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].eq));
				//vpp_BtCallSetAudioParamReloadFlag(1);
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 17:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocEQParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 18:
                calib_SetAudioVocDigGain(nITF, nParam, param_len);
                vpp_SpeechBTSetScaleDigitalGainPara(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].gain), SND_SPK_VOL_7);
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 19:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioVocDigGain(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

			case 20:
                calib_SetAudioVocWebRtcAGCTXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetWebRtcAgcParaTx(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].webrtcagc.tx));
	            vpp_SpeechBTSetWebRtcAgcReloadFlag(1,0);
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 21:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocWebRtcAGCTXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 22:
                calib_SetAudioVocWebRtcAGCRXParam(nITF, nParam, param_len);
                vpp_SpeechBTSetWebRtcAgcParaRx(&(calibPtr->aud.audioVoc[(UINT8)calibPtr->aud.audioVocItfMap[nITF]].webrtcagc.rx));
                vpp_SpeechBTSetWebRtcAgcReloadFlag(0,1);
                OK_CALIB_RETURN(pResult, "+CAVCP: ", pParam->nDLCI);

            case 23:
                strcpy(nParam,"+CAVCP: " );
                pParambuff = nParam+strlen("+CAVCP: ");
                *pParambuff = '"';
                param_len =  calib_GetAudioVocWebRtcAGCRXParam(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            default:
                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
    }
    else
    {
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }

_exit:
    if (nParam)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
        nParam = NULL;
#else
        AT_FREE(nParam);
#endif
    }

    return;
}


VOID AT_EMOD_CmdFunc_CACCP(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 iIndex = 0;
    UINT8 nITF = 0;
    UINT8 nCtrl = 0;
    UINT16 param_len = 1;
    UINT8 Rsp[20] = {0,} ;

    UINT8 *nParam = NULL;

    hal_HstSendEvent(SYS_EVENT, 0x20161402);

    //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
    nParam = AudioOverlayBuf;
#else
    nParam = (UINT8*)AT_MALLOC(AT_EMOD_BUFFER_SIZE);
#endif

    hal_HstSendEvent(SYS_EVENT, 0x20161412);

    if(!nParam)
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);

    memset(nParam,0x00,AT_EMOD_BUFFER_SIZE);

    if (pParam->iType == AT_CMD_TEST)
    {
        OK_CALIB_RETURN(pResult, "+CACCP ", pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        strcpy(Rsp, "Not Support");
        OK_CALIB_RETURN(pResult, Rsp, pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_SET)
    {
        if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if (uParaCount != 3)
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nITF, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 1;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nCtrl, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = AT_EMOD_BUFFER_SIZE;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_STRING, nParam, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
        //AT_TC(g_sw_SPEC, "------%s, nParam len=%d", __FUNCTION__, strlen(nParam));

        if(!_CheckITFValue(&nITF))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
		
        hal_HstSendEvent(SYS_EVENT, 0x20161458);
        hal_HstSendEvent(SYS_EVENT, nCtrl);
	    hal_HstSendEvent(SYS_EVENT, nITF);
		hal_HstSendEvent(SYS_EVENT, musicItf);
		hal_HstSendEvent(SYS_EVENT, audioItf);
		
        if(!((0 <= nCtrl)&&(nCtrl<=7)))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if(nCtrl % 2 == 0)
        {
            if ((0 <= nITF)&&(nITF <= 5))
            {
                if ((nITF) != (audioItf))
                {
                    hal_HstSendEvent(SYS_EVENT, 0x20161457);
	             hal_HstSendEvent(SYS_EVENT, nITF);
		      hal_HstSendEvent(SYS_EVENT, audioItf);
					
                    ERROR_CALIB_RETURN(pResult, ERR_AT_CME_ITF_DIFFERENT, pParam->nDLCI);
                }

            }
            else
            {
                if ((nITF-10) != (musicItf))
                {
                    hal_HstSendEvent(SYS_EVENT, 0x20161457);
	             hal_HstSendEvent(SYS_EVENT, nITF);
		      hal_HstSendEvent(SYS_EVENT, musicItf);
					
                    ERROR_CALIB_RETURN(pResult, ERR_AT_CME_ITF_DIFFERENT, pParam->nDLCI);
                }
            }
        }

        UINT8 *pParambuff;
        AUD_LEVEL_T *pAudioLevelCfg = aud_CodecCommonGetLatestLevel();
        AUD_MAP_LEVEL_T * pVoisAudioCfg = (AUD_MAP_LEVEL_T *)BTSco_GetAudioCfg();



        switch(nCtrl)
        {
            case 0:
            {
                if(nITF>=10)
                {
                    calib_SetAudioCodecIngain(nITF-10, nParam,param_len );
                    aud_Setup(nITF-10, pAudioLevelCfg);
                }
                else
                {
                    calib_SetAudioCodecIngain(nITF, nParam,param_len );
                    BTSco_Setup(nITF, (VOIS_AUDIO_CFG_T *)pVoisAudioCfg);
                }
                OK_CALIB_RETURN(pResult, "+CACCP: ", pParam->nDLCI);
            }
            case 1:
            {
                strcpy(nParam,"+CACCP: " );
                pParambuff = nParam+strlen("+CACCP: ");
                *pParambuff = '"';
                if(nITF>=10)
                    param_len = calib_GetAudioCodecIngain(nITF-10, pParambuff+1);
                else
                    param_len = calib_GetAudioCodecIngain(nITF,  pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);
            }
            case 2:
            {
                if(nITF>=10)
                {					
                    calib_SetAudioCodecOutGains(nITF-10, nParam,param_len,TRUE );
                    aud_Setup(nITF-10, pAudioLevelCfg);
                }
                else
                {
                    calib_SetAudioCodecOutGains(nITF, nParam,param_len , FALSE);
                    BTSco_Setup(nITF, (VOIS_AUDIO_CFG_T *)pVoisAudioCfg);
                }
                OK_CALIB_RETURN(pResult, "+CACCP: ", pParam->nDLCI);
            }
            case 3:
            {
                strcpy(nParam,"+CACCP: " );
                pParambuff = nParam+strlen("+CACCP: ");
                *pParambuff ='"';
                if(nITF>=10)
                {
                    param_len = calib_GetAudioCodecOutgains(nITF-10, pParambuff+1, TRUE);
                }
                else
                {
                    param_len = calib_GetAudioCodecOutgains(nITF, pParambuff+1, FALSE);
                }
                pParambuff[param_len+1] = '"';
                pParambuff[param_len+2] = 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);
            }
            case 4:
            {

                if(nITF>=10)
                {
                    calib_SetAudioCodecSidetone(nITF-10, nParam,param_len );
                    aud_Setup(nITF-10, pAudioLevelCfg);
                }
                else
                {
                    calib_SetAudioCodecSidetone(nITF, nParam ,param_len);
                    BTSco_Setup(nITF, (VOIS_AUDIO_CFG_T *)pVoisAudioCfg);
                }

                OK_CALIB_RETURN(pResult, "+CACCP: ", pParam->nDLCI);
            }
            case 5:
            {
                strcpy(nParam,"+CACCP: " );
                pParambuff = nParam+strlen("+CACCP: ");
                *pParambuff ='"';
                if(nITF>=10)
                    param_len = calib_GetAudioCodecSidtone(nITF-10, pParambuff+1);
                else
                    param_len = calib_GetAudioCodecSidtone(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);
            }

            case 6:
            {
                if(nITF>=10)
                {
                    calib_SetAudioMusicInGainsRecord(nITF-10, nParam, param_len);
                    aud_Setup(nITF-10, pAudioLevelCfg);
                }
                else
                {
                    calib_SetAudioMusicInGainsRecord(nITF, nParam, param_len);
                    BTSco_Setup(nITF, (VOIS_AUDIO_CFG_T *)pVoisAudioCfg);
                }

                OK_CALIB_RETURN(pResult, "+CACCP: ", pParam->nDLCI);
            }
            case 7:
            {
                strcpy(nParam,"+CACCP: " );
                pParambuff = nParam+strlen("+CACCP: ");
                *pParambuff ='"';
                if(nITF>=10)
                    param_len =calib_GetAudioMusicInGainsRecord(nITF-10, pParambuff+1);
                else
                    param_len =calib_GetAudioMusicInGainsRecord(nITF, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);
            }

            default:
            {

                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
            }
        }
    }
    else
    {

        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }

_exit:
    if (nParam)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
        nParam = NULL;
#else
        AT_FREE(nParam);
#endif
    }
	hal_HstSendEvent(SYS_EVENT, 0x20161500);
    return;
}

VOID AT_EMOD_CmdFunc_CAMCP(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 iIndex = 0;
    UINT8 nITF = 0;
    UINT8 nCtrl = 0;
    UINT16 param_len = 1;
    UINT8 Rsp[20] = {0,} ;

    UINT8 *nParam = NULL;

    hal_HstSendEvent(SYS_EVENT, 0x20161403);

    //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
    nParam = AudioOverlayBuf;
#else
    nParam = (UINT8*)AT_MALLOC(AT_EMOD_BUFFER_SIZE);
#endif

    if(!nParam)
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);

    memset(nParam,0x00,AT_EMOD_BUFFER_SIZE);

    if (pParam->iType == AT_CMD_TEST)
    {
        OK_CALIB_RETURN(pResult, "+CAMCP ", pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        strcpy(Rsp, "Not Support");
        OK_CALIB_RETURN(pResult, Rsp, pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_SET)
    {
        if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (uParaCount != 3)
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nITF, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 1;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nCtrl, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = AT_EMOD_BUFFER_SIZE;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_STRING, nParam, &param_len))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        //AT_TC(g_sw_SPEC, "------%s, param_len=%d", __FUNCTION__, param_len);
        //AT_TC(g_sw_SPEC, "------%s, nParam len=%d", __FUNCTION__, strlen(nParam));

        if(!_CheckITFValue(&nITF))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
		
        hal_HstSendEvent(SYS_EVENT, 0x20161460);
        hal_HstSendEvent(SYS_EVENT, nCtrl);
	    hal_HstSendEvent(SYS_EVENT, nITF);
		hal_HstSendEvent(SYS_EVENT, musicItf);
		hal_HstSendEvent(SYS_EVENT, audioItf);
		
        if(!((0 <= nCtrl)&&(nCtrl <= 5)))
        {
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if(nCtrl % 2 == 0)
        {
            if((nITF-10) != (musicItf))
            {
                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_ITF_DIFFERENT, pParam->nDLCI);
            }
        }

        UINT8 *pParambuff;
		CALIB_CALIBRATION_T * calibPtr = tgt_GetCalibConfig();
        switch(nCtrl)
        {
            case 0:
				hal_HstSendEvent(SYS_EVENT, 0x20161471);

                calib_SetAudioMusicEQParam(nITF-10, nParam, param_len);

			    //set eq flag
				vpp_Mp3DecSetAudioParamReloadFlag();
				vpp_SBCDecSetAudioParamReloadFlag(3);// 2:key set eq; 3:calib set eq;

				hal_HstSendEvent(SYS_EVENT, 0x20161475);
				hal_HstSendEvent(SYS_EVENT, param_len);

                OK_CALIB_RETURN(pResult, "+CAMCP: ", pParam->nDLCI);

            case 1:
				
				hal_HstSendEvent(SYS_EVENT, 0x20161461);
				
                strcpy(nParam,"+CAMCP: " );
                pParambuff = nParam+strlen("+CAMCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioMusicEQParam(nITF-10, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;

				hal_HstSendEvent(SYS_EVENT, 0x20161463);
				hal_HstSendEvent(SYS_EVENT, param_len);

                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            case 2:
                calib_SetAudioMusicALCParam(nITF-10, nParam, param_len);
				hal_HstSendEvent(SYS_EVENT, 0x02201658);
				vpp_DecSetDrcmodeParam(calibPtr->aud.audioMusic[(UINT8)calibPtr->aud.audioMusicItfMap[nITF-10]].drc,0);
				hal_HstSendEvent(SYS_EVENT, 0x02201659);
                OK_CALIB_RETURN(pResult, "+CAMCP: ", pParam->nDLCI);

            case 3:
                strcpy(nParam,"+CAMCP: " );
                pParambuff = nParam+strlen("+CAMCP: ");
                *pParambuff = '"';
                param_len = calib_GetAudioMusicALCParam(nITF-10, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            default:
                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
    }
    else
    {
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }

_exit:
    if (nParam)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
        nParam = NULL;
#else
        AT_FREE(nParam);
#endif
    }

    return;
}

VOID AT_EMOD_CmdFunc_CAWTF(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 iIndex = 0;
    UINT8 nITF = 0;
    UINT8 nCtrl = 0;
    UINT16 param_len = 1;
    UINT8 Rsp[20] = {0,} ;

    hal_HstSendEvent(SYS_EVENT, 0x20161404);

    if (pParam->iType == AT_CMD_TEST)
    {
        OK_RETURN(pResult, "+CAWTF ", pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        strcpy(Rsp, "Not Support");
        OK_RETURN(pResult, Rsp, pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_SET)
    {
        if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (uParaCount != 3)
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nITF, &param_len))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 1;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nCtrl, &param_len))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }


        if(!(0 == nITF))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if(!(0 == nCtrl))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        switch(nCtrl)
        {
            case 0:
                if(!calib_BurnFlash())
                {
#ifdef CHIP_HAS_AP
                    ERROR_RETURN(pResult, ERR_AT_CME_BURN_FLASH_FAIL, pParam->nDLCI);
#else
                    ERROR_CALIB_RESET_RETURN(pResult, ERR_AT_CME_BURN_FLASH_FAIL, pParam->nDLCI);
                    //hal_TimDelay(5 MS_WAITING);
                    //hal_SysShutdown();
#endif
                }

#ifdef CHIP_HAS_AP
                OK_RETURN(pResult, "+CAWTF: ", pParam->nDLCI);
#else
                OK_CALIB_RESET_RETURN(pResult, "+CAWTF: ", pParam->nDLCI);
                //hal_TimDelay(5 MS_WAITING);
                //hal_SysShutdown();
                break;
#endif

            default:
                ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
    }
    else
    {
        ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }
}

PRIVATE VOID AT_FREE_Buffer(VOID)
{
    UINT8 *pBuffer = calib_GetBufferPointers();
    if (pBuffer)
    {
        sxr_Free(pBuffer);
    }

    return;
}

VOID AT_EMOD_CmdFunc_CAIET(AT_CMD_PARA *pParam)
{
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 iIndex = 0;
    UINT8 nITF = 0;
    UINT8 nCtrl = 0;
    UINT16 nOffset = 0;
    UINT16 nLength = 0;
    UINT16 param_len = 1;
    UINT8 Rsp[20] = {0,} ;

    UINT8 *nParam = NULL;

    hal_HstSendEvent(SYS_EVENT, 0x20161405);

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
    nParam = AudioOverlayBuf;
#else
    nParam = (UINT8*)AT_MALLOC(AT_EMOD_BUFFER_SIZE);
#endif

    if(!nParam)
    {
        AT_FREE_Buffer();
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }

    memset(nParam,0x00,AT_EMOD_BUFFER_SIZE);

    if (pParam->iType == AT_CMD_TEST)
    {
        AT_FREE_Buffer();
        OK_CALIB_RETURN(pResult, "+CAIET ", pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        AT_FREE_Buffer();
        strcpy(Rsp, "Not Support");
        OK_CALIB_RETURN(pResult, Rsp, pParam->nDLCI);
    }
    else if (pParam->iType == AT_CMD_SET)
    {
        if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (uParaCount != 5)
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nITF, &param_len))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 1;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT8, &nCtrl, &param_len))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 2;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT16, &nOffset, &param_len))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = 2;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_UINT16, &nLength, &param_len))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        param_len = AT_EMOD_BUFFER_SIZE;
        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, iIndex++, AT_UTIL_PARA_TYPE_STRING, nParam, &param_len))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        //AT_TC(g_sw_SPEC, "nOffset=%d,nLength=%d", nOffset,nLength);

        if(!(0 == nITF))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if(!((0 <= nCtrl)&&(nCtrl <= 1)))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if((nOffset < 0))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        if((nLength < 0))
        {
            AT_FREE_Buffer();
            ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }

        UINT8 *pParambuff;

        switch(nCtrl)
        {
            case 0:
                if((nLength * 2) != param_len)
                {
                    AT_FREE_Buffer();
                    ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
                }
                else if((nOffset + nLength) > sizeof(CALIB_BUFFER_T))
                {
                    AT_FREE_Buffer();
                    ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
                }
                else if((nOffset + nLength) == sizeof(CALIB_BUFFER_T))
                {
                    if(!calib_SetBufferData(nOffset, nLength, nParam))
                    {
                        AT_FREE_Buffer();
                        ERROR_CALIB_RESET_RETURN(pResult, ERR_AT_CME_BURN_FLASH_FAIL, pParam->nDLCI);
                        //hal_TimDelay(5 MS_WAITING);
                        //hal_SysShutdown();
                    }

                    OK_CALIB_RESET_RETURN(pResult, "+CAIET: ", pParam->nDLCI);
                    goto _exit; 

                    //hal_TimDelay(5 MS_WAITING);
                    //hal_SysShutdown();
                }
                else
                {
                    if(!calib_SetBufferData(nOffset, nLength, nParam))
                    {
                        AT_FREE_Buffer();
                        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_BURN_FLASH_FAIL, pParam->nDLCI);
                    }

                    OK_CALIB_RETURN(pResult, "+CAIET: ", pParam->nDLCI);
                }

            case 1:
                strcpy(nParam,"+CAIET: " );
                pParambuff = nParam+strlen("+CAIET: ");
                *pParambuff = '"';
                param_len = calib_GetBufferData(nOffset, nLength, pParambuff+1);
                pParambuff[param_len+1]= '"';
                pParambuff[param_len+2]= 0;
                OK_CALIB_RETURN(pResult, nParam, pParam->nDLCI);

            default:
                AT_FREE_Buffer();
                ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
        }
    }
    else
    {
        AT_FREE_Buffer();
        ERROR_CALIB_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
    }

_exit:
    if (nParam)
    {
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_5856E)    
        nParam = NULL;
#else
        AT_FREE(nParam);
#endif
    }

    return;
}

#endif
#endif

