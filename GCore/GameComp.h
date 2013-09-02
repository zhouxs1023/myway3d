#pragma once

#include "GameObject.h"

class GmCompMove : public GmComponent
{
public:
	GmCompMove();
	virtual ~GmCompMove();

	void SetTargetPos(const Vec3 & pos);
	const Vec3 & GetTargetPos();

	virtual void SetObject(GmObj * obj);
	virtual void Update(float time);

protected:
	Vec3 mTargetPos;
};



class GmCompRotate : public GmComponent
{
public:
	GmCompRotate();
	virtual ~GmCompRotate();

	void SetTargetDir(float dir);
	float GetTargetDir();

	virtual void SetObject(GmObj * obj);
	virtual void Update(float time);

protected:
	float mTargetDir;
};