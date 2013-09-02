#pragma once

enum eUserPower
{
	POWER_Player,
	POWER_GM,
};

enum eActorType
{
	ACTOR_Unknown,
	ACTOR_Npc,
	ACTOR_Player,
};

enum eSex
{
	SEX_Female,
	SEX_Male,
};

enum eRace
{
	RACE_Human,			// 人族
	RACE_FoxEvil,		// 狐妖
	RACE_Ghost,			// 鬼仙
	RACE_Spirit,		// 精灵
};

enum eEquipment
{
	EQUIP_Weapon,		// 武器
	EQUIP_Helmet,		// 头盔
	EQUIP_Shoulder,		// 肩膀
	EQUIP_Clothes,		// 衣服
	EQUIP_Shoes,		// 鞋子

	EQUIP_Max,
};

#define PLAYER_ID_MAGIC 0x80000000

struct PlayerViewInfo
{
	TString32 Name;			// 名字
	int Sex;				// 性别  --- eSex
	int Race;				// 种族  --- eRace

	int ActorId;			// 角色Id

	int HP;					// 生命值
	int MP;					// 魔法值

	//int Equip[EQUIP_Max];	// 身上的装备

	Vec3 Position;			// 位置
	float Direction;		// 方向
};


struct NpcViewInfo
{
	int ActorId;
	int TypeId;

	int HP;
	int MP;

	Vec3 Position;
	float Direction;

	NpcViewInfo()
		: ActorId(-1)
		, TypeId(-1)
		, HP(0)
		, MP(0)
		, Position(0, 0, 0)
		, Direction(0)
	{
	}
};

enum eActorState
{
	STATE_Idle,

	STATE_Fight,
};