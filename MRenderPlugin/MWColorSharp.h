#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

    class MRENDER_ENTRY ColorSharp
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