#include "stdafx.h"
#include "xApp.h"

DWORD WINAPI MainLoop(LPVOID p)
{
    xApp::Instance()->_InitEngine();

    while (!xApp::Instance()->IsQuit())
        xApp::Instance()->Run();

    return 0;
}

IMP_SLN(xApp);

xApp gxApp;

Event xApp::OnInit;
Event xApp::OnInitUI;
Event xApp::OnShutdown;
Event xApp::OnSelectObj;
Event xApp::OnUnSelectObj;
Event xApp::OnUpdate;

_Locker::_Locker()
{ 
    xApp::Instance()->_lock(); 
}

_Locker::~_Locker()
{ 
    xApp::Instance()->_unlock();
}

xApp::xApp()
{
    INIT_SLN;

    mMutex = NULL;

    mEngine = NULL;
    mQuit = false;

    mNeedResize = false;

    mInited = false;

	mHelperShaderLib = NULL;
}

xApp::~xApp()
{
    SHUT_SLN;
    mQuit = false;
}

void xApp::Run()
{
    if (IsInited())
    {
        __enter();

        if (mQuit)
            return ;

        if (mNeedResize)
        {
            System::Sleep(10);

            RECT rc;

            GetWindowRect(mhWnd, &rc);

            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;

            mEngine->Resize(width, height);

            mNeedResize = false;
        }

		OnUpdate.Call(NULL);

        _input();
        mEngine->Run();
    }

    System::Sleep(10);
}

void xApp::_input()
{
    InputSystem::Instance()->Update();

    SceneNode * cam = World::Instance()->MainCameraNode();

    if (IKeyboard::Instance()->KeyPressed(KC_W))
    {
        cam->Move(5.0f);
    }
    else if (IKeyboard::Instance()->KeyPressed(KC_S))
    {
        cam->Move(-5.0f);
    }
    else if (IKeyboard::Instance()->KeyPressed(KC_A))
    {
        cam->Right(-5.0f);
    }
    else if (IKeyboard::Instance()->KeyPressed(KC_D))
    {
        cam->Right(5.0f);
    }

    if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
    {
        Point2i pt = IMouse::Instance()->GetPositionDiff();

        if (abs(pt.y) >abs(pt.x))
        {
            cam->Pitch(pt.y * 0.005f, TS_LOCAL);
        }
        else
        {
            cam->Yaw(pt.x * 0.005f, TS_PARENT);
        }
    }
}

void xApp::Init(HWND hWnd)
{
    mhWnd = hWnd;
    _InitEngine();

    mInited = true;
    //mThread.Intialize(MainLoop);
    //mThread.Run();
}

void xApp::_InitEngine()
{
    __enter();

    RECT rc;

    mEngine = new Engine();

    GetWindowRect(mhWnd, &rc);

    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    DeviceProperty dp;
    dp.hInst = GetModuleHandle(NULL);
    dp.hWnd = mhWnd;
    dp.bWnded = TRUE;
    dp.Width = width;
    dp.Height = height;
    dp.bVSync = FALSE;
    dp.RefreshRate = 0;
    dp.bNVPerfHUD = TRUE;

    Engine::Instance()->Init(&dp, "resource.ini", "plugin.ini");

    Environment::Instance()->LoadTerrain("Terrain.terrain");
    Environment::Instance()->InitEv();

    SceneNode * cam = World::Instance()->MainCameraNode();

    cam->SetPosition(500, 200, 500);

    Quat q; 
    q.FromAxis(Vec3::UnitX, Vec3::UnitZ, Vec3::NegUnitY);
    cam->SetOrientation(q);

    World::Instance()->Resize(1024, 1024, 1024);

	mHelperShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Helper", "Helper.ShaderLib");

	OnInit.Call();
    OnInitUI.Call();
}

void xApp::Shutdown()
{
    if (!IsInited())
        return ;

    __enter();

    mQuit = true;

    mThread.Shudown(true);

    OnShutdown.Call();

    mEngine->Shutdown();
    delete mEngine;

	mSelectedObjs.Clear();
	mHelperShaderLib = NULL;
}

void xApp::Resize(int w, int h)
{
    __enter();

    mNeedResize =  true;
}

void xApp::SetSelectedObj(xObj * obj)
{
	mSelectedObjs.Clear();

	if (obj == NULL)
	{
		OnUnSelectObj.Call();
	}

	mSelectedObjs.PushBack(obj);

	OnSelectObj.Call();
}

void xApp::SetSelectedObjs(xObj ** objs, int size)
{
	mSelectedObjs.Clear();

	for (int i = 0; i < size; ++i)
	{
		mSelectedObjs.PushBack(objs[i]);
	}

	OnSelectObj.Call();
}

int xApp::GetSelectedObjSize()
{
	return mSelectedObjs.Size();
}

xObj * xApp:: GetSelectedObj(int index)
{
	return GetSelectedObjSize() ? mSelectedObjs[index] : NULL;
}