/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODELIMITATTRIBUTE_H
#define __NODELIMITATTRIBUTE_H

// include the core system
#include "../Core/Source/MCore.h"

// include the node attribute base class
#include "NodeAttribute.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The node/joint limit attribute class.
 * This class describes position, rotation and scale limits of a given node.
 * These values can be used during calculations of controllers, such as Inverse Kinematics solvers.
 * However, when a node has limits assigned to them, this does not mean these limits are used.
 * Controllers are not forced to use these limits, so adjusting values might not have any influence if they are
 * not supported by the specific controller.
 */
class NodeLimitAttribute : public NodeAttribute
{
	MEMORYOBJECTCATEGORY(NodeLimitAttribute, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_NODEATTRIBUTES);

	public:
		// the unique type ID of this node attribute
		enum { TYPE_ID = 0x07000002 };
	
		/**
		 * The limit activation flags.
		 * They specify what components are limited.
		 */
		enum ELimitType
		{
			TRANSLATIONX	= 1 << 0,	/**< Position limit on the x axis. */
			TRANSLATIONY	= 1 << 1,	/**< Position limit on the y axis. */
			TRANSLATIONZ	= 1 << 2,	/**< Position limit on the z axis. */
			ROTATIONX		= 1 << 3,	/**< Rotation limit on the x axis. */
			ROTATIONY		= 1 << 4,	/**< Rotation limit on the y axis. */
			ROTATIONZ		= 1 << 5,	/**< Rotation limit on the z axis. */
			SCALEX			= 1 << 6,	/**< Scale limit on the x axis. */
			SCALEY			= 1 << 7,	/**< Scale limit on the y axis. */
			SCALEZ			= 1 << 8	/**< Scale limit on the z axis. */
		};

		/**
		 * The constructor.
		 */
		NodeLimitAttribute() : NodeAttribute() 	{ mLimitFlags=0; mTranslationMax.Set(0,0,0); mRotationMax.Set(0,0,0); mScaleMax.Set(1,1,1); }

		/**
		 * The destructor.
		 */
		virtual ~NodeLimitAttribute() {}

		/**
		 * Get the attribute type.
		 * @result The attribute ID.
		 */
		int	GetType() const						{ return TYPE_ID; }

		/**
		 * Get the attribute type as a string.
		 * This string should contain the name of the class.
		 * @result The string containing the type name.
		 */
		const char* GetTypeString() const		{ return "NodeLimitAttribute"; }

		/**
		 * Clone the node attribute.
		 * @result Returns a pointer to a newly created exact copy of the node attribute.
		 */
		NodeAttribute* Clone()
		{
			NodeLimitAttribute* clone = new NodeLimitAttribute();

			// copy limits
			clone->mTranslationMin	= mTranslationMin;
			clone->mTranslationMax	= mTranslationMax;
			clone->mRotationMin		= mRotationMin;
			clone->mRotationMax		= mRotationMax;
			clone->mScaleMin		= mScaleMin;
			clone->mScaleMax		= mScaleMax;
			clone->mLimitFlags		= mLimitFlags;

			// return the cloned limit
			return clone;
		}

		/**
		 * Set the minimum translation values, and automatically enable the limit to be true.
		 * @param translateMin The minimum translation values.
		 */
		inline void SetTranslationMin(const MCore::Vector3& translateMin);

		/**
		 * Set the maximum translation values, and automatically enable the limit to be true.
		 * @param translateMax The maximum translation values.
		 */
		inline void SetTranslationMax(const MCore::Vector3& translateMax);

		/**
		 * Set the minimum rotation angles, and automatically enable the limit to be true.
		 * The rotation angles must be in radians.
		 * @param rotationMin The minimum rotation angles.
		 */
		inline void SetRotationMin(const MCore::Vector3& rotationMin);

		/**
		 * Set the maximum rotation angles, and automatically enable the limit to be true.
		 * The rotation angles must be in radians.
		 * @param rotationMax The maximum rotation angles.
		 */
		inline void SetRotationMax(const MCore::Vector3& rotationMax);

		/**
		 * Set the minimum scale values, and automatically enable the limit to be true.
		 * @param scaleMin The minimum scale values.
		 */
		inline void SetScaleMin(const MCore::Vector3& scaleMin);

		/**
		 * Set the maximum scale values, and automatically enable the limit to be true.
		 * @param scaleMax The maximum scale values.
		 */
		inline void SetScaleMax(const MCore::Vector3& scaleMax);

		/**
		 * Get the minimum translation values.
		 * @return The minimum translation values.
		 */
		inline const MCore::Vector3& GetTranslationMin() const;

		/**
		 * Get the maximum translation values.
		 * @return The maximum translation values.
		 */
		inline const MCore::Vector3& GetTranslationMax() const;

		/**
		 * Get the minimum rotation values.
		 * The rotation angles are in RADs.
		 * @return The minimum rotation values.
		 */
		inline const MCore::Vector3& GetRotationMin() const;

		/**
		 * Get the maximum rotation values.
		 * The rotation angles are in RADs.
		 * @return The maximum rotation values.
		 */
		inline const MCore::Vector3& GetRotationMax() const;

		/**
		 * Get the minimum scale values.
		 * @return The minimum scale values.
		 */
		inline const MCore::Vector3& GetScaleMin() const;

		/**
		 * Get the maximum scale values.
		 * @return The maximum scale values.
		 */
		inline const MCore::Vector3& GetScaleMax() const;

		/**
		 * Enable or disable the limit for the specified limit type.
		 * @param limitType The limit type to enable or disable.
		 * @param flag True to enable the specified limit, false to disable it.
		 */
		inline void EnableLimit(const ELimitType limitType, const bool flag = true);

		/**
		 * Toggle limit state.
		 * @param limitType The limit type to toggle.
		 */
		inline void ToggleLimit(const ELimitType limitType);

		/**
		 * Determine if the specified limit is enabled or disabled.
		 * @param limitType The limit type to check.
		 * @return True if specified limit is limited, false it not.
		 */
		inline bool IsLimited(const ELimitType limitType) const;


	protected:
		MCore::Vector3		mTranslationMin;	/**< The minimum translation values. */
		MCore::Vector3		mTranslationMax;	/**< The maximum translation values. */
		MCore::Vector3		mRotationMin;		/**< The minimum rotation values. */
		MCore::Vector3		mRotationMax;		/**< The maximum rotation values. */
		MCore::Vector3		mScaleMin;			/**< The minimum scale values. */
		MCore::Vector3		mScaleMax;			/**< The maximum scale values. */
		unsigned short		mLimitFlags;		/**< The limit type activation flags. */
};

// include inline code
#include "NodeLimitAttribute.inl"

} // namespace EMotionFX


#endif