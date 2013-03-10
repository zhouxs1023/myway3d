#include "stdafx.h"
#include "SceneDialog.h"

namespace Infinite {

	SceneDialog::SceneDialog()
		: BaseLayout("SceneDialog.layout")
	{
		assignWidget(mFloderList, "ID_FLODER_LIST");
		assignWidget(mSceneList, "ID_SCENE_LIST");
		assignWidget(mSceneName, "ID_SCENE_NAME");
		assignWidget(mOK, "ID_OK");
		assignWidget(mCancel, "ID_CANCEL");

		OnInit();

		mFloderList->eventListChangePosition += MyGUI::newDelegate(this, &SceneDialog::OnFloderListSelChanged);
		mSceneList->eventListChangePosition += MyGUI::newDelegate(this, &SceneDialog::OnSceneListSelChanged);
		mOK->eventMouseButtonClick += MyGUI::newDelegate(this, &SceneDialog::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &SceneDialog::OnOK);
	}

	SceneDialog::~SceneDialog()
	{
	}

	void SceneDialog::DoModel()
	{
		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	const char * SceneDialog::GetSceneFloder()
	{
		size_t index = mFloderList->getIndexSelected();

		if (index == MyGUI::ITEM_NONE)
			return "";

		return mFloderList->getItemNameAt(index).asUTF8_c_str();
	}

	const char * SceneDialog::GetSceneName()
	{
		return mSceneName->getCaption().asUTF8_c_str();
	}

	void SceneDialog::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void SceneDialog::OnInit()
	{
		mMainWidget->setVisible(false);

		ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

		ResourceGroup::ArchiveVisitor v = rg->GetArchives();

		int index = 0;
		while (!v.Endof())
		{
			const TString128 & floder = (*v.Cursor())->GetName();

			mFloderList->insertItemAt(index, floder.c_str());

			++v;
		}
	}

	void SceneDialog::OnFloderListSelChanged(MyGUI::ListBox* _sender, size_t _index)
	{
		const MyGUI::UString & _floder = mFloderList->getItemNameAt(_index);

		mSceneList->removeAllItems();

		ResourceGroup * rg = ResourceManager::Instance()->GetResourceGroup();

		Archive * ar = rg->GetArchive(_floder.asUTF8_c_str());

		Archive::FileInfoVisitor v = ar->GetFileInfos();

		while (!v.eof())
		{
			const TString128 & filename = v.Cursor()->second.name;

			if (File::GetExternName(filename) == "scene")
			{
				mSceneList->addItem(filename.c_str());
			}

			++v;
		}

		mSceneName->setCaption("");
	}

	void SceneDialog::OnSceneListSelChanged(MyGUI::ListBox* _sender, size_t _index)
	{
		const MyGUI::UString & _sceneName = mSceneList->getItemNameAt(_index);

		mSceneName->setCaption(_sceneName);
	}

	void SceneDialog::OnOK(MyGUI::Widget *)
	{
		Event_OnOK();
		EndDialog();
	}

	void SceneDialog::OnCancel(MyGUI::Widget *)
	{
		EndDialog();
	}
}