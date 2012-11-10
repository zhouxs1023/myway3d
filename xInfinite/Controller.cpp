#include "stdafx.h"
#include "Controller.h"

namespace xInfi {

    DWORD WINAPI MainLoop(LPVOID p)
    {
        Controller::Instance()->_InitEngine();

        while (!Controller::Instance()->IsQuit())
            Controller::Instance()->Run();

        return 0;
    }

    IMP_SLN(Controller);

    Controller gController;

    Event Controller::OnEngineInit;
    Event Controller::OnEngineDeInit;

    _Locker::_Locker()
    { 
        Controller::Instance()->_lock(); 
    }

    _Locker::~_Locker()
    { 
        Controller::Instance()->_unlock();
    }

    Controller::Controller()
    {
        INIT_SLN;

        mMutex = NULL;

        mEngine = NULL;
        mQuit = false;

        mNeedResize = false;

        mInited = false;
    }

    Controller::~Controller()
    {
        SHUT_SLN;
        mQuit = false;
    }

    void Controller::Run()
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

            _input();
            mEngine->Run();
        }

        System::Sleep(10);
    }

    void Controller::_input()
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

    void Controller::Init(HWND hWnd)
    {
        return ;

        mhWnd = hWnd;
        _InitEngine();

        mInited = true;
        //mThread.Intialize(MainLoop);
        //mThread.Run();
    }

    void Controller::_InitEngine()
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

        OnEngineInit.Call();
    }

    void Controller::Shutdown()
    {
        if (!IsInited())
            return ;

        __enter();

        mQuit = true;

        mThread.Shudown(true);

        OnEngineDeInit.Call();

        mEngine->Shutdown();
        delete mEngine;
    }

    void Controller::Resize(int w, int h)
    {
        __enter();

        mNeedResize =  true;
    }
}

