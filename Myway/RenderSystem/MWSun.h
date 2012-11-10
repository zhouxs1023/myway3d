#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class Sun
    {
        DECLARE_ALLOC();

    public:
        Sun();
        ~Sun();

        void Render();
        void Lighting(Texture * colorTex, Texture * normalTex);

    protected:
        void _geometry();

    protected:
        RenderDesc mRender;
        Technique * mTech;
        Technique * mTech_Lighting;
    };

}