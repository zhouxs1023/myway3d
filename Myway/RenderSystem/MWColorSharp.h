#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY ColorSharp
    {
        DECLARE_ALLOC();
        
    public:
        ColorSharp();
        ~ColorSharp();

        void Render(Texture * texColor, Texture * texDepth);

    protected:
        Technique * mTech;
    };
}