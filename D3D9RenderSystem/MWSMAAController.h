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

		void Init();
		void Shutdown();

		void SetSMAAType(eSmaaType::enum_t type);
		void Do(RenderTarget * rt, Texture * colorTex);

		eSmaaType::enum_t GetSMAAType() { return mType; }

	protected:
		eSmaaType::enum_t mType;
		SMAA * mSMAA;
		int mWidth, mHeight;
	};
}