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
 *	?
 * Author
 * 	?
 * Date
 * 	2007/11/25
 * Descriptions:
 *	...
 *
 *********************************************************/

#include "at_common.h"
#include "at_sa.h"
#include "at_module.h"

VOID AT_FreeCurCmd(AT_CMD** ppCmdList, AT_CMD* pCmd)
{
  if (*ppCmdList == NULL)
  {
    return;
  }

  if (pCmd != NULL)
  {
    *ppCmdList = pCmd->pNext;
    AT_FREE(pCmd);
  }

  return;
}

VOID AT_ResetCmdList(AT_CMD** ppCmdList)
{
  AT_CMD* p = NULL;
  AT_CMD* q = NULL;

  if (*ppCmdList == NULL)
  {
    return;
  }

  p = *ppCmdList;

  while (p != NULL)
  {
    q = p->pNext;
    AT_FreeCurCmd(ppCmdList, p);
    p = NULL;
    p = q;
  }

  *ppCmdList = NULL;

  return;
}
BOOL AT_AddCmdToList(AT_CMD* pCmd)
{
  AT_CMD* p = NULL;
  AT_CMD* q = NULL;
  AT_CMD** ppCmdList = AT_GetCommandList();

  if (pCmd == NULL)
    return FALSE;

  p = *ppCmdList;

  pCmd->pNext = NULL;

  pCmd->pCmdFunc = SA_GetCmdHandler(pCmd->pName, 0);

  if (p == NULL)
  {
    *ppCmdList = pCmd;
  }
  else
  {
    while (p != NULL)
    {
      q = p;
      p = p->pNext;
    }

    q->pNext = pCmd;
  }

  return TRUE;
}

INT32 AT_GetCmdCount(AT_CMD* pCmdList)
{
  INT32 count = 0;

  AT_CMD* p = NULL;
  AT_CMD* q = NULL;

  if (pCmdList == NULL)
  {
    return count;
  }

  p = pCmdList;

  while (p != NULL)
  {
    q = p->pNext;
    count++;
    p = q;
  }

  return count;
}

VOID AT_FreeCurInd(AT_IND** ppIndList, AT_IND* pInd)
{
  if (*ppIndList == NULL)
  {
    return;
  }

  if (pInd != NULL)
  {
    *ppIndList = pInd->pNext;
    AT_FREE(pInd);
  }

  return;
}

VOID AT_ResetIndList(AT_IND** ppIndList)
{
  AT_IND* p = NULL;
  AT_IND* q = NULL;

  if (*ppIndList == NULL)
  {
    return;
  }

  p = *ppIndList;

  while (p != NULL)
  {
    q = p->pNext;
    AT_FreeCurInd(ppIndList, p);

    if (p != NULL)
    {
      AT_FREE(p);
    }

    p = q;
  }

  *ppIndList = NULL;

  return;
}

VOID AT_AddIndToList(AT_IND** ppIndList, AT_IND* pInd)
{
  AT_IND* p = NULL;
  AT_IND* q = NULL;

  if (pInd == NULL)
  {
    return;
  }

  p = *ppIndList;

  if (p == NULL)
  {
    *ppIndList = pInd;
  }
  else
  {
    while (p != NULL)
    {
      q = p;
      p = p->pNext;
    }

    q->pNext = pInd;
  }

  return;
}

INT32 AT_GetIndCount(AT_IND* pIndList)
{
  INT32 count = 0;

  AT_IND* p = NULL;
  AT_IND* q = NULL;

  if (pIndList == NULL)
  {
    return count;
  }

  p = pIndList;

  while (p != NULL)
  {
    q = p->pNext;
    count++;
    p = q;
  }

  return count;
}
 
