#pragma once

#include "MWApp_Win32.h"
#include "GameMode.h"

class GCORE_ENTRY Game : public App_Win32
{
public:
	Game();
	~Game();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();
	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

	void SetMode(IGameMode * mode);

protected:
	MGUI_System mUISystem;
	RenderScheme * mRenderer;

	IGameMode * mCurrentMode;
};