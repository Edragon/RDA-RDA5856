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

// //////////////////////////////////////////////////////////////////////////////
#include "at_common.h"
#include "ap_common.h"
#include "MainTask.h"
#include "at_module.h"

#if defined(__AT_MOD_MUSIC__)
#include "at_cmd_music.h"
#ifdef CODEC_SUPPORT
#include "ap_codec.h"
#endif
#include "ap_music.h"
#include "mci.h"
#ifndef WIN32
#include "hal_mem_map.h"

#include "hal_rda_audio.h"
#include "tgt_calib_m.h"
#endif


#include "aud_snd_types.h"

#include "hal_uart.h"
#include "at_uart.h"
#include "resource_id.h"





#if defined(APP_SUPPORT_UART_PLAY) &&(APP_SUPPORT_UART_PLAY == 1)
static UINT8 g_checkenable = 0;
static UINT8 g_ReqStreamType = AT_MUSIC_STREAM_NONE;
#endif
extern AUD_ITF_T audioItf;

extern music_vars_t *g_music_vars;
extern INT8 g_play_disk;
HANDLE g_ATMusicSem = 0;

#define AT_MUSIC_INIT_EVENT()     do{if(!g_ATMusicSem) g_ATMusicSem = COS_CreateSemaphore(0);}while(0)
#define AT_MUSIC_WAIT_EVENT()     COS_WaitForSemaphore(g_ATMusicSem, COS_WAIT_FOREVER)
#define AT_MUSIC_SIGNAL_EVENT()   COS_ReleaseSemaphore(g_ATMusicSem)

VOID AT_MUSIC_Result_OK(UINT32 uReturnValue,
                       UINT32 uResultCode, UINT8 nDelayTime, UINT8* pBuffer, UINT16 nDataSize, UINT8 nDLCI)

{
    PAT_CMD_RESULT pResult = NULL;

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

VOID AT_MUSIC_Result_Err(UINT32 uErrorCode, UINT8 nErrorType, UINT8 nDLCI)
{
    PAT_CMD_RESULT pResult = NULL;
    pResult = AT_CreateRC(CMD_FUNC_FAIL, CMD_RC_ERROR, uErrorCode, nErrorType, 0, 0, 0, nDLCI);
    AT_Notify2ATM(pResult, nDLCI);

    if (pResult != NULL)
    {
        AT_FREE(pResult);
        pResult = NULL;
    }

    return;
}

/*****************************************************************************
* Name:        AT_MUSIC_CmdFunc_ModeSwitch
* Description: MUSIC module execute error process.
* Parameter:   UINT32 uErrorCode, UINT8 nErrorType
* Return:      VOID
* Remark:      n/a
* Author:      YangYang
* Data:        2008-5-15
******************************************************************************/
VOID AT_MUSIC_CmdFunc_ModeSwitch(AT_CMD_PARA* pParam)
{
    return;
}

/*****************************************************************************
* Name:        AT_MUSIC_CmdFunc_Play
* Description: MUSIC module execute error process.
* Parameter:   UINT32 uErrorCode, UINT8 nErrorType
* Return:      VOID
* Remark:      n/a
* Author:      YangYang
* Data:        2008-5-15
******************************************************************************/
VOID AT_MUSIC_CmdFunc_Play(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;
    ev.nEventId = (UINT16)AT_MUSIC_PLAY;
    ev.nParam1 = (UINT16)(AT_MUSIC_PLAY>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}

/*****************************************************************************
* Name:        AT_MUSIC_CmdFunc_PAUSE
* Description: MUSIC module execute error process.
* Parameter:   UINT32 uErrorCode, UINT8 nErrorType
* Return:      VOID
* Remark:      n/a
* Author:      YangYang
* Data:        2008-5-15
******************************************************************************/
VOID AT_MUSIC_CmdFunc_Pause(AT_CMD_PARA* pParam)
{
    COS_EVENT ev;
    ev.nEventId = (UINT16)AT_MUSIC_PAUSE;
    ev.nParam1 = (UINT16)(AT_MUSIC_PAUSE>>16);
    COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}



/******************************************************************************
 * AT_MUSIC_CmdFunc_State:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
VOID AT_MUSIC_CmdFunc_State(AT_CMD_PARA* pParam)
{

    if (NULL == pParam)
    {
        goto iOpen_ERROR;
    }
    else
    {
        if(NULL == pParam->pPara)
        {
            goto iOpen_ERROR;
        }

        switch (pParam->iType)
        {
 
            case AT_CMD_READ:
            {
#if APP_SUPPORT_MUSIC==1
                UINT8 time_total_m,time_total_s;
                UINT8 time_m,time_s;
                music_screen_gui_t *tmp = (music_screen_gui_t *) GetHistoryGuiBuffer(SCR_ID_MUSIC_MAIN);

                if(IS_MUSIC_MODE() && (g_music_vars != NULL) && (tmp != NULL))
                {

                    AT_WriteUart((UINT8*)"Dir=", AT_StrLen("Dir="));

                    AT_WriteUart(g_music_vars->location[g_play_disk].subdir_name,
                                AT_StrLen(g_music_vars->location[g_play_disk].subdir_name));

                    AT_WriteUart((UINT8*)",", AT_StrLen(","));

                    AT_WriteUart(tmp->TagInfoBuf, AT_StrLen(tmp->TagInfoBuf));//g_TagInfoBuf_name: unicode


                    time_m = g_music_vars->CurrentTime[g_play_disk]/1000/60;
                    time_s = g_music_vars->CurrentTime[g_play_disk]/1000-time_m*60;
                    time_total_m = tmp->TotalTime/1000/60;
                    time_total_s = tmp->TotalTime/1000-time_total_m*60;

                    AT_WriteUartString(",%02d:%02d/%02d:%02d,%03d/%03d,%d,%d,%d",
                                            time_m, time_s, time_total_m, time_total_s,
                                            tmp->musicsequence, tmp->musicamount,
                                            tmp->currentMusicType,
                                            g_music_vars->repeat_mode,
                                            tmp->PlayStatus);



                }
                else
                {
                    AT_WriteUart((UINT8*)"MusicState=0,0,0", AT_StrLen("MusicState=0,0,0"));
                }


                AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
#endif
                return;
            }

            case AT_CMD_SET:
            default:
            {
                goto iOpen_ERROR;
            }
        }
    }

iOpen_ERROR:
    AT_MUSIC_Result_Err(CMD_ERROR_CODE_OK, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
}

#if defined(APP_SUPPORT_UART_PLAY) && (APP_SUPPORT_UART_PLAY == 1)
typedef enum
{
    UART_PLAY_STATUS_STOP,
    UART_PLAY_STATUS_PLAY,
    UART_PLAY_STATUS_PAUSE,
} AT_UART_PLAY_STATUS;

typedef struct _AT_MUSIC_UARTPLAY_
{
    AT_UART_PLAY_STATUS status;
    UINT8  music_type; /*mci_type_enum*/
    UINT8 *pbuffer;
    UINT32 buffer_size;
    UINT32 min_len;
    UINT32 buffer_threshold;
    UINT32 remote_rq_len;
    UINT8  rec_timeout_id;
    UINT8 stop_flag; /* 0: stop immediately, 1: stop after play left audio data in rx buffer. */
} AT_MUSIC_UARTPLAY_T;

AT_MUSIC_UARTPLAY_T   AT_MUSIC_UartPlay =
{
    UART_PLAY_STATUS_STOP,
    0,
    NULL,
    0,
    0,
    0,
    0,
    0,
};

//#define UART_PLAY_DEBUG_EVENT_EN
//#define UART_PLAY_REQUEST_DATA_ENABLE

UINT32 uart_play_read_raw_data_len = 0;

extern UINT8 *g_codec_buffer;

extern VOID gpio_UartPlayRequestData(BOOL on);
extern int32 Audio_MpegSetTestMode (uint8 mode, AUDIO_FACTORY_TEST_HANDLER_T hld);

static void AT_MUSIC_UartPlayRequestData(BOOL on)
{
#ifdef UART_PLAY_REQUEST_DATA_ENABLE
    gpio_UartPlayRequestData(on);
#endif
}

static VOID AT_MUSIC_UartPlay_Stop(VOID)
{
    AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_Stop");

    AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
    AT_MUSIC_UartPlay.stop_flag = 0;
    AT_MUSIC_UartPlayRequestData(FALSE);
    MCI_AudioStopStream();

    if (AT_MUSIC_UartPlay.pbuffer)
    {
        //COS_Free(AT_MUSIC_UartPlay.pbuffer);
        AT_MUSIC_UartPlay.pbuffer = NULL;
    }

    uart_play_read_raw_data_len = 0;
}

static VOID AT_MUSIC_UartPlay_AllLeftData_Stop(VOID)
{
    AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_AllLeftData_Stop");

    //AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
    AT_MUSIC_UartPlayRequestData(FALSE);
    MCI_AudioPlayAllLeftData();

    uart_play_read_raw_data_len = 0;
}

static void AT_MUSIC_UartPlay_Callback(MCI_STREAM_STATUS_T evt )
{
    COS_EVENT event = { 0 };

#ifdef UART_PLAY_DEBUG_EVENT_EN
    hal_HstSendEvent(SYS_EVENT, 0xdddd0000 + evt);
#endif


    switch (evt) {
    case STREAM_STATUS_REQUEST_DATA:
    /*
    Do NOT stop play when no data to support pause/resume
    */
    case STREAM_STATUS_NO_MORE_DATA:
        if (AT_MUSIC_UartPlay.remote_rq_len > 0)
        {
            ;//To send OK response
        }
        else
        {
#ifdef UART_PLAY_REQUEST_DATA_ENABLE
            if (mmc_GetRemainingBytes() < (AT_MUSIC_UartPlay.buffer_threshold > 1024 ?
            (AT_MUSIC_UartPlay.buffer_threshold - 1024) : (AT_MUSIC_UartPlay.buffer_threshold >> 1))) {
                AT_MUSIC_UartPlayRequestData(TRUE);
            }
#endif
            return;
        }
    break;

    case STREAM_STATUS_END:
    {
        AT_MUSIC_UartPlayRequestData(FALSE);

        if (g_ReqStreamType == AT_MUSIC_STREAM_LOCAL)
        {
            if (AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_PLAY)
            {

                AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                AT_MUSIC_SIGNAL_EVENT();
            }
            else if(AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_STOP)
            {

                g_ReqStreamType = AT_MUSIC_STREAM_NONE;
                AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, 0);
            }
            return;
        }
    }
    break;
    case STREAM_STATUS_ERR:
    break;

    default:
    break;
    }


    event.nEventId = EV_AUDIO_PLAY_STREAM_IND;
    event.nParam1 = evt;

    COS_SendEvent(AT_MOD_ID, &event, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

int AT_MUSIC_UartPlay_GetStatus(void)
{
    return AT_MUSIC_UartPlay.status;
}

VOID AT_MUSIC_Receive_Data_TimeOut(VOID* pParam)
{
    if (uart_play_read_raw_data_len > 0)
    {
        hal_HstSendEvent (SYS_EVENT, 0x201870c7);
        AT_MUSIC_StopReadRawData();
        AT_WriteUartString("\r\nOK\r\n");
    }
}

int AT_MUSIC_StartReadRawData(UINT32 size)
{
    HAL_UART_IRQ_STATUS_T mask;

    uart_play_read_raw_data_len = size;

#if 0
    hal_UartRxDmaStop(g_UartInfo.Scom_UartId);

    if (0 == hal_UartGetData(g_UartInfo.Scom_UartId,
        g_UartInfo.rx_buff + g_UartInfo.rx_buff_p,
        size))
    {
        hal_HstSendEvent (SYS_EVENT, 0xddff4410);
        AT_ASSERT(0);
    }
#endif

#if 1
    if (AT_MUSIC_UartPlay.rec_timeout_id) {
        COS_KillTimer(AT_MUSIC_UartPlay.rec_timeout_id);
		AT_MUSIC_UartPlay.rec_timeout_id = 0;
    }

    // start timer to check timeout
    AT_MUSIC_UartPlay.rec_timeout_id = COS_SetTimer(600,
                        AT_MUSIC_Receive_Data_TimeOut, NULL, COS_TIMER_MODE_SINGLE);
#endif

#if 0
    // clear DMA timeout mask
    mask = hal_UartIrqGetMask(g_UartInfo.Scom_UartId);
    mask.rxDmaTimeout = 0;
    hal_UartIrqSetMask(g_UartInfo.Scom_UartId, mask);
#endif
}

int AT_MUSIC_StopReadRawData(void)
{
    HAL_UART_IRQ_STATUS_T mask;

    uart_play_read_raw_data_len = 0;

#if 1
    if (AT_MUSIC_UartPlay.rec_timeout_id) {
        COS_KillTimer(AT_MUSIC_UartPlay.rec_timeout_id);
        AT_MUSIC_UartPlay.rec_timeout_id = 0;
    }
#endif

#if 0
    // set DMA timeout mask
    mask = hal_UartIrqGetMask(g_UartInfo.Scom_UartId);
    mask.rxDmaTimeout = 1;
    hal_UartIrqSetMask(g_UartInfo.Scom_UartId, mask);
#endif
}

BOOL AT_MUSIC_UartPlay_AsyncEventProcess(COS_EVENT* pEvent)
{
    switch (pEvent->nEventId) {
        case UART_RAWDATA_REC_FINISH:
            AT_WriteUartString("\r\nOK\r\n");
        break;

        case EV_AUDIO_PLAY_STREAM_IND:
        {
            MCI_STREAM_STATUS_T evt = (MCI_STREAM_STATUS_T) pEvent->nParam1;

#ifdef UART_PLAY_DEBUG_EVENT_EN
            hal_HstSendEvent(SYS_EVENT, 0xeeee0000 + evt);
#endif
            switch (evt) {
                case STREAM_STATUS_REQUEST_DATA:
                    if (mmc_GetEmptyBytes() < AT_MUSIC_UartPlay.remote_rq_len) {
                        hal_HstSendEvent(SYS_EVENT, 0x2018e001);
                        break;
                    } else {
                        ;//Fall Through
                    }
                case STREAM_STATUS_NO_MORE_DATA:
                    if (AT_MUSIC_UartPlay.remote_rq_len > 0) {
                        hal_HstSendEvent(SYS_EVENT, 0x2018e002);
                        AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_AsyncEventProcess RemoteRequestSendDataLen=%d", AT_MUSIC_UartPlay.remote_rq_len);

                        AT_MUSIC_StartReadRawData(AT_MUSIC_UartPlay.remote_rq_len);
                        AT_MUSIC_UartPlay.remote_rq_len = 0;
                        //and send OK
                        AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, 0);
                    }
                    break;

                case STREAM_STATUS_END:
                case STREAM_STATUS_ERR:
                    hal_HstSendEvent(SYS_EVENT, 0x20183708);

                    if (AT_MUSIC_UartPlay.stop_flag == 1)
                    {
                        AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                        MCI_AudioStopStream();
                        AT_MUSIC_UartPlay.stop_flag = 0;
                        break;
                    }

                    INT32 ret;
                    AT_TC(g_sw_SA,"restart audioplayer bytes %d,%d",MCI_GetRemainingBytes(),AT_MUSIC_UartPlay.remote_rq_len);

                    if (MCI_GetRemainingBytes() >= AT_MUSIC_UartPlay.min_len)
                    {
                        ret = MCI_AudioPlayStream(1, AT_MUSIC_UartPlay_Callback,0);
                        AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PLAY;
                        if (ret != MCI_ERR_NO)
                        {
                            AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                        }

                        AT_TC(g_sw_SA,"restart audioplayer %d",ret);
                        hal_HstSendEvent(SYS_EVENT, 0x20183718);
                    }
                    else if (AT_MUSIC_UartPlay.remote_rq_len > 0)
                    {

                        AT_MUSIC_StartReadRawData(AT_MUSIC_UartPlay.remote_rq_len);
                        AT_MUSIC_UartPlay.remote_rq_len = 0;
                        AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                        AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, 0);
                        hal_HstSendEvent(SYS_EVENT, 0x20183728);
                    }
                    else
                    {
                        AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                    }
                    break;

                default:
                break;
            }
        }
        break;

        default:
        return FALSE;
    }

    return TRUE;
}

VOID AT_MUSIC_UartPlay_CmdFunc_Play(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS;

    if(NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {

            case AT_CMD_SET:
            {
                UINT8 param_cnt ;
                UINT16 Len;
                INT32 ret;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if((ERR_SUCCESS != iRet)||(param_cnt < 3))
                {
                    goto Param_ERROR;
                }

                Len = 1;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &AT_MUSIC_UartPlay.music_type, &Len);

                Len = 4;
                AT_Util_GetParaWithRule(pParam->pPara, 1, AT_UTIL_PARA_TYPE_UINT32, &AT_MUSIC_UartPlay.buffer_size, &Len);

                Len = 4;
                AT_Util_GetParaWithRule(pParam->pPara, 2, AT_UTIL_PARA_TYPE_UINT32, &AT_MUSIC_UartPlay.min_len, &Len);

#ifdef UART_PLAY_REQUEST_DATA_ENABLE
                Len = 4;
                AT_Util_GetParaWithRule(pParam->pPara, 3, AT_UTIL_PARA_TYPE_UINT32, &AT_MUSIC_UartPlay.buffer_threshold, &Len);
#else
                AT_MUSIC_UartPlay.buffer_threshold = (AT_MUSIC_UartPlay.buffer_size >> 1);
#endif

                AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_Play: m_type=%d, buf_size = %d, min_buf_size=%d, buf_thd=%d",
                              AT_MUSIC_UartPlay.music_type, AT_MUSIC_UartPlay.buffer_size, AT_MUSIC_UartPlay.min_len,
                              AT_MUSIC_UartPlay.buffer_threshold);

                //AT_MUSIC_UartPlay.pbuffer = (UINT8 *) AT_MALLOC(AT_MUSIC_UartPlay.buffer_size);
                AT_MUSIC_UartPlay.pbuffer = g_codec_buffer;
                g_ReqStreamType = AT_MUSIC_STREAM_UART;

                hal_HstSendEvent(SYS_EVENT, 0x201891a7);
                hal_HstSendEvent(SYS_EVENT, AT_MUSIC_UartPlay.pbuffer);

                if (NULL == AT_MUSIC_UartPlay.pbuffer)
                {
                    AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_Play malloc buffer fail!!!");
                    AT_MUSIC_Result_Err(ERR_AT_CME_NO_MEMORY, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
                    return;
                }

                if (AT_MUSIC_UartPlay.buffer_size > AP_CODEC_BUFFER_SIZE_MAX)
                {
                    AT_MUSIC_UartPlay.buffer_size = AP_CODEC_BUFFER_SIZE_MAX;
                }
                memset(AT_MUSIC_UartPlay.pbuffer, 0x0, AP_CODEC_BUFFER_SIZE_MAX);

                ret = MCI_AudioSetupStream(AT_MUSIC_UartPlay.pbuffer,
                    AT_MUSIC_UartPlay.buffer_size, 0, AT_MUSIC_UartPlay.buffer_threshold,AT_MUSIC_UartPlay.music_type);
                AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;

                g_checkenable = 1;

                if (ret != MCI_ERR_NO)
                {
                    //COS_Free(AT_MUSIC_UartPlay.pbuffer);
                    AT_MUSIC_UartPlay.pbuffer = NULL;
                    AT_MUSIC_Result_Err(ret, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
                }
                else
                {
                    //AT_UartReopen(ds_high);
                    AT_MUSIC_UartPlayRequestData(TRUE);
                    //AT_TC(g_sw_SA, "[gpio request test]AT_MUSIC_UartPlay_Stop 444, TRUE.");
                    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                }

                return;
            }
            default:
            {
                goto Param_ERROR;
            }

        }
    }

Param_ERROR:

    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;

}


PUBLIC UINT32 AT_MUSIC_Receive_Data(UINT8 *data, UINT32 len)
{
    UINT32 error;
#ifdef UART_PLAY_REQUEST_DATA_ENABLE
    UINT32 remainByte = 0;
#endif

    error = mmc_AddedData(data, len);

#ifdef UART_PLAY_REQUEST_DATA_ENABLE
    remainByte = mmc_GetRemainingBytes();

    if (remainByte > AT_MUSIC_UartPlay.buffer_threshold)
    {
        AT_MUSIC_UartPlayRequestData(FALSE);
    }
    else if (remainByte < (AT_MUSIC_UartPlay.buffer_threshold > 1024 ?
        (AT_MUSIC_UartPlay.buffer_threshold - 1024) : (AT_MUSIC_UartPlay.buffer_threshold >> 1)))
    {
        AT_MUSIC_UartPlayRequestData(TRUE);
    }
#endif

    return error;
}

VOID AT_MUSIC_UartPlay_CmdFunc_RawData(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS,skiplen = 0;

    if(NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
                UINT8 param_cnt ;
                UINT32 data_len;
                UINT32 pbuf_len;
                UINT8 *data_ptr = NULL;
                UINT8 *pbuf = NULL;

                UINT16 Len;
                INT32 ret = MCI_ERR_NO;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if((ERR_SUCCESS != iRet)||(param_cnt < 1))
                {
                    goto Param_ERROR;
                }

                Len = 4;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT32, &data_len, &Len);

                AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_RawData data_len=%d", data_len);

                hal_HstSendEvent(SYS_EVENT, 0x20188001);
                if (g_ReqStreamType != AT_MUSIC_STREAM_UART)
                {
                    g_ReqStreamType = AT_MUSIC_STREAM_UART;
                    mmc_ResumeStream(AT_MUSIC_UartPlay.pbuffer,AT_MUSIC_UartPlay.buffer_size,AT_MUSIC_UartPlay.music_type);
                }

                if (AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_STOP)
                {

                    if (MCI_GetRemainingBytes() >= AT_MUSIC_UartPlay.min_len)
                    {
                        if (g_checkenable == 1)
                        {
                            skiplen = mpeg_stream_check(&AT_MUSIC_UartPlay.music_type,AT_MUSIC_UartPlay.pbuffer,MCI_GetRemainingBytes());

                            if (skiplen >0) {

                                mmc_StreamSkip(skiplen);

                            }
                        }
                        AT_TC(g_sw_SA, "MCI_AudioPlayStream data_len=%d,skiplen:%d", MCI_GetRemainingBytes(),skiplen);
                        hal_HstSendEvent(SYS_EVENT, *((int *)AT_MUSIC_UartPlay.pbuffer));
                        if (MCI_GetRemainingBytes() >= AT_MUSIC_UartPlay.min_len) {

                            hal_HstSendEvent(SYS_EVENT, 0x201891a8);

                          //  ret = MCI_AudioSetupStream(AT_MUSIC_UartPlay.pbuffer, AT_MUSIC_UartPlay.buffer_size,
                                 //.   MCI_GetRemainingBytes(), AT_MUSIC_UartPlay.buffer_threshold,AT_MUSIC_UartPlay.music_type);

                            ret = MCI_AudioPlayStream(1, AT_MUSIC_UartPlay_Callback,0);
                            AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PLAY;
                            g_checkenable = 0;

                        }

                    }
                }

                if((AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_PAUSE) && (MCI_GetRemainingBytes() >= AT_MUSIC_UartPlay.min_len))
                {
                    AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PLAY;
                    hal_HstSendEvent(SYS_EVENT, 0x201891a9);

                    MCI_AudioPlayResume();
                }

                if(ret != MCI_ERR_NO)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x20188002);
                    hal_HstSendEvent(SYS_EVENT, ret);
                    AT_MUSIC_Result_Err(ERR_AT_CME_EXE_FAIL, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);
                }
                else if(mmc_GetEmptyBytes() < data_len)
                {
                    hal_HstSendEvent(SYS_EVENT, 0x20188003);
                    //Delay to send OK when we haven't enough space
                    AT_MUSIC_UartPlay.remote_rq_len = data_len;
                    AT_TC(g_sw_SA, "AT_MUSIC_UartPlay.remote_rq_len =%d", data_len);

                    AT_MUSIC_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                }
                else
                {
                    hal_HstSendEvent(SYS_EVENT, 0x20188004);
                    AT_MUSIC_StartReadRawData(data_len);
                    //and send OK
                    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                }

                return;
            }
            default:
            {
                goto Param_ERROR;
            }

        }
    }

Param_ERROR:

    hal_HstSendEvent(SYS_EVENT, 0x20188005);

    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;

}


VOID AT_MUSIC_UartPlay_CmdFunc_GetStatus(AT_CMD_PARA* pParam)
{
    UINT8 status[128];

    hal_HstSendEvent(SYS_EVENT, 0x20ff0001);

    memset(status, 0, sizeof(status));
    sprintf(status,"%d", AT_MUSIC_UartPlay.status);

    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, status, strlen(status), pParam->nDLCI);

    return;
}

VOID AT_MUSIC_UartPlay_CmdFunc_Pause(AT_CMD_PARA* pParam)
{
    hal_HstSendEvent(SYS_EVENT, 0x20189a3e);

    MCI_AudioPlayPause();
    AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PAUSE;

    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}

VOID AT_MUSIC_UartPlay_CmdFunc_Resume(AT_CMD_PARA* pParam)
{
    hal_HstSendEvent(SYS_EVENT, 0x20188e3e);

    MCI_AudioPlayResume();

    AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PLAY;
    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

    return;
}

VOID AT_MUSIC_UartPlay_CmdFunc_Stop(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS,RemainingLen = 0;

    if (NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
                UINT8 param_cnt ;
                UINT16 Len;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if ((ERR_SUCCESS != iRet) || (param_cnt < 1))
                {
                    goto Param_ERROR;
                }

                Len = 1;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &AT_MUSIC_UartPlay.stop_flag, &Len);
                AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_Stop data_len=%d", AT_MUSIC_UartPlay.stop_flag);
                hal_HstSendEvent(SYS_EVENT, 0x20183709);
                hal_HstSendEvent(SYS_EVENT, AT_MUSIC_UartPlay.stop_flag);
                AT_MUSIC_StopReadRawData();
                RemainingLen = MCI_GetRemainingBytes();
                if (AT_MUSIC_UartPlay.music_type == MCI_TYPE_TS && RemainingLen < 188)
                {
                    RemainingLen = 0;

                }
                iRet = MCI_ERR_NO;
                if (AT_MUSIC_UartPlay.stop_flag) // 1: stop after play left audio data in rx buffer.
                {

                    if (AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_STOP && (RemainingLen > 0))
                    {
                        AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_PLAY;
                        hal_HstSendEvent(SYS_EVENT, 0x20180eee);

                      //  MCI_AudioSetupStream(AT_MUSIC_UartPlay.pbuffer, AT_MUSIC_UartPlay.buffer_size,
                            //    MCI_GetRemainingBytes(), AT_MUSIC_UartPlay.buffer_threshold,AT_MUSIC_UartPlay.music_type);

                        iRet = MCI_AudioPlayStream(1, AT_MUSIC_UartPlay_Callback,0);

                        if (iRet != MCI_ERR_NO)
                        {
                            AT_MUSIC_UartPlay_Stop();
                        }
                    }
                    else if(AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_PLAY)
                    {

                        AT_MUSIC_UartPlay_AllLeftData_Stop();

                    }
                    else
                    {
                        AT_MUSIC_UartPlay_Stop();
                    }


                    // response ok immediately for Baidu's requirements.
                    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                    //AT_MUSIC_Result_OK(CMD_FUNC_SUCC_ASYN, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                }
                else // 0: stop immediately
                {
                    AT_MUSIC_UartPlay_Stop();
                    AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);
                }

                return;
            }
            default:
            {
                goto Param_ERROR;
            }
        }
    }

Param_ERROR:
    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;
}

#define MIC_VOLUME_LEVEL  { \
        {0, -13, 0,0},\
        {6, -13, 0,0},\
        {12, -13, 0,0},\
        {18, -13, 0,0},\
        {21, -12, 0,0},\
        {21, -8, 0,0},\
        {21, -4, 0,0},\
        {21, 0, 0,0},\
        {21, 4, 0,0},\
        {21, 8, 0,0},\
        {21, 12, 0,0},\
        {21, 16, 0,0},\
        {21, 16, 1,0}}


PRIVATE const CALIB_AUDIO_IN_GAINS_T inRecordGainLevel[13] = MIC_VOLUME_LEVEL;

//volume range =[0 ,12]
PRIVATE VOID AT_SetMicCfg(UINT8 volume)
{

    CALIB_CALIBRATION_T* calibPtr = tgt_GetCalibConfig();
    CALIB_AUDIO_IN_GAINS_T *inRecordGain,micfg;

    volume = volume > 12 ? 12:volume;
    micfg = inRecordGainLevel[volume];

    inRecordGain = &calibPtr->aud.audioGains[calibPtr->aud.audioGainItfMap[audioItf]].inGainsCall;
    *inRecordGain = micfg;

    UINT32 anaGain = hal_AudMicGainDb2Val(micfg.ana);
    UINT32 adcGain = hal_AudAdcGainDb2Val(micfg.adc);

    AT_TC(g_sw_SA,"AT_SetMicCfg,ana=%d(%d),adc=%d(%d)",anaGain,micfg.ana,adcGain,micfg.adc);

    hal_AudSetMicAnaVolume(anaGain);
    hal_AudSetAdcVolume(adcGain);

}

VOID AT_MUSIC_UartPlay_CmdFunc_SetMicGain(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS;

    hal_HstSendEvent(SYS_EVENT, 0x2018000e);

    if (NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
                UINT8 param_cnt,volume;
                UINT16 Len;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if ((ERR_SUCCESS != iRet) || (param_cnt < 1))
                {
                    goto Param_ERROR;
                }

                Len = 1;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &volume,&Len);

                AT_SetMicCfg(volume);

                AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

                return;
            }
            default:
            {
                goto Param_ERROR;
            }
        }
    }

Param_ERROR:
    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;
}


static void AT_MUSIC_FactoryTestCallback(uint8 testResult)
{
    hal_HstSendEvent(SYS_EVENT, 0x2018f7c1);
    hal_HstSendEvent(SYS_EVENT, testResult);

    switch (testResult)
    {
    case 0:
        AT_MUSIC_Result_OK(CMD_FUNC_FAIL, CMD_RC_OK, 10, NULL, 0, 0);
        break;
    case 1:
        AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, 0);
        break;
    default:
        break;
    }
    Audio_MpegSetTestMode(0, NULL);
}

UINT8  FTCodec_ToneId = GUI_AUDIO_FT_CODEC;
VOID AT_MUSIC_UartPlay_CmdFunc_FTCodec(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS;

    hal_HstSendEvent(SYS_EVENT, 0x2018f7c0);

    if (NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
                UINT8 param_cnt, test_mode;
                UINT16 Len;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if ((ERR_SUCCESS != iRet) || (param_cnt < 1))
                {
                    goto Param_ERROR;
                }

                Len = 1;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &test_mode, &Len);
                AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_FTCodec test_mode=%d", test_mode);
                media_PlayInternalAudio(FTCodec_ToneId, 1, FALSE);
                Audio_MpegSetTestMode(test_mode, AT_MUSIC_FactoryTestCallback);

                //AT_MUSIC_Result_OK(CMD_FUNC_SUCC, CMD_RC_OK, 10, NULL, 0, pParam->nDLCI);

                return;
            }
            default:
            {
                goto Param_ERROR;
            }
        }
    }

Param_ERROR:
    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;
}



VOID AT_MUSIC_UartPlay_CmdFunc_PlayInterAud(AT_CMD_PARA* pParam)
{
    PAT_CMD_RESULT pResult;
    INT32 iRet = ERR_SUCCESS;
    hal_HstSendEvent(SYS_EVENT, 0x20181543);
    if (NULL == pParam)
    {
        goto Param_ERROR;
    }
    else
    {
        if (pParam->pPara == NULL)
        {
            goto Param_ERROR;
        }

        switch (pParam->iType)
        {
            case AT_CMD_SET:
            {
                UINT8 param_cnt;
                UINT16 Len,audio_id;

                iRet = AT_Util_GetParaCount(pParam->pPara, &param_cnt);

                if ((ERR_SUCCESS != iRet) || (param_cnt < 1))
                {
                    goto Param_ERROR;
                }

                Len = 1;
                AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_UINT8, &audio_id, &Len);
                AT_TC(g_sw_SA, "AT_MUSIC_UartPlay_CmdFunc_PlayInterAud audio_id=%d", audio_id);
                g_ReqStreamType = AT_MUSIC_STREAM_LOCAL;

                if(AT_MUSIC_UartPlay.status == UART_PLAY_STATUS_PLAY)
                {

                    AT_MUSIC_UartPlay_AllLeftData_Stop();

                    AT_MUSIC_INIT_EVENT();
                    AT_MUSIC_WAIT_EVENT();
                }
                else
                {
                   MCI_AudioStop();//To avoid clean buffer,so call this api.
                   AT_MUSIC_UartPlay.status = UART_PLAY_STATUS_STOP;
                }
                MCI_PlayNotificationSnd(audio_id,AT_MUSIC_UartPlay_Callback);
                return;
            }
            default:
            {
                goto Param_ERROR;
            }
        }
    }

Param_ERROR:
    AT_MUSIC_Result_Err(ERR_AT_CME_PARAM_INVALID, CMD_ERROR_CODE_TYPE_CME, pParam->nDLCI);

    return;
}


#endif /* APP_SUPPORT_UART_PLAY */
#endif /* __AT_MOD_MUSIC__ */

