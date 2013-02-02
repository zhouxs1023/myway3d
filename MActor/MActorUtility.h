#pragma once

#include <EMotionFX.h>
#include "MWMath.h"

namespace Myway {

	struct MActorUtility
	{
		static void ToMat4(Mat4 & m, const MCore::Matrix & em)
		{
			memcpy(&m, &em, sizeof(m));
		}

		static void ToBoneMatrix(Mat3x4 & m, const MCore::Matrix & em)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int i = 0; i < 4; ++i)
				{
					m.m[j][i] = em.m44[i][j];
				}
			}
		}
	};
}
