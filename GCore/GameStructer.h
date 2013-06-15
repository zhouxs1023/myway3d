#pragma once

#include "GameCoreEntry.h"

struct GCORE_ENTRY GameNpcInfo
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

	GameNpcInfo()
	{
		id = -1;
	}
};
