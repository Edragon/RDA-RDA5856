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

/*
================================================================================
  File         sxs_rmt.c
--------------------------------------------------------------------------------

  Scope      : Data exchange with remote computer.

  History    :
--------------------------------------------------------------------------------
 Jun 13 2003 |  ADA  | Creation
 Oct 20 2005 |  ADA  | Track lost data.
 Dec 15 2005 |  ADA  | Handle DbgBuffer in boot structure.
 Jan 05 2006 |  JFN  | Handle DataLost flag properly.
 Jan 27 2006 |  ADA  | sxs_RmtInit: Use function sxs_InitRmc to send the flow INIT_RMC.
 Jun 29 2006 |  ADA  | - sxs_GetRmtBuffer: time out in case there is no remote PC
             |       | connected.
             |       | - sxs_SendToRemote: frame of size 0 doesn't provoke overflow
             |       | notification.
================================================================================
*/

#include "sxr_ops.h"
#include "sxs_rmc.h"
#include "sxs_rmt.h"
#include "sxs_rmt.hp"
#include "sxs_lib.h"
#include "sxs_io.h"
#include "sxr_cpu.h"
#include "sxs_srl.h"

#ifndef __TARGET__
#include "sxs_udp.h"
#endif

// Trace the number of lost msgs when tracing time stamp
#define SXS_RMT_TRACE_DATA_LOST_NUM
#define SXS_RMT_DATA_LOST_NUM_MAX 0xFF

// =============================================================================
// sxs_RmtInit
// -----------------------------------------------------------------------------
/// Data exchange with remote computer initialization.
// =============================================================================
void sxs_RmtInit (void)
{
    u32 i;

    for (i=0; i<SXS_RMT_NB_BUF; i++)
    {
        sxs_PrcsRmtDataArray [i] = sxs_PrcsRmtData;
    }

    for (i=0; i<SXS_RMT_NB_BUF; i++)
    {
        memset (&sxs_Rmt [i], 0, sizeof (sxs_Rmt) / SXS_RMT_NB_BUF);
#ifdef __SXS_RMT_PROCESS_IDLE__
        sxs_Rmt [i].RmtQueue = sxr_NewQueue ("Rmt.");
#endif
        //_sxs_SendToRemote (SXS_INIT_RMC, NIL, 4, (u8)i);
        sxs_InitRmc ();
    }
}


// =============================================================================
// sxs_SetPrcsRmtData
// -----------------------------------------------------------------------------
/// Set the function to be called to process the data coming from
/// the specified remote processor.
/// @param RmtCpuIdx            Index of the remote processor.
/// @param sxs_PrcsRmtDataFunc  Function for data process.
// =============================================================================
void sxs_SetPrcsRmtData (u8 RmtCpuIdx, void (*sxs_PrcsRmtDataFunc)(u8, u8*, u16))
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    sxs_PrcsRmtDataArray [RmtCpuIdx] = sxs_PrcsRmtDataFunc;
}



// =============================================================================
// sxs_RmtBufferFlush
// -----------------------------------------------------------------------------
/// In simulation, flush the remote buffer.
/// @return TRUE when the buffer is not empty, FALSE otherwise.
// =============================================================================
#define NO__RMT_TEST_MODE__

#ifndef __EMBEDDED__
u8 sxs_RmtBufferFlush (void)
{
#ifdef __RMT_TEST_MODE__
#define SXS_FLUSH_BUFFER_SIZE 512
#define SXS_FLUSH_BUFFER_MSK  (SXS_FLUSH_BUFFER_SIZE - 1)

    static u8 Buffer [SXS_FLUSH_BUFFER_SIZE];
    static u16 WIdx = 0;
    static u16 RIdx = 0;
    static u8 DoNotEnter = FALSE;

    u8  RetValue = FALSE;
    u16 Size;
    u8  RmtCpuIdx;

    if (DoNotEnter)
    {
        return FALSE;
    }

    DoNotEnter = TRUE;

    for (RmtCpuIdx = 0; RmtCpuIdx < SXS_RMT_NB_BUF; RmtCpuIdx++)
    {
        do
        {
            Size = (RIdx - WIdx -1) & SXS_FLUSH_BUFFER_MSK;

            if (WIdx + Size > SXS_FLUSH_BUFFER_SIZE)
            {
                Size = SXS_FLUSH_BUFFER_SIZE - WIdx;
            }

            Size = sxs_ReadTxRmtData (&Buffer [WIdx], Size, RmtCpuIdx);

            RetValue = Size != 0;

            WIdx = (WIdx + Size) & SXS_FLUSH_BUFFER_MSK;

            Size = (WIdx - RIdx) & SXS_FLUSH_BUFFER_MSK;

            if (RIdx + Size > SXS_FLUSH_BUFFER_SIZE)
            {
                Size = SXS_FLUSH_BUFFER_SIZE - RIdx;
            }

            //Size = sxs_WriteRxRmtData (&Buffer [RIdx], Size, RmtCpuIdx);
            Size = sxs_WriteRxRmtData (&Buffer [RIdx], Size, 1);

            RIdx = (RIdx + Size) & SXS_FLUSH_BUFFER_MSK;

        } while (RetValue);
    }

    DoNotEnter = FALSE;

    sxs_UdpIdleHook ();

    return RetValue;
#else
    return sxs_SerialFlush ();
#endif
}
#endif

// =============================================================================
// sxs_SendToRemote
// -----------------------------------------------------------------------------
/// Format data corresponding to the identified flow and store
/// them in remote buffer.
/// @param Id        Flow Id.
/// @param Data      Pointer onto data or NIL.
/// @param Size      Size of data.
/// @param RmtCpuIdx Remote CPU index.
/// @return          Pointer where data can be stored.
// =============================================================================
u8 *_sxs_SendToRemote (u8 Id, u8 *Data, u16 Size, u8 RmtCpuIdx)
{
    u16 ReqSize, FreeSize, FreeSizeToBufferEnd;
    u16 Length;
    u16 WIdx;
    u32 i;
    u32 Status;

#ifndef __EMBEDDED__
    sxs_RmtBufferFlush ();
#endif

    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    // The frame structure in the remote buffer is the following.
    // SXS_RMT_START_FRM
    // Frame Length Msb
    // Frame Length Lsb
    // Flow Id
    // Data 1
    //   .
    //   .
    //   .
    // Data n
    // Next Frame offset.

    ReqSize = (Size + SXS_RMT_CASING_SIZE + 3) & ~((u16)3);

    if (Size == 0)
    {
        return NIL;
    }

    Status = sxr_EnterSc ();

    if ( (ReqSize > SXS_RMT_BUF_SIZE)
    ||   (sxs_Rmt [RmtCpuIdx].Suspended == TRUE) )
    {
#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
        if (sxs_Rmt [RmtCpuIdx].DataLost < SXS_RMT_DATA_LOST_NUM_MAX)
        {
            sxs_Rmt [RmtCpuIdx].DataLost++;
        }
#else
        sxs_Rmt [RmtCpuIdx].DataLost = TRUE;
#endif
        sxr_ExitSc (Status);
        return NIL;
    }

    if (!sxs_Rmt [RmtCpuIdx].StampStarted)
    {
#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
        if ( (sxs_Rmt [RmtCpuIdx].DataLost > 0)
        ||   ( (sxs_Rmt [RmtCpuIdx].FnStamp != NIL)
          &&   ( (sxs_Rmt [RmtCpuIdx].Fn & ~(0xff<<24)) !=
                 (*sxs_Rmt [RmtCpuIdx].FnStamp & ~(0xff<<24)) )
          &&   (Id == SXS_RMC_YIELD)
             )
           )
#else
        if ( (sxs_Rmt [RmtCpuIdx].DataLost == TRUE)
        ||   ( (sxs_Rmt [RmtCpuIdx].FnStamp != NIL)
          &&   ( (sxs_Rmt [RmtCpuIdx].Fn & ~(1<<31)) !=
                 (*sxs_Rmt [RmtCpuIdx].FnStamp & ~(1<<31)) )
          &&   (Id == SXS_RMC_YIELD)
             )
           )
#endif
        {
            sxs_Rmt [RmtCpuIdx].Fn = (sxs_Rmt [RmtCpuIdx].FnStamp != NIL) ?
                            *sxs_Rmt [RmtCpuIdx].FnStamp :
                            0;

#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
            sxs_Rmt [RmtCpuIdx].Fn &= ~(0xff<<24);
            if (sxs_Rmt [RmtCpuIdx].DataLost > 0)
            {
                sxs_Rmt [RmtCpuIdx].Fn |= (sxs_Rmt [RmtCpuIdx].DataLost << 24);
            }
#else
            if (sxs_Rmt [RmtCpuIdx].DataLost == TRUE)
            {
                sxs_Rmt [RmtCpuIdx].Fn |= (1 << 31);
            }
#endif

            sxs_Rmt [RmtCpuIdx].StampStarted = TRUE;
            _sxs_SendToRemote (SXS_TIME_STAMP_RMC, (u8 *)&sxs_Rmt [RmtCpuIdx].Fn, 4, RmtCpuIdx);
            sxs_Rmt [RmtCpuIdx].StampStarted = FALSE;
        }
    }

    WIdx = sxs_Rmt [RmtCpuIdx].WIdx;

    FreeSize = (sxs_Rmt [RmtCpuIdx].RIdx - WIdx - 1) & SXS_RMT_BUF_MOD;

    if ( ( (!(Id & SXS_RMC_YIELD)) && (FreeSize < (SXS_RMT_BUF_SIZE / 2)) )
    ||  (FreeSize < ReqSize))
    {
#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
        if ( (Id & SXS_RMC_YIELD) != 0
        &&   sxs_Rmt [RmtCpuIdx].DataLost < SXS_RMT_DATA_LOST_NUM_MAX )
        {
            sxs_Rmt [RmtCpuIdx].DataLost++;
        }
#else
        sxs_Rmt [RmtCpuIdx].DataLost = ((Id & SXS_RMC_YIELD) != 0)||sxs_Rmt [RmtCpuIdx].DataLost;
#endif
        sxr_ExitSc (Status);
        return NIL;
    }

    if (Data == NIL)
    {
        FreeSizeToBufferEnd = SXS_RMT_BUF_SIZE - WIdx;

        if (FreeSizeToBufferEnd < ReqSize)
        {
            /* Jump on buffer top necessary. */

            /* If the buffer is empty, we will start from the beginning.
             * Otherwise, test that the empty space at the beginning of the
             * buffer is big enough to fit. */
            if ((FreeSize - FreeSizeToBufferEnd) < ReqSize && (sxs_Rmt [RmtCpuIdx].RIdx != WIdx))
            {
#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
                if ( (Id & SXS_RMC_YIELD) != 0
                &&   sxs_Rmt [RmtCpuIdx].DataLost < SXS_RMT_DATA_LOST_NUM_MAX )
                {
                    sxs_Rmt [RmtCpuIdx].DataLost++;
                }
#else
                sxs_Rmt [RmtCpuIdx].DataLost = (Id & SXS_RMC_YIELD) != 0;
#endif
                sxr_ExitSc (Status);
                return NIL;
            }

            if (sxs_Rmt [RmtCpuIdx].RIdx == WIdx)
            {
                sxs_Rmt [RmtCpuIdx].RIdx = 0;
                FreeSize = SXS_RMT_BUF_SIZE;
            }
            else
            {
                FreeSize = sxs_Rmt [RmtCpuIdx].RIdx - 1;
            }

            sxs_Rmt [RmtCpuIdx].Buf [sxs_Rmt [RmtCpuIdx].NextIdx & SXS_RMT_BUF_MOD] = 0;
            WIdx = 0;
        }
    }

    if (Id == SXS_TIME_STAMP_RMC)
    {
#ifdef SXS_RMT_TRACE_DATA_LOST_NUM
        sxs_Rmt [RmtCpuIdx].DataLost = 0;
#else
        sxs_Rmt [RmtCpuIdx].DataLost = FALSE;
#endif
    }

    Length = Size + 1;   /* One byte Id comes on top. */

    sxs_Rmt [RmtCpuIdx].Buf [WIdx & SXS_RMT_BUF_MOD] = SXS_RMT_START_FRM;
    WIdx = (WIdx + 1) & SXS_RMT_BUF_MOD;

    sxs_Rmt [RmtCpuIdx].Buf [WIdx] = Length >> 8;
    WIdx = (WIdx + 1) & SXS_RMT_BUF_MOD;

    sxs_Rmt [RmtCpuIdx].Buf [WIdx] = (u8) Length;
    WIdx = (WIdx + 1) & SXS_RMT_BUF_MOD;

    sxs_Rmt [RmtCpuIdx].Buf [WIdx] = Id;
    WIdx = (WIdx + 1) & SXS_RMT_BUF_MOD;

    sxs_Rmt [RmtCpuIdx].NextIdx = (WIdx + Size) & SXS_RMT_BUF_MOD;
    sxs_Rmt [RmtCpuIdx].WIdx    = ((sxs_Rmt [RmtCpuIdx].NextIdx + 3) & ~((u16)3)) & SXS_RMT_BUF_MOD;

    if (sxs_Rmt [RmtCpuIdx].NextIdx == sxs_Rmt [RmtCpuIdx].WIdx)
    {
        sxs_Rmt [RmtCpuIdx].WIdx += 4;
    }

    sxs_Rmt [RmtCpuIdx].Buf [sxs_Rmt [RmtCpuIdx].NextIdx] = ((sxs_Rmt [RmtCpuIdx].WIdx - sxs_Rmt [RmtCpuIdx].NextIdx - 1) & 3) + 1;

    sxr_ExitSc (Status);

    if (Data == NIL)
    {
        return &sxs_Rmt [RmtCpuIdx].Buf [WIdx& SXS_RMT_BUF_MOD];
    }

    for (i=0; i<Size; i++)
    {
        sxs_Rmt [RmtCpuIdx].Buf [(i+WIdx) & SXS_RMT_BUF_MOD] = Data [i];
    }

#ifndef __EMBEDDED__
    sxs_RmtBufferFlush ();
#endif

    return NIL;
}

// =============================================================================
// sxs_ReadTxRmtData
// -----------------------------------------------------------------------------
/// Read a bunch of data from the debug remote buffer.
/// @param DataOut   points a buffer where data must be copied.
/// @param Size      Size in bytes of buffer pointed by DataOut.
/// @param RmtCpuIdx Remote CPU index.
/// @return          Number of bytes copied in buffer.
// =============================================================================
u16 sxs_ReadTxRmtData (u8 *DataOut, u16 Size, u8 RmtCpuIdx)
{
    static struct
    {
        u8  CheckSum;
        u8  HeaderCnt;
        u8  CiphKey;
        u16 Length;
    } sxs_ReadTxRmt [SXS_RMT_NB_BUF];

    u16 RIdx, SizeAvailable;
    u16 DataSize = 0;
    u16 i, Index = 0;

    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    RIdx = sxs_Rmt [RmtCpuIdx].RIdx;
    SizeAvailable = (sxs_Rmt [RmtCpuIdx].WIdx - sxs_Rmt [RmtCpuIdx].RIdx) & SXS_RMT_BUF_MOD;

    if ( (SizeAvailable == 0)
    ||   (Size == 0) )
    {
        return 0;
    }

    if (sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt < 3)
    {
        u32 HdLp = MIN (Size, SizeAvailable);
        HdLp = MIN (HdLp, (u32)(3 - sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt));

        for (i=0; i< HdLp; i++)
        {
            switch (sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt)
            {
                case 0:

                    if (sxs_Rmt [RmtCpuIdx].Buf [sxs_Rmt [RmtCpuIdx].RIdx] != SXS_RMT_START_FRM)
                    {
                        sxs_Rmt [RmtCpuIdx].RIdx = (sxs_Rmt [RmtCpuIdx].RIdx + 1) & SXS_RMT_BUF_MOD;
                        return 0;
                    }
                    DataOut [0] = SXS_RMT_START_FRM;
                    break;

                case 1:
                    sxs_ReadTxRmt [RmtCpuIdx].Length = (sxs_Rmt [RmtCpuIdx].Buf [RIdx] << 8);
                    break;

                case 2:
                    sxs_ReadTxRmt [RmtCpuIdx].CiphKey = SXS_RMT_START_FRM; // sxs_Rmt [RmtCpuIdx].Buf [RIdx];
                    sxs_ReadTxRmt [RmtCpuIdx].Length |= sxs_Rmt [RmtCpuIdx].Buf [RIdx];
                    sxs_ReadTxRmt [RmtCpuIdx].Length += 1; /* Add one byte for Check sum. */
                    break;
            }

            DataOut [Index++] = sxs_Rmt [RmtCpuIdx].Buf [RIdx];
            RIdx = (RIdx + 1) & SXS_RMT_BUF_MOD;
            sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt++;
        }
    }

    DataSize = MIN (Size - Index, sxs_ReadTxRmt [RmtCpuIdx].Length);

    if (DataSize > SizeAvailable)
    {
        DataSize = SizeAvailable;
    }

    for (i=0; i<DataSize; i++)
    {
        if (sxs_ReadTxRmt [RmtCpuIdx].Length != 1)
        {
#ifdef __SXS_RMT_CIPH__
            DataOut [Index] = sxs_Rmt [RmtCpuIdx].Buf [RIdx] ^ sxs_ReadTxRmt [RmtCpuIdx].CiphKey;
#else
            DataOut [Index] = sxs_Rmt [RmtCpuIdx].Buf [RIdx];
#endif
            sxs_ReadTxRmt [RmtCpuIdx].CheckSum ^= DataOut [Index];
            RIdx = (RIdx + 1) & SXS_RMT_BUF_MOD;
        }
        else // if (sxs_ReadTxRmt [RmtCpuIdx].Length == 0)
        {
            DataOut [Index] = sxs_ReadTxRmt [RmtCpuIdx].CheckSum;
        }

        sxs_ReadTxRmt [RmtCpuIdx].Length--;

        Index++;
    }

    sxs_Rmt [RmtCpuIdx].RIdx = RIdx;

    if ( (sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt == 3)
    &&   (sxs_ReadTxRmt [RmtCpuIdx].Length == 0) )
    {
        u32 Status = sxr_EnterSc ();

        if (sxs_Rmt [RmtCpuIdx].Buf [sxs_Rmt [RmtCpuIdx].RIdx] == 0)
        {
            sxs_Rmt [RmtCpuIdx].RIdx = 0;
        }
        else
        {
            sxs_Rmt [RmtCpuIdx].RIdx = (sxs_Rmt [RmtCpuIdx].RIdx + sxs_Rmt [RmtCpuIdx].Buf [sxs_Rmt [RmtCpuIdx].RIdx]) & SXS_RMT_BUF_MOD;
        }

        sxs_ReadTxRmt [RmtCpuIdx].HeaderCnt = 0;
        sxs_ReadTxRmt [RmtCpuIdx].CheckSum  = 0;

        sxr_ExitSc (Status);
    }

    return Index;
}

// =============================================================================
// sxs_WriteRxRmtData
// -----------------------------------------------------------------------------
/// Extract and process a frame coming from the remote computer
/// from the reception buffer.
/// @param DataIn    points to the remote computer Rx data.
/// @param Size      is the number of bytes available in DataIn.
/// @param RmtCpuIdx Remote CPU index.
/// @return          The number of bytes processed in DataIn.
// =============================================================================
u16 sxs_WriteRxRmtData (u8 *DataIn, u16 Size, u8 RmtCpuIdx)
{
    u32 i;
    u16 Index      = 0;
    u16 DataSize   = 0;

    static struct
    {
        u8 *Data;
        u16 Length;
        u16 Cnt;
        u8  HeaderCnt;
        u8  Id;
        u8  CiphKey;
        u8  CheckSum;
    } sxs_WriteRxRmt [SXS_RMT_NB_BUF];

    if (Size == 0)
    {
        return 0;
    }

    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    if (sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt < 4)
    {
        u32 HdLp = MIN (Size, 4 - sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt);

        for (i=0; i<HdLp; i++)
        {
            switch (sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt)
            {
                case 0:
                    if (DataIn [Index] != SXS_RMT_START_FRM)
                    {
                        return 1;
                    }
                    break;

                case 1:
                    sxs_WriteRxRmt [RmtCpuIdx].Length  = DataIn [Index] << 8;
                    break;

                case 2:
                    sxs_WriteRxRmt [RmtCpuIdx].Length |= DataIn [Index];
                    sxs_WriteRxRmt [RmtCpuIdx].CiphKey = SXS_RMT_START_FRM; //DataIn [Index];

#ifdef __TARGET__
                    if (sxs_WriteRxRmt [RmtCpuIdx].Length < 4000)
#endif
                    {
                        sxs_WriteRxRmt [RmtCpuIdx].Data = (u8 *)sxr_HMalloc ((u16)(sxs_WriteRxRmt [RmtCpuIdx].Length + sizeof (sxs_RmtHeader_t)));
                    }

                    if (sxs_WriteRxRmt [RmtCpuIdx].Data == NIL)
                    {
                        SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(1),TSTR("Rejected frame, size %i\n",0x06be0001), sxs_WriteRxRmt [RmtCpuIdx].Length);
                        sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt = 0;
                        return Size;    /* No more memory, frame is lost. */
                    }
                    break;

                case 3:
#ifdef __SXS_RMT_CIPH__
                    sxs_WriteRxRmt [RmtCpuIdx].Id = DataIn [Index] ^ sxs_WriteRxRmt [RmtCpuIdx].CiphKey;
#else
                    sxs_WriteRxRmt [RmtCpuIdx].Id = DataIn [Index];
#endif
                    sxs_WriteRxRmt [RmtCpuIdx].CheckSum ^= DataIn [Index];
                    break;

            }
            sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt++;
            Index++;
        }
    }

    DataSize = MIN (Size - Index, sxs_WriteRxRmt [RmtCpuIdx].Length - sxs_WriteRxRmt [RmtCpuIdx].Cnt);

    for (i=0; i<DataSize; i++)
    {
#ifdef __SXS_RMT_CIPH__
        sxs_WriteRxRmt [RmtCpuIdx].Data [sizeof (sxs_RmtHeader_t) + sxs_WriteRxRmt [RmtCpuIdx].Cnt++] = DataIn [Index] ^ sxs_WriteRxRmt [RmtCpuIdx].CiphKey;
#else
        sxs_WriteRxRmt [RmtCpuIdx].Data [sizeof (sxs_RmtHeader_t) + sxs_WriteRxRmt [RmtCpuIdx].Cnt++] = DataIn [Index];
#endif
        sxs_WriteRxRmt [RmtCpuIdx].CheckSum ^= DataIn [Index];
        Index++;
    }

    if ( (sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt == 4)
    &&   (sxs_WriteRxRmt [RmtCpuIdx].Length <= sxs_WriteRxRmt [RmtCpuIdx].Cnt) )
    {
        u8 UsrPrcsDone = FALSE;

        /* Original length includes Id, not check sum. Here it must be understood        */
        /* as length of data + 1 byte checksum. Checksum is the last byte of the buffer. */

        if (sxs_WriteRxRmt [RmtCpuIdx].CheckSum != 0)
        {
            SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(3),TSTR("Check sum error, flow 0x%x Size %i (0x%x)\n",0x06be0002), sxs_WriteRxRmt [RmtCpuIdx].Id, sxs_WriteRxRmt [RmtCpuIdx].Length, &sxs_WriteRxRmt);
        }
        else
        {
#ifndef __SXS_RMT_PROCESS_IDLE__
            if (sxs_Rmt [RmtCpuIdx].UserPrcsRmtData != NIL)
            {
                if (sxs_Rmt [RmtCpuIdx].UserPrcsRmtData (sxs_WriteRxRmt [RmtCpuIdx].Id, &sxs_WriteRxRmt [RmtCpuIdx].Data [sizeof (sxs_RmtHeader_t)], sxs_WriteRxRmt [RmtCpuIdx].Length))
                {
                    UsrPrcsDone = TRUE;
                }
            }

            // When there is trace buffer to recover during startup (sxs_GetRmtBuffer), target will run to here during startup.
            // At the same time, if remote computer sends any command (e.g., trace levels) to target, crash will happen, as
            // sxs_PrcsRmtDataArray[] has NOT been initialized yet (sxs_RmtInit),
            if (!UsrPrcsDone && sxs_PrcsRmtDataArray[RmtCpuIdx])
            {
                sxs_PrcsRmtDataArray [RmtCpuIdx] (sxs_WriteRxRmt [RmtCpuIdx].Id, &sxs_WriteRxRmt [RmtCpuIdx].Data [sizeof (sxs_RmtHeader_t)], sxs_WriteRxRmt [RmtCpuIdx].Length);
            }

#else
            {
                sxs_RmtHeader_t *sxs_RmtHeader = (sxs_RmtHeader_t *)sxs_WriteRxRmt [RmtCpuIdx].Data;
                sxs_RmtHeader -> Id = sxs_WriteRxRmt [RmtCpuIdx].Id;
                sxs_RmtHeader -> Length = sxs_WriteRxRmt [RmtCpuIdx].Length;
                sxr_PopIn(sxs_WriteRxRmt [RmtCpuIdx].Data, sxs_Rmt [RmtCpuIdx].RmtQueue);
            }
#endif

            //SXS_DUMP (_SXR, "%2x", sxs_WriteRxRmt [RmtCpuIdx].Data, sxs_WriteRxRmt [RmtCpuIdx].Length);
        }

#ifndef __SXS_RMT_PROCESS_IDLE__
        sxr_HFree (sxs_WriteRxRmt [RmtCpuIdx].Data);
#endif

        sxs_WriteRxRmt [RmtCpuIdx].HeaderCnt = FALSE;
        sxs_WriteRxRmt [RmtCpuIdx].Cnt       = 0;
        sxs_WriteRxRmt [RmtCpuIdx].CheckSum  = 0;
        sxs_WriteRxRmt [RmtCpuIdx].Data      = 0;
    }

    return Index;
}


// =============================================================================
// sxs_RmtIdleHook
// -----------------------------------------------------------------------------
/// Process remote data during idle.
// =============================================================================
void sxs_RmtIdleHook (void)
{
#ifdef __SXS_RMT_PROCESS_IDLE__
    u32 i;

    for (i=0; i<SXS_RMT_NB_BUF; i++)
    {
        sxs_RmtHeader_t *sxs_RmtHeader;

        //sxr_CheckQueue (sxs_Rmt [i].RmtQueue);

        while ((sxs_RmtHeader = (sxs_RmtHeader_t *)sxr_PopOut(sxs_Rmt [i].RmtQueue )) != NIL)
        {
            //sxs_PrcsRmtData (sxs_RmtHeader -> Id, ((u8 *)sxs_RmtHeader) + sizeof (sxs_RmtHeader_t), sxs_RmtHeader -> Length);
            sxs_PrcsRmtDataArray [i] (sxs_RmtHeader -> Id, ((u8 *)sxs_RmtHeader) + sizeof (sxs_RmtHeader_t), sxs_RmtHeader -> Length);
            sxr_HFree (sxs_RmtHeader);
        }
    }
#endif
#ifndef __TARGET__
    sxs_RmtBufferFlush ();
#endif


}


// =============================================================================
// sxs_RmtSuspend
// -----------------------------------------------------------------------------
/// Suspend trace storage remote buffer.
/// @param RmtCpuIdx Remote CPU index.
// =============================================================================
void sxs_RmtSuspend (u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    sxs_Rmt [RmtCpuIdx].Suspended = TRUE;
}


// =============================================================================
// sxs_RmtResume
// -----------------------------------------------------------------------------
/// Resume trace storage in remote buffer.
/// @param RmtCpuIdx Remote CPU index.
// =============================================================================
void sxs_RmtResume (u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    sxs_Rmt [RmtCpuIdx].Suspended = FALSE;
}

// =============================================================================
// sxs_GetRmtBufferStatus
// -----------------------------------------------------------------------------
/// Get the status on the remote buffer.
/// @param RmtCpuIdx Remote CPU index.
/// @return          Number of data present in remote buffer.
// =============================================================================
u16 sxs_GetRmtBufferStatus (u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    return (sxs_Rmt [RmtCpuIdx].RIdx - sxs_Rmt [RmtCpuIdx].WIdx) & SXS_RMT_BUF_MOD;
}


// =============================================================================
// sxs_SetFnStamp
// -----------------------------------------------------------------------------
/// Start trace time stamping.
/// @param FnStamp   Address of the frame number stamp.
/// @param RmtCpuIdx Remote CPU index.
// =============================================================================
void _sxs_SetFnStamp (u32 *FnStamp, u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    sxs_Rmt [RmtCpuIdx].FnStamp = FnStamp;
}


// =============================================================================
// sxs_GetFnStamp
// -----------------------------------------------------------------------------
/// Get current stamping value.
/// @param RmtCpuIdx Remote CPU index.
/// @return          Stamp value.
// =============================================================================
u32 _sxs_GetFnStamp (u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    return *sxs_Rmt [RmtCpuIdx].FnStamp;
}


// =============================================================================
// sxs_SetUserPrcsRmtData
// -----------------------------------------------------------------------------
/// Set the user function designed to process data from remote computer.
/// @param UserPrcsRmtData  User function.
/// @param RmtCpuIdx        Remote CPU index.
// =============================================================================
void sxs_SetUserPrcsRmtData (u8 (*UserPrcsRmtData) (u8, u8 *, u16), u8 RmtCpuIdx)
{
    if (SXS_RMT_NB_BUF == 1)
    {
        RmtCpuIdx = 0;
    }

    sxs_Rmt [RmtCpuIdx].UserPrcsRmtData = UserPrcsRmtData;
}


// =============================================================================
// sxs_SaveRmtBuffer
// -----------------------------------------------------------------------------
/// Save the remote buffer to the end of head to allow trace recovery after reset.
// =============================================================================
#ifdef __TARGET__
void sxs_SaveRmtBuffer (void)
{
    u8 *Address = (u8 *)sxr_GetHeapEnd (0);
    // Heaps are allocated only after OS is started
    if (NIL == Address)
    {
        return;
    }
    Address -= sizeof (sxs_Rmt) / SXS_RMT_NB_BUF;
    memcpy (Address, &sxs_Rmt [SXS_RMT_DBG_CPU_IDX], sizeof (sxs_Rmt) / SXS_RMT_NB_BUF);
    sxs_Boot -> RmtBufferSaved = TRUE;
}
#endif


// =============================================================================
// sxs_GetRmtBuffer
// -----------------------------------------------------------------------------
/// Restore saved remote buffer by sxs_SaveRmtBuffer() and place it in new trace
/// buffer.
// =============================================================================
#ifdef __TARGET__
void sxs_GetRmtBuffer (void)
{
    u32 TimeOut = SXS_RMT_BUF_SIZE * 20;

    if (sxs_Boot -> Pattern != SXS_BOOT_PATTERN)
    {
        sxs_Boot -> Pattern = SXS_BOOT_PATTERN;
        sxs_Boot -> ExceptionCount = 0;
        sxs_Boot -> DwlReq = FALSE;
        sxs_Boot -> ExceptionExpected = FALSE;
        sxs_Boot -> RmtBufferSaved = FALSE;

    }
    else
    {
        if (sxs_Boot -> RmtBufferSaved)
        {
            u8 *Address = (u8 *)sxr_GetHeapEnd (0);
            // Heaps are allocated only after OS is started
            if (NIL == Address)
            {
                return;
            }

            SXS_TRACE (_SXR|TSTDOUT|TDB,TSTR("--------- Trace buffer recovery. ----------\n",0x06be0003));
            while (sxs_SerialFlush ()&& (TimeOut--));

            Address -= sizeof (sxs_Rmt) / SXS_RMT_NB_BUF;
            memcpy (&sxs_Rmt [SXS_RMT_DBG_CPU_IDX], Address, sizeof (sxs_Rmt) / SXS_RMT_NB_BUF);
            sxs_Boot -> RmtBufferSaved = FALSE;

            while (sxs_SerialFlush () && (TimeOut--));

            SXS_TRACE (_SXR|TSTDOUT|TDB,TSTR("--------- End buffer recovery. ----------\n",0x06be0004));
        }
    }
}
#endif



// =============================================================================
// sxs_ChangeSerialSpeed
// -----------------------------------------------------------------------------
/// Change serial speed and inform remote computer.
/// @param Speed     New Speed.
/// @param SerialIdx Serial port idx.
// =============================================================================
#ifdef __SXS_SPLIT_2_CPU__
void sxs_ChangeSerialSpeed (u8 Speed, u8 SerialIdx)
{
    u16 Setting;
    sxs_FlwSerial_t *sxs_FlwSerial = (sxs_FlwSerial_t *)_sxs_SendToRemote (SXS_SERIAL_RMC, NIL, sizeof (sxs_FlwSerial_t), SXS_RMT_SPL_CPU_IDX);

    sxs_FlwSerial -> Id        = SXS_RMC_REQ;
    sxs_FlwSerial -> Speed     = Speed;
    sxs_FlwSerial -> SerialIdx = 0;

    while (sxs_SerialFlush ());

    Setting = sxs_GetSerialCfg (SerialIdx);
    sxs_SetSerialCfg ((Setting & ~SXS_RATE_MSK) | SXS_SET_RATE(Speed), SerialIdx);

}
#endif


// =============================================================================
// sxs_FuncName
// -----------------------------------------------------------------------------
/// Scope
/// @param 
/// @return
// =============================================================================

