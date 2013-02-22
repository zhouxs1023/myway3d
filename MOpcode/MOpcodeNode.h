#pragma once

#include "MOpcodeEntry.h"
#include "MWSceneNode.h"
#include "MWColMesh.h"
#include "MWPhysics.h"
#include "MOpcodeColObj.h"

namespace Opcode {
	class Model;
	class MeshInterface;
	class CollisionFaces;
}

namespace Myway {

	class MOpcodeNode
	{
	public:
		MOpcodeNode(SceneNode * node);
		~MOpcodeNode();

		virtual bool RayCheck(HitInfoSet & info, const Ray & ray);

		void SetColObj(IColObjPtr obj) { mColObj = obj; }
		IColObjPtr GetColObj() { return mColObj; }
		SceneNode * GetSceneNode() { return mSceneNode; }

	protected:
		SceneNode * mSceneNode;
		IColObjPtr mColObj;
		unsigned int mCache;
	};
}