#include "MWSun.h"
#include "MWEnvironment.h"
#include "MWAstronomy.h"
#include "MWRenderHelper.h"

namespace Myway {

    Sun::Sun()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Sun");
        mTech_Lighting = Environment::Instance()->GetShaderLib()->GetTechnique("DirLighting");

        d_assert (mTech_Lighting != NULL && mTech != NULL);

        _geometry();
    }

    Sun::~Sun()
    {
    }

    void Sun::Render()
    {
        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor;
        float sunLum = Environment::Instance()->GetEvParam()->SunLum;
        float sunPower = Environment::Instance()->GetEvParam()->SunPower;
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        float sunSize = Environment::Instance()->GetEvParam()->SunSize;
        Vec3 sunPos = Environment::Instance()->GetEvParam()->SunPos;

        ShaderParam * uTransform = mTech->GetVertexShaderParamTable()->GetParam("gTransform");
        ShaderParam * uSunColor = mTech->GetPixelShaderParamTable()->GetParam("gSunColor");
        ShaderParam * uSunParam = mTech->GetPixelShaderParamTable()->GetParam("gSunParam");

        uTransform->SetUnifom(sunPos.x, sunPos.y, sunPos.z, sunSize);

        uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, sunColor.a);
        uSunParam->SetUnifom(sunPower, sunLum, 0, 0);

        render->Render(mTech, &mRender);
    }

    void Sun::Lighting(Texture * colorTex, Texture * normalTex)
    {
        ShaderParam * uLightDir = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gLightDir");
        ShaderParam * uAmbient = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gAmbient");
        ShaderParam * uDiffuse = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gDiffuse");
        ShaderParam * uSpecular = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gSpecular");

        Vec3 lightDir = -Environment::Instance()->GetEvParam()->SunDir;
        Color4 ambient = Color4::Gray;
        Color4 diffuse = Color4::White;
        Color4 specular = Color4::White;

        uLightDir->SetUnifom(lightDir.x, lightDir.y, lightDir.z, 0);
        uAmbient->SetUnifom(ambient.r, ambient.g, ambient.b, ambient.a);
        uDiffuse->SetUnifom(diffuse.r, diffuse.g, diffuse.b, diffuse.a);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        state.Filter = TEXF_POINT;

        RenderSystem::Instance()->SetTexture(0, state, colorTex);
        RenderSystem::Instance()->SetTexture(1, state, normalTex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Lighting);
    }

    void Sun::_geometry()
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

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20);

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

        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }

}