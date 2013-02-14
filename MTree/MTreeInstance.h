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

		void SetTree(const TString128 & treeSource);
		void SetTree(MTreePtr tree);
		MTreePtr GetTree();

		void Compute();

		void NotifyCamera(Camera * cam);
		void UpdateGeometry();

		void UpdateLod();
		int GetLod();

		void AddRenderQueue(RenderQueue * rq);

		inline float GetWindMatrixOffset() const { return mWindMatrixOffset; }

	protected:
		MTreePtr mTree;
		int mLod;
		float mWindMatrixOffset;
	};
}