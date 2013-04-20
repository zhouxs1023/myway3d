#include "stdafx.h"

#include "GNpc.h"

namespace game {

	GNpc::GNpc(int id, int mid)
		: GActor(("Npc_" + TString128(id)).c_str())
		, mUId(id)
	{
		_init(mid);
	}

	GNpc::~GNpc()
	{
	}

	void GNpc::_init(int mid)
	{
		mNpcInfo = GDataManager::Instance()->GetNpcInfo(mid);

		d_assert (mNpcInfo != NULL);

		SetPart(GActor::Main, mNpcInfo->part_main.c_str());
		SetPart(GActor::Weapon, mNpcInfo->part_weapon.c_str());
	}
}