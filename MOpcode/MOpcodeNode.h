#pragma once

#include "MOpcodeEntry.h"
#include "MWSceneNode.h"
#include "MWColMesh.h"
#include "MWPhysics.h"

namespace Opcode {
	class Model;
	class MeshInterface;
	class CollisionFaces;
}

namespace Myway {

	class MOpcodeMesh : public IColObj
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

		virtual bool RayTrace(PhyHitInfo & info, const Ray & ray, const Mat4 & worldTm, unsigned int * cache);

	protected:
		Opcode::Model *	mCollisionModel;
		Opcode::MeshInterface *	mMeshInterface;

		Array<Vec3> mVertices;
		Array<int> mIndices;
	};

	class MOpcodeNode
	{
	public:
		MOpcodeNode(SceneNode * node);
		~MOpcodeNode();

		virtual bool RayTrace(PhyHitInfo & info, const Ray & ray);

		void SetColObj(IColObjPtr obj) { mColObj = obj; }
		IColObjPtr GetColObj() { return mColObj; }
		SceneNode * GetSceneNode() { return mSceneNode; }

	protected:
		SceneNode * mSceneNode;
		IColObjPtr mColObj;
		unsigned int mCache;
	};
}