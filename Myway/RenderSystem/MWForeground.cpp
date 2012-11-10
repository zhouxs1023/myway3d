#include "MWForeground.h"

using namespace Myway;

Foreground::Foreground(const String & sName, const String & sMaterialName)
: m_sName(sName),
  m_bVisible(TRUE),
  mZOrder(0)
{
    /*
    m_VertexCount = 4;
    m_IndexCount = 0;
    mPrimCount = 2;
    mPrimType = PRIM_TRIANGLESTRIP;

    m_VertexDecl = VideoBufferManager::Instance().CreateVertexDeclaration();
    m_VertexDecl->AddElement(TransStream::NUMBER, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    m_VertexDecl->AddElement(TransStream::NUMBER, 12, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
    m_VertexDecl->EndDecl();

    m_TransStream.Stride = 20;
    m_TransStream.pVertexBuffer = VideoBufferManager::Instance().CreateVertexBuffer(20 * 4, USAGE_NORMAL, POOL_MANAGED);
    float * data;
    m_TransStream.pVertexBuffer->Lock(0, 0, (void**)&data, LOCK_DISCARD);
    data[0] = -1.0f;    data[1] = 1.0f;     data[2] = 0.0f;     data[3] = 0.0f;     data[4] = 0.0f;
    data[5] = 1.0f;     data[6] = 1.0f;     data[7] = 0.0f;     data[8] = 1.0f;     data[9] = 0.0f;
    data[10] = -1.0f;   data[11] = -1.0f;   data[12] = 0.0f;    data[13] = 0.0f;    data[14] = 1.0f;
    data[15] = 1.0f;    data[16] = -1.0f;   data[17] = 0.0f;    data[18] = 1.0f;    data[19] = 1.0f;
    m_TransStream.pVertexBuffer->Unlock();
    */

    SetMaterial(sMaterialName);
}

Foreground::~Foreground()
{
}

void Foreground::SetVisible(bool visible)
{
    m_bVisible = visible;
}

bool Foreground::GetVisible()
{
    return m_bVisible;
}

const String & Foreground::GetName()
{
    return m_sName;
}

void Foreground::SetZOrder(int iZOrder)
{
    mZOrder = iZOrder;
}

int Foreground::GetZOrder() const
{
    return mZOrder;
}
