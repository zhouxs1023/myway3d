#pragma once

#include "MWRenderScheme.h"
#include "MWRenderSystem.h"

namespace Myway {

	class SoftLeaf
	{
	public:
		SoftLeaf();
		~SoftLeaf();

		void Do(Texture * colorTex, Texture * depthTex);

	protected:
		void _resize(void *, void *);

		void _downSample(Texture * colorTex, Texture * depthTex);
		void _blur();
		void _blend();

	protected:
		tEventListener<SoftLeaf> OnResize;

		RenderTargetPtr mRT_Quad;
		TexturePtr mTex_Quad;

		Technique * mTech_DownSample;
		Technique * mTech_Blend;
		Technique * mTech_BlurH;
		Technique * mTech_BlurV;
	};
}