#pragma once

#include "MWApp_Win32.h"
#include "Client.h"

class App : public App_Win32
{
public:
	App();
	~App();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();
	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

protected:
	void OnGUIRender_(Event * _sender);

protected:
	RenderScheme * mRenderer;
	MGUI_Engine * mUIEngine;

	GmRoot mGame;

	Client * mClient;

	tEventListener<App> OnGUIRender;
};

