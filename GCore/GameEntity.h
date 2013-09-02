#pragma once

#include "GameCoreEntry.h"

class GCORE_ENTRY GmEntity
{
	DECLARE_ALLOC();

public:
	GmEntity();
	virtual ~GmEntity();

	void SetPosition(const Vec3 & v);
	void SetPosition(float x, float y, float z);
	Vec3 GetPosition() const;

	void SetOrientation(const Quat & q);
	void SetOrientation(float x, float y, float z, float w);
	Quat GetOrientation() const;

	void SetScale(float x);
	float GetScale();

	void SetVisible(bool b);
	bool IsVisible();

	Node * GetNode() { return mNode; }

protected:
	Node * mNode;
};

