#pragma once

#include "MVegetation.h"

namespace Myway {

	class MVegetationBlock
	{
		struct Inst
		{
			MVegetation * Veg;
			Vec3 Position;
			float Size;
		};

	public:
		MVegetationBlock(int xSection, int ySection, const RectF & rc);
		~MVegetationBlock();

		void AddVegetation(MVegetation * veg, const Vec3 & position, float size);

		void ReInitGeometry();

		const Aabb & GetBound() const { return mBound; }
		const RectF & GetRect() const { return mRect; }
		int GetXSection() const { return mXSection; }
		int GetZSection() const { return mZSection; }

		int GetRenderOpCount() const { return mNumOfRenderOps; }
		RenderOp * GetRenderOp(int index) const { d_assert (index < mNumOfRenderOps); return &mRenderOps[index]; }
		int GetRenderOpGeoType(int index) const { d_assert (index < mNumOfRenderOps); return mGeoTypes[index]; }

	protected:
		void _mergeVeg_Mesh(List<Inst> & Vegs, int index);
		void _mergeVeg_Billboard(List<Inst> & Vegs, int index);
		void _mergeVeg_X2(List<Inst> & Vegs, int index);

	protected:
		Aabb mBound;
		RectF mRect;
		int mXSection;
		int mZSection;

		List<Inst> mInsts;

		int mNumOfRenderOps;
		RenderOp * mRenderOps;
		int * mGeoTypes;
	};

}