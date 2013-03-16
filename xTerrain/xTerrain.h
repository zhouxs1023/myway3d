#pragma once

#include "xObj.h"
#include "xApp.h"

class xTerrain : public xObj
{
public:
	xTerrain();
	virtual ~xTerrain();

	void _create(const Terrain::Config & config);
	void _load(const char * source);

	virtual void Serialize(xSerializer & Serializer);

	virtual TString128 GetTypeName() { return "Terrain"; }

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


class xTerrainFactoryListener
{
public:
	xTerrainFactoryListener()
		: OnInit(xEvent::OnInit, this, &xTerrainFactoryListener::_Init)
	{
	}

	virtual ~xTerrainFactoryListener()
	{
	}

	void _Init(Event * sender)
	{
		xObjManager::Instance()->AddFactory(new xTerrainFactory());
	}

	tEventListener<xTerrainFactoryListener> OnInit;
};
