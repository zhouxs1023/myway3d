#include "stdafx.h"
#include "xTerrainPane.h"
#include "xApp.h"
#include "resource.h"


#define xHeightPage 0
#define xLayerPage 1

//////////////////////////////////////////////////////////////////////////
//
// Terrain Pane
//
BEGIN_MESSAGE_MAP(xTerrainPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()

END_MESSAGE_MAP()

IMP_SLN(xTerrainPane);

xTerrainPane::xTerrainPane()
	: OnInit(xApp::OnInitUI, this, &xTerrainPane::_Init)
	, OnShutdown(xApp::OnShutdown, this, &xTerrainPane::_Shutdown)
	, OnUpdate(xApp::OnUpdate, this, &xTerrainPane::_Update)
	, OnRender(RenderEvent::OnAfterDeffererShading, this, &xTerrainPane::_Render)
	, OnRenderUI(RenderEvent::OnAfterRender, this, &xTerrainPane::_RenderUI)
{
	INIT_SLN;
}

xTerrainPane::~xTerrainPane()
{
	SHUT_SLN;
}

int xTerrainPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create tab control
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!mTab.Create(CMFCTabCtrl::STYLE_3D_VS2005, rectDummy, this, IDC_Terrain_Tab))
	{
		TRACE0("Create \"Terrain Tab\" failed!\n");
		return -1;
	}

	mTab.SetLocation(CMFCBaseTabCtrl::LOCATION_TOP);

	// create height dialog
	if (!mHeightDlg.Create(IDD_Terrain_Height, &mTab))
	{
		TRACE0("Create \"Terrain Height Dialog\" failed!\n");
		return -1;
	}

	// create layer dialog
	if (!mLayerDlg.Create(IDD_Terrain_Layer, &mTab))
	{
		TRACE0("Create \"Terrain Layer Dialog\" failed!\n");
		return -1;
	}

	mTab.AddTab(&mHeightDlg, "Height");
	mTab.AddTab(&mLayerDlg, "Layer");

	AdjustLayout();

	return 0;
}

void xTerrainPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void xTerrainPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;

	mTab.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);

	CRect rc;   
	mTab.GetClientRect(rc);   
	rc.top += 20;   
	rc.bottom -= 5;   
	rc.left += 5;   
	rc.right -= 5;

	mHeightDlg.MoveWindow(&rc);
	mLayerDlg.MoveWindow(&rc);
}

void xTerrainPane::_Init(void *)
{
	mEditHeight._Init(NULL);
	mEditLayer._Init(NULL);

	mHeightDlg._Init(NULL);
	mLayerDlg._Init(NULL);
}

void xTerrainPane::_Shutdown(void *)
{
	mEditHeight._Shutdown(NULL);
	mEditLayer._Shutdown(NULL);
}

void xTerrainPane::_Update(void *)
{
	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	int layerId = mLayerDlg.GetCurLayer();
	mEditLayer.SetLayer(layerId);

	int isel = mTab.GetActiveTab();

	if (isel == xHeightPage)
	{
		mEditHeight._Update(NULL);
	}
	else if (isel == xLayerPage)
	{
		mEditLayer._Update(NULL);
	}
}

void xTerrainPane::_Render(void *)
{
	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	int isel = mTab.GetActiveTab();

	if (isel == xHeightPage)
	{
		mEditHeight._Render(NULL);
	}
	else if (isel == xLayerPage)
	{
		mEditLayer._Render(NULL);
	}
}

void xTerrainPane::_RenderUI(void *)
{
	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	int isel = mTab.GetActiveTab();

	if (isel == xHeightPage)
	{
	}
	else if (isel == xLayerPage)
	{
		mEditLayer._RenderSectionLayer();
	}
}