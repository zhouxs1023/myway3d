#pragma once
#include "xObjectView.h"
#include "OutputWnd.h"
#include "xPropertyGrid.h"
#include "xExplorer.h"
#include "IDockPane.h"

class CMainFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MESSAGE_MAP()

protected:
	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

	xObjectView * GetObjectView() { return &mObjectView; }
	xPropertyGrid * GetPropertyGrid() { return &mProperty; }
	xExplorer * GetExplorer() { return &mExplorer; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

protected:
	xApp mApp;

	CMFCMenuBar m_wndMenuBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	Array<IDockPane*> mPanels;

	xObjectView mObjectView;
	xPropertyGrid mProperty;
	xExplorer mExplorer;
};


