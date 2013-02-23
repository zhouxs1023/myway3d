//////////////////////////////////////////////////////////////////////////
//
// Newton Integration.
//   newton physics engine: http://newtondynamics.com/forum/newton.php
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "Myway.h"
#include "Newton.h"

namespace Myway {

	namespace Newton {

		class tWorld 
		{
		public:
			tWorld();
			~tWorld();

			void Init();
			void Shutdown();
			void Update();

			void SetWorldSize(const Vec3 & vMin, const Vec3 & vMax);

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