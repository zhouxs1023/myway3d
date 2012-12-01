#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY HDRLighting
    {
    public:
        HDRLighting();
        ~HDRLighting();

        void Render(Texture * sceneTex);

    protected:
		void _resize(void * param0, void * param1);

        void _initRT();
        void _initTech();
        void _deinitRT();
        void _deinitTech();

        void _downScale_clr(Texture * sceneTex);
        void _lumInitial();
        void _lumDownScale_16x();
        void _lumDownScale_4x();
        void _lumFinal();
        void _lumAdapted();
        void _bright();
        void _blurH(RenderTarget * rt, Texture * tex);
        void _blurV(RenderTarget * rt, Texture * tex);
        void _toneMap(Texture * sceneTex);

    protected:
        RenderTargetPtr mRT_Quad;
        RenderTargetPtr mRT_Bloom0;
        RenderTargetPtr mRT_Bloom1;
        RenderTargetPtr mRT_Bloom2;
        RenderTargetPtr mRT_64x64;
        RenderTargetPtr mRT_16x16;
        RenderTargetPtr mRT_4x4;
        RenderTargetPtr mRT_1x1;
        
        TexturePtr mTex_Quad;
        TexturePtr mTex_Bloom0;
        TexturePtr mTex_Bloom1;
        TexturePtr mTex_Bloom2;
        TexturePtr mTex_64x64;
        TexturePtr mTex_16x16;
        TexturePtr mTex_4x4;
        TexturePtr mTex_1x1_0;
        TexturePtr mTex_1x1_1;

        Technique * mTech_DownScale2x;
        Technique * mTech_DownScale4x;
        Technique * mTech_LumInitial;
        Technique * mTech_LumFinal;
        Technique * mTech_Adapted;
        Technique * mTech_Bright;
        Technique * mTech_BlurH;
        Technique * mTech_BlurV;
        Technique * mTech_ToneMap;

		tEventListener<HDRLighting> OnResize;
    };

}