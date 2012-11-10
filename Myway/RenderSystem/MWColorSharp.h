#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY ColorSharp
    {
        DECLARE_ALLOC();
        
    public:
        ColorSharp();
        ~ColorSharp();

        void Render(Texture * tex);

    protected:
        Technique * mTech;
    };
}