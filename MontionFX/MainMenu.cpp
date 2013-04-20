#include "stdafx.h"

#include "MainMenu.h"

namespace MotionFX {

	MainMenu::MainMenu(MyGUI::Widget * _parent)
		: BaseLayout("MainMenu.layout", _parent)
	{
		assignWidget(mMainMenu, "MainMenu");

		Init();

		mMainMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &MainMenu::OnMenuCtrlAccept);

		mMode = eUnknown;
	}

	MainMenu::~MainMenu()
	{
		mMainMenu->eventMenuCtrlAccept -= MyGUI::newDelegate(this, &MainMenu::OnMenuCtrlAccept);
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
		}
	}


	void MainMenu::OnMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
	{
		const std::string& id = _item->getItemId();

		if (id == "CTRL_Menu_Load")
			OnLoad();
		else if (id == "CTRL_Menu_Save")
			OnSave();
		else if (id == "CTRL_Menu_New")
			OnNew();
	}

	void MainMenu::OnLoad()
	{
		mMode = eOnLoad;
	}

	void MainMenu::OnSave()
	{
		mMode = eOnSave;
	}

	void MainMenu::OnNew()
	{
		mMode = eOnNew;
	}

}