#pragma once

#include "MOpcodeEntry.h"
#include "MWSceneNode.h"
#include "MWColMesh.h"
#include "MWPhysics.h"
#include "MOpcodeColObj.h"

namespace Myway {

	class MOpcodeMesh : public MOpcodeColObj
	{
		DECLARE_ALLOC();
		DeclareRefObj();

	public:
		MOpcodeMesh(void * uid, float scale);
		virtual ~MOpcodeMesh();

		virtual MColType GetType() { return CT_Mesh; }

		void Build(ColMesh * mesh);
		void Build(MOpcodeMesh * rk);

		IColObj * Clone(float scale);

	protected:
		void _getTriVertex(int faceIndex, Vec3 & v0, Vec3 & v1, Vec3 & v2)
		{
			int index = faceIndex * 3;

			d_assert (index < mIndices.Size());

			int i0 = mIndices[index + 0];
			int i1 = mIndices[index + 1];
			int i2 = mIndices[index + 2];

			v0 = mVertices[i0];
			v1 = mVertices[i1];
			v2 = mVertices[i2];
		}

	protected:
		Opcode::MeshInterface mMeshInterface;

		Array<Vec3> mVertices;
		Array<int> mIndices;
	};
}