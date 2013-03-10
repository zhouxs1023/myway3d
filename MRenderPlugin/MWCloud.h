#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

    class MRENDER_ENTRY Cloud
    {
    public:
        Cloud();
        ~Cloud();

        void Render(bool lighting = true);

		void Resize(int w, int h);

	protected:
        void _initGeometry();
        void _initTechnique();
        void _initRenderTarget();
        void _initLayer();

		void _cloud();
        void _lighting();
        void _blur();
        void _shading(bool lighting);

    protected:
        RenderOp mRender;

		Technique * mTech_Cloud;
        Technique * mTech_Lighting;
        Technique * mTech_BlurH;
        Technique * mTech_BlurV;
        Technique * mTech_Shading;

		RenderTargetPtr mRT_Cloud;
		TexturePtr mTex_Cloud;

        RenderTargetPtr mRT_Lighting;
        TexturePtr mTex_Lighting;

        TexturePtr mTex_Layer0;
        TexturePtr mTex_Layer1;

		Mat4 mMatProj;
		float mFarClip;
    };
}