#include "stdafx.h"

#include "Plugin.h"
#include "Npc.h"

namespace game {

	GamePlugin::GamePlugin()
		: OnInit(xEvent::OnInit, this, &GamePlugin::_OnInit)
		, OnShutown(xEvent::OnShutdown, this, &GamePlugin::_OnShutdown)
	{
	}

	GamePlugin::~GamePlugin()
	{
	}

	void GamePlugin::Export(const char * filename)
	{
	}

	void GamePlugin::_OnInit(Event * _sender)
	{
		ShapeManager::Instance()->AddFactory(new NpcFactory);

		mGameMain.Init();
	}

	void GamePlugin::_OnShutdown(Event * _sender)
	{
		mGameMain.Shutdown();
	}
	
}