#pragma once

#include "MWShaderFX.h"

namespace Myway
{

    class MW_ENTRY ShaderProvider_Main
    {
		DECLARE_ALLOC();

    public:
        enum
        {
            R_Base,
            R_Mirror,
            R_Shadow,
            R_MAX
        };

    public:
        ShaderProvider_Main();
        ~ShaderProvider_Main();

        Technique * GetTechnique(int type, bool skined);

        Technique * GetFrushTech() { return mFrushTech; }
        Technique * GetClearTech() { return mClearTech; }

    protected:
        ShaderLib * mShaderLib;
		Technique * mTechs[R_MAX];
        Technique * mTechs_Skined[R_MAX];

        Technique * mFrushTech;
        Technique * mClearTech;
    };

}
