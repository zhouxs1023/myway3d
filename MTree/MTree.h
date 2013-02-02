#pragma once

#include "MTreeEntry.h"
#include "MTreeGlobal.h"
#include "MWResource.h"
#include "MWTexture.h"
#include "MWRenderer.h"
#include "MWVertexBuffer.h"
#include "SpeedTree\\SpeedTreeRT.h"

namespace Myway {

	class MTREE_ENTRY MTree : public Resource, public RefObj
	{
		DECLARE_ALLOC();

	public:
		struct Material
		{
			Color4 Ambient;
			Color4 Diffuse;
			Color4 Specular;
			Color4 Emissive;
			float Power;

			TexturePtr DiffuseMap;
			TexturePtr NormalMap;
			TexturePtr SpecularMap;

			Material()
			{
				Ambient = Color4::Gray;
				Diffuse = Color4::White;
				Specular = Color4::Black;
				Emissive = Color4::Black;
				Power = 5;
			}

			void Set(const float* pMaterialArray)
			{
				memcpy(&Diffuse, pMaterialArray, 3 * sizeof(float));
				Diffuse.a = 1.0f;

				memcpy(&Ambient, pMaterialArray + 3, 3 * sizeof(float));
				Ambient.a = 1.0f;

				memcpy(&Specular, pMaterialArray + 6, 3 * sizeof(float));
				Specular.a = 1.0f;

				memcpy(&Emissive, pMaterialArray + 9, 3 * sizeof(float));
				Emissive.a = 1.0f;

				Power = pMaterialArray[12];
			}


		};

		struct BranchVertex
		{
			Vec3 Position;	
			Vec3 Normal;
			Vec3 Tangent;
			float TexCoords[2];
			float WindIndex;
			float WindWeight;			
		};

		struct LeafVertex
		{
			Vec3 Position;
			Vec3 Normal;
			float TexCoords[2];
			float WindIndex;
			float WindWeight;
			float PlacementIndex;
			float ScalarValue;
		};


	public:
		MTree(const TString128 & source);
		virtual ~MTree();

		virtual void DeleteSelf();

		virtual void Load();
		virtual void Reload();
		virtual void Unload();

		virtual void LoadImp(DataStreamPtr stream);

		const float * GetBoundingBox() const { return mBoundingBox; }

		Material * _getBranchMaterial() { return &mBranchMaterial; }
		Material * _getFrondMaterial()  { return &mFrondMaterial; }
		Material * _getLeafMaterial(int i)  { d_assert (i < mNumLeafs); return &mLeafMaterial[i]; }

		RenderOp * _getBranchRenderOp(int lod);
		RenderOp * _getLeafRenderOp(int lod);

		CSpeedTreeRT * _getSpeedTree() { return mSpeedTree; }

		void PrepareRenderLeaf(ShaderParam * bdTabel);

	protected:
		void _setupGeometry();
		void _setupTexture();

		void _setupBranchGeometry();
		void _setupLeafGeometry();

	protected:
		CSpeedTreeRT * mSpeedTree;
		CSpeedTreeRT::STextures * mTextureInfo;
		CSpeedTreeRT::SGeometry * mGeometryCache;

		float mBoundingBox[6];

		Material mBranchMaterial;
		Material mLeafMaterial[MTreeGlobal::K_MaxLeafPerTree];
		Material mFrondMaterial;

		int mNumBranchLods;
		unsigned short * mBranchIndexCounts;
		RenderOp * mRenderOp_Branch;

		int mNumLeafs;
		int mNumLeafLods;
		VertexBufferPtr * mLeafBuffers;
		RenderOp * mRenderOp_Leaf;

	};

	DeclareSmartPtr(MTree);
}