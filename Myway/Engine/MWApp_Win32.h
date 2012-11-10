#pragma once

#include "Engine.h"

#ifdef MW_PLATFORM_WIN32

namespace Myway
{

class MW_ENTRY App_Win32
{
    DECLARE_SINGLETON(App_Win32);

public:
    App_Win32();
    virtual ~App_Win32();

    virtual void OnPreCreateWindow(WNDCLASS & wc, LPTSTR tile, DWORD & style, POINT & pt, SIZE & sz);
    virtual void OnPostCreateWindow();

    virtual bool Init();
    virtual void Update();
    virtual void Shutdown();
    virtual bool Quit();

    virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

    HINSTANCE _hInstance() { return mhInst; }
    HWND _hWnd() { return mhWnd; }

    void Run(HINSTANCE hInstance);

protected:
    HINSTANCE mhInst;
    HWND mhWnd;
};

}


#define DECLARE_APPLICATION(classname) classname gAppWin32;

#endif