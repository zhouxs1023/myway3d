/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "EyeBlinkKeyFrameGenerator.h"
#include "KeyTrack.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
EyeBlinkKeyFrameGenerator::EyeBlinkKeyFrameGenerator() : KeyFrameGenerator<float, Compressed8BitFloat>()
{
	mStartTime	= 0;
	mEndTime	= 30.0f;
	mInterval	= 2.0f;
	mRandomness	= 1.5f;
	mBlinkSpeed	= 0.15f;
}


// destructor
EyeBlinkKeyFrameGenerator::~EyeBlinkKeyFrameGenerator()
{
}


float EyeBlinkKeyFrameGenerator::AlignToFPS(const float timeValue, const int framesPerSecond)
{
	float dif = fmod(timeValue, 1.0f / framesPerSecond);
	return timeValue + dif;
}


// generate method
void EyeBlinkKeyFrameGenerator::Generate(KeyTrack<float, Compressed8BitFloat>* outTrack)
{
	// precalc some values
	const float halfBlinkSpeed = mBlinkSpeed * 0.5f;
	
	// start a bit after the first time
	float curTime = mStartTime;

	// add a first key, with eyes open
	outTrack->AddKeySorted(AlignToFPS(mStartTime, 30), 0.0f);

	// while we can still add blinks in the range
	const float lastTime = mEndTime - mInterval - mRandomness;
	while (curTime < lastTime)
	{
		// calculate the displacement on the interval
		float displacement = MCore::Random::RandF(-mRandomness, mRandomness);

		// calculate the time of the eyblink, when the eyes should be closed
		curTime += mInterval + displacement;

		float startBlinkTime= AlignToFPS(curTime - halfBlinkSpeed, 30);
		float blinkTime		= AlignToFPS(curTime, 30);
		float endBlinkTime  = AlignToFPS(curTime + halfBlinkSpeed, 30);

		// add the keys to generate a blink
		outTrack->AddKeySorted(startBlinkTime, 0.0f);
		outTrack->AddKeySorted(blinkTime, 1.0f);
		outTrack->AddKeySorted(endBlinkTime, 0.0f);
	}

	// add the last key, with eyes open
	outTrack->AddKeySorted(AlignToFPS(mEndTime, 30), 0.0f);
}


// get the description
const char* EyeBlinkKeyFrameGenerator::GetDescription() const
{
	return "EyeBlinkKeyFrameGenerator";
}


// get the generator ID
int EyeBlinkKeyFrameGenerator::GetType() const
{
	return EyeBlinkKeyFrameGenerator::TYPE_ID;
}


// setup the properties of the system
void EyeBlinkKeyFrameGenerator::SetProperties(const float startTime, const float endTime, const float interval, const float randomness, const float blinkSpeed)
{
	mStartTime	= startTime;
	mEndTime	= endTime;
	mInterval	= interval;
	mRandomness	= randomness;
	mBlinkSpeed	= blinkSpeed;
}


// get the start time
float EyeBlinkKeyFrameGenerator::GetStartTime() const
{
	return mStartTime;
}


// get the end time
float EyeBlinkKeyFrameGenerator::GetEndTime() const
{
	return mEndTime; 
}


// get the randomness
float EyeBlinkKeyFrameGenerator::GetRandomness() const
{ 
	return mRandomness; 
}


// get the interval
float EyeBlinkKeyFrameGenerator::GetInterval() const
{
	return mInterval; 
}


// get the blink speed
float EyeBlinkKeyFrameGenerator::GetBlinkSpeed() const
{ 
	return mBlinkSpeed; 
}

} // namespace EMotionFX