#include "MWTexture.h"
#include "MWResourceManager.h"
#include "Engine.h"

namespace Myway
{
    Texture::Texture()
        : mName("")
        , mHeight(0)
        , mWidth(0)
        , mDepth(0)
        , mMipLevels(0)
        , mUsage(USAGE_STATIC)
        , mFormat(FMT_UNKNOWN)
        , mType(TEXTYPE_2D)

    {
    }

    Texture::~Texture()
    {
    }

	const TString128 & Texture::GetName() const
	{ 
		return mName;
	}

	int Texture::GetWidth() const
	{ 
		if (mWidth != -1)
			return mWidth;
		else
			return Engine::Instance()->GetDeviceProperty()->Width;
	}

	int Texture::GetHeight() const
	{
		if (mHeight != -1)
			return mHeight;
		else
			return Engine::Instance()->GetDeviceProperty()->Height;
	}

	int Texture::GetDepth() const
	{ 
		return mDepth;
	}

	int Texture::GetMipLevels() const
	{ 
		return mMipLevels;
	}

	USAGE Texture::GetUsage() const
	{ 
		return mUsage;
	}

	FORMAT Texture::GetFormat() const
	{
		return mFormat;
	}

	TEXTURE_TYPE Texture::GetTextureType() const
	{ 
		return mType;
	}
}