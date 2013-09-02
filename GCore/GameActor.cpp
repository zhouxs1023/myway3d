#include "stdafx.h"

#include "GameActor.h"
#include "GameAI.h"

TString128 _ActorPartName[] = {
	TString128("Main"),
	TString128("Weapon"),
	TString128("Helmet"),
	TString128("Shoulder"),
	TString128("Clothes"),
	TString128("Shoes")
};

ImplementRTTI(GmActor, GmObj);

GmActor::GmActor()
{
	for (int i = 0; i < PT_Max; ++i)
	{
		mEntity[i] = NULL;
	}

	mMoveSpeed = 30.0f;
	mRotateSpeed = 360.0f;
	mDirection = 0;

	mInitOrt.FromAxis(Vec3::UnitY, Math::PI_1);

	mCompAI = new GmCompAI;
	mCompMove = new GmCompMove;
	mCompRotate = new GmCompRotate;

	AddComponent(mCompAI);
	AddComponent(mCompMove);
	AddComponent(mCompRotate);
}

GmActor::~GmActor()
{
	for (int i = 0; i < PT_Max; ++i)
	{
		if (mEntity[i] != NULL)
			World::Instance()->DestroyEntity(mEntity[i]);
	}
}

GmCompAI * GmActor::GetAIComponent()
{
	return mCompAI;
}

GmCompMove * GmActor::GetMoveComponent()
{
	return mCompMove;
}

GmCompRotate * GmActor::GetRotateComponent()
{
	return mCompRotate;
}

void GmActor::Init()
{
	GmCompAI * pAIComp = new GmCompAI;

	AddComponent(pAIComp);

	Idle();
}

void GmActor::Idle()
{
	if (!RTTI_TypeOf(AIState_Move, mCompAI->GetCurrentState()))
		mCompAI->ChangeState(new AIState_Idle);
}

void GmActor::Run()
{
	if (!RTTI_TypeOf(AIState_Move, mCompAI->GetCurrentState()))
		mCompAI->ChangeState(new AIState_Move);
}

void GmActor::MoveTo(const Vec3 & tarPos)
{
	GmCompAI * pAIComp = GetAIComponent();

	d_assert (pAIComp != NULL);

	const Vec3 & myPos = GetPosition();
	Vec3 dir = tarPos - myPos;

	if (dir.NormalizeL() > 0.01f)
	{
		float dt = dir.Dot(Vec3::UnitZ);
		float deg = Math::ACos(dt);

		if (tarPos.x < myPos.x)
			deg = -deg;

		RotateTo(Math::RadianToDegree(deg));

		mCompMove->SetTargetPos(tarPos);

		Run();
	}
}

void GmActor::RotateTo(float dir)
{
	dir = Math::DegreeNormalize(dir);
	mCompRotate->SetTargetDir(dir);
}

void GmActor::ForceMove(const Vec3 & pos)
{
	SetPosition(pos);
	mCompMove->SetTargetPos(pos);
}

void GmActor::ForceRotate(float dir)
{
	dir = Math::DegreeNormalize(dir);
	SetDirection(dir);
	mCompRotate->SetTargetDir(dir);
}

void GmActor::SetPart(PartType type, const char * mesh)
{
	if (mEntity[type])
	{
		World::Instance()->DestroyEntity(mEntity[type]);
		mEntity[type] = NULL;
	}

	if (strcmp(mesh, "") == 0)
		return ;

	TString128 uname = mName + "_" + mId + "_" + _ActorPartName[type];

	mEntity[type] = World::Instance()->CreateEntity(uname, mesh);

	mNode->Attach(mEntity[type]);
}

void GmActor::PlayAnimation(const char * anim, const MotionBlendInfo & mbi)
{
	d_assert (mEntity[Main] != NULL);
	mEntity[GmActor::Main]->PlayAnimation(anim, mbi);
}

void GmActor::Update(float dtime)
{
	GmObj::Update(dtime);

	mEntity[GmActor::Main]->UpdateAnimation(dtime);
}

void GmActor::SetDirection(float dir)
{
	mDirection = Math::DegreeNormalize(dir);

	Quat quat = Quat::S_FromAxis(Vec3::UnitY, Math::DegreeToRadian(mDirection));

	SetOrientation(mInitOrt * quat);
}
