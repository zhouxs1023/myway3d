#include "stdafx.h"
#include "xGizmo.h"
#include "xApp.h"
#include "xBaseOperator.h"

xGizmo::xGizmo()
	: OnInit(&xEvent::OnInit, this, &xGizmo::Init)
	, OnShutdown(&xEvent::OnShutdown, this, &xGizmo::Shutdown)
	, OnRender(&RenderEvent::OnAfterRender, this, &xGizmo::Render)
	, OnUpdate(&xEvent::OnUpdate, this, &xGizmo::Update)
	, mPicked(false)
	, mPickedAxis(-1)
{
}

xGizmo::~xGizmo()
{
}

void xGizmo::Init(void * param0, void * param1)
{
	_initGeo_Move();
	_initGeo_Move_Render();

	_initGeo_Rotate();
	_initGeo_Rotate_Render();

	_initGeo_Scale();
	_initGeo_Scale_Render();
	

	mTech = xApp::Instance()->GetHelperShaderLib()->GetTechnique("Color");

	d_assert (mTech);
}

void xGizmo::Shutdown(void * param0, void * param1)
{
	delete[] mVertex_Move;
	delete[] mIndex_Move;
	delete mRender_Move;

	delete[] mVertex_Rotate;
	delete[] mIndex_Rotate;
	delete mRender_Rotate;

	delete[] mVertex_Scale;
	delete[] mIndex_Scale;
	delete mRender_Scale;
}

void xGizmo::Render(void * param0, void * param1)
{
	int op = xApp::Instance()->GetOperator();

	if (op == xMoveOp::eOp_Move)
		_renderMove();
	else if (op == xRotateOp::eOp_Rotate)
		_renderRotate();
	else if (op == xScaleOp::eOp_Scale)
		_renderScale();
}

void xGizmo::Update(void * param0, void * param1)
{
	Camera * cam = World::Instance()->MainCamera();

	if (IMouse::Instance()->KeyUp(MKC_BUTTON0))
	{
		mPicked = false;
	}

	int op = xApp::Instance()->GetOperator();

	if (op == xMoveOp::eOp_Move)
	{
		_mouseMoved_Move();
		_update_Move();
	}
	else if (op == xRotateOp::eOp_Rotate)
	{
		_mouseMoved_Rotate();
		_update_Rotate();
	}
	else if (op == xScaleOp::eOp_Scale)
	{
		_mouseMoved_Scale();
		_update_Scale();
	}
}


void xGizmo::_initGeo_Move()
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

void xGizmo::_initGeo_Move_Render()
{
	mRender_Move = new RenderOp();

	VertexStream * vxStream = &mRender_Move->vxStream;
	IndexStream * ixStream = &mRender_Move->ixStream;

	int iVertexCount = mNumVertex_Move;
	int iIndexCount = mNumIndex_Move;
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

	mRender_Move->iPrimCount = iPrimCount;
	mRender_Move->ePrimType = PRIM_TRIANGLELIST;

	mRender_Move->rState.cullMode = CULL_NONE;
	mRender_Move->rState.blendMode = BM_OPATICY;
	mRender_Move->rState.depthWrite = false;
	mRender_Move->rState.depthCheck = DCM_ALWAYS;
}

void xGizmo::_initGeo_Rotate()
{
	int segments = 20;

	mNumVertex_Rotate = (segments + 1) * 4;
	mNumIndex_Rotate = segments * 8 * 3;
	
	mVertex_Rotate = new Vec3[mNumVertex_Rotate];
	mIndex_Rotate = new short[mNumIndex_Rotate];

	Memzero(mIndex_Rotate, mNumIndex_Rotate * 2);

	const float w = 0.025f;
	for (int i = 0; i < segments + 1; ++i)
	{
		float rads = i / float(segments) * Math::PI_2;

		float cosine = Math::Cos(rads);
		float sine = Math::Sin(rads);

		Vec3 dir = Vec3(cosine, 0, sine);

		mVertex_Rotate[i * 4 + 0] = Vec3(0, -w, 0) + dir * (1 - w);
		mVertex_Rotate[i * 4 + 1] = Vec3(0, -w, 0) + dir * (1 + w);
		mVertex_Rotate[i * 4 + 2] = Vec3(0, +w, 0) + dir * (1 - w);
		mVertex_Rotate[i * 4 + 3] = Vec3(0, +w, 0) + dir * (1 + w);
	}

	for (int i = 0; i < segments; ++i)
	{
		int s = i * 4;
		int n = s + 4;
		int idx = i * 24;

		// bottom
		mIndex_Rotate[idx++] = s; 
		mIndex_Rotate[idx++] = s + 1; 
		mIndex_Rotate[idx++] = n;

		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = s + 1;
		mIndex_Rotate[idx++] = n + 1;

		// top
		mIndex_Rotate[idx++] = s + 2;
		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 2;

		mIndex_Rotate[idx++] = n + 2;
		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 3;

		// left
		mIndex_Rotate[idx++] = s;
		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = s + 2;

		mIndex_Rotate[idx++] = s + 2;
		mIndex_Rotate[idx++] = n;
		mIndex_Rotate[idx++] = n + 2;

		// right
		mIndex_Rotate[idx++] = s + 1;
		mIndex_Rotate[idx++] = n + 1;
		mIndex_Rotate[idx++] = s + 3;

		mIndex_Rotate[idx++] = s + 3;
		mIndex_Rotate[idx++] = n + 1;
		mIndex_Rotate[idx++] = n + 3;
	}
}

void xGizmo::_initGeo_Rotate_Render()
{
	mRender_Rotate = new RenderOp();

	VertexStream * vxStream = &mRender_Rotate->vxStream;
	IndexStream * ixStream = &mRender_Rotate->ixStream;

	int iVertexCount = mNumVertex_Rotate;
	int iIndexCount = mNumIndex_Rotate;
	int iPrimCount = iIndexCount / 3;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof (Vec3));

	float * verteces;
	verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(verteces, mVertex_Rotate, iVertexCount * sizeof (Vec3));
	}
	buffer->Unlock();

	vxStream->Bind(0, buffer, 12);
	vxStream->SetCount(iVertexCount);

	IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices;
	indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(indices, mIndex_Rotate, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRender_Rotate->iPrimCount = iPrimCount;
	mRender_Rotate->ePrimType = PRIM_TRIANGLELIST;

	mRender_Rotate->rState.cullMode = CULL_NONE;
	mRender_Rotate->rState.blendMode = BM_OPATICY;
	mRender_Rotate->rState.depthWrite = false;
	mRender_Rotate->rState.depthCheck = DCM_ALWAYS;
}

void xGizmo::_initGeo_Scale()
{
	mNumVertex_Scale = 8;
	mNumIndex_Scale = 36;

	mVertex_Scale= new Vec3[mNumVertex_Scale];
	mIndex_Scale = new short[mNumIndex_Scale];

	float * vert = (float *)mVertex_Scale;
	{
		const float w = 0.02f;
		const float h = 0.02f;
		const float d = 0.02f;
		Vec3 pos;

		//front
		pos = Vec3(1 - w, h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 + w, h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 - w, -h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 + w, -h, -d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		//back
		pos = Vec3(1 - w, h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 + w, h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 - w, -h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;

		pos = Vec3(1 + w, -h, d);
		*vert++ = pos.x;
		*vert++ = pos.y;
		*vert++ = pos.z;
	}

	short * indices = (short *)mIndex_Scale;
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
	}
}

void xGizmo::_initGeo_Scale_Render()
{
	mRender_Scale = new RenderOp();

	VertexStream * vxStream = &mRender_Scale->vxStream;
	IndexStream * ixStream = &mRender_Scale->ixStream;

	int iVertexCount = mNumVertex_Scale;
	int iIndexCount = mNumIndex_Scale;
	int iPrimCount = iIndexCount / 3;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
	decl->Init();

	vxStream->SetDeclaration(decl);

	VertexBufferPtr buffer = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * sizeof (Vec3));

	float * verteces;
	verteces = (float *)buffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(verteces, mVertex_Scale, iVertexCount * sizeof (Vec3));
	}
	buffer->Unlock();

	vxStream->Bind(0, buffer, 12);
	vxStream->SetCount(iVertexCount);

	IndexBufferPtr ibuffer = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
	short * indices;
	indices = (short *)ibuffer->Lock(0, 0, LOCK_DISCARD);
	{
		Memcpy(indices, mIndex_Scale, iIndexCount * sizeof (short));
	}
	ibuffer->Unlock();

	ixStream->SetCount(iIndexCount);
	ixStream->Bind(ibuffer, 0);

	mRender_Scale->iPrimCount = iPrimCount;
	mRender_Scale->ePrimType = PRIM_TRIANGLELIST;

	mRender_Scale->rState.cullMode = CULL_NONE;
	mRender_Scale->rState.blendMode = BM_OPATICY;
	mRender_Scale->rState.depthWrite = false;
	mRender_Scale->rState.depthCheck = DCM_ALWAYS;
}

void xGizmo::_renderMove()
{
	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	float w = _getObjSize(obj);

	Vec3 position = obj->GetPosition();
	Quat orientation = Quat::Identity;
	Vec3 scale = Vec3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	// xAxis
	{
		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Move->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 0)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(1, 0, 0, 1);

		render->Render(mTech, mRender_Move);
	}

	// yAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Move->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 1)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 1, 0, 1);

		render->Render(mTech, mRender_Move);
	}

	// zAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Move->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 2)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 0, 1, 1);

		render->Render(mTech, mRender_Move);
	}
}

void xGizmo::_renderRotate()
{
	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	float w = _getObjSize(obj);

	Vec3 position = obj->GetPosition();
	Quat orientation = Quat::Identity;
	Vec3 scale = Vec3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	// xAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Rotate->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 0)
		uColor->SetUnifom(1, 1, 1, 1);
		else
		uColor->SetUnifom(1, 0, 0, 1);

		render->Render(mTech, mRender_Rotate);
	}

	// yAxis
	{
		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Rotate->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 1)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 1, 0, 1);

		render->Render(mTech, mRender_Rotate);
	}

	// zAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationX(Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Rotate->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 2)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 0, 1, 1);

		render->Render(mTech, mRender_Rotate);
	}
}

void xGizmo::_renderScale()
{
	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	float w = _getObjSize(obj);

	Vec3 position = obj->GetPosition();
	Quat orientation = Quat::Identity;
	Vec3 scale = Vec3(w, w, w);

	RenderSystem * render = RenderSystem::Instance();

	// xAxis
	{
		Mat4 matLocal, matWorld;

		matLocal = Mat4::Identity;
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Scale->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 0)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(1, 0, 0, 1);

		render->Render(mTech, mRender_Scale);
	}

	// yAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Scale->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 1)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 1, 0, 1);

		render->Render(mTech, mRender_Scale);
	}

	// zAxis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-Math::PI_1 / 2);
		matWorld.MakeTransform(position, orientation, scale);
		matWorld = matLocal * matWorld;
		mRender_Scale->xform = matWorld;

		ShaderParam * uColor = mTech->GetPixelShaderParamTable()->GetParam("gColor");

		if (mPickedAxis == 2)
			uColor->SetUnifom(1, 1, 1, 1);
		else
			uColor->SetUnifom(0, 0, 1, 1);

		render->Render(mTech, mRender_Scale);
	}
}


void xGizmo::_update_Move()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	float w = _getObjSize(obj);

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
		mPicked = true;
	}
}

void xGizmo::_mouseMoved_Move()
{
	Camera * cam = World::Instance()->MainCamera();

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	float w = _getObjSize(obj);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Point2f pt = IMouse::Instance()->GetPositionDiffUnit();

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Vec3 p0 = obj->GetPosition();
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

		obj->SetPosition(obj->GetPosition() + p1 * dt * w * 5);
	}
}

void xGizmo::_update_Rotate()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	float w = _getObjSize(obj);

	Vec3 position = obj->GetPosition();
	Quat orientation = obj->GetOrientation();
	Vec3 scale = Vec3(w, w, w);

	float dx, dy, dz;

	// x axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationZ(Math::PI_1 / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		RayIntersectionInfo result;

		Math::RayIntersection(result, ray, mVertex_Rotate, mIndex_Rotate,
			mNumIndex_Rotate / 3, matWorld);


		if (result.iterscetion)
			dx = result.distance;
		else
			dx = -1;
	}

	// y axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationY(-Math::PI_1 / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		RayIntersectionInfo result;

		Math::RayIntersection(result, ray, mVertex_Rotate, mIndex_Rotate,
			mNumIndex_Rotate / 3, matWorld);

		if (result.iterscetion)
			dy = result.distance;
		else
			dy = -2;
	}

	// z axis
	{
		Mat4 matLocal, matWorld;

		matLocal.MakeRotationX(Math::PI_1 / 2);
		matWorld.MakeTransform(position, Quat::Identity, scale);
		matWorld = matLocal * matWorld;

		RayIntersectionInfo result;

		Math::RayIntersection(result, ray, mVertex_Rotate, mIndex_Rotate,
			mNumIndex_Rotate / 3, matWorld);

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
		mPicked = true;
	}
}

void xGizmo::_mouseMoved_Rotate()
{
	Camera * cam = World::Instance()->MainCamera();

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);
	Vec3 pos = obj->GetPosition();

	float w = _getObjSize(obj);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Point2f pt = IMouse::Instance()->GetPositionDiffUnit();

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Vec3 p0 = pos;
		Vec3 p1, axis;

		if (mPickedAxis == 0)
		{
			p1 = Vec3(0, 0, 1);
			axis = Vec3::UnitX;
		}
		else if (mPickedAxis == 1)
		{
			p1 = Vec3(-1, 0, 0);
			axis = Vec3::UnitY;
		}
		else
		{
			p1 = Vec3(0, -1, 0);
			axis = Vec3::UnitZ;
		}

		Vec3 p2 = p0 * matVP;
		Vec3 p3 = (p0 + p1) * matVP;

		p2.x = (p2.x + 1) / 2;
		p2.y = (1 - p2.y) / 2;

		p3.x = (p3.x + 1) / 2;
		p3.y = (1 - p3.y) / 2;

		Vec2 d = Vec2(p3.x - p2.x, p3.y - p2.y);
		d.NormalizeL();

		dt = d.x * pt.x + d.y * pt.y;

		Quat ort = obj->GetOrientation();

		Quat q;
		q.FromAxis(axis, dt * Math::PI_1);
		ort = ort * q;
		obj->SetOrientation(ort);
	}
}

void xGizmo::_update_Scale()
{
	Camera * cam = World::Instance()->MainCamera();

	if (mPicked)
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();

	Ray ray = cam->GetViewportRay(pt.x, pt.y);

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	float w = _getObjSize(obj);

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

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			Vec3 p = mVertex_Scale[i] * matWorld;
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

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			Vec3 p = mVertex_Scale[i] * matWorld;
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

		for (int i = 0; i < mNumVertex_Scale; ++i)
		{
			Vec3 p = mVertex_Scale[i] * matWorld;
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
		mPicked = true;
	}
}

void xGizmo::_mouseMoved_Scale()
{
	Camera * cam = World::Instance()->MainCamera();

	if (xApp::Instance()->GetSelectedObjSize() == 0 ||
		xApp::Instance()->GetSelectedObjSize() > 1)
		return ;

	xObj * obj = xApp::Instance()->GetSelectedObj(0);

	if (mPickedAxis != -1 && mPicked && IMouse::Instance()->MouseMoved())
	{
		Point2f pt = IMouse::Instance()->GetPositionDiffUnit();

		const Mat4 & matVP = cam->GetViewProjMatrix();

		float dt = 0;

		Vec3 p0 = obj->GetPosition();
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

		Vec3 scale = obj->GetScale();
		scale *= 1 + dt;
		obj->SetScale(scale);
	}
}

float xGizmo::_getObjSize(xObj * obj)
{
	Aabb box = obj->GetBound();
	Vec3 size = box.GetSize();
	float w = size.x;

	w = Math::Maximum(w, size.y);
	w = Math::Maximum(w, size.z);
	w *= 0.5f;
	
	Camera * cam = World::Instance()->MainCamera();
	Vec3 pos = obj->GetPosition();

	pos *= cam->GetViewProjMatrix();

	pos = Vec3(0.3f, 0, pos.z);

	pos *= cam->GetProjMatrix().Inverse();

	return Math::Maximum(pos.x, w);
}