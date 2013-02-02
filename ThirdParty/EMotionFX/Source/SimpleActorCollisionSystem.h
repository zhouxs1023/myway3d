/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SIMPLEACTORCOLLISIONSYSTEM_H
#define __SIMPLEACTORCOLLISIONSYSTEM_H

#include "ActorCollisionSystem.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * Simple actor collision system is inherited from the collision system base-class.
 * This class provides several intersection functions which could be performed on actors.
 * It checks every node which is holded by the actor for intersections. The intersection
 * tests themselves will be called by the node collision systems. Two types of
 * intersection checks are available. Firstly you can force the system to check with any
 * of the collision meshes of all nodes of this actor or you can test it
 * with the real meshes which will be not that fast but more exact.
 */
class SimpleActorCollisionSystem : public ActorCollisionSystem
{
	DECLARE_CLASS(SimpleActorCollisionSystem);
	MEMORYOBJECTCATEGORY(SimpleActorCollisionSystem, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_COLLISIONDETECTION);

	public:
		// the type ID of this class, returned by GetType
		enum { TYPE_ID = 0x00000001 };

		/**
		 * Default constructor.
		 * @param actor The actor where this system should work on.
		 */
		SimpleActorCollisionSystem(Actor* actor);

		/**
		 * Destructor.
		 */
		~SimpleActorCollisionSystem();

		/**
		 * Check for an intersection between the collision mesh of this actor and a given ray.
		 * Returns a pointer to the node it detected a collision in case there is a collision with any of the collision meshes of all nodes of this actor.
		 * If there is no collision mesh attached to the nodes, no intersection test will be done, and NULL will be returned.
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to check.
		 * @return A pointer to the node we detected the first intersection with (doesn't have to be the closest), or NULL when no intersection found.
		 */
		Node* IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray);

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
		Node* IntersectsCollisionMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, MCore::Vector3* outNormal=NULL, MCore::Vector2* outUV=NULL, float* outBaryU=NULL, float* outBaryV=NULL, int* outStartIndex=NULL);

		/**
		 * Check for an intersection between the real mesh (if present) of this actor and a given ray.
		 * Returns a pointer to the node it detected a collision in case there is a collision with any of the real meshes of all nodes of this actor.
		 * If there is no mesh attached to this node, no intersection test will be performed and NULL will be returned.
		 * @param lodLevel The level of detail to check the collision with.
		 * @param ray The ray to test with.
		 * @return Returns a pointer to itself when an intersection occured, or NULL when no intersection found.
		 */
		Node* IntersectsMesh(const int lodLevel, const MCore::Ray& ray);

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
		Node* IntersectsMesh(const int lodLevel, const MCore::Ray& ray, MCore::Vector3* outIntersect, MCore::Vector3* outNormal=NULL, MCore::Vector2* outUV=NULL, float* outBaryU=NULL, float* outBaryV=NULL, int* outStartIndex=NULL);

		/**
		 * Creates an exact clone of this actor collision system.
		 * @param newActor The actor where the cloned version will work on.
		 * @return A pointer to the clone.
		 */
		ActorCollisionSystem* Clone(Actor* newActor) const;

		/**
		 * Returns the type identification number of the collision system class.
		 * @result The type identification number.
		 */
		int GetType() const;
};

} // namespace EMotionFX


#endif