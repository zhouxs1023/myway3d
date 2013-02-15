#pragma once

#include "MWRenderScheme.h"
#include "MWRenderSystem.h"

namespace Myway {

	class MSoftLeaf
	{
	public:
		MSoftLeaf();
		~MSoftLeaf();

		void Do();

	protected:
		void _resize(void *, void *);

		void _downSample();
		void _blur();
		void _blend();

	protected:
		RenderTargetPtr mRT_Quad;
		TexturePtr mTex_Quad;

		Technique * mTech_DownSample;
		Technique * mTech_BlurH;
		Technique * mTech_BlurV;
		Technique * mTech_Blend;
	};
}