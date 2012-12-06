#include "stdafx.h"
#include "InfinateDoc.h"
#include "InfinateView.h"
#include "xApp.h"
#include "xScene.h"


IMPLEMENT_DYNCREATE(CInfinateView, CView)

BEGIN_MESSAGE_MAP(CInfinateView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DROPFILES()

END_MESSAGE_MAP()


IMP_SLN(CInfinateView);

CInfinateView::CInfinateView()
{
	INIT_SLN;
}

CInfinateView::~CInfinateView()
{
	SHUT_SLN;
}

BOOL CInfinateView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

INT CInfinateView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CView::OnCreate(lpcs) == -1)
		return -1;

	DragAcceptFiles();

	return 0;
}

void CInfinateView::OnDraw(CDC* /*pDC*/)
{
	CInfinateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	static bool bInited = false;
	if (!bInited)
	{
		xApp::Instance()->Init(m_hWnd);
		bInited = true;
	}
}

void CInfinateView::OnDropFiles(HDROP hDropInfo)
{
	POINT pt;
	RECT rc;

	GetCursorPos(&pt);

	CInfinateView::Instance()->ScreenToClient(&pt);
	CInfinateView::Instance()->GetClientRect(&rc);

	if (pt.x <= rc.left || pt.x >= rc.right ||
		pt.y <= rc.top || pt.y >= rc.bottom ||
		!xScene::Instance()->IsInited())
		return ;

	float fx = pt.x / (float)(rc.right - rc.left);
	float fy = pt.y / (float)(rc.bottom - rc.top);

	int DropCount=DragQueryFile(hDropInfo, -1, NULL, 0);

	char filename[128] = { 0 };
	Point2f point = Point2f(fx, fy);
	
	DragQueryFile(hDropInfo, 0, filename, 128);

	xEvent::OnDragFile(&point, filename);

	SetForegroundWindow();
}


void CInfinateView::OnSize(UINT nType, int cx, int cy)
{
	if (nType != SIZE_MINIMIZED)
		xApp::Instance()->Resize(cx, cy);
}

CInfinateDoc* CInfinateView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInfinateDoc)));
	return (CInfinateDoc*)m_pDocument;
}
