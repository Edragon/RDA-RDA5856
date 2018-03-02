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
#include "dsm_cf.h"
#include "fat_base.h"
#include "exfat_base.h"
#include "fat_local.h"
#include "exfat_local.h"
#include "fs.h"
#include "fs_base.h"
#ifdef _FS_SIMULATOR_
#include <time.h>
#endif

static void wait_on_inode( struct inode * inode )
{
    inode = inode;
}


void iput( struct inode * inode )
{
    if( NULL == inode)
    {
        D((DL_VFSERROR, "in iput, inode is NULL\n"));
        return;
    }
    
    wait_on_inode( inode );

    if( 0 == inode->i_count)
    {
        D((DL_VFSERROR, "in iput, inode->i_count = %d\n",inode->i_count));
        return;
    }
    
    if ( !inode->i_dev )             /*should be an empty inode in g_fs_inode_table*/
    {
        D((DL_VFSWARNING, "in iput, inode->i_dev = %d\n",inode->i_dev));
        inode->i_count--;
        return;
    }
    inode->i_count--;

    if ( inode->i_count > 0 )
    {
        return ;
    }

    if ( FS_TYPE_FFS == inode->i_fstype )
    {
        // ffs_clear_inode( inode );
    }
    else if(FS_TYPE_FAT == inode->i_fstype)
    {
        fat_write_inode( inode);
        inode_list_del(&(inode->i_sb->inode_chain), inode);
    }
    else if(FS_TYPE_EXFAT == inode->i_fstype)
    {
        exfat_write_inode( inode);
        inode_list_del(&(inode->i_sb->inode_chain), inode);
    }
    return ;
}


#if 0
struct inode *get_empty_inode( void )
{
    struct inode *inode;
    int i;
    for ( i = 0; i < FS_NR_INODES_MAX; i++ )
    {
        if ( !g_fs_inode_table[ i ].i_count )
            break;
    }
    if( FS_NR_INODES_MAX == i) //The maximum inode has created
    {
        return NULL;
    }
    inode = g_fs_inode_table + i;
    wait_on_inode( inode );
    DSM_MemSet( inode, 0, sizeof( *inode ) );
    return inode;
}
#endif
struct inode* get_empty_inode( void )
{
    struct inode * inode = NULL;

    inode = (struct inode *)DSM_MAlloc(SIZEOF(struct inode));
    if( NULL == inode)
    {
        DSM_ASSERT(0,"get_empty_inode: 1.malloc(0x%x) failed.",SIZEOF(struct inode)); 
        return NULL;
    }
    
    DSM_MemSet( inode, 0x00, sizeof( *inode ) );
    return inode;
}


struct inode* inode_list_add(struct inode **ppsInodeList, struct inode *psInode)
{
    if (NULL == ppsInodeList || NULL == psInode)
    {
        D ((DL_FATERROR, "inode_list_add: _ERR_FAT_PARAM_ERROR, ppsInodeList(0x%x), psInode(0x%x)", ppsInodeList, psInode));
        return NULL;
    }
    
    psInode->next = *ppsInodeList;
    *ppsInodeList = psInode;
    return psInode;
}


INT32 inode_list_del(struct inode **ppsInodeList, struct inode *psInode)
{
    struct inode **ppPrei;
    struct inode *pCuri;
    
    if (NULL == ppsInodeList || NULL == psInode)
    {
        D ((DL_FATERROR, "inode_list_del: _ERR_FAT_PARAM_ERROR, ppsInodeList(0x%x), psInode(0x%x)", ppsInodeList, psInode));
        return ERR_FS_INVALID_PARAMETER;
    }

    ppPrei = ppsInodeList;
    pCuri = *ppsInodeList;
    while (pCuri != NULL)
    {
        if (pCuri == psInode)
        {
            *ppPrei = pCuri->next;
            DSM_Free(pCuri);
            return ERR_SUCCESS;
        }
        else
        {
            ppPrei = &(pCuri->next);
            pCuri = pCuri->next;
        }
    }
    return ERR_FS_INVALID_PARAMETER;
    
}


struct inode * iget( struct super_block *sb, UINT64 inr)
{
    struct inode *inode;
    struct inode *empty;
    UINT32 dev_nr;
    long lRet = ERR_SUCCESS;

    dev_nr = sb->s_dev;
    //inode = g_fs_inode_table;
    inode = sb->inode_chain;

    while (inode != NULL)
    {
        if ( 0 == inode->i_count ||     /* unused inode */
              inode->i_dev != dev_nr ||
              inode->i_ino != inr )
        {       
            inode = inode->next;
            continue;
        }

        wait_on_inode( inode );

        inode->i_count++;

        if ( inode->i_is_mountpoint )
        {
            sb = fs_get_inode_super(inode);
            if(!sb)
            {
                return NULL;
            }            
            iput( inode );
            dev_nr = sb->s_dev;
            inr = FS_ROOT_INO;
            inode = sb->inode_chain;
            continue;
        }

        return inode;
    }

    empty = get_empty_inode();

    if( NULL == empty ) // inode table is full
    {
        D((DL_VFSWARNING, "in iget, inode table is full\n"));
        return NULL;
    }

    inode = empty;
    inode->i_count = 1;
    inode->i_dev = dev_nr;
    inode->i_ino = inr;
    inode->i_sb = sb;
    inode->i_fstype = sb->s_fstype;
    //inode->i_atime = inode->i_mtime = inode->i_ctime = DSM_GetFileTime();
    inode = inode_list_add(&sb->inode_chain, inode);

    
    if ( FS_TYPE_FAT == inode->i_fstype )
    {
        lRet = fat_read_inode( inode);
    }
    else if( FS_TYPE_EXFAT == inode->i_fstype )
    {
        lRet = exfat_read_inode( inode);
    }
    else
    {
        iput( inode );
        return NULL;
    }

    if ( lRet != ERR_SUCCESS ) // reading inode from flash failed
    {
        D((DL_VFSERROR, "in iget, fat_read_inode error,lRet = %d\n",lRet));
        iput( inode );
        return NULL;
    }
    return inode;
}

