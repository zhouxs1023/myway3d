#pragma once

#include "xApp.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};



class xPropertyGrid : public IDockPane
{
	DECLARE_SINGLETON(xPropertyGrid);
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) {}

	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);

public:
	xPropertyGrid();
	virtual ~xPropertyGrid();

	void AdjustLayout();

protected:
	void _OnSelect(void * param0, void * param1);
	void _OnUnSelect(void * param0, void * param1);
	void Show(xObj * obj);
	void _ToCtrl(CMFCPropertyGridProperty * gp, xObj * obj, const Property * p);

protected:
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;


	tEventListener<xPropertyGrid> OnSelectObj;
	tEventListener<xPropertyGrid> OnUnSelectObj;
};

