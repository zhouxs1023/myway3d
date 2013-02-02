#include "MBranch.h"
#include "MWVideoBufferManager.h"
#include "MWRenderHelper.h"

namespace Myway {


MBranch::MBranch()
{
	mLevel = 1;

	mInfo[0].MinRadius = 1;
	mInfo[0].MaxRadius = 3;
	mInfo[0].MinLength = 80;
	mInfo[0].MaxLength = 80;
	mInfo[0].Segments = 10;
	mInfo[0].Rings = 8;
	mInfo[0].UVTile = Vec2(3, 4);
	mInfo[0].StartGen = 0.5f;
	mInfo[0].EndGen = 1.0f;
	mInfo[0].MinSubAngle = 15.0f;
	mInfo[0].MaxSubAngle = 60.0f;
	mInfo[0].SubFregH = 2;
	mInfo[0].SubFregV = 5;

	mInfo[1].MinRadius = 0.1f;
	mInfo[1].MaxRadius = 1;
	mInfo[1].MinLength = 20;
	mInfo[1].MaxLength = 60;
	mInfo[1].Segments = 5;
	mInfo[1].Rings = 4;
	mInfo[1].UVTile = Vec2(1, 3);
	mInfo[1].StartGen = 0.5f;
	mInfo[1].EndGen = 1.0f;
	mInfo[1].MinSubAngle = 15.0f;
	mInfo[1].MaxSubAngle = 60.0f;
	mInfo[1].SubFregH = 2;
	mInfo[1].SubFregV = 5;

	SetControl(0, 0.0f);
	SetControl(1, 0.0f);
	SetControl(2, 0.0f);
	SetControl(3, 0.0f);

	mMaterialColor = Color4::White;
	
	mDiffuseMap = "Tree\\default_branch.tga";
	mNormalMap = "Tree\\default_branch_n.tga";
	mSpecularMap = "";
	mDetailMap = "";
}

MBranch::~MBranch()
{
}

void MBranch::SetControl(int index, float c)
{
	mBezierCurve.SetControl(index, c);
}

void MBranch::Compute()
{
	for (int i = 0; i < K_MaxSubLevel; ++i)
	{
		mVertex[i].Clear();
		mIndex[i].Clear();
	}

	_genTrunk();
	_genNormal();
}

void MBranch::_genTrunk()
{
	const Info & info = mInfo[0];

	Curve<float> curve(info.Segments + 1);

	mBezierCurve.ToCurve(curve);

	float BLength = Math::RandRange(info.MinLength, info.MaxLength);
	float SLength = BLength / info.Segments;

	for (int i = 0; i < info.Rings + 1; ++i)
	{
		Vec3 p1 = Vec3(curve.GetPoint(0), 0, 0) * BLength;

		float r = info.MaxRadius;
		float theta = i / float(info.Rings) * Math::PI_2;

		float x = Math::Cos(theta) * r;
		float z = Math::Sin(theta) * r;

		Vec3 xAxis = Vec3(1, 0, 0);
		Vec3 zAxis = Vec3(0, 0, 1);

		Vertex v;

		v.Position = p1 + x * xAxis + z * zAxis;
		v.Normal = (v.Position - p1).Normalize();
		v.UV.x = i / float(info.Rings);
		v.UV.y = 1;

		mVertex[0].PushBack(v);
	}

	for (int j = 1; j < info.Segments + 1; ++j)
	{
		Vec3 p0 = Vec3(curve.GetPoint(j - 1) * BLength, (j - 1) * SLength, 0);
		Vec3 p1 = Vec3(curve.GetPoint(j) * BLength, j * SLength, 0);

		p0 = p1 - p0;
		p0.NormalizeL();

		float t = j / float(info.Segments);
		float r = Math::Lerp(info.MinRadius, info.MaxRadius, 1 - t);

		for (int i = 0; i < info.Rings + 1; ++i)
		{
			float theta = i / float(info.Rings) * Math::PI_2;

			float x = Math::Cos(theta) * r;
			float z = Math::Sin(theta) * r;

			Vec3 xAxis = p0.CrossN(Vec3::UnitZ);
			Vec3 zAxis = Vec3::UnitZ;

			Vertex v;

			v.Position = p1 + x * xAxis + z * zAxis;
			v.Normal = (v.Position - p1).Normalize();
			v.UV.x = i / float(info.Rings);
			v.UV.y = 1 - j / (float)info.Segments;

			mVertex[0].PushBack(v);
		}
	}

	// build triangles
	for (int j = 0; j < info.Segments; ++j)
	{
		for (int i = 0; i < info.Rings; ++i)
		{
			int i0 = j * (info.Rings + 1) + i;
			int i1 = (j + 1) * (info.Rings + 1) + i;
			int i2 = i0 + 1;
			int i3 = i1 + 1;

			mIndex[0].PushBack(i0);
			mIndex[0].PushBack(i1);
			mIndex[0].PushBack(i2);

			mIndex[0].PushBack(i2);
			mIndex[0].PushBack(i1);
			mIndex[0].PushBack(i3);
		}
	}

	if (mLevel == 0)
		return ;

	{
		const Info & info = mInfo[0];

		int numFregH = info.SubFregH;
		int numFregV = info.SubFregV;
		float firstGenPoint = info.StartGen;
		float lastGenPoint = info.EndGen - 0.01f;

		float stepV = (lastGenPoint - firstGenPoint) / numFregV;

		for (int j = 0 ; j < numFregV; ++j)
		{
			float pointUnit = firstGenPoint + j * stepV;
			float pointUnit_n = pointUnit + stepV;

			// 从父干生成枝干的点.
			Vec2 pointGen = Vec2(curve.GetPointLerp(pointUnit) * BLength, pointUnit * BLength);
			Vec2 pointNext = Vec2(curve.GetPointLerp(pointUnit_n), pointUnit_n * BLength);;

			Vec3 upVec = Vec3(pointNext.x - pointGen.x, pointNext.y - pointGen.y, 0);
			float firstAngle = Math::RandRange(0.0f, Math::PI_2);
			float stepAngle = Math::PI_2 / numFregH;

			upVec.NormalizeL();
			Vec3 rotVec = Vec3::UnitZ;

			if (Math::Abs(rotVec.Dot(upVec)) > 0.999f)
			{
				if (upVec.z > 0)
					rotVec = Vec3::UnitX;
				else
					rotVec = -Vec3::UnitX;
			}

			Vec3 tmpVec = upVec.Cross(rotVec);
			rotVec = tmpVec.Cross(upVec);

			for (int i = 0; i < numFregH; ++i)
			{
				// 枝干的方向
				float bangle = Math::RandRange(info.MinSubAngle, info.MaxSubAngle);
				bangle = Math::DegreeToRadian(bangle);

				Quat r0, r1;

				r0.FromAxis(rotVec, bangle);
				r1.FromAxis(upVec, firstAngle + stepAngle * i);

				r0 = r1 * r0;

				Vec3 dir = r0.Rotate(upVec);

				_genSubLevel(1, Vec3(pointGen.x, pointGen.y, 0), dir, upVec);
			}
		}
	}
	
}

void MBranch::_genSubLevel(int level, const Vec3 & pos, const Vec3 & dir, const Vec3 & up)
{
	const Info & info = mInfo[level];

	float BLength = Math::RandRange(info.MinLength, info.MaxLength);
	Curve3 curve(info.Segments + 1);
	int startIndex = mVertex[level].Size();

	// generate curve
	{
		Vec3 tDir = dir;
		float tLen = BLength / info.Segments;

		for (int i = 0; i < info.Segments + 1; ++i)
		{
			Vec3 point = pos + (i * tLen) * tDir;
			curve.SetPoint(i, point);
		}
	}

	// generate vertex
	{
		for (int i = 0; i < info.Rings + 1; ++i)
		{
			Vec3 p1 = curve.GetPoint(0);

			float r = info.MaxRadius;
			float theta = i / float(info.Rings) * Math::PI_2;

			float x = Math::Cos(theta) * r;
			float z = Math::Sin(theta) * r;

			Vec3 yAxis = dir;
			Vec3 zAxis = up.CrossN(dir);
			Vec3 xAxis = yAxis.CrossN(zAxis);

			Vertex v;

			v.Position = p1 + x * xAxis + z * zAxis;
			v.Normal = (v.Position - p1).Normalize();
			v.UV.x = i / float(info.Rings);
			v.UV.y = 1;

			mVertex[level].PushBack(v);
		}

		for (int j = 1; j < info.Segments + 1; ++j)
		{
			Vec3 p0 = curve.GetPoint(j - 1);
			Vec3 p1 = curve.GetPoint(j);

			Vec3 yAxis = p1 - p0;
			yAxis.NormalizeL();
			Vec3 zAxis = up.CrossN(dir);
			Vec3 xAxis = yAxis.CrossN(zAxis);

			float t = j / float(info.Segments);
			float r = Math::Lerp(info.MinRadius, info.MaxRadius, 1 - t);

			for (int i = 0; i < info.Rings + 1; ++i)
			{
				float theta = i / float(info.Rings) * Math::PI_2;

				float x = Math::Cos(theta) * r;
				float z = Math::Sin(theta) * r;

				Vertex v;

				v.Position = p1 + x * xAxis + z * zAxis;
				v.Normal = (v.Position - p1).Normalize();
				v.UV.x = i / float(info.Rings);
				v.UV.y = 1 - j / float(info.Segments);

				mVertex[level].PushBack(v);
			}
		}
	}

	// generate index
	for (int j = 0; j < info.Segments; ++j)
	{
		for (int i = 0; i < info.Rings; ++i)
		{
			int i0 = j * (info.Rings + 1) + i;
			int i1 = (j + 1) * (info.Rings + 1) + i;
			int i2 = i0 + 1;
			int i3 = i1 + 1;

			mIndex[level].PushBack(i0 + startIndex);
			mIndex[level].PushBack(i1 + startIndex);
			mIndex[level].PushBack(i2 + startIndex);

			mIndex[level].PushBack(i2 + startIndex);
			mIndex[level].PushBack(i1 + startIndex);
			mIndex[level].PushBack(i3 + startIndex);
		}
	}
}

void MBranch::_genNormal()
{
	return ;
	for (int i = 0; i <= mLevel; ++i)
	{
		Array<Vertex> & vVertex = mVertex[i];
		Array<unsigned short> & vIndex = mIndex[i];

		int size = vVertex.Size();
		int tsize = vIndex.Size() / 3;

		for (int v = 0; v < size; ++v)
		{
			vVertex[v].Normal = Vec3::Zero;
		}

		for (int t = 0; t < tsize; ++t)
		{
			unsigned short t0 = vIndex[t * 3 + 0];
			unsigned short t1 = vIndex[t * 3 + 1];
			unsigned short t2 = vIndex[t * 3 + 2];

			Vertex & v0 = vVertex[t0];
			Vertex & v1 = vVertex[t1];
			Vertex & v2 = vVertex[t2];

			Vec3 dx = v1.Position - v0.Position;
			Vec3 dy = v2.Position - v0.Position;

			dx.NormalizeL();
			dy.NormalizeL();

			Vec3 n = dx.CrossN(dy);

			v0.Normal += n;
			v1.Normal += n;
			v2.Normal += n;
		}

		for (int v = 0; v < size; ++v)
		{
			vVertex[v].Normal.NormalizeL();
		}
	}
}

int MBranch::GetVertexCount()
{
	int count = 0;

	for (int i = 0; i <= mLevel; ++i)
	{
		count += mVertex[i].Size();
	}

	return count;
}

int MBranch::GetIndexCount()
{
	int count = 0;

	for (int i = 0; i <= mLevel; ++i)
	{
		count += mIndex[i].Size();
	}

	return count;
}















MTreeRT::MTreeRT()
{
}

MTreeRT::~MTreeRT()
{
}

//void MTreeRT::GetGeomtry(MTreeGeometry & geo)
//{
//	mBranch.GetGeomtry(geo);
//
//	const TString128 & branchDiffuseMap = mBranch.GetDiffuseMap();
//	const TString128 & branchNormalMap = mBranch.GetNormalMap();
//	const TString128 & branchSpecularMap = mBranch.GetSpecularMap();
//	const TString128 & branchDetailMap = mBranch.GetDetailMap();
//
//	mBranchTextrues[TT_Diffuse] = VideoBufferManager::Instance()->Load2DTexture(branchDiffuseMap, branchDiffuseMap);
//
//	if (branchNormalMap != "")
//		mBranchTextrues[TT_Normal] = VideoBufferManager::Instance()->Load2DTexture(branchNormalMap, branchNormalMap);
//	else
//		mBranchTextrues[TT_Normal] = RenderHelper::Instance()->GetDefaultNormalTexture();
//
//	if (branchSpecularMap != "")
//		mBranchTextrues[TT_Specular] = VideoBufferManager::Instance()->Load2DTexture(branchSpecularMap, branchSpecularMap);
//	else
//		mBranchTextrues[TT_Specular] = RenderHelper::Instance()->GetBlackTexture();
//
//	if (branchDetailMap != "")
//		mBranchTextrues[TT_Detail] = VideoBufferManager::Instance()->Load2DTexture(branchDetailMap, branchDetailMap);
//	else
//		mBranchTextrues[TT_Detail] = RenderHelper::Instance()->GetBlackTexture();
//}



}
