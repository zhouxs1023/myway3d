#pragma once

#include "ViewTree.h"
#include "xApp.h"

class CResourceToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};


class xResourceTree : public CViewTree
{
	DECLARE_MESSAGE_MAP()

public:
	xResourceTree();
	~xResourceTree();

protected:
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	HTREEITEM mDragItem;
	bool mDragging;
};

class xResourceView : public CDockablePane
{
	DECLARE_MESSAGE_MAP()

public:
	xResourceView();
	virtual ~xResourceView();

	void AdjustLayout();
	void OnChangeVisualStyle();

protected:
	CResourceToolBar m_wndToolBar;
	xResourceTree mViewTree;
	CImageList mImages;
	UINT m_nCurrSort;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void _Init(Event * sender);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

protected:
	tEventListener<xResourceView> OnInit;
	Map<TString128, int> mTypeIconMap;
	Map<TString128, TString128> mExternTypes;
};

