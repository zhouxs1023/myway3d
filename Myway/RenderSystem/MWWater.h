#pragma once

#include "MWRenderSystem.h"

namespace Myway {

    class UnderWaterFog;
    class UnderWaterNoise;
    class UnderWaterGodRay;
    class UnderWaterCaustics;
    class UnderWaterBubble;
    class ProjectedGrid;

    class MW_ENTRY Water : public EventListener
    {
        DECLARE_ALLOC();

    public:
        Water();
        ~Water();

        void OnCall(Event * sender, void * data);

        void Render(Texture * depthTex, Texture * colorTex);

        void RenderUnderFog(Texture * depthTex, Texture * colorTex);
        
        void RenderUnderNoise(Texture * colorTex);
        void RenderUnderGodRay();
        void RenderUnderCaustics(Texture * depthTex, Texture * colorTex);
        void RenderUnderBubble();

        void Update();
        bool IsUnderWater();

        const Vec3 & GetPosition() { return mPosition; }

    protected:
        void _renderUnderWater(Texture * depthTex, Texture * colorTex);
        void _renderUpWater(Texture * depthTex, Texture * colorTex);
        void _init();

        void _renderRelfection();

    protected:
        RenderOp mRender;

        TexturePtr mTex_Wave;
        TexturePtr mTex_Fresnel;
        TexturePtr mTex_Normal0;
        TexturePtr mTex_Normal1;

        RenderTargetPtr mRT_Refl;
        TexturePtr mTex_Refl;
        DepthStencilPtr mDepthStencil;

        Technique * mTech;
        Technique * mTech_UnderWater;

        Plane mPlane;
        Vec3 mPosition;
        Vec3 mScale;
        Camera * mCamera;
        SceneNode * mCameraNode;
        VisibleCullResult mCullResult;
        RenderQueue mRenderQueue;

        UnderWaterFog * mFog;
        UnderWaterNoise * mNoise;
        UnderWaterGodRay * mGodRay;
        UnderWaterCaustics * mCaustics;
        UnderWaterBubble * mBubble;

        ProjectedGrid * mProjGrid;
    };

}