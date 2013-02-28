#pragma once

#include "MWRenderSystem.h"

namespace Myway {

	class SSAO
	{
		DECLARE_ALLOC();

	public:
		SSAO();
		~SSAO();

		void Render(Texture * depthTex, Texture * normalTex);

	protected:
		void _init();
		void _resize(Event * sender);

		void _renderAo(Texture * depthTex, Texture * normalTex);
		void _downSample();
		void _blur();
		void _blend();

	protected:
		RenderTargetPtr mRT_Ao;
		TexturePtr mTex_Ao;

		RenderTargetPtr mRT_Quad;
		TexturePtr mTex_Quad;

		TexturePtr mTex_Random;

		Technique * mTech_Ao;
		Technique * mTech_DownSample;
		Technique * mTech_BlurH;
		Technique * mTech_BlurV;
		Technique * mTech_Blend;

		tEventListener<SSAO> OnResize;
	};
}