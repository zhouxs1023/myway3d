#pragma once

#include "Shape.h"

class Helper : public Shape
{
	DeclareRTTI();

public:
	Vec3 Position;
	float Radius;
	TString128 UserString;

public:
	Helper(const TString128 & name);
	virtual ~Helper();

	virtual TString128 GetTypeName() { return "Helper"; }

	virtual void SetName(const TString128 & name);

	virtual Shape * Clone();
	virtual bool IsSceneNode(SceneNode * node);

	virtual void SetPosition(const Vec3 & p);
	virtual void SetScale(float s);
	virtual void SetUserString(const TString128 & str);

	virtual Vec3 GetPosition() { return Position; }
	virtual float GetScale() { return Radius; }
	virtual TString128 GetUserString() { return UserString; }

	virtual void Serialize(xSerializer & serializer);

	virtual Aabb GetBound();
	virtual ColMesh * GetColMesh();

	virtual bool OnPropertyChanged(const Property * p);

protected:
	Entity * mEntity;
	SceneNode * mNode;
};


class HelperFactory : public ShapeFactory
{
public:
	HelperFactory() {};
	virtual ~HelperFactory() {};

	virtual Shape * Create(const char * name) { return new Helper(name); }

	virtual const char * GetGroupName()		{ return "Game"; }
	virtual const char * GetTypeName()		{ return "Helper"; }
	virtual const char * GetExternName()	{ return "Helper"; }
};