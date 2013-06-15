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

ImplementRTTI(GameActor, IGameObject);

GameActor::GameActor()
{
	for (int i = 0; i < PT_Max; ++i)
	{
		mActor[i] = NULL;
	}
}

GameActor::~GameActor()
{
	for (int i = 0; i < PT_Max; ++i)
	{
		if (mActor[i])
			World::Instance()->DestroyActor(mActor[i]);
	}
}

GameComponent_AI * GameActor::GetAIComponent()
{
	for (int i = 0; i < mComponents.Size(); ++i)
	{
		if (RTTI_KindOf(GameComponent_AI, mComponents[i]))
			return RTTI_StaticCast(GameComponent_AI, mComponents[i]);
	}

	return NULL;
}


void GameActor::Init()
{
	GameComponent_AI * pAIComp = new GameComponent_AI;

	AddComponent(pAIComp);

	Idle();
}

void GameActor::Idle()
{
	GameComponent_AI * pAIComp = GetAIComponent();

	d_assert (pAIComp != NULL);

	pAIComp->ChangeState(new GameAIState_Idle);
}

void GameActor::MoveTo(const Vec3 & pos)
{
	GameComponent_AI * pAIComp = GetAIComponent();

	d_assert (pAIComp != NULL);

	pAIComp->ChangeState(new GameAIState_Move(pos));
}

void GameActor::SetPart(PartType type, const char * mesh)
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

void GameActor::PlayAnimation(const char * anim, const MotionBlendInfo & mbi)
{
	d_assert (mActor[Main] != NULL);
	mActor[GameActor::Main]->PlayAnimation(anim, mbi);
}
