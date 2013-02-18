#pragma once

#include "MActorEntry.h"
#include "MWResource.h"
#include "MWRenderer.h"
#include "MMotion.h"

namespace EMotionFX {
	class Actor;
	class Motion;
	class Node;
}

namespace Myway {

	class MACTOR_ENTRY MActorRes : public RefObj, public Resource
	{
	public:
		static const int K_MaxLod = 4;
		static const int K_NumNoSkinShaderConst = 24;

		struct Vertex
		{
			Vec3 Position;
			Vec3 Normal;
			Vec2 Texcoord;
			Vec4 Tangent;
		};

		struct VertexSkined
		{
			Vec3 Position;
			Vec3 Normal;
			Vec2 Texcoord;
			Vec4 Tangent;
			unsigned char BIndices[4];
			float BWeights[4];
		};

		struct SPrim
		{
			DECLARE_ALLOC();

			int		NodeId;
			int		MaterialId;

			RenderOp Rop;
		};

		struct SMesh
		{
			int PrimCount;
			SPrim * Primitives;
			Array<EMotionFX::Node *> BoneArray;

			SMesh()
			{
				PrimCount = 0;
				Primitives = NULL;
			}

			~SMesh()
			{
				delete[] Primitives;
			}
		};

		struct SMtl
		{
			bool DoubleSide;
			float Opacity;

			TexturePtr DiffuseMap;
			TexturePtr NormalMap;
			TexturePtr SpecularMap;

			SMtl()
			{
				DoubleSide = false;
				Opacity = 1;
			}
		};


	public:
		MActorRes(const TString128 & source);
		virtual ~MActorRes();

		virtual void Load();
		virtual void Reload();
		virtual void Unload();

		virtual void LoadImp(DataStreamPtr stream);

		virtual void DeleteSelf();

		int GetMeshCount() const { return mMeshCount; }
		SMesh * GetMesh(int i) const { d_assert (i < mMeshCount); return &mMeshes[i]; }

		int GetMaterialCount() const { return mMaterialCount; }
		SMtl * GetMaterial(int i) const { d_assert (i < mMaterialCount); return &mMaterials[i]; }

		int GetMotionCount() const { return mMotionCount; }
		MMotion * GetMotion(int i) const { d_assert (i < mMotionCount); return &mMotionSet[i]; }
		MMotion * GetMotion(const TString128 & motionName);

		const Aabb & GetAabb() const { return mAabb; }
		const Sphere & GetSphere() const { return mSphere; }

		EMotionFX::Actor * GetEMotionActor() { return mActor; }

	protected:
		void _init();
		void _shutdown();
		void _initMat();

		MMotion * _loadMotion(const TString128 & motionSource);

	protected:
		EMotionFX::Actor * mActor;

		int mMeshCount;
		SMesh* mMeshes;

		int mMaterialCount;
		SMtl * mMaterials;

		int mMotionCount;
		MMotion * mMotionSet;

		Aabb mAabb;
		Sphere mSphere;
	};

	DeclareSmartPtr(MActorRes);

}