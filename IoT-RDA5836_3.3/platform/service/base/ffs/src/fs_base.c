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
#include "fat_base.h"
#include "exfat_base.h"
#include "fat_local.h"
#include "exfat_local.h"
#include "fs.h"
#include "dsm_dbg.h"
#include "fs_base.h"
#include "fs_namei.h"
#include "exfat_dir.h"


struct task_struct g_fs_current = {NULL,NULL,{0,},{0,}};


struct file* g_fs_file_list = NULL;

struct fs_cfg_info g_fs_cfg = {FS_DEFAULT_NR_SUPER, FS_NR_OPEN_FILES_MAX};


FS_DEV_INFO* g_pFsDevInfo = NULL;
#if CHIP_BTOVERLAY_SUPPORTED==1
static struct super_block g_fs_super_block_table_def[FS_DEFAULT_DEV_COUNT];    //we now only have two uFsDevCount
struct super_block *g_fs_super_block_table = g_fs_super_block_table_def;
#else
struct super_block *g_fs_super_block_table = NULL;
#endif
extern FAT_CACHE* g_FatCache;
extern UINT32 g_FatCahcheNum;

INT32 fs_set_error_code(INT32 iErrCode)
{    
    INT32 iRet = ERR_SUCCESS;
    
    switch(iErrCode)
    {
        case _ERR_FAT_SUCCESS:          iRet = ERR_SUCCESS                 ;break;
        case _ERR_FAT_PARAM_ERROR:      iRet = ERR_FS_INVALID_PARAMETER    ;break;
        case _ERR_FAT_WRITE_SEC_FAILED: iRet = ERR_FS_WRITE_SECTOR_FAILED  ;break;
        case _ERR_FAT_READ_SEC_FAILED:  iRet = ERR_FS_READ_SECTOR_FAILED   ;break;
        case _ERR_FAT_MALLOC_FAILED:    iRet = ERR_FS_NO_MORE_MEMORY       ;break;
        case _ERR_FAT_DISK_FULL:        iRet = ERR_FS_DISK_FULL            ;break;
        case _ERR_FAT_ROOT_FULL:        iRet = ERR_FS_ROOT_FULL            ;break;
        case _ERR_FAT_READ_EXCEED:      iRet = ERR_FS_READ_FILE_EXCEED     ;break;
        case _ERR_FAT_WRITE_EXCEED:     iRet = ERR_FS_WRITE_FILE_EXCEED    ;break;
        case _ERR_FAT_NOT_SUPPORT:      iRet = ERR_FS_NOT_SUPPORT          ;break;
        case _ERR_FAT_BPB_ERROR:        iRet = ERR_FS_DATA_DESTROY         ;break;
        case _ERR_FAT_FILE_TOO_MORE:    iRet = ERR_FS_FILE_TOO_MORE        ;break;
        case _ERR_FAT_FILE_NOT_EXISTED: iRet = ERR_FS_FILE_NOT_EXIST       ;break;
        case _ERR_FAT_DIFF_DEV:         iRet = ERR_FS_DEVICE_DIFF          ;break;
        case _ERR_FAT_HAS_FORMATED:     iRet = ERR_FS_HAS_FORMATED         ;break;
        case _ERR_FAT_NOT_FORMAT:       iRet = ERR_FS_NOT_FORMAT           ;break;
        case _ERR_FAT_NO_MORE_INODE:    iRet = ERR_FS_NO_MORE_MEMORY        ;break;
        case _ERR_FS_NO_MORE_SB_ITEM:   iRet = ERR_FS_NO_MORE_SB_ITEM      ;break;
        case _ERR_FS_NOT_MOUNT:         iRet = ERR_FS_NOT_MOUNT            ;break; 
        case _ERR_FAT_DIR_NOT_NULL:     iRet = ERR_FS_DIR_NOT_EMPTY        ;break; 
        case _ERR_FAT_ERROR:            iRet = ERR_FS_DATA_DESTROY         ;break;
        case _ERR_FAT_NO_MORE_NULL_ENTRY: iRet = ERR_FS_ROOT_FULL            ;break;
        default:                       
            iRet = iErrCode;
            break;
    }
    return iRet;
}


INT32 fs_global_var_init()
{
    UINT32 iResult;
    UINT32 i = 0,n = 0;
    DSM_DEV_INFO* pDevInfo = NULL;
    UINT32 uDsmDevCount = 0;
    UINT32 uFsDevCount = 0;

    exfat_init_dir_cache();

    // Get Device Info and create fs device table.
    iResult = DSM_GetDeviceInfo(&uDsmDevCount,&pDevInfo);
    if(ERR_SUCCESS != iResult)
    {
       D((DL_FATERROR, "fs_global_var_init: GetDeviceInfo failed.iResult = %d",iResult));
        return ERR_FS_GET_DEV_INFO_FAILED;
    }
    // compute the device count.
    for(i = 0; i < uDsmDevCount; i++)
    {
        if(DSM_MODULE_FS_ROOT == pDevInfo[i].module_id ||
            DSM_MODULE_FS == pDevInfo[i].module_id)
        {
             uFsDevCount ++;
        }

    }
    // check the FS device count.
    if(uFsDevCount == 0)
    {
        D((DL_FATERROR, "fs_global_var_init: the fs device count is zero."));
        return ERR_FS_DEVICE_NOT_REGISTER;
    }

    // malloc FS device table.
    g_pFsDevInfo = DSM_MAlloc(SIZEOF(FS_DEV_INFO)*uFsDevCount);
    if(NULL == g_pFsDevInfo)
    {
        D((DL_FATERROR, "fs_global_var_init: malloc fs device table failed!"));
        DSM_ASSERT(0,"fs_global_var_init: 1.malloc(0x%x) failed.",(SIZEOF(FS_DEV_INFO)*uFsDevCount)); 
        return ERR_FS_NO_MORE_MEMORY;
    }
   
    // set the device talble.
    for(i = 0; i < uDsmDevCount; i++)
    {
        // device table.
        if(DSM_MODULE_FS_ROOT == pDevInfo[i].module_id)
        {
               DSM_StrCpy(g_pFsDevInfo[n].dev_name,pDevInfo[i].dev_name);
               g_pFsDevInfo[n].dev_type = pDevInfo[i].dev_type == DSM_MEM_DEV_FLASH ? FS_DEV_TYPE_FLASH : FS_DEV_TYPE_TFLASH;
               g_pFsDevInfo[n].is_root = TRUE;
               n++;
        }
        if(DSM_MODULE_FS == pDevInfo[i].module_id)
        {
               DSM_StrCpy(g_pFsDevInfo[n].dev_name,pDevInfo[i].dev_name);
               g_pFsDevInfo[n].dev_type = pDevInfo[i].dev_type == DSM_MEM_DEV_FLASH ? FS_DEV_TYPE_FLASH : FS_DEV_TYPE_TFLASH;
               g_pFsDevInfo[n].is_root = FALSE;
               n++;
        }     
    }
    
    g_fs_cfg.uNrSuperBlock = uFsDevCount;

#if CHIP_BTOVERLAY_SUPPORTED!=1
    // malloc FAT chache.
     g_FatCache = (FAT_CACHE*)DSM_MAlloc(SIZEOF(FAT_CACHE)*uFsDevCount);
    
    if(NULL == g_FatCache)
    {
        D((DL_FATERROR, "fs_global_var_init: malloc fs fat cache failed!"));
        DSM_ASSERT(0,"fs_global_var_init: 2.malloc(0x%x) failed.",(SIZEOF(FAT_CACHE)*uFsDevCount)); 
        goto failed;
    }    
#endif   
    DSM_MemZero(g_FatCache, (SIZEOF(FAT_CACHE)*uFsDevCount));
     // set fat cache.
     n = 0;
     for(i = 0; i < uDsmDevCount; i++) 
     {
         if(DSM_MODULE_FS_ROOT == pDevInfo[i].module_id ||
            DSM_MODULE_FS == pDevInfo[i].module_id )
         {
            // fat chache.    
            g_FatCache[n].iDevNo= pDevInfo[i].dev_no;
            g_FatCache[n].iSecNum = 0;
            g_FatCache[n].psFileInodeList = NULL;
            g_FatCache[n].iDirty = 0;
            if(DSM_MEM_DEV_TFLASH == pDevInfo[i].dev_type)
            {
                DRV_SET_WCACHE_SIZE(g_FatCache[n].iDevNo,DRV_DEF_CACHE_BLOCK_CNT*DEFAULT_SECSIZE);
                DRV_SET_RCACHE_SIZE(g_FatCache[n].iDevNo,DRV_DEF_CACHE_BLOCK_CNT*DEFAULT_SECSIZE*8);
            }
            n ++;
         }
     }
     
    g_FatCahcheNum = n;

#if CHIP_BTOVERLAY_SUPPORTED!=1    
    g_fs_super_block_table = DSM_MAlloc((SIZEOF(struct super_block)) * g_fs_cfg.uNrSuperBlock);

    if (NULL == g_fs_super_block_table)
    {
        D((DL_FATERROR, "fs_global_var_init: malloc g_fs_super_block_table failed!"));
        DSM_ASSERT(0,"fs_global_var_init: 3.malloc(0x%x) failed.",((SIZEOF(struct super_block)) * g_fs_cfg.uNrSuperBlock));         
        goto failed;
    }
#endif
    DSM_MemZero(g_fs_super_block_table, (SIZEOF(struct super_block)) * g_fs_cfg.uNrSuperBlock);
  
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;
    DSM_MemSet( g_fs_current.filp, 0x00, ((SIZEOF( UINT32) ) *FS_NR_OPEN_FILES_MAX));
    DSM_MemSet( g_fs_current.wdPath, 0x00, (FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));

    return ERR_SUCCESS;

#if CHIP_BTOVERLAY_SUPPORTED!=1
failed:
    if (g_fs_super_block_table != NULL)
    {
        DSM_Free(g_fs_super_block_table);
        g_fs_super_block_table = NULL;
    }

    if (g_FatCache != NULL)
    {
        DSM_Free(g_FatCache);
        g_FatCache = NULL;
    }
#endif
    /*
    if (g_fs_current.filp != NULL)
    {
        DSM_Free(g_fs_current.filp);
        g_fs_current.filp = NULL;
    }
    if (g_fs_current.wdPath != NULL)
    {
        DSM_Free(g_fs_current.wdPath);
        g_fs_current.wdPath = NULL;
    }
    */
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;

    return ERR_FS_NO_MORE_MEMORY;
}


INT32 fs_global_var_free()
{
#if CHIP_BTOVERLAY_SUPPORTED!=1   
    UINT32 i;
    if (g_fs_super_block_table != NULL)
    {
        for(i = 0; i < g_fs_cfg.uNrSuperBlock; i++)
        {
            if(NULL != (g_fs_super_block_table+i)->fat_fsi)
            {
                DSM_Free((g_fs_super_block_table+i)->fat_fsi);
            }
        }
        DSM_Free(g_fs_super_block_table);
        g_fs_super_block_table = NULL;
    }  

    if (g_FatCache != NULL)
    {
        DSM_Free(g_FatCache);
        g_FatCache = NULL;
    }
#endif    
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;

    return ERR_SUCCESS;
}


INT32 fs_set_work_dir(WCHAR *WorkDirPath, WCHAR *ChangeDirPath)
{
    WCHAR *p = NULL;
    WCHAR *tmp = NULL;
    WCHAR *p2 = NULL;
    WCHAR *dirName;
    INT32 nameLen;
    WCHAR c;
    WCHAR c2;
    UINT8 cheat = 1;
    
    if (!WorkDirPath || !ChangeDirPath)
    {
        D( ( DL_VFSERROR, "in fs_set_work_dir, parameter error" ) );
        return ERR_FS_INVALID_PARAMETER;
    }
    
    if(DSM_TcStrCmp((PCWSTR)L"..",ChangeDirPath) == 0)
    {
        p = WorkDirPath;

        while(*p++);         

        while(p > WorkDirPath)
        {
            if(*p == L'/' || *p == L'\\')
            {
                *p = 0;
                break;
            }
            p--;
        }
        
        if(p == WorkDirPath)
        {
            if(*p == L'/' || *p == L'\\')
            {
                *(p+1) = 0;
            }
        }
        return ERR_SUCCESS;
    }
    if(DSM_TcStrCmp((PCWSTR)L".",ChangeDirPath) == 0)
    {
        return 0;
    }

    tmp = (WCHAR *)DSM_MAlloc((FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
    if (NULL == tmp)
    {
        D((DL_VFSERROR, "fs_set_work_dir: Malloc failed!"));
        DSM_ASSERT(0,"fs_set_work_dir: 1.malloc(0x%x) failed.",((FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR))); 
        return ERR_FS_NO_MORE_MEMORY;
    }

    DSM_TcStrCpy(tmp, WorkDirPath);
    p = tmp;
    
    if (_T_DSM('\\') == *ChangeDirPath || _T_DSM('/') == *ChangeDirPath)
    {       
        *p = 0;
    }
    else
    {        
        while (*p)
        {
            p++;
        }
        while (p > tmp && (_T_DSM('/') == (c = *(p - 1)) || _T_DSM('\\') == c))
        {
            p--;
            *p = 0;
        }
    }

    while (_T_DSM('\\') == *ChangeDirPath || _T_DSM('/') == *ChangeDirPath)
    {
        ChangeDirPath++;
    }
    while (cheat)
    {
        while (_T_DSM(' ') == *ChangeDirPath)
        {
            ChangeDirPath++;
        }
        dirName = ChangeDirPath;
        while ( (c = *(ChangeDirPath++)) != 0 && c != _T_DSM('\\') && c != _T_DSM('/'))
        {
            ;
        }
        p2 = ChangeDirPath - 1;
        while (p2 > dirName && (_T_DSM(' ') == *p2))
        {
            p2--;
        }
        if ((nameLen = p2 - dirName) > 0)
        {
            if (1 == nameLen && 0 == DSM_TcStrNCmp(dirName, (PCWSTR)_T_DSM("."), 1))
            {
                ;
            }
            else if (2 == nameLen && 0 == DSM_TcStrNCmp(dirName, (PCWSTR)_T_DSM(".."), 2))
            {
                while (p > tmp && (c2 = *(--p)) != _T_DSM('\\') && c2 != _T_DSM('/'))
                {
                    ;// do nothing.
                }
                *p = 0;
            }
            else
            {
                if ((p + (nameLen + 1)) > (tmp + FS_PATH_LEN * SIZEOF(WCHAR)))
                {
                    DSM_Free(tmp);
                    return ERR_FS_PATHNAME_PARSE_FAILED;
                }
                DSM_TcStrNCpy(p, (PCWSTR)_L_DSM("/"), 1);
                p++;
                DSM_TcStrNCpy(p, dirName, nameLen);
                p += nameLen;
            }
        }
        if (!c)
        {
            break;
        }
    }

    if (p == tmp)
    {
        DSM_TcStrCpy(p, (PCWSTR)_L_DSM("/"));
    }
    else
    {
        *p = _T_DSM('\0');
    }
    DSM_TcStrCpy(WorkDirPath, tmp);
    DSM_Free(tmp);
    return ERR_SUCCESS;

}


INT32 fs_get_free_sb(struct super_block** psSuperBlock)
{
    INT32 iRet = _ERR_FS_NO_MORE_SB_ITEM;
    INT32 i;

    for ( i = 0; i < (INT32)g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        if( NULL == g_fs_super_block_table[ i ].s_root &&
            INVALID_DEVICE_NUMBER == g_fs_super_block_table[ i ].s_dev)
        {
            *psSuperBlock = &g_fs_super_block_table[i];
            iRet = _ERR_FS_SUCCESS;
            break;
        }
    }
    if(iRet != _ERR_FS_SUCCESS)
    {
        D((DL_VFSWARNING, "in fs_get_free_sb  super_block_table is full\n"));
    }
    return iRet;
}


INT32 fs_find_sb(UINT32 iDev,struct super_block** psSuperBlock) 
{
    INT32 iRet = _ERR_FS_NOT_MOUNT;
    INT32 i;

    for ( i = 0; i < (INT32)g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        if( iDev == g_fs_super_block_table[ i ].s_dev && 
            NULL != g_fs_super_block_table[ i ].s_root)
        {
            *psSuperBlock = &g_fs_super_block_table[i] ;
            iRet = _ERR_FS_SUCCESS;
            break;
        }
    }
    if(_ERR_FS_SUCCESS != iRet)
    {
		*psSuperBlock = NULL ;
        D((DL_VFSWARNING, "in fs_find_sb  super_block is not found,iDev =  \n",iDev));        
    }
    return iRet;
}

VOID fs_delete_sb(UINT32 iDev) 
{
    UINT32 i;

    for ( i = 0; i < (UINT32)g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        if( iDev == g_fs_super_block_table[ i ].s_dev)
        {
            DSM_MemSet(&g_fs_super_block_table[ i ],0,sizeof(struct super_block));
            g_fs_super_block_table[ i ].s_dev = INVALID_DEVICE_NUMBER;
			g_fs_super_block_table[ i ].s_root = NULL;
            break;
        }
    }
    if(g_fs_cfg.uNrSuperBlock == i)
    {
        D((DL_VFSWARNING, "in fs_find_sb  super_block is not found,iDev =  \n",iDev));        
    }
}
struct super_block*  fs_get_super( UINT32 devnr )
{
    UINT32 i;

    if ( !devnr )
        return NULL;

    for ( i = 0; i < g_fs_cfg.uNrSuperBlock; i++ )
    {
        if ( g_fs_super_block_table[ i ].s_dev == ( UINT32 ) devnr &&
			g_fs_super_block_table[ i ].s_root != NULL)
        {
            return g_fs_super_block_table + i;
        }
    }
    D((DL_FATWARNING, "in fs_get_super,  no superblock is found\n"));
    return NULL;
}

struct super_block* fs_get_inode_super(struct inode* inode)
{
    int i;
    struct super_block* sb = NULL;
    
    for ( i = 0; i < (INT32)g_fs_cfg.uNrSuperBlock; i++ )
    {
        if ( g_fs_super_block_table[ i ].s_mountpoint == inode )
        {
            sb = &g_fs_super_block_table[i];
            break;
        }
    }
	return sb;
}


struct file *fs_alloc_fd( void )
{
    struct file *fd = NULL;

    fd = (struct file *)DSM_MAlloc(SIZEOF(struct file));
    if( NULL == fd)
    {
        DSM_ASSERT(0, "fs_alloc_fd: 1.malloc(0x%x) failed.", (SIZEOF(struct file)));
        return NULL;
    }

    DSM_MemSet( fd, 0x00, sizeof(*fd) );
    return fd;
}


struct file *fs_fd_list_add(struct file **ppsFileList, struct file *psFD)
{
    if (NULL == ppsFileList || NULL == psFD)
    {
        D ((DL_FATERROR, "fs_fd_list_add: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD));
        return (struct file *)NULL;
    }

    psFD->next = *ppsFileList;
    *ppsFileList = psFD;
    return psFD;
}


INT32 fs_fd_list_del(struct file **ppsFileList, struct file *psFD)
{
    struct file **ppPrei;
    struct file *pCuri;

    if (NULL == ppsFileList || NULL == psFD)
    {
        D ((DL_FATERROR, "fs_fd_list_del: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD));
        return ERR_FS_INVALID_PARAMETER;
    }

    ppPrei = ppsFileList;
    pCuri = *ppsFileList;
    while (pCuri != NULL)
    {
        if (pCuri == psFD)
        {
            *ppPrei = pCuri->next;
            DSM_Free(pCuri);
            return ERR_SUCCESS;
        }
        else
        {
            ppPrei = &(pCuri->next);
            ppPrei = ppPrei;
            pCuri = pCuri->next;
        }
    }
    return ERR_FS_INVALID_PARAMETER;

}


BOOL fs_is_file_opened(struct file **ppsFileList, struct file *psFD)
{
    struct file **ppPrei;
    struct file *pCuri;

    if (NULL == ppsFileList || NULL == psFD)
    {
        D ((DL_FATERROR, "fs_fd_list_del: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD));
        return FALSE;
    }

    ppPrei = ppsFileList;
    pCuri = *ppsFileList;
    while (pCuri != NULL)
    {
        if (pCuri == psFD)
        {
            return TRUE;
        }
        else
        {
            ppPrei = &(pCuri->next);
            ppPrei = ppPrei;
            pCuri = pCuri->next;
        }
    }
    return FALSE;
}

BOOL fs_is_opened(UINT32 iDev)
{
    struct file **ppPrei;
    struct file *pCuri;

    if (NULL == g_fs_file_list )
    {
        return FALSE;
    }

    ppPrei = &g_fs_file_list;
    pCuri = g_fs_file_list;
    while (NULL != pCuri)
    {
        if(NULL != pCuri->f_inode)
        {
            if (pCuri->f_inode->i_dev == iDev)
            {
                return TRUE;
            }
            else
            {
                ppPrei = &(pCuri->next);
                pCuri = pCuri->next;
            }
        }
        else
        {
            ppPrei = &(pCuri->next);
            ppPrei = ppPrei;
            pCuri = pCuri->next;
        }

    }
    return FALSE;
}

INT32 fs_read_mbr(UINT8 *buf, MBR_BOOT_RECORD* pMbr, UINT32 uDevNo, UINT8 *fstype, UINT32 *iPartitionOffset)
{
    INT32 iRet = ERR_SUCCESS;
    INT32 iResult;
    UINT64 iSecNo = 0;
    UINT32 i;

    iResult = DRV_BLOCK_READ(uDevNo, iSecNo, buf);
    if(_ERR_FAT_SUCCESS != iResult)
    {
        D((DL_FATERROR, "fs_read_super(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
        iRet = _ERR_FAT_READ_SEC_FAILED;
        // DSM_HstSendEvent(0xaabb7702);
        // DSM_HstSendEvent(iResult);
        // DSM_HstSendEvent(uDevNo);
        return iRet;
    }

    Buf2MBR(buf, pMbr);
    if(pMbr->trail_sig == FAT_FSI_TRAILSIG )
    {
        for(i = 0; i < MBR_SUB_RECORD_COUNT; i++)
        {
            if(pMbr->sub_rec[i].active_partition != 0x00 &&
                pMbr->sub_rec[i].active_partition != 0x80)
            {
                D((DL_FATWARNING, " fs_read_super: [%d]active_partition = 0x%x.",i,pMbr->sub_rec[i].active_partition));                    
                continue;
            }        
            // part_type_indicator   file system type
            // 0x01   FAT12
            // 0X06   FAT16
            // 0X07   NTFS/EXFAT
            // 0X0B   Win95 FAT32   
            // 0X0C   Win95 FAT32
            // 0X0E   Win95 FAT16
            // 0X0F  Win95 Extend
            if(pMbr->sub_rec[i].part_type_indicator == 0x01 ||
                pMbr->sub_rec[i].part_type_indicator == 0x06 ||
                pMbr->sub_rec[i].part_type_indicator == 0x0b ||
                pMbr->sub_rec[i].part_type_indicator == 0x0c ||
                pMbr->sub_rec[i].part_type_indicator == 0x0e ||
                pMbr->sub_rec[i].part_type_indicator == 0x0f)
            {
                *fstype = FS_TYPE_FAT;
                // DSM_HstSendEvent(0xaabb7703);
            }
            else if(pMbr->sub_rec[i].part_type_indicator == 0x07)
            {           
                *fstype = FS_TYPE_EXFAT;
                // DSM_HstSendEvent(0xaabb7704);
            }
            else
            {
                continue;
            }
            if(pMbr->sub_rec[i].sectors_preceding == 0x00 )
            {
                D((DL_FATWARNING, "fs_read_super: [%d]sectors_preceding = 0x%x.",i,pMbr->sub_rec[i].sectors_preceding));
                continue;
            }

            *iPartitionOffset = pMbr->sub_rec[i].sectors_preceding;
            if(pMbr->sub_rec[i].setcors_partion == 0x00 )
            {
                D((DL_FATWARNING, "fs_read_super: [%d]setcors_partion = 0x%x.",i,pMbr->sub_rec[i].setcors_partion));
                continue;
            }
            //iSecNo = pMbr->sub_rec[i].sectors_preceding;
            break;
        }
    
        if(i == MBR_SUB_RECORD_COUNT)
        {
            // DSM_HstSendEvent(0xaabb7705);
            iRet = _ERR_FAT_BPB_ERROR;
        }    
    }   
    else
    {
        D((DL_FATWARNING, "fs_read_super: trail_sig error. trail_sig = 0x%x.",pMbr->trail_sig));
        // DSM_HstSendEvent(0xaabb7706);
        iRet = _ERR_FAT_BPB_ERROR;
    }
    // DSM_HstSendEvent(0xaabb7707);
    return iRet;
}

INT32 fs_write_mbr(UINT8 *buf, MBR_BOOT_RECORD *pMbr, UINT32 uDevNo, UINT8 fstype, UINT32 iDiskTotSec)
{
    INT32 iRet = ERR_SUCCESS;
    INT32 iResult;
    UINT32 iSecNum = 0;
    UINT8 start_head;
    UINT8 start_sector;
    UINT16 start_cylinder;
    UINT8 end_head;
    UINT8 end_sector;
    UINT16 end_cylinder;
    UINT8 offset_head;
    UINT8 offset_sector;
    UINT16 offset_cylinder;
    UINT32 partition_offset;
    DSM_MemSet(pMbr,0,SIZEOF(MBR_BOOT_RECORD));
    pMbr->sub_rec[0].active_partition = 0x00;
    partition_offset = fs_get_partition_offset(iDiskTotSec);
    //start CHS address
    offset_cylinder = (UINT16)(partition_offset / SECTOR_PER_CYLINDER);
    offset_head = (UINT8)((partition_offset % SECTOR_PER_CYLINDER) / SECTOR_PER_TRACK);
    offset_sector = (UINT8)((partition_offset % SECTOR_PER_CYLINDER) % SECTOR_PER_TRACK + 1);
    start_sector = offset_sector;
    start_head = offset_head;
    start_cylinder = offset_cylinder;
    pMbr->sub_rec[0].start_head = start_head;
    pMbr->sub_rec[0].start_sector = start_sector;
    pMbr->sub_rec[0].start_cylinder = start_cylinder;
    if(fstype == FS_TYPE_EXFAT)
    {
        pMbr->sub_rec[0].part_type_indicator = 0x07;
    }
    else
    {
        if(iDiskTotSec > 16777216)
        {
            pMbr->sub_rec[0].part_type_indicator = 0x0F;
        }
        else
        {
            pMbr->sub_rec[0].part_type_indicator = 0x0B;
        }
    }

    //end CHS address
    offset_cylinder = (UINT16)((iDiskTotSec - 1) / SECTOR_PER_CYLINDER);
    offset_head = (UINT8)(((iDiskTotSec - 1) % SECTOR_PER_CYLINDER) / SECTOR_PER_TRACK);
    offset_sector = (UINT8)(((iDiskTotSec - 1) % SECTOR_PER_CYLINDER) % SECTOR_PER_TRACK);
    end_sector = (start_sector + offset_sector) % SECTOR_PER_TRACK;
    end_head = (start_head + offset_head + (start_sector + offset_sector) / SECTOR_PER_TRACK) % HEAD_PER_CYLINDER;
    end_cylinder = start_cylinder + offset_cylinder + \
        (start_head + offset_head + (start_sector + offset_sector) / SECTOR_PER_TRACK) / HEAD_PER_CYLINDER;
    pMbr->sub_rec[0].end_head = end_head;
    pMbr->sub_rec[0].end_sector = end_sector;
    pMbr->sub_rec[0].end_cylinder = end_cylinder;
    pMbr->sub_rec[0].sectors_preceding = partition_offset;
    pMbr->sub_rec[0].setcors_partion = iDiskTotSec;
    pMbr->trail_sig = FAT_FSI_TRAILSIG;
    MBR2Buf(pMbr, buf);
    iResult = DRV_BLOCK_WRITE(uDevNo,iSecNum,buf);
    if(ERR_SUCCESS != iResult)
    {
        D((DL_FATERROR, "fs_write_mbr: DRV_BLOCK_WRITE error,iResult = %d,iSecNum = 0x%x.\n",iResult,iSecNum));
        iRet = _ERR_FAT_WRITE_SEC_FAILED;
    }

    DSM_MemSet(buf, 0, DEFAULT_SECSIZE);
    for(iSecNum = 1; iSecNum < partition_offset; iSecNum++)
    {
        iResult = DRV_BLOCK_WRITE( uDevNo, iSecNum, buf );
        if(ERR_SUCCESS != iResult)
        {
            D((DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_1 error,err code = %d.\n", iResult));
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
        }
    }

    return iRet;
}

INT32 fs_set_mbr_type(UINT8 *buf, MBR_BOOT_RECORD *pMbr, UINT32 uDevNo, UINT8 fstype, UINT32 iDiskTotSec)
{
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;
    UINT32 iSecNum = 0;
    Buf2MBR(buf, pMbr);
    if(fstype == FS_TYPE_EXFAT)
    {
        pMbr->sub_rec[0].part_type_indicator = 0x07;
    }
    else
    {
        if(iDiskTotSec > 16777216)
        {
            pMbr->sub_rec[0].part_type_indicator = 0x0F;
        }
        else
        {
            pMbr->sub_rec[0].part_type_indicator = 0x0B;
        }
    }

    MBR2Buf(pMbr, buf);
    iResult = DRV_BLOCK_WRITE(uDevNo, iSecNum, buf);
    if(ERR_SUCCESS != iResult)
    {
        D((DL_FATERROR, "fs_set_mbr_type: DRV_BLOCK_WRITE error,iResult = %d,iSecNum = 0x%x.\n",iResult,iSecNum));
        iRet = _ERR_FAT_WRITE_SEC_FAILED;
    }

    return iRet;
}

INT32 fs_read_super( struct super_block *sb, UINT32 uDevNo)
{
    INT32 iRet = ERR_SUCCESS;
    INT32 iResult;
    UINT8* secbuf = NULL;
    MBR_BOOT_RECORD* pMbr = NULL;
    FAT_SB_INFO* fat_sb;
    EXFAT_SB_INFO* exfat_sb = NULL;
    UINT32 iSecNo = 0;
    UINT8 fstype = FS_TYPE_FAT;
    
    // DSM_HstSendEvent(0xaabb8801);
    fat_sb = &sb->u.fat_sb;
    exfat_sb = &sb->u.exfat_sb;
    sb->s_dev = uDevNo;
    sb->s_magic = FAT_MAGIC;
    //sb->s_fstype = FS_TYPE_FAT;
    fat_sb->sb = sb;
    exfat_sb->sb = sb;
    if(DSM_IsRemoveableDev(uDevNo))
    {
        secbuf = FAT_SECT_BUF_ALLOC();    
        if(!secbuf)
        {
            D((DL_FATERROR, "fs_read_super: FAT_SECT_BUF_ALLOC failed."));
            DSM_ASSERT(0,"fs_read_super: FAT_SECT_BUF_ALLOC failed.");
            iRet = _ERR_FAT_MALLOC_FAILED;
            // DSM_HstSendEvent(0xaabb8802);
            goto _func_end;
        }
        
        pMbr = DSM_MAlloc(sizeof(MBR_BOOT_RECORD));
        if(!pMbr)
        {
            D((DL_FATERROR, "fs_read_super: malloc(%d) failed.",sizeof(MBR_BOOT_RECORD)));
            DSM_ASSERT(0,"fs_read_super: malloc(%d) failed.",sizeof(MBR_BOOT_RECORD));
            iRet =  _ERR_FAT_MALLOC_FAILED;
            // DSM_HstSendEvent(0xaabb8803);
            goto _func_end;
        }
        //check MBR 
        iResult = fs_read_mbr(secbuf, pMbr, uDevNo, &fstype, &iSecNo);
        if(_ERR_FAT_SUCCESS != iResult)
        {
            // iRet = _ERR_FAT_PARAM_ERROR;
             // DSM_HstSendEvent(0xaabb8804);
             // DSM_HstSendEvent(uDevNo);
            // goto _func_end;
            iSecNo = 0;

            //set fstype with first sector
            iResult = DRV_BLOCK_READ(uDevNo, iSecNo, secbuf);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                D((DL_FATERROR, "fs_read_super(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto _func_end;
            }
            if( (secbuf[0] == 0xEB) && (secbuf[1] == 0x76) && (secbuf[2] == 0x90) && 
                 (DSM_StrCmp((UINT8*)(&secbuf[3]), "EXFAT   ") == 0) )
            {
                fstype = FS_TYPE_EXFAT;
            }
            else
            {
                fstype = FS_TYPE_FAT;
            }
        }
        else
        {
             // DSM_HstSendEvent(0xaabb8814);
             // DSM_HstSendEvent(uDevNo);
        }
    }
    
    if(fstype == FS_TYPE_EXFAT)
    {
        iResult = exfat_read_sb_info(uDevNo,iSecNo,exfat_sb);
        if( _ERR_FAT_SUCCESS == iResult)
        {            
            sb->s_fstype = FS_TYPE_EXFAT;
            iRet = ERR_SUCCESS;
            // DSM_HstSendEvent(0xaabb8805);
            goto _func_end;
        }
        else
        {
            D( ( DL_FATERROR,"in fs_read_super, exfat_read_sb_info failed,iSecNo = 0x%x, err_code = %d\n",iSecNo,iResult));    
            iRet = _ERR_FAT_BPB_ERROR;
            // DSM_HstSendEvent(0xaabb8806);
            goto _func_end;
        }        
    }
    else
    {
        iResult = fat_read_sb_info(uDevNo,iSecNo,fat_sb);
        if( _ERR_FAT_SUCCESS == iResult)
        {
            sb->s_fstype = FS_TYPE_FAT;
            iRet = ERR_SUCCESS;
            // DSM_HstSendEvent(0xaabb8807);
            goto _func_end;
        }
        else
        {
            D( ( DL_FATERROR,"in fs_read_super, fat_read_sb_info failed,iSecNo = 0x%x,  err_code = %d\n",iSecNo,iResult));
            iRet = _ERR_FAT_BPB_ERROR;
            // DSM_HstSendEvent(0xaabb8808);
            goto _func_end;
        }
    }
    _func_end:

    if(secbuf)
    {
        DSM_Free(secbuf);
    }
    if(pMbr)
    {
        DSM_Free(pMbr);
    }
    return iRet;
}


INT32 fs_has_formated(UINT32 uDevNo)
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb = NULL;
    // DSM_HstSendEvent(0xaabb9901);
    // DSM_HstSendEvent(uDevNo);
    iResult = fs_find_sb(uDevNo, &sb);
    if(iResult == ERR_SUCCESS)
    {
        D((DL_FATERROR, "in fs_has_formated the device has mount. uDevNo = %d\n",uDevNo));
        // DSM_HstSendEvent(0xaabb9902);
        return _ERR_FAT_HAS_FORMATED;
    }

    iResult = fs_get_free_sb(&sb);
    if(iResult != ERR_SUCCESS)
    {
        D((DL_FATERROR, "in fs_has_formated no more sb itme. uDevNo = %d\n",uDevNo));
        // DSM_HstSendEvent(0xaabb9903);
        return iResult;
    }
    iResult = fs_read_super(sb,uDevNo);
    if(iResult != ERR_SUCCESS)
    {
        D(( DL_FATERROR, "in fs_has_formated[%d]:: Read sb failed.iResult = %d.", uDevNo,iResult));
        iRet = _ERR_FAT_NOT_FORMAT;
        // DSM_HstSendEvent(0xaabb9904);
        goto _func_end;
    }
    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_is_formated(sb);
       // DSM_HstSendEvent(0xaabb9905);
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_is_formated(sb);   
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
_func_end:
    if(sb)
    {
        fs_delete_sb(uDevNo);
    }
    return iRet;
}


UINT32 fs_get_fs_type(UINT64 iTotalSec)
{
    UINT32 iType = FS_TYPE_UNKNOW;

    if(8 > iTotalSec)
    {
       iType = FS_TYPE_UNKNOW;
    }
    else if(2097152 > iTotalSec)
    {
        iType = FS_TYPE_FAT;
    }
    else
    {
        iType = FS_TYPE_EXFAT;
    }
    return iType;
}

UINT32 fs_get_partition_offset(UINT32 iTotalSec)
{
    UINT32 iOffset = 0;

    if(4194304 > iTotalSec)
    {
        iOffset = 64;
    }
    else if(67108864 > iTotalSec)
    {
        iOffset = 8192;
    }
    else if(268435456 > iTotalSec)
    {
        iOffset = 32768;
    }
    else
    {
        iOffset = 65536;
    }
    
    return iOffset;
}



INT32 fs_format(UINT32 uDevNo,UINT8 iFsType,UINT8 iFlag)
{
    INT32 iResult;
    INT32 iRet;    
    UINT32 iBlockSize;
    UINT64 iBlockCount = 0;
    UINT64 iDiskTotSec = 0;
    UINT64 iBlockCount32 = 0;
    UINT32 iDiskTotSec32 = 0;
    UINT32 iPartitionOffset = 0;
    UINT8 iOriFsType = 0;
    UINT8 szVolLab[FBR_VOL_LAB_SIZE + 1];
    UINT8 szOEMName[FBR_OEM_NAME_SIZE + 1];
    struct super_block* sb;
    UINT8 *secbuf = NULL;
    MBR_BOOT_RECORD* pMbr = NULL;
    
    //sb = fs_get_super( uDevNo );
    iResult = fs_find_sb(uDevNo, &sb);
    if (ERR_SUCCESS == iResult)
    {
        D( ( DL_FATERROR,"fs_format: a device which is mounting is NOT permitted" )); 
        iRet = ERR_FS_FORMAT_MOUNTING_DEVICE;                
        return iRet;
    }	
    iBlockSize = 0;
    iBlockCount = 0;
    iBlockCount32 = 0;
    if(DSM_IsRemoveableDev(uDevNo))
    {
        D( ( DL_FATDETAIL, " Call DRV_GET_DEV_TFLASH_INFO  uDevNo = %d.\n", uDevNo));
        iResult = DRV_GET_DEV_TFLASH_INFO(uDevNo,&iBlockCount,&iBlockSize);
        if(ERR_SUCCESS != iResult)
        {
            D( ( DL_FATERROR,
                    " DRV_GET_DEV_FLASH_INFO 1  failed!,iResult = %d.\n",
                    iResult)); 
            iRet = ERR_FS_GET_DEV_INFO_FAILED;                
            return iRet;
        }
         iDiskTotSec = ((UINT64)iBlockSize / DEFAULT_SECSIZE) * iBlockCount;
         iDiskTotSec32 = (UINT32)((iBlockSize / DEFAULT_SECSIZE) * (UINT32)iBlockCount);
         D( ( DL_FATDETAIL,
            " DRV_GET_DEV_TFLASH_INFO  success!,iBlockCount = %lld,iBlockSize = %d.\n",
            iBlockCount,
            iBlockSize));
    }
    else
    {
        D( ( DL_FATDETAIL, " Call DRV_GET_DEV_FLASH_INFO  uDevNo = %d.\n", uDevNo));
        iResult = DRV_GET_DEV_TFLASH_INFO(uDevNo,&iBlockCount32,&iBlockSize);
        if(ERR_SUCCESS != iResult)
        {
            D( ( DL_FATERROR,
                    " DRV_GET_DEV_FLASH_INFO 2  failed!,iResult = %d.\n",
                    iResult)); 
            iRet = ERR_FS_GET_DEV_INFO_FAILED;                
            return iRet;
        }
         iDiskTotSec = ((UINT64)iBlockSize / DEFAULT_SECSIZE) * (UINT64)iBlockCount32;
         iDiskTotSec32 = ((iBlockSize / DEFAULT_SECSIZE) * iBlockCount32);
         D( ( DL_FATDETAIL,
            " DRV_GET_DEV_TFLASH_INFO  success!,iBlockCount32 = 0x%x,iBlockSize = %d.\n",
            iBlockCount32,
            iBlockSize));
    }
    DSM_StrCpy((INT8*)szVolLab,(INT8*)FAT_VOLLABLE);
    DSM_StrCpy((INT8*)szOEMName,(INT8*)FAT_OEMNAME);
    if(DSM_IsRemoveableDev(uDevNo))
    {
        secbuf = FAT_SECT_BUF_ALLOC();    
        if(!secbuf)
        {
            D((DL_FATERROR, "fs_format: FAT_SECT_BUF_ALLOC failed."));
            DSM_ASSERT(0,"fs_format: FAT_SECT_BUF_ALLOC failed.");
            iRet = _ERR_FAT_MALLOC_FAILED;
            goto _func_end;
        }
        
        pMbr = DSM_MAlloc(sizeof(MBR_BOOT_RECORD));
        if(!pMbr)
        {
            D((DL_FATERROR, "fs_format: malloc(%d) failed.",sizeof(MBR_BOOT_RECORD)));
            DSM_ASSERT(0,"fs_format: malloc(%d) failed.",sizeof(MBR_BOOT_RECORD));
            iRet =  _ERR_FAT_MALLOC_FAILED;
            goto _func_end;
        }
        //check MBR 
        iResult = fs_read_mbr(secbuf, pMbr, uDevNo, &iOriFsType, &iPartitionOffset);
        //iResult = fs_write_mbr(secbuf, pMbr, uDevNo, (UINT8)iFsType, iDiskTotSec);
        if(ERR_SUCCESS != iResult)
        {
            iResult = fs_write_mbr(secbuf, pMbr, uDevNo, (UINT8)iFsType, iDiskTotSec32);
            if(ERR_SUCCESS != iResult)
            {
                D((DL_FATERROR, "fs_write_mbr(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
                iRet = _ERR_FAT_WRITE_SEC_FAILED;
                goto _func_end;
            }
            
            iPartitionOffset = pMbr->sub_rec[0].sectors_preceding;
        }
        else if(iFsType != iOriFsType)
        {
            iResult = fs_set_mbr_type(secbuf, pMbr, uDevNo, (UINT8)iFsType, (UINT32)iDiskTotSec);
            if(ERR_SUCCESS != iResult)
            {
                D((DL_FATERROR, "fs_set_mbr_type(),DRV_BLOCK_READ error ErrorCode = %d",iResult));
                iRet = _ERR_FAT_WRITE_SEC_FAILED;
                goto _func_end;
            }

            iPartitionOffset = pMbr->sub_rec[0].sectors_preceding;
        }

    }
    CSW_TRACE(BASE_DSM_TS_ID,
            "Call fat_format with uDevNo = %d, iDiskTotSec = 0x%x,szVolLab = %s,szOEMName = %s,iOpt = %d.\n",
            uDevNo,
            iDiskTotSec32,
            (UINT8*)szVolLab,
            (UINT8*)szOEMName,
            iFlag); 
    switch(iFsType)
    {
    case FS_TYPE_FAT:
       iResult = fat_format(uDevNo, iDiskTotSec32, iPartitionOffset, (UINT8*)szVolLab,(UINT8*)szOEMName,iFlag);
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_format(uDevNo, iDiskTotSec, (UINT64)iPartitionOffset, (UINT8*)szVolLab,(UINT8*)szOEMName,iFlag);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    
    _func_end:

    if(secbuf)
    {
        DSM_Free(secbuf);
    }
    if(pMbr)
    {
        DSM_Free(pMbr);
    }
    
    return iRet;
}


INT32 fs_get_space_info(UINT32 uDevNo,UINT64* piTotalSize,UINT64* piFreeSize)
{

    UINT32 iFreeClusNum;
    UINT32 iSizePerClus = 0;
    UINT64 iTotalSize = 0;
    UINT64 iFreeSize = 0;
    struct super_block* sb = NULL;
    FAT_SB_INFO*  fat_sb;
    EXFAT_SB_INFO*  exfat_sb;
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;
    
    iResult = fs_find_sb(uDevNo,&sb);
    if(iResult != ERR_SUCCESS) 
    {
        iRet = iResult;
        return iRet;
    }
    iFreeSize = 0;
    iFreeClusNum = 0;
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
        fat_sb = &sb->u.fat_sb;
        iSizePerClus = (fat_sb->iBytesPerSec * fat_sb->iSecPerClus);
        iTotalSize = (UINT64)(fat_sb->iDataSec/fat_sb->iSecPerClus)*iSizePerClus; 
        iResult = fat_stat_free_cluster(fat_sb, &iFreeClusNum);
        iRet = iResult;
        break;
    case FS_TYPE_EXFAT:
        exfat_sb = &sb->u.exfat_sb;
        iSizePerClus = (exfat_sb->SectorsPerCluster * exfat_sb->BytesPerSector);
        iTotalSize = (UINT64)(exfat_sb->ClusterCount * iSizePerClus); 
        iResult = exfat_stat_free_cluster(exfat_sb, &iFreeClusNum);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }       

    if(iRet == ERR_SUCCESS)
    {
        iFreeSize = (UINT64)iFreeClusNum*iSizePerClus;
        *piFreeSize = iFreeSize;
        *piTotalSize = iTotalSize;
    }
    return iRet;
}


INT32 fs_do_link( struct dentry *old_de, struct inode *dir_i, struct dentry *de, UINT32 type )
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    sb = dir_i->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_do_link(old_de,dir_i,de,type);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_do_link(old_de,dir_i,de,type);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_do_unlink( struct inode *dir_i, struct dentry *de, BOOL bRename )
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    sb = dir_i->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_do_unlink(dir_i,de,bRename);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_do_unlink(dir_i,de,bRename);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


VOID fs_flush_cache(UINT32 uDevNo)    
{
    FAT_CACHE* psFatCache;
    struct super_block* sb; 
    INT32 iResult;
     
    iResult = fs_find_sb(uDevNo,&sb);
    if(iResult != ERR_SUCCESS) 
    {
        return;
    }
    psFatCache = fat_get_fat_cache(uDevNo);
    if(psFatCache->iSecNum != 0)
    {
          fat_synch_fat_cache(&(sb->u.fat_sb), psFatCache);
    }
    psFatCache->iDirty = 0;
    psFatCache->iSecNum = 0;
    psFatCache->psFileInodeList = NULL;        
}


INT32 fs_create_file( struct inode *dir_i, struct dentry *de, UINT8 mode )
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    sb = dir_i->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_create_file(dir_i,de,mode);
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_create_file(dir_i,de,mode);  
        iRet = iResult;
        break;
    default:
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_trunc_file(struct inode * dir_i, struct dentry * de, UINT8 mode)
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    sb = dir_i->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_trunc_file(dir_i,de,mode);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_trunc_file(dir_i,de,mode);  
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_file_read(struct file * filp, INT8 * buf, INT32 len)
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    
    sb = filp->f_inode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_file_read(filp,buf,len);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_file_read(filp,buf,len);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_file_write(struct file * filp, INT8 * buf, INT32 len)
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 
    
    sb = filp->f_inode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_file_write(filp,buf,len);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_file_write(filp,buf,len);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_file_extend(struct file * filp, UINT64 SizeExtend)
{
    INT32 iResult;
    INT32 iRet;    
    struct super_block* sb; 

    sb = filp->f_inode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_file_extend(filp,SizeExtend);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_file_extend(filp,SizeExtend);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}

INT64 fs_file_truncate(struct file * filp, UINT64 SizeExtend)
{
    INT32 iResult;
    INT32 iRet;    
    struct inode *pinode;
    struct super_block* sb; 

    pinode = filp->f_inode;
    sb = filp->f_inode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_file_truncate(pinode,SizeExtend);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_file_truncate(pinode,SizeExtend);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_write_inode( struct inode * pinode)
{
    INT32 iResult;
    INT32 iRet;       
    struct super_block* sb; 
    
    sb = pinode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_write_inode(pinode);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_write_inode(pinode);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}

#if 0 
INT32 fs_get_dir_size(struct inode *pinode,UINT64* pSize,BOOL IgnoreDir)
{
    INT32 iResult;
    INT32 iRet;       
    struct super_block* sb; 
    
    sb = pinode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_get_dir_size(pinode,pSize,IgnoreDir);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
      //  iResult = exfat_get_dir_size(pinode,pSize,IgnoreDir);
		iResult = 0;
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}    
#endif
INT32 fs_get_file_name(struct inode * pinode, WCHAR *fileName)
{
    INT32 iResult;
    INT32 iRet;       
    struct super_block* sb; 
    
    sb = pinode->i_sb;    
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:
       iResult = fat_get_file_name(pinode,fileName);       
       iRet = iResult;
       break;
    case FS_TYPE_EXFAT:
        iResult = exfat_get_file_name(pinode,fileName);
        iRet = iResult;
        break;
    default:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    return iRet;
}


INT32 fs_scan_disk(UINT8* pszDevName)
{      
    UINT32 uDevNo;
    INT32 iResult;
    INT32 iRet;
    struct super_block* sb = NULL; 

    uDevNo = DSM_DevName2DevNo(pszDevName);
    if(INVALID_DEVICE_NUMBER == uDevNo)
    {
        D( ( DL_FATERROR, "in fs_scan_disk,uDevNo = %d\n",uDevNo) );
        D( ( DL_FATERROR, "in fs_scan_disk, DevName = %s\n",pszDevName) );
        return ERR_FS_DEVICE_NOT_REGISTER;
    }
    iResult = fs_find_sb(uDevNo, &sb);
    if(iResult == ERR_SUCCESS)
    {
        D((DL_FATERROR, "in fs_scan_disk the device is mounted. uDevNo = %d\n",uDevNo));
        return _ERR_FAT_HAS_MOUNTED;
    }

    iResult = fs_get_free_sb(&sb);
    if(iResult != ERR_SUCCESS)
    {
        D((DL_FATERROR, "in fs_scan_disk no more sb itme. uDevNo = %d\n",uDevNo));
        return iResult;
    }
    
    iResult = fs_read_super(sb,uDevNo);;
    if(iResult != ERR_SUCCESS)
    {
        D(( DL_FATERROR, "fs_scan_disk[%d]:Read sb failed.iResult = %d.", uDevNo,iResult));
        iRet = iResult;
        goto _func_end;
    }  
    switch(sb->s_fstype)
    {
    case FS_TYPE_FAT:       
        iResult = fat_scan_disk(sb);
		iRet = iResult;
		break;
    case FS_TYPE_EXFAT:
        iResult = exfat_scan_disk(sb);
		iRet = iResult;
		break;
    default:
        D( ( DL_FATERROR, "in fs_scan_disk, unknown file system\n") );
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
    
    if(DSM_IsRemoveableDev(uDevNo))
    {
       if(!fs_is_opened(uDevNo))
       {
          DRV_DEV_DEACTIVE(uDevNo);
       }
    }
_func_end:
    if(sb)
    {
        fs_delete_sb(uDevNo);
    }
    return iRet;    
}
