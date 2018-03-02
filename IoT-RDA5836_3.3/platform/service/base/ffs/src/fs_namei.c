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
#include "fat_base.h"
#include "exfat_base.h"
#include "fat_local.h"
#include "exfat_local.h"
#include "fs.h"
#include "fs_base.h"
#include "exfat_dir.h"
#include "fs_namei.h"



extern struct task_struct g_fs_current;
extern struct file *g_fs_file_list;
INT32 g_tstFsLocalErrCode = 0;


// Function:
// Chech the file name if valid.It is allowable that with full path.
// This function will check the per directory name or file name.it is't allowable that include wildcard character in the directory,
// the last name include wildcard character is allowed when input parameter bIgnoreBaseName is TRUE.
// Parameter:
// pathname: The path name in unicode format.
// bIgnoreBaseName: The flag of ignore base name. if it is TRUE, allow filename is NULL or include wildcard characters,else
//            not allow.
// Return
//   The path name is valid,return TRUE.
//   The path name is invalid, return FALSE.
BOOL is_file_name_valid( PCWSTR pathname, BOOL bIgnoreBaseName )
{
    UINT32 dir_depth = 0;
    UINT32 name_len = 0;
    UINT16 *p = pathname;
    BOOL bLoop = TRUE;
    BOOL bRet = FALSE;
    FS_NV_STATUE  eNVStatue = FS_NV_INIT;

    while(TRUE == bLoop)
    {
        switch(eNVStatue)
        {
            case FS_NV_INIT:
                if(IS_TERMINAL(*p))
                {
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    D((DL_FATERROR, "in is_file_name_valid,file name is null."));
                    continue;
                }
                else if(IS_NV_INVALID(*p))
                {
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    D((DL_FATERROR, "in is_file_name_valid,have invalid character:0x%x.", *p));
                    continue;
                }
                else if(IS_NV_PART(*p))
                {
                    eNVStatue =  FS_NV_PARTITIOIN;
                    dir_depth ++;

                }
                else if(IS_NV_WILDCARD(*p))
                {
                    eNVStatue =  FS_NV_WILDCARD_ENTRY;
                }
                else
                {
                    eNVStatue =  FS_NV_BASE_ENTRY;
                }
                break;
            case FS_NV_PARTITIOIN:
                if(dir_depth > FS_NR_DIR_LAYERS_MAX)
                {
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    D((DL_FATERROR, "in is_file_name_valid,the directory too deep.depth = %d.", dir_depth));
                    continue;
                }
                if(IS_TERMINAL(*p))
                {
                    eNVStatue =  FS_NV_NULL_ENTRY_END;
                    continue;
                }
                else if(IS_NV_INVALID(*p))
                {
                    D((DL_FATERROR, "in is_file_name_valid,have invalid character:0x%x.", *p));
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    continue;
                }
                else if(IS_NV_PART(*p))
                {
                    eNVStatue =  FS_NV_PARTITIOIN;
                    dir_depth ++;
                }
                else if(IS_NV_WILDCARD(*p))
                {
                    eNVStatue =  FS_NV_WILDCARD_ENTRY;
                    name_len = 0;
                }
                else
                {
                    eNVStatue =  FS_NV_BASE_ENTRY;
                    name_len = 0;
                }
                break;
            case FS_NV_BASE_ENTRY:
                if(IS_TERMINAL(*p))
                {
                    eNVStatue =  FS_NV_BASE_ENTRY_END;
                    continue;
                }
                else if(IS_NV_INVALID(*p))
                {
                    D((DL_FATERROR, "in is_file_name_valid,have invalid character:0x%x.", *p));
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    continue;
                }
                else if(IS_NV_PART(*p))
                {
                    eNVStatue =  FS_NV_PARTITIOIN;
                    dir_depth ++;
                }
                else if(IS_NV_WILDCARD(*p))
                {

                    name_len ++;
                    if(name_len > FS_FILE_NAME_LEN)
                    {
                        D((DL_FATERROR, "in is_file_name_valid,file name too long.name length = %d.", name_len));
                        eNVStatue =  FS_NV_INVALID_ENTRY_END;
                        continue;
                    }
                    else
                    {
                        eNVStatue =  FS_NV_WILDCARD_ENTRY;
                    }
                }
                else
                {
                    name_len ++;
                    if(name_len > FS_FILE_NAME_LEN)
                    {
                        D((DL_FATERROR, "in is_file_name_valid,file name too long.name length = %d.", name_len));
                        eNVStatue =  FS_NV_INVALID_ENTRY_END;
                        continue;
                    }
                    else
                    {
                        eNVStatue =  FS_NV_BASE_ENTRY;
                    }
                }
                break;
            case FS_NV_WILDCARD_ENTRY:
                if(IS_TERMINAL(*p))
                {
                    eNVStatue =  FS_NV_WILDCARD_ENTRY_END;
                    continue;
                }
                else if(IS_NV_INVALID(*p))
                {
                    D((DL_FATERROR, "in is_file_name_valid,have invalid character:0x%x.", *p));
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    continue;
                }
                else if(IS_NV_PART(*p))
                {
                    D((DL_FATERROR, "in is_file_name_valid,have wildcard character:0x%x in dir name.", *p));
                    eNVStatue =  FS_NV_INVALID_ENTRY_END;
                    continue;
                }
                else
                {
                    name_len ++;
                    if(name_len > FS_FILE_NAME_LEN)
                    {
                        D((DL_FATERROR, "in is_file_name_valid,file name too long.name length = %d.", name_len));
                        eNVStatue =  FS_NV_INVALID_ENTRY_END;
                        continue;
                    }
                    else
                    {
                        eNVStatue =  FS_NV_WILDCARD_ENTRY;
                    }
                }
                break;
            case FS_NV_BASE_ENTRY_END:
                bRet = TRUE;
                bLoop = FALSE;
                continue;
                break;
            case FS_NV_WILDCARD_ENTRY_END:
                if(bIgnoreBaseName == TRUE)
                {
                    bRet = TRUE;
                }
                else
                {
                    D((DL_FATERROR, "in is_file_name_valid,the name include wildcard character,bug the base name not allow ignore."));
                    bRet = FALSE;
                }

                bLoop = FALSE;
                continue;
                break;
            case FS_NV_NULL_ENTRY_END:
                if(bIgnoreBaseName == TRUE)
                {
                    bRet = TRUE;
                }
                else
                {
                    D((DL_FATERROR, "in is_file_name_valid,the file name is null,bug the base name not allow ignore."));
                    bRet = FALSE;
                }
                bLoop = FALSE;
                continue;
                break;
            case FS_NV_INVALID_ENTRY_END:
                bLoop = FALSE;
                bRet = FALSE;
                continue;
                break;
            default:
                bRet = FALSE;
                bLoop = FALSE;
                continue;
                break;
        }
        p++;
    }

    return bRet;

}


// Assume that
//    1. pathname contains one character at least.
//    2. pathname specifies a DIRECTORY file, rather than REGULAR file.

WCHAR *format_dir_name( PCWSTR pathname )
{
    WCHAR *path = NULL;
    UINT16 len;

    len = DSM_UnicodeLen((UINT8 *) pathname );
    if( len > FS_PATH_UNICODE_LEN || len < 2 )
    {
        // DSM_ASSERT(0,"format_dir_name: dir name too long,len = %d.",len);
        return NULL;
    }

    //path = (UINT8 *)DSM_MAlloc(len);
    //path = g_NameBuf2;
    path = NAME_BUF_ALLOC();
    if(NULL == path)
    {
        D((DL_VFSERROR, "in format_dir_name,no more memory."));
        DSM_ASSERT(0, "in format_dir_name,no more memory.");
        return NULL;
    }

    len = len / 2 - 1;
    // if the terminate character is '/' or '\\',append character '*' after it.
    if ( _T_DSM('/') == pathname[ len ] || _T_DSM('\\') == pathname[ len ])
    {
        DSM_TcStrCpy( path, pathname );
        DSM_TcStrCat( path, (PCWSTR)_T_DSM(".") );
        return path;
    }
    else
    {
        DSM_TcStrCpy( path, pathname );
        return path;
    }
}


typedef enum
{
    START, MATCH_CHAR, MATCH_STAR, SEARCH_STRING, MATCH_STRING, DONE, ERROR
} STATE_T;

#define IS_LETTER(c)        ((c >= _T_DSM('A') && c <= _T_DSM('Z')) || (c >= _T_DSM('a') && c <= _T_DSM('z')))
#define IS_MATCH_LETTER_I(c1, c2)   (IS_LETTER(c1) && IS_LETTER(c2)&& (c1 - c2 == 32 || c2 - c1 == 32 || c1 -c2 == 0))


int pattern_match( WCHAR *name, WCHAR *pattern )
{
    WCHAR *p = NULL, *q = NULL;
    UINT16 i = 0;
    STATE_T state = START;

    p = name;
    q = pattern;

    if (DSM_TcStrChr(name, _T_DSM('*')) || DSM_TcStrChr(name, _T_DSM('?')))
    {
        return PATTERN_CMP_UNMATCH;
    }

    while (state != DONE && state != ERROR)
    {
        switch (state)
        {
            case START:
                if (NULL == p || NULL == q)
                {
                    state = ERROR;
                }
                else
                {
                    state = MATCH_CHAR;
                }
                break;
            case MATCH_CHAR:
                if ((*p == *q || IS_MATCH_LETTER_I(*p, *q)) || (_T_DSM('?') == *q && *p != _T_DSM('\0')))
                {
                    if (_T_DSM('\0') == *q && _T_DSM('\0') == *p)
                    {
                        state = DONE;
                    }
                    else
                    {
                        p++, q++;
                    }
                }
                else if (_T_DSM('*') == *q)
                {
                    state = MATCH_STAR;
                }
                else
                {
                    state = ERROR;
                }
                break;
            case MATCH_STAR:
                if (_T_DSM('\0') == *(q + 1))
                {
                    state = DONE;
                }
                else if (_T_DSM('*') == *(q + 1))
                {
                    ++q;
                }
                else
                {
                    i = 1;
                    state = SEARCH_STRING;
                }
                break;
            case SEARCH_STRING:
                if (_T_DSM('\0') == *p)
                {
                    state = ERROR;
                }
                else if (*p != *(q + i) && !IS_MATCH_LETTER_I(*p, *(q + i)) && *(q + i) != _T_DSM('?'))
                {
                    ++p;
                }
                else
                {
                    ++i;
                    state = MATCH_STRING;
                }
                break;
            case MATCH_STRING:
                if (_T_DSM('*') == *(q + i))
                {
                    q += i;
                    p += (i - 1);
                    state = MATCH_STAR;
                }
                else if (_T_DSM('\0') == *(p + i - 1) && *(q + i) != _T_DSM('\0'))
                {
                    state = ERROR;
                }
                else if (*(p + i - 1) == *(q + i) || IS_MATCH_LETTER_I(*(p + i - 1), *(q + i)) || _T_DSM('?') == *(q + i))
                {
                    if (_T_DSM('\0') == *(p + i - 1) && _T_DSM('\0') == *(q + i))
                    {
                        state = DONE;
                    }
                    else
                    {
                        ++i;
                    }
                }
                else
                {
                    i = 1, ++p;
                    state = SEARCH_STRING;
                }
                break;

            case DONE:

            case ERROR:

            default:
                break;
        }
    }

    return (DONE == state ? PATTERN_CMP_MATCH : PATTERN_CMP_UNMATCH);

}


INT32 pattern_match_oem(UINT8 *name, UINT8 *pattern)
{
    UINT8 *p = NULL, *q = NULL;
    UINT16 i = 0;
    STATE_T state = START;

    p = name;
    q = pattern;

    if (DSM_StrChr(name, '*') || DSM_StrChr(name, '?'))
    {
        return PATTERN_CMP_UNMATCH;
    }

    while (state != DONE && state != ERROR)
    {
        switch (state)
        {
            case START:
                if (NULL == p || NULL == q)
                {
                    state = ERROR;
                }
                else
                {
                    state = MATCH_CHAR;
                }
                break;
            case MATCH_CHAR:
                if (*p == *q || IS_MATCH_LETTER_I(*p, *q) ||  ('?' == *q && *p != '\0'))
                {
                    if (*p == *q && '\0' == *p)
                    {
                        state = DONE;
                    }
                    else
                    {
                        p++, q++;
                    }
                }
                else if ('*' == *q)
                {
                    state = MATCH_STAR;
                }
                else
                {
                    state = ERROR;
                }
                break;
            case MATCH_STAR:
                if ('\0' == *(q + 1))
                {
                    state = DONE;
                }
                else if ('*' == *(q + 1))
                {
                    ++q;
                }
                else
                {
                    i = 1;
                    state = SEARCH_STRING;
                }
                break;
            case SEARCH_STRING:
                if ('\0' == *p)
                {
                    state = ERROR;
                }
                else if (*p != *(q + i) && *(q + i) != '?')
                {
                    ++p;
                }
                else // *(p + j) == *(q + i) || *(q + i) == '?'
                {
                    ++i;
                    state = MATCH_STRING;
                }
                break;
            case MATCH_STRING:
                if ('*' == *(q + i))
                {
                    q += i;
                    p += (i - 1);
                    state = MATCH_STAR;
                }
                else if ('\0' == *(p + i - 1) && *(q + i) != '\0')
                {
                    state = ERROR;
                }
                else if (*(p + i - 1) == *(q + i) || IS_MATCH_LETTER_I(*(p + i - 1), *(q + i)) || '?' == *(q + i))
                {
                    if ('\0' == *(p + i - 1) && '\0' == *(q + i))
                    {
                        state = DONE;
                    }
                    else
                    {
                        ++i;
                    }
                }
                else// (*(p + i - 1) != *(q + i))
                {
                    i = 1, ++p;
                    state = SEARCH_STRING;
                }
                break;

            case DONE:

            case ERROR:

            default:
                break;
        }
    }

    return (DONE == state ? PATTERN_CMP_MATCH : PATTERN_CMP_UNMATCH);
    
}


int ext_match(const UINT8 *name, const UINT8 *pattern )
{
    int i, j;
    BOOL match = TRUE;

	if(pattern == NULL)
		return PATTERN_CMP_MATCH;

    for(i=0, j=0; i<DSM_StrLen(pattern); i++, j++)
    {
        if(pattern[i] == '.')
        {
            if(match)
                break;
            match = TRUE;
            j = 0;
			i++;
        }
        if(pattern[i] != name[j] && pattern[i]+0x20 != name[j])
            match = FALSE;
    }
	if(match)
		return PATTERN_CMP_MATCH;
	else
	    return PATTERN_CMP_UNMATCH;
}

UINT8 ext_match_unicode(const UINT16 *name, const UINT8 *pattern )
{
    int i, j;
    BOOL match = TRUE;

	if(pattern == NULL)
		return PATTERN_CMP_MATCH;

    for(i=0, j=0; i<DSM_StrLen(pattern); i++, j++)
    {
        if(pattern[i] == '.')
        {
            if(match)
                break;
            match = TRUE;
            j = 0;
			i++;
        }
        if(pattern[i] != (UINT8)(name[j]&0xff) && pattern[i]+0x20 != (UINT8)(name[j]&0xff))
            match = FALSE;
    }
	if(match)
		return PATTERN_CMP_MATCH;
	else
	    return PATTERN_CMP_UNMATCH;
}

/*
 *do_find_entry()
 *
 * finds an entry in the specified directory with the wanted name.
 *
 * when the entry is found, we INCREASE REFERENCE COUNT of the inode
 * associated with the entry.
 *
 * if entry's name to be found is ".", DOT_FOUND is returned; for "..", DOTDOT_FOUND is returned;
 * for other value, if found, DIR_ENTRY_FOUND is found and make *ppFound  point to the entry found,
 * else DIR_ENTRY_NOT_FOUND is returned.
 *
 * If @dents is NULL, we search from dir_i->dents; else from @dents.
 *
 * struct ffs_dentry_data
 *  {
 *       struct ffs_full_entry *dents;          //IN parameter
 *      struct ffs_full_entry *pFound;       //OUT parameter
 *   };
 */


int do_find_entry(struct inode ** dir_i, struct dentry *de)
{
    struct super_block *sb;
    UINT64 ino = 0;
    INT32 lErrCode = ERR_SUCCESS;

    if( NULL != de->inode )
    {
        D((DL_VFSERROR, "do_find_entry: parameter error, de->inode != NULL.\n"));
        return -1;
    }
    
    
    if ( FS_TYPE_FAT != (*dir_i)->i_fstype &&
          FS_TYPE_EXFAT != (*dir_i)->i_fstype)
    {
        D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (*dir_i)->i_fstype));
        return DIR_ENTRY_NOT_FOUND;
    }
    else 
    {
        if ( DSM_TcStrCmp(de->name,(PCWSTR)L"." ) == 0)
        {
                de->inode = *dir_i;
                de->inode->i_count++;
                return DOT_FOUND;
        }
        else if(DSM_TcStrCmp(de->name,(PCWSTR)L".." ) == 0)
        {
           if ( *dir_i == g_fs_current.root )
            {
                de->inode = *dir_i;
                de->inode->i_count++;
                return DOTDOT_FOUND;
            }
            else if ( (*dir_i)->i_ino == FS_ROOT_INO )
            {
                /* '..' over a mount-point results in 'dir_i' being exchanged for the mounted
                   directory-inode. NOTE! We set mounted, so that we can iput the new dir_i */
                //sb = fs_get_super( (*dir_i)->i_dev );
                fs_find_sb((*dir_i)->i_dev, &sb);
                if ( sb->s_mountpoint )
                { 
                    iput( *dir_i );
                    *dir_i = sb->s_mountpoint;
                    (*dir_i)->i_count++;
                }
            }               
        }
        if ( FS_TYPE_FAT == (*dir_i)->i_fstype )
        {
             lErrCode = fat_do_find_entry(*dir_i, de, &ino);
            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
        }
        else if ( FS_TYPE_EXFAT == (*dir_i)->i_fstype )
        {
            lErrCode = exfat_do_find_entry(*dir_i, de, &ino);
            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
        }    
    }    
    
    de->inode = iget( (*dir_i)->i_sb, ino);  
    if( NULL == de->inode ) // iget failed
    {
        D((DL_VFSERROR, "do_find_entry: iget de->inode is NULL.ino = %d\n",ino));
        return DIR_ENTRY_NOT_FOUND;
    }    
    /*we have increased the refcnt for de->inode in function iget()*/
    //
    // Deal with the member 'pfsdata'  of 'de'.
    // If 'pfsdata' is unused, just do nothing
    //
    if ( de->inode )
    {
        if ( FS_TYPE_FFS == (*dir_i)->i_fstype )
        {
            // DO NOTHING.
            return DIR_ENTRY_NOT_FOUND;
        }
        else if ( FS_TYPE_FAT == (*dir_i)->i_fstype )
        {
            // DO NOTHING.
        }
        else if ( FS_TYPE_EXFAT == (*dir_i)->i_fstype )
        {
            // DO NOTHING.
        }
        else // unknown filesystem type
        {
            return DIR_ENTRY_NOT_FOUND;
        }        
        return DIR_ENTRY_FOUND;
    }
    return DIR_ENTRY_NOT_FOUND;
}


/*
 * find_entry()
 *
 * finds an entry in the specified directory with the wanted name. 
 * WHEN THE ENTRY IS FOUND, WE INCREASE THE REFERENCE COUNT of the INODE 
 * ASSOCIATED WITH IT.
 * 
 * 0 is returned when found else -1 returned.
 */
INT32  find_entry( struct inode ** dir_i, struct dentry *de )
{
   INT32 result;
   INT32 ret;
   result = do_find_entry( dir_i, de);   
   if(DIR_ENTRY_FOUND == result ||
       DOT_FOUND == result ||
       DOTDOT_FOUND == result)
   {
       ret = 0;
   }
   else
   {
      ret = -1;
   }
   return ret;
}

int get_next_entry( struct inode * dir_i, UINT64 *ino, UINT8 type, UINT32 *pCount, const PTCHAR pExtName, UINT8 mode)
{
    UINT32 bclu=0, startclu=0;
    UINT16 bsec=0, eoff=0;
    int lErrCode;
    static UINT32 last_count = 0;
    UINT64 curino = 0;
    UINT64 nexino = 0;
    // UINT8* uni_name = NULL;
    // UINT32 uni_len = 0;
    // const UINT8* oem_name = "*";
    UINT16 ecount = 0;
    
    // find last, save count
    if(mode == GET_ENTRY_COUNT && last_count != 0)
    {
        *pCount = last_count;
        last_count = 0;
        return DIR_ENTRY_FOUND;
    }
    if(dir_i->i_fstype == FS_TYPE_FAT)
    {
        bclu = FAT_INO2CLUS(*ino);
        bsec = FAT_INO2SEC(*ino);
        eoff = FAT_INO2OFF(*ino);
    }
    else if(dir_i->i_fstype == FS_TYPE_EXFAT)
    {
        bclu = EXFAT_INO2CLUS(*ino);
        bsec = EXFAT_INO2SEC(*ino);
        eoff = EXFAT_INO2EOFF(*ino);
        curino = *ino;		//by ymh
    }
    else
    {
        return DIR_ENTRY_FOUND;
    }
    if(mode == GET_PREV_ENTRY && (type & 0x80))
        pCount = &last_count;
    else
        last_count = 0;

    if(0 == bclu || mode != GET_NEXT_ENTRY)
    {
        if(dir_i->i_fstype == FS_TYPE_FAT)
        {
            if(((UINT64)FS_ROOT_INO == dir_i->i_ino 
            || 0 == (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusLO | (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI<<16)))
            && dir_i->i_sb->u.fat_sb.iFATn != FAT32)
            { // FAT12或FAT16的根目录。FAT12与FAT16的根目录没有起始簇须特殊处理，
                //而FAT32的根目录则有起始簇。
                startclu = 0;//FS_ROOT_CLUSTER;
            }
            else
            {
                startclu = dir_i->u.fat_i.entry.DIR_FstClusLO;
                startclu |= (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI << 16);
            }
        }
        else if(dir_i->i_fstype == FS_TYPE_EXFAT)
        {
            startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;
        }
    }
    else
    {
        startclu = bclu;
    }

    curino = EXFAT_CSO2INO(startclu, bsec, eoff);

    if ( FS_TYPE_FFS == (dir_i)->i_fstype )
    {
        D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
        return DIR_ENTRY_NOT_FOUND;
    }
    else if ( FS_TYPE_FAT == (dir_i)->i_fstype )
    {
        //DSM_HstSendEvent(0x12011205);
        lErrCode = fat_get_next_entry(&(dir_i->i_sb->u.fat_sb), startclu, &bclu, &bsec, &eoff, type, pCount,pExtName, mode);
        if(ERR_SUCCESS != lErrCode)
        {
            D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
            return DIR_ENTRY_NOT_FOUND;
        }
        *ino = FAT_CSO2INO(bclu, bsec, eoff);   //by ymh
    }
    else if ( FS_TYPE_EXFAT == (dir_i)->i_fstype )
    {
        //DSM_HstSendEvent(0x12011205);
        //lErrCode = exfat_get_next_entry(&(dir_i->i_sb->u.exfat_sb), startclu, &bclu, &bsec, &eoff, type, pCount,pExtName, mode);
        if(mode == GET_PARENT_ENTRY)
        {
        }
        if(mode == GET_NEXT_ENTRY)
        {
            DSM_HstSendEvent(0x02090000);
            nexino = *ino;
            lErrCode = exfat_lookup_dir(dir_i, (WCHAR*)pExtName, &curino, &nexino,NULL,type,&ecount,pCount,mode);
            DSM_HstSendEvent(0x02090001);
            DSM_HstSendEvent((UINT32)(curino&0xffffffff));
            DSM_HstSendEvent((UINT32)((curino>>32)&0xffffffff));
            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "get_next_entry: exfat_lookup_file() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
            *ino = curino;
        }
        else if(mode == GET_PREV_ENTRY)
        {
            startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;            
            curino = EXFAT_CSO2INO(startclu, 0, 0);
            nexino = *ino;
            DSM_HstSendEvent(0xbbc22046);
            DSM_HstSendEvent((UINT32)(curino&0xffffffff));
            DSM_HstSendEvent((UINT32)((curino>>32)&0xffffffff));
            lErrCode = exfat_lookup_dir(dir_i, (WCHAR*)pExtName, &curino, &nexino,NULL,type,&ecount,pCount,mode);

            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "get_next_entry: exfat_lookup_file() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
            *ino = curino;
        }
        else if(mode == GET_LAST_ENTRY)
        {
            startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;            
            curino = EXFAT_CSO2INO(startclu, 0, 0);
            nexino = 0;
            lErrCode = exfat_lookup_dir(dir_i, (WCHAR*)pExtName, &curino, &nexino,NULL,type,&ecount,pCount,mode);

            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "get_next_entry: exfat_lookup_file() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
            *ino = curino;
        }
        else if(mode == GET_ENTRY_COUNT)
        {
            startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;            
            curino = EXFAT_CSO2INO(startclu,bsec,eoff);
            lErrCode = exfat_lookup_dir(dir_i, (WCHAR*)pExtName, &curino, &nexino,NULL,type,&ecount,pCount,mode);
            if(ERR_SUCCESS != lErrCode)
            {
                D((DL_VFSBRIEF, "get_next_entry: exfat_lookup_file() return = %d.\n", lErrCode));
                return DIR_ENTRY_NOT_FOUND;
            }
            *ino = curino;
        }
        else
        {
            lErrCode = DIR_ENTRY_NOT_FOUND;
        }
        if(ERR_SUCCESS != lErrCode)
        {
            D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
            return DIR_ENTRY_NOT_FOUND;
        }
    }
    else // unknown filesystem type
    {
        D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
        return DIR_ENTRY_NOT_FOUND;
    }

    // find last, save count
    if(mode == GET_PREV_ENTRY && (type & 0x80))
    {

    }
    //*ino = FAT_CSO2INO(bclu, bsec, eoff); //by ymh
    //DSM_HstSendEvent(*ino);
    DSM_HstSendEvent(0x0209000f);
    DSM_HstSendEvent((UINT32)((*ino)&0xffffffff));
    DSM_HstSendEvent((UINT32)(((*ino)>>32)&0xffffffff));
    return DIR_ENTRY_FOUND;
}


int get_parent_node(struct inode * dir_i, UINT64 *ino)
{
    UINT32 bclu, startclu;
    UINT16 bsec, eoff;
    int lErrCode;
    UINT64 parentInode;

	
	bclu = 0;
	bsec = 0;
	eoff = 0;

    if ( FS_TYPE_FFS == (dir_i)->i_fstype )
    {
        D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
        return DIR_ENTRY_NOT_FOUND;
    }
    else if ( FS_TYPE_FAT == (dir_i)->i_fstype )
    {
		startclu = dir_i->u.fat_i.entry.DIR_FstClusLO;
		startclu |= (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI << 16);
		
		//DSM_HstSendEvent(0x12011205);
        lErrCode = fat_get_next_entry(&(dir_i->i_sb->u.fat_sb), startclu, &bclu, &bsec, &eoff, 0x81, NULL, NULL, GET_PARENT_ENTRY);
        if(ERR_SUCCESS != lErrCode)
        {
            D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
            return DIR_ENTRY_NOT_FOUND;
        }
        *ino = FAT_CSO2INO(bclu, bsec, eoff);
    }
    else if ( FS_TYPE_EXFAT == (dir_i)->i_fstype )
    {
        startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;
        lErrCode = exfat_search_dir_parent_cache(ino, &parentInode);
        if(ERR_SUCCESS != lErrCode)
        {
            D((DL_VFSBRIEF, "exfat_search_dir_parent_cache: exfat_search_dir_parent_cache() return = %d.\n", lErrCode));
            return DIR_ENTRY_NOT_FOUND;
        }
        *ino = parentInode;
    }
    else // unknown filesystem type
    {
        D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
        return DIR_ENTRY_NOT_FOUND;
    }

	//*ino = FAT_CSO2INO(bclu, bsec, eoff);
	
	return DIR_ENTRY_FOUND;
}


int find_entry_by_cluster(struct inode * dir_i, UINT32 entryclu, UINT64 *ino)
{
	UINT32 bclu=0, startclu=0;
	UINT16 bsec=0, eoff=0;
	int lErrCode;
	UINT64 curino = 0;
       UINT64 nexino = 0;
       //UINT8* uni_name = NULL;
       //UINT32 uni_len = 0;
       //const UINT8* oem_name = "*";
       UINT16 ecount = 0;
	bclu = 0;
	bsec = 0;
	eoff = 0;

	if ( FS_TYPE_FFS == (dir_i)->i_fstype )
	{
		D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
		return DIR_ENTRY_NOT_FOUND;
	}
	else if ( FS_TYPE_FAT == (dir_i)->i_fstype )
	{
		startclu = dir_i->u.fat_i.entry.DIR_FstClusLO;
		startclu |= (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI << 16);
		
		//DSM_HstSendEvent(0x12011205);
		lErrCode = fat_get_next_entry(&(dir_i->i_sb->u.fat_sb), startclu, &bclu, &bsec, &eoff, 0x81, &entryclu, NULL, GET_ENTRY_CLUSTER);
		if(ERR_SUCCESS != lErrCode)
		{
			D((DL_VFSBRIEF, "do_find_entry: fat_do_find_entry() return = %d.\n", lErrCode));
			return DIR_ENTRY_NOT_FOUND;
		}
              *ino = FAT_CSO2INO(bclu, bsec, eoff);
	}
       else if ( FS_TYPE_EXFAT == (dir_i)->i_fstype )
       {
              startclu = dir_i->u.exfat_i.ext_entry.FirstCluster;
		curino = EXFAT_CSO2INO(startclu,bsec,eoff);

                lErrCode = exfat_lookup_dir(dir_i, NULL, &curino, &nexino,NULL,0x81,&ecount,&entryclu,GET_ENTRY_CLUSTER);

                if(ERR_SUCCESS != lErrCode)
                {
                    D((DL_VFSBRIEF, "find_entry_by_cluster: exfat_lookup_dir() return = %d.\n", lErrCode));
                    return DIR_ENTRY_NOT_FOUND;
                }
                *ino = curino;
              
       }
	else // unknown filesystem type
	{
		D((DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (dir_i)->i_fstype));
		return DIR_ENTRY_NOT_FOUND;
	}
	return DIR_ENTRY_FOUND;
}

// We ASSUME that the pathname contains the basename. i.e, the pathname is
// not terminated with COMPONENT SEPATATOR.
//
// Convert directory name( the part of the pathname that excludes the basename)
// to the corresponding inode.
//
// NOTE: THE PATH MUST BE NOT ENDED WITH '/', ELSE WE CONSIDER
// THAT THE LAST COMPONENT NAME IS MISSED.
//
// dir_namei() returns the inode of the directory, and the basename.
//
// when the directory name is converted to inode SUCCESSFULLY,
// we INCREASE THE REFERENCE COUNT of the inode associated with the directory name.
struct inode *dir_namei(PCWSTR pathname, INT32 *namelen, CONST WCHAR **basename)
{
    WCHAR *p = NULL;
    struct inode *working_i = NULL;
    WCHAR *thisname = NULL;
    int len = 0;
    struct dentry psDe;
    long lRet;
    UINT32 cheat = 1;

    p = (WCHAR *)pathname;
    if( NULL == g_fs_current.root || 0 == g_fs_current.root->i_count)
    {
        D((DL_VFSERROR, "dir_namei: No root inode, or refcount of root inode is 0"));
        g_tstFsLocalErrCode = 9001;
       // DSM_HstSendEvent(g_tstFsLocalErrCode);
        return (struct inode *)NULL;
    }

    if( NULL == g_fs_current.pwd || 0 == g_fs_current.pwd->i_count)
    {
        D((DL_VFSERROR, "dir_namei: No cwd inode, or refcount of cwd inode is 0"));
        g_tstFsLocalErrCode = 9002;
       // DSM_HstSendEvent(g_tstFsLocalErrCode);
        return (struct inode *)NULL;
    }

    if ( IS_COMPONENT_SEPATATOR(*p) )
    {
        working_i = g_fs_current.root;
        p++;

        while ( IS_COMPONENT_SEPATATOR(*p) )          /*skip  one or more continuous '/' */
            p++;
    }
    else if ( *p )
    {
        working_i = g_fs_current.pwd;
    }
    else
    {
        D((DL_VFSWARNING, "dir_namei: pathname is empty"));
        g_tstFsLocalErrCode = 9003;
     //   DSM_HstSendEvent(g_tstFsLocalErrCode);
        return (struct inode *)NULL;
    }

    working_i->i_count++;

    while ( cheat ) // cheat = 1, to avoid warning in level 4
    {
        thisname = p;
        if ( !FS_IS_DIR( working_i->i_mode ) )
        {
            D((DL_VFSWARNING, "dir_namei: inode is not dir.\n"));
            iput( working_i );
            g_tstFsLocalErrCode = 9004;
          //  DSM_HstSendEvent(g_tstFsLocalErrCode);
            return (struct inode *)NULL;
        }

        for ( len = 0; *p && ( !IS_COMPONENT_SEPATATOR(*p ) ); p++, len++ )
            /*NULL*/;

        if ( !*p )
        {
            break;
        }
        while (  IS_COMPONENT_SEPATATOR(*p ) )          /*skip  one or more continuous '/' */
            p++;

        /* We ASSUME that the length of @thisname is not more than FS_FILE_NAME_LEN */
        if (len > FS_FILE_NAME_LEN)
        {
            D((DL_VFSERROR, "dir_namei: name-len too long, len = %d.\n", len));
            iput(working_i);
            g_tstFsLocalErrCode = 9005;
       //     DSM_HstSendEvent(g_tstFsLocalErrCode);
            return (struct inode *)NULL;
        }
        DSM_MemSet(&psDe, 0, sizeof(struct dentry) );
        DSM_TcStrNCpy(psDe.name, thisname, len );
        psDe.append_data2 = FS_FLAG_NOT_FIND_FILE;
        lRet = find_entry( &working_i, &psDe );
        if ( -1 == lRet ) // find_entry failed
        {
            D((DL_VFSWARNING, "dir_namei: find_entry() failed: lRet = %d.\n", lRet));
            iput( working_i );
            g_tstFsLocalErrCode = 9006;
         //   DSM_HstSendEvent(g_tstFsLocalErrCode);
            return (struct inode *)NULL;
        }
        iput( working_i );
        working_i = psDe.inode;
        if(NULL == working_i)
        {
            g_tstFsLocalErrCode = 9007;
         //   DSM_HstSendEvent(g_tstFsLocalErrCode);
        }
    }

    /* NULL character is not counted */
    *namelen = p - thisname;
    *basename = thisname;
    return working_i;
}


//
// namei()
//
// is used by most simple commands to get the inode of a specified name.
// Open, link etc use their own routines, but this is enough for things
// like 'chmod' etc.
// WHEN THE ENTRY IS FOUND, WE INCREASE THE REFCNT OF the INODE
// ASSOCIATED WITH IT.
//
struct inode *namei( PCWSTR pathname )
{
    WCHAR *basename;
    INT32 namelen;
    struct inode *dir_i;
    struct inode *psInode = NULL;
    struct dentry psDe;
    long lRet;

    dir_i = dir_namei( pathname, &namelen, (CONST WCHAR **)&basename );
    if( !dir_i ) // path is invalid
    {
        D((DL_VFSWARNING, "namei: dir_namei() return NULL.\n"));
        return (struct inode *)NULL;
    }

    if ( !namelen )                 /* special case: '/home/gauss/' etc */
        return dir_i;

    if (namelen > FS_FILE_NAME_LEN)
    {
        D((DL_VFSERROR, "namei: basename too long, namelen = %d.\n", namelen));
        iput(dir_i);
        return (struct inode *)NULL;
    }
    DSM_MemSet( &psDe, 0, sizeof( psDe ) );
    DSM_TcStrCpy( psDe.name, basename );
    psDe.append_data2 = FS_FLAG_NOT_FIND_FILE;
    lRet = find_entry( &dir_i, &psDe );
    if ( -1 == lRet )
    {
        D((DL_VFSWARNING, "namei: find_entry() return lRet = %d.\n", lRet));
        iput( dir_i );
        return (struct inode *)NULL;
    }
    psInode = psDe.inode;
    iput( dir_i );
    return psInode;
}


INT32 fs_close(INT32 fd)
{
    struct file *filp;
    INT32 iRet;
    FAT_CACHE *psFatCache = NULL;

    if( !FD_IS_VALID( fd ) ) // fd is invalid
    {
        return ERR_FS_BAD_FD;
    }

    filp = g_fs_current.filp[ fd ];
    if(!fs_is_file_opened(&g_fs_file_list, filp))
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(filp->f_flags & FS_O_FIND) // handle is for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }

    if(!filp->f_count) // f_count should not be ZERO
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if ( --filp->f_count )
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    psFatCache = fat_get_fat_cache(filp->f_inode->i_dev);
    if(NULL == psFatCache)
    {
        D((DL_FATERROR, "fs_close: fat cache is null,s_dev = 0x%x.\n", filp->f_inode->i_dev));
        return _ERR_FAT_CACHE_IS_NULL;
    }
    if(psFatCache->iSecNum != 0)
    {
        fat_synch_fat_cache(&(filp->f_inode->i_sb->u.fat_sb), psFatCache);
    }
    iput( filp->f_inode );
    iRet = fs_fd_list_del(&g_fs_file_list, filp);
    g_fs_current.filp[ fd ] = NULL;

label_exit:
    return iRet;
}


INT32 fs_findclose(INT32 fd)
{
    struct file *filp;
    FAT_CACHE *psFatCache = NULL;
    INT32 iRet;

    if(!FD_IS_VALID( fd )) // hFile is invalid
    {
        return ERR_FS_BAD_FD;
    }
    filp = g_fs_current.filp[ fd ];
    if(!fs_is_file_opened(&g_fs_file_list, filp))
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(0 == (filp->f_flags & FS_O_FIND)) // handle is not for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }

    if(!filp->f_count)
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if ( --filp->f_count )                /*more fd redirects to the same file{}*/
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(filp->f_pattern != NULL)
    {
        DSM_Free(filp->f_pattern);
        filp->f_pattern = NULL;
    }

    psFatCache = fat_get_fat_cache(filp->f_inode->i_dev);
    if(NULL == psFatCache)
    {
        D((DL_FATERROR, "fs_findclose: fat cache is null,s_dev = 0x%x.\n", filp->f_inode->i_dev));
        return _ERR_FAT_CACHE_IS_NULL;
    }
    if(psFatCache->iSecNum != 0)
    {
        fat_synch_fat_cache(&(filp->f_inode->i_sb->u.fat_sb), psFatCache);
    }
    iput( filp->f_inode );
    fs_fd_list_del(&g_fs_file_list, filp);
    g_fs_current.filp[ fd ] = NULL;
    iRet = ERR_SUCCESS;
label_exit:
    return iRet;
}

/******************************************************************************
 * fs_unicodeNameToFATFileName: 
 * DESCRIPTION: -     change unicode file name to fat 8+3: 8 bytes file name and 3 bytes extend name. 
 *              
 *
 * Input: 
 *          pUniName -  unicode name.
 *          pFatName -  fat name.
 * Output: 
 * Returns: 
 *          error code
 * 
 * 
 ******************************************************************************/
INT32 fs_unicodeNameToFATFileName(WCHAR *pUniName, PSTR pFatName)
{
    UINT8 nameBuff[13], *pByte;     //8+'.'+3+'\0'
    WCHAR *p=NULL, *pDotPos=NULL;
    UINT32 nameLen=0, fatNameLen=0;//uniNameLen=0;
    INT32 iRet;

    //check input
    if( (pUniName == NULL) || (pFatName == NULL) )
        return _ERR_FAT_PARAM_ERROR;
//FS_FILE_NAME_LEN
    //get last '.' position
    p = pUniName;
    do
    {
        p = DSM_TcStrChr(p, (UINT16)(L'.'));
        
        if( p > pUniName + (FS_FILE_NAME_LEN*sizeof(WCHAR)) )
            return _ERR_FAT_PARAM_ERROR;
        
        if(p)
        {
            pDotPos = p++;
        }
    }
    while(p);

    //no dot, return error
    if(pDotPos == NULL)
        return _ERR_FAT_PARAM_ERROR;

    //copy none zero unicode to namebuff
    nameLen = (UINT32)pDotPos - (UINT32)pUniName;
    pByte = (UINT8*)pUniName;

    while( (nameLen>0) && (fatNameLen<8) )
    {
        if((*pByte) != 0x00)
        {
            nameBuff[fatNameLen++] = *pByte;
        }
        pByte++;
        nameLen--;
    }

    //fill blank if needed
    while(fatNameLen < 8)
    {
        nameBuff[fatNameLen++] = ' ';
    }
    
    //set dot
    nameBuff[fatNameLen++] = '.';

    //copy none zero unicode extend name to namebuff
    pByte = (UINT8*)(pDotPos+1);
    while( (((*pByte) != 0) || ((*(pByte+1)) != 0)) && (fatNameLen < 12) )
    {
        if((*pByte) != 0x00)
        {
            nameBuff[fatNameLen++] = *pByte;
        }
        pByte++;
    }

    //fill blank if needed
    while(fatNameLen < 12)
    {
        nameBuff[fatNameLen++] = ' ';
    }

    nameBuff[12] = 0;
    
    iRet = NameUser2Fat(nameBuff, pFatName);
    if(iRet != ERR_SUCCESS)
    {
        DSM_MemCpy(pFatName, "unknown    ", 11);
        DSM_MemCpy(pFatName+8, nameBuff+9, 3);
    }

    return iRet;
}


