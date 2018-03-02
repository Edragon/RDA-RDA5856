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


#include "vpp_audiojpegenc.h"
#include "mmc.h"
#include "cs_types.h"
#include"mcip_debug.h"
#if defined(CAMERA_SUPPORT)
#include"mmc_camera.h"
#endif /* CAMERA_SUPPORT */

#define JPEG_HEAD  608
static vpp_AudioJpeg_ENC_IN_T *g_vppaudiojpeg_encDecIn;

volatile int jpgEnFirstIsr=0;

void JPEGENCIsr(void)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_JPEGENC]JPEGENCIsr %d",jpgEnFirstIsr);

    if (jpgEnFirstIsr)
    {
    #if defined(CAMERA_SUPPORT)
        mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_JPEGENC_FINISH);
    #endif 
    }
    else
        jpgEnFirstIsr=1;
}


INT32 initVocJpegEncode(void)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_JPEGENC]initVocJpegEncode");
    jpgEnFirstIsr=0;
#if defined(CAMERA_SUPPORT)
    if(HAL_ERR_RESOURCE_BUSY == vpp_AudioJpegEncOpen((HAL_VOC_IRQ_HANDLER_T)JPEGENCIsr))
        return -1;
    else
    {
        g_vppaudiojpeg_encDecIn = (vpp_AudioJpeg_ENC_IN_T*) COS_MALLOC(sizeof(vpp_AudioJpeg_ENC_IN_T));

        if(NULL == g_vppaudiojpeg_encDecIn)
        {
            return -1;
        }
    }
 #endif 

   return 0;
}

void quitVocJpegEncode(void)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_JPEGENC]quitVocJpegEncode");
    vpp_AudioJpegEncClose();
    COS_FREE(g_vppaudiojpeg_encDecIn);
}

uint32 getJpegEncodeLen(void)
{
    vpp_AudioJpeg_ENC_OUT_T  g_vppaudiojpeg_encDecStatus;

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0,  "[MMC_JPEGENC]getJpegEncodeLen");

    //abtain the outputlen
    vpp_AudioJpegEncStatus(&g_vppaudiojpeg_encDecStatus);
    return g_vppaudiojpeg_encDecStatus.output_len;
    //do what you want....
}

void  MMC_jpgEn (UINT16 srcw, UINT16 srch, UINT16 imgw, UINT16 imgh, UINT16 quality, char *bufin, char *bufout, UINT16 yuvmode)
{
    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT, 0, "[MMC_JPEGENC]MMC_jpgEn sw=%d,sh=%d,iw=%d,ih=%d,q=%d", 
                        srcw,srch, imgw, imgh,quality);

    //encode a picture
    g_vppaudiojpeg_encDecIn->SampleRate = ((srch<<16)|srcw);
    g_vppaudiojpeg_encDecIn->imag_width = imgw;
    g_vppaudiojpeg_encDecIn->imag_height = imgh;
    g_vppaudiojpeg_encDecIn->imag_quality = quality;
    g_vppaudiojpeg_encDecIn->inStreamBufAddr = (INT32*)bufin;
    g_vppaudiojpeg_encDecIn->outStreamBufAddr = (INT32*)(bufout + JPEG_HEAD);
    g_vppaudiojpeg_encDecIn->mode = MMC_MJPEG_ENCODE;
    g_vppaudiojpeg_encDecIn->channel_num = yuvmode;

    MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT,
              0,
              "[MMC_JPEGENC]MMC_jpgEn......BEGIN TIME=%d",
              hal_TimGetUpTime());

    vpp_AudioJpegEncScheduleOneFrame(g_vppaudiojpeg_encDecIn);
}


BOOL mmc_JpegEncUserMsg(COS_EVENT *pMsg)
{

#if defined(CAMERA_SUPPORT)

    switch (pMsg->nEventId)
    {
        case MSG_JPEGENC_FINISH:
        {
            UINT8 * buff_out;

            buff_out = (UINT8 *) g_vppaudiojpeg_encDecIn->outStreamBufAddr;
            buff_out -= JPEG_HEAD;

            MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT,
                      0,
                      "[MMC_JPEGENC]ENCODING DONE! END TIME=%d",
                      hal_TimGetUpTime());

            VPP_WRITE_JPEGHeadr(g_vppaudiojpeg_encDecIn->imag_quality,
                                g_vppaudiojpeg_encDecIn->imag_width,
                                g_vppaudiojpeg_encDecIn->imag_height,
                                (uint8*) buff_out);
            hal_HstSendEvent(0xbcbc0104);
            MCI_TRACE(MCI_CAMERA_TRC|TSTDOUT,
                      0,
                      "[MMC_JPEGENC]VPP_WRITE_JPEGHeadr DONE! END TIME=%d",
                      hal_TimGetUpTime());

            mmc_SendEvent(GetMCITaskHandle(MBOX_ID_MMC), MSG_JPEGENC_CALLBACK);
        }
        break;

        default:
            return FALSE;
    }
   #endif 

    return TRUE;

}


