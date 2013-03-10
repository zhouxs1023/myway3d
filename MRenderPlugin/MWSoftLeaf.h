#pragma once

#include "MRenderEntry.h"
#include "MWRenderScheme.h"
#include "MWRenderSystem.h"

namespace Myway {

	class SoftLeaf
	{
	public:
		SoftLeaf();
		~SoftLeaf();

		void Resize(int w, int h);

		void Do(Texture * colorTex, Texture * depthTex);

	protected:

		void _downSample(Texture * colorTex, Texture * depthTex);
		void _blur();
		void _blend();

	protected:
		RenderTargetPtr mRT_Quad;
		TexturePtr mTex_Quad;

		Technique * mTech_DownSample;
		Technique * mTech_Blend;
		Technique * mTech_BlurH;
		Technique * mTech_BlurV;
	};
}