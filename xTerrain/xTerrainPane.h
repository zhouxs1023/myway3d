#pragma once

#include "xTerrainOp.h"
#include "xTerrainHeightDlg.h"
#include "xTerrainLayerDlg.h"
#include "xEditTerrainHeight.h"
#include "xEditTerrainLayer.h"

extern HANDLE ghModule;

class xTerrainPane : public CDockablePane
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


	void _Create(void * param);
	void _Init(void *);
	void _Shutdown(void *);
	void _Update(void *);
	void _Render(void *);
	void _RenderUI(void *);

protected:
	tEventListener<xTerrainPane> OnCreatePane;
	tEventListener<xTerrainPane> OnInit;
	tEventListener<xTerrainPane> OnShutdown;
	tEventListener<xTerrainPane> OnUpdate;
	tEventListener<xTerrainPane> OnRender;
	tEventListener<xTerrainPane> OnRenderUI;

	CMFCTabCtrl mTab;

	xTerrainHeightDlg mHeightDlg;
	xTerrainLayerDlg mLayerDlg;

	xEditTerrainHeight mEditHeight;
	xEditTerrainLayer mEditLayer;
};

