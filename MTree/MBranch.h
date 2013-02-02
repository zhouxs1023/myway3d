#pragma once

#include "MWMath.h"
#include "MWTexture.h"

namespace Myway {

	class MBranch
	{
		DECLARE_ALLOC();

		static const int K_MaxSubLevel = 4;

	public:
		struct Vertex
		{
			Vec3 Position;
			Vec3 Normal;
			Vec3 Tangent;
			Vec2 UV;
		};

		struct Info
		{
			float MaxRadius, MinRadius;
			float MinLength, MaxLength;
			int Segments;
			int Rings;
			Vec2 UVTile;
			float StartGen;
			float EndGen;
			float MinSubAngle, MaxSubAngle;
			int SubFregH, SubFregV;
		};
		
	public:
		MBranch();
		~MBranch();

		int GetLevel() { return mLevel; }

		void SetDiffuseMap(const TString128 & source) { mDiffuseMap = source; }
		const TString128 & GetDiffuseMap() const { return mDiffuseMap; }

		void SetNormalMap(const TString128 & source) { mNormalMap = source; }
		const TString128 & GetNormalMap() const { return mNormalMap; }

		void SetSpecularMap(const TString128 & source) { mSpecularMap = source; }
		const TString128 & GetSpecularMap() const { return mSpecularMap; }

		void SetDetailMap(const TString128 & source) { mDetailMap = source; }
		const TString128 & GetDetailMap() const { return mDetailMap; }

		const Array<Vertex> & GetVertex(int level) { d_assert (level <= mLevel); return mVertex[level]; }
		const Array<unsigned short> & GetIndex(int level) { d_assert (level <= mLevel); return mIndex[level]; }

		int GetVertexCount();
		int GetIndexCount();

		void SetControl(int index, float c);

		void Compute();

	protected:
		void _genTrunk();
		void _genSubLevel(int level, const Vec3 & pos, const Vec3 & dir, const Vec3 & up);

		void _genNormal();

	protected:
		int mLevel;
		Info mInfo[K_MaxSubLevel];

		Color4 mMaterialColor;
		TString128 mDiffuseMap;
		TString128 mNormalMap;
		TString128 mSpecularMap;
		TString128 mDetailMap;

		BezierCurve<float> mBezierCurve;

		Array<Vertex> mVertex[K_MaxSubLevel];
		Array<unsigned short> mIndex[K_MaxSubLevel];
	};

	class MTreeRT
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
		MTreeRT();
		~MTreeRT();
		
		MBranch * GetBranch() { return &mBranch; }

	protected:
		MBranch mBranch;
	};
}