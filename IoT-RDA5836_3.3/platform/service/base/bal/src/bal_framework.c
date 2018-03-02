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


#include "ap_common.h"
#include "platform.h"
#include "event.h"
#include "ap_message.h"
#include "ap_gui.h"
#include "ap_media.h"
#ifndef WIN32
#include "hal_overlay.h"
#endif

#include "hal_debug.h"

/* array of history data
   The TOP history is current runing screen
*/
static historyNode historyData[MAX_HISTORY];
static S8 currHistoryIndex = -1;

#define IS_TOP_HISTORY()    (MIN_HISTORY == currHistoryIndex)
#define IS_VAIL_HISTORY_INDEX(i)  ((i) >= MIN_HISTORY  && (i) < MAX_HISTORY)
#define CALL_MSG_HANDLER(ev_id, ev_para) \
    do{\
        if(AppCurMsgHandler)\
        {\
            COS_EVENT ev={ev_id, ev_para};\
            AppCurMsgHandler(&ev);\
        }\
    }while(0)


void InitHistory(void)
{
    memset(historyData , 0X0, sizeof(historyData));
}

static void PushHistory(const historyNode *pNode)
{
    APP_ASSERT((currHistoryIndex + 1) < (MAX_HISTORY - 1));
    APP_ASSERT(NULL != pNode);

    memcpy(&historyData[(UINT8)(++currHistoryIndex)], pNode, sizeof(historyNode));
}

static void PopHistory(historyNode *pNode)
{
    APP_ASSERT(currHistoryIndex >= MIN_HISTORY);
    APP_ASSERT(NULL != pNode);

    memcpy(pNode, &historyData[(UINT8)(currHistoryIndex)], sizeof(historyNode));
    memset(&historyData[(UINT8)(currHistoryIndex)], 0x0, sizeof(historyNode));

    currHistoryIndex--;
}

static void DeleteHistoryNode(S8 index)
{
    S8 i;

    APP_ASSERT(IS_VAIL_HISTORY_INDEX(index));

    //History empty
    if(currHistoryIndex < MIN_HISTORY)
        return;

    APP_ASSERT(index <= currHistoryIndex);

    for(i = index; i <= currHistoryIndex; i++)
    {
        if(i < MAX_HISTORY - 1)
        {
            historyData[(UINT8)i] = historyData[(UINT8)(i + 1)];
        }
    }

    currHistoryIndex--;
}

static void FreeGuiBuffer(S8 index)
{
    if(IS_VAIL_HISTORY_INDEX(index))
    {
        if(historyData[(UINT8)index].gui_buffer)
        {
            COS_Free(historyData[(UINT8)index].gui_buffer);
            historyData[(UINT8)index].gui_buffer = NULL;
        }
    }
}

BOOL ClearGuiBuffer(U16 ScrID)
{
    S8 i = currHistoryIndex;

	if(!IS_VAIL_HISTORY_INDEX(i))
	{
		return FALSE;
	}

    for( ; i >= MIN_HISTORY; i--)
    {
        if(historyData[(UINT8)i].scrnID == ScrID)
        {
            //Free cur gui buffer
            FreeGuiBuffer(i);
        }
    }

    return TRUE;
}

BOOL IsScreenPresent(U16 ScrID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 count;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    for (count = 0; ((count < MAX_HISTORY)&&((count <= currHistoryIndex) && (currHistoryIndex != -1))); count++)
    {
        if(historyData[count].scrnID == ScrID )
        {
            return TRUE;
        }
    }
    return FALSE;
}

U16 GetCurrentScreenId(void)
{
    S8 i = currHistoryIndex;

    if(IS_VAIL_HISTORY_INDEX(i))
        return historyData[(UINT8)i].scrnID;
    else
        return SCR_ID_START;
}


U8 * GetHistoryGuiBuffer(U16 ScrID)
{
    S8 count;

    count = currHistoryIndex;

    if( historyData[(UINT8)currHistoryIndex].scrnID  == ScrID )
    {
        return (historyData[(UINT8)count].gui_buffer);
    }
    else
    {
        return NULL;
    }
}

App_Msg_Handler GetCurrentScreenMsgHandler(void)
{
    S8 i = currHistoryIndex;

    if(IS_VAIL_HISTORY_INDEX(i))
        return historyData[(UINT8)i].handler;
    else
        return NULL;
}
/******************************************************************************
 * EntryNewScreen:
 * DESCRIPTION: -
 *
 * Input:
 * gui_buffer will be free after screen ON_EXIT msg handled, DO NOT free before.
 * start_param - for start new screen
 * pause_param - for pause current screen
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
U8 EntryNewScreen(U16 newScreenID,
                  App_Msg_Handler newMsgHandler,
                  U8 *gui_buffer,
                  UINT32 start_param,
                  UINT32 pause_param)
{
    historyNode Node;


    //call pause handler
    CALL_MSG_HANDLER(EV_UI_FW_ON_PAUSE, pause_param);

    // TODO: Drop messags of previous screen

    //push history
    Node.scrnID = newScreenID;
    Node.handler = newMsgHandler;
    Node.gui_buffer = gui_buffer;
    PushHistory(&Node);


    //Call On_start_event handler
    CALL_MSG_HANDLER(EV_UI_FW_ON_START, start_param);

    return TRUE;
}


/******************************************************************************
 * GobackScreen:
 * DESCRIPTION: -
 *
 * Input:
 * exit_param - for current screen
 * resume_param - for screen be resumed
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void GobackScreen(UINT32 exit_param, UINT32 resume_param)
{
    historyNode Node;

    if(currHistoryIndex < MIN_HISTORY)
    {
        //There is no history
        return;
    }

    if(!IS_TOP_HISTORY())
    {
        //call exit handler
        CALL_MSG_HANDLER(EV_UI_FW_ON_EXIT, exit_param);

        //Free cur gui buffer
        FreeGuiBuffer(currHistoryIndex);

        // TODO: Drop messags of previous screen

        //resume parent msg handler
        PopHistory(&Node);
    }

    //Call resume msg handler
    CALL_MSG_HANDLER(EV_UI_FW_ON_RESUME, resume_param);

}



/******************************************************************************
 * GotoRootHistoryScreen:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void GotoRootHistoryScreen(void)
{
    historyNode Node;

    if(currHistoryIndex < MIN_HISTORY)
    {
        //There is no history
        return;
    }

    //while history isn't empty
    while(!IS_TOP_HISTORY())
    {
        //call exit handler
        CALL_MSG_HANDLER(EV_UI_FW_ON_EXIT, SCREEN_EXIT_BACKGROUND);

        //Free cur gui buffer
        FreeGuiBuffer(currHistoryIndex);

        //history pop
        PopHistory(&Node);
    }

}

/******************************************************************************
 * GobackAllHistory:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
void GobackAllHistory(void)
{
    historyNode Node;

    //while history isn't empty
    while(IS_VAIL_HISTORY_INDEX(currHistoryIndex))
    {
        //call exit handler
        CALL_MSG_HANDLER(EV_UI_FW_ON_EXIT, SCREEN_EXIT_BACKGROUND);

        //Free cur gui buffer
        FreeGuiBuffer(currHistoryIndex);

        //history pop
        PopHistory(&Node);
    }

}
/******************************************************************************
 * DeleteScreenIfPresent:
 * DESCRIPTION: -
 * Delete all histories which scrnID is  'ScrID'
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
U8 DeleteScreenIfPresent(U16 ScrID)
{
    S8 i = currHistoryIndex;
    U8 count = 0;
    COS_EVENT ev;

	if(!IS_VAIL_HISTORY_INDEX(i))
	{
		return 0;
	}

    for( ; i >= MIN_HISTORY; i--)
    {
        if(historyData[(UINT8)i].scrnID == ScrID)
        {
            //call exit handler
            if(historyData[(UINT8)i].handler)
            {
                ev.nEventId = EV_UI_FW_ON_EXIT;
                ev.nParam1 = SCREEN_EXIT_BACKGROUND;
                historyData[(UINT8)i].handler(&ev);
            }

            //Free cur gui buffer
            FreeGuiBuffer(i);

            DeleteHistoryNode(i);
            count++;
        }
    }

    return count;
}

/******************************************************************************
 * AddHistoryScreen:
 * DESCRIPTION: -
 *
 * Input:
 * gui_buffer will be free after screen ON_EXIT msg handled, DO NOT free before.
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
U8 AddHistoryScreen(U16 newScreenID,
                  App_Msg_Handler newMsgHandler,
                  U8 *gui_buffer)
{
    historyNode Node;

    // TODO: Drop messags of previous screen

    //push history
    Node.scrnID = newScreenID;
    Node.handler = newMsgHandler;
    Node.gui_buffer = gui_buffer;
    PushHistory(&Node);

    return TRUE;
}


typedef struct
{
    INT16 title;
    INT16 StringID;
    INT8 *string;
    UINT8 audio_id;
    BOOL audio_play_finished;
    UINT32 msg_timer_count;
    void(*callbackFunc)(void);
} GUI_DISP_MSG_T, *GUI_DISP_MSG_PTR;

/******************************************************************************
 * GUI_DisplayMessageWithCallback_MsgHandler:
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
BOOL BAL_DisplayMessageWithCallback_MsgHandler(COS_EVENT *pEvent)
{
    GUI_DISP_MSG_PTR gui_disp_msg_ptr = (GUI_DISP_MSG_PTR) GetHistoryGuiBuffer(SCR_ID_GUI_MESSAGE);
    APP_ASSERT(pEvent);

    if(NULL == gui_disp_msg_ptr) return FALSE;
    
    switch(pEvent->nEventId)
    {
        case EV_UI_FW_ON_START:
            //Play tone
            if(gui_disp_msg_ptr->audio_id != 0)
            {
                media_PlayInternalAudio(gui_disp_msg_ptr->audio_id, 1, 0);
            }
        case EV_UI_FW_REDRAW:
            break;

        case EV_UI_FW_ON_RESUME:
            //Should NOT receive resume message!!
            app_trace(APP_GUI_TRC, "BAL_DisplayMessageWithCallback recv resume msg!");
        case EV_UI_FW_ON_PAUSE://Not push history
            DeleteScreenIfPresent(SCR_ID_GUI_MESSAGE);
            break;

        case EV_UI_FW_ON_EXIT:
            //Stop play tone
            media_StopInternalAudio();
            if(gui_disp_msg_ptr->callbackFunc)
                mmi_timer_create(1, (void(*)(void *))gui_disp_msg_ptr->callbackFunc,NULL,COS_TIMER_MODE_SINGLE);
            gui_disp_msg_ptr->callbackFunc = NULL;
            ClearGuiBuffer(SCR_ID_GUI_MESSAGE);
        	break;
        
        case EV_AUDIO_INTERNAL_PLAY_TERMINATED_IND:
            if(gui_disp_msg_ptr->audio_id != pEvent->nParam1)
                return FALSE;
        case EV_AUDIO_INTERNAL_PLAY_FINISH_IND:
            gui_disp_msg_ptr->audio_play_finished = TRUE;
            GobackScreen(0, 0);
            break;

        //scase EV_KEY_PRESS:
        case EV_KEY_DOWN:
            GobackScreen(0, 0);
            break;

        default:
            //key event
            switch(MESSAGE_ConvertToKeyValue(pEvent))
            {
                case AP_MSG_RTC:
#if APP_SUPPORT_LCD==1
                    if(gui_disp_msg_ptr->msg_timer_count++< 4)
                    {
                        break;
                    }
#endif
                case AP_MSG_WAIT_TIMEOUT:
                    if(gui_disp_msg_ptr->audio_play_finished)
                    {
                        GobackScreen(0, 0);
                    }
                case AP_MSG_STANDBY:
                    //do nothing
                    break;

                default:
                    //Common events, DONOT deal it.
                    return FALSE;
            }
            break;
    }

    return TRUE;
}


/******************************************************************************
 * BAL_DisplayMessageWithCallback:
 * DESCRIPTION: - NOTE: This screen will NEVER push in history!
 *
 * Input:
 * Output:
 * Returns:
 *
 *
 ******************************************************************************/
UINT32 BAL_DisplayMessageWithCallback(
    INT16 title,
    INT16 message,
    UINT8 *string,
    UINT8 audio_id,
    void(*callbackFunc)(void),
    UINT8 flag)
{
    //UINT32 key;
    //INT8 *str;
    //INT32 time=0;
    GUI_DISP_MSG_PTR gui_disp_msg_ptr;

    app_trace(APP_GUI_TRC, "BAL_DisplayMessageWithCallback,msg=%d,aud=%d,cbk=0x%x,f=%d",
    						message, audio_id, callbackFunc, flag);


	
    if((flag == GUI_MSG_FLAG_DISPLAY)
#if APP_SUPPORT_LCD == 0
    ||(0 == audio_id)
#endif
    )
    {
        if(callbackFunc)
            callbackFunc();
        return 0;
    }

    //Not popup message screen when there is no screen present.
    if(GetCurrentScreenId() == SCR_ID_START)
    {
        if(callbackFunc)
            callbackFunc();
        return 0;
    }

    //Make sure there is no history
    DeleteScreenIfPresent(SCR_ID_GUI_MESSAGE);

    gui_disp_msg_ptr = (GUI_DISP_MSG_PTR) COS_MALLOC(sizeof(GUI_DISP_MSG_T));
    memset(gui_disp_msg_ptr, 0x0, sizeof(GUI_DISP_MSG_T));
    gui_disp_msg_ptr->title = title;
    gui_disp_msg_ptr->StringID= message;
    gui_disp_msg_ptr->string= string;
    gui_disp_msg_ptr->audio_id = audio_id;
    gui_disp_msg_ptr->callbackFunc = callbackFunc;
    gui_disp_msg_ptr->audio_play_finished = (0 == audio_id) ? TRUE:FALSE;

    EntryNewScreen(SCR_ID_GUI_MESSAGE, BAL_DisplayMessageWithCallback_MsgHandler, (U8*)gui_disp_msg_ptr, 0, 0);

    return 0;
}


