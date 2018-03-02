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


#include "wmaudio_type.h"


#define MIN_OBJECT_SIZE     24
#define DATA_OBJECT_SIZE    50
#define WMA_MAX_DATA_REQUESTED  128

#define GetUnalignedWord( pb, w )   (w) = *( WORD*)(pb); 

#define GetUnalignedDword( pb, dw ) (dw) = *( WORD*)(pb); (dw)|= ((DWORD)*(( WORD*)(pb)+1))<<16;

#define GetUnalignedQword( pb, qw ) (qw) = *( QWORD*)(pb);

//#define GetUnalignedDouble( pb, d ) (d) = *( double*)(pb);

#define GetUnalignedWordEx( pb, w )     GetUnalignedWord( pb, w ); (pb) += sizeof(WORD);
#define GetUnalignedDwordEx( pb, dw )   GetUnalignedDword( pb, dw ); (pb) += sizeof(DWORD);
#define GetUnalignedQwordEx( pb, qw )   GetUnalignedQword( pb, qw ); (pb) += sizeof(QWORD);
//#define GetUnalignedDoubleEx( pb, d )   GetUnalignedDouble( pb, d ); (pb) += sizeof(double);

#define LoadBYTE( b, p )    b = *(BYTE *)(p);  (p) += sizeof( BYTE )

#define LoadWORD( w, p )    GetUnalignedWordEx( p, w )
#define LoadDWORD( dw, p )  GetUnalignedDwordEx( p, dw )
#define LoadQWORD( qw, p )  GetUnalignedQwordEx( p, qw )

#define LoadGUID( g, p ) \
        { \
            LoadDWORD( (g).Data1, p ); \
            LoadWORD( (g).Data2, p ); \
            LoadWORD( (g).Data3, p ); \
            LoadBYTE( (g).Data4[0], p ); \
            LoadBYTE( (g).Data4[1], p ); \
            LoadBYTE( (g).Data4[2], p ); \
            LoadBYTE( (g).Data4[3], p ); \
            LoadBYTE( (g).Data4[4], p ); \
            LoadBYTE( (g).Data4[5], p ); \
            LoadBYTE( (g).Data4[6], p ); \
            LoadBYTE( (g).Data4[7], p ); \
        }

//

WMAERR WMA_LoadObjectHeader(tWMAFileHdrStateInternal *pInt, GUID *pObjectId, QWORD *pqwSize);
WMAERR WMA_LoadHeaderObject(tWMAFileHdrStateInternal *pInt, int isFull);
WMAERR WMA_LoadPropertiesObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize, int isFull);
WMAERR WMA_LoadAudioObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize, int isFull);
WMAERR WMA_LoadEncryptionObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize);
WMAERR WMA_LoadContentDescriptionObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize);

WMAERR WMA_ParseAsfHeader(tWMAFileHdrStateInternal *pInt, int isFull);


