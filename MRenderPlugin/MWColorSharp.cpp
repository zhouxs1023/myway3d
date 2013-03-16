#include "MWColorSharp.h"
#include "MWEnvironment.h"
#include "MWRenderHelper.h"
#include "MWKeyboard.h"

namespace Myway {

    ColorSharp::ColorSharp()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("ColorSharp");
        d_assert (mTech);
    }

    ColorSharp::~ColorSharp()
    {
    }

    void ColorSharp::Render(Texture * tex, Texture * texDepth)
    {
		RS_RenderEvent(ColorSharp);

		float colorSharpScale = Environment::Instance()->GetEvParam()->ColorSharpScale;
		float colorSharpFadeStart = Environment::Instance()->GetGlobalParam()->ColorSharpFadeStart;
        float colorSharpFadeEnd = Environment::Instance()->GetGlobalParam()->ColorSharpFadeEnd;

        ShaderParam * uSharpParam = mTech->GetPixelShaderParamTable()->GetParam("gSharpParam");

        uSharpParam->SetUnifom(colorSharpScale, colorSharpFadeStart, 1 / (colorSharpFadeEnd - colorSharpFadeStart), 0);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        state.Filter = TEXF_POINT;

		RenderSystem::Instance()->SetTexture(0, state, tex);
        RenderSystem::Instance()->SetTexture(1, state, texDepth);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech);
    }
}