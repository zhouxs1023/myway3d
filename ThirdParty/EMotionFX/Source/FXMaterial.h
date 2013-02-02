/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FXMATERIAL_H
#define __FXMATERIAL_H

// include the core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Material.h"


namespace EMotionFX
{

/**
 * An FX material parameter.
 * Each parameter has a unique name and a value.
 */
template <class T>
class FXMaterialParameter
{
	DECLARE_CLASS(FXMaterialParameter<>);
	MEMORYOBJECTCATEGORY(FXMaterial, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MATERIALS);

	friend class FXMaterial;

	public:
		/**
		 * Constructor.
		 * @param name The name of the parameter.
		 * @param value The value of the parameter.
		 */
		FXMaterialParameter(const MCore::String& name, const T& value);

		/**
		 * Get the value of the parameter.
		 * @return The value of the parameter.
		 */
		inline const T& GetValue() const;

		/**
		 * Get the name of the parameter.
		 * @return The name of the parameter.
		 */
		inline const MCore::String& GetName() const;

		/**
		 * Get the name of the parameter in form of a const char pointer.
		 * @return The name of the parameter.
		 */
		inline const char* GetNamePtr() const;

		/**
		 * Set the value of the parameter.
		 * @param value The value of the parameter.
		 */
		inline void SetValue(const T& value);

		/**
		 * Set the name of the parameter.
		 * @param name The new name of the parameter.
		 */
		inline void SetName(const MCore::String& name);

	private:
		MCore::String	mName;		/**< The name of the parameter. */
		T				mValue;		/**< The value of the parameter. */
};



/**
 * The FX material class.
 * This class stores a couple of parameters, which each have a name and a value.
 * The types of parameters stored are: int parameters, float parameters, color parameters and string parameters.
 * The string parameters can be used to represent texture filenames.
 * FX materials can be exported from 3D Studio Max when using the "DirectX Material" of 3D Studio Max 6.
 * All FX parameters and their values can be setup by the artist, and are stored inside this class, which you can 
 * use again to set the values for a given effect file that you are using to render some objects.
 */
class FXMaterial : public Material
{
	DECLARE_CLASS(FXMaterial);
	MEMORYOBJECTCATEGORY(FXMaterial, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MATERIALS);

	public:
		enum { TYPE_ID = 0x00000002 };

		/**
		 * The constructor.
		 * @param name The name of the material.
		 */
		FXMaterial(const MCore::String& name);

		/**
		 * The destructor.
		 */
		virtual ~FXMaterial();

		/**
		 * Get the unique material type ID.
		 * @result The material type identification number.
		 */
		virtual int GetType() const;

		/**
		 * Get the type identification string.
		 * This can be a description or the class name of the material.
		 * @result A pointer to the string containing the name.
		 */
		virtual const char* GetTypeString() const;

		/**
		 * Create a clone of the material.
		 * @result A smart-pointer to the cloned material.
		 */
		virtual MCore::Pointer<Material> Clone();

		/**
		 * Get a given integer parameter by its index.
		 * @param nr The index of the integer parameter to return.
		 * @return The given integer parameter.
		 */
		inline FXMaterialParameter<int>& GetIntParameter(const int nr);

		/**
		 * Get a given float parameter by its index.
		 * @param nr The index of the float parameter to return.
		 * @return The given float parameter.
		 */
		inline FXMaterialParameter<float>& GetFloatParameter(const int nr);

		/**
		 * Get a given color parameter by its index.
		 * @param nr The index of the color parameter to return.
		 * @return The given color parameter.
		 */
		inline FXMaterialParameter<MCore::RGBAColor>& GetColorParameter(const int nr);

		/**
		 * Get a given string parameter by its index.
		 * @param nr The index of the string parameter to return.
		 * @return The given string parameter.
		 */
		inline FXMaterialParameter<MCore::String>& GetStringParameter(const int nr);

		/**
		 * Get the number of float parameters.
		 * @return The number of float parameters.
		 */
		inline int GetNumFloatParameters() const;

		/**
		 * Get the number of integer parameters.
		 * @return The number of integer parameters.
		 */
		inline int GetNumIntParameters() const;

		/**
		 * Get the number of color parameters.
		 * @return The number of color parameters.
		 */
		inline int GetNumColorParameters() const;

		/**
		 * Get the number of string parameters.
		 * @return The number of string parameters.
		 */
		inline int GetNumStringParameters() const;

		/**
		 * Add an integer parameter to the material.
		 * @param name The name of the parameter to add.
		 * @param value The integer value which will be assigned to the new parameter.
		 */
		inline void AddIntParameter(const MCore::String& name, const int value);

		/**
		 * Add an float parameter to the material.
		 * @param name The name of the parameter to add.
		 * @param value The float value which will be assigned to the new parameter.
		 */
		inline void AddFloatParameter(const MCore::String& name, const float value);

		/**
		 * Add an color parameter to the material.
		 * @param name The name of the parameter to add.
		 * @param value The color value which will be assigned to the new parameter.
		 */
		inline void AddColorParameter(const MCore::String& name, const MCore::RGBAColor& value);

		/**
		 * Add an string parameter to the material.
		 * @param name The name of the parameter to add.
		 * @param value The string value which will be assigned to the new parameter.
		 */
		inline void AddStringParameter(const MCore::String& name, const MCore::String& value);

		/**
		 * Remove the given integer parameter by its index.
		 * @param index The index of the integer parameter to remove.
		 */
		inline void RemoveIntParameter(const int index);

		/**
		 * Remove the given float parameter by its index.
		 * @param index The index of the float parameter to remove.
		 */
		inline void RemoveFloatParameter(const int index);

		/**
		 * Remove the given color parameter by its index.
		 * @param index The index of the color parameter to remove.
		 */
		inline void RemoveColorParameter(const int index);

		/**
		 * Remove the given string parameter by its index.
		 * @param index The index of the string parameter to remove.
		 */
		inline void RemoveStringParameter(const int index);

		/**
		 * Set the value of the given integer parameter.
		 * @param index The index of the integer parameter.
		 * @param value The integer value to set.
		 */
		inline void SetIntParameter(const int index, const int value);

		/**
		 * Set the value of the given float parameter.
		 * @param index The index of the float parameter.
		 * @param value The floating point value to set.
		 */
		inline void SetFloatParameter(const int index, const float value);

		/**
		 * Set the value of the given color parameter.
		 * @param index The index of the integer parameter.
		 * @param value The high definition color value to set.
		 */
		inline void SetColorParameter(const int index, const MCore::RGBAColor& value);

		/**
		 * Set the value of the given string parameter.
		 * @param index The index of the string parameter.
		 * @param value The string value to set.
		 */
		inline void SetStringParameter(const int index, const MCore::String& value);

		/**
		 * Set the value of the given integer parameter by name.
		 * @param name The name of the integer parameter.
		 * @param value The integer value to set.
		 * @return Returns true when the parameter value has been set successfully. False is returned when there is no parameter with the specified name.
		 */
		inline bool SetIntParameter(const MCore::String& name, const int value);

		/**
		 * Set the value of the given float parameter by name.
		 * @param name The name of the float parameter.
		 * @param value The floating point value to set.
		 * @return Returns true when the parameter value has been set successfully. False is returned when there is no parameter with the specified name.
		 */
		inline bool SetFloatParameter(const MCore::String& name, const float value);

		/**
		 * Set the value of the given color parameter by name.
		 * @param name The name of the color parameter.
		 * @param value The color value to set.
		 * @return Returns true when the parameter value has been set successfully. False is returned when there is no parameter with the specified name.
		 */
		inline bool SetColorParameter(const MCore::String& name, const MCore::RGBAColor& value);

		/**
		 * Set the value of the given string parameter by name.
		 * @param name The name of the string parameter.
		 * @param value The string value to set.
		 * @return Returns true when the parameter value has been set successfully. False is returned when there is no parameter with the specified name.
		 */
		inline bool SetStringParameter(const MCore::String& name, const MCore::String& value);

		/**
		 * Get the index of the given integer parameter.
		 * @param name The name of the integer parameter to search for.
		 * @return The index of the parameter. -1 if the parameter hasn't been found.
		 */
		inline int FindIntParameterIndex(const MCore::String& name) const;

		/**
		 * Get the index of the given float parameter.
		 * @param name The name of the float parameter to search for.
		 * @return The index of the parameter. -1 if the parameter hasn't been found.
		 */
		inline int FindFloatParameterIndex(const MCore::String& name) const;

		/**
		 * Get the index of the given color parameter.
		 * @param name The name of the color parameter to search for.
		 * @return The index of the parameter. -1 if the parameter hasn't been found.
		 */
		inline int FindColorParameterIndex(const MCore::String& name) const;

		/**
		 * Get the index of the given string parameter.
		 * @param name The name of the string parameter to search for.
		 * @return The index of the parameter. -1 if the parameter hasn't been found.
		 */
		inline int FindStringParameterIndex(const MCore::String& name) const;

		/**
		 * Set the effect file name.
		 * @param filename The filename of the effect file to use.
		 */
		inline void SetEffectFile(const MCore::String& filename);

		/**
		 * Get the effect file name.
		 * @return The filename of the effect file to use.
		 */
		inline const MCore::String& GetEffectFile() const;

	protected:
		MCore::Array< FXMaterialParameter<MCore::String> >		mStringParams;	/**< Parameters of strings to represent texture filenames. */
		MCore::Array< FXMaterialParameter<MCore::RGBAColor> >	mColorParams;	/**< Parameters of high definition colors. */
		MCore::Array< FXMaterialParameter<float> >				mFloatParams;	/**< Parameters of floats. */
		MCore::Array< FXMaterialParameter<int> >				mIntParams;		/**< Parameters of integers. */
		MCore::String											mEffectFile;	/**< The effect filename. */
};

// include inline code
#include "FXMaterial.inl"

} // namespace EMotionFX


#endif