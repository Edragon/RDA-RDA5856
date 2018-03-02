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
#ifndef _CII_FS_BASE_H
#define _CII_FS_BASE_H

//CHS para
#define SECTOR_PER_CYLINDER                 16065
#define HEAD_PER_CYLINDER                   255
#define SECTOR_PER_TRACK                     63
// VFS Version.
#define FS_VFS_VERSION                      0x1002

// Create mode define
// FILE
#define FS_MODE_FILE 0
// Directory
#define FS_MODE_DIR  1

// fs status is power on.
#define FS_POWER_ON             0x01

// fs status is power off.
#define FS_POWER_OFF            0x00


#define FS_FLAG_NOT_FIND_FILE   0
#define FS_FLAG_FIND_FILE       1

#define FS_DEFAULT_NR_SUPER 4

#define FS_DEFAULT_DEV_COUNT 2

#define PATTERN_CMP_MATCH           1
#define PATTERN_CMP_UNMATCH         2

#define DOT_FOUND                   1
#define DOTDOT_FOUND                2
#define DIR_ENTRY_FOUND             4
#define DIR_ENTRY_NOT_FOUND         5

// Minix文件系统中根目录使用1号i节点，但在FAT12及FAT16中，
// 1会被转换成根目录下的第二个目录项的物理位置导致出错。
#define FS_ROOT_INO                 0xffffffffffffffff
#define FS_PARENT_INO               (FS_ROOT_INO-1) 

// use 1 for the fat16 root cluster to provide ino to all zero
#define FS_ROOT_CLUSTER             (1) // root cluster for FAT16
#define LEN_FOR_NULL_CHAR 1

enum e_GetEntryMode{
	GET_PARENT_ENTRY,
	GET_NEXT_ENTRY,
	GET_PREV_ENTRY,
	GET_LAST_ENTRY,
	GET_ENTRY_COUNT,
	GET_ENTRY_NO,
	GET_ENTRY_CLUSTER
};

// ================
// The state  for  check file name begin.
// NV: Name Valid.
typedef enum
{
    FS_NV_INIT,                                     // initialization
    FS_NV_PARTITIOIN,                         // dir entry partition "/" or "\\"
    FS_NV_BASE_ENTRY,                       // base entry name.
    FS_NV_WILDCARD_ENTRY,              // wildcard character as "*" "?"
    FS_NV_BASE_ENTRY_END,              // base entry end.
    FS_NV_WILDCARD_ENTRY_END,     // wildcard character end.
    FS_NV_NULL_ENTRY_END,              // NULL entry.
    FS_NV_INVALID_ENTRY_END        // invalid entry, include invalid character.as
} FS_NV_STATUE;
// Define the character type judge macro.
// Dir entry partition character.include \ /
#define IS_NV_PART(ch)         (0x2f == ch || 0x5c == ch)

// Wildcard character. include * ?
#define IS_NV_WILDCARD(ch)  (0x2a == ch || 0x3f == ch)

// Invalid character. include  : "  < > |
#define IS_NV_INVALID(ch)    (0x3a == ch || \
                                                0x22 == ch || \
                                                0x3c == ch || \
                                                0x3e == ch || \
                                                0x7c == ch)

// The base long name character. not include \ / * ? : "  < > |
#define IS_NV_BASE(ch) (!IS_NV_PART(ch) &&\
                                         !IS_NV_WILDCARD(ch) &&\
                                         !IS_NV_INVALID(ch))
// Terminal character. NULL.
#define IS_TERMINAL(ch) (0 == ch)

// The state  for  check file name end.
// ================

#if defined(_MS_VC_VER_) ||defined(_T_UPGRADE_PROGRAMMER)
#define FS_SemInit() 
#define FS_Down()    
#define FS_Up()   
#else
//#define FS_SemInit()  { if (!g_fs_sem) { g_fs_sem = COS_CreateSemaphore(1); }} 
 #if OS_KERNEL==SXR
#define FS_Down()   sxr_TakeSemaphore(g_fs_sem)
#define FS_Up()     sxr_ReleaseSemaphore( g_fs_sem ) 
#else
#define FS_Down()   COS_WaitForSemaphore(g_fs_sem,1000)
#define FS_Up()     COS_ReleaseSemaphore( g_fs_sem ) 
#endif
#endif

// FS configure information。
struct fs_cfg_info
{
    UINT32 uNrSuperBlock;    // max super-block number.
    UINT32 uNrOpenFileMax; // max open file number ontime.
};

struct super_block
{
    UINT32  s_dev; /*MUST be long, for we will convert it to pointer*/
    UINT32  s_magic;
    struct inode *s_root;
    struct inode *s_mountpoint;
    UINT32 s_time;
    //struct task_struct * s_wait;
    UINT8 s_lock;
    UINT8 s_rd_only;
    UINT8 padding[1];
    UINT8 s_fstype;
    struct inode *inode_chain;
    struct fat_fsi *fat_fsi;
    union
    {
        struct fat_sb_info  fat_sb;
        struct exfat_sb_info  exfat_sb;
        void       *generic_sbp;
    } u;
};

struct inode
{
    union
    {
        struct fat_inode_info        fat_i;
        struct exfat_inode_info    exfat_i; 
        //struct ffs_inode_info        ffs_i;
        void                        *generic_ip;
    } u;

    UINT32 i_dev;                /*MUST be long, for we will convert it to pointer*/
    UINT64 i_ino;                /*inode number*/
    UINT64 i_size;            /*the length of the file*/
    UINT32 i_mode;
    UINT32 i_count;
    UINT8  i_nlinks;
    UINT8  i_is_mountpoint;
    UINT8  i_lock;
    UINT8  pad[1];
    UINT32  i_fstype;
    UINT32 i_atime;       /*Time of last access to the file*/
    UINT32 i_mtime;      /*Time of last data modification*/
    UINT32 i_ctime;       /*Time of last status(or inode) change*/
    struct super_block    *i_sb;
    struct inode *next;
};

struct dentry
{
    struct inode *inode;
    WCHAR name [FS_FILE_NAME_LEN + LEN_FOR_NULL_CHAR];
    UINT64 append_data;        // append data
    UINT32 append_data2;
};

struct file
{
    UINT32 f_mode;
    UINT16 f_flags;
    UINT16 f_count;
    UINT64 f_pos;
    PVOID  f_pattern;
    //    PVOID  f_prvdata;
    struct inode *f_inode;
    struct file *next;
};


struct task_struct
{
    struct inode *pwd;
    struct inode *root;
    struct file   *filp[FS_NR_OPEN_FILES_MAX];
    WCHAR wdPath[FS_PATH_LEN + LEN_FOR_NULL_CHAR]; // 当前工作目录的路径。
};

#define DENTRY_ALLOC()  (struct dentry*)DSM_MAlloc(sizeof(struct dentry))
#define DENTRY_FREE(psDe)     DSM_Free(psDe)



#define IS_COMPONENT_SEPATATOR( c ) ( _T_DSM('\\') == (c) || _T_DSM('/') == (c) )
#define IS_VALID_DRIVER_NAME( c ) ( (_T_DSM('c') <= (c) && _T_DSM('z') >= (c)) ||(_T_DSM('C') <= (c) && _T_DSM('Z') >= (c)) )


// local function
void iput( struct inode *inode );
struct inode * iget( struct super_block *sb, UINT64 inr);

extern struct fs_cfg_info g_fs_cfg;

#define FD_IS_VALID( fd ) ((fd) >= 0 && (INT32)( fd ) < (INT32)g_fs_cfg.uNrOpenFileMax)

#define FS_O_FIND                00400

//BOOL is_unicode_name_too_long(CONST WCHAR *UniPathname );
BOOL is_file_name_valid( PCWSTR pathname, BOOL bIgnoreBaseName );
struct inode *dir_namei( PCWSTR pathname, INT32 *namelen, CONST WCHAR **basename );
INT32 find_entry( struct inode ** dir_i, struct dentry *de );
WCHAR *format_dir_name( PCWSTR pathname );
int pattern_match( WCHAR *name, WCHAR *pattern );
INT32 pattern_match_oem(UINT8 *name, UINT8 *pattern);
int ext_match(const UINT8 *name, const UINT8 *pattern );
int get_next_entry( struct inode * dir_i, UINT64 *ino, UINT8 type, UINT32 *pCount, const PTCHAR pExtName, UINT8 mode);
int do_find_entry( struct inode ** dir_i, struct dentry *de);
int get_parent_node(struct inode * dir_i, UINT64 *ino);
int find_entry_by_cluster(struct inode * dir_i, UINT32 startclu, UINT64 *ino);
struct inode *get_empty_inode( void );
struct inode *namei( PCWSTR pathname );
struct super_block*  fs_get_super( UINT32 devnr );
struct super_block* fs_get_inode_super(struct inode* inode);
VOID fs_delete_sb(UINT32 iDev);
struct inode* inode_list_add(struct inode **ppsInodeList, struct inode *psInode);
INT32 inode_list_del(struct inode **ppsInodeList, struct inode *psInode);
INT32 fs_global_var_init();
INT32 fs_global_var_free();

INT32 fs_get_free_sb(struct super_block** psSuperBlock);
INT32 fs_find_sb(UINT32 iDev,struct super_block** psSuperBlock);
INT32 fs_set_work_dir(WCHAR* WorkDirPath, WCHAR* ChangeDirPath);
INT32 fs_set_error_code(INT32 iErrCode);

struct file *fs_alloc_fd( void );
struct file *fs_fd_list_add(struct file **ppsFileList, struct file *psFD);
INT32 fs_fd_list_del(struct file **ppsFileList, struct file *psFD);
BOOL fs_is_file_opened(struct file **ppsFileList, struct file *psFD);

INT32 fs_close(INT32 fd);
INT32 fs_findclose(INT32 fd);

// Check the g_fs_file_table array if have opened files.
// if existed, return TRUE, else return FALSE.
BOOL fs_is_opened(UINT32 iDev);
INT32 fs_read_mbr(UINT8 *buf, MBR_BOOT_RECORD* pMbr, UINT32 uDevNo, UINT8 *fstype, UINT32 *iPartitionOffset);
INT32 fs_write_mbr(UINT8 *buf, MBR_BOOT_RECORD *pMbr, UINT32 uDevNo, UINT8 fstype, UINT32 iDiskTotSec);
INT32 fs_set_mbr_type(UINT8 *buf, MBR_BOOT_RECORD *pMbr, UINT32 uDevNo, UINT8 fstype, UINT32 iDiskTotSec);
INT32 fs_read_super( struct super_block *sb, UINT32 uDevNo);
INT32 fs_has_formated(UINT32 uDevNo);
UINT32 fs_get_fs_type(UINT64 iTotalSec);
UINT32 fs_get_partition_offset(UINT32 iTotalSec);
INT32 fs_format(UINT32 uDevNo,UINT8 iFsType, UINT8 iFlag);
INT32 fs_get_space_info(UINT32 uDevNo,UINT64* piTotalSize,UINT64* piFreeSize);
INT32 fs_do_link( struct dentry *old_de, struct inode *dir_i, struct dentry *de, UINT32 type );
INT32 fs_do_unlink( struct inode *dir_i, struct dentry *de, BOOL bRename );
VOID fs_flush_cache(UINT32 uDevNo);
INT32 fs_create_file( struct inode *dir_i, struct dentry *de, UINT8 mode );
INT32 fs_trunc_file(struct inode * dir_i, struct dentry * de, UINT8 mode);
INT32 fs_file_read(struct file * filp, INT8 * buf, INT32 len);
INT32 fs_file_write(struct file * filp, INT8 * buf, INT32 len);
INT32 fs_file_extend(struct file * filp, UINT64 SizeExtend);
INT64 fs_file_truncate(struct file * filp, UINT64 SizeExtend);
INT32 fs_write_inode( struct inode * pinode);
//INT32 fs_get_dir_size(struct inode *pinode,UINT64* pSize,BOOL IgnoreDir);
INT32 fs_get_file_name(struct inode * pinode, WCHAR *fileName);
INT32 fs_scan_disk(UINT8* pszDevName);

#define _ERR_FS_SUCCESS             ERR_SUCCESS

#define _ERR_FS_NO_MORE_SB_ITEM       -101001 // no more super block item in the table.
#define _ERR_FS_NOT_MOUNT             -101002 // the device unmount.
#define _ERR_FS_HAS_MOUNTED               -101003 // the device unmount.
#endif //_CII_FS_BASE_H

