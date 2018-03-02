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
// CoolSimulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SimDevice.h"
#include "Simulator.h"
#include "SimulatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp

BEGIN_MESSAGE_MAP(CSimulatorApp, CWinApp)
	//{{AFX_MSG_MAP(CSimulatorApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp construction
extern "C" BYTE *gpResourceBase;
extern "C" HANDLE g_hDataFile;
extern "C" HANDLE g_hPhoneDataFile;

CSimulatorApp::CSimulatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSimulatorApp object

CSimulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp initialization

BOOL CSimulatorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  // read resource file
  HANDLE hResourceFile = CreateFile("res.bin", GENERIC_READ,
                              FILE_SHARE_READ, NULL, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
  if (hResourceFile == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, "Can't open resouce file", "Error", MB_OK);
        return 0;
  }

  DWORD cbRead, length;
   length = GetFileSize(hResourceFile, &cbRead);
    gpResourceBase = new unsigned char[length];

    ReadFile(hResourceFile, gpResourceBase , length, &cbRead, NULL);
  CloseHandle(hResourceFile);

    // user data file
    g_hDataFile = CreateFile("user.data", GENERIC_READ|GENERIC_WRITE,
                              0, NULL, OPEN_ALWAYS ,
                              FILE_ATTRIBUTE_NORMAL, NULL);

    g_hPhoneDataFile = CreateFile("pbap.data", GENERIC_READ|GENERIC_WRITE,
                              0, NULL, OPEN_ALWAYS ,
                              FILE_ATTRIBUTE_NORMAL, NULL);

	CSimulatorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



int CSimulatorApp::ExitInstance() 
{
    if(gpResourceBase)	
        delete []gpResourceBase;
    CloseHandle(g_hDataFile);
    CloseHandle(g_hPhoneDataFile);
	return CWinApp::ExitInstance();
}
