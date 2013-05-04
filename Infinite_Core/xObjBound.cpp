#include "stdafx.h"
#include "xObjBound.h"
#include "Editor.h"

namespace Infinite {

xObjBound::xObjBound()
	: OnInit(xEvent::OnInit, this, &xObjBound::Init)
	, OnShutdown(xEvent::OnShutdown, this, &xObjBound::Shutdown)
	, OnRender(RenderEvent::OnAfterDefferedShading, this, &xObjBound::Render)
{
}

xObjBound::~xObjBound()
{
}

void xObjBound::Init(Event * sender)
{
	mRenderAABB = new RenderOp();

	VertexStream * vxStream = &mRenderAABB->vxStream;
	IndexStream * ixStream = &mRenderAABB->ixStream;

	int iVertexCount = 8;
	int iIndexCount = 12 * 2;
	int iPrimCount = 12;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12, 12);

	float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
	{
		const float half_w = 0.5f;
		const float half_h = 0.5f;
		const float half_d = 0.5f;
		Vec3 pos;

		//front
		pos = Vec3(-half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(-half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Vec3(-half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(-half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}
	vb->Unlock();

	vxStream->Bind(0, vb, 12);
	vxStream->SetCount(iVertexCount);

	IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	{
		*indices++ = 0, *indices++ = 1;
		*indices++ = 2, *indices++ = 3;
		*indices++ = 0, *indices++ = 2;
		*indices++ = 1, *indices++ = 3;

		*indices++ = 4, *indices++ = 5;
		*indices++ = 6, *indices++ = 7;
		*indices++ = 4, *indices++ = 6;
		*indices++ = 5, *indices++ = 7;

		*indices++ = 0, *indices++ = 4;
		*indices++ = 1, *indices++ = 5;
		*indices++ = 2, *indices++ = 6;
		*indices++ = 3, *indices++ = 7;
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRenderAABB->iPrimCount = iPrimCount;
	mRenderAABB->ePrimType = PRIM_LINELIST;

	mRenderAABB->rState.cullMode = CULL_NONE;
	mRenderAABB->rState.blendMode = BM_OPATICY;

	mTech = Editor::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);

	mRenderColMesh = new RenderOp();
	mRenderColMesh->vxStream.SetDeclaration(decl);
	mRenderColMesh->ePrimType = PRIM_TRIANGLELIST;
	mRenderColMesh->rState.cullMode = CULL_NONE;
	mRenderColMesh->rState.fillMode = FILL_FRAME;
	mRenderColMesh->rState.blendMode = BM_OPATICY;
}

void xObjBound::Shutdown(Event * sender)
{
	safe_delete (mRenderAABB);
	safe_delete (mRenderColMesh);
}

void xObjBound::RenderColMesh()
{
	RS_RenderEvent(Render_ColMesh);

	Shape * obj = Editor::Instance()->GetSelectedShape(0);
	ColMesh * colMesh = obj->GetColMesh();

	Vec3 pos = obj->GetPosition();
	Quat ort = obj->GetOrientation();
	Vec3 scale = obj->GetScale();

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

	uColor->SetUnifom(1, 1, 0, 1);

	mRenderColMesh->xform.MakeTransform(pos, ort, scale);
	mRenderColMesh->iPrimCount = colMesh->GetIndices().Size() / 3;

	render->RenderUp(mTech, mRenderColMesh,
		             &(colMesh->GetPositions()[0]), sizeof(Vec3), colMesh->GetPositions().Size(),
					 &(colMesh->GetIndices()[0]));
}

void xObjBound::Render(Event * sender)
{
	if (Editor::Instance()->GetSelectedShapeSize() == 0)
		return ;

	if (Editor::Instance()->GetSelectedShapeSize() == 1 &&
		Editor::Instance()->GetSelectedShape(0)->GetColMesh() != NULL &&
		Editor::Instance()->GetSelectedShape(0)->GetColMesh()->GetPositions().Size() > 0 &&
		Editor::Instance()->GetSelectedShape(0)->GetColMesh()->GetIndices().Size() > 0)
	{
		RenderColMesh();
		return ;
	}

	Aabb box = Aabb::Invalid;

	for (int i = 0; i < Editor::Instance()->GetSelectedShapeSize(); ++i)
	{
		Shape * obj = Editor::Instance()->GetSelectedShape(i);
		box = box.Merge(obj->GetBound());
	}

	Vec3 pos = box.GetCenter();
	Vec3 scale = box.GetSize();

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

	uColor->SetUnifom(1, 1, 0, 1);

	mRenderAABB->xform.MakeTransform(pos, Quat::Identity, scale);

	render->Render(mTech, mRenderAABB);
}

}
