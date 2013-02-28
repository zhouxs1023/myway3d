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
			SetMaxSlope(30.0f);

			mState = eOnFall;
		}

		tPlayerController::~tPlayerController()
		{
			//NewtonDestroyJoint(tWorld::Instance()->_getNewtonWorld(), mJoint);
		}

		void tPlayerController::Update(float timeStep)
		{
			_upateState(timeStep);

			if (mState == eOnLand)
			{
				mNode->Move(timeStep * mVelocity); 
			}
			else if (mState == eOnFall)
			{
				//mNode->Move(timeStep * mVelocity);
				mNode->Translate(0, timeStep * mGravity, 0);
			}
		}

		void tPlayerController::_upateState(float timeStep)
		{
			const Vec3 & worldPos = mNode->GetPosition();
			NewtonWorld * ntWorld = tWorld::Instance()->_getNewtonWorld();

			NewtonWorldConvexCastReturnInfo info[_NT_MAX_CONTACTS];

			Mat4 worldTm;

			worldTm.MakeTranslate(worldPos.x, worldPos.y, worldPos.z);

			Vec3 dest = worldPos + mVelocity * timeStep;
			float hitParam;

			int contacts = NewtonWorldConvexCast(ntWorld, worldTm[0], &dest.x, mShape->_getNewtonCollision(), &hitParam, this,
				                  _convexCastBodyFilter, info, _NT_MAX_CONTACTS, 0);

			// check on land
			if (contacts)
			{
				Vec3 upDir = Vec3(0, 1, 0);
				int bestContact = 0;

				Vec3 normalOnHit = Vec3(info[0].m_normalOnHitPoint[0], info[0].m_normalOnHitPoint[1], info[0].m_normalOnHitPoint[2]);
				dFloat bestValue = upDir.Dot(normalOnHit);

				for (int i = 1; i < contacts; i ++) {

					dFloat value = upDir.Dot(info[i].m_normalOnHitPoint);

					if (value < bestValue) {

						bestContact = i;
						bestValue = value;
					}
				}

				Vec3 normal; 
				
				normal.x = info[bestContact].m_normalOnHitPoint[0];
				normal.y = info[bestContact].m_normalOnHitPoint[1];
				normal.z = info[bestContact].m_normalOnHitPoint[2];

				float dt = upDir.Dot(normal);

				if (dt < mMaxSlopeCos) {
					mState = eOnIlligalRamp;
				} else {
					mState = eOnLand;
				}
			}
			else
				mState = eOnFall;
		}
		
	}

}