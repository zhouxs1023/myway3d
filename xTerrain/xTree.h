#pragma once

#include "xObj.h"
#include "xApp.h"

//class xTree : public xObj
//{
//	DECLARE_PROPERTY(xObj);
//
//public:
//	TString128 TreeFile;
//	Vec3 Position;
//	float Scale;
//
//public:
//	xTree(const TString128 & name);
//	virtual ~xTree();
//
//	virtual TString128 GetTypeName() { return "Tree"; }
//
//	virtual void SetName(const TString128 & name);
//	virtual void SetTreeFile(const TString128 & meshFile);
//
//	virtual xObj * Clone();
//	virtual bool IsSceneNode(SceneNode * node);
//
//	virtual void SetPosition(const Vec3 & p);
//	virtual void SetOrientation(const Quat & q);
//	virtual void SetScale(const Vec3 & s);
//
//	virtual Vec3 GetPosition() { return Position; }
//	virtual Quat GetOrientation() { return Quat::Identity; }
//	virtual Vec3 GetScale() { return Scale; }
//
//	virtual void Serialize(xSerializer & serializer);
//
//	virtual Aabb GetBound();
//
//	virtual bool OnPropertyChanged(const Property * p);
//
//protected:
//	SceneNode * mNode;
//	MTreeInstance * mTree;
//};
//
//class xTreeFactory : public xObjFactory
//{
//public:
//	xTreeFactory() {};
//	virtual ~xTreeFactory() {};
//
//	virtual xTree * Create(const char * name) { return new xTree(name); }
//
//	virtual const char * GetGroupName()		{ return "Entity"; }
//	virtual const char * GetTypeName()		{ return "Tree"; }
//	virtual const char * GetExternName()	{ return "spt"; }
//};
//
//class xTreeFactoryListener
//{
//public:
//	xTreeFactoryListener()
//		: OnInit(&xEvent::OnInit, this, &xTreeFactoryListener::_Init)
//		, OnDragFile(&xEvent::OnDragFile, this, &xTreeFactoryListener::_OnDragFile)
//	{
//	}
//
//	virtual ~xTreeFactoryListener()
//	{
//	}
//
//	void _Init(void * param0, void * param1)
//	{
//		xObjManager::Instance()->AddFactory(new xTreeFactory());
//	}
//
//	void _OnDragFile(void * param0, void * param1);
//
//	tEventListener<xTreeFactoryListener> OnInit;
//	tEventListener<xTreeFactoryListener> OnDragFile;
//};
