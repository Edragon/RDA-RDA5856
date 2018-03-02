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


/***********************************************
	FILENAME	:Coolprofile.h
	PURPOSE		:Coolprofile tool stub define
	REMARKS		:For TBM780
	DATE		:
************************************************/





#define u16 UINT16
#define u8  UINT8
#define CPMASK 0x3fff
#define CPEXITFLAG 0x8000 
#define TBM_ENTRY(id) hal_PXTS_SendProfilingCode((id) & CPMASK)
#define TBM_EXIT(id) hal_PXTS_SendProfilingCode(((id) & CPMASK) | CPEXITFLAG)

extern void hal_PXTS_SendProfilingCode(u16 code);
