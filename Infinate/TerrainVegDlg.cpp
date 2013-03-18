#include "stdafx.h"

#include "TerrainVegDlg.h"
#include "Editor.h"

namespace Infinite {

	TerrainVegDlg::TerrainVegDlg(MyGUI::Widget * _parent)
		: BaseLayout("TerrainVegetation.layout", _parent)
		, OnAddVeg(this, &TerrainVegDlg::_OnAddVeg)
		, OnEditVeg(this, &TerrainVegDlg::_OnEditVeg)
		, mVegInfoDlg(NULL)
	{
		assignWidget(mBrushSize, "BrushSize");
		assignWidget(mBrushDensity, "BrushDensity");
		assignWidget(mBrushType, "BrushType");
		assignWidget(mAdd, "Add");
		assignWidget(mRemove, "Remove");
		assignWidget(mEdit, "Edit");
		assignWidget(mVegList, "VegList");
		assignWidget(mMinSize, "MinSize");
		assignWidget(mMaxSize, "MaxSize");
		assignWidget(mMinLum, "MinLum");
		assignWidget(mMaxLum, "MaxLum");

		mBrushType->insertItemAt(0, "Add");
		mBrushType->insertItemAt(1, "Remove");
		mBrushType->setIndexSelected(0);

		mBrushSize->setCaption("50");
		mBrushDensity->setCaption("3");

		mMinSize->setCaption("10");
		mMaxSize->setCaption("15");
		mMinLum->setCaption("1");
		mMaxLum->setCaption("1");

		mAdd->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainVegDlg::notifyAdd);
		mRemove->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainVegDlg::notifyRemove);
		mEdit->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainVegDlg::notifyEdit);
	}

	TerrainVegDlg::~TerrainVegDlg()
	{
		if (mVegInfoDlg)
		{
			mVegInfoDlg->Event_OnOK -= &OnAddVeg;
			mVegInfoDlg->Event_OnOK -= &OnEditVeg;
		}

		safe_delete (mVegInfoDlg);
	}

	void TerrainVegDlg::_UnloadScene(Event * sender)
	{
		mVegList->removeAllItems();
		MForest::Instance()->RemoveAllVegetationBlock();
	}

	void TerrainVegDlg::_AfterLoadScene(Event * sender)
	{
		for (int i = 0; i < MForest::Instance()->GetVegetationCount(); ++i)
		{
			MVegetation * veg = MForest::Instance()->GetVegetation(i);

			mVegList->insertItemAt(i, veg->Name.c_wstr());
		}

		if (Environment::Instance()->GetTerrain() && MForest::Instance()->GetVegetationBlockCount() == 0)
		{
			Terrain * terrain = Environment::Instance()->GetTerrain();

			const Terrain::Config & config = terrain->GetConfig();
			RectF rect = RectF(0, 0, config.xSize, config.zSize);

			MForest::Instance()->CreateVegetationBlocks(rect, config.xSectionCount, config.zSectionCount);
		}
	}

	MVegetation * TerrainVegDlg::GetCurVeg()
	{
		int curSel = mVegList->getIndexSelected();

		if (curSel == -1)
			return NULL;

		return MForest::Instance()->GetVegetation(curSel);
	}

	bool TerrainVegDlg::IsAddMode()
	{
		return mBrushType->getIndexSelected() == 0;
	}

	float TerrainVegDlg::BrushSize()
	{
		TString128 txt = mBrushSize->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	int TerrainVegDlg::BrushDensity()
	{
		TString128 txt = mBrushDensity->getOnlyText().asUTF8_c_str();

		return atoi(txt.c_str());
	}

	float TerrainVegDlg::MinSize()
	{
		TString128 txt = mMinSize->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	float TerrainVegDlg::MaxSize()
	{
		TString128 txt = mMaxSize->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	float TerrainVegDlg::MinLum()
	{
		TString128 txt = mMinLum->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	float TerrainVegDlg::MaxLum()
	{
		TString128 txt = mMaxLum->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	void TerrainVegDlg::_refrushList()
	{
		mVegList->removeAllItems();

		for (int i = 0; i < MForest::Instance()->GetVegetationCount(); ++i)
		{
			MVegetation * veg = MForest::Instance()->GetVegetation(i);

			mVegList->insertItemAt(i, veg->Name.c_wstr());
		}
	}

	void TerrainVegDlg::notifyAdd(MyGUI::Widget * _sender)
	{
		if (Environment::Instance()->GetTerrain() == NULL)
			return ;

		ShowInfoDlg(NULL);
	}

	void TerrainVegDlg::notifyRemove(MyGUI::Widget * _sender)
	{
		MVegetation * veg = GetCurVeg();

		if (veg && Editor::Instance()->MessageBox("Need Delete Vegetation ?", "Warning", MB_OKCANCEL) == IDOK)
		{
			MForest::Instance()->RemoveVegetation(veg);
			_refrushList();
		}
	}

	void TerrainVegDlg::notifyEdit(MyGUI::Widget * _sender)
	{
		if (Environment::Instance()->GetTerrain() == NULL)
			return ;

		MVegetation * veg = GetCurVeg();

		if (veg)
			ShowInfoDlg(veg);
	}

	void TerrainVegDlg::ShowInfoDlg(MVegetation * veg)
	{
		if (mVegInfoDlg == NULL)
		{
			mVegInfoDlg = new TerrainVegInfoDlg();

			mVegInfoDlg->Event_OnOK += &OnAddVeg;
			mVegInfoDlg->Event_OnOK += &OnEditVeg;
		}

		mVegInfoDlg->DoModal(veg);
	}

	void TerrainVegDlg::_OnAddVeg(Event * _sender)
	{
		if (mVegInfoDlg->GetVegetation() != NULL)
			return ;

		MForest::Instance()->AddVegetation(mVegInfoDlg->Name(), mVegInfoDlg->Type(), mVegInfoDlg->MeshFile(),
			mVegInfoDlg->DiffuseMap(), mVegInfoDlg->NormalMap(), mVegInfoDlg->SpecularMap());

		_refrushList();
	}

	void TerrainVegDlg::_OnEditVeg(Event * _sender)
	{
		if (mVegInfoDlg->GetVegetation() == NULL)
			return ;

		MVegetation * veg = mVegInfoDlg->GetVegetation();

		d_assert (veg);

		veg->Type = mVegInfoDlg->Type();

		if (mVegInfoDlg->MeshFile() != "")
			veg->pMesh = MeshManager::Instance()->Load(mVegInfoDlg->MeshFile(), mVegInfoDlg->MeshFile());
		else
			veg->pMesh = NULL;

		veg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(mVegInfoDlg->DiffuseMap(), mVegInfoDlg->DiffuseMap());

		if (mVegInfoDlg->NormalMap() != "")
			veg->NormalMap = VideoBufferManager::Instance()->Load2DTexture(mVegInfoDlg->NormalMap(), mVegInfoDlg->NormalMap());
		else
			veg->NormalMap = NULL;

		if (mVegInfoDlg->SpecularMap() != "")
			veg->SpecularMap = VideoBufferManager::Instance()->Load2DTexture(mVegInfoDlg->SpecularMap(), mVegInfoDlg->SpecularMap());
		else
			veg->SpecularMap = NULL;

		MForest::Instance()->OnVegetationChanged(veg);
	}

}