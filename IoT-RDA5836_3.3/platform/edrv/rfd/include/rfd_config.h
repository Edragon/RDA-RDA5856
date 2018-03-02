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


#ifndef _RFD_CONFIG_H_
#define _RFD_CONFIG_H_

#include "cs_types.h"

typedef struct
{
    UINT8  RST;
    UINT8  PDN;
}XCV_CONFIG_T;

typedef struct
{
    UINT8  ENA;
    UINT8  TXEN;
    UINT8  BS;
}PA_CONFIG_T;

typedef struct
{
    UINT8  SW1;
    UINT8  SW2;
    UINT8  SW3;
}SW_CONFIG_T;

#endif
