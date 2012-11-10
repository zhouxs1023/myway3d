#include "MWAABBRenderer.h"
#include "MWSceneNode.h"
#include "Engine.h"

using namespace Myway;

AABBRenderer::AABBRenderer(SceneNode * sn)
{
    mSceneNode = sn;

    _Initialize();
}

AABBRenderer::~AABBRenderer()
{
}

void AABBRenderer::_Initialize()
{
    VertexDeclarationPtr decl = VideoBufferManager::Instance().CreateVertexDeclaration();
    decl->AddElement(0, 0, DECLTYPE_FLOAT3, DECLMETHOD_DEFAULT, DECLUSAGE_POSITION, 0);
    decl->EndDecl();

    int stride = 12;
    VertexBufferPtr buffer = VideoBufferManager::Instance().CreateVertexBuffer(12 * 8, USAGE_WRITEONLY, POOL_DEFAULT);

    float * data;
    buffer->Lock(0, 0, (void**)&data, LOCK_DISCARD);
    {
        data[0] = -0.5f;    data[1] = 0.5f;     data[2] = -0.5f;
        data[3] = 0.5f;     data[4] = 0.5f;     data[5] = -0.5f;
        data[6] = -0.5f;    data[7] = -0.5f;    data[8] = -0.5f;
        data[9] = 0.5f;     data[10] = -0.5f;   data[11] = -0.5f;

        data[12] = -0.5f;   data[13] = 0.5f;    data[14] = 0.5f;
        data[15] = 0.5f;    data[16] = 0.5f;    data[17] = 0.5f;
        data[18] = -0.5f;   data[19] = -0.5f;   data[20] = 0.5f;
        data[21] = 0.5f;    data[22] = -0.5f;   data[23] = 0.5f;
    }
    buffer->Unlock();

    mVertexStream.SetCount(8);
    mVertexStream.SetDeclaration(decl);
    mVertexStream.Bind(0, buffer, stride);

    IndexBufferPtr ibuffer = VideoBufferManager::Instance().CreateIndexBuffer(24 * sizeof(short),
                                                                              USAGE_WRITEONLY,
                                                                              POOL_DEFAULT,
                                                                              FMT_INDEX16);
    short * indices;
    ibuffer->Lock(0, 0, (void**)&indices, LOCK_DISCARD);
    {
        indices[0] = 0;     indices[1] = 1;
        indices[2] = 2;     indices[3] = 3;
        indices[4] = 0;     indices[5] = 2;
        indices[6] = 1;     indices[7] = 3;

        indices[8] = 4;     indices[9] = 5;
        indices[10] = 6;    indices[11] = 7;
        indices[12] = 4;    indices[13] = 6;
        indices[14] = 5;    indices[15] = 7;

        indices[16] = 0;    indices[17] = 4;
        indices[18] = 2;    indices[19] = 6;
        indices[20] = 1;    indices[21] = 5;
        indices[22] = 3;    indices[23] = 7;
    }
    ibuffer->Unlock();

    mIndexStream.SetCount(24);
    mIndexStream.Bind(ibuffer, 0);

    mPrimCount = 12;
    mPrimType = PRIM_LINELIST;

    SetMaterial(Engine::Instance().GetAABBMaterial());
}

void AABBRenderer::GetWorldPosition(Vec3 * pos)
{
    *pos = mSceneNode->GetWorldAabb().GetCenter();
}

void AABBRenderer::GetWorldTransform(Mat4 * form)
{
    const Aabb & aabb = mSceneNode->GetWorldAabb();

    Math::MatTransform(*form, aabb.GetCenter(), Quat::Identity, Vec3(aabb.GetWidth(), aabb.GetHeight(), aabb.GetDepth()));
}
