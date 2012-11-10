#pragma once




class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
    DECLARE_MESSAGE_MAP()
	
public:
    CMainFrame();
    virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnClose();
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


