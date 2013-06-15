#pragma once

#include "GameObject.h"
#include "GameEntity.h"

class GameComponent_AI;

class GCORE_ENTRY GameActor : public IGameObject, public IGameEntity
{
	DeclareRTTI();

public:
	enum PartType
	{
		Main,

		Weapon,
		Helmet,
		Shoulder,
		Clothes,
		Shoes,

		PT_Max,
	};

	struct ActorPart
	{
		int id;
		char mesh[32];
	};

public:
	GameActor();
	virtual ~GameActor();

	virtual void Init();

	// AI
	//
	GameComponent_AI * GetAIComponent();
	virtual void Idle();
	virtual void MoveTo(const Vec3 & pos);

	// »»×°
	//
	void SetPart(PartType type, const char * mesh);

	// ¶¯×÷
	//
	void PlayAnimation(const char * anim, const MotionBlendInfo & mbi);

protected:
	Actor * mActor[PT_Max];
};

	
