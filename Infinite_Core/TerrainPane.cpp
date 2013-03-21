#include "stdafx.h"

#include "TerrainPane.h"
#include "Editor.h"

namespace Infinite {

	#define xHeightPage 0
	#define xLayerPage 1
	#define xVegPage 2

	IMP_SLN(TerrainPane);

	TerrainPane::TerrainPane(MyGUI::Widget * _parent)
		: BaseLayout("Terrain.layout", _parent)
		, OnInit(xEvent::OnInitUI, this, &TerrainPane::_Init)
		, OnShutdown(xEvent::OnShutdown, this, &TerrainPane::_Shutdown)
		, OnUpdate(xEvent::OnUpdate, this, &TerrainPane::_Update)
		, OnRender(RenderEvent::OnAfterDefferedShading, this, &TerrainPane::_Render)
		, OnRenderUI(RenderEvent::OnAfterRender, this, &TerrainPane::_RenderUI)
		, OnUnLoadScene(xEvent::OnUnloadScene, this, &TerrainPane::_UnloadScene)
		, OnAfterLoadScene(xEvent::OnAfterLoadScene, this, &TerrainPane::_AfterloadScene)
	{
		INIT_SLN;

		assignWidget(mTabControl, "TabControl");
		assignBase(mHeightDlg, "Height");
		assignBase(mLayerDlg, "Layer");
		assignBase(mVegDlg, "Vegetation");
	}

	TerrainPane::~TerrainPane()
	{
		SHUT_SLN;
	}

	void TerrainPane::_Init(Event * sender)
	{
		mEditHeight._Init();
		mEditLayer._Init();
		mEditVeg._Init();
	}

	void TerrainPane::_Shutdown(Event * sender)
	{
		mEditHeight._Shutdown();
		mEditLayer._Shutdown();
		mEditVeg._Shutdown();
	}

	void TerrainPane::_Update(Event * sender)
	{
		int op = Editor::Instance()->GetOperator();

		if (op != eOP_Terrain)
			return ;

		int layerId = mLayerDlg->GetCurLayer();
		mEditLayer.SetLayer(layerId);

		int isel = mTabControl->getIndexSelected();

		if (isel == xHeightPage)
		{
			mEditHeight._Update();
		}
		else if (isel == xLayerPage)
		{
			mEditLayer._Update();
		}
		else if (isel == xVegPage)
		{
			mEditVeg._Update();
		}
	}

	void TerrainPane::_Render(Event * sender)
	{
		int op = Editor::Instance()->GetOperator();

		if (op != eOP_Terrain)
			return ;

		int isel = mTabControl->getIndexSelected();

		if (isel == xHeightPage)
		{
			mEditHeight._Render();
		}
		else if (isel == xLayerPage)
		{
			mEditLayer._Render();
		}
		else if (isel == xVegPage)
		{
			mEditVeg._Render();
		}
	}

	void TerrainPane::_RenderUI(Event * sender)
	{
		int op = Editor::Instance()->GetOperator();

		if (op != eOP_Terrain)
			return ;

		int isel = mTabControl->getIndexSelected();

		if (isel == xHeightPage)
		{
		}
		else if (isel == xLayerPage)
		{
			mEditLayer._RenderSectionLayer();
		}
	}

	void TerrainPane::_UnloadScene(Event * sender)
	{
		mEditLayer.SetLayer(-1);

		mLayerDlg->_UnloadScene(NULL);
		mVegDlg->_UnloadScene(NULL);
	}

	void TerrainPane::_AfterloadScene(Event * sender)
	{
		mEditLayer.SetLayer(-1);

		mLayerDlg->_AfterLoadScene(NULL);
		mVegDlg->_AfterLoadScene(NULL);
	}

}