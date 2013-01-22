#include "MBranch.h"
#include "MWVideoBufferManager.h"
#include "MWRenderHelper.h"

namespace Myway {


MBranch::MBranch()
{
	mMaxRadius = 3; mMinRadius = 1;
	mHight = 80;

	mSegments = 60;
	mRings = 15;

	mLevel = 1;

	mDiffuseUVScale = Vec2(3, 4);

	for (int i = 0; i < K_MaxSubLevel; ++i)
	{
		SetControl(i, 0, Vec2(0, 0.0f));
		SetControl(i, 1, Vec2(0, 0.5f));
		SetControl(i, 2, Vec2(0, 0.5f));
		SetControl(i, 3, Vec2(0, 1.0f));
	}
	
	mDiffuseMap = "Tree\\default_branch.tga";
	mNormalMap = "Tree\\default_branch_n.tga";
	mSpecularMap = "";
	mDetailMap = "";
}

MBranch::~MBranch()
{
}

void MBranch::SetControl(int level, int index, const Vec2 & c)
{
	d_assert (level < K_MaxSubLevel);

	mBezierCurve[level].SetControl(index, c);
}

void MBranch::GetGeomtry(MTreeGeometry & geo)
{
	geo.BranchVerts.Clear();
	geo.BranchFaces.Clear();

	_doGen(geo, Mat4::Identity, mSegments, mRings, mHight, mMinRadius, mMaxRadius, 0);
}

void MBranch::_doGen(MTreeGeometry & geo, Mat4 form, int segments, int rings, float hight, float minR, float maxR, int subLevel)
{
	// build vertex
	Curve2 curve(segments + 1);

	mBezierCurve[subLevel].ToCurve(curve);

	float length = 0.0000000001f;

	for (int i = 1; i < segments + 1; ++i)
	{
		Vec3 p0 = Vec3(curve.GetPoint(i - 1).x, curve.GetPoint(i - 1).y, 0) * hight;
		Vec3 p1 = Vec3(curve.GetPoint(i).x, curve.GetPoint(i).y, 0) * hight;

		length += p1.Distance(p0);
	}

	float tcoord_v = 1;

	for (int i = 0; i < rings + 1; ++i)
	{
		Vec3 p1 = Vec3(curve.GetPoint(0).x, curve.GetPoint(0).y, 0) * hight;

		float r = maxR;
		float theta = i / float(rings) * Math::PI_2;

		float x = Math::Cos(theta) * r;
		float z = Math::Sin(theta) * r;

		Vec3 xAxis = Vec3(1, 0, 0);
		Vec3 zAxis = Vec3(0, 0, 1);

		MTreeVertex v;

		v.Position = p1 + x * xAxis + z * zAxis;
		v.Normal = (v.Position - p1).Normalize();
		v.UV.x = i / float(rings);
		v.UV.y = tcoord_v;

		geo.BranchVerts.PushBack(v);
	}

	for (int j = 1; j < segments + 1; ++j)
	{
		Vec3 p0 = Vec3(curve.GetPoint(j - 1).x, curve.GetPoint(j - 1).y, 0) * hight;
		Vec3 p1 = Vec3(curve.GetPoint(j).x, curve.GetPoint(j).y, 0) * hight;

		tcoord_v -= p1.Distance(p0) / length;

		p0 = p1 - p0;
		p0.NormalizeL();

		float t = j / float(segments);
		float r = Math::Lerp(maxR, minR, t);

		for (int i = 0; i < rings + 1; ++i)
		{
			float theta = i / float(rings) * Math::PI_2;

			float x = Math::Cos(theta) * r;
			float z = Math::Sin(theta) * r;

			Vec3 xAxis = p0.CrossN(Vec3::UnitZ);
			Vec3 zAxis = Vec3::UnitZ;

			MTreeVertex v;

			v.Position = p1 + x * xAxis + z * zAxis;
			v.Normal = (v.Position - p1).Normalize();
			v.UV.x = i / float(rings);
			v.UV.y = tcoord_v;

			geo.BranchVerts.PushBack(v);
		}
	}

	// build triangles
	for (int j = 0; j < segments; ++j)
	{
		for (int i = 0; i < rings; ++i)
		{
			int i0 = j * (rings + 1) + i;
			int i1 = (j + 1) * (rings + 1) + i;
			int i2 = i0 + 1;
			int i3 = i1 + 1;

			MTreeFace f0 = { i0, i1, i2 };
			MTreeFace f1 = { i2, i1, i3 };

			geo.BranchFaces.PushBack(f0);
			geo.BranchFaces.PushBack(f1);
		}
	}

	// build sub level
	if (subLevel >= mLevel)
		return;

	int K_BranchRing = 3;
	int K_BranchSegment = 5;

	float K_StartSubBranch = 0.45f;
	float K_EndSubBranch = 0.9f;

	for (int j = 0; j < K_BranchSegment; ++j)
	{
		for (int i = 0; i < K_BranchRing; ++i)
		{
			
		}
	}
}

























MTreeRes::MTreeRes()
{
}

MTreeRes::~MTreeRes()
{
}

void MTreeRes::GetGeomtry(MTreeGeometry & geo)
{
	mBranch.GetGeomtry(geo);

	const TString128 & branchDiffuseMap = mBranch.GetDiffuseMap();
	const TString128 & branchNormalMap = mBranch.GetNormalMap();
	const TString128 & branchSpecularMap = mBranch.GetSpecularMap();
	const TString128 & branchDetailMap = mBranch.GetDetailMap();

	mBranchTextrues[TT_Diffuse] = VideoBufferManager::Instance()->Load2DTexture(branchDiffuseMap, branchDiffuseMap);

	if (branchNormalMap != "")
		mBranchTextrues[TT_Normal] = VideoBufferManager::Instance()->Load2DTexture(branchNormalMap, branchNormalMap);
	else
		mBranchTextrues[TT_Normal] = RenderHelper::Instance()->GetDefaultNormalTexture();

	if (branchSpecularMap != "")
		mBranchTextrues[TT_Specular] = VideoBufferManager::Instance()->Load2DTexture(branchSpecularMap, branchSpecularMap);
	else
		mBranchTextrues[TT_Specular] = RenderHelper::Instance()->GetBlackTexture();

	if (branchDetailMap != "")
		mBranchTextrues[TT_Detail] = VideoBufferManager::Instance()->Load2DTexture(branchDetailMap, branchDetailMap);
	else
		mBranchTextrues[TT_Detail] = RenderHelper::Instance()->GetBlackTexture();
}



}
