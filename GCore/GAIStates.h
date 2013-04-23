#pragma once

#include "GState.h"
#include "GActor.h"

namespace game {

	class GState_Idle : public GState
	{
	public:
		static const int K_Type = 'Idle';

	public:
		GState_Idle(GActor * actor);
		virtual ~GState_Idle();

		virtual int GetType() { return K_Type; }

		virtual void Enter(GAIEntity * entity);
		virtual void Exit(GAIEntity * entity);

		virtual void Think(GAIEntity * entity);


	protected:
		GActor * mActor;
		int mRandomIdleCount;
	};

}