#pragma once

class xObj : public IPropertyObj
{
	DECLARE_PROPERTY(IPropertyObj);

public:
	xObj(const char * name) {}
	virtual ~xObj() {}

	virtual const char * GetName() = 0;

	virtual bool SetPosition(float x, float y, float z) { return true; }
	virtual bool SetOrientation(float x, float y, float z) { return false; }
	virtual bool SetScale(float x, float y, float z) { return false; }
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
    
class xObjBound : public EventListener
{
	DECLARE_SINGLETON(xObjManager);

public:
	xObjBound();
	virtual ~xObjBound();

	void Init();
	void Shutdown();
	void Render();

	void OnCall(Event * sender, void * data);

protected:
	Technique * mTech;
	RenderDesc * mRender;

	tEventListener<xObjBound> OnInit;
	tEventListener<xObjBound> OnShutdown;
	tEventListener<xObjBound> OnRender;
};