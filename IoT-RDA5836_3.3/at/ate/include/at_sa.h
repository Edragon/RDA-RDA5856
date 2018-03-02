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

/*********************************************************
 *
 * File Name
 *	at_sa.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/
#ifndef __AT_SA_H__
#define __AT_SA_H__

#include "at_common.h"

typedef enum _SA_STAT_CLASS
{
  SA_POWER_ON_MODE,
  SA_ATCMD_SMSO_RESULT,
  SA_ATCMD_CFUN_RESULT,
  SA_ATCMD_ATO_RESULT,
  SA_ATCMD_ATD_RESULT,
  SA_ATCMD_ATA_RESULT,  // jyk
  SA_ATCMD_3PLUS_RESULT,
  SA_ATCMD_ATH_RESULT,
  SA_ATCMD_CGDATA_RESULT,
  SA_INIT_STATUS,
  SA_SIM_STATUS,
  SA_ATS_INIT_STATUS,
  SA_NW_REG_STATUS,
  AT_EVENT,

  // DO NOT execute set operation on the following:
  SA_INNER_FSM_STATUS_,
  SA_INNER_PROC_MODE_,
  SA_INNER_RUNCOMMAND_STATE_,
} SA_STAT_CLASS;

// TODO: 想法把bit set的详细情况直观地在文档和代码描述出来.

// TODO: 状态的完整定义.

/*
SA_POWER_ON_MODE class status mask
UINT32: 0x0000 xxxx
**/
#define SA_POWRN_ON_CAUSE_KEY			0x00000001
#define SA_POWRN_ON_CAUSE_CHARGE		0x00000002
#define SA_POWRN_ON_CAUSE_ALARM			0x00000003
#define SA_POWRN_ON_CAUSE_EXCEPTION		0x00000004
#define SA_POWRN_ON_CAUSE_RESET			0x00000005
#define SA_POWRN_ON_CAUSE_UNKOWN		0x00000006
#define SA_POWRN_ON_MASK_CLEAR			0x00000000

/*
SA_ATCMD_SMSO_RESULT  class status mask
UINT32: 0x0000 000x
**/
#define SA_ATCMD_SMSO_OK				0x00000001
#define SA_ATCMD_SMSO_CLEAR				0x00000000

/*
SA_ATCMD_CFUN_RESULT  class status mask
UINT32: 0x0000 000x
**/
#define SA_ATCMD_CFUN_5					0x00000001
#define SA_ATCMD_CFUN_6					0x00000002
#define SA_ATCMD_CFUN_CLEAR				0x00000000

#if 0 // DEMO
{
  // set +CFUN5 okay
  SA_SetSysStatus(SA_ATCMD_CFUN_RESULT, SA_ATCMD_CFUN_5);

  // clear +CFUN mask
  SA_SetSysStatus(SA_ATCMD_CFUN_RESULT, SA_ATCMD_CFUN_CLEAR);
}

#endif

/*
SA_ATCMD_ATO_RESULT  class status mask
UINT32: 0x0000 000x
**/
#define SA_ATCMD_ATO_OK					0x00000001
#define SA_ATCMD_ATO_CLEAR				0x00000000

/*
SA_ATCMD_3PLUS_RESULT  class status mask
UINT32: 0x0000 00xx
**/
#define SA_ATCMD_3PLUS_OK				0x00000001
#define SA_ATCMD_3PLUS_CLEAR			0x00000000

/*
#define SA_ATCMD_3PLUS_OK				0x00000001
#define SA_ATCMD_3PLUS_CLEAR			0x00000000
SA_ATCMD_ATH_RESULT  class status mask
UINT32: 0x0000 00xx
**/
#define SA_ATCMD_ATH_OK					0x00000001
#define SA_ATCMD_ATH_CLEAR				0x00000000

/*
SA_ATCMD_CGDATA_RESULT  class status mask
UINT32: 0x0000 00xx
**/
#define SA_ATCMD_CGDATA_OK				0x00000001
#define SA_ATCMD_CGDATA_CLEAR			0x00000000
/**
 *ATD*#    JYK 08/04/08
 */
#define SA_ATCMD_ATD_PS_OK			0x00000001
#define SA_ATCMD_ATD_PS_CLEAR			0x00000000
/**
 *other ATD cmd
 */
#define SA_ATCMD_ATD_OK			0x00000001
#define SA_ATCMD_ATD_CLEAR			0x00000000

/*
SA_INIT_STATUS  class status mask
UINT32: 0x0000 000x(bit: 00xx)
**/
#define SA_INIT_UNINIT					0x00000001
#define SA_INIT_INITIALLING				0x00000002
#define SA_INIT_COMPLETED				0x00000003
#define SA_INIT_MASK_CLEAR				0x00000000

/*
SA_SIM_STATUS  class status mask
UINT32: 0x0000 00xx
**/
#define SA_SIM_NO_SIM					0x00000001
#define SA_SIM_NEW_SIM					0x00000002
#define SA_SIM_TYPE_NORMAL				0x00000004
#define SA_SIM_TYPE_TEST				0x00000008
#define SA_SIM_TYPE_UNKNOW				0x0000000C
#define SA_SIM_AUTH_READY				0x00000010
#define SA_SIM_WAIT_PUK1				0x00000020
#define SA_SIM_WAIT_PIN1				0x00000040
#define SA_SIM_SAT_READY				0x00000100
#define SA_SIM_SMS_READY				0x00000200
#define SA_SIM_PBK_READY				0x00000400
#define SA_SIM_MASK_CLEAR				0x00000000

/*
SA_ATS_INIT_STATUS  class status mask
UINT32: 0x0000 000x(bit: 0001 >>SMS, 0010 >>PBK)
**/
#define SA_ATS_SMS_READY				0x00000001
#define SA_ATS_PBK_READY				0x00000002
#define SA_ATS_MASK_CLEAR				0x00000000

/*
SA_NW_REG_STATUS  class status mask
UINT32: 0x0000 000x(bit: 0001)
**/
#define SA_NW_REG_OK					0x00000001
#define SA_NW_REG_MASK_CLEAR			0x00000000

/*
SA_INNER_RUNCOMMAND_STATE_  class status mask
BOOL: TRUE, need to run next command; FALSE, needless to...
**/

typedef enum _sa_proc_mode_t
{
  SA_PROC_MODE_COMMAND,
  SA_PROC_MODE_ONLINE_CMD,
  SA_PROC_MODE_CS,
  SA_PROC_MODE_PS,
  SA_PROC_MODE_COUNT_,  // counter of this enum.
  SA_PROC_MODE_INVALID_,  // invalid value of this enum.
} sa_proc_mode_t;

#define IS_PS_MODE() ((SA_GetSysStatus(SA_INNER_PROC_MODE_) & SA_PROC_MODE_PS) == SA_PROC_MODE_PS)
#define IS_PS_MODE_MUX(nDlci) ((SA_GetSysStatusMux(SA_INNER_PROC_MODE_,nDlci) & SA_PROC_MODE_PS) == SA_PROC_MODE_PS)

/***************************************************************************************************
								Command Table
****************************************************************************************************/

typedef struct
{
  // Every FSM status according to a bit.
  // b0: SA_FSM_STATE_POWER_DOWN,
  // b1: SA_FSM_STATE_NORMAL,
  // b2: SA_FSM_STATE_CHGR_NORMAL,
  // b3: SA_FSM_STATE_CHGR_ONLY,
  // b4: SA_FSM_STATE_ALARM_NORMAL,
  // b5: SA_FSM_STATE_ALARM_ONLY,
  // b6: SA_FSM_STATE_SLEEP,
  UINT32 fsm_status_mask;

  // Every process mode according to a bit.
  // b0: SA_PROC_MODE_COMMAND,
  // b1: SA_PROC_MODE_ONLINE_CMD,
  // b2: SA_PROC_MODE_ONLINE_DATA,
  UINT32 proc_mode_mask;
} SA_CMDTBL_ENABLE_MASK_T;

/*
Commonly, we can classify AT commands to several level, for example, CC and SMS and PBK commands all are SIM related commands,
and HW, GC commands all are SIM not related commands. This is the 1st level class, and CC, SMS, PBK is the 2nd level class, and so on.

So, we can use some bits to mark the 1st level class, and use another group of bits to mark 2nd level class, and so on.

But, now for simpleness purpose, we just make a single level class, these are:
	SYS
	CC
	SMS
	NW
	SS
	PBK
	SAT
	ID
	SIM
	HW
	DS
	GC
	EX
	...
**/
typedef enum
{
    SA_CMDCLS_SYS,
    SA_CMDCLS_CC,
    SA_CMDCLS_SMS,
    SA_CMDCLS_NW,
    SA_CMDCLS_SS,
    SA_CMDCLS_PBK,
    SA_CMDCLS_SAT,
    SA_CMDCLS_ID,
    SA_CMDCLS_SIM,
    SA_CMDCLS_HW,
    SA_CMDCLS_DS,
    SA_CMDCLS_GC,
    SA_CMDCLS_EX,
    SA_CMDCLS_COUNT_,
    SA_CMDCLS_GPRS,
    SA_CMDCLS_TCPIP,
    SA_CMDCLS_DTM,
    SA_CMDCLS_DFMS,
	SA_CMDCLS_MCUWIFI,
	SA_CMDCLS_MUSIC,
	SA_CMDCLS_RECORD,
	SA_CMDCLS_COMMON,
    SA_CMDCLS_INVALID_ = 0x9FFF,  // An invalid enum value maker, and also force this enum type as UINT16 data type.
} SA_CMD_CLASS_T;

typedef struct
{
  UINT8* pName; // command name
  pfn_AtCmdHandler pFunc; // function point.
  SA_CMD_CLASS_T cmd_class; // class flag.
  UINT16 chief_enable_flag; // chief switcher. 0: disable, 1: enable.
  SA_CMDTBL_ENABLE_MASK_T enable_mask;
} SA_COMMAND_TABLE_T;

/*
For supporting GUI customization tool, need to define the command table in a special code file.
**/
extern const SA_COMMAND_TABLE_T g_CommandTable[];

extern HANDLE g_hAtTask;

#define SA_SetSysStatus(s,f)
#define SA_GetSysStatus(s)  (0xffff)
/*
**/
extern pfn_AtCmdHandler SA_GetCmdHandler(UINT8* cmd_name, UINT8 nDLCI);
extern VOID SA_DefaultCmdHandler_NotSuppotted(AT_CMD_PARA* pParam);
extern VOID SA_DefaultCmdHandler_CR(AT_CMD_PARA* pParam);
extern VOID SA_DefaultCmdHandler_Error(AT_CMD_PARA* pParam);
extern VOID SA_DefaultCmdHandler_AT(AT_CMD_PARA* pParam);
extern VOID AT_EX_CmdFunc_RCMD(AT_CMD_PARA* pParam);

#endif
 
