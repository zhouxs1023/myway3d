#pragma once

#pragma once

#include "MWRenderScheme.h"
#include "MWRenderTarget.h"
#include "MWDepthStencil.h"

namespace Myway
{
    class MW_ENTRY RenderLoop
    {
		friend class RS_Scheme;

    public:
        RenderLoop(RS_Scheme * sch);
        ~RenderLoop();

        void DoRender();

    protected:
        void _updateTexture();
        void _updateColorTexture();

        void _clear();
        void _frush(RenderTarget * finalRT);

        void _renderSolidObjects(bool deffered);
		void _randerTransObjects();

		void _doLighting();

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
