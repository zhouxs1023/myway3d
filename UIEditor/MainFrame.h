#pragma once

#include "MainMenu.h"
#include "MainStatusBar.h"
#include "MainLookFeelPanel.h"
#include "AllLookFeel.h"

namespace UIEditor {

	class MainFrame
	{
	public:
		MainFrame();
		~MainFrame();

		void Layout();

	protected:
		AllLookFeel * mAllLookFeel;
		MainMenu * mMainMenu;
		MainStatusBar * mMainStatusBar;
		MainLookFeelPanel * mMainLookFeelPanel;
	};

}