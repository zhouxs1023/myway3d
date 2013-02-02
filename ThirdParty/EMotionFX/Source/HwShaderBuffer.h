/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __HWSHADERBUFFER_H
#define __HWSHADERBUFFER_H

// include the Core system
#include "../Core/Source/MCore.h"
#include "MemoryCategories.h"

namespace EMotionFX
{

// forward declarations
class Node;
class Material;
class Mesh;


/**
 * A hardware shader influence.
 * This is part of the hardware buffer generator.
 * An influence holds an index to a bone and a weight.
 */
class HwShaderInfluence
{
	//MEMORYOBJECTCATEGORY(HwShaderInfluence, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_HWSHADERBUFFERS);
	friend class HwShaderBuffer;

	public:
		/**
		 * Constructor.
		 * @param boneNumber The bone number to use for this influence.
		 * @param weight The weight value to use for this influence.
		 */
		inline HwShaderInfluence(const int boneNumber, const float weight);

		/**
		 * Get the bone number for this influence.
		 * @result The bone number, which points into the buffers bone array.
		 */
		inline int GetBoneNumber() const;

		/**
		 * Get the weight for this influence.
		 * @result The weight value.
		 */
		inline float GetWeight() const;


	private:
		int		mBoneNr;	/**< The bone number, which is an index into the HwShaderBuffer bones array. */
		float	mWeight;	/**< The weight, which ranges from [0..1]. */
};



/**
 * A hardware shader vertex.
 * This is part of the hardware buffer generator.
 * The vertex holds a mesh it belongs to, a vertex number in that mesh and a set of influences
 * which are used for soft skinning.
 */
class HwShaderVertex
{
	//MEMORYOBJECTCATEGORY(HwShaderVertex, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_HWSHADERBUFFERS);
	friend class HwShaderBuffer;
	
	public:
		inline HwShaderVertex() {}

		/**
		 * The constructor.
		 * @param mesh The mesh the vertex belongs to.
		 * @param vertexNumber The vertex number in the given mesh.
		 */
		inline HwShaderVertex(Mesh* mesh, const int vertexNumber);

		/**
		 * The destructor.
		 * Automatically cleans up the influences.
		 */
		~HwShaderVertex() { }

		/**
		 * Add an influence to the vertex.
		 * @param influence The influence to add.
		 */
		inline void AddInfluence(const HwShaderInfluence& influence);

		/**
		 * Get the vertex number, which points into the mesh the vertex belongs to.
		 * @result The vertex number.
		 */
		inline int GetVertexNumber() const;

		/**
		 * Get the mesh where the vertex belongs to.
		 * @result A pointer to the mesh.
		 */
		inline Mesh* GetMesh() const;

		/**
		 * Get the number of influences in this vertex.
		 * @result The number of influences.
		 */
		inline int GetNumInfluences() const;

		/**
		 * Get a given influence.
		 * @param nr The influence number.
		 * @result A reference to the influence.
		 */
		inline HwShaderInfluence& GetInfluence(const int nr);


	private:
		MCore::Array<HwShaderInfluence>	mInfluences;	/**< A collection of influences for this vertex. */
		Mesh*							mMesh;			/**< A pointer to the mesh where this vertex belongs to. */
		int								mVertexNr;		/**< The vertex number in the mesh. */
};



/**
 * A hardware shader primitive.
 * This describes a primitive call which should be made to the hardware.
 * It would render a part of a mesh (or the complete mesh).
 * One HwShaderBuffer can hold multiple primitive. On this way multiple primitives can be grouped
 * for efficiency, since we could group multiple smaller vertex and index buffers into a bigger one.
 * When rendering a primitive you probably need to use the world space transformation matrix of the
 * node which you are currently rendering. That is why the primitive also stores a pointer to a node.
 */
class HwShaderPrimitive
{
	//MEMORYOBJECTCATEGORY(HwShaderPrimitive, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_HWSHADERBUFFERS);
	friend class HwShaderBuffer;

	public:
		/**
		 * Constructor.
		 * @param startIndex The start index offset. This is an offset into the index array of the HwShaderBuffer.
		 * @param numTriangles The number of triangles used by this primitive, after the start index.
		 * @param node The node where this primitive is a part of. So it's part of a mesh of the given node.
		 * @param materialNr The material number of the primitive.
		 * @param startVertex The start vertex, which points into the HwShaderBuffer.
		 * @param numVertices The number of vertices used by this primitive, after the start vertex.
		 */
		inline HwShaderPrimitive(const int startIndex, const int numTriangles, Node* node, const int materialNr, const int startVertex, const int numVertices);

		/**
		 * Get the number of triangles to be rendered by this primitive.
		 * @result The number of triangles, which means numTriangles*3 indices.
		 */
		inline int GetNumTriangles() const;

		/**
		 * Get the start index, which is an offset into the index array inside the HwShaderBuffer.
		 * @result The offset into the index array, where the triangle data starts.
		 */
		inline int GetStartIndex() const;

		/**
		 * Get a pointer to the node from which this primitives would render a part of.
		 * @result A pointer to the node.
		 */
		inline Node* GetNode() const;

		/**
		 * Get the material number, which points to the material array inside the actor.
		 * @result The material number. You can use Actor::GetMaterial(lodLevel, matNr) to get the real material information.
		 */
		inline int GetMaterialNr() const;

		/**
		 * Get the start vertex, which is an offset into the array of vertices inside the HwShaderBuffer where this primitive is part of.
		 * Together with GetNumVertices() this describes a range in the buffer's array of vertices, which is used by this primitive.
		 * In Direct3D 8 you can pass this to the DrawIndexedPrimitive as the start index.
		 * @result The start vertex.
		 */
		inline int GetStartVertex() const;

		/**
		 * Get the number of vertices used by this primitive.
		 * Together with GetStartVertex() this describes a range in the buffer's array of vertices, which is used by this primitive.
		 * In Direct3D 8 you can pass this to the DrawIndexedPrimitive as the number of vertices used.
		 * @result The number of vertices used by this primitive.
		 */
		inline int GetNumVertices() const;


	private:
		int			mMaterialNr;	/**< The material number. */
		int			mNumTriangles;	/**< The number of triangles to render. */
		int			mStartIndex;	/**< The start index, which is an offset in the index array of an HwShaderBuffer. */
		int			mStartVertex;	/**< The start vertex, which points into the HwShaderBuffer. */
		int			mNumVertices;	/**< The number of vertices used by this primitive, after the start vertex. */
		Node*		mNode;			/**< A pointer to the node from which this primitives draws a part of. */

		// operators used to perform sorting using MCore::Array::Sort()
		friend bool	operator< (const HwShaderPrimitive& primA, const HwShaderPrimitive& primB)	{ return (primA.GetMaterialNr() < primB.GetMaterialNr()); }
		friend bool	operator==(const HwShaderPrimitive& primA, const HwShaderPrimitive& primB)	{ return (primA.GetMaterialNr() == primB.GetMaterialNr()); }		
};



/**
 * A "hardware shader friendly" buffer.
 * This class can be used when you want to perform softskinning using hardware vertex shaders.
 * Because the data inside EMotion FX isn't stored on a hardware friendly way, it would not be efficient
 * or easy to build support for hardware shaders using EMotion FX.
 * Fortunately for you, we have build a system which provides you with the same data, in a vertex shader friendly way.
 * If you want to perform soft skinning in vertex shaders, you first of all have to sort all mesh faces on material.
 * This is already done by the submeshes inside the mesh class of EMotion FX. However, for hardware without branching
 * support, you will also need to sort the faces on the number of influences (weights). And last but not least, you have
 * only a limited amount of constant registers to feed the bone matrices to. This number can be different on different hardware as well.
 * The GeForce 3 for example has 96 constant registers. Each bone takes 3 constant registers, so theoretically there would fit
 * 96/3=32 bones in a shader, while high detail characters often have over 80 bones! Next to that, in shaders you also have to
 * perform the lighting and projection and other transformations yourself, and you also need constant registers for that.
 * Practically on a GeForce 3, this means you will be able to feed about 15 to 20 bones to each shader at maximum.
 * As you might understand you have to do all kind of work in order to do all this splitting. But fortunately all this work
 * can be done for you, using EMotion FX.
 * EMotion FX will return to you an array of hardware shader friendly buffers (HwShaderBuffer), which you can
 * quite easily convert into your rendering API (D3D, OpenGL, or your own) format.
 * Each buffer contains a set of primitives, bones, vertices and indices and a material number, which is all you need
 * in order to render using vertex/pixel shaders.
 *
 * You can let EMotion FX generate this array of buffers using the method: Actor::GenerateHardwareShaderBuffers().
 * With this method you can specify the maximum number of bones per shader, so you can keep it dynamic and take advantage
 * of higher possible values on newer hardware. Also you can specify the level of detail you want to generate the buffers for.
 * After you have generated the buffers, and converted them to your rendering API format, you must not forget to remove the actual
 * meshes and deformers from all nodes inside the actor. Otherwise all soft skinning will still be done in software as well!
 * You can remove the meshes from the actor nodes with the method Actor::RemoveAllNodeMeshes().
 *
 * @see Actor::GenerateHardwareShaderBuffers
 * @see Actor::RemoveAllNodeMeshes
 */
class HwShaderBuffer
{
	//MEMORYOBJECTCATEGORY(HwShaderBuffer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_GEOMETRY_HWSHADERBUFFERS);

	public:
		/**
		 * The constructor.
		 * @param materialNumber The material number to use for this buffer.
		 * @param numInfluences The number of influences per vertex to use for this buffer.
		 */
		HwShaderBuffer(const int materialNumber, const int numInfluences);

		/**
		 * The destructor. Gets rid of all allocated memory automatically.
		 */
		~HwShaderBuffer();

		/**
		 * Check if a given face can be added to this buffer or not.
		 * @param numInfluences The maximum number of influences used by the face.
		 *						The number of the maximum of the influences of the vertices in the face.
		 *						So, if there is a triangle with vertices having 2, 4 and 3 influences (so vertex #1 has 2 influences,
		 *						vertex #2 has 4 influences and vertex #3 has 3 influences), then numInfluences should be 4.
		 * @param faceBones The array containing the bones used by the face. This array can be calculated using Mesh::GatherBonesForFace().
		 * @param materialNumber The material number used by the given face.
		 * @param maxBonesPerBuffer The maximum number of bones per buffer. Each bone takes 3 vertex shader registers when doing standard vertex shader skinning.
		 * @param allowDifferentWeights Set to true when the buffer accepts any amount of weights per vertex. Whent it is set to false it will
		 *        only allow the same number of weights returned by GetNumInfluences().
		 * @result Returns true when the buffer can handle the face, or false when it cannot handle it.
		 */
		bool CanHandleFace(const int numInfluences, const MCore::Array<Node*>& faceBones, const int materialNumber, const int maxBonesPerBuffer, const bool allowDifferentWeights);

		/**
		 * Adds a face to this buffer.
		 * @param mesh The mesh, where the face is a part of.
		 * @param startIndex The offset in the index array of the specified mesh, where this face starts.
		 */
		void AddFace(Mesh* mesh, const int startIndex);

		/**
		 * Get the number of vertices inside this buffer.
		 * @result The number of vertices.
		 */
		inline int GetNumVertices() const;

		/**
		 * Get the number of indices inside this buffer.
		 * @result The number of indices.
		 */
		inline int GetNumIndices() const;

		/**
		 * Get the number of primitives inside this buffer.
		 * @result The number of primitives.
		 */
		inline int GetNumPrimitives() const;

		/**
		 * Get the number of bones used by this buffer.
		 * @result The number of bones used by this buffer.
		 */
		inline int GetNumBones() const;

		/**
		 * Get the material number used by this HwShaderBuffer.
		 * You can get access to the real material by using Actor::GetMaterial(lodLevel, materialNr).
		 * @result The material number, which points into the Actor you are generating the buffers for.
		 */
		//inline int GetMaterialNumber() const;

		/**
		 * Get the number of influences used by this buffer.
		 * This value stands for the number of influences for every vertex inside this HwShaderBuffer.
		 * @result The number of influences used by every vertex inside this buffer.
		 */
		inline int GetNumInfluences() const;

		/**
		 * Starts a primitive. This is part of the automatic primitive generation.
		 * You first call StartPrimitive, then add vertices and indices and bones, and simply call
		 * EndPrimitive(). When you have added vertices and indices, a new primitive will be added to 
		 * the primitive array inside this HwShaderBuffer.
		 * @param node The node you are currently generating primitives for.
		 * @param matNr The material number for the primitive.
		 */
		inline void StartPrimitive(Node* node, const int matNr);

		/**
		 * Ends a primitive. This is part of the automatic primitive generation.
		 * You first call StartPrimitive, then add vertices and indices and bones, and simply call
		 * this method. When you have added vertices and indices, a new primitive will be added to 
		 * the primitive array inside this HwShaderBuffer.
		 */
		inline void EndPrimitive();

		/**
		 * Get a given vertex.
		 * @param nr The vertex number to get.
		 * @result A reference to the vertex.
		 */
		inline HwShaderVertex& GetVertex(const int vtxNr);

		/**
		 * Get a given index value, which contains a vertex number inside the vertices array of this HwShaderBuffer.
		 * Indices are used to form the triangles. They specify between which vertices to render a triangle. Every triangle
		 * has three indices, which contain the three vertex numbers to draw a triangle between.
		 * @param indexNr The index number.
		 * @result The vertex number at this index.
		 */
		inline int GetIndex(const int indexNr) const;

		/**
		 * Get a given primitive from the buffer.
		 * @param primNr The primitive number to get.
		 * @result A reference to the primitive.
		 */
		inline const HwShaderPrimitive& GetPrimitive(const int primNr) const;

		/**
		 * Get a given bone from the buffer.
		 * @param nr The bone number to get.
		 * @result A pointer to the bone.
		 */
		inline Node* GetBone(const int nr) const;

		/**
		 * Attach given buffer to this buffer.
		 * @param buffer The buffer to attach.
		 */
		void Attach(const HwShaderBuffer& buffer);

		/**
		 * Optimizes the primitives inside the buffer.
		 * This is done by first sorting the primitives on material, and then
		 * grouping the primitives with the same material together into one single primitive.
		 */		 
		void OptimizePrimitives();


	private:
		MCore::Array<HwShaderVertex>		mVertices;			/**< The array of vertices inside this buffer. */
		MCore::Array<HwShaderPrimitive>		mPrimitives;		/**< The primitives of this buffer. */
		MCore::Array<Node*>					mBones;				/**< The bones used in this buffer. */
		MCore::Array<int>					mIndices;			/**< The indices used inside this buffer. */
		int									mMaterialNumber;	/**< The material number, which points inside the actor. */
		int									mNumInfluences;		/**< The number of influences used by this buffer. */
		int									mStartIndex;		/**< The start index. Used for automatic primitive generation. */
		int									mStartVertex;		/**< The start vertex. Used for automatic primitive generation. */
		int									mStartMatNr;		/**< The start material. Used for automatic primitive generation. */
		Node*								mStartNode;			/**< The start node. Used for automatic primitive generation. */

		// operators used to perform sorting using MCore::Array::Sort()
		friend bool	operator< (const HwShaderBuffer& bufA, const HwShaderBuffer& bufB)	{ return (bufA.GetNumInfluences() < bufB.GetNumInfluences()); }
		friend bool	operator==(const HwShaderBuffer& bufA, const HwShaderBuffer& bufB)	{ return (bufA.GetNumInfluences() == bufB.GetNumInfluences()); }		
};

// include inline code
#include "HwShaderBuffer.inl"

} // namespace EMotionFX

#endif