#include "stdafx.h"

#include "TerrainLayerInfoDlg.h"
#include "Infinite.h"

namespace Infinite {

	TerrainLayerInfoDlg::TerrainLayerInfoDlg()
		: BaseLayout("TerrainLayerInfo.layout")
		, mLayer(NULL)
	{
		assignWidget(mOk, "Ok");
		assignWidget(mCancel, "Cancel");

		assignWidget(mDiffuseMap, "DiffuseMap");
		assignWidget(mNormalMap, "NormalMap");
		assignWidget(mSpecularMap, "SpecularMap");
		assignWidget(mUVScale, "UVScale");
		assignWidget(mMaterialId, "MaterialId");

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainLayerInfoDlg::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainLayerInfoDlg::OnCancel);
	}

	TerrainLayerInfoDlg::~TerrainLayerInfoDlg()
	{
	}

	void TerrainLayerInfoDlg::DoModal(const Terrain::Layer * layer)
	{
		mLayer = layer;

		if (mLayer)
		{
			mDiffuseMap->setCaption(layer->detail.c_wstr());
			mNormalMap->setCaption(layer->normal.c_wstr());
			mSpecularMap->setCaption(layer->specular.c_wstr());

			mUVScale->setCaption(Utils::ToString(layer->scale));
		}
		else
		{
			mDiffuseMap->setCaption("");
			mNormalMap->setCaption("");
			mSpecularMap->setCaption("");
			mUVScale->setCaption("100");
		}

		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	TString128 TerrainLayerInfoDlg::GetDiffuseMap()
	{
		return mDiffuseMap->getOnlyText().asUTF8_c_str();
	}

	TString128 TerrainLayerInfoDlg::GetNormalMap()
	{
		return mNormalMap->getOnlyText().asUTF8_c_str();
	}

	TString128 TerrainLayerInfoDlg::GetSpecularMap()
	{
		return mSpecularMap->getOnlyText().asUTF8_c_str();
	}

	float TerrainLayerInfoDlg::GetUVScale()
	{
		TString128 txt = mUVScale->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	void TerrainLayerInfoDlg::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void TerrainLayerInfoDlg::OnOK(MyGUI::Widget * _sender)
	{
		Event_OnOK();
		EndDialog();
	}

	void TerrainLayerInfoDlg::OnCancel(MyGUI::Widget * _sender)
	{
		EndDialog();
	}

}