#include "stdafx.h"
#include "xObj.h"
#include "xApp.h"

DF_PROPERTY_BEGIN(xObj)
	DF_PROPERTY(xObj, Name, "General", "Name", PT_String, 128)
	DF_PROPERTY(xObj, Position, "Transform", "Position", PT_Vec3, 12)
	DF_PROPERTY(xObj, Orientation, "Transform", "Orientation", PT_Vec4, 16)
	DF_PROPERTY(xObj, Scale, "Transform", "Scale", PT_Vec3, 12)
DF_PROPERTY_END()

xObj::xObj(const char * name)
{
	Strcpy(Name, 128, name);

	Position = Vec3::Zero; 
	Orientation = Quat::Identity;
	Scale = Vec3::Unit;
}

xObj::~xObj()
{
}

const char * xObj::GetName()
{
	return Name;
}

void xObj::SetPosition(const Vec3 & p)
{
	Position = p;
	xPropertyChanged(Position);
}

void xObj::SetOrientation(const Quat & q)
{
	Orientation = q;
	xPropertyChanged(Orientation);
}

void xObj::SetScale(const Vec3 & s)
{
	Scale = s;
	xPropertyChanged(Scale);
}

Vec3 xObj::GetPosition()
{
	return Position;
}

Quat xObj::GetOrientation()
{
	return Orientation;
}

Vec3 xObj::GetScale()
{
	return Scale;
}


xObjManager gxObjMgr;

IMP_SLN(xObjManager);

xObjManager::xObjManager()
{
	INIT_SLN;
	xApp::OnShutdown += this;
}

xObjManager::~xObjManager()
{
	xApp::OnShutdown -= this;
	SHUT_SLN;
}

void xObjManager::OnCall(Event * sender, void * data)
{
	if (sender == &xApp::OnShutdown)
	{
		for (int i = 0; i < mFactorys.Size(); ++i)
		{
			delete mFactorys[i];
		}

		for (int i = 0; i < mObjs.Size(); ++i)
		{
			delete mObjs[i];
		}

		mFactorys.Clear();
		mObjs.Clear();
	}
}

void xObjManager::AddFactory(xObjFactory * sf)
{
	d_assert (GetFactory(sf->GetTypeName()) == NULL);

	mFactorys.PushBack(sf);
}

xObjFactory * xObjManager::GetFactory(const char * type)
{
	for (int i = 0; i < mFactorys.Size(); ++i)
	{
		if (Strcmp(type, mFactorys[i]->GetTypeName()) ==0)
			return mFactorys[i];
	}

	return NULL;
}

int xObjManager::GetFactoryCount()
{
	return mFactorys.Size();
}

xObjFactory * xObjManager::GetFactory(int index)
{
	d_assert (index < GetFactoryCount());
	return mFactorys[index];
}


xObj * xObjManager::Create(const char * type)
{
	xObjFactory * sf = GetFactory(type);

	d_assert (sf);

	int uId = 0;
	TString128 uName = "xObj";
	TString128 uxObjName;
	bool flag = true;

	while (1)
	{
		uxObjName = uName + uId++;

		if (Get(uxObjName.c_str()) == NULL)
			break;
	}

	xObj * xObj = sf->Create(uxObjName.c_str());

	mObjs.PushBack(xObj);

	return xObj;
}

void xObjManager::Distroy(xObj * xObj)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (xObj == mObjs[i])
		{
			delete mObjs[i];
			return ;
		}
	}

	d_assert (0);
}

xObj * xObjManager::Get(const char * name)
{
	for (int i = 0; i < mObjs.Size(); ++i)
	{
		if (Strcmp(name, mObjs[i]->GetName()) == 0)
			return mObjs[i];
	}

	return NULL;
}

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

	uColor->SetUnifom(0, 1, 1, 1);

	mRender->xform.MakeTransform(pos, Quat::Identity, scale);

	SamplerState state;
	state.Address = TEXA_CLAMP;

	render->Render(mTech, mRender);
}



xGizmo::xGizmo()
	: OnInit(xApp::OnInit, this, &xGizmo::Init)
	, OnShutdown(xApp::OnShutdown, this, &xGizmo::Shutdown)
	, OnRender(RenderScheme::OnAfterDeffererShading, this, &xGizmo::Render)
{
}

xGizmo::~xGizmo()
{
}

void xGizmo::Init(void * data)
{
	mRender = new RenderDesc();

	VertexStream * vxStream = &mRender->vxStream;
	IndexStream * ixStream = &mRender->ixStream;

	int iVertexCount = 8;
	int iIndexCount = 12 * 3;
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
		//front
		*indices++ = 0, *indices++ = 1, *indices++ = 2;
		*indices++ = 2, *indices++ = 1, *indices++ = 3;

		//back
		*indices++ = 5, *indices++ = 4, *indices++ = 7;
		*indices++ = 7, *indices++ = 4, *indices++ = 6;

		//left
		*indices++ = 4, *indices++ = 0, *indices++ = 6;
		*indices++ = 6, *indices++ = 0, *indices++ = 2;

		//right
		*indices++ = 1, *indices++ = 5, *indices++ = 3;
		*indices++ = 3, *indices++ = 5, *indices++ = 7;

		//top
		*indices++ = 0, *indices++ = 1, *indices++ = 4;
		*indices++ = 4, *indices++ = 1, *indices++ = 5;

		//bottom
		*indices++ = 2, *indices++ = 3, *indices++ = 6;
		*indices++ = 6, *indices++ = 3, *indices++ = 7;
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRender->iPrimCount = iPrimCount;
	mRender->ePrimType = PRIM_TRIANGLELIST;

	mRender->rState.cullMode = CULL_NONE;
	mRender->rState.blendMode = BM_OPATICY;
	mRender->rState.fillMode = FILL_FRAME;

	mTech = xApp::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);
}

void xGizmo::Shutdown(void * data)
{
	delete mRender;
}

void xGizmo::Render(void * data)
{
	/*if (xApp::Instance()->GetSelectedObjSize() == 0 ||
	xApp::Instance()->GetSelectedObjSize() > 1)
	return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

	uColor->SetUnifom(0, 1, 1, 1);

	mRender->xform.MakeTransform(pos, Quat::Identity, scale);

	SamplerState state;
	state.Address = TEXA_CLAMP;

	render->Render(mTech, mRender);*/
}