#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

    class MRENDER_ENTRY Fog
    {
        DECLARE_ALLOC();

    public:
        Fog();
        ~Fog();

        void Render(Texture * depthTex);

    protected:
        Technique * mTech;
    };

}