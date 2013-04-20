#include "stdafx.h"

#include "GMain.h"

namespace game {

	GMain::GMain()
	{
	}

	GMain::~GMain()
	{
	}

	void GMain::Init()
	{
		mDataManager.Init();
		mWorld.Init();
	}

	void GMain::Shutdown()
	{
		mWorld.Shutdown();
		mDataManager.Shutdown();
	}
}