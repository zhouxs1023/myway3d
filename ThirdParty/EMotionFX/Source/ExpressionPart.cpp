/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "KeyTrack.h"
#include "Node.h"
#include "Actor.h"
#include "LinearInterpolators.h"
#include "ExpressionPart.h"
#include "NodeIDGenerator.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor
ExpressionPart::ExpressionPart(Actor* parentActor, const MCore::String& name)
{
	mCurWeight		= 0.0f;
	mActor			= parentActor;
	mName			= name;
	mCharacter		= 0;
	mRangeMin		= 0.0f;
	mRangeMax		= 1.0f;
	mManualMode		= false;

	// calculate the ID
	SetName( name );
}


// destructor
ExpressionPart::~ExpressionPart()
{
}


// update the name and ID
void ExpressionPart::SetName(const MCore::String& name)
{
	mName = name;
	mID   = NodeIDGenerator::GetInstance().GenerateIDForString( name );
}


// set the phoneme sets
void ExpressionPart::SetPhonemeSets(const MCore::Array<ExpressionPart::EPhonemeSet>& phonemeSets)
{
	mPhonemeSets = phonemeSets;
}


// get the phoneme sets
const MCore::Array<ExpressionPart::EPhonemeSet>& ExpressionPart::GetPhonemeSets() const
{
	return mPhonemeSets;
}


// set the character of the expression (phoneme)
void ExpressionPart::SetPhonemeCharacter(const char character)
{
	mCharacter = character;
}


// get the character of the expression (phoneme)
char ExpressionPart::GetPhonemeCharacter() const
{
	return mCharacter;
}


void ExpressionPart::SetRangeMin(const float rangeMin)
{
	mRangeMin = rangeMin;
}


void ExpressionPart::SetRangeMax(const float rangeMax)
{
	mRangeMax = rangeMax;
}


float ExpressionPart::GetRangeMin() const
{
	return mRangeMin;
}


float ExpressionPart::GetRangeMax() const
{
	return mRangeMax;
}


void ExpressionPart::SetEnableManualMode(const bool enabled)
{
	mManualMode = enabled;
}


float ExpressionPart::CalcZeroInfluenceWeight() const
{
	return Math::Abs(mRangeMin) / Math::Abs(mRangeMax - mRangeMin);
}



} // namespace EMotionFX