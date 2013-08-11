#pragma once

namespace UIEditor {

	class MainMenu
	{
	public:
		MainMenu();
		~MainMenu();

		void Layout();

	protected:
		MGUI_Layout * mLayout;
		MGUI_Button * mButton_LookFeelFile;
	};

}