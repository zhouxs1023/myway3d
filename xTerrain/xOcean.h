#pragma once

#include "xObj.h"
#include "xApp.h"

class xOcean : public xObj
{
	DECLARE_PROPERTY(xObj);

protected:
	float Height;

public:
	xOcean();
	virtual ~xOcean();

	virtual const char * GetName() { return "Ocean"; }
	virtual const char * GetTypeName() { return "Ocean"; }

	virtual bool OnPropertyChanged(const Property * p);

protected:
	void _setHeight(float h);

protected:
	Ocean * mOcean;
};

class xOceanFactory : public xObjFactory
{
public:
	xOceanFactory() {};
	virtual ~xOceanFactory() {};

	virtual xObj * Create(const char * name);

	virtual const char * GetGroupName() { return "Water"; }
	virtual const char * GetTypeName() { return "Ocean"; }
};

class xOceanFactoryListener : public EventListener
{
public:
	xOceanFactoryListener()
	{
		xEvent::OnInit += this;
	}

	virtual ~xOceanFactoryListener()
	{
		xEvent::OnInit -= this;
	}

	virtual void OnCall(Event * sender, void * data)
	{
		if (sender == &xEvent::OnInit)
		{
			xObjManager::Instance()->AddFactory(new xOceanFactory());
		}
	}

};