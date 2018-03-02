/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
/*
 * MODULE NAME:     winpcap_for_wifi.c
 * DESCRIPTION:   simulate wincap as  wifi's MAC layer
 * AUTHOR:         huangyx
 * DATE:           20150518
 *
 1.Setup winpcap driver for OS
 2.Connect WiFi to AP
 3.Disable system WiFi TCP/IP protecols
 4.Set local MAC value in wifi_PowerOn
 */
#if defined(WIFI_SUPPORT)

#include "cswtype.h"
#include "Cos.h"
#include "Event.h"

#include "winpcap_for_wifi.h"


static pcap_t *g_adhandle = 0;
static HANDLE_WIN32 winpcap_rec_task_handler = 0;
static HANDLE_WIN32 winpcap_send_task_handler = 0;
int winpcap_init(void);
static int wifi_mac_data_RecvInd_win32(UINT8 *data, unsigned short len);


//
//External var
//
extern VOID ip_WifiConnectedInd(void);
extern BOOL ap_connected ;
extern BOOL ap_connecting;
extern UINT8 localMac[6] ;
extern void win32_trace(char* fmt,...);
#define winpcap_printf win32_trace //trace_null    //win32_trace



/******************************************************************************
 * winpcap_rec_task: 
 * DESCRIPTION: - 
 * 
 * Input: 
 * Output: 
 * Returns: 
 * 
 * 
 ******************************************************************************/
DWORD WINAPI winpcap_rec_task ( VOID* pData)
{
	int res = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	
	if(0!=g_adhandle)
	{
		//pcap_dispatch(g_adhandle, 0, packet_handler, NULL);
		//pcap_loop(g_adhandle, 0, packet_handler, NULL);
		while((res = pcap_next_ex(g_adhandle, &header, &pkt_data)) >= 0)
		{
		  if(res == 0)
		   /* Timeout elapsed */
		   continue;

#if (0)
		  winpcap_printf("[%d],%d len:%d\n", header->ts.tv_usec, header->caplen,header->len);

		  if(1)
		  {		      
			  winpcap_printf("destMac=%x:%x:%x:%x:%x:%x\nsrcMac=%x:%x:%x:%x:%x:%x\n	  type=%x",
			  *(pkt_data+0),*(pkt_data+1),*(pkt_data+2),*(pkt_data+3),*(pkt_data+4),*(pkt_data+5),
			  *(pkt_data+6),*(pkt_data+7),*(pkt_data+8),*(pkt_data+9),*(pkt_data+10),*(pkt_data+11),
			   ((*(pkt_data+12))<<8|(*(pkt_data+13)))
			  );
		  }
#elif 1
		wifi_mac_data_RecvInd_win32(pkt_data,header->len);
#else
		test_rda5890_data_rx(pkt_data,header->len);

#endif /* 1 */

		  

		}
	}
}

DWORD WINAPI winpcap_send_task ( VOID* pData)
{
	int res = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;

	if(0!=g_adhandle)
	{
		/*
		 * Send the just received packet to the output adaper
		 */
		if(pcap_sendpacket(g_adhandle, pkt_data, header->caplen) != 0)
		{
		
		}

	}
}

int winpcap_init(void)
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int inum;
	int i=0;
	pcap_t *adhandle;
	char errbuf[PCAP_ERRBUF_SIZE];
	u_int netmask;
	char packet_filter[] = "ip or arp" ;  //"arp";  //"ip and udp";
	struct bpf_program fcode;

	if(g_adhandle != 0)
	{
		return;
	}

	winpcap_printf( "Retrieving the device list..........");

	/* Retrieve the device list */
	//if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		winpcap_printf( "Error in pcap_findalldevs: %s\n", errbuf);
		return;
	}
	
	/* Print the list */
	for(d=alldevs; d; d=d->next)
	{
		winpcap_printf("%d. %s", ++i, d->name);
		if (d->description)
			winpcap_printf(" (%s)\n", d->description);
		else
			winpcap_printf(" (No description available)\n");
	}

	if(i==0)
	{
		winpcap_printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	
	winpcap_printf("Modify the interface number (1-%d) in function winpcap_init().",i);
	//scanf("%d", &inum);

	
	inum = 2;

	
	if(inum < 1 || inum > i)
	{
		winpcap_printf("\nInterface number out of range.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* Jump to the selected adapter */
	for(d=alldevs, i=0; i< inum-1 ;d=d->next, i++);
	
	/* Open the adapter */
	if ( (adhandle= pcap_open(d->name,	// name of the device
							 65536,		// portion of the packet to capture. 
										// 65536 grants that the whole packet will be captured on all the MACs.
							 (PCAP_OPENFLAG_PROMISCUOUS 
#if (1)
							 |PCAP_OPENFLAG_NOCAPTURE_LOCAL
#endif /* 0 */
							),			// promiscuous mode
							 1,		//1000  // read timeout
							 NULL,		// remote authentication
							 errbuf		// error buffer
							 ) ) == NULL)
	{
		winpcap_printf("\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* Check the link layer. We support only Ethernet for simplicity. */
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		winpcap_printf("\nThis program works only on Ethernet networks.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if(d->addresses != NULL)
		/* Retrieve the mask of the first address of the interface */
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* If the interface is without addresses we suppose to be in a C class network */
		netmask=0xffffff; 


	//compile the filter
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		winpcap_printf("\nUnable to compile the packet filter. Check the syntax.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	//set the filter
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		winpcap_printf("\nError setting the filter.\n");
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	winpcap_printf("\nlistening on <%d> %s...\n", inum, d->description);
	
	/* At this point, we don't need any more the device list. Free it */
	pcap_freealldevs(alldevs);

	/* 
	 * Start the threads that will forward the packets 
	 */


	
	/* start the capture */
	g_adhandle = adhandle;
//	pcap_dispatch(adhandle, 0, packet_handler, NULL);
	//pcap_loop(adhandle, 0, packet_handler, NULL);

	return 0;
}

void winpcap_deinit(void)
{
	if(0!=g_adhandle)
	{
		pcap_close(g_adhandle);
		g_adhandle = 0;
	}


	/* Kill the first thread. Not very gentle at all...*/
	if(0!=winpcap_rec_task_handler)
	{
		TerminateThread(winpcap_rec_task_handler, 0);
		winpcap_rec_task_handler = 0;
	}
	
	if(0!=winpcap_send_task_handler)
	{
		TerminateThread(winpcap_send_task_handler, 0);
		winpcap_send_task_handler = 0;
	}



}

VOID wifi_PowerOn(VOID)
{
    DWORD ThreadID;

    winpcap_rec_task_handler   =(HANDLE_WIN32)CreateThread(NULL,0,winpcap_rec_task,NULL,0,&ThreadID);
//    winpcap_send_task_handler =(HANDLE_WIN32)CreateThread(NULL,0,winpcap_send_task,NULL,0,&ThreadID);

	ap_connecting  = 1;
	ap_connected = 1;

	// TODO: Set your WiFi adpter's real MAC address below
	localMac[0] = 0x00;
	localMac[1] = 0x23;
	localMac[2] = 0xcd;
	localMac[3] = 0xaf;
	localMac[4] = 0x2b;
	localMac[5] = 0x36;
	
	ip_WifiConnectedInd();
}


int wifi_data_tx(unsigned char *data, unsigned short len)
{
	if(0!=g_adhandle)
	{
		/*
		 * Send the just received packet to the output adaper
		 */
		winpcap_printf( ">>>>test_rda5890_data_tx, len=%d\r\n\r\n\r\n",len);

		if(0 == pcap_sendpacket(g_adhandle, data, len))
		{
			return 1; //successfully
		}
	}

	return 0;//Error
}


#if defined(WIN32)
BOOL ap_connected =FALSE;
BOOL ap_connecting =FALSE;
BOOL wifi_sleep_flag = FALSE;
BOOL wifi_no_off = FALSE;
BOOL useingIrq = FALSE;
BOOL wifi_Connected = FALSE;

UINT8 check_wifi_rssi(void)
{
    return 0X0;
}

void wifi_inputEapolPkt(const UINT8 *src_addr,const UINT8 *buf, size_t len)
{
// 	wpa_sm_rx_eapol( &sm_cxt,src_addr,buf, len);

}

void wifi_PowerOff(void)
{
}
void wifi_test_rx(void)
{
}

INT rda5990_set_TxRate( UINT8 val)
{
}

VOID svc_WifiClose()
{
}


static int wifi_mac_data_RecvInd_win32(UINT8 *data, unsigned short len)
{
	COS_EVENT ev;
    UINT8 *send_data = NULL;
    
    send_data = (UINT8 *) COS_MALLOC(len);

    if(send_data == NULL)  return -1;
    
    memset(send_data, 0x0, len);
    memcpy(send_data, data, len);
    
	ev.nEventId = EV_MAC_WIFI_DATA_IND;
	ev.nParam1 = (UINT32)send_data;
	ev.nParam2 = len;

	winpcap_printf( "<<<<wifi_mac_data_RecvInd_win32, len=%d",len);
	
	COS_SendEvent(MOD_WIFI_REC,  &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return 0;
}
#endif /* WIN32_SDIO */

#endif /* WIFI_SUPPORT */
