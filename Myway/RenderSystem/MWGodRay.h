#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY GodRay
    {
        DECLARE_ALLOC();

    public:
        GodRay();
        virtual ~GodRay();

        void Render(Texture * depthTex);

    protected:
		void _resize(void * param0, void * param1);

        void _initGeometry();
        void _initTechnique();
        void _initRenderTarget();

		void _occlusion();
        void _renderSun();
        void _godray(Texture * depthTex);
        void _blur();
        void _blend();

    protected:
        RenderOp mRender_Sun;

		Technique * mTech_SunOcc;
        Technique * mTech_Sun;
        Technique * mTech_GodRay;
        Technique * mTech_BlurH;
        Technique * mTech_BlurV;
        Technique * mTech_Blend;

        RenderTargetPtr mRenderTarget;
        TexturePtr mTexture;
		float mOccLighting;
		float mOccRadius;

		tEventListener<GodRay> OnResize;
    };
}