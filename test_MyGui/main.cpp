#include "MWApp_Win32.h"
#include "MGUI_System.h"

using namespace Myway;

class MyApp : public App_Win32
{
	MGUI_System mUISystem;

public:
	MyApp()
	{
	}

	~MyApp() {}

	virtual bool Init()
	{
		App_Win32::Init();

		mUISystem.Init();

		return true;
	}

	virtual void Shutdown()
	{
		mUISystem.Shutdown();

		App_Win32::Shutdown();
	}

	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
	{
	}

	virtual void Update()
	{
		App_Win32::Update();

		InputSystem::Instance()->Update();
	}
};


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));

	//BreakAllock(803);

	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	MyApp app;

	app.Run(hInstance);

	return 0;
}