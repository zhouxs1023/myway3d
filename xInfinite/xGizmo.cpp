#include "stdafx.h"
#include "xGizmo.h"
#include "xApp.h"

xGizmo gGizmo;

xGizmo::xGizmo()
	: OnInit(xApp::OnInit, this, &xGizmo::Init)
	, OnShutdown(xApp::OnShutdown, this, &xGizmo::Shutdown)
	, OnRender(RenderScheme::OnAfterRender, this, &xGizmo::Render)
	, OnUpdate(xApp::OnUpdate, this, &xGizmo::Update)
{
}

xGizmo::~xGizmo()
{
}

void xGizmo::Init(void * data)
{
	_initGeo();

	mRender = new RenderDesc();

	VertexStream * vxStream = &mRender->vxStream;
	IndexStream * ixStream = &mRender->ixStream;

	int iVertexCount = 8 + 5;
	int iIndexCount = 12 * 3 + 4 * 3;
	int iPrimCount = iIndexCount / 3;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof (Vec3));

	float * verteces;
	verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(verteces, mVertex_Move, iVertexCount * sizeof (Vec3));
	}
	buffer->Unlock();

	vxStream->Bind(0, buffer, 12);
	vxStream->SetCount(iVertexCount);

	IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices;
	indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(indices, mIndex_Move, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRender->iPrimCount = iPrimCount;
	mRender->ePrimType = PRIM_TRIANGLELIST;

	mRender->rState.cullMode = CULL_NONE;
	mRender->rState.blendMode = BM_OPATICY;
	mRender->rState.depthWrite = false;
	mRender->rState.depthCheck = DCM_ALWAYS;

	mTech = xApp::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);
}

void xGizmo::Shutdown(void * data)
{
	delete[] mVertex_Move;
	delete[] mIndex_Move;
	delete mRender;
}

void xGizmo::Render(void * data)
{
	Update(data);

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	Aabb box = obj->GetBound();
	Vec3 size = box.GetSize();
	float w = size.x;

	w = Math::Maximum(w, size.y);
	w = Math::Maximum(w, size.z);
	w *= 0.5f;

	Vec3 position = obj->GetPosition();
	Quat orientation = obj->GetOrientation();
	Vec3 scale = Vec3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	// xAxis
	{
		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;
		mRender->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 0)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(1, 0, 0, 1);

		render->Render(mTech, mRender);
	}

	// yAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 1)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 1, 0, 1);

		render->Render(mTech, mRender);
	}

	// zAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 2)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 0, 1, 1);

		render->Render(mTech, mRender);
	}
}

void xGizmo::Update(void * data)
{
	Camera * cam = World::Instance()->MainCamera();

	if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
	{
		mNeedMove = false;
	}

	_mouseMoved();

	if (mNeedMove)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	Aabb box = obj->GetBound();
	Vec3 size = box.GetSize();
	float w = size.x;

	w = Math::Maximum(w, size.y);
	w = Math::Maximum(w, size.z);
	w *= 0.5f;

	Vec3 position = obj->GetPosition();
	Quat orientation = obj->GetOrientation();
	Vec3 scale = Vec3(w, w, w);

	float dx, dy, dz;

	// x axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Vec3 p = mVertex_Move[i] * matWorld;
			bound = bound.Merge(p);
		}

		RayIntersectionInfo result = ray.Intersection(bound);

		if (result.iterscetion)
			dx = result.distance;
		else
			dx = -1;
	}

	// y axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Vec3 p = mVertex_Move[i] * matWorld;
			bound = bound.Merge(p);
		}

		RayIntersectionInfo result = ray.Intersection(bound);

		if (result.iterscetion)
			dy = result.distance;
		else
			dy = -2;
	}

	// z axis
	{
		Aabb bound = Aabb::Invalid;

		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-Math::PI_1 / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		for (int i = 0; i < mNumVertex_Move; ++i)
		{
			Vec3 p = mVertex_Move[i] * matWorld;
			bound = bound.Merge(p);
		}

		RayIntersectionInfo result = ray.Intersection(bound);

		if (result.iterscetion)
			dz = result.distance;
		else
			dz = -3;
	}

	mPickedAxis = -1;

	if (dx > 0)
		mPickedAxis = 0;

	if (dy > 0 && dy > dx)
		mPickedAxis = 1;

	if (dz > 0 && dz > dy && dz > dx)
		mPickedAxis = 2;

	if (mPickedAxis != -1 && IMouse::Instance()->KeyDown(MKC_BUTTON0))
	{
		mNeedMove = true;
	}

}


void xGizmo::_initGeo()
{
	mNumVertex_Move = 8 + 5;
	mNumIndex_Move = 12 * 3 + 4 * 3;

	mVertex_Move = new Vec3[mNumVertex_Move];
	mIndex_Move = new short[mNumIndex_Move];

	float * vert = (float *)mVertex_Move;
	{
		const float off = 0.1f;
		const float half_w = 1 + off;
		const float half_h = 0.02f;
		const float half_d = 0.02f;
		Vec3 pos;

		//front
		pos = Vec3(off, half_h, -half_d);
		*vert++ = off;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(off, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, -half_h, -half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Vec3(off, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(off, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w, -half_h, half_d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//
		pos = Vec3(half_w + 0.3f, 0, 0);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w - 0.1f,  half_h + 0.01f,  half_d + 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w - 0.1f, -half_h - 0.01f,  half_d + 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w - 0.1f, -half_h - 0.01f, -half_d - 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(half_w - 0.1f,  half_h + 0.01f, -half_d - 0.01f);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}

	short * indices = (short *)mIndex_Move;
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
		*indices++ = 4, *indices++ = 5, *indices++ = 0;
		*indices++ = 0, *indices++ = 5, *indices++ = 1;

		//bottom
		*indices++ = 2, *indices++ = 3, *indices++ = 6;
		*indices++ = 6, *indices++ = 3, *indices++ = 7;

		//
		*indices++ = 8, *indices++ =  9, *indices++ = 10;
		*indices++ = 8, *indices++ = 10, *indices++ = 11;
		*indices++ = 8, *indices++ = 11, *indices++ = 12;
		*indices++ = 8, *indices++ = 12, *indices++ = 9;
	}
}

void xGizmo::_mouseMoved()
{
	Camera * cam = World::Instance()->MainCamera();

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	Vec3 pos = obj->GetPosition();

	Aabb box = obj->GetBound();
	Vec3 size = box.GetSize();
	float w = size.x;

	w = Math::Maximum(w, size.y);
	w = Math::Maximum(w, size.z);

	if (mPickedAxis != -1 && mNeedMove && IMouse::Instance()->MouseMoved())
	{
		Point2f pt = IMouse::Instance()->GetPositionDiffUnit();

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Vec3 p0 = pos;
		Vec3 p1;

		if (mPickedAxis == 0)
			p1 = Vec3(1, 0, 0);
		else if (mPickedAxis == 1)
			p1 = Vec3(0, 1, 0);
		else
			p1 = Vec3(0, 0, 1);

		Vec3 p2 = p0 * matVP;
		Vec3 p3 = (p0 + p1) * matVP;

		p2.x = (p2.x + 1) / 2;
		p2.y = (1 - p2.y) / 2;

		p3.x = (p3.x + 1) / 2;
		p3.y = (1 - p3.y) / 2;

		Vec2 d = Vec2(p3.x - p2.x, p3.y - p2.y);
		d.NormalizeL();

		dt = d.x * pt.x + d.y * pt.y;

		obj->SetPosition(pos + p1 * dt * w);
	}
}
