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
#include "fs.h"
#include "dsm_dbg.h"
#include "fat_base.h"
#include "exfat_base.h"
#include "fat_local.h"
#include "exfat_local.h"
#include "fs_base.h"
#include "hal_host.h"

//#include <string.h>
//#include <stdlib.h>
extern unsigned long strtoul(const char *nptr, char **endptr, int base);
#define LOD_BUF_LINE_SIZE       50

static FS_LodPacket* LOD_AllocateLodPacket(FS_LodPacket *packet, unsigned int address,unsigned int nbWords);


INT32 SRVAPI    FS_LodGetNextPacket(INT32 fd,FS_LodPacket* packet)
{
    char buf[LOD_BUF_LINE_SIZE];

    //Find the start
    bool started = false;
    unsigned int addr;
    int nbwords = 0;// countPacketWords(fd);
    int i;
    
        //hal_HstSendEvent(SYS_EVENT, 0x10d00010);
        packet->address = 0;
    while(!started)
    {
             //hal_HstSendEvent(SYS_EVENT, 0x10d00011);
        if(!FS_GetLine(buf,LOD_BUF_LINE_SIZE,fd))
             {
            return ERR_FS_READ_FILE_EXCEED;
              }
        if(buf[0]=='@')
        {
            started = true;
            addr = strtoul(buf+1,0,16);
            break;
        }
        //else, ok, it's a comment or an empty line
    }

       // hal_HstSendEvent(SYS_EVENT, 0x10d00020);

    LOD_AllocateLodPacket(packet, addr,nbwords);
    for(i=0;;i++)
    {
        if(FS_GetLine(buf,12,fd) == NULL)
                  break;
              if(buf[0] == '@')
              {
                  FS_GetLine(NULL, -DSM_StrLen(buf), fd);
                  break;
              }
              if(buf[0] == '#')
              {
                  break;
              }
        packet->words[i] = strtoul(buf,0,16);
    }
       nbwords = i;
       packet->nbWords = nbwords;
       //hal_HstSendEvent(SYS_EVENT, 0x10d00030);
       //hal_HstSendEvent(SYS_EVENT, nbwords);

       CSW_TRACE(BASE_FFS_TS_ID,  "FS_LodGetNextPacket addr=%x, nbwords=%d",addr, nbwords);
    return ERR_SUCCESS;
}

INT32 SRVAPI FS_LodGetUserLodPacket(INT32 inode_lod,FS_LodPacket* packet)
{
    bool started = false;
    unsigned int addr = 0;
    unsigned int size = 0x2000;
    unsigned int base=0x08000000;
    char buf[LOD_BUF_LINE_SIZE];
    int32 fd; 

    fd = FS_OpenDirect(inode_lod, FS_O_RDONLY, 0);
    hal_HstSendEvent(SYS_EVENT, 0x10d000e0);
    packet->address = 0;
    while(!started)
    {
        if(!FS_GetLine(buf,LOD_BUF_LINE_SIZE,fd))
            return ERR_SUCCESS;

        if(DSM_StrNCmp(buf, "#$base", 6)==0)
        {
            base = strtoul(buf+9,0,16);
        }
        if(DSM_StrNCmp(buf, "#$USER_DATA_BASE", 16)==0)
        {
            addr = strtoul(buf+19,0,16);
        }
        if(DSM_StrNCmp(buf, "#$USER_DATA_SIZE",16)==0)
        {
            size = strtoul(buf+19,0,16);
            break;
        }
        if(buf[0]=='@')
        {
            break;
        }
        //else, ok, it's a comment or an empty line
    }

    LOD_AllocateLodPacket(packet, base+addr,size/4);
    DSM_MemSet(packet->words, 0xff, size);

    FS_Close(fd);
    
    return ERR_SUCCESS;
}

#if defined(_T_UPGRADE_PROGRAMMER)
uint32 bigbuf[0x1000]; // 16k
#else
extern uint32 bigbuf[0x1000]; // 16k
#endif
static FS_LodPacket* LOD_AllocateLodPacket(FS_LodPacket* packet, unsigned int address,unsigned int nbWords)
{

        packet->nbWords = nbWords;
        packet->address = address;
        packet->words = (UINT32*)bigbuf;

        return packet;
}



INT32 FS_GetLodHeader(int32 inode_lod, FS_LodHeader *header)
{
    bool started = false;
    //unsigned int addr = 0;
    //unsigned int size = 0x2000;
    //unsigned int base=0x08000000;
    char buf[LOD_BUF_LINE_SIZE];
    INT32 fd;

    hal_HstSendEvent(BOOT_EVENT, 0x11111111);

    fd = FS_OpenDirect(inode_lod, FS_O_RDONLY, 0);

    hal_HstSendEvent(BOOT_EVENT, fd);

    while(!started)
    {
        if(!FS_GetLine(buf,LOD_BUF_LINE_SIZE,fd))
            return ERR_SUCCESS;

        if(DSM_StrNCmp(buf, "#$base", 6)==0)
        {
           header->base = strtoul(buf+9,0,16);
           
           hal_HstSendEvent(BOOT_EVENT, header->base);

        }
        #if 0
        if(DSM_StrNCmp(buf, "#$USER_DATA_BASE", 16)==0)
        {
            header->user_data_base = strtoul(buf+19,0,16);
        }
        if(DSM_StrNCmp(buf, "#$USER_DATA_SIZE",16)==0)
        {
            header->user_data_size= strtoul(buf+19,0,16);
            break;
        }
        #endif
        if(buf[0]=='@')
        {
            break;
        }
        //else, ok, it's a comment or an empty line
    }

    FS_Close(fd);
       
    return ERR_SUCCESS;
}
