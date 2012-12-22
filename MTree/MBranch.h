#pragma once

#include "MWMath.h"

namespace Myway {

	struct MTreeVertex
	{
		Vec3 Position;
		Vec3 Normal;
		Vec3 Tangent;
		Vec2 UV;
	};

	struct MTreeFace
	{
		short p0;
		short p1;
		short p2;
	};

	struct MTreeGeometry
	{
		Array<MTreeVertex> BranchVerts;
		Array<MTreeFace> BranchFaces;
	};

	class MBranch
	{
		DECLARE_ALLOC();

	public:
		MBranch();
		~MBranch();

		void SetRadius(float minRadius, float maxRadius) { mMinRadius = minRadius; mMaxRadius = maxRadius; }
		void GetRadius(float & minRadius, float & maxRadius) const { minRadius = mMinRadius; maxRadius = mMaxRadius; }

		void SetHight(float hight) { mHight = hight; }
		float GetHight() const { return mHight; }

		void SetSegments(int segs) { mSegments = segs; }
		int GetSegments() const { return mSegments; }

		void SetRings(int rings) { mRings = rings; }
		int GetRings() const { return mRings; }

		void SetControl(int index, const Vec2 & c);

		void GetGeomtry(MTreeGeometry & geo);

	protected:
		float mMaxRadius, mMinRadius;
		float mHight;
		int mSegments;
		int mRings;

		BezierCurve2 mBezierCurve;
	};

	class MTreeRes
	{
		DECLARE_ALLOC();

	public:
		MTreeRes();
		~MTreeRes();

		MBranch * GetBranch() { return &mBranch; }

		void GetGeomtry(MTreeGeometry & geo);

	protected:
		MBranch mBranch;
	};
}