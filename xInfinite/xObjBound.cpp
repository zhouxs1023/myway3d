#include "stdafx.h"
#include "xObjBound.h"
#include "xApp.h"


xObjBound gBound;
xObjBound::xObjBound()
	: OnInit(xApp::OnInit, this, &xObjBound::Init)
	, OnShutdown(xApp::OnShutdown, this, &xObjBound::Shutdown)
	, OnRender(RenderScheme::OnAfterDeffererShading, this, &xObjBound::Render)
{
}

xObjBound::~xObjBound()
{
}

void xObjBound::Init(void * data)
{
	mRender = new RenderDesc();

	VertexStream * vxStream = &mRender->vxStream;
	IndexStream * ixStream = &mRender->ixStream;

	int iVertexCount = 8;
	int iIndexCount = 12 * 2;
	int iPrimCount = 12;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12);

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

	mRender->iPrimCount = iPrimCount;
	mRender->ePrimType = PRIM_LINELIST;

	mRender->rState.cullMode = CULL_NONE;
	mRender->rState.blendMode = BM_OPATICY;

	mTech = xApp::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);
}

void xObjBound::Shutdown(void * data)
{
	delete mRender;
}

void xObjBound::Render(void * data)
{
	if (xApp::Instance()->GetSelectedObjSize() == 0)
		return ;

	Aabb box = Aabb::Invalid;

	for (int i = 0; i < xApp::Instance()->GetSelectedObjSize(); ++i)
	{
		xObj * obj = xApp::Instance()->GetSelectedObj(i);
		box = box.Merge(obj->GetBound());
	}

	Vec3 pos = box.GetCenter();
	Vec3 scale = box.GetSize();

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

	uColor->SetUnifom(1, 1, 0, 1);

	mRender->xform.MakeTransform(pos, Quat::Identity, scale);

	SamplerState state;
	state.Address = TEXA_CLAMP;

	render->Render(mTech, mRender);
}

