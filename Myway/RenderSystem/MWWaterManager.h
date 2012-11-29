#pragma once

#include "MWRenderSystem.h"

#include "MWOcean.h"

namespace Myway {

	class MW_ENTRY WaterManager
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

	protected:
		Ocean * mOcean;

		UnderWaterFog * mFog;
		UnderWaterNoise * mNoise;
		UnderWaterGodRay * mGodRay;
		UnderWaterCaustics * mCaustics;
		UnderWaterBubble * mBubble;
	};

}