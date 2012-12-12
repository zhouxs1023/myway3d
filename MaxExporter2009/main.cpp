#include "stdafx.h"

int controlsInit = FALSE;
HINSTANCE ghInstance;
//
//CWinApp gApp;
//
//BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
//{
//	ghInstance = hinstDLL;
//	if ( !controlsInit )
//	{
//		controlsInit = TRUE;
//		
//		// jaguar controls
//		InitCustomControls(ghInstance);
//
//		// initialize Chicago controls
//		InitCommonControls();
//	}
//
//	switch(fdwReason)
//	{
//		case DLL_PROCESS_ATTACH:
//			
//			break;
//		case DLL_THREAD_ATTACH:
//			break;
//		case DLL_THREAD_DETACH:
//			break;
//		case DLL_PROCESS_DETACH:
//			break;
//	}
//
//	return(TRUE);
//}


class MyDll: public CWinApp
{
public:
    /* do I need constructor and destructor here? */
    virtual BOOL InitInstance();
    virtual BOOL ExitInstance();
} theDll;

BOOL MyDll::InitInstance()
{
    CWinApp::InitInstance();
    /* code from old DllMain, DLL_PROCESS_ATTACH. 
       For hInst use theDll.m_hInstance */

	InitCustomControls(m_hInstance);
	InitCommonControls();
	if (FAILED(CoInitialize(NULL)))
		return FALSE;

    return TRUE;
}

BOOL MyDll::ExitInstance()
{
    /* code from old DllMain, DLL_PROCESS_DETACH */
    return CWinApp::ExitInstance();
}