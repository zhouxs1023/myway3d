/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "CharacterFactory.h"

#include "SoftSkinDeformer.h"
#include "MeshDeformerStack.h"
#include "SoftSkinManager.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "SkinningInfoVertexAttributeLayer.h"


// use the Core namespace
using namespace MCore;

namespace EMotionFX
{

// default constructor
CharacterFactory::CharacterFactory()
{
}


// destructor
CharacterFactory::~CharacterFactory()
{
}


// create a character from character parts
Actor* CharacterFactory::CreateCharacter(Array<Actor*>& bodyParts)
{
	// return if there are no character parts
	if (bodyParts.IsEmpty())
		return NULL;


	// pick the first character part as main character to which the other parts get added to
	// clone the actor so that we can reuse the parts to create other actors
	Actor* firstPart = bodyParts[0];
	Actor* result = firstPart->Clone( (Actor::EActorCloneFlags)Actor::ACTOR_CLONE_ALL );

	// delete the actor we have choosen to be the resulting actor
	bodyParts.Remove(0);

    // assemble character
	AddParts(result, bodyParts);

	// add the actor again, so that we do not modify the array
	bodyParts.Insert(0, firstPart);


	// return the assembled character
	return result;
}


// add several character parts to the given main actor
void CharacterFactory::AddParts(Actor* mainActor, const MCore::Array<Actor*>& bodyParts)
{
	// return if there are no character parts or the main skeleton actor is invalid
	if (bodyParts.IsEmpty() || !mainActor)
		return;


	// iterate through add all parts and add them to our resulting actor
	const int numParts = bodyParts.GetLength();
	for (int i=0; i<numParts; i++)
	{
		// retrieve the current character part
		Actor* partToAdd = bodyParts[i];

		// embed the character part in the resulting one
		AddPart(mainActor, partToAdd, false);
	}


	// resolve the parent/child relationships
	mainActor->RelinkHierarchy();

	// update the character so that everything is up-to-date
	mainActor->Update(0.0f, true);
	//mainActor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
}


// add a character part to an extisting actor
void CharacterFactory::AddPart(Actor* actor, Actor* partToAdd, const bool replaceNodes, const bool relinkHierarchy)
{
	// check if the actors are valid
	if (!actor || !partToAdd)
		return;


	// retrieve the number of nodes
	const int numNodes = partToAdd->GetNumNodes();

	int i;
	// clone all nodes which aren't in the actor yet and add them to it (without parents, childs and motion links)
	for (i=0; i<numNodes; i++)
	{
		// retrieve the current node from the character part actor to add
		Node* node = partToAdd->GetNode(i);

		// add the current node to the actor
		IntegrateNode(actor, node, partToAdd, replaceNodes, false);
	}


	// iterate through all level of details and clone the materials if they aren't in the resulting actor already
	CopyMaterials(partToAdd, actor);


	// if given update and relink the hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		actor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		actor->Update(0.0f, true);
		//actor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// integrate node to a character
void CharacterFactory::IntegrateNode(Actor* actor, Node* nodeToIntegrate, Actor* integrationNodeActor, const bool replaceNodes, const bool relinkHierarchy)
{
	// check if the actors and the mesh node are valid
	if (!actor || !nodeToIntegrate || !integrationNodeActor)
		return;


	// get the material offset for the following mesh nodes
	int materialOffset = actor->GetNumMaterials(0);

	Node*	found		= NULL;
	int		foundIndex	= -1; 

	// search if the given node is already in the assembled actor
	const int numNodes = actor->GetNumNodes();
	for (int j=0; j<numNodes; j++)
	{
		// retrieve the current search node
		Node* searchNode = actor->GetNode(j);

		// compare the name (id) of the given node with the name(id) of the current search node
		if (nodeToIntegrate->GetID() == searchNode->GetID())
		{
			// the node is already in the actor, break the loop
			found		= searchNode;
			foundIndex	= j;
			break;
		}
	}


	// if the given node isn't in the assembled actor then clone and add the node to the actor
	if (!found)
	{
		// clone the current node
		Node* clone = nodeToIntegrate->Clone( actor, (Node::ENodeCloneFlags)Node::NODE_CLONE_DEFAULT );

		// set the clone's parent to the original node's one of the other actor. in this case it is safe, because
		// that link will be only used to get the parent name for relinking hierarchy to that we can find the parent node
		// of this actor by name(using the name of the parent from the other actor)
		clone->SetParent( nodeToIntegrate->GetParent() );

		// add it to the actor
		actor->AddNode(clone);

		// set the material offset for the new node
		clone->SetMaterialOffset(materialOffset);

		//LOG("  - CharacterFactory::IntegrateNode: #AddingNewNode: Node=%s Parent=%s, matOffset=%i", clone->GetName().AsChar(), clone->GetParent()->GetName().AsChar(), materialOffset);
	}
	else
	{
		// if the node already exists in the main actor we simply replace the meshes of the existing one with the one from the character part we add
		if (found->HasMesh() || nodeToIntegrate->HasMesh())
		{
			if (replaceNodes)
			{
				//LOG("  - CharacterFactory::IntegrateNode: #ReplacingMeshes: Node=%s Parent=%s", found->GetName().AsChar(), found->GetParent()->GetName().AsChar());
				ReplaceMesh(actor, found, integrationNodeActor, nodeToIntegrate);
			}
		}
	}


	// if given update and relink the actor hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		actor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		actor->Update(0.0f, true);
		//actor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// remove a given character part from an extisting actor
void CharacterFactory::RemovePart(Actor* mainActor, Actor* partToRemove, const bool relinkHierarchy)
{
	// check if the mainActor and the node are valid
	if (!mainActor || !partToRemove)
		return;


	// retrieve the number of nodes
	const int numNodes = partToRemove->GetNumNodes();

	// iterate through all nodes from the mainActor part to remove
	for (int i=0; i<numNodes; i++)
	{
		// retrieve the current node from the character part mainActor to remove.
		Node* node = partToRemove->GetNode(i);

		// remove the current node
		RestoreNode(mainActor, node, false);
	}


	// if given update and relink the hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		mainActor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		mainActor->Update(0.0f, true);
		//mainActor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// restore node from a character
void CharacterFactory::RestoreNode(Actor* actor, Node* nodeToRestore, const bool relinkHierarchy)
{
	// check if the actor and the mesh node are valid
	if (!actor || !nodeToRestore)
		return;


	// search if the given node is in the assembled actor
	Node* found = NULL;
	int foundIndex = -1;
	const int numNodes = actor->GetNumNodes();
	for (int j=0; j<numNodes; j++)
	{
		// retrieve the current search node
		Node* searchNode = actor->GetNode(j);

		// compare the name(id) of the given node with the name(id) of the current search node
		if (nodeToRestore->GetID() == searchNode->GetID())
		{
			// the node is in the actor, break the loop
			found = searchNode;
			foundIndex = j;
			break;
		}
	}

	// if the given node contains a mesh delete it
	// since we cannot delete any node from the skeleton, because we don't know if it will be used by
	// any skinned vertex or if it would destroy the skeleton
	if (found)
	{
		/*// delete the node if it is a mesh node and is not embedded in the hierarchy, so has no childs
		if (removeNodes)
		{
			//LOG("CharacterFactory::RemovePart: Removing Node=%s numChilds=%i", found->GetName().AsChar(), found->GetNumChilds());

			// also remove the node from the actor child list
			actor->RemoveChild( actor->GetNode(foundIndex) );

			// remove the node from the actor
			actor->RemoveNode(foundIndex);

			// remove all materials which aren't used by the actor
			actor->RemoveUnusedMaterials();
		}
		else
		{*/
			if (found->HasMesh())
			{
				//LOG("CharacterFactory::RemovePart: Removing Mesh From Node=%s numChilds=%i", found->GetName().AsChar(), found->GetNumChilds());

				// remove all meshes and collision meshes including their deformer stacks
				found->RemoveCollisionMeshes();
				found->RemoveMeshes();

				// remove all materials which aren't used by the actor
				actor->RemoveUnusedMaterials();
			}
		//}
	}


	// if given update and relink the hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		actor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		actor->Update(0.0f, true);
		//actor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// replace an existing character part with another one
void CharacterFactory::ReplacePart(Actor* mainActor, Actor* replaceWith, const bool relinkHierarchy)
{
	// check if the mainActor and the node are valid
	if (!mainActor || !replaceWith)
		return;


	// retrieve the number of nodes from the replace character part
	const int numNodes = replaceWith->GetNumNodes();

	int i;
	// iterate through all nodes of the actor of which we need to replace the nodes
	for (i=0; i<numNodes; i++)
	{
		// retrieve the current node from the replace with character part
		Node* replaceWithNode = replaceWith->GetNode(i);

		// search if the given node is in the assembled actor
		Node* found = NULL;
		int foundIndex = -1;
		const int numNodes = mainActor->GetNumNodes();
		for (int j=0; j<numNodes; j++)
		{
			// retrieve the current search node
			Node* searchNode = mainActor->GetNode(j);

			// compare the name(id) of the given node with the name(id) of the current search node
			if (replaceWithNode->GetID() == searchNode->GetID())
			{
				// the node is in the actor, break the loop
				found = searchNode;
				foundIndex = j;
				break;
			}
		}

		if(found)
		{
			// if we have found a node from the replacing actor in the main actor replace the meshes
			if (found->HasMesh() || replaceWithNode->HasMesh())
				ReplaceMesh(mainActor, found, replaceWith, replaceWithNode);
		}
	}


	// if given update and relink the hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		mainActor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		mainActor->Update(0.0f, true);
		//mainActor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// replace the mesh of a node by a mesh from another mesh node
void CharacterFactory::ReplaceMesh(Actor* mainActor, Node* meshToReplace, Actor* replaceWithActor, Node* replaceWith, const bool relinkHierarchy)
{
	// check if the actors and nodes are valid
	if (!mainActor || !meshToReplace || !replaceWithActor || !replaceWith)
		return;

	// remove all meshes and their deformer stacks from the node
	meshToReplace->RemoveMeshes();
	meshToReplace->RemoveCollisionMeshes();

	// after removing the meshes of this node remove all unused materials which aren't used by the mainActor now anymore
	mainActor->RemoveUnusedMaterials();

	// get the new material offset for the following mesh node
	int materialOffset = mainActor->GetNumMaterials(0);

	// set the material offset of the mesh node
	meshToReplace->SetMaterialOffset(materialOffset);

	// iterate through all level of details and clone the materials if they aren't in the resulting mainActor already
	CopyMaterials(replaceWithActor, mainActor);

	// the nodes must have the same amount of meshes
	//assert(meshToReplace->GetNumMeshes() == replaceWith->GetNumMeshes());

	// retrieve the number of meshes (depending on number of level of details)
	int i;
	const int numMeshes = replaceWith->GetNumMeshes();
	for (i=0; i<numMeshes; i++)
	{
		// get the current mesh from the node to replace with
		Pointer<Mesh> mesh = replaceWith->GetMesh(i);

		// check if the mesh is valid
		if (!mesh)
			continue;

		// since there is always a NULL mesh, even if the node doesn't contain one we have to set
		// the first mesh directly and don't add it
		if (i==0)
			meshToReplace->SetMesh(mesh, i);
		else
			meshToReplace->AddLODMesh(mesh);
	}

	// clone the mesh deformer stacks from the replace main actor
	meshToReplace->CloneNodeStacksFromNode(replaceWith, mainActor);

	// retrieve the number of collision meshes (depending on number of level of details)
	const int numCollisionMeshes = replaceWith->GetNumCollisionMeshes();
	for (i=0; i<numCollisionMeshes; i++)
	{
		// get the current mesh from the node to replace with
		Pointer<Mesh> mesh = replaceWith->GetCollisionMesh(i);

		// check if the mesh is valid
		if (!mesh)
			continue;

		// since there is always a NULL mesh, even if the node doesn't contain one we have to set
		// the first mesh directly and don't add it
		if (i==0)
			meshToReplace->SetCollisionMesh(mesh, i);
		else
			meshToReplace->AddLODCollisionMesh(mesh);
	}

	// clone the collision mesh deformer stacks from the replace mainActor
	meshToReplace->CloneNodeCollisionStacksFromNode(replaceWith, mainActor);


	// if given update and relink the hierarchy
	if (relinkHierarchy)
	{
		// resolve the parent/child relationships
		mainActor->RelinkHierarchy();

		// update the character so that everything is up-to-date
		mainActor->Update(0.0f, true);
		//mainActor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);
	}
}


// copy materials from a given actor and add them to another one
void CharacterFactory::CopyMaterials(Actor* from, Actor* to)
{
	// retrieve the number of levels of detail
	const int numLODs = from->GetNumLODLevels();

	//LOG("CharacterFactory::AddPart: NumMaterials Before=%i", actor->GetNumMaterials(0));

	// iterate through all level of details and clone the materials if they aren't in the resulting actor already
	for (int i=0; i<numLODs; i++)
	{
		// retrieve the number of materials
		const int numMaterials = from->GetNumMaterials(i);

		// iterate through all materials of the part to add
		for (int j=0; j<numMaterials; j++)
		{
			// retrieve the current material from the character part to add
			Pointer<Material> material = from->GetMaterial(i, j);

			// clone the current material
			Pointer<Material> clone = material->Clone();

			// add the material to the resulting actor
			to->AddMaterial(i, clone);

			//LOG("CharacterFactory::AddPart: Adding material: name=%s", clone->GetName().AsChar());
		}
	}
	//LOG("CharacterFactory::AddPart: NumMaterials After=%i", actor->GetNumMaterials(0));
}


// update the mesh deformer stacks of the given actor
void CharacterFactory::UpdateDeformerStacks(Actor* actor)
{
	// retrieve the number of nodes
	const int numNodes = actor->GetNumNodes();

	// iterate through all nodes reinitializing their deformer stacks
	for (int i=0; i<numNodes; i++)
	{
		// retrieve the current node from the character part actor to add
		Node* node = actor->GetNode(i);

		// process all LOD levels and update the deformer stacks
		const int numMeshDeformerStacks = node->GetNumMeshDeformerStacks();
		int lod;
		for (lod=0; lod<numMeshDeformerStacks; lod++)
		{
			// retrieve the current mesh deformer stack
			MeshDeformerStack* meshDeformerStack = node->GetMeshDeformerStack(lod).GetPointer();

			// check if this deformer stack is valid
			if (!meshDeformerStack)
				continue;

			// reinitialize deformers
			meshDeformerStack->ReInitializeDeformers();
		}

		// process all LOD levels and update the collision mesh deformer stacks
		const int numCollisionMeshDeformerStacks = node->GetNumCollisionMeshDeformerStacks();
		for (lod=0; lod<numCollisionMeshDeformerStacks; lod++)
		{
			// retrieve the current deformer stack
			MeshDeformerStack* meshDeformerStack = node->GetCollisionMeshDeformerStack(lod).GetPointer();

			// check if this deformer stack is valid
			if (!meshDeformerStack)
				continue;

			// reinitialize deformers
			meshDeformerStack->ReInitializeDeformers();
		}
	}
}

/*
void CharacterFactory::UpdateSkinningInfo(Actor* actor)
{
	// retrieve the number of nodes
	const int numNodes = actor->GetNumNodes();

	// iterate through all nodes reinitializing their deformer stacks
	for (int i=0; i<numNodes; i++)
	{
		// retrieve the current node from the character part actor to add
		Node* node = actor->GetNode(i);

		// retrieve the number of meshes (depending on number of level of details)
		const int numMeshes = node->GetNumMeshes();

		int j;
		for (j=0; j<numMeshes; j++)
		{
			// get the current mesh from the node to replace with
			Pointer<Mesh> mesh = node->GetMesh(j);

			// check if the mesh is valid
			if (!mesh)
				continue;

			// get the skinning info attribute layer
			SkinningInfoVertexAttributeLayer* skinningLayer = (SkinningInfoVertexAttributeLayer*)mesh->FindSharedVertexAttributeLayer( SkinningInfoVertexAttributeLayer::TYPE_ID );

			// get the number of attributes which is the number of original vertices
			const int numOrgVerts = mesh->GetNumOrgVertices();
			for (int v=0; v<numOrgVerts; v++)
			{
				// retrieve the number of influences
				const int numInfluences = skinningLayer->GetNumInfluences(i);
				for (int a=0; a<numInfluences; a++)
				{
					SkinInfluence& influence = skinningLayer->GetInfluence(v, a);

					Node* node = influence.GetBone();

					//if (!node)
					//	continue;

					Node* newNode = actor->GetNodeByID(node->GetID());

					//if (newNode)
						influence.SetBone(newNode);

						LOG("oldID=%i newID=%i", node, newNode);
				}
			}
		}


		// retrieve the number of collision meshes (depending on number of level of details)
		const int numCollisionMeshes = actor->GetNumCollisionMeshes();

		for (j=0; j<numCollisionMeshes; j++)
		{
			// get the current mesh from the node to replace with
			Pointer<Mesh> mesh = actor->GetCollisionMesh(j);

			// check if the mesh is valid
			if (!mesh)
				continue;

		}
	}
}*/

} // namespace EMotionFX