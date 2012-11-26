#include "MWEvKeyFrame.h"

namespace Myway
{

#define DEFAULT_SKY_TEXTURE "Sky.png"
#define DEFAULT_MOON_TEXTURE "Moon.dds"

    DF_PROPERTY_BEGIN(EvKeyFrame)
        DF_PROPERTY(EvKeyFrame, SkyLum, "Sky", "SkyLum",       PT_Float,   4)

        DF_PROPERTY(EvKeyFrame, SunColor, "Sky", "SunColor",     PT_Color,   16)
        DF_PROPERTY(EvKeyFrame, SunLum, "Sky", "SunLum",       PT_Float,   4)
        DF_PROPERTY(EvKeyFrame, SunPower, "Sky", "SunPower",     PT_Float,   4)
        DF_PROPERTY(EvKeyFrame, SunSize, "Sky", "SunSize",      PT_Float,   4)

        DF_PROPERTY(EvKeyFrame, MoonPhase, "Sky", "MoonPhase",    PT_Float,   4)
        DF_PROPERTY(EvKeyFrame, MoonLum, "Sky", "MoonLum",      PT_Float,   4)
        DF_PROPERTY(EvKeyFrame, MoonSize, "Sky", "MoonSize",     PT_Float,   4)

        DF_PROPERTY(EvKeyFrame, FogColor, "Fog", "FogColor",     PT_Color,   16)
        DF_PROPERTY(EvKeyFrame, FogStart, "Fog", "FogStart",     PT_Float,   4)
        DF_PROPERTY(EvKeyFrame, FogEnd, "Fog", "FogEnd",       PT_Float,   4)

        DF_PROPERTY(EvKeyFrame, GodRaySunLum, "GodRay", "SunLum", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, GodRaySunPower, "GodRay", "SunPower", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, GodRaySunSize, "GodRay", "SunSize", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, GodRayUVStep, "GodRay", "UVStep", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, GodRayBlendWeight, "GodRay", "Weight", PT_Float, 4)

        DF_PROPERTY(EvKeyFrame, HdrExposure, "HDR", "Exposure", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, HdrBrightThreshold, "HDR", "Threshold", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, HdrBrightWeight0, "HDR", "Weight0", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, HdrBrightWeight1, "HDR", "Weight1", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, HdrBrightWeight2, "HDR", "Weight2", PT_Float, 4)

        DF_PROPERTY(EvKeyFrame, CloudHeight, "Cloud", "Height", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudCurved, "Cloud", "Curved", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudMass, "Cloud", "Mass", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudWeight0, "Cloud", "Weight0", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudWeight1, "Cloud", "Weight1", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudUVScale0, "Cloud", "UVScale0", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudUVScale1, "Cloud", "UVScale1", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudUVScroll0, "Cloud", "UVScroll0", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudUVScroll1, "Cloud", "UVScroll1", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudAlpha, "Cloud", "Alpha", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudAlphaStrength, "Cloud", "AlphaStrength", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudLightStrength, "Cloud", "LightStrength", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudFadeStart, "Cloud", "FadeStart", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudFadeStrength, "Cloud", "FadeStrength", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudAmbient, "Cloud", "Ambient", PT_Color, 16)
        DF_PROPERTY(EvKeyFrame, CloudDiffuse, "Cloud", "Diffuse", PT_Color, 16)
        DF_PROPERTY(EvKeyFrame, CloudAmbientScale, "Cloud", "AmbientScale", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, CloudDiffuseScale, "Cloud", "DiffuseScale", PT_Float, 4)

        DF_PROPERTY(EvKeyFrame, DeepColor, "Water", "DeepColor", PT_Color, 16)
        DF_PROPERTY(EvKeyFrame, UnderWaterFogColor, "Water", "fogColor", PT_Color, 16)
        DF_PROPERTY(EvKeyFrame, UnderWaterFogStart, "Water", "fogStart", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, UnderWaterFogEnd, "Water", "fogEnd", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, UnderWaterGodRayLum, "Water", "rayLum", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, UnderWaterGodRayExp0, "Water", "rayExp0", PT_Float, 4)
        DF_PROPERTY(EvKeyFrame, UnderWaterGodRayExp1, "Water", "rayExp1", PT_Float, 4)


    DF_PROPERTY_END()

    EvKeyFrame::EvKeyFrame()
    {
        SkyLum = 1;

        SunColor = Color4(1, 0.95f, 0.76f);
        SunLum = 0.5f;
        SunPower = 3;
        SunSize = 2000;

        MoonPhase = 1.4f;
        MoonLum = 1;
        MoonSize = 1000;

        FogStart = 500;
        FogEnd = 2000;
        FogColor = Color4::White;

        GodRaySunLum = 0.5f;
        GodRaySunPower = 1;
        GodRaySunSize = 4000;
        GodRayUVStep = 48;
        GodRayBlendWeight = 0.5f;

        HdrExposure = 2;
        HdrBrightThreshold = 0.8f;
        HdrBrightWeight0 = 0.1f;
        HdrBrightWeight1 = 0.3f;
        HdrBrightWeight2 = 1.0f;

        CloudHeight = 0.15f;
        CloudCurved = 0.4f;
        CloudMass = 0.4f;
        CloudWeight0 = 0.7f;
        CloudWeight1 = 0.3f;
        CloudUVScale0 = 1.5f;
        CloudUVScale1 = 2.5f;
        CloudUVScroll0 = 0.005f;
        CloudUVScroll1 = 0.015f;
        CloudAlpha = 0.25f;
        CloudAlphaStrength = 15;
        CloudLightStrength = 9.0f;
        CloudFadeStart = 0.7f;
        CloudFadeStrength = 1;
        CloudAmbient = Color4(0.4f, 0.66f, 0.84f, 1);
        CloudDiffuse = Color4(0.78f, 0.88f, 1, 1);
        CloudAmbientScale = 0.7f;
        CloudDiffuseScale = 1.5f;

        DeepColor = Color4(18.0f / 255.0f, 22.0f / 255.0f, 36.0f / 255.0f, 1);
        UnderWaterFogColor = Color4(29.0f / 255.0f, 57.0f / 255.0f, 68.0f / 255.0f, 1);
        UnderWaterFogStart = 20;
        UnderWaterFogEnd = 120;
        UnderWaterGodRayLum = 0.005f;
        UnderWaterGodRayExp0 = 0.50f;
        UnderWaterGodRayExp1 = 2.49f;
    }




    EvCloudParam::EvCloudParam()
    {
        height = 0.3f;
        curved = 0.2f;
        mass = 0.5f;
        weight0 = 0.5f;
        weight1 = 0.5f;
        uvScale0 = 1;
        uvScale1 = 1;
        uvScroll0 = 0.2f;
        uvScroll1 = 0.2f;
        alpha = 0.2f;
        alphaStrength = 7;
        lightStrength = 3.5f;
        fadeStart = 0.7f;
        fadeStrength = 1;
        ambient = Color4(0.3f, 0.3f, 0.4f, 1);
        diffuse = Color4(1, 1, 1, 1);
        ambientScale = 1;
        diffuseScale = 1;
    }

    EvWaterParam::EvWaterParam()
    {
        deepColor = Color4(18.0f / 255.0f, 22.0f / 255.0f, 36.0f / 255.0f, 1);
        fogColor = Color4(29.0f / 255.0f, 57.0f / 255.0f, 68.0f / 255.0f, 1);
        fogStart = 20;
        fogEnd = 120;
        godrayLum = 0.005f;
        godrayExp0 = 0.50f;
        godrayExp1 = 2.49f;
    }

    EvHdrParam::EvHdrParam()
    {
        exposure = 2;
        threshold = 0.8f;
        weight0 = 0.1f;
        weight1 = 0.3f;
        weight2 = 1.0f; 
    }


    DF_PROPERTY_BEGIN(EvGlobalParam)
    DF_PROPERTY_END()

    EvGlobalParam::EvGlobalParam()
    {
        SkyVOffset = 0;
        SkyTexture = DEFAULT_SKY_TEXTURE;

        SunPicth = 0;
        SunYaw = 0;

        MoonTexture = DEFAULT_MOON_TEXTURE;
    }



    EvParam::EvParam()
    {
        SunDir = MoonDir = LightDir = Vec3(0, -1, 0);

        SkyU = 0.5f;

        SunColor = Color4(1, 0.95f, 0.76f);
        SunLum = 0.5f;
        SunPower = 3;
        SunSize = 2000;

        MoonPase = 1.4f;
        MoonLum = 1;
        MoonSize = 1000;

        FogStart = 100;
        FogEnd = 300;
        FogColor = Color4::White;

        GodRaySunLum = 0.5f;
        GodRaySunPower = 1;
        GodRaySunSize = 4000;
        GodRayUVStep = 48;
        GodRayBlendWeight = 0.6f;

        ColorSharpScale = 0.5f;
    }
}