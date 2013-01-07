#include "MVegetationBlock.h"

namespace Myway {

	MVegetationBlock::MVegetationBlock(int xSection, int zSection, const RectF & rc)
		: mXSection(xSection)
		, mZSection(zSection)
		, mRect(rc)
		, mNumOfRenderOps(0)
		, mRenderOps(NULL)
		, mGeoTypes(NULL)
	{
	}

	MVegetationBlock::~MVegetationBlock()
	{
		safe_delete_array (mRenderOps);
		safe_delete_array (mGeoTypes);
	}

	void MVegetationBlock::AddVegetation(MVegetation * veg, const Vec3 & position, float size)
	{
		Inst inst = { veg, position, size };

		mInsts.PushBack(inst);
	}

	void MVegetationBlock::ReInitGeometry()
	{
		safe_delete_array (mRenderOps);
		safe_delete_array (mGeoTypes); 
		mNumOfRenderOps = 0;

		MultiMap<MVegetation *, Inst> mmap;

		{
			List<Inst>::Iterator whr = mInsts.Begin();
			List<Inst>::Iterator end = mInsts.End();

			while (whr != end)
			{
				mmap.Insert(whr->Veg, *whr);

				++whr;
			}
		}
		

		if (mmap.Size() == 0)
			return ;

		mNumOfRenderOps = mmap.Size();
		mRenderOps = new RenderOp[mNumOfRenderOps];
		mGeoTypes = new int[mNumOfRenderOps];

		{
			MultiMap<MVegetation *, Inst>::Iterator whr = mmap.Begin();
			MultiMap<MVegetation *, Inst>::Iterator end = mmap.End();

			int index = 0;

			while (whr != end)
			{
				MVegetation * Veg = whr->first;

				mGeoTypes[index] = Veg->Type;

				if (Veg->Type == MVegetation::GT_Mesh)
				{
					_mergeVeg_Mesh(whr->second, index++);
				}
				else if (Veg->Type == MVegetation::GT_BillBoard)
				{
					_mergeVeg_Billboard(whr->second, index++);
				}
				else if (Veg->Type == MVegetation::GT_X2)
				{
					_mergeVeg_X2(whr->second, index++);
				}

				++whr;
			}
		}
	}

	void MVegetationBlock::_mergeVeg_Mesh(List<Inst> & Vegs, int index)
	{
		RenderOp & rop = mRenderOps[index];
		VertexStream & vxStream = mRenderOps[index].vxStream;
		IndexStream & ixStream = mRenderOps[index].ixStream;

		List<Inst>::Iterator whr = Vegs.Begin();
		List<Inst>::Iterator end = Vegs.End();

		MVegetation * v = whr->Veg;

		MeshPtr mesh = v->pMesh;

		SubMesh * sm = v->pMesh->GetSubMesh(0);
		VertexDeclarationPtr meshVertexDecl = sm->GetVertexStream()->GetDeclaration();

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();

		for (int i = 0; i < meshVertexDecl->GetElementCount(); ++i)
		{
			const VertexElement & ve = meshVertexDecl->GetElement(i);

			d_assert (ve.Stream == 0);

			vdecl->AddElement(0, ve.Offset, ve.Type, ve.Usage, ve.UsageIndex);
		}

		vdecl->AddElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 3);
		vdecl->Init();


		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size());

		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		while (whr != end)
		{
			*verts++ = whr->Position.x;
			*verts++ = whr->Position.y;
			*verts++ = whr->Position.z;
			*verts++ = whr->Size;

			++whr;
		}
		vb->Unlock();

		vxStream.Bind(0, sm->GetVertexStream()->GetStream(0), sm->GetVertexStream()->GetStreamStride(0), Vegs.Size());
		vxStream.Bind(1, vb, 16);
		vxStream.SetCount(sm->GetVertexStream()->GetCount());

		ixStream.Bind(sm->GetIndexStream()->GetStream(), sm->GetIndexStream()->GetStartVertex());
		ixStream.SetCount(sm->GetIndexStream()->GetCount());

		rop.rState.blendMode = BM_ALPHA_TEST;
		rop.rState.cullMode = CULL_NONE;
		rop.iPrimCount = sm->GetPrimitiveCount();
		rop.ePrimType = sm->GetPrimitiveType();
	}

	void MVegetationBlock::_mergeVeg_Billboard(List<Inst> & Vegs, int index)
	{
		RenderOp & rop = mRenderOps[index];
		VertexStream & vxStream = mRenderOps[index].vxStream;
		IndexStream & ixStream = mRenderOps[index].ixStream;

		List<Inst>::Iterator whr = Vegs.Begin();
		List<Inst>::Iterator end = Vegs.End();

		MVegetation * v = whr->Veg;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();

		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_TEXCOORD, 0);
		vdecl->AddElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 3);
		vdecl->Init();

		VertexBufferPtr vb0 = VideoBufferManager::Instance()->CreateVertexBuffer(20 * 6);
		float * verts0 = (float *)vb0->Lock(0, 0, LOCK_NORMAL);
		{
			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++= 0;
			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 1; *verts0++= 0;
			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++= 1;

			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++= 1;
			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 1; *verts0++= 0;
			*verts0++ = 0; *verts0++ = 0; *verts0++ = 0; *verts0++ = 1; *verts0++= 1;
		}
		vb0->Unlock();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size());
		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		while (whr != end)
		{
			*verts++ = whr->Position.x;
			*verts++ = whr->Position.y;
			*verts++ = whr->Position.z;
			*verts++ = whr->Size;

			++whr;
		}
		vb->Unlock();

		vxStream.Bind(0, vb0, 20, Vegs.Size());
		vxStream.Bind(1, vb, 16);
		vxStream.SetCount(6);

		rop.rState.blendMode = BM_ALPHA_TEST;
		rop.rState.cullMode = CULL_NONE;
		rop.iPrimCount = 2;
		rop.ePrimType = PRIM_TRIANGLELIST;
	}

	void MVegetationBlock::_mergeVeg_X2(List<Inst> & Vegs, int index)
	{
		RenderOp & rop = mRenderOps[index];
		VertexStream & vxStream = mRenderOps[index].vxStream;
		IndexStream & ixStream = mRenderOps[index].ixStream;

		List<Inst>::Iterator whr = Vegs.Begin();
		List<Inst>::Iterator end = Vegs.End();

		MVegetation * v = whr->Veg;

		VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();

		vdecl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		vdecl->AddElement(0, 12, DT_FLOAT3, DU_TEXCOORD, 0);
		vdecl->AddElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 3);
		vdecl->Init();

		VertexBufferPtr vb0 = VideoBufferManager::Instance()->CreateVertexBuffer(20 * 12);
		float * verts0 = (float *)vb0->Lock(0, 0, LOCK_NORMAL);
		{
			*verts0++ = -0.5f; *verts0++ = 1.0f; *verts0++ = 0.0f; *verts0++ = 0; *verts0++= 0;
			*verts0++ =  0.5f; *verts0++ = 1.0f; *verts0++ = 0.0f; *verts0++ = 1; *verts0++= 0;
			*verts0++ = -0.5f; *verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ = 0; *verts0++= 1;

			*verts0++ = -0.5f; *verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ = 0; *verts0++= 1;
			*verts0++ =  0.5f; *verts0++ = 1.0f; *verts0++ = 0.0f; *verts0++ = 1; *verts0++= 0;
			*verts0++ =  0.5f; *verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ = 1; *verts0++= 1;


			*verts0++ = 0.0f; *verts0++ = 1.0f; *verts0++ = -0.5f; *verts0++ = 0; *verts0++= 0;
			*verts0++ = 0.0f; *verts0++ = 1.0f; *verts0++ =  0.5f; *verts0++ = 1; *verts0++= 0;
			*verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ = -0.5f; *verts0++ = 0; *verts0++= 1;

			*verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ = -0.5f; *verts0++ = 0; *verts0++= 1;
			*verts0++ = 0.0f; *verts0++ = 1.0f; *verts0++ =  0.5f; *verts0++ = 1; *verts0++= 0;
			*verts0++ = 0.0f; *verts0++ = 0.0f; *verts0++ =  0.5f; *verts0++ = 1; *verts0++= 1;
		}
		vb0->Unlock();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size());
		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		while (whr != end)
		{
			*verts++ = whr->Position.x;
			*verts++ = whr->Position.y;
			*verts++ = whr->Position.z;
			*verts++ = whr->Size;

			++whr;
		}
		vb->Unlock();

		vxStream.Bind(0, vb0, 20, Vegs.Size());
		vxStream.Bind(1, vb, 16);
		vxStream.SetCount(12);

		rop.rState.blendMode = BM_ALPHA_TEST;
		rop.rState.cullMode = CULL_NONE;
		rop.iPrimCount = 4;
		rop.ePrimType = PRIM_TRIANGLELIST;
	}
}