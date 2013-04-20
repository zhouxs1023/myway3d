#include "stdafx.h"

#include "BackCell.h"
#include "Editor.h"

namespace MotionFX {

	BackCell::BackCell()
		: OnRender(RenderEvent::OnAfterDefferedShading, this, &BackCell::_Render)
	{
	}

	BackCell::~BackCell()
	{
	}

	void BackCell::Init()
	{
		mRenderOp = new RenderOp;

		int iVertexCount = 11 * 11;
		int iIndexCount = 11 * 11 * 2;
		int iStride = 28;

		VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
		decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
		decl->AddElement(0, 12, DT_FLOAT4, DU_COLOR, 0);
		decl->Init();

		VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * iStride);

		float * verts = (float *)vb->Lock(0, 0, LOCK_NORMAL);
		{
			for (int j = 0; j < 11; ++j)
			{
				for (int i = 0; i < 11; ++i)
				{
					*verts++ = -1.0f + i * 0.2f;
					*verts++ = 0;
					*verts++ = -1.0f + j * 0.2f;

					if (i == 5 || j == 5)
					{
						*verts++ = 0.8f; *verts++ = 0; *verts++ = 0; *verts++ = 1;
					}
					else
					{
						*verts++ = 0.5f; *verts++ = 0.5f; *verts++ = 0.5f; *verts++ = 1;
					}
				}
			}
		}
		vb->Unlock();

		IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));

		short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);

		{
			for (int i = 0; i < 11; ++i)
			{
				*idx++ = i * 11;
				*idx++ = i * 11 + 10;
			}

			for (int i = 0; i < 11; ++i)
			{
				*idx++ = i;
				*idx++ = 10 * 11 + i;
			}
		}

		ib->Unlock();

		mRenderOp->vxStream.SetDeclaration(decl);
		mRenderOp->vxStream.Bind(0, vb, iStride);
		mRenderOp->vxStream.SetCount(iVertexCount);

		mRenderOp->ixStream.Bind(ib, 0);
		mRenderOp->ixStream.SetCount(iIndexCount);

		mRenderOp->iPrimCount = 11 + 11;
		mRenderOp->ePrimType = PRIM_LINELIST;

		mTech = Editor::Instance()->GetHelperShaderLib()->GetTechnique("VColor");

		d_assert (mTech != NULL);
	}

	void BackCell::Shutdown()
	{
		safe_delete(mRenderOp);
	}

	void BackCell::_Render(Event * _sender)
	{
		RenderSystem * render = RenderSystem::Instance();

		mRenderOp->xform.MakeTransform(Vec3::Zero, Quat::Identity, 200);

		render->Render(mTech, mRenderOp);
	}
}