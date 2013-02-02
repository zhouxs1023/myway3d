/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __ACTOR_H
#define __ACTOR_H

#include "../Core/Source/MCore.h"
#include "Controller.h"
#include "Node.h"
#include "MotionQueue.h"
#include "HwShaderBuffer.h"
#include "Material.h"
#include "ActorCollisionSystem.h"
#include "MotionSystem.h"
#include "MotionLayerSystem.h"
#include "FacialSetup.h"
#include "MemoryCategories.h"


namespace EMotionFX
{

// forward declarations.
class MotionLayer;


/**
 * The actor is a completely animatable object, like a human character or an animal.
 * It is also inherited from a Node, so that means you can link actors together as well.
 * However, watch out that there are no loops in the links, which will cause infinite recursion.
 */
class Actor : public Node
{
	DECLARE_CLASS(Actor);
	MEMORYOBJECTCATEGORY(Actor, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_ACTORS);

	public:
		/// The actor clone flags (options).
		enum EActorCloneFlags
		{
			CLONE_MATERIALS			= 0x00000001,			/**< Clones the materials itself, so do not share them. */
			CLONE_NODEATTRIBUTES	= 0x00000010,			/**< Clones the node attributes, so do not share them. */
			CLONE_CONTROLLERS		= 0x00000100,			/**< Clone (duplicate) the controllers. If not set, the controllers will NOT be copied. */
			ACTOR_CLONE_DEFAULT		= (CLONE_NODEATTRIBUTES | CLONE_CONTROLLERS),	/**< The default actor cloning flags, which only copies the node attributes. The rest will be shared. */
			ACTOR_CLONE_ALL			= (CLONE_MATERIALS | CLONE_NODEATTRIBUTES | CLONE_CONTROLLERS) /**< The actor cloning flags to clone all attributes available. So nothing will be shared. */
		};

		/// The bounding volume generation types.
		enum EBoundsType
		{
			BOUNDS_NODE_BASED			= 0,	/**< Calculate the bounding volumes based on the worldspace node positions. */
			BOUNDS_MESH_BASED			= 1,	/**< Calculate the bounding volumes based on the worldspace vertex positions. */
			BOUNDS_COLLISIONMESH_BASED	= 2		/**< Calculate the bounding volumes based on the worldspace collision mesh vertex positions. */
		};

		/// The motion based actor repositioning mask
		enum ERepositioningMask
		{
			REPOSITION_POSITION			= 0x00000001,	/**< Update the actor position based on the repositioning node. This is a relative movement applied to the actor position. */
			REPOSITION_ROTATION			= 0x00000010,	/**< Update the actor rotation based on the repositioning node. This is an absolute value, so it overwrites the actor rotation! */
			REPOSITION_SCALE			= 0x00000100	/**< Update the actor scale based on the repositioning node. This is an absolute value, so it overwrites the actor's scale!*/
		};

		/**
		 * Constructor.
		 * @param name The name of the actor.
		 */
		Actor(const MCore::String& name);

		/**
		 * Destructor.
		 */
		virtual ~Actor();

		/**
		 * Start playing the specified motion on this actor.
		 * @param motion The motion to play.
		 * @param info A pointer to an object containing playback information. When set to NULL, default playback info will be used.
		 *             You can read the default values of the playback info object in the documentation of the constructor of the PlayBackInfo class.
		 * @result A pointer to a created motion instance object. You can use this motion instance object to adjust and retrieve playback information at any time.
		 * @see PlayBackInfo
		 */
		MotionInstance* PlayMotion(Motion* motion, class PlayBackInfo* info=NULL);

		/**
		 * Stop playing the motion in <blendTime> seconds.
		 * When <blendTime> is set to 0, it will be immediately stopped, which can cause a not so nice popping effects.
		 * When set to 1.0f, it would smoothly fade out the motion in exactly one second.
		 * @param motion The motion to play.
		 * @param blendTime The time in seconds until the motion has no influence anymore.
		 */
		void StopMotion(MotionInstance* motion, const float blendTime=0.3f);

		/**
		 * Set the motion system.
		 * @param motionSystem The motion system.
		 */
		inline void SetMotionSystem(MotionSystem* motionSystem);

		/**
		 * Get the motion system.
		 * @return The motion system.
		 */
		inline MotionSystem* GetMotionSystem() const;

        /**
		 * Update the node, if 'isVisible' is false heavy calculations are being skipped, which is a lot faster (use this when the actor is not visible).
		 * Also the mesh deformation stacks for all nodes in the hierarchy are not being processed when this is set to false.
		 * The animation time will still be updated, but simply the transformations (forward kinematics etc) will not be done, since
		 * the character would be not visible anyway.
		 * Finally, this method allows you to specify your own world space matrix for the actor.
		 * The actor itself is the root node in the hierarchy of this actor, so if you specify your own
		 * world space matrix, all nodes in the hierarchy will be using this matrix too in their calculations.
		 * If you specify NULL, the world space matrix for the actor will be automatically calculated from the
		 * current local transformation information (localPos, localRot, localScale).
		 * Beware that specifying your own worldTM to this method would result in incorrect world space
		 * transformation components returned by GetWorldRot() and GetWorldScale() and GetLocalRot() and GetLocalScale() and GetLocalPos().
		 *
		 * <b>IMPORTANT NOTE: </b>
		 * You have to call the UpdateBounds(...) method after the update method yourself in order to update
		 * the bounding volumes of the actor! This is not done automatically by this method!
		 *
		 * @param timePassedInSeconds The time that has passed since the last call.
		 * @param isVisible If set to false, most heavy calculations, which are not needed when the actor is not on screen, will not be performed.
		 * @param actorTM A pointer to the local space (is worldspace in case there is no parent) transformation matrix of the actor, or NULL when you want it to be calculated automatically.
		 * @see UpdateBounds
		 */
		virtual void Update(const double timePassedInSeconds, const bool isVisible, MCore::Matrix* actorTM=NULL);

		//------------------------------------------------

		/**
		 * Checks if an actor is an instance. So if it is using shared data from another actor, like the mesh.
		 * @result Returns true if the actor is an instance of another actor, or false when it's not.
		 */
		inline bool IsInstance() const;

		/**
		 * Search for a node by name (case sensitive), returns NULL when not found.
		 * @param name The name of the node.
		 * @return A pointer to the node, returns NULL when not found.
		 */
		inline Node* GetNodeByName(const char* name) const;

		/**
		 * Search for a node by name (non case sensitive), returns NULL when no node can be found.
		 * Note: when there are two nodes with the same non case sensitive name, the first one is returned.
		 * So if there is a node named "Blah" and one named "BLAH" and we search on "blah", the first one ("Blah") is returned.
		 * @param name The name of the node, non case sensitive.
		 * @result A pointer to the node with the given name, or NULL when none could be found.
		 */
		inline Node* GetNodeByNameNoCase(const char* name) const;

		/**
		 * Search for a node on ID. Returns NULL when no node found.
		 * @param id The ID to search for.
		 * @return A pointer to the node, or NULL when no node with the given ID found.
		 */
		inline Node* GetNodeByID(const int id) const;

		/**
		 * Search for a node by name, and returns it's number in the array (case sensitive), returns -1
		 * when not found.
		 * @param name The name of the node.
		 * @return The number of the node, returns -1 when not found.
		 */
		inline int GetNodeNumberByName(const char* name) const;

		/**
		 * Get the number of nodes this actor is constructed from (bones, helpers, attachment points, etc).
		 * @return The number of nodes.
		 */
		inline int GetNumNodes() const;

		/**
		 * Get the nodes array.
		 * @return The array of nodes.
		 */
		inline MCore::Array<Node*>& GetNodes();

		/**
		 * Get access to a given node.
 		 * @param nr The number of the node.
		 * @return A pointer to the Node.
		 */
		inline Node* GetNode(const int nr) const;

		/**
		 * Returns the axis aligned bounding box.
		 * @return The bounding box.
		 */
		inline const MCore::AABB& GetBoundingBox() const;

		/**
		 * Returns the bounding sphere.
		 * @return The bounding sphere.
		 */
		inline const MCore::BoundingSphere& GetBoundingSphere() const;

		/**
		 * Update the bounding volumes of the actor.
		 * @param lodLevel The level of detail number to generate the LOD for. Must be in range of 0..GetNumLODLevels()-1.
		 *                 The same LOD level will be choosen for the attachments when they will be included.
		 *                 If the specified LOD level is lower than the number of attachment LODS, the lowest attachment LOD will be choosen.
		 * @param boundsType The bounding volume generation method, each having different accuracies and generation speeds.
		 * @param includeAttachments Set to true if you'd like to include the attachment bounding volumes inside the bounding
		 *                           volumes for this actor. Set to false if you would like to exclude them.
		 */
		void UpdateBounds(const int lodLevel, EBoundsType boundsType=BOUNDS_NODE_BASED, const bool includeAttachments=true);

		/**
		 * Calculate the axis aligned bounding box based on the worldspace vertex coordinates of the meshes.
		 * If the actor has no meshes, the created box will be invalid.
		 * @param result The AABB where this method should store the resulting box in.
		 * @param lodLevel The LOD level to calculate the box for.
		 */
		void CalcMeshBasedAABB(MCore::AABB* result, const int lodLevel);

		/**
		 * Calculate the axis aligned bounding box based on the worldspace vertex coordinates of the collision meshes.
		 * If the actor has no collision meshes, the created box will be invalid.
		 * @param result The AABB where this method should store the resulting box in.
		 * @param lodLevel The LOD level to calculate the box for.
		 */
		void CalcCollisionMeshBasedAABB(MCore::AABB* result, const int lodLevel);

		/**
		 * Add a node to this actor.
		 * @param node The node to add.
		 */
		inline void AddNode(Node* node);

		/**
		 * Remove a given node.
		 * @param nr The node to remove.
		 * @param delMem If true the allocated memory of the node will be deleted.
		 */
		inline void RemoveNode(const int nr, const bool delMem=true);

		/**
		 * Remove all nodes from memory.
		 */
		inline void DeleteAllNodes();

		/**
		 * Reset all nodes to the initial pose transformations (as how they where loaded from the LMA actor file).
		 */
		inline void RestoreInitialPose();

		/**
		 * Resolve the parent/child relationships. This relinks all the parent and child pointers of all nodes.
		 * While modifying the actor's hierarchy by adding new nodes from another actor which still includes its old links and refers
		 * to nodes from the old actor, you don't have a valid hierarchy anymore. To solve this, we can simply do some searches on names (IDs)
		 * of nodes in the original nodes array. First find all the parents, once we have the parents we can also find the childs.
		 * If one node is linked to a node which isn't part of the given actor it will be parented to the actor itself and will act as root node.
		 */
		void RelinkHierarchy();

		/**
		 * Process the hierarchy for all nodes. This will construct/update all LocalTM's from all nodes
		 * attached to this actor. It will also calculate the world space transformation matrix (WorldTM)
		 * and the world space position, rotation and scale.
		 * Finally, this method allows you to specify your own world space matrix for the actor.
		 * The actor itself is the root node in the hierarchy of this actor, so if you specify your own
		 * world space matrix, all nodes in the hierarchy will be using this matrix too in their calculations.
		 * If you specify NULL, the world space matrix for the actor will be automatically calculated from the
		 * current local transformation information (localPos, localRot, localScale).
		 * Beware that specifying your own worldTM to this method would result in incorrect world space
		 * transformation components returned by GetWorldRot() and GetWorldScale() and GetLocalRot() and GetLocalScale() and GetLocalPos().
		 * NOTE: This method is automatically called by the Update method.
		 * See Node::RecursiveHierarchyUpdate() for more info.
		 * @param actorWorldTM A pointer to the local space (or worldspace if the actor has no parent) transformation matrix of the actor, or NULL when you want it to be calculated automatically.
		 * @see Node::RecursiveHierarchyUpdate()
		 */
		void ProcessHierarchy(MCore::Matrix* actorTM=NULL);

		/**
		 * Updates the mesh deformer stacks of all nodes which are part of this actor.
		 * NOTE: This method is automatically called by the Update method.
		 * @param timePassedInSeconds The passed time since the last call, in seconds.
		 */
		inline void UpdateNodeStacks(const double& timePassedInSeconds);

		/**
		 * Clones this actor (excluding the motions [motion infos and motion tree]).
		 * @return A pointer to the clone instance.
		 */
		Actor* Clone(EActorCloneFlags flags = ACTOR_CLONE_DEFAULT);

		/**
		 * Get a pointer to the original actor. When NULL, the actor doesn't depend on any other actors shared data.
		 * This means, if this method returns a NON-NULL value, it means this actor is an instance of another actor.
		 * The pointer returned points to the actor this actor is an instance from.
		 * @result A pointer to the original actor or NULL if it is an original one.
		 */
		inline Actor* GetOriginalActor();

		//---------------------------------------------------------------------

		/**
		 * Get the node inside the actor that will move the actor.
		 * This is used when you want a motion to automatically move the actor position.
		 * Imagine a walk motion, where there is a node such as "Bip01", which acts as root node.
		 * This root node moves during the animation, which moves all its child nodes as well, which visually
		 * moves the character on the scene. However, the actor object will remain the same, even while the
		 * actor on screen moves. In order to capture the movement of the root node and apply it on the actor, you
		 * will have to specify which node to capture the movements from. In our example case this would be the node
		 * with the name "Bip01". This node is the repositioning node that is returned by this method.
		 * When this method returns NULL, it means that no motion based actor repositioning will be performed.
		 * @result A parameter to the node which movement will be captured and applied on this actor.
		 */
		inline Node* GetRepositioningNode();

		/**
		 * Get the repositioning mask.
		 * The repositioning mask describes what transformation components to extract from the motion and to apply to this actor.
		 * This mask will only have any influence when the repositioning node has been set and thus does not equal NULL.
		 * You can set the repositioning node using the SetRepositioningNode or AutoSetRepositioningNode methods.
		 * @result The repositioning mask, which can be a combination of the values inside the specific enum.
		 */
		inline ERepositioningMask GetRepositioningMask() const;

		/**
		 * Set the repositioning mask.
		 * The repositioning mask describes what transformation components to extract from the motion and to apply to this actor.
		 * This mask will only have any influence when the repositioning node has been set and thus does not equal NULL.
		 * You can set the repositioning node using the SetRepositioningNode or AutoSetRepositioningNode methods.
		 * @param mask The repositioning mask to set. This will only have influence when the repositioning node is not NULL.
		 */
		inline void SetRepositioningMask(ERepositioningMask mask);

		/**
		 * Set the repositioning mask using three boolean values, which might be easier than creating a mask by or-ing enums together.
		 * The repositioning mask describes what transformation components to extract from the motion and to apply to this actor.
		 * This mask will only have any influence when the repositioning node has been set and thus does not equal NULL.
		 * You can set the repositioning node using the SetRepositioningNode or AutoSetRepositioningNode methods.
		 * @param positionEnabled Set to true when you want to extract and apply relative translation from the motions.
		 * @param rotationEnabled Set to true when you want to extract and apply absolute rotation from the motions.
		 * @param scaleEnabled Set to true when you want to extract and apply the absolute scaling from the motions.
		 */
		inline void SetRepositioningMask(const bool positionEnabled, const bool rotationEnabled, const bool scaleEnabled);

		/**
		 * Get the node inside the actor that will move the actor.
		 * This is used when you want a motion to automatically move the actor position.
		 * Imagine a walk motion, where there is a node such as "Bip01", which acts as root node.
		 * This root node moves during the animation, which moves all its child nodes as well, which visually
		 * moves the character on the scene. However, the actor object will remain the same, even while the
		 * actor on screen moves. In order to capture the movement of the root node and apply it on the actor, you
		 * will have to specify which node to capture the movements from. In our example case this would be the node
		 * with the name "Bip01". This node is the repositioning node that is returned by this method.
		 * When the specified root node is NULL, it means that no motion based actor repositioning will be performed.
		 * @param repositioningNode The node from which the movement will be applied to the actor instead of to this root node itself.
		 */
		inline void SetRepositioningNode(Node* repositioningNode);

		/**
		 * Try to find the best repositioning node automatically.
		 * It picks the node with the most number of child nodes down the hierarchy below that node.
		 * @result A pointer to the best suitable repositioning node, or NULL when no node could be found.
		 */
		Node* FindBestRepositioningNode();

		/**
		 * Automatically find the best repositioning node, using FindBestRepositioningNode and set it as current
		 * repositioning node.
		 */
		void AutoSetRepositioningNode();

		//---------------------------------------------------------------------

		/**
		 * Check if there is any node inside this actor, which has a deformable mesh for the specified LOD.
		 * @param lodLevel The level of detail you want to check the result for. Must be in range of [0..GetNumLODLevels()-1].
		 * @result Returns true when there is a node which is deformable. When false is returned, this means that all meshes (if there are any)
		 *         are rigid, non-deformable meshes.
		 */
		bool HasDeformableMesh(const int lodLevel) const;

		/**
		 * Check if there is any node inside this actor, which has a deformable collision mesh for the specified LOD.
		 * @param lodLevel The level of detail you want to check the result for. Must be in range of [0..GetNumLODLevels()-1].
		 * @result Returns true when there is a node which is deformable. When false is returned, this means that all collision meshes (if there are any)
		 *         are rigid, non-deformable meshes.
		 */
		bool HasDeformableCollisionMesh(const int lodLevel) const;

		/**
		 * Check if this actor contains any nodes that have meshes or colliison meshes.
		 * @result Returns true when this actor contains nodes that have collision meshes or meshes. Otherwise false is returned.
		 */
		bool HasMesh() const;

		/**
		 * Extract a list with nodes that represent bones.
		 * Not all nodes inside an actor have to be bones. With bones we mean ndoes that appear inside the skinning
		 * information of the meshes. So nodes that have vertices linked to them.
		 * Extracting this information is not fast, so shouldn't be done inside a loop.
		 * @param lodLevel The LOD level, which must be in range of 0..GetNumLODLevels().
		 * @param outBoneList The array of pointers to nodes that will be filled with the nodes that are bones. When the outBoneList array
		 *                    already contains items, the array will first be cleared, so all existing contents will be lost.
		 */
		void ExtractBoneList(const int lodLevel, MCore::Array<Node*>* outBoneList);

		/**
		 * Set a new collision system to this actor. It deletes the old one, if there was one available.
		 * @param collisionSystem The new collision system.
		 */
		void SetActorCollisionSystem(ActorCollisionSystem* collisionSystem);

		/**
		 * Return the collision system of this actor.
		 * @return A pointer to the collision system, NULL if it doesn't exist.
		 */
		inline ActorCollisionSystem* GetActorCollisionSystem() const;

		//------------------------------------------------

		/**
		 * Get a given material.
		 * @param lodLevel The LOD level to get the material from.
		 * @param nr The material number to get.
		 * @result A pointer to the material.
		 */
		inline MCore::Pointer<Material>& GetMaterial(const int lodLevel, const int nr);

		/**
		 * Get the material by name (case sensitive).
		 * @param lodLevel The level of detail to get the material from.
		 * @param name The name of the material.
		 * @result Returns a NULL pointer in case no material has been found with the given name.
		 */
		inline Material* GetMaterialByName(const int lodLevel, const MCore::String& name) const;

		/**
		 * Set a given material.
		 * @param lodLevel The LOD level to set the material at.
		 * @param nr The material number to set.
		 * @param mat The material to set at this index.
		 */
		inline void SetMaterial(const int lodLevel, const int nr, const MCore::Pointer<Material>& mat);

		/**
		 * Add a material to the back of the material list.
		 * @param lodLevel The LOD level add the material to.
		 * @param mat The material to add to the back of the list.
		 */
		inline void AddMaterial(const int lodLevel, const MCore::Pointer<Material>& mat);

		/**
		 * Remove the given material from the material list and reassign all material numbers of the sub meshes
		 * since the material order will be changed after removing a material. This means that several sub meshes
		 * do not point to their correct materials anymore or that they would be even out of range. If one of the sub
		 * meshes got a bigger material index number than the material which has been removed, the offset of the node
		 * to which this sub mesh belongs to will be changed so that the sub mesh point to its right material again.
		 * This will be fixed by decreasing their material offset.
		 * @param lodLevel The LOD level add the material to.
		 * @param index The material index of the material to remove.
		 */
		void RemoveMaterial(const int lodLevel, const int index);

		/**
		 * Get the number of materials.
		 * @param lodLevel The LOD level to get the number of material from.
		 * @result The number of materials this actor has/uses.
		 */
		inline int GetNumMaterials(const int lodLevel) const;

		/**
		 * Removes all materials from this actor.
		 */
		void RemoveAllMaterials();

		/**
		 * Remove all materials which are not used by a mesh of the actor.
		 * If there has been found a material which isn't used by a mesh it will be removed.
		 * @see RemoveMaterial()
		 */
		void RemoveUnusedMaterials();

		/**
		 * Check whether the given material is used by one of the meshes in the actor.
		 * So checks each material with the the material which is passed as parameter if they point to the same material
		 * object in memory. If they are equal it returns true, this means that the given material is used by a mesh of
		 * the actor. False means that no mesh uses the given material.
		 * @param lodLevel The LOD level of the material to check.
		 * @param index The material number to check.
		 */
		bool IsMaterialUsed(const int lodLevel, const int index);

		//------------------------------------------------

		/**
		 * Add an actor as LOD model.
		 * This method only copies the needed information from the given actor, and sets these as LOD versions.
		 * So the actor itself will not be stored. Only pointer values will be copied (including reference counting).
		 * So right after you have added the actor as LOD model to this actor, it is safe to release the added LOD actor
		 * from memory, since it isn't used internally anyway.
		 * @param lodActor A pointer to the actor to add as next level of detail to this actor.
		 * @param delLODActorFromMem When set to true, the method will automatically delete the given lodActor from memory.
		 */
		void AddLODModel(Actor* lodActor, bool delLODActorFromMem=true);

		/**
		 * Get the number of LOD levels inside this actor.
		 * @result The number of LOD levels. This value is at least 1, since the full detail LOD is always there.
		 */
		inline int GetNumLODLevels() const;

		/**
		 * Get the current LOD level.
		 * LOD levels start at 0, which is the highest detail. A value of 1 would be the next lower detail, etc.
		 * @result The current level of detail.
		 */
		inline int GetCurrentLOD() const;

		/**
		 * Switch to another level of detail.
		 * The system is designed on such a way that switching between LOD levels does not take any speed.
		 * It is safe to switch LOD as much as you want, without having to worry about performance related costs to switch LOD.
		 * You can request the total amount of LOD levels with GetNumLODLevels().
		 * @param lodNumber The level of detail to switch to, where 0 is the highest detail, and 1 would be the next lower detail, etc.
		 */
		inline void SetLOD(const int lodNumber);

		//--------------------------------------------------------------------------

		/**
		 * Removes all meshes from all nodes inside this actor.
		 * This means all memory will be released and pointers to the meshes will not be valid anymore.
		 */
		void RemoveAllNodeMeshes();

		/**
		 * Generates hardware vertex shader ready renderable buffers.
		 * This greatly simplifies the hardware shading support for you. The problem with hardware shaders is
		 * that the number of bones per shader are limited, because of a limited amount of constant registers.
		 * This method spits out buffers, which will fit the requirements for vertex shaders you specified.
		 * In other words, they will have not more bones per buffer as you specify. Also buffers are sorted on 
		 * material and number of weights. Last but not least all meshes inside the actor have been merged into
		 * the buffers for more efficient rendering. Each buffer contains an array of primitives, which will describe
		 * what parts of the buffer should be rendered with what transformations (you can get them from the node pointers 
		 * inside the buffer primitives). Last but not least, you can also specify the level of detail to generate the buffers for.
		 * You can clear the buffers from memory again by simply calling outBuffers.Clear();
		 * After you have converted these buffers into your rendering API buffer format it is safe to delete all actor node meshes
		 * and the buffers from memory. You can remove all meshes using the RemoveAllNodeMeshes() method.
		 *
		 * NOTE: do NOT delete the meshes of the actor nodes before you have converted these buffers into the format of your rendering
		 * API (like Direct3D or OpenGL). The reason for this is that the buffers contain pointers to vertex data into the meshes.
		 *
		 * @param outBuffers The array where to store the buffers in. This should be an empty array, otherwise the new buffers will be added at the end of the array.
		 * @param lodLevel The LOD level to work on, where 0 is the highest detail. This value must be in range of [0..GetNumLODLevels()-1].
		 * @param maxBonesPerShader The maximum number of bones per vertex shader call. The default is 15, which takes 3*15=45 constant registers for only the bone matrices.
		 * @param sortOnNumWeights Set to true when each buffer should have the same amount of influences per vertex.
		 *                         Set to false if the same buffer is allowed to have different number of influences per vertex.
		 * @param includeSmartMeshMorphedMeshes Set to true if you want to include the meshes that have smart mesh morphing deformers on them.
		 *                                      Nodes that have facial animation applied on them using mesh morphing will most likely be processed on the CPU, so therefor you do not
		 *                                      want them to be included in the output by this method. However, if you plan to do the morphing on the GPU, you have to set this parameter to true.
		 *                                      On default it is set to false.
		 */
		void GenerateHardwareShaderBuffers(MCore::Array<HwShaderBuffer>& outBuffers, const int lodLevel, const int maxBonesPerShader=15, const bool sortOnNumWeights=true, const bool includeSmartMeshMorphedMeshes=false);

		/**
		 * Calculates the total number of vertices and indices of all node meshes for the given LOD.
		 * The number of faces can be calculated by dividing the resulting number of indices by 3.
		 * @param lodLevel The LOD level, where 0 is the highest detail LOD level. This value must be in range of [0..GetNumLODLevels()-1].
		 * @param outNumVertices The integer to write the number of vertices in.
		 * @param outNumIndices The integer to write the number of indices in.
		 */
		void CalcMeshTotals(const int lodLevel, int* outNumVertices, int* outNumIndices);

		/**
		 * Calculates the total number of vertices and indices of all STATIC node meshes for the given LOD.
		 * The number of faces can be calculated by dividing the resulting number of indices by 3.
		 * With static we mean the meshes that are NOT effected by any deformers, so which are completely rigid.
		 * @param lodLevel The LOD level, where 0 is the highest detail LOD level. This value must be in range of [0..GetNumLODLevels()-1].
		 * @param outNumVertices The integer to write the number of vertices in.
		 * @param outNumIndices The integer to write the number of indices in.
		 */
		void CalcStaticMeshTotals(const int lodLevel, int* outNumVertices, int* outNumIndices);

		/**
		 * Calculates the total number of vertices and indices of all DEFORMABLE node meshes for the given LOD.
		 * With deformable we mean meshes that are effected by mesh deformers.
		 * The number of faces can be calculated by dividing the resulting number of indices by 3.
		 * @param lodLevel The LOD level, where 0 is the highest detail LOD level. This value must be in range of [0..GetNumLODLevels()-1].
		 * @param outNumVertices The integer to write the number of vertices in.
		 * @param outNumIndices The integer to write the number of indices in.
		 */
		void CalcDeformableMeshTotals(const int lodLevel, int* outNumVertices, int* outNumIndices);

		/**
		 * Calculates the maximum number of bone influences.
		 * This is calculated by for each vertex checking the number of bone influences, and take the maximum of that amount.
		 * @param lodLevel The LOD level, where 0 is the highest detail LOD level. This value must be in range of [0..GetNumLODLevels()-1].
		 * @result The maximum number of influences. This will be 0 for non-softskinned objects.
		 */
		int CalcMaxNumInfluences(const int lodLevel);

		/**
		 * Calculates the maximum number of bone influences.
		 * Also provides an array containing the number of vertices for each number of influences.
		 * @param lodLevel The LOD level, where 0 is the highest detail LOD level. This value must be in range of [0..GetNumLODLevels()-1].
		 * @param vertexCounts An output array, which will contain for each number of influences, the number of vertices with this number of
		 * influences. In other words, the first element of the array will contain the number of vertices, which have 0 influences.
		 * The next element in the array will contain the number of vertices, which are influenced by exactly 1 bone, etc.
		 * @result The maximum number of vertex/bone influences. This will be 0 for rigid, non-skinned objects.
		 */
		int CalcMaxNumInfluences(const int lodLevel, MCore::Array<int>& vertexCounts);

		//--------------------------------------------------------------------------

		/**
		 * Add a controller to the back of the controller stack.
		 * This controller will be executed BEFORE the world space information is calculated.
		 * So it will be calculated BEFORE the ProcessHierarchy is called.
		 * @param controller The controller to add to the back of the stack.
		 */
		inline void AddController(Controller* controller);

		/**
		 * Insert a controller somewhere in the stack of controllers.
		 * This controller will be executed BEFORE the world space information is calculated.
		 * So it will be calculated BEFORE the ProcessHierarchy is called.
		 * @param pos The position in the stack, which must be in range of [0..GetNumControllers()-1]
		 * @param controller The controller to insert at this position.
		 */
		inline void InsertController(const int pos, Controller* controller);

		/**
		 * Remove a specified controller.
		 * @param nr The controller number to remove from the controller stack.
		 * @param delFromMem If set to true, the controller will be deleted from memory.
		 */
		inline void RemoveController(const int nr, const bool delFromMem=true);

		/**
		 * Add a controller to the back of the controller stack.
		 * @param controller The controller to remove from the controller stack.
		 * @param delFromMem If set to true, the controller will be deleted from memory.
		 */
		inline void RemoveController(Controller *controller, const bool delFromMem=true);

		/**
		 * Removes all controllers from the controller stack.
		 * @param delFromMem If set to true, the controllers will be deleted from memory.
		 */
		void RemoveAllControllers(const bool delFromMem=true);

		/**
		 * Gets a given controller.
		 * This controller will be executed BEFORE the world space information is calculated.
		 * So it will be calculated BEFORE the ProcessHierarchy is called.
		 * @param nr The controller number to get.
		 * @result A pointer to the controller.
		 */
		inline Controller* GetController(const int nr) const;

		/**
		 * Get the number of controllers on the controller stack.
		 * These controllers will be executed BEFORE the world space information is calculated.
		 * So they will be calculated BEFORE the ProcessHierarchy is called.
		 * @result The number of controllers on the controller stack.
		 */
		inline int GetNumControllers() const;

		/**
		 * Updates all controllers on the stack, from the first till the last (top) on the stack.
		 * These controllers will be executed BEFORE the world space information is calculated.
		 * So they will be calculated BEFORE the ProcessHierarchy is called.
		 * @param timePassedInSeconds The time passed since the last update, in seconds.
		 */
		inline void UpdateControllers(const double timePassedInSeconds);

		//--------------------------------------------------------------------------

		/**
		 * Add a controller to the back of the post controller stack.
		 * This controller will be executed AFTER the world space information is calculated.
		 * So it will be calculated AFTER the ProcessHierarchy is called.
		 * @param controller The controller to add to the back of the stack.
		 */
		inline void AddPostController(Controller* controller);

		/**
		 * Insert a controller somewhere in the stack of controllers.
		 * This controller will be executed AFTER the world space information is calculated.
		 * So it will be calculated AFTER the ProcessHierarchy is called.
		 * @param pos The position in the stack, which must be in range of [0..GetNumPostControllers()-1]
		 * @param controller The controller to insert at this position.
		 */
		inline void InsertPostController(const int pos, Controller* controller);

		/**
		 * Remove a specified post controller.
		 * @param nr The controller number to remove from the post controller stack.
		 * @param delFromMem If set to true, the controller will be deleted from memory.
		 */
		inline void RemovePostController(const int nr, const bool delFromMem=true);

		/**
		 * Add a controller to the back of the post controller stack.
		 * @param controller The post controller to remove from the controller stack.
		 * @param delFromMem If set to true, the controller will be deleted from memory.
		 */
		inline void RemovePostController(Controller *controller, const bool delFromMem=true);

		/**
		 * Removes all post controllers from the post controller stack.
		 * @param delFromMem If set to true, the post controllers will be deleted from memory.
		 */
		void RemoveAllPostControllers(const bool delFromMem=true);

		/**
		 * Gets a given post controller.
		 * This controller will be executed AFTER the world space information is calculated.
		 * So it will be calculated AFTER the ProcessHierarchy is called.
		 * @param nr The controller number to get.
		 * @result A pointer to the controller.
		 */
		inline Controller* GetPostController(const int nr) const;

		/**
		 * Get the number of post controllers on the post controller stack.
		 * These post controllers will be executed AFTER the world space information is calculated.
		 * So they will be calculated AFTER the ProcessHierarchy is called.
		 * @result The number of controllers on the controller stack.
		 */
		inline int GetNumPostControllers() const;

		/**
		 * Updates all post controllers on the stack, from the first till the last (top) on the stack.
		 * These controllers will be executed AFTER the world space information is calculated.
		 * So they will be calculated AFTER the ProcessHierarchy is called.
		 * @param timePassedInSeconds The time passed since the last update, in seconds.
		 */
		inline void UpdatePostControllers(const double timePassedInSeconds);

		//--------------------------------------------------------------------------

		/**
		 * Adds a new attachment to the actor, and link it to a specified target node.
		 * An attachment could for example be a sword in the hand of a knight.
		 * @param attachment The attachment to add to the actor, for example an actor representing a sword.
		 * @param attachNode The node inside this actor to attach the attachment to. For example the hand node of the knight.
		 * @result The attachment index number. Please keep in mind that this number can become incorrect when attachments are removed later on.
		 */
		int AddAttachment(Actor* attachment, Node* attachTarget);

		/**
		 * Removes a specified attachment from this actor. This attachment must have been attached using AddAttachment before.
		 * @param attachment The attachment actor to remove. For example the sword of a knight.
		 * @param delFromMem When set to true, the specified attachment will be deleted from memory, otherwise it will just be
		 *                   removed from the actor while remaining the memory allocated.
		 */
		void RemoveAttachment(Actor* attachment, const bool delFromMem=true);

		/**
		 * Removes a specified attachment from this actor, by index. This attachment must have been attached before, using AddAttachment.
		 * @param nr The attachment number to remove. This value must be in range of [0..GetNumAttachments()-1].
		 * @param delFromMem When set to true, the specified attachment will be deleted from memory, otherwise it will just be
		 *                   removed from the actor while remaining the memory allocated.
		 */
		void RemoveAttachment(const int nr, const bool delFromMem=true);

		/**
		 * Removes all attachments from the actor, which have been added using AddAttachment before.
		 * @param delFromMem When set to true, the specified attachment will be deleted from memory, otherwise it will just be
		 *                   removed from the actor while remaining the memory allocated.
		 */
		void RemoveAllAttachments(const bool delFromMem=true);

		/**
		 * Get the number of attachments that have been added using AddAttachment.
		 * @result The number of attachments added to this actor.
		 */
		inline int GetNumAttachments() const;

		/**
		 * Link an attachment, which has been added before (using AddAttachment), to another target node.
		 * So this just changes the node to which the attachment is attached.
		 * Do NOT use NULL as attachTarget here. If you want to unlink the attachment (so detach it), use RemoveAttachment and set delFromMem to false.
		 * @param attachment The attachment you want to relink. This attachment must have been added to this actor using AddAttachment before.
		 * @param attachTarget The new node to attach the attachment to. Again, NULL is NOT allowed.
		 */
		void ReLinkAttachment(Actor* attachment, Node* attachTarget);

		/**
		 * Link an attachment, which has been added before (using AddAttachment), to another target node.
		 * So this just changes the node to which the attachment is attached.
		 * Do NOT use NULL as attachTarget here. If you want to unlink the attachment (so detach it), use RemoveAttachment and set delFromMem to false.
		 * @param nr The attachment number to relink. This must be in range of [0..GetNumAttachments()-1].
		 * @param attachTarget The new node to attach the attachment to. Again, NULL is NOT allowed.
		 */
		void ReLinkAttachment(const int nr, Node* attachTarget);

		/**
		 * Get a pointer to a given attachment, which has been added to this actor before, using AddAttachment.
		 * @param nr The attachment number to get. This must be in range of [0..GetNumAttachments()-1].
		 * @result A pointer to the attachment.
		 */
		inline Actor* GetAttachment(const int nr) const;

		/**
		 * Find an attachment by using a name search.
		 * @param name The name of the attachment to search for.
		 * @result Returns a pointer to the actor having the specified name, or NULL when no attachment with this name could be found.
		 */
		Actor* FindAttachmentByName(const MCore::String& name) const;

		/**
		 * Find an attachment by using an ID check, which comes down to a name compare, but fast.
		 * @param id The ID of the actor to search for.
		 * @result Returns a pointer to the actor having the ID, or NULL when no attachment with this ID could be found.
		 */
		Actor* FindAttachmentByID(const int id) const;

		/**
		 * Find the attachment index number from a given name.
		 * @param name The name of the attachment to search for.
		 * @result Returns the index number of the attachment, or -1 when no attachment with the given name could be found.
		 */
		int FindAttachmentNumberByName(const MCore::String& name) const;

		/**
		 * Find the attachment index number from a given ID.
		 * @param id The ID of the attachment to search for.
		 * @result Returns the index number of the attachment, or -1 when no attachment with the given ID could be found.
		 */
		int FindAttachmentNumberByID(const int id);

		/**
		 * Find the attachment index number from a given pointer.
		 * @param attachment A pointer to the actor that represents the attachment we like to search the index number for.
		 * @result Returns the index number of the attachment, or -1 when the specified attachment isn't an attachment inside this actor.
		 */
		int FindAttachmentNumberByPointer(Actor* attachment) const;

		//------------

		/**
		 * Set the facial setup for a given LOD level.
		 * It is possible to set the facial setup for a given LOD level to NULL.
		 * If the facial setup is NULL, no facial animation is being processed for the given LOD.
		 * @param lodLevel The LOD level, which must be in range of [0..GetNumLODLevels()-1].
		 * @param setup The facial setup for this LOD.
		 */
		void SetFacialSetup(const int lodLevel, const MCore::Pointer<FacialSetup>& setup);

		/**
		 * Get the facial setup for a given LOD level.
		 * A facial setup can be NULL. In that case no facial animation will be processed for the given LOD level.
		 * @param lodLevel The LOD level, which must be in range of [0..GetNumLODLevels()-1].
		 * @result Returns a smart pointer to the facial setup for the specified LOD level.
		 */
		MCore::Pointer<FacialSetup>& GetFacialSetup(const int lodLevel);

		/**
		 * Remove all facial setups for all the LOD levels.
		 * This sets all facial setups to NULL, which means the reference counts of the smart pointers will also be
		 * automatically decreased. This also automatically removes all SmartMeshMorphDeformer mesh deformers from all 
		 * the mesh deformer stacks of all nodes, including all their LOD levels.
		 */
		void RemoveAllFacialSetups();


	protected:
		MCore::Array< MCore::Array< MCore::Pointer<Material> > >	mMaterials;		/**< A collection of materials (for each lod). */
		MCore::Array< MCore::Pointer<FacialSetup> >					mFacialSetups;	/**< The facial setups, containing the set of expression parts and phonemes, etc. One setup for every LOD level. */
		MCore::Array<Actor*>			mAttachments;			/**< A collection of attachments. */
		MCore::Array<Node*>				mNodes;					/**< The collection of nodes inside this actor (doesn't need to be the same as all childs! because actors can be linked with eachother). */
		MCore::Array<Controller*>		mControllers;			/**< The controller stack (before worldspace). */
		MCore::Array<Controller*>		mPostControllers;		/**< The post controller stack (after worldspace). */
		MCore::AABB						mBoundingBox;			/**< Axis aligned bounding box. */
		MCore::BoundingSphere			mBoundingSphere;		/**< The bounding sphere. */
		Actor*							mOriginalActor;			/**< A pointer original actor (an instance points to the actor it has been instanced from). When this value is NULL, it means its not an instance. */
		Node*							mRepositioningNode;		/**< The node which will move the actor object itself, can be NULL too in case this is not desired. */
		MotionSystem*					mMotionSystem;			/**< The motion system. */
		ActorCollisionSystem*			mActorCollisionSystem;	/**< A pointer to the active actor collision system. */
		int								mCurrentLOD;			/**< The currently active LOD level, where 0 is the highest detail. */
		int								mNumLODs;				/**< The number of LOD levels. This value is always at least 1. */
		ERepositioningMask				mRepositioningMask;		/**< The repositioning mask, which describes what transformation components to adjust when repositioning is enabled. */

		/**
		 * Release allocated memory used by this actor.
		 */
		void Release();
};

// include inline code
#include "Actor.inl"

} // namespace EMotionFX


#endif