#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

#include "MWSky.h"
#include "MWSun.h"
#include "MWMoon.h"
#include "MWCloud.h"
#include "MWStarfield.h"
#include "MWWaterManager.h"
#include "MWTerrain.h"

#include "MWFog.h"
#include "MWGodRay.h"
#include "MWHDR.h"
#include "MWSSAO.h"
#include "MWShadow.h"
#include "MWColorSharp.h"
#include "MWSoftLeaf.h"

#include "MWEvKeyFrame.h"

namespace Myway {

    class Terrain;
    
    class MRENDER_ENTRY Environment
    {
        DECLARE_ALLOC();
        DECLARE_SINGLETON(Environment);		

    public:
        Environment();
        ~Environment();

		void _resize(int w, int h);

        void InitEv();
        void DeInitEv();
		
		Terrain * CreateTerrain(const Terrain::Config & config);
        void LoadTerrain(const char * source);
        void UnloadTerrain();

        ShaderLib * GetShaderLib() { return mShaderLib; }

        Terrain * GetTerrain() { return mTerrain; }
		Sky * GetSky() { return mSky; }
        Sky2 * GetSky2() { return mSky2; }
        Sun * GetSun() { return mSun; }
        Moon * GetMoon() { return mMoon; }
		Starfield * GetStarfield() { return mStarfield; }
        Cloud * GetCloud() { return mCloud; }
		Shadow * GetShadow() { return mShadow; }
		WaterManager * GetWaterManager() { return mWaterManager; }

        Fog * GetFog() { return mFog; }
        GodRay * GetGodRay() { return mGodRay; }
        HDRLighting * GetHDR() { return mHDR; }
		SSAO * GetSSAO() { return mSSAO; }
		ColorSharp * GetColorSharp() { return mColorSharp; }
		SoftLeaf * GetSoftLeaf() { return mSoftLeaf; }

        EvKeyFrame * GetKeyFrame(int type) { return &mKeyFrames[type]; }
        EvGlobalParam * GetGlobalParam() { return &mGlobalParam; }

        void SetMode(EvPlayMode mode) { mMode = mode; }
        EvPlayMode GetMode() { return mMode; }

        void SetKey(EvKeyType type) { mCurKey = type; }
        EvKeyType GetKey() { return mCurKey; }
        float GetKeyTime(EvKeyType type);

        void SetTime(float time) { if (mMode == EVPM_Time) mCurTime = time; }
        float GetTime() { return mCurTime; }

        const EvParam * GetEvParam() { return &mParam; }

		void SetSSAOEnable(bool b);
		void SetHDREnable(bool b);
		void SetColorSharpEnable(bool b);
		void SetShadowEnable(bool b);
		void SetGodRayEnable(bool b);
		void SetSoftLeafEnable(bool b);

    protected:
		void _OnOptimizeCullResult(Event * _sender);

        float _getSkyU(float time);
        float _getSunRoll(float time);
        float _getMoonRoll(float time);
		float _getLightRoll(float time);

        void _update(Event * sender);
        void _updateTime();
        void _updateKey(int k0, int k1, float d);

        Vec3 _makeSunDir(float y, float p, float r);
        Vec3 _makeMoonDir(float y, float p, float r);
		Vec3 _makeLightDir(const Vec3 & sunDir, const Vec3 & moonDir);

    protected:
        ShaderLib * mShaderLib;

		Sky * mSky;
        Sky2 * mSky2;
        Sun * mSun;
        Moon * mMoon;
		Starfield * mStarfield;
        Cloud * mCloud;
		Shadow * mShadow;

        Terrain * mTerrain;

		WaterManager * mWaterManager;
        Fog * mFog;
        GodRay * mGodRay;
        HDRLighting * mHDR;
		SSAO * mSSAO;
		ColorSharp * mColorSharp;
		SoftLeaf * mSoftLeaf;

        EvKeyFrame mKeyFrames[EVKT_Max];
        EvGlobalParam mGlobalParam;

        EvPlayMode mMode;
        EvKeyType mCurKey;
        float mCurTime;

        EvParam mParam;

		tEventListener<Environment> OnUpdate;
		tEventListener<Environment> OnOptimizeCullResult;
    };
}