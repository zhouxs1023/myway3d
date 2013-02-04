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

		MActor * CreateActor(const TString128 & name);
		MActor * CreateActor(const TString128 & name, const TString128 & source, float priority = 0);
		MActor * CreateActorU(const TString128 & source, float priority = 0);
		void DestroyActor(MActor * actor);
		MActor * GetActor(const TString128 & name);

		MActorResPtr _LoadActorRes(const TString128 & source, float priority);
		MActorRes * GetActorRes(const TString128 & source);
		void _DeleteActorRes(MActorRes * res);

		void _addVisibleActor(MActor * actor);

	protected:
		void _Init(void *, void *);
		void _Shutdown(void *, void *);
		void _PreVisibleCull(void *, void *);

		void _Render(void *, void *);
		void _RenderDepth(void *, void *);

		void _drawMesh(MActorRes::SMesh * mesh);
		void _drawMeshSkined(MActorRes::SMesh * mesh);

	protected:
		tEventListener<MActorManager> OnInit;
		tEventListener<MActorManager> OnShutdown;
		tEventListener<MActorManager> OnPreVisibleCull;
		tEventListener<MActorManager> OnRender;

		VertexDeclarationPtr mVertexDecl;
		VertexDeclarationPtr mVertexDeclSkined;

		Array<MActorRes *> mActorResources;
		Array<MActor *> mActors;

		Array<MActor *> mVisibleActors;

		ShaderLib * mShaderLib;
		Technique * mTech_Deferred;
		Technique * mSkinedTech_Deferred;
		int mUId;
	};

}