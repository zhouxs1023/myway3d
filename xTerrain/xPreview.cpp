#include "stdafx.h"

#include "xPreview.h"
#include "MActorManager.h"
#include "xApp.h"

xPreViewer::xPreViewer()
	: mActor(NULL)
	, mNode(NULL)
{
}

xPreViewer::~xPreViewer()
{
}

void xPreViewer::Init()
{
	if (Environment::Instance()->GetTerrain())
	{
		IPhyWorld::Instance()->CreateTerrain();
	}

	TString128 ActorFile = "Actor\\Test.lma";

	mNode = World::Instance()->CreateSceneNode();
	mActor = MActorManager::Instance()->CreateActor("PreviewActor", ActorFile);
	mNode->Attach(mActor);

	Camera * cam = World::Instance()->MainCamera();

	Mat4 matRotation;
	matRotation.MakeRotationY(Math::PI_1);

	mActor->SetRotationMatrix(matRotation);

	mNode->SetPosition(cam->GetPosition());
	mNode->SetScale(0.3f);

	Ray ray;

	ray.origin = mNode->GetPosition() + Vec3::UnitY * 5.0f;
	ray.direction = Vec3::NegUnitY;

	HitInfoSetArray infos;

	IPhyWorld::Instance()->RayCheck(infos, ray, 10, PICK_Flag);

	if (!infos.Size())
	{
		OnXailuo();
	}
	else
	{
		mNode->SetPosition(infos[0].r_that_position);
		OnIdle();
	}
}

void xPreViewer::Shutdown()
{
	if (Environment::Instance()->GetTerrain())
	{
		IPhyWorld::Instance()->DestroyTerrain();
	}

	World::Instance()->DestroySceneNode(mNode);
	MActorManager::Instance()->DestroyActor(mActor);

	mNode = NULL;
	mActor = NULL;
}


void xPreViewer::Update()
{
	float frameTime = Engine::Instance()->GetFrameTime();

	if (mState == K_XiaLuo)
	{
		OnXailuo();

		Ray ray;

		ray.origin = mNode->GetPosition() + Vec3::UnitY * 5.0f;
		ray.direction = Vec3::NegUnitY;

		HitInfoSetArray infos;

		IPhyWorld::Instance()->RayCheck(infos, ray, 10, PICK_Flag);

		if (infos.Size())
		{
			mNode->SetPosition(infos[0].r_that_position);
			OnIdle();
		}
		else
		{
			mNode->Translate(Vec3::NegUnitY * frameTime * 10);
		}
	}

	else if (mState == K_Run)
	{
		OnRun();

		if (IKeyboard::Instance()->KeyPressed(KC_W))
		{
			Vec3 nextPos = mNode->GetPosition() + mNode->GetDirection( ) * frameTime * 100;

			Ray ray;
			float height = mNode->GetWorldAabb().GetHeight();

			ray.origin = nextPos + Vec3::UnitY * height;
			ray.direction = Vec3::NegUnitY;

			HitInfoSetArray infos;

			IPhyWorld::Instance()->RayCheck(infos, ray, 100, PICK_Flag);

			if (infos.Size())
			{
				mNode->SetPosition(infos[0].r_that_position);
			}
			else
			{
				mNode->SetPosition(nextPos);
				OnXailuo();
			}

		}
		else
		{
			OnIdle();
			d_printf("idle");
		}
	}

	else if (mState == K_Idle)
	{
		OnIdle();

		if (IKeyboard::Instance()->KeyDown(KC_W))
		{
			OnRun();
			d_printf("run");
		}
	}

	if (IKeyboard::Instance()->KeyUp(KC_GRAVE))
	{
		OnWaveArm();
	}


	if (IMouse::Instance()->MouseMoved() && IMouse::Instance()->KeyPressed(MKC_BUTTON1))
	{
		Point2i pt = IMouse::Instance()->GetPositionDiff();

		mNode->Yaw(pt.x * 0.005f, TS_PARENT);

		if (abs(pt.y) >abs(pt.x))
		{
			//mNode->Rotate(pt.y * 0.005f, 0, 0, TS_LOCAL);
		}
		else
		{
			
		}
	}


	// update camera
	{
		float height = mNode->GetWorldAabb().GetHeight();

		Camera * cam = World::Instance()->MainCamera();

		Quat q;

		q.FromAxis(mNode->GetRight(), Math::PI_1 / 30);

		cam->SetPosition(mNode->GetPosition() + Vec3::UnitY * height * 0.8f);
		cam->SetOrientation(q * mNode->GetOrientation());
		cam->SetPosition(cam->GetPosition() - cam->GetDirection() * 50);
	}
}

void xPreViewer::OnXailuo()
{
	mActor->PlayMotion("idle");
	mState = K_XiaLuo;
}

void xPreViewer::OnIdle()
{
	mActor->PlayMotion("idle");
	mState = K_Idle;
}

void xPreViewer::OnWalk()
{
	mActor->PlayMotion("walk");
	mState = K_Walk;
}

void xPreViewer::OnRun()
{
	mActor->PlayMotion("run");
	mState = K_Run;
}

void xPreViewer::OnJump()
{
	mActor->PlayMotion("jump");
	mState = K_Jump;
}

void xPreViewer::OnWaveArm()
{
	MMotion::PlayInfo playInfo;
	playInfo.Loops = 1;
	playInfo.Mixed = true;
	mActor->PlayMotion("ArmMixMotion", playInfo);
}







xPreViewerListener gPreViewerListener;

xPreViewerListener::xPreViewerListener()
	: OnAppModeChanged(&xEvent::OnAppModeChanged, this, &xPreViewerListener::_OnAppModeChanged)
	, OnUpdate(&xEvent::OnUpdateGame, this, &xPreViewerListener::_OnUpdate)
{
}

xPreViewerListener::~xPreViewerListener()
{
}

void xPreViewerListener::_OnUpdate(Event * sender)
{
	mPreviewer.Update();
}

void xPreViewerListener::_OnAppModeChanged(Event * sender)
{
	switch (xApp::Instance()->GetAppMode())
	{
	case eAM_Editor:
		mPreviewer.Shutdown();
		break;

	case eAM_Game:
		mPreviewer.Init();
		break;
	}
}