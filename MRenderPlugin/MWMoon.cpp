#include "MWMoon.h"
#include "MWEnvironment.h"
#include "MWAstronomy.h"

namespace Myway {

    Moon::Moon()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Moon");

        _geometry();
    }

    Moon::~Moon()
    {
    }

    void Moon::Render()
    {
        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        Vec3 moonPos = Environment::Instance()->GetEvParam()->MoonPos;
        Vec3 moonDir = Environment::Instance()->GetEvParam()->MoonDir;
        float moonLum = Environment::Instance()->GetEvParam()->MoonLum;
        float moonSize = Environment::Instance()->GetEvParam()->MoonSize;
        float moonPhase = Environment::Instance()->GetEvParam()->MoonPase;
        const char * moonTex = Environment::Instance()->GetGlobalParam()->MoonTexture.c_str();

        _setTexture(moonTex);

        ShaderParam * uTransform = mTech->GetVertexShaderParamTable()->GetParam("gTransform");
        ShaderParam * uMoonParam = mTech->GetPixelShaderParamTable()->GetParam("gMoonParam");

        uTransform->SetUnifom(moonPos.x, moonPos.y, moonPos.z, moonSize);
        uMoonParam->SetUnifom(moonPhase, moonLum, 0, 0);

        SamplerState s0;
        Texture * t0 = mTexture.c_ptr();
        
        render->SetTexture(0, s0, t0);
        render->Render(mTech, &mRender);
    }

    void Moon::_geometry()
    {
        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int iVertexCount = 4;
        int iPrimCount = 2;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, 20);

        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            float x = 0, y = 0, z = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 0; *vert++ = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 1; *vert++ = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 0; *vert++ = 1;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 1; *vert++ = 1;
        }
        vb->Unlock();

        vxStream->Bind(0, vb, 20);
        vxStream->SetCount(iVertexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLESTRIP;

        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }

    void Moon::_setTexture(const char * tex)
    {
        if (mTexture == NULL || mTexture->GetName() != tex)
            mTexture = VideoBufferManager::Instance()->Load2DTexture(tex, tex);
    }

}