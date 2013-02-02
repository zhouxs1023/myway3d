/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __CHARACTERFACTORY_H
#define __CHARACTERFACTORY_H

// include the Core system
#include "../Core/Source/MCore.h"

#include "Actor.h"


namespace EMotionFX
{

/// The character factory macro for easy access.
#define CHARACTERFACTORY CharacterFactory::GetInstance()

/**
 * The character factory.
 * The character factory can be used to assemble actors from different body parts. Inside the character factory we call
 * a body part a character part. A character part is an actor which contains a section of a complete character, for example 
 * an upper body, or just a head. Character parts can be fully skinned, so they are not similar to regular attachments.
 * The main difference between attachments and using the character factory is that the character factory result in one
 * single actor which can be fully skinned. Using attachments however means you will have several different actors, which 
 * have to be animated separately. You can read more about the character factory in the EMotion FX manual.
 */
class CharacterFactory
{
	DECLARE_CLASS(CharacterFactory);
	SINGLETON( CharacterFactory );
	MEMORYOBJECTCATEGORY(CharacterFactory, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_NODES_ACTORS);

	public:
		/**
		 * Create a character from a set of character parts.
		 * The first array element that you pass as parameter to this method must contain the main skeleton.
		 * All character parts that come after this first element will be added to this first character part.
		 * So this method works the same as already having loaded your main actor, which could contain only the skeleton hierarchy
		 * without any meshes, and then making a call to AddParts to add all the other character parts to this main actor.
		 * @param bodyParts The array of actors that represent the character parts. The first array element must contain the
		 *                  main actor/structure.
		 * @result A pointer to the newly created actor that has been assembled from all the character parts that have been specified.
		 */
		Actor* CreateCharacter(MCore::Array<Actor*>& bodyParts);

		/**
		 * Add a set of character parts to the given actor.
		 * @param mainActor The main actor, to which we like the character parts to be added to.
		 * @param bodyParts The collection of character parts (body parts) that we like to add to the main actor.
		 */
		void AddParts(Actor* mainActor, const MCore::Array<Actor*>& bodyParts);

		/**
		 * Add a character part to an extisting actor.
		 * @param mainActor The main actor, to which we like the character part to be added to.
		 * @param bodyPartToAdd The actor representing the body part we'd like to add to the main actor.
		 * @param replaceNodes This will replace the meshes of nodes which already are in the main actor with the ones from the character part to add.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void AddPart(Actor* mainActor, Actor* partToAdd, const bool replaceNodes=true, const bool relinkHierarchy=true);

		/**
		 * Remove a given character part from an extisting actor.
		 * You can for example remove an arm from a character after it has been shot.
		 * You can specify if it will only remove the mesh and if nodes will get deleted which could be dangerous since we do not know if there is
		 * some controller applied to it or if any skinning influences are linked to it. If a mesh node has been found all materials which have
		 * been used by the mesh will be removed, too.
		 * @param mainActor The actor to remove the part from.
		 * @param partToRemove The body part to remove. Name matching is used to determine from which nodes to remove the meshes.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void RemovePart(Actor* mainActor, Actor* partToRemove, const bool relinkHierarchy=true);

		/**
		 * Replace an existing character part with another one.
		 * This can be useful if you have a collection of different heads for a character and want to change the current one.
		 * This function will replace the meshes from the main actor with the meshes from the replace actor which have the same name.
		 * The nodes of the meshes you want to replace from the replace actor need to have the same name as in the main actor.
		 * @param mainActor The actor to replace the parts from.
		 * @param replaceWith The actor that contains the nodes and meshes we like to replace.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void ReplacePart(Actor* mainActor, Actor* replaceWith, const bool relinkHierarchy=true);

		/**
		 * Replace the mesh of a node by a mesh from another mesh node.
		 * This can be used when you have a collection of for example 10 different heads where you like to replace
		 * the current head on the main actor with one of those.
		 * @param mainActor The actor on which you want to replace for example the head mesh.
		 * @param meshToReplace The node which contains the mesh to replace by another one. This node must be part of the main actor.
		 * @param replaceWithActor The actor that contains the the new mesh you like to use.
		 * @param replaceWith The node which contains the new mesh you like to use.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void ReplaceMesh(Actor* mainActor, Node* meshToReplace, Actor* replaceWithActor, Node* replaceWith, const bool relinkHierarchy=false);


	private:
		/**
		 * Default constructor.
		 */
		CharacterFactory();

		/**
		 * Destructor.
		 */
		~CharacterFactory();

		/**
		 * Integrate node to a character.
		 * This function will copy all nodes which aren't in the passed actor yet. So nodes which contain meshes and hierarchy nodes
		 * will be cloned including all materials.
		 * @param actor The character factory actor to work on. The mesh node will be integrated to this actor.
		 * @param nodeToIntegrate The node which will be integrated to the actor.
		 * @param integrationNodeActor The actor to which the node to integrate belongs to.
		 * @param replaceNodes This will replace the meshes of nodes which already are in the main actor with the ones from the character part to add.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void IntegrateNode(Actor* actor, Node* nodeToIntegrate, Actor* integrationNodeActor, const bool replaceNodes=true, const bool relinkHierarchy=false);

		/**
		 * Remove the mesh from a given node from a character.
		 * The function will remove the mesh of the node if the node has been found in the given actor.
		 * You can specify if it will only remove the mesh and if nodes will get deleted which could be dangerous since we do not know if there is
		 * some controller applied to it or if any skinning influences are linked to it. If a mesh node has been found all materials which have
		 * been used by the mesh will be removed, too.
		 * @param actor The character factory actor to work on. The mesh node will be removed from this actor.
		 * @param nodeToRestore The node which will be removed from the actor if the search was successful.
		 * @param relinkHierarchy Specifies if we need to relink all the parent and child pointers inside the hierarchy of the main actor. 
		 *                        Basically always keep it on the default value, then you should be fine.
		 */
		void RestoreNode(Actor* actor, Node* nodeToRestore, const bool relinkHierarchy=false);

		/**
		 * Copy materials from a given actor and add them to another one.
		 * @param from The actor from which we clone all materials.
		 * @param to The actor where all the cloned materials will be added to.
		 */
		void CopyMaterials(Actor* from, Actor* to);

		/**
		 * Reinitialize the deformer stacks of the meshes and collision meshes from the given actor.
		 * @param actor The actor to reinitialize the deformer stacks.
		 */
		void UpdateDeformerStacks(Actor* actor);
};

} // namespace EMotionFX


#endif