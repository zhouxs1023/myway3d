#pragma once

#include "MWPhysics.h"

#include "MOpcodeEntry.h"
#include "MOpcodeMesh.h"
#include "MOpcodeTerrain.h"
#include "MOpcodeNode.h"

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

		Opcode::AABBTreeCollider opcTreeCollider;
		Opcode::RayCollider      opcRayCollider;
		Opcode::SphereCollider   opcSphereCollider;
		Opcode::PlanesCollider   opcPlanesCollider;
		Opcode::LSSCollider      opcSweptSphereCollider;
		Opcode::BVTCache         opcTreeCache;
		Opcode::CollisionFaces   opcFaceCache;
		Opcode::SphereCache      opcSphereCache;
		Opcode::PlanesCache      opcPlanesCache;
		Opcode::LSSCache         opcSweptSphereCache;


	public:
		MOpcodeScene();
		~MOpcodeScene();

		virtual IColObjPtr AddColMesh(void * uId, ColMesh * colMesh, float scale);
		virtual IColObjPtr GetColMesh(void * uId, float scale);
		virtual void RemoveColMesh(IColObj * obj);

		virtual void CreateTerrain();
		virtual void DestroyTerrain();

		virtual void AddNode(SceneNode * sceneNode, IColObjPtr colObj);
		virtual void RemoveNode(SceneNode * sceneNode);
		virtual void OnNodeScaleChanged(SceneNode * sceneNode);

		virtual void RayCheck(HitInfoSetArray & hitInfos, const Ray & ray, float dist, int flag, bool ifNoPhyData);

	protected:
		Array<IColObj *> mColObjs;
		MOpcodeTerrain * mTerrain;
	};

}