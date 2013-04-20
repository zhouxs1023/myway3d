#include "stdafx.h"

#include "MainPane.h"

namespace MotionFX {

	MainPane::MainPane()
		: wraps::BaseLayout("MainPane.layout")
	{
		assignBase(mMainMenu, "MainMenuControl");
		assignBase(mWorkSpace, "MainWorkspaceControl");
	}

	MainPane::~MainPane()
	{
	}
}

