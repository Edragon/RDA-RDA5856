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

#ifndef _TS_H_
#define _TS_H_

#include "cs_types.h"
#if 0
#define CFW_DRV_TS_ID         1
#define DM_TS_ID              2
#define CFW_DM_TS_ID          2
#define CFW_PM_TS_ID          3
#define BASE_BAL_TS_ID        10
#define BASE_COS_TS_ID        11
#define BASE_DB_TS_ID         12
#define BASE_REG_TS_ID        13
#define BASE_FFS_TS_ID        14
#define BASE_FAT_TS_ID        15
#define BASE_SUL_TS_ID        16
#define BASE_TM_TS_ID         17
#define BASE_VFS_TS_ID        18
#define TMS_TS_ID             19

#define CFW_AOM_TS_ID         30
#define CFW_CFG_TS_ID         31
#define CFW_NW_TS_ID          32
#define CFW_SIM_TS_ID         33
#define CFW_CC_TS_ID          34
#define CFW_SMS_TS_ID         35
#define CFW_SS_TS_ID          36
#define CFW_PBK_TS_ID         37
#define CFW_GPRS_TS_ID        38
#define CFW_CSD_TS_ID         39
#define CFW_TCPIP_TS_ID       40
#define CFW_PPP_TS_ID         41
#define CFW_SHELL_TS_ID       42
#define CFW_ML_TS_ID          43

#define AT_TS_ID              50
#define MMI_TS_ID_BASE        100

#define CSW_TS_ID             (MMI_TS_ID_BASE)
#else

#define CSW_TS_ID             	0
#define CFW_AOM_TS_ID         	1
#define CFW_SHELL_TS_ID       	2
#define CFW_NW_TS_ID          	3
#define CFW_SIM_TS_ID         	4
#define CFW_CFG_TS_ID         	5
#define CFW_CC_TS_ID          	6
#define CFW_SMS_TS_ID         	7
#define CFW_SS_TS_ID          	8
#define CFW_GPRS_TS_ID        	9
#define CFW_ML_TS_ID          	10
#define CFW_PM_TS_ID          	11
#define CFW_DM_TS_ID          	CFW_PM_TS_ID
#define CFW_DRV_TS_ID         	CFW_CFG_TS_ID
#define CFW_SAT_TS_ID         	CFW_ML_TS_ID
#define CSW_DM_TS_ID          4
#define CSW_DRV_TS_ID          5

#define BASE_DSM_TS_ID    1                 //must NOT bigger than 256
#define BASE_VDS_TS_ID    1                 //must NOT bigger than 256
#define BASE_FAT_TS_ID    2                 //must NOT bigger than 256
#define BASE_DB_TS_ID     3                 //must NOT bigger than 256
#define BASE_REG_TS_ID    4
#define BASE_BAL_TS_ID        12
#define BASE_COS_TS_ID        BASE_BAL_TS_ID
#define BASE_FFS_TS_ID        13
//#define BASE_FAT_TS_ID        14
#define BASE_TM_TS_ID         15
#define CFW_API_TS_ID         16
#endif
#define TS_PORT_SERIAL      0x01

#define TS_PORT_USB           0x08
#define TS_PORT_FILE          0x10

#define TS_OUTPUT_FILE        0x01
#define TS_OUTPUT_VC          0x02 // VC Debug output window.
#define TS_OUTPUT_CONSOLE     0x04 // Console window.
#define TS_OUTPUT_MF32        0x08 // MF32 tools

#define TS_ERROR_LEVEL        0x01
#define TS_WARNING_LEVEL      0x02
#define TS_INFO_LEVEL         0x04
#define TS_ASSERT_LEVEL       0x08

#define CSW_OUTPUT_TS_ERROR
#define CSW_OUTPUT_TS_WARNING
#define CSW_OUTPUT_TS_BRIEF
#define CSW_OUTPUT_TS_DETAIL


#ifdef CSW_OUTPUT_TS_ERROR
#define C_ERROR (0<<8)
#else
#define C_ERROR (1<<8)
#endif

#ifdef CSW_OUTPUT_TS_WARNING
#define C_WARNING (0<<8)
#else
#define C_WARNING (1<<8)
#endif

#ifdef CSW_OUTPUT_TS_BRIEF
#define C_BRIEF (0<<8)
#else
#define C_BRIEF (1<<8)
#endif

#ifdef CSW_OUTPUT_TS_DETAIL
#define C_DETAIL (0<<8)
#else
#define C_DETAIL (1<<8)
#endif

#ifndef HAVE_NO_TS_IF

#define TS_Init_Inner() TS_Open(TRUE)

UINT32 TS_Open(BOOL clearLevels);
UINT32 TS_Close(VOID);
UINT32 TS_SetOutputMask(UINT16 nIndex, BOOL bMode);
UINT32 SRVAPI TS_OutputText(UINT16 nModuleIdx, PCSTR pFormat, ...);
PCSTR SRVAPI TS_GetEventName(UINT32 nEvId);

#ifndef CSW_NO_TRACE
#if COMPILER==2
#define CSW_TRACE	TS_OutputText
#else
#define CSW_TRACE(nModuleIdx, pFormat, ...)  TS_OutputText((UINT16)nModuleIdx, (PCSTR)pFormat, ##__VA_ARGS__)
#endif
#else
#define CSW_TRACE(nModuleIdx, ...)
#endif //CSW_NO_TRACE


//////////////////////////////////////////////////////////////////////////

//
// Redefine the compatible debug API to trace.
// 
#if defined(DEBUG)
#define TRACE0(fmt)                               CSW_TRACE(0, (PCSTR)fmt) 
#define TRACE1(fmt, arg1)                         CSW_TRACE(0,(PCSTR)fmt,arg1)
#define TRACE2(fmt, arg1, arg2)                   CSW_TRACE(0,(PCSTR)fmt, arg1, arg2)
#define TRACE3(fmt, arg1, arg2, arg3)             CSW_TRACE(0,(PCSTR)fmt, arg1, arg2, arg3)
#define TRACE4(fmt, arg1, arg2, arg3, arg4)       CSW_TRACE(0,(PCSTR)fmt, arg1, arg2, arg3, arg4)
#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) CSW_TRACE(0,(PCSTR)fmt, arg1, arg2, arg3, arg4, arg5)
#define ASSERT(x)                                 DS_ASSERT(x)
#define CHECKHANDLE(handle)                       DS_CHECKHANDLE(handle)
#else // _DEBUG
#define TRACE0(f)					 {	 }
#define TRACE1(f, e1)				 {	 }
#define TRACE2(f, e1, e2)			 {	 }
#define TRACE3(f, e1, e2, e3)		 {	 }
#define TRACE4(f, e1, e2, e3, e4)	 {	 }
#define TRACE5(f, e1, e2, e3, e4, e5){	 }
#define CHECKHANDLE(handle)          {	 }
#define ASSERT(x)                    {	 }
#endif // _DEBUG

#ifndef _FS_SIMULATOR_
//#define printf                       {   }
//#define printf(fmt, ...)  TS_OutputText(9, (PCSTR)fmt, ##__VA_ARGS__) 
/* Map standard vpritnf to sxs_vpritnf to print the log to remote computer */
//#define vprintf(fmt, param)   sxs_vprintf(2 | TLEVEL(9), fmt,param)
#endif

#endif  //HAVE_NO_TS_IF
//
//add by lxp
//
#define TS_START_FUNC()  CSW_TRACE(0, "%s() Start \n", __FUNCTION__); 
#define TS_END_FUNC()    CSW_TRACE(0, "%s() End \n", __FUNCTION__); 
#endif // _H_

