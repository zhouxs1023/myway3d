#pragma once

#include "MSpeedTreeEntry.h"
#include "MTree.h"
#include "MWMover.h"

namespace Myway {

	class MTREE_ENTRY MTreeInstance : public Mover
	{
		DECLARE_ALLOC();
		
	public:
		MTreeInstance(const TString128 & name);
		virtual ~MTreeInstance();

		void SetTree(const TString128 & source);
		MTreePtr GetTree();

		void NotifyCamera(Camera * cam);
		void UpdateGeometry();

		void AddRenderQueue(RenderQueue * rq);

		float GetWindMatrixOffset() { return mWindMatrixOffset; }

	protected:
		MTreePtr mTree;
		float mWindMatrixOffset;
	};
}