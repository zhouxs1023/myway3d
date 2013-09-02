#pragma once

#include "GameObject.h"
#include "GameEntity.h"
#include "GameComp.h"

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

	virtual void Update(float frameTime);


	// AI
	//
	GmCompAI * GetAIComponent();
	virtual void Idle();
	virtual void Run();

	GmCompMove * GetMoveComponent();
	GmCompRotate * GetRotateComponent();

	virtual void MoveTo(const Vec3 & pos);
	virtual void RotateTo(float dir);
	virtual void ForceMove(const Vec3 & pos);
	virtual void ForceRotate(float dir);

	// »»×°
	//
	void SetPart(PartType type, const char * mesh);

	// ¶¯×÷
	//
	void PlayAnimation(const char * anim, const MotionBlendInfo & mbi);


	//
	//
	void SetMoveSpeed(float speed) { mMoveSpeed = speed; }
	float GetMoveSpeed() const { return mMoveSpeed; }

	void SetRotateSpeed(float speed) { mRotateSpeed = speed; }
	float GetRotateSpeed() const { return mRotateSpeed; }

	void SetInitOrientation(const Quat & ort) { mInitOrt = ort; }
	const Quat & GetInitOrientation() const { return mInitOrt; }

	void SetDirection(float dir);
	float GetDirection() { return mDirection; }

protected:
	Entity * mEntity[PT_Max];

	GmCompAI * mCompAI;
	GmCompMove * mCompMove;
	GmCompRotate * mCompRotate;

	float mMoveSpeed;
	float mRotateSpeed;

	Quat mInitOrt;

	float mDirection;
};

	
