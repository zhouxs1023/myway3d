#include "stdafx.h"

#include "Npc.h"

ImplementRTTI(cNpc, GmActor);

cNpc::cNpc(int _typeId)
	: mTypeId(_typeId)
	, mHP(100)
	, mMP(100)
{
	mNpcInfo = GmDataManager::Instance()->GetNpcInfo(_typeId);

	d_assert (mNpcInfo != NULL);

	SetPart(GmActor::Main, mNpcInfo->part_main.c_str());

	if (mNpcInfo->anim_Idle0 != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle0.c_str(), mNpcInfo->anim_Idle0.c_str());

	if (mNpcInfo->anim_Idle1 != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle1.c_str(), mNpcInfo->anim_Idle1.c_str());

	if (mNpcInfo->anim_Walk != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Walk.c_str(), mNpcInfo->anim_Walk.c_str());

	if (mNpcInfo->anim_Run != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Run.c_str(), mNpcInfo->anim_Run.c_str());
}

cNpc::~cNpc()
{
}

const TString128 & cNpc::GetName()
{
	d_assert (mNpcInfo != NULL);

	return mNpcInfo->name.c_str();
}