#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class Moon
    {
        DECLARE_ALLOC();

    public:
        Moon();
        ~Moon();

        void Render();

    protected:
        void _geometry();

        void _setTexture(const char * tex);

    protected:
        TexturePtr mTexture;
        RenderDesc mRender;
        Technique * mTech;
    };

}