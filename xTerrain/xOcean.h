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

	void SetHeight(float h);

	virtual TString128 GetTypeName() { return "Ocean"; }
	virtual void Serialize(xSerializer & Serializer);

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

class xOceanFactoryListener
{
public:
	xOceanFactoryListener()
		: OnInit(&xEvent::OnInit, this, &xOceanFactoryListener::_Init)
	{
	}

	virtual ~xOceanFactoryListener()
	{
	}

	void _Init(Event * sender)
	{
		xObjManager::Instance()->AddFactory(new xOceanFactory());
	}

	tEventListener<xOceanFactoryListener> OnInit;
};
