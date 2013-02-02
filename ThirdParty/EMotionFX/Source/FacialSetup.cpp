/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include EMotion FX related headers
#include "FacialSetup.h"
#include "ExpressionPart.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
FacialSetup::FacialSetup()
{
}

// destructor
FacialSetup::~FacialSetup()
{
	RemoveAllExpressionParts();
	RemoveAllPhonemes();
}


// add an expression part
void FacialSetup::AddExpressionPart(ExpressionPart* part)
{
	mExpressionParts.Add( part );
}


// remove an expression part
void FacialSetup::RemoveExpressionPart(const int nr, const bool delFromMem)
{
	if (delFromMem)
		delete mExpressionParts[nr];

	mExpressionParts.Remove(nr);
}


// remove an expression part
void FacialSetup::RemoveExpressionPart(ExpressionPart* part, const bool delFromMem)
{
	mExpressionParts.Remove( part );

	if (delFromMem)
		delete part;
}


// remove all expression parts
void FacialSetup::RemoveAllExpressionParts()
{
	// remove all expression parts
	while (mExpressionParts.GetLength())
	{
		delete mExpressionParts.GetLast();
		mExpressionParts.RemoveLast();
	}
}


// get an expression part by ID
ExpressionPart* FacialSetup::GetExpressionPartByID(const int id) const
{
	// linear search, and check ID's
	const int numParts = mExpressionParts.GetLength();
	for (int i=0; i<numParts; i++)
		if (mExpressionParts[i]->GetID() == id)
			return mExpressionParts[i];

	// nothing found
	return NULL;
}


// get an expression part by ID
int FacialSetup::GetExpressionPartNumberByID(const int id) const
{
	// linear search, and check ID's
	const int numParts = mExpressionParts.GetLength();
	for (int i=0; i<numParts; i++)
		if (mExpressionParts[i]->GetID() == id)
			return i;

	// nothing found
	return -1;
}


//-----------------

// add a phoneme
void FacialSetup::AddPhoneme(ExpressionPart* phoneme)
{
	mPhonemes.Add( phoneme );
}


// remove a phoneme
void FacialSetup::RemovePhoneme(const int nr, const bool delFromMem)
{
	if (delFromMem)
		delete mPhonemes[nr];

	mPhonemes.Remove(nr);
}


// remove a phoneme
void FacialSetup::RemovePhoneme(ExpressionPart* phoneme, const bool delFromMem)
{
	mPhonemes.Remove( phoneme );

	if (delFromMem)
		delete phoneme;
}


// remove all phonemes
void FacialSetup::RemoveAllPhonemes()
{
	while (mPhonemes.GetLength())
	{
		delete mPhonemes.GetLast();
		mPhonemes.RemoveLast();
	}
}


// get a phoneme by ID
ExpressionPart* FacialSetup::GetPhonemeByID(const int id) const
{
	// linear search, and check ID's
	const int numParts = mPhonemes.GetLength();
	for (int i=0; i<numParts; i++)
		if (mPhonemes[i]->GetID() == id)
			return mPhonemes[i];

	// nothing found
	return NULL;
}


// get a phoneme by ID
int FacialSetup::GetPhonemeNumberByID(const int id) const
{
	// linear search, and check ID's
	const int numParts = mPhonemes.GetLength();
	for (int i=0; i<numParts; i++)
		if (mPhonemes[i]->GetID() == id)
			return i;

	// nothing found
	return -1;
}

} // namespace EMotionFX