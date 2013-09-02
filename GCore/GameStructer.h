#pragma once

#include "GameCoreEntry.h"

struct GCORE_ENTRY GmNpcInfo
{
	int id;
	TString32 name;

	TString32 part_main;
	TString32 part_weapon;
	TString32 part_helmet;
	TString32 part_shoulder;
	TString32 part_clothes;
	TString32 part_shoes;

	TString32 anim_Idle0;
	TString32 anim_Idle1;
	TString32 anim_Walk;
	TString32 anim_Run;

	GmNpcInfo()
	{
		id = -1;
	}
};

struct GCORE_ENTRY GmMapInfo
{
	int				MapId;
	TString32		Name;
	TString128		MapSource;
};