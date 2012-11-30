#pragma once

#include "xEditor.h"

#define xPropertyChanged(prop) OnPropertyChanged(GetProperty(#prop))

class X_ENTRY xObj : public IPropertyObj
{
	DECLARE_PROPERTY(IPropertyObj);

protected:
	char Name[128];
	Vec3 Position;
	Quat Orientation;
	Vec3 Scale;

public:
	xObj(const char * name);
	virtual ~xObj();

	virtual const char * GetName();

	virtual void SetPosition(const Vec3 & p);
	virtual void SetOrientation(const Quat & q);
	virtual void SetScale(const Vec3 & s);

	virtual Vec3 GetPosition();
	virtual Quat GetOrientation();
	virtual Vec3 GetScale();

	virtual Aabb GetBound() { return Aabb::Identiy; }
};


class X_ENTRY xObjFactory
{
public:
	xObjFactory() {}
	virtual ~xObjFactory() {}

	virtual xObj * Create(const char * name) = 0;
	virtual const char * GetGroupName() { return "Unknown"; }
	virtual const char * GetTypeName() { return "Unknown"; }
};

class X_ENTRY xObjManager : public EventListener
{
	DECLARE_SINGLETON(xObjManager);

public:
	xObjManager();
	~xObjManager();

	virtual void OnCall(Event * sender, void * data);

	void AddFactory(xObjFactory * sf);
	xObjFactory * GetFactory(const char * type);
	int GetFactoryCount();
	xObjFactory * GetFactory(int index);

	xObj * Create(const char * type);
	void Distroy(xObj * shape);

	xObj * Get(const char * name);

protected:
	Array<xObjFactory *> mFactorys;
	Array<xObj *> mObjs;
};
    
