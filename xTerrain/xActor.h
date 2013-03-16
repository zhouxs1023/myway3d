#pragma once

#include "xObj.h"
#include "xApp.h"
#include "MActorInclude.h"

class xActor : public xObj
{
	DECLARE_PROPERTY(xObj);

public:
	TString128 ActorFile;
	Vec3 Position;
	Quat Orientation;
	float Scale;

public:
	xActor(const TString128 & name);
	virtual ~xActor();

	virtual TString128 GetTypeName() { return "Actor"; }

	virtual void SetName(const TString128 & name);
	virtual void SetActorFile(const TString128 & ActorFile);

	virtual xObj * Clone();
	virtual bool IsSceneNode(SceneNode * node);

	virtual void SetPosition(const Vec3 & p);
	virtual void SetOrientation(const Quat & q);
	virtual void SetScale(float s);

	virtual Vec3 GetPosition() { return Position; }
	virtual Quat GetOrientation() { return Orientation; }
	virtual float GetScale() { return Scale; }

	virtual void Serialize(xSerializer & serializer);

	virtual Aabb GetBound();

	virtual bool OnPropertyChanged(const Property * p);

protected:
	SceneNode * mNode;
	MActor * mActor;
};

class xActorFactory : public xObjFactory
{
public:
	xActorFactory() {};
	virtual ~xActorFactory() {};

	virtual xActor * Create(const char * name) { return new xActor(name); }

	virtual const char * GetGroupName()		{ return "Entity"; }
	virtual const char * GetTypeName()		{ return "Actor"; }
	virtual const char * GetExternName()	{ return "Actor"; }
};

class xActorFactoryListener
{
public:
	xActorFactoryListener()
		: OnInit(xEvent::OnInit, this, &xActorFactoryListener::_Init)
		, OnDragFile(xEvent::OnDragFile, this, &xActorFactoryListener::_OnDragFile)
	{
	}

	virtual ~xActorFactoryListener()
	{
	}

	void _Init(Event * sender)
	{
		xObjManager::Instance()->AddFactory(new xActorFactory());
	}

	void _OnDragFile(Event * sender);

	tEventListener<xActorFactoryListener> OnInit;
	tEventListener<xActorFactoryListener> OnDragFile;
};
