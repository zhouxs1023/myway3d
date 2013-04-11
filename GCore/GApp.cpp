#include "stdafx.h"

#include "GApp.h"

namespace game {

	GApp::GApp()
	{
	}

	GApp::~GApp()
	{
	}

	bool GApp::Init()
	{
		SetResourceConfig("game_res.ini");
		SetPluginConfig("game_plugin.ini");

		App_Win32::Init();

		mRenderer = new DeferredRenderer();

		Engine::Instance()->SetRenderScheme(mRenderer);

		mUISystem.Init();

		mCurGameMode = NULL;

		//bool hr = MyGUI::ResourceManager::getInstance().load("Editor.xml"); d_assert (hr);

		return true;
	}

	void GApp::Shutdown()
	{
		for (int i = 0; i < mGameModes.Size(); ++i)
			delete mGameModes[i];

		mGameModes.Clear();

		mUISystem.Shutdown();

		delete mRenderer;

		App_Win32::Shutdown();
	}

	void GApp::Update()
	{
		App_Win32::Update();

		InputSystem::Instance()->Update();

		mUISystem.InjectMouseEvent();

		if (mCurGameMode)
			mCurGameMode->OnRun();
	}

	void GApp::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
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

	void GApp::EntryGameMode(int type)
	{
		if (mCurGameMode)
		{
			d_assert (mCurGameMode->GetType() != type);

			mCurGameMode->OnShutdown();
		}

		mCurGameMode = NULL;

		for (int i = 0; i < mGameModes.Size(); ++i)
		{
			if (mGameModes[i]->GetType() == type)
			{
				mCurGameMode = mGameModes[i];
				break;
			}
		}

		d_assert (mCurGameMode != NULL);

		mCurGameMode->OnInit();
	}

	void GApp::AddGameMode(GMode * mode)
	{
#ifdef _DEBUG
		for (int i = 0; i < mGameModes.Size(); ++i)
		{
			d_assert(mGameModes[i]->GetType() != mode->GetType());
		}
#endif

		mGameModes.PushBack(mode);
	}
}