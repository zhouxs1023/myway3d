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

#include "NewtonEntry.h"

namespace Myway {

	namespace Newton {

		class tShape;

		class MNEWTON_ENTRY tPlayerController
		{
			DECLARE_ALLOC();

		protected:
			enum eState {
				eOnFall,
				eOnLand,
				eOnIlligalRamp
			};

		public:
			tPlayerController(SceneNode * node, tShape * player);
			~tPlayerController();

			void Update(float timeStep);

			void SetVelocity(float v) { mVelocity = v; }
			float GetVelocity() const { return mVelocity; }

			void SetMaxSlope(float degree) { mMaxSlope = degree; mMaxSlopeCos = Math::Cos(Math::DegreeToRadian(mMaxSlope)); }
			float GetMaxSlopeCos() const { return mMaxSlope; }

		protected:
			void _upateState(float timeStep);

		protected:
			SceneNode * mNode;
			tShape * mShape;
			int mState;

			float mGravity;
			float mVelocity;
			float mMaxSlope;
			float mMaxSlopeCos;
		};
	}

}