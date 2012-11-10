#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY Fog
    {
        DECLARE_ALLOC();

    public:
        Fog();
        ~Fog();

        void Render(Texture * depthTex, Texture * sceneTex);

    protected:
        Technique * mTech;
    };

}