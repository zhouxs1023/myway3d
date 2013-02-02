/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MATERIAL_H
#define __MATERIAL_H

// include the core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The material base class.
 * All material types are inherited from this base class.
 */
class Material
{
	DECLARE_CLASS(Material);
	MEMORYOBJECTCATEGORY(Material, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MATERIALS);

	public:
		/**
		 * The constructor.
		 * @param name The name of the material.
		 */
		Material(const MCore::String& name)	: mName(name) {}

		/**
		 * The destructor.
		 */
		virtual ~Material() {}

		/**
		 * Get the unique type ID of the material.
		 * Every material class will have a unique ID which can be used to identify the material type.
		 * @result The unique type ID number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Get the material class description or class name.
		 * @result A pointer to the string containing a description or the class name.
		 */
		virtual const char* GetTypeString() const = 0;

		/**
		 * Clone the material.
		 * This returns a duplicated version of this material that is exactly the same.
		 * @result A smartpointer to the new material, which is an exact copy of this material.
		 */
		virtual MCore::Pointer<Material> Clone()  = 0;

		/**
		 * Get the material name.
		 * @result The name of the material.
		 */
		const MCore::String& GetName() const		{ return mName; }

		/**
		 * Set the material name.
		 * @param name The new name of the material.
		 */
		void SetName(const MCore::String& name)		{ mName = name; }


	protected:
		MCore::String	mName;	/**< The material name. */
};


}	// namespace EMotionFX


#endif