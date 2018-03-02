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



/* FILE NAME                                                                  */
/*      csw_shell.h                                                           */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   Communication Framework constant definitions and macros.                 */
/*   See CSW developement Manual for more.                                    */
/*                                                                            */
/******************************************************************************/

#ifndef _CSW_SHELL_H_
#define _CSW_SHELL_H_

#include <cfw.h>

//
// Shell 
//

UINT32 SRVAPI SHL_EraseFlash(
    UINT8 nMode  // 0: FS flash area, 1: all flash area exclude code.
); 

#ifdef CFW_PBK_SYNC_VER
UINT32 SRVAPI SHL_SimAddPbkEntry (
  UINT8 nStorage,
  CFW_SIM_PBK_ENTRY_INFO* pEntryInfo,
  CFW_PBK_OUT_PARAM* pOutParam
);

UINT32 SRVAPI SHL_SimDeletePbkEntry (
  UINT8 nStorage,
  UINT8 nIndex,
  CFW_PBK_OUT_PARAM* pOutParam
);
#endif 

#endif // _H

