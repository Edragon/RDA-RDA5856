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
// CoolSimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Simulator.h"
#include "SimDevice.h"
#include "ScreenDlg.h"
#include "ATDlg.h"
#include "SimulatorDlg.h"
#include "Screen.h"

#include "TraceModule.h"    // Trace Module
extern "C" 
{
	//#include "mmi_data_types.h"
	//#include "MMI_features.h"
	#include "tgt_app_cfg.h"
	#include "GlobalConstants.h" /* KEY_INVALID */
    #include "cswtype.h"
    #include "ap_gui.h"
    #include "cos.h"
    #include "event.h"
    #include "fs.h"
    #include "mci.h"

//	#include "frameworkstruct.h"
//	#include "taskinit.h"
//  #include "lcd_sw_inc.h"   // Get LCD_WIDTH,LCD_HEIGHT

 	extern BOOL BAL_TaskInit();
    extern void COS_HandleTimeout(UINT8 id);
	extern void lcd_screen_init();
	extern void	UI_update(void);
    extern void FS_SetRootPath(FS_DEV_TYPE dev_type, UINT16 *path);
	extern void TM_CheckAlarm(void);
#if defined(WIFI_SUPPORT)
extern void winpcap_init(void);
extern void winpcap_deinit(void);
#endif /* WIFI_SUPPORT */

#ifdef TOUCH_PANEL_SUPPORT
	extern void mmi_pen_simulator_button_down_hdlr(S16 x, S16 y);
	extern void mmi_pen_simulator_button_move_hdlr(S16 x, S16 y);
	extern void mmi_pen_simulator_button_up_hdlr(S16 x, S16 y);
#endif

#if defined(__AT_SUPPORT__)
extern void ATDlgIrqHandler(void);
#endif /* __AT_SUPPORT__ */

	HWND	g_hSimWnd ;
    extern BYTE g_usb_connected; 
extern UINT8 g_sim_pm_Charging;
extern UINT8 g_sim_pm_Battery;
extern UINT8 g_sim_pm_Line_In;
UINT8 flag_TFlash = 0;
UINT32 last_TFlash = 0;

}

#define SIM_TIMER_ID_BASE           10

#define HIMETRIC_PER_INCH   2540

extern "C" void lcdd_update_screen(U8 *data,const region_t *region)
{
  CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
  UINT32 color = pMainDlg->m_Phone.Screen().GetColor();
  UINT32 bkcolor = pMainDlg->m_Phone.Screen().GetBKColor();
  UINT32 width = pMainDlg->m_Phone.Screen().GetWidth();
  UINT32 heigth = pMainDlg->m_Phone.Screen().GetHeight();
  BITMAPINFO bitmap_info;
  UINT32 x,y,*buff;
  bitmap_info.bmiHeader.biSize				= sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth			    = width;
	bitmap_info.bmiHeader.biHeight			= heigth;
	bitmap_info.bmiHeader.biPlanes			= 1;
	bitmap_info.bmiHeader.biBitCount		= 32;
	bitmap_info.bmiHeader.biCompression		= BI_RGB;
	bitmap_info.bmiHeader.biSizeImage		= width*heigth*4;
	bitmap_info.bmiHeader.biClrUsed			= 0;
	bitmap_info.bmiHeader.biClrImportant	= 0;

  buff = new UINT32[width*heigth];
  for(y=0;y<heigth;y++)
    for(x=0;x<width;x++)
    {
#if APP_SUPPORT_RGBLCD==1
        UINT16 color = data[(y*width+x)*2]+(data[(y*width+x)*2+1]<<8);
        buff[(heigth-1-y)*width+x] = ((color&0x7c00)<<9) | ((color&0x3e0)<<6) | ((color& 0x1f)<<3);
#else
        if(data[((y/8)*width+x)]&(1<<(y&7)))
        {
            buff[(heigth-1-y)*width+x] = color;
        }
        else
        {
            buff[(heigth-1-y)*width+x] = bkcolor;
        }
#endif
    }
  pMainDlg->m_Phone.Screen().UpdateScreen((long*)buff,(long*)&bitmap_info);

   HANDLE  hBMPFile = CreateFile("tmp.bmp",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
   if(hBMPFile != INVALID_HANDLE_VALUE)
    {
        DWORD cbWrite;
        BITMAPFILEHEADER filehead;
        filehead.bfType=0x4D42;
         filehead.bfReserved1=0;
         filehead.bfReserved2=0;
         filehead.bfOffBits=54;
         filehead.bfSize=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+width*heigth*4;

        WriteFile(hBMPFile, &filehead, sizeof(filehead), &cbWrite, NULL);
        WriteFile(hBMPFile, &bitmap_info, sizeof(bitmap_info)-4, &cbWrite, NULL);
        WriteFile(hBMPFile, buff, width*heigth*4, &cbWrite, NULL);
        CloseHandle(hBMPFile);

        if(pMainDlg->m_pScreenDlg != NULL)
            pMainDlg->m_pScreenDlg->Invalidate(FALSE);
    }

  delete buff;

}

extern void DrawLed8S(UINT32 width, UINT32 height, UINT16 flag, UINT8* Data, UINT32* buff, UINT32 color, UINT32 bkcolor);

extern "C" void led8s_update_screen(UINT16 flag, UINT8* Data)
{
#if APP_SUPPORT_LED8S==1   // 8 段数码管显示
  CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
  UINT32 color = pMainDlg->m_Phone.Screen().GetColor();
  UINT32 bkcolor = pMainDlg->m_Phone.Screen().GetBKColor();
  UINT32 width = pMainDlg->m_Phone.Screen().GetWidth();
  UINT32 heigth = pMainDlg->m_Phone.Screen().GetHeight();
  BITMAPINFO bitmap_info;
  UINT32 x,y,*buff;
  bitmap_info.bmiHeader.biSize				= sizeof(bitmap_info.bmiHeader);
  bitmap_info.bmiHeader.biWidth			    = width;
	bitmap_info.bmiHeader.biHeight			= heigth;
	bitmap_info.bmiHeader.biPlanes			= 1;
	bitmap_info.bmiHeader.biBitCount		= 32;
	bitmap_info.bmiHeader.biCompression		= BI_RGB;
	bitmap_info.bmiHeader.biSizeImage		= width*heigth*4;
	bitmap_info.bmiHeader.biClrUsed			= 0;
	bitmap_info.bmiHeader.biClrImportant	= 0;

  buff = new UINT32[width*heigth];
  DrawLed8S(width, heigth, flag, Data, buff, color, bkcolor);
  pMainDlg->m_Phone.Screen().UpdateScreen((long*)buff,(long*)&bitmap_info);


   HANDLE  hBMPFile = CreateFile("tmp.bmp",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
   if(hBMPFile != INVALID_HANDLE_VALUE)
    {
        DWORD cbWrite;
        BITMAPFILEHEADER filehead;
        filehead.bfType=0x4D42;
         filehead.bfReserved1=0;
         filehead.bfReserved2=0;
         filehead.bfOffBits=54;
         filehead.bfSize=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+width*heigth*4;

        WriteFile(hBMPFile, &filehead, sizeof(filehead), &cbWrite, NULL);
        WriteFile(hBMPFile, &bitmap_info, sizeof(bitmap_info)-4, &cbWrite, NULL);
        WriteFile(hBMPFile, buff, width*heigth*4, &cbWrite, NULL);
        CloseHandle(hBMPFile);

        if(pMainDlg->m_pScreenDlg != NULL)
            pMainDlg->m_pScreenDlg->Invalidate(FALSE);
    }

  delete buff;
#endif
}

extern "C" unsigned int CATDlg_SendData(const unsigned char* str, unsigned int length)
{
//	CString str;
    CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
    CATDlg *ATDlg = pMainDlg->m_pATdlg;
    static unsigned char Buffer[1024];

	
	

	if(ATDlg)
	{
		strncpy((char *)Buffer, (char *)str ,length);
		Buffer[length] = 0;
		Buffer[length+1] = 0;
		
		//ATDlg->PrintString(str);
		SendMessage(ATDlg->m_hWnd, WM_USER_SEND_AT_CMD_REQ, 0, (LPARAM)Buffer);
	}

	return length;
}

extern "C" unsigned int CATDlg_GetSendData(unsigned char* str, unsigned int length)
{
    CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
    CATDlg *ATDlg = pMainDlg->m_pATdlg;
	unsigned int read_len;
	char *pBuffer;
	unsigned short Buffer_len;
	
	read_len = (length < ATDlg->send_len) ? length : ATDlg->send_len;

	pBuffer = (LPSTR)(LPCTSTR)ATDlg->send_text;

	Buffer_len = strlen(pBuffer);
	
	strncpy((char *)str, pBuffer+(Buffer_len - ATDlg->send_len) ,read_len);
	ATDlg->send_len -= read_len;


	return read_len;
}

extern "C" unsigned char CATDlg_GetSendDataLength(void)
{
    CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
    CATDlg *ATDlg = pMainDlg->m_pATdlg;

	
	return ATDlg->send_len;	
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__ ;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CSimulatorDlg dialog

CSimulatorDlg::CSimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimulatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bMouseDown = FALSE;
  m_bLayerShow = FALSE;
  m_pScreenDlg = NULL;
  m_tflash_mount = FALSE;
  m_usb_mount = FALSE;
	m_pATdlg = NULL;
}

CSimulatorDlg::~CSimulatorDlg()
{
    if(m_pScreenDlg != NULL)
    {
        m_pScreenDlg->DestroyWindow();
        delete m_pScreenDlg;
    }
}

void CSimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimulatorDlg)
	DDX_Control(pDX, IDC_COOLPHONECTRL, m_Phone);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimulatorDlg, CDialog)
	//{{AFX_MSG_MAP(CSimulatorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_Exit, OnFILEExit)
	ON_COMMAND(IDM_TOOL_LOAD_SKIN, OnToolLoadSkin)
	ON_WM_TIMER()
	ON_COMMAND(IDM_FILE_MOUNT_TFLASH, OnFileMountTflash)
	ON_UPDATE_COMMAND_UI(IDM_FILE_MOUNT_TFLASH, OnUpdateFileMountTflash)
	ON_COMMAND(IDM_FILE_MOUNT_UDISK, OnFileMountUdisk)
	ON_UPDATE_COMMAND_UI(IDM_FILE_MOUNT_UDISK, OnUpdateFileMountUdisk)
	ON_COMMAND(IDM_FILE_UNMOUNT_TFLASH, OnFileUnmountTflash)
	ON_UPDATE_COMMAND_UI(IDM_FILE_UNMOUNT_TFLASH, OnUpdateFileUnmountTflash)
	ON_COMMAND(IDM_FILE_UNMOUNT_UDISK, OnFileUnmountUdisk)
	ON_UPDATE_COMMAND_UI(IDM_FILE_UNMOUNT_UDISK, OnUpdateFileUnmountUdisk)
	ON_COMMAND(IDD_ABOUTBOX, OnAboutbox)
	ON_COMMAND(ID_TOOL_SHOW_SCREEN, OnToolShowScreen)
	ON_COMMAND(ID_FILE_POWER_ON, OnFilePowerOn)
	ON_COMMAND(ID_FILE_CONNECT_PC, OnFileConnectPc)
	ON_COMMAND(ID_FILE_DISCONNECT_PC, OnFileDisconnectPc)
	ON_COMMAND(ID_TOOL_LOW_POWER, OnToolLowPower)
	ON_COMMAND(ID_TOOL_CHARGING, OnToolCharging)
	ON_COMMAND(ID_TOOL_BATTERY_FULL, OnToolBatteryFull)
	ON_COMMAND(ID_MENU_PLUG_LINE_IN, OnMenuPlugLineIn)
	ON_COMMAND(ID_MENU_REMOVE_LINE_IN, OnMenuRemoveLineIn)
	ON_COMMAND(ID_MENU_ATCMD, OnMenuAtcmd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulatorDlg message handlers

BOOL CSimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


  // Add Trace Module
  CTraceModule::DebugWindowAlloc(this->m_hWnd);

	//Enable at cmd window
	OnMenuAtcmd();

	
  // Get module file name
  TCHAR szFileName[_MAX_PATH] = {0};
  TCHAR szDir[_MAX_PATH] = {0};
  TCHAR szDriver[_MAX_PATH] = {0};
  GetModuleFileName(NULL, szFileName, _MAX_PATH);
  _tsplitpath(szFileName, szDriver, szDir, NULL, NULL);
  
  //获得模拟器Skin配置列表
  CString str = CString(_T("Skins\\SkinList.ini"));
  //  str.LoadString(IDS_STRING_PHONE_LIST);
  CString strExeDir = CString(szDriver) + szDir+str;
  
  this->m_Phone.LoadSkinProfile(strExeDir);

  m_bDoublePress = 0;
    m_bHoldKey = 0;
    m_running = FALSE;
  
  g_hSimWnd = this->m_hWnd;
  FS_PowerOn();
#if defined(WIFI_SUPPORT)
  winpcap_init();
#endif /* WIFI_SUPPORT */

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimulatorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimulatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CSimulatorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSimulatorDlg)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 1 /* ChangeSize */, OnChangeSizeCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 8 /* SIMKeyDown */, OnSIMKeyDownCoolphonectrl, VTS_BSTR VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 7 /* SIMKeyUp */, OnSIMKeyUpCoolphonectrl, VTS_BSTR VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 3 /* PenDown */, OnPenDownCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 4 /* PenUp */, OnPenUpCoolphonectrl, VTS_I4 VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 5 /* PenMove */, OnPenMoveCoolphonectrl, VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, 6 /* UpdateSkin */, OnUpdateSkinCoolphonectrl, VTS_NONE)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, -602 /* KeyDown */, OnKeyDownCoolphonectrl, VTS_PI2 VTS_I2)
	ON_EVENT(CSimulatorDlg, IDC_COOLPHONECTRL, -604 /* KeyUp */, OnKeyUpCoolphonectrl, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSimulatorDlg::OnChangeSizeCoolphonectrl(long nWidth, long nHeight) 
{
	// TODO: Add your control notification handler code here
  CRect rect1,rect2;
  this->GetWindowRect(rect1);
  this->GetClientRect(rect2);
  this->MoveWindow(100,100, rect1.Width(), rect1.Height());
	this->m_Phone.MoveWindow(0,0,nWidth,nHeight);
  this->SetWindowPos(NULL,0,0,nWidth+(rect1.Width()-rect2.Width()),nHeight+(rect1.Height()-rect2.Height()),SWP_NOMOVE);

  GetDlgItem(IDC_STATIC_COORDINATE)->MoveWindow(nWidth-56,1,55,20);
}

/*****************************************************************************
* FUNCTION
*  writeKeyMsgOnCondition
* DESCRIPTION
*  If message queue is not too crowed, write key message. Also onsider key up pairing.
*
* PARAMETERS
*	void
* RETURNS
*  wrote to queue or not
*  
* GLOBALS AFFECTED
*  void
*****************************************************************************/
BOOL writeKeyMsgOnCondition(void *Message)
{
	/*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   BOOL        toSend = TRUE;
// U8          num;
   static BOOL waitKeyUpMsg = TRUE;
   /* for debugging */
   static int  keyNumSkipped = 0;

	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#if 0
   /* check sending critera */
   if (/*(num = OslNumOfQMsgs() < (MAX_PRT_NODES - (MAX_PRT_NODES/4.0))) ||*/
      ((waitKeyUpMsg == TRUE) && ((Message->oslMsgId == WM_KEYUP) || (Message->oslMsgId == WM_LBUTTONUP))))
   {
      toSend = TRUE;
   }
   else
   {
      toSend = FALSE;
   }

   /* send the msg */
   if (toSend == TRUE)
   {
      if ((Message->oslMsgId != WM_KEYUP) && (Message->oslMsgId != WM_LBUTTONUP))
         waitKeyUpMsg = TRUE;
      else
         waitKeyUpMsg = FALSE;

      //OslMsgSendExtQueue(Message);
      return TRUE;
   }
   else
   {
      keyNumSkipped ++;
      return FALSE;
   }
#endif
   return TRUE;
}
void CSimulatorDlg::OnSIMKeyDownCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode) 
{
	// TODO: Add your control notification handler code here
//	UINT				i;
//	void				*tempKeyBrdMsg;
        COS_EVENT ev;
//	static MYQUEUE	Message;
//	KEYBRD_MESSAGE	*KeyBrdMsg;
        m_bHoldKey = 0;
        SetTimer(1, 1000, NULL);
        if(m_bDoublePress == 0 || m_nCurrentKey != nKeyCode)
        {
            m_bDoublePress = 1;
            //SetTimer(2, 1000, NULL);
        }
        else
        {
            m_bDoublePress = 2;
        }
        ev.nEventId = EV_KEY_DOWN;
        ev.nParam1 = nKeyCode;
    //    ev.nParam2 = 0;//for delete nParam2
  //      ev.nParam3 = 0;//for delete nParam3
        COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, 0);
        
        m_nCurrentKey = nKeyCode;

}

void CSimulatorDlg::OnSIMKeyUpCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode) 
{
	// TODO: Add your control notification handler code here
//	int   i;
//	void				*tempKeyBrdMsg;
        COS_EVENT ev;
//	static MYQUEUE	Message;
//	KEYBRD_MESSAGE	*KeyBrdMsg;
        KillTimer(1);
        /*if(m_bDoublePress == 2)
        {
            ev.nEventId = EV_KEY_DOUBLE_PRESS;
            ev.nParam1 = nKeyCode;
            ev.nParam2 = 0;
            ev.nParam3 = 0;
            COS_SendEvent(MOD_APP, &ev, 0, 0);
        }
        else*/
        if(m_bHoldKey == 1)
        {
            ev.nEventId = EV_KEY_LONG_PRESS;
            ev.nParam1 = nKeyCode;
    //        ev.nParam2 = 0;//for delete nParam2
   //         ev.nParam3 = 0;//for delete nParam3
            //COS_SendEvent(MOD_APP, &ev, 0, 0);
        }
        else
        {
            ev.nEventId = EV_KEY_PRESS;
            ev.nParam1 = nKeyCode;
   //         ev.nParam2 = 0;//for delete nParam2
  //          ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, 0);
        }

        ev.nEventId = EV_KEY_UP;
        ev.nParam1 = nKeyCode;
  //      ev.nParam2 = 0;//for delete nParam2
 //       ev.nParam3 = 0;//for delete nParam3
        COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, 0);

}

void CSimulatorDlg::OnPenDownCoolphonectrl(long xPos, long yPos) 
{
	// TODO: Add your control notification handler code here
#ifdef TOUCH_PANEL_SUPPORT
		mmi_pen_simulator_button_down_hdlr((S16)xPos, (S16)yPos);
#endif
		m_bMouseDown = TRUE;
}

void CSimulatorDlg::OnPenMoveCoolphonectrl(long xPos, long yPos, long nFlag) 
{
	// TODO: Add your control notification handler code here
	if (m_bMouseDown)
	{
#ifdef TOUCH_PANEL_SUPPORT
  mmi_pen_simulator_button_move_hdlr((S16)xPos, (S16)yPos);
#endif
	}
        SetCoordinate(xPos,yPos);
}
void CSimulatorDlg::OnPenUpCoolphonectrl(long xPos, long yPos) 
{
	// TODO: Add your control notification handler code here
	if(m_bMouseDown)
	{
#ifdef TOUCH_PANEL_SUPPORT
		mmi_pen_simulator_button_up_hdlr((S16)xPos, (S16)yPos);
#endif
		m_bMouseDown = FALSE;
	}
}

void CSimulatorDlg::OnUpdateSkinCoolphonectrl() 
{
	// TODO: Add your control notification handler code here
	//	UI_update();
	
}

void CSimulatorDlg::OnKeyDownCoolphonectrl(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	
}

void CSimulatorDlg::OnKeyUpCoolphonectrl(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	
}

void CSimulatorDlg::OnFILEExit() 
{
	// TODO: Add your command handler code here
	SendMessage(WM_CLOSE);
}

void CSimulatorDlg::OnToolLoadSkin() 
{
	// TODO: Add your command handler code here
  m_Phone.ShowSkinSetupDlg();
}

void CSimulatorDlg::OnTimer(UINT nIDEvent) 
{
	TM_CheckAlarm();

    if(nIDEvent>=SIM_TIMER_ID_BASE)
    {
        COS_HandleTimeout(nIDEvent - SIM_TIMER_ID_BASE);
    }
    else
    {
        COS_EVENT ev;
        if(nIDEvent == 1)
        {
            if(m_bHoldKey == 0)
            {
                m_bHoldKey = 1;
                SetTimer(1, 200, NULL);
            }
            //else if(m_bHoldKey == 1)
            {
                ev.nEventId = EV_KEY_LONG_PRESS;
                ev.nParam1 = m_nCurrentKey;
   //             ev.nParam2 = 0;//for delete nParam2
 //               ev.nParam3 = 0;//for delete nParam3
                COS_SendEvent(MOD_APP, &ev, COS_WAIT_FOREVER, 0);
            }
        }
        if(nIDEvent == 2)
        {
            m_bDoublePress = 0;
            KillTimer(2);
        }
        if(nIDEvent == 3)
		{
            ev.nEventId = 0;
            ev.nParam1 = 0;
    //        ev.nParam2 = 0;//for delete nParam2
  //          ev.nParam3 = 0;//for delete nParam3
            //COS_SendEvent(MOD_APP, &ev, 0, 0);
            COS_SendEvent(MOD_BT, &ev, 0, 0);
		}
        if(nIDEvent == 5)
		{
            ev.nEventId = AP_MSG_RTC;
            ev.nParam1 = 0;
    //        ev.nParam2 = 0;//for delete nParam2
    //        ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, 0, 0);
		}
        if(nIDEvent == 6)
        {
            if(g_sim_pm_Battery > 0)
                g_sim_pm_Battery--;
            else
                KillTimer(6);
        }
    }
	CDialog::OnTimer(nIDEvent);
}

bool						isShutDownSystem = true;

/*****************************************************************************
* FUNCTION
*	KillWindowObject
* DESCRIPTION
*	Distroy the window
*
* PARAMETERS
*  nil
* RETURNS
*	void
* GLOBALS AFFECTED
*	nil
*****************************************************************************/
extern "C"{
	void KillWindowObject(void)
	{
		/*----------------------------------------------------------------*/
		/* Local Variables                                                */
		/*----------------------------------------------------------------*/

		/*----------------------------------------------------------------*/
		/* Code Body                                                      */
		/*----------------------------------------------------------------*/
//     ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetReadQueueHandler(NULL);
//     ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetWriteQueueHandler(NULL);
//     ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetReadQueueHandle(NULL);
//     ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->m_SimulatorStart.SetWriteQueueHandle(NULL);

		if(isShutDownSystem == true)
		{
			isShutDownSystem = false;
			SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd, WM_CLOSE, 0, 0);
		}
	} /* end of KillWindowObject */
}


void CSimulatorDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CSimulatorDlg::OnFileMountTflash() 
{
	// TODO: Add your command handler code here
    BROWSEINFO   bi;   
    TCHAR   szDisplayName[MAX_PATH];   
    LPITEMIDLIST   pidl;   
    UINT16 unicode_str[MAX_PATH];

    UpdateData();

    ZeroMemory(&bi,   sizeof(bi));             
    bi.hwndOwner   =   GetSafeHwnd();   
    bi.pszDisplayName   =   szDisplayName;   
    bi.lpszTitle   =   TEXT( "Please select the path for to mount as tflash card: ");
    bi.ulFlags   =   BIF_RETURNONLYFSDIRS;     
    pidl   =   SHBrowseForFolder(&bi);   //选择文件夹 
    if   (pidl)   
    {   
        SHGetPathFromIDList(pidl,   szDisplayName);   //   set   the   directory   name.   

    	MultiByteToWideChar(CP_ACP, 0, szDisplayName, -1, (LPWSTR)unicode_str, MAX_PATH);
        FS_SetRootPath(FS_DEV_TYPE_TFLASH, unicode_str);
        m_tflash_mount = TRUE;
       if(m_running)
        {
        COS_EVENT ev;

			flag_TFlash = 1;
			last_TFlash = 1;
            ev.nEventId = EV_FS_TFLASH_DETECT;
            ev.nParam1 = 1;
   //         ev.nParam2 = 0;//for delete nParam2
   //         ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, 0, 0);
       }
        CoTaskMemFree (pidl);
    }   
}

void CSimulatorDlg::OnUpdateFileMountTflash(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_tflash_mount);
}

void CSimulatorDlg::OnFileMountUdisk() 
{
	// TODO: Add your command handler code here
	
    BROWSEINFO   bi;   
    TCHAR   szDisplayName[MAX_PATH];   
    LPITEMIDLIST   pidl;   
    UINT16 unicode_str[MAX_PATH];

    UpdateData();

    ZeroMemory(&bi,   sizeof(bi));             
    bi.hwndOwner   =   GetSafeHwnd();   
    bi.pszDisplayName   =   szDisplayName;   
    bi.lpszTitle   =   TEXT( "Please select the path for to mount as usbdisk: ");
    bi.ulFlags   =   BIF_RETURNONLYFSDIRS;     
    pidl   =   SHBrowseForFolder(&bi);   //选择文件夹 
    if   (pidl)   
    {   
        SHGetPathFromIDList(pidl,   szDisplayName);   //   set   the   directory   name.   

    	MultiByteToWideChar(CP_ACP, 0, szDisplayName, -1, (LPWSTR)unicode_str, MAX_PATH);
        FS_SetRootPath(FS_DEV_TYPE_USBDISK, unicode_str);
        m_usb_mount = TRUE;
        CoTaskMemFree (pidl);
       if(m_running)
        {
        COS_EVENT ev;

            ev.nEventId = EV_FS_UDISK_READY;
            ev.nParam1 = 0;
   //         ev.nParam2 = 0;//for delete nParam2
 //           ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, 0, 0);
       }
    }   
}

void CSimulatorDlg::OnUpdateFileMountUdisk(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_usb_mount);
}

void CSimulatorDlg::OnFileUnmountTflash() 
{
	// TODO: Add your command handler code here
	FS_SetRootPath(FS_DEV_TYPE_TFLASH, NULL);
    m_tflash_mount = FALSE;
       if(m_running)
        {
        COS_EVENT ev;

			flag_TFlash = 1;
			last_TFlash = 0;
            ev.nEventId = EV_FS_TFLASH_DETECT;
            ev.nParam1 = 0;
  //          ev.nParam2 = 0;//for delete nParam2
  //          ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, 0, 0);
       }
}

void CSimulatorDlg::OnUpdateFileUnmountTflash(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_tflash_mount);
}

void CSimulatorDlg::OnFileUnmountUdisk() 
{
	// TODO: Add your command handler code here
	FS_SetRootPath(FS_DEV_TYPE_USBDISK, NULL);
    m_usb_mount = FALSE;
       if(m_running)
        {
        COS_EVENT ev;

            ev.nEventId = EV_FS_UDISK_REMOVE;
            ev.nParam1 = 0;
  //          ev.nParam2 = 0;//for delete nParam2
   //         ev.nParam3 = 0;//for delete nParam3
            COS_SendEvent(MOD_APP, &ev, 0, 0);
       }
}

void CSimulatorDlg::OnUpdateFileUnmountUdisk(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_usb_mount);
}


void CSimulatorDlg::OnAboutbox() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CSimulatorDlg::OnToolShowScreen() 
{
    if(m_pScreenDlg == NULL)
    {
        RECT rect;
        GetWindowRect(&rect);
        m_pScreenDlg = new CScreenDlg();
        m_pScreenDlg->Create(IDD_DIALOG_SCREEN, this);
        m_pScreenDlg->MoveWindow(rect.right, rect.top, m_Phone.Screen().GetWidth()*2+8, m_Phone.Screen().GetHeight()*2+34);
    }
	m_pScreenDlg->ShowWindow(SW_SHOW);
}

void CSimulatorDlg::SetCoordinate(int x, int y)
{
  CString str;
  str.Format(_T("%ld,%ld"),x,y);
	SetDlgItemText(IDC_STATIC_COORDINATE,str);
}


extern "C" void SetSimTimer(int id, int nMillisecondes)
{
    ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->SetTimer(id+SIM_TIMER_ID_BASE, nMillisecondes, NULL);
}

extern "C" void KillSimTimer(int id)
{
    ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd))->KillTimer(id+SIM_TIMER_ID_BASE);
}

extern "C" void hal_SysRestart()
{
}

extern "C" void hal_SysSetBootMode()
{
}

void CSimulatorDlg::OnFilePowerOn() 
{
  m_running = TRUE;
  BAL_TaskInit();
  SetTimer(3,200, NULL);
  SetTimer(5,500, NULL);
}

void CSimulatorDlg::OnFileConnectPc() 
{
    COS_EVENT ev;

    g_usb_connected = TRUE;

    ev.nEventId = EV_FS_UMSS_DETECT;
    ev.nParam1 = 0;
  //  ev.nParam2 = 0;//for delete nParam2
 //   ev.nParam3 = 0;//for delete nParam3
    COS_SendEvent(MOD_APP, &ev, 0, 0);
}

void CSimulatorDlg::OnFileDisconnectPc() 
{
    COS_EVENT ev;
    
    g_usb_connected = FALSE;
    
    ev.nEventId = EV_FS_UMSS_PLUG_OUT;
    ev.nParam1 = 0;
  //  ev.nParam2 = 0;//for delete nParam2
 //   ev.nParam3 = 0;//for delete nParam3
    COS_SendEvent(MOD_APP, &ev, 0, 0);
}


extern "C" void gpio_SetLED(int index, int on)
{
    static int led_mask = 0;
    CSimulatorDlg *pMainDlg = ((CSimulatorDlg*)(AfxGetApp()->m_pMainWnd));
    if(on)
        led_mask |= (1<<index);
    else
        led_mask &= ~(1<<index);
    pMainDlg->m_Phone.ShowLeds(led_mask);
}


void CSimulatorDlg::OnToolLowPower() 
{
	SetTimer(6,200, NULL);
}

void CSimulatorDlg::OnToolCharging() 
{
    g_sim_pm_Charging = TRUE;
}

void CSimulatorDlg::OnToolBatteryFull() 
{
	g_sim_pm_Charging  = FALSE;	
	g_sim_pm_Battery = 100;
}

void CSimulatorDlg::OnMenuPlugLineIn() 
{
    COS_EVENT ev;
	g_sim_pm_Line_In = TRUE;

    ev.nEventId = EV_LINE_IN;
    ev.nParam1 = 1;
  //  ev.nParam2 = 0;//for delete nParam2
//    ev.nParam3 = 0;//for delete nParam3
    COS_SendEvent(MOD_APP, &ev, 0, 0);
}

void CSimulatorDlg::OnMenuRemoveLineIn() 
{
    COS_EVENT ev;
    g_sim_pm_Line_In = FALSE;

    ev.nEventId = EV_LINE_IN;
    ev.nParam1 = 0;
 //   ev.nParam2 = 0;//for delete nParam2
 //   ev.nParam3 = 0;//for delete nParam3
    COS_SendEvent(MOD_APP, &ev, 0, 0);
}

extern "C" BOOL gpio_detect_linein()
{
	return g_sim_pm_Line_In;
}

extern "C" MCI_ERR_T MCI_DecodePicture(UINT8 *data, UINT16 length, char* outbuf,long outbuf_len,
								 short width,short height,
								 short start_x,short start_y,
								 short end_x,short end_y,
								 DEC_UNIT* decUnit, short kind)
{
    UINT nPicWidth,nPicHeight;
    HGLOBAL hImageMemory=GlobalAlloc(GMEM_MOVEABLE, length); //给图片分配全局内存
    void *pImageMemory=GlobalLock(hImageMemory); //锁定内存
	CDC *m_pDC1 = AfxGetApp()->m_pMainWnd->GetDC();
    CDC                    m_MemDC1;       //频道1内存DC
    HBITMAP             m_hBitmap;        //位图句柄
    CBitmap             m_Bmp1;
    int i,j;
    WORD *result;
    //CImage                m_Image;            //频道1 CImage对象，用户在内存中进行图片压缩
    
    IStream *pIStream;//创建一个IStream接口指针，用来保存图片流
    IPicture *pIPicture;//创建一个IPicture接口指针，表示图片对象
    memcpy(pImageMemory, data, length); //copy图片到全局内存当中
    GlobalUnlock(hImageMemory); //解锁内存
    CreateStreamOnHGlobal(hImageMemory, false, &pIStream); //用全局内存初使化IStream接口指针
    OleLoadPicture(pIStream, 0, false, IID_IPicture, (LPVOID*)&(pIPicture));//用OleLoadPicture获得IPicture接口指针
    if(pIPicture == NULL)
        return MCI_ERR_INVALID_FORMAT;
    //得到IPicture COM接口对象后，你就可以进行获得图片信息、显示图片等操作
    OLE_XSIZE_HIMETRIC hmWidth;
    OLE_YSIZE_HIMETRIC hmHeight;
    pIPicture->get_Width(&hmWidth); //用接口方法获得图片的宽和高
    pIPicture->get_Height(&hmHeight);
     // 转化单位为象素
    nPicWidth = MulDiv(hmWidth, GetDeviceCaps(m_pDC1->m_hDC, LOGPIXELSX), HIMETRIC_PER_INCH);
    nPicHeight = MulDiv(hmHeight, GetDeviceCaps(m_pDC1->m_hDC, LOGPIXELSY), HIMETRIC_PER_INCH);

	m_MemDC1.CreateCompatibleDC(NULL);		//创建内存DC

	m_Bmp1.CreateCompatibleBitmap(m_pDC1,nPicWidth,nPicHeight);	//创建与频道1物理DC关联的位图
	HBITMAP hBitmap=HBITMAP(m_Bmp1);//获得位图的句柄
	HGDIOBJ   hOldBMP   =   ::SelectObject(m_MemDC1,hBitmap); //把该位图选入内存DC

    pIPicture->Render(m_MemDC1,0,0,nPicWidth,nPicHeight,0,hmHeight,hmWidth,-hmHeight,NULL); //在指定的DC上绘出图片
    
	//m_pDC1->BitBlt(0,0,320,240,&m_MemDC1,0,0,SRCCOPY);	//把内存DC中的图片拷贝到物理DC中显示出来

	hBitmap=(HBITMAP)::SelectObject(m_MemDC1,hOldBMP);  //获得内存中上述位图的句柄

        WORD *outdatabuf;
        BITMAPINFO outinfobuf[2];


         outinfobuf->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); //信息头大小（不含调色板）
         outinfobuf->bmiHeader.biWidth = nPicWidth; 
         outinfobuf->bmiHeader.biHeight = nPicHeight; 
         outinfobuf->bmiHeader.biPlanes = 1; 
         outinfobuf->bmiHeader.biBitCount = 16;
          outinfobuf->bmiHeader.biClrUsed = 0;
          outinfobuf->bmiHeader.biCompression = BI_RGB;
          outinfobuf->bmiHeader.biSizeImage = nPicWidth*(nPicHeight)*2;
          outinfobuf->bmiHeader.biClrImportant = 0; 

          outdatabuf = new WORD[nPicWidth*(nPicHeight)*2];

        int res = GetDIBits(    //根据DC和BITMAP得到位图数据
        m_MemDC1, 
        hBitmap, 
        0, 
        (WORD) nPicHeight, 
        outdatabuf,     // outdatabuf中得到位图数据
        outinfobuf,      
        DIB_RGB_COLORS);

    result = (WORD*)outbuf;
    for(i=0;i<height;i++)
        for(j=0;j<width;j++)
            result[i*width+j] = outdatabuf[(nPicHeight-(i+start_y)-1)*((nPicWidth+1)&~1)+j];
    //memcpy(outbuf, outdatabuf, nPicWidth*nPicHeight*2);

            delete[] outdatabuf;
	m_MemDC1.DeleteDC();//删除内存DC
	m_pDC1->DeleteDC();//删除物理DC


    //pIPicture->SelectPicture(AfxGetMainWnd()->GetDC()->m_hDC);

    GlobalFree(hImageMemory); //释放全局内存
    pIStream->Release(); //释放pIStream
    pIPicture->Release(); //释放pIPicture

    decUnit->height = height;
    decUnit->width = width;
    decUnit->len = height*width*2;

    return MCI_ERR_NO;
}


void CSimulatorDlg::OnMenuAtcmd() 
{
	if(m_pATdlg == NULL)
	{
		m_pATdlg = new CATDlg();
		m_pATdlg->Create(IDD_DIALOG_ATWND);
	}

	m_pATdlg->ShowWindow(SW_SHOW);
}
