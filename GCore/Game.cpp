#include "stdafx.h"
#include "Game.h"

namespace Game {

Game * Game::msInstance = NULL;

Game::Game()
	: mCurrentMode(NULL)
	, mShaderLib(NULL)
{
	msInstance = this;
}

Game::~Game()
{
	msInstance = NULL;
}

void Game::Init()
{
	mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("GameShaderLib", "Game.ShaderLib");
	d_assert (mShaderLib);

	mUIUtil.Init();

	mCurrentMode = NULL;
}

void Game::Shutdown()
{
	mUIUtil.Shutdown();

	if (mCurrentMode)
	{
		mCurrentMode->Shutdown();
		delete mCurrentMode;
	}
}

void Game::Update()
{
	if (mCurrentMode)
		mCurrentMode->Update(Engine::Instance()->GetFrameTime());
}

void Game::SetMode(IGameMode * mode)
{
	d_assert (mode != NULL);

	if (mCurrentMode != NULL)
		mCurrentMode->Shutdown();

	safe_delete (mCurrentMode);

	mCurrentMode = mode;

	mCurrentMode->Init();
}

IGameMode * Game::GetMode()
{
	return mCurrentMode;
}


}