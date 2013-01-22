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
        void Lighting(Texture * colorTex, Texture * specTex, Texture * normalTex, Texture * depthTex);

    protected:
        void _geometry();

    protected:
        RenderOp mRender;
        Technique * mTech;
        Technique * mTech_Lighting;
    };

}