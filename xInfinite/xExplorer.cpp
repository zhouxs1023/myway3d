#include "stdafx.h"
#include "xExplorer.h"
#include "Resource.h"
#include "Infinate.h"


class xExplorerMenuButton : public CMFCToolBarMenuButton
{
	friend class xExplorer;

	DECLARE_SERIAL(xExplorerMenuButton)

public:
	xExplorerMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
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

IMPLEMENT_SERIAL(xExplorerMenuButton, CMFCToolBarMenuButton, 1)

xExplorer::xExplorer()
: OnInit(&xEvent::OnInit, this, &xExplorer::_Init)
, OnUnloadScene(&xEvent::OnInit, this, &xExplorer::_UnloadScene)
, OnSerialize(&xEvent::OnSerialize, this, &xExplorer::_OnSerialize)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

xExplorer::~xExplorer()
{
}

BEGIN_MESSAGE_MAP(xExplorer, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
END_MESSAGE_MAP()

int xExplorer::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, xExplorerMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	xExplorerMenuButton* pButton =  DYNAMIC_DOWNCAST(xExplorerMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	return 0;
}

void xExplorer::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void xExplorer::FillClassView()
{
	HTREEITEM hRoot = mViewTree.InsertItem(_T("FakeApp 类"), 0, 0);
	mViewTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = mViewTree.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	mViewTree.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	mViewTree.Expand(hRoot, TVE_EXPAND);

	hClass = mViewTree.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	mViewTree.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = mViewTree.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	mViewTree.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	mViewTree.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = mViewTree.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	mViewTree.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	mViewTree.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	mViewTree.Expand(hClass, TVE_EXPAND);

	hClass = mViewTree.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	mViewTree.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	mViewTree.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	mViewTree.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	mViewTree.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);

	hClass = mViewTree.InsertItem(_T("Globals"), 2, 2, hRoot);
	mViewTree.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	mViewTree.Expand(hClass, TVE_EXPAND);
}

void xExplorer::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void xExplorer::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	mViewTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL xExplorer::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void xExplorer::_Init(void * param0, void * param1)
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

		HICON hIcon = (HICON)LoadImage(theApp.m_hInstance, icon, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		int index = mImages.Add(hIcon);

		mTypeIconMap.Insert(type, index);
	}

	mViewTree.SetImageList(&mImages, TVSIL_NORMAL);
}

void xExplorer::_UnloadScene(void * param0, void * param1)
{
	mViewTree.DeleteAllItems();
}

void xExplorer::_SaveItem(Item & item, xSerializer & Serializer)
{
	int size = item.children.Size();

	Serializer << item.name;
	Serializer << item.floder;
	Serializer << size;

	for (int i = 0; i < size; ++i)
	{
		_SaveItem(item.children[i], Serializer);
	}
}

void xExplorer::_LoadItem(Item & item, xSerializer & Serializer)
{
	Serializer >> item.name;
	Serializer >> item.floder;

	int size = 0;

	Serializer >> size;

	for (int i = 0; i < size; ++i)
	{
		Item child;

		_LoadItem(child, Serializer);

		item.children.PushBack(child);
	}
}

void xExplorer::_OnSerialize(void * param0, void * param1)
{
	int chunkId = *(int *)param0;
	xSerializer & Serializer = *(xSerializer *)param1;

	const int K_Version = 0;

	if (Serializer.IsSave())
	{
		Serializer << K_Version;

		int size = mItems.Size();

		Serializer << size;

		for (int i = 0; i < size; ++i)
			_SaveItem(mItems[i], Serializer);
	}
	else
	{
		int version = 0;

		Serializer >> version;

		if (version == 0)
		{
			int size;
			Serializer >> size;

			for (int i = 0; i < size; ++i)
			{
				Item item;

				_LoadItem(item, Serializer);

				mItems.PushBack(item);
			}
		}
	}
}

void xExplorer::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	xExplorerMenuButton* pButton =  DYNAMIC_DOWNCAST(xExplorerMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void xExplorer::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void xExplorer::OnNewFolder()
{
	AfxMessageBox(_T("新建文件夹..."));
}

void xExplorer::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	mViewTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void xExplorer::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	mViewTree.SetFocus();
}

void xExplorer::OnChangeVisualStyle()
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
