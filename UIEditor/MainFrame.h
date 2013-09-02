#pragma once

#include "MainMenu.h"
#include "MainStatusBar.h"
#include "LookFeelEditor.h"
#include "AllLookFeel.h"
#include "ColorDialog.h"

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
		LookFeelEditor * mLookFeelEditor;

		ColorDialog * mColorDialog;
	};

}