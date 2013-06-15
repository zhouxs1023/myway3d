#pragma once

#include "MRenderEntry.h"
#include "MWRenderScheme.h"
#include "MWRenderTarget.h"
#include "MWDepthStencil.h"

namespace Myway
{
    class MRENDER_ENTRY RenderLoop
    {
		friend class DeferredRenderer;

		DECLARE_ALLOC();

    public:
        RenderLoop(DeferredRenderer * sch);
        ~RenderLoop();

        void DoRender();

    protected:
		void _resize(int w, int h);

        void _updateTexture();
        void _updateColorTexture();
		void _LDR_To_HDR();

        void _clear();
        void _frush(RenderTarget * finalRT);

        void _renderSolidObjects(bool deffered);
		void _randerTransObjects();

		void _doLighting();

    protected:
        DeferredRenderer * mScheme;

		RenderTargetPtr mRT_Color;
        RenderTargetPtr mRT_LDRColor;
		RenderTargetPtr mRT_HDRColor;
        RenderTargetPtr mRT_Normal;
        RenderTargetPtr mRT_Material;
        RenderTargetPtr mRT_Depth;

        DepthStencilPtr mDepthStencil;

		TexturePtr mTex_Color;
        TexturePtr mTex_LDRColor;
		TexturePtr mTex_HDRColor;
        TexturePtr mTex_Normal;
        TexturePtr mTex_Material;
        TexturePtr mTex_Depth;
    };

	
}
