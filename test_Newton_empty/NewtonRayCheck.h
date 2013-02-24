#pragma once

#include "Myway.h"

namespace Myway {

	namespace Newton {

		class tWorld;
		class tBody;

		class tRayCheck
		{
		public:
			tRayCheck();
			virtual ~tRayCheck();

			void Do(tWorld * world, const Vec3 & start, const Vec3 & end);

			virtual bool UserFilter(tBody * body) { return true; }
			virtual bool UserCallback(tBody * body, float dist, const Vec3 & nrm, int collisionId) = 0;
		};


		class tRayCheckBase : public tRayCheck
		{
		public:
			class Info
			{
			public:
				tBody*	body;
				float	dist;	// range [0,1].
				Vec3	normal;
				int		collId;
				
				Info()
				{
					body = NULL;
					dist = FLT_MAX;
					normal = Vec3::Zero;
					collId = -1;
				}
			};

		public:
			tRayCheckBase() {}
			~tRayCheckBase() {}

			virtual bool UserCallback(tBody * body, float dist, const Vec3 & nrm, int collisionId);

			void Clear();

			const Info * GetFirst() const;

			int GetHitCount() const;
			const Info * GetInfo(int at) const;

		protected:
			Array<Info> mInfos;
		};
	}
}