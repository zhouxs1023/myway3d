#include "stdafx.h"

#include "TerrainLayerDlg.h"
#include "TerrainPane.h"

namespace Infinite {

	TerrainLayerDlg::TerrainLayerDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainLayer.layout", _parent)
		, OnAddLayer(this, &TerrainLayerDlg::_notifyAddLayer)
		, OnEditLayer(this, &TerrainLayerDlg::_notifyEditLayer)
		, mLayerInfoDlg(NULL)
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushDensity, "BrushDensity");
		assignWidget(mBrushType, "BrushType");
		assignWidget(mAdd, "Add");
		assignWidget(mRemove, "Remove");
		assignWidget(mEdit, "Edit");
		assignWidget(mLayerList, "LayerList");

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

		notifyBrushSizeDensityChanged(NULL, 0);
		notifyBrushTypeChanged(mBrushType, 0);

		mBrushSize->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyBrushSizeDensityChanged);
		mBrushDensity->eventScrollChangePosition += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyBrushSizeDensityChanged);

		mBrushType->eventComboChangePosition += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyBrushTypeChanged);

		mAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyAdd);
		mRemove->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyRemove);
		mEdit->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainLayerDlg::notifyEdit);
	}

	TerrainLayerDlg::~TerrainLayerDlg()
	{
		if (mLayerInfoDlg)
		{
			mLayerInfoDlg->Event_OnOK -= &OnAddLayer;
			mLayerInfoDlg->Event_OnOK -= &OnEditLayer;
		}

		safe_delete (mLayerInfoDlg);
	}

	void TerrainLayerDlg::_UnloadScene(Event * sender)
	{
		mLayerList->removeAllItems();
		mLayerIds.Clear();
	}

	void TerrainLayerDlg::_AfterLoadScene(Event * sender)
	{
		mLayerList->removeAllItems();
		mLayerIds.Clear();

		Terrain * terrain = Environment::Instance()->GetTerrain();

		if (!terrain)
			return ;

		int index = 0;
		for (int i = 0; i < Terrain::kMaxLayers; ++i)
		{
			const Terrain::Layer * layer = terrain->GetLayer(i);

			if (layer->detail != "")
			{
				mLayerList->insertItemAt(index++, layer->detail.c_str());
				mLayerIds.PushBack(i);
			}
		}
	}

	int TerrainLayerDlg::GetCurLayer()
	{
		size_t isel = mLayerList->getIndexSelected();

		if (isel == MyGUI::ITEM_NONE)
			return -1;

		return mLayerIds[isel];
	}


	void TerrainLayerDlg::notifyBrushSizeDensityChanged(MyGUI::ScrollBar* _sender, size_t _position)
	{
		size_t size = mBrushSize->getScrollPosition();
		size_t density = mBrushDensity->getScrollPosition();

		TerrainPane::Instance()->GetTerrainLayer()->SetBrushSize((float)size);
		TerrainPane::Instance()->GetTerrainLayer()->SetBrushDensity(density / (float)200);
	}

	void TerrainLayerDlg::notifyBrushTypeChanged(MyGUI::ComboBox* _sender, size_t _index)
	{
		const char * brush = mBrushType->getItemNameAt(_index).asUTF8_c_str();

		TString128 texture = TString128("Terrain\\Brushes\\") + brush + ".png";

		TerrainPane::Instance()->GetTerrainLayer()->SetBrush(texture);
	}

	void TerrainLayerDlg::notifyAdd(MyGUI::Widget * _sender)
	{
		Terrain * tn = Environment::Instance()->GetTerrain();

		if (tn == NULL)
			return ;

		ShowLayerInfo(NULL);
	}

	void TerrainLayerDlg::notifyRemove(MyGUI::Widget * _sender)
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();

		if (terrain == NULL)
			return ;

		size_t curSel = mLayerList->getIndexSelected();

		if (curSel == MyGUI::ITEM_NONE)
			return ;

		mLayerList->removeItemAt(curSel);

		int layer = mLayerIds[curSel];

		terrain->RemoveLayer(curSel);

		mLayerIds.Erase(curSel);
	}

	void TerrainLayerDlg::notifyEdit(MyGUI::Widget * _sender)
	{
		Terrain * tn = Environment::Instance()->GetTerrain();

		if (tn == NULL)
			return ;

		size_t isel = mLayerList->getIndexSelected();

		if (isel == MyGUI::ITEM_NONE)
			return ;

		int layerId = mLayerIds[isel];
		const Terrain::Layer * layer = tn->GetLayer(layerId);

		ShowLayerInfo(layer);
	}

	void TerrainLayerDlg::ShowLayerInfo(const Terrain::Layer * layer)
	{
		if (mLayerInfoDlg == NULL)
		{
			mLayerInfoDlg = new TerrainLayerInfoDlg();

			mLayerInfoDlg->Event_OnOK += &OnAddLayer;
			mLayerInfoDlg->Event_OnOK += &OnEditLayer;
		}

		mLayerInfoDlg->DoModal(layer);
	}

	void TerrainLayerDlg::_notifyAddLayer(Event * _sender)
	{
		if (mLayerInfoDlg->GetLayer()) // is edit
			return ;

		Terrain * tn = Environment::Instance()->GetTerrain();

		Terrain::Layer layer;

		layer.detail = mLayerInfoDlg->GetDiffuseMap();
		layer.normal = mLayerInfoDlg->GetNormalMap();
		layer.specular = mLayerInfoDlg->GetSpecularMap();
		layer.scale = mLayerInfoDlg->GetUVScale();
		layer.material = -1;

		int id = tn->AddLayer(layer);

		mLayerList->insertItemAt(mLayerList->getItemCount(), layer.detail.c_str());

		mLayerIds.PushBack(id);
	}

	void TerrainLayerDlg::_notifyEditLayer(Event * _sender)
	{
		if (mLayerInfoDlg->GetLayer() == NULL) // is add
			return ;

		Terrain * tn = Environment::Instance()->GetTerrain();

		int layerId = GetCurLayer();

		d_assert (layerId != -1);

		Terrain::Layer layer;

		layer.detail = mLayerInfoDlg->GetDiffuseMap();
		layer.normal = mLayerInfoDlg->GetNormalMap();
		layer.specular = mLayerInfoDlg->GetSpecularMap();
		layer.scale = mLayerInfoDlg->GetUVScale();
		layer.material = -1;

		tn->SetLayer(layerId, layer);

		size_t isel = mLayerList->getIndexSelected();

		d_assert (isel != MyGUI::ITEM_NONE);

		mLayerList->setItemNameAt(isel, layer.detail.c_wstr());
	}
}