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


#include "ap_gui.h"
#include "string.h"


#if (APP_SUPPORT_RGBLCD==1) || (APP_SUPPORT_LCD==1)
extern BOOL ui_auto_update;
extern BOOL ui_auto_select;

extern TCHAR ExtMusic[];
extern TCHAR ExtVoice[];

#define MAX_LIST_FILENAME           50

#define DIRECTORY_MENU_OPEN    (AP_MSG_USER_DIRECTORY+1)
#define DIRECTORY_MENU_SELECT    (AP_MSG_USER_DIRECTORY+2)

enum
{
    SELECT_AND_EXIT = 0,
    FOLDER_FILE_LIST = 1,
    ROOT_LIST = 2,
};

typedef void (*FileBrowerCallBack)(file_location_t *f, UINT16 result);

typedef struct
{
    file_location_t location;
    FileBrowerCallBack cbk;
    TCHAR *ext_name;
    UINT32 highlight_index;
    INT32 last_index;
    UINT32 folder_count;
    UINT8 file_name[MAX_LIST_FILENAME]; //align UINT16
    UINT8 file_type;
    UINT8 cur_item_type;
} GUI_DIRECTORY_T, *GUI_DIRECTORY_PTR;

static GUI_DIRECTORY_PTR  gui_dir_ptr = NULL;


void GUI_ExitFileBrowser(UINT16 result);


void GUI_FileBrowserCallback(UINT16 result)
{
    FS_GetShortName(gui_dir_ptr->location.subdir_entry, gui_dir_ptr->location.subdir_name);
    FS_GetShortName(gui_dir_ptr->location.file_entry, gui_dir_ptr->location.file_name);

    if(gui_dir_ptr->cbk)
        gui_dir_ptr->cbk(&gui_dir_ptr->location, result);
}

BOOL GUI_DirectoryListSelectItem(INT32 value)
{
    UINT32 LEntry;
    UINT32 i;

    gui_dir_ptr->highlight_index = 0;

    if(gui_dir_ptr->cur_item_type == ROOT_LIST)
    {
        if(FS_MountDevice(value+1)==ERR_SUCCESS)
        {
            gui_dir_ptr->cur_item_type = FOLDER_FILE_LIST;
            FS_ChangeDir(FS_CD_ROOT);
            gui_dir_ptr->location.subdir_entry = FS_ROOT_INO;
            return TRUE;
        }
        else
        {
            GUI_DisplayMessage(0, GUI_STR_MREADERR, NULL, GUI_MSG_FLAG_WAIT);
            return FALSE;
        }
    }

    gui_dir_ptr->cur_item_type = FOLDER_FILE_LIST;
    if(value==0)
    {
        if(ERR_SUCCESS != FS_ChangeDir(FS_CD_PARENT))
        {
            gui_dir_ptr->cur_item_type = ROOT_LIST; // goto root
        }
        else
            FS_GetSubDirEntry(&gui_dir_ptr->location.subdir_entry);
    }
    else if(value<=gui_dir_ptr->folder_count)
    {
        FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR);
        for(i=0; i<value-1; i++)
            FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR_CONT);
        FS_ChangeSubDir(LEntry);
        FS_GetSubDirEntry(&gui_dir_ptr->location.subdir_entry);
    }
    else
    {
        FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE);
        for(i=0; i<value-gui_dir_ptr->folder_count-1; i++)
            FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE_CONT);
        gui_dir_ptr->location.file_entry = LEntry;


        GUI_ExitFileBrowser(LIST_CALLBACK_SELECT_ITEM);
    }

    return TRUE;
}



INT32 file_list_callback(INT32 type, INT32 value, INT8 **string)
{
    UINT32 LEntry;
    INT32 result = 0;
    INT i;
    if(LIST_CALLBACK_GET_ITEM == type)
    {
        if(gui_dir_ptr->cur_item_type == ROOT_LIST)
        {
            if(value==0)
                *string = "TFlashCard";
#if APP_SUPPORT_USB
            else
                *string = "USB Disk";
#endif
            return 2;
        }

        if(value==0)
        {
            *string = "Up ..";
            return 0;
        }
        else if(value<=gui_dir_ptr->folder_count)
        {
            if(value == 1)
                FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR);
            else if(value == gui_dir_ptr->last_index+1)
                FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR_CONT);
            else
            {
                FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR);
                for(i=0; i<value-1; i++)
                    FS_GetNextEntry(&LEntry, NULL, FS_DIR_DIR_CONT);
            }
            result = 0;
        }
        else
        {
            if(value == gui_dir_ptr->folder_count+1)
                FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE);
            else if(value == gui_dir_ptr->last_index+1)
                FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE_CONT);
            else
            {
                FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE);
                for(i=0; i<value-gui_dir_ptr->folder_count-1; i++)
                    FS_GetNextEntry(&LEntry, gui_dir_ptr->ext_name, FS_DIR_FILE_CONT);
            }
            if(gui_dir_ptr->file_type == FSEL_TYPE_ALLFILE)
            {
                FS_GetShortName(LEntry, gui_dir_ptr->file_name);
                if(MUSIC_CheckType(gui_dir_ptr->file_name) == 0)
                    result = 4;
                else
                    result = 3;
            }
            else
                result = 3;
        }
        FS_GetEntryName(LEntry,MAX_LIST_FILENAME,gui_dir_ptr->file_name);
        gui_unicode2local(gui_dir_ptr->file_name, MAX_LIST_FILENAME-2);
        gui_dir_ptr->last_index = value;
        *string = gui_dir_ptr->file_name;
        return result;
    }
    else if(LIST_CALLBACK_SELECT_ITEM == type)
    {
        gui_dir_ptr->highlight_index = value;
        if(DIRECTORY_ALL == gui_dir_ptr->file_type && (gui_dir_ptr->cur_item_type==ROOT_LIST || value > 0))
        {
            //entry folder browser option menu
            DeleteScreenIfPresent(SCR_ID_GUI_LIST);
            GUI_Display_Menu(GUI_MENU_FOLDERLIST, NULL);
            return 1;//Not exit gui_list screen
        }
        else
        {
            GUI_DirectoryListSelectItem(value);
        }
        return 0;
    }

	return 0;
}

BOOL GUI_FileBrowser_MsgHandler(COS_EVENT *pEvent)
{
    //MenuItem_t *item;
    UINT32 count;

    APP_ASSERT(pEvent);

    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            if(ERR_FS_NOT_MOUNT == FS_GetEntryCount(&gui_dir_ptr->folder_count, NULL, FS_DIR_DIR))
            {
                // no disk mount, list disks
                gui_dir_ptr->highlight_index = 0;
                gui_dir_ptr->cur_item_type = ROOT_LIST;
            }
            else
            {
                gui_dir_ptr->highlight_index = gui_dir_ptr->folder_count;
                if(DIRECTORY_ALL != gui_dir_ptr->file_type)
                    gui_dir_ptr->highlight_index += fselGetNo();
            }
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        case EV_UI_FW_REDRAW:
            if(gui_dir_ptr->cur_item_type == ROOT_LIST)
            {
#if APP_SUPPORT_USB
                gui_dir_ptr->folder_count = 1;
#else
                gui_dir_ptr->folder_count = 0;
#endif
                count = 0;
            }
            else
            {
                FS_GetEntryCount(&gui_dir_ptr->folder_count, NULL, FS_DIR_DIR);
                if(gui_dir_ptr->file_type == DIRECTORY_ALL)
                    count = 0;
                else
                    FS_GetEntryCount(&count, gui_dir_ptr->ext_name, FS_DIR_FILE);
            }

            GUI_Display_List(LIST_TYPE_CUSTOM_IMAGE, GUI_IMG_FOLDER_LIST,
                             (UINT16) (count+gui_dir_ptr->folder_count+1), (UINT16)gui_dir_ptr->highlight_index, file_list_callback, FALSE);
            break;

        case EV_UI_FW_ON_RESUME:
            if(RESULT_IGNORE == pEvent->nParam1)
            {
                GUI_ExitFileBrowser(LIST_CALLBACK_CANCEL);
            }
            else
            {
                SendAppEvent(EV_UI_FW_REDRAW, 0);
            }
            break;

        case EV_UI_FW_ON_PAUSE:
            break;

        case DIRECTORY_MENU_OPEN:
            GUI_DirectoryListSelectItem(gui_dir_ptr->highlight_index);
            SendAppEvent(EV_UI_FW_REDRAW, 0);
            break;

        //Select folder
        case DIRECTORY_MENU_SELECT:
            if(GUI_DirectoryListSelectItem(gui_dir_ptr->highlight_index))
            {
                GUI_ExitFileBrowser(LIST_CALLBACK_SELECT_ITEM);
            }
            break;

        case EV_UI_FW_ON_EXIT:
            gui_dir_ptr->cur_item_type = SELECT_AND_EXIT;

            break;

        default:
            return FALSE;
    }

    return TRUE;
}




/********************************************************************************
* Description : select the directory.
*
* Arguments  :
*            location: get thedisk type
*            type : show file kind
*            string_id: language type
* Returns     :
*            other:    Successful
*            0:    Fail, detail information in fselError
* Notes       :  only select the valid directory
*
********************************************************************************/
INT32 GUI_Directory(file_location_t *location, UINT8 type,UINT8 string_id)
{
    //return GUI_EntryFileBrowser(location, type, string_id, NULL);
}
INT32 GUI_EntryFileBrowser(file_location_t *location, UINT8 type,UINT8 string_id, FileBrowerCallBack callback)
{
    gui_dir_ptr = (GUI_DIRECTORY_PTR) COS_MALLOC(sizeof(GUI_DIRECTORY_T));
    APP_ASSERT(gui_dir_ptr);
    memset(gui_dir_ptr, 0, sizeof(GUI_DIRECTORY_T));

    gui_dir_ptr->location = *location;
    gui_dir_ptr->file_type = type;
    gui_dir_ptr->cbk = callback;

    if(type == DIRECTORY_MUSIC)
        gui_dir_ptr->ext_name = ExtMusic;
    else
        gui_dir_ptr->ext_name = NULL;


    EntryNewScreen(SCR_ID_GUI_DIRECTORY, GUI_FileBrowser_MsgHandler, (U8 *)gui_dir_ptr, 0, 0);

}


void GUI_ExitFileBrowser(UINT16 result)
{
    GUI_FileBrowserCallback(result);

    if(GetCurrentScreenId() != SCR_ID_GUI_DIRECTORY)
    {
        DeleteScreenIfPresent(SCR_ID_GUI_DIRECTORY);
    }
    else
    {
        AppGobackScreen();
    }
}
#endif


