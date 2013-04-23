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

#include "MVegetation.h"
#include "MWSceneNode.h"

namespace Myway {

	class MVegetationBlock : public Mover
	{
	public:
		struct Inst
		{
			MVegetation * Veg;
			Vec3 Position;
			float Size;
		};

		typedef MVegetation *MVegetationPtr;

	public:
		MVegetationBlock(int xSection, int ySection, const RectF & rc);
		~MVegetationBlock();

		virtual void AddRenderQueue(RenderQueue * rq);

		void AddVegetation(MVegetation * veg, const Vec3 & position, float size);
		void RemoveVegetation(MVegetation * veg, const RectF & rc);

		const RectF & GetRect() const { return mRect; }
		int GetXSection() const { return mXSection; }
		int GetZSection() const { return mZSection; }

		int GetRenderOpCount() const { return mNumOfRenderOps; }
		RenderOp * GetRenderOp(int index) const { d_assert (index < mNumOfRenderOps); return &mRenderOps[index]; }
		MVegetation * GetRenderVegetation(int index) const { d_assert (index < mNumOfRenderOps); return mRenderVegs[index]; }

		void _OnVegRemoved(MVegetation * veg);
		void _OnVegChanged(MVegetation * veg);
		void _UpdateGeometry();

		int _getInstanceSize()  { return mInsts.Size(); }
		List<Inst> & _getInstList() { return mInsts; }
		void _notifyNeedUpdate() { mNeedUpdate = TRUE; }

	protected:
		void _mergeVeg_Mesh(List<Inst> & Vegs, int index);
		void _mergeVeg_Billboard(List<Inst> & Vegs, int index);
		void _mergeVeg_X2(List<Inst> & Vegs, int index);

		void _calcuBound();

	protected:
		RectF mRect;
		int mXSection;
		int mZSection;

		List<Inst> mInsts;

		int mNumOfRenderOps;
		RenderOp * mRenderOps;
		MVegetationPtr * mRenderVegs;
		SceneNode * mSceneNode;

		bool mNeedUpdate;
	};

}