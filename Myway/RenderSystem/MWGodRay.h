#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class MW_ENTRY GodRay : public EventListener
    {
        DECLARE_ALLOC();

    public:
        GodRay();
        virtual ~GodRay();

        void Render(Texture * depthTex);
        void OnCall(Event * sender, void * data);

    protected:
        void _initGeometry();
        void _initTechnique();
        void _initRenderTarget();

        void _renderSun();
        void _godray(Texture * depthTex);
        void _blur();
        void _blend();

    protected:
        RenderOp mRender_Sun;

        Technique * mTech_Sun;
        Technique * mTech_GodRay;
        Technique * mTech_BlurH;
        Technique * mTech_BlurV;
        Technique * mTech_Blend;

        RenderTargetPtr mRenderTarget;
        TexturePtr mTexture;
    };
}