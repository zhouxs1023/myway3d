#pragma once

#include "MOpcodeEntry.h"
#include "MWMath.h"
#include "MWColMesh.h"

namespace Opcode {
	class Model;
	class MeshInterface;
	class CollisionFaces;
}

namespace Myway {

	class MOpcodeScene
	{
	public:
		MOpcodeScene();
		~MOpcodeScene();

		void AddMesh(ColMesh * mesh, const Mat4 & form);

		void Build();
		void Clear();

		bool RayTrace(const Ray & ray, float * dist, unsigned int * cache, int * mtlId = 0, Vec3 * colPos = 0, Vec3 * colNml = 0);

	protected:
		Array<Vec3> mVertices;
		Array<int> mTriangles;
		Array<int> mMateirals;

		Opcode::CollisionFaces *	mQueryResult;
		Opcode::Model *				mCollisionModel;
		Opcode::MeshInterface *		mMeshInterface;
	};

}