#include "stdafx.h"
#include "xResourceView.h"
#include "Resource.h"
#include "Infinate.h"
#include "InfinateDoc.h"
#include "InfinateView.h"
#include "xScene.h"
#include "xEvent.h"


BEGIN_MESSAGE_MAP(xResourceTree, CViewTree)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
END_MESSAGE_MAP()

xResourceTree::xResourceTree()
{
}

xResourceTree::~xResourceTree()
{
}

void xResourceTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
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

void xResourceTree::OnMouseMove(UINT nFlags, CPoint point)
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

void xResourceTree::OnLButtonUp(UINT nFlags, CPoint point)
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
			TString128 filename = (LPCTSTR)GetItemText(mDragItem);

			Point2f point;

			point.x = pt.x / (float)(rc.right - rc.left);
			point.y = pt.y / (float)(rc.bottom - rc.top);

			xEvent::OnDragFile(&point, filename.c_str());
		}

		mDragging = false; 
	} 
}






class xResourceViewMenuButton : public CMFCToolBarMenuButton
{
	friend class xResourceView;

	DECLARE_SERIAL(xResourceViewMenuButton)

public:
	xResourceViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(xResourceViewMenuButton, CMFCToolBarMenuButton, 1)

xResourceView::xResourceView()
	: OnInit(xEvent::OnInit, this, &xResourceView::_Init)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

xResourceView::~xResourceView()
{
}

BEGIN_MESSAGE_MAP(xResourceView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

int xResourceView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!mViewTree.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("未能创建类视图\n");
		return -1;
	}

	// 加载图像:
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SORT);
	m_wndToolBar.LoadToolBar(IDR_SORT, 0, 0, TRUE);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 所有命令将通过此控件路由，而不是通过主框架路由:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, xResourceViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	xResourceViewMenuButton* pButton =  DYNAMIC_DOWNCAST(xResourceViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	return 0;
}

void xResourceView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void xResourceView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void xResourceView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mViewTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL xResourceView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void xResourceView::_Init(Event * sender)
{
	mTypeIconMap.Clear();
	mImages.DeleteImageList();

	mImages.Create(16, 16, ILC_MASK, 0, 4);

	{
		HICON hIcon = (HICON)LoadImage(theApp.m_hInstance, "..\\ui\\default.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		mImages.Add(hIcon);
	}

	for (int i = 0; i < xObjManager::Instance()->GetFactoryCount(); ++i)
	{
		xObjFactory * obj =  xObjManager::Instance()->GetFactory(i);
		TString128 type = obj->GetTypeName();
		const char * icon = obj->GetIcon();
		TString128 ext = obj->GetExternName();

		HICON hIcon = (HICON)LoadImage(theApp.m_hInstance, icon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		int index = mImages.Add(hIcon);

		mTypeIconMap.Insert(type, index);
		mExternTypes.Insert(ext, type);
	}

	mViewTree.SetImageList(&mImages, TVSIL_NORMAL);

	ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();
	ResourceGroup::ArchiveVisitor v = rg->GetArchives();

	while (!v.eof())
	{
		Archive * ar = *v.Cursor();

		HTREEITEM hItem =mViewTree.InsertItem(ar->GetName().c_str(), 0, 0);

		Archive::FileInfoVisitor fv = ar->GetFileInfos();

		while (!fv.eof())
		{
			const Archive::FileInfo & fileInfo = fv.Cursor()->second;
			const TString128 & filename = fileInfo.name;
			TString128 ext = File::GetExternName(filename);
			
			Map<TString128, TString128>::Iterator whr = mExternTypes.Find(ext);
			Map<TString128, TString128>::Iterator end = mExternTypes.End();
			
			if (whr != end)
			{
				const TString128 & type = whr->second;

				Map<TString128, int>::Iterator whr1 = mTypeIconMap.Find(type);
				Map<TString128, int>::Iterator end1 = mTypeIconMap.End();

				int index = 0;
				if (whr1 != end1)
					index = whr1->second;

				mViewTree.InsertItem(filename.c_str(), index, index, hItem);
			}

			++fv;
		}

		++v;
	}
}

void xResourceView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	xResourceViewMenuButton* pButton =  DYNAMIC_DOWNCAST(xResourceViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void xResourceView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void xResourceView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	mViewTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void xResourceView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	mViewTree.SetFocus();
}

void xResourceView::OnChangeVisualStyle()
{
	mImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

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

	mImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	mImages.Add(&bmp, RGB(255, 0, 0));

	mViewTree.SetImageList(&mImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SORT_24 : IDR_SORT, 0, 0, TRUE /* 锁定*/);
}
