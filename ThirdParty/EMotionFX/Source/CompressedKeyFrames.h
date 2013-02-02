/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __COMPRESSEDKEYFRAMES_H
#define __COMPRESSEDKEYFRAMES_H

// include Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "KeyFrame.h"


namespace EMotionFX
{

//--------------------------------------------------------------------------------------
// Partial Template Specialization for KeyFrames with the following template arguments:
// ReturnType:  Quaternion
// StorageType: Compressed8BitQuaternion
//--------------------------------------------------------------------------------------
// compress a quaternion
template<>
inline void KeyFrame<MCore::Quaternion, MCore::Compressed8BitQuaternion>::SetValue(const MCore::Quaternion& value)		{ mValue.FromQuaternion(value); }


// decompress into a quaternion
template<>
inline MCore::Quaternion KeyFrame<MCore::Quaternion, MCore::Compressed8BitQuaternion>::GetValue() const					{ return mValue.ToQuaternion(); }



//--------------------------------------------------------------------------------------
// Partial Template Specialization for KeyFrames with the following template arguments:
// ReturnType:  Quaternion
// StorageType: Compressed16BitQuaternion
//--------------------------------------------------------------------------------------
// compress a quaternion
template<>
inline void KeyFrame<MCore::Quaternion, MCore::Compressed16BitQuaternion>::SetValue(const MCore::Quaternion& value)		{ mValue.FromQuaternion(value); }


// decompress into a quaternion
template<>
inline MCore::Quaternion KeyFrame<MCore::Quaternion, MCore::Compressed16BitQuaternion>::GetValue() const				{ return mValue.ToQuaternion(); }



//--------------------------------------------------------------------------------------
// Partial Template Specialization for KeyFrames with the following template arguments:
// ReturnType:  float
// StorageType: Compressed8BitFloat
//--------------------------------------------------------------------------------------
// compress a float
template<>
inline void KeyFrame<float, MCore::Compressed8BitFloat>::SetValue(const float& value)	{ mValue.FromFloat(value, 0.0f, 1.0f); }


// decompress into a float
template<>
inline float KeyFrame<float, MCore::Compressed8BitFloat>::GetValue() const				{ return mValue.ToFloat(0.0f, 1.0f); }


//--------------------------------------------------------------------------------------
// Partial Template Specialization for KeyFrames with the following template arguments:
// ReturnType:  float
// StorageType: Compressed16BitFloat
//--------------------------------------------------------------------------------------
// compress a float
template<>
inline void KeyFrame<float, MCore::Compressed16BitFloat>::SetValue(const float& value)	{ mValue.FromFloat(value, 0.0f, 1.0f); }


// decompress into a float
template<>
inline float KeyFrame<float, MCore::Compressed16BitFloat>::GetValue() const				{ return mValue.ToFloat(0.0f, 1.0f); }


} // namespace EMotionFX


#endif