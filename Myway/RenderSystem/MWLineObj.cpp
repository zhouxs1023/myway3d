#include "MWLineObj.h"
#include "MWRenderQueue.h"
#include "MWNode.h"

using namespace Myway;

DECLARE_GUID128_CONST(LineObj::guid, 'l', 'i', 'n', 'e', 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0);

LineObj::LineObj(const String & name)
: m_name(name),
  moint1(Vec3::Zero),
  moint2(Vec3::Zero),
  m_need_render(FALSE),
  m_need_update(FALSE)
{
    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->EndDecl();

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(2 * 12, USAGE_WRITEONLY, POOL_MANAGED);

    mVertexStream.SetCount(2);
    mVertexStream.SetDeclaration(decl);
    mVertexStream.Bind(0, buffer, 12);

    mPrimType = PRIM_LINELIST;
    mPrimCount = 1;
}

LineObj::~LineObj()
{
}

const String & LineObj::GetName() const
{
    return m_name;
}

void LineObj::PointTo(const Vec3 & p)
{
    m_need_render = FALSE;
    m_need_update = FALSE;
    moint1 = p;
}

void LineObj::MoveAt(const Vec3 & p)
{
    m_need_render = TRUE;
    m_need_update = TRUE;
    moint2 = p;

    UpdateBound();
}

const Vec3 & LineObj::GetPoint1() const
{
    return moint1;
}

const Vec3 & LineObj::GetPoint2() const
{
    return moint2;
}

const guid128 & LineObj::GetGuid()
{
    return guid;
}

void LineObj::UpdateBound()
{
    if (m_need_update)
        UpdateGeomtry();

    Math::VecMinimum(m_aabb_local.minimum, moint1, moint2);
    Math::VecMaximum(m_aabb_local.maximum, moint1, moint2);
    
    m_sph_local.center = (moint1 + moint2) * 0.5f;
    m_sph_local.radius = Math::VecDistance(m_sph_local.center, moint1);

    if (m_node)
    {
        const Mat4 & form = m_node->GetWorldMatrix();

        Math::AABBTransform(m_aabb_world, m_aabb_local, form);
        Math::SphereTransform(m_sph_world, m_sph_local, form);
    }
    else
    {
        m_aabb_world = m_aabb_local;
        m_aabb_world = m_aabb_local;
    }
}

void LineObj::UpdateGeomtry()
{
    Vec3 * data;

    VertexBufferPtr buffer = mVertexStream.GetStream(0);

    buffer->Lock(NULL, 0, (void**)&data, LOCK_DISCARD);

    data[0] = moint1;
    data[1] = moint2;

    buffer->Unlock();

    m_need_update = FALSE;
}

void LineObj::AddRenderQueue(RenderQueue * rq)
{
    if (m_need_update)
        UpdateGeomtry();

    if (m_need_render)
        rq->AddRenderer(this);
}

void LineObj::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void LineObj::GetWorldTransform(Mat4 * form)
{
    *form = m_node->GetWorldMatrix();
}