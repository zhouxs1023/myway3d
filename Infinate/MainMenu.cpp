#include "stdafx.h"

#include "MainMenu.h"
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
		else if (id == "CTRL_Menu_Export")
			OnExport();
		else if (id == "CTRL_Menu_New")
			OnNew();
	}

	void MainMenu::OnLoad()
	{
		mMode = eOnLoad;
		mSceneDialog->DoModel();
	}

	void MainMenu::OnSave()
	{
		mMode = eOnSave;
	}

	void MainMenu::OnExport()
	{
		mMode = eOnExport;
	}

	void MainMenu::OnNew()
	{
		mMode = eOnNew;
	}

	void MainMenu::_OnDialogOK(Event * sender)
	{
		if (mMode == eOnLoad)
		{
			xScene::Instance()->Load(mSceneDialog->GetSceneName(), mSceneDialog->GetSceneFloder());
		}
	}
}