#include "stdafx.h"

#include "Pack_PlayerEnterCL.h"
#include "User.h"
#include "World.h"

Pack_PlayerEnterCL::Pack_PlayerEnterCL()
{
	mPlayerId = -1;
}

void Pack_PlayerEnterCL::In(PkIStream & stream)
{
	stream.Read(&mPlayerId, sizeof(int));
}

void Pack_PlayerEnterCL::Execute(void * param)
{
	User * user = (User *)param;

	d_assert (user->GetState() == User::LOGIN);

	user->SetState(User::PLAY);

	svPlayer * player = new svPlayer(PLAYER_ID_MAGIC | 1, user);
	player->SetName("ºüÀê¾«");
	player->SetSex(SEX_Female);
	player->SetRace(RACE_FoxEvil);
	player->SetPosition(Vec3(0, 0, 0));
	player->SetDirection(0);
	player->SetHP(100);
	player->SetMP(100);

	svWorld::Instance()->AddActor(player);
	svWorld::Instance()->GetMapById(10001)->Enter(player);
}