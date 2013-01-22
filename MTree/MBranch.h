#pragma once

#include "MWMath.h"
#include "MWTexture.h"

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

		static const int K_MaxSubLevel = 4;

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

		void SetLevel(int level) { mLevel = level; }
		int GetLevel() const { return mLevel; }

		void SetDiffuseUVScale(const Vec2 & scale) { mDiffuseUVScale = scale; }
		const Vec2 & GetDiffuseUVScale() const { return mDiffuseUVScale; }

		void SetDiffuseMap(const TString128 & source) { mDiffuseMap = source; }
		const TString128 & GetDiffuseMap() const { return mDiffuseMap; }

		void SetNormalMap(const TString128 & source) { mNormalMap = source; }
		const TString128 & GetNormalMap() const { return mNormalMap; }

		void SetSpecularMap(const TString128 & source) { mSpecularMap = source; }
		const TString128 & GetSpecularMap() const { return mSpecularMap; }

		void SetDetailMap(const TString128 & source) { mDetailMap = source; }
		const TString128 & GetDetailMap() const { return mDetailMap; }

		void SetControl(int level, int index, const Vec2 & c);

		void GetGeomtry(MTreeGeometry & geo);

	protected:
		void _doGen(MTreeGeometry & geo, Mat4 form, int segments, int rings, float hight, float minR, float maxR, int subLevel);

	protected:
		float mMaxRadius, mMinRadius;
		float mHight;
		int mSegments;
		int mRings;
		int mLevel;
		Vec2 mDiffuseUVScale;
		TString128 mDiffuseMap;
		TString128 mNormalMap;
		TString128 mSpecularMap;
		TString128 mDetailMap;

		BezierCurve2 mBezierCurve[K_MaxSubLevel];
	};

	class MTreeRes
	{
		DECLARE_ALLOC();

	public:
		enum TextureType
		{
			TT_Diffuse,
			TT_Normal,
			TT_Specular,
			TT_Detail,
			TT_Max,
		};

	public:
		MTreeRes();
		~MTreeRes();

		MBranch * GetBranch() { return &mBranch; }

		void GetGeomtry(MTreeGeometry & geo);

		TexturePtr GetBranchTexture(TextureType type) { return mBranchTextrues[type]; }

	protected:
		MBranch mBranch;

		TexturePtr mBranchTextrues[TT_Max];
	};
}