

inline const TString128 & Texture::GetName() const
{
    return mName;
}

inline int Texture::GetWidth() const
{ 
    return mWidth;
}

inline int Texture::GetHeight() const
{ 
    return mHeight;
}

inline int Texture::GetDepth() const
{ 
    return mDepth;
}

inline int Texture::GetMipLevels() const
{ 
    return mMipLevels;
}

inline int Texture::GetUsage() const
{ 
    return mUsage;
}

inline POOL Texture::GetPool() const
{ 
    return mPool;
}

inline FORMAT Texture::GetFormat() const
{ 
    return mFormat;
}

inline TEXTURE_TYPE Texture::GetTextureType() const
{ 
    return mType;
}
