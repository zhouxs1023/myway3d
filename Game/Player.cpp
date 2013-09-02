#include "stdafx.h"

#include "Player.h"

cPlayer::cPlayer(const PlayerViewInfo & _info)
{
	const GmNpcInfo * tInfo = GmDataManager::Instance()->GetNpcInfo(0);

	d_assert (tInfo != NULL);

	SetPart(GmActor::Main, tInfo->part_main.c_str());

	if (tInfo->anim_Idle0 != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle0.c_str(), tInfo->anim_Idle0.c_str());

	if (tInfo->anim_Idle1 != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Idle1.c_str(), tInfo->anim_Idle1.c_str());

	if (tInfo->anim_Walk != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Walk.c_str(), tInfo->anim_Walk.c_str());

	if (tInfo->anim_Run != "")
		mEntity[GmActor::Main]->LoadAnimation(GmHelper::S_Anim_Run.c_str(), tInfo->anim_Run.c_str());

	ForceMove(_info.Position);
	ForceRotate(_info.Direction);

	PlayAnimation(GmHelper::S_Anim_Idle0.c_str(), MotionBlendInfo());
}

cPlayer::~cPlayer()
{
}