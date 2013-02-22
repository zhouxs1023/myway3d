#include "stdafx.h"
#include "xApp.h"
#include "xPlugin.h"
#include "xScene.h"

DWORD WINAPI MainLoop(LPVOID p)
{
    xApp::Instance()->_InitEngine();

    while (!xApp::Instance()->IsQuit())
        xApp::Instance()->Run();

    return 0;
}

IMP_SLN(xApp);

_Locker::_Locker()
{ 
    xApp::Instance()->_lock(); 
}

_Locker::~_Locker()
{ 
    xApp::Instance()->_unlock();
}

xApp::xApp()
	: mObjMgr()
	, mOpToolBar()
	, OnUnloadScene(&xEvent::OnUnloadScene, this, &xApp::_unloadScene)
{
    INIT_SLN;

    mMutex = NULL;

    mEngine = NULL;
    mQuit = false;

    mNeedResize = false;

    mInited = false;

	mHelperShaderLib = NULL;

	mOperator = -1;

	mActivate = true;

	mMode = eAM_Editor;

	_loadPlugins();
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

		try
		{
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

			InputSystem::Instance()->Update();

			if (mActivate && xScene::Instance()->IsInited())
			{
				if (mMode == eAM_Editor)
				{
					xEvent::OnUpdate(NULL, NULL);
					_input();
				}
				else
				{
					xEvent::OnUpdateGame(NULL, NULL);
				}

				if (IKeyboard::Instance()->KeyUp(KC_F12))
				{
					if (mMode == eAM_Editor)
						mMode = eAM_Game;
					else
						mMode = eAM_Editor;

					xEvent::OnAppModeChanged(NULL, NULL);
				}
			}

			mEngine->Run();
		}
		catch (Exception & e)
		{
			d_assert (0);
			const char * msg = e.What();
		}
		catch (...)
		{
			d_assert (0);
		}
    }

    System::Sleep(10);
}

void xApp::_input()
{
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
	else if (IKeyboard::Instance()->KeyUp(KC_DELETE))
	{
		for (int i = 0; i < mSelectedObjs.Size(); ++i)
		{
			mObjMgr.Distroy(mSelectedObjs[i]);
		}

		if (mSelectedObjs.Size() > 0)
			SetSelectedObj(NULL);

		mSelectedObjs.Clear();
	}

	// parse 
	else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
		IKeyboard::Instance()->KeyUp(KC_V))
	{
		int count = 0;
		xObj * objs[1024];

		for (int i = 0; i < mSelectedObjs.Size(); ++i)
		{
			xObj * newObj = mSelectedObjs[i]->Clone();

			if (newObj)
				objs[count++] = newObj;
		}

		if (count)
			SetSelectedObjs(objs, count);
	}
	
	// undo
	else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
		IKeyboard::Instance()->KeyUp(KC_Z))
	{
		mUndoRedoManager.Undo();
	}

	// redo
	else if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) &&
		IKeyboard::Instance()->KeyUp(KC_Y))
	{
		mUndoRedoManager.Redo();
	}
	
	// pick
	if (IMouse::Instance()->KeyUp(MKC_BUTTON0) && !mGizmo.IsPicked())
	{
		 Point2f pt = IMouse::Instance()->GetPositionUnit();

		 if (pt.x > 0 && pt.x < 1 && pt.y > 0 && pt.y < 1)
		 {
			 Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

			/* List<SceneNode*> list;

			 World::Instance()->RayTracing(ray, 1000.0f, list, PICK_Flag);

			 List<SceneNode *>::Iterator whr = list.Begin();
			 List<SceneNode *>::Iterator end = list.End();

			 float dist = FLT_MAX;
			 SceneNode * node = NULL;
			 Vec3 camPos = World::Instance()->MainCamera()->GetPosition();

			 while (whr != end)
			 {
				 SceneNode * node1 = (*whr);

				 float dist1 = camPos.Distance(node1->GetPosition());

				 if (dist1 < dist)
				 {
					 dist = dist1;
					 node = node1;
				 }

				 ++whr;
			 }*/


			 HitInfoSetArray infos;

			 IPhyWorld::Instance()->RayCheck(infos, ray, 1000.0f, PICK_Flag, true);

			 if (infos.Size() > 0 && infos[0].node)
			 {
				 xObj * obj = mObjMgr.Get(infos[0].node);

				 if (obj)
					 SetSelectedObj(obj);
			 }
		 }
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

	if (IMouse::Instance()->MouseWheel())
	{
		cam->Move(0.5f * IMouse::Instance()->MouseWheel());
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

	try
	{
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

		//Environment::Instance()->LoadTerrain("Terrain.terrain");
		//Environment::Instance()->InitEv();
		//Environment::Instance()->SetKey(EVKT_Noon);

		SceneNode * cam = World::Instance()->MainCameraNode();

		cam->SetPosition(0, 200, 0);

		Quat q; 
		q.FromAxis(Vec3::UnitX, Vec3::UnitZ, Vec3::NegUnitY);
		cam->SetOrientation(q);

		World::Instance()->Resize(2048, 2048, 2048);

		mHelperShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Helper", "Helper.ShaderLib");

		xPluginManager::Instance()->Init();

		xEvent::OnInit(NULL, NULL);
		xEvent::OnInitUI(NULL, NULL);
	}
	catch (Exception & e)
	{
		d_assert (0);
		const char * msg = e.What();
	}
	catch (...)
	{
		d_assert (0);
	}
}

void xApp::Shutdown()
{
    if (!IsInited())
        return ;

    __enter();

    mQuit = true;

    mThread.Shudown(true);

    xEvent::OnShutdown(NULL, NULL);

	xPluginManager::Instance()->Shutdown();

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
		xEvent::OnUnSelectObj(NULL, NULL);

		return ;
	}

	mSelectedObjs.PushBack(obj);

	xEvent::OnSelectObj(NULL, NULL);
}

void xApp::SetSelectedObjs(xObj ** objs, int size)
{
	mSelectedObjs.Clear();

	for (int i = 0; i < size; ++i)
	{
		mSelectedObjs.PushBack(objs[i]);
	}

	xEvent::OnSelectObj(NULL, NULL);
}

int xApp::GetSelectedObjSize()
{
	return mSelectedObjs.Size();
}

xObj * xApp::GetSelectedObj(int index)
{
	return GetSelectedObjSize() ? mSelectedObjs[index] : NULL;
}

void xApp::_loadPlugins()
{
	FileSystem fs("xPlugin");

	fs.Load();

	Archive::FileInfoVisitor v = fs.GetFileInfos();

	while (!v.Endof())
	{
		TString128 plugin = v.Cursor()->second.name;

		if (File::GetExternName(plugin) == "dll")
		{
			Dll dll("xPlugin\\" + plugin);

			dll.Load();
			//DllManager::Instance()->Load();
		}

		++v;
	}
}

void xApp::_unloadScene(void * param0, void * param1)
{
	SetSelectedObj(NULL);
}

Vec3 xApp::GetHitPosition(float x, float y)
{
	Ray ray = World::Instance()->MainCamera()->GetViewportRay(x, y);

	Vec3 pos = ray.origin + ray.direction * 80;

	return pos;
}