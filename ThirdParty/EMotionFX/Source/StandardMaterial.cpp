/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "StandardMaterial.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

//-------------------------------------------------------------------------------------------------
// StandardMaterialLayer
//-------------------------------------------------------------------------------------------------

// constructor
StandardMaterialLayer::StandardMaterialLayer() : mLayerTypeID(LAYERTYPE_UNKNOWN)
{
	mAmount			= 1.0f;
	mUOffset		= 0.0f;
	mVOffset		= 0.0f;
	mUTiling		= 1.0f;
	mVTiling		= 1.0f;
	mRotationRadians= 0.0f;
}


// extra constructor
StandardMaterialLayer::StandardMaterialLayer(const int layerType, const MCore::String& filename, const float amount) : mLayerTypeID(layerType)
{
	mFilename		= filename;
	mAmount			= amount;
	mUOffset		= 0.0f;
	mVOffset		= 0.0f;
	mUTiling		= 1.0f;
	mVTiling		= 1.0f;
	mRotationRadians= 0.0f;
}


// destructor
StandardMaterialLayer::~StandardMaterialLayer()
{
}


// clone the layer
StandardMaterialLayer* StandardMaterialLayer::Clone()
{
	StandardMaterialLayer* result = new StandardMaterialLayer(mLayerTypeID, mFilename, mAmount);

	result->mUOffset		= mUOffset;
	result->mVOffset		= mVOffset;
	result->mUTiling		= mUTiling;
	result->mVTiling		= mVTiling;
	result->mRotationRadians= mRotationRadians;

	return result;
}


//-------------------------------------------------------------------------------------------------
// StandardMaterial
//-------------------------------------------------------------------------------------------------

// constructor
StandardMaterial::StandardMaterial(const String& name) : Material(name)
{
	mAmbient		= RGBAColor(0.2f, 0.2f, 0.2f);
	mDiffuse		= RGBAColor(1.0f, 0.0f, 0.0f);
	mSpecular		= RGBAColor(1.0f, 1.0f, 1.0f);
	mEmissive		= RGBAColor(1.0f, 0.0f, 0.0f);
	mShine			= 1;
	mShineStrength	= 1;
	mOpacity		= 1.0f;
	mIOR			= 1.5f;
	mDoubleSided	= true;
	mWireFrame		= false;

	mShaderMask		= 0;
	mAlphaRef		= 127;
	mTexFxn			= 0;
	mTexFxnUParm	= 0.0f;
	mTexFxnVParm	= 0.0f;	
	mTexFxnSub0Parm = 0.0f;
}


// destructor
StandardMaterial::~StandardMaterial()
{
	// get rid of all material layers
	RemoveAllLayers();
}


// clone the standard material
MCore::Pointer< Material > StandardMaterial::Clone()
{
	// create the new material
	MCore::Pointer< Material > clone( new StandardMaterial(mName) );

	// type-cast the standard material since it is stored as material base in the smart pointer
	StandardMaterial* standardMaterial = static_cast<StandardMaterial*>(clone.GetPointer());

	// copy the attributes
	standardMaterial->mAmbient			= mAmbient;
	standardMaterial->mDiffuse			= mDiffuse;
	standardMaterial->mSpecular			= mSpecular;
	standardMaterial->mEmissive			= mEmissive;
	standardMaterial->mShine			= mShine;
	standardMaterial->mShineStrength	= mShineStrength;
	standardMaterial->mOpacity			= mOpacity;
	standardMaterial->mIOR				= mIOR;
	standardMaterial->mDoubleSided		= mDoubleSided;
	standardMaterial->mWireFrame		= mWireFrame;

	standardMaterial->mShaderMask		= mShaderMask;
	standardMaterial->mAlphaRef			= mAlphaRef;
	standardMaterial->mTexFxn			= mTexFxn;
	standardMaterial->mTexFxnUParm		= mTexFxnUParm;
	standardMaterial->mTexFxnVParm		= mTexFxnVParm;
	standardMaterial->mTexFxnSub0Parm	= mTexFxnSub0Parm;


	// copy the layers
	const int numLayers = mLayers.GetLength();
	for (int i=0; i<numLayers; i++)
		standardMaterial->mLayers.Add( mLayers[i]->Clone() );

	// return the result
	return clone;
}

} // namespace EMotionFX