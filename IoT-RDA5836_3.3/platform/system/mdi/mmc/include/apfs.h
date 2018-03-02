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

//                                                                            //
/// @file apfs.h                                                               //
/// That file implementes the APFS service.                                    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef APFS_H
#define APFS_H


#include "cs_types.h"

#include "fs.h"
#include "mci.h"
#include "id3_parser.h"
extern PUBLIC MCI_ERR_T apfs_GetFileInformation (
                        CONST INT32 FileHander,
                         AudDesInfoStruct  * CONST DecInfo,
                        CONST mci_type_enum FileType   );



extern PUBLIC MCI_ERR_T apfs_GetDurationTime(
							INT32 FileHander, 
							mci_type_enum FileType,
							INT32 BeginPlayProgress,
							INT32 OffsetPlayProgress,
							MCI_ProgressInf* PlayInformation);

#endif


