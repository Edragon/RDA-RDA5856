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
/***********************************************************************
 *
 * MODULE NAME:    bt.h
 * DESCRIPTION:    Bluetooth Host Stack API 
 * AUTHOR:		   Martin Mellody
 *
 * SOURCE CONTROL: $Id: bt.h,v 1.3 2008/12/18 09:19:44 tianwq Exp $
 *
 * LICENSE:
 *	   This source code is copyright (c) 2000-2002 RDA Microelectronics.
 *	   All rights reserved.
 *
 ***********************************************************************/


/*
	CONFIGURATION OPTIONS:
	
	Before including this file, or in bt_sys.h....

	#define BLUETOOTH_NO_SDP to omit all of the sdp headers
	#define BLUETOOTH_NO_RFCOMMM to omit all of the rfcomm headers
	#define BLUETOOTH_NO_EXTAPI to omit all of the extended API headers
	#define BLUETOOTH_NO_SDP_SERVER to omit the sdp server headers
	#define BLUETOOTH_NO_SDP_CLIENT to omit the sdp client headers
	#define BLUETOOTH_NO_SECURITY to omit the host security headers
  
*/



#ifndef BLUETOOTH_HOST_DECLARED
#define BLUETOOTH_HOST_DECLARED

#ifdef __cplusplus
extern "C" {
#endif

/* Each platform should provide one of these for the build system to find */
#include "platform_config.h"

/* Controls presence of API parameter checking where implemented */
#define RDABT_API_PARAM_CHECKING          1 
/* If a RDA HC is detected - attempts to read proprietary version information */
#define QUERY_RDA_EXTENDED_INFO     1  /* ignored if pDEBUG==0 */

/* spec. version supported - this may be used in the build system */
#define RDABT_SPEC_VER_10B         0
#define RDABT_SPEC_VER_11          1
#define RDABT_SPEC_VER_12          2
#define RDABT_SPEC_VER_20          3
#define RDABT_SPEC_VER_21          4
#define RDABT_SPEC_VER_30          5
#define RDABT_SPEC_VER_40          6

#define RDABT_SPEC_VER             RDABT_SPEC_VER_30

/* The version number of the headers should match that of the core code */
#define BT_HOST_VERSION 	2 /* major software revision */
#define BT_HOST_REVISION	1 /* minor software revision */
#define BT_HOST_PATCHLEVEL	0 /* software patch level	 */


/* Error values returned by API functions */
#define RDABT_NOERROR			    0x00 
#define RDABT_PENDING			    0x01 
#define RDABT_INVALIDPARAM		    0x02 
#define RDABT_SECFAIL			    0x03 
#define RDABT_PINFAIL			    0x04 
#define RDABT_FLOWCTRL			    0x05
#define RDABT_NORESOURCES		    0x06 
#define RDABT_UNSUPPORTED		    0x07 
#define RDABT_HCITERROR		    0x08
#define RDABT_NOPRIV			    0x09
#define RDABT_HWERROR			    0x0A
#define RDABT_HOSTERROR		    0x0B
#define RDABT_UNKNOWNERROR		    0x0C
#define RDABT_HOSTINACTIVE		    0x0D
#define RDABT_RETRY			    0x0E
#define RDABT_AUTHORISATIONFAIL    0x0F
#define RDABT_AUTHENTICATIONFAIL   0x10
#define RDABT_ENCRYPTFAIL		    0x11
#define RDABT_TIMEOUT			    0x12
#define RDABT_PROTOERROR		    0x13
#define RDABT_DISALLOWED           0x14
#define RDABT_BUFTOOSMALL		    0x15
#define RDABT_DISCONNECTED         0x16

#define RDABT_INVALIDERRORCODE 0xfe


#define BT_GIAC_LAP  0x9E8B33
#define BT_LIAC_LAP  0x9E8B00
#ifndef GIAC_LAP
#define GIAC_LAP  BT_GIAC_LAP
#endif


#include "bt_types.h"
#include "bt_msg.h"
#include "bt_config.h"
#include "bt_utils.h"

#ifdef __cplusplus
} /* extern "C" { */
#endif


#endif /* BLUETOOTH_HOST_DECLARED */

