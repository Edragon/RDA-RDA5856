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
// FILENAME: mmc_audiocommon.c
//
// DESCRIPTION:
//   Play some type of file, which is store in T-card, buffer or stream
//
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   huangyx        2015.06.11      draft
//
////////////////////////////////////////////////////////////////////////////////

#include "cswtype.h"
#include "assert.h"
#include "fs.h"
#ifndef WIN32
#include "fs_asyn.h"
#include "aud_m.h"
#include "sxr_tls.h"
#include "mcip_debug.h"
#endif /* WIN32 */
#include "mci.h"
#include "mmc.h"
#include "cos.h"
#include "string.h"
#include "event.h"

#ifndef WIN32
#include "mmc_audioprefilter.h"
#else
#define MPEG2TSPACKETSIZE (188)
#endif

static UINT8 g_playstream = 0;
#ifdef WIN32
#define MCI_TRACE
#define MCI_AUDIO_TRC 1
#define VOLATILE
#endif /* WIN32 */
typedef struct _Audio_FileHandler_
{
	INT32 (*GetFileSize) (INT32 fd);
	INT32 (*FileSeek)(INT32 fd, INT32 iOffset, UINT8 iOrigin);
	INT32 (*FileRead)(INT32 fd, UINT8 *pBuf, UINT32 iLen);
} Audio_FileHandler;

typedef enum
{
    MMC_FILE_OPEN           = (1 << 0),
    MMC_FILE_GETFILESIZE    = (1 << 1),
    MMC_FILE_SEEK           = (1 << 2),
    MMC_FILE_READ           = (1 << 3),
    MMC_FILE_WRITE          = (1 << 4),
    MMC_FILE_CLOSE          = (1 << 5),
}Audio_FileOperate;
static Audio_FileHandler *mmc_file_handler = NULL;
#if 0
//flag which file opration is pending
static UINT32 mmc_file_operating = 0;

#define MMC_FILE_ENTER_OPERATE(p) (mmc_file_operating |= (p))
#define MMC_FILE_EXIT_OPERATE(p)    (mmc_file_operating &= ~(p))
#else
#define MMC_FILE_ENTER_OPERATE(p)
#define MMC_FILE_EXIT_OPERATE(p)
#endif /* 0 */

#if defined(__MMC_PLAY_BUFFER__) || defined(__MMC_PLAY_STREAM__)
static UINT32 mmc_Stream_Buffer_Length;
static UINT8 *mmc_Stream_Buffer_Address;
#endif /* __MMC_PLAY_STREAM__ || __MMC_PLAY_BUFFER__*/

#if defined(__MMC_PLAY_STREAM__)
/// Current position in the buffer. Next read data will be read from here.
static VOLATILE UINT8* g_mmcStreamCurPos= NULL;

/// Number of bytes of data to read in the buffer
static VOLATILE UINT32 g_mmcStreamRemainingBytes = 0;

/// End address of the stream buffer
static UINT8* g_mmcStreamTop = NULL;

/// Send request data when remain bytes < Threshold
static UINT32 g_mmcStreamThreshold = 0;

extern MCI_STREAM_USER_HANDLER_T g_mmcStreamUserHandler;

#ifndef CODEC_SUPPOT
BOOL g_audioIsReadingFIle = FALSE;
#endif

#define memcpy8 memcpy
static UINT8 *Mpeg2tsDemuxBuffer = NULL;
static UINT32 Mpeg2tsRemainingLen = 0;


extern mci_type_enum  g_audio_format;

#if 0
UINT8 * __memcpy8(UINT8 * dest, const UINT8 * src, UINT32 size)
{
    UINT8 *_u8_dest =  dest;
    UINT8 *_u8_end  =  dest + size;
    UINT8 *_u8_src  =  src;

    while (_u8_dest < _u8_end)
    {
        *_u8_dest++ = *_u8_src++;
    }

    return dest;
}
#endif /* 0 */

// =============================================================================
// mmc_AddedData
// -----------------------------------------------------------------------------
/// When a stream buffer is played in loop mode, already played data can be
/// replaced by new ones to play a long song seamlessly. The MCI service needs
/// to be informed about the number of new bytes to play in the buffer. This
/// is the role of the #mmc_AddedData function.
///
/// It is mandatory to call this function when a stream is played in loop mode
/// with handler defined.
///
/// If the function returns
/// #MCI_ERR_RESOURCE_BUSY, it means that the driver is busy with another
/// audio command.
///
/// @param addedDataBytes Number of bytes added to the buffer.
/// @return addedBytes Number of bytes REALLY added to the buffer.
// =============================================================================
UINT32 mmc_AddedData(const UINT8 *addedData, UINT32 addedDataBytes)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    UINT32 RemaingBytes = g_mmcStreamRemainingBytes;
    UINT8 *CurPos =(UINT8 *)g_mmcStreamCurPos;
    //non volatile
    UINT8 *WritePtr = NULL;
    UINT32 temp, WriteBytes = 0;

	assert(RemaingBytes <= mmc_Stream_Buffer_Length);
	//There is free buffer to wirte
	if(RemaingBytes < mmc_Stream_Buffer_Length)
	{
		WritePtr = CurPos+RemaingBytes;
        WriteBytes = mmc_Stream_Buffer_Length - RemaingBytes;
        WriteBytes = MIN(WriteBytes,addedDataBytes);

	    if(WritePtr >= g_mmcStreamTop )
	    {
	        WritePtr -= mmc_Stream_Buffer_Length;
	    }

		//If src address equal dst address, just add WriteBytes
	    if(WritePtr != addedData)
	    {
			if(WritePtr + WriteBytes > g_mmcStreamTop)
			{
				//Write buffer part 1
				temp = g_mmcStreamTop - WritePtr;
		        memcpy8(WritePtr, addedData, temp);

		        //Write buffer part 2
		        memcpy8(mmc_Stream_Buffer_Address,
		        			(addedData + temp),
		        			(WriteBytes - temp));
			}
			else
			{
		        memcpy8(WritePtr, addedData, WriteBytes);

			}
		}
	    g_mmcStreamRemainingBytes += WriteBytes;
	}

    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_AddedData:addedData=%x,WriteBytes=%d,RemainBytes = %d\n",
    					addedData, WriteBytes,g_mmcStreamRemainingBytes);

    hal_SysExitCriticalSection(status);
    return WriteBytes;
}

UINT32 mmc_GetRemainingBytes(void)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    UINT32 RemaingBytes = g_mmcStreamRemainingBytes;
    hal_SysExitCriticalSection(status);

    return RemaingBytes;
}

UINT32 mmc_GetEmptyBytes(void)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    UINT32 EmptyBytes = mmc_Stream_Buffer_Length - g_mmcStreamRemainingBytes;
    hal_SysExitCriticalSection(status);

    return EmptyBytes;
}


UINT32 mmc_GetWriteBuffer(UINT8 **buffer)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    UINT32 WriteAbleBytes,RemaingBytes = g_mmcStreamRemainingBytes;
    UINT8 *CurPos =(UINT8 *)g_mmcStreamCurPos;
    //non volatile
    UINT8 *WritePtr = NULL;

	WritePtr = CurPos+RemaingBytes;

    if(WritePtr >= g_mmcStreamTop )
    {
        WritePtr -= mmc_Stream_Buffer_Length;
    }

    if(WritePtr > CurPos)
    {
    	WriteAbleBytes = g_mmcStreamTop - WritePtr;
    }
    else if(WritePtr == CurPos)
    {
    	WriteAbleBytes = mmc_Stream_Buffer_Length - RemaingBytes;
    }
    else
    {
    	WriteAbleBytes = CurPos - WritePtr;
    }

    *buffer = WritePtr;

    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_GetWriteBuffer:WritePtr=%x, WriteAbleBytes=%d\n",
    					WritePtr, WriteAbleBytes);

    hal_SysExitCriticalSection(status);

    return WriteAbleBytes;
}


INT32 mmc_StreamSkip(UINT32 bytes)
{
    UINT32 status = hal_SysEnterCriticalSection();
    INT32 ret = 0;
    if (bytes <= mmc_Stream_Buffer_Length) {

        memcpy(mmc_Stream_Buffer_Address, (mmc_Stream_Buffer_Address+bytes), (mmc_Stream_Buffer_Length-bytes));
        g_mmcStreamRemainingBytes -= bytes;
    } else {

        ret =-1;

    }
    hal_SysExitCriticalSection(status);
    return ret;
}

static INT32 mmc_StreamRead(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    UINT32 RemaingBytes = g_mmcStreamRemainingBytes;
    UINT8 *CurPos =(UINT8 *)g_mmcStreamCurPos;
    UINT32 total_len;

    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_StreamRead:cur=%x,RemainBytes = %d, iLen=%d\n",
    					CurPos,RemaingBytes,iLen);

	total_len = MIN(iLen, RemaingBytes);

	if(CurPos + total_len > g_mmcStreamTop)
	{
	    UINT32 part1_len;

		//Write buffer part 1
		part1_len = g_mmcStreamTop - CurPos;
        memcpy8(pBuf, CurPos, part1_len);
        //Write buffer part 2
        CurPos = mmc_Stream_Buffer_Address;
        memcpy8((pBuf + part1_len), CurPos, (total_len - part1_len));
        g_mmcStreamCurPos = CurPos + total_len - part1_len;
	}
	else
	{
        memcpy8(pBuf, CurPos, total_len);
        g_mmcStreamCurPos = CurPos + total_len;
	}
	g_mmcStreamRemainingBytes -= total_len;

    if (g_mmcStreamRemainingBytes <= g_mmcStreamThreshold)
    {
	    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_StreamRead_REQUEST_DATA:RemainBytes = %d, Threshold=%d\n",
    					g_mmcStreamRemainingBytes,g_mmcStreamThreshold);
    	if (g_mmcStreamUserHandler)
    	{
    	    if(g_mmcStreamRemainingBytes)
        		g_mmcStreamUserHandler(STREAM_STATUS_REQUEST_DATA);
    	    else
        		g_mmcStreamUserHandler(STREAM_STATUS_NO_MORE_DATA);
    	}
    }

    hal_SysExitCriticalSection(status);


    return total_len;
}
static INT32 mmc_Mpeg2tsRead(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{

    UINT32 status = hal_SysEnterCriticalSection();

    UINT32 pid,demux_pid = 0x1100,datalen = 0,outlen = 0;
    UINT8 *end,*nextbuf,*data,*outbuf = pBuf,*buf;

    buf = Mpeg2tsDemuxBuffer + Mpeg2tsRemainingLen;

    if (iLen < MPEG2TSPACKETSIZE) {

        hal_SysExitCriticalSection(status);
        return 0;
    }

    while((mmc_GetRemainingBytes() + Mpeg2tsRemainingLen) >= MPEG2TSPACKETSIZE) {


        end = buf + mmc_StreamRead(fd,buf,Mpeg2tsDemuxBuffer + MPEG2TSPACKETSIZE - buf);

        buf = Mpeg2tsDemuxBuffer;

        for (; (nextbuf = buf + MPEG2TSPACKETSIZE) <= end; buf = nextbuf) {
            if (*buf != 0x47) {

                MCI_TRACE(MCI_AUDIO_TRC,0,"bad sync byte\n");
                nextbuf = buf + 1;
                continue;
            }
            pid = ((buf[1] << 8) + buf[2]) & 0x1fff;
            if (pid != demux_pid)
                continue;

            data = buf + 4;
            if (buf[3] & 0x20) {	/* buf contains an adaptation field */
                data = buf + 5 + buf[4];
                if (data > nextbuf)
                    continue;
            }
            if (buf[3] & 0x10)
            {
                MMC_Mpeg2tsDemux (data, nextbuf,outbuf,&outlen,(buf[1] & 0x40) ? 1 : 0,demux_pid);


               // hal_HstSendEvent(SYS_EVENT,0x88100013);
                //hal_HstSendEvent(SYS_EVENT,outlen);
               // hal_HstSendEvent(SYS_EVENT,(*outbuf<<24|*(outbuf+1)<<16|*(outbuf+2)<<8|*(outbuf+3)));
                outbuf += outlen;
                outlen = 0;
            }
        }



        Mpeg2tsRemainingLen = end - buf;
        memcpy (Mpeg2tsDemuxBuffer, buf, Mpeg2tsRemainingLen);
        buf = Mpeg2tsDemuxBuffer + Mpeg2tsRemainingLen;
        datalen  = outbuf - pBuf;
       // hal_HstSendEvent(SYS_EVENT,0x88100014);
        //hal_HstSendEvent(SYS_EVENT,datalen);


        if ((end != (Mpeg2tsDemuxBuffer + MPEG2TSPACKETSIZE))|| ((datalen + MPEG2TSPACKETSIZE) >= iLen))
        {
             break;
        }

    }

    hal_SysExitCriticalSection(status);
    return datalen;

}

static INT32 mmc_StreamSeek(INT32 fd, INT32 iOffset, UINT8 iOrigin)
{
    UINT32 status = hal_SysEnterCriticalSection();
    //volatile
    //UINT32 RemaingBytes = g_mmcStreamRemainingBytes;
    //UINT8 *CurPos =g_mmcStreamCurPos;

    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_StreamSeek RemainBytes =%d, iOffset=%d\n", g_mmcStreamRemainingBytes,iOffset);

    if((iOffset < 0) || (iOffset > g_mmcStreamRemainingBytes))
    {
	hal_SysExitCriticalSection(status);
    	return -1;
    }
    else
    {
    	g_mmcStreamCurPos +=iOffset;

    	if(g_mmcStreamCurPos > g_mmcStreamTop)
    		g_mmcStreamCurPos -= mmc_Stream_Buffer_Length;

    	g_mmcStreamRemainingBytes -= iOffset;
    }

    hal_SysExitCriticalSection(status);

    return 0;
}


static INT32 mmc_StreamGetSize(INT32 fd)
{
	return 0x7FFFFFFF;
}

void mmc_InitStream(UINT8 *buffer, UINT32 buf_len, UINT32 vaildLen, UINT32 buf_threshold)
{
    mmc_Stream_Buffer_Address          = buffer;
    g_mmcStreamTop                       	= buffer + buf_len;
    g_mmcStreamCurPos                  		= buffer;
    g_mmcStreamRemainingBytes    = vaildLen;
    mmc_Stream_Buffer_Length           = buf_len;
    g_mmcStreamThreshold              = buf_threshold;
    g_playstream                      = TRUE;
    //mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_StreamRead);
    if (g_audio_format == MCI_TYPE_TS)
    {
        if (Mpeg2tsDemuxBuffer == NULL)
            Mpeg2tsDemuxBuffer =  COS_MALLOC(MPEG2TSPACKETSIZE);

        Mpeg2tsRemainingLen = 0;
        mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_Mpeg2tsRead);
    }
    else
    {

         mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_StreamRead);

    }

    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_Stream_Buffer_Address: %x\n", mmc_Stream_Buffer_Address);
    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] g_mmcStreamTop: %x\n", g_mmcStreamTop);
    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_Stream_Buffer_Length: %d\n", mmc_Stream_Buffer_Length);
}

void mmc_EndStream(void)
{
    hal_HstSendEvent(SYS_EVENT,0x88100014);
    if(NULL != mmc_file_handler)
    {
        COS_FREE(mmc_file_handler);
        mmc_file_handler = NULL;
    }

    if (NULL != Mpeg2tsDemuxBuffer)
    {
        COS_FREE(Mpeg2tsDemuxBuffer);
        Mpeg2tsDemuxBuffer =  NULL;
    }
    g_mmcStreamRemainingBytes = NULL;
    g_playstream = NULL;
}
void mmc_ResumeStream(UINT8 *buffer, UINT32 buf_len,mci_type_enum  format)
{
    mmc_Stream_Buffer_Address          = buffer;
    mmc_Stream_Buffer_Length           = buf_len;
    g_playstream                      = TRUE;
    g_audio_format = format;
    //mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_StreamRead);
    if (g_audio_format == MCI_TYPE_TS)
    {
        if (Mpeg2tsDemuxBuffer == NULL)
            Mpeg2tsDemuxBuffer =  COS_MALLOC(MPEG2TSPACKETSIZE);

        mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_Mpeg2tsRead);
    }
    else
    {

         mmc_aud_SetupFileHandle(mmc_StreamGetSize,mmc_StreamSeek,mmc_StreamRead);

    }

}


#endif /* __MMC_PLAY_STREAM__ */


#if defined(__MMC_PLAY_BUFFER__)
static UINT32 g_Audio_Play_Offset; //Real offset in mmc_Stream_Buffer
static UINT8 g_Audio_Buffer_Loop;//Current loop times
static UINT8 g_Audio_Buffer_MaxLoopTimes;//Max loop times
static UINT32 g_Audio_Buffer_TagSize;

/*  Treat taotal play buffer as:
    | TAG | LOOP_CONTENT 0 | LOOP_CONTENT 1 | ...... | LOOP_CONTENT max_loop_time-1 | LOOP_CONTENT max_loop_time |
*/
#define loop_content_len   (mmc_Stream_Buffer_Length - g_Audio_Buffer_TagSize)
#define total_content_len  (g_Audio_Buffer_TagSize + (loop_content_len * g_Audio_Buffer_MaxLoopTimes))
/******************************************************************************
 * mmc_BuffTagSize:
 * DESCRIPTION: - Buffer head size
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void mmc_BuffTagSize(UINT32 tagsize)
{
    g_Audio_Buffer_TagSize = tagsize;
}

/******************************************************************************
 * mmc_BuffRead:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT32 mmc_BuffRead(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{
    INT32 len = 0;

//    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[BufR] iLen=%d, tagsize=%d,buf_len=%d,loop=%d,Maxloop=%d\n", iLen,
//                    g_Audio_Buffer_TagSize,        mmc_Stream_Buffer_Length,
//                    g_Audio_Buffer_Loop,        g_Audio_Buffer_MaxLoopTimes);

    if(g_Audio_Buffer_MaxLoopTimes!=0)
    {
        if(g_Audio_Buffer_Loop >= g_Audio_Buffer_MaxLoopTimes)
        {
            memset(pBuf, 0, iLen);
            return iLen;
        }
    }

    while(len < iLen)
    {

//        MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[BufR] len=%d, offset=%d,loop=%d\n",
//                   len,g_Audio_Play_Offset,g_Audio_Buffer_Loop);

        if(g_Audio_Play_Offset + iLen - len < mmc_Stream_Buffer_Length)
        {
            if(g_Audio_Buffer_Loop >= g_Audio_Buffer_MaxLoopTimes)
            {
                // make data error to stop it
                memset(pBuf, 0, iLen-len);
            }
            else
            {
                memcpy8(pBuf, mmc_Stream_Buffer_Address+g_Audio_Play_Offset, iLen-len);
            }
            g_Audio_Play_Offset += iLen - len;
            len = iLen;
        }
        else
        {
            memcpy8(pBuf, mmc_Stream_Buffer_Address+g_Audio_Play_Offset, mmc_Stream_Buffer_Length-g_Audio_Play_Offset);
            pBuf += mmc_Stream_Buffer_Length-g_Audio_Play_Offset;
            len += mmc_Stream_Buffer_Length-g_Audio_Play_Offset;
            g_Audio_Play_Offset = g_Audio_Buffer_TagSize;

            //Reach end of buffer
            if(g_Audio_Buffer_MaxLoopTimes!=0)
            {
                g_Audio_Buffer_Loop++;
                if(g_Audio_Buffer_Loop >= g_Audio_Buffer_MaxLoopTimes)
                {
                    return len;
                }
            }
        }
    };

    return len;
}

/******************************************************************************
 * mmc_BuffSeek:
 * DESCRIPTION: - seek in loop buffer
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT32 mmc_BuffSeek(INT32 fd, INT32 iOffset, UINT8 iOrigin)
{
    UINT8  loop;
    INT32 tmp = g_Audio_Play_Offset + (loop_content_len * g_Audio_Buffer_Loop);
    UINT32 file_size = total_content_len;


    switch(iOrigin)
    {
        case FS_SEEK_SET:
            tmp = 0 + iOffset;
            break;

        case FS_SEEK_CUR:
            tmp = tmp + iOffset;
            break;

        case FS_SEEK_END:
            tmp = file_size + iOffset;
            break;

        default:
            return ERR_FS_INVALID_PARAMETER;
    }

    tmp = tmp < 0 ? 0 : (tmp > (INT32)file_size ? (INT32)file_size: tmp);

    //convert to real buffer offset and loop times
    for(loop = 0; tmp > mmc_Stream_Buffer_Length; loop++)
    {
        tmp -= loop_content_len;
    }

    g_Audio_Play_Offset = tmp;
    g_Audio_Buffer_Loop = loop;

    return g_Audio_Play_Offset + (loop_content_len * g_Audio_Buffer_Loop);
}

/******************************************************************************
 * mmc_BuffGetSize:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
static INT32 mmc_BuffGetSize(INT32 fd)
{
    return total_content_len;
}


BOOL mmc_BuffInit(UINT8 *Pdata, UINT32 len, UINT8 LoopTimes)
{
    mmc_Stream_Buffer_Address = Pdata;
    mmc_Stream_Buffer_Length = len;
    g_Audio_Buffer_MaxLoopTimes = LoopTimes;
    g_Audio_Buffer_Loop  = 0;
    g_Audio_Play_Offset = 0;
    g_Audio_Buffer_TagSize = 0;
    g_playstream = 0;
    mmc_aud_SetupFileHandle(mmc_BuffGetSize,mmc_BuffSeek,mmc_BuffRead);

    return TRUE;
}
#endif /* __MMC_PLAY_BUFFER__ */



INT32 mmc_aud_GetFileSize(INT32 fd)
{
    INT32 filesize = 0;

    MMC_FILE_ENTER_OPERATE(MMC_FILE_GETFILESIZE);
    if(mmc_file_handler)
    {
        filesize = mmc_file_handler->GetFileSize(fd);
    }
#ifdef SERVICE_BASE_FS_SUPPORT
    else
    {
        filesize = (INT32)FS_GetFileSize(fd);
    }
#endif
    MMC_FILE_EXIT_OPERATE(MMC_FILE_GETFILESIZE);

    return filesize;
}

INT32 mmc_aud_FileSeek(INT32 fd, INT32 iOffset, UINT8 iOrigin)
{
    INT32 retval = 0;

    MMC_FILE_ENTER_OPERATE(MMC_FILE_SEEK);
    if(mmc_file_handler)
    {
        retval = mmc_file_handler->FileSeek(fd,iOffset,iOrigin);
    }
#ifdef SERVICE_BASE_FS_SUPPORT
    else
    {
        retval = FS_Seek(fd, iOffset, iOrigin);
    }
#endif
    MMC_FILE_EXIT_OPERATE(MMC_FILE_SEEK);

    return retval;
}

INT32 mmc_aud_FileRead(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{
    INT32 readlen = 0;

    MMC_FILE_ENTER_OPERATE(MMC_FILE_READ);
    MCI_TRACE(MCI_AUDIO_TRC|TSTDOUT,0,"[MCI] mmc_aud_FileRead: pBuf=0x%x, iLen=0x%x\n, mmc_file_handler:0x%x", pBuf, iLen, mmc_file_handler);

    if(mmc_file_handler)
    {
        readlen = mmc_file_handler->FileRead(fd,pBuf,iLen);
    }
#ifdef SERVICE_BASE_FS_SUPPORT
    else
    {
#ifndef CODEC_SUPPOT
    g_audioIsReadingFIle = TRUE;
#endif

        readlen = FS_Read(fd, pBuf, iLen);

#ifndef CODEC_SUPPOT
    g_audioIsReadingFIle = FALSE;
#endif
    }
#endif
    MMC_FILE_EXIT_OPERATE(MMC_FILE_READ);

    return readlen;
}

void mmc_aud_SetupFileHandle(
    INT32 (*GetFileSize) (INT32),
    INT32 (*FileSeek)(INT32, INT32, UINT8),
    INT32 (*FileRead)(INT32, UINT8 *, UINT32)
)
{
    if(NULL == mmc_file_handler)
        mmc_file_handler = (Audio_FileHandler *)COS_MALLOC(sizeof(Audio_FileHandler));

    mmc_file_handler->GetFileSize = GetFileSize;
    mmc_file_handler->FileSeek = FileSeek;
    mmc_file_handler->FileRead = FileRead;

}


void mmc_aud_ClearFileHandle(void)
{
    if(NULL != mmc_file_handler && g_playstream == NULL)
    {
        COS_FREE(mmc_file_handler);
        mmc_file_handler = NULL;
    }

}

