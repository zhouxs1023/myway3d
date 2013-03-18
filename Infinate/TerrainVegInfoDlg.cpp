#include "stdafx.h"

#include "TerrainVegInfoDlg.h"

namespace Infinite {

	TerrainVegInfoDlg::TerrainVegInfoDlg()
		: BaseLayout("TerrainVegInfo.layout")
		, mVegetation(NULL)
	{
		assignWidget(mName, "Name");
		assignWidget(mType, "Type");
		assignWidget(mMesh, "Mesh");
		assignWidget(mDiffuseMap, "DiffuseMap");
		assignWidget(mNormalMap, "NormalMap");
		assignWidget(mSpecularMap, "SpecularMap");
		assignWidget(mOk, "Ok");
		assignWidget(mCancel, "Cancel");

		mType->insertItemAt(0, "Billboard");
		mType->insertItemAt(1, "X2");
		mType->insertItemAt(2, "Mesh");
		mType->setIndexSelected(1);

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainVegInfoDlg::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainVegInfoDlg::OnCancel);
	}

	TerrainVegInfoDlg::~TerrainVegInfoDlg()
	{
	}

	TString32 TerrainVegInfoDlg::Name()
	{
		return mName->getOnlyText().asUTF8_c_str();
	}

	MVegetation::GeomType TerrainVegInfoDlg::Type()
	{
		switch (mType->getIndexSelected())
		{
		case 0:
			return MVegetation::GT_BillBoard;

		case 1:
			return MVegetation::GT_X2;

		case 2:
			return MVegetation::GT_Mesh;

		default:
			d_assert (0);
			return MVegetation::GT_X2;
		}
	}

	TString128 TerrainVegInfoDlg::MeshFile()
	{
		return mMesh->getOnlyText().asUTF8_c_str();
	}

	TString128 TerrainVegInfoDlg::DiffuseMap()
	{
		return mDiffuseMap->getOnlyText().asUTF8_c_str();
	}

	TString128 TerrainVegInfoDlg::SpecularMap()
	{
		return mSpecularMap->getOnlyText().asUTF8_c_str();
	}

	TString128 TerrainVegInfoDlg::NormalMap()
	{
		return mNormalMap->getOnlyText().asUTF8_c_str();
	}

	void TerrainVegInfoDlg::DoModal(MVegetation * veg)
	{
		mVegetation = veg;

		if (veg)
		{
			mName->setCaption(veg->Name.c_wstr());

			if (veg->pMesh != NULL)
				mMesh->setCaption(veg->pMesh->GetSourceName().c_wstr());
			else
				mMesh->setCaption("");

			if (veg->DiffuseMap != NULL)
				mDiffuseMap->setCaption(veg->DiffuseMap->GetSourceName().c_wstr());
			else
				mDiffuseMap->setCaption("");

			if (veg->NormalMap != NULL)
				mNormalMap->setCaption(veg->NormalMap->GetSourceName().c_wstr());
			else
				mNormalMap->setCaption("");

			if (veg->SpecularMap != NULL)
				mSpecularMap->setCaption(veg->SpecularMap->GetSourceName().c_wstr());
			else
				mSpecularMap->setCaption("");

			switch (veg->Type)
			{
			case MVegetation::GT_BillBoard:
				mType->setIndexSelected(0);
				break;

			case MVegetation::GT_X2:
				mType->setIndexSelected(1);
				break;

			case MVegetation::GT_Mesh:
				mType->setIndexSelected(2);
				break;
			}

			mName->setEnabled(false);
		}
		else
		{
			mName->setCaption("");
			mType->setIndexSelected(1);
			mMesh->setCaption("");
			mDiffuseMap->setCaption("");
			mNormalMap->setCaption("");
			mSpecularMap->setCaption("");

			mName->setEnabled(true);
		}

		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	void TerrainVegInfoDlg::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void TerrainVegInfoDlg::OnOK(MyGUI::Widget * _sender)
	{
		Event_OnOK();
		EndDialog();
	}

	void TerrainVegInfoDlg::OnCancel(MyGUI::Widget * _sender)
	{
		EndDialog();
	}
}