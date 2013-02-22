#include "MWApp_Win32.h"

using namespace Myway;

IMP_SLN(App_Win32);
App_Win32::App_Win32()
{
    INIT_SLN;

    mhInst = NULL;
    mhWnd = NULL;
}

App_Win32::~App_Win32()
{
    SHUT_SLN;
}

void App_Win32::OnPreCreateWindow(WNDCLASS & wc, LPTSTR tile, DWORD & style, POINT & pt, SIZE & sz)
{
}

void App_Win32::OnPostCreateWindow()
{
}

bool App_Win32::Init()
{
    Engine * engine = new Engine();

    HWND hWnd = mhWnd;
    RECT rect;
    GetClientRect(hWnd, &rect);
    DWORD iWidth = rect.right - rect.left;
    DWORD iHeight = rect.bottom - rect.top;

    DeviceProperty dp;
    dp.hInst = mhInst;
    dp.hWnd = hWnd;
    dp.bWnded = TRUE;
    dp.Width = iWidth;
    dp.Height = iHeight;
    dp.bVSync = FALSE;
    dp.RefreshRate = 0;
    dp.bNVPerfHUD = TRUE;

    Engine::Instance()->Init(&dp, "resource.ini", "plugin.ini");

    return true;
}

void App_Win32::Update()
{
    Engine::Instance()->Run();
}

void App_Win32::Shutdown()
{
    Engine::Instance()->Shutdown();
	delete Engine::Instance();
}

bool App_Win32::Quit()
{
    return false;
}

void App_Win32::OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
}


LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam) 
{ 
    switch(iMsg)
    {
    case WM_CREATE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    App_Win32::Instance()->OnMessage(hWnd, iMsg, wParam, lParam);

    return::DefWindowProc(hWnd, iMsg, wParam, lParam); 
}

#define WIN32_CLASS_NAME "Win32 Window For Game"

void App_Win32::Run(HINSTANCE hInstance)
{
    mhInst = hInstance;

    WNDCLASS wc;
    wc.cbClsExtra = 0; 
    wc.cbWndExtra = 0; 
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor= LoadCursor(NULL, IDC_ARROW); 
    wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION); 
    wc.hInstance = hInstance; 
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = WIN32_CLASS_NAME; 
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    TCHAR tile[128] = "Window";
    DWORD style = WS_OVERLAPPEDWINDOW;
    POINT pt = { CW_USEDEFAULT, CW_USEDEFAULT };
    SIZE sz = { CW_USEDEFAULT, CW_USEDEFAULT };

    OnPreCreateWindow(wc, tile, style, pt, sz);

    HWND hWnd = CreateWindow(WIN32_CLASS_NAME, tile, style, 
        pt.x, pt.y, sz.cx, sz.cy,
        NULL, NULL, hInstance, NULL);

    OnPostCreateWindow();

    ShowWindow(hWnd, SW_SHOW);

    mhWnd = hWnd;

    if (Init())
    {
        MSG msg;
        memset(&msg,0,sizeof(msg));
        while(msg.message != WM_QUIT && !Quit())
        {
            if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
            {
                Update();
            }
        }

        Shutdown();
    }

    UnregisterClass(WIN32_CLASS_NAME, hInstance);
}