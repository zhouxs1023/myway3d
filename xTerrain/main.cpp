#include "stdafx.h"

HANDLE ghModule;

bool APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call, LPVOID lpReserved)
{
	ghModule = hModule;

	switch( ul_reason_for_call ) 
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		;
	}
	return TRUE;
}
