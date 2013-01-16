#pragma once


#include "MWRenderDefine.h"
#include "MWTexture.h"

namespace Myway
{


class MW_ENTRY RenderScheme
{
    DECLARE_ALLOC();
    DECLARE_SINGLETON(RenderScheme);

public:
    RenderScheme();
    virtual ~RenderScheme();

	virtual Texture * GetDepthTexture() = 0;
	virtual Texture * GetColorTexture() = 0;
	virtual Texture * GetNormalTexture() = 0;
	virtual Texture * GetMaterialTexture() = 0;

    virtual void DoRender() = 0;
};


}