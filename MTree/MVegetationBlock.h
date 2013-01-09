#pragma once

#include "MVegetation.h"
#include "MWSceneNode.h"

namespace Myway {

	class MVegetationBlock : public Mover
	{
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

		void UpdateGeometry();

		const RectF & GetRect() const { return mRect; }
		int GetXSection() const { return mXSection; }
		int GetZSection() const { return mZSection; }

		int GetRenderOpCount() const { return mNumOfRenderOps; }
		RenderOp * GetRenderOp(int index) const { d_assert (index < mNumOfRenderOps); return &mRenderOps[index]; }
		MVegetation * GetRenderVegetation(int index) const { d_assert (index < mNumOfRenderOps); return mRenderVegs[index]; }

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