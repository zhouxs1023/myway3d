#include "stdafx.h"

#include "MWTerrainSection.h"

#include "xEditTerrainWater.h"
#include "Editor.h"

namespace Infinite {

	xEditTerrainWater::xEditTerrainWater()
	{
		mBrushSize = 5;
		mBrushOp_Add = false;
	}

	xEditTerrainWater::~xEditTerrainWater()
	{
	}

	void xEditTerrainWater::_Init()
	{
		mTech_Brush = Editor::Instance()->GetHelperShaderLib()->GetTechnique("Color");
		mTech_WaterHelper = Editor::Instance()->GetHelperShaderLib()->GetTechnique("Color");

		mRenderOp_WaterHelper = new RenderOp();

		int iVertexCount = 257 * 4;

		VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		decl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof(Vec3), sizeof(Vec3));
		Vec3 * vert = (Vec3 *)vb->Lock(0, 0, LOCK_DISCARD);
		{
			float x = 0, y = 0, z = 0;

			for (int i = 0; i < 257; ++i)
			{
				x = 0 - 128.5f;
				z = i - 128.5f;
				*vert++ = Vec3(x, y, z);

				x = 257 - 128.5f;
				z = i - 128.5f;
				*vert++ = Vec3(x, y, z);
			}

			for (int j = 0; j < 257; ++j)
			{
				x = j - 128.5f;
				z = 0 - 128.5f;
				*vert++ = Vec3(x, y, z);

				x = j - 128.5f;
				z = 257 - 128.5f;
				*vert++ = Vec3(x, y, z);
			}
		}
		vb->Unlock();

		mRenderOp_WaterHelper->vxStream.SetDeclaration(decl);
		mRenderOp_WaterHelper->vxStream.Bind(0, vb, sizeof(Vec3));
		mRenderOp_WaterHelper->vxStream.SetCount(iVertexCount);

		mRenderOp_WaterHelper->iPrimCount = iVertexCount / 2;
		mRenderOp_WaterHelper->ePrimType = PRIM_LINELIST;
		mRenderOp_WaterHelper->rState.depthWrite = false;


		mRenderOp_Brush = new RenderOp();
		iVertexCount = 4;

		vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof(Vec3), sizeof(Vec3));
		vert = (Vec3 *)vb->Lock(0, 0, LOCK_DISCARD);
		{
			*vert++ = Vec3(-0.5f, 0, -0.5f);
			*vert++ = Vec3(+0.5f, 0, -0.5f);
			*vert++ = Vec3(-0.5f, 0, +0.5f);
			*vert++ = Vec3(+0.5f, 0, +0.5f);
		}
		vb->Unlock();

		mRenderOp_Brush->vxStream.SetDeclaration(decl);
		mRenderOp_Brush->vxStream.Bind(0, vb, sizeof(Vec3));
		mRenderOp_Brush->vxStream.SetCount(iVertexCount);

		mRenderOp_Brush->iPrimCount = iVertexCount - 2;
		mRenderOp_Brush->ePrimType = PRIM_TRIANGLESTRIP;

		mRenderOp_Brush->rState.cullMode = CULL_NONE;
		mRenderOp_Brush->rState.depthWrite = false;
	}

	void xEditTerrainWater::_Shutdown()
	{
		delete mRenderOp_WaterHelper;
		delete mRenderOp_Brush;
	}

	void xEditTerrainWater::_Update()
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();
		Water * water = Environment::Instance()->GetWaterManager()->GetWater();

		if (!terrain || !Editor::Instance()->IsFoucs())
			return ;

		float mx = Editor::Instance()->GetMousePosition().x;
		float my = Editor::Instance()->GetMousePosition().y;

		if (mx <= 0 || mx >= 1 ||
			my <= 0 || my >= 1)
			return ;

		Ray ray = World::Instance()->MainCamera()->GetViewportRay(mx, my);
		float gridSize = water->GetGridSize();

		Plane plane = Plane(Vec3::UnitY, -water->GetHeight());

		RayIntersectionInfo info = ray.Intersection(plane);

		if (!info.iterscetion)
			return ;

		Vec3 position = ray.GetPosition(info.distance);

		int x = int((position.x / gridSize));
		int z = int((position.z / gridSize));

		mBrush.position.x = x * gridSize + gridSize / 2;
		mBrush.position.z = z * gridSize + gridSize / 2;
		mBrush.position.y = position.y;
		mBrush.size = gridSize * mBrushSize;

		if (!IMouse::Instance()->KeyUp(MKC_BUTTON0))
			return ;

		if (mBrushSize <= 0)
			return ;

		int halfBSize = mBrushSize / 2;
		char flag = mBrushOp_Add ? 1 : 0;
		int sizeX = water->GetSizeX();
		int sizeZ = water->GetSizeZ();

		for (int j = 0; j < mBrushSize; ++j)
		{
			for (int i = 0; i < mBrushSize; ++i)
			{
				int _x = x + i - halfBSize;
				int _z = z + j - halfBSize;

				if (_x < 0 || _x > sizeX - 1 ||
					_z < 0 || _z > sizeZ - 1)
					continue ;

				water->SetData(_x, _z, flag);
			}
		}

		Rect rect = { x - halfBSize, z - halfBSize, x + halfBSize, z + halfBSize };
		water->NotifyChanged(rect);
	}

	void xEditTerrainWater::_Render()
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();
		Water * water = Environment::Instance()->GetWaterManager()->GetWater();

		if (!terrain)
			return ;

		int op = Editor::Instance()->GetOperator();

		if (op != eOP_Terrain)
			return ;

		// render brush
		Vec3 waterPos = mBrush.position;
		Vec3 waterScale = Vec3(mBrush.size, 1, mBrush.size);

		mRenderOp_Brush->xform.MakeTransform(waterPos, Quat::Identity, waterScale);

		mTech_Brush->GetPixelShaderParamTable()->GetParam("gColor")->SetUnifom(0, 0, 1, 1);
		RenderSystem::Instance()->Render(mTech_Brush, mRenderOp_Brush);


		// render helper
		Vec3 camPos = World::Instance()->MainCamera()->GetPosition();
		float gridSize = water->GetGridSize();

		int x = int((camPos.x / gridSize));
		int z = int((camPos.z / gridSize));

		Vec3 helperPos;
		helperPos.x = x * gridSize + gridSize / 2;
		helperPos.z = z * gridSize + gridSize / 2;
		helperPos.y = water->GetHeight();

		Vec3 helperScale = Vec3(gridSize, 1, gridSize);

		mRenderOp_WaterHelper->xform.MakeTransform(helperPos, Quat::Identity, helperScale);

		mTech_WaterHelper->GetPixelShaderParamTable()->GetParam("gColor")->SetUnifom(1, 1, 0, 1);
		RenderSystem::Instance()->Render(mTech_WaterHelper, mRenderOp_WaterHelper);
	}

}