#pragma once

#include "GCoreEntry.h"

namespace game {

	class GAIEntity;

	class GCORE_ENTRY GState
	{
	public:
		GState() {};
		virtual ~GState() {}

		virtual int GetType() = 0;

		virtual void Enter(GAIEntity * entity) = 0;
		virtual void Exit(GAIEntity * entity) = 0;

		virtual void Think(GAIEntity * entity) = 0;
	};


	class GCORE_ENTRY GAIEntity
	{
	public:
		static Event OnStateChanged;

	public:
		GAIEntity();
		virtual ~GAIEntity();

		virtual void Think();

		virtual int GetType() const = 0;
		virtual void ChangeState(GState * state);
		GState * GetState();

	protected:
		GState * mState;
	};
}