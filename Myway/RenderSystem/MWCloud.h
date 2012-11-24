#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY Cloud : public EventListener
    {
    public:
        Cloud();
        ~Cloud();

        void Render(bool lighting = true);

    protected:
        void OnCall(Event * sender, void * data);

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
    };
}