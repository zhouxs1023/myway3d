#include "stdafx.h"

#include "xEditTerrainNavPath.h"
#include "Editor.h"
#include "MWEnvironment.h"


namespace Infinite {

	xEditTerrainNavPath::xEditTerrainNavPath()
	{
		mOp = eSet;
		mBrush.size = 5;
		mBrush.position = Vec3::Zero;

		mCurXSection = -1;
		mCurZSection = -1;
	}

	xEditTerrainNavPath::~xEditTerrainNavPath()
	{
	}

	void xEditTerrainNavPath::_Init()
	{
		mTech_Brush = Editor::Instance()->GetHelperShaderLib()->GetTechnique("Color");

		int cells = (Terrain::kSectionVertexSize - 1) * 3;

		mRenderOp = new RenderOp;

		int iVertexCount = cells * cells * 4;
		int iIndexCount = cells * cells * 6;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT4, DU_COLOR, 0);
		vdecl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 28, USAGE_DYNAMIC);

		float * verts = (float *)vb->Lock(0, 0, LOCK_DISCARD);
		{

		}
		vb->Unlock();

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));

		short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
		{
			int index = 0;
			for (int j = 0; j < cells; ++j)
			{
				for (int i = 0; i < cells; ++i)
				{
					*idx++ = index + 0;
					*idx++ = index + 1;
					*idx++ = index + 2;

					*idx++ = index + 2;
					*idx++ = index + 1;
					*idx++ = index + 3;

					index += 4;
				}
			}
		}
		ib->Unlock();

		mRenderOp->vxStream.SetDeclaration(vdecl);
		mRenderOp->vxStream.Bind(0, vb, 28);
		mRenderOp->vxStream.SetCount(iVertexCount);

		mRenderOp->ixStream.Bind(ib, 0);
		mRenderOp->ixStream.SetCount(iIndexCount);

		mRenderOp->iPrimCount = cells * cells * 2;
		mRenderOp->ePrimType = PRIM_TRIANGLELIST;
	}

	void xEditTerrainNavPath::_Shutdown()
	{
		safe_delete(mRenderOp);
	}

	void xEditTerrainNavPath::_Update()
	{
	}

	void xEditTerrainNavPath::_updateGeometry()
	{
		Terrain * terrain = Environment::Instance()->GetTerrain();

		if (terrain == NULL)
			return ;

		Camera * cam = World::Instance()->MainCamera();
		const Vec3 & pos = cam->GetPosition();
		const Terrain::Config & config = terrain->GetConfig();

		int cells = (Terrain::kSectionVertexSize - 1) * 3;

		int xIndex = pos.x / config.xSize * (config.xVertexCount - 1);
		int zIndex = (1 - pos.z) / config.zSize * (config.zVertexCount - 1);

		xIndex = xIndex / (Terrain::kSectionVertexSize - 1);
		zIndex = zIndex / (Terrain::kSectionVertexSize - 1);

		xIndex = Math::Clamp(xIndex, 1, config.xSectionCount - 2);
		zIndex = Math::Clamp(zIndex, 1, config.zSectionCount - 2);

		if (xIndex == mCurXSection && zIndex == mCurZSection)
			return ;

		VertexBufferPtr vb = mRenderOp->vxStream.GetStream(0);

		float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
		{
			Size2 size = NavEngine::Instance()->GetNavPath()->GetSize();

			float gridSize = config.xSize / size.w;

			for (int j = 0; j < cells; ++j)
			{
				for (int i = 0; i < cells; ++i)
				{
					
				}
			}
		}
		vb->Unlock();
	}

}