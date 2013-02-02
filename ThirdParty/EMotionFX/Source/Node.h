/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __NODE_H
#define __NODE_H

// include the core system
#include "../Core/Source/MCore.h"

// include EMotion FX related headers
#include "NodeTransform.h"
#include "MotionLink.h"
#include "MeshDeformerStack.h"
#include "NodeAttribute.h"
#include "NodeCollisionSystem.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declaration
class SimpleMesh;
class MotionInstance;
class Mesh;
class Material;


/**
 * The node class.
 * A node in EMotion FX is a connection point in the hierarchy of the character.
 * Each node can have one parent, but multiple child nodes.
 * Every node has a transformation describing the orientation of the node, so where it is positioned
 * and how it is scaled and rotated in 3D space.
 * Examples of nodes could be a bone, like an upper arm or lower arm, a finger, a head, a gun, a cigarette, a camera, a light, etc.
 * So basically it can be everything, which can be animated (but it doesn't have to be animated of course).
 */
class Node
{
	DECLARE_CLASS(Node);
	MEMORYOBJECTCATEGORY(Node, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_NODES);

	friend class MotionLayer;
	friend class Actor;

	public:
		/// The flags (options) to be used when cloning a node.
		enum ENodeCloneFlags
		{
			CLONE_ATTRIBUTES			= 0x00000001,		/**< Clone (duplicate) the node attributes. If not set, the node attributes will NOT be copied (and not shared either). */
			CLONE_NODESTACKS			= 0x00000100,		/**< Clone (duplicate) the node stacks from a given node. */
			CLONE_NODECOLLISIONSTACKS	= 0x00001000,		/**< Clone (duplicate) the collision node stacks from a given node. */
			NODE_CLONE_DEFAULT			= CLONE_ATTRIBUTES,	/**< The default node cloning flags, which only copies the node attributes. The rest will be shared. */
			NODE_CLONE_ALL				= (CLONE_ATTRIBUTES | CLONE_NODESTACKS | CLONE_NODECOLLISIONSTACKS) /**< The node cloning flags to clone all attributes available. So nothing will be shared. */
		};

		/**
		 * Constructor.
		 * @param name The name of the node.
		 */
		Node(const MCore::String& name);

		/**
		 * The destructor.
		 * Beware, when destructing this node, the child nodes of this node point to an already destructed Node!
		 * So be sure to set adjust the parent pointers of the child nodes before deleting this node.
		 */
		virtual ~Node();

		/**
		 * Recursively update the hierarchy below this node.
		 * With this we mean that forward kinematic calculations will be applied in order to transform all child nodes into worldspace, instead of only in 
		 * space relative to their parent. So here, the worldspace transformation (position, rotation and scale) will be calculated, for all childs of this node (recursively).
		 * This means that calling this method for the root node, will calculate the worldspace transformations for all nodes in the hierarchy.
		 * The NodeTM, NodeRot and NodeScale will be updated.
		 */
		void RecursiveHierarchyUpdate();

		/**
		* Recursively update the world space transformation matrix of the nodes and underlaying hierarchy.
		* The method can be used in two modes. The first mode is where the specified parameter is a NULL pointer.
		* When that is the case, the world transformation matrices will be calculated from it's parent, by performing
		* the calculation: mWorldTM = mLocalTM * mParent->GetWorldTM();
		* The other mode is where worldTM is not NULL. In this case, the worldTM of the current node will be set to
		* this matrix, and all childs will be recursively updated like in the first method.
		* You will use this method when you want to update the world space matrix of a node, and you want the complete
		* hierarchy below to adjust to this change as well. The only thing is that the localTM's and localRot and localPos
		* will all be invalid from this node and all childs of this node after calling this method with a NON-NULL pointer.
		* This method will mostly be used by POST controllers, which are executed after the hierarchy has been updated, so which
		* will most likely work by adjusting the world space matrices of nodes.
		* @param worldTM The pointer to the matrix which should be used as world space matrix, or NULL when you only want to update
		*        the world space matrices in the hierarchy, where starting at this node.
		*/
		void RecursiveUpdateWorldTM(const MCore::Matrix* worldTM = NULL);

		/**
		 * Update the local transformation matrix (LocalTM).
		 * This is done by converting the LocalPos, LocalRot and LocalScale into a matrix, named LocalTM.
		 * The LocalTM is needed for the recursive hierarchy update. So this means that if you change the local rotation quaternion (LocalRot)
		 * of this node, and you want to update the local transformation matrix (LocalTM) you need to call this method before using the LocalTM in other calculations.
		 * If you do not do this, the LocalTM will not yet contain your new rotation, but will still contain the old one.
		 * The method RecursiveHierarchyUpdate() automatically updates the LocalTM matrices, so you do not have to do it yourself, unless you do some fancy stuff.
		 */
		void UpdateLocalTM();

		/**
		 * Set a new collision system to this node. It deletes the old one, if there
		 * was one availible.
		 * @param collisionSystem The new collision system.
		 */
		void SetNodeCollisionSystem(NodeCollisionSystem* collisionSystem);

		/**
		 * Return the collision system of this node.
		 * @return A pointer to the collision system, NULL if it doesn't exist.
		 */
		inline NodeCollisionSystem* GetNodeCollisionSystem() const;

		//------------------------------------------------

		/**
		 * Creates an exact clone of this node, but without it's child and parent links and also without any motion links.
		 * @param actor The actor where the cloned node would belong to.
		 * @param flags The cloning options.
		 */
		virtual Node* Clone(Actor* actor, ENodeCloneFlags flags = NODE_CLONE_DEFAULT) const;

		/**
		 * Clones the node stacks from a specified source node.
		 * So all node stacks from the source node will be duplicated and stored inside this node.
		 * @param sourceNode The node to clone the stacks from.
		 * @param actor The actor where THIS node belongs to (so NOT the actor where the sourceNode belongs to).
		 */
		virtual void CloneNodeStacksFromNode(Node* sourceNode, Actor* actor);

		/**
		 * Clones the collision node stacks from a specified source node.
		 * So all node stacks from the source node will be duplicated and stored inside this node.
		 * @param sourceNode The node to clone the stacks from.
		 * @param actor The actor where THIS node belongs to (so NOT the actor where the sourceNode belongs to).
		 */
		virtual void CloneNodeCollisionStacksFromNode(Node* sourceNode, Actor* actor);

		/**
		 * Set the local transformation matrix.
		 * This matrix describes the transformation of the node relative to its parent.
		 * @param mat The matrix to use as LocalTM.
		 */
		inline void SetLocalTM(const MCore::Matrix& mat);

		/**
		 * Set the world space transformation matrix.
		 * This matrix however will automatically be calculated by the method RecursiveHierarchyUpdate().
		 * But in case you want to do your own transformations, you can still adjust the matrix.
		 * @param mat The matrix to use as world space matrix.
		 */
		inline void SetWorldTM(const MCore::Matrix& mat);

		/**
		 * Set the local rotation (relative to its parent).
		 * @param q The quaternion describing the rotation.
		 */
		inline void SetLocalRot(const MCore::Quaternion& q);

		/**
		 * Set the parent node.
		 * @param parent The parent node to link to (NULL will unlink the node, which makes it a root node).
		 */
		inline void SetParent(Node *parent);

		/**
		 * Set the local position (relative to its parent node).
		 * @param pos The position.
		 */
		inline void SetLocalPos(const MCore::Vector3& pos);

		/**
		 * Set the local scale (relative to its parent node).
		 * @param scale The scale vector (scale factor for each axis).
		 */
		inline void SetLocalScale(const MCore::Vector3& scale);

		/**
		 * Set the world space scale factor (for each axis).
		 * Beware, this will also be called by the method RecursiveHierarchyUpdate().
		 * @param scale The scale factor (for each axis).
		 */
		inline void SetWorldScale(const MCore::Vector3& scale);

		/**
		 * Set the inverse bone transformation matrix as it apears in the base pose.
		 * This is used in soft skinning, when this node acts like a bone in the skinned mesh.
		 * In order to deform the vertices, the world space vertex positions are multiplied with the 
		 * inverse worldspace transformation matrix and multiplied by the worldspace bone matrix at the current time 
		 * multiplied by the weight value for the vertex->bone link.
		 * So the skinning of a vertex goes like this (very unoptimized):
		 * <pre>
		 * MCore::Vector3 finalPos(0,0,0);
		 * for (int i=0; i<numBonesLinkedToThisVertex; i++)
		 * {
		 *     Node*       bone    = links[i].mBone;			// get the current bone
		 *     float          weight  = links[i].mWeight;		// get the current weight
		 *     MCore::Matrix   skinMat = bone->GetInverseBoneTM() * bone->GetWorldTM();	// calc the matrix which describes the transformation relative to the base pose
		 *
		 *     finalPos += (skinMat * worldspaceVertexPos) * weight;	// apply the relative transformation to the vertex, with the given weight
		 * }
		 * </pre>
		 * After this calculation, finalPos contains the skinned vertex position.
		 */
		inline void SetInvBoneTM(const MCore::Matrix& mat);

		/**
		 * Set the worldspace position.
		 * Beware, this will also be called by the method RecursiveHierarchyUpdate().
		 * @param pos The position.
		 */
		inline void SetWorldPos(const MCore::Vector3& pos);

		/**
		 * Set the original base position (relative to its parent), so the original LocalPos.
		 * @param pos The original base position.
		 */
		inline void SetOrgPos(const MCore::Vector3& pos);

		/**
		 * Set the original base rotation (relative to its parent), so the original LocalRot.
		 * @param rot The original base rotation.
		 */
		inline void SetOrgRot(const MCore::Quaternion& rot);

		/**
		 * Set the original base scale (relative to its parent), so the original LocalRot.
		 * @param scale The original base scale.
		 */
		inline void SetOrgScale(const MCore::Vector3& scale);

		/**
		 * Set a given motion link.
		 * This will link our node with the MotionPart, so that we know what part of the animation effects this node.
		 * The amount of motion links is equal to the amount of motions applied to this node.
		 * @param nr The motion link number.
		 * @param link The motion link.
		 */
		inline void SetMotionLink(const int nr, const MotionLink& link);

		/**
		 * Set the mesh of the node.
		 * @param mesh The pointer to the mesh object for this node.
		 * @param lodLevel The level of detail index of the mesh, where 0 is the full detail mesh, 1 the next lower lod, etc.
		 */
		inline void SetMesh(const MCore::Pointer<Mesh>& mesh, const int lodLevel);

		/**
		 * Returns the mesh of the node, if it has one.
		 * @param lodLevel The level of detail of the mesh to get, where 0 is the highest detail, 1 the next lower detail, etc.
		 * @result A pointer to the mesh, which can also be NULL.
		 */
		inline MCore::Pointer<Mesh>& GetMesh(const int lodLevel);

		/**
		 * Set the collision mesh of the node.
		 * @param mesh The pointer to the collision mesh object for this node.
		 * @param lodLevel The level of detail index of the mesh, where 0 is the full detail mesh, 1 the next lower lod, etc.
		 */
		inline void SetCollisionMesh(const MCore::Pointer<Mesh>& mesh, const int lodLevel);

		/**
		 * Returns the collision mesh of the node, if it has one.
		 * @param lodLevel The level of detail of the mesh to get, where 0 is the highest detail, 1 the next lower detail, etc.
		 * @result A pointer to the mesh, which can also be NULL.
		 */
		inline MCore::Pointer<Mesh>& GetCollisionMesh(const int lodLevel);

		/**
		 * Gets the name of the node.
		 * @result The node name.
		 */
		inline const MCore::String& GetName() const;

		/**
		 * Gets the name of the node in form of a C/C++ character buffer.
		 * @result A pointer to the null terminated buffer, containing the name of the node.
		 */
		inline const char* GetNamePtr() const;

		/**
		 * Get the unique ID of this node, which has been generated based on the name.
		 * You are guaranteed that nodes having the same name have the same ID.
		 * This means you can do fast compares between nodes by using the ID instead of a comparison
		 * between the node names.
		 * However, ever time you run the application, the ID values for given names can be different.
		 * So if you run your program, the node with name "Arm" might have an ID of 10. While next time you
		 * run the program this node might have an ID of for example 36.
		 * Still it is guaranteed that every time you run your program all nodes with the name "Arm" will have the
		 * same ID number.
		 * @result The node ID number, which can be used for fast compares between nodes.
		 */
		inline int GetID() const;

		/**
		 * Get the number of child nodes attached to this node.
		 * @result The number of child nodes.
		 */
		inline int GetNumChilds() const;

		/**
		 * Get the number of child nodes including the childreb of the childreb.
		 * This function traverses recursively through all child nodes getting the total number of child nodes.
		 * @return The total number of child nodes including the children of the children.
		 */
		int GetNumChildNodesRecursive();

		/**
		 * Get a pointer to the parent node.
		 * @result A pointer to the parent node. When NULL is returned, this node has no parent.
		 */
		inline Node* GetParent() const;

		/**
		 * Get a given child.
		 * @param nr The child number.
		 * @result A pointer to the given child.
		 */
		inline Node* GetChild(const int nr) const;

		/**
		 * Checks if the given node is a child of this node.
		 * @param node The node to check wheter it is a child or not.
		 * @param compareNames If enabled the function will search the child node
		 *					   by names, if not it compares the node pointers.
		 * @return True if the given node is a child, false if not.
		 */
		bool IsNodeChild(Node* node, const bool compareNames = false);

		/**
		 * Get the local transformation matrix (transformation relative to its parent).
		 * @result The local transformation matrix.
		 */
		inline const MCore::Matrix& GetLocalTM() const;

		/**
		 * Get the world space transformation matrix.
		 * @result The world space transformation matrix.
		 */
		inline const MCore::Matrix& GetWorldTM() const;

		/**
		 * Get the local transformation matrix (transformation relative to its parent).
		 * This includes write access to the matrix.
		 * @result The local transformation matrix.
		 */
		inline MCore::Matrix& GetLocalTM();

		/**
		 * Get the world space transformation matrix.
		 * This includes write access to the matrix.
		 * @result The world space transformation matrix.
		 */
		inline MCore::Matrix& GetWorldTM();

		/**
		 * Get the inverse bone transformation matrix.
		 * @result The inverse of the bone transformation matrix in worldspace, in the base pose.
		 * @see SetInvBoneTM()
		 */
		inline const MCore::Matrix& GetInvBoneTM() const;

		/**
		 * Get the local rotation in form of a quaternion (relative to its parent).
		 * @result The quaternion holding the local rotation.
		 */
		inline const MCore::Quaternion& GetLocalRot() const;

		/**
		 * Calculates the world space rotation in form of a quaternion.
		 * This only works correctly if the node is not scaled! (so if the scale is 1,1,1).
		 * The method simply performs the following code: return Quaternion::FromMatrix( mWorldTM );
		 * @result The world space rotation, in form of a quaternion.
		 */
		inline const MCore::Quaternion CalcWorldRot() const;

		/**
		 * Get the local position (relative to its parent).
		 * @result The local position.
		 */
		inline const MCore::Vector3& GetLocalPos() const;

		/**
		 * Get the world space position.
		 * @result The world space position.
		 */
		inline MCore::Vector3 GetWorldPos() const;

		/**
		 * Get the local scale factor (relative to its parent).
		 * @result The local scale factor.
		 */
		inline const MCore::Vector3& GetLocalScale() const;

		/**
		 * Get the world space scale factor.
		 * @result The world space scale factor.
		 */
		inline const MCore::Vector3& GetWorldScale() const;

		/**
		 * Get the numbef of motion links.
		 * This value describes how many motions currently influence this node.
		 * @result The number of motion links.
		 */
		inline int GetNumMotionLinks() const;

		/**
		 * Get the original position (the position in the base pose, relative to its parent).
		 * @result The position in the base pose, in local space (relative to its parent).
		 */
		inline const MCore::Vector3& GetOrgPos() const;

		/**
		 * Get the original rotation (the rotation in the base pose, relative to its parent).
		 * @result The rotation in the base pose, in local space (relative to its parent).
		 */
		inline const MCore::Quaternion& GetOrgRot() const;

		/**
		 * Get the original scale (the scale in the base pose, relative to its parent).
		 * @result The scale in the base pose, in local space (relative to its parent).
		 */
		inline const MCore::Vector3& GetOrgScale() const;

		/**
		 * Get a given motion link, which links this node with a part in a specific motion.
		 * @param nr The motion link number.
		 * @result The motion link.
		 */
		inline MotionLink& GetMotionLink(const int nr);
		
		/**
		 * Get the mesh deformer stack (can return NULL).
		 * @param lodLevel The LOD level to get the stack from, where 0 is the full detail model.
		 * @result A pointer to the mesh deformer stack (can return NULL);
		 */
		inline MCore::Pointer<MeshDeformerStack>& GetMeshDeformerStack(const int lodLevel);

		/**
		 * Get the collision mesh deformer stack (can return NULL).
		 * @param lodLevel The LOD level to get the stack from, where 0 is the full detail model.
		 * @result A pointer to the collision mesh deformer stack (can return NULL);
		 */
		inline MCore::Pointer<MeshDeformerStack>& GetCollisionMeshDeformerStack(const int lodLevel);

		/**
		 * Set the mesh deformer stack.
		 * @param lodLevel The LOD level to set the stack for, where 0 is the full detail model.
		 * @param stack A pointer to the mesh deformer stack.
		 */
		inline void SetMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack, const int lodLevel);

		/**
		 * Set the collision mesh deformer stack.
		 * @param lodLevel The LOD level to set the stack for, where 0 is the full detail model.
		 * @param stack A pointer to the collision mesh deformer stack.
		 */
		inline void SetCollisionMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack, const int lodLevel);

		/**
		 * Add a motion link to this node.
		 * @param link The motion link to add.
		 */
		inline void AddMotionLink(const MotionLink& link);

		/**
		 * Add a child to this node.
		 * @param newChild The child node to add.
		 */
		inline void AddChild(Node* newChild);

		/**
		 * Removes a given child (does not delete it from memory though).
		 * Beware, the child node will still point to this node after executing this method!
		 * So you have to adjust the parent pointer of the child node manually.
		 * @param child The child to remove.
		 */
		inline void RemoveChild(Node* child);

		/**
		 * Removes all child nodes (not from memory though, just clears the childs pointers in this node).
		 * Beware, the child nodes will still point to this node after executing this method!
		 * So you have to adjust these pointers manually.
		 */
		inline void RemoveAllChilds();

		/**
		 * Check if this node is a root node (if it has no parent).
		 * @result Returns true when this node is a root node, otherwise false is returned.
		 */
		inline bool IsRootNode() const;

		/**
		 * Checks if the collision mesh for the given LOD is deformable or not.
		 * A node is deformable when it has a mesh AND it has a mesh deformer stack AND it has deformers on the stack.
		 * So in other words, a node is deformable when the mesh of the node changes over time (like the vertex positions being deformed).
		 * @param lodLevel The level of detail to perform the check on. It is possible that a lower lod is not dynamic, while the highest detail is.
		 * @result Returns true when the node at the given LOD is deformable, otherwise false.
		 */
		virtual bool HasDeformableMesh(const int lodLevel) const;

		/**
		 * Checks if the collision mesh for the given LOD is deformable or not.
		 * A node is deformable when it has a mesh AND it has a mesh deformer stack AND it has deformers on the stack.
		 * So in other words, a node is deformable when the mesh of the node changes over time (like the vertex positions being deformed).
		 * @param lodLevel The level of detail to perform the check on. It is possible that a lower lod is not dynamic, while the highest detail is.
		 * @result Returns true when the node at the given LOD is deformable, otherwise false.
		 */
		virtual bool HasDeformableCollisionMesh(const int lodLevel) const;

		/**
		 * Check if this node contains any meshes or colliison meshes.
		 * @result Returns true when this node has one or more meshes or collision meshes, otherwise false is returned.
		 */
		virtual bool HasMesh() const;

		/**
		 * Check if this node has any child nodes.
		 * @result Returns true when this node has child nodes, otherwise false is returned.
		 */
		inline bool HasChilds() const;

		/**
		 * Remove all motion links.
		 */
		inline void RemoveLinks();

		/**
		 * Remove a given motion link in this node which uses a specified motion instance.
		 * @param instance The motion instance to search for.
		 * @result Returns true when the motion link has been deleted, otherwise false (when its not found).
		 */
		inline bool RemoveLink(MotionInstance *instance);

		/**
		 * Find the root node in the hierarchy (recursive method).
		 * @result A pointer to the root node (can point to itself).
		 */
		inline Node* FindRoot();

		/**
		 * Set the node to it's original transformations (position, rotation, scale).
		 */
		inline void SetToOriginalOrientation();

		/**
		 * Get the forward vector of this node, in world space.
		 * The vector lenght is not guaranteed to be normalized. If the object has a scale of 10 on each axis, it will
		 * result in a vector with a length of 10. If you did not scale anything however, it will be a length of 1.
		 * This vector can be used to see where the node/actor is facing to. You have to model the node/actor in a specific way too.
		 * We mean, if the character is modeled in 3DSMax and the face is looking directly at you in the back view, the forward vector
		 * will point always in the direction where the face is pointing to. If however you model it on a way that the characters face is
		 * looking at you in the front view, the forward vector would point to the oposite direction, so in the direction on the back of the head, away from you, instead of towards you.
		 * If you would now use the forward vector to move the character when its walking, it would work backwards instead of forwards (if you modeled it with the face pointing towards you 
		 * in the front view). So, just a little thing to keep in mind, in order to understand what is happening if it is not what you expected it to be.
		 * @result The vector pointing forward, in world space.
		 */
		inline MCore::Vector3 GetForward() const;

		/**
		 * Get the vector pointing to the right, in world space.
		 * Please look read the documentation for the method GetForward() in order to understand values which you would not expect.
		 * @result The vector pointing to the right, in worldspace.
		 * @see GetForward()
		 */
		inline MCore::Vector3 GetRight() const;

		/**
		 * Get the vector pointing up, in world space.
		 * Please look read the documentation for the method GetForward() in order to understand values which you would not expect.
		 * @result The vector pointing upwards, in worldspace.
		 * @see GetForward()
		 */
		inline MCore::Vector3 GetUp() const;

		/**
		 * Get the vector pointing forward, in local space, so relative to its parent.
		 * Please look read the documentation for the method GetForward() in order to understand values which you would not expect.
		 * @result The vector pointing forward, in local space (relative to its parent).
		 * @see GetForward()
		 */
		inline MCore::Vector3 GetLocalForward() const;

		/**
		 * Get the vector pointing to the right, in local space, so relative to its parent.
		 * Please look read the documentation for the method GetForward() in order to understand values which you would not expect.
		 * @result The vector pointing to the right, in local space (relative to its parent).
		 * @see GetForward()
		 */
		inline MCore::Vector3 GetLocalRight() const;

		/**
		 * Get the vector pointing up, in local space, so relative to its parent.
		 * Please look read the documentation for the method GetForward() in order to understand values which you would not expect.
		 * @result The vector pointing up, in local space (relative to its parent).
		 * @see GetForward()
		 */
		inline MCore::Vector3 GetLocalUp() const;

		/**
		 * Move the node/actor a given amount of units forward (in world space).
		 * See the documentation of method GetForward() for unexpected behaviour.
		 * @param units The number of units to move the character.
		 * @see GetForward()
		 */
		inline void MoveForward(const float units);

		/**
		 * Move the node/actor a given amount of units to the right (in world space).
		 * See the documentation of method GetForward() for unexpected behaviour.
		 * @param units The number of units to move the character.
		 * @see GetForward()
		 */
		inline void MoveRight(const float units);

		/**
		 * Move the node/actor a given amount upwards (in world space).
		 * See the documentation of method GetForward() for unexpected behaviour.
		 * @param units The number of units to move the character.
		 * @see GetForward()
		 */
		inline void MoveUp(const float units);

		/**
		 * Rotate the node/actor a given angle (in radians) around a given axis (in local space, so relative to its parent).
		 * @param axis The axis to rotate it around.
		 * @param angleRadians The angle to rotate around this axis, in radians.
		 */
		inline void Rotate(const MCore::Vector3& axis, const float angleRadians);

		/**
		 * Add a node attribute to this node.
		 * @param attribute The attribute to add to the node.
		 */
		inline void AddAttribute(NodeAttribute* attribute);

		/**
		 * Get the number of node attributes.
		 * @result The number of node attributes for this node.
		 */
		inline int GetNumAttributes() const;

		/**
		 * Get a given node attribute.
		 * You will need to typecast to the correct type of the attribute yourself.
		 * You can get the attribute type by using the NodeAttribute::GetType() method.
		 * @param attributeNr The attribute number to get from the node. You can find the attribute number
		 * when you have the attribute type by using the FindAttributeNumber() method.
		 * @result A pointer to the node attribute.
		 * @see FindNodeAttributeNumber
		 */
		inline NodeAttribute* GetAttribute(const int attributeNr);

		/**
		 * Get a given node attribute of a given type.
		 * The method will return NULL when no attribute of the specified type could be found.
		 * @result A pointer to the node attribute, or NULL when not found.
		 */
		inline NodeAttribute* GetAttributeByType(const int attributeType);

		/**
		 * Find the node attribute number of a given type.
	 	 * When there are no node attributes, or there is no attribute of the specified type
		 * this method will return -1.
		 * @param attributeTypeID The attribute type ID (returned by NodeAttribute::GetType()).
		 * @result The first located attribute number which is of the given type, or -1 when the attribute of this type could not be located.
		 */
		inline int FindAttributeNumber(const int attributeTypeID) const;

		/**
		 * Removes all node attributes from this node.
		 * The previously allocated attributes will be deleted from memory automatically.
		 */
		void RemoveAllAttributes();

		/**
		 * Add a LOD mesh to the node.
		 * Keep in mind that the number of meshes has to be the same as the number of mesh deformer stacks for this node.
		 * So when you add an LOD mesh, you also should add a mesh deformer stack to the node.
		 * @param lodMesh A pointer to the mesh to add as next LOD mesh.
		 */
		inline void AddLODMesh(const MCore::Pointer<Mesh>& lodMesh);

		/**
		 * Add a mesh deformer stack to the node.
		 * Keep in mind that the number of meshes has to be the same as the number of mesh deformer stacks for this node.
		 * So when you add a mesh deformer stack to this node, you also have to add a LOD mesh.
		 * @param stack A pointer to the mesh deformer stack to add as next LOD stack.
		 */
		inline void AddLODMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack);

		/**
		 * Get the number of meshes in this node.
		 * Basically it means the number of LOD meshes this node contains. This is at least 1, even if
		 * the node does not contain a mesh. The reason for this is that we use smartpointer objects. And 
		 * smartpointers can also contain NULL values. If a value of 4 would be returned, this means that
		 * this node has 4 meshes, where the first mesh is the full detail mesh, and the other 3 are lower detail versions.
		 * @result The number of meshes. This is at least 1, even if there is no mesh.
		 */
		inline int GetNumMeshes() const;

		/**
		 * Get the number of mesh deformer stacks for this node.
		 * Basically it means the number of mesh deformer stacks this node contains. This is at least 1, even if
		 * the node does not contain a stack at all. The reason for this is that we use smartpointer objects. And 
		 * smartpointers can also contain NULL values. If a value of 4 would be returned, this means that
		 * this node has 4 meshes, where the first mesh is the full detail mesh, and the other 3 are lower detail versions.
		 * The number of mesh deformer stacks is always equal to the number of meshes.
		 * @result The number of mesh deformer stacks. This is at least 1, even if there is no stack.
		 */
		inline int GetNumMeshDeformerStacks() const;

		/**
		 * Add a collision mesh LOD to the node.
		 * Keep in mind that the number of collision meshes has to be the same as the number of collision mesh deformer stacks for this node.
		 * So when you add an LOD mesh, you also should add a collision mesh deformer stack to the node.
		 * @param lodMesh A pointer to the mesh to add as next LOD mesh.
		 */
		inline void AddLODCollisionMesh(const MCore::Pointer<Mesh>& lodMesh);

		/**
		 * Add a collision mesh deformer stack to the node.
		 * Keep in mind that the number of collision meshes has to be the same as the number of collision mesh deformer stacks for this node.
		 * So when you add a collision mesh deformer stack to this node, you also have to add a LOD mesh.
		 * @param stack A pointer to the mesh deformer stack to add as next LOD stack.
		 */
		inline void AddLODCollisionMeshDeformerStack(const MCore::Pointer<MeshDeformerStack>& stack);

		/**
		 * Get the number of collision meshes in this node.
		 * Basically it means the number of LOD collision meshes this node contains. This is at least 1, even if
		 * the node does not contain a mesh. The reason for this is that we use smartpointer objects. And 
		 * smartpointers can also contain NULL values. If a value of 4 would be returned, this means that
		 * this node has 4 meshes, where the first mesh is the full detail mesh, and the other 3 are lower detail versions.
		 * @result The number of meshes. This is at least 1, even if there is no mesh.
		 */
		inline int GetNumCollisionMeshes() const;

		/**
		 * Get the number of collision mesh deformer stacks for this node.
		 * Basically it means the number of collision mesh deformer stacks this node contains. This is at least 1, even if
		 * the node does not contain a stack at all. The reason for this is that we use smartpointer objects. And 
		 * smartpointers can also contain NULL values. If a value of 4 would be returned, this means that
		 * this node has 4 meshes, where the first mesh is the full detail mesh, and the other 3 are lower detail versions.
		 * The number of mesh deformer stacks is always equal to the number of meshes.
		 * @result The number of mesh deformer stacks. This is at least 1, even if there is no stack.
		 */
		inline int GetNumCollisionMeshDeformerStacks() const;

		/**
		 * Removes the meshes and stacks for all level of details of this node.
		 * The memory is automatically released when the meshes and stacks are not used by any other objects anymore.
		 */
		void RemoveMeshes();

		/**
		 * Remove the meshes and mesh deformer stack for a given LOD.
		 * @param lodLevel The LOD level to remove the mesh and stack from.
		 */
		void RemoveMeshesForLOD(const int lodLevel);

		/**
		 * Removes the collision meshes and collision stacks for all level of details of this node.
		 * The memory is automatically released when the meshes and stacks are not used by any other objects anymore.
		 */
		void RemoveCollisionMeshes();

		/**
		 * Remove the collision meshes and collision mesh deformer stack for a given LOD.
		 * @param lodLevel The LOD level to remove the collision mesh and stack from.
		 */
		void RemoveCollisionMeshesForLOD(const int lodLevel);

		/**
		 * Search for the motion part from the given motion.
		 * @param motion The motion to search for.
		 * @return The motion part which belongs to this node.
		 */
		MotionPart* SearchMotionPart(Motion* motion);

		/**
		 * Search for the motion link from the given motion.
		 * @param motion The motion to search for.
		 * @return The first motion link which has the specified motion, or NULL when none found.
		 */
		MotionLink* SearchMotionLink(Motion* motion);

		/**
		 * Search for a motion link which has the given motion instance.
		 * @param instance The motion instance to search for.
		 * @return A pointer to the motion link having the given motion instance, or NULL when not found.
		 */
		MotionLink* SearchMotionLink(MotionInstance* instance);

		/**
		 * Calculate the axis aligned bounding box based on the worldspace vertex coordinates.
		 * If the node has no mesh, the created box will be invalid.
		 * @param result The AABB where this method should store the resulting box in.
		 * @param lodLevel The LOD level to calculate the box for.
		 */
		virtual void CalcMeshBasedAABB(MCore::AABB* result, const int lodLevel);

		/**
		 * Calculate the axis aligned bounding box based on the worldspace vertex coordinates of the collision meshes.
		 * If the node has no mesh, the created box will be invalid.
		 * @param result The AABB where this method should store the resulting box in.
		 * @param lodLevel The LOD level to calculate the box for.
		 */
		virtual void CalcCollisionMeshBasedAABB(MCore::AABB* result, const int lodLevel);

		/**
		 * Set the material offset.
		 * The sub meshes of the nodes get the material offset which will be added to the sub mesh's material index when getting a material.
		 * On that way the character factory can add mesh nodes including their materials to the actor and simply set a material offset
		 * for the given mesh node so that the sub meshes of the new mesh point to materials to which they really belong.
		 * @param materialOffset The material offset which will be added to the sub mesh's material number.
		 */
		inline void SetMaterialOffset(const int materialOffset);

		/**
		 * Get the material offset.
		 * The sub meshes of the nodes get the material offset which will be added to the sub mesh's material index when getting a material.
		 * On that way the character factory can add mesh nodes including their materials to the actor and simply set a material offset
		 * for the given mesh node so that the sub meshes of the new mesh point to materials to which they really belong.
		 * @return The material offset of this node which will be added to the sub mesh's index when getting
		 *		   a material from one of the sub meshes from this node.
		 */
		inline int GetMaterialOffset();

		inline bool	IsPropNode() {	return m_isProp; }


	protected:
		MCore::Matrix					mLocalTM;		/**< Local transformation matrix (before hierarchy applied). */
		MCore::Matrix					mWorldTM;		/**< The final node transformation matrix (after hierarchy applied). */
		MCore::Matrix					mInvBoneTM;		/**< Inverse of the initial bone transformation matrix (after hierarchy). */
		MCore::Quaternion				mOrgRot;		/**< Original rotation in local space (relative to parent). */
		MCore::Quaternion				mLocalRot;		/**< The rotation before hierarchy. */
		MCore::Vector3					mLocalPos;		/**< Local position (before hierarchy). */
		MCore::Vector3					mLocalScale;	/**< Local scaling (before hierarchy). */
		MCore::Vector3					mWorldScale;	/**< Node scale (after hierarchy). */
		MCore::Vector3					mOrgPos;		/**< Original position in local space (relative to parent). */
		MCore::Vector3					mOrgScale;		/**< Original scale in local space (relative to parent). */

		MCore::Array< MCore::Pointer<Mesh> >				mMeshes;	/**< The meshes, for each LOD level. */
		MCore::Array< MCore::Pointer<MeshDeformerStack> >	mStacks;	/**< The array of mesh deformer stacks, for each LOD. */
		MCore::Array< MCore::Pointer<Mesh> >				mColMeshes;	/**< The collision meshes, for each LOD level. */
		MCore::Array< MCore::Pointer<MeshDeformerStack> >	mColStacks;	/**< The array of collision mesh deformer stacks, for each LOD. */

		MCore::Array< Node* >			mChilds;		/**< A collection of child nodes. */
		MCore::Array< MotionLink >		mLinks;			/**< Motion links. */
		MCore::Array< NodeAttribute* >	mAttributes;	/**< The node attributes. */
		MCore::String					mName;			/**< The node name. */
		Node*							mParent;		/**< Pointer to the parent, which is NULL in case of a root node. */
		NodeCollisionSystem*			mCollisionSystem;	/**< A pointer to the active node collision system. */
		int								mID;			/**< The ID, which is generated from the name. You can use this for fast compares between nodes. */
		int								mMaterialOffset;	/**< The material index offset. Default=0 */
		bool							m_isProp;
};

// include inline code
#include "Node.inl"

} // namespace EMotionFX


#endif