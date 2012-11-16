#pragma once

#define xPropertyChanged(prop) OnPropertyChanged(GetProperty(#prop))

class xObj : public IPropertyObj
{
	DECLARE_PROPERTY(IPropertyObj);

protected:
	char Name[128];
	char MeshFile[128];
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

class xObjFactory
{
public:
	xObjFactory() {}
	virtual ~xObjFactory() {}

	virtual xObj * Create(const char * name) = 0;
	virtual const char * GetGroupName() { return "Unknown"; }
	virtual const char * GetTypeName() { return "Unknown"; }
};

class xObjManager : public EventListener
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
    
class xObjBound
{
public:
	xObjBound();
	virtual ~xObjBound();

	void Init(void * data);
	void Shutdown(void * data);
	void Render(void * data);

protected:
	Technique * mTech;
	RenderDesc * mRender;

	tEventListener<xObjBound> OnInit;
	tEventListener<xObjBound> OnShutdown;
	tEventListener<xObjBound> OnRender;
};

class xGizmo
{
public:
	xGizmo();
	~xGizmo();

	void Init(void * data);
	void Shutdown(void * data);
	void Render(void * data);
	void Update(void * data);

protected:
	void _InitGeo();

protected:
	Technique * mTech;
	RenderDesc * mRender;

	int mNumVertex_Move;
	int mNumIndex_Move;
	Vec3 * mVertex_Move;
	short * mIndex_Move;

	tEventListener<xGizmo> OnInit;
	tEventListener<xGizmo> OnShutdown;
	tEventListener<xGizmo> OnRender;
};