#include "MWShaderProvider_Prz.h"
#include "MWShaderFX.h"

namespace Myway
{
    ShaderProvider::ShaderProvider()
    {
        mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("main", "Shaders\\PZ_Main.ShaderLib");
        d_assert (mShaderLib);

		mTechs[eRenderTechType::RTT_Base] = mShaderLib->GetTechnique("base");
        mTechs[eRenderTechType::RTT_Mirror] = mShaderLib->GetTechnique("mirror");
        mTechs[eRenderTechType::RTT_Shadow] = mShaderLib->GetTechnique("shadow");

		mTechs_Skined[eRenderTechType::RTT_Base] = mShaderLib->GetTechnique("baseSkined");
		mTechs_Skined[eRenderTechType::RTT_Mirror] = mShaderLib->GetTechnique("mirrorSkined");
		mTechs_Skined[eRenderTechType::RTT_Shadow] = mShaderLib->GetTechnique("shadowSkined");

        mFrushTech = mShaderLib->GetTechnique("Frush");
        mClearTech = mShaderLib->GetTechnique("Clear");
		mTech_PointLight = mShaderLib->GetTechnique("Lighting_Point");

        d_assert (mFrushTech);
        d_assert (mClearTech);
		d_assert (mTech_PointLight);
    }

    ShaderProvider::~ShaderProvider()
    {
    }

    Technique * ShaderProvider::GetTechnique(eRenderTechType::enum_t type, bool skined)
    {
        d_assert (type < eRenderTechType::RTT_Max);

		if (!skined)
			return mTechs[type];
		else
			return mTechs_Skined[type];
    }
}