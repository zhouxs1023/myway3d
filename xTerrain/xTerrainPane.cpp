#include "stdafx.h"
#include "xTerrainPane.h"
#include "xApp.h"
#include "xAfxResourceSetup.h"
#include "resource.h"


#define xHeightPage 0
#define xLayerPage 1

//////////////////////////////////////////////////////////////////////////
//
// Terrain Pane
//
xTerrainPane gTerrainPane;

BEGIN_MESSAGE_MAP(xTerrainPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()

END_MESSAGE_MAP()

IMP_SLN(xTerrainPane);

xTerrainPane::xTerrainPane()
	: OnCreatePane(&xEvent::OnCreatePane, this, &xTerrainPane::_Create)
	, OnInit(&xEvent::OnInitUI, this, &xTerrainPane::_Init)
	, OnShutdown(&xEvent::OnShutdown, this, &xTerrainPane::_Shutdown)
	, OnUpdate(&xEvent::OnUpdate, this, &xTerrainPane::_Update)
	, OnRender(&RenderEvent::OnAfterDeffererShading, this, &xTerrainPane::_Render)
	, OnRenderUI(&RenderEvent::OnAfterRender, this, &xTerrainPane::_RenderUI)
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

	afx_resource_setup();

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

void xTerrainPane::_Create(void * param0, void * param1)
{
	CFrameWndEx * frame = (CFrameWndEx *)param0;

	if (!Create("Terrain", frame, CRect(0, 0, 200, 200), TRUE, IDD_Terrain, 
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("can't create \"property pane\"\n");
		return ;
	}

	EnableDocking(CBRS_ALIGN_ANY);
	frame->DockPane(this);
}

void xTerrainPane::_Init(void * param0, void * param1)
{
	mEditHeight._Init(NULL, NULL);
	mEditLayer._Init(NULL, NULL);

	mHeightDlg._Init(NULL, NULL);
	mLayerDlg._Init(NULL, NULL);
}

void xTerrainPane::_Shutdown(void * param0, void * param1)
{
	mEditHeight._Shutdown(NULL, NULL);
	mEditLayer._Shutdown(NULL, NULL);
}

void xTerrainPane::_Update(void * param0, void * param1)
{
	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	int layerId = mLayerDlg.GetCurLayer();
	mEditLayer.SetLayer(layerId);

	int isel = mTab.GetActiveTab();

	if (isel == xHeightPage)
	{
		mEditHeight._Update(NULL, NULL);
	}
	else if (isel == xLayerPage)
	{
		mEditLayer._Update(NULL, NULL);
	}
}

void xTerrainPane::_Render(void * param0, void * param1)
{
	int op = xApp::Instance()->GetOperator();

	if (op != xTerrainOp::eOp_Terrain)
		return ;

	int isel = mTab.GetActiveTab();

	if (isel == xHeightPage)
	{
		mEditHeight._Render(NULL, NULL);
	}
	else if (isel == xLayerPage)
	{
		mEditLayer._Render(NULL, NULL);
	}
}

void xTerrainPane::_RenderUI(void * param0, void * param1)
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