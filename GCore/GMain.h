#pragma once

#include "GData.h"
#include "GWorld.h"

namespace game {

	class GCORE_ENTRY GMain
	{
	public:
		GMain();
		virtual ~GMain();

		void Init();
		void Shutdown();

		void Update();

	protected:
		GDataManager mDataManager;
		GWorld mWorld;
	};

}