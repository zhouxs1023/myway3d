#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Init()
{
	SetResourceConfig("game_res.ini");
	SetPluginConfig("game_plugin.ini");

	App_Win32::Init();

	mRenderer = new DeferredRenderer();

	Engine::Instance()->SetRenderScheme(mRenderer);

	mUISystem.Init();

	mCurrentMode = NULL;

	//bool hr = MyGUI::ResourceManager::getInstance().load("Editor.xml"); d_assert (hr);

	return true;
}

void Game::Shutdown()
{
	mCurrentMode->Shutdown();

	delete mCurrentMode;

	mUISystem.Shutdown();

	delete mRenderer;

	App_Win32::Shutdown();
}

void Game::Update()
{
	App_Win32::Update();

	InputSystem::Instance()->Update();

	mUISystem.InjectMouseEvent();

	if (mCurrentMode)
		mCurrentMode->Update(Engine::Instance()->GetFrameTime());
}

void Game::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	if (mhWnd == hWnd && iMsg == WM_SIZE)
	{
		if (wParam != SIZE_MINIMIZED)
		{
			RECT rc;

			GetClientRect(hWnd, &rc);

			Engine::Instance()->Resize(rc.right - rc.left, rc.bottom - rc.top);
			mRenderer->Resize(rc.right - rc.left, rc.bottom - rc.top);
		}
	}

	mUISystem.InjectKeyEvent(iMsg, wParam, lParam);
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

