#include "MBranch.h"

namespace Myway {


MBranch::MBranch()
{
	mMaxRadius = 50; mMinRadius = 10;
	mHight = 500;

	mSegments = 60;
	mRings = 15;

	SetControl(0, Vec2(0, 0.0f));
	SetControl(1, Vec2(0, 0.5f));
	SetControl(2, Vec2(0, 0.5f));
	SetControl(3, Vec2(0, 1.0f));
}

MBranch::~MBranch()
{
}

void MBranch::SetControl(int index, const Vec2 & c)
{
	mBezierCurve.SetControl(index, c);
}

void MBranch::GetGeomtry(MTreeGeometry & geo)
{
	geo.BranchVerts.Clear();
	geo.BranchFaces.Clear();

	// build vertex
	Curve2 curve(mSegments + 1);

	mBezierCurve.ToCurve(curve);

	for (int i = 0; i < mRings + 1; ++i)
	{
		Vec3 p1 = Vec3(curve.GetPoint(0).x, curve.GetPoint(0).y, 0) * mHight;

		float r = mMaxRadius;
		float theta = i / float(mRings) * Math::PI_2;

		float x = Math::Cos(theta);
		float z = Math::Sin(theta);

		Vec3 xAxis = Vec3(1, 0, 0);
		Vec3 zAxis = Vec3(0, 0, 1);

		MTreeVertex v;

		v.Position = p1 + x * xAxis + z * zAxis;
		v.Normal = (v.Position - p1).Normalize();

		geo.BranchVerts.PushBack(v);
	}

	for (int j = 1; j < mSegments + 1; ++j)
	{
		for (int i = 0; i < mRings + 1; ++i)
		{
			float t = j / float(mSegments);

			Vec3 p0 = Vec3(curve.GetPoint(j - 1).x, curve.GetPoint(j - 1).y, 0) * mHight;
			Vec3 p1 = Vec3(curve.GetPoint(j).x, curve.GetPoint(j).y, 0) * mHight;

			p0 = p1 - p0;
			p0.NormalizeL();

			float r = Math::Lerp(mMaxRadius, mMinRadius, t);
			float theta = i / float(mRings) * Math::PI_2;

			float x = Math::Cos(theta);
			float z = Math::Sin(theta);

			Vec3 xAxis = p0.CrossN(Vec3::UnitZ);
			Vec3 zAxis = Vec3::UnitZ;

			MTreeVertex v;

			v.Position = p1 + x * xAxis + z * zAxis;
			v.Normal = (v.Position - p1).Normalize();

			geo.BranchVerts.PushBack(v);
		}
	}

	// build triangles
	for (int j = 0; j < mSegments; ++j)
	{
		for (int i = 0; i < mRings; ++i)
		{
			int i0 = j * (mRings + 1) + i;
			int i1 = i0 + 1;
			int i2 = (j + 1) * (mRings + 1) + i;
			int i3 = i2 + 1;

			MTreeFace f0 = { i0, i2, i1 };
			MTreeFace f1 = { i1, i2, i3 };

			geo.BranchFaces.PushBack(f0);
			geo.BranchFaces.PushBack(f1);
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
}



}
