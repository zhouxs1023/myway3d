#include "stdafx.h"

static int controlsInit = FALSE;
static HINSTANCE ghInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	ghInstance = hinstDLL;
	if ( !controlsInit )
	{
		controlsInit = TRUE;
		
		// jaguar controls
		InitCustomControls(ghInstance);

		// initialize Chicago controls
		InitCommonControls();
	}

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			if (FAILED(CoInitialize(NULL)))
				return FALSE;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
	}

	return(TRUE);
}