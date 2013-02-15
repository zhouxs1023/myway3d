#include <windows.h>

#include "MWFMAudioSystemPlugin.h"

using namespace Myway;

FMAudioSystemPlugin _audio;

bool APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch( ul_reason_for_call ) 
    {
    case DLL_PROCESS_ATTACH:
        PluginManager::Instance()->AddPlugin(&_audio);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        ;
    }

    return TRUE;
}