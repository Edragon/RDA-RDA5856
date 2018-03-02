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
 *	at_cmd_special_emod.h
 * Author
 * 	Felix
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#ifndef _AT_CMD_EX_GPS_H_
#define _AT_CMD_EX_GPS_H_

#ifdef __cplusplus
extern "C" {
#endif

  /**************************************************************************************************
	 *                                          INCLUDES
	 **************************************************************************************************/
#  include "at_common.h"

#if defined(__AT_GPS__)
  VOID AT_EMOD_CmdFunc_GPS(AT_CMD_PARA *pParam);
  VOID AT_EMOD_CmdFunc_GPS_Sleep(AT_CMD_PARA *pParam);  
  VOID AT_EMOD_CmdFunc_GPS_Send(AT_CMD_PARA *pParam);
  VOID AT_EMOD_CmdFunc_GPS_I2C(AT_CMD_PARA *pParam);  
  VOID AT_EMOD_CmdFunc_GPS_AGPS(AT_CMD_PARA *pParam);
#endif
  /* 
   *
   */

  /**************************************************************************************************
	**************************************************************************************************/

#ifdef __cplusplus
}
#endif
#endif
