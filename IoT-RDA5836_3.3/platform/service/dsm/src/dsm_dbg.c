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

#include "dsm_cf.h"
#include "dsm_dbg.h"
#include "reg.h"
#include "fs.h"

#ifdef _FS_SIMULATOR_
INT32 g_dsm_errno;
BOOL  wfcheat = FALSE;
#endif

UINT32 g_dsm_DbgSwtich[ NR_SUBMODULE ] = {1, 1, 1,1,1};
UINT32 g_dsm_DbgLevel[ NR_SUBMODULE ] = {DL_ERROR, DL_ERROR, DL_ERROR,DL_ERROR,DL_ERROR};


const DSM_MODE_GDB g_dsmGbdLevel[] = 
    {
    {"DL",DSM_MOD_DL,DL_ERROR},
    {"VDS",DSM_MOD_VDS,DL_ERROR},
    {"FAT",DSM_MOD_FAT,DL_ERROR},
    {"VFS",DSM_MOD_VFS,DL_ERROR},
    {"REG",DSM_MOD_REG,DL_ERROR},
    {"SMS",DSM_MOD_SMS,DL_ERROR},
    {"NVRAM",DSM_MOD_NVRAM,DL_ERROR}
    };



extern UINT32 SMS_DM_GetVersion(VOID);


#ifdef _FS_SIMULATOR_
int dsm_Printf( int index, const char *format, ... )
{
    /*
    这里假设展开后的字符串的长度不超过1024.
    例如: dsm_Printf( index, "%s %x", "gaokejun", 0xDEADBEEF );
    则展开后的字符串是"gaokejun DEADBEEF".
    */
    UINT8 cTemp[ 1024 + LEN_FOR_NULL_CHAR ];
    va_list	ap;

    index = 0;
    va_start( ap, format );
    ( void ) vsprintf( (char*)cTemp, format , ap );
    va_end( ap );

    printf( "%s", cTemp );
    return 0;
}
#endif


#if 0
UINT8 g_dsm_DbgBuf[ VDS_DEBUG_BUF_SIZE ];

VOID dsm_Debugout( UINT32 ulLevel, CONST UINT8* szFormat, ... )
{
    va_list va_format;
    UINT32 lLen = 0;
    UINT32 iIndex = 0;    
    UINT32 iCount = 0;
    UINT32 iModeId = 0;
    UINT32 iLevel = 0;

    iModeId = (ulLevel >>16);
    iLevel = (ulLevel & 0xffff); 
    iCount = sizeof(g_dsmGbdLevel)/sizeof(DSM_MODE_GDB);
    for(iIndex = 0; iIndex < iCount; iIndex++)
    {
        if((UINT32)g_dsmGbdLevel[iIndex].iModId == iModeId )
        {
             break;
        }
    }
    if(iIndex == iCount || iLevel < g_dsmGbdLevel[iIndex].iLevel)
    {
        //return ;
    }

    DSM_MemSet( g_dsm_DbgBuf, 0, VDS_DEBUG_BUF_SIZE );

    va_start( va_format, szFormat );
#ifdef _FS_SIMULATOR_

    lLen = ( long ) DSM_StrVPrint( (char*)g_dsm_DbgBuf, ( VDS_DEBUG_BUF_SIZE - 1 ), (char*)szFormat, va_format );
#else

    lLen = ( long ) DSM_StrVPrint( g_dsm_DbgBuf, szFormat, va_format );
#endif

    if ( lLen < 0 )
    {
        va_end( va_format );
        return ;
    }
    va_end( va_format );


    switch (iLevel)
    {
        case DSM_ERROR:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [ERROR] %s\r\n",  g_dsmGbdLevel[iIndex].szModName, g_dsm_DbgBuf );
            break;
        case DSM_WARNING:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [WARNING] %s\r\n",g_dsmGbdLevel[iIndex].szModName, g_dsm_DbgBuf );
            break;
        case DSM_BRIEF:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [brief] %s\r\n", g_dsmGbdLevel[iIndex].szModName, g_dsm_DbgBuf );
            break;
        case DSM_DETAIL:
            dsm_Printf( BASE_DSM_TS_ID, "%s: [detail] %s\r\n", g_dsmGbdLevel[iIndex].szModName, g_dsm_DbgBuf );
            break;
        default:
            break;
    }
    return ;
}

#endif 

#ifdef _T_UPGRADE_PROGRAMMER
VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion )
{
        DSM_StrCpy(pVersion,"0x0");
}

#else


/* It is the caller's reponsibility to allocate memory for pVersion */
VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion )
{ 
    
    switch ( iModId )
    {
    case DSM_MOD_VDS:
        DSM_StrPrint(pVersion,"0x%x", (unsigned int)VDS_GetVersion());
        break;
    case DSM_MOD_REG:
       DSM_StrPrint(pVersion,"0x%x",(unsigned int)REG_GetVersion());
        break;
    case DSM_MOD_SMS:
        DSM_StrPrint(pVersion,"0x%x", (unsigned int)SMS_DM_GetVersion());
        break;
    case DSM_MOD_FAT:
        DSM_StrPrint(pVersion,"0x%x", (unsigned int)FS_GetVersion() & 0xffff0000);
        break;
        
    case DSM_MOD_VFS:
        DSM_StrPrint(pVersion,"0x%x", (unsigned int)FS_GetVersion() & 0xffff);
    default:
        break;
    }

    return;
}

#endif
