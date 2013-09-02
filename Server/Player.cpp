#include "StdAfx.h"

#include "Player.h"
#include "Npc.h"

#include "Pack_AddNpcSV.h"

svPlayer::svPlayer(int _actorId, User * _user)
	: svActor(_actorId)
	, mUser(_user)
{
}

svPlayer::~svPlayer()
{
}

bool svPlayer::IsActorVisible(svActor * _actor)
{
	for (int i = 0; i < mVisibleActors.Size(); ++i)
	{
		if (mVisibleActors[i] == _actor)
			return true;
	}

	return false;
}

void svPlayer::AddVisibleActor(svActor * _actor)
{
	d_assert (!IsActorVisible(_actor));

	mVisibleActors.PushBack(_actor);

	// Send Message
	if (_actor->GetActorType() == ACTOR_Npc)
	{
		svNpc * npc = (svNpc *)_actor;

		NpcViewInfo viewInfo;
		viewInfo.ActorId = npc->GetActorId();
		viewInfo.TypeId = npc->GetTypeId();
		viewInfo.HP = npc->GetHP();
		viewInfo.MP = npc->GetMP();
		viewInfo.Position = npc->GetPosition();
		viewInfo.Direction = npc->GetDirection();

		Pack_AddNpcSV pack;

		pack.SetViewInfo(viewInfo);
	}
}

void svPlayer::RemoveVisibleActor(svActor * _actor)
{
	for (int i = 0; i < mVisibleActors.Size(); ++i)
	{
		if (mVisibleActors[i] == _actor)
		{
			// Send Message.

			mVisibleActors.Erase(i);
			return ;
		}
	}

	d_assert (0);
}