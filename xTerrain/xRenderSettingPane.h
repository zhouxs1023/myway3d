#pragma once

#include "IDockPane.h"

class xRenderSettingPane : public IDockPane
{
	DECLARE_MESSAGE_MAP()

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) {}

	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);

public:
	xRenderSettingPane();
	virtual ~xRenderSettingPane();

	void AdjustLayout();
	void Show(bool b);

protected:
	void _OnSelect(Event * sender);
	void _OnUnSelect(Event * sender);
	void _ToCtrl(CMFCPropertyGridProperty * gp, IPropertyObj * obj, const Property * p);

	void _Create(Event * sender);
	void _AfterLoadScene(Event * sender);

protected:
	tEventListener<xRenderSettingPane> OnCreatePane;
	tEventListener<xRenderSettingPane> OnAfterLoadScene;

	CMFCPropertyGridCtrl m_wndPropList;
};

