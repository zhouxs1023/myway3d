#pragma once


#include "MWRenderDefine.h"
#include "MWTexture.h"

namespace Myway
{


class MW_ENTRY RenderScheme
{
    DECLARE_ALLOC();

public:
    RenderScheme();
    virtual ~RenderScheme();

	virtual Texture * GetDepthTexture() = 0;
	virtual Texture * GetColorTexture() = 0;
	virtual Texture * GetNormalTexture() = 0;
	virtual Texture * GetMaterialTexture() = 0;

    virtual void DoRender() = 0;
	virtual void RenderInMirror(const Plane & mirrorPlane) = 0;
	virtual void Resize(int w, int h) = 0;
};


}