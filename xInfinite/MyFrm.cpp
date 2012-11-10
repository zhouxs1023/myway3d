#include "stdafx.h"
#include "MyFrm.h"
#include "Controller.h"


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_ACTIVATE()

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

    cs.style &= ~(LONG) FWS_ADDTOTITLE;

	return TRUE;
}


void CMainFrame::OnClose()
{
    if (MessageBox("是否退出?", "退出", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
    {
        CFrameWnd::OnClose();

        xInfi::Controller::Instance()->Shutdown();
    }
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    switch (nState)
    {
    case WA_INACTIVE:
        break;

    case WA_ACTIVE:
    case WA_CLICKACTIVE:
        break;
    }

    CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}
