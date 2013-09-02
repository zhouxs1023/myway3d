#include "stdafx.h"

#include "SelectRoleMode.h"
#include "Pack_PlayerEnterCL.h"
#include "MainMode.h"

cSelectRoleMode::cSelectRoleMode()
{
}

cSelectRoleMode::~cSelectRoleMode()
{
}

void cSelectRoleMode::Init()
{
	PlayerViewInfo pi;

	pi.Name = "Myway";
	pi.Sex = SEX_Female;

	pi.ActorId = PLAYER_ID_MAGIC | 1;

	pi.Position = Vec3(1964.325f, 1312.647f, 3060.589f);
	pi.Direction = 0;

	pi.HP = 100;
	pi.MP = 100;

	GmRoot::Instance()->SetMode(new cMainMode(pi));
}

void cSelectRoleMode::Shutdown()
{
}