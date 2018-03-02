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

#ifdef TARGET_SUPPORT_WIFI

#include "iot_wifi.h"

#define ERR_CREATE_SOCKET   -1
#define ERR_HOSTBYNAME      -2
#define ERR_CONNECT         -3
#define ERR_SEND            -4
#define ERR_TIMEOUT         -5
#define ERR_RECV            -6

#if !defined(MCU_WIFI_PLAT)

extern BOOL ap_connected;

// ============================================================================
// wifi_Open
// ----------------------------------------------------------------------------
/// WIFI module power on
/// 
/// @param VOID
// ============================================================================
VOID wifi_Open(VOID)
{
     wifi_PowerOn();
}

// ============================================================================
// wifi_Close
// ----------------------------------------------------------------------------
/// WIFI module power off
/// 
/// @param VOID
// ============================================================================
VOID wifi_Close(VOID)
{
    wifi_PowerOff();
}


// ============================================================================
// wifi_GetStatus
// ----------------------------------------------------------------------------
/// Get WIFI connection status
/// 
/// @param VOID
// ============================================================================
UINT8 wifi_GetStatus(VOID)
{
	return ap_connected;
}
#endif

// ============================================================================
// iot_Connect
// ----------------------------------------------------------------------------
/// Connect to URL like "www.rda.com" or ip address like "127.0.0.1"
/// @param addr
///
/// @param portno port number
/// @return
// ============================================================================
INT32 iot_Connect(const char *addr, INT16 portno)
{
	INT32 sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server = NULL;
    UINT32 ipaddr = 0;

#if 0
    if ((ipaddr = inet_addr(addr)) == IPADDR_NONE)
    {
        server = gethostbyname(addr);
        if (server == NULL) {
            //fprintf(stderr, "ERROR, no such host\n");
            //app_trace(APP_MAIN_TRC,"ERROR, no such host\n");
            iot_Printf("Error, no such host\n");
            return ERR_HOSTBYNAME;
        }
    }
#endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        iot_Printf("Error open socket\n");
        return ERR_CREATE_SOCKET;
    }

    memset((char *) &serv_addr, sizeof(serv_addr), 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(addr);

#if 0
    if (server != NULL)
    {
    	//memcpy((char *)server->h_addr,
        //       (char *)&serv_addr.sin_addr.s_addr,
        //       server->h_length);
    	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr,  server->h_length);
    }
    else
    {
        //memcpy((char *)server->h_addr,
        //        (char *)ipaddr,
        //        4); //only support ipv4
        serv_addr.sin_addr.s_addr = inet_addr(addr);
    }
#endif

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        iot_Printf("Error connecting\n");
        return ERR_CONNECT;
    }

    return sockfd;
}

// ============================================================================
// iot_Send
// ----------------------------------------------------------------------------
/// Send data to server
///
/// @param VOID
// ============================================================================
INT32 iot_Send(INT32 sockfd, const INT8* buffer, UINT32 len)
{
	INT32 total  = 0;
	INT32 n = 0;
    while (len != total)
    {
        /* send len - total bytes at one time*/
        n = send(sockfd,buffer + total,len - total,0);
        if (n <= 0)
        {
            iot_Printf("iot_send failed\n");
            return n;
        }
        /* Send n bytes success */
        total += n;
    }

    return total;
}

// ============================================================================
// iot_Recv
// ----------------------------------------------------------------------------
/// Read data from server
///
/// @param VOID
// ============================================================================
INT32 iot_Recv(INT32 sockfd, INT8 *buffer, UINT32 len)
{
	//no-blocking recv
    return recv(sockfd,buffer,len,0x08);
}

// ============================================================================
// iot_Close
// ----------------------------------------------------------------------------
/// Close the tcp socket
///
/// @param VOID
// ============================================================================
INT32 iot_Close(INT32 sockfd)
{
	return close(sockfd);

}

#endif // TARGET_SUPPORT_WIFI
