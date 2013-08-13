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
		mActor[i] = NULL;
	}
}

GmActor::~GmActor()
{
	for (int i = 0; i < PT_Max; ++i)
	{
		if (mActor[i])
			World::Instance()->DestroyActor(mActor[i]);
	}
}

GmCompAI * GmActor::GetAGmComponent()
{
	for (int i = 0; i < mComponents.Size(); ++i)
	{
		if (RTTI_KindOf(GmCompAI, mComponents[i]))
			return RTTI_StaticCast(GmCompAI, mComponents[i]);
	}

	return NULL;
}


void GmActor::Init()
{
	GmCompAI * pAIComp = new GmCompAI;

	AddComponent(pAIComp);

	Idle();
}

void GmActor::Idle()
{
	GmCompAI * pAIComp = GetAGmComponent();

	d_assert (pAIComp != NULL);

	pAIComp->ChangeState(new AIState_Idle);
}

void GmActor::MoveTo(const Vec3 & pos)
{
	GmCompAI * pAIComp = GetAGmComponent();

	d_assert (pAIComp != NULL);

	pAIComp->ChangeState(new AIState_Move(pos));
}

void GmActor::SetPart(PartType type, const char * mesh)
{
	if (mActor[type])
	{
		World::Instance()->DestroyActor(mActor[type]);
		mActor[type] = NULL;
	}

	if (strcmp(mesh, "") == 0)
		return ;

	TString128 uname = mName + "_" + mId + _ActorPartName[type];

	mActor[type] = World::Instance()->CreateActor(uname, mesh);

	mNode->Attach(mActor[type]);
}

void GmActor::PlayAnimation(const char * anim, const MotionBlendInfo & mbi)
{
	d_assert (mActor[Main] != NULL);
	mActor[GmActor::Main]->PlayAnimation(anim, mbi);
}
