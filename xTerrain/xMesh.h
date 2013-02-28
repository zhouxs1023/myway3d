#pragma once

#include "xObj.h"
#include "xApp.h"


class xMesh : public xObj
{
	DECLARE_PROPERTY(xObj);

public:
	TString128 MeshFile;
	TString128 AnimName;
	Vec3 Position;
	Quat Orientation;
	float Scale;

public:
	xMesh(const TString128 & name);
	virtual ~xMesh();

	virtual TString128 GetTypeName() { return "Mesh"; }

	virtual void SetName(const TString128 & name);
	virtual void SetMeshFile(const TString128 & meshFile);
	virtual void SetAnimName(const TString128 & animName);

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
	void _Update(Event * sender);

	void _renderSkel(Event * sender);

protected:
	SceneNode * mNode;
	Entity * mEntity;
	AnimationState * mAnimState;

	tEventListener<xMesh> OnUpdate;
	tEventListener<xMesh> OnRenderSkel;
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

	void _Init(Event * sender)
	{
		xObjManager::Instance()->AddFactory(new xMeshFactory());
	}

	void _OnDragFile(Event * sender);

	tEventListener<xMeshFactoryListener> OnInit;
	tEventListener<xMeshFactoryListener> OnDragFile;
};


class xSkeletonRenderer
{
	DECLARE_SINGLETON (xSkeletonRenderer);

public:
	xSkeletonRenderer();
	~xSkeletonRenderer();

	void Render(Entity * entity);

protected:
	void _init(Event * sender);
	void _shutdown(Event * sender);

protected:
	Technique * mTech;
	RenderOp * mRenderOp;

	tEventListener<xSkeletonRenderer> OnInit;
	tEventListener<xSkeletonRenderer> OnShutdown;
};