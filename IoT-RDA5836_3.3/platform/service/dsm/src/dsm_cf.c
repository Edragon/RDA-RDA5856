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

#ifdef _FS_SIMULATOR_

//#if (defined _DSM_SIMUFLASH_FILE)

INT32 CSW_TRACE(UINT32 id, CONST INT8 *fmt, ...)
{
    INT32 count;
    INT8 buf[1024];   
    static UINT8 is_first = 1;
    va_list ap;
#if WIN32
    INT32 fp = 0;
#else
    FILE* fp = NULL;       
#endif    

    va_start (ap, fmt);		
    if((count = vsprintf(buf, fmt, ap)) > 1022)
    {
         DSM_Assert(0, "count %d overflow%x\n", count);
    }
    va_end (ap);
    strcat(buf,"\n");
    printf("%s\n", buf);
    
#if WIN32
    if(is_first == 1)
    {
       fp = _open("log.txt",_O_WRONLY | _O_CREAT, _S_IREAD|_S_IWRITE);
       is_first = 0;
    }
    else
    {
       fp = _open("log.txt",_O_WRONLY |_O_APPEND, _S_IREAD|_S_IWRITE);
    }
    _write(fp,buf,strlen(buf));
    //_commit(fp);
    _close(fp);
    fp = 0;
#else
    if(is_first == 1)
    {
        fp = fopen("log.txt","w+");
        is_first = 0;
    }
    else
    {
        fp = fopen("log.txt","a+");
    }
    if(fp)
    {
       fwrite(buf,1,strlen(buf),fp);       
       fclose(fp);
       fp = 0;
    }
    else
    {
       printf("open log.txt file failed.");
    }    

#endif    
    return count;
}
//#endif

VOID CSW_TC_MEMBLOCK(UINT16 uMask, UINT8 *buffer, UINT16 len, UINT8 radix)
{
    uMask = uMask;
    buffer = buffer;
    len = len;
    radix = radix;
}

VOID DSM_Assert(BOOL ex,CONST CHAR* format, ...)
{
    INT8 buf[256];
    INT8 * arg = NULL;
    

    if(!ex)
    {
        arg = (INT8 *)((INT8*)(&format) + 4);
        vsprintf(buf, format, arg);
        strcat(buf,"\n");
        printf("%s",buf);    
        assert(0);
    }
}


char * strupr2(char * s)
{
    
    if(s == NULL)
    {
        return NULL;
    }

    while(*s)
    {
        if(*s >= 'a' && *s <= 'z')
        {
            *s -= 0x20;
        }
        
        s++;
    }

    return s;
}

char * strlwr2(char * s)
{
     
    if(s == NULL)
    {
        return NULL;
    }

    while(*s)
    {
        if(*s >= 'A' && *s <= 'Z')
        {
            *s += 0x20;
        }
        
        s++;
    }

    return s;
}

char *strrchr2(const char *s, int c)
{
  const char *found, *p;

  c = (unsigned char) c;

  if (c == '\0')
  { 
      return strchr (s, '\0');
  }
  
  found = NULL;
  while ((p = strchr (s, c)) != NULL)
    {
      found = p;
      s = p + 1;
    }

  return (char *) found;
}


UINT16 DSM_GetTime(VOID)
{
	
    struct tm* pTm;
    time_t lTime;
    UINT16 iTime;
 
    lTime = time(&lTime);
	pTm = localtime(&lTime);
	// time=hours*2048+minutes*32+second/2
	iTime = (UINT16)((pTm->tm_hour * 2048) + (pTm->tm_min * 32) + pTm->tm_sec/2); 
	return iTime;
}


UINT16 DSM_GetData(VOID)
{	
	struct tm* pTm;
    time_t lTime;
    UINT16 iDate;

	lTime = time(&lTime);
	pTm = localtime(&lTime);
	// date=(year-1980)*512+month*32+day
	iDate = (UINT16)((pTm->tm_year + 1900 -1980)*512 + ((pTm->tm_mon + 1)*32) + pTm->tm_mday); 
	return iDate;

}

UINT32 DSM_UniteFileTime(UINT16 iDate,UINT16 iTime,UINT8 iTimeTeenth)
{
   
    time_t iTime_t = 0;
    struct tm     sTm;

    iTimeTeenth = iTimeTeenth;
    if(0 < iDate)
    {
        sTm.tm_year = (UINT32)((iDate >> 9) + 1980 - 1900);
        sTm.tm_mon = (UINT32)(((iDate>>5) & 0x0f) -1);
        sTm.tm_mday = (UINT32)(iDate & 0x1f);
    }
    else
    {
        sTm.tm_year = 0;
        sTm.tm_mon = 0;
        sTm.tm_mday = 0;
    }
    if(0 < iTime)
    {    
        sTm.tm_hour = (UINT32)(iTime >> 11);
        sTm.tm_min = (UINT32)((iTime>> 5) & 0x3f);
        sTm.tm_sec = (UINT32)((iTime & 0x1f) * 2);
        sTm.tm_isdst = 0;
    }
    else
    {            
        sTm.tm_hour = 0;
        sTm.tm_min = 0;
        sTm.tm_sec = 0;
        sTm.tm_isdst = 0;    
    }
    
    if(0 < iDate || 0 < iTime)
    {
        iTime_t =  mktime(&sTm);
    }
    
    return iTime_t;
}

UINT32 DSM_GetFileTime(VOID)
{
   
    return time(NULL);
    
}


UINT16 DSM_GetFileTime2Time(UINT32 iFileTime)
{
    struct tm* pTm;
    time_t FileTime;
    UINT16 iTime;

    FileTime = iFileTime; 
    pTm = localtime(&FileTime);
    // time=hours*2048+minutes*32+second/2
    iTime = (UINT16)((pTm->tm_hour * 2048) + (pTm->tm_min * 32) + pTm->tm_sec/2); 
    return iTime;
}

UINT16 DSM_GetFileTime2Date(UINT32 iFileTime)
{
    struct tm* pTm;
    time_t FileTime;
    UINT16 iDate;
    
    FileTime = iFileTime; 
    pTm = localtime(&FileTime);
    // date=(year-1980)*512+month*32+day
    iDate = (UINT16)((pTm->tm_year + 1900 -1980)*512 + ((pTm->tm_mon + 1)*32) + pTm->tm_mday); 
    return iDate;    
}
UINT8 DSM_GetTimeTeenth(VOID)
{	
	return 0;
}
#ifdef USER_DATA_CACHE_SUPPORT
typedef INT32 ( *CallBack_Funcp) (VOID);

typedef struct 
{
    CallBack_Funcp pFlush;
     CallBack_Funcp pFlushAll;
}VDS_SIMU_PROFILE;

VDS_SIMU_PROFILE g_vds_simu_profile;


VOID hal_BootRegisterBeforeMonCallback(INT32 (*funcPtr)(VOID))
{
    g_vds_simu_profile.pFlushAll = (CallBack_Funcp)funcPtr;
}

void sxs_RegisterFsIdleHookFunc(void (*funcPtr)(void))
{
    g_vds_simu_profile.pFlush = (CallBack_Funcp)funcPtr;
}



BOOL hal_DmaDone()
{
    return TRUE;
}

void hal_AudForcePowerDown()
{
    
}

void hal_TimWatchDogClose()
{
    
}

BOOL hal_LpsRemainingFramesReady()
{
    return TRUE;
}

BOOL hal_GetVocDma()
{
    return TRUE;
}
BOOL tgt_FlushFactorySettings(VOID)
{
   return TRUE;
}
#endif
#else

UINT16 DSM_GetTime(VOID)
{
	
    TM_SYSTEMTIME sSysTm;
    UINT16 iTime;
 
    TM_GetSystemTime(&sSysTm);
	
	// time=hours*2048+minutes*32+second/2
	iTime = (UINT16)((sSysTm.uHour * 2048) + (sSysTm.uMinute * 32) + sSysTm.uSecond/2); 
	return iTime;
}


UINT16 DSM_GetData(VOID)
{	
	TM_SYSTEMTIME sSysTm;
    UINT16 iDate;

	TM_GetSystemTime(&sSysTm);
	// date=(year-1980)*512+month*32+day
	iDate = (UINT16)((sSysTm.uYear - 1980)*512 + ((sSysTm.uMonth)*32) + sSysTm.uDay); 
	return iDate;
}

UINT8 DSM_GetTimeTeenth(VOID)
{	
	TM_SYSTEMTIME sSysTm;
        UINT8 iTimeTeenth;

	TM_GetSystemTime(&sSysTm);
	// date=(year-1980)*512+month*32+day
	iTimeTeenth = (UINT8)sSysTm.uMilliseconds*2/10; 
	return iTimeTeenth;
}

UINT32 DSM_UniteFileTime(UINT16 iDate,UINT16 iTime,UINT8 iTimeTeenth)
{
    TM_FILETIME sFileTime;  
    TM_SYSTEMTIME sSysTm;

    sFileTime.DateTime = 0;
    if(0 < iDate)
    {
        sSysTm.uYear = (UINT32)((iDate >> 9) + 1980 );
        sSysTm.uMonth = (UINT32)(((iDate>>5) & 0x0f));
        sSysTm.uDay = (UINT32)(iDate & 0x1f);        
    }
    else
    {
        sSysTm.uYear = 0;
        sSysTm.uMonth = 0;
        sSysTm.uDay = 0;
    }
    if(0 < iTime)
    {
        sSysTm.uHour = (UINT32)(iTime >> 11);
        sSysTm.uMinute = (UINT32)((iTime>> 5) & 0x3f);
        sSysTm.uSecond = (UINT32)((iTime & 0x1f) * 2);
        //sSysTm.uMilliseconds = 0;  
        if(0 < iTimeTeenth)
        {
            sSysTm.uSecond += (UINT8)((iTimeTeenth/100)%2);
        }
    }
    else
    {
        sSysTm.uHour = 0;
        sSysTm.uMinute = 0;
        sSysTm.uSecond = 0;
        sSysTm.uMilliseconds = 0;
    }   

    TM_SystemTimeToFileTime(&sSysTm,&sFileTime);
    
    return sFileTime.DateTime;
}

UINT32 DSM_GetFileTime(VOID)
{
    TM_SYSTEMTIME sSysTm;
    UINT32 iFileTime;
    UINT16 iTime;
    UINT16 iDate;
    UINT8 iTimeTeenth;
    
    TM_GetSystemTime(&sSysTm);
    
	// time=hours*2048+minutes*32+second/2
	iTime = (UINT16)((sSysTm.uHour * 2048) + (sSysTm.uMinute * 32) + sSysTm.uSecond/2); 

    // date=(year-1980)*512+month*32+day
	iDate = (UINT16)((sSysTm.uYear - 1980)*512 + ((sSysTm.uMonth)*32) + sSysTm.uDay); 
    
    iTimeTeenth = (UINT8)(sSysTm.uMilliseconds/20);
    iFileTime = DSM_UniteFileTime(iDate,iTime,iTimeTeenth);
	return iFileTime;    
}


UINT16 DSM_GetFileTime2Date(UINT32 iFileTime)
{
    BOOL bRet;
    UINT16 iDate;
    TM_FILETIME sFileTime;  
    TM_SYSTEMTIME sSysTm;
    
    sFileTime.DateTime = iFileTime;
    bRet = TM_FileTimeToSystemTime(sFileTime,&sSysTm);
    if(TRUE == bRet)
    {
        // date=(year-1980)*512+month*32+day
        iDate = (UINT16)((sSysTm.uYear - 1980)*512 + ((sSysTm.uMonth)*32) + sSysTm.uDay); 
	
    }
    else
    {
        iDate = 0;
    }
    return iDate;
    
}


UINT16 DSM_GetFileTime2Time(UINT32 iFileTime)
{
    BOOL bRet;
    UINT16 iTime;
    TM_FILETIME sFileTime;  
    TM_SYSTEMTIME sSysTm;
    
    sFileTime.DateTime = iFileTime;
    bRet = TM_FileTimeToSystemTime(sFileTime,&sSysTm);
    if(TRUE == bRet)
    {
        // time=hours*2048+minutes*32+second/2
	    iTime = (UINT16)((sSysTm.uHour * 2048) + (sSysTm.uMinute * 32) + sSysTm.uSecond/2); 
	
    }
    else
    {
        iTime = 0;
    }
    return iTime;
}

void DSM_GetFileDateTime(UINT16 *date, UINT16 *time)
{
    UINT32 fileTime;

    fileTime = DSM_GetFileTime();
    if(date)
        *date = DSM_GetFileTime2Date(fileTime);
    if(time)
        *time = DSM_GetFileTime2Time(fileTime);
}

UINT8* itoa( INT32 value, UINT8* string, UINT32 radix )
{
    if(NULL == string)
    {
        return NULL;
    }
    if(10 == radix)
    {
        DSM_StrPrint(string,"%d",value);
    }
    else if(16 == radix)
    {
        DSM_StrPrint(string,"%x",value);
    }
    else
    {
        return NULL;
    }
    return string;
}


UINT32 DSM_GetRandNum(VOID)
{
    UINT32 rand_value = 0;
    
    rand_value = DSM_GetFileTime();
    return rand_value;
}
#endif

UINT8* ui64toa(UINT64  value,UINT8* string,UINT32 radix)
{
    UINT8 tmp;
    INT32 i,len;
    UINT32 high,low;
    UINT8* p = NULL;
    UINT8* q = NULL;

    p = string;
    q = string;
	
    if(NULL == string)
    {
        return NULL;
    }

    if(10 == radix)
    {
        for(i = 0; i < 32; i++)
        {
            *p = (char)(((UINT64)value%(UINT64)10) + 48);		    

            value /= 10;
            p ++;
            if(value == 0)
            {
                break;
            }
        }
        *p = 0;
        len = i;

        p--;
        for(i = 0; i < len; i++)
        {
            if(p < q)
            {
                break;
            }
            tmp = *q;
            *q = *p;
            *p = tmp;
            q ++;
            p --;
        }
    }
    else if(16 == radix)
    {
        high = (UINT32)(value>>32);
        low = (UINT32)(value & 0xffffffff);
        DSM_StrPrint((INT8*)string,"0x%x%x",(unsigned int)high,(unsigned int)low);
    }
    else
    {
        return NULL;
    }
    return string;
}



// Get the unicode string length.
UINT16 DSM_UnicodeLen(UINT8* pszUnicode)
{
    UINT16 i;
    for(i = 0;;i+=2)
    {
        if(0 == pszUnicode[i] && 0 == pszUnicode[i + 1])
        {
            break;
        }
    }
    
    return i;
}

VOID DSM_Dump(UINT8* pBuff,UINT32 iTotalSize,INT32 iLineSize)
{
    UINT8* p;
    UINT32 i;
    UINT32 iOutLen = 0;
    UINT8* pTmp = NULL;
  
    UINT8 szHex[16];

    p = pBuff;
    if(p == NULL)
   {
      return;
   }
    pTmp = DSM_MAlloc((iLineSize + 2)*5);
    if(NULL == pTmp)
    {
         DSM_ASSERT(0,"DSM_Dump malloc failed. size = 0x%x",((iLineSize + 2)*5));
    }
    DSM_MemSet(pTmp,0,(iLineSize + 2)*5);
    for(i = 0;i < iTotalSize; i++)
    {
        if(((i % iLineSize) == 0 && i != 0))
        {
            //DSM_StrPrint(pTmp, "%s ",pTmp);
            CSW_TRACE(BASE_DSM_TS_ID, "%s",pTmp);  
            DSM_MemSet(pTmp,0,(iLineSize + 2)*5);            
            DSM_StrCpy(pTmp,"\n");
            iOutLen += iLineSize;
        }
        DSM_MemSet((INT8*)szHex, 0,16);
        DSM_StrPrint((INT8*)szHex, "0x%02x ",*p);
        DSM_StrCat(pTmp,szHex);
        
        p++;
    }

    if(iOutLen < iTotalSize)
    {
        //DSM_StrPrint(pTmp, "%s ",pTmp);
        CSW_TRACE(BASE_DSM_TS_ID, "%s",pTmp);       
  
    }

    CSW_TRACE(BASE_DSM_TS_ID, "\n");
    DSM_Free(pTmp);
}

#define DSM_BTOH( b ) (( b ) <= 9 ? (( b ) + '0' ) : (( b ) - 10 + 'A' ))
void DSM_Hex2Str(UINT8* hex,INT32 len,UINT8* bufout)
{
    UINT8* p = bufout;
    
    while ( len )
    {
        *p = (UINT8)DSM_BTOH( ( ( UINT8 ) * hex ) >> 4 );
        *(p + 1 ) = (UINT8)DSM_BTOH( ( ( UINT8 ) * hex ) & 0xf );
        len--;
        hex++;
        
        *( p + 2 ) = ' ';
        p += 3;        
    }
    *p = '\0';
    *(p-1) = 0;
}

#ifdef FS_PRIVATE_MEM_MANAGE

#define FORMAT_STRING "%-36s: %-8d[0x%-8x] ( %-4dM %-4dK %-4dB)"

struct mem_free_blk
{
    int       nunits;
    struct mem_free_blk* next;
};

struct ext_mem_free_blk
{
    int       nunits;
    struct mem_free_blk* next;
    int      max_alloced;
    int      cur_alloced;
    int      max_counter;
    int      cur_counter;
};

#define MEM_UNIT_SIZE sizeof( struct mem_free_blk )


struct mem_free_blk * g_fs_mm_free_list = NULL;
struct mem_free_blk g_fs_mm_blocks[ 24*1024] = { { 0, NULL }, }; 
struct ext_mem_free_blk g_fs_mm_base;

// TODO: HANDLE g_fs_mm_sem;  // 必要时需要加上信号量

void get_mega_kilo_byte( int n, int *mega, int *kilo, int *mybyte )
{
    *mega = *kilo = *mybyte = 0;

    if ( n > 1024 * 1024 )
        *mega = n / (1024 * 1024);
    n -= (*mega) * ( 1024 * 1024 );
    if ( n > 1024 )
        *kilo = n / 1024;
    n -= (*kilo) * 1024;
    *mybyte = n;
}

void* private_malloc( UINT32 size )
{
    int nunits;
    struct mem_free_blk **pp;
    struct mem_free_blk *p;
    int mega = 0, kilo = 0, mybyte = 0;
    int remained = 0;
    
    if ( 0 == size )
        return NULL;

    // TODO: down( &g_fs_mm_sem );
    
    if ( !g_fs_mm_free_list )
    {
        g_fs_mm_free_list = (struct mem_free_blk*)&g_fs_mm_base;
        g_fs_mm_base.nunits = 0;
        g_fs_mm_base.next = g_fs_mm_blocks;
        g_fs_mm_base.max_alloced = g_fs_mm_base.cur_alloced = 0;
        g_fs_mm_base.cur_counter = 0;
        g_fs_mm_base.max_counter = 0;
        g_fs_mm_blocks[ 0 ].next = (struct mem_free_blk*)&g_fs_mm_base;
        g_fs_mm_blocks[ 0 ].nunits = sizeof(g_fs_mm_blocks) / sizeof(*g_fs_mm_blocks);
    }
    
    nunits =( ( size - 1 ) / MEM_UNIT_SIZE + 1 ) + 1/* for header itself */;
    
    for ( pp = &(g_fs_mm_free_list->next); *pp != g_fs_mm_free_list; pp = &( (*pp)->next)  )
    {
        if ( (*pp)->next != g_fs_mm_free_list && *pp > (*pp)->next )
        {
            CSW_TRACE(BASE_DSM_TS_ID,"disordered list!!!!!!!!!!!!!!\n");
            goto failed;
        }

        if( (*pp)->nunits >= nunits )
        {
            if ( (*pp)->nunits == nunits )
            {
                p = *pp;
                *pp = p->next;
            }
            else
            {
                (*pp)->nunits -= nunits;
                p = *pp + (*pp)->nunits;
                p->nunits = nunits;
            }
            
            p->next = (struct mem_free_blk *)0xDEADBEEF;
            g_fs_mm_base.cur_counter++;
            if ( g_fs_mm_base.cur_counter > g_fs_mm_base.max_counter )
                g_fs_mm_base.max_counter = g_fs_mm_base.cur_counter;

            g_fs_mm_base.cur_alloced += (nunits - 1 ) * MEM_UNIT_SIZE;
            if ( g_fs_mm_base.cur_alloced > g_fs_mm_base.max_alloced )
                g_fs_mm_base.max_alloced = g_fs_mm_base.cur_alloced;
            
            // TODO: up( &g_fs_mm_sem );
            return ( void* )( p + 1 );
        }
    }
failed:    

    CSW_TRACE(BASE_DSM_TS_ID, "memory: 0x%p --> 0x%p\n",
        g_fs_mm_blocks, (char*)g_fs_mm_blocks + sizeof( g_fs_mm_blocks ));

    for ( p = g_fs_mm_free_list->next; p != g_fs_mm_free_list; p = p->next  )
    {
        remained += p->nunits * MEM_UNIT_SIZE;
        CSW_TRACE(BASE_DSM_TS_ID,"freed block(offset: %08p,  size: %04d)\n", 
            p, p->nunits * MEM_UNIT_SIZE);
    }
    CSW_TRACE(BASE_DSM_TS_ID,"remained memory: %d\n", remained);

    for ( p = g_fs_mm_free_list->next; p != g_fs_mm_free_list; p = p->next  )
    {
        if ( p->next != g_fs_mm_free_list )
        {
            CSW_TRACE(BASE_DSM_TS_ID,"fragment   (offset: %08p,  size: %04d)\n", 
                p + p->nunits, ( p->next - p ) * MEM_UNIT_SIZE);
        }
    }
    
    get_mega_kilo_byte( g_fs_mm_base.cur_alloced, &mega, &kilo, &mybyte );
    CSW_TRACE(BASE_DSM_TS_ID,FORMAT_STRING"\n", "static memory allocated",
                     g_fs_mm_base.cur_alloced, g_fs_mm_base.cur_alloced, mega, kilo, mybyte );

    get_mega_kilo_byte( g_fs_mm_base.max_alloced, &mega, &kilo, &mybyte );
    CSW_TRACE(BASE_DSM_TS_ID, FORMAT_STRING"\n", "maximum static memory allocated",
                     g_fs_mm_base.max_alloced, g_fs_mm_base.max_alloced, mega, kilo, mybyte );

    CSW_TRACE(BASE_DSM_TS_ID,"current counter [%d]\n", g_fs_mm_base.cur_counter);
    CSW_TRACE(BASE_DSM_TS_ID,"maximum counter [%d]\n", g_fs_mm_base.max_counter );

    get_mega_kilo_byte(  MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, &mega, &kilo, &mybyte );
    CSW_TRACE(BASE_DSM_TS_ID, FORMAT_STRING"\n", "current control info allocated",
        MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, mega, kilo, mybyte );

    get_mega_kilo_byte(  MEM_UNIT_SIZE * g_fs_mm_base.max_counter, &mega, &kilo, &mybyte );
    CSW_TRACE(BASE_DSM_TS_ID, FORMAT_STRING"\n", "maximum control infoallocated",
        MEM_UNIT_SIZE * g_fs_mm_base.max_counter, MEM_UNIT_SIZE * g_fs_mm_base.max_counter, mega, kilo, mybyte );

    // TODO: up( &g_fs_mm_sem );
    return NULL;
}


void private_free( void *ptr )
{
    struct mem_free_blk *bp = ( struct mem_free_blk *)ptr - 1;
    struct mem_free_blk *p, *prev;
    int     size = ( bp->nunits - 1 ) * MEM_UNIT_SIZE;

    // TODO: down( &g_fs_mm_sem );
    if ( g_fs_mm_free_list == g_fs_mm_free_list->next )
    {
        CSW_TRACE(BASE_DSM_TS_ID,"free list is empty\n" );
        g_fs_mm_free_list->next = bp;
        bp->next = g_fs_mm_free_list;
    }
    else
    {
        for ( prev = g_fs_mm_free_list, p = prev->next; p != g_fs_mm_free_list && p + p->nunits <= bp; prev = p, p =  p->next )
        {
            if(p == bp)
            {
                CSW_TRACE(BASE_DSM_TS_ID, "===========ASSERT===========\n");
                CSW_TRACE(BASE_DSM_TS_ID,"ASSERT: [%s][%d]\n", __FILE__, __LINE__ );
                CSW_TRACE(BASE_DSM_TS_ID, "===========ASSERT===========\n");
                
                return;
            }
            //FS_ASSERT_RWV( p != bp );
        }

        if ( p != g_fs_mm_free_list )
        {
            if ( bp + bp->nunits == p )
            {
                bp->nunits += p->nunits;
                bp->next = p->next;
            }
            else
            {
                bp->next = p;
            }
            
            if ( bp == prev + prev->nunits )
            {
                prev->nunits += bp->nunits;
                prev->next = bp->next;
            }
            else 
            {
                prev->next = bp;
            }
        }
        else
        {
            if ( bp == prev + prev->nunits )
            {
                prev->nunits += bp->nunits;
            }
            else
            {
                bp->next = prev->next;
                prev->next = bp;
            }
        }
    }

    g_fs_mm_base.cur_counter--;
    g_fs_mm_base.cur_alloced -= size;
    
    // TODO: up( &g_fs_mm_sem );

    if(!(g_fs_mm_base.cur_alloced < g_fs_mm_base.max_alloced))
    {
        CSW_TRACE(BASE_DSM_TS_ID, "===========ASSERT===========\n" );
        CSW_TRACE(BASE_DSM_TS_ID, "ASSERT: [%s][%d]\n", __FILE__, __LINE__ );
        CSW_TRACE(BASE_DSM_TS_ID, "===========ASSERT===========\n" );
        
        return;
    }
    
    // FS_ASSERT_RWV( g_fs_mm_base.cur_alloced < g_fs_mm_base.max_alloced );
}

#endif // #ifdef  FS_PRIVATE_MEM_MANAGE


