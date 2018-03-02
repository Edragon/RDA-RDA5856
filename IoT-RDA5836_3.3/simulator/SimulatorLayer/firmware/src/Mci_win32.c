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

#include "stdio.h"
#include "mci.h"
#include "cos.h"

static UINT8 win32_mci_play_timer = 0;
MCI_STREAM_USER_HANDLER_T g_mmcStreamUserHandler = NULL;
 mci_type_enum  g_audio_format;

static void win32_mci_play_timeout(void *param)
{
    UINT8 pbuf[1024];
    if(mmc_aud_FileRead(0, pbuf, sizeof(pbuf)) < 100)
    {
        g_mmcStreamUserHandler(2);
    }

}

#if 1//defined(APBS)

INT32  MCI_AudioPlayStream(UINT8 *pBuffer, UINT32 len, UINT8 loop,
                           MCI_STREAM_USER_HANDLER_T callback,
                           mci_type_enum  format,INT32 startPosition)
{
    INT32 result;


    if(format==MCI_TYPE_DAF)
    {
        g_mmcStreamUserHandler = callback;

        mmc_InitStream(pBuffer,len);
        win32_mci_play_timer = COS_SetTimer(100, win32_mci_play_timeout, NULL, COS_TIMER_MODE_PERIODIC);
    }
    else
        return MCI_ERR_BAD_FORMAT;


    return result;
}

INT32  MCI_AudioStopStream(void)
{
    MCI_AudioStop();
}

UINT32 MCI_AudioPlayPause(VOID)
{
    INT32 result;

    return result;
}

UINT32 MCI_AudioPlayResume(VOID)
{
    INT32 result;

    return result;
}
UINT32 MCI_AddedData(UINT8* addedData, UINT32 addedDataBytes)
{
    return mmc_AddedData(addedData, addedDataBytes);
}

UINT32 MCI_GetRemainingBytes(void)
{
    return mmc_GetRemainingBytes();
}
UINT32 MCI_GetWriteBuffer(UINT8 **buffer)
{
    return mmc_GetWriteBuffer(buffer);
}
#endif /* APBS */

#if (CSW_EXTENDED_API_CAMERA == 0)
static const UINT8 *sim_cap_data_ptr[]=
{
	"a.jpg",
	"b.jpg",
	"c.jpg"
};

static UINT16 sim_cap_index = 0;
#define sim_cap_data_len (sizeof(sim_cap_data_ptr)/sizeof(UINT8 *))

UINT32 MCI_CamPowerUp (INT32  vediomode,void (*cb)(int32))   // MCI_CAM_POWER_UP_REQ
{
	sim_cap_index = 0;
    return 0;
}

UINT32 MCI_CamPowerDown (VOID)  //MCI_CAM_POWER_DOWN_REQ
{
	sim_cap_index = 0;
    return 0;
}

UINT32 MCI_CamCapture (void *data)    //MCI_CAM_CAPTURE_REQ
{
	sim_cap_index ++;

	if(sim_cap_index > sim_cap_data_len - 1)
		sim_cap_index = 0;

    return 0;
}

/******************************************************************************
 * MCI_CamSaveToBuffer:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns: file length, 0 if failed
 *
 *
 ******************************************************************************/
UINT32 MCI_CamSaveToBuffer(UINT8 *filebuffer, UINT32 buffersize)
{
	FILE  *fp;
	int nFileLen = 0; 
	int nReadLen = 0; 
	
	if((fp=fopen(sim_cap_data_ptr[sim_cap_index],"rb"))==NULL)
	{
		return 0;
	} 

	fseek(fp,0,SEEK_END); //定位到文件末
	nFileLen = ftell(fp); //文件长度 

	if(buffersize >= nFileLen && nFileLen > 0)
	{
		fseek(fp,0,SEEK_SET);
	    nReadLen = fread((char *)filebuffer,1,nFileLen,fp);
		fclose(fp);

	    if(nReadLen == nFileLen)
		{
			return nFileLen;
		}
	}
	
	return 0;
}


#if defined(IP_CAMERA_SUPPORT)
UINT32 MCI_IPCamStart(CAM_CAPTURE_STRUCT* data)
{
    return 0;

}

UINT32 MCI_IPCamStop(VOID)  
{
	return 0;
}

UINT32 MCI_IPCamGetFrame(UINT8 *buffer, UINT32 buffersize)
{
	MCI_CamCapture(NULL);
	return MCI_CamSaveToBuffer(buffer, buffersize);
}

#endif /* IP_CAMERA_SUPPORT */

#endif /* CSW_EXTENDED_API_CAMERA */

INT16 *MMC_SCOGetRXBuffer(VOID)
{
	
    //return (int16 *)(ReceivedScoDataCache);
     return NULL;
}

VOID MMC_SCOReceiveData(INT32 length)
{
}

void MMC_AudioCleanA2dpData(void)
{

}