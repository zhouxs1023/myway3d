#pragma once

#include "MActorEntry.h"
#include "MWMover.h"
#include "MActorRes.h"
#include "MMotion.h"

namespace Myway {

	class MACTOR_ENTRY MActor : public Mover
	{
		DECLARE_ALLOC();

	public:
		static const int K_MAX_PART = 32;
		static const int K_MAIN_PART = 0;

	public:
		MActor(const TString128 & name);
		virtual ~MActor();

		void SetPart(int part, MActorResPtr pActor);
		MActorResPtr GetPart(int part);

		virtual void UpdateGeometry();
		virtual void AddRenderQueue(RenderQueue * rq);

		bool PlayMotion(const TString128 & motionName, const MMotion::PlayInfo & info = MMotion::PlayInfo());

		EMotionFX::Actor * _GetMainActor() { return mMainActor; }

	protected:
		void _OnActorLoaded(void *, void *);
		void _OnMainPartBind();
		void _OnUpdateAnimtion();

	protected:
		EMotionFX::Actor * mMainActor;

		MActorResPtr mActorParts[K_MAX_PART];

		tEventListener<MActor> OnActorLoaded;
	};

}