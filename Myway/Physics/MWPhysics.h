#pragma once

#include "MWMath.h"
#include "MWSceneNode.h"

namespace Myway {

	struct PhyHitInfo
	{
		SceneNode * node;
		bool Hitted;
		float Distance;
		Vec3 Normal;
		int MaterialId;

		PhyHitInfo()
		{
			node = NULL;
			Hitted = false;
			Distance = FLT_MAX;
			Normal = Vec3::Zero;
			MaterialId = -1;
		}
	};

	class IColObj : public RefObj
	{
	public:
		IColObj(void * uid, float scale) : mUId(uid), mScale(scale) {}
		virtual ~IColObj() {};

		virtual MColType GetType() = 0;

		virtual bool RayTrace(PhyHitInfo & info, const Ray & ray, const Mat4 & worldTm, unsigned int * cache) = 0;

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

		virtual void AddNode(SceneNode * sceneNode, IColObjPtr colObj) = 0;
		virtual void RemoveNode(SceneNode * sceneNode) = 0;
		virtual void OnNodeScaleChanged(SceneNode * sceneNode) = 0;

		virtual PhyHitInfo RayTrace(const Ray & ray, float dist, int flag, bool ifNoPhyData = false) = 0;
	};
}