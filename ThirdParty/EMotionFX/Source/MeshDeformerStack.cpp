/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MeshDeformerStack.h"
#include "Mesh.h"
#include "Actor.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
MeshDeformerStack::MeshDeformerStack(Mesh* mesh) : mMesh(mesh)
{
}


// destructor
MeshDeformerStack::~MeshDeformerStack()
{
	// iterate through the deformers and delete their objects
	while (mDeformers.GetLength())
	{
		delete mDeformers.GetLast();
		mDeformers.RemoveLast();
	}

	// reset
	mMesh = NULL;
}


// returns the mesh
Mesh* MeshDeformerStack::GetMesh() const
{
	return mMesh;
}


void MeshDeformerStack::Update(Actor* actor, Node* node, const double timeDelta)
{
	// if we have deformers in the stack
	const int numDeformers = mDeformers.GetLength();
	if (numDeformers > 0)
	{
		// reset all output vertex data to the original vertex data
		mMesh->ResetToOriginalData();

		// iterate through the deformers and update them
		for (int i=0; i<numDeformers; i++)
			if (mDeformers[i]->IsEnabled())
				mDeformers[i]->Update(actor, node, timeDelta);
	}
}


// reinitialize mesh deformers
void MeshDeformerStack::ReInitializeDeformers()
{
	// if we have deformers in the stack
	const int numDeformers = mDeformers.GetLength();

	// iterate through the deformers and reinitialize them
	for (int i=0; i<numDeformers; i++)
		mDeformers[i]->ReInitialize();
}


void MeshDeformerStack::AddDeformer(MeshDeformer* meshDeformer)
{
	// add the object into the stack
	mDeformers.Add(meshDeformer);
}


void MeshDeformerStack::InsertDeformer(const int pos, MeshDeformer* meshDeformer)
{
	// add the object into the stack
	mDeformers.Insert(pos, meshDeformer);
}


bool MeshDeformerStack::RemoveDeformer(MeshDeformer* meshDeformer)
{
	// delete the object
	return mDeformers.Remove(meshDeformer);
}


MeshDeformerStack* MeshDeformerStack::Clone(Mesh* mesh, Actor* actor)
{
	// create the clone passing the mesh pointer
	MeshDeformerStack* newStack = new MeshDeformerStack(mesh);

	// clone all deformers
	const int numDeformers = mDeformers.GetLength();
	for (int i=0; i<numDeformers; i++)
		newStack->AddDeformer( mDeformers[i]->Clone(mesh, actor) );

	// return a pointer to the clone
	return newStack;
}


int MeshDeformerStack::GetNumDeformers() const
{
	return mDeformers.GetLength();
}


MeshDeformer* MeshDeformerStack::GetDeformer(const int nr) const
{
	assert(nr<mDeformers.GetLength());
	return mDeformers[nr];
}


// remove all the deformers of a given type
int MeshDeformerStack::RemoveAllDeformersByType(const int deformerTypeID)
{
	int numRemoved = 0;
	for (int a=0; a<mDeformers.GetLength();)
	{
		MeshDeformer* deformer = mDeformers[a];
		if (deformer->GetType() == deformerTypeID)
		{
			RemoveDeformer( deformer );
			delete deformer;
			numRemoved++;
		}
		else
			a++;
	}

	return numRemoved;
}


// remove all the deformers
void MeshDeformerStack::RemoveAllDeformers()
{
	for (int i=0; i<mDeformers.GetLength(); i++)
	{
		// retrieve the current deformer
		MeshDeformer* deformer = mDeformers[i];

		// remove the deformer
		RemoveDeformer( deformer );
		delete deformer;
	}
}


// enabled or disable all controllers of a given type
int MeshDeformerStack::EnableAllDeformersByType(const int deformerTypeID, const bool enabled)
{
	int numChanged = 0;
	const int numDeformers = mDeformers.GetLength();
	for (int a=0; a<numDeformers; a++)
	{
		MeshDeformer* deformer = mDeformers[a];
		if (deformer->GetType() == deformerTypeID)
		{
			deformer->SetEnabled( enabled );
			numChanged++;
		}
	}

	return numChanged;
}


// check if the stack contains a deformer of a specified type
bool MeshDeformerStack::HasDeformerOfType(const int deformerTypeID) const
{
	const int numDeformers = mDeformers.GetLength();
	for (int a=0; a<numDeformers; a++)
	{
		if (mDeformers[a]->GetType() == deformerTypeID)
			return true;
	}

	return false;
}


} // namespace EMotionFX