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
// NOTE : all size and laddr defines are given in word size

// definition of the sections
#define SECTION_0_START            (((RAM_I_MIN)+0x0000)/2)
#define SECTION_1_START            (((RAM_I_MIN)+0x0200)/2)

// start of the images
#define SECTION_MAIN_START          SECTION_0_START
#define SECTION_HI_START            SECTION_1_START
#define SECTION_BYE_START           SECTION_1_START

// end of the images
#define SECTION_MAIN_END            (((RAM_I_MIN)+0x0100)/2)
#define SECTION_HI_END              (((RAM_I_MIN)+0x0280)/2)
#define SECTION_BYE_END             (((RAM_I_MIN)+0x0280)/2)

// size of the images
#define SECTION_MAIN_SIZE           (SECTION_MAIN_END - SECTION_MAIN_START)
#define SECTION_HI_SIZE             (SECTION_HI_END   - SECTION_HI_START)
#define SECTION_BYE_SIZE            (SECTION_BYE_END  - SECTION_BYE_START)
