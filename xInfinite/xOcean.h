#pragma once

#include "xObj.h"
#include "xApp.h"

class xOcean : public xObj
{
public:
	xOcean();
	virtual ~xOcean();

	virtual const char * GetName() { return "Ocean"; }
	virtual const char * GetTypeName() { return "Ocean"; }

	virtual bool OnPropertyChanged(const Property * p);

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
		xApp::OnInit += this;
	}

	virtual ~xOceanFactoryListener()
	{
		xApp::OnInit -= this;
	}

	virtual void OnCall(Event * sender, void * data)
	{
		if (sender == &xApp::OnInit)
		{
			xObjManager::Instance()->AddFactory(new xOceanFactory());
		}
	}

};