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


#ifndef _MMC_SNDREC_H_
#define _MMC_SNDREC_H_
#include"cs_types.h"
#include"fs.h"
#include"mci.h"

#define SNDRECBUF_LEN (1024*2)  //(8000)
#define SNDRECBUF_LEN_AMR475 (900)
#define SNDRECBUF_LEN_AMR515 (1200)
#define SNDRECBUF_LEN_AMR59 (1200)
#define SNDRECBUF_LEN_AMR67 (750)
#define SNDRECBUF_LEN_AMR74 (750)
#define SNDRECBUF_LEN_AMR795 (750)
#define SNDRECBUF_LEN_AMR102 (1050)
#define SNDRECBUF_LEN_AMR122 (1200)


typedef enum
{
	SNDREC_DATA_PART_HALF,
	SNDREC_DATA_PART_END		
}SNDREC_DATA_PART;

void sndRec_msgHandle(SNDREC_DATA_PART part);
void sndrec_halfHandler();
void sndrec_endHandler();

extern INT32 Mmc_sndRecStart(HANDLE fhd,U8 quality,mci_type_enum format, UINT32 samplerate ,MCI_AUDIO_FILE_RECORD_CALLBACK_T callback,  MCI_AUDIO_RECORD_BUFFER_PLAY_CALLBACK_T usercallback);
extern INT32 Mmc_sndRecStop(void);
extern INT32 Mmc_sndRecPause(void);
extern INT32 Mmc_sndRecResume(void);
extern INT32 Mmc_FmRecStart(HANDLE fhd,mci_type_enum format);



#endif //_MMC_SNDREC_H_
