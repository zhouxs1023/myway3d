#include "MTreeInstance.h"
#include "MForest.h"

namespace Myway {

	MTreeInstance::MTreeInstance(const TString128 & name)
		: Mover(name)
	{
		mTree = NULL;
	}

	MTreeInstance::~MTreeInstance()
	{
	}

	void MTreeInstance::SetTree(const TString128 & source)
	{
		if (mTree != NULL && mTree->GetSourceName() == source)
			return ;

		mTree = MForest::Instance()->LoadTree(source);

		Compute();
	}

	void MTreeInstance::SetTree(MTreePtr tree)
	{
		d_assert (tree != NULL);

		if (mTree != tree)
		{
			mTree = tree;

			Compute();
		}
	}

	MTreePtr MTreeInstance::GetTree()
	{
		return mTree;
	}

	void MTreeInstance::Compute()
	{
		if (mTree == NULL)
			return ;

		mTree->DoGenerate();

		SetBounds(mTree->GetAabb(), mTree->GetSphere());

		if (mNode)
			mNode->_NotifyUpdate();
	}

	void MTreeInstance::NotifyCamera(Camera * cam)
	{
	}

	void MTreeInstance::UpdateGeometry()
	{
	}

	void MTreeInstance::AddRenderQueue(RenderQueue * rq)
	{
		MForest::Instance()->_AddVisibleTreeInstance(this);
	}
}

