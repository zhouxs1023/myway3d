#include "MWFXComposerRenderer.h"

using namespace Myway;

FXComposerRectObj::FXComposerRectObj()
{
    Initialize();
}

FXComposerRectObj::~FXComposerRectObj()
{
}

void FXComposerRectObj::Initialize()
{
    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->AddElement(0, 8, DECLTYPE_FLOAT2, DECLMETHOD_DEFAULT, DECLUSAGE_TEXCOORD, 0);
    decl->EndDecl();

    int stride = 16;
    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(64, USAGE_WRITEONLY, POOL_MANAGED);
    float * data;
    buffer->Lock(0, 0, (void**)&data, LOCK_DISCARD);
    data[0] = -1.0f;    data[1] = 1.0f;     data[2] = 0.0f;     data[3] = 0.0f;
    data[4] = 1.0f;     data[5] = 1.0f;     data[6] = 1.0f;     data[7] = 0.0f;
    data[8] = -1.0f;    data[9] = -1.0f;    data[10] = 0.0f;    data[11] = 1.0f;
    data[12] = 1.0f;    data[13] = -1.0f;   data[14] = 1.0f;    data[15] = 1.0f;
    buffer->Unlock();

    mVertexStream.SetCount(4);
    mVertexStream.SetDeclaration(decl);
    mVertexStream.Bind(0, buffer, stride);

    mPrimCount = 2;
    mPrimType = PRIM_TRIANGLESTRIP;
}
