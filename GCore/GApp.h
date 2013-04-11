#pragma once

#include "MWApp_Win32.h"
#include "GMode.h"

namespace game {

	class GApp : public App_Win32
	{
	public:
		GApp();
		virtual ~GApp();

		virtual bool Init();
		virtual void Shutdown();
		virtual void Update();
		virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

		void EntryGameMode(int type);
		void AddGameMode(GMode * mode);

	protected:
		MGUI_System mUISystem;
		RenderScheme * mRenderer;

		GMode * mCurGameMode;
		Array<GMode*> mGameModes;
	};
}