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
#ifndef _EXFAT_DIR_H

#define _EXFAT_DIR_H

//#define EXFAT_DIR_INODE_CACHE_SIZE              10
#define FS_ROOT_INO                                            0xffffffffffffffff
#define EXFAT_INODE_CACHE_TYPE                      UINT64


typedef struct _EXFAT_DIR_CACHE
{
    UINT8 len;
    UINT8 head;
    UINT8 tail;
    UINT8 nop;
    EXFAT_INODE_CACHE_TYPE *inodeCache;
} EXFAT_DIR_CACHE;


INT32 exfat_trunc_fatentry(struct inode* pinode, UINT32 lst_clus,UINT32 trunc_clus);

INT32 exfat_remove_entry(struct inode *pinode, UINT32 bclu, UINT16 bsec, 
            UINT16 eoff, BOOL bRename);

INT32 exfat_get_null_entry_ino(struct inode *diri,UINT32 ecount,UINT64 *ino,
            UINT32 *nullcount);

VOID exfat_make_entries(EXFAT_SB_INFO* sb_info,WCHAR* pszFileName,UINT16 attrib,
            UINT64 data_len, EXFAT_DIR_ENTRY* pdentries);

INT32 exfat_add_entry(struct inode *diri, EXFAT_DIR_ENTRY* pdentries,
            UINT32 fst_clus,UINT32* bclu, UINT16 *bsec, UINT16 *eoff);

INT32 exfat_read_entries(struct inode *pinode,  UINT32 bclu,
                UINT16 bsec, UINT16 eoff, EXFAT_DIR_ENTRY* dir_entry, UINT16* ecount);

INT32 exfat_write_entries(struct inode *pinode,
            UINT32 bclu, UINT16 bsec, UINT16 eoff, EXFAT_DIR_ENTRY* dentry, UINT16 ecount);

INT32 exfat_lookup_file(struct inode *inode, 
                                           WCHAR* filefilter,
                                           UINT64 *cur_ino,
                                           UINT64 *nex_ino,
                                           WCHAR* filename,
                                           UINT16* ecount);

INT32 exfat_get_name_for_entrys(struct inode *pinode, WCHAR* filename, UINT32 bclu, 
            UINT16 bsec, UINT16 eoff);

BOOL exfat_checkFileAttributes(UINT8 type, UINT16 attr);

BOOL exfat_checkFileFilter(WCHAR* fileName, WCHAR* fileFilter);

INT32 exfat_lookup_dir(struct inode *inode, 
                                           WCHAR* filefilter,
                                           UINT64 *cur_ino,
                                           UINT64 *nex_ino,
                                           WCHAR* filename,
                                           UINT8 type,
                                           UINT16* ecount,
                                           UINT32* total_count,
                                           UINT8 mode);

PUBLIC VOID exfat_init_dir_cache(VOID);

INT32 exfat_search_dir_parent_cache(UINT64 *curInode, UINT64 *parentInode);

INT32 exfat_update_dir_cache(UINT64 curInode);
INT32 exfat_set_dir_cache(UINT64 *dirCache, UINT8 size, UINT8 length, UINT8 head, UINT8 tail);
INT32 exfat_get_dir_cache(UINT64 *dirCache, UINT8 size, UINT8 *length, UINT8 *head, UINT8 *tail);

#endif

