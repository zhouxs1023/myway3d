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

	class MOpcodeColObj : public IColObj
	{
	public:
		MOpcodeColObj(void * uid, float scale);
		virtual ~MOpcodeColObj();

		virtual bool RayCheck(HitInfoSet & info, const Ray & ray, const Mat4 & worldTm, unsigned int * cache);
		//virtual bool SweptSphereCheck(HitInfoSet & info, const SweptSphere & sph, const Mat4 & worldTm);

	protected:
		virtual void _getTriVertex(int faceIndex, Vec3 & v0, Vec3 & v1, Vec3 & v2) = 0;

		//bool _SweptSphereIntersection(CollisionPair * cp, const SweptSphere & sph, const Vec3 & v0, const Vec3 & v1, const Vec3 & v2);

	protected:
		Opcode::Model *	mCollisionModel;
	};

}