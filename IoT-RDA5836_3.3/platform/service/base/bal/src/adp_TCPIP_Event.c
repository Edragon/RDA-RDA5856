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

#if 0//defined(__SLIMMMI_TCPIP__)
#include "ap_media.h"
#include "event.h"
#include "adp_events.h"

#include "tcpip_opt.h"


#ifdef __MMI_WLAN_FEATURES__

#endif /* __MMI_WLAN_FEATURES__ */

#include "soc_api.h"


INT8 g_nSocCid  = -1;


PFN_TCPIP_CALLBACK pfn_TCPIPCB[MEMP_NUM_NETCONN+1] = {NULL,};

UINT32 CFW_SetTCPIPCallBackEx ( PFN_TCPIP_CALLBACK pfnEv, UINT8 SocketId )
{
    app_trace (APP_ADP_TRC, "ADP_GPRS: CFW_SetTCPIPCallBackEx, socket ID: %d, callback: 0x%x",SocketId,pfnEv) ;

    if( SocketId > MEMP_NUM_NETCONN)
        return ERR_INVALID_PARAMETER;

    pfn_TCPIPCB[SocketId] = pfnEv;

    return ERR_SUCCESS;
}


#if 1
HANDLE g_TCPIPRequestTaskID[MEMP_NUM_NETCONN+1] = {NULL,};
UINT32 CFW_SetTCPIPRequestTaskIDEx ( HANDLE taskID, UINT8 SocketId )
{
    app_trace (APP_ADP_TRC, "ADP_GPRS: CFW_SetTCPIPRequestTaskIDEx, socket ID: %d, taskID: 0x%x",SocketId,taskID) ;


    if( SocketId > MEMP_NUM_NETCONN)
        hal_DbgAssert("CFW_SetTCPIPRequestTaskIDEx SocketId > MEMP_NUM_NETCONN");

    g_TCPIPRequestTaskID[SocketId] = taskID;

#if defined(__rda_internal_debug__)
    {
        UINT8 i = 0;
        UINT8 j = 0;
        UINT8 sameTCPIPRequestTaskIDCnt = 0;
        HANDLE checkerTCPIPRequestTaskHANDLE[MEMP_NUM_NETCONN+1]= {0,};
        UINT8 checkerTCPIPRequestSocketID[MEMP_NUM_NETCONN+1]= {0,};
        for (i=0; i<MEMP_NUM_NETCONN; i++)
        {
            if (g_TCPIPRequestTaskID[i] == taskID)
            {
                if (taskID)
                {
                    checkerTCPIPRequestTaskHANDLE[j]=g_TCPIPRequestTaskID[i];
                    checkerTCPIPRequestSocketID[j]=i;
                    sameTCPIPRequestTaskIDCnt ++;
                    j++;
                }
            }
        }

        if (sameTCPIPRequestTaskIDCnt > 1 )
        {
            TS_OutputText(CSW_TS_ID, "CFW_SetTCPIPRequestTaskIDEx id:%d HANDLE:%x\n",SocketId,taskID);
            for (i=0; i<sameTCPIPRequestTaskIDCnt; i++)
            {
                TS_OutputText(CSW_TS_ID, "Dump index:%d id:%d HANDLE:%x %s\n",
                              i,
                              checkerTCPIPRequestSocketID[i],
                              checkerTCPIPRequestTaskHANDLE[i],
                              sxr_GetTaskNameById(((TASK_HANDLE*)checkerTCPIPRequestTaskHANDLE[i])->nTaskId));
            }
            hal_DbgAssert("CFW_SetTCPIPRequestTaskIDEx2");
        }
    }
#endif
    return ERR_SUCCESS;
}

HANDLE CFW_GetTCPIPRequestTaskIDEx ( UINT8 SocketId )
{
    if( SocketId > MEMP_NUM_NETCONN)
        hal_DbgAssert("CFW_SetTCPIPRequestTaskIDEx SocketId > MEMP_NUM_NETCONN");

    return g_TCPIPRequestTaskID[SocketId];
}

BOOL CFW_FindTCPIPRequestTaskIDEx ( HANDLE taskID)
{
    UINT8 i = 0;

    app_trace (APP_ADP_TRC, "ADP_GPRS: CFW_FindTCPIPRequestTaskIDEx taskID:0x%x",taskID) ;

    for (i=0; i<MEMP_NUM_NETCONN; i++)
    {
        if (g_TCPIPRequestTaskID[i] == taskID)
        {
            hal_DbgAssert("CFW_FindTCPIPRequestTaskIDEx id:%d HANDLE:%x\n",i,g_TCPIPRequestTaskID[i]);
            return TRUE;
        }
    }
    return FALSE;
}

UINT8 CFW_GetTCPIPRequestTaskIDindex(HANDLE taskID)
{
    UINT8 i = 0;

    for (i=0; i<MEMP_NUM_NETCONN; i++)
    {
        if (g_TCPIPRequestTaskID[i] == taskID)
            return i;
    }
    return 0xff;
}

app_sco_timeoutval g_TCPIPTimeoutDataCnt[MEMP_NUM_NETCONN+1]= {NULL,};
app_sco_timeoutval g_TCPIPTimeoutDataMask[MEMP_NUM_NETCONN+1]= {NULL,};

BOOL isTCPIPTimeoutRunning = 0;
BOOL isTCPIPTimeoutInit = 0;

UINT32 soc_tcpipTimeoutProcessHandler (void)
{
    UINT8 i = 0;
    COS_EVENT EV = { 0 ,};
    UINT32 scStatus;
    sxr_StopFunctionTimer(soc_tcpipTimeoutProcessHandler);

    scStatus = COS_EnterCriticalSection();

    for (i=0; i<MEMP_NUM_NETCONN; i++)
    {
        if (0 < g_TCPIPTimeoutDataCnt[i].connTimeOut)
        {
            g_TCPIPTimeoutDataCnt[i].connTimeOut--;
        }
        else if (0 == g_TCPIPTimeoutDataCnt[i].connTimeOut)
        {
            TS_OutputText(CSW_TS_ID, "soc_tcpip connTimeOut id:%d ",i);
//          hal_DbgAssert("soc_tcpip connTimeOut sockID:%d ", i, __func__, __LINE__);
#if defined(__SXR__)
            appSendMessage2Netdevice(EV_SOC_TCPIP_SOCKET_TIMEOUT_RSP, i, 0, 0);
#endif /* __SXR__ */
            g_TCPIPTimeoutDataCnt[i].connTimeOut = -1;
        }
        else if (0 > g_TCPIPTimeoutDataCnt[i].connTimeOut)
        {
            if (0 < g_TCPIPTimeoutDataCnt[i].commTimeOut)
            {
                g_TCPIPTimeoutDataCnt[i].commTimeOut--;
            }
            else if (0 == g_TCPIPTimeoutDataCnt[i].commTimeOut)
            {
//              hal_DbgAssert("soc_tcpip connTimeOut id:%d ", i, __func__, __LINE__);
                TS_OutputText(CSW_TS_ID, "soc_tcpip commTimeOut sockID:%d ",i);
#if defined(__SXR__)
                appSendMessage2Netdevice(EV_SOC_TCPIP_SOCKET_TIMEOUT_RSP, i, 0, 0);
#endif /* __SXR__ */
                g_TCPIPTimeoutDataCnt[i].commTimeOut = -1;
            }
            else if (0 > g_TCPIPTimeoutDataCnt[i].commTimeOut)
            {

            }
        }
    }
    COS_ExitCriticalSection(scStatus);

    sxr_StartFunctionTimer(100 MS_WAITING , soc_tcpipTimeoutProcessHandler, NULL, 0);
    return ERR_SUCCESS;
}

UINT32 soc_tcpipSetTimeoutValEx (app_sco_timeoutval *Timeout100mSec, UINT8 SocketId)
{
    UINT8 i = 0;
    UINT8 checker = 0;
    UINT32 scStatus;

    app_trace (APP_ADP_TRC, "soc_tcpipSetTimeoutValEx") ;

    if( SocketId > MEMP_NUM_NETCONN)
        return ERR_INVALID_PARAMETER;

    scStatus = COS_EnterCriticalSection();

    if (!isTCPIPTimeoutInit)
    {
        for (i=0; i<MEMP_NUM_NETCONN; i++)
        {
            g_TCPIPTimeoutDataCnt[i].connTimeOut = -1;
            g_TCPIPTimeoutDataCnt[i].commTimeOut = -1;
            g_TCPIPTimeoutDataMask[i].connTimeOut = -1;
            g_TCPIPTimeoutDataMask[i].commTimeOut = -1;
        }
        isTCPIPTimeoutInit = TRUE;
    }

    g_TCPIPTimeoutDataMask[SocketId].connTimeOut = Timeout100mSec->connTimeOut;
    g_TCPIPTimeoutDataMask[SocketId].commTimeOut = Timeout100mSec->commTimeOut;

    g_TCPIPTimeoutDataCnt[SocketId].connTimeOut = g_TCPIPTimeoutDataMask[SocketId].connTimeOut;
    g_TCPIPTimeoutDataCnt[SocketId].commTimeOut= g_TCPIPTimeoutDataMask[SocketId].commTimeOut;

    if (!isTCPIPTimeoutRunning)
    {
        for (i=0; i<MEMP_NUM_NETCONN; i++)
        {
            if ((g_TCPIPTimeoutDataCnt[i].connTimeOut > 0) ||
                (g_TCPIPTimeoutDataCnt[i].commTimeOut > 0))
            {
                sxr_StartFunctionTimer(100 MS_WAITING, soc_tcpipTimeoutProcessHandler, NULL, 0);
                isTCPIPTimeoutRunning = TRUE;
                break;
            }
        }
    }
    else
    {
        for (i=0; i<MEMP_NUM_NETCONN; i++)
        {
            if ((g_TCPIPTimeoutDataCnt[i].connTimeOut < 0) &&
                (g_TCPIPTimeoutDataCnt[i].commTimeOut < 0))
            {
                checker++;
            }
        }
        if ( MEMP_NUM_NETCONN == checker)
        {
            sxr_StopFunctionTimer(soc_tcpipTimeoutProcessHandler);
            isTCPIPTimeoutRunning = FALSE;
        }
    }

    COS_ExitCriticalSection(scStatus);
    return ERR_SUCCESS;
}

UINT32 soc_tcpipGetTimeoutValEx (app_sco_timeoutval *Timeout100mSec, UINT8 SocketId)
{
    UINT32 scStatus;

    app_trace (APP_ADP_TRC, "soc_tcpipGetTimeoutValEx") ;

    if( SocketId > MEMP_NUM_NETCONN)
        return ERR_INVALID_PARAMETER;

    scStatus = COS_EnterCriticalSection();

    Timeout100mSec->connTimeOut = g_TCPIPTimeoutDataMask[SocketId].connTimeOut;
    Timeout100mSec->commTimeOut = g_TCPIPTimeoutDataMask[SocketId].commTimeOut;

    COS_ExitCriticalSection(scStatus);
    return ERR_SUCCESS;
}

UINT32  soc_tcpipReloadTimeoutCnt (UINT8 SocketId, UINT32 nParam1)
{
    UINT8 i = 0;
    UINT8 checker = 0;
    UINT32 scStatus;

    app_trace (APP_ADP_TRC, "soc_tcpipReloadTimeoutCnt",SocketId) ;

    if( SocketId > MEMP_NUM_NETCONN)
        return ERR_INVALID_PARAMETER;
    if (!isTCPIPTimeoutRunning || !isTCPIPTimeoutInit)
        return ERR_INVALID_PARAMETER;

    scStatus = COS_EnterCriticalSection();

    if ((-2 != g_TCPIPTimeoutDataCnt[SocketId].connTimeOut) &&
        (EV_CFW_TCPIP_SOCKET_CONNECT_RSP == nParam1))
    {
        g_TCPIPTimeoutDataCnt[SocketId].connTimeOut = -1;
    }

    if (-2 != g_TCPIPTimeoutDataMask[SocketId].commTimeOut)
        g_TCPIPTimeoutDataCnt[SocketId].commTimeOut= g_TCPIPTimeoutDataMask[SocketId].commTimeOut;

    COS_ExitCriticalSection(scStatus);

    return ERR_SUCCESS;
}
#endif

void soc_callappcb(int socketid, UINT8 event, BOOL result, INT8 cause, void *msg_ptr )
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    app_soc_notify_ind_struct ind_ptr = {0};

    ind_ptr.socket_id = socketid;
    ind_ptr.event_type = event;
    ind_ptr.result = result;
    ind_ptr.error_cause = cause;
    ind_ptr.msg_ptr = msg_ptr;

    if( NULL != pfn_TCPIPCB[socketid] )
    {
        (*pfn_TCPIPCB[socketid])(&ind_ptr);
    }

}


BOOL app_SocketEventcb (COS_EVENT *pstCosEv)
{
    UINT8 addr[4];
    UINT8 addr_len;
    INT8  *hostname;
    INT8      nsimID;
    INT8      ncid;

    int i;
    BOOL bIsProcessed = TRUE;

    switch (pstCosEv->nEventId)
    {
        case EV_CFW_TCPIP_SOCKET_CONNECT_RSP:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, EV_CFW_TCPIP_SOCKET_CONNECT_RSP);

            app_trace(APP_ADP_TRC," EV_CFW_TCPIP_SOCKET_CONNECT_RSP Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_CONNECT, TRUE, 0, 0);
            break ;

        case EV_CFW_TCPIP_SOCKET_SEND_RSP:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, NULL);
            app_trace(APP_ADP_TRC," EV_CFW_TCPIP_SOCKET_SEND_RSP Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_WRITE, TRUE, 0, 0);
            break ;
        case EV_CFW_TCPIP_REV_DATA_IND:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, NULL);
            app_trace(APP_ADP_TRC," EV_CFW_TCPIP_REV_DATA_IND Func: %s %d", __FUNCTION__, pstCosEv->nParam2);
            soc_callappcb(pstCosEv->nParam1, SOC_READ, TRUE, 0, 0);
            break ;
        case EV_CFW_TCPIP_ACCEPT_IND:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, NULL);
            app_trace(APP_ADP_TRC," EV_CFW_TCPIP_ACCEPT_IND Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_ACCEPT, TRUE, 0, 0);
            break ;

        case EV_CFW_TCPIP_ERR_IND:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, NULL);
            app_trace(APP_ADP_TRC,"SOC: EV_CFW_TCPIP_ERR_IND Func: %s ", __FUNCTION__);
            if( CFW_ERR_TCPIP_CONN_TIMEOUT == pstCosEv->nParam2 )
            {
                soc_callappcb(pstCosEv->nParam1, SOC_CONNECT, FALSE, 0, 0);
            }
            else
            {
                soc_callappcb(pstCosEv->nParam1, SOC_ERROR_IND, 0, pstCosEv->nParam2, CFW_TcpipGetLastError());
            }
            break ;

        case EV_CFW_TCPIP_CLOSE_IND:
            soc_tcpipReloadTimeoutCnt(pstCosEv->nParam1, NULL);
            app_trace(APP_ADP_TRC,"SOC: EV_CFW_TCPIP_CLOSE_IND Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_CLOSE, TRUE, 0, 0);
            break ;

        case EV_CFW_TCPIP_SOCKET_CLOSE_RSP:
            app_trace(APP_ADP_TRC," EV_CFW_TCPIP_SOCKET_CLOSE_RSP Func: %s ", __FUNCTION__);
            //soc_callappcb(pstCosEv->nParam1, SOC_CLOSE, TRUE, 0, 0);
            break ;

        case EV_CFW_DNS_RESOLV_SUC_IND:
            app_trace(APP_ADP_TRC," EV_CFW_DNS_RESOLV_SUC_IND Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_DNS_RESOLV, TRUE, 0, 0);
            break ;

        case EV_CFW_DNS_RESOLV_ERR_IND:
            app_trace(APP_ADP_TRC,"SOC: EV_CFW_DNS_RESOLV_ERR_IND Func: %s ", __FUNCTION__);
            soc_callappcb(pstCosEv->nParam1, SOC_DNS_RESOLV, FALSE, 0, 0);
            break ;


        default:
            app_trace(APP_ADP_TRC,"SOC: jdd_SocketEventcb : Unknown case") ;
            bIsProcessed = FALSE ;
            break ;
    }

    return bIsProcessed ;

}

#endif//__GPRS_MODE__
