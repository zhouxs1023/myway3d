#include "MWShaderProvider_Prz.h"
#include "MWShaderFX.h"

namespace Myway
{
    ShaderProvider_Main::ShaderProvider_Main()
    {
        mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("main", "PZ_Main.ShaderLib", "core");
        d_assert (mShaderLib);

        mTechs[R_Base] = mShaderLib->GetTechnique("base");
        mTechs[R_Mirror] = mShaderLib->GetTechnique("mirror");
        mTechs[R_Shadow] = mShaderLib->GetTechnique("shadow");

        mFrushTech = mShaderLib->GetTechnique("Frush");
        mClearTech = mShaderLib->GetTechnique("Clear");

        d_assert (mFrushTech);
        d_assert (mClearTech);
    }

    ShaderProvider_Main::~ShaderProvider_Main()
    {
    }

    Technique * ShaderProvider_Main::GetTechnique(int type)
    {
        d_assert (type < R_MAX);
        return mTechs[type];
    }
}