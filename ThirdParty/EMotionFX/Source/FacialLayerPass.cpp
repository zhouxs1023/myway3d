/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include required headers
#include "FacialLayerPass.h"
#include "MotionLayerSystem.h"
#include "FacialMotion.h"
#include "Actor.h"
#include "FacialSetup.h"


// use the Core namespace
using namespace MCore;


namespace EMotionFX
{

// the constructor
FacialLayerPass::FacialLayerPass(MotionLayerSystem* motionLayerSystem) : LayerPass(motionLayerSystem)
{
}


// the destructor
FacialLayerPass::~FacialLayerPass() 
{
}


// get the layer pass type
int FacialLayerPass::GetType() const
{
	return FacialLayerPass::TYPE_ID;
}


// the main function that processes the pass
void FacialLayerPass::Process()
{
	// get the facial setup of the actor
	Actor* actor = mMotionSystem->GetActor();
	int curLOD = actor->GetCurrentLOD();
	FacialSetup* facialSetup = actor->GetFacialSetup( curLOD ).GetPointer();
	if (facialSetup == NULL) 
		return;

	// reset the weights
	const int numExpressionParts = facialSetup->GetNumExpressionParts();
	for (int i=0; i<numExpressionParts; i++)
	{
		// get the expression part
		ExpressionPart* expPart = facialSetup->GetExpressionPart( i );
		if (expPart->IsInManualMode()) continue;
		expPart->SetWeight( expPart->CalcZeroInfluenceWeight() );
	}

	const int numPhonemes = facialSetup->GetNumPhonemes();
	for (int i=0; i<numPhonemes; i++)
	{
		// get the expression part
		ExpressionPart* expPart = facialSetup->GetPhoneme( i );
		if (expPart->IsInManualMode()) continue;
		expPart->SetWeight( expPart->CalcZeroInfluenceWeight() );
	}

	// get the root layer of the motion system
	MotionLayer* rootLayer = mMotionSystem->GetMotionTree();
	if (rootLayer == NULL)
		return;


	// find the last layer
	MotionLayer* curLayer = rootLayer;
	while (curLayer->GetDestLayer() != NULL)
		curLayer = curLayer->GetDestLayer();


	// now curLayer contains the bottom layer
	// we can now work our way up back to the root layer again
	while (curLayer)
	{
		// get the motion instance from the layer
		MotionInstance* motionInstance = curLayer->GetSource();

		// get the weight from the motion instance
		const float weight = motionInstance->GetWeight();
		const float finalWeight = MCore::CalcCosineInterpolationWeight( weight );

		// if we are dealing with a facial motion here
		if (motionInstance->GetMotion()->GetType() == FacialMotion::TYPE_ID)
		{
			// get the facial motion we are dealing with
			FacialMotion* motion = (FacialMotion*)motionInstance->GetMotion();

			// for all expression parts
			const int numExpressionParts = facialSetup->GetNumExpressionParts();
			for (int i=0; i<numExpressionParts; i++)
			{
				// get the expression part
				ExpressionPart* expPart = facialSetup->GetExpressionPart( i );

				// don't process expresion parts that are controlled manually
				if (expPart->IsInManualMode()) continue;

				// get the weight for this expression part at the given playback times
				const int motionPartNr = motion->FindExpMotionPart( expPart->GetID() );
				float value;

				// if the motion part cannot be found, we 'disable' this slider
				if (motionPartNr == -1)
				{
					if (motionInstance->IsMixing())
						value = expPart->GetWeight();
					else
						value = expPart->CalcZeroInfluenceWeight();
				}
				else
				{
					// blend the weight
					KeyTrack<float, MCore::Compressed8BitFloat>* keyTrack = motion->GetExpKeyTrack( motionPartNr );
					value = keyTrack->GetValueAtTime( motionInstance->GetCurrentTime() );
				}

				// calculate and set the new slider weight
				const float inWeight  = expPart->GetWeight();
				const float newWeight = finalWeight * value + inWeight * (1.0f - finalWeight);
				expPart->SetWeight( newWeight );
			}


			// for all phonemes
			const int numPhonemes = facialSetup->GetNumPhonemes();
			for (int i=0; i<numPhonemes; i++)
			{
				// get the expression part
				ExpressionPart* expPart = facialSetup->GetPhoneme( i );

				// don't process expresion parts that are controlled manually
				if (expPart->IsInManualMode()) continue;

				// get the weight for this expression part at the given playback times
				const int motionPartNr = motion->FindPhoMotionPart( expPart->GetID() );
				float value;

				// if the motion part cannot be found, we 'disable' this slider
				if (motionPartNr == -1)
				{
					if (motionInstance->IsMixing())
						value = expPart->GetWeight();
					else
						value = expPart->CalcZeroInfluenceWeight();
				}
				else
				{
					// blend the weight
					KeyTrack<float, MCore::Compressed8BitFloat>* keyTrack = motion->GetPhoKeyTrack( motionPartNr );
					value = keyTrack->GetValueAtTime( motionInstance->GetCurrentTime() );
				}

				// calculate and set the new slider weight
				const float inWeight  = expPart->GetWeight();
				const float newWeight = finalWeight * value + inWeight * (1.0f - finalWeight);
				expPart->SetWeight( newWeight );
			}
		}
		else	// no facial motion
		{
			// for all expression parts
			const int numExpressionParts = facialSetup->GetNumExpressionParts();
			for (int i=0; i<numExpressionParts; i++)
			{
				// get the expression part
				ExpressionPart* expPart = facialSetup->GetExpressionPart( i );

				// don't process expresion parts that are controlled manually
				if (expPart->IsInManualMode()) continue;

				// calculate and set the new slider weight
				const float inWeight  = expPart->GetWeight();
				const float newWeight = inWeight * (1.0f - finalWeight);
				expPart->SetWeight( newWeight );
			}

			// for all phonemes
			const int numPhonemes = facialSetup->GetNumPhonemes();
			for (int i=0; i<numPhonemes; i++)
			{
				// get the expression part
				ExpressionPart* expPart = facialSetup->GetPhoneme( i );

				// don't process expresion parts that are controlled manually
				if (expPart->IsInManualMode()) continue;

				// calculate and set the new slider weight
				const float inWeight  = expPart->GetWeight();
				const float newWeight = inWeight * (1.0f - finalWeight);
				expPart->SetWeight( newWeight );
			}			
		}

		// go to the layer above the current one (towards the root layer)
		curLayer = curLayer->GetParent();
	}
}


} // namespace EMotionFX