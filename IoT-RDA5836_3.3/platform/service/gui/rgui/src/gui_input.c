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

#define GUI_INPUT_TIME                (15)

extern BOOL        ui_auto_update;	//是否自动刷新屏幕
extern BOOL        ui_auto_select;	//是否自动确认


//static UINT32 last_key     = 0; /*record last press key*/
//static UINT32 key_counter  = 0; /*counter for press key*/
//static INT32  input_length = 0;
//static UINT8  time_out     = 1; /*0: not time out,  1: time out*/
//static UINT8  key_timer    = 0; /*counter timer*/

//static void _InputStopTimer(void)
//{
//   if (key_timer != 0)
//      mmi_cancel_timer(key_timer);
//   key_timer = 0;
//}
//static void _InputTimeout(void *param)
//{
//   time_out = 1;
//   //input_length++;
//   if(key_counter > 0)
//   {
//      key_counter = 0;
//   }
//}
//static void _InputStartTimer(void)
//{
//   time_out = 0;
//   key_timer = mmi_timer_create(GUI_INPUT_TIME, _InputTimeout, NULL, COS_TIMER_MODE_SINGLE);
//}


INT32 GUI_InputText(INT8* string, INT32 stringlen, INT8 type, BOOL ispwd, INT32 maxlen, input_callback callback)
{

#if (0)

   UINT32 key;
   UINT32 key_value    = 0;
   UINT8  input_maxlen = 0;
   BOOL   need_draw    = TRUE;	 
   INT8   input_number[GUI_INPUT_MAXLNE+2] = {0};

   MESSAGE_Initial(g_comval);	

   memset(input_number,0,sizeof(input_number));
   if (string != NULL && stringlen > 0)
   {
      input_length = stringlen>GUI_INPUT_MAXLNE?(GUI_INPUT_MAXLNE-1):(stringlen-1);
      memcpy(input_number, string, input_length+1);
   }

   input_maxlen = maxlen>0?maxlen:GUI_INPUT_MAXLNE;

   last_key = 0;
   while(1)
   {   
      if(input_length>GUI_INPUT_MAXLNE)
      {
         input_length=GUI_INPUT_MAXLNE;
         input_number[input_length]=0;
      }
      if (need_draw)
      { 
         ui_auto_update = FALSE;

#if APP_SUPPORT_LCD==1
         if(AP_Support_LCD())
         {
            GUI_ClearScreen(NULL);
            //GUI_DisplayBattaryLever();
            GUI_DisplayText(0, 16, input_number);
            if(input_length>16)
               GUI_DisplayText(0, 32, input_number+16);
            if(input_length>32)
               GUI_DisplayText(0, 48, input_number+32);
            GUI_UpdateScreen(NULL);	 
         }
#elif APP_SUPPORT_LED8S==1
         {
            if(input_length>4)
               GUI_DisplayText(0,0,input_number+input_length-4);
            else
               GUI_DisplayText(0,0,input_number);
            GUI_UpdateScreen(NULL);
         }
#endif
         ui_auto_update = TRUE;
         need_draw = FALSE;
      }

      key = MESSAGE_Wait();

      if((key&0xffff) == AP_KEY_PRESS || (key&0xffff) == AP_KEY_HOLD)
      {
         key_value = key&0xFFFF0000;
         switch(key_value)
         {
            case AP_KEY_NUM0:
            case AP_KEY_NUM1:
            case AP_KEY_NUM2:
            case AP_KEY_NUM3:
            case AP_KEY_NUM4:
            case AP_KEY_NUM5:
            case AP_KEY_NUM6:
            case AP_KEY_NUM7:
            case AP_KEY_NUM8:
            case AP_KEY_NUM9:
            case AP_KEY_STAR:
            case AP_KEY_HASH:
            {
               UINT8 i = 0;
               hal_HstSendEvent(SYS_EVENT,0x00001234);
               _InputStopTimer();
               for(i = 0; i < KEYMAP_COUNT; i++)
               {
                  if(_gKeyMapTable[i].key == key_value)
                  {
                     break;
                  }
               }
               if (i >= KEYMAP_COUNT)
               {
                  break;
               }
            
               if ((last_key != 0 && last_key != key_value) || time_out == 1)
               {
                  hal_HstSendEvent(SYS_EVENT,0x00001235);
                  input_length++;
                  hal_HstSendEvent(SYS_EVENT,input_length);
                  key_counter = 0;
               }
 
               hal_HstSendEvent(SYS_EVENT,input_length);
               if (input_length >= (input_maxlen - 1))
               {
                  input_length = input_maxlen - 1;
               }
               input_number[input_length] = _gKeyMapTable[i].value[key_counter%_gKeyMapTable[i].len];
               
               last_key = key_value;
               if ( type == GUI_INPUT_TYPE_TEXT)
                  key_counter++;
               _InputStartTimer();
               need_draw = TRUE;
               hal_HstSendEvent(SYS_EVENT,0x00001236);
             }
                break;
            case AP_KEY_MODE:
               _InputStopTimer();
               return 0;
            case AP_KEY_PLAY:
               _InputStopTimer();
               callback(input_number);
               return 0;
            case AP_KEY_PREV: // remove last number
               _InputStopTimer();
               hal_HstSendEvent(SYS_EVENT,0x00001237);
               if(input_length>=0)
               {
                  input_number[input_length--] = 0;
                  if (time_out == 0)
                  {
                     time_out = 1;
                  }
                  need_draw = TRUE;
               }
               last_key = key_value;
               hal_HstSendEvent(SYS_EVENT,0x00001238);
               hal_HstSendEvent(SYS_EVENT,input_length);
               break;
            default:
               break;
         }
      }
      else
      {
         key=MESSAGE_HandleHotkey(key);

         if(key == RESULT_REDRAW)
            need_draw=TRUE;
         else if(key!=0)
            return key;
      }
   }
#endif /* 0 */

   return 0;
}

