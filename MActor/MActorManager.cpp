#include "MActorManager.h"
#include "MWRenderEvent.h"
#include "MWRenderSystem.h"
#include "MWRenderHelper.h"
#include "MActorUtility.h"
#include <EMotionFX.h>

namespace Myway {

	IMP_SLN(MActorManager);

	MActorManager gActorManager;

	MActorManager::MActorManager()
		: OnInit(&RenderEvent::OnEngineInit, this, &MActorManager::_Init)
		, OnShutdown(&RenderEvent::OnEngineShutdown, this, &MActorManager::_Shutdown)
		, OnPreVisibleCull(&RenderEvent::OnPreVisibleCull, this, &MActorManager::_PreVisibleCull)
		, OnRender(&RenderEvent::OnRenderSolid2, this, &MActorManager::_Render)
		, mUId(0)
	{
		INIT_SLN;
	}

	MActorManager::~MActorManager()
	{
		SHUT_SLN;
	}

	void MActorManager::_Init(void *, void *)
	{
		mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Actor\\Actor.ShaderLib", "Actor\\Actor.ShaderLib");

		d_assert (mShaderLib != NULL);

		mTech_Deferred = mShaderLib->GetTechnique("ActorDeferred");
		mSkinedTech_Deferred = mShaderLib->GetTechnique("ActorDeferredSkined");

		d_assert (mTech_Deferred != NULL && mSkinedTech_Deferred != NULL);
	}

	void MActorManager::_Shutdown(void *, void *)
	{
		d_assert (mActorResources.Size() == 0);

		mVertexDecl = NULL;
		mVertexDeclSkined = NULL;
	}

	void MActorManager::_PreVisibleCull(void *, void *)
	{
		mVisibleActors.Clear();
	}

	VertexDeclarationPtr MActorManager::GetVertexDeclaration(bool skined)
	{
		if (mVertexDecl == NULL)
		{
			mVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
			mVertexDecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
			mVertexDecl->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
			mVertexDecl->AddElement(0, 24, DT_FLOAT2, DU_TEXCOORD, 0);
			mVertexDecl->AddElement(0, 32, DT_FLOAT2, DU_TEXCOORD, 1);
			mVertexDecl->AddElement(0, 40, DT_FLOAT4, DU_TANGENT, 0);

			mVertexDecl->Init();
		}

		if (mVertexDeclSkined == NULL)
		{
			mVertexDeclSkined = VideoBufferManager::Instance()->CreateVertexDeclaration();
			mVertexDeclSkined->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
			mVertexDeclSkined->AddElement(0, 12, DT_FLOAT3, DU_NORMAL, 0);
			mVertexDeclSkined->AddElement(0, 24, DT_FLOAT2, DU_TEXCOORD, 0);
			mVertexDeclSkined->AddElement(0, 32, DT_FLOAT4, DU_TEXCOORD, 1);
			mVertexDeclSkined->AddElement(0, 48, DT_UBYTE4, DU_TEXCOORD, 2);
			mVertexDeclSkined->AddElement(0, 52, DT_FLOAT4, DU_TEXCOORD, 3);
			mVertexDeclSkined->Init();
		}

		if (skined)
		{
			return mVertexDeclSkined;
		}
		else
		{
			return mVertexDecl;
		}
	}

	MActor * MActorManager::CreateActor(const TString128 & name)
	{
		d_assert (GetActor(name) == NULL);

		MActor * actor = new MActor(name);

		mActors.PushBack(actor);

		return actor;
	}

	MActor * MActorManager::CreateActor(const TString128 & name, const TString128 & source, float priority)
	{
		d_assert (GetActor(name) == NULL);

		MActorResPtr res = _LoadActorRes(source, priority);

		MActor * actor = new MActor(name);

		actor->SetPart(0, res);

		mActors.PushBack(actor);

		return actor;
	}

	MActor * MActorManager::CreateActorU(const TString128 & source, float priority)
	{
		MActorResPtr res = _LoadActorRes(source, priority);

		MActor * actor = new MActor(TString128("UId") + mUId++);

		actor->SetPart(0, res);

		mActors.PushBack(actor);

		return actor;
	}

	void MActorManager::DestroyActor(MActor * actor)
	{
		for (int i = 0; i < mActors.Size(); ++i)
		{
			if (mActors[i] == actor)
			{
				delete actor;
				mActors.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	MActor * MActorManager::GetActor(const TString128 & name)
	{
		for (int i = 0; i < mActors.Size(); ++i)
		{
			if (mActors[i]->GetName() == name)
			{
				return mActors[i];
			}
		}

		return NULL;
	}

	MActorResPtr MActorManager::_LoadActorRes(const TString128 & source, float priority)
	{
		MActorRes * res = GetActorRes(source);

		if (res == NULL)
		{
			res = new MActorRes(source);
			res->SetPriority(priority);
			res->Load();

			mActorResources.PushBack(res);
		}

		return res;
	}

	MActorRes * MActorManager::GetActorRes(const TString128 & source)
	{
		for (int i = 0; i < mActorResources.Size(); ++i)
		{
			if (mActorResources[i]->GetSourceName() == source)
			{
				return mActorResources[i];
			}
		}

		return NULL;
	}

	void MActorManager::_DeleteActorRes(MActorRes * res)
	{
		for (int i = 0; i < mActorResources.Size(); ++i)
		{
			if (mActorResources[i] == res)
			{
				delete res;
				mActorResources.Erase(i);
				return ;
			}
		}

		d_assert (0);
	}

	void MActorManager::_addVisibleActor(MActor * actor)
	{
		mVisibleActors.PushBack(actor);
	}

	void MActorManager::_Render(void * p0, void * p1)
	{
		RenderSystem * render = RenderSystem::Instance();

		render->_BeginEvent("Render Actor");

		MCore::Matrix matBlend[255];

		for (int i = 0; i < mVisibleActors.Size(); ++i)
		{
			MActor * actor = mVisibleActors[i];
			Node * node = actor->GetAttachNode();
			MActorResPtr res = actor->GetPart(0);
			EMotionFX::Actor * eactor = actor->_GetMainActor();

			if (res == NULL)
				continue;

			for (int j = 0; j < res->GetMeshCount(); ++j)
			{
				MActorRes::SMesh * mesh = res->GetMesh(j);

				int boneCount = mesh->BoneArray.Size();

				for (int b = 0; b < boneCount; ++b)
				{
					EMotionFX::Node * bone = eactor->GetNodeByID(mesh->BoneArray[b]->GetID());

					d_assert (bone != NULL);

					matBlend[b] = bone->GetInvBoneTM() * bone->GetWorldTM();
				}

				RenderRegister::Instance()->SetBlendMatrix((const Mat4 *)matBlend, boneCount);

				for (int p = 0; p < mesh->PrimCount; ++p)
				{
					MActorRes::SPrim & prim = mesh->Primitives[p];
					EMotionFX::Node * enode = eactor->GetNodeByID(prim.NodeId);
					MActorRes::SMtl * mtl = res->GetMaterial(prim.MaterialId);

					Mat4 worldTM;

					MActorUtility::ToMat4(worldTM, enode->GetWorldTM());

					prim.Rop.xform = worldTM;

					SamplerState state;

					render->SetTexture(0, state, mtl->DiffuseMap.c_ptr());

					render->Render(boneCount > 0 ? mSkinedTech_Deferred : mTech_Deferred, &prim.Rop);

				} // end of p

			} // end of for j

		} // end of for i

		render->_EndEvent();
	}

	void MActorManager::_RenderDepth(void *, void *)
	{
	}
}