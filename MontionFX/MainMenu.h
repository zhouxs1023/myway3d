//////////////////////////////////////////////////////////////////////////
//
// MotionFX: Motion Tool For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "Common\\BaseLayout.h"

namespace MotionFX {

	class MainMenu : public wraps::BaseLayout
	{
		enum {
			eUnknown,
			eOnLoad,
			eOnSave,
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

	protected:
		MyGUI::MenuBar* mMainMenu;

		int mMode;
	};

}