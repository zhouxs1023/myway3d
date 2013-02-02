/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "FXMaterial.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// constructor
FXMaterial::FXMaterial(const MCore::String& name) : Material(name)
{
}


// destructor
FXMaterial::~FXMaterial()
{
}


// return the unique material type identification number
int FXMaterial::GetType() const
{
	return TYPE_ID;
}


// return the type identification string
const char* FXMaterial::GetTypeString() const
{
	return "FXMaterial";
}


// create and return a clone of the material
MCore::Pointer<Material> FXMaterial::Clone()
{
	// create the new material
	MCore::Pointer<Material> clone( new FXMaterial(mName) );

	// type-cast the fx material since it is stored as material base in the smart pointer
	FXMaterial* fxMaterial = static_cast<FXMaterial*>(clone.GetPointer());

	// retrieve the numbers of the different parameters
	const int numStringParams	= mStringParams.GetLength();
	const int numColorParams	= mColorParams.GetLength();
	const int numFloatParams	= mFloatParams.GetLength();
	const int numIntParams		= mIntParams.GetLength();

	int i;

	// iterate through the string parameters and copy their values
	for (i=0; i<numStringParams; ++i)
		fxMaterial->AddStringParameter(mStringParams[i].GetName(), mStringParams[i].GetValue());

	// iterate through the color parameters and copy their values
	for (i=0; i<numColorParams; ++i)
		fxMaterial->AddColorParameter(mColorParams[i].GetName(), mColorParams[i].GetValue());

	// iterate through the float parameters and copy their values
	for (i=0; i<numFloatParams; ++i)
		fxMaterial->AddFloatParameter(mFloatParams[i].GetName(), mFloatParams[i].GetValue());

	// iterate through the int parameters and copy their values
	for (i=0; i<numIntParams; ++i)
		fxMaterial->AddIntParameter(mIntParams[i].GetName(), mIntParams[i].GetValue());

	// return the cloned fx material
	return clone;
}

} // namespace EMotionFX