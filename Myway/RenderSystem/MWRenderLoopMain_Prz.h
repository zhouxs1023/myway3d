#pragma once

#pragma once

#include "MWRenderScheme.h"
#include "MWRenderTarget.h"
#include "MWDepthStencil.h"

namespace Myway
{
    class MW_ENTRY RenderLoop_Main
    {
		friend class RS_Scheme;

    public:
        RenderLoop_Main(RS_Scheme * sch);
        ~RenderLoop_Main();

        void DoRender();

    protected:
        void _updateTexture();
        void _updateColorTexture();

        void _clear();
        void _frush();

        void _renderSolidObjects();

    protected:
        RS_Scheme * mScheme;

        RenderTargetPtr mRT_Color;
        RenderTargetPtr mRT_Normal;
        RenderTargetPtr mRT_Material;
        RenderTargetPtr mRT_Depth;

        DepthStencilPtr mDepthStencil;

        TexturePtr mTex_Color;
        TexturePtr mTex_Normal;
        TexturePtr mTex_Material;
        TexturePtr mTex_Depth;
    };

}
