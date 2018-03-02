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

//FFS WIN32
#define _WIN32_WINNT 0x0500
#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <stdlib.h>
#include <wchar.h>

#include "cswtype.h"
#include "fs.h"
#include "errorcode.h"

#define MAX_FILES 1000
#define MAX_DIRS 100
#define MAX_PATH 100
//#include "fs_type.h"
//#include "fat_fs.h"
//#include "FSSim_def.h"

TCHAR tflash_root[MAX_PATH];
TCHAR usbdisk_root[MAX_PATH];
TCHAR current_path[MAX_PATH];

U8 has_tflash, has_usbdisk;
FS_DEV_TYPE current_device;

struct {
    WORD file_num;
    WORD parent;
    TCHAR dirname[MAX_PATH];
    struct {
        TCHAR filename[MAX_PATH];
        UINT8 type; // 0 for file, 1 for directory
        HANDLE hFile;
    } list_files[MAX_FILES];
} list_dirs[MAX_DIRS];

WORD total_file = 0;
WORD total_dir = 0;
WORD current_dir = 0;
WORD current_entry = 0;
DWORD current_subdir = 0;

TCHAR open_filename[MAX_PATH];

int file_inode = 0;


INT32 fs_scan_all_Files(void);


INT32 SRVAPI FS_MountDevice(
	FS_DEV_TYPE dev_type
)
{
  if(dev_type == FS_DEV_TYPE_TFLASH)
    {
        if(has_tflash)
        {
            current_device = FS_DEV_TYPE_TFLASH;
            fs_scan_all_Files();
            return ERR_SUCCESS;
        }
        else
            return ERR_FS_UNMOUNT_FAILED;
    }
  else if(dev_type == FS_DEV_TYPE_USBDISK)
    {
        if(has_usbdisk)
        {
            current_device = FS_DEV_TYPE_USBDISK;
            fs_scan_all_Files();
            return ERR_SUCCESS;
        }
        else
            return ERR_FS_UNMOUNT_FAILED;
    }
    else
      return ERR_FS_INVALID_PARAMETER;
}

INT32 SRVAPI FS_UnmountRoot(void)
{
    current_device = FS_DEV_TYPE_INVALID;
}


INT32 SRVAPI FS_GetRootType(
	FS_DEV_TYPE *pDevType, 
	UINT8 *pFsType
)
{
  *pDevType = current_device;
  *pFsType = FS_TYPE_FAT;
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_HasFormatted(
    PCSTR pszDevName, 
    UINT8 iFsType
)
{
  return ERR_FS_HAS_FORMATED;
}

INT32 SRVAPI FS_Format(
    PCSTR pszDevName, 
    UINT8 iFsType, 
    UINT8 iFlag
){}

INT32 FS_Mount
( 
    PCSTR pszDevName, 
    PCSTR pszMountPoint, 
    INT32 iFlag, 
    UINT8 iType 
)
{
  return ERR_SUCCESS;
}


INT32 fs_scan_folder(int index)
{
    WIN32_FIND_DATA finddata;
    HANDLE hFind;
    
    TCHAR find_path[MAX_PATH];

    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(find_path, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(find_path, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;

    _tcscat(find_path, L"/");
    _tcscat(find_path, list_dirs[index].dirname);
    _tcscat(find_path, L"*");

    list_dirs[index].file_num = 0;

    hFind = FindFirstFile(find_path, &finddata);

    if(hFind == NULL)
        return ERR_FS_NO_MORE_FILES;

    do
    {
        if(finddata.dwFileAttributes & FS_ATTR_DIR)
        {
            if(finddata.cFileName[0] != L'.')
            {
            if(total_dir >= MAX_DIRS)
                break;

                total_dir ++;
                
                _tcscpy(list_dirs[total_dir].dirname, list_dirs[index].dirname);
                _tcscat(list_dirs[total_dir].dirname, finddata.cFileName);
                _tcscat(list_dirs[total_dir].dirname, L"/");
                list_dirs[total_dir].file_num = 0;
                list_dirs[total_dir].parent = index;

                _tcscpy(list_dirs[index].list_files[list_dirs[index].file_num].filename, finddata.cFileName);
                list_dirs[index].list_files[list_dirs[index].file_num].type = 1;
                list_dirs[index].list_files[list_dirs[index].file_num].hFile = total_dir;
                list_dirs[index].file_num ++;

            }
        }
        else
        {
            if(list_dirs[index].file_num >= MAX_FILES)
                break;
            //_tcscpy(list_dirs[index].list_files[list_dirs[index].file_num].filename, list_dirs[index].dirname);
            _tcscpy(list_dirs[index].list_files[list_dirs[index].file_num].filename, finddata.cFileName);
            list_dirs[index].list_files[list_dirs[index].file_num].type = 0;
            list_dirs[index].list_files[list_dirs[index].file_num].hFile = 0;
            list_dirs[index].file_num ++;

        }

    }while(FindNextFile(hFind, &finddata) != 0);

    FindClose(hFind);
    total_file += list_dirs[index].file_num;

    return 0;
}


INT32 fs_scan_all_Files(void)
{
    int i;

    total_dir = 1;

    _tcscpy(list_dirs[1].dirname, L"/");

    for(i = 1; i<= total_dir; i++)
    {
        fs_scan_folder(i);
    }

    current_dir = 1;
    current_subdir = FS_ROOT_INO;
    current_entry = 0;

  return ERR_SUCCESS;
}


INT32 SRVAPI FS_Open(
    PCSTR pszFileName, 
    UINT32 iFlag, 
    UINT32 iMode
)
{
    INT32 hFile;

    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(open_filename, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(open_filename, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;
    
    _tcscat(open_filename, current_path);
    _tcscat(open_filename, L"/");
    _tcscat(open_filename, pszFileName);

    file_inode ++;

    hFile = CreateFile(open_filename, ((iFlag&FS_O_ACCMODE)==FS_O_RDONLY)?GENERIC_READ:GENERIC_READ|GENERIC_WRITE,
                              0, NULL, (iFlag&FS_O_CREAT)?CREATE_ALWAYS:OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);

    return hFile;
}


int fs_check_ext(PTCHAR name, const char *pExtName)
{
    int i, j;
    PTCHAR ext;
    BOOL match = TRUE;

    if(pExtName == NULL)
        return TRUE;
    
    ext = name + _tcslen(name)-1;
    while(ext > name)
    {
        if(*(ext-1) == L'.')
            break;

        ext --;
    }

    for(i=0, j=0; i<strlen(pExtName); i++, j++)
    {
        if(pExtName[i] == '.')
        {
            if(match)
                return TRUE;
            match = TRUE;
            j = 0;
			i++;
        }
        if(pExtName[i] != ext[j] && pExtName[i]+0x20 != ext[j])
            match = FALSE;
    }
    return match;
}


INT32 SRVAPI FS_GetNextEntry(
    UINT32 *pINode, 
    const PTCHAR pExtName,
    BYTE type)
{
  int i;

  if(type & 0x80)
    i = 0;
  else
    i = current_entry+1;

  for(; i<list_dirs[current_dir].file_num; i++)
  {
    if(list_dirs[current_dir].list_files[i].type == (type&0xf))
    {
        if(fs_check_ext(list_dirs[current_dir].list_files[i].filename, pExtName))
        {
            current_entry = i;
            *pINode = (current_dir<<16)|current_entry;
            return ERR_SUCCESS;
        }
    }
  }

  return ERR_FS_NO_MORE_FILES;
}

INT32 SRVAPI FS_GetPrevEntry(
    UINT32 *pINode, 
    const PTCHAR pExtName,
    BYTE type)
{
  int i;

  if(type & 0x80)
    i = list_dirs[current_dir].file_num-1;
  else
    i = current_entry-1;

  for(; i>=0; i--)
  {
    if(list_dirs[current_dir].list_files[i].type == (type&0xf))
    {
        if(fs_check_ext(list_dirs[current_dir].list_files[i].filename, pExtName))
        {
            current_entry = i;
            *pINode = (current_dir<<16)|current_entry;
            return ERR_SUCCESS;
        }
    }
  }

  return ERR_FS_NO_MORE_FILES;
}

INT32 SRVAPI FS_GetEntryCount(
    UINT32 *pCount, 
    const PTCHAR pExtName,
    BYTE type)
{
  int i, count = 0;

    if(current_device == FS_DEV_TYPE_INVALID)
        return ERR_FS_NOT_MOUNT;

  for(i=0; i<list_dirs[current_dir].file_num; i++)
  {
    if(list_dirs[current_dir].list_files[i].type == (type&0xf))
    {
        if(fs_check_ext(list_dirs[current_dir].list_files[i].filename, pExtName))
        {
            count ++;
        }
    }
  }
  *pCount = count;
  return ERR_FS_NO_MORE_FILES;
}

INT32 SRVAPI FS_OpenDirect(UINT32 ino,	UINT32 iFlag, UINT32 iMode)
{
  HANDLE hFile;

  int dir = ino >> 16;
  int entry = ino & 0xffff;


    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(open_filename, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(open_filename, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;
    
    _tcscat(open_filename, list_dirs[dir].dirname);
    _tcscat(open_filename, list_dirs[dir].list_files[entry].filename);

    hFile = CreateFile(open_filename, (iFlag&FS_O_RDONLY)?GENERIC_READ:GENERIC_READ|GENERIC_WRITE,
                              0, NULL, (iFlag&FS_O_CREAT)?OPEN_ALWAYS:OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    return hFile;
}

INT32 SRVAPI FS_GetShortName(
    UINT32 iNode,
	PTCHAR pFileName
)
{
  int ret;
  int dir = iNode >> 16;
  int entry = iNode & 0xffff;
  WCHAR longname[MAX_PATH], pathname[MAX_PATH];
  char shotname[MAX_PATH], filename[30], extname[5];

  if(iNode == FS_ROOT_INO)
  {
    strcpy(pFileName, "/");
    return ERR_SUCCESS;
  }
    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(longname, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(longname, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;
    
    _tcscat(longname, list_dirs[dir].dirname);
    _tcscat(longname, list_dirs[dir].list_files[entry].filename);

  ret = GetShortPathName(longname, pathname, MAX_PATH);

  WideCharToMultiByte(CP_ACP, 0, (LPCSTR)pathname, -1, shotname, MAX_PATH, 0, 0);

  _splitpath(shotname, NULL, NULL, filename, extname);
  strcat(filename, "            ");
  filename[8] = 0;
  if(extname[0])
      strcat(filename+8, extname+1);
  strcat(filename, "   ");
  filename[11] = 0;
  strcpy(pFileName, filename);
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_GetEntryName(
    UINT32 iNode,
	INT32 iNameBufferLen,
	PTCHAR pFileName
)
{
  int ret;
  int dir = iNode >> 16;
  int entry = iNode & 0xffff;

  if(current_device == FS_DEV_TYPE_INVALID)
    return ERR_FS_NOT_MOUNT;
  
  if(iNode == FS_ROOT_INO)
  {
    _tcscpy(pFileName, L"/");
    return ERR_SUCCESS;
  }

  _tcsncpy(pFileName, list_dirs[dir].list_files[entry].filename, iNameBufferLen/2);
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_ChangeSubDir(
    UINT32 iNode
)
{
  if(FS_ROOT_INO == iNode)
  {
      current_dir = 1;
      current_entry = 0;
      current_subdir = FS_ROOT_INO;
      _tcscpy(current_path, L"/");
      return ERR_SUCCESS;
   }
   else if(FS_PARENT_INO == iNode)
   {
      WORD old_dir;

      if(current_dir == 0)
        return ERR_FS_FILE_NOT_EXIST;

      old_dir = current_dir;
      current_dir = list_dirs[current_dir].parent;

      for(current_entry = 0; current_entry<list_dirs[current_dir].file_num; current_entry++)
      {
        if(list_dirs[current_dir].list_files[current_entry].hFile == old_dir)
        {
            _tcscpy(current_path, list_dirs[current_dir].dirname);
            if(current_dir == 0)
                current_subdir = FS_ROOT_INO;
            else
            {
                old_dir = list_dirs[current_dir].parent;
                for(current_subdir = 0; current_subdir<list_dirs[current_dir].file_num; current_subdir++)
                {
                 if(list_dirs[old_dir].list_files[current_entry].hFile == current_dir)
                 {
                    current_subdir |= (old_dir<<16);
                    break;
                 }
                }
             }
            return ERR_SUCCESS;
        }
      }


  }
  else if((iNode >> 16)>0 && (iNode >> 16)<total_dir && (iNode & 0xffff)==0xffff)
  {
      current_subdir = iNode;
      current_dir = iNode >> 16;
      _tcscpy(current_path, list_dirs[current_dir].dirname);
      current_entry = 0;
      return ERR_SUCCESS;
  }
  else if((iNode >> 16)>0 && (iNode >> 16)<total_dir && (iNode & 0xffff)<list_dirs[(iNode >> 16)].file_num && list_dirs[(iNode >> 16)].list_files[(iNode & 0xffff)].type==1)
  {
      current_subdir = iNode;
      current_dir = iNode >> 16;
      current_entry = iNode & 0xffff;
      current_dir = list_dirs[current_dir].list_files[current_entry].hFile;
      _tcscpy(current_path, list_dirs[current_dir].dirname);
      current_entry = 0;
      return ERR_SUCCESS;
  }

   return ERR_FS_NOT_DIRECTORY;
   
}

INT32 SRVAPI FS_GetSubDirEntry(
    INT32 *pDirEntry
)
{
  *pDirEntry = current_subdir;
  return ERR_SUCCESS;
}


INT32 SRVAPI FS_GetCurDirEntry(
    INT32 *pDirEntry
 )
{
  *pDirEntry = (current_dir<<16)|current_entry;
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_SetCurDirEntry(
    INT32 DirEntry
)
{
  current_entry = DirEntry&0xffff;
  return ERR_SUCCESS;
}

    
INT32 SRVAPI FS_Close(
    INT32 iFd
){
    CloseHandle(iFd);
}

INT32 SRVAPI FS_Read(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
){
    DWORD cbRead;
    ReadFile(iFd, pBuf , iLen, &cbRead, NULL);
    return cbRead;
}

INT32 SRVAPI FS_Write(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
){
    DWORD cbRead;
    WriteFile(iFd, pBuf , iLen, &cbRead, NULL);
    return cbRead;
}

INT32 SRVAPI FS_Delete(
    PCSTR pszFileName
)
{
	return !DeleteFile(open_filename);
}

INT32 SRVAPI FS_Seek(
    INT32 iFd, 
    INT32 iOffset, 
    UINT8 iOrigin
){
    return SetFilePointer(iFd, iOffset , 0, iOrigin);
}

INT32 SRVAPI FS_MakeDir(
    PCSTR pszDirName, 
    UINT32 iMode
)
{
    TCHAR new_dirname[MAX_PATH];

    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(new_dirname, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(new_dirname, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;
    
    _tcscat(new_dirname, list_dirs[current_dir].dirname);
    _tcscat(new_dirname, pszDirName);

  if(CreateDirectory(new_dirname, NULL))
  {
        total_dir ++;
        
        _tcscpy(list_dirs[total_dir].dirname, list_dirs[current_dir].dirname);
        _tcscat(list_dirs[total_dir].dirname, pszDirName);
        _tcscat(list_dirs[total_dir].dirname, L"/");
        list_dirs[total_dir].file_num = 0;
        list_dirs[total_dir].parent = current_dir;

        _tcscpy(list_dirs[current_dir].list_files[list_dirs[current_dir].file_num].filename, pszDirName);
        list_dirs[current_dir].list_files[list_dirs[current_dir].file_num].type = 1;
        list_dirs[current_dir].list_files[list_dirs[current_dir].file_num].hFile = total_dir;
        list_dirs[current_dir].file_num ++;
      return ERR_SUCCESS;
  }
  else
    return ERR_FS_FAILURE;
}

INT32 SRVAPI FS_RemoveDir(
    PCSTR pszDirName
){}

INT32 SRVAPI FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
){
    WIN32_FIND_DATA findata;
    TCHAR filename[MAX_PATH];
    TCHAR find_path[MAX_PATH];
    HANDLE hFind;

    if(current_device == FS_DEV_TYPE_TFLASH)
        _tcscpy(find_path, tflash_root);
    else if(current_device == FS_DEV_TYPE_USBDISK)
        _tcscpy(find_path, usbdisk_root);
    else
        return ERR_FS_NOT_MOUNT;

    _tcscat(find_path, current_path);
    _tcscat(find_path, L"/");
    _tcscat(find_path, pszFileName);
    hFind = FindFirstFile(find_path, &findata);

    if(hFind == NULL)
        return ERR_FS_NO_MORE_FILES;

    _tcscpy(pFindData->st_name, findata.cFileName);
    //pFindData->i_no = file_inode;
    pFindData->st_size = findata.nFileSizeLow;
    pFindData->st_mode = findata.dwFileAttributes;

    return hFind;
}

INT32 SRVAPI FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
){
    TCHAR filename[MAX_PATH];
    WIN32_FIND_DATA findata;
    INT32 iRet;

    iRet = FindNextFile(iFd, &findata);

    if(iRet == 0)
        return ERR_FS_NO_MORE_FILES;

    _tcscpy(pFindData->st_name, findata.cFileName);
    pFindData->st_size = findata.nFileSizeLow;
    pFindData->st_mode = findata.dwFileAttributes;

    return ERR_SUCCESS;
}

INT32 SRVAPI FS_FindClose(
    INT32 iFd
){
    FindClose(iFd);
    return ERR_SUCCESS;
}

INT32 SRVAPI FS_GetFileAttr(
    PCSTR     pszFileName,
    FS_FILE_ATTR* pFileAttr
){
	   return ERR_SUCCESS;
}

INT32 SRVAPI FS_GetFSInfo(
    PCSTR pszDevName,
    FS_INFO* psFSInfo
){
    psFSInfo->iTotalSize = 256*1024*1024;
    psFSInfo->iUsedSize = 10*1024*1024;
    return ERR_SUCCESS;
}

INT32 SRVAPI FS_IsEndOfFile(
    INT32 iFd
){
	   return ERR_SUCCESS;
}

INT32 SRVAPI FS_Rename( 
    PCSTR pszOldname, 
    PCSTR pszNewname 
){
	   return ERR_SUCCESS;
}

INT32 FS_GetFileName(INT32 iFd, INT32 iNameBufferLen, UINT8 *pcFileName)
{
   // GetFinalPathNameByHandle(iFd, pcFileName, iNameBufferLen, 0);
	   return ERR_SUCCESS;
}

INT32 SRVAPI FS_PowerOn(
	VOID
)
{
    has_tflash = has_usbdisk = FALSE;
    current_device = FS_DEV_TYPE_INVALID;
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_PowerOff(
	VOID
)
{

  return ERR_SUCCESS;
}


// for simulator use only
void FS_SetRootPath(FS_DEV_TYPE dev_type, TCHAR *path)
{
    if(dev_type == FS_DEV_TYPE_TFLASH)
    {
        if(path != NULL)
        {
            _tcscpy(tflash_root, path);
            has_tflash = TRUE;
        }
        else
        {
            has_tflash = FALSE;
        }
    }
    if(dev_type == FS_DEV_TYPE_USBDISK)
    {
        if(path != NULL)
        {
            _tcscpy(usbdisk_root, path);
            has_usbdisk= TRUE;
        }
        else
        {
            has_usbdisk = FALSE;
        }
    }
}

//jiashuo add start  
/*************************************************************************/
/*																	   */
/* FUNCTION															   */
/*																	   */
/*	  FS_GetFileSize												   */
/*																	   */
/* DESCRIPTION														   */
/*																	   */
/*	  This function is used to the file size.						   */
/*																	   */
/* INPUTS 															   */
/*																	   */
/*								                                       */
/*																	   */
/* OUTPUTS															   */
/*																	   */
/*	  Error code													   */
/*																	   */
/*************************************************************************/
INT32 SRVAPI FS_GetFileSize( 
    HANDLE hFile
)
{
  DWORD size_high, size;

  size_high = 0;

  size = GetFileSize(hFile, &size_high);

  return size;
}
//jiashuo add end
INT32 FS_ChangeSize(INT32 fd, UINT32 nFileSize)
{
  return (-1);
}
 INT32 FS_Truncate(HANDLE FileHandle)
 {
  return (-1);
 }

INT32 FS_Unmount( 
    PCSTR pszMountPoint,
    BOOL  bForce
){
	   return ERR_SUCCESS;
}

#if 0 /* delete by zhuoxz,2009-3-24 */
INT32 SRVAPI FS_MountRoot(
    PCSTR pszRootDevName 
){}


INT32 SRVAPI FS_Create(
    PCSTR pszFileName, 
    UINT32 iMode
){}
INT32 SRVAPI FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
){}

INT32 SRVAPI FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
){}

INT32 SRVAPI FS_FindClose(
    INT32 iFd
){}
INT32 SRVAPI FS_Rename( 
    PCSTR pszOldname, 
    PCSTR pszNewname 
){}
INT32 SRVAPI FS_PowerOn(
    PCSTR pszDevName
){}

INT32 SRVAPI FS_PowerOff(
    PCSTR pszDevName
){}
PCSTR FS_GetNameFromHandle(
                          HANDLE hFile
                          )
{
    return(-1);
}

INT16 fat_UnicodeLen(UINT8* pszUnicode){}

INT16 fat_ConvertGBToUni( UINT16 c ){}

INT32 fat_Unicode2OEM(UINT8* pszUnicode,UINT16 iULen,UINT8* pszOEM,UINT16* piOLen){}

#endif /* #if 0 */ /* delete by zhuoxz,2009-3-24 */

/* Add by zhuoxz,2009-4-29 */
/* start */
INT32 SRVAPI FS_Flush(INT32 fd)
{
  return 0;
}

INT32 SRVAPI FS_GetCurDir(UINT32 uSize, PSTR pCurDir)
{
  return _tcscpy(pCurDir, list_dirs[current_dir].dirname);
}

INT32 SRVAPI FS_ChangeDir(PCSTR pszDirName)
{
    WORD old_dir;
    int i;
   if(pszDirName[0] == L'/')
    {
      current_dir = 1;
      current_entry = 0;
      current_subdir = FS_ROOT_INO;
      _tcscpy(current_path, L"/");
      return ERR_SUCCESS;
    }
   else if(pszDirName[0] == L'.' && pszDirName[2] == L'.')
    {
      if(list_dirs[current_dir].parent==0)
        return ERR_FS_NO_DIR_ENTRY;
      old_dir = current_dir;
      current_dir = list_dirs[current_dir].parent;

      for(current_entry = 0; current_entry<list_dirs[current_dir].file_num; current_entry++)
      {
        if(list_dirs[current_dir].list_files[current_entry].hFile == old_dir)
        {
            _tcscpy(current_path, list_dirs[current_dir].dirname);
            current_subdir = (current_dir<<16)|0xffff;
            return ERR_SUCCESS;
        }
      }
    }
   else
    {
        for(i=0; i<list_dirs[current_dir].file_num; i++)
        {
            if(_tcscmp(pszDirName, list_dirs[current_dir].list_files[i].filename) == 0)
            {
                current_subdir = (current_dir<<16)|i;
                current_dir = list_dirs[current_dir].list_files[i].hFile;
                current_entry = 0;
                _tcscpy(current_path, list_dirs[current_dir].dirname);
                return ERR_SUCCESS;
            }
        }
    }

  return ERR_FS_FILE_NOT_EXIST;
}

PUBLIC UINT32 gpio_detect_tcard(VOID)
{
	return current_device == FS_DEV_TYPE_TFLASH;
}

PUBLIC UINT32 gpio_CardDetectPowerOn(VOID)
{
	return current_device == FS_DEV_TYPE_TFLASH;
}

VOID SRVAPI FS_SetExfatDirCache(UINT64 *dirCache, UINT8 size, UINT8 length, UINT8 head, UINT8 tail)
{

}

VOID SRVAPI FS_GetExfatDirCache(UINT64 *dirCache, UINT8 size, UINT8 *length, UINT8 *head, UINT8 *tail)
{

}

/* end */

//////

