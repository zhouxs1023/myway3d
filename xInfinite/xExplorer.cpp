#include "stdafx.h"
#include "xExplorer.h"
#include "Resource.h"
#include "Infinate.h"
#include "xScene.h"

BEGIN_MESSAGE_MAP(xExplorerTree, CViewTree)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
END_MESSAGE_MAP()

xExplorerTree::xExplorerTree(xExplorer * explorer)
{
	mExplorer = explorer;
	mDragging = false;
	mDragItem = NULL;
}

xExplorerTree::~xExplorerTree()
{
}

void xExplorerTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
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

void xExplorerTree::OnMouseMove(UINT nFlags, CPoint point)
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

void xExplorerTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mDragging) 
	{ 
		ImageList_EndDrag(); 

		ReleaseCapture();

		CRect winRC;
		GetWindowRect(&winRC);
		HTREEITEM hItem;
		if((hItem = HitTest(point, &nFlags)) != NULL)
		{
			if (mDragItem != hItem && hItem != GetParentItem(mDragItem))
			{
				mExplorer->_CopyItem(hItem, mDragItem);
			}

			mDragging = false; 
		}
	}
}








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
: IDockPane("Explorer")
, OnInit(&xEvent::OnInit, this, &xExplorer::_Init)
, OnUnloadScene(&xEvent::OnUnloadScene, this, &xExplorer::_UnloadScene)
, OnSerialize(&xEvent::OnSerialize, this, &xExplorer::_OnSerialize)
, OnAfterLoadScene(&xEvent::OnAfterLoadScene, this, &xExplorer::_AfterLoadScene)
, OnObjCreated(&xEvent::OnObjCreated, this, &xExplorer::_ObjCreated)
, OnObjDistroy(&xEvent::OnObjDistroy, this, &xExplorer::_ObjDistroy)
, mViewTree(this)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

xExplorer::~xExplorer()
{
	_deleteAllItem();
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


void xExplorer::_CopyItem(HTREEITEM hDesItem, HTREEITEM hSrcItem)
{
	if (hDesItem == NULL || hSrcItem == NULL)
		return;

	Item * desItem = _getItem(hDesItem);
	Item * srcItem = _getItem(hSrcItem);

	d_assert (desItem && srcItem);

	if (!desItem->floder)
		return ;


	int image = 0, image1 = 0;
	mViewTree.GetItemImage(hSrcItem, image, image1);
	
	_InsertItem(desItem->hItem, *srcItem);

	desItem->children.PushBack(srcItem);

	if (srcItem->parent)
	{
		for (int i = 0; i < srcItem->parent->children.Size(); ++i)
		{
			if (srcItem->parent->children[i] == srcItem)
			{
				srcItem->parent->children.Erase(i);
				break;
			}
		}
	}
	
	mViewTree.DeleteItem(hSrcItem);

	Invalidate();
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
	_deleteAllItem();
	mItems.Destroy();
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
		_SaveItem(*item.children[i], Serializer);
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
		Item * child = new Item();
		child->parent = &item;
		_LoadItem(*child, Serializer);

		item.children.PushBack(child);
	}
}

void xExplorer::_OnSerialize(void * param0, void * param1)
{
	int chunkId = *(int *)param0;
	xSerializer & Serializer = *(xSerializer *)param1;

	const int K_ChunkId = 'xExp';
	const int K_Version = 0;

	if (Serializer.IsSave())
	{
		Serializer << K_ChunkId;
		Serializer << K_Version;

		int size = mItems.Size();

		Serializer << size;

		for (int i = 0; i < size; ++i)
			_SaveItem(*mItems[i], Serializer);
	}
	else
	{
		if (K_ChunkId != chunkId)
			return ;

		int version = 0;

		Serializer >> version;

		if (version == 0)
		{
			int size;
			Serializer >> size;

			for (int i = 0; i < size; ++i)
			{
				Item * item = new Item();
				item->parent = NULL;
				_LoadItem(*item, Serializer);

				mItems.PushBack(item);
			}
		}
	}
}

void xExplorer::_AfterLoadScene(void * param0, void * param1)
{
	mViewTree.DeleteAllItems();

	for (int i = 0; i < mItems.Size(); ++i)
	{
		Item & item = *mItems[i];

		if (item.floder)
		{
			item.hItem = mViewTree.InsertItem(item.name.c_str(), 0, 0);

			for (int j = 0; j < item.children.Size(); ++j)
				_InsertItem(item.hItem, *item.children[j]);
		}
		else
		{
			xObj * obj = xObjManager::Instance()->Get(item.name.c_str());
			d_assert (obj);

			TString128 type = obj->GetTypeName();

			Map<TString128, int>::Iterator whr = mTypeIconMap.Find(type);
			Map<TString128, int>::Iterator end = mTypeIconMap.End();

			int index = 0;

			if (whr != end)
				index = whr->second;

			item.hItem = mViewTree.InsertItem(item.name.c_str(), index, index);
		}
	}
}

void xExplorer::_ObjCreated(void * param0, void * param1)
{
	xObj * obj = (xObj *)param0;

	TString128 type = obj->GetTypeName();

	Map<TString128, int>::Iterator whr = mTypeIconMap.Find(type);
	Map<TString128, int>::Iterator end = mTypeIconMap.End();

	int index = 0;

	if (whr != end)
		index = whr->second;

	Item * item = new Item();
	item->parent = NULL;
	item->name = obj->GetName();
	item->floder = false;

	item->hItem = mViewTree.InsertItem(item->name.c_str(), index, index);

	mItems.PushBack(item);
}

void xExplorer::_ObjDistroy(void * param0, void * param1)
{
	xObj * obj = (xObj *)param0;

	Item * item = _getItem(obj);

	if (item)
	{
		mViewTree.DeleteItem(item->hItem);

		if (item->parent)
		{
			for (int i = 0; i < item->parent->children.Size(); ++i)
			{
				if (item->parent->children[i] == item)
				{
					item->parent->children.Erase(i);
					break;
				}
			}
		}
		else
		{
			for (int i = 0; i < mItems.Size(); ++i)
			{
				if (mItems[i] == item)
				{
					mItems.Erase(i);
					break;
				}
			}
		}

		delete item;
	}
}

void xExplorer::_InsertItem(HTREEITEM hItem, Item & item)
{
	if (item.floder)
	{
		item.hItem = mViewTree.InsertItem(item.name.c_str(), 0, 0, hItem);

		for (int j = 0; j < item.children.Size(); ++j)
			_InsertItem(item.hItem, *item.children[j]);
	}
	else
	{
		xObj * obj = xObjManager::Instance()->Get(item.name.c_str());
		d_assert (obj);

		TString128 type = obj->GetTypeName();

		Map<TString128, int>::Iterator whr = mTypeIconMap.Find(type);
		Map<TString128, int>::Iterator end = mTypeIconMap.End();

		int index = 0;

		if (whr != end)
			index = whr->second;

		item.hItem = mViewTree.InsertItem(item.name.c_str(), index, index, hItem);
	}

	mViewTree.Expand(hItem, TVE_EXPAND);
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

xExplorer::Item * xExplorer::_getItem(HTREEITEM hItem, Item & cItem)
{
	if (cItem.hItem == hItem)
		return &cItem;
	
	for (int i = 0; i < cItem.children.Size(); ++i)
	{
		Item * ritem = _getItem(hItem, *cItem.children[i]);

		if (ritem != NULL)
			return ritem;
	}

	return NULL;
}

xExplorer::Item * xExplorer::_getItem(HTREEITEM hItem)
{
	for (int i = 0; i < mItems.Size(); ++i)
	{
		Item & item = *mItems[i];
		if (item.hItem == hItem)
			return &item;

		for (int j = 0; j < item.children.Size(); ++j)
		{
			Item * ritem = _getItem(hItem, *item.children[j]);

			if (ritem != NULL)
				return ritem;
		}
	}

	return NULL;
}

xExplorer::Item * xExplorer::_getItem(xObj * obj, Item & item)
{
	if (item.name == obj->GetName() && !item.floder)
		return &item;

	for (int j = 0; j < item.children.Size(); ++j)
	{
		Item * ritem = _getItem(obj, *item.children[j]);

		if (ritem != NULL)
			return ritem;
	}

	return NULL;
}

xExplorer::Item * xExplorer::_getItem(xObj * obj)
{
	for (int i = 0; i < mItems.Size(); ++i)
	{
		Item & item = *mItems[i];
		if (item.name == obj->GetName() && !item.floder)
			return &item;

		for (int j = 0; j < item.children.Size(); ++j)
		{
			Item * ritem = _getItem(obj, *item.children[j]);

			if (ritem != NULL)
				return ritem;
		}
	}

	return NULL;
}

void xExplorer::_deleteAllItem()
{
	for (int i = 0; i < mItems.Size(); ++i)
	{
		delete mItems[i];
	}

	mItems.Clear();
}

void xExplorer::OnNewFolder()
{
	if (!xScene::Instance()->IsInited())
		return ;

	HTREEITEM hItem = mViewTree.GetSelectedItem();

	bool bRoot = false;
	Item * rItem = NULL;

	if (hItem == NULL)
		bRoot = true;
	else
	{
		rItem = _getItem(hItem);

		d_assert (rItem);

		if (rItem->floder)
		{
			bRoot = false;
		}
		else
		{
			bRoot = true;
		}
	}

	if (bRoot)
	{
		Item * item = new Item();
		item->parent = NULL;
		item->name = "Floder";
		item->floder = true;
		item->hItem = mViewTree.InsertItem(item->name.c_str(), 0, 0);

		mItems.PushBack(item);
	}
	else
	{
		Item * item = new Item();
		item->parent = rItem;
		item->name = "Floder";
		item->floder = true;
		item->hItem = mViewTree.InsertItem(item->name.c_str(), 0, 0, rItem->hItem);

		rItem->children.PushBack(item);
	}

	mViewTree.Invalidate();
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
