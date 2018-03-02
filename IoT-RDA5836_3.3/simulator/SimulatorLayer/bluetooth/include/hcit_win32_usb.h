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
 * MODULE NAME:    hcit_win32_usb.h
 * DESCRIPTION:    HCI Transport Driver Header for Win32 - USB HCI bus
 * AUTHOR:         Martin Mellody
 * DATE:           28 November 2001
 *
 * SOURCE CONTROL: $Id: hcit_win32_usb.h,v 1.3 2008/06/19 09:49:51 jianguo Exp $
 *
 *
 */


#ifndef HCI_TRANSPORT_USB_IMPL_H
#    define HCI_TRANSPORT_USB_IMPL_H




t_api USB_HCIt_Init(struct st_TransportConfig_Win32 *platformConfig);
t_api USB_HCIt_Shutdown(struct st_TransportConfig_Win32 *platformConfig);
t_api USB_HCIt_PDU_Tx(struct st_TransportConfig_Win32 *platformConfig, t_DataBuf *pdu, u_int8 type, u_int16 flags, u_int16 hci_handle_flags);

#endif

