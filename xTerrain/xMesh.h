#pragma once

#include "xObj.h"
#include "xApp.h"


class xMesh : public xObj
{
    DECLARE_PROPERTY(xObj);

public:
	TString128 MeshFile;
	Vec3 Position;
	Quat Orientation;
	Vec3 Scale;

public:
    xMesh(const TString128 & name);
    virtual ~xMesh();

	virtual TString128 GetTypeName() { return "Mesh"; }

	virtual void SetName(const TString128 & name);
	virtual void SetMeshFile(const TString128 & meshFile);

	virtual xObj * Clone();
	virtual bool IsSceneNode(SceneNode * node);

	virtual void SetPosition(const Vec3 & p);
	virtual void SetOrientation(const Quat & q);
	virtual void SetScale(const Vec3 & s);

	virtual Vec3 GetPosition() { return Position; }
	virtual Quat GetOrientation() { return Orientation; }
	virtual Vec3 GetScale() { return Scale; }

	virtual void Serialize(xSerializer & serializer);

	virtual Aabb GetBound();

    virtual bool OnPropertyChanged(const Property * p);

protected:
    SceneNode * mNode;
    Entity * mEntity;
};

class xMeshFactory : public xObjFactory
{
public:
	xMeshFactory() {};
	virtual ~xMeshFactory() {};

	virtual xMesh * Create(const char * name) { return new xMesh(name); }

	virtual const char * GetGroupName()		{ return "Entity"; }
	virtual const char * GetTypeName()		{ return "Mesh"; }
	virtual const char * GetExternName()	{ return "mesh"; }
};

class xMeshFactoryListener
{
public:
	xMeshFactoryListener()
		: OnInit(&xEvent::OnInit, this, &xMeshFactoryListener::_Init)
		, OnDragFile(&xEvent::OnDragFile, this, &xMeshFactoryListener::_OnDragFile)
	{
	}

	virtual ~xMeshFactoryListener()
	{
	}

	void _Init(void * param0, void * param1)
	{
		xObjManager::Instance()->AddFactory(new xMeshFactory());
	}

	void _OnDragFile(void * param0, void * param1);

	tEventListener<xMeshFactoryListener> OnInit;
	tEventListener<xMeshFactoryListener> OnDragFile;
};
