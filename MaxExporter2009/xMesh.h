#pragma once

#include "xMaterial.h"

namespace MaxPlugin {

	struct xBlendWeight
	{
		float w[4];

		xBlendWeight() { w[0] = 1; w[1] = 0; w[2] = 0; w[3] = 0; }

		void normalize()
		{
			float sum = w[0] + w[1] + w[2] + w[3];

			if (sum > 0)
			{
				w[0] /= sum;
				w[1] /= sum;
				w[2] /= sum;
				w[3] /= sum;
			} 
		}

		bool operator == (const xBlendWeight & rk) const
		{
			return w[0] == rk.w[0] && w[1] == rk.w[1] && w[2] == rk.w[2] && w[3] == rk.w[3];
		}
	};

	struct xBlendIndex
	{
		unsigned char i[4];

		xBlendIndex() { i[0] = 0; i[1] = 0; i[2] = 0; i[3] = 0; }

		bool operator == (const xBlendIndex & rk) const
		{
			return i[0] == rk.i[0] && i[1] == rk.i[1] && i[2] == rk.i[2] && i[3] == rk.i[3];
		}
	};

	struct xMaxMesh
	{
		Array<Vec3> P;			// position
		Array<Vec3> N;			// normal
		Array<Vec4> C;			// vertex color
		Array<Vec2> UV;			// uv
		Array<Vec2> LUV;			// light map uv
		Array<xBlendWeight> BW;		// blend weight
		Array<xBlendIndex> BI;	// blend indices

		void Clear()
		{
			P.Clear();
			N.Clear();
			C.Clear();
			UV.Clear();
			LUV.Clear();
			BW.Clear();
			BI.Clear();
		}
	};

	class xVertex
	{
	public:
		xVertex();
		~xVertex();

		void SetPosition(const Vec3 & position) { mPosition = position; }
		const Vec3 & GetPosition() const { return mPosition; }

		void SetNormal(const Vec3 & normal) { mNormal = normal; }
		const Vec3 & GetNormal() const { return mNormal; }

		void SetTangent(const Vec3 & tangent) { mTangent = tangent; }
		const Vec3 & GetTangent() const { return mTangent; }

		void SetColor(const Color4 & color) { mColor = color; }
		const Color4 & GetColor() const { return mColor; }

		void SetTexcoord(const Vec2 & uv) { mTexcoord = uv; }
		const Vec2 & GetTexcoord() const { return mTexcoord; }

		void SetLightmapUV(const Vec2 & uv) { mLightmapUV = uv; }
		const Vec2 & GetLightmapUV() const { return mLightmapUV; }

		void SetBlendWeight(const xBlendWeight & bw) { mBlendWeight = bw; }
		const xBlendWeight & GetBlendWeight() const { return mBlendWeight; }

		void SetBlendIndex(const xBlendIndex & bi) { mBlendIndex = bi; }
		const xBlendIndex & GetBlendIndex() const { return mBlendIndex; }

		bool operator ==(const xVertex & rk) const
		{
			return mPosition == rk.mPosition &&
				mNormal == rk.mNormal &&
				mTangent == rk.mTangent &&
				mColor == rk.mColor &&
				mTexcoord == rk.mTexcoord &&
				mLightmapUV == rk.mLightmapUV &&
				mBlendWeight == rk.mBlendWeight&&
				mBlendIndex == rk.mBlendIndex;
		}

	protected:
		Vec3 mPosition;
		Vec3 mNormal;
		Vec3 mTangent;
		Color4 mColor;
		Vec2 mTexcoord;
		Vec2 mLightmapUV;
		xBlendWeight mBlendWeight;
		xBlendIndex mBlendIndex;
	};

	class xVertexList
	{
	public:
		xVertexList();
		~xVertexList();

		int Add(const xVertex & v);
		int Size() const;
		const xVertex & GetVertex(int index) const;
		xVertex & GetVertex(int index);

	protected:
		Array<xVertex> mVerts;
	};

	struct xFace
	{
		int p[3];
	};

	struct xSubMesh
	{
		int mVertexElems;
		xVertexList mVertexList;
		Array<xFace> mFaces;
		xMaterial mMaterial;
	};


	class xMesh
	{
	public:
		xMesh();
		~xMesh();

		void Extract(IGameNode * node);

		int GetSubMeshCount() { return mSubMeshes.Size(); }
		xSubMesh * GetSubMesh(int i) { return mSubMeshes[i]; }

	protected:
		void _extractSkinInfo(IGameObject * obj);
		void _extractSubMesh(IGameMesh * mesh);
		void _addFace(xSubMesh * subMesh, FaceEx * face);

	protected:
		void _buildMat(xSubMesh * subMesh, IGameMaterial * mtl);
		void _buildSkinInfo(IGameObject * obj);

	protected:
		int mVertexElems;
		xMaxMesh mMaxMesh;

		Array<xSubMesh*> mSubMeshes;
	};

}