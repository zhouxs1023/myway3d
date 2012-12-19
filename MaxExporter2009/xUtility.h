#pragma once

namespace MaxPlugin {

	struct xUtility
	{
		static Vec3 ToVec3(const Point3 & p)
		{
			//return Vec3(p.x, p.z, p.y);
			return Vec3(p.x, p.y, p.z);
		}

		static MQuat ToQuat(const ::Quat & q)
		{
			/*float x, y, z;
			q.GetEuler(&x, &y, &z);

			MQuat mq, mz;

			mz.FromAxis(Vec3::UnitZ, -Math::PI_1_2);
			mq.FromPitchYawRoll(x, y, z);

			mq *= mz;

			return mq;*/

			MQuat mq = MQuat(-q.x, -q.y, -q.z, q.w);

			return mq;
		}
	};

}