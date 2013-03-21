#include "stdafx.h"

#include "TerrainCreateDlg.h"

namespace Infinite {

	TerrainCreateDlg::TerrainCreateDlg()
		: BaseLayout("TerrainCreate.layout")
	{
		assignWidget(mXSize, "xSize");
		assignWidget(mZSize, "zSize");
		assignWidget(mXVertSize, "xVertSize");
		assignWidget(mZVertSize, "zVertSize");
		assignWidget(mOK, "Ok");
		assignWidget(mCancel, "Cancel");

		OnInit();

		mOK->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainCreateDlg::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &TerrainCreateDlg::OnCancel);
	}

	TerrainCreateDlg::~TerrainCreateDlg()
	{
	}

	void TerrainCreateDlg::OnInit()
	{
		mXVertSize->insertItemAt(0, "256");
		mXVertSize->insertItemAt(1, "512");
		mXVertSize->insertItemAt(2, "1024");
		mXVertSize->insertItemAt(3, "2048");
		mXVertSize->insertItemAt(4, "4096");

		mZVertSize->insertItemAt(0, "256");
		mZVertSize->insertItemAt(1, "512");
		mZVertSize->insertItemAt(2, "1024");
		mZVertSize->insertItemAt(3, "2048");
		mZVertSize->insertItemAt(4, "4096");

		mXVertSize->setIndexSelected(2);
		mZVertSize->setIndexSelected(2);
	}

	float TerrainCreateDlg::GetXSize()
	{
		TString128 txt = mXSize->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	float TerrainCreateDlg::GetZSize()
	{
		TString128 txt = mZSize->getOnlyText().asUTF8_c_str();

		return (float)atof(txt.c_str());
	}

	int TerrainCreateDlg::GetXVertSize()
	{
		int isel = mXVertSize->getIndexSelected();

		d_assert (isel >= 0);

		TString128 txt = mXVertSize->getItemNameAt(isel).asUTF8_c_str();

		return atoi(txt.c_str());
	}

	int TerrainCreateDlg::GetZVertSize()
	{
		int isel = mZVertSize->getIndexSelected();

		d_assert (isel >= 0);

		TString128 txt = mZVertSize->getItemNameAt(isel).asUTF8_c_str();

		return atoi(txt.c_str());
	}

	void TerrainCreateDlg::DoModal()
	{
		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	void TerrainCreateDlg::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void TerrainCreateDlg::OnOK(MyGUI::Widget *)
	{
		Event_OnOK();
		EndDialog();
	}

	void TerrainCreateDlg::OnCancel(MyGUI::Widget *)
	{
		Event_OnCancel();
		EndDialog();
	}


}
