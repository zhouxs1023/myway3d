#include "stdafx.h"

#include "GameComp.h"
#include "GameActor.h"
#include "GameAI.h"

GmCompMove::GmCompMove()
{
}

GmCompMove::~GmCompMove()
{
}

void GmCompMove::SetTargetPos(const Vec3 & pos)
{
	mTargetPos = pos;
}

const Vec3 & GmCompMove::GetTargetPos()
{
	return mTargetPos;
}

void GmCompMove::SetObject(GmObj * obj)
{
	GmComponent::SetObject(obj);

	d_assert (RTTI_KindOf(GmActor, mObject));

	mTargetPos = obj->GetPosition();
}

void GmCompMove::Update(float time)
{
	GmActor * pActor = (GmActor *)mObject;

	Vec3 curPos = pActor->GetPosition();
	Vec3 tarPos = mTargetPos;
	float speed = pActor->GetMoveSpeed() * time;

	if (mTargetPos.Distance(curPos) <= speed)
	{
		pActor->SetPosition(mTargetPos);
	}
	else
	{
		Vec3 dir = tarPos - curPos;
		dir.NormalizeL();

		pActor->SetPosition(curPos + dir * speed);
	}
}






GmCompRotate::GmCompRotate()
{
}

GmCompRotate::~GmCompRotate()
{
}

void GmCompRotate::SetTargetDir(float dir)
{
	mTargetDir = dir;
}

float GmCompRotate::GetTargetDir()
{
	return mTargetDir;
}

void GmCompRotate::SetObject(GmObj * obj)
{
	GmComponent::SetObject(obj);

	d_assert (RTTI_KindOf(GmActor, mObject));

	mTargetDir = ((GmActor *)mObject)->GetDirection();
}

void GmCompRotate::Update(float time)
{
	GmActor * pActor = (GmActor *)mObject;

	float curDir = pActor->GetDirection();
	float tarDir = mTargetDir;
	float speed = pActor->GetRotateSpeed() * time;
	float dir = curDir;

	if (tarDir > curDir + 180)
	{
		curDir += 360;
	}

	if (curDir > tarDir + 180)
	{
		tarDir += 360;
	}

	float dk = tarDir - curDir;

	if (Math::Abs(dk) < speed)
	{
		dir = mTargetDir;
	}
	else
	{
		if (dk > 0)
			dk = 1;
		else
			dk = -1;

		dir += dk * speed;
	}

	pActor->SetDirection(dir);
}