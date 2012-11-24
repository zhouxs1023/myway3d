#pragma once

#include "MWRenderSystem.h"
#include "MWPerlin.h"

namespace Myway {

    class UnderWaterFog
    {
        DECLARE_ALLOC();

    public:
        UnderWaterFog();
        ~UnderWaterFog();

        void Render(Texture * depthTex, Texture * sceneTex);

    protected:
        Technique * mTech;
    };

    class UnderWaterNoise
    {
        DECLARE_ALLOC();

    public:
        UnderWaterNoise();
        ~UnderWaterNoise();

        void Render(Texture * sceneTex);

    protected:
        Technique * mTech;
        TexturePtr mTexture;
    };

    class Water;

    class UnderWaterGodRay
    {
        DECLARE_ALLOC();

    public:
        UnderWaterGodRay();
        ~UnderWaterGodRay();

        void Render(Water * water);

		inline void SetSimulationSpeed(const float & Speed)
		{
			mSimulationSpeed = Speed;
		}

		inline const float & GetSimulationSpeed() const
		{
			return mSimulationSpeed;
		}

		inline void SetNumberOfRays(const int& NumberOfRays)
        {
            mNumberOfRays = NumberOfRays;

            _createGodRays();
        }

		inline const int& GetNumberOfRays() const
		{
			return mNumberOfRays;
		}

		inline void SetRaysSize(const float & Size)
		{
			mRaysSize = Size;
		}

		inline float GetRaysSize() const
		{
			return mRaysSize;
		}

		
	private:
        void _init();

        void _createGodRays();

        void _updateRays();
        void _updateProjector();

        void _renderRay();
        void _blend();

		/** Calculate the current position of a ray
		    @param RayNumber Number of the ray range[0,NumerOfRays]
			@return Ray position range[-1,1]x[-1,1]
		 */
		Vec2 _calculateRayPosition(int RayNumber);

    protected:
		Perlin * mPerlin;
        Water * mWater;

		/// Normal derivation value
		float mNoiseDerivation;
		/// PositionMultiplier value
		float mNoisePositionMultiplier;
		/// Y normal component multiplier
		float mNoiseYNormalMultiplier;
		/// Normal multiplier
		float mNoiseNormalMultiplier;

		/// God rays simulation speed
		float mSimulationSpeed;
		/// Number of rays
		int mNumberOfRays;
		/// God rays size
		float mRaysSize;

        Vec3 mExposure;
        float mIntensity;

        RenderTargetPtr mRenderTarget;
        TexturePtr mTexture;
        RenderOp mRender;
        Technique * mTech_Ray;
        Technique * mTech_Blend;

        Vec3 mProjPosition;
        Mat4 mProjMatrix;
        bool mEnable;
    };









    class UnderWaterCaustics
    {
        DECLARE_ALLOC();

        static const int nFrames = 32;
    public:
        UnderWaterCaustics();
        ~UnderWaterCaustics();

        void Render(Texture * colorTex, Texture * depthTex);

    protected:
        void _init();
        Texture * GetTexture();

    protected:
        TexturePtr mCausticsTex[nFrames];
        Technique * mTech;
        
        float mInternalTime;
    };





    class UnderWaterBubble
    {
        DECLARE_ALLOC();

        struct Bubble
        {
            Vec3 position;
            float life;
            float maxlife;
            float speed;
            float size;
            float alpha;
        };

        static const int mMaxNum = 3000;

    public:
        UnderWaterBubble();
        ~UnderWaterBubble();

        void Render();

    protected:
        void _init();
        void _deinit();

        void _update();
        void _emit();
        void _geom();

        Vec3 _randomPosition();
        float _randomLife();
        float _randomSize();
        float _randomSpeed();

    protected:
        float mRate;
        float mLastEmitTime;
        float mInternalTime;

        TexturePtr mTex_Bubble0;
        TexturePtr mTex_Bubble1;

        Technique * mTech;

        RenderOp mRender;

        List<Bubble> mBubbles; 
    };
}