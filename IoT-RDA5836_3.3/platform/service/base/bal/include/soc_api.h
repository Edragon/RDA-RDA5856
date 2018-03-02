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

/*******************************************************************************
 * Filename:
 * ---------
 *	soc_api.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file contains function prototypes, constants and enum for Socket API.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------

 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#if 0//ndef _SOC_API_H
#define _SOC_API_H

#if 0/* JIASHUO FOR WAP/MMS */
#if (!defined(__COOLSAND_TARGET__) || defined(WIN_SOC)) && !defined(GEN_FOR_PC)

#if !defined(_WINDOWS_) && defined(_PIXCOM_DATA_TYPES_H)
#error "Please include Windows.h before MMI_Data_Types.h in non __COOLSAND_TARGET__ compile option"
#endif

#ifndef _WINSOCKAPI_
#include <winsock2.h>
#endif//_WINSOCKAPI_
#endif
#endif
#include "cfw.h"
/*=============================
 * Constants
 *=============================*/
#define MAX_SOCKET_NUM	            (10) /* maximum sockets supported */
#define SOC_MAX_A_ENTRY              (5)

#define MAX_SOCK_ADDR_LEN           (16)

#define  IP_ADDR_LEN    (4)     /* length of an IP address */
#define  ETHER_ADDR_LEN     (6) /* length of an Ethernet address */

#ifdef MMI_ON_HARDWARE_P

/*
 * Protocol Familty
 */
#define PF_INET (0) /* currently only support Internet */

/* 
 * Protocols (RFC 1700)
 */

/* currently, only allow applicaion to set this type when RAW_SOCKET is used */
#define	IPPROTO_ICMP		(1)	   /* control message protocol */

/* for SOC/TCP/IP internal use, application shall NOT use */
#define	IPPROTO_IP		(0)             /* dummy for IP */
#define	IPPROTO_HOPOPTS	        (0)             /* IP6 hop-by-hop options */
#define	IPPROTO_IGMP		(2)	        /* group mgmt protocol */
#define	IPPROTO_IPV4		(4) 		/* IPv4 encapsulation */
#define	IPPROTO_IPIP		IPPROTO_IPV4	/* for compatibility */
#define	IPPROTO_TCP		(6)		/* tcp */
#define	IPPROTO_UDP		(17)		/* user datagram protocol */
#define	IPPROTO_RAW		(255)		/* raw IP packet */

/* 
 * Macros for SELECT 
 */
 #if 0
#define	FD_SET(n, p)	((p)->fds_bits[n] |= 0x01)
#define	FD_CLR(n, p)	((p)->fds_bits[n] &= ~(0x01))
#define	FD_ISSET(n, p)	((p)->fds_bits[n] & 0x02)
#define	FD_ZERO(p)	kal_mem_set(p, 0x00, sizeof(*(p)))

typedef struct
{
   UINT8	fds_bits[MAX_SOCKET_NUM];
} fd_set;
#endif

/*
 * Socket Type
 */
typedef enum
{
   SOCK_STREAM =0,	/* stream socket, TCP */
   SOCK_DGRAM,	        /* datagram socket, UDP */
   SOCK_SMS,         /* SMS bearer */
   SOCK_RAW          /* raw socket */
} socket_type_enum;

#else

typedef enum
{

   SOCK_SMS=100         /* SMS bearer */

} socket_type_enum;

#endif /* !WIN_SOC */

/* 
 * Any IP address
 */
//johnnie#define	INADDR_ANY		0x00000000

/*
 * Socket return codes, negative values stand for errors
 */




/*=============================
 * Enums
 *=============================*/


 
/*
 * howto arguments for shutdown(2), specified by Posix.1g.
 */
typedef enum
{
  SHUT_RD	=0,		/* shut down the reading side */
  SHUT_WR,			/* shut down the writing side */
  SHUT_RDWR			/* shut down both sides */
} soc_shutdown_enum;		

/*
 * Socket Options
 */
typedef enum
{
   SOC_OOBINLINE   = 0x01, /* not support yet */	
   SOC_LINGER	   = 0x02,  /* linger on close */
   SOC_NBIO	   = 0x04, /* Nonblocking */
   SOC_ASYNC	   = 0x08,  /* Asynchronous notification */   

   SOC_NODELAY     = 0x10,  /* disable Nagle algorithm or not */
   SOC_KEEPALIVE   = 0x20,  
   SOC_RCVBUF      = 0x40,  
   SOC_SENDBUF     = 0x80,

   SOC_NREAD       = 0x0100,  /* no. of bytes for read, only for soc_getsockopt */
   SOC_PKT_SIZE    = 0x0200,   
    SOC_SILENT_LISTEN = 0x0400, /* SOCK_SMS property */
    SOC_QOS = 0x0800,
    SOC_TCP_MAXSEG = 0x1000,
    SOC_IP_TTL = 0x2000,
    SOC_LISTEN_BEARER = 0x4000
} soc_option_enum;

/* 
 * event
 */
typedef enum
{
   SOC_READ    = 0x01,  /* Notify for read */
   SOC_WRITE   = 0x02,  /* Notify for write */
   SOC_ACCEPT  = 0x04,  /* Notify for accept */
   SOC_CONNECT = 0x08,  /* Notify for connect */
   SOC_CLOSE   = 0x10,   /* Notify for close */
   SOC_ERROR_IND   = 0x20,   /* Notify for close */
   SOC_DNS_RESOLV   = 0x40,   /* Notify for dns resolv */

} soc_event_enum;

typedef enum
{
    SOC_SUCCESS           = 0,
    SOC_ERROR             = -1,
    SOC_WOULDBLOCK        = -2,
    SOC_LIMIT_RESOURCE    = -3,    /* limited resource */
    SOC_INVALID_SOCKET    = -4,    /* invalid socket */
    SOC_INVALID_ACCOUNT   = -5,    /* invalid account id */
    SOC_NAMETOOLONG       = -6,    /* address too long */
    SOC_ALREADY           = -7,    /* operation already in progress */
    SOC_OPNOTSUPP         = -8,    /* operation not support */
    SOC_CONNABORTED       = -9,    /* Software caused connection abort */
    SOC_INVAL             = -10,   /* invalid argument */
    SOC_PIPE              = -11,   /* broken pipe */
    SOC_NOTCONN           = -12,   /* socket is not connected */
    SOC_MSGSIZE           = -13,   /* msg is too long */
    SOC_BEARER_FAIL       = -14,   /* bearer is broken */
    SOC_CONNRESET         = -15,   /* TCP half-write close, i.e., FINED */
    SOC_DHCP_ERROR        = -16,
    SOC_IP_CHANGED        = -17,
    SOC_ADDRINUSE         = -18,
    SOC_CANCEL_ACT_BEARER = -19    /* cancel the activation of bearer */
} soc_error_enum;

/*
 * bearer 
 */
typedef enum
{
    SOC_CSD  = 0x01,
    SOC_BEARER_CSD = SOC_CSD,
    SOC_GPRS = 0x02,
    SOC_BEARER_GPRS = SOC_GPRS,
    SOC_WIFI = 0x04,
    SOC_BEARER_WIFI = SOC_WIFI,
    SOC_BEARER_ANY = 0xff
} soc_bearer_enum;

typedef enum
{
   SOC_ACTIVATING,	
   SOC_ACTIVATED,
   SOC_DEACTIVATED,
   SOC_DEACTIVATING,
   SOC_AUTO_DISC_TIMEOUT
} soc_auto_disc_state_enum;

typedef enum
{
    SOC_QOS_BEST_EFFORT = 0x00,
    SOC_QOS_BACKGROUND = 0x10,
    SOC_QOS_VIDEO = 0x20,
    SOC_QOS_VOICE = 0x30
} soc_qos_enum;

/*=============================
 * structs 
 *=============================*/
typedef struct 
{
   UINT32 tv_sec;  /* no. of second */
   UINT32 tv_usec; /* no. of microsecond */
} soc_timeval_struct;



typedef struct 
{
   INT16	addr_len;
   UINT8	addr[MAX_SOCK_ADDR_LEN];
   UINT16	port;
 
} sockaddr_struct;

typedef struct
{
   BOOL   onoff;
   UINT16 linger_time; /* unit: sec */
} soc_linger_struct;

typedef struct
{
   UINT8       ref_count;
   UINT16      msg_len;	
   INT8        socket_id;    /* socket ID */
   soc_event_enum  event_type;   /* soc_event_enum */
   BOOL        result;
   soc_error_enum  error_cause;  /* used only when EVENT is close/connect */
   void      		*msg_ptr;	/* refer to ps_cause_enum if error_cause is
                                  * SOC_BEARER_FAIL */
} app_soc_notify_ind_struct;

/* The structure for DNS A RR entry */
typedef struct
{
    UINT8 address[4];   /* resolved IP address for queried domain name */
} soc_dns_a_struct;


/* Notification to application the result of DNS address query (soc_gethostbyname).
 * P.S. if application wants to accept this primitive, it should run in 
 * its own task instead of a task containing multiple applications, 
 * such as MMI. However, if application wants to run in the task of MMI 
 * and also wants to receive this primitive, it should use the 
 * "framework multiple protocol event handler" (since 08B). 
 * Otherwise, the primitive may be received by other 
 * applications running in the same task as you.
 */
 
typedef struct
{
    INT32	request_id; /* request id */
    UINT8   access_id;  /* access id */
    UINT32  account_id; /* network account id */
    UINT8	addr_len;   /* the first record in entry,  */
    UINT8	addr[16];   /* for backward compatibility */
    UINT8   num_entry;  /* number of entries. field of num_entry and entry are made 
                               to carry multiple answers for one domain name mapping to 
                               multiple IP addresses. */
    soc_dns_a_struct entry[SOC_MAX_A_ENTRY]; /* entries */

} app_soc_get_host_by_name_ind_struct;

#define MAX_DOMAIN_NAME_LENGTH 101//32 modified by wuys 2008-05-21
#define MAX_CONNECT_NUM    7*2+6

#define WEP_DNS_ID 1
#define JAVA_DNS_ID 2

typedef struct
{
    INT32	request_id; /* request id */
    UINT8    access_id;
    INT8      nsimID;
    INT8      ncid;
   INT8      domain_name[MAX_DOMAIN_NAME_LENGTH];
   
}HostName_RequestID_struct;

typedef struct _app_sco_timeoutval {
	INT32 connTimeOut;
	INT32 commTimeOut;
} app_sco_timeoutval;


/*=================================================== 
 *
 * Socket APIs prototypes
 *
 *===================================================*/
extern INT8 soc_create(UINT8  domain,
                           UINT8  type,
                           UINT8  protocol,
                           UINT16 mod_id,
                           UINT32 nwk_account_id);

extern INT8 soc_createupnp(UINT8  domain,
                           UINT8  type,
                           UINT8  protocol,
                           HANDLE	  taskID);


extern INT32 soc_close(INT8 s);

extern INT32 soc_bind(INT8 s, sockaddr_struct *addr);

extern INT32 soc_listen(INT8 s, UINT8 backlog);

extern INT32 soc_accept(INT8 s, CFW_TCPIP_SOCKET_ADDR *addr, HANDLE taskID);

extern INT32 soc_connect(INT8 s, sockaddr_struct *addr);

extern INT32 soc_sendto(INT8	     s,
                            UINT8       *buf,
                            UINT16	     len,
                            UINT8 	     flags,
                            CFW_TCPIP_SOCKET_ADDR *addr);

extern INT32 soc_send(INT8	 s,
                          UINT8  *buf,
                          UINT16  len,
                          UINT8	 flags);

extern INT32 soc_recvfrom(INT8	       s,
                              UINT8       *buf,
                              INT32       len,
                              UINT8	    flags,
                              CFW_TCPIP_SOCKET_ADDR *fromaddr);

extern INT32 soc_recv(INT8	     s,
                          UINT8      *buf,
                          INT32      len,
                          UINT8	     flags);         
extern INT32 soc_setsockopt(INT8 s,INT32 level, UINT32 option, UINT8 *val, UINT32 val_size);

extern INT32 soc_getsockopt(INT8   s,
	                           INT32 level,
                               INT32 option,
                               UINT8  *val,
                               INT32  val_size);

/* currently, this function only support non-blocking mode */
extern UINT32 soc_gethostbyname(BOOL       is_blocking,
                           UINT16     mod_id,
                           INT32      request_id,
                           const INT8 *domain_name,
                           UINT8	   *addr,
                           UINT8	   *addr_len,
                           UINT8      access_id,
                           UINT32 nwk_account_id);	

extern
INT8 soc_abort_dns_query(BOOL   by_mod_id,
                             UINT16 mod_id,
                             BOOL   by_request_id,
                             UINT32 request_id,
                             BOOL   by_access_id,
                             UINT8  access_id,
                             BOOL   by_nwk_account_id,
                             UINT32 nwk_account_id);
                             
extern void soc_close_nwk_account(UINT16 mod_id);
  
extern void soc_close_nwk_account_by_id(UINT16 mod_id, 
                                        UINT8 account_id);
  
extern 
INT8 soc_get_account_localip(INT8 s, UINT8 *local_ip);

extern 
UINT32 soc_getsockaddr(INT8 s, 
                         BOOL is_local,
                         sockaddr_struct *addr);  

extern
UINT32 soc_get_last_error(void);
extern
INT8 soc_open_bearer(INT8 s);

extern
INT8 soc_close_bearer(INT8 s);

extern
BOOL soc_ip_check(INT8 *asci_addr, 
                      UINT8 *ip_addr, 
                      BOOL *ip_validity);
void get_string_locaddr(char *buf);

                      
#ifdef __COOLSAND_TARGET__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
extern UINT32 htonl(UINT32 a);
#define ntohl(a)    htonl(a)
extern UINT16 htons(UINT16 a);
#define ntohs(a)    htons(a)
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __COOLSAND_TARGET__ */
#define	NTOHL(x)	(x) = ntohl((UINT32)(x))
#define	NTOHS(x)	(x) = ntohs((UINT16)(x))
#define	HTONL(x)	(x) = htonl((UINT32)(x))
#define	HTONS(x)	(x) = htons((UINT16)(x))

#endif /* _SOC_API_H */

