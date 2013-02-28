#include "MWApp_Win32.h"
#include "NewtonPhysics.h"
#include "Player.h"

using namespace Myway;


class MyApp : public App_Win32
{
	Newton::tWorld ntWorld;

	Newton::tShape * floorShape;
	Newton::tBody * floorBody;

	Newton::tShape * sphShape;
	Newton::tBody * sphBody[5];

	Newton::tMaterialId floorMatId;
	Newton::tMaterialId sphMatId;
	Newton::tContact contact;

	Entity * floorEntity;

	Entity * sphEntity[5];
	SceneNode * sphNode[5];

	Newton::tRayCheckBase rayCheck;

	tEventListener<MyApp> OnContract;

	Player mPlayer;

public:
	MyApp()
		: OnContract(this, &MyApp::_ContactProcess)
	{
	}

	~MyApp() {}

	virtual bool Init()
	{
		App_Win32::Init();

		ntWorld.Init();

		CreateScene();

		mPlayer.Init();

		return true;
	}

	void CreateScene()
	{
		Environment::Instance()->InitEv();

		floorMatId.Query();
		sphMatId.Query();

		contact.Set(&sphMatId, &floorMatId);

		// create floor
		MeshPtr floorMesh = MeshManager::Instance()->CreatePlane("floor", Plane(Vec3(0, 1, 0), 0), 1000, 1000);
		floorEntity = World::Instance()->CreateEntity("floor", floorMesh);
		SceneNode * floorNode = World::Instance()->CreateSceneNode();

		floorNode->Attach(floorEntity);

		floorShape = new Newton::tPlane(Plane(Vec3(0, 1, 0), 0), 1000, 1000);
		//floorShape = new Newton::tBox(ntWorld, floorNode->GetWorldAabb().GetSize());
		floorBody = new Newton::tRigidBody(floorShape, floorNode, 0);
		floorBody->SetMaterialId(&floorMatId);

		// create sphere
		MeshPtr sphMesh = MeshManager::Instance()->CreateSphere("sphere", 20, 20, 20);
		
		sphShape = new Newton::tEllipsoid(20);

		for (int x = -2; x <= 2; x++)
		{
			sphEntity[x + 2] = World::Instance()->CreateEntity(TString128("sphere") + (x + 2), sphMesh);
			sphNode[x + 2] = World::Instance()->CreateSceneNode();

			sphNode[x + 2]->Attach(sphEntity[x + 2]);
			sphNode[x + 2]->SetPosition(x * 40.0f, 300, 0);

			sphBody[x + 2] = new Newton::tRigidBody(sphShape, sphNode[x + 2], 1 + 1.0f * (2 - x));

			sphBody[x + 2]->SetMaterialId(&sphMatId);
		}

		Newton::tBody * parent = NULL;
		Newton::tBody * child = NULL;
		Vec3 pos = Vec3(-2 * 40, 300, 0);

		World::Instance()->MainCameraNode()->SetPosition(0, 200, -500);
		World::Instance()->MainCamera()->SetDirection(Vec3(0, 0, 0) - Vec3(0, 200, -500));

		OnContract.Attach(&contact.OnContactProcess);
	}

	void _ContactProcess(Event * sender)
	{
		Vec3 pos = *(Vec3 *)sender->GetParam(0);
		Vec3 nrm = *(Vec3 *)sender->GetParam(1);

		float r = Math::RandRange(0.0f, 1.0f);
		float g = Math::RandRange(0.0f, 1.0f);
		float b = Math::RandRange(0.0f, 1.0f);
		float a = 1;

		Color4 diffuse = Color4(r, g, b, a);

		floorEntity->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
	}

	virtual void Shutdown()
	{
		mPlayer.Shutdown();

		OnContract.Detach();

		delete floorBody;
		delete floorShape;

		delete sphBody[0];
		delete sphBody[1];
		delete sphBody[2];
		delete sphBody[3];
		delete sphBody[4];
		delete sphShape;

		ntWorld.Shutdown();

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

		/*if (IKeyboard::Instance()->KeyPressed(KC_W))
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
		}*/

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

		Point2f pt = IMouse::Instance()->GetPositionUnit();

		if (pt.x > 0 && pt.x < 1 && pt.y > 0 && pt.y < 1)
		{
			Ray ray = World::Instance()->MainCamera()->GetViewportRay(pt.x, pt.y);

			if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
			{
				rayCheck.Clear();

				rayCheck.Do(ray.origin, ray.origin + ray.direction * 1000.0f);

				const Newton::tRayCheckBase::Info * tInfo = rayCheck.GetFirst();

				if (tInfo)
				{
					float r = Math::RandRange(0.0f, 1.0f);
					float g = Math::RandRange(0.0f, 1.0f);
					float b = Math::RandRange(0.0f, 1.0f);
					float a = 1;

					Color4 diffuse = Color4(r, g, b, a);

					if (tInfo->body == sphBody[0])
					{
						sphEntity[0]->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
					}
					else if (tInfo->body == sphBody[1])
					{
						sphEntity[1]->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
					}
					else if (tInfo->body == sphBody[2])
					{
						sphEntity[2]->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
					}
					else if (tInfo->body == sphBody[3])
					{
						sphEntity[3]->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
					}
					else if (tInfo->body == sphBody[4])
					{
						sphEntity[4]->GetSubEntity(0)->GetMaterial()->SetDiffuse(diffuse);
					}
				}
			}
		}

		mPlayer.Update();
		ntWorld.Update();
	}
};


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));

	//BreakAllock(803);

	char sFileName[1024];
	GetModuleFileName(GetModuleHandle(NULL), sFileName, 1024);

	TString128 fileDir = File::GetFileDir(sFileName);

	SetCurrentDirectory(fileDir.c_str());

	MyApp app;

	app.Run(hInstance);

	return 0;
}