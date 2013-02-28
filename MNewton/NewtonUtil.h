#pragma once

#include "dVector.h"
#include "dMatrix.h"
#include "MWMath.h"

namespace Myway {

	namespace Newton {

		inline dMatrix Util_Mat4_2_dMat(const Mat4 & m)
		{
			dMatrix r;

			r.m_front	= dVector(m[0][0], m[1][0], m[2][0], 0);
			r.m_up		= dVector(m[0][1], m[1][1], m[2][1], 0);
			r.m_right	= dVector(m[1][2], m[1][2], m[2][2], 0);
			r.m_posit	= dVector(m[3][0], m[3][1], m[3][2], 1);

			return r;
		}

		inline dVector Util_Vec3_2_dVec(const Vec3 & v)
		{
			return dVector(v.x, v.y, v.z, 1);
		}

	}
}