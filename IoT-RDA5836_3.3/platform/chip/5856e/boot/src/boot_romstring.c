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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file hal_romstring.h
/// That file describes the Dark Minds Behind This All.
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define STRINGIFY_VALUE(s) STRINGIFY(s)
#define STRINGIFY(s) #s

const char boot_romCsString[] =
    STRINGIFY_VALUE(CT_ASIC_CAPD) "\n"
    "Copyright RDA Microelectronics 2006-2016\n";


const char boot_romDT[] =
    "Coolsand\n"
    "RDA\n";



