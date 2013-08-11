#pragma once

#include "SkinRenderer.h"
#include "MWApp_Win32.h"
#include "MainFrame.h"

namespace UIEditor {

class App : public App_Win32
{
public:
	static App * Instance() { return (App*)App_Win32::Instance(); }

public:
	App();
	virtual ~App();

	virtual bool Init();
	virtual void Shutdown();
	virtual void Update();
	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

	void GetViewSize(int & w, int & h);

	ShaderLib * GetShaderLib() { return mShaderLib; }

protected:
	void OnGUIRender_(Event * _sender);

protected:
	DeferredRenderer * mRenderer;
	MGUI_Engine * mUIEngine;
	ShaderLib * mShaderLib;
	SkinRenderer * mSkinRenderer;
	MainFrame * mMainFrame;

	tEventListener<App> OnGUIRender;
};

}
