#include "stdafx.h"

#include "xMaterial.h"

namespace MaxPlugin {

	xMaterial::xMaterial()
	{
		mBlendMode = BM_OPATICY;

		mEmissive = Color4::Black;
		mAmbient = Color4::Gray;
		mDiffuse = Color4::White;
		mSpecular = Color4::Black;
		mSpecularPower = 80;
	}

	xMaterial::~xMaterial()
	{
	}
}