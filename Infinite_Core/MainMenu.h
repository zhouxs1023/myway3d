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

#include "SceneDialog.h"

namespace Infinite {

	class MainMenu : public wraps::BaseLayout
	{
		enum {
			eUnknown,
			eOnLoad,
			eOnSave,
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
		void OnExport();
		void OnNew();

		void _OnDialogOK(Event * sender);

	protected:
		MyGUI::MenuBar* mMainMenu;

		SceneDialog * mSceneDialog;
		tEventListener<MainMenu> OnSceneDialogOK;

		int mMode;
	};

}