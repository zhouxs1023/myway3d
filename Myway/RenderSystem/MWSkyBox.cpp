#include "MWSkyBox.h"

using namespace Myway;

SkyBox::SkyBox(const String & sName, const String & sMaterialName, float width, float height, float depth)
: m_sName(sName),
  mWidth(width),
  mHeight(height),
  mDepth(depth),
  m_bEnable(FALSE)
{
    _Initialize();
    SetMaterial(sMaterialName);
}

SkyBox::~SkyBox()
{
}

const String & SkyBox::GetName() const
{
    return m_sName;
}

float SkyBox::GetWidth() const
{
    return mWidth;
}

float SkyBox::GetHeight() const
{
    return mHeight;
}

float SkyBox::GetDepth() const
{
    return mDepth;
}

void SkyBox::SetEnable(bool bEnable)
{
    m_bEnable = bEnable;
}

bool SkyBox::GetEnable() const
{
    return m_bEnable;
}

void SkyBox::_Initialize()
{
    int iVertexCount = 8;
    int iIndexCount = 12 * 3;
    int iPrimCount = 12;

    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->EndDecl();

    mVertexStream.SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(iVertexCount * 12, USAGE_WRITEONLY, POOL_MANAGED);

    float * verteces;
    const float half_w = mWidth * 0.5f;
    const float half_h = mHeight * 0.5f;
    const float half_d = mDepth * 0.5f;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD);
    {
        Vec3 pos, normal;

        //front
        pos = Vec3(-half_w, half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(-half_w, -half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, -half_h, -half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        //back
        pos = Vec3(-half_w, half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(-half_w, -half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;

        pos = Vec3(half_w, -half_h, half_d);
        Math::VecNormalize(normal, pos);
        *verteces++ = pos.x;
        *verteces++ = pos.y;
        *verteces++ = pos.z;
        *verteces++ = normal.x;
        *verteces++ = normal.y;
        *verteces++ = normal.z;
    }
    buffer->Unlock();

    mVertexStream.SetCount(iVertexCount);
    mVertexStream.Bind(0, buffer, 12);

    //index stream
    IndexBufferPtr ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(iIndexCount * sizeof(short),
                                                                              USAGE_WRITEONLY,
                                                                              POOL_MANAGED,
                                                                              FMT_INDEX16);
    short * indices;
    ibuffer->Lock(0, 0, (void**)&indices, LOCK_DISCARD);
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
    ibuffer->Unlock();

    mIndexStream.SetCount(iIndexCount);
    mIndexStream.Bind(ibuffer, 0);

    SetPrimitiveCount(iPrimCount);
    SetPrimitiveType(PRIM_TRIANGLELIST);
}