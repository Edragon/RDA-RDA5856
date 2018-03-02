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
#include "exfat_dir.h"
#include "fat_local.h"
#include "exfat_local.h"
#include "fs_base.h"
#include "reg.h"
#include "sxr_sbx.h"
#include "fs_namei.h"


extern struct task_struct g_fs_current;
extern HANDLE g_fs_sem;
extern struct file* g_fs_file_list;
extern UINT16 fselTotal;
UINT64 g_list_current_entry;

/*
INT32 SRVAPI FS_GetNextEntry(
    UINT64 *pIno,
    const PTCHAR pExtName,
    BYTE type)
{

    INT32 iRet = ERR_SUCCESS;
    struct inode *dir_i = g_fs_current.pwd;
    UINT64 ino;
    // struct inode * inode = NULL;

    if(dir_i == NULL)
        return ERR_FS_NOT_MOUNT;
    if(type & 0x80)
        ino = FAT_CSO2INO(0, 0, 0);
    else
        ino = g_list_current_entry;
    FS_Down();
    iRet = get_next_entry(dir_i, &ino, type, NULL, pExtName, GET_NEXT_ENTRY);
    if ( DIR_ENTRY_NOT_FOUND == iRet )
    {
        D((DL_FATWARNING, "+++ do_find_entry() return failed\n"));
        iRet = ERR_FS_NO_MORE_FILES;
        goto failed;
    }
    if (DIR_ENTRY_FOUND == iRet)
    {
        *pIno = g_list_current_entry = ino;
        iRet = ERR_SUCCESS;
    }

    failed:
    FS_Up();

    return iRet;
}
*/
INT32 SRVAPI FS_GetNextEntry(
    UINT64 *pIno,
    const PTCHAR pExtName,
    BYTE type)
{

    INT32 iRet = ERR_SUCCESS;
    struct inode *dir_i = g_fs_current.pwd;
    UINT64 ino;
    // struct inode * inode = NULL;
    DSM_HstSendEvent(0xabcd1001);
    DSM_HstSendEvent((UINT32)type);
    if(dir_i == NULL)
        return ERR_FS_NOT_MOUNT;
    if(type & 0x80)
    {
        if(dir_i->i_fstype == FS_TYPE_FAT)
        {
            ino = FAT_CSO2INO(0,0,0);
        }
        else if(dir_i->i_fstype == FS_TYPE_EXFAT)
        {
            ino = EXFAT_CSO2INO(0,0,0);
        }
    }
    else
        ino = g_list_current_entry;
    FS_Down();
    iRet = get_next_entry(dir_i, &ino, type, NULL, pExtName, GET_NEXT_ENTRY);
    if ( DIR_ENTRY_NOT_FOUND == iRet )
    {
        D((DL_FATWARNING, "+++ do_find_entry() return failed\n"));
        iRet = ERR_FS_NO_MORE_FILES;
        goto failed;
    }
    if (DIR_ENTRY_FOUND == iRet)
    {
        *pIno = g_list_current_entry = ino;
        iRet = ERR_SUCCESS;
    }
    DSM_HstSendEvent(0xabcd1002);
    DSM_HstSendEvent((UINT32)((ino) >> 32));
    DSM_HstSendEvent((UINT32)((ino) & 0xffffffff));

    failed:
    FS_Up();

    return iRet;
}


INT32 SRVAPI FS_GetPrevEntry(
    UINT64 *pIno,
    const PTCHAR pExtName,
    BYTE type)
{
  INT32 iRet = ERR_SUCCESS;
  struct inode *dir_i = g_fs_current.pwd;
  UINT64 ino;
  UINT8 mode;
  // struct inode * inode = NULL;

  DSM_HstSendEvent(0xabcd2001);
  DSM_HstSendEvent((UINT32)type);
  if(dir_i == NULL)
    return ERR_FS_NOT_MOUNT;

  if(type & 0x80)
	  ino = FAT_CSO2INO(0, 0, 0);
  else
	  ino = g_list_current_entry;

  FS_Down();

	if(ino == 0)
    {
        mode = GET_LAST_ENTRY;
    }
    else
    {
        mode = GET_PREV_ENTRY;
    }

    iRet = get_next_entry(dir_i, &ino, type, NULL, pExtName, mode);


	if ( DIR_ENTRY_NOT_FOUND == iRet )
	{
		D((DL_FATWARNING, "+++ do_find_entry() return failed\n"));
		iRet = ERR_FS_NO_MORE_FILES;
		goto failed;
	}
	if (DIR_ENTRY_FOUND == iRet)
	{
	   *pIno = g_list_current_entry = ino;
	   iRet = ERR_SUCCESS;
	}
        DSM_HstSendEvent(0xabcd2002);
        DSM_HstSendEvent((UINT32)((ino) >> 32));
        DSM_HstSendEvent((UINT32)((ino) & 0xffffffff));
failed:
	FS_Up();
  return iRet;
}


INT32 SRVAPI FS_GetEntryCount(
    UINT32 *pCount,
    const PTCHAR pExtName,
    BYTE type)
{

  INT32 iRet = ERR_SUCCESS;
  struct inode *dir_i = g_fs_current.pwd;
  UINT64 ino;
  // struct inode * inode = NULL;
  *pCount = 0;

  DSM_HstSendEvent(0xabcd3001);
  DSM_HstSendEvent((UINT32)type);
  if(dir_i == NULL)
    return ERR_FS_NOT_MOUNT;

  FS_Down();
  ino = FAT_CSO2INO(0, 0, 0);
  iRet = get_next_entry(dir_i, &ino, type, pCount, pExtName, GET_ENTRY_COUNT);

  FS_Up();

  if (DIR_ENTRY_FOUND == iRet)
  {
     iRet = ERR_SUCCESS;
  }
  DSM_HstSendEvent(0xabcd3002);
  DSM_HstSendEvent((UINT32)(*pCount));
  DSM_HstSendEvent((UINT32)((ino) >> 32));
  DSM_HstSendEvent((UINT32)((ino) & 0xffffffff));
  return iRet;
}


INT32 SRVAPI FS_OpenDirect(UINT64 ino,	UINT32 iFlag, UINT32 iMode)
{
	INT32 fd;
	struct inode * inode = NULL;
	struct file * f = NULL;
	// WCHAR *basename = NULL;
	// INT32 namelen;
	INT32 iErrCode = ERR_SUCCESS;
	// UINT16 iULen = 0;
	UINT32 iFileMode;

       DSM_HstSendEvent(0xabcd4001);
       DSM_HstSendEvent((UINT32)iMode);
       DSM_HstSendEvent(0xabcd4002);
       DSM_HstSendEvent((UINT32)((ino) >> 32));
       DSM_HstSendEvent((UINT32)((ino) & 0xffffffff));
	iFileMode= FS_ATTR_MARK;
	iFileMode |= ((iMode & FS_ATTR_RO)	== FS_ATTR_RO) ? FS_ATTR_RO : FS_ATTR_MARK;
	iFileMode |= ((iMode & FS_ATTR_HIDDEN)	== FS_ATTR_HIDDEN) ? FS_ATTR_HIDDEN : FS_ATTR_MARK;
	iFileMode |= ((iMode & FS_ATTR_SYSTEM)	== FS_ATTR_SYSTEM) ? FS_ATTR_SYSTEM : FS_ATTR_MARK;
	iFileMode |= ((iMode & FS_ATTR_ARCHIVE)  == FS_ATTR_ARCHIVE) ? FS_ATTR_ARCHIVE : FS_ATTR_MARK;

    /*if(ino == 0)
    {
		D((DL_FATERROR, "FS_OpenDirect: ino = 0.\n"));
        return ERR_FS_FILE_NOT_EXIST;
    }*/

	// cannot create file use ino
	if( iFlag & FS_O_CREAT)
	{
		D((DL_FATERROR, "FS_OpenDirect: ERR_FS_INVALID_PARAMETER1,iFlag = 0x%x.\n",iFlag));
		return ERR_FS_INVALID_PARAMETER;
	}

	// To truncate the file, we should have the write access to the file
	if ( ( iFlag & FS_O_TRUNC ) && ( ( iFlag & FS_O_ACCMODE ) == FS_O_RDONLY ) )
	{
		D((DL_FATERROR, "FS_OpenDirect: ERR_FS_INVALID_PARAMETER1,iFlag = 0x%x.\n",iFlag));
		return ERR_FS_INVALID_PARAMETER;
	}
	// To append the file, we should have the write access to the file
	if ( ( iFlag & FS_O_APPEND ) && ( ( iFlag & FS_O_ACCMODE ) == FS_O_RDONLY ) )
	{
		D((DL_FATERROR, "FS_OpenDirect: ERR_FS_INVALID_PARAMETER2,iFlag = 0x%x.\n",iFlag));
		return ERR_FS_INVALID_PARAMETER;
	}
	// The low byte greate less 3.
	if((iFlag & 0xf) > FS_O_ACCMODE )
	{
		D((DL_FATERROR, "FS_OpenDirect: ERR_FS_INVALID_PARAMETER3.iFlag = 0x%x.\n",iFlag));
		return ERR_FS_INVALID_PARAMETER;
	}
	FS_Down();

	// Get a valid file descriptor
	for ( fd = 0; fd < (INT32)g_fs_cfg.uNrOpenFileMax; fd++ )
	{
		if ( NULL == g_fs_current.filp[ fd ] )				   /*unused fhd found*/
			break;
	}

	if(fd == (INT32)g_fs_cfg.uNrOpenFileMax)
	{
		D((DL_FATWARNING, "FS_OpenDirect: FS_NR_OPEN_FILES_MAX.\n"));
		iErrCode = ERR_FS_FDS_MAX;
		goto step0_failed;
	}

	f = fs_alloc_fd(); // 向内存动态分配一个新的文件描述符。
	if (NULL == f)
	{
		D( ( DL_FATERROR, "in FS_OpenDirect, fs_alloc_fd() return NULL") );
		iErrCode = ERR_FS_NO_MORE_MEMORY;
		goto step0_failed;
	}

	f = fs_fd_list_add(&g_fs_file_list, f); // 添加到文件描述符链表。

    inode = iget( (g_fs_current.pwd)->i_sb, ino);

    if( NULL == inode ) // iget failed
    {
        D((DL_VFSERROR, "FS_OpenDirect: iget inode is NULL.ino = %d\n",ino));
        iErrCode = ERR_FS_FILE_NOT_EXIST;
		goto step0_failed;
    }

	// If the file is existd, creating action must be done
	if(( iFlag & FS_O_CREAT ) && ( iFlag &FS_O_EXCL ))
	{
		D( ( DL_FATERROR, "in FS_OpenDirect the file already existed, and flag is FS_O_CREAT") );
		iErrCode = ERR_FS_FILE_EXIST;
		goto step2_failed;
	}

	// 找到是一个目录而不是文件则出错。
	if ( FS_IS_DIR( inode->i_mode ) ) // Attempt to open a directroy file
	{
		D( ( DL_FATERROR, "in FS_OpenDirect, the inode is not a file,but a dir\n") );
		iErrCode = ERR_FS_IS_DIRECTORY;
		goto step2_failed;
	}

       /*
	if ( iFlag & FS_O_TRUNC )
	{
	   switch ( g_fs_current.pwd->i_fstype )
	   {
		case FS_TYPE_FAT:
			if(!(iFlag & FS_O_CREAT))
			{
				iFileMode = inode->i_mode;
			}
			iErrCode = fat_trunc_file( g_fs_current.pwd, inode, iFileMode );
			if(ERR_SUCCESS != iErrCode) // create file failed
			{
				D( ( DL_FATERROR, "in FS_OpenDirect, trunc fat file error, ErrorCode = %d\n",iErrCode) );
				//lErrCode = ERR_FS_TRUNC_FILE_FAILED;
				goto step2_failed;
			}
			break;
              case FS_TYPE_EXFAT:
			if(!(iFlag & FS_O_CREAT))
			{
				iFileMode = inode->i_mode;
			}
			iErrCode = exfat_trunc_file( g_fs_current.pwd, inode, iFileMode );
			if(ERR_SUCCESS != iErrCode) // create file failed
			{
				D( ( DL_FATERROR, "in FS_OpenDirect, trunc exfat file error, ErrorCode = %d\n",iErrCode) );
				//lErrCode = ERR_FS_TRUNC_FILE_FAILED;
				goto step2_failed;
			}
			break;
		default:
			D( ( DL_FATERROR, "in FS_OpenDirect, unknown file system type_1\n") );
			iErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
			goto step2_failed;
			break;
		}
	}
	*/
//exit_success:
	// TODO: 此时才关联，是否会让其他process抢了该file{}
	g_fs_current.filp[ fd ] = f;
	f->f_mode = inode->i_mode;
	f->f_flags = ( UINT16 ) iFlag;
	f->f_count = 1;
	f->f_inode = inode;
	/* Whether FS_O_APPEND flag is specified or not, we always set file pointer to 0,
		 because we may read the file before the first write. */
	f->f_pos = 0;

  //  CSW_TRACE(BASE_DSM_TS_ID,
   //			"#fs debug open success = %d",
  //			fd);
	FS_Up();
	CSW_TRACE(BASE_DSM_TS_ID, "open hd = 0x%x",fd);
	return fd;

step2_failed:
	iput( inode );

step0_failed:
	if (f != NULL)
	{
		fs_fd_list_del(&g_fs_file_list, f);
	}
	FS_Up();
	if(iErrCode < 0)
	{
		D( ( DL_FATERROR, "in FS_OpenDirect, error,iErrCode = %d.\n",iErrCode) );
	}
	return fs_set_error_code(iErrCode);
}


INT32 SRVAPI FS_GetEntryName(
    UINT64 ino,
	INT32 iNameBufferLen,
	PTCHAR pFileName
)
{
    UINT32 bclu=0;
    UINT16 bsec=0, eoff=0;
    INT32 iRet;
    WCHAR *pUniName = NULL;
    UINT32 uUniLen;
    struct inode * inode;

    DSM_HstSendEvent(0xabcd5001);
    DSM_HstSendEvent((UINT32)(ino >> 32));
    DSM_HstSendEvent((UINT32)(ino & 0xffffffff));
    pUniName = NAME_BUF_ALLOC();

    if (!pUniName)
    {
        D( ( DL_FATERROR, "in fat_get_file_name, inode = 0x%08x,fileName = 0x%x.\n",ino,pUniName) );
        return _ERR_FAT_PARAM_ERROR;
    }
    if(g_fs_current.pwd->i_fstype == FS_TYPE_FAT)
    {
        bclu = FAT_INO2CLUS(ino);
        bsec = FAT_INO2SEC(ino);
        eoff = FAT_INO2OFF(ino);
    }
    else if(g_fs_current.pwd->i_fstype == FS_TYPE_EXFAT)
    {
        bclu = EXFAT_INO2CLUS(ino);
        bsec = EXFAT_INO2SEC(ino);
        eoff = EXFAT_INO2EOFF(ino);
    }

    FS_Down();
    if ( FS_TYPE_FAT == g_fs_current.pwd->i_fstype )
    {
        iRet = fat_get_name_for_entrys(&(g_fs_current.pwd->i_sb->u.fat_sb), pUniName, bclu,  bsec, eoff);
        if(_ERR_FAT_SUCCESS == iRet)
        {
            //DSM_MemZero(pUniName, FAT_MAX_LONG_PATH_LEN + 2);
            uUniLen = DSM_UnicodeLen((UINT8*)pUniName);
            if ((UINT32)(uUniLen + LEN_FOR_NULL_CHAR) > iNameBufferLen / SIZEOF(WCHAR))
            {
                //iRet = ERR_FS_NAME_BUFFER_TOO_SHORT;
                //goto failed;
                uUniLen = iNameBufferLen / SIZEOF(WCHAR) - LEN_FOR_NULL_CHAR;
            }
            DSM_TcStrNCpy((PCWSTR)pFileName, pUniName, (uUniLen + LEN_FOR_NULL_CHAR));
            //gui_unicode2local(pFileName, uUniLen);
            iRet = ERR_SUCCESS;
        }
        else
        {
            D( ( DL_FATERROR, "fat_get_file_name error, errorCode = %d.\n", iRet) );
            iRet = fs_set_error_code(iRet);
        }
    }
    else if ( FS_TYPE_EXFAT == g_fs_current.pwd->i_fstype )
    {
        inode = iget( (g_fs_current.pwd)->i_sb, ino);
        if( NULL == inode ) // iget failed
        {
             D((DL_VFSERROR, "FS_GetShortName: iget de->inode is NULL.ino = 0x%08x\n",ino));
             iRet = DIR_ENTRY_NOT_FOUND;
             goto _func_end;
        }
        iRet = exfat_get_name_for_entrys(inode, pUniName, bclu,  bsec, eoff);
        if(_ERR_FAT_SUCCESS == iRet)
        {
            //DSM_MemZero(pUniName, FAT_MAX_LONG_PATH_LEN + 2);
            uUniLen = DSM_UnicodeLen((UINT8*)pUniName);
            if ((UINT32)(uUniLen + LEN_FOR_NULL_CHAR) > iNameBufferLen / SIZEOF(WCHAR))
            {
                //iRet = ERR_FS_NAME_BUFFER_TOO_SHORT;
                //goto failed;
                uUniLen = iNameBufferLen / SIZEOF(WCHAR) - LEN_FOR_NULL_CHAR;
            }
            DSM_TcStrNCpy((PCWSTR)pFileName, pUniName, (uUniLen + LEN_FOR_NULL_CHAR));
            gui_unicode2local(pFileName, uUniLen);
            iRet = ERR_SUCCESS;
        }
        else
        {
            D( ( DL_FATERROR, "fat_get_file_name error, errorCode = %d.\n", iRet) );
            iRet = fs_set_error_code(iRet);
        }
        iput(inode);
    }
    else
    {
        D( ( DL_FATERROR, "FS_GetFileName: unknow filesystem\n") );
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
    }

_func_end:
    //failed:
    if (pUniName)
    {
        NAME_BUF_FREE(pUniName);
    }
    FS_Up();

    if(iRet != ERR_SUCCESS)
    {
        UINT8 fatname[12],shortname[13];
        D( ( DL_FATWARNING, "FS_GetFileName fail, call FS_GetShortName\n") );
        iRet = FS_GetShortName(ino, fatname);

        if(iRet == ERR_SUCCESS)
        {
            NameFat2User(fatname, shortname, 0);

            iRet = DSM_OEM2Uincode(shortname, (UINT16)DSM_StrLen(shortname), (UINT8**)&pUniName, &uUniLen,NULL);
            if(ERR_SUCCESS == iRet)
            {
                DSM_MemCpy(pFileName,pUniName,uUniLen+2);
                DSM_Free(pUniName);
            }
            else
            {
                DSM_MemSet(pFileName, 0,iNameBufferLen);
            }
        }
        else
        {
            DSM_MemSet(pFileName, 0,iNameBufferLen);
        }
    }

    return iRet;
}

INT32 SRVAPI FS_GetShortName(UINT64 ino, PTCHAR pFileName)
{
	UINT32 bclu;
	UINT16 bsec, eoff;
	INT32 iRet;
    //UINT8 pname[16];
    //UINT32 pname_len = 0;
    //UINT8 ename[8];
    //UINT32 ename_len;
    //UINT32 lossy = 0;
    struct inode *dir_i;
    struct inode * inode = NULL;
    WCHAR* name_buff = NULL;

    DSM_HstSendEvent(0xabcd6001);
    DSM_HstSendEvent((UINT32)(ino >> 32));
    DSM_HstSendEvent((UINT32)(ino & 0xffffffff));

    if(ino == FS_ROOT_INO)
    {
        DSM_MemSet(pFileName, 0, 12);
        pFileName[0] = '/';
        return ERR_SUCCESS;
    }
    FS_Down();
    dir_i = g_fs_current.pwd;
    if(dir_i->i_fstype == FS_TYPE_FAT)
    {
        bclu = FAT_INO2CLUS(ino);
        bsec = FAT_INO2SEC(ino);
        eoff = FAT_INO2OFF(ino);
    }
    else if(dir_i->i_fstype == FS_TYPE_EXFAT)
    {
        bclu = EXFAT_INO2CLUS(ino);
        bsec = EXFAT_INO2SEC(ino);
        eoff = EXFAT_INO2EOFF(ino);
    }
    if ( FS_TYPE_FAT == dir_i->i_fstype )
    {
        iRet = fat_get_short_name(&(dir_i->i_sb->u.fat_sb), pFileName, bclu,  bsec, eoff);
        if(_ERR_FAT_SUCCESS == iRet)
        {
            iRet = ERR_SUCCESS;
        }
        else
        {
            D( ( DL_FATERROR, "fat_get_short_name error, errorCode = %d.\n", iRet) );
            iRet = fs_set_error_code(iRet);
        }
    }
    else if ( FS_TYPE_EXFAT == dir_i->i_fstype )
    {
        inode = iget( (g_fs_current.pwd)->i_sb, ino);
        if( NULL == inode ) // iget failed
        {
            D((DL_VFSERROR, "FS_GetShortName: iget de->inode is NULL.ino = 0x%08x\n",ino));
            iRet = DIR_ENTRY_NOT_FOUND;
            goto _func_end;
        }
        name_buff = NAME_BUF_ALLOC();
        iRet = exfat_get_name_for_entrys(inode, name_buff, bclu,  bsec, eoff);
        if(_ERR_FAT_SUCCESS == iRet)
        {
            iRet = fs_unicodeNameToFATFileName(name_buff, pFileName);
        }
        else
        {
            D( ( DL_FATERROR, "exfat_get_name_for_entrys error, errorCode = %d.\n", iRet) );
            iRet = fs_set_error_code(iRet);
        }
    }
    else
    {
        D( ( DL_FATERROR, "FS_GetShortName: unknow filesystem\n") );
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
    }

_func_end:
    if(name_buff)
    {
        NAME_BUF_FREE(name_buff);
    }
    FS_Up();

    if(inode)
    {
        iput(inode);
    }

    return iRet;
}


INT32 SRVAPI FS_ChangeSubDir(UINT64 ino)
{
    struct inode * inode;
    //struct dentry *psDe = NULL;
    //WCHAR *basename;
    //INT32 namelen;
    INT32 iErrCode;
    //UINT16 iULen = 0;
    UINT32 uDevNo = 0;
    UINT32 startclu = 0;
    DSM_HstSendEvent(0xabcd7001);
    DSM_HstSendEvent((UINT32)(ino >> 32));
    DSM_HstSendEvent((UINT32)(ino & 0xffffffff));

    if(ino == 0)
    {
        if( (g_fs_current.root->i_sb->s_fstype == FS_TYPE_EXFAT) ||
           ((g_fs_current.root->i_sb->s_fstype == FS_TYPE_FAT) &&  (g_fs_current.root->i_sb->u.fat_sb.iFATn == FAT32)) )
        {
            D((DL_FATERROR, "FS_ChangeSubDir: iNode = 0.\n"));
            return ERR_FS_FILE_NOT_EXIST;
        }
    }
    /*if( (ino == 0) && (g_fs_current.root->i_sb->s_fstype == FS_TYPE_FAT))
    {
        if(g_fs_current.root->i_sb->u.fat_sb.iFATn == FAT32)
        {
            D((DL_FATERROR, "FS_ChangeSubDir: iNode = 0.\n"));
            return ERR_FS_FILE_NOT_EXIST;
        }
    }*/
    FS_Down();
    if(ino == FS_PARENT_INO)
    {
        if(g_fs_current.pwd == g_fs_current.root)
        {
            iErrCode = ERR_FS_NOT_DIRECTORY;
            goto step0_failed;
        }
        ino = g_fs_current.pwd->i_ino;
        iErrCode = get_parent_node(g_fs_current.pwd, &ino);
        if(iErrCode != DIR_ENTRY_FOUND)
        {
            DSM_HstSendEvent(0xbbc33000);
            if(g_fs_current.root->i_sb->s_fstype == FS_TYPE_EXFAT)
            {
                DSM_HstSendEvent(0xbbc33001);
                exfat_update_dir_cache(FS_ROOT_INO);
                iErrCode = ERR_FS_NOT_DIRECTORY;
                goto step0_failed;
            }
            else
            {
                iErrCode = ERR_FS_NOT_DIRECTORY;
                goto step0_failed;
            }
        }
        if(g_fs_current.root->i_sb->s_fstype == FS_TYPE_FAT)
        {
            startclu = g_fs_current.pwd->u.fat_i.entry.DIR_FstClusLO;
            startclu |= (UINT32)(g_fs_current.pwd->u.fat_i.entry.DIR_FstClusHI << 16);
        }
        else if(g_fs_current.root->i_sb->s_fstype == FS_TYPE_EXFAT)
        {
            startclu = g_fs_current.pwd->u.exfat_i.ext_entry.FirstCluster;
        }
        else
        {
            startclu = 0;
        }
    }

    if(ino == FS_ROOT_INO)
    {
        inode = g_fs_current.root;
        if(g_fs_current.pwd != inode)
            inode->i_count ++ ;
        goto change_success;
    }
    inode = iget( (g_fs_current.pwd)->i_sb, ino);
    if( NULL == inode ) // iget failed
    {
        D((DL_VFSERROR, "do_find_entry: iget de->inode is NULL.ino = 0x%08x\n",ino));
        iErrCode = DIR_ENTRY_NOT_FOUND;
        goto step0_failed;
    }

    if ( !FS_IS_DIR( inode->i_mode ) )
    {
        D( ( DL_FATERROR, "in FS_ChangeDir, inode is not dir") );
        iput( inode );
        iErrCode = ERR_FS_NOT_DIRECTORY;
        goto step0_failed;
    }

/*	iErrCode = fs_SetWorkDirPath(g_fs_current.wdPath, (PCWSTR)pszDirName);
	if (iErrCode != ERR_SUCCESS)
	{
		D((DL_FATERROR, "FS_ChangeDir: fs_SetWorkDirPath return %d", iErrCode));
		iput(inode);
		goto step1_failed;
	}*/
change_success:

    if(startclu == 0)
        g_list_current_entry = FAT_CSO2INO(0, 0, 0);
    else
        {
        iErrCode = find_entry_by_cluster(inode, startclu, &g_list_current_entry);
        if(iErrCode != DIR_ENTRY_FOUND)
        {
            g_list_current_entry = FAT_CSO2INO(0, 0, 0);
        }
    }
    if(g_fs_current.pwd != inode)
    {
        iput( g_fs_current.pwd );
        g_fs_current.pwd = inode;	/*we have add the refcnt for inode in find_entry()*/
    }

    if ( FS_TYPE_EXFAT ==(g_fs_current.pwd->i_fstype) )
    {
        exfat_update_dir_cache(g_fs_current.pwd->i_ino);
    }

    DSM_HstSendEvent(0xbbc60220);
    DSM_HstSendEvent((UINT32)(g_list_current_entry >> 32));
    DSM_HstSendEvent((UINT32)(g_list_current_entry & 0xffffffff));
    DSM_HstSendEvent((UINT32)((g_fs_current.pwd->i_ino) >> 32));
    DSM_HstSendEvent((UINT32)((g_fs_current.pwd->i_ino) & 0xffffffff));
    iErrCode = ERR_SUCCESS;
step0_failed:

    if(DSM_IsRemoveableDev(uDevNo))
    {
        if(!fs_is_opened(uDevNo))
        {
            DRV_DEV_DEACTIVE(uDevNo);
        }
    }
    FS_Up();
    return fs_set_error_code(iErrCode);
}

VOID SRVAPI FS_SetExfatDirCache(UINT64 *dirCache, UINT8 size, UINT8 length, UINT8 head, UINT8 tail)
{
    if ( FS_TYPE_EXFAT ==(g_fs_current.pwd->i_fstype) )
    {
        if(ERR_SUCCESS != exfat_set_dir_cache(dirCache, size, length, head, tail))
        {
            D( (DL_FATERROR, "FS_SetExfatDirCache: exfat_set_dir_cache error\n") );
        }
    }
}

VOID SRVAPI FS_GetExfatDirCache(UINT64 *dirCache, UINT8 size, UINT8 *length, UINT8 *head, UINT8 *tail)
{
    if ( FS_TYPE_EXFAT ==(g_fs_current.pwd->i_fstype) )
    {
        if(ERR_SUCCESS != exfat_get_dir_cache(dirCache, size, length, head, tail))
        {
            D( (DL_FATERROR, "FS_GetExfatDirCache: exfat_get_dir_cache error\n") );
        }
    }
}

INT32 SRVAPI FS_GetSubDirEntry(UINT64 *pIno)
{
    *pIno = g_fs_current.pwd->i_ino;
    DSM_HstSendEvent(0xabcd8001);
    DSM_HstSendEvent((UINT32)((*pIno) >> 32));
    DSM_HstSendEvent((UINT32)((*pIno) & 0xffffffff));

    return ERR_SUCCESS;
}


INT32 SRVAPI FS_GetCurDirEntry(UINT64 *pIno)
{
    *pIno = g_list_current_entry;
    DSM_HstSendEvent(0xabcd9001);
    DSM_HstSendEvent((UINT32)((*pIno) >> 32));
    DSM_HstSendEvent((UINT32)((*pIno) & 0xffffffff));
    return ERR_SUCCESS;
}

INT32 SRVAPI FS_SetCurDirEntry(
    UINT64 ino
)
{
    // check entry valid
    g_list_current_entry = ino;
    DSM_HstSendEvent(0xabcda001);
    DSM_HstSendEvent((UINT32)((ino) >> 32));
    DSM_HstSendEvent((UINT32)((ino) & 0xffffffff));
    return ERR_SUCCESS;
}


