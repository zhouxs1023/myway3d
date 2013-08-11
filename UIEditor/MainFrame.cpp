#include "stdafx.h"
#include "MainFrame.h"

namespace UIEditor {

	MainFrame::MainFrame()
	{
		mAllLookFeel = new AllLookFeel;
		mMainMenu = new MainMenu;
		mMainLookFeelPanel = new MainLookFeelPanel;
		mMainStatusBar = new MainStatusBar;

		Layout();
	}

	MainFrame::~MainFrame()
	{
		delete mMainMenu;
		delete mMainLookFeelPanel;
		delete mMainStatusBar;
		delete mAllLookFeel;
	}

	void MainFrame::Layout()
	{
		mMainMenu->Layout();
		mMainLookFeelPanel->Layout();
		mMainStatusBar->Layout();
	}


}