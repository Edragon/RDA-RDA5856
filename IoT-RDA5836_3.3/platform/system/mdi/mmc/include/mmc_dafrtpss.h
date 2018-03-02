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
// FILENAME: mmc_audiompeg.c
//
// DESCRIPTION:
//   TODO...
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   xuml
//
////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mmc_dafrtpss.h
 *
 * Project:
 * --------
 *   Maui_sw
 *
 * Description:
 * ------------
 *   DAF sender streaming over RTP (RFC2250),
 *   including interface support for Bluetooth A2DP
 *
 * Author:
 * -------
 * -------

 ****************************************************************************/
#ifndef MMC_DAFRTPSS_H
#define MMC_DAFRTPSS_H

 
#include "cs_types.h"

#include "mci.h"

extern A2DP_codec_struct *A2DP_DAF_Open( SSHdl **ss_handle, A2DP_Callback pHandler, 
										bt_a2dp_mp3_codec_cap_struct *daf_config_data, 
										UINT8 *buf, UINT32 buf_len);
extern VOID A2DP_DAF_Close( SSHdl **ss_handle );
INT32 SSHdl_GetFreeSpace(SSHdl *hdl);
void SSHdl_GetWriteBuffer(SSHdl *hdl, UINT8 **buf, INT32 *buf_len);
void SSHdl_ShiftWritePointer(SSHdl *hdl, INT32 shamt);
INT32 SSHdl_GetDataCount(SSHdl *hdl);
void SSHdl_SniffNBytes(SSHdl *hdl, UINT32 offset, UINT32 N, UINT8 *dest);
 UINT32 GET_UINT32(UINT8 *ptr);
void SSHdl_GetReadBuffer(SSHdl *hdl, UINT8 **buf, INT32 *buf_len);
void SSHdl_ShiftReadPointer(SSHdl *hdl, INT32 shamt);
#endif


