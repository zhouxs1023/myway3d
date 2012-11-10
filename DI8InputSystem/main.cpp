#include "MWDIInputSystemPlugin.h"

using namespace Myway;

DIInputSystemPlugin _input;

bool APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch( ul_reason_for_call ) 
    {
    case DLL_PROCESS_ATTACH:
        PluginManager::Instance()->AddPlugin(&_input);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        ;
    }

    return TRUE;
}