#pragma once

#include "ViewTree.h"
#include "xApp.h"

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class xExplorer;

class xExplorerTree : public CViewTree
{
	DECLARE_MESSAGE_MAP()

public:
	xExplorerTree(xExplorer * explorer);
	~xExplorerTree();

protected:
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	HTREEITEM mDragItem;
	bool mDragging;
	xExplorer * mExplorer;
};

class xExplorer : public IDockPane
{
	DECLARE_MESSAGE_MAP()

	struct Item
	{
		Item * parent;
		HTREEITEM hItem;
		TString128 name;
		bool floder;

		Array<Item*> children;

		Item() {}
		~Item()
		{
			for (int i = 0; i < children.Size(); ++i)
			{
				delete children[i];
			}

			children.Destroy();
		}
	};

public:
	xExplorer();
	virtual ~xExplorer();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	xExplorerTree mViewTree;
	CImageList mImages;
	UINT m_nCurrSort;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void _Init(Event * sender);
	void _UnloadScene(Event * sender);
	void _OnSerialize(Event * sender);
	void _AfterLoadScene(Event * sender);
	void _ObjCreated(Event * sender);
	void _ObjDistroy(Event * sender);

	void _SaveItem(Item & item, xSerializer & Serializer);
	void _LoadItem(Item & item, xSerializer & Serializer);

	void _CopyItem(HTREEITEM hDesItem, HTREEITEM hSrcItem);

protected:
	void _InsertItem(HTREEITEM hItem, Item & item);
	void _InitTreeView();
	Item * _getItem(HTREEITEM hItem);
	Item * _getItem(HTREEITEM hItem, Item & cItem);
	Item * _getItem(xObj * obj, Item & cItem);
	Item * _getItem(xObj * obj);
	void _deleteAllItem();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

protected:
	tEventListener<xExplorer> OnInit;
	tEventListener<xExplorer> OnUnloadScene;
	tEventListener<xExplorer> OnAfterLoadScene;
	tEventListener<xExplorer> OnSerialize;
	tEventListener<xExplorer> OnObjCreated;
	tEventListener<xExplorer> OnObjDistroy;

	Map<TString128, int> mTypeIconMap;

	Array<Item*> mItems;
};

