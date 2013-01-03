#pragma once

#include "SMAA.h"
#include "MWD3D9Texture.h"
#include "MWD3D9RenderTarget.h"

namespace Myway {
	
	class SMAAController
	{
	public:
		SMAAController();
		~SMAAController();

		void Init(IDirect3DDevice9 * device);
		void Shutdown();

		void Do(RenderTarget * rt, Texture * colorTex);

	protected:
		SMAA * mSMAA;
	};
}