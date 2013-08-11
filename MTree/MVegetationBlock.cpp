#include "MVegetationBlock.h"
#include "MWRenderEvent.h"
#include "MForest.h"
#include "MWWorld.h"

namespace Myway {

	class MVegetationHelper
	{
	public:
		MVegetationHelper()
			: OnInit(RenderEvent::OnEngineInit, this, &MVegetationHelper::_Init)
			, OnShutdown(RenderEvent::OnEngineShutdown, this, &MVegetationHelper::_Shutdown)
		{
		}

		~MVegetationHelper()
		{

		}

		void _Init(Event * sender)
		{
			mVertexBuffer_Billboard = VideoBufferManager::Instance()->CreateVertexBuffer(20 * 4, 20);

			float * vert = (float*)mVertexBuffer_Billboard->Lock(0, 0, LOCK_NORMAL);
			{
				*vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 0;
				*vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 1; *vert++ = 0;
				*vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 1;
				*vert++ = 0; *vert++ = 0; *vert++ = 0; *vert++ = 1; *vert++ = 1;
			}
			mVertexBuffer_Billboard->Unlock();

			mVertexBuffer_X2 = VideoBufferManager::Instance()->CreateVertexBuffer(20 * 8, 20);
			vert = (float*)mVertexBuffer_X2->Lock(0, 0, LOCK_NORMAL);
			{
				Mat4 mat;

				Vec3 v[] = {
					Vec3(-0.5f, 1, 0),
					Vec3( 0.5f, 1, 0),
					Vec3(-0.5f, 0, 0),
					Vec3( 0.5f, 0, 0),

					Vec3(0, 1, -0.5f),
					Vec3(0, 1,  0.5f),
					Vec3(0, 0, -0.5f),
					Vec3(0, 0,  0.5f),
				};

				Math::MatRotationY(mat, Math::PI_1 / 4);
				Math::VecTransformAffine(v, v, mat, 8);

				*vert++ = v[0].x; *vert++ = v[0].y; *vert++ = v[0].z; *vert++ = 0; *vert++= 0;
				*vert++ = v[1].x; *vert++ = v[1].y; *vert++ = v[1].z; *vert++ = 1; *vert++= 0;
				*vert++ = v[2].x; *vert++ = v[2].y; *vert++ = v[2].z; *vert++ = 0; *vert++= 1;
				*vert++ = v[3].x; *vert++ = v[3].y; *vert++ = v[3].z; *vert++ = 1; *vert++= 1;


				*vert++ = v[4].x; *vert++ = v[4].y; *vert++ = v[4].z; *vert++ = 0; *vert++= 0;
				*vert++ = v[5].x; *vert++ = v[5].y; *vert++ = v[5].z; *vert++ = 1; *vert++= 0;
				*vert++ = v[6].x; *vert++ = v[6].y; *vert++ = v[6].z; *vert++ = 0; *vert++= 1;
				*vert++ = v[7].x; *vert++ = v[7].y; *vert++ = v[7].z; *vert++ = 1; *vert++= 1;
			}
			mVertexBuffer_X2->Unlock();

			mIndexBuffer_Billboard = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * 6);
			short * idx = (short *)mIndexBuffer_Billboard->Lock(0, 0, LOCK_NORMAL);
			{
				*idx++ = 0; *idx++ = 1; *idx++ = 2;
				*idx++ = 2; *idx++ = 1; *idx++ = 3;
			}
			mIndexBuffer_Billboard->Unlock();

			mIndexBuffer_X2 = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof(short) * 12);
			idx = (short *)mIndexBuffer_X2->Lock(0, 0, LOCK_NORMAL);
			{
				*idx++ = 0; *idx++ = 1; *idx++ = 2;
				*idx++ = 2; *idx++ = 1; *idx++ = 3;

				*idx++ = 4; *idx++ = 5; *idx++ = 6;
				*idx++ = 6; *idx++ = 5; *idx++ = 7;
			}
			mIndexBuffer_X2->Unlock();
		}

		void _Shutdown(Event * sender)
		{
			mVertexBuffer_Billboard = NULL;
			mVertexBuffer_X2 = NULL;
			mIndexBuffer_Billboard = NULL;
			mIndexBuffer_X2 = NULL;
		}

		VertexBufferPtr VB_Billboard() { return mVertexBuffer_Billboard; }
		VertexBufferPtr VB_X2() { return mVertexBuffer_X2; }
		IndexBufferPtr IB_Billboard() { return mIndexBuffer_Billboard; }
		IndexBufferPtr IB_X2() { return mIndexBuffer_X2; }

	protected:
		VertexBufferPtr mVertexBuffer_Billboard;
		VertexBufferPtr mVertexBuffer_X2;

		IndexBufferPtr mIndexBuffer_Billboard;
		IndexBufferPtr mIndexBuffer_X2;

		tEventListener<MVegetationHelper> OnInit;
		tEventListener<MVegetationHelper> OnShutdown;
	};

	MVegetationHelper gVegHelper;

	MVegetationBlock::MVegetationBlock(int xSection, int zSection, const RectF & rc)
		: Mover("VegetationBlock") 
		, mXSection(xSection)
		, mZSection(zSection)
		, mRect(rc)
		, mNumOfRenderOps(0)
		, mRenderOps(NULL)
		, mRenderVegs(NULL)
		, mNeedUpdate(false)
	{
		mSceneNode = World::Instance()->CreateSceneNode();
		mSceneNode->Attach(this);
	}

	MVegetationBlock::~MVegetationBlock()
	{
		World::Instance()->DestroySceneNode(mSceneNode);

		safe_delete_array (mRenderOps);
		safe_delete_array (mRenderVegs);
	}

	void MVegetationBlock::AddRenderQueue(RenderQueue * rq)
	{
		MForest::Instance()->_AddVisibleVegetationBlock(this);
	}

	void MVegetationBlock::AddVegetation(MVegetation * veg, const Vec3 & position, float size)
	{
		Inst inst = { veg, position, size };

		mInsts.PushBack(inst);

		mNeedUpdate = true;
	}

	void MVegetationBlock::RemoveVegetation(MVegetation * veg, const RectF & rc)
	{
		List<Inst>::Iterator whr = mInsts.Begin();
		List<Inst>::Iterator end = mInsts.End();

		while (whr != end)
		{
			const Vec3 & position = whr->Position;

			if (position.x >= rc.x1 && position.x <= rc.x2 &&
				position.z >= rc.y1 && position.z <= rc.y2)
			{
				whr = mInsts.Erase(whr);
				mNeedUpdate = true;

				continue;
			}

			++whr;
		}
	}

	void MVegetationBlock::_OnVegRemoved(MVegetation * veg)
	{
		List<Inst>::Iterator whr = mInsts.Begin();
		List<Inst>::Iterator end = mInsts.End();

		while (whr != end)
		{
			if (whr->Veg == veg)
			{
				whr = mInsts.Erase(whr);
				mNeedUpdate = true;

				continue;
			}

			++whr;
		}
	}

	void MVegetationBlock::_OnVegChanged(MVegetation * veg)
	{
		List<Inst>::Iterator whr = mInsts.Begin();
		List<Inst>::Iterator end = mInsts.End();

		while (whr != end)
		{
			if (whr->Veg == veg)
			{
				mNeedUpdate = true;

				break;
			}

			++whr;
		}
	}

	void MVegetationBlock::_UpdateGeometry()
	{
		if (!mNeedUpdate)
			return ;

		_calcuBound();

		safe_delete_array (mRenderOps);
		safe_delete_array (mRenderVegs); 
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
		mRenderVegs = new MVegetationPtr[mNumOfRenderOps];

		{
			MultiMap<MVegetation *, Inst>::Iterator whr = mmap.Begin();
			MultiMap<MVegetation *, Inst>::Iterator end = mmap.End();

			int index = 0;

			while (whr != end)
			{
				MVegetation * Veg = whr->first;

				mRenderVegs[index] = Veg;

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

		mNeedUpdate = false;
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


		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size(), 16);

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

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, sm->GetVertexStream()->GetStream(0), 0, Vegs.Size());
		vxStream.Bind(1, vb, 16);
		vxStream.SetCount(sm->GetVertexStream()->GetCount());

		ixStream.Bind(sm->GetIndexStream()->GetStream(), sm->GetIndexStream()->GetStart());
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
		vdecl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
		vdecl->AddElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 3);
		vdecl->Init();

		VertexBufferPtr vbBillboard = gVegHelper.VB_Billboard();
		IndexBufferPtr ibBillboard = gVegHelper.IB_Billboard();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size(), 16);
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

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vbBillboard, 20, Vegs.Size());
		vxStream.Bind(1, vb, 16, 1);
		vxStream.SetCount(6);

		ixStream.Bind(ibBillboard, 0);
		ixStream.SetCount(6);

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
		vdecl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
		vdecl->AddElement(1, 0, DT_FLOAT4, DU_TEXCOORD, 3);
		vdecl->Init();

		VertexBufferPtr vbX2 = gVegHelper.VB_X2();
		IndexBufferPtr ibX2 = gVegHelper.IB_X2();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(16 * Vegs.Size(), 16);
		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		while (whr != end)
		{
			Vec4 posSize = Vec4(whr->Position, whr->Size);

			*verts++ = posSize.x;
			*verts++ = posSize.y;
			*verts++ = posSize.z;
			*verts++ = posSize.w;

			++whr;
		}
		vb->Unlock();

		vxStream.SetDeclaration(vdecl);
		vxStream.Bind(0, vbX2, 20, Vegs.Size());
		vxStream.Bind(1, vb, 16, 1);
		vxStream.SetCount(8);

		ixStream.Bind(ibX2, 0);
		ixStream.SetCount(12);

		rop.rState.blendMode = BM_ALPHA_TEST;
		rop.rState.cullMode = CULL_NONE;
		rop.iPrimCount = 4;
		rop.ePrimType = PRIM_TRIANGLELIST;
	}

	void MVegetationBlock::_calcuBound()
	{
		mAabbLocal = Aabb::Zero;

		List<Inst>::Iterator whr = mInsts.Begin();
		List<Inst>::Iterator end = mInsts.End();

		while (whr != end)
		{
			Aabb aabb;
			float hw = 0.5f * whr->Size;

			switch (whr->Veg->Type)
			{
			case MVegetation::GT_Mesh:
				{
					Aabb mbb = whr->Veg->pMesh->GetAabb();
					Vec3 center = mbb.GetCenter();

					aabb.minimum = center - Vec3(-mbb.GetWidth(), -mbb.GetHeight(), -mbb.GetDepth()) * hw;
					aabb.maximum = center + Vec3( mbb.GetWidth(),  mbb.GetHeight(),  mbb.GetDepth()) * hw;
				}
				break;

			case MVegetation::GT_BillBoard:
			case MVegetation::GT_X2:
				{
					aabb.minimum = whr->Position - Vec3(-1, 0, -1) * hw;
					aabb.maximum = whr->Position + Vec3( 1, 1,  1) * hw;
				}
				break;
			}

			mAabbLocal = mAabbLocal.Merge(aabb);

			++whr;
		}

		mSphLocal.center = mAabbLocal.GetCenter();
		mSphLocal.radius = mSphLocal.center.Distance(mAabbLocal.minimum);

		if (mNode)
			mNode->_NotifyUpdate();
	}

}