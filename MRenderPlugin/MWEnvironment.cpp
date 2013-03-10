#include "MWEnvironment.h"
#include "Engine.h"

namespace Myway {

    IMP_SLN(Environment);

    Environment::Environment()
		: OnUpdate(RenderEvent::OnPostUpdateScene, this, &Environment::_update)
    {
        INIT_SLN;

        mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("Core_Environment", "Shaders\\Environment.ShaderLib");
        d_assert (mShaderLib);

        mTerrain = NULL;

		mSky = NULL;
        mSky2 = NULL;
        mSun = NULL;
        mMoon = NULL;
        mCloud = NULL;
        mWaterManager = NULL;

        mFog = NULL;
        mGodRay = NULL;
        mHDR = NULL;
		mColorSharp = NULL;
		mSoftLeaf = NULL;

        mMode = EVPM_KeyFrame;
        mCurKey = EVKT_Morning;
        mCurTime = 8;
    }

    Environment::~Environment()
    {
        DeInitEv();
        UnloadTerrain();

        SHUT_SLN;
    }

	void Environment::_resize(int w, int h)
	{
	}

    void Environment::InitEv()
    {
        DeInitEv();

		mSky2 = new Sky2();
        mSky = new Sky();
        mSun = new Sun();
        mMoon = new Moon();
		mStarfield = new Starfield();
        mCloud = new Cloud();
        mWaterManager = new WaterManager();

        mFog = new Fog();
	}

    void Environment::DeInitEv()
    {
		safe_delete (mSky);
        safe_delete (mSky2);
        safe_delete (mSun);
        safe_delete (mMoon);
        safe_delete (mCloud);
		safe_delete (mStarfield);
        safe_delete (mWaterManager);

        safe_delete (mFog);
        safe_delete (mGodRay);
		safe_delete (mHDR);
        safe_delete (mSSAO);
		safe_delete (mShadow);
		safe_delete (mColorSharp);
		safe_delete (mSoftLeaf);
    }

	void Environment::SetSSAOEnable(bool b)
	{
		if (!b && mSSAO)
		{
			safe_delete(mSSAO);
		}
		else if (b && !mSSAO)
		{
			mSSAO = new SSAO();
		}
	}

	void Environment::SetHDREnable(bool b)
	{
		if (!b && mHDR)
		{
			safe_delete(mHDR);
		}
		else if (b && !mHDR)
		{
			mHDR = new HDRLighting();
		}
	}

	void Environment::SetColorSharpEnable(bool b)
	{
		if (!b && mColorSharp)
		{
			safe_delete(mColorSharp);
		}
		else if (b && !mColorSharp)
		{
			mColorSharp = new ColorSharp();
		}
	}

	void Environment::SetShadowEnable(bool b)
	{
		if (!b && mShadow)
		{
			safe_delete(mShadow);
		}
		else if (b && !mShadow)
		{
			mShadow = new Shadow();
		}
	}

	void Environment::SetGodRayEnable(bool b)
	{
		if (!b && mGodRay)
		{
			safe_delete(mGodRay);
		}
		else if (b && !mGodRay)
		{
			mGodRay = new GodRay();
		}
	}

	void Environment::SetSoftLeafEnable(bool b)
	{
		if (!b && mSoftLeaf)
		{
			safe_delete(mSoftLeaf);
		}
		else if (b && !mSoftLeaf)
		{
			mSoftLeaf = new SoftLeaf();
		}
	}

	void Environment::CreateTerrain(const Terrain::Config & config)
	{
		UnloadTerrain();

		mTerrain = new Terrain(config);
	}

	void Environment::LoadTerrain(const char * source)
	{
		UnloadTerrain();

		mTerrain = new Terrain(source);
	}

    void Environment::UnloadTerrain()
    {
        safe_delete (mTerrain);
    }

    float Environment::GetKeyTime(EvKeyType type)
    {
        switch (mCurKey)
        {
        case EVKT_Night:
            return 0;

        case EVKT_Morning:
            return 7;

        case EVKT_Noon:
            return 12;

        case EVKT_Evening:
           return 17;

        default:
            d_assert (0);
            return 0;
        }
    }


    float Environment::_getSkyU(float time)
    {
        const float mu[] = {0, 0.25f, 0.5f, 0.75f, 1};

        if (time < 7)
            return 0.25f * time / (7 - 0);
        else if (time < 17)
            return 0.25f + 0.5f * (time - 7) / (17 - 7);
        else if (time < 24)
            return 0.75f + 0.25f * (time - 17) / (24 - 17);

        d_assert (0);

        return 0;
    }

    float Environment::_getSunRoll(float time)
    {
        float k = time / 24;

        float degree = -90 + k * 360;

        if (degree < 0)
            degree += 360;

        return degree;
    }

    float Environment::_getMoonRoll(float time)
    {
        float k = 1 - time / 24;

        float degree = 90 + k * 360;

        if (degree >= 360)
            degree -= 360;

        return degree;
    }

	float Environment::_getLightRoll(float time)
	{
		float roll = 0;

		if (time >= 6 && time <= 18)
			roll = _getSunRoll(time);
		else
			roll = _getMoonRoll(time);

		roll = Math::Maximum(roll, 30.0f);
		roll = Math::Minimum(roll, 150.f);

		return roll;
	}

    void Environment::_update(Event * sender)
    {
        _updateTime();

        int k0, k1;
        float d;

        if (mCurTime > 5 && mCurTime <= 7)
        {
            k0 = EVKT_Night;
            k1 = EVKT_Morning;
            d = (mCurTime - 5) / (7 - 5);
        }
        else if (mCurTime > 7 && mCurTime <= 12)
        {
            k0 = EVKT_Morning;
            k1 = EVKT_Noon;
            d = (mCurTime - 7) / (12 - 7);
        }
        else if (mCurTime > 12 && mCurTime <= 17)
        {
            k0 = EVKT_Noon;
            k1 = EVKT_Evening;
            d = (mCurTime - 12) / (17 - 12);
        }
        else
        {
            k0 = k1 = EVKT_Night;
            d = 0;
        }

        d_assert (d <= 1.00001f);

        _updateKey(k0, k1, d);
    }

    void Environment::_updateTime()
    {
        if (mMode == EVPM_KeyFrame)
        {
            mCurTime = GetKeyTime(mCurKey);
        }
        else if (mMode == EVPM_Auto)
        {
            mCurTime += Engine::Instance()->GetFrameTime();
        }

        while (mCurTime >= 24)
            mCurTime -= 24;
    }

    void Environment::_updateKey(int k0, int k1, float d)
    {
        EvParam p;

        const EvKeyFrame & kf0 = mKeyFrames[k0];
        const EvKeyFrame & kf1 = mKeyFrames[k1];

        float sunRoll = _getSunRoll(mCurTime);
        float moonRoll = _getMoonRoll(mCurTime);
		float lightRoll = _getLightRoll(mCurTime);
        float lightYaw = 0;
        float lightPicth = mGlobalParam.SunPicth;

		if (lightPicth >= 0)
			lightPicth = Math::Clamp(lightPicth, 1.0f, 89.0f);
		else
			lightPicth = Math::Clamp(lightPicth, -89.0f, -1.0f);

		p.SkyU = _getSkyU(mCurTime);

        p.SunDir = _makeSunDir(lightYaw, lightPicth, sunRoll);
        p.MoonDir = _makeMoonDir(lightYaw, lightPicth, moonRoll);
		p.LightDir = _makeLightDir(p.SunDir, p.MoonDir);

        p.SunColor = Math::Lerp(kf0.SunColor, kf1.SunColor, d);
        p.SunLum = Math::Lerp(kf0.SunLum, kf1.SunLum, d);
        p.SunPower = Math::Lerp(kf0.SunPower, kf1.SunPower, d);
        p.SunSize = Math::Lerp(kf0.SunSize, kf1.SunSize, d);

		p.LightAmbient = Math::Lerp(kf0.LightAmbient, kf1.LightAmbient, d);
		p.LightDiffuse = Math::Lerp(kf0.LightDiffuse, kf1.LightDiffuse, d);
		p.LightSpecular = Math::Lerp(kf0.LightSpecular, kf1.LightSpecular, d);

        p.MoonPase = Math::Lerp(kf0.MoonPhase, kf1.MoonPhase, d);
        p.MoonLum = Math::Lerp(kf0.MoonLum, kf1.MoonLum, d);
        p.MoonSize = Math::Lerp(kf0.MoonSize, kf1.MoonSize, d);

		p.StarLum = Math::Lerp(kf0.StarLum, kf1.StarLum, d);

        p.FogStart = Math::Lerp(kf0.FogStart, kf1.FogStart, d);
        p.FogEnd = Math::Lerp(kf0.FogEnd, kf1.FogEnd, d);
        p.FogColor = Math::Lerp(kf0.FogColor, kf1.FogColor, d);

		p.GodRaySunLum = Math::Lerp(kf0.GodRaySunLum, kf1.GodRaySunLum, d);
        p.GodRaySunInner = Math::Lerp(kf0.GodRaySunInner, kf1.GodRaySunInner, d);
        p.GodRaySunPower = Math::Lerp(kf0.GodRaySunPower, kf1.GodRaySunPower, d);
        p.GodRaySunSize = Math::Lerp(kf0.GodRaySunSize, kf1.GodRaySunSize, d);
        p.GodRayUVStep = Math::Lerp(kf0.GodRayUVStep, kf1.GodRayUVStep, d);
        p.GodRayBlendWeight = Math::Lerp(kf0.GodRayBlendWeight, kf1.GodRayBlendWeight, d);

        p.HdrParam.exposure = Math::Lerp(kf0.HdrExposure, kf1.HdrExposure, d);
        p.HdrParam.threshold = Math::Lerp(kf0.HdrBrightThreshold, kf1.HdrBrightThreshold, d);
        p.HdrParam.weight0 = Math::Lerp(kf0.HdrBrightWeight0, kf1.HdrBrightWeight0, d);
        p.HdrParam.weight1 = Math::Lerp(kf0.HdrBrightWeight1, kf1.HdrBrightWeight1, d);
        p.HdrParam.weight2 = Math::Lerp(kf0.HdrBrightWeight2, kf1.HdrBrightWeight2, d);

        p.CloudParam.height = Math::Lerp(kf0.CloudHeight, kf1.CloudHeight, d);
        p.CloudParam.curved = Math::Lerp(kf0.CloudCurved, kf1.CloudCurved, d);
        p.CloudParam.mass = Math::Lerp(kf0.CloudMass, kf1.CloudMass, d);
        p.CloudParam.weight0 = Math::Lerp(kf0.CloudWeight0, kf1.CloudWeight0, d);
        p.CloudParam.weight1 = Math::Lerp(kf0.CloudWeight1, kf1.CloudWeight1, d);
        p.CloudParam.uvScale0 = Math::Lerp(kf0.CloudUVScale0, kf1.CloudUVScale0, d);
        p.CloudParam.uvScale1 = Math::Lerp(kf0.CloudUVScale1, kf1.CloudUVScale1, d);
        p.CloudParam.uvScroll0 = Math::Lerp(kf0.CloudUVScroll0, kf1.CloudUVScroll0, d);
        p.CloudParam.uvScroll1 = Math::Lerp(kf0.CloudUVScroll1, kf1.CloudUVScroll1, d);
        p.CloudParam.alpha = Math::Lerp(kf0.CloudAlpha, kf1.CloudAlpha, d);
        p.CloudParam.alphaStrength = Math::Lerp(kf0.CloudAlphaStrength, kf1.CloudAlphaStrength, d);
        p.CloudParam.lightStrength = Math::Lerp(kf0.CloudLightStrength, kf1.CloudLightStrength, d);
        p.CloudParam.fadeStart = Math::Lerp(kf0.CloudFadeStart, kf1.CloudFadeStart, d);
        p.CloudParam.fadeStrength = Math::Lerp(kf0.CloudFadeStrength, kf1.CloudFadeStrength, d);
        p.CloudParam.ambient = Math::Lerp(kf0.CloudAmbient, kf1.CloudAmbient, d);
        p.CloudParam.diffuse = Math::Lerp(kf0.CloudDiffuse, kf1.CloudDiffuse, d);
        p.CloudParam.ambientScale = Math::Lerp(kf0.CloudAmbientScale, kf1.CloudAmbientScale, d);
        p.CloudParam.diffuseScale = Math::Lerp(kf0.CloudDiffuseScale, kf1.CloudDiffuseScale, d);
        
        p.WaterParam.deepColor = Math::Lerp(kf0.DeepColor, kf1.DeepColor, d);
        p.WaterParam.fogColor = Math::Lerp(kf0.UnderWaterFogColor, kf1.UnderWaterFogColor, d);
        p.WaterParam.fogStart = Math::Lerp(kf0.UnderWaterFogStart, kf1.UnderWaterFogStart, d);
        p.WaterParam.fogEnd = Math::Lerp(kf0.UnderWaterFogEnd, kf1.UnderWaterFogEnd, d);
        p.WaterParam.godrayLum = Math::Lerp(kf0.UnderWaterGodRayLum, kf1.UnderWaterGodRayLum, d);
        p.WaterParam.godrayExp0 = Math::Lerp(kf0.UnderWaterGodRayExp0, kf1.UnderWaterGodRayExp0, d);
        p.WaterParam.godrayExp1 = Math::Lerp(kf0.UnderWaterGodRayExp1, kf1.UnderWaterGodRayExp1, d);

        Camera * cam = World::Instance()->MainCamera();
        float farclip = cam->GetFarClip() * 0.9f;

        p.SunPos = cam->GetPosition() - farclip * p.SunDir;
        p.MoonPos = cam->GetPosition() - farclip * p.MoonDir;

        mParam = p;
    }

    Vec3 Environment::_makeSunDir(float y, float p, float r)
    {
        Mat4 matY, matP, matR;

        y = Math::DegreeToRadian(y);
        p = Math::DegreeToRadian(p);
        r = Math::DegreeToRadian(r);

        matY.MakeRotationY(y);
        matP.MakeRotationX(p);
        matR.MakeRotationZ(r);

        matR *= matY;
        matR *= matP;

        Vec3 dir = Vec3(-1, 0, 0);

        return dir * matR;
    }

    Vec3 Environment::_makeMoonDir(float y, float p, float r)
    {
        Mat4 matY, matP, matR;

        y = Math::DegreeToRadian(y);
        p = Math::DegreeToRadian(p);
        r = Math::DegreeToRadian(r);

        matY.MakeRotationY(y);
        matP.MakeRotationX(p);
        matR.MakeRotationZ(r);

        matR *= matY;
        matR *= matP;

        Vec3 dir = Vec3(-1, 0, 0);

        return dir * matR;
    }

	Vec3 Environment::_makeLightDir(const Vec3 & sunDir, const Vec3 & moonDir)
	{
		Vec3 dir;

		if (sunDir.y <= 0 )
			dir = sunDir;

		if (moonDir.y <= 0)
			dir = moonDir;

		d_assert(fabs(dir.y) < 1.0f && "not support!");

		Vec3 xzDir = Vec3(-dir.x, 0, -dir.z);

		xzDir.NormalizeL();

		if ( xzDir.Dot(-dir) > Math::Cos(Math::PI_1_6) )
		{
			Vec3 right = Vec3::UnitY.CrossN(dir);

			Quat q;

			q.FromAxis(right, Math::PI_1_6);

			xzDir = q.Rotate(xzDir);

			dir = -xzDir;
		}

		return dir;
	}

}