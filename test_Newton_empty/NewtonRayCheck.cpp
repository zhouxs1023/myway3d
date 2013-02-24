#include "NewtonRayCheck.h"
#include "NewtonWorld.h"

namespace Myway {

	namespace Newton {

		static float newtonRaycastFilter(const NewtonBody* body, const float* hitNormal, int collisionID, void* userData, float dist)
		{
			tBody * meBody = (tBody *)NewtonBodyGetUserData(body);
			tRayCheck * rayCheck = (tRayCheck *)userData;
			const Vec3 & normal = *(const Vec3 *)hitNormal;

			if (rayCheck->UserCallback(meBody, dist, normal, collisionID))
				return dist;
			else
				return 1.1f;
		}

		static unsigned newtonRaycastPreFilter( const NewtonBody * body, const NewtonCollision* collision, void* userData )
		{
			tBody * meBody = (tBody *)NewtonBodyGetUserData(body);
			tRayCheck * rayCheck = (tRayCheck *)userData;

			if (rayCheck->UserFilter(meBody))
				return 1;
			else
				return 0;
		}

		tRayCheck::tRayCheck()
		{
		}

		tRayCheck::~tRayCheck()
		{
		}

		void tRayCheck::Do(tWorld * world, const Vec3 & start, const Vec3 & end)
		{
			NewtonWorldRayCast(world->_getNewtonWorld(), &start.x, &end.x, newtonRaycastFilter, this, newtonRaycastPreFilter);
		}













		bool tRayCheckBase::UserCallback(tBody * body, float dist, const Vec3 & nrm, int collisionId)
		{
			Info newinfo;

			newinfo.body = body;
			newinfo.dist = dist;
			newinfo.normal = nrm;
			newinfo.collId = collisionId;

			mInfos.PushBack(newinfo);

			return false;
		}

		void tRayCheckBase::Clear()
		{
			mInfos.Clear();
		}

		const tRayCheckBase::Info * tRayCheckBase::GetFirst() const
		{
			if (mInfos.Size() == 0)
				return NULL;

			int index = 0;

			for (int i = 1; i < mInfos.Size(); ++i)
			{
				if (mInfos[index].dist > mInfos[i].dist)
					index = i;
			}

			return &mInfos[index];
		}

		int tRayCheckBase::GetHitCount() const
		{
			return mInfos.Size();
		}

		const tRayCheckBase::Info * tRayCheckBase::GetInfo(int at) const
		{
			return &mInfos[at];
		}
	}

}