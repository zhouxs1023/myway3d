#include "stdafx.h"
#include "plugin.h"

xmdl::t_plugin plugin;

bool APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch( ul_reason_for_call ) 
	{
	case DLL_PROCESS_ATTACH:
		Infinite::Editor::Instance()->AddPlugin(&plugin);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
