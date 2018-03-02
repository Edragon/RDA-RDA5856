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


#ifndef SHEEN_VC_DEBUG
#include "global.h"
#endif

#ifdef MP4_3GP_SUPPORT

#ifndef _LIST_H_
#define _LIST_H_

#include "tools.h"

struct _tag_array
{
	void **slots;
	uint32 entryCount;
};

typedef struct _tag_array GF_List;


/*!
 *	\brief list constructor
 *
 *	Constructs a new list object
 *	\return new list object
 */
GF_List *gf_list_new();
/*!
 *	\brief list destructor
 *
 *	Destructs a list object
 *	\param ptr list object to destruct
 *	\note It is the caller responsability to destroy the content of the list if needed
 */
void gf_list_del(GF_List *ptr);

uint32 gf_list_count(GF_List *ptr);
/*!
 *	\brief add item
 *GF_Err
 *	Adds an item at the end of the list
 *	\param ptr target list object
 *	\param item item to add
 */
GF_Err gf_list_add(GF_List *ptr,void* item);
GF_Err gf_list_add2(GF_List *ptr, void *item);//add shenh
GF_Err gf_list_add3(GF_List *ptr, void *item);

GF_Err gf_list_insert(GF_List *ptr, void *item, uint32 position);
/*!
 *	\brief removes item
 *
 *	Removes an item from the list given its position
 *	\param ptr target list object
 *	\param position position of the item to remove. It is expressed between 0 and gf_list_count-1.
 *	\note It is the caller responsability to destroy the content of the list if needed
 */
GF_Err gf_list_rem(GF_List *ptr, uint32 position);
/*!
 *	\brief gets item
 *
 *	Gets an item from the list given its position
 *	\param ptr target list object
 *	\param position position of the item to get. It is expressed between 0 and gf_list_count-1.
 */
void *gf_list_get(GF_List *ptr, uint32 position);
/*!
 *	\brief finds item
 *
 *	Finds an item in the list
 *	\param ptr target list object.
 *	\param item the item to find.
 *	\return 0-based item position in the list, or -1 if the item could not be found.
 */
s32 gf_list_find(GF_List *ptr, void *item);
/*!
 *	\brief deletes item
 *
 *	Deletes an item from the list
 *	\param ptr target list object.
 *	\param item the item to find.
 *	\return 0-based item position in the list before removal, or -1 if the item could not be found.
 */
s32 gf_list_del_item(GF_List *ptr, void *item);
/*!
 *	\brief resets list
 *
 *	Resets the content of the list
 *	\param ptr target list object.
 *	\note It is the caller responsability to destroy the content of the list if needed
 */
void gf_list_reset(GF_List *ptr);

#endif

#endif


