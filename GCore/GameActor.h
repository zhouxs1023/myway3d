#pragma once

#include "GameObject.h"
#include "GameEntity.h"

class GmCompAI;

class GCORE_ENTRY GmActor : public GmObj
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
	GmActor();
	virtual ~GmActor();

	virtual void Init();

	// AI
	//
	GmCompAI * GetAGmComponent();
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

	
