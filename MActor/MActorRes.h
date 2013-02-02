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
			Vec2 LightmapUV;
			Vec4 Tangent;
		};

		struct VertexSkined
		{
			Vec3 Position;
			Vec3 Normal;
			Vec2 Texcoord;
			Vec4 Tangent;
			float BIndices[4];
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


	public:
		MActorRes(const TString128 & source);
		virtual ~MActorRes();

		virtual void Load();
		virtual void Reload();
		virtual void Unload();

		virtual void LoadImp(DataStreamPtr stream);

		virtual void DeleteSelf();

		int GetMeshCount() const { return mMeshes.Size(); }
		SMesh * GetMesh(int i) const { return mMeshes[i]; }
		const Aabb & GetAabb() const { return mAabb; }
		const Sphere & GetSphere() const { return mSphere; }

		MMotion * LoadMotion(const TString128 & motionSource);
		MMotion * GetMotion(const TString128 & motionName);

		EMotionFX::Actor * GetEMotionActor() { return mActor; }

	protected:
		void _init();
		void _shutdown();

	protected:
		EMotionFX::Actor * mActor;

		Array<SMesh*> mMeshes;
		Aabb mAabb;
		Sphere mSphere;

		Array<MMotion *> mMotionSet;
	};

	DeclareSmartPtr(MActorRes);

}