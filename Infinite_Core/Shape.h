//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#include "xSerializer.h"

namespace Infinite {

#define xPropertyChanged(prop) OnPropertyChanged(GetProperty(#prop))

class INFI_ENTRY Shape : public IPropertyObj
{
	DECLARE_PROPERTY(IPropertyObj);

public:
	static const Aabb K_DefaultBound;
	static const Sphere K_DefaultSphere;

protected:
	TString128 Name;

public:
	Shape(const TString128 & name);
	virtual ~Shape();

	virtual const TString128 & GetName();
	virtual TString128 GetTypeName() = 0;
	virtual Shape * Clone() { return NULL; }
	virtual bool IsSceneNode(SceneNode * node) { return false; } 

	virtual void SetPosition(const Vec3 & p) {}
	virtual void SetOrientation(const Quat & q) {}
	virtual void SetScale(float s) {}

	virtual Vec3 GetPosition() { return Vec3::Zero; }
	virtual Quat GetOrientation() { return Quat::Identity; }
	virtual float GetScale() { return 1; }

	virtual ColMesh * GetColMesh() { return NULL; }

	virtual void Serialize(xSerializer & Serializer);

	virtual Aabb GetBound() { return Aabb::Identiy; }
};


class INFI_ENTRY ShapeFactory
{
public:
	ShapeFactory() {}
	virtual ~ShapeFactory() {}

	virtual Shape * Create(const char * name) = 0;
	virtual const char * GetGroupName() = 0;
	virtual const char * GetTypeName() = 0;
	virtual const char * GetExternName() { return "unknown"; }
	virtual const char * GetIcon() { return "..\\ui\\default.ico"; }
};

class INFI_ENTRY ShapeManager
{
	DECLARE_SINGLETON(ShapeManager);

public:
	ShapeManager();
	~ShapeManager();

	void AddFactory(ShapeFactory * sf);
	ShapeFactory * GetFactory(const char * type);
	int GetFactoryCount();
	ShapeFactory * GetFactory(int index);

	Shape * Create(const char * type);
	void Distroy(Shape * shape);

	Shape * Get(const char * name);
	Shape * Get(SceneNode * node);

	Shape * _Create(const TString128 & name, const TString128 & type);

	void _Shutdown(Event * sender);

protected:
	void _Serialize(Event * sender);
	void _UnloadScene(Event * sender);

	void _Load(xSerializer & Serializer);
	void _Save(xSerializer & Serializer);

protected:
	Array<ShapeFactory *> mFactorys;
	Array<Shape *> mObjs;

	tEventListener<ShapeManager> OnUnloadScene;
	tEventListener<ShapeManager> OnSerialize;
};
    
}