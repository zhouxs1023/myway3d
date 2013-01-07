#include "MForest.h"
#include "Engine.h"
#include "MWRenderEvent.h"

namespace Myway {

	IMP_SLN (MForest);

	MForest::MForest()
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
		mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Tree.ShaderLib", "Tree\\Tree.ShaderLib");
		d_assert (mShaderLib);

		mTech_VegMesh = mShaderLib->GetTechnique("GrassMesh");
		mTech_VegBillboard = mShaderLib->GetTechnique("GrassBillboasrd");
		mTech_VegX2 = mShaderLib->GetTechnique("GrassX2");

		d_assert (mTech_VegX2);

		MVegetation * veg = new MVegetation();

		veg->Name = "Default";
		veg->Type = MVegetation::GT_X2;
		veg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture("Tree\\g1.tga", "Tree\\g1.tga");

		mVegetations.PushBack(veg);
	}

	void MForest::Shutdown()
	{
		for (int i = 0; i < mVegetationBlocks.Size(); ++i)
		{
			delete mVegetationBlocks[i];
		}

		mVegetationBlocks.Clear();

		for (int i = 0; i < mVegetations.Size(); ++i)
		{
			delete mVegetations[i];
		}

		mVegetations.Clear();
	}

	void MForest::_preVisibleCull(void*, void*)
	{
	}

	void MForest::_render(void*, void*)
	{
		_drawMeshVeg();
		_drawBillboardVeg();
		_drawX2Veg();
	}

	void MForest::_drawMeshVeg()
	{
	}

	void MForest::_drawBillboardVeg()
	{
	}

	void MForest::_drawX2Veg()
	{
		for (int i = 0; i < mVisibleVegetationBlocks.Size(); ++i)
		{
			MVegetationBlock * block = mVisibleVegetationBlocks[i];

			for (int j = 0; j < block->GetRenderOpCount(); ++j)
			{
				if (block->GetRenderOpGeoType(j) != MVegetation::GT_X2)
					continue;

				RenderOp * rop = block->GetRenderOp(j);

				RenderSystem::Instance()->Render(mTech_VegX2, rop);
			}
		}
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
	}
}

