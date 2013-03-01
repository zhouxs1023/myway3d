#include "NewtonPlayerController.h"
#include "NewtonShape.h"
#include "NewtonBody.h"
#include "NewtonWorld.h"
#include "NewtonUtil.h"

#define _NT_MAX_CONTACTS 16

namespace Myway {

	namespace Newton {

		unsigned _convexCastBodyFilter(const NewtonBody* body, const NewtonCollision* collision, void* userData)
		{
			tPlayerController * ctrl = (tPlayerController *)userData;
			tBody * tbody = (tBody *)NewtonBodyGetUserData(body);

			return 1;
		}




		tPlayerController::tPlayerController(SceneNode * node, tShape * player)
		{
			mNode = node;
			mShape = player;
			mGravity = tWorld::Instance()->GetDefaultGravity();
			SetMaxSlope(60.0f);

			mForwordVelocity = 0;
			mSideVelocity = 0;
			mJumpVelocity = -mGravity * 0.5f;

			mInternalJumpVel = 0;

			mPlayerHeight = node->GetWorldAabb().GetSize().y;

			mJumpHighLimit = 0.9f;
			mJumpLowLimit = 0.1f;

			mLandLimit = 0.2f;

			mState = eOnFall;
		}

		tPlayerController::~tPlayerController()
		{
			//NewtonDestroyJoint(tWorld::Instance()->_getNewtonWorld(), mJoint);
		}

		void tPlayerController::Update(float timeStep)
		{
			if (timeStep > 1 / tWorld::Instance()->GetSimulateFPS())
				timeStep = 1 / tWorld::Instance()->GetSimulateFPS();

			_upateState(timeStep);

			if (mState == eOnLand)
			{
				//mNode->Move(timeStep * mForwordVelocity);
				//mNode->Right(timeStep * mSideVelocity);
			}
			else if (mState == eOnFall)
			{
				//mNode->Move(timeStep * mVelocity);
				mNode->Translate(0, timeStep * mGravity, 0);
			}
			else if (mState == eOnJump)
			{
				Vec3 nextStep;
				nextStep += mForwordVelocity * mNode->GetDirection();
				nextStep += mSideVelocity * mNode->GetRight();
				nextStep += mInternalJumpVel * Vec3::UnitY;


				mInternalJumpVel += timeStep * mGravity;
			}
		}

		void tPlayerController::Jump()
		{
			if (mState == eOnJump)
				return ;

			mState = eOnJump;
			mInternalJumpVel = mJumpVelocity;
		}

		void tPlayerController::_upateState(float timeStep)
		{
			Vec3 nextStep = Vec3::Zero;

			if (mState == eOnLand)
			{
				nextStep += mForwordVelocity * mNode->GetDirection();
				nextStep += mSideVelocity * mNode->GetRight();
			}
			else if (mState == eOnFall)
			{
				nextStep += Vec3::UnitY * mGravity;
			}
			else if (mState == eOnJump)
			{
				nextStep += mForwordVelocity * mNode->GetDirection();
				nextStep += mSideVelocity * mNode->GetRight();
				nextStep += mInternalJumpVel * Vec3::UnitY;
			}

			nextStep *= timeStep;

			const Vec3 & worldPos = mNode->GetPosition();
			NewtonWorld * ntWorld = tWorld::Instance()->_getNewtonWorld();

			NewtonWorldConvexCastReturnInfo info[_NT_MAX_CONTACTS];

			Mat4 worldTm;

			worldTm.MakeTranslate(worldPos.x, worldPos.y, worldPos.z);
			 
			Vec3 dest = worldPos + nextStep;
			float hitParam;

			// check on land
			if (mState == eOnJump)
			{
				int contacts = NewtonWorldConvexCast(ntWorld, worldTm[0], &dest.x, mShape->_getNewtonCollision(), &hitParam, this,
													 _convexCastBodyFilter, info, _NT_MAX_CONTACTS, 0);

				if (contacts)
				{
					float bestValue = info[0].m_point[1];
					float lowestValue = info[0].m_point[1];

					for (int i = 1; i < contacts; i ++)
					{
						float value = info[i].m_point[1];

						if (value > bestValue)
							bestValue = value;

						if (value < lowestValue)
							lowestValue = value;
					}

					if (mInternalJumpVel >= 0 && bestValue > mPlayerHeight * mJumpHighLimit)
						mInternalJumpVel = 0;

					/*if (mInternalJumpVel <= 0 && lowestValue < mPlayerHeight * mLandLimit)
					{
						mState = eOnLand;
					}
					*/

					Vec3 Normal, Position;
					if (mInternalJumpVel <= 0 && _checkOnLand(dest, &Normal, &Position))
					{
						mNode->SetPosition(Position);
						mState = eOnLand;
					}
				}

				mNode->SetPosition(dest);

				return ;
			}
			
			Vec3 Normal, Position;
			if (_checkOnLand(dest, &Normal, &Position))
			{
				int contacts = NewtonWorldConvexCast(ntWorld, worldTm[0], &dest.x, mShape->_getNewtonCollision(), &hitParam, this,
													 _convexCastBodyFilter, info, _NT_MAX_CONTACTS, 0);

				float bestValue = info[0].m_point[1];
				float lowestValue = info[0].m_point[1];

				for (int i = 1; i < contacts; i ++)
				{
					float value = info[i].m_point[1];

					if (value > bestValue)
						bestValue = value;

					if (value < lowestValue)
						lowestValue = value;
				}

				if (bestValue < dest.y + mPlayerHeight * mLandLimit)
					mState = eOnLand;
				else
					mState = eOnIlligalRamp;

				/*if (Normal.y < mMaxSlopeCos)
				{
				mState = eOnIlligalRamp;
				return ;
				}*/

				if (mState == eOnLand)
				{
					mNode->SetPosition(Position);
				}

				return ;
			}

			mNode->SetPosition(dest);
			
			mState = eOnFall;
		}
		

		bool tPlayerController::_checkOnLand(const Vec3 & dest, Vec3 * normal, Vec3 * pos)
		{
			float halfPlayerHight = mPlayerHeight * 0.5f;

			Vec3 start = dest, end = dest;

			start.y += halfPlayerHight;
			end.y -= halfPlayerHight * 0.01f;

			mRayChecker.Clear();
			mRayChecker.Do(start, end);

			if (mRayChecker.GetHitCount())
			{
				if (normal)
					*normal = mRayChecker.GetFirst()->normal;

				if (pos)
					*pos = start + (end - start) * mRayChecker.GetFirst()->dist;

				return true;
			}

			return false;
		}

	}

}