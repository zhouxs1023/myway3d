//////////////////////////////////////////////////////////////////////////
//
// EMotionFX Integration.
//   EMotionFX engine : http://www.mysticgd.com
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MActorEntry.h"
#include "MActorRes.h"
#include "MActor.h"

namespace Myway {

	class MACTOR_ENTRY MActorManager
	{
		DECLARE_SINGLETON(MActorManager);

	public:
		MActorManager();
		~MActorManager();

		VertexDeclarationPtr GetVertexDeclaration(bool skined);
		VertexDeclarationPtr GetStaticMeshVertexDeclaration();

		MActor * CreateActor(const TString128 & name);
		MActor * CreateActor(const TString128 & name, const TString128 & source, float priority = 0);
		MActor * CreateActorU(const TString128 & source, float priority = 0);
		void DestroyActor(MActor * actor);
		MActor * GetActor(const TString128 & name);

		MActorResPtr _LoadActorRes(const TString128 & source, float priority);
		MActorResPtr GetActorRes(const TString128 & source);
		void _DeleteActorRes(MActorRes * res);

		void _addVisibleActor(MActor * actor);

	protected:
		void _Init(Event * sender);
		void _Shutdown(Event * sender);
		void _PreVisibleCull(Event * sender);

		void _Render(Event * sender);
		void _RenderDepth(Event * sender);

		void _drawMesh(MActorRes::SMesh * mesh);
		void _drawMeshSkined(MActorRes::SMesh * mesh);

	protected:
		tEventListener<MActorManager> OnInit;
		tEventListener<MActorManager> OnShutdown;
		tEventListener<MActorManager> OnPreVisibleCull;
		tEventListener<MActorManager> OnRender;
		tEventListener<MActorManager> OnRenderDepth;

		VertexDeclarationPtr mVertexDecl;
		VertexDeclarationPtr mVertexDeclSkined;
		VertexDeclarationPtr mVertexDeclStaticMesh;

		Array<MActorRes *> mActorResources;
		Array<MActor *> mActors;

		Array<MActor *> mVisibleActors;

		ShaderLib * mShaderLib;
		Technique * mTech_Deferred;
		Technique * mSkinedTech_Deferred;

		Technique * mTech_Depth;
		Technique * mSkinedTech_Depth;
		int mUId;
	};

}