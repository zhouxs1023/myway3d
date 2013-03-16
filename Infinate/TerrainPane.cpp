#include "stdafx.h"

#include "TerrainPane.h"
#include "Editor.h"

namespace Infinite {

	TerrainHeightDlg::TerrainHeightDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainHeight.layout", _parent)
		
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushDensity, "BrushDensity");
		assignWidget(mBrushType, "BrushType");
		assignWidget(mBrushOperator, "BrushOperator");

		mBrushOperator->insertItemAt(0, "Up");
		mBrushOperator->insertItemAt(1, "Down");
		mBrushOperator->insertItemAt(2, "Smooth");
		mBrushOperator->setIndexSelected(0);

		FileSystem fs("../Core/Terrain/Brushes");

		fs.Load();

		Archive::FileInfoVisitor v = fs.GetFileInfos();

		while (!v.Endof())
		{
			const Archive::FileInfo & info = v.Cursor()->second;

			if (info.type == Archive::FILE_ARCHIVE)
			{
				TString128 base = File::RemoveExternName(info.name);
				TString128 ext = File::GetExternName(info.name);

				if (ext == "png")
					mBrushType->addItem(base.c_wstr());
			}

			++v;
		}

		mBrushType->setIndexSelected(0);

		mBrushSize->setScrollRange(200);
		mBrushSize->setScrollPosition(50);

		mBrushDensity->setScrollRange(200);
		mBrushDensity->setScrollPosition(100);

		mBrushSize->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushSizeDensityChanged);
		mBrushDensity->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainHeightDlg::notifyBrushSizeDensityChanged);
	}

	TerrainHeightDlg::~TerrainHeightDlg()
	{
	}














	#define xHeightPage 0
	#define xLayerPage 1
	#define xVegPage 2

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
		assignWidget(mTabControl, "TabControl");
	}

	TerrainPane::~TerrainPane()
	{
	}

	void TerrainPane::_Init(Event * sender)
	{
		mEditHeight._Init(NULL);
		mEditLayer._Init(NULL);
		mEditVeg._Init(NULL);
	}

	void TerrainPane::_Shutdown(Event * sender)
	{
		mEditHeight._Shutdown(NULL);
		mEditLayer._Shutdown(NULL);
		mEditVeg._Shutdown(NULL);
	}

	void TerrainPane::_Update(Event * sender)
	{
		int op = Editor::Instance()->GetOperator();

		if (op != eOP_Terrain)
			return ;

		/*int layerId = mLayerDlg.GetCurLayer();
		mEditLayer.SetLayer(layerId);*/

		int isel = mTabControl->getIndexSelected();

		if (isel == xHeightPage)
		{
			mEditHeight._Update(NULL);
		}
		else if (isel == xLayerPage)
		{
			mEditLayer._Update(NULL);
		}
		else if (isel == xVegPage)
		{
			mEditVeg._Update(NULL);
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
			mEditHeight._Render(NULL);
		}
		else if (isel == xLayerPage)
		{
			mEditLayer._Render(NULL);
		}
		else if (isel == xVegPage)
		{
			mEditVeg._Render(NULL);
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
		//mLayerDlg._UnloadScene(NULL);
		//mVegDlg._UnloadScene(NULL);
	}

	void TerrainPane::_AfterloadScene(Event * sender)
	{
		mEditLayer.SetLayer(-1);
		//mLayerDlg._AfterLoadScene(NULL);
		//mVegDlg._AfterLoadScene(NULL);
	}

}