#include "MWTriangleObj.h"
#include "MWRenderQueue.h"
#include "MWNode.h"


using namespace Myway;


DECLARE_GUID128_CONST(TriangleObj::guid, 't', 'r', 'i', 'a', 'n', 'g', 'l', 'e',
                                         'o', 'b', 'j', 0, 0, 0, 0, 0);


TriangleObj::TriangleObj(const String & name)
: m_name(name),
  moint1(Vec3::Zero),
  moint2(Vec3::Zero),
  moint3(Vec3::Zero),
  m_need_update(FALSE),
  m_need_render(FALSE),
  mMode(TRUE)
{
    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->EndDecl();

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(3 * sizeof(Vec3), 
                                                                               USAGE_WRITEONLY,
                                                                               POOL_MANAGED);

    mVertexStream.SetCount(3);
    mVertexStream.SetDeclaration(decl);
    mVertexStream.Bind(0, buffer, sizeof(Vec3));

    mPrimType = PRIM_LINESTRIP;
    mPrimCount = 3;
}

TriangleObj::~TriangleObj()
{
}

const guid128 & TriangleObj::GetGuid()
{
    return guid;
}

const String & TriangleObj::GetName() const
{
    return m_name;
}

void TriangleObj::SetFrameMode(bool bFrameModel)
{
    mMode = bFrameModel;

    if (mMode)
    {
        mPrimCount = 3;
        mPrimType = PRIM_LINESTRIP;
    }
    else
    {
        mPrimCount = 1;
        mPrimType = PRIM_TRIANGLELIST;
    }
}

bool TriangleObj::GetFrameMode() const
{
    return mMode;
}

void TriangleObj::SetPoint(const Vec3 & p1, const Vec3 & p2, const Vec3 & p3)
{
    m_need_render = TRUE;
    m_need_update = TRUE;

    moint1 = p1;
    moint2 = p2;
    moint3 = p3;

    _UpdateBound();
}

const Vec3 & TriangleObj::GetPoint1() const
{
    return moint1;
}

const Vec3 & TriangleObj::GetPoint2() const
{
    return moint2;
}

const Vec3 & TriangleObj::GetPoint3() const
{
    return moint3;
}

void TriangleObj::_UpdateBound()
{
    Math::VecMinimum(m_aabb_local.minimum, moint1, moint2);
    Math::VecMinimum(m_aabb_local.minimum, m_aabb_local.minimum, moint3);

    Math::VecMaximum(m_aabb_local.maximum, moint1, moint2);
    Math::VecMaximum(m_aabb_local.maximum, m_aabb_local.maximum, moint3);

    m_sph_local.center = (moint1 + moint2) * 0.5f;
    m_sph_local.center = (m_sph_local.center + moint3) * 0.5f;
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
        m_sph_world = m_sph_local;
    }
}

void TriangleObj::_UpdateGeomtry()
{
    Vec3 * data;

    VertexBufferPtr buffer = mVertexStream.GetStream(0);

    buffer->Lock(NULL, 0, (void**)&data, LOCK_DISCARD);

    data[0] = moint1;
    data[1] = moint2;
    data[2] = moint3;

    buffer->Unlock();

    m_need_update = FALSE;
}

void TriangleObj::AddRenderQueue(RenderQueue * rq)
{
    if (m_need_render)
    {
        if (m_need_update)
            _UpdateGeomtry();

        rq->AddRenderer(this);
    }
}

void TriangleObj::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void TriangleObj::GetWorldTransform(Mat4 * form)
{
    *form = m_node->GetWorldMatrix();
}
