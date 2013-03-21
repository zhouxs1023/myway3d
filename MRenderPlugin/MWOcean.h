#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

	class UnderWaterFog;
	class UnderWaterNoise;
	class UnderWaterGodRay;
	class UnderWaterCaustics;
	class UnderWaterBubble;
	class ProjectedGrid;

	class MRENDER_ENTRY Ocean
	{
		DECLARE_ALLOC();

		tEventListener<Ocean> OnPreRender;

		void _PreRender(Event * sender);

	public:
		Ocean();
		~Ocean();

		void Resize(int w, int h);

		void Render(Texture * depthTex, Texture * colorTex);

		void SetHeight(float h) { mPosition.y = h; }
		const Vec3 & GetPosition() { return mPosition; }

	protected:
		void _init();
		void _initRT();

		void _renderUnderWater(Texture * depthTex, Texture * colorTex);
		void _renderUpWater(Texture * depthTex, Texture * colorTex);

		void _renderRelfection();

	protected:
		TexturePtr mTex_Wave;
		TexturePtr mTex_Fresnel;
		TexturePtr mTex_Normal0;
		TexturePtr mTex_Normal1;

		RenderTargetPtr mRT_Refl;
		TexturePtr mTex_Refl;
		DepthStencilPtr mDepthStencil;

		Technique * mTech;
		Technique * mTech_UnderWater;

		Vec3 mPosition;

		ProjectedGrid * mProjGrid;
	};

}