#include "stdafx.h"

#include "App.h"

namespace Infinite
{

	App::App()
	{
	}

	App::~App()
	{
	}

	bool App::Init()
	{
		SetResourceConfig("infi_res.ini");
		SetPluginConfig("infi_plugin.ini");

		App_Win32::Init();

		mRenderer = new DeferredRenderer();

		Engine::Instance()->SetRenderScheme(mRenderer);

		mUISystem.Init();

		bool hr = MyGUI::ResourceManager::getInstance().load("Editor.xml"); d_assert (hr);

		mPointManager.createPointerManager((size_t)mhWnd);

		mBackgroundPane = new BackgroundPane();
		mMainPane = new MainPane();

		mEditor.Init();

		return true;
	}

	void App::Shutdown()
	{
		mEditor.Shutdown();
		delete mMainPane;
		delete mBackgroundPane;

		mPointManager.destroyPointerManager();

		mUISystem.Shutdown();

		delete mRenderer;

		App_Win32::Shutdown();
	}

	void App::Update()
	{
		App_Win32::Update();

		InputSystem::Instance()->Update();

		mUISystem.InjectMouseEvent();
	}

	void App::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
	{
		if (mhWnd == hWnd && iMsg == WM_SIZE)
		{
			if (wParam != SIZE_MINIMIZED)
			{
				RECT rc;

				GetClientRect(hWnd, &rc);

				RenderWindow::Instance()->Shutdown();
				Engine::Instance()->Resize(rc.right - rc.left, rc.bottom - rc.top);
				RenderWindow::Instance()->Init();
			}
		}

		mUISystem.InjectKeyEvent(iMsg, wParam, lParam);
	}


}