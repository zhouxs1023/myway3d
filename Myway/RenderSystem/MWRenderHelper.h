#pragma once

#include "MWRenderDefine.h"
#include "MWRenderSystem.h"

namespace Myway
{

class MW_ENTRY RenderHelper
{
    DECLARE_SINGLETON(RenderHelper);

public:
    RenderHelper();
    ~RenderHelper();

    void OnCall(Event * sender, void * data);

    Texture * GetTexture(Material * mat, SamplerBindType type);

    TexturePtr GetWhiteTexture() { return mWhiteTexture; }
    TexturePtr GetBlackTexture() { return mBlackTexture; }
    TexturePtr GetDefaultNormalTexture() { return mDefaultNormaLTexture; }

    void SetDefaultTextureFilter(TEXTURE_FILTER_TYPE type) { mDefaultTextureFilter = type; }
    TEXTURE_FILTER_TYPE GetDefaultTextureFilter() { return mDefaultTextureFilter; }

    void DrawScreenQuad(BLEND_MODE mode, Technique * tech);

protected:
	void _resize(void * param0, void * param1);
    void _initTexture();
    void _initScreenQuad();
    void _updateScreenQuad();

protected:
    TexturePtr mWhiteTexture;
    TexturePtr mBlackTexture;
    TexturePtr mDefaultNormaLTexture;
    
    TEXTURE_FILTER_TYPE mDefaultTextureFilter;

    RenderOp mScreenQuad;

	tEventListener<RenderHelper> OnReisze;
};

   
}