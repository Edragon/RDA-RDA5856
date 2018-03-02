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

#include "mmc_audioprefilter.h"
#include "mcip_debug.h"
#include "mci.h"
#include "mmc.h"

#define DEMUX_PAYLOAD_START 1
#define IS_ATDS(p)    ((p)[0] == 0xff && ((p)[1] &0xf0) == 0xf0)
#define IS_SYNC(p)    ((p)[0] == 0xff && ((p)[1] &0xE0) == 0xE0)
#define IS_ADIF(p)    ((p)[0] == 'A' && (p)[1] == 'D' && (p)[2] == 'I' && (p)[3] == 'F')
#define BEWORD(a,b,c,d) ((d) | ((c) << 8) | ((b)<<16) | ((unsigned)(a) << 24))
#define ID3v2_DEFAULT_MAGIC "ID3"
#define ID3v2_HEADER_SIZE 10

#ifdef WIN32
#define MCI_TRACE(level, tsmap, fmt, ...)
#endif

INT32 MMC_Mpeg2tsDemux(UINT8 * buf, UINT8 * end, UINT8 * outbuf,UINT32 *outlen,INT32 flags,INT32 demux_pid)
{
    static int mpeg1_skip_table[16] = {
    0, 0, 4, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

#define DEMUX_HEADER 0
#define DEMUX_DATA 1
#define DEMUX_SKIP 2
    static int state = DEMUX_SKIP;
    static int state_bytes = 0;
    static UINT8 head_buf[264];
    static int demux_track = 0xc0;
    UINT8 * header;
    int bytes;
    int len;

#define NEEDBYTES(x)                        \
    do {                            \
    int missing;                        \
                                \
    missing = (x) - bytes;                  \
    if (missing > 0) {                  \
        if (header == head_buf) {               \
        if (missing <= end - buf) {         \
            memcpy (header + bytes, buf, missing);  \
            buf += missing;             \
            bytes = (x);                \
        } else {                    \
            memcpy (header + bytes, buf, end - buf);    \
            state_bytes = bytes + end - buf;        \
            return 0;                   \
        }                       \
        } else {                        \
        memcpy (head_buf, header, bytes);       \
        state = DEMUX_HEADER;               \
        state_bytes = bytes;                \
        return 0;                   \
        }                           \
    }                           \
    } while (0)

#define DONEBYTES(x)        \
    do {            \
    if (header != head_buf) \
        buf = header + (x); \
    } while (0)

    if (flags & DEMUX_PAYLOAD_START)
    goto payload_start;

    switch (state) {
    case DEMUX_HEADER:
    if (state_bytes > 0) {
        header = head_buf;
        bytes = state_bytes;
        goto continue_header;
    }
    break;
    case DEMUX_DATA:
    if (demux_pid || (state_bytes > end - buf)) {

        memcpy (outbuf,buf, end - buf);
        *outlen += (end - buf);
        outbuf += (end - buf);
        state_bytes -= end - buf;
        return 0;
    }
    memcpy (outbuf,buf, state_bytes);
    *outlen += state_bytes;

    outbuf += state_bytes;
    buf += state_bytes;
    break;
    case DEMUX_SKIP:
    if (demux_pid || (state_bytes > end - buf)) {
        state_bytes -= end - buf;
        return 0;
    }
    buf += state_bytes;
    break;
    }

    while (1) {
    if (demux_pid) {
        state = DEMUX_SKIP;
        return 0;
    }
    payload_start:
    header = buf;
    bytes = end - buf;
    continue_header:
    NEEDBYTES (4);
    if (header[0] || header[1] || (header[2] != 1)) {
        if (demux_pid) {
            state = DEMUX_SKIP;
            return 0;
        }
        else if (header != head_buf)
        {
            buf++;
            goto payload_start;
        }
        else
        {
            header[0] = header[1];
            header[1] = header[2];
            header[2] = header[3];
            bytes = 3;
            goto continue_header;
        }
    }
    if (demux_pid) {
        if ((header[3] >= 0xc0) && (header[3] <= 0xef))
            goto pes;

        MCI_TRACE (MCI_AUDIO_TRC,0,"bad stream id %x\n", header[3]);
        return -1;
    }
    switch (header[3]) {
    case 0xb9:  /* program end code */
        /* DONEBYTES (4); */
        /* break;         */
        return 1;
    case 0xba:  /* pack header */
        NEEDBYTES (5);
        if ((header[4] & 0xc0) == 0x40) {   /* mpeg2 */
        NEEDBYTES (14);
        len = 14 + (header[13] & 7);
        NEEDBYTES (len);
        DONEBYTES (len);
        /* header points to the mpeg2 pack header */
        } else if ((header[4] & 0xf0) == 0x20) {    /* mpeg1 */
        NEEDBYTES (12);
        DONEBYTES (12);
        /* header points to the mpeg1 pack header */
        } else {

        MCI_TRACE (MCI_AUDIO_TRC,0,"weird pack header\n");

        DONEBYTES (5);
        }
        break;
    default:
        if (header[3] == demux_track) {
            pes:
            NEEDBYTES (7);
            if ((header[6] & 0xc0) == 0x80) {   /* mpeg2 */
                NEEDBYTES (9);
                len = 9 + header[8];
                NEEDBYTES (len);
                /* header points to the mpeg2 pes header */
            }
            else
            {   /* mpeg1 */
                len = 7;
                while ((header-1)[len] == 0xff) {
                    len++;
                    NEEDBYTES (len);
                    if (len > 23) {

                        MCI_TRACE (MCI_AUDIO_TRC,0,"too much stuffing\n");
                        break;
                    }
                }
                if (((header-1)[len] & 0xc0) == 0x40) {
                    len += 2;
                    NEEDBYTES (len);
                }
                len += mpeg1_skip_table[(header - 1)[len] >> 4];
                NEEDBYTES (len);
                /* header points to the mpeg1 pes header */
            }
            DONEBYTES (len);
            bytes = 6 + (header[4] << 8) + header[5] - len;
            if (demux_pid || (bytes > end - buf)) {

                memcpy(outbuf,buf, end - buf);
                *outlen += (end - buf);
                outbuf += (end - buf);
                state = DEMUX_DATA;
                state_bytes = bytes - (end - buf);
                return 0;
            } else if (bytes <= 0)
                continue;

            memcpy(outbuf,buf, end - buf);
            *outlen += (end - buf);

            outbuf += (end - buf);

            buf += bytes;
        }
        else if (header[3] < 0xb9)
        {

            MCI_TRACE (MCI_AUDIO_TRC,0,"looks like a video stream, not system stream\n");

            DONEBYTES (4);
        }
        else
        {
            NEEDBYTES (6);
            DONEBYTES (6);
            bytes = (header[4] << 8) + header[5];
            if (bytes > end - buf) {
                state = DEMUX_SKIP;
                state_bytes = bytes - (end - buf);
                return 0;
            }
            buf += bytes;
        }
    }
    }
}

static int mpeg_mpa_check_header(unsigned int header){
    /* header */
    if ((header & 0xffe00000) != 0xffe00000)
        return -1;

    /*version check */
    if (((header >> 19) & 3 ) == 0x01) {
        return FALSE;
    }
    /* layer check */
    if ((header & (3<<17)) == 0 || (header & (3<<17)) == (3<<17))
        return -1;
    /* bit rate */
    if (((header & (0xf<<12)) == 0xf<<12) ||((header & (0xf<<12)) == 0))
        return -1;
    /* frequency */
    if ((header & (3<<10)) == 3<<10)
        return -1;
    return 0;
}

static int mpeg_adts_header_check(unsigned int header) {


    if((header & 0xfff60000) != 0xfff00000)
        return -1 ;
    if (((header >> 17) & 0x3) != 0)
        return -1;

    if (((header >>14) & 0x3) != 1)
        return -1;

    if (((header >> 10) & 0xf) >= 12)
        return -1;

    if (((header >> 6) & 0x7) > 2 )
        return -1;


    return 0;

}

static int mpeg_id3v2_match(const unsigned char *buf, const char * magic)
{
    return  buf[0]         == magic[0] &&
        buf[1]         == magic[1] &&
        buf[2]         == magic[2] &&
        buf[3]         != 0xff &&
        buf[4]         != 0xff &&
        (buf[6] & 0x80) ==    0 &&
        (buf[7] & 0x80) ==    0 &&
        (buf[8] & 0x80) ==    0 &&
        (buf[9] & 0x80) ==    0;
}


static int mpeg_id3v2_tag_len(const unsigned char * buf)
{
    int len = ((buf[6] & 0x7f) << 21) +
        ((buf[7] & 0x7f) << 14) +
        ((buf[8] & 0x7f) << 7) +
        (buf[9] & 0x7f) +
        ID3v2_HEADER_SIZE;
    if (buf[5] & 0x10)
        len += ID3v2_HEADER_SIZE;
    return len;
}

INT32 mpeg_stream_check(INT32 *filetype,UINT8 *buffer,UINT32 checklength)
{


    UINT8 *buf0,*end,*nextbuf;
    int fileoffset = 0,result = 0;
    UINT32 header,pid,demux_pid =0x1100;

    buf0=buffer;
    end = buffer+checklength-10;
    if (*filetype == MCI_TYPE_WAV ||*filetype == MCI_TYPE_WMA)
        return 0;

    while (buf0 <= end) {

        if (buf0 >= end)
        {
            return fileoffset = checklength;
        }

        if (*buf0) {

            if (*filetype == MCI_TYPE_TS)
            {
                for (; (nextbuf = buf0 + MPEG2TSPACKETSIZE) <= (end +10); buf0 = nextbuf) {
                    if (*buf0 != 0x47) {

                        MCI_TRACE(MCI_AUDIO_TRC,0,"bad sync byte\n");
                        nextbuf = buf0 + 1;
                        continue;
                    }
                    pid = ((buf0[1] << 8) + buf0[2]) & 0x1fff;
                    if (pid != demux_pid)
                        continue;
                    else
                       break;
                }

                return ((UINT32)buf0 - (UINT32)buffer);

            }
            else if (IS_ATDS(buf0) || IS_SYNC(buf0) || IS_ADIF(buf0))
            {

                header = BEWORD(buf0[0], buf0[1], buf0[2], buf0[3]);


                if (*filetype == MMC_AAC_DECODE || *filetype == MCI_TYPE_AAC)
                {
                    if (!IS_ADIF(buf0))
                    {
                        result = mpeg_adts_header_check(header);
                    }

                }
                else if (*filetype == MMC_MP3_DECODE ||*filetype == MCI_TYPE_DAF)
                {

                    result = mpeg_mpa_check_header(header);

                }
                else if (*filetype == MMC_MPEG_NONE)
                {

                    if ((result = mpeg_adts_header_check(header)) == 0)
                    {
                        *filetype = MMC_AAC_DECODE;

                    }
                    else if ((result = mpeg_mpa_check_header(header)) == 0)
                    {


                          *filetype = MMC_MP3_DECODE;

                    }
                }

                if (result == 0)
                {

                    fileoffset = ((UINT32)buf0 - (UINT32)buffer);
                    return fileoffset;
                }

            }
            else if(mpeg_id3v2_match(buf0,ID3v2_DEFAULT_MAGIC))
            {

                fileoffset = ((UINT32)buf0 - (UINT32)buffer);
                fileoffset += mpeg_id3v2_tag_len(buf0);
                buf0 =buffer + fileoffset;
                continue;
            }

        }
        buf0++;
    }
    return 0;
}


