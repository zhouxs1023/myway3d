#pragma once

#include "Myway.h"
#include "Newton.h"

namespace Myway {

	namespace Newton {

		class World 
		{
			DECLARE_SINGLETON(World);

		public:
			World();
			~World();

			void Init();
			void Shutdown();
			void Update();

			void SetSimulateFPS(float fps) { d_assert (fps > 0); mSimulateTime = 1 / fps; }
			float GetSimulateFPS() const { return 1 / mSimulateTime; }

			inline NewtonWorld * _getNewtonWorld() { return mNtWorld; }

		protected:
			NewtonWorld * mNtWorld;
			float mSimulateTime;
			float mInteralTime;

		};

	}

}