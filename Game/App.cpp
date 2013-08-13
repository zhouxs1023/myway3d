#include "stdafx.h"
#include "App.h"
#include "cMainMode.h"

Application::Application()
{
}

Application::~Application()
{
}

bool Application::Init()
{
	SetResourceConfig("game_res.ini");
	SetPluginConfig("game_plugin.ini");

	App_Win32::Init();

	mRenderer = new DeferredRenderer();

	Engine::Instance()->SetRenderScheme(mRenderer);

	mGame.Init();

	mGame.SetMode(new Game::cMainMode());

	return true;
}

void Application::Shutdown()
{
	mGame.Shutdown();


	delete mRenderer;

	App_Win32::Shutdown();
}

void Application::Update()
{
	App_Win32::Update();

	InputSystem::Instance()->Update();

	mGame.Update();
}

void Application::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
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
}


