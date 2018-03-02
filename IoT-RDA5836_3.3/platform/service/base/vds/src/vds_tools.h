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
#ifndef _VDS_TOOLS_H_
#define _VDS_TOOLS_H_

BOOL vds_GetLineFromStr( char *line, int n, char *str, int *offset );
UINT32 vds_CRC32( CONST VOID *pvData, UINT32 iLen );
UINT32 vds_GetPBDCRC( VDS_PBD* psPBD );
INT32 vds_SetPBDCRC( VDS_PARTITION *psCP, UINT16 iSN, UINT16 iPBN, UINT32 iCRC );
BOOL vds_IsCRCChecked( UINT8* pBuff, UINT32 iLen, UINT32 iCRC, UINT32 iMode );


void vds_str_to_hex( char *str, long *len, char *buf );
void vds_hex_to_str( char *hex, long len, char *buf, char separator );


#endif // _VDS_TOOLS_H_




