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

class xExplorer : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

	struct Item
	{
		TString128 name;
		bool floder;

		Array<Item> children;
	};

public:
	xExplorer();
	virtual ~xExplorer();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CClassToolBar m_wndToolBar;
	CViewTree mViewTree;
	CImageList mImages;
	UINT m_nCurrSort;

	void FillClassView();

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void _Init(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);
	void _OnSerialize(void * param0, void * param1);

	void _SaveItem(Item & item, xSerializer & Serializer);
	void _LoadItem(Item & item, xSerializer & Serializer);

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
	tEventListener<xExplorer> OnSerialize;

	Map<TString128, int> mTypeIconMap;

	Array<Item> mItems;
};

