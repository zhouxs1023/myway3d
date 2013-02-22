#pragma once

#ifdef MOPCODE_EXPORT

#define MOPCODE_ENTRY __declspec(dllexport)

#else

#define MOPCODE_ENTRY __declspec(dllimport)

#endif

#include "MWMatrix4.h"
#include "MWVector3.h"
#include "Opcode\\Opcode.h"

namespace Myway {

	struct MOpcodeUtil
	{
		IceMaths::Matrix4x4 ToIceMat4(const Mat4 & m)
		{
			return *(IceMaths::Matrix4x4*)&m;
		}

		IceMaths::Point ToIcePoint(const Vec3 & v)
		{
			return *(IceMaths::Point*)&v;
		}
	};

}

namespace Opcode {
	class Model;
	class MeshInterface;
	class CollisionFaces;
}