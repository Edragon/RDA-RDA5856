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
#if !defined(AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_)
#define AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimulatorDlg dialog
//#include "SIMULATORLAYER\FIRMWARE\INCLUDE\cswtype.h"	// Added by ClassView
class CScreenDlg;
class CATDlg;

#define MAX_LEDS        3



class CSimulatorDlg : public CDialog
{
// Construction
public:
	CSimulatorDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CSimulatorDlg();

    CScreenDlg *m_pScreenDlg;
	CATDlg *m_pATdlg;
    BOOL m_tflash_mount;
    BOOL m_usb_mount;
    BOOL m_running;
    struct {
        BYTE count;
        BYTE mode;
    } m_leds[MAX_LEDS];
// Dialog Data
	//{{AFX_DATA(CSimulatorDlg)
	enum { IDD = IDD_COOLSIMULATOR_DIALOG };
	CCoolPhone	m_Phone;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimulatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeSizeCoolphonectrl(long nWidth, long nHeight);
	afx_msg void OnSIMKeyDownCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode);
	afx_msg void OnSIMKeyUpCoolphonectrl(LPCTSTR lpszKeyName, long nKeyCode);
	afx_msg void OnPenDownCoolphonectrl(long xPos, long yPos);
	afx_msg void OnPenUpCoolphonectrl(long xPos, long yPos);
	afx_msg void OnPenMoveCoolphonectrl(long xPos, long yPos, long nFlag);
	afx_msg void OnUpdateSkinCoolphonectrl();
	afx_msg void OnKeyDownCoolphonectrl(short FAR* KeyCode, short Shift);
	afx_msg void OnKeyUpCoolphonectrl(short FAR* KeyCode, short Shift);
	afx_msg void OnFILEExit();
	afx_msg void OnToolLoadSkin();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnFileMountTflash();
	afx_msg void OnUpdateFileMountTflash(CCmdUI* pCmdUI);
	afx_msg void OnFileMountUdisk();
	afx_msg void OnUpdateFileMountUdisk(CCmdUI* pCmdUI);
	afx_msg void OnFileUnmountTflash();
	afx_msg void OnUpdateFileUnmountTflash(CCmdUI* pCmdUI);
	afx_msg void OnFileUnmountUdisk();
	afx_msg void OnUpdateFileUnmountUdisk(CCmdUI* pCmdUI);
	afx_msg void OnAboutbox();
	afx_msg void OnToolShowScreen();
	afx_msg void OnFilePowerOn();
	afx_msg void OnFileConnectPc();
	afx_msg void OnFileDisconnectPc();
	afx_msg void OnToolLowPower();
	afx_msg void OnToolCharging();
	afx_msg void OnToolBatteryFull();
	afx_msg void OnMenuPlugLineIn();
	afx_msg void OnMenuRemoveLineIn();
	afx_msg void OnMenuAtcmd();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void SetCoordinate(int x, int y);

private:
	BOOL m_bLayerShow;
	BOOL m_bMouseDown;
        long m_nCurrentKey;
        BOOL m_bHoldKey;
        BOOL m_bDoublePress;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLSIMULATORDLG_H__4E18A583_06BD_4401_99DE_B2856FB8E703__INCLUDED_)
