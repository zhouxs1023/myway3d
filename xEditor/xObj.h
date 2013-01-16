#pragma once

#include "xEditor.h"
#include "xSerializer.h"

#define xPropertyChanged(prop) OnPropertyChanged(GetProperty(#prop))

class X_ENTRY xObj : public IPropertyObj
{
	DECLARE_PROPERTY(IPropertyObj);

public:
	static const Aabb K_DefaultBound;
	static const Sphere K_DefaultSphere;

protected:
	TString128 Name;

public:
	xObj(const TString128 & name);
	virtual ~xObj();

	virtual const TString128 & GetName();
	virtual TString128 GetTypeName() = 0;
	virtual xObj * Clone() { return NULL; }
	virtual bool IsSceneNode(SceneNode * node) { return false; } 

	virtual void SetPosition(const Vec3 & p) {}
	virtual void SetOrientation(const Quat & q) {}
	virtual void SetScale(float s) {}

	virtual Vec3 GetPosition() { return Vec3::Zero; }
	virtual Quat GetOrientation() { return Quat::Identity; }
	virtual float GetScale() { return 1; }

	virtual void Serialize(xSerializer & Serializer);

	virtual Aabb GetBound() { return Aabb::Identiy; }
};


class X_ENTRY xObjFactory
{
public:
	xObjFactory() {}
	virtual ~xObjFactory() {}

	virtual xObj * Create(const char * name) = 0;
	virtual const char * GetGroupName() = 0;
	virtual const char * GetTypeName() = 0;
	virtual const char * GetExternName() { return "unknown"; }
	virtual const char * GetIcon() { return "..\\ui\\default.ico"; }
};

class X_ENTRY xObjManager
{
	DECLARE_SINGLETON(xObjManager);

public:
	xObjManager();
	~xObjManager();

	void AddFactory(xObjFactory * sf);
	xObjFactory * GetFactory(const char * type);
	int GetFactoryCount();
	xObjFactory * GetFactory(int index);

	xObj * Create(const char * type);
	void Distroy(xObj * shape);

	xObj * Get(const char * name);
	xObj * Get(SceneNode * node);

protected:
	xObj * _Create(const TString128 & name, const TString128 & type);

	void _Shutdown(void * param0, void * param1);
	void _Serialize(void * param0, void * param1);
	void _UnloadScene(void * param0, void * param1);

	void _Load(xSerializer & Serializer);
	void _Save(xSerializer & Serializer);

protected:
	Array<xObjFactory *> mFactorys;
	Array<xObj *> mObjs;

	tEventListener<xObjManager> OnUnloadScene;
	tEventListener<xObjManager> OnShutdown;
	tEventListener<xObjManager> OnSerialize;
};
    
