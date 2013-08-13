#pragma once

#include "MWApp_Win32.h"

class Application : public App_Win32
{
public:
	Application();
	~Application();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();
	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

protected:
	RenderScheme * mRenderer;

	GmRoot mGame;
};

