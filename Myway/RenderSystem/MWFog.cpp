#include "MWFog.h"
#include "MWEnvironment.h"
#include "MWRenderHelper.h"

namespace Myway {

    Fog::Fog()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterFog");
        d_assert (mTech);
    }

    Fog::~Fog()
    {
    }

    void Fog::Render(Texture * depthTex, Texture * sceneTex)
    {
        float fogStart = Environment::Instance()->GetEvParam()->FogStart;
        float fogEnd = Environment::Instance()->GetEvParam()->FogEnd;
        Color4 fogColor = Environment::Instance()->GetEvParam()->FogColor;

        Camera * cam = World::Instance()->MainCamera();
        const Vec3 * corner = cam->GetCorner();

        Vec3 cornerLeftTop = corner[4];
        Vec3 cornerRightDir = corner[5] - corner[4];
        Vec3 cornerDownDir = corner[6] - corner[4];

        ShaderParam * uFogParam = mTech->GetPixelShaderParamTable()->GetParam("gFogParam");
        ShaderParam * uFogColor = mTech->GetPixelShaderParamTable()->GetParam("gFogColor");
        ShaderParam * uCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
        ShaderParam * uCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
        ShaderParam * uCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerDownDir");

        uFogParam->SetUnifom(fogStart, fogEnd, 1 / (fogEnd - fogStart), 0);
        uFogColor->SetUnifom(fogColor.r, fogColor.g, fogColor.b, 0);
		uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
		uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
		uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        state.Filter = TEXF_POINT;

        RenderSystem::Instance()->SetTexture(0, state, depthTex);
        RenderSystem::Instance()->SetTexture(1, state, sceneTex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech);
    }
}