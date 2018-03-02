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
#if !defined(AFX_SCREENDLG_H__04374C66_0C7E_4820_8A59_2598D4776A21__INCLUDED_)
#define AFX_SCREENDLG_H__04374C66_0C7E_4820_8A59_2598D4776A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog

class CScreenDlg : public CDialog
{
// Construction
public:
	void RedrawScreen();
	CScreenDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL m_needRedraw;
    float m_zoom;
// Dialog Data
	//{{AFX_DATA(CScreenDlg)
	enum { IDD = IDD_DIALOG_SCREEN };
	CStatic	m_screen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenDlg)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENDLG_H__04374C66_0C7E_4820_8A59_2598D4776A21__INCLUDED_)
