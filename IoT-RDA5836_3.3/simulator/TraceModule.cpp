/* 
* Copyright (c) 2006-2016 RDA Microelectronics, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
// TraceModule.cpp: implementation of the CTraceModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TraceModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraceModule::CTraceModule()
{

}

CTraceModule::~CTraceModule()
{

}


void CTraceModule::DebugWindowAlloc(HWND hWnd)
{
	RECT rcClient;				/* console window rectangle pos			*/
	HWND conshWnd;				/* console window handle				*/

	/* allocates a new console for the calling process */
	if(!AllocConsole())
	{	
		exit(1);
	}

	/* sets console window title */
#ifdef __ASCII
	SetConsoleTitle(" Background Process ");
#endif

#ifdef __UCS2_ENCODING
	SetConsoleTitle(__TEXT(" Background Process "));
#endif

	/* specified window into the foreground and activates the window */ 
	SetForegroundWindow(hWnd);

	/* gets handle for console window */ 
	conshWnd = GetWindow(hWnd, GW_HWNDNEXT);

	/* gets rect pos for console window */ 
  GetClientRect(conshWnd, &rcClient); 

	/* sets console window pos */ 
  SetWindowPos(conshWnd, HWND_NOTOPMOST, rcClient.left+300, rcClient.top+40, 0, 0, SWP_NOSIZE); 
 
	/* by default, the console window is minimized(forcing to minimize) */
  /*ShowWindow(conshWnd, SW_FORCEMINIMIZE);*/

	ShowWindow(conshWnd, SW_SHOWNORMAL); 

	/* specified window into the foreground and activates the window */ 
	SetForegroundWindow(hWnd);

	/* sets console control handler */
  SetConsoleCtrlHandler((PHANDLER_ROUTINE) CtrlHandler, TRUE);   
}

BOOL CTraceModule::CtrlHandler(unsigned int fdwCtrlType) 
{ 
  /* receives console CTRL signal type */
  switch (fdwCtrlType) 
  { 
    // [004]    
		case CTRL_C_EVENT: 
    case CTRL_CLOSE_EVENT: 
    case CTRL_BREAK_EVENT: 
    case CTRL_LOGOFF_EVENT: 
    case CTRL_SHUTDOWN_EVENT: 
      /*ShutdownSystemOperation();*/
      FreeConsole();
      return TRUE; 
      // [004]  
    default: 
      return FALSE; 
  } 
}

void CTraceModule::DisplayToDebugwindow(char *nPrintableStr)
{
  HANDLE OutHndle = NULL;						/* handle to screen buffer			*/
  unsigned long lpNumberOfCharsWritten = 0;	/* number of characters written		*/
  
  /* retrieves a handle for the standard output */
  OutHndle = GetStdHandle(STD_OUTPUT_HANDLE);
  
  /* writes a character string to a console screen buffer beginning at the current cursor location */
  if(!WriteConsole(
    OutHndle,								/* handle to screen buffer			*/
    nPrintableStr,							/* write buffer						*/
    strlen(nPrintableStr),					/* number of characters to write	*/
    &lpNumberOfCharsWritten,				/* number of characters written		*/
    NULL									/* reserved							*/
    ))
	{
    //return 0;
	}
}