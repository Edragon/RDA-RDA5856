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

/*
 * MODULE NAME:    hci_transport_win32.c
 * DESCRIPTION:    HCI Transport Driver Implementation for Win32 - USB HCI bus
 * AUTHOR:         Denis Gleeson
 * DATE:           20 March 2002
 *
 * SOURCE CONTROL: $Id: hcit_win32_usb.c,v 1.16 2009/09/25 11:15:18 jianguo Exp $
 *
 *
 */

#include "bt.h"
#include "bt_types.h"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include "winioctl.h"

//#include "hci_transport_win32.h" 
#include "hci.h"
//#include "bt_ctrl_win32.h"
#include "papi.h"

#include "hcit_win32_usb.h"

//Zhu Jianguo added it. 2008.2.1
#include "objbase.h"
#include "setupapi.h"
#include "initguid.h"
//end of added by Zhu Jianguo. 2008.2.1

#include "csrbc_usb.h"

#include "rdabt_main.h"

#define _T(txt)   txt

#ifdef _tcs_MODULENAME
#undef _tcs_MODULENAME
#endif
#ifdef MODULENAME
#undef MODULENAME
#endif
#define _tcs_MODULENAME _T("uHCIt")
#define MODULENAME "uHCIt"


//A5DCBF10-6530-11D2-901F-00C04FB951ED   //common USB GUID 
DEFINE_GUID(USBGuid, 0xA5DCBF10,0x6530,0x11D2,0x90,0x1F,0x00,0xC0,0x4F,0xB9,0x51,0xED);


#if pDEBUG
#	define DEBUGBUFLEN		4096 /* used for debugging output only */
#endif

#if pDEBUG
    static char *transportTypes[] = {        
        "HCI Command",
        "ACL Data",
        "SCO Data",
        "HCI Event"
    };
#endif

typedef struct _usb_data{
    HANDLE Driver_Handle;
    HANDLE Event_Thread_Handle;
    HANDLE ACL_Thread_Handle;
    BOOL Thread_Exit;
} st_usb_transport_data;

#define READ_BUFFER_SIZE 256
#define POLL_SLEEPTIME 10 
#define OUT_BUFFER_LEN 256

//static DWORD Error_Code=ERROR_SUCCESS;
//static TCHAR Error_Message[1024]; /* Must be big enough for the sprintf()s below */

static HANDLE Thread_Sync_Event;

static st_usb_transport_data *transport_data;

static BOOL Driver_Open_USB(void *platformConfig);
static DWORD WINAPI Event_Thread_USB(LPVOID); 
static DWORD WINAPI ACL_Thread_USB(LPVOID); 

BOOL Select_Interface(u_int8 Alter_interface);


t_api USB_HCIt_Init(struct st_TransportConfig_Win32 *platformConfig) 
{
    DWORD Error_Code=ERROR_SUCCESS;
    TCHAR Error_Message[1024]; /* Must be big enough for the sprintf()s below */
    DWORD Ind_Thread_ID; /* Unused */
    HANDLE Driver_Handle=INVALID_HANDLE_VALUE;
    
	if (Driver_Open_USB(&Driver_Handle)) 
        {
            transport_data = (st_usb_transport_data *)rdabt_malloc(sizeof(st_usb_transport_data));

            transport_data->Driver_Handle = Driver_Handle;
            transport_data->Thread_Exit = 0;

            Thread_Sync_Event=CreateEvent(NULL,TRUE,FALSE,NULL);
		if (Thread_Sync_Event==INVALID_HANDLE_VALUE) {
			Error_Code=GetLastError();
			sprintf(Error_Message,_T("%s: CreateEvent() returned error code %d"),_tcs_MODULENAME,Error_Code);
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s\n"));
			return RDABT_NORESOURCES;
		}

		transport_data->Event_Thread_Handle=CreateThread(NULL,0,Event_Thread_USB,platformConfig,0,&Ind_Thread_ID);
		if (transport_data->Event_Thread_Handle==INVALID_HANDLE_VALUE) {
			Error_Code=GetLastError();
			sprintf(Error_Message,_T("%s: CreateThread() returned error code %d"),_tcs_MODULENAME,Error_Code);
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s\n")); 
                     USB_HCIt_Shutdown(platformConfig);
			return RDABT_NORESOURCES;
		}

		transport_data->ACL_Thread_Handle=CreateThread(NULL,0,ACL_Thread_USB,platformConfig,0,&Ind_Thread_ID);
		if (transport_data->ACL_Thread_Handle==INVALID_HANDLE_VALUE) {
			Error_Code=GetLastError();
			sprintf(Error_Message,_T("%s: CreateThread() returned error code %d"),_tcs_MODULENAME,Error_Code);
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s\n")); 
                     USB_HCIt_Shutdown(platformConfig);
			return RDABT_NORESOURCES;
		}
		/* Wait for the thread to start */
            WaitForSingleObject(Thread_Sync_Event,INFINITE);
            CloseHandle(Thread_Sync_Event);

		return RDABT_NOERROR;
	} else {
		return RDABT_HCITERROR;
	}	
}

t_api USB_HCIt_Shutdown(struct st_TransportConfig_Win32 *platformConfig) {
    transport_data->Thread_Exit=TRUE;
    if (transport_data->Driver_Handle!=INVALID_HANDLE_VALUE) {
        CloseHandle(transport_data->Driver_Handle);
    }
    Sleep(100); // let thread terminate grecefully if possible
    if (transport_data->Event_Thread_Handle!=INVALID_HANDLE_VALUE) {
        TerminateThread(transport_data->Event_Thread_Handle,0);
	 CloseHandle(transport_data->Event_Thread_Handle);
    }
    if (transport_data->ACL_Thread_Handle!=INVALID_HANDLE_VALUE) {
        TerminateThread(transport_data->ACL_Thread_Handle,0);
	 CloseHandle(transport_data->ACL_Thread_Handle);
    }
    rdabt_free(transport_data);
    return RDABT_NOERROR;
}


BOOL Driver_Open_USB(HANDLE *handle) 
{
	int ReqLength=0;
	int Length;
	int Flags=DIGCF_PRESENT|DIGCF_DEVICEINTERFACE;
	HANDLE hDevInfo;
	int Index =1;
	BOOL status;
	int  MemberIndex = 0;
	BOOL    MyDeviceDetected = FALSE;
	char completeDeviceName[256] = ""; 
        DWORD Error_Code=ERROR_SUCCESS;
        TCHAR Error_Message[1024]; /* Must be big enough for the sprintf()s below */
        HANDLE Driver_Handle=INVALID_HANDLE_VALUE;

	OSVERSIONINFO  osversioninfo = { sizeof(OSVERSIONINFO) };

    char buffer[READ_BUFFER_SIZE];
    DWORD Bytes_Read;

	PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceDetailData;
	SP_DEVICE_INTERFACE_DATA        DeviceInterfaceData;

	SP_DEVINFO_DATA devinfo;
	devinfo.cbSize = sizeof(SP_DEVINFO_DATA);
	memset(&DeviceInterfaceData,0,sizeof(SP_DEVICE_INTERFACE_DATA));
	DeviceInterfaceData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);

	GetVersionEx(&osversioninfo);

	hDevInfo=SetupDiGetClassDevs(&USBGuid,NULL,NULL,Flags);

	Index = 0;
	do{
		status=SetupDiEnumDeviceInterfaces(hDevInfo,NULL,&USBGuid,Index, &DeviceInterfaceData);
		if(!status)
			Error_Code=GetLastError();
		else
		{
			SetupDiGetInterfaceDeviceDetail(hDevInfo,&DeviceInterfaceData,NULL,0,&Length,NULL);
			DeviceDetailData=(PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(Length);
			if(!DeviceDetailData)
			{
				Error_Code=GetLastError();
				sprintf(Error_Message,_T("%s: USB_Open() :- ERROR NOT ENOUGH MEMORY! returned error code %d"),_tcs_MODULENAME,Error_Code);
				pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s\n",Error_Message));
				continue;
			}
			memset(DeviceDetailData,0,sizeof(PSP_INTERFACE_DEVICE_DETAIL_DATA));
			ReqLength = Length;
			DeviceDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
			
			status=SetupDiGetInterfaceDeviceDetail(hDevInfo,&DeviceInterfaceData,DeviceDetailData,Length,&ReqLength,&devinfo);
			if(!status)
			{
				Error_Code=GetLastError();
				free(DeviceDetailData);
				DeviceDetailData=NULL;
			}
				if( (osversioninfo.dwMajorVersion == 5)  && (osversioninfo.dwMinorVersion == 1))  //xp
				{
					if(devinfo.ClassGuid.Data1 == 0x36FC9E60 //our usb dirver's GUID for xp not install IVT
						&& devinfo.ClassGuid.Data2 == 0xC465
						&& devinfo.ClassGuid.Data3 == 0x11CF
						&& devinfo.ClassGuid.Data4[0] == 0x80
						&& devinfo.ClassGuid.Data4[1] == 0x56
						&& devinfo.ClassGuid.Data4[2] == 0x44
						&& devinfo.ClassGuid.Data4[3] == 0x45
						&& devinfo.ClassGuid.Data4[4] == 0x53
						&& devinfo.ClassGuid.Data4[5] == 0x54
						&& devinfo.ClassGuid.Data4[6] == 0x00
						&& devinfo.ClassGuid.Data4[7] == 0x00)
					Driver_Handle=CreateFile(DeviceDetailData->DevicePath,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
				}
 
				else if( (osversioninfo.dwMajorVersion == 6) && (osversioninfo.dwMinorVersion == 1))  //win 7
				{
				//e0cbf06c-cd8b-4647-bb8a-263b43f0f974			
					if(devinfo.ClassGuid.Data1 == 0xe0cbf06c //our usb dirver's GUID for WIN 7 not install IVT
						&& devinfo.ClassGuid.Data2 == 0xcd8b
						&& devinfo.ClassGuid.Data3 == 0x4647
						&& devinfo.ClassGuid.Data4[0] == 0xbb
						&& devinfo.ClassGuid.Data4[1] == 0x8a
						&& devinfo.ClassGuid.Data4[2] == 0x26
						&& devinfo.ClassGuid.Data4[3] == 0x3b
						&& devinfo.ClassGuid.Data4[4] == 0x43
						&& devinfo.ClassGuid.Data4[5] == 0xf0
						&& devinfo.ClassGuid.Data4[6] == 0xf9
						&& devinfo.ClassGuid.Data4[7] == 0x74)
					Driver_Handle=CreateFile(DeviceDetailData->DevicePath,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

				}
						free(DeviceDetailData);

				if(Driver_Handle!=INVALID_HANDLE_VALUE) 
					break;
		}

		Index ++;


	}while(Error_Code != ERROR_NO_MORE_ITEMS);

	if(Error_Code == ERROR_NO_MORE_ITEMS)
		return FALSE;

	//Driver_Handle=CreateFile(DeviceDetailData->DevicePath,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

	if(Driver_Handle==INVALID_HANDLE_VALUE) 
	{
		Error_Code=GetLastError();
		sprintf(Error_Message,_T("%s: USB_Open() :- CreateFile()returned error code %d"),_tcs_MODULENAME,Error_Code);
		pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s\n",Error_Message));
		return FALSE;
	}  
       *handle = Driver_Handle;
    // empty usb buffer
    do {
	    DeviceIoControl(Driver_Handle,
					IOCTL_CSRBC_GET_HCI_DATA,
					0,
					0,
					buffer,
					READ_BUFFER_SIZE,
					&Bytes_Read,
					NULL);
    }while(Bytes_Read);
    do {
	    DeviceIoControl(Driver_Handle,
					IOCTL_CSRBC_GET_HCI_EVENT,
					0,
					0,
					buffer,
					READ_BUFFER_SIZE,
					&Bytes_Read,
					NULL);
    }while(Bytes_Read);
    
	return TRUE;
}

t_api BT_Transport_RX_PDU(struct st_TransportConfig_Win32 *transport, t_DataBuf *pdu, u_int8 type)
{
	static HANDLE rx_mutex;
    adp_data_msg_t data_msg;
    extern BOOL g_test_mode;

	//pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"------------------------BT_Transport_RX_PDU--------------------"));
	if(rx_mutex == NULL)
	{
		rx_mutex = CreateEvent(NULL,FALSE,TRUE,NULL);
	}
	WaitForSingleObject(rx_mutex,INFINITE);
    data_msg.buff = pdu;
    data_msg.type = type;
    pdu->transport=TRANSPORT_UART_ID;
    //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"BT_Transport_RX_PDU Rx packet type=%d, buff=0x%x", type, pdu));
    RDABT_Send_Message(ADP_DATA_IND, RDABT_ADP, RDABT_HCI, sizeof(adp_data_msg_t), &data_msg);
    if(g_test_mode)
    {
        if(type==HCI_pduEVENT)
        {
            if(pdu->buf[0]==HCI_evEXTENDEDINQUIRYRESULT||pdu->buf[0]==HCI_evINQUIRYRESULTWITHRSSI||pdu->buf[0]==HCI_evINQUIRYRESULT)
                rdabt_send_msg_up(EV_BT_READY_IND, 0);
            else if(pdu->buf[0]==HCI_evINQUIRYCOMPLETE)
                rdabt_send_msg_up(EV_BT_READY_IND, 1);
        }
    }
    rdabt_send_notify();
	SetEvent(rx_mutex);
    return RDABT_NOERROR;
}

DWORD WINAPI Event_Thread_USB(LPVOID Arg) 
{
    t_DataBuf *Read_Buffer;
    BYTE packetType;
	int i = 0;
    DWORD Bytes_Read, Error_Code;
    BOOL Result = TRUE;
    u_int8 buffer[READ_BUFFER_SIZE];
    DWORD ReadedLen=0, total_length = 0;
    TCHAR Error_Message[1024]; /* Must be big enough for the sprintf()s below */
    struct st_TransportConfig_Win32 *platformConfig=(struct st_TransportConfig_Win32 *)Arg;

    Bytes_Read=0;


    SetEvent(Thread_Sync_Event);  /* Notify the init function that the thread is running */


    do {
	Result=DeviceIoControl(transport_data->Driver_Handle,
					IOCTL_CSRBC_GET_HCI_EVENT,//IOCTL_CSRBC_GET_HCI_DATA,
					0,
					0,
					buffer,//Read_Buffer_Data->data,
					READ_BUFFER_SIZE,
					&Bytes_Read,//&Bytes_Read1,
					NULL);
            if(transport_data->Thread_Exit)
            {
                return 0;
            }
        	if(!Result)
        	{

        			Error_Code=GetLastError();
        		pDebugPrintfEX((pLOGERROR,pLOGHCI,"Event_Thread_USB %s: ReadFile() for %d bytes returned error code %d after %d bytes read - rx thread aborting\n",MODULENAME,READ_BUFFER_SIZE,Error_Code,Bytes_Read));
                   // continue;
        		return 0;
        	}

        if(Bytes_Read != 0)
        {
            total_length = buffer[1] +2;
            HCI_GetReceiveBuffer(&Read_Buffer, total_length);
            if (!(Read_Buffer)) 
            {  
			sprintf(Error_Message,_T("%s: Ind_Thread() :- Cannot allocate %d bytes of memory"),_tcs_MODULENAME, total_length);
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s",Error_Message));
			return RDABT_NORESOURCES;
		}                             
            packetType = HCI_pduEVENT;
            memcpy(Read_Buffer->buf, buffer, Bytes_Read);
            ReadedLen = Bytes_Read;
            while(ReadedLen < total_length)
            {
                	Result=DeviceIoControl(transport_data->Driver_Handle,
					IOCTL_CSRBC_GET_HCI_EVENT,//IOCTL_CSRBC_GET_HCI_DATA,
					0,
					0,
					Read_Buffer->buf+ReadedLen,//Read_Buffer_Data->data,
					total_length-ReadedLen,
					&Bytes_Read,//&Bytes_Read1,
					NULL);
                    ReadedLen+=Bytes_Read;
            }
            Read_Buffer->dataLen = ReadedLen;
            /*pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"++++++++++++++++++++++++++%s: Read total %d bytes of %s PDU, dump: ",MODULENAME, Read_Buffer->dataLen,packetType-1<4?transportTypes[packetType-1]:"INVALID"));
            for(i = 0; i < Read_Buffer->dataLen; i++)
			{
				pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"0x%x:", Read_Buffer->buf[i]));
			}*/
			//pDebugDumpEX(pLOGDEBUG,pLOGHCI,Read_Buffer->buf,Read_Buffer->dataLen);

            BT_Transport_RX_PDU(platformConfig, Read_Buffer,(u_int8)packetType);
        }
        else
        {
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"usb driver: No data read, sleep %d millisecord", POLL_SLEEPTIME));
            Sleep(POLL_SLEEPTIME);
        }
    } while (!transport_data->Thread_Exit);

	
    pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"%s: Got thread exit event in Event_Ind_Thread() - terminating\n",MODULENAME));
    return 0;
}

DWORD WINAPI ACL_Thread_USB(LPVOID Arg) 
{
    t_DataBuf *Read_Buffer;
    BYTE packetType;
    DWORD Bytes_Read, Error_Code;
    BOOL Result = TRUE;
    u_int8 buffer[READ_BUFFER_SIZE];
    DWORD ReadedLen=0, total_length = 0;
    TCHAR Error_Message[1024]; /* Must be big enough for the sprintf()s below */
    struct st_TransportConfig_Win32 *platformConfig=(struct st_TransportConfig_Win32 *)Arg;

    Bytes_Read=0;

    do {
	Result=DeviceIoControl(transport_data->Driver_Handle,
					IOCTL_CSRBC_GET_HCI_DATA,
					0,
					0,
					buffer,
					READ_BUFFER_SIZE,
					&Bytes_Read,
					NULL);
            if(transport_data->Thread_Exit)
            {
                return 0;
            }
        	if(!Result)
        	{

        			Error_Code=GetLastError();
        		pDebugPrintfEX((pLOGERROR,pLOGHCI,"ACL_Thread_USB %s: ReadFile() for %d bytes returned error code %d after %d bytes read - rx thread aborting\n",MODULENAME,READ_BUFFER_SIZE,Error_Code,Bytes_Read));
                    //continue;
        		return 0;
        	}

        if(Bytes_Read != 0)
        {
            total_length = ((buffer[3]<<8)| buffer[2])+4;
            HCI_GetReceiveBuffer(&Read_Buffer, total_length);
            if (!(Read_Buffer)) 
            {  
			sprintf(Error_Message,_T("%s: Ind_Thread() :- Cannot allocate %d bytes of memory"),_tcs_MODULENAME, total_length);
			pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s",Error_Message));
			return RDABT_NORESOURCES;
		}                             
            packetType = HCI_pduACLDATA;
            memcpy(Read_Buffer->buf, buffer, Bytes_Read);
            ReadedLen = Bytes_Read;
            while(ReadedLen < total_length)
            {
                	Result=DeviceIoControl(transport_data->Driver_Handle,
					IOCTL_CSRBC_GET_HCI_DATA,
					0,
					0,
					Read_Buffer->buf+ReadedLen,
					total_length-ReadedLen,
					&Bytes_Read,
					NULL);
                    ReadedLen+=Bytes_Read;
            }
            Read_Buffer->dataLen = ReadedLen;
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"%s: Read total %d bytes of %s PDU, dump: ",MODULENAME, Read_Buffer->dataLen,packetType-1<4?transportTypes[packetType-1]:"INVALID"));
            //pDebugDumpEX(pLOGDEBUG,pLOGHCI,Read_Buffer->buf,Read_Buffer->dataLen);

            BT_Transport_RX_PDU(platformConfig, Read_Buffer,(u_int8)packetType);
        }
        else
        {
            //pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"usb driver: No data read, sleep %d millisecord", POLL_SLEEPTIME));
            Sleep(POLL_SLEEPTIME);
        }
    } while (!transport_data->Thread_Exit);

    pDebugPrintfEX((pLOGNOTICE,pLOGHCI,"%s: Got thread exit event in ACL_Ind_Thread() - terminating\n",MODULENAME));
    return 0;
}

t_api USB_HCIt_PDU_Tx(struct st_TransportConfig_Win32 *platformConfig, t_DataBuf *pdu, u_int8 type, u_int16 flags, u_int16 hci_handle_flags) 
{
    u_int8 *buffer=NULL;
    DWORD Bytes_Written=0, Error_Code=0;
    ULONG BufferSize=0;
    DWORD BytesReturned = OUT_BUFFER_LEN;
    BOOL bResult=FALSE;
    u_int8 rx_head_buf[4];
    u_int8 out_buffer[OUT_BUFFER_LEN];

	switch(type)
	{
		case HCI_pduCOMMAND:
			buffer = (char*)malloc(pdu->dataLen);
			memcpy(buffer,pdu->buf,pdu->dataLen);
			bResult=DeviceIoControl(transport_data->Driver_Handle,
							IOCTL_CSRBC_SEND_HCI_COMMAND,
							buffer,
							pdu->dataLen,//sizeof(DWORD),
							out_buffer,//&BufferSize,
							OUT_BUFFER_LEN,//sizeof(DWORD),
							&BytesReturned,
							NULL);
	

			break;
			
		case  HCI_pduACLDATA:
			if (flags & RDABT_HOST_BUF_HCI_HEADER_PRESENT)
			{
				buffer = (char*)malloc(pdu->dataLen); 
				Bytes_Written = pdu->dataLen;
				memcpy(buffer,pdu->buf,pdu->dataLen);
			}
			else
			{
				rx_head_buf[0]=hci_handle_flags&0xFF;
				rx_head_buf[1]=hci_handle_flags >> 8;
				rx_head_buf[2]=pdu->dataLen & 0xFF;
				rx_head_buf[3]=pdu->dataLen >> 8;
				buffer = (char*)malloc(pdu->dataLen+4); 
				Bytes_Written = pdu->dataLen+4;
				memcpy(buffer,rx_head_buf,4);
				memcpy(buffer+4, pdu->buf, pdu->dataLen);
			}

			bResult=DeviceIoControl(transport_data->Driver_Handle,
							IOCTL_CSRBC_SEND_HCI_DATA,
							buffer,
							Bytes_Written,
							out_buffer,//&BufferSize,
							OUT_BUFFER_LEN,//sizeof(DWORD),
							&BytesReturned,
							NULL);

			break;

		case HCI_pduSCODATA:
			break;

		case HCI_pduEVENT:
			
			break;
			
	}
	
	if(!bResult) 
	{
		Error_Code=GetLastError();
        pDebugPrintfEX((pLOGERROR,pLOGHCI,"%s: DeviceIoControl() to set packet type 0x%02X returned error code %d\n",MODULENAME,type,Error_Code));
        HCI_FreeReceiveBuffer(pdu, type);
        return RDABT_HCITERROR;
	}

	if(buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	
    pDebugPrintfEX((pLOGDEBUG,pLOGHCI,"%s: Wrote total %d bytes of %s PDU, address = %x dump: ",MODULENAME, pdu->dataLen,type-1<6?transportTypes[type-1]:"INVALID",pdu->buf));
    pDebugDumpEX(pLOGDEBUG,pLOGHCI,pdu->buf,pdu->dataLen);

    HCI_FreeReceiveBuffer(pdu, type);
	return RDABT_NOERROR;
}


BOOL Select_Interface(u_int8 Alter_interface)
{
#if 0
	u_int8 buf[1];
	ULONG BufferSize=0;
	DWORD BytesReturned = 0;
	u_int8 out_buffer[OUT_BUFFER_LEN];
	BOOL Result_sco1=FALSE;

	buf[0] = Alter_interface;  //alternate interface

	Result_sco1=DeviceIoControl(Driver_Handle, 
						IOCTL_CSRBC_START_SCO_DATA, 
						buf, 
						1, 
						out_buffer, 
						OUT_BUFFER_LEN, 
						&BytesReturned, 
						NULL);
#endif
	return FALSE;
}

