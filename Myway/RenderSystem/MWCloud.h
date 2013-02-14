#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY Cloud
    {
    public:
        Cloud();
        ~Cloud();

        void Render(bool lighting = true);

    protected:
		void _resize(void * param0, void * param1);

        void _initGeometry();
        void _initTechnique();
        void _initRenderTarget();
        void _initLayer();

        void _lighting();
        void _blur();
        void _shading(bool lighting);

    protected:
        RenderOp mRender;

        Technique * mTech_Lighting;
        Technique * mTech_BlurH;
        Technique * mTech_BlurV;
        Technique * mTech_Shading;

        RenderTargetPtr mRenderTarget;
        TexturePtr mTexture;
        TexturePtr mTex_Layer0;
        TexturePtr mTex_Layer1;

		Mat4 mMatProj;
		float mFarClip;

		tEventListener<Cloud> OnResize;
    };
}