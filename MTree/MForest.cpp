#include "MForest.h"
#include "MWRenderEvent.h"
#include "Engine.h"
#include "SpeedTreeExp.h"

namespace Myway {

	IMP_SLN (MForest);

	MForest::MForest()
		: OnPreVisibleCull(&RenderEvent::OnPreVisibleCull, this, &MForest::_preVisibleCull)
		, OnRender(&RenderEvent::OnAfterRenderSolid, this, &MForest::_render)
	{
		INIT_SLN;

		Init();
	}

	MForest::~MForest()
	{
		SHUT_SLN;

		Shutdown();
	}

	void MForest::Init()
	{
		mWind.Load("SpeedTree\\SpeedWind.ini");

		mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("SpeedTree.ShaderLib", "SpeedTree\\SpeedTree.ShaderLib");
		d_assert (mShaderLib);

		mTech_Branch = mShaderLib->GetTechnique("Branch");

		d_assert (mTech_Branch);

		mParam_Branch.Bind(mTech_Branch);
	}

	void MForest::Shutdown()
	{
		d_assert(mTreeInstances.Size() == 0);
		d_assert(mTrees.Size() == 0);
	}

	void MForest::_preVisibleCull(void*, void*)
	{
		mVisibleInsts.Clear();
	}

	void MForest::Advance()
	{
		float time = Engine::Instance()->GetTime();

		mWind.Advance(time);

		const Vec3 & camPos = World::Instance()->MainCamera()->GetPosition();
		const Vec3 & camDir = World::Instance()->MainCamera()->GetDirection();

		SpeedTreeExp::SetCamera((float *)&camPos, (float *)&camDir);
	}

	void MForest::_render(void*, void*)
	{
		mWind._getWindMaxtrix(mMatWind);

		for (int i = 0; i < mVisibleInsts.Size(); ++i)
		{
			_renderBranch(mVisibleInsts[i]);
		}
	}

	void MForest::_renderBranch(MTreeInstance * inst)
	{
		MTreePtr tree = inst->GetTree();

		if (tree == NULL || tree->GetLoadState() != Resource::LOADED)
			return ;

		RenderOp * rop = tree->GetRenderOp_Branch();

		if (rop == NULL)
			 return ;

		Node * node = inst->GetAttachNode();

		d_assert (node == NULL);

		const Vec3 & position = node->GetPosition();
		const Quat & rotation = node->GetOrientation();
		const Vec3 & scale = node->GetScale();

		Vec4 posNscale = Vec4(position, scale.x);
		Mat4 matRotation = rotation.ToMatrix();
		Mat4 * matWind = mMatWind;
		float windMatOff = inst->GetWindMatrixOffset();

		mParam_Branch.uWindMatOff->SetFloat(windMatOff);
		mParam_Branch.uWindMatrix->SetMatrix(matWind, MTreeGlobal::K_NumWindMatrices);
		mParam_Branch.uPosition->SetVector(posNscale);
		mParam_Branch.uMatRotation->SetMatrix(matRotation);

		RenderSystem::Instance()->Render(mTech_Branch, rop);
	}


	void MForest::Clear()
	{
		for (int i = 0; mTreeInstances.Size(); ++i)
		{
			delete mTreeInstances[i];
		}

		mTreeInstances.Clear();
	}

	void MForest::_addVisibleTree(MTreeInstance * inst)
	{
		mVisibleInsts.PushBack(inst);
	}

	MTreeInstance * MForest::CreateTreeInstance(const TString128 & name)
	{
		d_assert (GetTreeInstance(name) == NULL);

		MTreeInstance * inst = new MTreeInstance(name);

		mTreeInstances.PushBack(inst);

		return inst;
	}

	MTreeInstance * MForest::GetTreeInstance(const TString128 & name)
	{
		for (int i = 0; i < mTreeInstances.Size(); ++i)
		{
			if (mTreeInstances[i]->GetName() == name)
			{
				return mTreeInstances[i];
			}
		}

		return NULL;
	}

	void MForest::DestroyTreeInstance(MTreeInstance * inst)
	{
		MTreeInstance * instDelete = NULL;

		for (int i = 0; i < mTreeInstances.Size(); ++i)
		{
			if (mTreeInstances[i] == inst)
			{
				mTreeInstances.Erase(i);
				break;
			}
		}

		d_assert (instDelete != NULL);

		delete instDelete;
	}

	MTreePtr MForest::LoadTree(const TString128 & sourceName)
	{
		MTreePtr tree = GetTree(sourceName);

		if (tree != NULL)
			return tree;

		tree = new MTree(sourceName);
		tree->Load();

		mTrees.PushBack(tree.c_ptr());

		return tree;
	}

	void MForest::DestroyTree(MTree * tree)
	{
		MTree * treeDelete = NULL;

		for (int i = 0; i < mTrees.Size(); ++i)
		{
			if (mTrees[i] == tree)
			{
				treeDelete = tree;
				mTrees.Erase(i);
				break;
			}
		}

		d_assert (treeDelete != NULL);

		delete treeDelete;
	}

	MTreePtr MForest::GetTree(const TString128 & sourceName)
	{
		for (int i = 0; i < mTrees.Size(); ++i)
		{
			if (mTrees[i]->GetSourceName() == sourceName)
				return mTrees[i];
		}

		return NULL;
	}








	MForestListener gForestListener;

	MForestListener::MForestListener()
		: OnInit(&RenderEvent::OnEngineInit, this, &MForestListener::_init)
		, OnShutdown(&RenderEvent::OnEngineShutdown, this, &MForestListener::_shutdown)
		, OnUpdate(&RenderEvent::OnPreUpdateScene, this, &MForestListener::_update)
	{
	}

	MForestListener::~MForestListener()
	{
	}

	void MForestListener::_init(void * param0, void * param1)
	{
		mForest = new MForest();
	}

	void MForestListener::_shutdown(void * param0, void * param1)
	{
		delete mForest;
	}

	void MForestListener::_update(void * param0, void * param1)
	{
		mForest->Advance();
	}
}

