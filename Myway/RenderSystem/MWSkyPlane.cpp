#include "MWSkyPlane.h"

using namespace Myway;

SkyPlane::SkyPlane(const String & sName, const String & sMaterialName, float width, float height, float depth)
: m_sName(sName),
  mWidth(width),
  mHeight(height),
  mDepth(depth),
  m_bEnable(FALSE)
{
    _Initialize();
    SetMaterial(sMaterialName);
}

SkyPlane::~SkyPlane()
{
}

const String & SkyPlane::GetName() const
{
    return m_sName;
}

float SkyPlane::GetWidth() const
{
    return mWidth;
}

float SkyPlane::GetHeight() const
{
    return mHeight;
}

float SkyPlane::GetDepth() const
{
    return mDepth;
}

void SkyPlane::SetEnable(bool bEnable)
{
    m_bEnable = bEnable;
}

bool SkyPlane::GetEnable() const
{
    return m_bEnable;
}

void SkyPlane::_Initialize()
{
    int iVertexCount = 4;
    int iIndexCount = 0;
    int iPrimCount = 2;

    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->AddElement(0, 12, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
    decl->EndDecl();

    mVertexStream.SetDeclaration(decl);
    
    EXCEPTION_DEBUG(iIndexCount < 65536, "index buffer to large");

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(20 * iVertexCount,
                                                                               USAGE_WRITEONLY,
                                                                               POOL_MANAGED);

    float * verteces;
    buffer->Lock(0, 0, (void**)&verteces, LOCK_DISCARD);
    {
        float half_w = mWidth * 0.5f;
        float half_h = mHeight * 0.5f;
        float half_d = mDepth * 0.5f;

        *verteces++ = -half_w;
        *verteces++ = mHeight;
        *verteces++ = -half_d;
        *verteces++ = 0.0f;
        *verteces++ = 1.0f;

        *verteces++ = half_w;
        *verteces++ = mHeight;
        *verteces++ = -half_d;
        *verteces++ = 1.0f;
        *verteces++ = 1.0f;

        *verteces++ = -half_w;
        *verteces++ = mHeight;
        *verteces++ = half_d;
        *verteces++ = 0.0f;
        *verteces++ = 0.0f;

        *verteces++ = half_w;
        *verteces++ = mHeight;
        *verteces++ = half_d;
        *verteces++ = 1.0f;
        *verteces++ = 0.0f;
    }
    buffer->Unlock();

    mVertexStream.SetCount(iVertexCount);
    mVertexStream.Bind(0, buffer, 20);

    SetPrimitiveCount(iPrimCount);
    SetPrimitiveType(PRIM_TRIANGLESTRIP);
}