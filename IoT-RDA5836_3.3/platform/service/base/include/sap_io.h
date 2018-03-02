//------------------------------------------------------------------------------
//  $Log: sap_io.h,v $
//  Revision 1.6  2006/06/07 12:54:08  fch
//  Change to set array of pointer in ROM
//
//  Revision 1.5  2006/03/24 10:30:26  fch
//  Update with Bob
//
//  Revision 1.4  2006/02/13 02:35:17  oth
//  Finish to remove SXS from the IO modules
//
//  Revision 1.3  2006/01/17 13:45:00  oth
//  Update from Bob Ref v1.18
//
//  Revision 1.2  2005/12/29 17:39:16  pca
//  Log keyword added to all files
//
//------------------------------------------------------------------------------
/*
================================================================================

  This source code is property of StackCom. The information contained in this
  file is confidential. Distribution, reproduction, as well as exploitation,or
  transmisison of any content of this file is not allowed except if expressly
  permitted. Infringements result in damage claims.

  Copyright StackCom (c) 2003

================================================================================
*/

/*
================================================================================
  File       : sap_io.h
--------------------------------------------------------------------------------

  Scope      : User I/O relatif defines.

  History    :
--------------------------------------------------------------------------------
  Mar 08 06  |  MCE  | removed "SXS" from sxs_TraceDesc  
  Jan 16 06  |  MCE  | Removed __SXS
  Jan 12 06  |  FCH  | Removed __MMP
  Jan 11 06  |  MCE  | Removed __RR
  Jan 10 06  |  FCH  | Removed __CB
  Jan 04 06  |  OTH  | Removed __L2R, __SMT
  Dec 30 05  |  OTH  | Removed __MAC and __COD
  Nov 23 05  |  FCH  | Created __RLP, __L2R
  Nov 14 05  |  MCE  | Created __RRD, _RRD, __RRI and _RRI
  Dec 10 04  |  ADA  | Creation
================================================================================
*/

#if 0 // ndef __SAP_IO_H__
#define __SAP_IO_H__

typedef enum {__SXR, __PAL, __L1A, __L1S, __LAP, __RLU, __RLD,
              __LLC, __MM,  __CC,  __SS,  __SMS,  __SM, __SND, __API,
              __MMI, __SIM, __AT , __M2A, __STT, __RRI, __RRD, __RLP,
              SXS_NB_ID} sxs_TraceId_t;

#define _SXR    TID(__SXR)
#define _PAL    TID(__PAL)
#define _L1A    TID(__L1A)
#define _L1S    TID(__L1S)
#define _LAP    TID(__LAP)
#define _RLU    TID(__RLU)
#define _RLD    TID(__RLD)
#define _LLC    TID(__LLC)
#define _MM     TID(__MM)
#define _CC     TID(__CC)
#define _SS     TID(__SS)
#define _SMS    TID(__SMS)
#define _SM     TID(__SM)
#define _SND    TID(__SND)
#define _API    TID(__API)
#define _MMI    TID(__MMI)
#define _SIM    TID(__SIM)
#define _AT     TID(__AT)
#define _M2A    TID(__M2A)
#define _STT    TID(__STT)
#define _RRI    TID(__RRI)
#define _RRD    TID(__RRD)
#define _RLP    TID(__RLP)

#ifdef __SXS_IO_VAR__

const ascii * const sxs_TraceDesc [SXS_NB_ID] =
{
 "SXR", "PAL", "L1A", "L1S", "LAP", "RLU", "RLD",
 "LLC", "MM ", "CC ", "SS ", "SMS", "SM ", "SND", "API",
 "MMI", "SIM", "AT ", "M2A", "STT", "RRI", "RRD", "RLP"
};

#else
extern const u8 * const sxs_TraceDesc [SXS_NB_ID];
#endif



#endif
