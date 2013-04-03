#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"
#include "MWOcean.h"
#include "MWWater.h"

namespace Myway {

	class MRENDER_ENTRY WaterManager
	{
		DECLARE_SINGLETON(WaterManager);

	public:
		WaterManager();
		~WaterManager();
			
		bool IsUnderWater();

		void Render(Texture * depthTex, Texture * colorTex);

		void RenderUnderFog(Texture * depthTex, Texture * colorTex);

		void RenderUnderNoise(Texture * colorTex);
		void RenderUnderGodRay();
		void RenderUnderCaustics(Texture * depthTex, Texture * colorTex);
		void RenderUnderBubble();

		Ocean * CreateOcean();
		Ocean * GetOcean() { return mOcean; }

		Water * GetWater() { return mWater; }

		void Resize(int w, int h);

	protected:
		Ocean * mOcean;
		Water * mWater;

		UnderWaterFog * mFog;
		UnderWaterNoise * mNoise;
		UnderWaterGodRay * mGodRay;
		UnderWaterCaustics * mCaustics;
		UnderWaterBubble * mBubble;
	};

}