#include "MWViewAlignedQuad.h"
#include "MWRenderQueue.h"

using namespace Myway;

ViewAlignedQuad::ViewAlignedQuad(const String & sName, const String & sMaterialName)
: m_sName(sName)
{
    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->AddElement(0, 8, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
    decl->EndDecl();

    mVertexStream.SetDeclaration(decl);

    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(64, USAGE_WRITEONLY, POOL_MANAGED);

    float * data;
    buffer->Lock(0, 0, (void**)&data, LOCK_DISCARD);
    data[0] = -1.0f;    data[1] = 1.0f;     data[2] = 0.0f;     data[3] = 0.0f;
    data[4] = 1.0f;     data[5] = 1.0f;     data[6] = 1.0f;     data[7] = 0.0f;
    data[8] = -1.0f;    data[9] = -1.0f;    data[10] = 0.0f;    data[11] = 1.0f;
    data[12] = 1.0f;    data[13] = -1.0f;   data[14] = 1.0f;    data[15] = 1.0f;
    buffer->Unlock();

    mVertexStream.SetCount(4);
    mVertexStream.Bind(0, buffer, 16);

    mIndexStream.SetCount(0);
    mIndexStream.Bind(NULL, 0);

    mPrimCount = 2;
    mPrimType = PRIM_TRIANGLESTRIP;
    
    SetMaterial(sMaterialName);

    SetBounds(Aabb::Infinite, Sphere::Infinite);
}

ViewAlignedQuad::~ViewAlignedQuad()
{
}

const String & ViewAlignedQuad::GetName()
{
    return m_sName;
}

void ViewAlignedQuad::AddRenderQueue(RenderQueue * rq)
{
    rq->AddRenderer(this);
}