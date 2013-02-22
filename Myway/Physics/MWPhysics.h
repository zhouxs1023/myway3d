#pragma once

#include "MWMath.h"
#include "MWSceneNode.h"

namespace Myway {

	class HitInfo
	{
		Vec3 this_position;
		Vec3 this_normal;
		int this_faceId;

		Vec3 that_position;
		Vec3 that_normal;
		int that_faceId;
	};

	struct HitInfoSet
	{
		SceneNode * node;

		MColType colType;

		Vec3 r_that_position;				// only for ray trace
		Vec3 r_that_normal;					// only for ray trace
		int r_that_faceId;					// only for ray trace
		float r_distance;					// only for ray trace

		Array<HitInfo> hitInfos;

		HitInfoSet()
		{
			node = NULL;
			colType = CT_Unknown;

			r_that_position = Vec3::Zero;
			r_that_normal = Vec3::Zero;
			r_that_faceId = -1;
			r_distance = FLT_MAX;
		}
	};

	typedef Array<HitInfoSet> HitInfoSetArray;

	class IColObj : public RefObj
	{
	public:
		IColObj(void * uid, float scale) : mUId(uid), mScale(scale) {}
		virtual ~IColObj() {};

		virtual MColType GetType() = 0;

		void * GetUId() const { return mUId; }
		float GetScale() const { return mScale; }

		bool IsSameScale(float scale)
		{
			return Math::Abs(mScale - scale) < Math::EPSILON_E4;
		}

	protected:
		void * mUId;
		float mScale;
	};

	DeclareSmartPtr(IColObj);





	class MW_ENTRY IPhyWorld
	{
		DECLARE_SINGLETON(IPhyWorld);

	public:
		IPhyWorld();
		virtual ~IPhyWorld();

		virtual IColObjPtr AddColMesh(void * uId, ColMesh * colMesh, float scale)= 0; 
		virtual IColObjPtr GetColMesh(void * uId, float scale) = 0;
		virtual void RemoveColMesh(IColObj * obj) = 0;

		virtual void CreateTerrain() = 0;
		virtual void DestroyTerrain() = 0;

		virtual void AddNode(SceneNode * sceneNode, IColObjPtr colObj) = 0;
		virtual void RemoveNode(SceneNode * sceneNode) = 0;
		virtual void OnNodeScaleChanged(SceneNode * sceneNode) = 0;

		virtual void RayCheck(HitInfoSetArray & hitInfos, const Ray & ray, float dist, int flag, bool ifNoPhyData = false) = 0;
	};
}