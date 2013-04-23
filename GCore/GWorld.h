#pragma once

#include "GNpc.h"

namespace game {

	class GCORE_ENTRY GWorld
	{
		DECLARE_SINGLETON(GWorld);

	public:
		GWorld();
		~GWorld();

		void Init();
		void Shutdown();

		void Update();

		GNpc * CreateNpc(int uId, int mId);
		void DestroyNpc(GNpc * npc);
		void DestroyNpc(int uId);
		GNpc * GetNpc(int uId);
		void DestroyAllNpc();

	protected:
		Array<GNpc*> mNpcs;
	};

}