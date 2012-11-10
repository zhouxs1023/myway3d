#pragma once

#include "MWRenderDefine.h"

namespace Myway
{
    class Technique;
    class ShaderLib;

    class MW_ENTRY ShaderProvider
    {
    public:
        ShaderProvider() {}
        virtual ~ShaderProvider() {}

        virtual Technique * GetTechnique(int type) = 0;
    };
}