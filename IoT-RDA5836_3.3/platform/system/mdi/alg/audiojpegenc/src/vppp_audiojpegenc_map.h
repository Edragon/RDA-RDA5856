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
#ifndef MAP_EXPORT_H
#define MAP_EXPORT_H

/* This file defines the addresses of exported variables.
   It's created by voc_map automatically.*/

#define vpp_AudioJpeg_ENC_IN_STRUCT             	(0 + RAM_Y_BEGIN_ADDR)
#define vpp_AudioJpeg_ENC_OUT_STRUCT            	(14 + RAM_Y_BEGIN_ADDR)
#define VPP_MP3_Enc_Code_ExternalAddr_addr      	(22 + RAM_Y_BEGIN_ADDR)
#define VPP_AMR_Enc_Code_ExternalAddr_addr      	(32 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_MIC_DIGITAL_GAIN_ADDR            	(38 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_xScale                           	(7624 + RAM_Y_BEGIN_ADDR)
#define GLOBAL_yScale                           	(7626 + RAM_Y_BEGIN_ADDR)

#endif
