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
#include "at.h"
#include <drv_prv.h>
#include "at_module.h"
#include "at_cmd_special_gps.h"
#include "at_cfg.h"
#include "gpsd_hw_interface.h"
#include "mmi_trace.h"
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
#if defined(__AT_GPS__)

static UINT8 g_GPS_status = 0;
static UINT8 g_GPSsleep_status = 0;

VOID AT_EMOD_CmdFunc_GPS(AT_CMD_PARA *pParam)
{
    static UINT8 on_flag = 0;
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 GPS_state = 0;
    UINT8 param_len = 1;
    UINT8 Rsp[30] = {0,};
    if (pParam->iType == AT_CMD_TEST)
    {
        AT_Sprintf(Rsp, "+GPS: (0,1)");
        OK_RETURN(pResult, Rsp, pParam->nDLCI);
        return;
    
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        AT_Sprintf(Rsp, "+GPS: %d", g_GPS_status);
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

        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &GPS_state, &param_len))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
            return;
        }
	
		if (GPS_state==g_GPS_status)
		{
			AT_Sprintf(Rsp, "+GPS: %d", g_GPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}		

        if (GPS_state == 0)
        {
			gpsd_Close();
        	g_GPS_status=GPS_state;
			AT_Sprintf(Rsp, "+GPS: %d", g_GPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
            return;
        }
        else if (GPS_state == 1)
        {
        
        	/*开GPS*/
			gpsd_Open();
			#if 0
			/*改变bin 模式为NMEA 模式*/
			{
				UINT8 mode_bin_2_nmea[] = {0xE0,0xE2,0x03,0x00,0x01,0x81,0x01,0x81,0x01,0xF0,0xF3};
				GPS_uart_SendData(mode_bin_2_nmea,sizeof(mode_bin_2_nmea),GPS_DEFAULT_UART);
				sxr_Sleep(16384);
			}
			/*NEMA模式下面冷启动*/
			{
				UINT8 code_reset[] = {"$PANWS,RST,3*3d\r\n"};
				GPS_uart_SendData(code_reset,strlen(code_reset),GPS_DEFAULT_UART);
				sxr_Sleep(16384);
			}
			#endif
			
        	g_GPS_status=GPS_state;
			AT_Sprintf(Rsp, "+GPS: %d", g_GPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
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

VOID AT_EMOD_CmdFunc_GPS_Sleep(AT_CMD_PARA *pParam)
{
    static UINT8 on_flag = 0;
    PAT_CMD_RESULT pResult = NULL;
    UINT8 uParaCount = 0;
    UINT8 GPSsleep_state = 0;
    UINT8 param_len = 1;
    UINT8 Rsp[30] = {0,};

    if (pParam->iType == AT_CMD_TEST)
    {
        AT_Sprintf(Rsp, "+GPSSLEEP: (0,1)");
        OK_RETURN(pResult, Rsp, pParam->nDLCI);
        return;
    
    }
    else if (pParam->iType == AT_CMD_READ)
    {
        AT_Sprintf(Rsp, "+GPSSLEEP: %d", g_GPSsleep_status);
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

        if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &GPSsleep_state, &param_len))
        {
            ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
            return;
        }
	
        if (GPSsleep_state == 0)
        {
			gpsd_sleep(GPSsleep_state);
        	g_GPSsleep_status=GPSsleep_state;
			AT_Sprintf(Rsp, "+GPSSLEEP: %d", g_GPSsleep_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
            return;
        }
        else if (GPSsleep_state == 1)
        {
			gpsd_sleep(GPSsleep_state);
        	g_GPSsleep_status=GPSsleep_state;
			AT_Sprintf(Rsp, "+GPSSLEEP: %d", g_GPSsleep_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
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

#define GPS_DEFAULT_UART 2

//extern 	BOOL Send_Tx_Msg2GPSTask(UINT8* pBuf, UINT16 len,HAL_UART_ID_T uart_id);
VOID AT_EMOD_CmdFunc_GPS_Send(AT_CMD_PARA *pParam)
{
	PAT_CMD_RESULT pResult = NULL;
	UINT8 gps_uart_buf[100] = {0};
	UINT16 param_len = 1;
	UINT8 Rsp[20] = {0,};
	UINT8* p_rec_data;

	if (pParam->iType == AT_CMD_SET)
	{
		param_len = sizeof(gps_uart_buf)-1;
		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, &gps_uart_buf, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		/*原始命令之后需要加0x0d 0x0a 再加一位0x00，所以要malloc  +3 个BYTE*/
		p_rec_data = AT_MALLOC(param_len+3);
		if(NULL == p_rec_data)
		{
			hal_DbgAssert(0);//malloc error 
		}
		memcpy(p_rec_data,gps_uart_buf,param_len);

		/* 增加换行符*/
		UINT8 end_line[2]={0x0d,0x0a};
		memcpy(p_rec_data + param_len,end_line,2);
		param_len += 2;

		GPS_uart_SendData(p_rec_data,param_len,GPS_DEFAULT_UART);
		//Send_Tx_Msg2GPSTask(p_rec_data, param_len,GPS_DEFAULT_UART);

		AT_TC(g_sw_SPEC, "AT_EMOD_CmdFunc_BTBOND --> BtBondDev");
		OK_RETURN(pResult, Rsp, pParam->nDLCI);	
		return;
	}
	else
	{
		ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
		return;
	}
}

VOID AT_EMOD_CmdFunc_GPS_I2C(AT_CMD_PARA *pParam)
{
	static UINT8 on_flag = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 uParaCount = 0;
	UINT32 read_write = 0;
	UINT32 i2c_adder = 0;
	UINT8 i2c_adder_str[20] = {0};
	UINT32 data = 0;
	UINT8 data_str[20] = {0};
	UINT8 param_len = 1;
	UINT8 Rsp[30] = {0,};

	if (pParam->iType == AT_CMD_SET)
	{
		if ((pParam->pPara == NULL) || (ERR_SUCCESS != AT_Util_GetParaCount(pParam->pPara, &uParaCount))) /* GET param count */
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		if (uParaCount != 3)
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}
		
		param_len = 4;
		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT32, &read_write, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}

		param_len = 20;
		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_STRING, &i2c_adder_str, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}
		i2c_adder   =   strtoul(i2c_adder_str, NULL, 16); 
			
		param_len = 20;
		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 2, AT_UTIL_PARA_TYPE_STRING, &data_str, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}
		data   =   strtoul(data_str, NULL, 16); 

		if (0 ==read_write)/*read*/
		{
			RDAGPS_I2C_Read(i2c_adder,&data);
			AT_Sprintf(Rsp, "+i2c read: %02x,%02x", i2c_adder,data);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}
		else if(1 == read_write)/*write*/
		{
			RDAGPS_I2C_Write(i2c_adder,data);
			AT_Sprintf(Rsp, "+i2c write: %02x,%02x", i2c_adder,data);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}
		else if (2 ==read_write)/*read dig*/
		{
			RDAGPS_I2C_DIG_Read(i2c_adder,&data);
			AT_Sprintf(Rsp, "+i2c read: %02x,%02x", i2c_adder,data);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}
		else if(3 == read_write)/*write dig*/
		{
			RDAGPS_I2C_DIG_Write(i2c_adder,data);
			AT_Sprintf(Rsp, "+i2c write: %02x,%02x", i2c_adder,data);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
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
#endif

#if defined(__AT_GPS_AGPS__)

static UINT8 g_GPS_AGPS_status = 0;

VOID AT_EMOD_CmdFunc_GPS_AGPS(AT_CMD_PARA *pParam)
{
	static UINT8 on_flag = 0;
	PAT_CMD_RESULT pResult = NULL;
	UINT8 uParaCount = 0;
	UINT8 GPS_AGPS_state = 0;
	UINT8 param_len = 1;
	UINT8 Rsp[30] = {0,};

	if (pParam->iType == AT_CMD_TEST)
	{
		AT_Sprintf(Rsp, "+AGPS: (0,1)");
		OK_RETURN(pResult, Rsp, pParam->nDLCI);
		return;
	
	}
	else if (pParam->iType == AT_CMD_READ)
	{
		AT_Sprintf(Rsp, "+AGPS: %d", g_GPS_AGPS_status);
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

		if (ERR_SUCCESS != AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &GPS_AGPS_state, &param_len))
		{
			ERROR_RETURN(pResult, ERR_AT_CME_PARAM_INVALID, pParam->nDLCI);
			return;
		}
	
		if (GPS_AGPS_state==g_GPS_AGPS_status)
		{
			AT_Sprintf(Rsp, "+AGPS: %d", g_GPS_AGPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}		

		if (GPS_AGPS_state == 0)
		{
			//gpsd_Close();
			g_GPS_AGPS_status=GPS_AGPS_state;
			AT_Sprintf(Rsp, "+AGPS: %d", g_GPS_AGPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
			return;
		}
		else if (GPS_AGPS_state == 1)
		{
			//gpsd_Open();
			Send_Init_Msg2GPS_AGPSTask();//agp_main();
			g_GPS_AGPS_status=GPS_AGPS_state;
			AT_Sprintf(Rsp, "+AGPS: %d", g_GPS_AGPS_status);
			OK_RETURN(pResult, Rsp, pParam->nDLCI);
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

#endif


