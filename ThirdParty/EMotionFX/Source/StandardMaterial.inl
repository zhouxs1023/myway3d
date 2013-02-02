/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "StandardMaterial.h"


inline float StandardMaterialLayer::GetUOffset() const
{
	return mUOffset;
}


inline float StandardMaterialLayer::GetVOffset() const
{
	return mVOffset;
}


inline float StandardMaterialLayer::GetUTiling() const
{
	return mUTiling;
}


inline float StandardMaterialLayer::GetVTiling() const
{
	return mVTiling;
}


inline float StandardMaterialLayer::GetRotationRadians() const
{
	return mRotationRadians;
}


inline void StandardMaterialLayer::SetUOffset(const float uOffset)
{
	mUOffset = uOffset;
}


inline void StandardMaterialLayer::SetVOffset(const float vOffset)
{
	mVOffset = vOffset;
}


inline void StandardMaterialLayer::SetUTiling(const float uTiling)
{
	mUTiling = uTiling;
}


inline void StandardMaterialLayer::SetVTiling(const float vTiling)
{
	mVTiling = vTiling;
}


inline void StandardMaterialLayer::SetRotationRadians(const float rotationRadians)
{
	mRotationRadians = rotationRadians;
}


inline const MCore::String& StandardMaterialLayer::GetFilename() const
{
	return mFilename;
}


inline void StandardMaterialLayer::SetFilename(const MCore::String& filename)
{
	mFilename = filename;
}


inline void StandardMaterialLayer::SetAmount(const float amount)
{
	mAmount = amount;
}


inline float StandardMaterialLayer::GetAmount() const
{
	return mAmount;
}


inline int StandardMaterialLayer::GetType() const
{
	return mLayerTypeID;
}


inline void StandardMaterialLayer::SetType(const int typeID)
{
	mLayerTypeID = typeID;
}


inline void StandardMaterial::SetAmbient(const MCore::RGBAColor& ambient)
{
	mAmbient = ambient;
}


inline void StandardMaterial::SetDiffuse(const MCore::RGBAColor& diffuse)
{
	mDiffuse = diffuse;
}


inline void StandardMaterial::SetSpecular(const MCore::RGBAColor& specular)
{
	mSpecular = specular;
}


inline void StandardMaterial::SetEmissive(const MCore::RGBAColor& emissive)
{
	mEmissive = emissive;
}


inline void StandardMaterial::SetShine(const float shine)
{
	mShine = shine;
}


inline void StandardMaterial::SetShineStrength(const float shineStrength)
{
	mShineStrength = shineStrength;
}


inline void StandardMaterial::SetOpacity(const float opacity)
{
	mOpacity = opacity;
}


inline void StandardMaterial::SetIOR(const float ior)
{
	mIOR = ior;
}


inline void StandardMaterial::SetDoubleSided(const bool doubleSided)
{
	mDoubleSided = doubleSided;
}


inline void StandardMaterial::SetWireFrame(const bool wireFrame)
{
	mWireFrame = wireFrame;
}

/*
void StandardMaterial::SetTransparencyType(const char& transparencyType)
{
	mTransparencyType = transparencyType;
}
*/


inline const MCore::RGBAColor& StandardMaterial::GetAmbient() const
{
	return mAmbient;
}


inline const MCore::RGBAColor& StandardMaterial::GetDiffuse() const
{
	return mDiffuse;
}


inline const MCore::RGBAColor& StandardMaterial::GetSpecular() const
{
	return mSpecular;
}


inline const MCore::RGBAColor& StandardMaterial::GetEmissive() const
{
	return mEmissive;
}


inline float StandardMaterial::GetShine() const
{
	return mShine;
}


inline float StandardMaterial::GetShineStrength() const
{
	return mShineStrength;
}


inline float StandardMaterial::GetOpacity() const
{
	return mOpacity;
}


inline float StandardMaterial::GetIOR() const
{
	return mIOR;
}


inline bool StandardMaterial::GetDoubleSided() const
{
	return mDoubleSided;
}


inline bool StandardMaterial::GetWireFrame() const
{
	return mWireFrame;
}

/*
char StandardMaterial::GetTransparencyType() const
{
	return mTransparencyType;
}
*/


inline void StandardMaterial::AddLayer(MCore::Pointer< StandardMaterialLayer > layer)
{
	mLayers.Add( layer );
}


inline int StandardMaterial::GetNumLayers() const
{
	return mLayers.GetLength();
}


inline MCore::Pointer< StandardMaterialLayer > StandardMaterial::GetLayer(const int nr) const
{
	assert(nr < mLayers.GetLength());
	return mLayers[nr];
}


inline void StandardMaterial::RemoveLayer(const int nr)
{
	assert(nr < mLayers.GetLength());		
	mLayers.Remove(nr);
}


inline void StandardMaterial::RemoveLayer(MCore::Pointer< StandardMaterialLayer > layer)
{
	mLayers.Remove(layer);
}


inline void StandardMaterial::RemoveAllLayers()
{
	while (mLayers.GetLength())
	{
		mLayers.RemoveLast();
	}
}


inline int StandardMaterial::FindLayer(const int layerType) const
{
	// search through all layers
	const int numLayers = mLayers.GetLength();
	for (int i=0; i<numLayers; i++)
		if (mLayers[i].GetPointer()->GetType() == layerType)
			return i;

	return -1;
}