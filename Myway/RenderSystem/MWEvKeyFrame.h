#pragma once

#include "MWProperty.h"
#include "MWRenderSystem.h"

namespace Myway
{
    struct MW_ENTRY EvKeyFrame : public IPropertyObj
    {
        DECLARE_PROPERTY(IPropertyObj);

        float SkyLum;

        Color4 SunColor;
        float SunLum;
        float SunPower;
        float SunSize;

        float MoonPhase;
        float MoonLum;
        float MoonSize;

        float FogStart;
        float FogEnd;
        Color4 FogColor;

        float GodRaySunLum;
        float GodRaySunPower;
        float GodRaySunSize;
        float GodRayUVStep;
        float GodRayBlendWeight;

        float HdrExposure;
        float HdrBrightThreshold;
        float HdrBrightWeight0;
        float HdrBrightWeight1;
        float HdrBrightWeight2;

        float CloudHeight;
        float CloudCurved;
        float CloudMass;
        float CloudWeight0;
        float CloudWeight1;
        float CloudUVScale0;
        float CloudUVScale1;
        float CloudUVScroll0;
        float CloudUVScroll1;
        float CloudAlpha;
        float CloudAlphaStrength;
        float CloudLightStrength;
        float CloudFadeStart;
        float CloudFadeStrength;
        Color4 CloudAmbient;
        Color4 CloudDiffuse;
        float CloudAmbientScale;
        float CloudDiffuseScale;

        Color4 DeepColor;
        Color4 UnderWaterFogColor;
        float UnderWaterFogStart;
        float UnderWaterFogEnd;
        float UnderWaterGodRayLum;
        float UnderWaterGodRayExp0;
        float UnderWaterGodRayExp1;

        EvKeyFrame();
    };

    struct EvGlobalParam : public IPropertyObj
    {
        DECLARE_PROPERTY(IPropertyObj);

        float SkyVOffset;
        TString128 SkyTexture;

        float SunPicth;
        float SunYaw;

        TString128 MoonTexture;

        EvGlobalParam();
    };


    struct EvHdrParam
    {
        EvHdrParam();

        float exposure;
        float threshold;
        float weight0;
        float weight1;
        float weight2;
    };

    struct EvCloudParam
    {
        EvCloudParam();

        float height;
        float curved;
        float mass;
        float weight0;
        float weight1;
        float uvScale0;
        float uvScale1;
        float uvScroll0;
        float uvScroll1;
        float alpha;
        float alphaStrength;
        float lightStrength;
        float fadeStart;
        float fadeStrength;
        Color4 ambient;
        Color4 diffuse;
        float ambientScale;
        float diffuseScale;
    };

    struct EvWaterParam
    {
        EvWaterParam();

        Color4 deepColor;
        Color4 fogColor;
        float fogStart;
        float fogEnd;
        float godrayLum;
        float godrayExp0;
        float godrayExp1;
    };

    struct EvParam
    {
        EvParam();

        float SkyU;

        Vec3 SunPos;
        Vec3 MoonPos;
        Vec3 SunDir;
        Vec3 MoonDir;
        Vec3 LightDir;

        Color4 SunColor;
        float SunLum;
        float SunPower;
        float SunSize;

        float MoonPase;
        float MoonLum;
        float MoonSize;

        float FogStart;
        float FogEnd;
        Color4 FogColor;

        float GodRaySunLum;
        float GodRaySunPower;
        float GodRaySunSize;
        float GodRayUVStep;
        float GodRayBlendWeight;

        float ColorSharpScale;

        EvCloudParam CloudParam;

        EvWaterParam WaterParam;

        EvHdrParam HdrParam;
    };


    enum EvKeyType
    {
        EVKT_Night,
        EVKT_Morning,
        EVKT_Noon,
        EVKT_Evening,

        EVKT_Max,
    };

    enum EvPlayMode
    {
        EVPM_KeyFrame,
        EVPM_Time,
        EVPM_Auto,
    };
}