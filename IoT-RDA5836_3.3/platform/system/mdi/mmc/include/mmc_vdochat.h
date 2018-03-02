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


#ifndef __MMC_VDOCHAT_H__
#define __MMC_VDOCHAT_H__


//declaration for customer
int   mmc_vdochat_powerOn(BOOL bIsRGB);
void mmc_vdochat_powerOff(void);

void mmc_vdochat_previewStart(UINT32 startX,UINT32 startY,UINT32 preW,UINT32 preH);
void mmc_vdochat_previewStop(void);

UINT8* mmc_vdochat_getData(UINT16 width, UINT16 height);

typedef enum
{
	//STOP  MESSAGE
	VDOCHAT_PRIVIEW=0x30,
	VDOCHAT_SETUP=0x31,
	VDOCHAT_ERR =  0x32,

} VDOCHAT_MSGID;

//declaration for ourself
 void vdochat_Callback(UINT8 id,  UINT8 ActiveBufNum);
void vdochat_usrmsgHandle(COS_EVENT *pnMsg) ;

#endif//__MMC_VDOCHAT_H__

