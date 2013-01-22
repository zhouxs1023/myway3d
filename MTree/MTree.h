#pragma once

#include "MTreeEntry.h"
#include "MWResource.h"
#include "MWTexture.h"
#include "MWRenderer.h"
#include "MBranch.h"

namespace Myway {

	class MTREE_ENTRY MTree : public Resource, public RefObj
	{
		DECLARE_ALLOC();

	public:
		MTree(const TString128 & source);
		virtual ~MTree();

		virtual void DeleteSelf();

		virtual void Load();
		virtual void Reload();
		virtual void Unload();

		virtual void LoadImp(DataStreamPtr stream);

		void DoGenerate();
		MTreeRes * GetTreeResource();

		const Aabb & GetAabb() { return mBound; }
		const Sphere & GetSphere() { return mSphere; }

		RenderOp * _getBranchRenderOp() { return mRenderOp_Branch; }

	protected:
		void _setupBranchGeometry(MTreeGeometry & geo);
		void _calcuBound(MTreeGeometry & geo);

	protected:
		MTreeRes mResource;

		Aabb mBound;
		Sphere mSphere;

		RenderOp * mRenderOp_Branch;
	};

	DeclareSmartPtr(MTree);
}