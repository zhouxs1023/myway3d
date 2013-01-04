#pragma once

#include "xObj.h"
#include "xApp.h"

class xPointLight : public xObj
{
	DECLARE_PROPERTY(xObj);

public:
	Vec3 Position;
	Color4 Diffuse;
	Color4 Specular;
	float Range;

public:
	xPointLight(const TString128 & name);
	virtual ~xPointLight();

	virtual TString128 GetTypeName() { return "PointLight"; }

	virtual void SetName(const TString128 & name);

	virtual xObj * Clone();
	virtual bool IsSceneNode(SceneNode * node);

	virtual void SetPosition(const Vec3 & p);
	virtual Vec3 GetPosition() { return Position; }

	virtual void SetDiffuse(const Color4 & clr);
	virtual Color4 GetDiffuse() { return Diffuse; }

	virtual void SetSpecular(const Color4 & clr);
	virtual Color4 GetSpecular() { return Specular; }

	virtual void SetRange(float range);
	virtual float GetRange() { return Range; }

	virtual void Serialize(xSerializer & serializer);

	virtual Aabb GetBound();

	virtual bool OnPropertyChanged(const Property * p);

protected:
	SceneNode * mNode;
	Light * mLight;
};

class xPointLightFactory : public xObjFactory
{
public:
	xPointLightFactory() {};
	virtual ~xPointLightFactory() {};

	virtual xPointLight * Create(const char * name) { return new xPointLight(name); }

	virtual const char * GetGroupName()		{ return "Light"; }
	virtual const char * GetTypeName()		{ return "PointLight"; }
};


class xLightFactoryListener
{
public:
	xLightFactoryListener()
		: OnInit(&xEvent::OnInit, this, &xLightFactoryListener::_Init)
	{
	}

	virtual ~xLightFactoryListener()
	{
	}

	void _Init(void * param0, void * param1)
	{
		xObjManager::Instance()->AddFactory(new xPointLightFactory());
	}

	tEventListener<xLightFactoryListener> OnInit;
};
