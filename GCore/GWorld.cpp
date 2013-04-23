#include "stdafx.h"

#include "GWorld.h"

namespace game {

	IMP_SLN(GWorld);

	GWorld::GWorld()
	{
		INIT_SLN;
	}

	GWorld::~GWorld()
	{
		SHUT_SLN;
	}

	void GWorld::Init()
	{
	}

	void GWorld::Shutdown()
	{
		DestroyAllNpc();
	}

	void GWorld::Update()
	{
		for (int i = 0; i < mNpcs.Size(); ++i)
		{
			mNpcs[i]->Update();
		}
	}

	GNpc * GWorld::CreateNpc(int uId, int mId)
	{
		d_assert (GetNpc(uId) == NULL);

		GNpc * npc = new GNpc(uId, mId);

		mNpcs.PushBack(npc);

		return npc;
	}

	void GWorld::DestroyNpc(GNpc * npc)
	{
		for (int i = 0; i < mNpcs.Size(); ++i)
		{
			if (mNpcs[i] == npc)
			{
				delete npc;
				mNpcs.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void GWorld::DestroyNpc(int uId)
	{
		for (int i = 0; i < mNpcs.Size(); ++i)
		{
			if (mNpcs[i]->GetUId() == uId)
			{
				delete mNpcs[i];
				mNpcs.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	GNpc * GWorld::GetNpc(int uId)
	{
		for (int i = 0; i < mNpcs.Size(); ++i)
		{
			if (mNpcs[i]->GetUId() == uId)
			{
				return mNpcs[i];
			}
		}

		return NULL;
	}

	void GWorld::DestroyAllNpc()
	{
		for (int i = 0; i < mNpcs.Size(); ++i)
		{
			delete mNpcs[i];
		}

		mNpcs.Clear();
	}

}