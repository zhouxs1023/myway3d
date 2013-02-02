/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include <limits>
#include "FacialMotion.h"
#include "ExpressionPart.h"
#include "Actor.h"
#include "FacialSetup.h"

// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// constructor
FacialMotion::FacialMotion(const String& name) : Motion(name)
{
}


// destructor
FacialMotion::~FacialMotion()
{
	// delete all expression motion parts
	while (mExpMotionParts.GetLength())
	{
		delete mExpMotionParts.GetLast();
		mExpMotionParts.RemoveLast();
	}

	// delete all phoneme motion parts
	while (mPhoMotionParts.GetLength())
	{
		delete mPhoMotionParts.GetLast();
		mPhoMotionParts.RemoveLast();
	}
}


// returns the type ID of the motion
int FacialMotion::GetType() const
{
	return FacialMotion::TYPE_ID;
}


// returns the type as string
const char* FacialMotion::GetTypeString() const
{
	return "FacialMotion";
}


// restore all touched nodes inside the actor to their original transformations
void FacialMotion::RestoreActor(Actor* actor)
{
	// check if we have a facial setup
	FacialSetup* facialSetup = actor->GetFacialSetup( actor->GetCurrentLOD() ).GetPointer();
	if (facialSetup == NULL)
		return;

	// process all expression parts
	const int numExpressionParts = facialSetup->GetNumExpressionParts();
	int i;
	for (i=0; i<numExpressionParts; i++)
		facialSetup->GetExpressionPart(i)->RestoreActor( actor );

	// process all phonemes
	const int numPhonemes = facialSetup->GetNumPhonemes();
	for (i=0; i<numPhonemes; i++)
		facialSetup->GetPhoneme(i)->RestoreActor( actor );
}


// create the motion links
void FacialMotion::CreateMotionLinks(Actor* actor, MotionInstance* instance)
{
	// check if we have a facial setup
	FacialSetup* facialSetup = actor->GetFacialSetup(0).GetPointer();
	if (facialSetup == NULL)
		return;

	// first build a list of nodes which are influenced by all expression parts
	MCore::Array<Node*> nodes;

	// traverse all nodes
	int numNodes = actor->GetNumNodes();
	int n;
	for (n=0; n<numNodes; n++)
	{
		Node* curNode = actor->GetNode( n );

		// traverse all expression parts
		const int numExpressionParts = facialSetup->GetNumExpressionParts();
		int i;
		for (i=0; i<numExpressionParts; i++)
		{
			// if the expression part influences this node
			if (facialSetup->GetExpressionPart(i)->Influences( curNode ))
			{
				// if we haven't already added the current node, add it
				if (!nodes.Contains( curNode ))
					nodes.Add( curNode );
			}
		}

		// traverse all phonemes
		const int numPhonemes = facialSetup->GetNumPhonemes();
		for (i=0; i<numPhonemes; i++)
		{
			// if the phoneme influences this node
			if (facialSetup->GetPhoneme(i)->Influences( curNode ))
			{
				// if we haven't already added the current node, add it
				if (!nodes.Contains( curNode ))
					nodes.Add( curNode );
			}
		}
	}

	// so now we have a list of nodes which are influenced by this facial motion
	// now we can create motion links inside these nodes
	numNodes = nodes.GetLength();
	for (n=0; n<numNodes; n++)
	{
		// add the motion link, with a NULL motion part, since we don't use the motion part for this motion				
		nodes[n]->AddMotionLink( MotionLink(NULL, instance) );
		//LOG("Adding motion link for node %s", nodes[n]->GetNamePtr());
	}
}


// get the maximum time value and update the stored one
void FacialMotion::UpdateMaxTime()
{
	// check expression tracks for max value
	int i;
	int numExpParts = mExpMotionParts.GetLength();
	for (i=0; i<numExpParts; i++)
	{
		FacialMotionPart* motionPart = mExpMotionParts[i];
		KeyTrack<float, MCore::Compressed8BitFloat>* keytrack = motionPart->GetKeyTrack();

		// check the two time values and return the biggest one
		mMaxTime = Max(mMaxTime, keytrack->GetLastTime());
	}

	// check phoneme tracks for max value
	int numPhoParts = mPhoMotionParts.GetLength();
	for (i=0; i<numPhoParts; i++)
	{
		FacialMotionPart* motionPart = mPhoMotionParts[i];
		KeyTrack<float, MCore::Compressed8BitFloat>* keytrack = motionPart->GetKeyTrack();

		// check the two time values and return the biggest one
		mMaxTime = Max(mMaxTime, keytrack->GetLastTime());
	}
}



// the main function which returns the new node transformation after this motion has been processed
NodeTransform FacialMotion::CalcNodeTransform(Actor* actor, Node* node, const float timeValue)
{
	// get the current local transformation of the node
	Vector3		position = node->GetLocalPos();
	Quaternion	rotation = node->GetLocalRot();
	Vector3		scale	 = node->GetLocalScale();

	// check if we have a facial setup
	FacialSetup* facialSetup = actor->GetFacialSetup( actor->GetCurrentLOD() ).GetPointer();
	if (facialSetup != NULL)
	{
		int i;
		// pass them through all expression parts
		const int numExpressionParts = facialSetup->GetNumExpressionParts();
		for (i=0; i<numExpressionParts; i++)
		{			
			ExpressionPart* part = facialSetup->GetExpressionPart(i);

			// find the motion part number with the given ID
			int motionPartNr = FindExpMotionPart( part->GetID() );
			if (motionPartNr == -1)
				continue;

			// get the keytrack
			KeyTrack<float, Compressed8BitFloat>* keyTrack = mExpMotionParts[motionPartNr]->GetKeyTrack();

			// update the weight and apply the transformation
			// don't process expresion parts that are controlled manually
			if (part->IsInManualMode() == false)
				part->SetWeight( keyTrack->GetValueAtTime( timeValue ) );

			// apply the transformation
			part->ApplyTransformation(actor, node, position, rotation, scale);
		}

		// pass them through all phonemes
		const int numPhonemes = facialSetup->GetNumPhonemes();
		for (i=0; i<numPhonemes; i++)
		{
			ExpressionPart* phoneme = facialSetup->GetPhoneme(i);

			// find the motion part number with the given ID
			int motionPartNr = FindPhoMotionPart( phoneme->GetID() );
			if (motionPartNr == -1)
				continue;

			// get the keytrack
			KeyTrack<float, MCore::Compressed8BitFloat>* keyTrack = mPhoMotionParts[motionPartNr]->GetKeyTrack();

			// update the weight and apply the transformation
			if (phoneme->IsInManualMode() == false)
				phoneme->SetWeight( keyTrack->GetValueAtTime( timeValue ) );

			// apply the transformation
			phoneme->ApplyTransformation(actor, node, position, rotation, scale);
		}
	}

	// construct the resulting node transformation
	NodeTransform result;
	result.mPosition = position;
	result.mRotation = rotation;
	result.mScale	 = scale;

	// return the resulting node transform
	return result;
}


// apply the facial deformations to the actor
void FacialMotion::Apply(Actor* actor, const bool updateWeights)
{
	// restore the actor
	RestoreActor( actor );

	// check if we have a facial setup
	FacialSetup* facialSetup = actor->GetFacialSetup( actor->GetCurrentLOD() ).GetPointer();
	if (facialSetup == NULL)
		return;

	// pass them through all expression parts
	const int numExpressionParts = facialSetup->GetNumExpressionParts();
	int i;
	for (i=0; i<numExpressionParts; i++)
	{
		ExpressionPart* expPart = facialSetup->GetExpressionPart(i);

		// if we need to extract and overwrite the weight value of the expression part
		if (updateWeights && (expPart->IsInManualMode() == false))
		{
			int motionPartNr = FindExpMotionPart( expPart->GetID() );

			if (motionPartNr != -1)
			{
				float weight = mExpMotionParts[motionPartNr]->GetKeyTrack()->GetCurrentValue();
				expPart->SetWeight( weight );
			}
		}

		// apply the expression part
		expPart->Apply(actor);
	}

	// pass them through all phonemes
	const int numPhonemes = facialSetup->GetNumPhonemes();
	for (i=0; i<numPhonemes; i++)
	{
		ExpressionPart* expPart = facialSetup->GetPhoneme(i);

		// if we need to extract and overwrite the weight value of the expression part
		if (updateWeights && (expPart->IsInManualMode() == false))
		{
			int motionPartNr = FindPhoMotionPart( expPart->GetID() );

			if (motionPartNr != -1)
			{
				float weight = mPhoMotionParts[motionPartNr]->GetKeyTrack()->GetCurrentValue();
				expPart->SetWeight( weight );
			}
		}

		// apply the expression part
		expPart->Apply(actor);
	}
}


void FacialMotion::AddExpMotionPart(FacialMotionPart* motionPart )
{
	mExpMotionParts.Add( motionPart );
}


void FacialMotion::RemoveExpMotionPart(const int nr)
{
	delete mExpMotionParts[nr];
	mExpMotionParts.Remove(nr);
}


int FacialMotion::FindExpMotionPart(const int id) const
{
	// find the motion part with the specified ID
	const int numParts = mExpMotionParts.GetLength();
	for (int i=0; i<numParts; i++)
		if (mExpMotionParts[i]->GetID() == id)
			return i;

	// not found
	return -1;
}


FacialMotionPart* FacialMotion::GetExpMotionPart(const int nr)
{
	return mExpMotionParts[nr];
}


//-------

void FacialMotion::AddPhoMotionPart(FacialMotionPart* motionPart)
{
	mPhoMotionParts.Add( motionPart );
}


void FacialMotion::RemovePhoMotionPart(const int nr)
{
	delete mPhoMotionParts[nr];
	mPhoMotionParts.Remove(nr);
}


int FacialMotion::FindPhoMotionPart(const int id) const
{
	// find the motion part with the specified ID
	const int numParts = mPhoMotionParts.GetLength();
	for (int i=0; i<numParts; i++)
		if (mPhoMotionParts[i]->GetID() == id)
			return i;

	// not found
	return -1;
}


FacialMotionPart* FacialMotion::GetPhoMotionPart(const int nr)
{
	return mPhoMotionParts[nr];
}


// returns if we want to overwrite nodes in non-mixing mode or not
bool FacialMotion::GetDoesOverwriteInNonMixMode() const
{
	return false;
}


void FacialMotion::MakeLoopable(const float fadeTime)
{
	// make the expression part tracks loopable
	int numParts = mExpMotionParts.GetLength();
	int i;
	for (i=0; i<numParts; i++)
		GetExpKeyTrack(i)->MakeLoopable( fadeTime );

	// make the phoneme tracks loopable
	numParts = mPhoMotionParts.GetLength();
	for (i=0; i<numParts; i++)
		GetPhoKeyTrack(i)->MakeLoopable( fadeTime );

	// update the maximum time
	UpdateMaxTime();
}


} // namespace EMotionFX