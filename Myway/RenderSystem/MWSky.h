#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY Sky
    {
        DECLARE_ALLOC();

    public:
        Sky();
        ~Sky();

        void Render();

    protected:
        void _geometry();

    protected:
        RenderDesc mRender;
        Technique * mTech;
        TexturePtr mTexture;
    };

    class MW_ENTRY Sky2
    {
        DECLARE_ALLOC();

    protected:
        struct Vertex
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
            float o;
        };

    public:
        Sky2();
        ~Sky2();

    protected:
        void _init();

    protected:
        RenderDesc mRender;
    };

}