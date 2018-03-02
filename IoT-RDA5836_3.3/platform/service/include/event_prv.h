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
/*                                                                            */
/* FILE NAME                                                                  */
/*       event_prv.h                                                          */
/*                                                                            */
/* DESCRIPTION                                                                */
/*   event constant definitions and macros.                                   */
/*                                                                            */
/******************************************************************************/

#ifndef __EVENT_PRV_H__
#define __EVENT_PRV_H__

#define EV_CFW_INIT_REQ          (EV_CSW_REQ_BASE+1)
#define EV_CFW_XXX_END_          (EV_CSW_REQ_BASE+1)

#if (EV_CFW_XXX_END_ >= (EV_CSW_REQ_END_))
#error 'Error: invalid EV_CFW_XXX_END_ definition!'
#endif


#endif // _H_


