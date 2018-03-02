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

#include "stdafx.h"
#include "simulator.h"
#include "ATDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" void ATDlgIrqHandler(void);

/////////////////////////////////////////////////////////////////////////////
// CATDlg dialog
CATDlg::CATDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATDlg)
	m_input = _T("");
	m_text = _T("");
	//}}AFX_DATA_INIT
}


void CATDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATDlg)
	DDX_Control(pDX, IDC_LIST_LIST, m_list);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_input);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CATDlg, CDialog)
	//{{AFX_MSG_MAP(CATDlg)
	ON_MESSAGE(WM_USER_SEND_AT_CMD_REQ, OnATCmdReq)
	ON_LBN_DBLCLK(IDC_LIST_LIST, OnDblclkListList)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST_LIST, &CATDlg::OnLbnSelchangeListList)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATDlg message handlers

void CATDlg::PrintString(CString s) 
{	
	CEdit* edit= (CEdit*) GetDlgItem(IDC_EDIT_TEXT);

	
	UpdateData();
	m_text += s;

	UpdateData(FALSE);

	edit->LineScroll(edit->GetLineCount());
}


void CATDlg::SendString(CString s) 
{

	send_text = s;
	send_len = s.GetLength();

	ATDlgIrqHandler();

}


void CATDlg::OnDblclkListList() 
{
	CString str;
	m_list.GetText(m_list.GetCurSel(), str);
	
	SendString( str + "\r\n");

}

void CATDlg::OnButtonSend() 
{
	CEdit* edit= (CEdit*) GetDlgItem(IDC_EDIT_TEXT);

	UpdateData();
	SendString(m_input + "\r\n");
	UpdateData(FALSE);

	edit->LineScroll(edit->GetLineCount());
}

LRESULT CATDlg::OnATCmdReq(WPARAM wParam, LPARAM lParam) 
{
	PrintString((unsigned char*) lParam);
    return 0;
}

BOOL CATDlg::OnInitDialog() 
{
	char* pszFileName="at_command.txt"; 
    CStdioFile myFile; 
    CFileException fileException; 
	RECT rcClient;				/* console window rectangle pos			*/

	CDialog::OnInitDialog();

	if(myFile.Open(pszFileName,CFile::typeText|CFile::modeRead,&fileException))
	{
		myFile.SeekToBegin(); 

		CString str="";
		while(myFile.ReadString(str))
		{
			str.Trim();
			if(str.GetLength() > 0)
				m_list.AddString(str);
		}
	}
	else
	{
		if(myFile.Open(pszFileName,CFile::typeText|CFile::modeCreate|CFile::modeWrite,&fileException))
		{ 
			m_list.AddString("AT");
			myFile.WriteString("AT\n");
		}
		else
		{
			//MessageBox(NULL, "Can't create at command file", "Error", MB_OK);
			return 0;
		}
	}

	myFile.Close();

	m_list.AddString("AT+BTOPEN");
	m_list.AddString("AT+BTINQUIRY=10");
	m_list.AddString("AT+BTDISCHFP");
	m_list.AddString("AT+BTDISCA2DP");
	m_list.AddString("AT+BTCLOSE");
	m_list.AddString("AT+BTSWITCH=1");
	m_list.AddString("AT+BTVISIBLE");
	m_list.AddString("AT+BTINVISIBLE");
	m_list.AddString("AT+BTSETNAME");
	m_list.AddString("AT+BTCONHFP");
	m_list.AddString("AT+BTDISCHFP");
	m_list.AddString("AT+BTACCEPTCALL");
	m_list.AddString("AT+BTREJECTCALL");
	m_list.AddString("AT+BTTERMINALCALL");
	m_list.AddString("AT+BTCALL=5,13062806597");
	m_list.AddString("AT+BTSENDAT=AT+IPHONEACCEV=1,1,50");
	m_list.AddString("AT+BTCONA2DP");
	m_list.AddString("AT+BTDISCA2DP");
	m_list.AddString("AT+BTCONAVRCP");
	m_list.AddString("AT+BTDISCAVRCP");
	m_list.AddString("AT+BTVOLUP");
	m_list.AddString("AT+BTVOLDOWN");
	m_list.AddString("AT+BTATGETPB");
	m_list.AddString("AT+BTGETPB");
	m_list.AddString("AT+BTGETCR");
	m_list.AddString("AT+BTGETMAP");
	m_list.AddString("AT+BTSETMNS");
	m_list.AddString("AT+BTCONHID");
	m_list.AddString("AT+DH");
	/* gets rect pos for console window */ 
    GetClientRect(&rcClient); 
	/* sets console window pos */ 

  	//SetWindowPos(this->m_hWnd, rcClient.left+300, rcClient.top+40, 0, 0, SWP_NOSIZE); 


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CATDlg::OnLbnSelchangeListList()
{
	// TODO: Add your control notification handler code here
}


void CATDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
