#include "MWApp_Win32.h"
#include "NewtonPhysics.h"

using namespace Myway;


class MyApp : public App_Win32
{
	Newton::tWorld  * ntWorld;

	Newton::tShape * floorShape;
	Newton::tBody * floorBody;

	Newton::tShape * sphShape;
	Newton::tBody * sphBody;

public:
	MyApp()
	{
	}

	~MyApp() {}

	virtual bool Init()
	{
		App_Win32::Init();

		ntWorld = new Newton::tWorld;

		ntWorld->Init();

		CreateScene();

		return true;
	}

	void CreateScene()
	{
		// create floor
		MeshPtr floorMesh = MeshManager::Instance()->CreatePlane("floor", Plane(Vec3(0, 1, 0), 0), 1000, 1000);
		Entity * floorEnt = World::Instance()->CreateEntity("floor", floorMesh);
		SceneNode * floorNode = World::Instance()->CreateSceneNode();

		floorNode->Attach(floorEnt);

		floorShape = new Newton::tPlane(ntWorld, Plane(Vec3(0, 1, 0), 0), 1000, 1000);
		//floorShape = new Newton::tBox(ntWorld, floorNode->GetWorldAabb().GetSize());
		floorBody = new Newton::tRigidBody(ntWorld, floorShape, floorNode, 0);


		// create sphere
		MeshPtr sphMesh = MeshManager::Instance()->CreateSphere("sphere", 20, 20, 20);
		Entity * sphEntity = World::Instance()->CreateEntity("sphere", sphMesh);
		SceneNode * sphNode = World::Instance()->CreateSceneNode();

		sphNode->Attach(sphEntity);
		sphNode->SetPosition(0, 300, 0);

		sphShape = new Newton::tEllipsoid(ntWorld, 20);
		sphBody = new Newton::tRigidBody(ntWorld, sphShape, sphNode, 10);

		World::Instance()->MainCameraNode()->SetPosition(0, 200, -500);
		World::Instance()->MainCamera()->SetDirection(Vec3(0, 0, 0) - Vec3(0, 200, -500));

		Environment::Instance()->InitEv();
	}

	virtual void Shutdown()
	{
		delete floorBody;
		delete floorShape;

		delete sphBody;
		delete sphShape;

		ntWorld->Shutdown();

		App_Win32::Shutdown();
	}

	virtual void OnMessage(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
	{
	}

	virtual void Update()
	{
		App_Win32::Update();

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

		ntWorld->Update();
	}
};


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));

	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	MyApp app;

	app.Run(hInstance);

	return 0;
}