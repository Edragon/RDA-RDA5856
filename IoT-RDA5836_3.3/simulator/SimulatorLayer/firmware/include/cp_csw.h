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


// THIS FILE HAS BEEN CREATED WITH COOL PROFILE --- PLEASE DO NOT EDIT
#ifndef COOL_PROFILE_CP_CSW_H
#define COOL_PROFILE_CP_CSW_H

/*
csw base value start from 4000
*/
#define CP_CSW       			0x0FA1

/*
base value for csw module
*/
#define CP_CSW_FS				( CP_CSW + 1)
#define CP_CSW_DB				( CP_CSW + 400 )
#define CP_CSW_REG      		( CP_CSW + 500 )
#define CP_CSW_PM        		( CP_CSW + 600 )
#define CP_CSW_TM       		( CP_CSW + 750 )
#define CP_CSW_TCPIP        	( CP_CSW + 900 )
#define CP_CSW_DM        		( CP_CSW + 1400)
#define CP_CSW_CFW				( CP_CSW + 1700)
#define CP_CSW_AOM				( CP_CSW + 2200)
#define CP_CSW_GPRS				( CP_CSW + 2250)
//for cos, bal, shell and others
#define CP_CSW_OTH				( CP_CSW + 2350)

// FS
#define CP_FS_ChangeDir             	( CP_CSW_FS + 0 )
#define CP_FS_OpenByHandle       	    ( CP_CSW_FS + 1 )
#define CP_FS_Seek               		( CP_CSW_FS + 2 )
#define CP_FS_Read               		( CP_CSW_FS + 3 )
#define CP_FS_Write              		( CP_CSW_FS + 4 )
#define CP_FS_Close              		( CP_CSW_FS + 5 )
#define CP_FS_MakeDir            		( CP_CSW_FS + 6 )
#define CP_FS_ReadDir            		( CP_CSW_FS + 7 )
#define CP_FS_MountRoot          		( CP_CSW_FS + 8 )
#define CP_FS_RemoveDir          		( CP_CSW_FS + 9 )
#define CP_FS_Link               		( CP_CSW_FS + 10 )
#define CP_FS_UnLinkByHandle     		( CP_CSW_FS + 11 )
#define CP_FS_Format             		( CP_CSW_FS + 12 )
#define CP_FS_Stat               		( CP_CSW_FS + 13 )
#define CP_FS_FindFirstFile      		( CP_CSW_FS + 14 )
#define CP_FS_FindNextFile       		( CP_CSW_FS + 15 )
#define CP_FS_FindClose          		( CP_CSW_FS + 16 )
#define CP_FS_GetFileSize        		( CP_CSW_FS + 17 )
#define CP_FS_IsEndOfFile        		( CP_CSW_FS + 18 )
#define CP_FS_FindEx         			( CP_CSW_FS + 19 )

// DB
#define CP_DB_CreateDatabase       	( CP_CSW_DB + 0 )
#define CP_DB_DeleteDatabase			( CP_CSW_DB + 1 )    
#define CP_DB_OpenDatabase    			( CP_CSW_DB + 2 )
#define CP_DB_CloseDatabase			( CP_CSW_DB + 3 )
#define CP_DB_WriteRecord				( CP_CSW_DB + 4 )
#define CP_DB_ReadRecord				( CP_CSW_DB + 5 )
#define CP_GetDBRecordInfo				( CP_CSW_DB + 6 )
#define CP_GetDBHeaderInfo				( CP_CSW_DB + 7 )
#define CP_DB_SeekRecord				( CP_CSW_DB + 8 )
#define CP_DB_GetMatchedRecordCount	( CP_CSW_DB + 9 )
#define CP_DB_SeekRecordEx				( CP_CSW_DB + 10 )
#define CP_DB_RemoveRecord				( CP_CSW_DB + 11 )    
#define CP_DB_GetDatabaseInfo			( CP_CSW_DB + 12 )    
#define CP_DB_GetRecordInfo			( CP_CSW_DB + 13 )    
#define CP_DB_ReadFields				( CP_CSW_DB + 14 )    
#define CP_DB_WriteFields				( CP_CSW_DB + 15 )    


//REG
#define CP_REG_CloseKey             ( CP_CSW_REG + 0 )
#define CP_REG_CreateKey 			( CP_CSW_REG + 1 )
#define CP_REG_OpenKey 				( CP_CSW_REG + 2 )
#define CP_REG_SetValue 			( CP_CSW_REG + 3 )
#define CP_REG_QueryValue 			( CP_CSW_REG + 4 )
#define CP_REG_DeleteKey 			( CP_CSW_REG + 5 )
#define CP_REG_DeleteValue 			( CP_CSW_REG + 6 )
#define CP_REG_ExportFile 			( CP_CSW_REG + 7 )
#define CP_REG_ImportFile 			( CP_CSW_REG + 8 )


//
//AOM
//
#define CP_Dispatch                           ( CP_CSW_AOM + 0 )   
#define CP_CFW_RegisterAo                ( CP_CSW_AOM + 1 )   
#define CP_DeleteReadyAO                 ( CP_CSW_AOM + 2 )   
#define CP_CFW_UnRegisterAO            ( CP_CSW_AOM + 3 )   
#define CP_CFW_SetAoProcCode         ( CP_CSW_AOM + 4 )   
#define CP_TriggerStateMachine          ( CP_CSW_AOM + 5 )   
#define CP_CFW_AoScheduler             ( CP_CSW_AOM + 6 )   
#define CP_CFW_PostNotifyEvent        (CP_CSW_AOM + 7 )

//
//Gprs
//
#define CP_Gprs_MSG_Redirect                                (CP_CSW_GPRS + 0)
#define CP_CFW_GprsAoProc                                    (CP_CSW_GPRS + 1)
#define CP_GprsMTAoProc                                        (CP_CSW_GPRS + 2)
#define CP_CFW_GprsGetPdpAddr                             (CP_CSW_GPRS + 3)
#define CP_CFW_GetGprsActState                             (CP_CSW_GPRS + 4)
#define CP_CFW_GprsAct                                          (CP_CSW_GPRS + 5)
#define CP_CFW_GprsCtxModify                                (CP_CSW_GPRS + 6)
#define CP_CFW_GprsSendData                                (CP_CSW_GPRS + 7)
#define CP_CFW_GprsSetPdpCxt                               (CP_CSW_GPRS + 8)
#define CP_CFW_GprsGetPdpCxt                               (CP_CSW_GPRS + 9)
#define CP_CFW_GprsSetReqQos                               (CP_CSW_GPRS + 10)
#define CP_CFW_GprsGetReqQos                               (CP_CSW_GPRS + 11)
#define CP_CFW_GprsSetMinQos                               (CP_CSW_GPRS + 12)
#define CP_CFW_GprsGetMInQos                               (CP_CSW_GPRS + 13)
#define CP_CFW_GprsQos2Api                                  (CP_CSW_GPRS + 14)
#define CP_CFW_GprsApi2Qos                                  (CP_CSW_GPRS + 15)
#define CP_CFW_GprsGetFreeCid                              (CP_CSW_GPRS + 16)
#define CP_CFW_GprsManualAcc                              (CP_CSW_GPRS + 17)
#define CP_CFW_GprsManualRej                               (CP_CSW_GPRS + 18)



//
//Network
//
#define CP_CSW_NW                                               (CP_CSW_GPRS + 50)
#define CP_NetWork_timer_proc                              ( CP_CSW_NW + 0 )   
#define CP_CFW_NwAoProc                                     ( CP_CSW_NW + 1 )   
#define CP_CFW_NwGetAvailableOperators               ( CP_CSW_NW + 2 )   
#define CP_CFW_NwGetCurrentOperator                   ( CP_CSW_NW + 3 )   
#define CP_CFW_NwGetImei                                    ( CP_CSW_NW + 4 )   
#define CP_CFW_NwGetSignalQuality                        ( CP_CSW_NW + 5 )   
#define CP_CFW_NwSetRegistration                          ( CP_CSW_NW + 6 )   
#define CP_CFW_NwStartStack                                 ( CP_CSW_NW + 7 )   
#define CP_CFW_NwStopStack                                 ( CP_CSW_NW + 8 )   
#define CP_CFW_NwQualReportConf                         ( CP_CSW_NW + 9 )   
#define CP_CFW_NwListConf                                    ( CP_CSW_NW + 10 )   
#define CP_CFW_NwPsStatusConf                             ( CP_CSW_NW + 11 )   
#define CP_CFW_NwCsStatusConf                            ( CP_CSW_NW + 12 )   
#define CP_CFW_GprsAtt                                         ( CP_CSW_NW + 13 )   
#define CP_CFW_GetGprsAttState                             ( CP_CSW_NW + 14 )   
#define CP_CFW_GprsGetstatus                                ( CP_CSW_NW + 15 )   
#define CP_CFW_NwAbortListOperators                     ( CP_CSW_NW + 16 )   
#define CP_CFW_GprsDeactiveAll                             ( CP_CSW_NW + 17 )   


 //
// tcpip
//
#define CP_netbuf_alloc                                                ( CP_CSW_TCPIP + 0  )
#define CP_netbuf_chain                                                ( CP_CSW_TCPIP + 1  )
#define CP_netbuf_copy                                                 ( CP_CSW_TCPIP + 2  )
#define CP_netbuf_copy_partial                                         ( CP_CSW_TCPIP + 3  )
#define CP_netbuf_data                                                 ( CP_CSW_TCPIP + 4  )
#define CP_netbuf_delete                                               ( CP_CSW_TCPIP + 5  )
#define CP_netbuf_first                                                ( CP_CSW_TCPIP + 6  )
#define CP_netbuf_free                                                 ( CP_CSW_TCPIP + 7  )
#define CP_netbuf_fromaddr                                             ( CP_CSW_TCPIP + 8  )
#define CP_netbuf_fromport                                             ( CP_CSW_TCPIP + 9  )
#define CP_netbuf_len                                                  ( CP_CSW_TCPIP + 10 )
#define CP_netbuf_new                                                  ( CP_CSW_TCPIP + 11 )
#define CP_netbuf_next                                                 ( CP_CSW_TCPIP + 12 )
#define CP_netbuf_ref                                                  ( CP_CSW_TCPIP + 13 )
#define CP_netconn_accept                                              ( CP_CSW_TCPIP + 14 )
#define CP_netconn_addr                                                ( CP_CSW_TCPIP + 15 )
#define CP_netconn_bind                                                ( CP_CSW_TCPIP + 16 )
#define CP_netconn_close                                               ( CP_CSW_TCPIP + 17 )
#define CP_netconn_connect                                             ( CP_CSW_TCPIP + 18 )
#define CP_netconn_delete                                              ( CP_CSW_TCPIP + 19 )
#define CP_netconn_disconnect                                          ( CP_CSW_TCPIP + 20 )
#define CP_netconn_err                                                 ( CP_CSW_TCPIP + 21 )
#define CP_netconn_listen                                              ( CP_CSW_TCPIP + 22 )
#define CP_netconn_new                                                 ( CP_CSW_TCPIP + 23 )
#define CP_netconn_new_with_callback                                   ( CP_CSW_TCPIP + 24 )
#define CP_netconn_new_with_proto_and_callback                         ( CP_CSW_TCPIP + 25 )
#define CP_netconn_peer                                                ( CP_CSW_TCPIP + 26 )
#define CP_netconn_recv                                                ( CP_CSW_TCPIP + 27 )
#define CP_netconn_send                                                ( CP_CSW_TCPIP + 28 )
#define CP_netconn_type                                                ( CP_CSW_TCPIP + 29 )
#define CP_netconn_write                                               ( CP_CSW_TCPIP + 30 )
#define CP_accept_function                                             ( CP_CSW_TCPIP + 31 )
#define CP_api_msg_input                                               ( CP_CSW_TCPIP + 32 )
#define CP_api_msg_post                                                ( CP_CSW_TCPIP + 33 )
#define CP_do_accept                                                   ( CP_CSW_TCPIP + 34 )
#define CP_do_bind                                                     ( CP_CSW_TCPIP + 35 )
#define CP_do_close                                                    ( CP_CSW_TCPIP + 36 )
#define CP_do_connect                                                  ( CP_CSW_TCPIP + 37 )
#define CP_do_connected                                                ( CP_CSW_TCPIP + 38 )
#define CP_do_delconn                                                  ( CP_CSW_TCPIP + 39 )
#define CP_do_disconnect                                               ( CP_CSW_TCPIP + 40 )
#define CP_do_listen                                                   ( CP_CSW_TCPIP + 41 )
#define CP_do_newconn                                                  ( CP_CSW_TCPIP + 42 )
#define CP_do_recv                                                     ( CP_CSW_TCPIP + 43 )
#define CP_do_send                                                     ( CP_CSW_TCPIP + 44 )
#define CP_do_write                                                    ( CP_CSW_TCPIP + 45 )
#define CP_err_tcp                                                     ( CP_CSW_TCPIP + 46 )
#define CP_poll_tcp                                                    ( CP_CSW_TCPIP + 47 )
#define CP_recv_raw                                                    ( CP_CSW_TCPIP + 48 )
#define CP_recv_tcp                                                    ( CP_CSW_TCPIP + 49 )
#define CP_recv_udp                                                    ( CP_CSW_TCPIP + 50 )
#define CP_sent_tcp                                                    ( CP_CSW_TCPIP + 51 )
#define CP_setup_tcp                                                   ( CP_CSW_TCPIP + 52 )
#define CP_lwip_strerr                                                 ( CP_CSW_TCPIP + 53 )
#define CP_icmp_dest_unreach                                           ( CP_CSW_TCPIP + 54 )
#define CP_icmp_input                                                  ( CP_CSW_TCPIP + 55 )
#define CP_icmp_time_exceeded                                          ( CP_CSW_TCPIP + 56 )
#define CP_CFW_TcpipInetAddr                                           ( CP_CSW_TCPIP + 57 )
#define CP_htonl                                                       ( CP_CSW_TCPIP + 58 )
#define CP_htons                                                       ( CP_CSW_TCPIP + 59 )
#define CP_inet_aton                                                   ( CP_CSW_TCPIP + 60 )
#define CP_inet_chksum                                                 ( CP_CSW_TCPIP + 61 )
#define CP_inet_chksum_pbuf                                            ( CP_CSW_TCPIP + 62 )
#define CP_inet_chksum_pseudo                                          ( CP_CSW_TCPIP + 63 )
#define CP_inet_ntoa                                                   ( CP_CSW_TCPIP + 64 )
#define CP_lwip_standard_chksum                                        ( CP_CSW_TCPIP + 65 )
#define CP_ntohl                                                       ( CP_CSW_TCPIP + 66 )
#define CP_ntohs                                                       ( CP_CSW_TCPIP + 67 )
#define CP_ip_forward                                                  ( CP_CSW_TCPIP + 68 )
#define CP_ip_debug_print                                              ( CP_CSW_TCPIP + 69 )
#define CP_ip_init                                                     ( CP_CSW_TCPIP + 70 )
#define CP_ip_input                                                    ( CP_CSW_TCPIP + 71 )
#define CP_ip_output                                                   ( CP_CSW_TCPIP + 72 )
#define CP_ip_output_if                                                ( CP_CSW_TCPIP + 73 )
#define CP_ip_route                                                    ( CP_CSW_TCPIP + 74 )
#define CP_ip_addr_isbroadcast                                         ( CP_CSW_TCPIP + 75 )
#define CP_copy_from_pbuf                                              ( CP_CSW_TCPIP + 76 )
#define CP_ip_frag                                                     ( CP_CSW_TCPIP + 77 )
#define CP_ip_reass                                                    ( CP_CSW_TCPIP + 78 )
#define CP_ip_reass_mem_free                                           ( CP_CSW_TCPIP + 79 )
#define CP_ip_reass_mem_init                                           ( CP_CSW_TCPIP + 80 )
#define CP_ip_reass_tmr                                                ( CP_CSW_TCPIP + 81 )
#define CP_free_sxr_mem                                                ( CP_CSW_TCPIP + 82 )
#define CP_mem_free                                                    ( CP_CSW_TCPIP + 83 )
#define CP_mem_init                                                    ( CP_CSW_TCPIP + 84 )
#define CP_mem_malloc                                                  ( CP_CSW_TCPIP + 85 )
#define CP_mem_realloc                                                 ( CP_CSW_TCPIP + 86 )
#define CP_mem_reallocm                                                ( CP_CSW_TCPIP + 87 )
#define CP_plug_holes                                                  ( CP_CSW_TCPIP + 88 )
#define CP_free_sxr_memp                                               ( CP_CSW_TCPIP + 89 )
#define CP_memp_free                                                   ( CP_CSW_TCPIP + 90 )
#define CP_memp_init                                                   ( CP_CSW_TCPIP + 91 )
#define CP_memp_malloc                                                 ( CP_CSW_TCPIP + 92 )
#define CP_memp_sanity                                                 ( CP_CSW_TCPIP + 93 )
#define CP_msg_memp_malloc                                             ( CP_CSW_TCPIP + 94 )
#define CP_netif_add                                                   ( CP_CSW_TCPIP + 95 )
#define CP_netif_find                                                  ( CP_CSW_TCPIP + 96 )
#define CP_netif_init                                                  ( CP_CSW_TCPIP + 97 )
#define CP_netif_is_up                                                 ( CP_CSW_TCPIP + 98 )
#define CP_netif_remove                                                ( CP_CSW_TCPIP + 99 )
#define CP_netif_set_addr                                              ( CP_CSW_TCPIP + 100)
#define CP_netif_set_default                                           ( CP_CSW_TCPIP + 101)
#define CP_netif_set_down                                              ( CP_CSW_TCPIP + 102)
#define CP_netif_set_gw                                                ( CP_CSW_TCPIP + 103)
#define CP_netif_set_ipaddr                                            ( CP_CSW_TCPIP + 104)
#define CP_netif_set_netmask                                           ( CP_CSW_TCPIP + 105)
#define CP_netif_set_up                                                ( CP_CSW_TCPIP + 106)
#define CP_free_sxr_pbuf                                               ( CP_CSW_TCPIP + 107)
#define CP_pbuf_alloc                                                  ( CP_CSW_TCPIP + 108)
#define CP_pbuf_cat                                                    ( CP_CSW_TCPIP + 109)
#define CP_pbuf_chain                                                  ( CP_CSW_TCPIP + 110)
#define CP_pbuf_clen                                                   ( CP_CSW_TCPIP + 111)
#define CP_pbuf_dechain                                                ( CP_CSW_TCPIP + 112)
#define CP_pbuf_dequeue                                                ( CP_CSW_TCPIP + 113)
#define CP_pbuf_free                                                   ( CP_CSW_TCPIP + 114)
#define CP_pbuf_header                                                 ( CP_CSW_TCPIP + 115)
#define CP_pbuf_init                                                   ( CP_CSW_TCPIP + 116)
#define CP_pbuf_pool_alloc                                             ( CP_CSW_TCPIP + 117)
#define CP_pbuf_queue                                                  ( CP_CSW_TCPIP + 118)
#define CP_pbuf_realloc                                                ( CP_CSW_TCPIP + 119)
#define CP_pbuf_ref                                                    ( CP_CSW_TCPIP + 120)
#define CP_pbuf_take                                                   ( CP_CSW_TCPIP + 121)
#define CP_raw_bind                                                    ( CP_CSW_TCPIP + 122)
#define CP_raw_connect                                                 ( CP_CSW_TCPIP + 123)
#define CP_raw_init                                                    ( CP_CSW_TCPIP + 124)
#define CP_raw_input                                                   ( CP_CSW_TCPIP + 125)
#define CP_raw_new                                                     ( CP_CSW_TCPIP + 126)
#define CP_raw_recv                                                    ( CP_CSW_TCPIP + 127)
#define CP_raw_remove                                                  ( CP_CSW_TCPIP + 128)
#define CP_raw_send                                                    ( CP_CSW_TCPIP + 129)
#define CP_raw_sendto                                                  ( CP_CSW_TCPIP + 130)
#define CP_get_socket                                                  ( CP_CSW_TCPIP + 131)
#define CP_alloc_socket                                                ( CP_CSW_TCPIP + 132)
#define CP_CFW_TcpipAvailableBuffer                                    ( CP_CSW_TCPIP + 133)
#define CP_CFW_TcpipGetLastError                                       ( CP_CSW_TCPIP + 134)
#define CP_CFW_TcpipSocket                                             ( CP_CSW_TCPIP + 135)
#define CP_CFW_TcpipSocketAccept                                       ( CP_CSW_TCPIP + 136)
#define CP_CFW_TcpipSocketBind                                         ( CP_CSW_TCPIP + 137)
#define CP_CFW_TcpipSocketClose                                        ( CP_CSW_TCPIP + 138)
#define CP_CFW_TcpipSocketConnect                                      ( CP_CSW_TCPIP + 139)
#define CP_CFW_TcpipSocketGetpeername                                  ( CP_CSW_TCPIP + 140)
#define CP_CFW_TcpipSocketGetsockname                                  ( CP_CSW_TCPIP + 141)
#define CP_CFW_TcpipSocketGetsockopt                                   ( CP_CSW_TCPIP + 142)
#define CP_CFW_TcpipSocketIoctl                                        ( CP_CSW_TCPIP + 143)
#define CP_CFW_TcpipSocketListen                                       ( CP_CSW_TCPIP + 144)
#define CP_CFW_TcpipSocketRead                                         ( CP_CSW_TCPIP + 145)
#define CP_CFW_TcpipSocketRecv                                         ( CP_CSW_TCPIP + 146)
#define CP_CFW_TcpipSocketRecvfrom                                     ( CP_CSW_TCPIP + 147)
#define CP_CFW_TcpipSocketSelect                                       ( CP_CSW_TCPIP + 148)
#define CP_CFW_TcpipSocketSelscan                                      ( CP_CSW_TCPIP + 149)
#define CP_CFW_TcpipSocketSend                                         ( CP_CSW_TCPIP + 150)
#define CP_CFW_TcpipSocketSendto                                       ( CP_CSW_TCPIP + 151)
#define CP_CFW_TcpipSocketSetsockopt                                   ( CP_CSW_TCPIP + 152)
#define CP_CFW_TcpipSocketShutdown                                     ( CP_CSW_TCPIP + 153)
#define CP_CFW_TcpipSocketWrite                                        ( CP_CSW_TCPIP + 154)
#define CP_event_callback                                              ( CP_CSW_TCPIP + 155)
#define CP_free_sock_sem                                               ( CP_CSW_TCPIP + 156)
#define CP_stats_display                                               ( CP_CSW_TCPIP + 157)
#define CP_stats_display_mem                                           ( CP_CSW_TCPIP + 158)
#define CP_stats_display_pbuf                                          ( CP_CSW_TCPIP + 159)
#define CP_stats_display_proto                                         ( CP_CSW_TCPIP + 160)
#define CP_stats_init                                                  ( CP_CSW_TCPIP + 161)
#define CP_sys_arch_mbox_fetch                                         ( CP_CSW_TCPIP + 162)
#define CP_sys_arch_sem_wait                                           ( CP_CSW_TCPIP + 163)
#define CP_sys_arch_timeouts                                           ( CP_CSW_TCPIP + 164)
#define CP_sys_mbox_fetch                                              ( CP_CSW_TCPIP + 165)
#define CP_sys_mbox_free                                               ( CP_CSW_TCPIP + 166)
#define CP_sys_mbox_new                                                ( CP_CSW_TCPIP + 167)
#define CP_sys_mbox_post                                               ( CP_CSW_TCPIP + 168)
#define CP_sys_msleep                                                  ( CP_CSW_TCPIP + 169)
#define CP_sys_post_event_to_APP                                       ( CP_CSW_TCPIP + 170)
#define CP_sys_post_event_to_APS                                       ( CP_CSW_TCPIP + 171)
#define CP_sys_sem_free                                                ( CP_CSW_TCPIP + 172)
#define CP_sys_sem_new                                                 ( CP_CSW_TCPIP + 173)
#define CP_sys_sem_signal                                              ( CP_CSW_TCPIP + 174)
#define CP_sys_sem_wait                                                ( CP_CSW_TCPIP + 175)
#define CP_sys_sem_wait_timeout                                        ( CP_CSW_TCPIP + 176)
#define CP_sys_timeout                                                 ( CP_CSW_TCPIP + 177)
#define CP_sys_untimeout                                               ( CP_CSW_TCPIP + 178)
#define CP_tcp_abort                                                   ( CP_CSW_TCPIP + 179)
#define CP_tcp_accept                                                  ( CP_CSW_TCPIP + 180)
#define CP_tcp_accept_null                                             ( CP_CSW_TCPIP + 181)
#define CP_tcp_alloc                                                   ( CP_CSW_TCPIP + 182)
#define CP_tcp_arg                                                     ( CP_CSW_TCPIP + 183)
#define CP_tcp_bind                                                    ( CP_CSW_TCPIP + 184)
#define CP_tcp_close                                                   ( CP_CSW_TCPIP + 185)
#define CP_tcp_connect                                                 ( CP_CSW_TCPIP + 186)
#define CP_tcp_debug_print                                             ( CP_CSW_TCPIP + 187)
#define CP_tcp_debug_print_flags                                       ( CP_CSW_TCPIP + 188)
#define CP_tcp_debug_print_pcbs                                        ( CP_CSW_TCPIP + 189)
#define CP_tcp_debug_print_state                                       ( CP_CSW_TCPIP + 190)
#define CP_tcp_err                                                     ( CP_CSW_TCPIP + 191)
#define CP_tcp_fasttmr                                                 ( CP_CSW_TCPIP + 192)
#define CP_tcp_init                                                    ( CP_CSW_TCPIP + 193)
#define CP_tcp_kill_prio                                               ( CP_CSW_TCPIP + 194)
#define CP_tcp_kill_timewait                                           ( CP_CSW_TCPIP + 195)
#define CP_tcp_listen                                                  ( CP_CSW_TCPIP + 196)
#define CP_tcp_new                                                     ( CP_CSW_TCPIP + 197)
#define CP_tcp_new_port                                                ( CP_CSW_TCPIP + 198)
#define CP_tcp_next_iss                                                ( CP_CSW_TCPIP + 199)
#define CP_tcp_pcb_purge                                               ( CP_CSW_TCPIP + 200)
#define CP_tcp_pcb_remove                                              ( CP_CSW_TCPIP + 201)
#define CP_tcp_pcbs_sane                                               ( CP_CSW_TCPIP + 202)
#define CP_tcp_poll                                                    ( CP_CSW_TCPIP + 203)
#define CP_tcp_recv                                                    ( CP_CSW_TCPIP + 204)
#define CP_tcp_recv_null                                               ( CP_CSW_TCPIP + 205)
#define CP_tcp_recved                                                  ( CP_CSW_TCPIP + 206)
#define CP_tcp_seg_copy                                                ( CP_CSW_TCPIP + 207)
#define CP_tcp_seg_free                                                ( CP_CSW_TCPIP + 208)
#define CP_tcp_segs_free                                               ( CP_CSW_TCPIP + 209)
#define CP_tcp_sent                                                    ( CP_CSW_TCPIP + 210)
#define CP_tcp_setprio                                                 ( CP_CSW_TCPIP + 211)
#define CP_tcp_slowtmr                                                 ( CP_CSW_TCPIP + 212)
#define CP_tcp_tmr                                                     ( CP_CSW_TCPIP + 213)
#define CP_tcp_input                                                   ( CP_CSW_TCPIP + 214)
#define CP_tcp_listen_input                                            ( CP_CSW_TCPIP + 215)
#define CP_tcp_parseopt                                                ( CP_CSW_TCPIP + 216)
#define CP_tcp_process                                                 ( CP_CSW_TCPIP + 217)
#define CP_tcp_receive                                                 ( CP_CSW_TCPIP + 218)
#define CP_tcp_timewait_input                                          ( CP_CSW_TCPIP + 219)
#define CP_tcp_enqueue                                                 ( CP_CSW_TCPIP + 220)
#define CP_tcp_keepalive                                               ( CP_CSW_TCPIP + 221)
#define CP_tcp_output                                                  ( CP_CSW_TCPIP + 222)
#define CP_tcp_output_segment                                          ( CP_CSW_TCPIP + 223)
#define CP_tcp_rexmit                                                  ( CP_CSW_TCPIP + 224)
#define CP_tcp_rexmit_rto                                              ( CP_CSW_TCPIP + 225)
#define CP_tcp_rst                                                     ( CP_CSW_TCPIP + 226)
#define CP_tcp_send_ctrl                                               ( CP_CSW_TCPIP + 227)
#define CP_tcp_write                                                   ( CP_CSW_TCPIP + 228)
#define CP_data_output                                                 ( CP_CSW_TCPIP + 229)
#define CP_ip_timer                                                    ( CP_CSW_TCPIP + 230)
#define CP_tcp_timer_needed                                            ( CP_CSW_TCPIP + 231)
#define CP_tcpip_apimsg                                                ( CP_CSW_TCPIP + 232)
#define CP_tcpip_callback                                              ( CP_CSW_TCPIP + 233)
#define CP_Tcpip_Destroy                                               ( CP_CSW_TCPIP + 234)
#define CP_tcpip_init                                                  ( CP_CSW_TCPIP + 235)
#define CP_tcpip_input                                                 ( CP_CSW_TCPIP + 236)
#define CP_tcpip_tcp_timer                                             ( CP_CSW_TCPIP + 237)
#define CP_TCpIPInit                                                   ( CP_CSW_TCPIP + 238)
#define CP_socketdata_get                                              ( CP_CSW_TCPIP + 239)
#define CP_socketdata_initial                                          ( CP_CSW_TCPIP + 240)
#define CP_socketdata_store                                            ( CP_CSW_TCPIP + 241)
#define CP_udp_bind                                                    ( CP_CSW_TCPIP + 242)
#define CP_udp_connect                                                 ( CP_CSW_TCPIP + 243)
#define CP_udp_debug_print                                             ( CP_CSW_TCPIP + 244)
#define CP_udp_disconnect                                              ( CP_CSW_TCPIP + 245)
#define CP_udp_init                                                    ( CP_CSW_TCPIP + 246)
#define CP_udp_input                                                   ( CP_CSW_TCPIP + 247)
#define CP_udp_new                                                     ( CP_CSW_TCPIP + 248)
#define CP_udp_recv                                                    ( CP_CSW_TCPIP + 249)
#define CP_udp_remove                                                  ( CP_CSW_TCPIP + 250)
#define CP_udp_send                                                    ( CP_CSW_TCPIP + 251)
#define CP_udp_sendto                                                  ( CP_CSW_TCPIP + 252)

//PM
#define CP_PM_GetACPowerStatus			( CP_CSW_PM + 0 )
#define CP_PM_GetBatteryState           ( CP_CSW_PM + 1 )
#define CP_PM_GetBatteryInfo            ( CP_CSW_PM + 2 )
#define CP_PM_GetGpadcInfo              ( CP_CSW_PM + 3 )
#define CP_PM_BatteryChemistry          ( CP_CSW_PM + 4 )
#define CP_PM_SetBatteryChargeMode      ( CP_CSW_PM + 5 )
#define CP_PM_SetPhoneFunctionality     ( CP_CSW_PM + 6 )
#define CP_PM_GetPhoneFunctionality     ( CP_CSW_PM + 7 )
#define CP_PM_SetChkGpadcPeriod         ( CP_CSW_PM + 8 )
#define CP_pm_IdleScanProc              ( CP_CSW_PM + 9 )
#define CP_PM_PowerOnChck               ( CP_CSW_PM + 10 )
#define CP_PM_GetPowerOnVol             ( CP_CSW_PM + 11 )
#define CP_Chg_PulseStart               ( CP_CSW_PM + 12 )
#define CP_Chg_PulsePeriodic            ( CP_CSW_PM + 13 )
#define CP_pm_StartFastCharge           ( CP_CSW_PM + 14 )
#define CP_pm_SetPulseChargeParam       ( CP_CSW_PM + 15 )
#define CP_pm_StartPulsCharge           ( CP_CSW_PM + 16 )
#define CP_pm_StopCharge                ( CP_CSW_PM + 17 )
#define CP_pm_PulseChargeOnOff          ( CP_CSW_PM + 18 )
#define CP_pm_BatteryInit               ( CP_CSW_PM + 19 )
#define CP_pm_BatteryMonitorProc        ( CP_CSW_PM + 20 )
#define CP_bat_ProgNextEvent            ( CP_CSW_PM + 21 )
#define CP_gpadc_measures               ( CP_CSW_PM + 22 )
#define CP_dm_BatteryCtrlProc           ( CP_CSW_PM + 23 )
#define CP_BattGetBcs                   ( CP_CSW_PM + 24 )
#define CP_pm_BattGetBcl                ( CP_CSW_PM + 25 )
#define CP_BattUnderVoltageRemind       ( CP_CSW_PM + 26 )
#define CP_BattChangeVoltageRemind      ( CP_CSW_PM + 27 )
#define CP_BattChargeFullRemind         ( CP_CSW_PM + 28 )
#define CP_BattChargeStarRemind         ( CP_CSW_PM + 29 )
#define CP_BattChargePlugOffRemind      ( CP_CSW_PM + 30 )
#define CP_dm_gpadc_send                ( CP_CSW_PM + 31 )
#define CP_pm_SetCheckADCTime           ( CP_CSW_PM + 32 )


//TM
#define CP_TM_IsLeapYear				( CP_CSW_TM + 0 )
#define CP_TM_GetDayNoInYear            ( CP_CSW_TM + 1 )
#define CP_TM_GetDayOfWeek              ( CP_CSW_TM + 2 )
#define CP_TM_SystemTimeToFileTime      ( CP_CSW_TM + 3 )
#define CP_TM_FileTimeToSystemTime      ( CP_CSW_TM + 4 )
#define CP_TM_FormatSystemTime          ( CP_CSW_TM + 5 )
#define CP_TM_SetLocalTime              ( CP_CSW_TM + 6 )
#define CP_TM_GetLocalTime              ( CP_CSW_TM + 7 )
#define CP_TM_FormatFileTime            ( CP_CSW_TM + 8 )
#define CP_TM_SetTimeFormat             ( CP_CSW_TM + 9 )
#define CP_TM_GetTimeFormat             ( CP_CSW_TM + 10 )
#define CP_TM_GetTime                   ( CP_CSW_TM + 11 )
#define CP_TM_SetSystemTime             ( CP_CSW_TM + 12 )
#define CP_TM_GetSystemTime             ( CP_CSW_TM + 13 )
#define CP_TM_GetTimeZone               ( CP_CSW_TM + 14 )
#define CP_TM_SetTimeZone               ( CP_CSW_TM + 15 )
#define CP_TM_GetTimeBase               ( CP_CSW_TM + 16 )
#define CP_TM_SetTimeBase               ( CP_CSW_TM + 17 )
#define CP_TM_GetSystemFileTime         ( CP_CSW_TM + 18 )
#define CP_TM_FormatDateTime            ( CP_CSW_TM + 19 )
#define CP_TM_FormatDateTimeEx          ( CP_CSW_TM + 20 )
#define CP_TM_FormatFileTimeToStringEx  ( CP_CSW_TM + 21 )
#define CP_TM_GetDayOfWeekEX            ( CP_CSW_TM + 22 )
#define CP_TM_SmsTimeStampToFileTime    ( CP_CSW_TM + 23 )
#define CP_TM_ListAlarm                 ( CP_CSW_TM + 24 )
#define CP_TM_SetAlarm                  ( CP_CSW_TM + 25 )
#define CP_TM_KillAlarm                 ( CP_CSW_TM + 26 )
#define CP_TM_GetAlarm                  ( CP_CSW_TM + 27 )
#define CP_TIM_OpenKey                  ( CP_CSW_TM + 28 )
#define CP_TIM_CloseKey                 ( CP_CSW_TM + 29 )
#define CP_TIM_WriteBinSet              ( CP_CSW_TM + 30 )
#define CP_TIM_ReadBinSet               ( CP_CSW_TM + 31 )
#define CP_TIM_WriteRegData             ( CP_CSW_TM + 32 )
#define CP_TIM_ReadRegData              ( CP_CSW_TM + 33 )
#define CP_TIM_WriteFlash               ( CP_CSW_TM + 34 )
#define CP_TIM_ReadFlash                ( CP_CSW_TM + 35 )
#define CP_TM_ResetRTC                  ( CP_CSW_TM + 36 )
#define CP_TIM_GetMinAlarm              ( CP_CSW_TM + 37 )
#define CP_TIM_CheckAlarmOfWeek         ( CP_CSW_TM + 38 )
#define CP_dm_AlarmDisposeInEvt         ( CP_CSW_TM + 39 )
#define CP_TIM_SetAlarmEX               ( CP_CSW_TM + 40 )
#define CP_TIM_KillAlarmEX              ( CP_CSW_TM + 41 )
#define CP_TIM_GetAlarmEX               ( CP_CSW_TM + 42 )
#define CP_TIM_PowerOnInitData          ( CP_CSW_TM + 43 )
#define CP_TM_PowerOn                   ( CP_CSW_TM + 44 )
#define CP_TM_PowerOff                  ( CP_CSW_TM + 45 )
#define CP_TIM_SetAlarmSelfDefine       ( CP_CSW_TM + 46 )
#define CP_TIM_SetAlarmAndCompare       ( CP_CSW_TM + 47 )
#define CP_TIM_ResetAlarm               ( CP_CSW_TM + 48 )
#define CP_TM_ClearAllAlarm             ( CP_CSW_TM + 49 )
#define CP_TM_IsValidFormat             ( CP_CSW_TM + 50 )
#define CP_TM_GetTimeZoneTime           ( CP_CSW_TM + 51 )
#define CP_TM_IsValidTime               ( CP_CSW_TM + 52 )
#define CP_DM_StopGpadcDetect           ( CP_CSW_TM + 53 )
#define CP_DM_StartGpadcDetect          ( CP_CSW_TM + 54 )
#define CP_TM_SetTimeBaseToFlash        ( CP_CSW_TM + 55 )


//dm
#define CP_DM_SetAudioMode_From_Calibration             ( CP_CSW_DM + 1 )
#define CP_DM_SetAudioMode                              ( CP_CSW_DM + 2 )
#define CP_DM_SetAudio_From_Calibration_Volume          ( CP_CSW_DM + 3 )
#define CP_DM_SetAudioVolume                            ( CP_CSW_DM + 4 )
#define CP_DM_MicStartRecord                            ( CP_CSW_DM + 5 )
#define CP_DM_StopAudio                                 ( CP_CSW_DM + 6 )
#define CP_DM_StartAudio                                ( CP_CSW_DM + 7 )
#define CP_DM_Audio_StartStream                         ( CP_CSW_DM + 8 )
#define CP_DM_Audio_StopStream                          ( CP_CSW_DM + 9 )
#define CP_DM_PlayTone                                  ( CP_CSW_DM + 10 )
#define CP_DM_StopTone                                  ( CP_CSW_DM + 11 )



#define CP_CSW_SIM_GENERAL_BASE		(CP_CSW_CFW + 0)
#define CP_CSW_SIM_PBK_BASE			(CP_CSW_CFW + 60)	  
#define CP_CSW_SIM_SMS_BASE			(CP_CSW_CFW + 80)	
#define CP_CSW_SIM_SAT_BASE			(CP_CSW_CFW + 130)	
#define CP_CSW_CC_BASE				(CP_CSW_CFW + 140)
#define CP_CSW_SS_BASE				(CP_CSW_CFW + 200)
#define CP_CSW_EMOD_BASE			(CP_CSW_CFW + 250)
#define CP_CSW_CFG_BASE				(CP_CSW_CFW + 270)
#define CP_CSW_SMS_BASE				(CP_CSW_CFW + 400)	
#define CP_CSW_SHELL_BASE			(CP_CSW_CFW + 550)  //shell°üº¬50¸ö



	
//
//sim_sms
//
#define CP_CFW_SimReadMessage			CP_CSW_SIM_SMS_BASE + 2
#define CP_CFW_SimWriteMessage			CP_CSW_SIM_SMS_BASE + 3
#define CP_CFW_SimListMessage			CP_CSW_SIM_SMS_BASE + 4
#define CP_CFW_SimDeleteMessage			CP_CSW_SIM_SMS_BASE + 5
#define CP_CFW_SimReadBinary			CP_CSW_SIM_SMS_BASE + 6
#define CP_CFW_SimUpdateBinary			CP_CSW_SIM_SMS_BASE + 7
#define CP_CFW_SimSetMR					CP_CSW_SIM_SMS_BASE + 8
#define CP_CFW_SimGetMR					CP_CSW_SIM_SMS_BASE + 9
#define CP_CFW_SimGetSmsStorageInfo		CP_CSW_SIM_SMS_BASE + 11
#define CP_CFW_SimSetSmsParameters		CP_CSW_SIM_SMS_BASE + 12
#define CP_CFW_SimSmsInitProc			CP_CSW_SIM_SMS_BASE + 13
#define CP_CFW_SimSmsInit				CP_CSW_SIM_SMS_BASE + 14
#define CP_CFW_DecomposePDU				CP_CSW_SIM_SMS_BASE + 15
#define CP_CFW_SimReadMessageProc		CP_CSW_SIM_SMS_BASE + 16
#define CP_CFW_SimWriteMessageProc		CP_CSW_SIM_SMS_BASE + 17
#define CP_CFW_SimListMessageProc		CP_CSW_SIM_SMS_BASE + 19
#define CP_CFW_SimReadBinaryProc		CP_CSW_SIM_SMS_BASE + 20
#define CP_CFW_SimUpdateBinaryProc		CP_CSW_SIM_SMS_BASE + 21
#define CP_CFW_SimGetMRProc				CP_CSW_SIM_SMS_BASE + 22
#define CP_CFW_SimSetMRProc				CP_CSW_SIM_SMS_BASE + 23
#define CP_CFW_SimGetSmsStorageInfoProc	CP_CSW_SIM_SMS_BASE + 24
#define CP_CFW_SimGetSmsParametersProc	CP_CSW_SIM_SMS_BASE + 25
#define CP_CFW_SimSetSmsParametersProc	CP_CSW_SIM_SMS_BASE + 26

#define CP_CFW_SimTestProc          	CP_CSW_SIM_SMS_BASE + 27
#define CP_CFW_SimTest              	CP_CSW_SIM_SMS_BASE + 28
#define CP_Sim_ParseSW1SW2          	CP_CSW_SIM_SMS_BASE + 29
#define CP_SimParseDedicatedStatus  	CP_CSW_SIM_SMS_BASE + 30
#define CP_SimReadBinaryReq         	CP_CSW_SIM_SMS_BASE + 31
#define CP_SimUpdateBinaryReq       	CP_CSW_SIM_SMS_BASE + 32
#define CP_SimElemFileStatusReq     	CP_CSW_SIM_SMS_BASE + 33
#define CP_SimDedicFileStatusReq    	CP_CSW_SIM_SMS_BASE + 34
#define CP_SimReadRecordReq         	CP_CSW_SIM_SMS_BASE + 35
#define CP_SimUpdateRecordReq       	CP_CSW_SIM_SMS_BASE + 36


#define CP_SimSeekReq                  	CP_CSW_SIM_SMS_BASE + 37
#define CP_SimVerifyCHVReq             	CP_CSW_SIM_SMS_BASE + 38
#define CP_SimChangeCHVReq             	CP_CSW_SIM_SMS_BASE + 39
#define CP_SimDisableCHVReq            	CP_CSW_SIM_SMS_BASE + 40
#define CP_SimEnableCHVReq             	CP_CSW_SIM_SMS_BASE + 41
#define CP_SimUnblockCHVReq            	CP_CSW_SIM_SMS_BASE + 42
#define CP_SimInvalidateReq            	CP_CSW_SIM_SMS_BASE + 43
#define CP_SimRehabilitateReq          	CP_CSW_SIM_SMS_BASE + 44
#define CP_cfw_IMSItoASC               	CP_CSW_SIM_SMS_BASE + 45
#define CP_cfw_PLMNtoBCD               	CP_CSW_SIM_SMS_BASE + 46
#define CP_cfw_BCDtoPLMN               	CP_CSW_SIM_SMS_BASE + 47
#define CP_CFW_SimGetAuthenStatusProc  	            CP_CSW_SIM_SMS_BASE + 48
#define CP_CFW_SimEnterAuthenticationProc           CP_CSW_SIM_SMS_BASE + 49
#define CP_CFW_SimChangePasswordProc               	CP_CSW_SIM_SMS_BASE + 50
#define CP_CFW_SimGetFacilityProc               	CP_CSW_SIM_SMS_BASE + 51


#define CP_CFW_SimGetAuthenticationStatus         	CP_CSW_SIM_SMS_BASE + 52
#define CP_CFW_SimEnterAuthentication              	CP_CSW_SIM_SMS_BASE + 53
#define CP_CFW_SimChangePassword                   	CP_CSW_SIM_SMS_BASE + 54
#define CP_CFW_SimGetFacilityLock               	CP_CSW_SIM_SMS_BASE + 55
#define CP_CFW_SimSetFacilityLock               	CP_CSW_SIM_SMS_BASE + 56

#define CP_CFW_SimGetECC                         	CP_CSW_SIM_SMS_BASE + 57
#define CP_CFW_SimSetECC                          	CP_CSW_SIM_SMS_BASE + 58
#define CP_CFW_SimGetLP                            	CP_CSW_SIM_SMS_BASE + 59
#define CP_SimSpySimCard                           	CP_CSW_SIM_SMS_BASE + 60
#define CP_CFW_SimInitStage3                       	CP_CSW_SIM_SMS_BASE + 61

#define CP_CFW_SimInitStage2                     	CP_CSW_SIM_SMS_BASE + 62
#define CP_CFW_SimInit                            	CP_CSW_SIM_SMS_BASE + 63
#define CP_CFW_SimGetPUCT                          	CP_CSW_SIM_SMS_BASE + 64
#define CP_CFW_SimSetPUCT                          	CP_CSW_SIM_SMS_BASE + 65
#define CP_CFW_SimGetProviderId                    	CP_CSW_SIM_SMS_BASE + 66

#define CP_CFW_SimSetPrefOperatorList              	CP_CSW_SIM_SMS_BASE + 67
#define CP_CFW_SimGetPrefOperatorList              	CP_CSW_SIM_SMS_BASE + 68
#define CP_CFW_SimGetPrefOperatorListMaxNum        	CP_CSW_SIM_SMS_BASE + 69
#define CP_CFW_SimGetACM                           	CP_CSW_SIM_SMS_BASE + 70
#define CP_CFW_SimSetACMMax                        	CP_CSW_SIM_SMS_BASE + 71
#define CP_CFW_SimGetACMMax                        	CP_CSW_SIM_SMS_BASE + 72
#define CP_CFW_SimReSetACM                        	CP_CSW_SIM_SMS_BASE + 73

#define CP_CFW_SimInitStage3Proc                  	CP_CSW_SIM_SMS_BASE + 74
#define CP_CFW_SimInitStage1Proc                  	CP_CSW_SIM_SMS_BASE + 75
#define CP_CFW_SimGetPUCTProc                   	CP_CSW_SIM_SMS_BASE + 76
#define CP_CFW_SimSetPUCTProc                      	CP_CSW_SIM_SMS_BASE + 77
#define CP_CFW_SimMisGetProviderIdProc             	CP_CSW_SIM_SMS_BASE + 78
#define CP_CFW_SimMisSetPrefListProc               	CP_CSW_SIM_SMS_BASE + 79
#define CP_CFW_SimMisGetPrefListProc              	CP_CSW_SIM_SMS_BASE + 80

#define CP_CFW_SimMisGetPrefListMAXNumProc         	CP_CSW_SIM_SMS_BASE + 81
#define CP_CFW_SimMisGetACMProc                    	CP_CSW_SIM_SMS_BASE + 82
#define CP_CFW_SimMisReSetACMProc               	CP_CSW_SIM_SMS_BASE + 83
#define CP_CFW_SimSetACMMaxProc                    	CP_CSW_SIM_SMS_BASE + 84
#define CP_CFW_SimGetACMMaxProc                    	CP_CSW_SIM_SMS_BASE + 85


#define CP_CFW_SimAddPbkEntry                     	CP_CSW_SIM_SMS_BASE + 86
#define CP_CFW_SimDeletePbkEntry                  	CP_CSW_SIM_SMS_BASE + 87
#define CP_CFW_SimGetPbkEntry                   	CP_CSW_SIM_SMS_BASE + 88
#define CP_CFW_SimListCountPbkEntries              	CP_CSW_SIM_SMS_BASE + 89
#define CP_CFW_SimListPbkEntries                 	CP_CSW_SIM_SMS_BASE + 90
#define CP_CFW_SimGetPbkStrorageInfo              	CP_CSW_SIM_SMS_BASE + 92
#define CP_CFW_SimGetPbkStorage                  	CP_CSW_SIM_SMS_BASE + 93

#define CP_CFW_SatActivation                      	CP_CSW_SIM_SMS_BASE + 94
#define CP_CFW_SatResponse                         	CP_CSW_SIM_SMS_BASE + 95
#define CP_CFW_SatGetInformation                   	CP_CSW_SIM_SMS_BASE + 96
#define CP_CFW_SatResponseProc                   	CP_CSW_SIM_SMS_BASE + 97
#define CP_CFW_SatActivationProc                   	CP_CSW_SIM_SMS_BASE + 98
#define CP_CFW_SimGetSmsParameters              	CP_CSW_SIM_SMS_BASE + 99
#define CP_CFW_SimDeleMessageProc                  	CP_CSW_SIM_SMS_BASE + 100


//
//shell
//
#define CP_BAL_CfwSetupCbs				CP_CSW_SHELL_BASE + 1
#define CP_CFW_ShellControl				CP_CSW_SHELL_BASE + 2
#define CP_CFW_ShellProc				CP_CSW_SHELL_BASE + 3
#define CP_SHL_InitGlobalVariables		CP_CSW_SHELL_BASE + 4
#define CP_SHL_DisableModules			CP_CSW_SHELL_BASE + 5
#define CP_SHL_GetDisableModule			CP_CSW_SHELL_BASE + 6
#define CP_SHL_CleanupData				CP_CSW_SHELL_BASE + 7

//
//sms
//

#define CP_sms_mo_ComposeLongPDU 				CP_CSW_SMS_BASE + 1
#define CP_sms_mo_InitMeLongMSGPara   	        CP_CSW_SMS_BASE + 2
#define CP_sms_mo_Parse_PDU    	                CP_CSW_SMS_BASE + 3
#define CP_sms_mo_Parse_SmsPPErrorInd   		CP_CSW_SMS_BASE + 4
#define CP_sms_mo_SmsPPSendMMAReq               CP_CSW_SMS_BASE + 5
#define CP_sms_mt_CompressedDataDecode          CP_CSW_SMS_BASE + 6
#define CP_sms_mt_GSM2UCS2                      CP_CSW_SMS_BASE + 7
#define CP_sms_mt_Parse_LongMSGPara             CP_CSW_SMS_BASE + 8
#define CP_sms_mt_Parse_SmsPPReceiveInd         CP_CSW_SMS_BASE + 9
#define CP_sms_mt_Parse_SmsPPReceiveInd_SR      CP_CSW_SMS_BASE + 10
#define CP_sms_mt_SerializeTime                 CP_CSW_SMS_BASE + 11
#define CP_sms_mt_SetLongPara                   CP_CSW_SMS_BASE + 12
#define CP_sms_mt_SmsParseDCS                   CP_CSW_SMS_BASE + 13
#define CP_sms_mt_SmsPPAckReq                   CP_CSW_SMS_BASE + 14
#define CP_sms_mt_SmsPPErrorReq                 CP_CSW_SMS_BASE + 15
#define CP_sms_mt_Store_SmsPPReceiveInd         CP_CSW_SMS_BASE + 16
#define CP_sms_mt_UncompressedDataDecode        CP_CSW_SMS_BASE + 17
#define CP_sms_Sync_Delete                      CP_CSW_SMS_BASE + 18
#define CP_sms_Sync_List                        CP_CSW_SMS_BASE + 19
#define CP_sms_Sync_ListHeader                  CP_CSW_SMS_BASE + 20
#define CP_sms_Sync_Read                        CP_CSW_SMS_BASE + 21
#define CP_sms_Sync_SetUnRead2Read              CP_CSW_SMS_BASE + 22
#define CP_sms_Sync_SetUnSent2Sent              CP_CSW_SMS_BASE + 23
#define CP_sms_Sync_Write                       CP_CSW_SMS_BASE + 24
#define CP_sms_tool_ASCII2BCD                   CP_CSW_SMS_BASE + 25
#define CP_sms_tool_BCD2ASCII                   CP_CSW_SMS_BASE + 26
#define CP_sms_tool_ComposePDU                  CP_CSW_SMS_BASE + 27
#define CP_sms_tool_DecomposePDU                CP_CSW_SMS_BASE + 28
#define CP_sms_tool_GetLongMSGPara              CP_CSW_SMS_BASE + 30
#define CP_sms_tool_Parse_LongMSGPara           CP_CSW_SMS_BASE + 31
#define CP_sms_tool_SetLongMSGPara              CP_CSW_SMS_BASE + 33
#define CP_sms_tool_SMSTime_TO_SCTS             CP_CSW_SMS_BASE + 34
#define CP_sms_tool_SUL_AsciiToGsmBcd           CP_CSW_SMS_BASE + 35
#define CP_sms_tool_SUL_GsmBcdToAscii           CP_CSW_SMS_BASE + 36
#define CP_SUL_UserDialNum2StackDialNum         CP_CSW_SMS_BASE + 37
#define CP_CFW_SmsCopyMessages                  CP_CSW_SMS_BASE + 38
#define CP_CFW_SmsDeleteMessage                 CP_CSW_SMS_BASE + 39
#define CP_CFW_SmsDeleteMessageSync             CP_CSW_SMS_BASE + 40
#define CP_CFW_SmsGetMessageNode                CP_CSW_SMS_BASE + 41
#define CP_CFW_SmsListMessages                  CP_CSW_SMS_BASE + 42
#define CP_CFW_SmsListMessagesHeader            CP_CSW_SMS_BASE + 43
#define CP_CFW_SmsListMessagesHeaderSync        CP_CSW_SMS_BASE + 44
#define CP_CFW_SmsListMessagesSync              CP_CSW_SMS_BASE + 45
#define CP_CFW_SmsNewSmsAck                     CP_CSW_SMS_BASE + 46
#define CP_CFW_SmsQueryOverflowInd              CP_CSW_SMS_BASE + 47
#define CP_CFW_SmsReadMessage                   CP_CSW_SMS_BASE + 48
#define CP_CFW_SmsReadMessageSync               CP_CSW_SMS_BASE + 49
#define CP_CFW_SmsSendMessage                   CP_CSW_SMS_BASE + 50
#define CP_CFW_SmsSetUnRead2Read                CP_CSW_SMS_BASE + 51
#define CP_CFW_SmsSetUnRead2ReadSync            CP_CSW_SMS_BASE + 52
#define CP_CFW_SmsSetUnSent2Sent                CP_CSW_SMS_BASE + 53
#define CP_CFW_SmsSetUnSent2SentSync            CP_CSW_SMS_BASE + 54
#define CP_CFW_SmsWriteMessage                  CP_CSW_SMS_BASE + 55
#define CP_CFW_SmsWriteMessageSync              CP_CSW_SMS_BASE + 56
#define CP_CFW_SIMSmsInit                       CP_CSW_SMS_BASE + 57
#define CP_CFW_SmsCbInit                        CP_CSW_SMS_BASE + 59
#define CP_CFW_SmsMoInit                        CP_CSW_SMS_BASE + 60
#define CP_CFW_SmsMtInit                        CP_CSW_SMS_BASE + 61
#define CP_sms_cb_Reg                           CP_CSW_SMS_BASE + 62
#define CP_sms_CBAoProc                         CP_CSW_SMS_BASE + 63
#define CP_sms_CopyAoProc                       CP_CSW_SMS_BASE + 64
#define CP_sms_DleteAoProc                      CP_CSW_SMS_BASE + 65
#define CP_sms_InitAoProc                       CP_CSW_SMS_BASE + 66
#define CP_sms_ListAoProc                       CP_CSW_SMS_BASE + 67
#define CP_sms_ListHeaderAoProc                 CP_CSW_SMS_BASE + 68
#define CP_sms_mt_Reg                           CP_CSW_SMS_BASE + 69
#define CP_sms_MTAoProc                         CP_CSW_SMS_BASE + 70
#define CP_sms_ReadAoProc                       CP_CSW_SMS_BASE + 71
#define CP_sms_SendProc                         CP_CSW_SMS_BASE + 72
#define CP_sms_SetUnRead2ReadProc               CP_CSW_SMS_BASE + 73
#define CP_sms_SetUnSent2SentProc               CP_CSW_SMS_BASE + 74
#define CP_sms_WriteAoProc                      CP_CSW_SMS_BASE + 75
#define CP_CFW_MeDeleteMessage_Ex               CP_CSW_SMS_BASE + 76
#define CP_CFW_MeGetStorageInfo_Ex              CP_CSW_SMS_BASE + 77
#define CP_CFW_MeListMessage_Ex                 CP_CSW_SMS_BASE + 78
#define CP_CFW_MeReadMessage_Ex                 CP_CSW_SMS_BASE + 79
#define CP_CFW_MeWriteMessage_Ex                CP_CSW_SMS_BASE + 80
#define CP_CFW_SmsMoInit_Ex                     CP_CSW_SMS_BASE + 81
#define CP_SMS_SFSim2User_Ex                    CP_CSW_SMS_BASE + 82
#define CP_SMS_SFUser2Sim_Ex                    CP_CSW_SMS_BASE + 83
#define CP_CFW_MeDeleteMessage                  CP_CSW_SMS_BASE + 84
#define CP_CFW_MeGetStorageInfo                 CP_CSW_SMS_BASE + 85
#define CP_CFW_MeListMessage                    CP_CSW_SMS_BASE + 86
#define CP_CFW_MeReadMessage                    CP_CSW_SMS_BASE + 87
#define CP_CFW_MeWriteMessage                   CP_CSW_SMS_BASE + 88
#define CP_Create_SMS_DATA_DB                   CP_CSW_SMS_BASE + 89
#define CP_Create_SMS_DB                        CP_CSW_SMS_BASE + 90
#define CP_Remove_SMS_DATA_DB                   CP_CSW_SMS_BASE + 91
#define CP_Remove_SMS_DB                        CP_CSW_SMS_BASE + 92
#define CP_SMS_Append                           CP_CSW_SMS_BASE + 93
#define CP_sms_db_Init                          CP_CSW_SMS_BASE + 94
#define CP_SMS_DeleteFromMeByIndex              CP_CSW_SMS_BASE + 95
#define CP_SMS_DeleteFromMeByStatus             CP_CSW_SMS_BASE + 96
#define CP_SMS_GetStorageInfo                   CP_CSW_SMS_BASE + 97
#define CP_SMS_GetStorageInfo2                  CP_CSW_SMS_BASE + 98
#define CP_SMS_HaveFreeSlot                     CP_CSW_SMS_BASE + 99
#define CP_SMS_Read                             CP_CSW_SMS_BASE + 100
#define CP_SMS_Read2                            CP_CSW_SMS_BASE + 101
#define CP_SMS_SetStorageInfo                   CP_CSW_SMS_BASE + 102
#define CP_SMS_SFSim2User                       CP_CSW_SMS_BASE + 103
#define CP_SMS_SFUser2Sim                       CP_CSW_SMS_BASE + 104
#define CP_SMS_UpdateStorageInfo                CP_CSW_SMS_BASE + 105
#define CP_SMS_Write                            CP_CSW_SMS_BASE + 106
#define CP_sms_SetRead2UnReadProc               CP_CSW_SMS_BASE + 107
#define CP_CFW_SmsSetRead2UnRead                CP_CSW_SMS_BASE + 108
#define CP_CFW_SmsComposePdu                    CP_CSW_SMS_BASE + 109

//
//sim_pbk
//
#define CP_CFW_SimPBKAddProc				CP_CSW_SIM_PBK_BASE+1
#define CP_CFW_SimPBKDelProc				CP_CSW_SIM_PBK_BASE+2
#define CP_CFW_SimPBKEntryProc				CP_CSW_SIM_PBK_BASE+3
#define CP_CFW_SimPBKListEntryProc			CP_CSW_SIM_PBK_BASE+4
#define CP_CFW_SimPBKListCountEntryProc		CP_CSW_SIM_PBK_BASE+5
#define CP_CFW_SimPBKStorageInfoProc		CP_CSW_SIM_PBK_BASE+6
#define CP_CFW_SimPBKStorageProc			CP_CSW_SIM_PBK_BASE+7
#define CP_cfw_SimParsePBKRecData			CP_CSW_SIM_PBK_BASE+8
//
//sim_emod
//
#define CP_CFW_EMOD_GetBatteryInfo			CP_CSW_EMOD_BASE+1
#define CP_CFW_EMOD_AudioTestStart			CP_CSW_EMOD_BASE+2
#define CP_CFW_EMOD_AudioTestEnd			CP_CSW_EMOD_BASE+3
#define CP_CFW_EMOD_ClearUserInfo			CP_CSW_EMOD_BASE+4
#define CP_CFW_EMOD_GetIMEI 				CP_CSW_EMOD_BASE+5

//
//cc
//

#define CP_CC_Disc			            CP_CSW_CC_BASE+1 
#define CP_CloseAudio			            CP_CSW_CC_BASE+2 
#define CP_StopTheStupidStack		        CP_CSW_CC_BASE+3
#define CP_CFW_CcInit			            CP_CSW_CC_BASE+4
#define CP_CFW_CcAoProc			        CP_CSW_CC_BASE+5
#define CP_CFW_CcInitiateSpeechCall	    CP_CSW_CC_BASE+6
#define CP_CFW_CcInitiateSpeechCallEx	    CP_CSW_CC_BASE+7 
#define CP_CFW_CcReleaseCall		        CP_CSW_CC_BASE+8
#define CP_CFW_CcAcceptSpeechCall		    (0+9)
#define CP_CmMTAoProc			            CP_CSW_CC_BASE+10
#define CP_CFW_CcGetCurrentCall		    CP_CSW_CC_BASE+11
#define CP_GetCallStatus			        CP_CSW_CC_BASE+12
#define CP_CFW_CcGetCallStatus		        CP_CSW_CC_BASE+13
#define CP_CFW_CcCallHoldMultiparty	    CP_CSW_CC_BASE+14
#define CP_CFW_CcEmcDial			        CP_CSW_CC_BASE+15
#define CP_CFW_CcEmcDialEx			        CP_CSW_CC_BASE+16
#define CP_CFW_CcPlayTone			        CP_CSW_CC_BASE+17
#define CP_CFW_CcRejectCall		        CP_CSW_CC_BASE+18

#define CP_CallingPartyBCD2String		    CP_CSW_CC_BASE+19
#define CP_CallingPartyrRaw2BCD		    CP_CSW_CC_BASE+20
#define CP_String2BCD			            CP_CSW_CC_BASE+21
#define CP_CC_Release			            CP_CSW_CC_BASE+22
#define CP_CC_ReleaseCmplt			        CP_CSW_CC_BASE+23
#define CP_CC_CallConfirm			        CP_CSW_CC_BASE+24
#define CP_CC_Alert			            CP_CSW_CC_BASE+25
#define CP_CC_RejectCall			        CP_CSW_CC_BASE+26
#define CP_CC_Facility			            CP_CSW_CC_BASE+27
#define CP_CC_TerminateOne			        CP_CSW_CC_BASE+28
#define CP_CC_TerminateMulti		        CP_CSW_CC_BASE+29
#define CP_CC_RetrieveCall			        CP_CSW_CC_BASE+30
#define CP_CC_HoldCall			            CP_CSW_CC_BASE+31
#define CP_CC_Hold_AcceptCall		        CP_CSW_CC_BASE+32
#define CP_CC_HoldMPTY			            CP_CSW_CC_BASE+33
#define CP_CC_HoldMPTY_Acceptcall		    CP_CSW_CC_BASE+34
#define CP_CC_RetrieveMPTY			        CP_CSW_CC_BASE+35
#define CP_CC_StartDTMF			        CP_CSW_CC_BASE+36
#define CP_CC_StopDTMF			            CP_CSW_CC_BASE+37
#define CP_CC_GetIndex			            CP_CSW_CC_BASE+38
#define CP_CC_ProcNwDisconnectMsg		    CP_CSW_CC_BASE+39
#define CP_CC_ProcNwReleaseMsg		        CP_CSW_CC_BASE+40
#define CP_CC_ProcNwReleaseCmplMsg		    CP_CSW_CC_BASE+41
#define CP_CC_ProcHeldStateCHLD2X		    CP_CSW_CC_BASE+42
#define CP_CC_ProcMeetingStateCHLD2X	    CP_CSW_CC_BASE+43
#define CP_CC_ProcMeetingHeldStateCHLD2X	CP_CSW_CC_BASE+44
#define CP_CC_ModifyOtherAoState		    CP_CSW_CC_BASE+45
#define CP_CC_ModifyToMeetingHldState	    CP_CSW_CC_BASE+46
#define CP_CC_SendCrssInfoNotify		    CP_CSW_CC_BASE+47





//
//ss
//
#define CP_CFW_SsInit				        CP_CSW_SS_BASE+1
#define CP_CFW_SsStaticAoProc			    CP_CSW_SS_BASE+2
#define CP_CFW_SsAoProc				    CP_CSW_SS_BASE+3
#define CP_ss_GetMmiServCode			    CP_CSW_SS_BASE+4
#define CP_GetBSProtocolCode			    CP_CSW_SS_BASE+5
#define CP_GetBSMmiCode				    CP_CSW_SS_BASE+6
#define CP_ss_SetMmiClass				    CP_CSW_SS_BASE+7
#define CP_CFW_SsQueryCallWaiting 		CP_CSW_SS_BASE+15
#define CP_CFW_SsSetCallWaiting 		CP_CSW_SS_BASE+16
#define CP_CFW_SsQueryCallForwarding 	CP_CSW_SS_BASE+17
#define CP_CFW_SsSetCallForwarding 		CP_CSW_SS_BASE+18
#define CP_CFW_SsSetFacilityLock 		CP_CSW_SS_BASE+19
#define CP_CFW_SsQueryFacilityLock 		CP_CSW_SS_BASE+20
#define CP_CFW_SsChangePassword 		CP_CSW_SS_BASE+21
#define CP_CFW_SsQueryClip 			    CP_CSW_SS_BASE+22
#define CP_CFW_SsQueryClir 			    CP_CSW_SS_BASE+23
#define CP_CFW_SsQueryColp 			    CP_CSW_SS_BASE+24
#define CP_CFW_SsQueryColr 			    CP_CSW_SS_BASE+25
#define CP_CFW_SsSendUSSD 			    CP_CSW_SS_BASE+26
#define CP_CFW_SsTerminate 			    CP_CSW_SS_BASE+27
#define CP_ss_SetCF_Conf 			    CP_CSW_SS_BASE+28
#define CP_ss_interCFList_Conf		    CP_CSW_SS_BASE+29
#define CP_ss_Api_Err_Ind   		    CP_CSW_SS_BASE+30
#define CP_ss_inter_BSGroupList_Conf	CP_CSW_SS_BASE+31
#define CP_ss_SetCBInfo_Conf		    CP_CSW_SS_BASE+32
#define CP_ss_Inter_StatusInfo_Conf	    CP_CSW_SS_BASE+33
#define CP_ss_Inter_GSInfo_Conf		    CP_CSW_SS_BASE+34



//
//cfg
//

//GENERAL PART
#define CP_CFW_CfgSetPhoneFunctionality CP_CSW_CFG_BASE+1
#define CP_CFW_CfgGetPhoneFunctionality CP_CSW_CFG_BASE+2
#define CP_CFW_CfgStoreUserProfile      CP_CSW_CFG_BASE+3
#define CP_CFW_CfgRestoreFactoryProfile CP_CSW_CFG_BASE+4
#define CP_CFW_CfgGetUserProfile        CP_CSW_CFG_BASE+5

#define CP_CFW_CfgSetServiceReportingMode CP_CSW_CFG_BASE+6
#define CP_CFW_CfgGetServiceReportingMode CP_CSW_CFG_BASE+7
#define CP_CFW_CfgSetRLPType               CP_CSW_CFG_BASE+8
#define CP_CFW_CfgGetRLPType               CP_CSW_CFG_BASE+9
#define CP_CFW_CfgGetCharacterSet          CP_CSW_CFG_BASE+10

#define CP_CFW_CfgSetCharacterSet        CP_CSW_CFG_BASE+11
#define CP_CFW_CfgSetIndicatorEvent      CP_CSW_CFG_BASE+12
#define CP_CFW_CfgGetIndicatorEvent      CP_CSW_CFG_BASE+13
#define CP_CFW_CfgSetIndicatorEventInfo  CP_CSW_CFG_BASE+14
#define CP_CFW_CfgGetIndicatorEventInfo  CP_CSW_CFG_BASE+15

#define CP_CFW_CfgGetEventReport    CP_CSW_CFG_BASE+16
#define CP_CFW_CfgSetEventReport    CP_CSW_CFG_BASE+17
#define CP_CFW_CfgSetResultCodeFormatMode CP_CSW_CFG_BASE+18
#define CP_CFW_CfgSetResultCodePresentMode  CP_CSW_CFG_BASE+19
#define CP_CFW_CfgGetResultCodePresentMode  CP_CSW_CFG_BASE+20

#define CP_CFW_CfgGetResultCodeFormatMode CP_CSW_CFG_BASE+21
#define CP_CFW_CfgSetEchoMode               CP_CSW_CFG_BASE+22
#define CP_CFW_CfgGetEchoMode               CP_CSW_CFG_BASE+23
#define CP_CFW_CfgGetProductIdentification  CP_CSW_CFG_BASE+24
#define CP_CFW_CfgGetExtendedErrorV0        CP_CSW_CFG_BASE+25

#define CP_CFW_CfgSetExtendedErrorV0   CP_CSW_CFG_BASE+26
#define CP_CFW_CfgGetExtendedErrorV1   CP_CSW_CFG_BASE+27
#define CP_CFW_CfgGetErrorReport       CP_CSW_CFG_BASE+28
#define CP_CFW_CfgSetErrorReport       CP_CSW_CFG_BASE+29
#define CP_CFW_GetAtCapability         (CP_CSW_CFG_BASE+30)

//sms
#define CP_CFW_CfgSetSmsStorage      CP_CSW_CFG_BASE+31
#define CP_CFW_CfgGetSmsStorage      CP_CSW_CFG_BASE+32
#define CP_CFW_CfgSelectSmsService   CP_CSW_CFG_BASE+33
#define CP_CFW_CfgQuerySmsService    CP_CSW_CFG_BASE+34
#define CP_CFW_CfgSetSmsOverflowInd   CP_CSW_CFG_BASE+35

#define CP_CFW_CfgGetSmsOverflowInd   CP_CSW_CFG_BASE+36
#define CP_CFW_CfgSetSmsFormat   CP_CSW_CFG_BASE+37
#define CP_CFW_CfgGetSmsFormat   CP_CSW_CFG_BASE+38
#define CP_CFW_CfgSetSmsShowTextModeParam CP_CSW_CFG_BASE+39
#define CP_CFW_CfgGetSmsShowTextModeParam CP_CSW_CFG_BASE+40

#define CP_CFW_CfgSetSmsCB   CP_CSW_CFG_BASE+41
#define CP_CFW_CfgGetSmsCB CP_CSW_CFG_BASE+42
#define CP_CFW_CfgSetNewSmsAck    CP_CSW_CFG_BASE+43
#define CP_CFW_CfgGetNewSmsAck    CP_CSW_CFG_BASE+44
#define CP_CFW_CfgSetSmsCenter   CP_CSW_CFG_BASE+45

#define CP_CFW_CfgGetSmsCenter   CP_CSW_CFG_BASE+46
#define CP_CFW_CfgSetSmsFullForNew   CP_CSW_CFG_BASE+47
#define CP_CFW_CfgGetSmsFullForNew  CP_CSW_CFG_BASE+48
#define CP_CFW_CfgSetSmsParam   CP_CSW_CFG_BASE+49
#define CP_CFW_CfgGetSmsParam   CP_CSW_CFG_BASE+50

#define CP_CFW_CfgGetDefaultSmsParam CP_CSW_CFG_BASE+51
#define CP_CFW_CfgSetDefaultSmsParam CP_CSW_CFG_BASE+52
#define CP_CFW_CfgSetSmsStorageInfo  CP_CSW_CFG_BASE+53
#define CP_CFW_CfgGetSmsStorageInfo  CP_CSW_CFG_BASE+54
#define CP_CFW_CfgSetNewSmsOption    CP_CSW_CFG_BASE+55

#define CP_CFW_CfgGetNewSmsOption   CP_CSW_CFG_BASE+56
//pbk part
#define CP_CFW_CfgSetPbkStorage   CP_CSW_CFG_BASE+57
#define CP_CFW_CfgGetPbkStorage   CP_CSW_CFG_BASE+58
#define CP_CFW_CfgSetPbkStrorageInfo   CP_CSW_CFG_BASE+59
#define CP_CFW_CfgGetPbkStrorageInfo   CP_CSW_CFG_BASE+60

#define CP_CFW_CfgSetPbkCapacityInfo   CP_CSW_CFG_BASE+61
#define CP_CFW_CfgSetCalllogCapacityInfo CP_CSW_CFG_BASE+62
//nw part
#define CP_CFW_CfgSetNwOperDispFormat CP_CSW_CFG_BASE+63
#define CP_CFW_CfgGetNwOperDispFormat CP_CSW_CFG_BASE+64
#define CP_CFW_CfgNwGetOperatorId  CP_CSW_CFG_BASE+65

#define CP_CFW_CfgNwGetOperatorName    CP_CSW_CFG_BASE+66
#define CP_CFW_CfgNwGetOperatorInfo    CP_CSW_CFG_BASE+67
#define CP_CFW_CfgSetNwStatus   CP_CSW_CFG_BASE+68
#define CP_CFW_CfgGetNwStatus   CP_CSW_CFG_BASE+69
#define CP_CFW_CfgNwSetFrequencyBand    CP_CSW_CFG_BASE+70

#define CP_CFW_CfgNwGetFrequencyBand CP_CSW_CFG_BASE+71
#define CP_CFW_CfgNwSetNetWorkMode CP_CSW_CFG_BASE+72
#define CP_CFW_CfgNwGetNetWorkMode CP_CSW_CFG_BASE+73
//cm part
#define CP_CFW_CfgSetSSN  CP_CSW_CFG_BASE+74
#define CP_CFW_CfgGetSSN   CP_CSW_CFG_BASE+75

#define CP_CFW_CfgSetCallWaiting  CP_CSW_CFG_BASE+76
#define CP_CFW_CfgGetCallWaiting  CP_CSW_CFG_BASE+77
#define CP_CFW_CfgSetClir   CP_CSW_CFG_BASE+78
#define CP_CFW_CfgGetClir   CP_CSW_CFG_BASE+79
#define CP_CFW_CfgSetToneDuration CP_CSW_CFG_BASE+80

#define CP_CFW_CfgGetToneDuration CP_CSW_CFG_BASE+81
#define CP_CFW_CfgSetClip  CP_CSW_CFG_BASE+82
#define CP_CFW_CfgGetClip  CP_CSW_CFG_BASE+83
#define CP_CFW_CfgSetCallForwarding CP_CSW_CFG_BASE+84
#define CP_CFW_CfgGetCallForwarding CP_CSW_CFG_BASE+85

#define CP_CFW_CfgSetIncomingCallResultMode CP_CSW_CFG_BASE+86
#define CP_CFW_CfgGetIncomingCallResultMode CP_CSW_CFG_BASE+87
#define CP_CFW_CfgSetBearServType CP_CSW_CFG_BASE+88
#define CP_CFW_CfgGetBearServType CP_CSW_CFG_BASE+89
#define CP_CFW_CfgSetConnectResultFormatCode CP_CSW_CFG_BASE+90

#define CP_CFW_CfgGetConnectResultFormatCode CP_CSW_CFG_BASE+91
#define CP_CFW_CfgSetColp  CP_CSW_CFG_BASE+92
#define CP_CFW_CfgGetColp  CP_CSW_CFG_BASE+93
//device part
#define CP_CFW_CfgSetAudioAudioMode   CP_CSW_CFG_BASE+94
#define CP_CFW_CfgGetAudioAudioMode   CP_CSW_CFG_BASE+95

#define CP_CFW_CfgSetLoudspeakerVolumeLevel   CP_CSW_CFG_BASE+96
#define CP_CFW_CfgGetLoudspeakerVolumeLevel   CP_CSW_CFG_BASE+97
#define CP_CFW_CfgGetLoudspeakerVolumeLevelRange    CP_CSW_CFG_BASE+98
//tm part
#define CP_CFW_CfgSetTmParam    CP_CSW_CFG_BASE+99
#define CP_CFW_CfgGetTmParam    CP_CSW_CFG_BASE+100

//init part
#define CP_CFW_Cfg_SetInitValue    CP_CSW_CFG_BASE+101
#define CP_CFW_CfgInit    CP_CSW_CFG_BASE+102
//main part
#define CP_CFW_Cfg_atoi    CP_CSW_CFG_BASE+103
#define CP_GetNumber    CP_CSW_CFG_BASE+104
#define CP_CFW_CfgNumRangeCheck   CP_CSW_CFG_BASE+105

#define CP_GetString   CP_CSW_CFG_BASE+106
#define CP_CFW_Cfg_strcmp   CP_CSW_CFG_BASE+107
#define CP_CFW_CfgStrRangeCheck   CP_CSW_CFG_BASE+108
#define CP_WriteRegData   CP_CSW_CFG_BASE+109
#define CP_ReadRegData  CP_CSW_CFG_BASE+110


#define CP_cfg_read_8               CP_CSW_CFG_BASE+111
#define CP_cfg_write_8              CP_CSW_CFG_BASE+113
#define CP_cfg_write_str            CP_CSW_CFG_BASE+114
#define CP_cfg_read_str             CP_CSW_CFG_BASE+115
#define CP_cfg_read_32              CP_CSW_CFG_BASE+116
#define CP_cfg_write_32             CP_CSW_CFG_BASE+117
#define CP_cfg_read_hex             CP_CSW_CFG_BASE+118
#define CP_cfg_write_hex            CP_CSW_CFG_BASE+119
#define CP_cfg_readdefault          CP_CSW_CFG_BASE+120



#define CP_CFW_CfgGprsSetPdpCxt        CP_CSW_CFG_BASE+128
#define CP_CFW_CfgGprsGetPdpCxt        CP_CSW_CFG_BASE+129 
#define CP_CFW_CfgGprsSetReqQos        CP_CSW_CFG_BASE+130
#define CP_CFW_CfgGprsGetReqQos        CP_CSW_CFG_BASE+131
#define CP_CFW_CfgGprsSetMinQos        CP_CSW_CFG_BASE+132
#define CP_CFW_CfgGprsGetMInQos        CP_CSW_CFG_BASE+133
#define CP_CFW_CfgGetEmodParam         CP_CSW_CFG_BASE+134
#define CP_CFW_CfgSetEmodParam         CP_CSW_CFG_BASE+135



#endif
