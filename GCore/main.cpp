#include "stdafx.h"

#include "GApp.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));

	//BreakAllock(803);

	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	game::GApp app;

	app.Run(hInstance);

	return 0;
}