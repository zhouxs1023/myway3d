#pragma once

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
		void SetTree(MTreePtr tree);
		MTreePtr GetTree();

		void Compute();

		void NotifyCamera(Camera * cam);
		void UpdateGeometry();

		void AddRenderQueue(RenderQueue * rq);

	protected:
		MTreePtr mTree;
	};
}