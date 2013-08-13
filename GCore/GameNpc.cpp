#include "stdafx.h"

#include "GameNpc.h"

ImplementRTTI(GmNpc, GmActor);

GmNpc::GmNpc(int templateId)
{
	_init(templateId);
}

GmNpc::~GmNpc()
{
}

void GmNpc::_init(int templateId)
{
	mNpcInfo = GmDataManager::Instance()->GetNpcInfo(templateId);

	d_assert (mNpcInfo != NULL);

	SetPart(GmActor::Main, mNpcInfo->part_main.c_str());

	if (mNpcInfo->anim_Idle0 != "")
		mActor[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle0.c_str(), mNpcInfo->anim_Idle0.c_str());

	if (mNpcInfo->anim_Idle1 != "")
		mActor[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle1.c_str(), mNpcInfo->anim_Idle1.c_str());

	if (mNpcInfo->anim_Walk != "")
		mActor[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Walk.c_str(), mNpcInfo->anim_Walk.c_str());

	if (mNpcInfo->anim_Run != "")
		mActor[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Run.c_str(), mNpcInfo->anim_Run.c_str());
}



