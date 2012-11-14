#include "MWSky.h"
#include "MWEnvironment.h"

namespace Myway {

    Sky::Sky()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Sky");

        d_assert (mTech);

        _geometry();
    }

    Sky::~Sky()
    {
    }

    void Sky::Render()
    {
        float mU = Environment::Instance()->GetEvParam()->SkyU;
        float mVOff = Environment::Instance()->GetGlobalParam()->SkyVOffset;
        const TString128 & skyTexture = Environment::Instance()->GetGlobalParam()->SkyTexture;

        if (mTexture.IsNull() || mTexture->GetName() == skyTexture)
            mTexture = VideoBufferManager::Instance()->Load2DTexture(skyTexture, skyTexture);

        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        float farclip = cam->GetFarClip() * 0.9f;
        Vec3 pos = cam->GetPosition();

		ShaderParam * uTransform = mTech->GetVertexShaderParamTable()->GetParam("gTransform");
		ShaderParam * uU = mTech->GetVertexShaderParamTable()->GetParam("gU");

		uTransform->SetUnifom(pos.x, pos.y, pos.z, farclip);
		uU->SetUnifom(mU, mVOff, 1 / (1 + mVOff), 0);

		SamplerState state;
		state.Address = TEXA_CLAMP;
		render->SetTexture(0, state, mTexture.c_ptr());

		render->Render(mTech, &mRender);
    }

    void Sky::_geometry()
    {
        int iRings = 10, iSegments = 10;
        int iVertexCount = (iRings + 1) * (iSegments + 1);
        int iIndexCount = iRings * iSegments * 6;
        int iPrimCount = iIndexCount / 3;

        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        d_assert(iIndexCount < 65536 &&"index buffer too large.");

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12);
        
        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            float fTileRingAngle = (Math::PI_1 / iRings);
            float fTileSegAngle = (Math::PI_2 / iSegments);
            float r;
            short i, j;
            float x, y, z;

            for (i = 0; i <= iRings; ++i)
            {
                r = Math::Sin(i * fTileRingAngle);
                y = Math::Cos(i * fTileRingAngle);

                for (j = 0; j <= iSegments; ++j)
                {
                    x = r * Math::Cos(j * fTileSegAngle);
                    z = r * Math::Sin(j * fTileSegAngle);

                    *vert++ = x;
                    *vert++ = y;
                    *vert++ = z;
                }

            }
        }
        vb->Unlock();

        vxStream->Bind(0,vb, 12);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
        short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
        {
            short row = 0, row_n = 0;
            short i, j;

            for (i = 0; i < iRings; ++i)
            {
                row_n = row + iSegments + 1;

                for (j = 0; j < iSegments; ++j)
                {
                    *idx++ = row + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j;

                    *idx++ = row_n + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j + 1;

                }

                row += iSegments + 1;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }






    Sky2::Sky2()
    {
    }

    Sky2::~Sky2()
    {
    }

    void Sky2::_init()
    {
        int iRings = 10, iSegments = 10;
        int iVertexCount = (iRings + 1) * (iSegments + 1);
        int iIndexCount = iRings * iSegments * 6;
        int iPrimCount = iIndexCount / 3;

        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        d_assert(iIndexCount < 65536 &&"index buffer too large.");

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 12);

        vxStream->Bind(0,vb, 12);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
        short * idx = (short *)ib->Lock(0, 0, LOCK_DISCARD);
        {
            short row = 0, row_n = 0;
            short i, j;

            for (i = 0; i < iRings; ++i)
            {
                row_n = row + iSegments + 1;

                for (j = 0; j < iSegments; ++j)
                {
                    *idx++ = row + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j;

                    *idx++ = row_n + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j + 1;

                }

                row += iSegments + 1;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }

}