#include "MForest.h"
#include "Engine.h"
#include "MWRenderEvent.h"
#include "MWEnvironment.h"

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
		veg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture("g1.tga", "g1.tga");

		mVegetations.PushBack(veg);
	}

	void MForest::Shutdown()
	{
		Unload();
	}

	void MForest::Update()
	{
		for (int i = 0; i < mVegetationBlocks.Size(); ++i)
		{
			mVegetationBlocks[i]->UpdateGeometry();
		}
	}

	void MForest::Unload()
	{
		RemoveAllVegetationBlock();

		for (int i = 0; i < mVegetations.Size(); ++i)
		{
			delete mVegetations[i];
		}

		mVegetations.Clear();
	}

	void MForest::_AddVisibleVegetationBlock(MVegetationBlock * block)
	{
		mVisibleVegetationBlocks.PushBack(block);
	}

	void MForest::AddVegetation(const TString128 & name, MVegetation::GeomType type,
								const TString128 & mesh, const TString128 & diffueMap,
								const TString128 & normalMap, const TString128 & specularMap)
	{
		MVegetation * veg = new MVegetation;

		veg->Name = name;
		veg->DiffuseMap = VideoBufferManager::Instance()->Load2DTexture(diffueMap, diffueMap);

		if (normalMap != "")
			veg->NormalMap = VideoBufferManager::Instance()->Load2DTexture(normalMap, normalMap);

		if (specularMap != "")
			veg->SpecularMap = VideoBufferManager::Instance()->Load2DTexture(specularMap, specularMap);

		mVegetations.PushBack(veg);
	}

	int MForest::GetVegetationCount() const
	{
		return mVegetations.Size();
	}

	MVegetation * MForest::GetVegetation(int index)
	{
		return mVegetations[index];
	}

	MVegetation * MForest::GetVegetationByName(const TString128 & name)
	{
		for (int i = 0; i < mVegetations.Size(); ++i)
		{
			MVegetation * veg = mVegetations[i];

			if (veg->Name == name)
				return veg;
		}

		return NULL;
	}

	void MForest::AddVegetationInst(MVegetation * veg, const Vec3 & position, float size)
	{
		for (int i = 0; i < mVegetationBlocks.Size(); ++i)
		{
			MVegetationBlock * block = mVegetationBlocks[i];
			const RectF & rc = block->GetRect();

			if (position.x >= rc.x1 && position.x <= rc.x2 &&
				position.y >= rc.y1 && position.y <= rc.y2)
			{
				block->AddVegetation(veg, position, size);
				break;
			}
		}
	}

	void MForest::RemoveVegetationInst(const RectF & rc, MVegetation * veg)
	{
		for (int i = 0; i < mVegetationBlocks.Size(); ++i)
		{
			MVegetationBlock * block = mVegetationBlocks[i];

			const RectF & rcBlock = block->GetRect();

			if (rc.x1 > rcBlock.x2)
				continue;

			if (rc.x2 < rcBlock.x1)
				continue;

			if (rc.y1 > rcBlock.y2)
				continue;

			if (rc.y2 < rcBlock.y1)
				continue;

			block->RemoveVegetation(veg, rc);
		}
	}

	void MForest::CreateVegetationBlocks(const RectF & rect, int xCount, int zCount)
	{
		d_assert (xCount > 0 && zCount > 0);

		float xStep = (rect.x2 - rect.x1) / xCount;
		float zStep = (rect.y2 - rect.y1) / zCount;

		for (int j = 0; j < zCount; ++j)
		{
			for (int i = 0; i < xCount; ++i)
			{
				RectF rc;

				rc.x1 = rect.x1 + xStep * i;
				rc.y1 = rect.y1 + zStep * j;
				rc.x2 = rc.x1 + xStep;
				rc.y2 = rc.y1 + zStep;

				MVegetationBlock * block = new MVegetationBlock(i, j, rc);

				mVegetationBlocks.PushBack(block);
			}
		}
	}

	void MForest::RemoveAllVegetationBlock()
	{
		for (int i = 0; i < mVegetationBlocks.Size(); ++i)
		{
			delete mVegetationBlocks[i];
		}

		mVegetationBlocks.Clear();
	}

	void MForest::_preVisibleCull()
	{
		mVisibleVegetationBlocks.Clear();
	}

	void MForest::_render()
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
		ShaderParam * uNormal = mTech_VegX2->GetVertexShaderParamTable()->GetParam("normal");

		Vec3 normal = -Environment::Instance()->GetEvParam()->LightDir;

		normal = normal.TransformN(World::Instance()->MainCamera()->GetViewMatrix());
		normal.NormalizeL();

		uNormal->SetUnifom(normal.x, normal.y, normal.z, 0);

		for (int i = 0; i < mVisibleVegetationBlocks.Size(); ++i)
		{
			MVegetationBlock * block = mVisibleVegetationBlocks[i];

			for (int j = 0; j < block->GetRenderOpCount(); ++j)
			{
				MVegetation * veg = block->GetRenderVegetation(j);

				if (veg->Type != MVegetation::GT_X2)
					continue;

				RenderOp * rop = block->GetRenderOp(j);

				SamplerState state;

				RenderSystem::Instance()->SetTexture(0, state, veg->DiffuseMap.c_ptr());
				RenderSystem::Instance()->Render(mTech_VegX2, rop);
			}
		}
	}
























	MForestListener gForestListener;

	MForestListener::MForestListener()
		: OnInit(&RenderEvent::OnEngineInit, this, &MForestListener::_init)
		, OnShutdown(&RenderEvent::OnEngineShutdown, this, &MForestListener::_shutdown)
		, OnUpdate(&RenderEvent::OnPostUpdateScene, this, &MForestListener::_update)
		, OnRender(&RenderEvent::OnAfterRenderSolid, this, &MForestListener::_render)
		, OnPreVisibleCull(&RenderEvent::OnPreVisibleCull, this, &MForestListener::_preVisibleCull)
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
		mForest->Update();
	}

	void MForestListener::_render(void * param0, void * param1)
	{
		mForest->_render();
	}

	void MForestListener::_preVisibleCull(void * param0, void * param1)
	{
		mForest->_preVisibleCull();
	}
}

