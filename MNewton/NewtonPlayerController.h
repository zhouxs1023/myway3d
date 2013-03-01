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
#include "NewtonRayCheck.h"

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
				eOnIlligalRamp,
				eOnJump,
			};

		public:
			tPlayerController(SceneNode * node, tShape * player);
			~tPlayerController();

			void Update(float timeStep);

			void SetForwordVelocity(float v) { mForwordVelocity = v; }
			float GetForwordVelocity() const { return mForwordVelocity; }

			void SetSideVelocity(float v) { mSideVelocity = v; }
			float GetSideVelocity() const { return mSideVelocity; }

			void SetJumpVelocity(float v) { mJumpVelocity = v; }
			float GetJumpVelocity() const { return mJumpVelocity; }

			void SetMaxSlope(float degree) { mMaxSlope = degree; mMaxSlopeCos = Math::Cos(Math::DegreeToRadian(mMaxSlope)); }
			float GetMaxSlopeCos() const { return mMaxSlope; }

			void Jump();

		protected:
			void _upateState(float timeStep);
			bool _checkOnLand(const Vec3 & dest, Vec3 * normal = NULL, Vec3 * pos = NULL);

		protected:
			SceneNode * mNode;
			tShape * mShape;
			int mState;

			float mGravity;
			float mForwordVelocity;
			float mSideVelocity;
			float mJumpVelocity;

			float mInternalJumpVel;

			float mMaxSlope;
			float mMaxSlopeCos;

			float mPlayerHeight;
			float mLandLimit;
			float mJumpHighLimit;
			float mJumpLowLimit;

			tRayCheckBase mRayChecker;
		};
	}

}