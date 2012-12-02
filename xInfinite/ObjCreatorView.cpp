#include "stdafx.h"
#include "mainfrm.h"
#include "ObjCreatorView.h"
#include "Resource.h"
#include "Infinate.h"
#include "InfinateDoc.h"
#include "InfinateView.h"
#include "xObj.h"
#include "xApp.h"
#include "xScene.h"


BEGIN_MESSAGE_MAP(ObjCreatorTree, CViewTree)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
END_MESSAGE_MAP()

ObjCreatorTree::ObjCreatorTree()
{
}

ObjCreatorTree::~ObjCreatorTree()
{
}

void ObjCreatorTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	mDragItem = pNMTreeView->itemNew.hItem;
	
	HIMAGELIST himl;    // handle to image list 
	RECT rcItem;        // bounding rectangle of item 

	// Tell the tree-view control to create an image to use 
	// for dragging. 
	himl = TreeView_CreateDragImage(m_hWnd, pNMTreeView->itemNew.hItem); 

	// Get the bounding rectangle of the item being dragged. 
	TreeView_GetItemRect(m_hWnd, pNMTreeView->itemNew.hItem, &rcItem, TRUE); 

	// Start the drag operation. 
	ImageList_BeginDrag(himl, 0, 0, 0);
	//ImageList_DragEnter(m_hWnd, pNMTreeView->ptDrag.x, pNMTreeView->ptDrag.x); 

	SetCapture();
	ImageList_DragShowNolock(TRUE);

	mDragging = TRUE;
}

void ObjCreatorTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mDragging) 
	{
		POINT pt;
		GetCursorPos(&pt);
		// Drag the item to the current position of the mouse pointer. 
		// First convert the dialog coordinates to control coordinates. 
		ImageList_DragMove(pt.x, pt.y);
	} 
}

void ObjCreatorTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mDragging) 
	{ 
		ImageList_EndDrag(); 

		ReleaseCapture();

		POINT pt;
		RECT rc;

		GetCursorPos(&pt);

		CInfinateView::Instance()->ScreenToClient(&pt);
		CInfinateView::Instance()->GetClientRect(&rc);

		if (pt.x > rc.left && pt.x < rc.right &&
			pt.y > rc.top && pt.y < rc.bottom &&
			xScene::Instance()->IsInited())
		{
			CString type = GetItemText(mDragItem);

			xObj * obj = xObjManager::Instance()->Create((const char *)type);

			if (obj)
			{
				float fx = pt.x / (float)(rc.right - rc.left);
				float fy = pt.y / (float)(rc.bottom - rc.top);

				Vec3 pos = xApp::Instance()->GetHitPosition(fx, fy);

				obj->SetPosition(pos);
				obj->SetScale(0.01f);
			}

			xApp::Instance()->SetSelectedObj(obj);
		}

		mDragging = false; 
	} 
}

void ObjCreatorTree::OnTimer(UINT_PTR nIDEvent)
{

}










ObjCreatorView::ObjCreatorView()
	: OnInit(&xEvent::OnInitUI, this, &ObjCreatorView::_Init)
{
}

ObjCreatorView::~ObjCreatorView()
{
}

BEGIN_MESSAGE_MAP(ObjCreatorView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int ObjCreatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!mCreatorView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("create object view failed\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	mCreatorView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)
	AdjustLayout();

	return 0;
}

void ObjCreatorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void ObjCreatorView::_Init(void * param0, void * param1)
{
	MultiMap<TString128, xObjFactory *> mmap;

	for (int i = 0; i < xObjManager::Instance()->GetFactoryCount(); ++i)
	{
		xObjFactory * obj = xObjManager::Instance()->GetFactory(i);
		mmap.Insert(obj->GetGroupName(), obj);
	}

	MultiMap<TString128, xObjFactory *>::Iterator whr = mmap.Begin();
	MultiMap<TString128, xObjFactory *>::Iterator end = mmap.End();

	while (whr != end)
	{
		HTREEITEM hGroup = mCreatorView.InsertItem(whr->first.c_str(), 0, 0);

		List<xObjFactory *>::Iterator w = whr->second.Begin();
		List<xObjFactory *>::Iterator e = whr->second.End();

		while (w != e)
		{
			xObjFactory * factory = *w;

			mCreatorView.InsertItem(factory->GetTypeName(), 1, 1, hGroup);

			++w;
		}

		mCreatorView.Expand(hGroup, TVE_EXPAND);

		++whr;
	}
}

void ObjCreatorView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mCreatorView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void ObjCreatorView::OnProperties()
{
	AfxMessageBox(_T("属性...."));
}

void ObjCreatorView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	mCreatorView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void ObjCreatorView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	mCreatorView.SetFocus();
}

void ObjCreatorView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	mCreatorView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


