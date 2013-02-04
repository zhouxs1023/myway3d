#pragma once

#include "xObj.h"
#include "xApp.h"
#include "MActorInclude.h"

class xPrefab : public xObj
{
	DECLARE_PROPERTY(xObj);

public:
	TString128 PrefabFile;
	Vec3 Position;
	Quat Orientation;
	float Scale;

public:
	xPrefab(const TString128 & name);
	~xPrefab();

	virtual TString128 GetTypeName() { return "Prefab"; }

	virtual void SetName(const TString128 & name);
	virtual void SetPrefabFile(const TString128 & PrefabFile);

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
	MPrefab * mPrefab;
};






class xPrefabFactory : public xObjFactory
{
public:
	xPrefabFactory() {};
	virtual ~xPrefabFactory() {};

	virtual xPrefab * Create(const char * name) { return new xPrefab(name); }

	virtual const char * GetGroupName()		{ return "Build"; }
	virtual const char * GetTypeName()		{ return "Prefab"; }
	virtual const char * GetExternName()	{ return "Prefab"; }
};

class xPrefabFactoryListener
{
public:
	xPrefabFactoryListener()
		: OnInit(&xEvent::OnInit, this, &xPrefabFactoryListener::_Init)
		, OnDragFile(&xEvent::OnDragFile, this, &xPrefabFactoryListener::_OnDragFile)
	{
	}

	virtual ~xPrefabFactoryListener()
	{
	}

	void _Init(void * param0, void * param1)
	{
		xObjManager::Instance()->AddFactory(new xPrefabFactory());
	}

	void _OnDragFile(void * param0, void * param1);

	tEventListener<xPrefabFactoryListener> OnInit;
	tEventListener<xPrefabFactoryListener> OnDragFile;
};
