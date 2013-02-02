/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __ACTORCOLLISIONSYSTEM_H
#define __ACTORCOLLISIONSYSTEM_H

// include the Core system
#include "../Core/Source/MCore.h"

// include the memory categories
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declaration
class Node;
class Actor;

/**
 * The actor collision system is the base class of every actor collision system.
 * This class provides several intersection functions which could be performed on actors.
 * It checks every node which is hold by the actor for intersections. The intersection
 * tests themselves will be called by the node collision systems. Two types of
 * intersection checks are available. Firstly you can force the system to check with any
 * of the collision meshes of all nodes of this actor or you can test it
 * with the real meshes which will be not be as fast as the collision meshes, but more accurate.
 */
class ActorCollisionSystem
{
	DECLARE_CLASS(ActorCollisionSystem);
	MEMORYOBJECTCATEGORY(ActorCollisionSystem, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_COLLISIONDETECTION);

	public:
		/**
		 * Constructor.
		 * @param actor The actor where this system should work on.
		 */
		ActorCollisionSystem(Actor* actor);

		/**
		 * Destructor.
		 */
		virtual ~ActorCollisionSystem();

		/**
		 * Returns the type identification number of the collision system class.
		 * @result The type identification number.
		 */
		virtual int GetType() const = 0;

		/**
		 * Check for an intersection between the collision mesh of this actor and a given ray.
		 * Returns a pointer to the node it detected a collision in case there is a collision with any of the collision meshes of all nodes of this actor.
		 * If there is no collision mesh attached to the nodes, no intersection test will be done, and NULL will be returned.
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to check.
		 * @return A pointer to the node we detected the first intersection with (doesn't have to be the closest), or NULL when no intersection found.
		 */
		virtual Node* IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray) = 0;

		/**
		 * Check for an intersection between the collision mesh of this actor and a given ray, and calculate the closest intersection point.
		 * If there is no collision mesh attached to the nodes, no intersection test will be done, and NULL will be returned.
		 * Returns a pointer to the node it detected a collision in case there is a collision with the collision meshes of the actor, 'outIntersect'
		 * will contain the closest intersection point in case there is a collision. Use the other Intersects method when you do not need the intersection
		 * point (since that one is faster).
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to test with.
		 * @param outIntersect A pointer to the vector to store the intersection point in, in case of a collision (NULL allowed).
		 * @param outNormal A pointer to the vector to store the normal at the intersection point, in case of a collison (NULL allowed).
		 * @param outUV A pointer to the vector to store the uv coordinate at the intersection point (NULL allowed).
		 * @param outBaryU A pointer to a float in which the method will store the barycentric U coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outBaryV A pointer to a float in which the method will store the barycentric V coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outStartIndex A pointer to the integer in which the method will store the offset in the index array returned by Mesh::GetIndices(), which points to the first vertex
		 *        of the triangle the intersection occured with. The two values in the index array after the one reported are the other two vertices of the triangle (NULL allowed).
		 * @return A pointer to the node we detected the closest intersection with, or NULL when no intersection found.
		 */
		virtual Node* IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, MCore::Vector3* outNormal=NULL, MCore::Vector2* outUV=NULL, float* outBaryU=NULL, float* outBaryV=NULL, int* outStartIndex=NULL) = 0;

		/**
		 * Check for an intersection between the real mesh (if present) of this actor and a given ray.
		 * Returns a pointer to the node it detected a collision in case there is a collision with any of the real meshes of all nodes of this actor.
		 * If there is no mesh attached to this node, no intersection test will be performed and NULL will be returned.
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to test with.
		 * @return Returns a pointer to itself when an intersection occured, or NULL when no intersection found.
		 */
		virtual Node* IntersectsMesh(const int lodLevel, const MCore::Ray& ray) = 0;

		/**
		 * Checks for an intersection between the real mesh (if present) of this actor and a given ray.
		 * Returns a pointer to the node it detected a collision in case there is a collision with any of the real meshes of all nodes of this actor,
		 * 'outIntersect' will contain the closest intersection point in case there is a collision.
		 * Both the intersection point and normal which are returned are in world space.
		 * Use the other Intersects method when you do not need the intersection point (since that one is faster).
		 * Both the intersection point and normal which are returned are in world space.
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to test with.
		 * @param outIntersect A pointer to the vector to store the intersection point in, in case of a collision (NULL allowed).
		 * @param outNormal A pointer to the vector to store the normal at the intersection point, in case of a collison (NULL allowed).
		 * @param outUV A pointer to the vector to store the uv coordinate at the intersection point (NULL allowed).
		 * @param outBaryU A pointer to a float in which the method will store the barycentric U coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outBaryV A pointer to a float in which the method will store the barycentric V coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outStartIndex A pointer to the integer in which the method will store the offset in the index array returned by Mesh::GetIndices(), which points to the first vertex
		 *        of the triangle the intersection occured with. The two values in the index array after the one reported are the other two vertices of the triangle (NULL allowed).
		 * @return A pointer to the node we detected the closest intersection with, or NULL when no intersection found.
		 */
		virtual Node* IntersectsMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, MCore::Vector3* outNormal=NULL, MCore::Vector2* outUV=NULL, float* outBaryU=NULL, float* outBaryV=NULL, int* outStartIndex=NULL) = 0;

		/**
		 * Creates an exact clone of this actor collision system.
		 * @param newActor The actor where the cloned version will work on.
		 * @result A pointer to the clone.
		 */
		virtual ActorCollisionSystem* Clone(Actor* newActor) const = 0;


	protected:
		Actor* mActor;	/**< A pointer to the actor where this system will work on. */
};

} // namespace EMotionFX


#endif