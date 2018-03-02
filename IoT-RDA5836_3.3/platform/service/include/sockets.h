/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#if 0//ndef __LWIP_SOCKETS_H__
#define __LWIP_SOCKETS_H__
#include <cswtype.h>
#include "tcpip_ip_addr.h"
#include "tcpip_opt.h"
struct sockaddr
{
	UINT8 sa_len;
	UINT8 sa_family;
	INT8 sa_data[14];
};

#ifndef socklen_t
#  define socklen_t INT32
#endif

#define CFW_TCPIP_SOCK_STREAM     1
#define CFW_TCPIP_SOCK_DGRAM      2
#define CFW_TCPIP_SOCK_RAW        3

/*
 * Option flags per-socket.
 */
#define  SO_DEBUG  0x0001	/* turn on debugging info recording */
#define  SO_ACCEPTCONN  0x0002	/* socket has had listen() */
#define  SO_REUSEADDR  0x0004	/* allow local address reuse */
#define  SO_KEEPALIVE  0x0008	/* keep connections alive */
#define  SO_DONTROUTE  0x0010	/* just use interface addresses */
#define  SO_BROADCAST  0x0020	/* permit sending of broadcast msgs */
#define  SO_USELOOPBACK  0x0040	/* bypass hardware when possible */
#define  SO_LINGER  0x0080	/* linger on close if data present */
#define  SO_OOBINLINE  0x0100	/* leave received OOB data in line */
#define	 SO_REUSEPORT	0x0200	/* allow local address & port reuse */

#define SO_DONTLINGER   (INT32)(~SO_LINGER)

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF  0x1001	/* send buffer size */
#define SO_RCVBUF  0x1002	/* receive buffer size */
#define SO_SNDLOWAT  0x1003	/* send low-water mark */
#define SO_RCVLOWAT  0x1004	/* receive low-water mark */
#define SO_SNDTIMEO  0x1005	/* send timeout */
#define SO_RCVTIMEO  0x1006	/* receive timeout */
#define  SO_ERROR  0x1007	/* get error status and clear */
#define  SO_TYPE    0x1008	/* get socket type */
#define SO_CONTIMEO  0x1009    /* Unimplemented: connect timeout */
#define SO_NO_CHECK  0x100a    /* don't create UDP checksum */

/*
 * Structure used for manipulating linger option.
 */
struct linger
{
	INT32 l_onoff;		/* option on/off */
	INT32 l_linger;		/* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define  SOL_SOCKET  0xfff	/* options for socket level */

#define CFW_TCPIP_AF_UNSPEC       0
#define CFW_TCPIP_AF_INET         2
#define CFW_TCPIP_PF_INET         CFW_TCPIP_AF_INET
#define CFW_TCPIP_PF_UNSPEC       CFW_TCPIP_AF_UNSPEC

#define CFW_TCPIP_IPPROTO_IP      0
#define CFW_TCPIP_IPPROTO_TCP     6
#define CFW_TCPIP_IPPROTO_UDP     17
#define IPPROTO_UDPLITE 136


#define LWIP_SOCK_STREAM     1
#define LWIP_SOCK_DGRAM      2
#define LWIP_SOCK_RAW        3
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



#define INADDR_ANY      0
#define INADDR_BROADCAST 0xffffffff

/* Flags we can use with send and recv. */
#define MSG_DONTWAIT    0x40	/* Nonblocking i/o for this operation only */

/*
 * Options for level CFW_TCPIP_IPPROTO_IP
 */
#define IP_TOS       1
#define IP_TTL       2


#if LWIP_IGMP
/*
 * Options and types for UDP multicast traffic handling
 */
#define IP_ADD_MEMBERSHIP  3
#define IP_DROP_MEMBERSHIP 4
#define IP_MULTICAST_TTL   5
#define IP_MULTICAST_IF    6
#define IP_MULTICAST_LOOP  7

typedef struct ip_mreq {
    struct _in_addr imr_multiaddr; /* IP multicast address of group */
    struct _in_addr imr_interface; /* local IP address of interface */
} ip_mreq;
#endif /* LWIP_IGMP */



#if LWIP_UDP && LWIP_UDPLITE
/*
 * Options for level IPPROTO_UDPLITE
 */
#define UDPLITE_SEND_CSCOV 0x01 /* sender checksum coverage */
#define UDPLITE_RECV_CSCOV 0x02 /* minimal receiver checksum coverage */
#endif /* LWIP_UDP && LWIP_UDPLITE*/

#define IPTOS_TOS_MASK          0x1E
#define IPTOS_TOS(tos)          ((tos) & IPTOS_TOS_MASK)
#define IPTOS_LOWDELAY          0x10
#define IPTOS_THROUGHPUT        0x08
#define IPTOS_RELIABILITY       0x04
#define IPTOS_LOWCOST           0x02
#define IPTOS_MINCOST           IPTOS_LOWCOST

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define IPTOS_PREC_MASK                 0xe0
#define IPTOS_PREC(tos)                ((tos) & IPTOS_PREC_MASK)
#define IPTOS_PREC_NETCONTROL           0xe0
#define IPTOS_PREC_INTERNETCONTROL      0xc0
#define IPTOS_PREC_CRITIC_ECP           0xa0
#define IPTOS_PREC_FLASHOVERRIDE        0x80
#define IPTOS_PREC_FLASH                0x60
#define IPTOS_PREC_IMMEDIATE            0x40
#define IPTOS_PREC_PRIORITY             0x20
#define IPTOS_PREC_ROUTINE              0x00

/*
 * Commands for ioctlsocket(),  taken from the BSD file fcntl.h.
 *
 *
 * Ioctl's have the command encoded in the lower word,
 * and the size of any in or out parameters in the upper
 * word.  The high 2 bits of the upper word are used
 * to encode the in/out status of the parameter; for now
 * we restrict parameters to at most 128 bytes.
 */
#if !defined(FIONREAD) || !defined(FIONBIO)
#define IOCPARM_MASK    0x7f	/* parameters must be < 128 bytes */
#define IOC_VOID        0x20000000	/* no parameters */
#define IOC_OUT         0x40000000	/* copy out parameters */
#define IOC_IN          0x80000000	/* copy in parameters */
#define IOC_INOUT       (IOC_IN|IOC_OUT)
					/* 0x20000000 distinguishes new &
					   old ioctl's */
#define _IO(x,y)        (IOC_VOID|((x)<<8)|(y))

#define _IOR(x,y,t)     (IOC_OUT|(((INT32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))

#define _IOW(x,y,t)     (IOC_IN|(((INT32)sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y))
#endif

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, UINT32)	/* get # bytes to read */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, UINT32)	/* set/clear non-blocking i/o */
#endif

/* Socket I/O Controls */
#ifndef SIOCSHIWAT
#define SIOCSHIWAT  _IOW('s',  0, UINT32)	/* set high watermark */
#define SIOCGHIWAT  _IOR('s',  1, UINT32)	/* get high watermark */
#define SIOCSLOWAT  _IOW('s',  2, UINT32)	/* set low watermark */
#define SIOCGLOWAT  _IOR('s',  3, UINT32)	/* get low watermark */
#define SIOCATMARK  _IOR('s',  7, UINT32)	/* at oob mark? */
#endif

#ifndef O_NONBLOCK
#define O_NONBLOCK    04000U
#endif

#ifndef FD_SET
#undef  FD_SETSIZE
#define FD_SETSIZE    16
#define FD_SET(n, p)  ((p)->fd_bits[(n)/8] |=  (1 << ((n) & 7)))
#define FD_CLR(n, p)  ((p)->fd_bits[(n)/8] &= ~(1 << ((n) & 7)))
#define FD_ISSET(n,p) ((p)->fd_bits[(n)/8] &   (1 << ((n) & 7)))
#define FD_ZERO(p)    SUL_MemSet8((VOID*)(p),0,sizeof(*(p)))

typedef struct fd_set
{
	UINT8 fd_bits[(FD_SETSIZE + 7) / 8];
} fd_set;

/* 
 * only define this in sockets.c so it does not interfere
 * with other projects namespaces where timeval is present
 */
#ifndef LWIP_TIMEVAL_PRIVATE
#define LWIP_TIMEVAL_PRIVATE 1
#endif

#if LWIP_TIMEVAL_PRIVATE
struct timeval
{
	INT32 tv_sec;		/* seconds */
	INT32 tv_usec;		/* and microseconds */
};
#endif

#endif
INT32 CFW_TcpipSocketShutdown(SOCKET nSocket, INT32 how);
INT32 CFW_TcpipSocketGetpeername(SOCKET nSocket, CFW_TCPIP_SOCKET_ADDR *pName, INT32 * pNameLen);
INT32 CFW_TcpipSocketGetsockname(SOCKET nSocket, CFW_TCPIP_SOCKET_ADDR *pName, INT32 * pNameLen);
INT32 CFW_TcpipSocketGetsockopt(SOCKET nSocket, INT32 level, INT32 optname, VOID *optval, INT32 * optlen);
INT32 CFW_TcpipSocketSetsockopt(SOCKET nSocket, INT32 level, INT32 optname, const VOID *optval, INT32 optlen);
INT32 CFW_TcpipSocketWrite(SOCKET nSocket, VOID *pData, UINT16 nDataSize);
INT32 CFW_TcpipSocketSelect(INT32 maxfdp1, fd_set * readset, fd_set * writeset, fd_set * exceptset, struct timeval *timeout);
INT32 CFW_TcpipSocketIoctl(SOCKET nSocket, INT32 cmd, VOID *argp);
VOID socketdata_initial(INT32 sockfd, UINT8 type);
void free_sock_sem();


#endif /* __LWIP_SOCKETS_H__ */
