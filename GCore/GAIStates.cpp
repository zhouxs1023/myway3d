#include "stdafx.h"

#include "GAIStates.h"
#include "GData.h"

namespace game {

	GState_Idle::GState_Idle(GActor * actor)
	{
		mActor = actor;
		mRandomIdleCount = 300;
	}

	GState_Idle::~GState_Idle()
	{
	}

	void GState_Idle::Enter()
	{
		mActor->PlayAnimation(GHelper::S_Anim_Idle0.c_str(), MotionBlendInfo::Default);
	}

	void GState_Idle::Exit()
	{
	}

	void GState_Idle::Think(GAIEntity * entity)
	{
		--mRandomIdleCount;

		if (mRandomIdleCount <= 0)
		{
			MotionBlendInfo mbi;

			mbi.Looped = false;
			mbi.Mixed = true;

			mActor->PlayAnimation(GHelper::S_Anim_Idle1.c_str(), mbi);

			mRandomIdleCount = 300;
		}
	}

}