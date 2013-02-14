#include "MTreeInstance.h"
#include "MForest.h"

namespace Myway {

	MTreeInstance::MTreeInstance(const TString128 & name)
		: Mover(name)
	{
		mTree = NULL;
		mLod = 0;
		mWindMatrixOffset = (float)Math::RandRange(0, int(MTreeGlobal::K_NumWindMatrix));
	}

	MTreeInstance::~MTreeInstance()
	{
	}

	void MTreeInstance::SetTree(const TString128 & source)
	{
		MTreePtr tree = MForest::Instance()->LoadTree(source);

		SetTree(tree);
	}

	void MTreeInstance::SetTree(MTreePtr tree)
	{
		if (mTree == tree)
			return ;

		mTree = tree;

		if (mTree != NULL)
		{
			const float * bound = tree->GetBoundingBox();
			mAabbLocal.minimum = Vec3(bound[0], bound[1], bound[2]);
			mAabbLocal.maximum = Vec3(bound[3], bound[4], bound[5]);

			mSphLocal.center = mAabbLocal.GetCenter();
			mSphLocal.radius = mSphLocal.center.Distance(mAabbLocal.minimum);

			if (mNode)
				mNode->_NotifyUpdate();
		}
	}

	MTreePtr MTreeInstance::GetTree()
	{
		return mTree;
	}

	void MTreeInstance::NotifyCamera(Camera * cam)
	{
	}

	void MTreeInstance::UpdateGeometry()
	{
	}

	void MTreeInstance::UpdateLod()
	{
		mLod = 0;
	}

	int MTreeInstance::GetLod()
	{
		return mLod;
	}

	void MTreeInstance::AddRenderQueue(RenderQueue * rq)
	{
		MForest::Instance()->_AddVisibleTreeInstance(this);
	}
}

