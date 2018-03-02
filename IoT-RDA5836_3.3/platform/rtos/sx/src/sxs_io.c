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
  File         sxs_io.c
--------------------------------------------------------------------------------

  Scope      : Input/Output.

  History    :
--------------------------------------------------------------------------------
  Jun 15 03  |  ADA  | Creation
  Sep 13 04  |  ADA  | Use macro PAL_IS_ROM for backtrace.
  Dec 13 04  |  ADA  | Manage Tdb.
  Dec 15 05  |  ADA  | sxs_IoInit: Retrieve PAL debug information after a crash 
             |       | (under __PAL_DEBUG__ flag).
================================================================================
*/

#if defined (__TARGET__)
#define __SXS_REMOTE_TRACE__
#endif

#include "sxs_type.h"
#define __SXS_IO_VAR__
#include "sxs_io.h"
#undef __SXS_IO_VAR__
#include "sxs_rmt.h"
#include "sxs_rmc.h"
#include "sxs_lib.h"
#include "sxs_io.hp"
#include "sxs_flsh.h"
#include "sxs_srl.h"
#include "sxr_cpu.h"
#include "sxr_jbsd.h"
#include "sxr_tksd.h"
#include "sxr_mem.h"
#include "sxr_csp.h"
#include "sxs_srl.hp"
#include "hal_host.h"
#ifndef __TARGET__
#include "sxs_udp.h"
#endif

#include <stdarg.h>

#define __SXS_REMOTE_TRACE__

#ifndef __SXS_REMOTE_TRACE__
#include <stdio.h>
#endif

#include "cs_types.h"
#include "hal_bt.h"
#include "rdabt_sharemem.h"

// =============================================================================
// g_sxRaiseDate
// -----------------------------------------------------------------------------
/// Holding the system time when the last raise occurs.
// =============================================================================
PUBLIC UINT32 g_sxRaiseDate = 0;

#if 1
#ifdef GSM_SUPPORT
#define SXS_TRACE_BUFF_SIZE             (8*1024)//Big trace buffer
#else
#define SXS_TRACE_BUFF_SIZE             (128)
#endif
extern  UINT32  g_sxsDebugIdleHookEnable;
extern  UINT32  g_sxExternalTraceEnable;
extern  UINT16  g_sxTraceLelvel[32];

UINT32 HAL_UNCACHED_BSS sxs_trace_buf[SXS_TRACE_BUFF_SIZE/4];
INT32 trace_pos = -1;

void sxs_vprintf_flash (u32 Id, const ascii *Fmt, va_list argPtr);

void sxs_IoInit (void)
{
    extern void (*g_pPrintf_ROM) (u32 Id, const ascii *Fmt, va_list argPtr);

    u16 Setting;
    Setting = SXS_SET_NB_DATA (SXS_8_DATA)
            | SXS_SET_NB_STOP (SXS_1_STOP)
            | SXS_SET_PARITY (SXS_PARITY_DISABLE)
            | SXS_SET_RATE (SXS_R115200);

#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5855)
    g_pPrintf_ROM = sxs_vprintf_flash;
#endif    
    //hal_HstTraceOpen(Setting, HAL_HST_TRACE_PORT_HOST);
    trace_pos = 0;
}


/*
==============================================================================
   Function   : sxs_vprintf
 -----------------------------------------------------------------------------
   Scope      : Send a formatted trace string to the remote computer.
   Parameters : Id (User Id + trace level + ...)
                Pointer onto formatted string.
                Arguments list.
   Return     : None.
==============================================================================
*/
#if (CHIP_ASIC_ID >= CHIP_ASIC_ID_5855)
void sxs_vprintf_flash (u32 Id, const ascii *Fmt, va_list argPtr)
#else
void sxs_vprintf(u32 Id, const ascii *Fmt, va_list argPtr)
#endif
{
    int i=0;
    int out_buf;

    if(g_sxExternalTraceEnable == 0)
        return;
    if(g_sxTraceLelvel[TGET_ID(Id)] == 0)
        return;
    if((g_sxTraceLelvel[TGET_ID(Id)]&(1L<<TGET_LEVEL(Id)))==0 /*&& (Id & TSTDOUT)==0*/)
        return;
    //#endif

#ifndef GSM_SUPPORT//(CHIP_ASIC_ID < CHIP_ASIC_ID_5855)
    memset(sxs_trace_buf, 0, sizeof(sxs_trace_buf));
    out_buf = vsnprintf((char*)sxs_trace_buf, sizeof(sxs_trace_buf)-1, Fmt, argPtr);
#if (CHIP_HAS_BTCPU == 1)
    if(hal_IsBtStart())
        rdabt_wait_btcpu_busy();
#endif
    hal_HstSendEvent(0xff, 0x72ace000 + out_buf);
    hal_HstSendEvent(0xff, Id);
    while(i<SXS_TRACE_BUFF_SIZE/4 && sxs_trace_buf[i]!=0)
    {
        hal_HstSendEvent(0xff, sxs_trace_buf[i++]);
    }
#if (CHIP_HAS_BTCPU == 1)
    if(hal_IsBtStart())
        rdabt_clear_xcpu_busy();
#endif
#else
    if(trace_pos<0)
        return;
    //hal_HstTraceDisable();
    //hal_HstTraceEnable();
    //memset(sxs_trace_buf+trace_pos, 0, SXS_TRACE_BUFF_SIZE/2);
    out_buf = vsnprintf(sxs_trace_buf+trace_pos, SXS_TRACE_BUFF_SIZE-trace_pos*4, Fmt, argPtr);
//	sxs_trace_buf[trace_pos] = (((out_buf+4)&0xff)<<16)|(((out_buf+4)&0xff00))|0xfe0000ad ;
//    sxs_trace_buf[trace_pos+1] = Id;
    //hal_HstTraceSendData(HAL_HST_TRACE_PORT_HOST, sxs_trace_buf+trace_pos, out_buf+8);
    if(out_buf <=0 || trace_pos*4 + out_buf +2 >= SXS_TRACE_BUFF_SIZE)
    {
        trace_pos = 0;
        out_buf = vsnprintf(sxs_trace_buf, SXS_TRACE_BUFF_SIZE, Fmt, argPtr);
    }
    
    hal_HstSendEvent(0xff,0x73ace000 + out_buf);
    hal_HstSendEvent(0xff,Id);
    hal_HstSendEvent(0xff,sxs_trace_buf+trace_pos);

    trace_pos += (out_buf+3)/4;

    //if((SXS_TRACE_BUFF_SIZE/4-trace_pos) < SXS_TRACE_BUFF_SIZE/16)
       // trace_pos = 0;
#endif
}

/*
==============================================================================
   Function   : sxs_fprintf
 -----------------------------------------------------------------------------
   Scope      : Send a formatted trace string to the remote computer.
   Parameters : Id (User Id + trace level + ...)
                Pointer onto formatted string.
                Arguments list.
   Return     : None.
==============================================================================
*/
void sxs_fprintf (u32 Id, const ascii *Fmt, ...)
{
	va_list ap;

	va_start (ap, Fmt);
    sxs_vprintf(Id, Fmt, ap);
    va_end (ap);
}

/*
==============================================================================
   Function   : sxs_Raise
 -----------------------------------------------------------------------------
   Scope      : Raise an exception. Send formatted trace string to remote
                computer. In case of TABORT, exit of the program.
   Parameters : Id (User Id + trace level + Abort + ...)
                Arguments list.
                Pointer onto formatted string.
   Return     : None.
==============================================================================
*/
void sxs_Raise (u32 Id, const ascii *Fmt, ...)
{
    u32 Ra;
    u32 Sp;
    va_list ArgPtr;

    SXS_GET_RA(&Ra);
    SXS_GET_SP(Sp);

    va_start (ArgPtr, Fmt);
    sxs_vprintf(Id, Fmt, ArgPtr);
    va_end (ArgPtr);

    sxs_fprintf(Id|TSTDOUT,"Exception %s, Ra=0x%08x, Sp=0x%08x",sxs_TraceDesc [TGET_ID(Id)],Ra,Sp);
}

/*
==============================================================================
   Function   : sxs_Dump
 -----------------------------------------------------------------------------
   Scope      : Dump some data with required format.
   Parameters : Pointer onto data to be dump.
                Size expressed in bytes of the data.
                Format string. Max 3 characters.
                %2x     dump bytes in hexa.
                %4x     dump word in hexa.
                %8x     dump double word in hexa.
                i for signed decimal, d for unsigned decimal...
   Return     : None.
==============================================================================
*/
void sxs_Dump    (u32 Id, ascii *Fmt, u8 *Data, u16 Size)
{
}
#else


/*
==============================================================================
   Function   : sxs_IoInit
 -----------------------------------------------------------------------------
   Scope      : I/O initialization.
   Parameters : None.
   Return     : None.
==============================================================================
*/
void sxs_IoInit (void)
{
#ifdef __TARGET__
 sxs_GetRmtBuffer ();
#endif

 sxs_RmtInit ();

#ifndef __SXS_NO_FLASH_HANDLER__
 sxs_FlashHandlerInit ();
 sxs_FileInit ();
#endif

}

/*
==============================================================================
   Function   : sxs_TraceOut
 -----------------------------------------------------------------------------
   Scope      : Format and send a trace to the remote computer.
   Parameters : Id (User Id + trace level + ...)
                Pointer onto formatted string.
                Arguments list.
   Return     : None.
==============================================================================
*/
#ifdef __SXS_REMOTE_TRACE__
void sxs_TraceOut (u32 Id, const ascii *Fmt, va_list ArgPtr)
{
 u32 Arg [8];
 u16 ArgSize [8];
 u8  *Out;
 u32 i;
 u8  NbArgs   = (Id & TARG) ? TGET_NB_ARG(Id) : SXS_DEFAULT_NB_ARG;
 u16 ArgsSize = 0;
 u8  Tdb = (Id & TDB) != 0;
 u16 FmtSize  = Tdb ? 4 : strlen (Fmt) + 1;


 for (i=0; i<NbArgs; i++)
 {
  Arg [i] = va_arg (ArgPtr, u32);

  if (TGET_SMAP(Id) & (1 << i))
  {
   ArgSize [i] = strlen ((ascii *)Arg [i]) + 1;
   ArgsSize += ArgSize [i];
  }
  else
   ArgsSize += 4;
 }

 if ((Out = sxs_SendToRemote (SXS_TRACE_RMC, NIL, (u16)(2 + ArgsSize + FmtSize))) != NIL)
 {
  *(u16 *)Out = (u16)Id;

  if (Tdb)
   memcpy (&Out[2], &Fmt, FmtSize);
  else
   memcpy (&Out[2], Fmt, FmtSize);

  Out += FmtSize + 2;

  if (TGET_SMAP(Id))
  {
   for (i=0; i<NbArgs; i++)
   {
    if (TGET_SMAP(Id) & (1 << i))
    {
     memcpy (Out, (ascii *)Arg [i], ArgSize [i]);
     Out += ArgSize [i];
    }
    else
    {
     memcpy (Out, (ascii *)&Arg [i], 4);
     Out += 4;
    }
   }
  }
  else if (ArgsSize)
   memcpy (Out, (u8 *)Arg, ArgsSize);
 }
}
#endif
/*
==============================================================================
   Function   : sxs_vprintf
 -----------------------------------------------------------------------------
   Scope      : Send a formatted trace string to the remote computer.
   Parameters : Id (User Id + trace level + ...)
                Pointer onto formatted string.
                Arguments list.
   Return     : None.
==============================================================================
*/
void sxs_vprintf (u32 Id, const ascii *Fmt, va_list argPtr)
{
 if ((sxs_IoCtx.TraceBitMap [TGET_ID(Id)] & (1L << TGET_LEVEL(Id)))
 ||  (Id & TSTDOUT))
 {
#ifdef __SXS_REMOTE_TRACE__
  sxs_TraceOut (Id, Fmt, argPtr);
#ifndef __EMBEDDED__
  sxs_RmtBufferFlush ();
#endif
#else
  if (!(Id & TIDU))
   printf ("%s : %2i ", sxs_TraceDesc [TGET_ID(Id)], TGET_LEVEL(Id));
  vfprintf (stdout, Fmt, argPtr);
#endif
 }
}


/*
==============================================================================
   Function   : sxs_fprintf
 -----------------------------------------------------------------------------
   Scope      : Send a formatted trace string to the remote computer.
   Parameters : Id (User Id + trace level + ...)
                Pointer onto formatted string.
                Arguments list.
   Return     : None.
==============================================================================
*/
void sxs_fprintf (u32 Id, const ascii *Fmt, ...)
{
  va_list ArgPtr;
  va_start (ArgPtr, Fmt);
  sxs_vprintf(Id, Fmt, ArgPtr);
  va_end( ArgPtr );
}

/*
==============================================================================
   Function   : sxs_Raise
 -----------------------------------------------------------------------------
   Scope      : Raise an exception. Send formatted trace string to remote
                computer. In case of TABORT, exit of the program.
   Parameters : Id (User Id + trace level + Abort + ...)
                Arguments list.
                Pointer onto formatted string.
   Return     : None.
==============================================================================
*/
void sxs_Raise (u32 Id, const ascii *Fmt, ...)
{
    static u8 FunctionLocked = FALSE;
    u32 Ra;
    u32 Sp;
    u8 FlowCtrl;
    va_list ArgPtr;

    if (FunctionLocked && (!(Id & TABORT)))
        return;

    FunctionLocked = TRUE;

    va_start (ArgPtr, Fmt);

    SXS_GET_RA(&Ra);
    SXS_GET_SP(Sp);

    g_sxRaiseDate = hal_TimGetUpTime();

    if (!(Id & TABORT))
    {
        Id |= TRAISE;  // de Lisieux
    }
    else
    {
        // Prohibit task scheduling
        sxr_EnterScSchedule ();
        // Prohibit interrupt handling
        sxr_EnterSc ();

        FlowCtrl = sxs_GetFlowControlStatus(PAL_DBG_PORT_IDX);
        if ((FlowCtrl & SXS_RELEASE_TX) == 0)
        {
            // Allow trace output
            sxs_SetFlowControl(PAL_DBG_PORT_IDX, SXS_RELEASE_TX);  
            // Empty the outdated RMT buffer
#ifdef __SXS_REMOTE_TRACE__
            while (sxs_SerialFlush());
#ifndef __EMBEDDED__
            sxs_RmtBufferFlush ();
#endif
#endif
        }

        // Print PAL abort information
        pal_SoftAbort ();
    }

#ifdef __SXS_REMOTE_TRACE__
    sxs_TraceOut (Id, Fmt, ArgPtr);
#ifndef __EMBEDDED__
    sxs_RmtBufferFlush ();
#endif
#else
    printf ("Exception %s ", sxs_TraceDesc [TGET_ID(Id)]);
    vfprintf (stdout, Fmt, ArgPtr);
#endif

    if (Id & TABORT)
    {
#if defined (__TARGET__) && (!defined(__SIMU__))
        sxs_Boot -> ExceptionCount++;
        sxs_Boot -> ExceptionExpected = TRUE;
        sxs_BackTrace (Ra, &Sp);

        if (sxs_Boot -> RmtBufferSaved == FALSE)
        sxs_SaveRmtBuffer ();
#endif
    }

    while (Id & TABORT)
    {
        pal_SoftBreak ();

#ifdef __TARGET__
        sxs_SerialFlush ();
#endif

#ifndef __EMBEDDED__
        sxs_RmtBufferFlush ();
#endif
    }
    FunctionLocked = FALSE;

    va_end( ArgPtr );
}

/*
==============================================================================
   Function   : sxs_Dump
 -----------------------------------------------------------------------------
   Scope      : Dump some data with required format.
   Parameters : Pointer onto data to be dump.
                Size expressed in bytes of the data.
                Format string. Max 3 characters.
                %2x     dump bytes in hexa.
                %4x     dump word in hexa.
                %8x     dump double word in hexa.
                i for signed decimal, d for unsigned decimal...
   Return     : None.
==============================================================================
*/
void sxs_Dump (u32 Id, ascii *Fmt, u8 *Data, u16 Size)
{
 if ((sxs_IoCtx.TraceBitMap [TGET_ID(Id)] & (1L << TGET_LEVEL(Id)))
 ||  (Id & TSTDOUT))
 {
  sxs_FlwDmp_t *FlwDmp = (sxs_FlwDmp_t *)sxs_SendToRemote (SXS_DUMP_RMC, NIL, (u16)(Size + sizeof (sxs_FlwDmp_t)));

  if (FlwDmp == NIL)
   return;

  FlwDmp -> Id   = (u16)Id;
  FlwDmp -> Size = Size;
  FlwDmp -> Address = Data;

  memcpy (FlwDmp -> Data, Data, Size);

  if (Fmt)
  {
   strcpy (FlwDmp -> Fmt, Fmt);
   FlwDmp -> Fmt [3] = 0;
  }
  else
   strcpy (FlwDmp -> Fmt, "%2x");

#ifndef __EMBEDDED__
  sxs_RmtBufferFlush ();
#endif
 }
}

#ifndef __SXS_NO_FLASH_HANDLER__
/*
==============================================================================
   Function   : sxs_FileInit
 -----------------------------------------------------------------------------
   Scope      : File manager context initialization.
   Parameters : none
   Return     : none
==============================================================================
*/
void sxs_FileInit (void)
{
#ifndef __SXS_STD_FILE_LIB__
 u32 FilesStart [SXS_FLH_NB_FILES_MAX];
 u32 FilesEnd [SXS_FLH_NB_FILES_MAX];
 u32 i;

 memset ((u8 *)&sxs_FileCtx, 0, sizeof (sxs_FileCtx));

 sxs_FlashBlockRead (0, (u8 *)sxs_FileCtx.Desc, sizeof (sxs_FileCtx.Desc));

 sxs_GetFilesLimit (FilesStart, FilesEnd);

 for (i=1; i<SXS_FLH_NB_FILES_MAX; i++)
 {
  if (sxs_FileCtx.Desc [i].Name [0] != 0)
  {
   sxs_FileCtx.NbFiles++;
   sxs_FileCtx.Desc [i].Size = (u16)(FilesEnd [i] - FilesStart [i]);
  }
  else
   sxs_FileCtx.Handle [sxs_FileCtx.FreeHandle++] = (u8)i;
 }
 sxs_FileCtx.Handle [sxs_FileCtx.FreeHandle] = 0xFF;
 sxs_FileCtx.FreeHandle = 0;
#endif
}

#ifndef __SXS_STD_FILE_LIB__
/*
==============================================================================
   Function   : sxs_GetFileHandle
 -----------------------------------------------------------------------------
   Scope      : Retreive a free handle.
   Parameters : none
   Return     : Handle number or -1 when no handle is left free.
==============================================================================
*/
s16 sxs_GetFileHandle (void)
{
 sxs_FileCtx.NbFiles++;

 if (sxs_FileCtx.Handle [sxs_FileCtx.FreeHandle] != 0xFF)
  return (s16)sxs_FileCtx.Handle [sxs_FileCtx.FreeHandle++];
 else
  sxs_Raise (_SXR|TABORT|TDB|TNB_ARG(1),TSTR("Too much files %i\n",0x06ba0001), sxs_FileCtx.NbFiles);

 return -1;
}

/*
==============================================================================
   Function   : sxs_SearchFile
 -----------------------------------------------------------------------------
   Scope      : Check if the file identified by its name already exists and
                if so, retreive its Handle.
   Parameters : Pointer on file name
   Return     : File Handle or -1 when file has not been found.
==============================================================================
*/
s16 sxs_SearchFile (const ascii *FileName)
{
 s16 i;

 for (i=1; i<SXS_FLH_NB_FILES_MAX; i++)
 {
  if (!strcmp (sxs_FileCtx.Desc [i].Name, FileName))
   return i;
 }
 return -1;
}


/*
==============================================================================
   Function   : sxs_FreeFileHandle
 -----------------------------------------------------------------------------
   Scope      : Free an allocated Handle.
   Parameters : Handle number.
   Return     : none
==============================================================================
*/
void sxs_FreeFileHandle (s16 Handle)
{
 sxs_FileCtx.NbFiles--;
 sxs_FileCtx.Handle [--sxs_FileCtx.FreeHandle] = (u8)Handle;
}

/*
==============================================================================
   Function   : sxs_Open
 -----------------------------------------------------------------------------
   Scope      : opens the file specified by filename and prepares the file
                for reading or writing, as specified by oflag.
                Files are always assumed binary. Text files are not managed.
   Parameters : - pointer on file name,
                - OFlag:
   SXS_O_APPEND      Moves file pointer to end of file before every write operation.
   SXS_O_BINARY      Opens file in binary mode. No Effect as file are assumed binary.
   SXS_O_CREAT       Creates and opens new file for writing. If file specified
                     by filename exists, existing file is opened for writing.
   SXS_O_RDONLY      Opens file for reading only.
   SXS_O_WRONLY      Opens file for writing only.
   SXS_O_RDWR        Opens file for both reading and writing.
   SXS_O_TRUNC       Opens file and truncates it to zero length. Used with,
                     _O_CREAT opens an existing file or creates a new file.
   SXS_O_TEMPORARY   Create file as temporary. File is created in stacked bank
                     and is deleted when stacked bank is full.
   SXS_O_SHORT_LIVED Create file as temporary. File is created in dynamic bank
                     and won't be moved in static bank in case of static bank
                     update.
                - PMode exists for compatibility purpose only. Access right
                are not managed.
   Return     : Handle number in case of success or -1.
==============================================================================
*/
s16 sxs_Open (const ascii *FileName, s16 OFlag, s16 PMode)
{
 s16 Handle = -1;

 if ((Handle = sxs_SearchFile (FileName)) != -1)
 {
  if (OFlag & SXS_O_TRUNC)
  {
   sxs_FlashBlockDelete (SXS_SET_FILE_IDX(Handle), SXS_FILE_IDX_MSK, TRUE);
   sxs_FileCtx.Desc [Handle].Size = 0;
  }

  if (OFlag & SXS_O_APPEND)
   sxs_FileCtx.Desc [Handle].Offset = sxs_FileCtx.Desc [Handle].Size; // -1;
  else
   sxs_FileCtx.Desc [Handle].Offset = 0;

  sxs_FileCtx.Desc [Handle].OFlag |= OFlag;
 }
 else if (OFlag & SXS_O_CREAT)
 {
  Handle = sxs_GetFileHandle ();

  sxs_FileCtx.Desc [Handle].Size = 0;
  sxs_FileCtx.Desc [Handle].Offset = 0;
  memcpy ((u8 *)&sxs_FileCtx.Desc [Handle], FileName, SXS_FILE_NAME_MAX_LEN);
  sxs_FileCtx.Desc [Handle].OFlag = (OFlag & ~SXS_O_RDWR);

  sxs_FlashBlockWrite ((Handle * sizeof (sxs_FileDesc_t)) | SXS_FLH_PROTECTED_BLOCK,
                       (u8 *)&sxs_FileCtx.Desc [Handle],
                       sizeof (sxs_FileDesc_t));

  sxs_FileCtx.Desc [Handle].OFlag = OFlag | SXS_O_RDWR;
 }

 return Handle;
}


/*
==============================================================================
   Function   : sxs_Close
 -----------------------------------------------------------------------------
   Scope      : Close identified file.
   Parameters : Handle referring to open file
   Return     : 0 in case of success, -1 otherwise.
==============================================================================
*/
s16 sxs_Close (s16 Handle)
{
 if (sxs_FileCtx.Desc [Handle].OFlag & SXS_O_RDWR)
 {
  sxs_FileCtx.Desc [Handle].OFlag &= ~SXS_O_RDWR;
  return 0;
 }
 else
  return -1;
}

/*
==============================================================================
   Function   : sxs_Read
 -----------------------------------------------------------------------------
   Scope      : Read a bunch of data in the identified file.
   Parameters : Handle referring to open file
                Buffer Storage location for data
                Count Maximum number of bytes
   Return     : Number of read bytes in case of success, -1 otherwise.
==============================================================================
*/
s16 sxs_Read (s16 Handle, void *Buffer, u16 Count)
{
 u16 Size;
 u32 Id;

 if (!(sxs_FileCtx.Desc [Handle].OFlag & SXS_O_RDWR))
 {
  sxs_Raise (_SXR|TDB|TNB_ARG(1),TSTR("File not opened %i\n",0x06ba0002), Handle);
  return -1;
 }

 Size = MIN (Count, sxs_FileCtx.Desc [Handle].Size - sxs_FileCtx.Desc [Handle].Offset);

 Id = (sxs_FileCtx.Desc [Handle].OFlag & SXS_O_TEMPORARY ? SXS_FLH_STACKED_BLOCK : 0)
    | (sxs_FileCtx.Desc [Handle].OFlag & SXS_O_SHORT_LIVED ? SXS_FLH_PROTECTED_BLOCK : 0);
 Id |= sxs_FileCtx.Desc [Handle].Offset | SXS_SET_FILE_IDX (Handle);

 sxs_FileCtx.Desc [Handle].Offset += Size;

 sxs_FlashBlockRead (Id, (u8 *)Buffer, Size);

 return Size;
}

/*
==============================================================================
   Function   : sxs_Write
 -----------------------------------------------------------------------------
   Scope      : Write a bunch of data in the identified file.
   Parameters : Handle referring to open file
                Buffer Storage location for data
                Count Maximum number of bytes
   Return     : Number of written bytes in case of success, -1 otherwise.
==============================================================================
*/
s16 sxs_Write (s16 Handle, const void *Buffer, u16 Count)
{
 u32 Id;

 if (!(sxs_FileCtx.Desc [Handle].OFlag & SXS_O_WRONLY))
 {
  sxs_Raise (_SXR|TDB|TNB_ARG(1),TSTR("File not opened for write %i\n",0x06ba0003), Handle);
  return -1;
 }

 Id = (sxs_FileCtx.Desc [Handle].OFlag & SXS_O_TEMPORARY ? SXS_FLH_STACKED_BLOCK : 0)
    | (sxs_FileCtx.Desc [Handle].OFlag & SXS_O_SHORT_LIVED ? 0 : SXS_FLH_PROTECTED_BLOCK);
 Id |= sxs_FileCtx.Desc [Handle].Offset | SXS_SET_FILE_IDX (Handle);

 sxs_FileCtx.Desc [Handle].Offset += Count;

 if (sxs_FileCtx.Desc [Handle].Size < sxs_FileCtx.Desc [Handle].Offset)
  sxs_FileCtx.Desc [Handle].Size = sxs_FileCtx.Desc [Handle].Offset;

 sxs_FlashBlockWrite (Id, (u8 *)Buffer, Count);

 return 0;
}

/*
==============================================================================
   Function   : sxs_LSeek
 -----------------------------------------------------------------------------
   Scope      : Move the file pointer associated with handle to a new
                location that is offset bytes from origin. The next operation
                on the file occurs at the new location.
   Parameters : Handle referring to open file
                Bytes offset
                Origin:
                SXS_SEEK_SET Beginning of file
                SXS_SEEK_CUR Current position of file pointer
                SXS_SEEK_END End of file
   Return     : none
==============================================================================
*/
s32 sxs_LSeek (s16 Handle, u32 Offset, u16 Origin)
{
 if (!(sxs_FileCtx.Desc [Handle].OFlag & SXS_O_RDWR))
 {
  sxs_Raise (_SXR|TDB|TNB_ARG(1),TSTR("File not opened %i\n",0x06ba0004), Handle);
  return -1;
 }

 if (Origin == SXS_SEEK_SET)
  sxs_FileCtx.Desc [Handle].Offset = (u16)MIN (Offset, (u16)(sxs_FileCtx.Desc [Handle].Size)); // sxs_FileCtx.Desc [Handle].Offset);
 else if (Origin == SXS_SEEK_CUR)
 {
  sxs_FileCtx.Desc [Handle].Offset += (u16)Offset;

  if (sxs_FileCtx.Desc [Handle].Offset > sxs_FileCtx.Desc [Handle].Size)
   sxs_FileCtx.Desc [Handle].Offset = sxs_FileCtx.Desc [Handle].Size;
 }
 else if (Origin == SXS_SEEK_END)
//  sxs_FileCtx.Desc [Handle].Offset = sxs_FileCtx.Desc [Handle].Size ? sxs_FileCtx.Desc [Handle].Size - 1 : 0;
  sxs_FileCtx.Desc [Handle].Offset = sxs_FileCtx.Desc [Handle].Size;

 return sxs_FileCtx.Desc [Handle].Offset;
}
#endif

/*
==============================================================================
   Function   : sxs_DelFile
 -----------------------------------------------------------------------------
   Scope      : Delete an existing file.
   Parameters : Handle referring to open file
   Return     : none
==============================================================================
*/
void sxs_DelFile (s16 Handle)
{
#ifndef __SXS_STD_FILE_LIB__
 if (sxs_FileCtx.Desc [Handle].Name [0] != 0)
 {
  sxs_FlashBlockDelete ( SXS_SET_FILE_IDX(Handle)
                       |(SXS_O_TEMPORARY ? SXS_FLH_STACKED_BLOCK : 0)
                       |(SXS_O_SHORT_LIVED ? 0 : SXS_FLH_STATIC_BLOCK),
                         SXS_FILE_IDX_MSK, TRUE);

  sxs_FlashBlockDelete ((Handle * sizeof (sxs_FileDesc_t)),
                        SXS_OFFSET_MSK | SXS_FILE_IDX_MSK, TRUE);

  memset ((u8 *)&sxs_FileCtx.Desc [Handle], 0, sizeof (sxs_FileDesc_t));

  sxs_FreeFileHandle (Handle);

 }
 else
  sxs_Raise (_SXR|TDB|TNB_ARG(1),TSTR("File not found for deletion %i\n",0x06ba0005), Handle);
#endif
}


/*
==============================================================================
   Function   : sxs_CheckFiles
 -----------------------------------------------------------------------------
   Scope      : Print a status for all existing files.
   Parameters : none
   Return     : none
==============================================================================
*/
void sxs_CheckFiles (void)
{
#ifndef __SXS_STD_FILE_LIB__
 u32 i;
 u32 TotalSize = 0;

 for (i=1; i<SXS_FLH_NB_FILES_MAX; i++)
 {
  if (sxs_FileCtx.Desc [i].Name [0] != 0)
  {
   SXS_TRACE (_SXR|TSTDOUT|TSMAP(1)|TDB|TNB_ARG(4),TSTR("File %s Size %i Offset %i state 0x%lx\n",0x06ba0006),
                               sxs_FileCtx.Desc [i].Name,
                               sxs_FileCtx.Desc [i].Size,
                               sxs_FileCtx.Desc [i].Offset,
                               sxs_FileCtx.Desc [i].OFlag);

   TotalSize += sxs_FileCtx.Desc [i].Size;
  }

 }
 SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(2),TSTR("%i files Total size %i\n",0x06ba0007), sxs_FileCtx.NbFiles, TotalSize);

#endif
}
#endif

/*
==============================================================================
   Function   :
 -----------------------------------------------------------------------------
   Scope      :
   Parameters : none
   Return     : none
==============================================================================
*/
void sxs_BackTrace (u32 Ra, u32 *Sp)
{
 u32 i = 0;
 u32 Cnt = 0;
 static u8 Lock = FALSE;

 pal_SoftAbort ();

 if (Lock != TRUE)
 {
  if (!sxs_CheckStkJb (Sp))
  {
   if (!sxs_CheckStkTk (Sp))
   {
    SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(1),TSTR("Stack owner not found 0x%x\n",0x06ba0008), Sp);
    sxr_CheckJob ();
   }
  }
  else
  {
   Lock = TRUE;
   sxs_CheckPendingTk ();
   Lock = FALSE;
  }
 }

 SXS_TRACE (_SXR|TSTDOUT|TDB,TSTR("Back trace\n",0x06ba0009));

 SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(1),TSTR("%lxy\n",0x06ba000a), Ra, Sp);

 while ((Cnt < 20) && (i++ < 300))
 {
  if (PAL_IS_ROM(*Sp))
  {
   SXS_TRACE (_SXR|TSTDOUT|TDB|TNB_ARG(1),TSTR("%lxy\n",0x06ba000b), *Sp);
   Cnt++;
  }
  (Sp)++;
 }

}

#endif

