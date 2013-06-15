#include "stdafx.h"

#include "Plugin.h"
#include "Npc.h"

namespace game {

	GamePlugin::GamePlugin()
		: OnInit(xEvent::OnInit, this, &GamePlugin::_OnInit)
		, OnShutown(xEvent::OnShutdown, this, &GamePlugin::_OnShutdown)
		, OnUpdate(xEvent::OnUpdate, this, &GamePlugin::_OnUpdate)
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

		mGameMain = new EditorGameMode();

		mGameMain->Init();
	}

	void GamePlugin::_OnShutdown(Event * _sender)
	{
		mGameMain->Shutdown();

		delete mGameMain;
	}
	
	void GamePlugin::_OnUpdate(Event * _sender)
	{
		mGameMain->Update(Engine::Instance()->GetFrameTime());
	}
}