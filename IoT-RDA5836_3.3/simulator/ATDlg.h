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

#if !defined(AFX_ATDLG_H__F067A4F4_AFEF_43E3_ADB0_DEC17A80A9C1__INCLUDED_)
#define AFX_ATDLG_H__F067A4F4_AFEF_43E3_ADB0_DEC17A80A9C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ATDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CATDlg dialog
#define WM_USER_SEND_AT_CMD_REQ (WM_APP+1)

class CATDlg : public CDialog
{
// Construction
public:
	CATDlg(CWnd* pParent = NULL);   // standard constructor
	
	LRESULT OnATCmdReq(WPARAM wParam, LPARAM lParam);
	
	CString	send_text;
	unsigned short send_len;
	
// Dialog Data
	//{{AFX_DATA(CATDlg)
	enum { IDD = IDD_DIALOG_ATWND };
	CListBox	m_list;
	CString	m_input;
	CString	m_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void PrintString(CString s);
	void SendString(CString s);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CATDlg)
	afx_msg void OnDblclkListList();
	afx_msg void OnButtonSend();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListList();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATDLG_H__F067A4F4_AFEF_43E3_ADB0_DEC17A80A9C1__INCLUDED_)
