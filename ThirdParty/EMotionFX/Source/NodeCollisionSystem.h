/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODECOLLISIONSYSTEM_H
#define __NODECOLLISIONSYSTEM_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declaration
class Node;

/**
 * The node collision system is the base class of every node collision system.
 * This class provides several intersection functions which could be performed on nodes.
 * Two types of intersection checks are available. Firstly you can force the system to
 * perform the collision tests on the collision meshes of the node. Next to that you can also perform
 * the intersection tests on the real meshes, which will give more accurate results, often at the
 * cost of some speed.
 */
class NodeCollisionSystem
{
	DECLARE_CLASS(NodeCollisionSystem);
	MEMORYOBJECTCATEGORY(NodeCollisionSystem, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_COLLISIONDETECTION);

	public:
		/**
		 * Constructor.
		 * @param node A pointer to the node where this system should work on.
		 */
		NodeCollisionSystem(Node* node);

		/**
		 * Destructor.
		 */
		virtual ~NodeCollisionSystem();

		/**
		 * Returns the type identification number of the collision system class.
		 * @result The type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Checks for an intersection between the collision mesh of this node and a given ray.
		 * If there is no collision mesh attached to this node, no intersection test will be done, and false will be returned.
		 * @param lodLevel The level of detail level to test intersection with.
		 * @param ray The ray to test with.
		 * @result Returns true when an intersection has occured, otherwise false.
		 */
		virtual bool IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray) = 0;

		/**
		 * Check for an intersection between the collision mesh of this node and a given ray, and calculate the closest intersection point.
		 * If there is no collision mesh attached to this node, no intersection test will be done, and false will be returned.
		 * If you do NOT need to know the intersection point, use the other Intersect method, because that one is faster, since it doesn't need to calculate
		 * the closest intersection point.
		 * The intersection point returned is in object space. In order to convert it into world space, do this:
		 * <pre>
		 * normal = node->GetWorldTM().Mul3x3( normal );	// multiply it with the rotation/scale part of the matrix to get it in world space
		 * normal.Normalize();								// normalize it, because the 3x3 part can also contain scaling
		 * </pre>
		 * @param lodLevel The level of detail to perform the intersection test on.
		 * @param ray The ray to test with.
		 * @param outIntersect A pointer to the vector to store the intersection point in, in case of a collision (NULL allowed).
		 * @param outBaryU A pointer to a float in which the method will store the barycentric U coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outBaryV A pointer to a float in which the method will store the barycentric V coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outStartIndex A pointer to the integer in which the method will store the offset in the index array returned by Mesh::GetIndices(), which points to the first vertex
		 *        of the triangle the intersection occured with. The two values in the index array after the one reported are the other two vertices of the triangle (NULL allowed).
		 * @result Returns true when an intersection has occured, otherwise false.
		 */
		virtual bool IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, float* outBaryU, float* outBaryV, int* outStartIndex) = 0;

		/**
		 * Checks for an intersection between the real mesh (if present) of this node and a given ray.
		 * If there is no mesh attached to this node, no intersection test will be performed and false will be returned.
		 * @param lodLevel The level of detail to perform the intersection test on.
		 * @param ray The ray to test with.
		 * @result Returns true when an intersection has occured, otherwise false.
		 */
		virtual bool IntersectsMesh(const int lodLevel, const MCore::Ray& ray) = 0;

		/**
		 * Check for an intersection between the mesh of this node and a given ray, and calculate the closest intersection point.
		 * If there is no mesh attached to this node, no intersection test will be done, and false will be returned.
		 * If you do NOT need to know the intersection point, use the other IntersectAccurate method, because that one is faster, since it doesn't need to calculate
		 * the closest intersection point.
		 * The intersection point returned is in object space. In order to convert it into world space, do this:
		 * <pre>
		 * normal = node->GetWorldTM().Mul3x3( normal );	// multiply it with the rotation/scale part of the matrix to get it in world space
		 * normal.Normalize();								// normalize it, because the 3x3 part can also contain scaling
		 * </pre>
		 * @param lodLevel The level of detail to perform the intersection test on.
		 * @param ray The ray to test with.
		 * @param outIntersect A pointer to the vector to store the intersection point in, in case of a collision (NULL allowed).
		 * @param outBaryU A pointer to a float in which the method will store the barycentric U coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outBaryV A pointer to a float in which the method will store the barycentric V coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outStartIndex A pointer to the integer in which the method will store the offset in the index array returned by Mesh::GetIndices(), which points to the first vertex
		 *        of the triangle the intersection occured with. The two values in the index array after the one reported are the other two vertices of the triangle (NULL allowed).
		 * @result Returns true when an intersection has occured, otherwise false.
		 */
		virtual bool IntersectsMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, float* outBaryU=NULL, float* outBaryV=NULL, int* outStartIndex=NULL) = 0;

		/**
		 * Creates an exact clone of this node collision system.
		 * @param newNode A pointer to the node where this clone will work on.
		 * @result A pointer to the cloned version of this collision system.
		 */
		virtual NodeCollisionSystem* Clone(Node* newNode) const = 0;


	protected:
		Node* mNode;	/**< The node where this collision system works on. */
};

} // namespace EMotionFX


#endif