/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "NormalMapGenerator.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
NormalMapGenerator::NormalMapGenerator() 
{
	mCurNormalMap	= -1;
	mRatio			= 1.0f;
	mDefaultRes		= 256;
}


// destructor
NormalMapGenerator::~NormalMapGenerator()
{
	ReleaseNormalMaps();
}


// get rid of normal maps
void NormalMapGenerator::ReleaseNormalMaps()
{
	while (mNormalMaps.GetLength())
	{
		delete mNormalMaps.GetLast();
		mNormalMaps.RemoveLast();
	}
}


// return number of normal maps
int NormalMapGenerator::GetNumNormalMaps() const
{
	return mNormalMaps.GetLength();
}


// get access to the given normal map
NormalMap* NormalMapGenerator::GetNormalMap(const int nr) const
{
	assert(nr < mNormalMaps.GetLength());
	return mNormalMaps[nr];
}


// return pointer to current normal map
int NormalMapGenerator::GetCurrentNormalMap() const
{
	return mCurNormalMap;
}


// set ratio of the normal maps
void NormalMapGenerator::SetRatio(const float ratio)
{
	mRatio = ratio;
}


// set default resolution, width and the height of the normal maps will be set to the same given value
void NormalMapGenerator::SetDefaultRes(const int resolution)
{
	mDefaultRes = resolution;
}

} // namespace EMotionFX