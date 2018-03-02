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

#ifdef APP_SUPPORT_PLAYLIST 
extern UINT8  PlayListFileName[];
extern UINT8  PlaylistClassTotal;
extern UINT16  PlaylistCurClass;
extern BOOL  g_ScrollFlag;
extern BOOL  ui_auto_update;
extern UINT16  PlaylistCurItem; 
#endif

/********************************************************************************
* Description : show the directory.
*
* Arguments  :
*            location: get thedisk type
*            Invpath: the "\" or name
*            Namebuf:the string you will display
*            type : show file kind
*            string_id: language type
* Returns     : the focus line number
* Notes       :  only display the valid directory
*
********************************************************************************/
UINT8 ShowPlaylist(file_location_t *location, INT8 *Longnamebuf, UINT8 string_id)
{
    
#ifdef APP_SUPPORT_PLAYLIST 
    region_t    region;
    UINT8        Invpath=0;

    ui_auto_update=FALSE;
		
    GUI_ClearScreen(NULL);

    //display icon & string
    if( Longnamebuf[0] =='\\' && Longnamebuf[1] == 0x00)            //当前目录是上级目录的显示
    {
	    //display first class name in line 1 : ROOT icon
        Invpath = 0;                //mode 0
        if(location->disk == FS_DEV_TYPE_FLASH)
        {
            GUI_ResShowPic(DIRROOT,0,0);
        }
        else
        {
            GUI_ResShowPic(DIRCARD, 0, 0);
        }

        //display string after ROOT icon
        GUI_DisplayText(14, 0, PlayListFileName);//list类型最大字符数是8个：COMPOSER,A3L不需要显示
             
	    //display next class name in line 2
        GUI_ResShowPic(DIRROOTA,0,16);                 //display normal directory icon
        if( fselGetCurGroupL(Longnamebuf,LISTNAME_DISP_LENNGTH) )
        {
         //   unitochar(Longnamebuf, LISTNAME_DISP_LENNGTH, string_id);   
            GUI_DisplayText(18,16,Longnamebuf);
        }
    }
    else
    {
		    //display first class name in line 1
        Invpath = 1;                    //mode 1
        GUI_ResShowPic(DIRROOTA,0,0);       //display normal directory icon

        if( fselGetCurGroupL(Longnamebuf,LISTNAME_DISP_LENNGTH) )
        {
       //     unitochar(Longnamebuf, LISTNAME_DISP_LENNGTH, string_id);
			GUI_DisplayText(18,0,Longnamebuf);
        }

        if ( PlaylistCurClass >= PlaylistClassTotal )
            goto Invert;    //display inverted line

	    //display next class name in line 2
        if( fselGetNextGroup(Longnamebuf) )         //如果当前目录的后续还有符合要求的目录，显示在第二行
        {
            GUI_ResShowPic(DIRROOTA,0,16);  //display normal directory icon
     //       unitochar(Longnamebuf, LISTNAME_DISP_LENNGTH, string_id);
			GUI_DisplayText(18,16,Longnamebuf);
            fselGetPrevGroup(Longnamebuf);          //pointer point the old position
         //   unitochar(Longnamebuf, LISTNAME_DISP_LENNGTH, string_id);            
        }
    }

Invert:                 //display inverted line
    if( Invpath == 0 )  //parent path
    {
        //region.x=14;    //the parent directory string begins form 14 in x coordinate
        region.y=16;
    }
    else                //sub directory
    {
        //region.x=18;    //the sub directory string begins form 18 in x coordinate
        region.y=0;    //the second row
    }

    region.x=18;    //the sub directory string begins form 18 in x coordinate
    region.width=128-region.x;
    region.height=16;
    GUI_InvertRegion(&region);            //选中的目录反白
    GUI_UpdateScreen(NULL);
    ui_auto_update=TRUE;

    {
        INT8 * pLongnamebuf = Longnamebuf;
        unsigned INT8 strcnt = 0;
        
        while ( *pLongnamebuf != 0 )
        {
            strcnt++;
            pLongnamebuf++;
        }
        if ( strcnt > (128-region.x)/8 )
        {
            return Invpath;
        }
        else
        {
            return Invpath|0x80;
        }
    }
#else
    return 0;
#endif	
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

INT32 GUI_PlayList(file_location_t *location, UINT8 type,UINT8 string_id)
{
#ifdef APP_SUPPORT_PLAYLIST 
    UINT8 listrootdispflag, i, pagebak;
    INT8 * Strname;
    UINT8 PriInv, result;        //焦点项记录
    UINT32 key;
    BOOL needscroll;
    UINT8 ScrollbeginCnt, focusline, TimeoutCnt;
    region_t   scrollregion;

    if ( !fselSetLocation(location) )
    {
        return RESULT_IGNORE;
    }

    if (PlaylistClassTotal == 0)
        return RESULT_IGNORE;

////////////////////////////////换页//////////////////////////////////////
#if 0   
    Strname = (INT8 *)STRNAMEBUF; //Strname buffer开在第6页0x7000位置
#endif
////////////////////////////////换页//////////////////////////////////////

    memset(Strname, 0x00, LISTNAME_DISP_LENNGTH+1);

    focusline = ShowPlaylist(location, Strname, string_id); //display cur class level condition
    listrootdispflag = 0;

    while(MESSAGE_Get()!=0) {;}

    MESSAGE_Initial(NULL);

    needscroll = FALSE;
    ScrollbeginCnt = TimeoutCnt = 0;
    PlaylistCurItem = 0;

    while(1)
    {
        key = MESSAGE_Get();
        switch(key)
        {
          case AP_KEY_NEXT:
          case AP_KEY_NEXT | AP_KEY_HOLD:
               if ( listrootdispflag == 1 )       // display the listroot
               {
                  Strname[0] = 0x00;
                  focusline = ShowPlaylist(location, Strname ,string_id);
                  listrootdispflag = 0;
                  PlaylistCurItem = 0;
               }
               else
               {
                  fselGetNextGroup(Strname);
                  focusline = ShowPlaylist(location, Strname ,string_id);
               }
               needscroll = FALSE;
               ScrollbeginCnt = 0;
               break;

          case AP_KEY_PREV:
          case AP_KEY_PREV | AP_KEY_HOLD:
               if ( PlaylistCurClass == 1 )       // if it is the parent do nothing
               {
                  Strname[0] = '\\';
                  Strname[1] = 0x00;
                  focusline = ShowPlaylist(location, Strname ,string_id);
                  listrootdispflag = 1;           // display the listroot
                  PlaylistCurItem = 0;
               }
               else
               {
                  fselGetPrevGroup(Strname);
                  focusline = ShowPlaylist(location, Strname ,string_id);
               }
               needscroll = FALSE;
               ScrollbeginCnt = 0;
               break;

          case AP_KEY_PLAY | AP_KEY_UP:
               fselSetLocation(location);         //restore the selecting result
               result = RESULT_IGNORE;
               goto funreturn;
               break;

          case AP_KEY_MODE | AP_KEY_LONG:
               if ( fselGetLocation(location) )   //store the selecting result
                   result = RESULT_NULL;
               else
                   result = RESULT_IGNORE;
               goto funreturn;
               break;

          case AP_KEY_MODE | AP_KEY_UP:
               break;

          case AP_MSG_WAIT_TIMEOUT:
               TimeoutCnt++;
               if ( TimeoutCnt >= 24 )  //192秒后退出，因为要看长名滚屏
               {
                  if (fselGetLocation(location))     //store the selecting result
                      result = RESULT_NULL;
                  else
                      result = RESULT_IGNORE;
                  goto funreturn;
               }
               break;

          case AP_MSG_SD_OUT:
               //如在游览SD卡，但SD卡又被拔出，就返回拔出信号
               if (location->disk == FS_DEV_TYPE_TFLASH)
               {
                   result = RESULT_SD_OUT;
                   goto funreturn;
               }
               break;

          case AP_MSG_SD_IN:
               break;

          case AP_MSG_RTC:
               if ( (ScrollbeginCnt == 1) && (needscroll == FALSE) && ((focusline&0x80) == 0) )   //长字串才滚屏
               {
                  needscroll = TRUE;
                  g_ScrollFlag = FALSE;

                  if ( focusline == 1 )
                  {
                      scrollregion.y  = 0;
                  }
                  else
                  {
                      scrollregion.y  = 16;
                  }
									
                  scrollregion.x      = 18;
                  scrollregion.width  = 128-scrollregion.x;
                  scrollregion.height = 16;
									
                  GUI_ClearScreen( &scrollregion );            //选中的目录反白
                  GUI_SetScrollRegin( &scrollregion );
                  GUI_Scroll(Strname, TRUE);         //输入参数：字符串首址，是否初始化true为要初始化
                  GUI_InvertRegion( &scrollregion );            //选中的目录反白
                  GUI_UpdateScreen( &scrollregion );
               }
               else
               {
                  ScrollbeginCnt++;
               }
               break;

          default:
               key=MESSAGE_HandleHotkey(key);
               if(key == RESULT_REDRAW)
               {
                  focusline = ShowPlaylist(location, Strname ,string_id);
               }
               else if( key != 0 )
               {
                  fselGetLocation(location);      //store the selecting result
                  result = key;
                  goto funreturn;
               }
		 } //switch()

     if ( g_ScrollFlag && needscroll )
     {
          INT8 i;
          
          g_ScrollFlag = FALSE;       //100ms设一次

          GUI_InvertRegion( &scrollregion );          //选中的目录反白
          for ( i = 0; i < 4; i++ )
          {
              GUI_Scroll(Strname, FALSE);                 //输入参数：字符串首址，是否初始化true为要初始化}
          }
          GUI_InvertRegion( &scrollregion );          //选中的目录反白
          GUI_UpdateScreen( &scrollregion );
     }
	}

funreturn:
    scrollregion.x = 0;
    scrollregion.y = 16;
    scrollregion.width = 128;
    scrollregion.height = 16;
    GUI_SetScrollRegin( &scrollregion );

////////////////////////////////换页//////////////////////////////////////
#if 0
    SetSWAPAddress(pagebak);
#endif
////////////////////////////////换页//////////////////////////////////////

    return result;
#else
    return 1;
#endif
}



