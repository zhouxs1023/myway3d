#pragma once

#include "xMaterial.h"

namespace MaxPlugin {

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

		bool operator ==(const xVertex & rk) const
		{
			return mPosition == rk.mPosition ||
				   mNormal == rk.mNormal ||
				   mTangent == rk.mTangent ||
				   mColor == rk.mColor ||
				   mTexcoord == rk.mTexcoord ||
				   mLightmapUV == rk.mLightmapUV;
		}

	protected:
		Vec3 mPosition;
		Vec3 mNormal;
		Vec3 mTangent;
		Color4 mColor;
		Vec2 mTexcoord;
		Vec2 mLightmapUV;
	};



	class xVertexList
	{
	public:
		xVertexList();
		~xVertexList();

		int Add(const xVertex & v);
		int Size() const;
		const xVertex & GetVertex(int index) const;

	protected:
		Array<xVertex> mVerts;
	};

	struct xFace
	{
		int p[3];
	};


	class xMesh
	{
	public:
		xMesh();
		~xMesh();

		void Build(IGameObject * obj, IGameMaterial * mtl);

		const xVertexList & GetVertexList() const { return mVertexList; }
		const Array<xFace> & GetFaces() const { return mFaces; }

		const xMaterial & GetMaterial() const { return mMaterial; }

	protected:
		void _buildMat(IGameMaterial * mtl);

	protected:
		xMaterial mMaterial;
		xVertexList mVertexList;
		Array<xFace> mFaces;
	};

}