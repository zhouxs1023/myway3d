#pragma once

#include "MOpcodeEntry.h"
#include "MOpcodeNode.h"
#include "MWPhysics.h"

namespace Opcode {
	class Model;
	class MeshInterface;
	class CollisionFaces;
}


namespace Myway {

	class MOpcodeScene : public IPhyWorld
	{
		static Opcode::CollisionFaces * mQueryResult;

	public:
		static Opcode::CollisionFaces * _getCollisionFaces() { return mQueryResult; }

	public:
		MOpcodeScene();
		~MOpcodeScene();

		virtual IColObjPtr AddColMesh(void * uId, ColMesh * colMesh, float scale);
		virtual IColObjPtr GetColMesh(void * uId, float scale);
		virtual void RemoveColMesh(IColObj * obj);

		virtual void AddNode(SceneNode * sceneNode, IColObjPtr colObj);
		virtual void RemoveNode(SceneNode * sceneNode);
		virtual void OnNodeScaleChanged(SceneNode * sceneNode);

		virtual PhyHitInfo RayTrace(const Ray & ray, float dist, int flag, bool ifNoPhyData);

	protected:
		Array<IColObj *> mColObjs;
	};

}