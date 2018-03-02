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


/******************************************************************************/
/* Filename:    cos_task.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*   COS implementation.                                                      */
/******************************************************************************/
#include "sxs_type.h"
#include "sxr_cnf.h"
#include "sxr_sbx.hp"
//#include "itf_api.h"
#include "cswtype.h"
#include "cos.h"

//#define ENV_TEST
#ifdef ENV_TEST
#include "coolprofile.h"
#endif
#define IRQ_ENABLE //Open by lixp for losing msg at 20091112.
//
//Modify by Lixp at 20070627 for mailbox 
//
BOOL hasFreeEnv(UINT8 nMbxId)
{
#ifdef ENV_TEST
TBM_ENTRY(10001);
#endif
  #ifndef SAP_RESEARVE_ENV
  #define SAP_RESEARVE_ENV  1  // Need to check SXR this value. [2007-6-28:shenhankun]
  #endif
  
  #ifndef SAP_NB_MAX_MBX
  #define SAP_NB_MAX_MBX     85
  #endif 
  #ifndef SAP_NB_MAX_SEM
  #define SAP_NB_MAX_SEM     35
  #endif 

  //UINT16 scan       = sxr_Sbx.EnvIdxFree;
  UINT16 scan       = 0x00;//Move the value Assignment into Critical Section (Modify by lixp at 20091112).
  UINT16 n_free_env = 0;


  if ((nMbxId >= (SAP_NB_MAX_MBX + SAP_NB_MAX_SEM)) || (sxr_Sbx.Queue[nMbxId].Id != SXR_SBX_ALLOCATED))
  {
  #ifdef ENV_TEST
TBM_EXIT(10001);
#endif
    return FALSE;
  }
  else
  {
#ifdef ENV_TEST
TBM_ENTRY(10001);
#endif

#ifdef IRQ_ENABLE
   HANDLE hSection = COS_EnterCriticalSection();
#endif
    scan = sxr_Sbx.EnvIdxFree;
    while ((scan != SXR_NO_ENV) && (n_free_env < SAP_RESEARVE_ENV))
    {
      scan = sxr_Sbx.Env[scan].Next;
      n_free_env++;
    }
#ifdef IRQ_ENABLE
	COS_ExitCriticalSection(hSection);
#endif
#ifdef ENV_TEST
TBM_EXIT(10001);
#endif
 }
  
  if (n_free_env < SAP_RESEARVE_ENV)
  {
  #ifdef ENV_TEST
TBM_EXIT(10001);
#endif
    return FALSE;
  }
  #ifdef ENV_TEST
TBM_EXIT(10001);
#endif  
  return TRUE;
}

