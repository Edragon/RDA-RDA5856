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
// ScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "simulator.h"
#include "ScreenDlg.h"
#include "SimDevice.h"
#include "SimulatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg dialog


CScreenDlg::CScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenDlg::IDD, pParent)
{
    m_needRedraw = FALSE;
	//{{AFX_DATA_INIT(CScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenDlg)
	DDX_Control(pDX, IDC_BMP_SCREEN, m_screen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenDlg)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenDlg message handlers

void CScreenDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    RedrawScreen();
}

BOOL CScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    //RedrawScreen();

    m_needRedraw = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScreenDlg::RedrawScreen()
{
    RECT rect;
	CDC dcMem;
    BITMAP bmp; 
    CBitmap cbmp;

	GetClientRect(&rect);
    dcMem.CreateCompatibleDC(GetDC());
    GetClientRect(&rect);

    HBITMAP hbmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(),"tmp.bmp",IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
    //m_screen.SetBitmap(hbmp);
    GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bmp);
    cbmp.Attach(hbmp);
    dcMem.SelectObject(cbmp);	

    m_zoom = min((float)(rect.right-rect.left)/bmp.bmWidth, (float)(rect.bottom-rect.top)/bmp.bmHeight);
    if(m_zoom < 1)
        m_zoom =1;

	m_screen.MoveWindow((int)rect.left, (int)rect.top, (int)(bmp.bmWidth*m_zoom), (int)(bmp.bmHeight*m_zoom));
    m_screen.GetDC()->StretchBlt((int)rect.left, (int)rect.top, (int)(bmp.bmWidth*m_zoom), (int)(bmp.bmHeight*m_zoom),
        &dcMem,0,0,(int)(bmp.bmWidth), (int)(bmp.bmHeight),SRCCOPY);
}

void CScreenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    CSimulatorDlg *pMainWnd = (CSimulatorDlg*)GetParent();
    pMainWnd->SetCoordinate((int)(point.x/m_zoom), (int)(point.y/m_zoom));	
	CDialog::OnMouseMove(nFlags, point);
}

void CScreenDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_needRedraw)
        RedrawScreen();
	
}
