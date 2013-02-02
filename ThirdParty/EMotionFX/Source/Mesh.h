/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MESH_H
#define __MESH_H

// include the core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "VertexAttributeLayer.h"
#include "UVVertexAttributeLayer.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declarations
class SubMesh;


/**
 * The mesh base class.
 * Every mesh contains a list of vertex data (positions, normals and uv coordinates) and a set of indices.
 * The indices describe the faces, where every face has 3 indices, which point into the vertex data.
 * The vertex data is stored in seperate arrays. So the position, normal, uv and tangent data is stored in three arrays, which
 * can be accessed directly by the GetPositions(), GetNormals() and GetTangents() methods.
 * The length of all these arrays equals the value returned by GetNumVertices().
 * So this means that there are ALWAYS the same amount of positions, normals and uv coordinates in the mesh.
 * Vertices automatically are duplicated when needed. This means that a flat shaded cube which normally has 8 vertices
 * will have 24 vertices in this mesh. This is done, because you can use this vertex data directly to render it on the
 * graphics hardware and it allows multiple normals and texture coordinates per vertex, which is needed for good 
 * texture mapping support and support for smoothing groups and flat shading.
 * Still, each of the 24 vertices in the 'cube' example will have indices to their original vertices (which are not stored).
 * This array of indices can be accessed by calling GetOrgVerts(). The values stored in this array will range from [0..7].
 * The mesh also contains a set of vertex attributes, which are user defined attributes per vertex.
 * Next to that there also are shared vertex attributes, which are only there for the real number of vertices.
 * All vertices have the same number of attributes, and attribute number 'n' has to be of the same type for all vertices as well.
 * @see SubMesh
 */
class Mesh
{
	DECLARE_CLASS(Mesh);
	MEMORYOBJECTCATEGORY(Mesh, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_MESHES);

	public:
		/**
		 * The memory block ID where mesh data will be allocated in.
		 * Giving mesh data their own memory blocks can reduce the amount of used total blocks when memory gets deleted again.
		 */
		enum { MEMORYBLOCK_ID = 100 };

		/**
		 * Default constructor.
		 */
		Mesh();

		/**
		 * Constructor which allocates mesh data.
		 * @param numVerts The number of vertices to allocate.
		 * @param numFaces The number of faces to allocate.
		 * @param numOrgVerts The number of original vertices.
		 */
		Mesh(const int numVerts, const int numFaces, const int numOrgVerts);

		/**
		 * Destructor.
		 * Automatically releases all data.
		 */
		virtual ~Mesh();

		/**
		 * Calculates the tangent vectors which can be used for per pixel lighting.
		 * These are vectors also known as S and T, used in per pixel lighting techniques, like bumpmapping.
		 * You can calculate the binormal for the given tangent by taking the cross product between the
		 * normal and the tangent for the given vertex. These three vectors (normal, tangent and binormal) are 
		 * used to build a matrix which transforms the lights into tangent space.
		 * You can only call this method after you have passed all other vertex and index data to the mesh.
		 * So after all that has been initialized, call this method to calculate these vectors automatically.
		 * @param The UV texture coordinate layer number. This is not the vertex attribute layer number, but the UV layer number.
		 *        This means that 0 means the first UV set, and 1 would mean the second UV set, etc.
		 *        When the UV layer doesn't exist, this method does nothing.
		 */
		void CalcTangents(const int uvLayer=0);

		/**
		 * Calculates the tangent and binormal for a given triangle.
		 * @param posA The position of the first vertex.
		 * @param posB The position of the second vertex.
		 * @param posC The position of the third vertex.
		 * @param uvA The texture coordinate of the first vertex.
		 * @param uvB The texture coordinate of the second vertex.
		 * @param uvC The texture coordinate of the third vertex.
		 * @param outTangent A pointer to the vector to store the calculated tangent vector.
		 * @param outBiNormal A pointer to the vector to store the calculated binormal vector (calculated using the gradients).
		 */
		void CalcTangentAndBiNormalForFace( const MCore::Vector3& posA, const MCore::Vector3& posB, const MCore::Vector3& posC, 
											const MCore::Vector2& uvA,  const MCore::Vector2& uvB,  const MCore::Vector2& uvC, 
											MCore::Vector3* outTangent, MCore::Vector3* outBiNormal);

		/**
		 * Allocate mesh data. If there is already data allocated, this data will be deleted first.
		 * @param numVerts The number of vertices to allocate.
		 * @param numFaces The number of faces to allocate.
		 */
		virtual void Allocate(const int numVerts, const int numFaces, const int numOrgVerts);

		/**
		 * Releases all allocated mesh data.
		 */
		virtual void ReleaseData();

		/**
		 * Get the number of vertices in the mesh.
		 * The number of positions, normals and uv coordinates is equal to the value returned by this method.
		 * @result The number of vertices.
		 */
		inline int GetNumVertices() const;

		/**
		 * Get a pointer to the final position data.
		 * The number of positions to follow after this address equals GetNumVertices().
		 * @result A pointer to the position data.
		 */
		inline MCore::Vector3* GetPositions();

		/**
		 * Get a pointer to the original position data.
		 * The number of positions to follow after this address equals GetNumVertices().
		 * @result A pointer to the original vertex data.
		 */
		inline MCore::Vector3* GetOrgPositions();

		/**
		 * Get a pointer to the final normals.
		 * The number of normals to follow after this address equals GetNumVertices().
		 * @result A pointer to the normal data.
		 */
		inline MCore::Vector3* GetNormals();

		/**
		 * Get a pointer to the original normals.
		 * The number of normals to follow after this address equals GetNumVertices().
		 * @result A pointer to the original normal data.
		 */
		inline MCore::Vector3* GetOrgNormals();

		/**
		 * Get a pointer to the final uv texture coordinates.
		 * The number of texture coordinates to follow after this address equals GetNumVertices().
		 * @param The uv set number. A value of 0 means the first UV set, a value of 1 means the second UV set, etc.
		 * @result A pointer to the texture coordinate data, or NULL when not found.
		 */
		inline UVVertexAttributeLayer* GetUVLayer(const int uvSet=0);

		/**
		 * Extract the number of UV coordinate layers.
		 * It is recommended NOT to put this function inside a loop, because it is not very fast.
		 * @result The number of UV layers/sets currently present inside this mesh.
		 */
		int CalcNumUVLayers() const;

		/**
		 * Get a pointer to the tangent vectors.
		 * The number of tangent vectors to follow after this address equals GetNumVertices().
		 * @result A pointer to the tangent vector data.
		 */
		inline MCore::Vector4* GetTangents();

		/**
		 * Get a pointer to the original tangent vectors.
		 * The number of tangent vectors to follow after this address equals GetNumVertices().
		 * @result A pointer to the original tangent vector data.
		 */
		inline MCore::Vector4* GetOrgTangents();

		/**
		 * Get a pointer to the array which contains for each vertex an original vertex number.
		 * A normal mesh of a cube has only eight vertices, while this mesh class can hold for example
		 * 24 vertices for this flat shaded cube (because of normals and texture coordinates).
		 * This means that every of the eight REAL vertices have three vertices in this mesh.
		 * Each of these three vertices in this mesh will point to the same vertex number in the REAL mesh.
		 * The array of integers returned by this method will contain these REAL vertex number per vertex in this mesh.
		 * This can be used to access the shared user properties. The influence data used for soft skinning could for
		 * example be only stored for the eight REAL vertices, while each of the 24 vertices in this mesh can then easily
		 * use the influence data, which would be the same for all three duplicated vertices.
		 * @result A pointer to the array of original vertex numbers. The length of the array equals GetNumVertices().
		 */
		inline int* GetOrgVerts();

		/**
		 * Get the number of original vertices. This can be lower compared to the value returned by GetNumVertices().
		 * For the example of the cube with 8 real vertices, but 24 vertices in this mesh, this method would return
		 * a value of 8, while the method GetNumVertices() would return 24.
		 * Please keep in mind that the number of elements in the array returned by GetOrgVerts() however equals
		 * the amount returned by GetNumVertices() and not the amount returned by this method!
		 * @result The number of original vertices in the mesh (which are not stored).
		 */
		inline int GetNumOrgVertices() const;

		/**
		 * Resets all final/output vertex data to its original data, as it was before any deformers where applied.
		 * This will copy the original positions over the final positions, the original normals over the final normals
		 * and the original uv coordinates over the final uv coordinates.
		 */
		void ResetToOriginalData();

		/**
		 * Get a pointer to the face indices.
		 * Every face has 3 indices and all faces indices are stored after eachother.
		 * @result A pointer to the face index data.
		 */
		inline int* GetIndices();

		/**
		 * Set the indices for a given face.
		 * @param faceNr The face number to set the indices for.
		 * @param a The first index.
		 * @param b The second index.
		 * @param c The third index.
		 */
		inline void SetFace(const int faceNr, const int a, const int b, const int c);

		/**
		 * Set the original position for a given vertex.
		 * @param vertexNr The vertex number to set the position for.
		 * @param orgPos The original position.
		 */
		inline void SetOrgVertex(const int vertexNr, const MCore::Vector3& orgPos);

		/**
		 * Get the number of faces in the mesh, which equals GetNumIndices() / 3.
		 * However, the value is precalced, so no divide is being done.
		 * @result The number of faces.
		 */
		inline int GetNumFaces() const;

		/**
		 * Returns the number of indices in the mesh, which equals GetNumFaces() * 3.
		 * @result The number of indices in the mesh.
		 */
		inline int GetNumIndices() const;

		/**
		 * Add a SubMesh to this mesh.
		 * @param subMesh A pointer to the SubMesh to add.
		 */
		inline void AddSubMesh(SubMesh* subMesh);

		/**
		 * Get the number of sub meshes currently in the mesh.
		 * @result The number of sub meshes.
		 */
		inline int GetNumSubMeshes() const;

		/**
		 * Get a given SubMesh.
		 * @param nr The SubMesh number to get.
		 * @result A pointer to the SubMesh.
		 */
		inline SubMesh* GetSubMesh(const int nr) const;

		/**
		 * Remove a given submesh from this mesh.
		 * @param nr The submesh index number to remove, which must be in range of 0..GetNumSubMeshes()-1.
		 * @param delFromMem Set to true when you want to delete the submesh from memory as well, otherwise set to false.
		 */
		void RemoveSubMesh(const int nr, const bool delFromMem=true);

		/**
		 * Insert a submesh into the array of submeshes.
		 * @param insertIndex The position in the submesh array to insert this new submesh.
		 * @param subMesh A pointer to the submesh to insert into this mesh.
		 */
		void InsertSubMesh(const int insertIndex, SubMesh* subMesh);

		/**
		 * Get the shared vertex attribute data of a given layer.
		 * The number of arrays inside the array returned equals the value returned by GetNumOrgVertices().
		 * @param layerNr The layer number to get the attributes from. Must be below the value returned by GetNumSharedVertexAttributeLayers().
		 * @result A pointer to the array of shared vertex attributes. You can typecast this pointer if you know the type of the vertex attributes.
		 */
		VertexAttributeLayer* GetSharedVertexAttributeLayer(const int layerNr);

		/**
		 * Adds a new layer of shared vertex attributes.
		 * The data will automatically be deleted from memory on destruction of the mesh.
		 * @param vertexData The pointer containing the vertex attributes. The array must contain GetNumOrgVertices() elements.
		 */
		void AddSharedVertexAttributeLayer(VertexAttributeLayer* layer);

		/**
		 * Get the number of shared vertex attributes.
		 * This value is the same for all shared vertices.
		 * @result The number of shared vertex attributes for every vertex.
		 */
		int GetNumSharedVertexAttributeLayers() const;

		/**
		 * Find and return the shared vertex attribute layer of a given type.
		 * If you like to find the first layer of the given type, the occurence parameter must be set to a value of 0.
		 * If you like to find the second layer of this type, the occurence parameter must be set to a value of of 1, etc.
		 * This function will return NULL when the layer cannot be found.
		 * @param layerTypeID the layer type ID to search for.
		 * @param occurence The occurence to search for. Set to 0 when you want the first layer of this type, set to 1 if you
		 *                  want the second layer of the given type, etc.
		 * @result The vertex attribute layer index number that you can pass to GetSharedVertexAttributeLayer. A value of -1 os returned
		 *         when no result could be found.
		 */
		int FindSharedVertexAttributeLayerNumber(const int layerTypeID, const int occurrence=0) const;

		/**
		 * Find and return the shared vertex attribute layer of a given type.
		 * If you like to find the first layer of the given type, the occurence parameter must be set to a value of 0.
		 * If you like to find the second layer of this type, the occurence parameter must be set to a value of of 1, etc.
		 * This function will return NULL when the layer cannot be found.
		 * @param layerTypeID the layer type ID to search for.
		 * @param occurence The occurence to search for. Set to 0 when you want the first layer of this type, set to 1 if you
		 *                  want the second layer of the given type, etc.
		 * @result A pointer to the vertex attribute layer, or NULL when none could be found.
		 */
		VertexAttributeLayer* FindSharedVertexAttributeLayer(const int layerTypeID, const int occurence=0) const;

		/**
		 * Removes all shared vertex attributes for all shared vertices.
		 * The previously allocated attributes will be deleted from memory automatically.
		 */
		void RemoveAllSharedVertexAttributeLayers();

		/**
		 * Remove a layer of shared vertex attributes.
		 * Automatically deletes the data from memory.
		 * @param layerNr The layer number to remove, must be below the value returned by GetNumSharedVertexAttributeLayers().
		 */
		void RemoveSharedVertexAttributeLayer(const int layerNr);

		/**
		 * Get the number of vertex attributes.
		 * This value is the same for all vertices.
		 * @result The number of vertex attributes for every vertex.
		 */
		int GetNumVertexAttributeLayers() const;

		/**
		 * Get the vertex attribute data of a given layer.
		 * The number of arrays inside the array returned equals the value returned by GetNumVertices().
		 * @param layerNr The layer number to get the attributes from. Must be below the value returned by GetNumVertexAttributeLayers().
		 * @result A pointer to the array of vertex attributes. You can typecast this pointer if you know the type of the vertex attributes.
		 */
		VertexAttributeLayer* GetVertexAttributeLayer(const int layerNr);

		/**
		 * Adds a new layer of vertex attributes.
		 * The data will automatically be deleted from memory on destruction of the mesh.
		 * @param vertexData The pointer containing the vertex attributes. The array must contain GetNumVertices() elements.
		 */
		void AddVertexAttributeLayer(VertexAttributeLayer* layer);

		/**
		 * Find and return the non-shared vertex attribute layer of a given type.
		 * If you like to find the first layer of the given type, the occurence parameter must be set to a value of 0.
		 * If you like to find the second layer of this type, the occurence parameter must be set to a value of of 1, etc.
		 * This function will return NULL when the layer cannot be found.
		 * @param layerTypeID the layer type ID to search for.
		 * @param occurence The occurence to search for. Set to 0 when you want the first layer of this type, set to 1 if you
		 *                  want the second layer of the given type, etc.
		 * @result The vertex attribute layer index number that you can pass to GetSharedVertexAttributeLayer. A value of -1 os returned
		 *         when no result could be found.
		 */
		int FindVertexAttributeLayerNumber(const int layerTypeID, const int occurrence=0) const;

		/**
		 * Find and return the non-shared vertex attribute layer of a given type.
		 * If you like to find the first layer of the given type, the occurence parameter must be set to a value of 0.
		 * If you like to find the second layer of this type, the occurence parameter must be set to a value of of 1, etc.
		 * This function will return NULL when the layer cannot be found.
		 * @param layerTypeID the layer type ID to search for.
		 * @param occurence The occurence to search for. Set to 0 when you want the first layer of this type, set to 1 if you
		 *                  want the second layer of the given type, etc.
		 * @result A pointer to the vertex attribute layer, or NULL when none could be found.
		 */
		VertexAttributeLayer* FindVertexAttributeLayer(const int layerTypeID, const int occurence=0) const;

		/**
		 * Removes all vertex attributes for all vertices.
		 * The previously allocated attributes will be deleted from memory automatically.
		 */
		void RemoveAllVertexAttributeLayers();

		/**
		 * Remove a layer of vertex attributes.
		 * Automatically deletes the data from memory.
		 * @param layerNr The layer number to remove, must be below the value returned by GetNumVertexAttributeLayers().
		 */
		void RemoveVertexAttributeLayer(const int layerNr);

		/**
		 * Checks if a given ray intersects this mesh.
		 * @param ray The ray to use.
		 * @param node The node to use for this test, this is used to convert the specified ray into space of the node.
		 * @result Returns true when there is an intersection, otherwise false is returned.
		 */
		bool Intersects(const MCore::Ray& ray, Node* node);

		/**
		 * Check if a given ray intersects this mesh. The closest intersection point will be calculated in case of an intersection.
		 * When you do not need to know the intersection point, use the other Intersects method, since that is quite a lot faster.
		 * You can use MCore::BarycentricInterpolate() to calculate for example the normals at the intersection point.
		 * @param ray The ray to use.
		 * @param node The node to use for this test, this is used to convert the specified ray into space of the node.
		 * @param outIntersect A pointer to the vector in which the intersection point will be stored in case of an intersection (NULL allowed).
		 * @param outBaryU A pointer to a float in which the method will store the barycentric U coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outBaryV A pointer to a float in which the method will store the barycentric V coordinate, to be used to interpolate values on the triangle (NULL allowed).
		 * @param outStartIndex A pointer to the integer in which the method will store the offset in the index array returned by GetIndices(), which points to the first vertex
		          of the triangle the intersection occured with. The two values in the index array after the one reported are the other two vertices of the triangle.
		 * @result Returns true in case of an intersection, otherwise false (and in that case outIntersect will be untouched).
		 *
		 * Here follows an example on how to calculate for example the normal at the intersection point:
		 * <pre>
		 * MCore::Vector3 intersectionPoint;		// the intersection point will be stored in here
		 * float   baryU, baryV;				// the barycentric coordinates will be stored in here
		 * int     startIndex;					// the offset in the array with indices will be stored here
		 *
		 * // check if there is an intersection with the mesh
		 * if (mesh->Intersects(someRay, someNode, &intersectionPoint, &baryU, &baryV, &startIndex))
		 * {
		 *     int* indices          = mesh->GetIndices();
		 *     MCore::Vector3 normals = mesh->GetNormals();
		 *     MCore::Vector3 n1      = normals[ indices[startIndex    ] ];    // the normal of triangle vertex 1
		 *     MCore::Vector3 n2      = normals[ indices[startIndex + 1] ];    // the normal of triangle vertex 2
		 *     MCore::Vector3 n3      = normals[ indices[startIndex + 2] ];    // the normal of triangle vertex 3
		 *
		 *     // calculate interpolated normal at the intersection point
		 *     MCore::Vector3 normal = BarycentricInterpolate< MCore::Vector3 >(baryU, baryV, n1, n2, n3);
		 * }
		 * </pre>
		 */
		bool Intersects(const MCore::Ray& ray, Node* node, MCore::Vector3* outIntersect, float *outBaryU, float *outBaryV, int *outStartIndex);

		/**
		 * Gather all bones influencing a specified face into a specified array.
		 * The outBones array will be cleared when it enters the method.
		 * @param startIndexOfFace The start index of the first vertex of the face. So not the vertex number, but the offset in the index array of this mesh.
		 * @param outBones The array to store the pointers to the bones in. Any existing array contents will be cleared when it enters the method.
		 */
		void GatherBonesForFace(const int startIndexOfFace, MCore::Array<Node*>& outBones);

		/**
		 * Calculates the maximum number of bone influences for a given face.
		 * This is calculated by for each vertex checking the number of bone influences, and take the maximum of that amount.
		 * @param startIndexOfFace The start index of the first vertex of the face. So not the vertex number, but the offset in the index array of this mesh.
		 * @result The maximum number of influences of the given face. This will be 0 for non-softskinned objects.
		 */
		int CalcMaxNumInfluencesForFace(const int startIndexOfFace);

		/**
		 * Calculates the maximum number of bone influences.
		 * This is calculated by for each vertex checking the number of bone influences, and take the maximum of that amount.
		 * @result The maximum number of influences. This will be 0 for non-softskinned objects.
		 */
		int CalcMaxNumInfluences();

		/**
		 * Calculates the maximum number of bone influences.
		 * This is calculated by for each vertex checking the number of bone influences, and take the maximum of that amount.
		 * @param vertexCounts Reference to an interger array, which will be filled by this function with vertex counts for specific number of influences.
		 *					   array[0] e.g. holds the number of vertices which have no influences, so non-skinned; array[4] holds the number of vertices
		 *					   which are effected by 4 bones.
		 * @result The maximum number of influences. This will be 0 for non-softskinned objects.
		 */
		int CalcMaxNumInfluences(MCore::Array<int>& vertexCounts);

		/**
		 * Relink both the shared and non-shared attributes to a given mesh that is part of a given node which again is part of a given actor.
		 * It might be that your layer attributes stores data which contains pointers to nodes inside some actor.
		 * Now imagine you want to copy over the vertex attribute layers of a mesh to a new actor and delete the original actor from memory.
		 * That would mean the pointers point to deleted memory. This method can be used to relink the pointers, indices or other data to the new actor and node.
		 * An example situation where this happens is when you make a call to Actor::AddLODModel(...). Here the mesh data is copied over to the actor containing all LOD levels.
		 * After this the original LOD model is deleted from memory.
		 * @param newMesh The new mesh where the vertex attribute layers will be part of.
		 * @param newNode The node where the new mesh belongs to.
		 * @param newActor The actor where the node of the second parameter belongs to.
		 */
		void ReLinkAllVertexAttributeLayers(Mesh* newMesh, Node* newNode, Actor* newActor);


	protected:
		MCore::Array<SubMesh*>		mSubMeshes;		/**< The collection of sub meshes. */
		MCore::Vector3*				mPositions;		/**< The array of vertex positions (size=mNumVertices*2). */
		MCore::Vector3*				mNormals;		/**< The array of normals (size=mNumVertices*2).*/
		MCore::Vector4*				mTangents;		/**< Tangent space vectors to form an orthogonal basis together with normal and binormal. */
		int*						mOrgVerts;		/**< The array of original vertex indices. */
		int*						mIndices;		/**< The array of indices, which define the faces (nrIndices = nrFaces * 3). */
		int							mNumOrgVerts;	/**< The number of original vertices. */
		int							mNumVertices;	/**< Number of vertices. */
		int							mNumIndices;	/**< Number of indices. */
		int							mNumFaces;		/**< The number of faces, precalced value, containing numIndices/3. */

		/**
		 * The array of shared vertex attribute layers.
		 * The number of attributes in each shared layer will be equal to the value returned by Mesh::GetNumOrgVertices().
		 */
		MCore::Array< VertexAttributeLayer* >	mSharedVertexAttributes;

		/**
		 * The array of non-shared vertex attribute layers.
		 * The number of attributes in each shared layer will be equal to the value returned by Mesh::GetNumVertices().
		 */
		MCore::Array< VertexAttributeLayer* >	mVertexAttributes;

/**
 * Added by cogito to extend Mesh Attribute.
 */
	public:
		void SetShdMapName(const char* szName)  {	strcpy(mShdMapName, szName); }
		const char* GetShdMapName()				{	return mShdMapName;	}
	
	protected:
		char mShdMapName[40];		/**< when GetNumVertexAttributeLayers() > 2 */
};

// include inline code
#include "Mesh.inl"

} // namespace EMotionFX

#endif
