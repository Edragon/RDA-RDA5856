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
// #include <hal.h>
#include "aud_m.h"
#include "at.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_audio.h"
#include "at_cfg.h"
#include "dm_audio.h"

/**************************************************************************************************
 *                                          CONSTANTS
 **************************************************************************************************/
// AT_20071113_CAOW_B
const UINT32 AT_Sound1_mid[] =
{
	/*#include "chd10_mid.ring.txt"*/
};

const UINT32 AT_Sound2_mid[] =
{
	/*#include "chd11_mid.ring.txt"*/
};

const UINT32 AT_Sound3_mid[] =
{
	/*#include "s1imy05_imy.ring.txt"*/
};

// AT_20071113_CAOW_E

/**************************************************************************************************
 *                                           MACROS
 **************************************************************************************************/
// dm_tone do not support bluetooth_nr and headphone
// supportted by at
#define DM_AUDIO_MODE_BLUETOOTH_NR 4
#define DM_AUDIO_MODE_HEADPHONE    5

/**************************************************************************************************
 *                                          TYPEDEFS
 **************************************************************************************************/

/**************************************************************************************************
 *                                       GLOBAL VARIABLES
 **************************************************************************************************/
extern BOOL dm_ResumMic(VOID);
extern BOOL dm_MuteMic(VOID);

extern AUD_LEVEL_T audio_cfg;

extern BOOL Speeching;

// AT_20071217_CAOW_B for bug#7226
#if 0
UINT8 g_VGR_Volume = 5;
UINT8 g_VGT_Volume = 5;
#else
// UINT8 g_VGR_Volume = 6;
// UINT8 g_VGT_Volume = 10;
#endif
// AT_20071217_CAOW_E
// UINT8 g_CMUT_mute = 0;
// UINT8 g_nAudioTestMode = AT_AUDIO_TEST_EARPIECE;
BOOL g_bTesting = FALSE;  // added by yangtt at 2008-05-05 for bug 8274

struct
{
	UINT8 nSpeakerGain;
	UINT8 nMicGain;
	UINT8 nSideGain;
} g_stGain =
{
	0, 0, 0
};

#ifdef AT_DUAL_SIM
VOID AT_Result_OK(UINT32 uReturnValue,
                  UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize, UINT8 nDLCI, UINT8 nSim)
#else
VOID AT_Result_OK(UINT32 uReturnValue, UINT32 uResultCode, UINT8 nDelayTime, UINT8 *pBuffer, UINT16 nDataSize,
                  UINT8 nDLCI)
#endif
{

	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	AT_BACKSIMID(nSim);
#endif
	// 填充结果码
	pResult = AT_CreateRC(uReturnValue,
	                      uResultCode, CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, nDelayTime, pBuffer, nDataSize, nDLCI);

	AT_Notify2ATM(pResult, nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

#ifdef AT_DUAL_SIM
VOID AT_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI, UINT8 nSim)
#else
VOID AT_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
#endif
{
	PAT_CMD_RESULT pResult = NULL;

#ifdef AT_DUAL_SIM
	AT_BACKSIMID(nSim);
#endif
	// 填充结果码
	pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR, uErrorCode, nErrorType, 0, 0, 0, nDLCI);

	AT_Notify2ATM(pResult, nDLCI);

	if (pResult != NULL)
	{
		AT_FREE(pResult);
		pResult = NULL;
	}

	return;
}

VOID AT_AUDIO_CmdFunc_VGR(AT_CMD_PARA *pParam)
{
	INT32 iRetValue;
	UINT8 uParaCount;
	UINT8 uData;
	UINT8 uStrLen;
	BOOL iRet;
	UINT8 uOutputStr[20] = { 0 };

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	// Check [in] Param

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET:

			if ((pParam->pPara == NULL) || (ERR_SUCCESS != (iRetValue = AT_Util_GetParaCount(pParam->pPara, &uParaCount)))) /* GET param count */
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
			else
			{
				if (1 != uParaCount)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				uStrLen = 1;

				iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uData, &uStrLen);

				if (iRetValue != ERR_SUCCESS)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (uData > 8)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

#ifdef CHIP_HAS_AP
				if(FALSE == Speeching)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}
#endif


#if 0

				if(uData == 0x00)
				{
					iRet = DM_SetAudioVolume(uData);
				}
				else if(uData > 100)
				{
					uData = 100;
				}

				iRet = DM_SetAudioVolume((uData - 1) * 7 / 100 + 1);

#else
				switch (uData)
				{
					// Remove spk gain maybe some problem by lixp 20120817
					/*
					        case 0:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m15dB);
					          break;

					        case 1:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m12dB);
					          break;

					        case 2:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m9dB);
					          break;

					        case 3:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m6dB);
					          break;

					        case 4:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m3dB);
					          break;
					*/
				case 5:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_0dB);
					break;

				case 6:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_3dB);
					break;

				case 7:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_6dB);
					break;

				case 8:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_MUTE);
					break;

				default:
					iRet = FALSE;
					break;
				}
#endif


				if (!iRet)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				}
				else
				{
					gATCurrentVGR_Volume = uData;
#ifdef AT_DUAL_SIM
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
				}

				return;
			}

			break;

		case AT_CMD_TEST:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrCat(uOutputStr, "+VGR: (0-8)");
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		case AT_CMD_READ:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrPrint(uOutputStr, "+VGR: %d", gATCurrentVGR_Volume);
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		default:
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
			break;

		}
	}

	return;
}

VOID AT_AUDIO_CmdFunc_VGT(AT_CMD_PARA *pParam)
{
	INT32 iRetValue;
	UINT8 uParaCount;
	UINT8 uData;
	UINT8 uStrLen;
	BOOL iRet;
	UINT8 uOutputStr[20] = { 0 };

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	// Check [in] Param

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET:

			if ((pParam->pPara == NULL) || (ERR_SUCCESS != (iRetValue = AT_Util_GetParaCount(pParam->pPara, &uParaCount)))) /* GET param count */
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
			else
			{
				if (1 != uParaCount)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				uStrLen = 1;

				iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uData, &uStrLen);

				if (iRetValue != ERR_SUCCESS)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (uData > 16)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

#ifdef CHIP_HAS_AP
				if(FALSE == Speeching)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}
#endif

				switch (uData)
				{
					// Remove spk gain maybe some problem by lixp 20120817
					/*case 0:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_0dB);
					          break;

					        case 1:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_3dB);
					          break;

					        case 2:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_6dB);
					          break;

					        case 3:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_9dB);
					          break;

					        case 4:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_12dB);
					          break;

					        case 5:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_15dB);
					          break;

					        case 6:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_18dB);
					          break;

					        case 7:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_21dB);
					          break;

					        case 8:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_24dB);
					          break;

					        case 9:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_27dB);
					          break;

					        case 10:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_30dB);
					          break;

					        case 11:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_33dB);
					          break;

					        case 12:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_36dB);
					          break;

					        case 13:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_39dB);
					          break;

					        case 14:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_42dB);
					          break;

					        case 15:
					          iRet = DM_SetMicGain(DM_AUDIO_MIC_45dB);
					          break;*/

				case 16:
					iRet = DM_SetMicGain(DM_AUDIO_MIC_MUTE);
					break;

				default:
					iRet = FALSE;
					break;
				}

				if (!iRet)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				}
				else
				{
					gATCurrentVGT_Volume = uData;
#ifdef AT_DUAL_SIM
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
				}

				return;
			}

			break;

		case AT_CMD_TEST:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrCat(uOutputStr, "+VGT: (0-16)");

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		case AT_CMD_READ:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrPrint(uOutputStr, "+VGT: %d", gATCurrentVGT_Volume);

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		default:
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
			break;

		}
	}

	return;
}

VOID AT_AUDIO_CmdFunc_CMUT(AT_CMD_PARA *pParam)
{
	INT32 iRetValue;
	UINT8 uParaCount;
	UINT8 uData;
	UINT8 uStrLen;
	BOOL iRet            = FALSE;
	UINT8 uOutputStr[20] = { 0 };

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	// Check [in] Param

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET:
			if ((pParam->pPara == NULL) || (ERR_SUCCESS != (iRetValue = AT_Util_GetParaCount(pParam->pPara, &uParaCount)))) /* GET param count */
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
#ifdef CHIP_HAS_AP
			else if(FALSE == Speeching)
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

			}
#endif
			else
			{
				if (1 != uParaCount)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				uStrLen = 1;

				iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uData, &uStrLen);

				if (iRetValue != ERR_SUCCESS)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (uData > 1)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (0 == uData)
				{
					// AT_20071122_CAOW_B  for bug#7091
					if (gATCurrentCMUT_mute != 0)
					{
						// AT_20071122_CAOW_E
						iRet = dm_ResumMic();

						if (!iRet)
						{
#ifdef AT_DUAL_SIM
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
							return;
						}

						gATCurrentCMUT_mute = 0;
					}

				}
				else if (1 == uData)
				{
					// AT_20071122_CAOW_B for bug#7091
					if (gATCurrentCMUT_mute != 1)
					{
						// AT_20071122_CAOW_E
						iRet = dm_MuteMic();

						if (!iRet)
						{
#ifdef AT_DUAL_SIM
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
							return;
						}

						gATCurrentCMUT_mute = 1;
					}

				}

#ifdef AT_DUAL_SIM
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif

				return;
			}

			break;

		case AT_CMD_TEST:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrCat(uOutputStr, "+CMUT: (0,1)");

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		case AT_CMD_READ:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrPrint(uOutputStr, "+CMUT: %d", gATCurrentCMUT_mute);

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		default:
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
			break;

		}
	}

	return;
}

VOID AT_AUDIO_CmdFunc_CLVL(AT_CMD_PARA *pParam)
{
	INT32 iRetValue;
	UINT8 uParaCount;
	UINT8 uData;
	UINT8 uSoundType;// add for android SetCallVolume(HRilClient client, SoundType type, int vol_level),value is (0-3),
	UINT8 uStrLen;
	BOOL iRet;
	UINT8 uOutputStr[20] = { 0 };

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	// Check [in] Param

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}
	else
	{
		switch (pParam->iType)
		{

		case AT_CMD_SET:

			if ((pParam->pPara == NULL) || (ERR_SUCCESS != (iRetValue = AT_Util_GetParaCount(pParam->pPara, &uParaCount)))) /* GET param count */
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}
			else
			{
				if ((uParaCount > 2) || (uParaCount < 1))
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				uStrLen = 1;

				iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uData, &uStrLen);

				if (iRetValue != ERR_SUCCESS)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				iRetValue = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT8, &uSoundType, &uStrLen);

				if (iRetValue != ERR_SUCCESS)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}
				//need more work to use uSoundType
				//



#ifdef CHIP_HAS_AP
				//Remove for changing speaker Gain parameter to index value
				/*
						if(uData <= 0)
							uData = 0;
						else if(uData >= 100)
							uData = 100;
						else
							uData = (uData - 1)*7/100+1;
				*/
				iRet = DM_SetAudioVolume(uData);
#else
				switch (uData)
				{
					// Remove spk gain maybe some problem by lixp 20120817
					/* case 0:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m15dB);
					          break;

					        case 1:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m12dB);
					          break;

					        case 2:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m9dB);
					          break;

					        case 3:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m6dB);
					          break;

					        case 4:
					          iRet = DM_SetAudioVolume(DM_AUDIO_SPK_m3dB);
					          break;*/

				case 5:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_0dB);
					break;

				case 6:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_3dB);
					break;

				case 7:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_6dB);
					break;

				case 8:
					iRet = DM_SetAudioVolume(DM_AUDIO_SPK_MUTE);
					break;

				default:
					iRet = FALSE;
					break;
				}
#endif
				if (!iRet)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				}
				else
				{
					gATCurrentVGR_Volume = uData;
#ifdef AT_DUAL_SIM
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
				}

				return;
			}

			break;

		case AT_CMD_TEST:
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrCat(uOutputStr, "+CLVL: (0-8)");

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			break;

		case AT_CMD_READ:
			AT_TC(g_sw_SPEC, "uOutputStr adress ======= %x", uOutputStr);
			SUL_ZeroMemory8(uOutputStr, 20);

			SUL_StrPrint(uOutputStr, "+CLVL: %d", gATCurrentVGR_Volume);

#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, uOutputStr, AT_StrLen(uOutputStr), pParam->nDLCI);
#endif
			AT_TC(g_sw_SPEC, "uOutputStr adress ======= %x", uOutputStr);
			break;

		default:
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
			break;

		}
	}

	return;
}

// AT_20071113_CAOW_B
VOID AT_AUDIO_CmdFunc_CRMP(AT_CMD_PARA *pParam)
{
	UINT8 arrRes[40];
	UINT8 iRet       = 0;
	UINT8 uParaCount = 0;
	UINT32 eParamOk  = 0;
	UINT8 uCallType  = 0;
	UINT8 uVolume    = 5;
	UINT8 uType      = 0;
	UINT8 uIndex     = 0;
	UINT8 uStrLen    = 0;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_MemZero(arrRes, sizeof(arrRes));
		AT_StrCpy(arrRes, "+CRMP: (0-3), (0-11), 0, (0-11)");
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
#endif
		return;
	}
	else if (pParam->iType == AT_CMD_SET)
	{
		if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		/* check para count */
		if ((1 > uParaCount) || (4 < uParaCount))
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		if (uParaCount == 3)  // if param count equal to 3,then the forth param must be filled
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		/* get all param */
		uStrLen = 1;

		eParamOk = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &uCallType, &uStrLen);

		if (eParamOk != ERR_SUCCESS)  /* get uCallType failed */
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		/* get uCallType successful */
		if (uCallType > 3)  /* veritify uCallType */
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		if (uParaCount > 1)
		{
			uStrLen  = 1;
			eParamOk = AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT8, &uVolume, &uStrLen);

			if (eParamOk != ERR_SUCCESS)  /* get uVolume failed */
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}

			if (uVolume > 11)
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}

			if (uParaCount > 2)
			{
				uStrLen  = 1;
				eParamOk = AT_Util_GetParaWithRule(pParam->pPara, 2, AT_UTIL_PARA_TYPE_UINT8, &uType, &uStrLen);

				if (eParamOk != ERR_SUCCESS)  /* get uType failed */
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (uType != 0)
				{
#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
					return;
				}

				if (uParaCount > 3)
				{
					uStrLen  = 1;
					eParamOk = AT_Util_GetParaWithRule(pParam->pPara, 3, AT_UTIL_PARA_TYPE_UINT8, &uIndex, &uStrLen);

					if (eParamOk != ERR_SUCCESS)  /* get uIndex failed */
					{
#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
						AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
						return;
					}

					if (uIndex > 11)
					{
#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
						AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
						return;
					}
				}
			}
		}

		AT_TC(g_sw_SPEC, "uParaCount = %d,uCallType = %d,uVolume = %d,uType = %d,uIndex = %d,", uParaCount, uCallType,
		      uVolume, uType, uIndex);

#ifdef CHIP_HAS_AP

		if(FALSE == Speeching)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
#endif

		if (uParaCount == 4 && uIndex == 0)
		{
			if (DM_Audio_StopStream())
			{
#ifdef AT_DUAL_SIM
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
			}
			else
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			}
		}
		else
		{
			// AT_20071217_CAOW_B for bug#7231
			if (!DM_Audio_StopStream())
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			}

			// AT_20071217_CAOW_E

			if (!DM_SetAudioVolume(uVolume))
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			}

			if (uCallType == 0)
			{
				switch (uIndex)
				{

				case 1:
					// AT_20071217_CAOW_B for bug#7229
#if 0
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound1_mid, sizeof(AT_Sound2_mid) / 4, DM_DICTAPHONE_MR122, 1);
#else
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound1_mid, sizeof(AT_Sound1_mid) / 4, DM_DICTAPHONE_MR122, 1);
#endif
					// AT_20071217_CAOW_E
					break;

				case 2:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound2_mid, sizeof(AT_Sound2_mid) / 4, DM_DICTAPHONE_MR122, 1);
					break;

				case 3:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound3_mid, sizeof(AT_Sound3_mid) / 4, DM_DICTAPHONE_MR122, 1);
					break;

				case 4:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound2_mid, sizeof(AT_Sound2_mid) / 4, DM_DICTAPHONE_MR122, 1);
					break;

				default:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound1_mid, sizeof(AT_Sound1_mid) / 4, DM_DICTAPHONE_MR122, 1);
					break;
				}
			}
			else if (uCallType == 1)
			{
				/* switch()
				   {
				   case:
				   break;
				   default:
				   break;
				   } */
				// DM_Audio_StartStream(UINT32 * buffer, UINT32 len, DM_PlayRingType codec, BOOL loop_mode);
			}
			else if (uCallType == 2)
			{
				/* switch()
				   {
				   case:
				   break;
				   default:
				   break;
				   } */
				// DM_Audio_StartStream(UINT32 * buffer, UINT32 len, DM_PlayRingType codec, BOOL loop_mode);
			}
			else if (uCallType == 3)
			{
				switch (uIndex)
				{

				case 1:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound1_mid, sizeof(AT_Sound1_mid) / 4, DM_DICTAPHONE_MR122, 0);
					break;

				case 2:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound2_mid, sizeof(AT_Sound2_mid) / 4, DM_DICTAPHONE_MR122, 0);
					break;

				case 3:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound3_mid, sizeof(AT_Sound3_mid) / 4, DM_DICTAPHONE_MR122, 0);
					break;

				case 4:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound2_mid, sizeof(AT_Sound2_mid) / 4, DM_DICTAPHONE_MR122, 0);
					break;

				default:
					iRet = DM_Audio_StartStream((UINT32 *)AT_Sound1_mid, sizeof(AT_Sound1_mid) / 4, DM_DICTAPHONE_MR122, 0);
					break;
				}
			}

			if (iRet)
			{
#ifdef AT_DUAL_SIM
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
			}
			else
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			}
		}

	}

	// AT_20071217_CAOW_B for bug#7210
	else
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	// AT_20071217_CAOW_E

}

// AT_20071113_CAOW_E

extern BOOL Speeching;

// yangtt at 2008-04-25 for bug 8171
BOOL AT_GC_GetAudioTest(VOID)
{
	if (g_bTesting == TRUE)
		return FALSE;
	else
		return TRUE;

}

// ^aust: select and execute audio cycle test mode :0--aux,1--mic,2--loud
VOID AT_AUDIO_CmdFunc_AUST(AT_CMD_PARA *pParam)
{
	/*
		UINT8 	arrRes[20];
		INT32 	iRet = ERR_SUCCESS;
		UINT8 	paraCount = 0;
		UINT8     nTestMode = 0;
	#ifdef AT_DUAL_SIM
		UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	#endif

		if (pParam == NULL)
		{

	#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
			return;
		}

		if (pParam->iType == AT_CMD_TEST)
		{
			AT_MemZero(arrRes, sizeof(arrRes));
			AT_StrCpy(arrRes, "+AUST: (0-2)"); // yangtt at 2008-04-28 for bug 8224
	#ifdef AT_DUAL_SIM
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes),pParam->nDLCI, nSim);
	#else
					AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
	#endif
			return;
		}
		else if (pParam->iType == AT_CMD_SET)
		{
			//if testing audio cycle, operation is not allowed.
			if (g_bTesting)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			//if cc is on, operation is not allowed.
			CFW_CC_CURRENT_CALL_INFO arrCurrentCall[7];

			UINT8 nCount = 0;
	#ifdef AT_DUAL_SIM

			UINT32 nResult = 	CFW_CcGetCurrentCall(arrCurrentCall, &nCount,nSim);
	#else
			UINT32 nResult =	CFW_CcGetCurrentCall(arrCurrentCall, &nCount);

	#endif

			//if (nResult != ERR_SUCCESS) //change by wulc

			if (nResult == ERR_SUCCESS)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			if (nCount != 0)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			iRet = AT_Util_GetParaCount(pParam->pPara, &paraCount);

			if (iRet != ERR_SUCCESS || paraCount != 1)
			{

	#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			UINT8 nParamLen = 1;

			iRet = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8,
										   &nTestMode , &nParamLen);

			if ((iRet != ERR_SUCCESS) || (nTestMode > 2))
			{

	#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			gATCurrentnAudioTestMode = nTestMode;

			DM_SetAudioMode(gATCurrentnAudioTestMode);  // add  by yangtt
	#ifdef AT_DUAL_SIM
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0,pParam->nDLCI, nSim);
	#else
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
	#endif
			return;
		}
		else if (pParam->iType == AT_CMD_EXE)
		{
			//if testing audio cycle, operation is not allowed.
			if (g_bTesting)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			//if cc is on, operation is not allowed.
			CFW_CC_CURRENT_CALL_INFO arrCurrentCall[7];

			UINT8 nCount = 0;
	#ifdef AT_DUAL_SIM

			UINT32 nResult = 	CFW_CcGetCurrentCall(arrCurrentCall, &nCount,nSim);
	#else
			UINT32 nResult =	CFW_CcGetCurrentCall(arrCurrentCall, &nCount);

	#endif

				if (nResult == ERR_SUCCESS)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			if (nCount != 0)
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			Speeching = TRUE;  // add by ytt

			//store current gains
			//g_stGain.nSpeakerGain = audio_cfg.spkGain;
			//g_stGain.nMicGain = audio_cfg.micGain;
			//g_stGain.nSideGain = audio_cfg.sideGain;

			AT_TC(g_sw_SPEC, "^AUST :gATCurrentnAudioTestMode--%d\n", gATCurrentnAudioTestMode);

			if (gATCurrentnAudioTestMode == AT_AUDIO_TEST_EARPIECE)
			{
				CFW_EmodAudioTestStart(AT_AUDIO_TEST_EARPIECE_CFW);
			}
			else if (gATCurrentnAudioTestMode == AT_AUDIO_TEST_HANDSET)
			{
				CFW_EmodAudioTestStart(AT_AUDIO_TEST_HANDSET_CFW);
			}
			else if (gATCurrentnAudioTestMode == AT_AUDIO_TEST_LOUDSPEAKER)
			{
				CFW_EmodAudioTestStart(AT_AUDIO_TEST_LOUDSPEAKER_CFW);
			}
			else
			{

	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
				return;
			}

			//set sidegain as -6db
			//DM_SetSideGain(DM_AUDIO_SIDE_m6dB);
			//change by wulc because #define DM_SetSideGain DM_SetAudioVolume  in dm_tone.c
			DM_SetAudioVolume(DM_AUDIO_SIDE_m6dB);

			g_bTesting = TRUE;

	#ifdef AT_DUAL_SIM
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0,pParam->nDLCI, nSim);
	#else
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
	#endif

			return;
		}
		else
		{
	#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
						AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif

			return;
		}
	*/
	return;

}

// ^auend: end audio cycle test
VOID AT_AUDIO_CmdFunc_AUEND(AT_CMD_PARA *pParam)
{
	/*
	#ifdef AT_DUAL_SIM
		UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
	#endif
		if (pParam == NULL)
		{
	#ifdef AT_DUAL_SIM
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
			return;
		}

		if (pParam->iType == AT_CMD_EXE)
		{
			if (g_bTesting)
			{
				AT_TC(g_sw_SPEC, "^AUEND :gATCurrentnAudioTestMode--%d\n", gATCurrentnAudioTestMode);
				CFW_EmodAudioTestEnd();
				g_bTesting = FALSE;
				Speeching = FALSE;      // add by ytt
				//restore to previous testmode and gains
				gATCurrentnAudioTestMode = AT_AUDIO_TEST_EARPIECE;
				DM_SetAudioMode(gATCurrentnAudioTestMode);// add by yangtt for bug 8029 at 04-18

				//set speaker gain

				//if (!DM_SetSpeakerGain(g_stGain.nSpeakerGain))// remove by wulc
				if (!DM_SetAudioVolume(g_stGain.nSpeakerGain))
				{
					COS_Sleep(10);

					//if (!DM_SetSpeakerGain(g_stGain.nSpeakerGain))
					if (!DM_SetAudioVolume(g_stGain.nSpeakerGain))
					{
	#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
						return;
					}
				}

				//set mic gain
				if (!DM_SetMicGain(g_stGain.nMicGain))
				{
					COS_Sleep(10);

					if (!DM_SetMicGain(g_stGain.nMicGain))
					{
	#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
						return;
					}
				}

				//set side gain
				//if (!DM_SetSideGain(g_stGain.nSideGain)) remove by wulc
				if (!DM_SetAudioVolume(g_stGain.nSideGain))
				{
					COS_Sleep(10);

					//if (!DM_SetSideGain(g_stGain.nSideGain))
					if (!DM_SetAudioVolume(g_stGain.nSideGain))
					{
	#ifdef AT_DUAL_SIM
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif
						return;
					}
				}
			}

	#ifdef AT_DUAL_SIM
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0,pParam->nDLCI, nSim);
	#else
						AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
	#endif

			return;
		}
		else
		{
	#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
	#else
				AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
	#endif

			return;
		}
	*/
	return;

}

// ^saic: set audio gain: speaker gain, mic gain and sidegain
VOID AT_AUDIO_CmdFunc_SAIC(AT_CMD_PARA *pParam)
{
	/*
		INT32 iRet = ERR_SUCCESS;
		UINT8 paraCount = 0;
		UINT8 nParamLen = 0;
		UINT8 operInfoBuf[40];
		UINT8 arrParam[3];
		BOOL  arrIsParamSet[3];

		if (pParam == NULL)
		{
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
			return;
		}

		if (pParam->iType == AT_CMD_TEST)
		{
			AT_MemSet(operInfoBuf, 0, sizeof(operInfoBuf));
			AT_StrCpy(operInfoBuf, "+SAIC: (0-8),(0-16),(0-15)");// yangtt at 2008-04-28 for bug 8224
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI);
			return;
		}
		else if (pParam->iType == AT_CMD_SET)
		{
			iRet = AT_Util_GetParaCount(pParam->pPara, &paraCount);

			if (iRet != ERR_SUCCESS || paraCount != 3) // yangtt 08-23-2008 for bug 8048
			{
				AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
				return;
			}

			if (paraCount == 0)// yangtt 08-23-2008 for bug 8048
			{
				AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, pParam->nDLCI);
				return;
			}

			AT_MemZero(arrParam, sizeof(arrParam));

			AT_MemZero(arrIsParamSet, sizeof(arrIsParamSet));

			//get and check params' validity

			while (paraCount-- > 0)
			{
				nParamLen = 1;
				iRet = AT_Util_GetParaWithRule(pParam->pPara, paraCount, AT_UTIL_PARA_TYPE_UINT8,
											   &arrParam[paraCount], &nParamLen);

				if (nParamLen == 0)// yangtt 08-23-2008 for bug 8048
				{
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
					return;
				}

				if (iRet == ERR_SUCCESS)
				{
					arrIsParamSet[paraCount] = TRUE;

					switch (paraCount)
					{

					case 2:

						if (arrParam[paraCount] > 15)
						{
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
							return;
						}

						break;

					case 1:

						if (arrParam[paraCount] > 16)
						{
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
							return;
						}

						break;

					case 0:

						if (arrParam[paraCount] > 8)
						{
							AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
							return;
						}

						break;

					default:
						break;
					}
				}
				else  if (iRet == ERR_AT_UTIL_CMD_PARA_NULL)
				{
					arrIsParamSet[paraCount] = FALSE;
				}
				else
				{
					AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
					return;
				}
			}

			//set speaker gain
			if (arrIsParamSet[0])
			{
				if (!DM_SetSpeakerGain(arrParam[0]))
				{
					COS_Sleep(10);

					if (!DM_SetSpeakerGain(arrParam[0]))
					{
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
						return;
					}
				}

			}

			//set mic gain
			if (arrIsParamSet[1])
			{
				if (!DM_SetMicGain(arrParam[1]))
				{
					COS_Sleep(10);

					if (!DM_SetMicGain(arrParam[1]))
					{
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
						return;
					}
				}
			}

			//set side gain
			if (arrIsParamSet[2])
			{
				if (!DM_SetSideGain(arrParam[2]))
				{
					COS_Sleep(10);

					if (!DM_SetSideGain(arrParam[2]))
					{
						AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
						return;
					}
				}
			}

			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, NULL, 0, pParam->nDLCI);

			return;
		}
		else if (pParam->iType == AT_CMD_READ)
		{
			//get speaker gain
			if (!DM_GetSpeakerVolume(&arrParam[0]))
			{
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
				return;
			}

			//get mic gain
			//arrParam[1] = audio_cfg.micGain;

			//arrParam[1] = 0;

			//get side gain
			//arrParam[2] = audio_cfg.sideGain;

			//arrParam[2] = 0;

			AT_MemSet(operInfoBuf, 0, sizeof(operInfoBuf));

			AT_Sprintf(operInfoBuf, "+SAIC: %u,%u,%u", arrParam[0], arrParam[1], arrParam[2]); // yangtt at 2008-06-06 for bug 8609

			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI);

			return;
		}
		else
		{
			AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
			return;
		}
		*/
	return;
}

// ^caudio:open(1) or close(0) audio
VOID AT_AUDIO_CmdFunc_CAUDIO(AT_CMD_PARA *pParam)
{
	INT32 iRet      = ERR_SUCCESS;
	UINT8 nAudio    = 0;
	UINT8 paraCount = 0;
	UINT8 nParamLen = 0;
	UINT8 operInfoBuf[20];

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_MemSet(operInfoBuf, 0, sizeof(operInfoBuf));

		// AT_20071220_CAOW_B for bug#7179
		AT_StrCpy(operInfoBuf, "+CAUDIO: (0-1)"); // yangtt at 2008-05-06 for bug 8224
		// AT_20071220_CAOW_E
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI);
#endif

		return;
	}
	else if (pParam->iType == AT_CMD_SET)
	{
		// if testing audio cycle, operation is not allowed.
		if (g_bTesting)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		iRet = AT_Util_GetParaCount(pParam->pPara, &paraCount);

		if (iRet != ERR_SUCCESS || paraCount != 1)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		nParamLen = 1;

		iRet = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nAudio, &nParamLen);

		if ((iRet != ERR_SUCCESS) || nAudio > 1)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

#ifdef CHIP_HAS_AP

		if(FALSE == Speeching)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
#endif

		if (nAudio == 0)
		{
			DM_StopAudio();

			/*
			   if (!DM_StopAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StopAudio-first time--error !\n");
			   COS_Sleep(10);
			   if (!DM_StopAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StopAudio-first time--error !\n");
			   AT_Result_Err(ERR_AT_CME_EXE_FAIL,CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
			   return;
			   }
			   }
			   AT_TC(g_sw_SPEC,"DM_StopAudio success !\n");
			 */
		}
		else
		{
			/*
			   if (!DM_StopAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StopAudio-first time--error !\n");
			   COS_Sleep(10);
			   if (!DM_StopAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StopAudio-first time--error !\n");
			   AT_Result_Err(ERR_AT_CME_EXE_FAIL,CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
			   return;
			   }
			   }
			   AT_TC(g_sw_SPEC,"DM_StopAudio success !\n");
			 */
			DM_StopAudio(); // modified for bug 7025
			DM_StartAudio();

			/*
			   if (!DM_StartAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StartAudio-first time--error !\n");
			   COS_Sleep(10);
			   if (!DM_StartAudio())
			   {
			   AT_TC(g_sw_SPEC,"DM_StartAudio-second time--error !\n");
			   AT_Result_Err(ERR_AT_CME_EXE_FAIL,CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
			   return;
			   }
			   }
			   AT_TC(g_sw_SPEC,"DM_StartAudio success !\n");
			 */
		}

#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif

		return;
	}
	else
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
	}

}

VOID AT_AUDIO_CmdFunc_CRSL(AT_CMD_PARA *pParam)
{
	INT32 iRet = ERR_SUCCESS;

	// UINT8 nAudio  = 0;
	UINT8 paraCount = 0;
	UINT8 nParamLen = 0;
	UINT8 operInfoBuf[20];
	UINT8 volumelevel;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif

	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_MemSet(operInfoBuf, 0, sizeof(operInfoBuf));
		AT_StrCpy(operInfoBuf, "+CRSL: (0-15)");
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI);
#endif
		return;
	}
	else if (pParam->iType == AT_CMD_READ)
	{
		// need more work

		if (DM_GetSpeakerVolume(&volumelevel))
		{

			AT_TC(g_sw_SPEC, "volumelevel--%d\n", volumelevel);
			SUL_ZeroMemory8(operInfoBuf, 20);
			SUL_StrPrint(operInfoBuf, "+CRSL: %d", volumelevel);
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, operInfoBuf, AT_StrLen(operInfoBuf), pParam->nDLCI);
#endif
		}

		return;

	}
	else if (pParam->iType == AT_CMD_SET)
	{

		iRet = AT_Util_GetParaCount(pParam->pPara, &paraCount);

		if (iRet != ERR_SUCCESS || paraCount != 1)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
		nParamLen = 1;

		iRet = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &volumelevel, &nParamLen);

		if ((iRet != ERR_SUCCESS) || volumelevel > 15 || volumelevel < 0)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

#ifdef CHIP_HAS_AP

		if(FALSE == Speeching)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
#endif

		if (DM_SetAudioVolume(volumelevel))
		{
			// need more work
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
			return;

		}

	}
	else
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
	}

}

// ^snfs:select audio hardware set, that is selecting audio mode:0--aux,1--mic,2--loud
VOID AT_AUDIO_CmdFunc_SNFS(AT_CMD_PARA *pParam)
{
	UINT8 nParamLen = 0;
	UINT8 arrRes[20];
	INT32 iRet       = ERR_SUCCESS;
	UINT8 paraCount  = 0;
	UINT8 nAudioMode = 0;

#ifdef AT_DUAL_SIM
	UINT8 nSim = AT_SIM_ID(pParam->nDLCI);
#endif
	if (pParam == NULL)
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
		return;
	}

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_MemZero(arrRes, sizeof(arrRes));
		AT_StrCpy(arrRes, "+SNFS: (0-2)");  // yangtt at 2008-04-28 for bug 8224
#ifdef AT_DUAL_SIM
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI, nSim);
#else
		AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
#endif
		return;
	}
	else if (pParam->iType == AT_CMD_SET)
	{
		// if testing audio cycle, operation is not allowed.
		if (g_bTesting)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		iRet = AT_Util_GetParaCount(pParam->pPara, &paraCount);

		if (iRet != ERR_SUCCESS || paraCount != 1)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		nParamLen = 1;

		iRet = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &nAudioMode, &nParamLen);

		if (iRet != ERR_SUCCESS)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		if (DM_AUDIO_MODE_HEADPHONE == nAudioMode)
		{
			if(!DM_AudForceReceiverMicSelection(TRUE))
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}

			// conver to headset ( android call headset, we call earpiece, why ?)
			nAudioMode = DM_AUDIO_MODE_HANDSET;
		}
		else
		{
			if(!DM_AudForceReceiverMicSelection(FALSE))
			{
#ifdef AT_DUAL_SIM
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
				AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
				return;
			}

		}


		if (!DM_SetAudioMode(nAudioMode))
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
		else
		{
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, 0, 0, pParam->nDLCI);
#endif
			return;
		}

	}
	else if (pParam->iType == AT_CMD_READ)
	{
		// if testing audio cycle, operation is not allowed.
		if (g_bTesting)
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_OPERATION_NOT_ALLOWED, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}

		if (!DM_GetAudioMode(&nAudioMode))
		{
#ifdef AT_DUAL_SIM
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
			AT_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif
			return;
		}
		else
		{
			AT_TC(g_sw_SPEC, "+SNFS :nAudioMode--%d\n", nAudioMode);  // added by yangtt at 2008-6-17 for bug 8609
			AT_MemZero(arrRes, sizeof(arrRes));
			AT_Sprintf(arrRes, "+SNFS: %u", nAudioMode);  // added by yangtt at 2008-6-17 for bug 8609
#ifdef AT_DUAL_SIM
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI, nSim);
#else
			AT_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 0, arrRes, AT_StrLen(arrRes), pParam->nDLCI);
#endif
			return;
		}

	}
	else
	{
#ifdef AT_DUAL_SIM
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI, nSim);
#else
		AT_Result_Err(ERR_AT_CME_EXE_NOT_SURPORT, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
#endif

		return;
	}

}

// added by yangtt at 2008-5-21 for bug 8506 begin
extern void Delay_for_audio(UINT32 time);

// extern BOOL DM_SetSideGain ( DM_SideToneGain nGain);

UINT8 audio_mute = 0;
VOID AT_Audio_Init(VOID)
{
	BOOL ret = 0;

	Delay_for_audio(5);
	ret = DM_SetAudioVolume(gATCurrentVGR_Volume);
	AT_TC(g_sw_SPEC, "  dm_ResumMic  AT_Audio_Init  ret = %d\n", ret);

	if (0 == gATCurrentCMUT_mute)
	{
		Delay_for_audio(5);
		ret = dm_ResumMic();
		AT_TC(g_sw_SPEC, "  dm_ResumMic  AT_Audio_Init  ret = %d\n", ret);
	}
	else if (1 == gATCurrentCMUT_mute)
	{
		Delay_for_audio(5);
		ret        = dm_MuteMic();
		audio_mute = 1;
		AT_TC(g_sw_SPEC, "   dm_MuteMic  AT_Audio_Init  ret =  %d\n", ret);

	}

	Delay_for_audio(5);

	if (audio_mute == 0)
	{
		ret = DM_SetMicGain(gATCurrentVGT_Volume);
		AT_TC(g_sw_SPEC, "    AT_Audio_Init  gATCurrentVGT_Volume =%d gATCurrentVGR_Volume =%d ret = %d\n",
		      gATCurrentVGT_Volume, gATCurrentVGR_Volume, ret);
	}
}

// added by yangtt at 2008-5-21 for bug 8506 end


#ifdef SOUND_RECORDER_SUPPORT

//Add by XP at 20140528 for AT audio record begin

#define AT_RECORD_BUF_SIZE 1024*30
UINT8 gATRecordBuf[AT_RECORD_BUF_SIZE] = {0x00,};
UINT32 nATRecordBufUsedSize = 0;

//MCI_AUDIO_RECORD_BUFFER_PLAY_CALLBACK_T
VOID ATRecordBufPlayCallBack(unsigned char *pBuf, unsigned int len, unsigned int *recordLen)
{
	AT_TC(g_sw_SPEC, "ATRecordBufPlayCallBack addr0x%x,len:%d,result%d", pBuf, len, *recordLen);

	//sxs_Dump(MCI_AUDIO_TRC,0,pBuf,len);
	if((AT_RECORD_BUF_SIZE - nATRecordBufUsedSize) >= len)
	{
		SUL_MemCopy8(&gATRecordBuf[nATRecordBufUsedSize], pBuf, len);
		nATRecordBufUsedSize += len;
	}
	else
	{
		nATRecordBufUsedSize = 0x00;
		SUL_MemCopy8(&gATRecordBuf[nATRecordBufUsedSize], pBuf, len);
		nATRecordBufUsedSize += len;
	}

	hal_HstSendEvent(0x05090000);
	*recordLen = len;//AT_RECORD_BUF_SIZE - nATRecordBufUsedSize;
}

//MCI_AUDIO_FILE_RECORD_CALLBACK_T
VOID ATStartRecordCallBack(AT_RECORD_ERR_T result)
{
	AT_TC(g_sw_SPEC, "ATStartRecordCallBack result %x", result);
	hal_HstSendEvent(0x05090001);
	hal_HstSendEvent(result);
}
VOID ATPlayRecordCallBack(AT_RECORD_ERR_T result)
{
	AT_TC(g_sw_SPEC, "ATPlayRecordCallBack result %x", result);
	hal_HstSendEvent(0x05090001);
	hal_HstSendEvent(result);
}
VOID AT_AUDIO_StartRecord()
{
	MCI_AudioRecordStart(2000, AT_RECORD_TYPE_AMR, AT_RECORD_PLAY_MODE_AMR122, ATStartRecordCallBack, ATRecordBufPlayCallBack);
}

VOID AT_AUDIO_StopRecord()
{
	MCI_AudioRecordStop();
}

VOID AT_AUDIO_ResumeRecord()
{
	MCI_AudioRecordResume();
}

VOID AT_AUDIO_PauseRecord()
{
	MCI_AudioRecordPause();
}

VOID AT_AUDIO_PlayRecord()
{
	MCI_AudioPlayBuffer(gATRecordBuf, nATRecordBufUsedSize, TRUE, ATPlayRecordCallBack, AT_RECORD_PLAY_MODE_AMR_RING, 0);
}
//Add by XP at 20140528 for AT audio record end
#endif
