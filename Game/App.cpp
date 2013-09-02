#include "stdafx.h"
#include "App.h"
#include "MainMode.h"
#include "LoginMode.h"
#include "SelectRoleMode.h"

App::App()
	: OnGUIRender(RenderEvent::OnRenderGUI2, this, &App::OnGUIRender_)
{
}

App::~App()
{
}

bool App::Init()
{
	SetResourceConfig("game_res.ini");
	SetPluginConfig("game_plugin.ini");

	App_Win32::Init();

	mRenderer = new DeferredRenderer();

	Engine::Instance()->SetRenderScheme(mRenderer);

	mUIEngine = new MGUI_Engine;

	MGUI_Font::Instance()->Load("Game.Font");

	mClient = new Client;

	mGame.Init();

	mGame.SetMode(new cSelectRoleMode);

	return true;
}

void App::Shutdown()
{
	delete mClient;

	mGame.Shutdown();

	delete mUIEngine;

	delete mRenderer;

	App_Win32::Shutdown();
}

void App::Update()
{
	InputSystem::Instance()->Update();

	mUIEngine->InjectMouseEvent();

	if (mUIEngine->GetMouseFocusedWidget() == NULL)
	{
		mGame.GetMode()->ProcessMouseEvent();
	}

	if (mUIEngine->GetKeyFocusedWidget() == NULL)
	{
		mGame.GetMode()->ProcessKeyEvent();
	}

	mUIEngine->Update();

	mClient->Update();

	mGame.Update();

	App_Win32::Update();
}

void App::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
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

	mUIEngine->InjectKeyEvent(iMsg, wParam, lParam);
}

void App::OnGUIRender_(Event * _sender)
{
	mUIEngine->Render();
}