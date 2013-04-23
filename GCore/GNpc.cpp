#include "stdafx.h"

#include "GNpc.h"
#include "GAIStates.h"

namespace game {

	GNpc::GNpc(int id, int templateId)
		: GActor(("Npc_" + TString128(id)).c_str())
		, mUId(id)
	{
		_init(templateId);
	}

	GNpc::~GNpc()
	{
	}

	void GNpc::_init(int templateId)
	{
		mNpcInfo = GDataManager::Instance()->GetNpcInfo(templateId);

		d_assert (mNpcInfo != NULL);

		SetPart(GActor::Main, mNpcInfo->part_main.c_str());
		
		if (mNpcInfo->anim_Idle0 != "")
			mActor[GActor::Main]->LoadAnimation(GHelper::S_Anim_Idle0.c_str(), mNpcInfo->anim_Idle0.c_str());

		if (mNpcInfo->anim_Idle1 != "")
			mActor[GActor::Main]->LoadAnimation(GHelper::S_Anim_Idle1.c_str(), mNpcInfo->anim_Idle1.c_str());

		if (mNpcInfo->anim_Walk != "")
			mActor[GActor::Main]->LoadAnimation(GHelper::S_Anim_Walk.c_str(), mNpcInfo->anim_Walk.c_str());

		if (mNpcInfo->anim_Run != "")
			mActor[GActor::Main]->LoadAnimation(GHelper::S_Anim_Run.c_str(), mNpcInfo->anim_Run.c_str());

		ChangeState(new GState_Idle(this));
	}

	void GNpc::Update()
	{
		GAIEntity::Think();
	}

	void GNpc::ChangeState(GState * state)
	{
		GAIEntity::ChangeState(state);
	}
}