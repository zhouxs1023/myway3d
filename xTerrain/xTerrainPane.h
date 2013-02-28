#pragma once

#include "IDockPane.h"
#include "xTerrainOp.h"
#include "xTerrainHeightDlg.h"
#include "xTerrainLayerDlg.h"
#include "xTerrainVegDlg.h"
#include "xEditTerrainHeight.h"
#include "xEditTerrainLayer.h"
#include "xEditTerrainVeg.h"


class xTerrainPane : public IDockPane
{
	DECLARE_MESSAGE_MAP()

	DECLARE_SINGLETON(xTerrainPane);

public:
	xTerrainPane();
	virtual ~xTerrainPane();

	void AdjustLayout();

	xEditTerrainHeight * GetTerrainHeight() { return &mEditHeight; }
	xEditTerrainLayer * GetTerrainLayer() { return &mEditLayer; }
	xEditTerrainVeg * GetTerrainVeg() { return &mEditVeg; }

	xTerrainHeightDlg * GetTerrainHeightDlg() { return &mHeightDlg; }
	xTerrainLayerDlg * GetTerrainLayerDlg() { return &mLayerDlg; }
	xTerrainVegDlg * GetTerrainVegDlg() { return &mVegDlg; }

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) {}

	void _Create(Event * sender);
	void _Init(Event * sender);
	void _Shutdown(Event * sender);
	void _Update(Event * sender);
	void _Render(Event * sender);
	void _RenderUI(Event * sender);
	void _UnloadScene(Event * sender);
	void _AfterloadScene(Event * sender);

protected:
	tEventListener<xTerrainPane> OnCreatePane;
	tEventListener<xTerrainPane> OnInit;
	tEventListener<xTerrainPane> OnShutdown;
	tEventListener<xTerrainPane> OnUpdate;
	tEventListener<xTerrainPane> OnRender;
	tEventListener<xTerrainPane> OnRenderUI;
	tEventListener<xTerrainPane> OnUnLoadScene;
	tEventListener<xTerrainPane> OnAfterLoadScene;

	CMFCTabCtrl mTab;

	xTerrainHeightDlg mHeightDlg;
	xTerrainLayerDlg mLayerDlg;
	xTerrainVegDlg mVegDlg;

	xEditTerrainHeight mEditHeight;
	xEditTerrainLayer mEditLayer;
	xEditTerrainVeg mEditVeg;
};

