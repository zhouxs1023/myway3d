/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SIMPLEMESH_H
#define __SIMPLEMESH_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

/**
 * A simple mesh, constructed out of vertices and triangles and
 * having an axis aligned bounding box and a bounding sphere.
 * No texture coordinates, normals or materials are stored.
 * This can be used for simplified collision detection, where not the real actual
 * mesh of the character is used, but a simplified version, constructed out of 
 * collision meshes, which are represented by this class.
 */
class SimpleMesh
{
	public:
		DECLARE_CLASS(SimpleMesh);
		MEMORYOBJECTCATEGORY(SimpleMesh, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MESHES);

		/**
		 * Constructor, which allocates a given amount of vertices and faces.
		 * @param numVertices The number of vertices to allocate (must be > 0).
		 * @param numFaces The number of faces to allocate (must be > 0).
		 */
		SimpleMesh(const int numVertices, const int numFaces);

		/**
		 * The destructor, cleans up all mesh data.
		 */
		~SimpleMesh();

		/**
		 * Allocate a given amount of vertices and faces.
		 * When there is existing data allocated, this data will be lost.
		 * @param nrVertices The number of vertices to allocate (must be > 0).
		 * @param nrFaces The number of faces to allocate (must be > 0).
		 */
		void Alloc(const int nrVertices, const int nrFaces);

		/**
		 * Release all allocated mesh data. 
		 * This also resets the bounding box and bounding sphere.
		 */
		void Release();

		/**
		 * Check if a given ray intersects this mesh. The closest intersection point will be calculated in case of an intersection.
		 * When you do not need to know the intersection point, use the other Intersects method, since that is quite a lot faster.
		 * If there is an intersection, the intersection point will be stored in worldspace.
		 * @param The node to use the worldspace matrix from.
		 * @param ray The ray to use.
		 * @param outIntersect A pointer to the vector in which the intersection point will be stored in case of an intersection (NULL is NOT allowed).
		 * @result Returns true in case of an intersection, otherwise false (and in that case outIntersect will be untouched).
		 */
		bool Intersects(Node* node, const MCore::Ray& ray, MCore::Vector3 *outIntersect);

		/**
		 * Checks if a given ray intersects this mesh.
		 * @param The node to use the worldspace matrix from.
		 * @param ray The ray to use.
		 * @result Returns true when there is an intersection, otherwise false is returned.
		 */
		bool Intersects(Node* node, const MCore::Ray& ray);

		/**
		 * Get the number of vertices in this mesh.
		 * @result The number of vertices in this mesh.
		 */
		inline int GetNumVertices() const;

		/**
		 * Get the number of faces in this mesh.
		 * @result The number of faces in this mesh.
		 */
		inline int GetNumFaces() const;

		/**
		 * Get the axis aligned bounding box of this mesh.
		 * This is the minimum axis aligned box, containing all vertices and faces of the mesh.
		 * The box is updated every time the Transform method is called. The box is in world space.
		 * @result The bounding box.
		 */
		inline MCore::AABB& GetBoundingBox();

		/**
		 * Get the bounding sphere of this mesh.
		 * This is the minimum bounding sphere, containing all vertices and faces of the mesh.
		 * The sphere is updated every time the Transform method is called. The sphere is in world space.
		 * @result The bounding sphere.
		 */
		inline MCore::BoundingSphere& GetBoundingSphere();

		/**
		 * Get the vertex positions (in object space).
		 * The number of positions stored in this array equals the value returned by GetNumVertices().
		 * @result A pointer to the first vertex position.
		 */
		inline MCore::Vector3* GetPositions();

		/**
		 * Get a pointer to the index data.
		 * Each triangle contains three indices. That makes the length of this array three times the
		 * number of faces. This value is precalculated and can be requested by GetNumIndices().
		 * @result A pointer to the first index value.
		 */
		inline int* GetIndices();

		/**
		 * Returns the number of indices.
		 * @result The number of indices stored in the array of indices which is returned by GetIndices().
		 */
		inline int GetNumIndices() const;


	private:
		MCore::AABB				mBoundingBox;		/**< The bounding box. */
		MCore::BoundingSphere	mBoundingSphere;	/**< The bounding sphere. */
		MCore::Vector3*			mPositions;			/**< Object space vertex positions. */
		int*					mIndices;			/**< The indices (nrFaces * 3). */
		int						mNumVerts;			/**< The number of vertices. */
		int						mNumFaces;			/**< The number of faces. */
		int						mNumIndices;		/**< The number of indices (mNumFaces*3)*/
};

// include inline code
#include "SimpleMesh.inl"

} // namespace EMotionFX

#endif
