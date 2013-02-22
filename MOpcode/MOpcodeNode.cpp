#include "MOpcodeNode.h"
#include "MOpcodeScene.h"

namespace Myway {

	MOpcodeNode::MOpcodeNode(SceneNode * node)
		: mSceneNode(node)
		, mColObj(NULL)
		, mCache(0)
	{
	}

	MOpcodeNode::~MOpcodeNode()
	{
	}

	bool MOpcodeNode::RayCheck(HitInfoSet & info, const Ray & ray)
	{
		d_assert (mColObj != NULL);

		Mat4 worldTM;

		worldTM.MakeTransform(mSceneNode->GetWorldPosition(), mSceneNode->GetWorldOrientation(), Vec3::Unit);

		MOpcodeColObj * colObj = (MOpcodeColObj *)mColObj.c_ptr();

		return colObj->RayCheck(info, ray, worldTM, &mCache);
	}


}