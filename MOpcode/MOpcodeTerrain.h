#pragma once

#include "MOpcodeEntry.h"
#include "MWSceneNode.h"
#include "MWColMesh.h"
#include "MWPhysics.h"
#include "MOpcodeColObj.h"

namespace Myway {

	class MOpcodeTerrain : public MOpcodeColObj
	{
		DECLARE_ALLOC();
		DeclareRefObj();

	public:
		MOpcodeTerrain(void * uid, float scale);
		virtual ~MOpcodeTerrain();

		virtual MColType GetType() { return CT_Terrain; }

		void Build();

	protected:
		virtual void _getTriVertex(int faceIndex, Vec3 & v0, Vec3 & v1, Vec3 & v2)
		{
		}

	protected:
	};
}