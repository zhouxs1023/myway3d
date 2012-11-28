#pragma once

#include "xObj.h"
#include "xApp.h"

class xTerrain : public xObj
{
public:
	xTerrain(const Terrain::Config & config);
	virtual ~xTerrain();

	virtual const char * GetName() { return "Terrain"; }
	virtual const char * GetTypeName() { return "Terrain"; }

	virtual bool OnPropertyChanged(const Property * p);

protected:
	Terrain * mTerrain;
};


class xTerrainFactory : public xObjFactory
{
public:
	xTerrainFactory() {};
	virtual ~xTerrainFactory() {};

	virtual xObj * Create(const char * name);

	virtual const char * GetGroupName() { return "Terrain"; }
	virtual const char * GetTypeName() { return "Terrain"; }
};

class xTerrainFactoryListener : public EventListener
{
public:
	xTerrainFactoryListener()
	{
		xApp::OnInit += this;
	}

	virtual ~xTerrainFactoryListener()
	{
		xApp::OnInit -= this;
	}

	virtual void OnCall(Event * sender, void * data)
	{
		if (sender == &xApp::OnInit)
		{
			xObjManager::Instance()->AddFactory(new xTerrainFactory());
		}
	}

};