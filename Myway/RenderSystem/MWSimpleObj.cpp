#include "MWSimpleObj.h"
#include "MWSceneManager.h"
#include "MWRenderQueue.h"

using namespace Myway;

#define _DECL_POSITION   1
#define _DECL_NORMAL     2
#define _DECL_COLOR      4
#define _DECL_TEXCOORD   8


DECLARE_GUID128_CONST(SimpleObj::Guid, 's', 'i', 'm', 'p', 'l', 'e', 'o', 'b',
                                       'j', 0, 0, 0, 0, 0, 0, 0);


SimpleObj::SimpleObj(const String & name)
: m_name(name),
  m_begined(FALSE),
  m_need_render(FALSE),
  m_need_update(FALSE),
  m_auto_update_bound(TRUE),
  m_decl_flag(0)
{
}

SimpleObj::~SimpleObj()
{
}

const String & SimpleObj::GetName() const
{
    return m_name;
}

void SimpleObj::Begin(PRIMITIVE_TYPE type)
{
    debug_assert(!m_begined, "Simple object has already begined.");

    m_begined = TRUE;
    m_need_render = FALSE;
    m_need_update = FALSE;

    mositions.Clear();
    m_normals.Clear();
    m_colors.Clear();
    m_texcoords.Clear();
    mndeces.Clear();

    SetPrimitiveType(type);
}

void SimpleObj::End()
{
    m_begined = FALSE;
    
    if (mositions.Size())
    {
        m_need_render = TRUE;
        m_need_update = TRUE;
    }
    else
    {
        m_need_render = FALSE;
        m_need_update = FALSE;
    }

    if (m_auto_update_bound)
        UpdateBound();
}

void SimpleObj::AddPosition(const Vec3 & pos)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    mositions.PushBack(pos);
}

void SimpleObj::AddPosition(float x, float y, float z)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    mositions.PushBack(Vec3(x, y, z));
}

void SimpleObj::AddNormal(const Vec3 & normal)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_normals.PushBack(normal);
}

void SimpleObj::AddNormal(float x, float y, float z)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_normals.PushBack(Vec3(x, y, z));
}

void SimpleObj::AddColor(const Color4 & color)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_colors.PushBack(color);
}

void SimpleObj::AddColor(float r, float g, float b, float a)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_colors.PushBack(Color4(r, g, b, a));
}

void SimpleObj::AddTexcoord(const Vec2 & texcoord)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_texcoords.PushBack(texcoord);
}

void SimpleObj::AddTexcoord(float u, float v)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    m_texcoords.PushBack(Vec2(u, v));
}

void SimpleObj::AddIndex(short index)
{
    debug_assert(m_begined, "Simple object didn't begin.");

    mndeces.PushBack(index);
}


void SimpleObj::SetAutoUpdateBound(bool auto_update)
{
    m_auto_update_bound = auto_update;
}

bool SimpleObj::GetAutoUpdateBound() const
{
    return m_auto_update_bound;
}

void SimpleObj::UpdateBound()
{
    if (mositions.Size())
    {
        Vector<Vec3>::Iterator iter;
        Vector<Vec3>::Iterator end;

        iter = mositions.Begin();
        end = mositions.End();

        m_aabb_local.minimum = *iter;
        m_aabb_local.maximum = *iter;

        while (++iter != end)
        {
            Math::VecMinimum(m_aabb_local.minimum, m_aabb_local.minimum, *iter);
            Math::VecMaximum(m_aabb_local.maximum, m_aabb_local.maximum, *iter);
        }

        m_sph_local.center = m_aabb_local.GetCenter();
        m_sph_local.radius = Math::VecDistance(m_sph_local.center, m_aabb_local.minimum);
    }
    else
    {
        m_aabb_local = Aabb::Zero;
        m_sph_local = Sphere::Zero;
    }

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

void SimpleObj::UpdateGeomtry()
{
    int decl_flag = _DECL_POSITION;

    if (m_normals.Size())
    {
        debug_assert(m_normals.Size() == mositions.Size(), "normal size != position size.");
        decl_flag |= _DECL_NORMAL;
    }

    if (m_colors.Size())
    {
        debug_assert(m_colors.Size() == mositions.Size(), "color size != position size.");
        decl_flag |= _DECL_COLOR;
    }

    if (m_texcoords.Size())
    {
        debug_assert(m_texcoords.Size() == mositions.Size(), "texcoord size != position size.");
        decl_flag |= _DECL_TEXCOORD;
    }

    int stride = 0;

    if (decl_flag != m_decl_flag)
    {
        VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();

        decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
        stride += 12;

        if (decl_flag & _DECL_NORMAL)
        {
            decl->AddElement(0, stride, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_NORMAL, 0);
            stride += 12;
        }

        if (decl_flag & _DECL_COLOR)
        {
            decl->AddElement(0, stride, DECLTYPE_FLOAT4, DECLMETHOD_DEFAULT, DECLUSAGE_COLOR, 0);
            stride += 16;
        }

        if (decl_flag & _DECL_TEXCOORD)
        {
            decl->AddElement(0, stride, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
            stride += 8;
        }

        decl->EndDecl();

        m_decl_flag = decl_flag;

        mVertexStream.SetDeclaration(decl);
    }

    stride = mVertexStream.GetDeclaration()->GetStreamSize(0);

    int size = stride * mositions.Size();

    VertexBufferPtr buffer = mVertexStream.GetStream(0);

    if (buffer.IsNull() || buffer->GetSize() < size)
    {
        buffer = VideoBufferManager::Instance().CreateVertexBuffer(size, USAGE_WRITEONLY, POOL_MANAGED);
    }


    //Update Geomtry;

    Vector<Vec3>::Iterator      pi;
    Vector<Vec3>::Iterator      pe;
    Vector<Vec3>::Iterator      ni;
    Vector<Vec3>::Iterator      ne;
    Vector<Color4>::Iterator    ci;
    Vector<Color4>::Iterator    ce;
    Vector<Vec2>::Iterator      ti;
    Vector<Vec2>::Iterator      te;

    pi = mositions.Begin();
    pe = mositions.End();
    ni = m_normals.Begin();
    ne = m_normals.End();
    ci = m_colors.Begin();
    ce = m_colors.End();
    ti = m_texcoords.Begin();
    te = m_texcoords.End();

    char * verteces;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD);

    while (pi != pe)
    {
        int offset = 0;

        if (m_decl_flag & _DECL_POSITION)
        {
            float * position = (float*)(verteces + offset);

            if (pi != pe)
            {
                position[0] = pi->x;
                position[1] = pi->y;
                position[2] = pi->z;

                ++pi;
            }

            offset += sizeof(Vec3);
        }


        if (m_decl_flag & _DECL_NORMAL)
        {
            float * normal = (float*)(verteces + offset);

            if (ni != ne)
            {
                normal[0] = ni->x;
                normal[1] = ni->y;
                normal[2] = ni->z;

                ++ni;
            }

            offset += sizeof(Vec3);
        }

        if (m_decl_flag & _DECL_COLOR)
        {
            float * color = (float*)(verteces + offset);

            if (ci != ce)
            {
                color[0] = ci->r;
                color[1] = ci->g;
                color[2] = ci->b;
                color[3] = ci->a;

                ++ci;
            }

            offset += sizeof(Color4);
        }

        if (m_decl_flag & _DECL_TEXCOORD)
        {
            float * texcoord = (float*)(verteces + offset);

            if (ti != te)
            {
                texcoord[0] = ti->x;
                texcoord[1] = ti->y;

                ++ti;
            }
        }

        verteces += stride;
    }
    
    buffer->Unlock();

    mVertexStream.Bind(0, buffer, stride);
    mVertexStream.SetCount(mositions.Size());

    //update index buffer
    if (mndeces.Size())
    {
        debug_assert(mndeces.Size() < 65536, "Simple object index buffer is to large.");
        size = sizeof(short) * mndeces.Size();

        IndexBufferPtr ibuffer = mIndexStream.GetStream();
        
        if (mIndexStream.GetCount() < mndeces.Size())
        {
            ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(size, USAGE_WRITEONLY, POOL_MANAGED, FMT_INDEX16);
        }

        short * indeces;
        ibuffer->Lock(0, 0, (void**)&indeces, LOCK_DISCARD);
        Memcpy(indeces, &mndeces[0], size);
        ibuffer->Unlock();

        mIndexStream.Bind(ibuffer, 0);
    }

    mIndexStream.SetCount(mndeces.Size());

    switch (mPrimType)
    {
    case PRIM_POINTLIST:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() : mositions.Size());
        break;

    case PRIM_LINELIST:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() / 2 : mositions.Size() / 2);
        break;

    case PRIM_LINESTRIP:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() - 1 : mositions.Size() - 1);
        break;

    case PRIM_TRIANGLELIST:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() / 3 : mositions.Size() / 3);
        break;

    case PRIM_TRIANGLESTRIP:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() - 2 : mositions.Size() - 2);
        break;

    case PRIM_TRIANGLEFAN:
        SetPrimitiveCount(mndeces.Size() ? mndeces.Size() - 2 : mositions.Size() - 2);
        break;
    }
}

const guid128 & SimpleObj::GetGuid()
{
    return Guid;
}

void SimpleObj::AddRenderQueue(RenderQueue * rq)
{
    if (m_need_update)
        UpdateGeomtry();

    if (m_need_render)
        rq->AddRenderer(this);
}

void SimpleObj::GetWorldPosition(Vec3 * pos)
{
    *pos = m_node->GetWorldPosition();
}

void SimpleObj::GetWorldTransform(Mat4 * form)
{
    *form = m_node->GetWorldMatrix();
}

LightList * SimpleObj::GetLightList()
{
    return &m_lights;
}
