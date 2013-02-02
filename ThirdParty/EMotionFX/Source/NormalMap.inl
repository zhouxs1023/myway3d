/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NormalMap.h"


// get the width of the normal map image in pixels
inline int NormalMap::GetWidth() const
{ 
	return mWidth; 
}


// get the height of the normal map image in pixels
inline int NormalMap::GetHeight() const 
{ 
	return mHeight;
}


// check if x and y are in the range of the image's resolution
inline bool NormalMap::InRange(const int x, const int y)
{
	return (x>=0 && y>=0 && x<mWidth && y<mHeight);
}


// check if the given pixel is used
inline bool NormalMap::IsFree(const int x, const int y)
{
	if (!InRange(x, y)) return false;
	return mPixels[x+y*mWidth].isFree;
}


// get the name of the normal map
inline const MCore::String& NormalMap::GetName() const
{
	return mName;
}


// get the pixel data
inline NormalMap::Pixel* NormalMap::GetPixelData()
{
	return mPixels;
}
