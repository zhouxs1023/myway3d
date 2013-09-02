#include "stdafx.h"

#include "Plugin.h"
#include "Npc.h"
#include "Helper.h"
#include "EditorExport.h"

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
		EditorExport exp;

		exp.Export(filename);
	}

	void GamePlugin::_OnInit(Event * _sender)
	{
		ShapeManager::Instance()->AddFactory(new NpcFactory);
		ShapeManager::Instance()->AddFactory(new HelperFactory);

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