//////////////////////////////////////////////////////////////////////////
//
// SpeedTreeRT Integration.
//   SpeedTreeRT engine: http://www.idvinc.com
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
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

		void SetLeafDiffuse(const Color4 & c) { mLeafDiffuse = c; }
		void SetFrondDiffuse(const Color4 & c) { mFrondDiffuse = c; }
		void SetBranchDiffuse(const Color4 & c) { mBranchDiffuse = c; }

		const Color4 & GetLeafDiffuse() const { return mLeafDiffuse; }
		const Color4 & GetFrondDiffuse() const { return mFrondDiffuse; }
		const Color4 & GetBranchDiffuse() const { return mBranchDiffuse; }

	protected:
		MTreePtr mTree;
		int mLod;
		float mWindMatrixOffset;

		Color4 mLeafDiffuse;
		Color4 mFrondDiffuse;
		Color4 mBranchDiffuse;
	};
}