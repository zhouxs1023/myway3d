#pragma once

#include "MWRenderSystem.h"

#include "MWSky.h"
#include "MWSun.h"
#include "MWMoon.h"
#include "MWCloud.h"
#include "MWWaterManager.h"
#include "MWTerrain.h"

#include "MWFog.h"
#include "MWGodRay.h"
#include "MWHDR.h"
#include "MWSSAO.h"
#include "MWShadow.h"
#include "MWColorSharp.h"

#include "MWEvKeyFrame.h"

namespace Myway {

    class Terrain;
    
    class MW_ENTRY Environment
    {
        DECLARE_ALLOC();
        DECLARE_SINGLETON(Environment);

    public:
        Environment();
        ~Environment();

        void InitEv();
        void DeInitEv();
		
		void CreateTerrain(const Terrain::Config & config);
        void LoadTerrain(const char * source);
        void UnloadTerrain();

        ShaderLib * GetShaderLib() { return mShaderLib; }

        Terrain * GetTerrain() { return mTerrain; }
		Sky * GetSky() { return mSky; }
        Sky2 * GetSky2() { return mSky2; }
        Sun * GetSun() { return mSun; }
        Moon * GetMoon() { return mMoon; }
        Cloud * GetCloud() { return mCloud; }
		Shadow * GetShadow() { return mShadow; }
		WaterManager * GetWaterManager() { return mWaterManager; }

        Fog * GetFog() { return mFog; }
        GodRay * GetGodRay() { return mGodRay; }
        HDRLighting * GetHDR() { return mHDR; }
		SSAO * GetSSAO() { return mSSAO; }
		ColorSharp * GetColorSharp() { return mColorSharp; }

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

    protected:
        void OnCall(Event * sender, void * data);

        float _getSkyU(float time);
        float _getSunRoll(float time);
        float _getMoonRoll(float time);
		float _getLightRoll(float time);

        void _update(void * param0, void * param1);
        void _updateTime();
        void _updateKey(int k0, int k1, float d);

        Vec3 _makeSunDir(float y, float p, float r);
        Vec3 _makeMoonDir(float y, float p, float r);

    protected:
        ShaderLib * mShaderLib;

		Sky * mSky;
        Sky2 * mSky2;
        Sun * mSun;
        Moon * mMoon;
        Cloud * mCloud;
		Shadow * mShadow;

        Terrain * mTerrain;

		WaterManager * mWaterManager;
        Fog * mFog;
        GodRay * mGodRay;
        HDRLighting * mHDR;
		SSAO * mSSAO;
		ColorSharp * mColorSharp;

        EvKeyFrame mKeyFrames[EVKT_Max];
        EvGlobalParam mGlobalParam;

        EvPlayMode mMode;
        EvKeyType mCurKey;
        float mCurTime;

        EvParam mParam;

		tEventListener<Environment> OnUpdate;
    };
}