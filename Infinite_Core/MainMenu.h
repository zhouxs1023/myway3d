//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Common\\BaseLayout.h"
#include "PluginDialog.h"

#include "SceneDialog.h"

namespace Infinite {

	class MainMenu : public wraps::BaseLayout
	{
		enum {
			eUnknown,
			eOnLoad,
			eOnSave,
			eOnImport,
			eOnExport,
			eOnNew,
		};

	public:
		MainMenu(MyGUI::Widget * _parent);
		~MainMenu();

	protected:
		void Init();

		void OnMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item);

		void OnLoad();
		void OnSave();
		void OnNew();
		void OnImport();
		void OnExport();

		void _OnDialogOK(Event * sender);

	protected:
		MyGUI::MenuBar* mMainMenu;

		SceneDialog * mSceneDialog;
		tEventListener<MainMenu> OnSceneDialogOK;

		int mMode;
	};

}