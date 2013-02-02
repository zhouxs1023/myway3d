/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SUBMESH_H
#define __SUBMESH_H

// include the Core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "Mesh.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Mesh;


/**
 * The submesh class.
 * A submesh is a part of a mesh, with vertex and triangle data having the same material properties.
 * This allows us to easily render these submeshes on the graphics hardware on an efficient way.
 * The submesh itself do not store any vertex data. All vertex and triangle (indices) are stored
 * in the Mesh class. You can access this parent mesh by the GetParentMesh() method.
 * All vertex and index data of all submeshes are stored in big arrays which contain all data for
 * all submeshes. This prevents small memory allocations and allows very efficient mesh updates.
 * The submeshes contain information about what place in the arrays the data for this submesh is stored.
 * So where the vertex data begins, and how many vertices are following after that. As well as where the
 * index values start in the big array, and how many indices will follow for this submesh.
 * Also there are some methods which gives you easy access to the vertex and index data stored inside
 * the parent mesh, so that you do not have to deal with the offsets returned by GetStartIndex() and
 * GetStartVertex().
 */
class SubMesh
{
	DECLARE_CLASS(SubMesh);
	MEMORYOBJECTCATEGORY(SubMesh, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MESHES);

	public:
		/**
		 * Constructor.
		 * @param parentMesh A pointer to the Mesh to which this submesh belongs to.
		 * @param startVertex The start vertex.
		 * @param startIndex The start index.
		 * @param numVerts Number of vertices which submesh holds.
		 * @param numIndices Number of indices which submesh holds.
		 * @param materialIndex The material.
		 */
		SubMesh(Mesh* parentMesh, const int startVertex, const int startIndex, const int numVerts, const int numIndices, const int materialIndex);

		/**
		 * Destructor.
		 */
		virtual ~SubMesh(); 

		/**
		 * Get the start index. This is the offset in the index array of the parent mesh where the index data for this
		 * submesh starts. So it is not the start vertex or whatsoever. The index array of the parent mesh contains
		 * the index data of all its submeshes. So it is one big array, with all index data of the submeshes sticked after
		 * eachother. The value returned by this method just contains the offset in the array where the index data for this
		 * submesh starts. The number of index values to follow equals the value returned by GetNumIndices().
		 * You can also request a pointer to the first index value of this submesh by using the method GetIndices().
		 * Please keep in mind that the index values stored are absolute and not relative. This means that index values
		 * for every submesh point directly into the array of vertex data from the Mesh where this submesh is a part of.
		 * @result The offset in the array of indices of the parent mesh, where the index data for this submesh starts.
		 */
		inline int GetStartIndex() const;

		/**
		 * Get the start vertex offset. This offset points into the vertex data arrays (positions, normals, uvs) of the
		 * parent mesh. The number of vertices to follow equals the amount returned by GetNumVertices().
		 * @result The offset in the vertex data arrays in the parent mesh, where the vertex data for this submesh starts.
		 */
		inline int GetStartVertex() const;

		/**
		 * Get a pointer to the index data for this submesh.
		 * The number of indices to follow equals the value returned by GetNumIndices().
		 * The index values are stored on an absolute way, so they point directly into the vertex data arrays of the 
		 * Mesh where this submesh belongs to.
		 * @result The pointer to the index data for this submesh.
		 */
		inline int* GetIndices();

		/**
		 * Get a pointer to the final vertex positions for this submesh.
		 * The number of positions to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the final position data for this submesh.
		 */
		inline MCore::Vector3* GetPositions();

		/**
		 * Get a pointer to the original vertex positions for this submesh.
		 * The number of positions to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the original position data for this submesh.
		 */
		inline MCore::Vector3* GetOrgPositions();

		/**
		 * Get a pointer to the final normals for this submesh.
		 * The number of normals to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the final normal data for this submesh.
		 */
		inline MCore::Vector3* GetNormals();

		/**
		 * Get a pointer to the original normals for this submesh.
		 * The number of normals to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the original normal data for this submesh.
		 */
		inline MCore::Vector3* GetOrgNormals();

		/**
		 * Get a pointer to the final UV texture coordinate layer for this submesh.
		 * The number of texture coordinates inside the layer equals the value returned by GetNumVertices().
		 * @param uvSet The UV set number, where 0 is the first UV set, and 1 the second, etc.
		 * @result A pointer to the final UV texture coordinate data for this submesh, or NULL when the data does not exist.
		 */
		//inline UVVertexAttributeLayer* GetUVLayer(const int uvSet=0);

		/**
		 * Get a pointer to the final tangent vectors for this submesh.
		 * The number of tangent vectors to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the final tangent vectors for this submesh.
		 */
		inline MCore::Vector3* GetTangents();

		/**
		 * Get a pointer to the original tangent vectors for this submesh.
		 * The number of tangent vectors to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the original tangent vectors for this submesh.
		 */
		inline MCore::Vector3* GetOrgTangents();

		/**
		 * Get a pointer to the original vertex numbers.
		 * See the description of the Mesh class for more information about what these number
		 * stand for. The number of integers to follow equals the value returned by GetNumVertices().
		 * @result A pointer to the original vertex numbers for this submesh.
		 */
		inline int* GetOrgVerts();

		/**
		 * Return the number of vertices.
		 * @result The number of vertices holded by the submesh.
		 */
		inline int GetNumVertices() const;

		/**
		 * Return the number of indices.
		 * @result The number of indices holded by the submesh.
		 */
		inline int GetNumIndices() const;

		/**
		 * Return parent mesh.
		 * @result A pointer to the parent mesh to which this submesh belongs to.
		 */
		inline Mesh* GetParentMesh() const;

		/**
		 * Set parent mesh.
		 * @param mesh A pointer to the parent mesh to which this submesh belongs to.
		 */
		inline void SetParentMesh(Mesh *mesh);

		/**
		 * Set the material index, which points into the array of materials in the Actor class.
		 * @param The material index.
		 */
		inline void SetMaterial(int materialIndex);

		/**
		 * Return the material index, which points in the array of materials in the Actor class.
		 * @result The material index.
		 */
		inline int GetMaterial() const;


	protected:
		int				mStartVertex;	/**< The start vertex number in the vertex data arrays of the parent mesh. */
		int				mStartIndex;	/**< The start index number in the index array of the parent mesh. */
		int				mNumVertices;	/**< The number of vertices in this submesh. */
		int				mNumIndices;	/**< The number of indices in this submesh. */
		int				mMaterial;		/**< The material index, which points into the materials array in the Node class. */
		Mesh*			mParentMesh;	/**< The parent mesh. */
};

// include inline code
#include "SubMesh.inl"

} // namespace EMotionFX


#endif