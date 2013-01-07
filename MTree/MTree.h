#pragma once

#include "MTreeEntry.h"
#include "MWMath.h"
#include "MWResource.h"
#include "MTreeGlobal.h"
#include "MWTexture.h"
#include "MWRenderer.h"

class CSpeedTreeRT;

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

		RenderOp * GetRenderOp_Branch() { return mRenderOp_Branch; }
		RenderOp * GetRenderOp_Frond() { return mRenderOp_Frond; }
		RenderOp * GetRenderOp_Leaf() { return mRenderOp_Leaf; }

	protected:
		void _calcuBound();
		void _loadTexture();
		void _setupBranchGeometry();
		void _setupFrondGeometry();
		void _setupLeafGeometry();
		void _setupBillboardGeometry();

	protected:
		CSpeedTreeRT * mSpeedTree;
		Aabb mBound;
		Sphere mSphere;

		TexturePtr mBranchTexture[MTreeGlobal::K_NumLeafAngles];
		
		/*TexturePtr mFrondTexture[CSpeedTreeRT::TL_NUM_TEX_LAYERS];
		TexturePtr mLeafTexture[CSpeedTreeRT::TL_NUM_TEX_LAYERS];
		TexturePtr mBillboradTexture[CSpeedTreeRT::TL_NUM_TEX_LAYERS];*/

		RenderOp * mRenderOp_Branch;
		RenderOp * mRenderOp_Frond;
		RenderOp * mRenderOp_Leaf;
		RenderOp * mRenderOp_Billboard;
	};

	DeclareSmartPtr(MTree);
}