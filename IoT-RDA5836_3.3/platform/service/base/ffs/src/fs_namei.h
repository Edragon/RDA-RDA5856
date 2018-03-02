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
#ifndef __FS_NAMEI_H__
#define __FS_NAMEI_H__

int pattern_match( WCHAR *name, WCHAR *pattern );
INT32 pattern_match_oem(UINT8 *name, UINT8 *pattern);
int ext_match(const UINT8 *name, const UINT8 *pattern );
UINT8 ext_match_unicode(const UINT16 *name, const UINT8 *pattern );
int do_find_entry(struct inode ** dir_i, struct dentry *de);
INT32  find_entry( struct inode ** dir_i, struct dentry *de );
int get_next_entry( struct inode * dir_i, UINT64 *ino, UINT8 type, UINT32 *pCount, const PTCHAR pExtName, UINT8 mode);
int get_parent_node(struct inode * dir_i, UINT64 *ino);
int find_entry_by_cluster(struct inode * dir_i, UINT32 entryclu, UINT64 *ino);
struct inode *dir_namei(PCWSTR pathname, INT32 *namelen, CONST WCHAR **basename);
struct inode *namei( PCWSTR pathname );
INT32 fs_close(INT32 fd);
INT32 fs_findclose(INT32 fd);
INT32 fs_unicodeNameToFATFileName(WCHAR *pUniName, PSTR pFatName);

#endif  //#ifndef __FS_NAMEI_H__