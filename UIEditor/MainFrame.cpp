#include "stdafx.h"
#include "MainFrame.h"

namespace UIEditor {

	MainFrame::MainFrame()
	{
		mAllLookFeel = new AllLookFeel;
		mMainMenu = new MainMenu;
		mMainStatusBar = new MainStatusBar;

		mLookFeelEditor= new LookFeelEditor;

		mColorDialog = new ColorDialog;

		Layout();
	}

	MainFrame::~MainFrame()
	{
		delete mColorDialog;

		delete mLookFeelEditor;

		delete mMainMenu;
		delete mMainStatusBar;
		delete mAllLookFeel;
	}

	void MainFrame::Layout()
	{
		mMainMenu->Layout();
		mMainStatusBar->Layout();

		mLookFeelEditor->Layout();
	}


}