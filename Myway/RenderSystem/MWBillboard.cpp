#include "MWBillboard.h"
#include "MWRenderEvent.h"

namespace Myway {

Billboard::Billboard(Technique * tech)
: Mover("Billboard")
, mTexcoord(0, 0, 1, 1)
, mColor(Color4::White)
, mWidth(0)
, mHeight(0)
{
	SetCastShadow(false);
	SetUsingDefferedShading(false);
	mNeedUpdate = true;
	mTechnique = tech;

	_initGeometry();
}

Billboard::~Billboard()
{
}

void Billboard::_initGeometry()
{
	int iVertexCount = 4;
	int iPrimCount = 2;

	VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	decl->AddElement(0, 0, DT_FLOAT4, DU_POSITION, 0);
	decl->AddElement(0, 16, DT_FLOAT4, DU_COLOR, 0);
	decl->Init();

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 32, 32);

	GetVertexStream()->SetDeclaration(decl);
	GetVertexStream()->SetCount(iVertexCount);
	GetVertexStream()->Bind(0, vb, 32);

	SetPrimitiveCount(iPrimCount);
	SetPrimitiveType(PRIM_TRIANGLESTRIP);
}

void Billboard::SetTexcoord(const RectF & rc)
{
	mTexcoord = rc;
	mNeedUpdate = true;
}

void Billboard::SetColor(const Color4 & color)
{
    mColor = color;
	mNeedUpdate = true;
}

void Billboard::SetWidth(float width)
{
    mWidth = width;
	mNeedUpdate = true;
}

void Billboard::SetHeight(float height)
{
    mHeight = height;
	mNeedUpdate = true;
}

float Billboard::GetWidth()
{
    return mWidth;
}

float Billboard::GetHeight()
{
    return mHeight;
}

const RectF & Billboard::GetTexcoord()
{
	return mTexcoord;
}

const Aabb & Billboard::GetWorldAabb()
{
	d_assert (mNode);

	return mNode->GetWorldAabb();
}

void Billboard::GetWorldPosition(Vec3 * pos)
{
	*pos = mNode->GetWorldPosition();
}

void Billboard::GetWorldTransform(Mat4 * form)
{
	*form = mNode->GetWorldMatrix();
}

void Billboard::UpdateGeometry()
{
	if (!mNeedUpdate)
		return ;

	VertexBufferPtr vb = GetVertexStream()->GetStream(0);

	float half_w = mWidth / 2;
	float half_h = mHeight / 2;

	float * verts = (float *)vb->Lock(0, 0, LOCK_DISCARD);
	{
		*verts++ = mTexcoord.x1; *verts++ = mTexcoord.y1;
		*verts++ = -half_w; *verts++ = half_h;
		*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;

		*verts++ = mTexcoord.x2; *verts++ = mTexcoord.y1;
		*verts++ = half_w; *verts++ = half_h;
		*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;

		*verts++ = mTexcoord.x1; *verts++ = mTexcoord.y2;
		*verts++ = -half_w; *verts++ = -half_h;
		*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;

		*verts++ = mTexcoord.x2; *verts++ = mTexcoord.y2;
		*verts++ = half_w; *verts++ = -half_h;
		*verts++ = mColor.r; *verts++ = mColor.g; *verts++ = mColor.b; *verts++ = mColor.a;
	}
	vb->Unlock();

	mNeedUpdate = false;
}

void Billboard::AddRenderQueue(RenderQueue * RQ)
{
	RQ->AddRenderer(this);
}

Technique *	Billboard::GetTechnique(eRenderTechType::enum_t type)
{
	d_assert (mTechnique && type == eRenderTechType::RTT_Base);
	return mTechnique;
}



BillboardManager gBillboardManager;

IMP_SLN(BillboardManager);

BillboardManager::BillboardManager()
{
	INIT_SLN;
}

BillboardManager::~BillboardManager()
{
	SHUT_SLN;
}

Billboard * BillboardManager::Create(Technique * tech)
{
	return new Billboard(tech);
}

void BillboardManager::Destroy(Billboard * billboard)
{
	delete billboard;
}


}
