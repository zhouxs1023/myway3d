#pragma once

#include "MWShaderProvider.h"

namespace Myway
{

    class MW_ENTRY ShaderProvider_Main : public ShaderProvider
    {
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
        virtual ~ShaderProvider_Main();

        virtual Technique * GetTechnique(int type);

        Technique * GetFrushTech() { return mFrushTech; }
        Technique * GetClearTech() { return mClearTech; }

    protected:
        ShaderLib * mShaderLib;
        Technique * mTechs[R_MAX];

        Technique * mFrushTech;
        Technique * mClearTech;
    };

}
