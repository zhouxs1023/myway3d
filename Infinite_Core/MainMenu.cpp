#include "stdafx.h"

#include "MainMenu.h"
#include "Editor.h"
#include "xScene.h"

namespace Infinite {

	MainMenu::MainMenu(MyGUI::Widget * _parent)
		: BaseLayout("MainMenu.layout", _parent)
		, OnSceneDialogOK(this, &MainMenu::_OnDialogOK)
	{
		assignWidget(mMainMenu, "MainMenu");

		Init();

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenu::OnMenuCtrlAccept);

		mSceneDialog = new SceneDialog();

		mSceneDialog->Event_OnOK += &OnSceneDialogOK;

		mMode = eUnknown;
	}

	MainMenu::~MainMenu()
	{
		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenu::OnMenuCtrlAccept);

		mSceneDialog->Event_OnOK -= &OnSceneDialogOK;
		delete mSceneDialog;
	}

	void MainMenu::Init()
	{
		// scene
		{
			MyGUI::MenuItem* menuScene = mMainMenu->findItemById("Scene", true); d_assert (menuScene);

			MyGUI::MenuControl * ctrl = menuScene->getItemChild();

			if (ctrl == NULL)
				ctrl = menuScene->createItemChild();

			ctrl->addItem("Load ...",  MyGUI::MenuItemType::Normal, "CTRL_Menu_Load");
			ctrl->addItem("New",  MyGUI::MenuItemType::Normal, "CTRL_Menu_New");
			ctrl->addItem("Save",  MyGUI::MenuItemType::Normal, "CTRL_Menu_Save");
			ctrl->addItem("Import",  MyGUI::MenuItemType::Normal, "CTRL_Menu_Import");
			ctrl->addItem("Export",  MyGUI::MenuItemType::Normal, "CTRL_Menu_Export");
		}
	}


	void MainMenu::OnMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
	{
		const std::string& id = _item->getItemId();

		if (id == "CTRL_Menu_Load")
			OnLoad();
		else if (id == "CTRL_Menu_Save")
			OnSave();
		else if (id == "CTRL_Menu_Import")
			OnImport();
		else if (id == "CTRL_Menu_Export")
			OnExport();
		else if (id == "CTRL_Menu_New")
			OnNew();
	}

	void MainMenu::OnLoad()
	{
		mMode = eOnLoad;
		mSceneDialog->DoModal();
	}

	void MainMenu::OnSave()
	{
		mMode = eOnSave;
		
		if (xScene::Instance()->IsInited())
		{
			xScene::Instance()->Save();
		}
	}

	void MainMenu::OnImport()
	{
		mMode = eOnImport;

		if (!xScene::Instance()->IsInited())
		{
			mSceneDialog->DoModal();
		}
	}

	void MainMenu::OnExport()
	{
		mMode = eOnExport;

		if (xScene::Instance()->IsInited())
		{
			xScene::Instance()->Export();
		}
	}

	void MainMenu::OnNew()
	{
		mMode = eOnNew;

		if (!xScene::Instance()->IsInited())
		{
			mSceneDialog->DoModal();
		}
	}

	void MainMenu::_OnDialogOK(Event * sender)
	{
		if (mMode == eOnLoad)
		{
			xScene::Instance()->Load(mSceneDialog->GetSceneName().c_str(), mSceneDialog->GetSceneFloder().c_str());
		}

		else if (mMode == eOnNew)
		{
			if (!ResourceManager::Instance()->Exist(mSceneDialog->GetSceneName().c_str()))
			{
				xScene::Instance()->New(mSceneDialog->GetSceneName().c_str(), mSceneDialog->GetSceneFloder().c_str());
			}
			else
			{
				MMessageBox::Instance()->DoModal("Scene File Exit!", "Error");
			}
		}
	}

}