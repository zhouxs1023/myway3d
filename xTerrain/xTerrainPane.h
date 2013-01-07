#pragma once

#include "IDockPane.h"
#include "xTerrainOp.h"
#include "xTerrainHeightDlg.h"
#include "xTerrainLayerDlg.h"
#include "xEditTerrainHeight.h"
#include "xEditTerrainLayer.h"


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

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) {}

	void _Create(void * param0, void * param1);
	void _Init(void * param0, void * param1);
	void _Shutdown(void * param0, void * param1);
	void _Update(void * param0, void * param1);
	void _Render(void * param0, void * param1);
	void _RenderUI(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);
	void _AfterloadScene(void * param0, void * param1);

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

	xEditTerrainHeight mEditHeight;
	xEditTerrainLayer mEditLayer;
};

