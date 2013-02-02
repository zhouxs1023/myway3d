/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// constructor
template <class T>
FXMaterialParameter<T>::FXMaterialParameter(const MCore::String& name, const T& value) : mName(name), mValue(value)
{
}


// get the value of the parameter
template <class T>
inline const T& FXMaterialParameter<T>::GetValue() const
{
	return mValue;
}


// get the name of the parameter
template <class T>
inline const MCore::String& FXMaterialParameter<T>::GetName() const
{
	return mName;
}


// get the name of the parameter
template <class T>
inline const char* FXMaterialParameter<T>::GetNamePtr() const
{
	return mName.AsChar();
}


// set the value of the parameter
template <class T>
inline void FXMaterialParameter<T>::SetValue(const T& value)
{
	mValue = value;
}


// set the name of the parameter
template <class T>
inline void FXMaterialParameter<T>::SetName(const MCore::String& name)
{
	mName = name;
}


// get a given integer parameter by its index
inline FXMaterialParameter<int>& FXMaterial::GetIntParameter(const int nr)
{
	return mIntParams[nr];
}


// get a given float parameter by its index
inline FXMaterialParameter<float>& FXMaterial::GetFloatParameter(const int nr)
{
	return mFloatParams[nr];
}


// get a given color parameter by its index
inline FXMaterialParameter<MCore::RGBAColor>& FXMaterial::GetColorParameter(const int nr)
{
	return mColorParams[nr];
}


// get a given string parameter by its index
inline FXMaterialParameter<MCore::String>& FXMaterial::GetStringParameter(const int nr)
{
	return mStringParams[nr];
}


// get the number of float parameters
inline int FXMaterial::GetNumFloatParameters() const
{
	return mFloatParams.GetLength();
}


// get the number of integer parameters
inline int FXMaterial::GetNumIntParameters() const
{
	return mIntParams.GetLength();
}


// get the number of color parameters
inline int FXMaterial::GetNumColorParameters() const
{
	return mColorParams.GetLength();
}


// get the number of string parameters
inline int FXMaterial::GetNumStringParameters() const
{
	return mStringParams.GetLength();
}


// add an integer parameter to the material
inline void FXMaterial::AddIntParameter(const MCore::String& name, const int value)
{
	mIntParams.Add( FXMaterialParameter<int>(name, value) );
}


// add an float parameter to the material
inline void FXMaterial::AddFloatParameter(const MCore::String& name, const float value)
{
	mFloatParams.Add( FXMaterialParameter<float>(name, value) );
}


// add an color parameter to the material
inline void FXMaterial::AddColorParameter(const MCore::String& name, const MCore::RGBAColor& value)
{
	mColorParams.Add( FXMaterialParameter<MCore::RGBAColor>(name, value) );
}


// add an string parameter to the material
inline void FXMaterial::AddStringParameter(const MCore::String& name, const MCore::String& value)
{
	mStringParams.Add( FXMaterialParameter<MCore::String>(name, value) );
}


// remove the given integer parameter by its index
inline void FXMaterial::RemoveIntParameter(const int index)
{
	mIntParams.Remove(index);
}


// remove the given float parameter by its index
inline void FXMaterial::RemoveFloatParameter(const int index)
{
	mFloatParams.Remove(index);
}

// remove the given color parameter by its index
inline void FXMaterial::RemoveColorParameter(const int index)
{
	mColorParams.Remove(index);
}


// remove the given string parameter by its index
inline void FXMaterial::RemoveStringParameter(const int index)
{
	mStringParams.Remove(index);
}


// set the value of the given integer parameter
inline void FXMaterial::SetIntParameter(const int index, const int value)
{
	mIntParams[index].mValue = value;
}


// set the value of the given float parameter
inline void FXMaterial::SetFloatParameter(const int index, const float value)
{
	mFloatParams[index].mValue = value;
}


// set the value of the given color parameter
inline void FXMaterial::SetColorParameter(const int index, const MCore::RGBAColor& value)
{
	mColorParams[index].mValue = value;
}


// set the value of the given string parameter
inline void FXMaterial::SetStringParameter(const int index, const MCore::String& value)
{
	mStringParams[index].mValue = value;
}


// set the value of the given integer parameter by name
inline bool FXMaterial::SetIntParameter(const MCore::String& name, const int value)
{
	// retrieve the parameter index
	const int index = FindIntParameterIndex(name);

	// if the index is not valid return
	if (index == -1)
		return false;

	mIntParams[index].mValue = value;

	return true;
}


// set the value of the given float parameter by name
inline bool FXMaterial::SetFloatParameter(const MCore::String& name, const float value)
{
	// retrieve the parameter index
	const int index = FindFloatParameterIndex(name);

	// if the index is not valid return
	if (index == -1)
		return false;

	mFloatParams[index].mValue = value;

	return true;
}


// set the value of the given color parameter by name
inline bool FXMaterial::SetColorParameter(const MCore::String& name, const MCore::RGBAColor& value)
{
	// retrieve the parameter index
	const int index = FindColorParameterIndex(name);

	// if the index is not valid return
	if (index == -1)
		return false;

	mColorParams[index].mValue = value;

	return true;
}


// set the value of the given string parameter by name
inline bool FXMaterial::SetStringParameter(const MCore::String& name, const MCore::String& value)
{
	// retrieve the parameter index
	const int index = FindStringParameterIndex(name);

	// if the index is not valid return
	if (index == -1)
		return false;

	mStringParams[index].mValue = value;

	return true;
}


// get the index of the given integer parameter
inline int FXMaterial::FindIntParameterIndex(const MCore::String& name) const
{
	// retrieve the number of integer parameters
	const int numParams = mIntParams.GetLength();

	// iterate through the parameters and search the given parameter
	for (int i=0; i<numParams; ++i)
		if (mIntParams[i].mName == name)
			return i;

	return -1;
}


// get the index of the given float parameter
inline int FXMaterial::FindFloatParameterIndex(const MCore::String& name) const
{
	// retrieve the number of float parameters
	const int numParams = mFloatParams.GetLength();

	// iterate through the parameters and search the given parameter
	for (int i=0; i<numParams; ++i)
		if (mFloatParams[i].mName == name)
			return i;

	return -1;
}


// get the index of the given color parameter
inline int FXMaterial::FindColorParameterIndex(const MCore::String& name) const
{
	// retrieve the number of color parameters
	const int numParams = mColorParams.GetLength();

	// iterate through the parameters and search the given parameter
	for (int i=0; i<numParams; ++i)
		if (mColorParams[i].mName == name)
			return i;

	return -1;
}


// get the index of the given string parameter
inline int FXMaterial::FindStringParameterIndex(const MCore::String& name) const
{
	// retrieve the number of string parameters
	const int numParams = mStringParams.GetLength();

	// iterate through the parameters and search the given parameter
	for (int i=0; i<numParams; ++i)
		if (mStringParams[i].mName == name)
			return i;

	return -1;
}


// set the effect file name
inline void FXMaterial::SetEffectFile(const MCore::String& filename)
{
	mEffectFile = filename;
}


// get the effect file name
inline const MCore::String& FXMaterial::GetEffectFile() const
{
	return mEffectFile;
}