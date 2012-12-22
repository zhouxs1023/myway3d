#include "MTreeInstance.h"
#include "MForest.h"

namespace Myway {

	MTreeInstance::MTreeInstance(const TString128 & name)
		: Mover(name)
	{
		mWindMatrixOffset = float(int(Math::RandRange(0.0f, 1.0f) * 10.0f));
		mTree = NULL;
	}

	MTreeInstance::~MTreeInstance()
	{
	}

	void MTreeInstance::SetTree(const TString128 & source)
	{
		mTree = MForest::Instance()->LoadTree(source);
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

	void MTreeInstance::AddRenderQueue(RenderQueue * rq)
	{
		MForest::Instance()->_addVisibleTree(this);
	}
}

