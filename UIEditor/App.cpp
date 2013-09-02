#include "stdafx.h"

#include "App.h"

namespace UIEditor {

App::App()
	: OnGUIRender(RenderEvent::OnRenderGUI2, this, &App::OnGUIRender_)
{
}

App::~App()
{
}

void App::Update()
{
	System::Sleep(10);

	InputSystem::Instance()->Update();

	mUIEngine->InjectMouseEvent();

	mUIEngine->Update();

	App_Win32::Update();
}



bool App::Init()
{
	SetResourceConfig("UIEditor_Res.ini");
	SetPluginConfig("UIEditor_Plugin.ini");

	App_Win32::Init();

	mRenderer = new DeferredRenderer();

	Engine::Instance()->SetRenderScheme(mRenderer);

	mUIEngine = new MGUI_Engine;

	MGUI_Font::Instance()->Load("Editor.Font");

	mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("UIEditor.ShaderLib", "Shaders\\UIEditor.ShaderLib");

	mMainFrame = new MainFrame;

	mSkinRenderer = new SkinRenderer;


	return true;
}

void App::Shutdown()
{
	delete mSkinRenderer;

	delete mMainFrame;

	delete mUIEngine;
	delete mRenderer;

	App_Win32::Shutdown();
}

void App::GetViewSize(int & w, int & h)
{
	w = Engine::Instance()->GetDeviceProperty()->Width;
	h = Engine::Instance()->GetDeviceProperty()->Height;
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

			mUIEngine->Resize();
			mMainFrame->Layout();
		}
	}

	if (mhWnd == hWnd && iMsg == WM_DROPFILES)
	{
		/*TCHAR meshFile[MAX_PATH] = { 0 };
		HDROP hDrop = (HDROP)wParam;
		int len = DragQueryFile(hDrop, 0, meshFile, MAX_PATH);

		if (len > 0)
		{
			RenderWindow::Instance()->OnDragFile(meshFile);
		}*/
	}

	mUIEngine->InjectMessage(iMsg, wParam, lParam);
}

void App::OnGUIRender_(Event * _sender)
{
	mUIEngine->Render();
}

}
