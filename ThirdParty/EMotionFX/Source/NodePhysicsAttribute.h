/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODEPHYSICSATTRIBUTE_H
#define __NODEPHYSICSATTRIBUTE_H

// include the Core system
#include "../Core/Source/MCore.h"

// include the node attribute base class
#include "NodeAttribute.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

/**
 * The node physics attribute class.
 * Each node can have a physics attribute attached to it. This physics attribute describes the physical
 * attributes of the node. This includes the mass of the node as well as a parametric collision object description.
 * These physics collision objects can be primitives such as spheres, boxes and cylinders. They are used during
 * the physics simulation of these rigid bodies.
 */
class NodePhysicsAttribute : public NodeAttribute
{
	MEMORYOBJECTCATEGORY(NodePhysicsAttribute, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_NODEATTRIBUTES);

	public:
		// the unique type ID of this attribute
		enum { TYPE_ID = 0x00500002 };

		/**
		 * The physics object base class.
		 * This class represents the physics simulation object.
		 * A physics object is used for the dynamics behaviour as well as collision detection during physics.
		 */
		class PhysicsObject
		{
			public:
				/**
				 * Get the unique type ID of this physics object.
				 * @result The unique type ID of the physics object class.
				 */
				virtual int	GetType() const = 0;

				/**
				 * Get the type string, which can be a description or the class name of the object.
				 * @result A pointer to the string containing the description or class name.
				 */
				virtual const char* GetTypeString() const = 0;

				/**
				 * Clone the physics object.
				 * @result A pointer to the cloned version of the physics object.
				 */
				virtual PhysicsObject* Clone() const = 0;
		};


		/**
		 * The box physics object.
		 */
		class Box : public PhysicsObject
		{
			public:
				// unique ID of the box physics object
				enum { TYPE_ID = 0x99900001 };

				/**
				* Set the width of the collision box.
				* @param width The width of the box.
				*/
				inline void SetWidth(const float width);

				/**
				* Set the height of the collision box.
				* @param height The height of the box.
				*/
				inline void SetHeight(const float height);

				/**
				* Set the depth of the collision box.
				* @param depth The depth of the box.
				*/
				inline void SetDepth(const float depth);

				/**
				* Get the width of the box.
				* @return The width of the box.
				*/
				inline float GetWidth() const;

				/**
				* Get the height of the box.
				* @return The height of the box.
				*/
				inline float GetHeight() const;

				/**
				* Get the depth of the box.
				* @return The depth of the box.
				*/
				inline float GetDepth() const;

				/**
				 * Get the unique type ID of this physics object.
				 * @result The unique type ID of the physics object class.
				 */
				int	GetType() const					{ return NodePhysicsAttribute::Box::TYPE_ID; }

				/**
				 * Get the type string, which can be a description or the class name of the object.
				 * @result A pointer to the string containing the description or class name.
				 */
				const char* GetTypeString() const	{ return "NodePhysicsAttribute::Box"; }

				/**
				 * Clone the physics object.
				 * @result A pointer to the cloned version of the physics object.
				 */
				PhysicsObject* Clone() const		{ NodePhysicsAttribute::Box* obj = new NodePhysicsAttribute::Box;	*obj = *this; return obj; }

			private:
				float	mBoxWidth;			/**< The width of the box. */
				float	mBoxHeight;			/**< The height of the box. */
				float	mBoxDepth;			/**< The depth of the box. */
		};

		/**
		 * The sphere physics object.
		 */
		class Sphere : public PhysicsObject
		{
			public:
				// the unique type ID of the sphere physics object class
				enum { TYPE_ID = 0x99900002 };

				/**
				 * Set the radius of the collision sphere.
				 * @param radius The radius of the sphere.
				 */
				inline void SetRadius(const float radius);

				/**
				 * Get the radius of the sphere.
				 * @return The radius of the sphere.
				 */
				inline float GetRadius() const;

				/**
				 * Get the unique type ID of this physics object.
				 * @result The unique type ID of the physics object class.
				 */
				int	GetType() const					{ return NodePhysicsAttribute::Sphere::TYPE_ID; }

				/**
				 * Get the type string, which can be a description or the class name of the object.
				 * @result A pointer to the string containing the description or class name.
				 */
				const char* GetTypeString() const	{ return "NodePhysicsAttribute::Sphere"; }

				/**
				 * Clone the physics object.
				 * @result A pointer to the cloned version of the physics object.
				 */
				PhysicsObject* Clone() const		{ NodePhysicsAttribute::Sphere* obj = new NodePhysicsAttribute::Sphere; *obj = *this; return obj; }

			private:
				float	mSphereRadius;		/**< The radius of the sphere. */
		};


		/**
		 * The cylinder physics object.
		 */
		class Cylinder : public PhysicsObject
		{
			public:
				// the unique ID of the cylinder physics object class
				enum { TYPE_ID = 0x99900003 };

				/**
				 * Set the radius of the collision cylinder.
				 * @param radius The radius of the cylinder.
				 */
				inline void SetRadius(const float radius);

				/**
				 * Set the height of the collision cylinder.
				 * @param height The height of the cylinder.
				 */
				inline void SetHeight(const float height);

				/**
				 * Get the radius of the cylinder.
				 * @return The radius of the cylinder.
				 */
				inline float GetRadius() const;

				/**
				 * Get the height of the cylinder.
				 * @return The height of the cylinder.
				 */
				inline float GetHeight() const;

				/**
				 * Get the unique type ID of this physics object.
				 * @result The unique type ID of the physics object class.
				 */
				int	GetType() const					{ return NodePhysicsAttribute::Cylinder::TYPE_ID; }

				/**
				 * Get the type string, which can be a description or the class name of the object.
				 * @result A pointer to the string containing the description or class name.
				 */
				const char* GetTypeString() const	{ return "NodePhysicsAttribute::Cylinder"; }

				/**
				 * Clone the physics object.
				 * @result A pointer to the cloned version of the physics object.
				 */
				PhysicsObject* Clone() const		{ NodePhysicsAttribute::Cylinder* obj = new NodePhysicsAttribute::Cylinder; *obj = *this; return obj; }

			private:
				float	mCylinderRadius;	/**< The radius of the cylinder. */
				float	mCylinderHeight;	/**< The height of the cylinder. */
		};


		/**
		 * The constructor.
		 */
		NodePhysicsAttribute() : NodeAttribute()	{ mPhysicsObject=NULL; mMass=0.0f; }

		/**
		 * The destructor.
		 */
		virtual ~NodePhysicsAttribute() {}

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
		const char* GetTypeString() const		{ return "NodePhysicsAttribute"; }

		/**
		 * Clone the node attribute.
		 * @result Returns a pointer to a newly created clone of this node attribute.
		 */
		NodeAttribute* Clone()
		{
			// create the clone
			NodePhysicsAttribute* clone = new NodePhysicsAttribute();

			// copy the attributes
			clone->mMass				= mMass;
			clone->mPhysicsObject		= (mPhysicsObject!=NULL) ? mPhysicsObject->Clone() : NULL;

			// return the clone
			return clone;
		};

		/**
		 * Set the mass of the object.
		 * @param mass The mass to set.
		 */
		inline void SetMass(const float mass);

		/**
		 * Set physics object. Could be a Box, Sphere or a Cylinder.
		 * Type is specified through the collision model.
		 * @param object A Box, Sphere or Cylinder.
		 */
		inline void SetPhysicsObject(PhysicsObject* object);

		/**
		 * Get the object mass.
		 * @return The mass of the object.
		 */
		inline float GetMass() const;

		/**
		 * Return physics object. Could be a Box, Sphere or a Cylinder.
		 * Type is specified through the collision model.
		 * @return A pointer to a Box, Sphere or Cylinder.
		 */
		inline PhysicsObject* GetPhysicsObject() const;

	private:
		float			mMass;				/**< The mass of the object part. */
		PhysicsObject*	mPhysicsObject;		/**< A pointer to the physics object. */
};

// include inline code
#include "NodePhysicsAttribute.inl"

} // namespace EMotionFX


#endif