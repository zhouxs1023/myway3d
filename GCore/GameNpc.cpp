#include "stdafx.h"

#include "GameNpc.h"

ImplementRTTI(GameNpc, GameActor);

GameNpc::GameNpc(int templateId)
{
	_init(templateId);
}

GameNpc::~GameNpc()
{
}

void GameNpc::_init(int templateId)
{
	mNpcInfo = GameDataManager::Instance()->GetNpcInfo(templateId);

	d_assert (mNpcInfo != NULL);

	SetPart(GameActor::Main, mNpcInfo->part_main.c_str());

	if (mNpcInfo->anim_Idle0 != "")
		mActor[GameActor::Main]->LoadAnimation(GameHelper::S_Anim_Idle0.c_str(), mNpcInfo->anim_Idle0.c_str());

	if (mNpcInfo->anim_Idle1 != "")
		mActor[GameActor::Main]->LoadAnimation(GameHelper::S_Anim_Idle1.c_str(), mNpcInfo->anim_Idle1.c_str());

	if (mNpcInfo->anim_Walk != "")
		mActor[GameActor::Main]->LoadAnimation(GameHelper::S_Anim_Walk.c_str(), mNpcInfo->anim_Walk.c_str());

	if (mNpcInfo->anim_Run != "")
		mActor[GameActor::Main]->LoadAnimation(GameHelper::S_Anim_Run.c_str(), mNpcInfo->anim_Run.c_str());
}



