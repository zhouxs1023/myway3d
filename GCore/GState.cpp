#include "stdafx.h"

#include "GState.h"

namespace game {

	Event GAIEntity::OnStateChanged;

	GAIEntity::GAIEntity()
		: mState(NULL)
	{
	}

	GAIEntity::~GAIEntity()
	{
		safe_delete(mState);
	}

	void GAIEntity::Think()
	{
		if (mState)
		{
			mState->Think(this);
		}
	}

	void GAIEntity::ChangeState(GState * state)
	{
		if (mState)
		{
			d_assert (mState->GetType() != state->GetType());
		}

		if (mState)
			mState->Exit(this);

		safe_delete(mState);

		mState = state;

		mState->Enter(this);

		OnStateChanged(mState);
	}
}